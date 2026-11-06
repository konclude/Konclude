/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CJNIQueryProcessor.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CJNIQueryProcessor::CJNIQueryProcessor(CJNIInstanceManager* jniInstanceManager, CJNIOntologyRevisionData* ontRevData) {
					mOntRevData = ontRevData;
					mJNIInstanceManager = jniInstanceManager;
					mJNICommandProcessor = jniInstanceManager->getJNICommandProcessor();
					mExpressionOntRev = nullptr;
					mExpressionBuilder = nullptr;
					mProgressManagersLookedUp = false;
					mReasonerManager = nullptr;
					mClassificationManager = nullptr;
				}

				CJNIQueryProcessor::~CJNIQueryProcessor() {
					delete mExpressionBuilder;
					// The revision the expressions of the queries were built into is not installed, so
					// the revision manager does not know it and nothing else deletes it; its ontology
					// references the data of the installed one and is preprocessed on top of it, so it
					// has to go while the installed ontology still exists. The querying bridge is closed
					// before the library instance, which deletes the installed ontology (issue #45).
					delete mExpressionOntRev;
					mExpressionOntRev = nullptr;
				}


				bool CJNIQueryProcessor::queryReasoningProgress(QVector<double>& values) {
					values.fill(0.,PROGRESS_VALUE_COUNT);
					if (!mProgressManagersLookedUp) {
						// the commander registers both managers in its global configuration, which
						// the command processor asks it for; the lookup goes through the commander's
						// thread, which answers while a calculation runs on the reasoner's
						CConfiguration* config = mJNICommandProcessor->getConfiguration();
						mReasonerManager = CConfigManagerReader::readReasonerManagerConfig(config);
						mClassificationManager = CConfigManagerReader::readClassificationManagerConfig(config);
						mProgressManagersLookedUp = mReasonerManager != nullptr || mClassificationManager != nullptr;
					}
					if (mReasonerManager) {
						CCalculationManager* calculationManager = mReasonerManager->getCalculationManager();
						if (calculationManager) {
							CConcurrentTaskCalculationEnvironment* calcEnv = dynamic_cast<CConcurrentTaskCalculationEnvironment*>(calculationManager->getCalculationContext());
							if (calcEnv) {
								values[0] = (double)calcEnv->getCalculationStatisticTasksProcessedCount();
								values[1] = calcEnv->getCalculationApproximatedRemainingTasksCount();
							}
						}
						CReasonerManagerThread* managerThread = dynamic_cast<CReasonerManagerThread*>(mReasonerManager);
						if (managerThread) {
							CRealizationManager* realizationManager = managerThread->getRealizationManager();
							if (realizationManager) {
								CRealizationProgress* realizationProgress = realizationManager->getRealizationProgress();
								if (realizationProgress) {
									values[6] = realizationProgress->getProgessPercent();
									values[7] = (double)realizationProgress->getTestedClasses();
									values[8] = (double)realizationProgress->getTotalClasses();
									values[9] = realizationProgress->getRemainingMilliSeconds();
								}
							}
						}
					}
					if (mClassificationManager) {
						CClassificationProgress* classificationProgress = mClassificationManager->getClassificationProgress();
						if (classificationProgress) {
							values[2] = classificationProgress->getProgessPercent();
							values[3] = (double)(classificationProgress->getTestedSatisfiable()+classificationProgress->getTestedSubsumptions());
							values[4] = (double)(classificationProgress->getTotalSatisfiable()+classificationProgress->getTotalSubsumptions());
							values[5] = classificationProgress->getRemainingMilliSeconds();
						}
					}
					// the saturation counts live on the ontology, the saturation algorithm writes them
					Reasoner::Consistence::CPrecomputation* precomputation = mOntRevData->getOntologyRevision()->getOntology()->getPrecomputation();
					if (precomputation) {
						values[10] = (double)precomputation->getInitializedSaturationNodeCount();
						values[11] = (double)precomputation->getSaturationNodeCount();
					}
					return mProgressManagersLookedUp;
				}


				bool CJNIQueryProcessor::checkIsOntologyConsistent() {
					bool consistent = false;
					CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
					CIsConsistentQuery* consistencyQuery = new CIsConsistentQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig);
					mJNICommandProcessor->calculateOntologyQuery(consistencyQuery);
					CQueryResult* result = consistencyQuery->getQueryResult();
					if (result) {
						CBooleanQueryResult* boolQueryResult = dynamic_cast<CBooleanQueryResult*>(result);
						consistent = boolQueryResult->getResult();
					}
					delete consistencyQuery;
					return consistent;
				}






				// A query about a named class went through the command processor, the commander and the reasoner manager
				// thread, with two log messages, to look the class up in the taxonomy: about 0.15 ms a call, 20 to 100
				// times the other reasoners in Protege, 3.3 minutes for the equivalent and direct super classes of the
				// 558 000 classes of SNOMED CT AU. Once the classification is complete the taxonomy is only read, so it
				// is looked up here; the condition is the one the reasoner manager tests before it answers such a query.
				// A class that is not in the taxonomy, or an incomplete classification, takes the way through the
				// reasoner, which also computes what is missing.
				void CJNIQueryProcessor::calculateNamedClassQuery(CTaxonomyPremisingQuery* query, CConcept* concept) {
					CTaxonomy* taxonomy = getCompletedClassTaxonomy(query->getOntology());
					if (taxonomy && taxonomy->hasHierarchyNode(concept)) {
						query->constructResult(taxonomy);
					} else {
						mJNICommandProcessor->calculateOntologyQuery(query);
					}
				}


				CTaxonomy* CJNIQueryProcessor::getCompletedClassTaxonomy(CConcreteOntology* ontology) {
					if (!ontology || !ontology->getProcessingSteps()) {
						return nullptr;
					}
					COntologyProcessingStep* classifyStep = COntologyProcessingStepVector::getProcessingStepVectorInstance()->getProcessingStep(COntologyProcessingStep::OPSCLASSCLASSIFY);
					COntologyProcessingStepData* stepData = ontology->getProcessingSteps()->getOntologyProcessingStepDataVector()->getProcessingStepData(classifyStep->getProcessingStepID());
					if (!stepData) {
						return nullptr;
					}
					COntologyProcessingStatus* status = stepData->getProcessingStatus();
					if (!status->hasPartialProcessingFlags(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED) || !status->hasPartialErrorFlags(COntologyProcessingStatus::PSSUCESSFULL)) {
						return nullptr;
					}
					CClassification* classification = ontology->getClassification();
					CClassConceptClassification* classConClassification = classification ? classification->getClassConceptClassification() : nullptr;
					return classConClassification ? classConClassification->getClassConceptTaxonomy() : nullptr;
				}


				bool CJNIQueryProcessor::queryOntologyClassHierarchy(CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QVector<qint32>& edgeVector) {
					// a caller that reads the whole hierarchy, such as a taxonomy walker, would otherwise ask
					// for the direct sub and super classes of every class, each a call through the bridge
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CTaxonomy* taxonomy = getCompletedClassTaxonomy(ont);
					if (!taxonomy || !taxonomy->getTopHierarchyNode()) {
						return false;
					}
					QHash<CHierarchyNode*,qint32> nodePositionHash;
					QList<CHierarchyNode*> nodeList;
					nodePositionHash.insert(taxonomy->getTopHierarchyNode(), 0);
					nodeList.append(taxonomy->getTopHierarchyNode());
					for (qint32 position = 0; position < nodeList.size(); ++position) {
						CHierarchyNode* node = nodeList.at(position);
						visitingCallback->startEntityExpressionSet(ont);
						QList<CConcept*>* eqConList = node->getEquivalentConceptList();
						for (QList<CConcept*>::const_iterator it = eqConList->constBegin(), itEnd = eqConList->constEnd(); it != itEnd; ++it) {
							visitingCallback->visitConceptAssociatedEntityExpression(*it, ont);
						}
						visitingCallback->endEntityExpressionSet(ont);
						QSet<CHierarchyNode*>* childNodeSet = node->getChildNodeSet();
						for (QSet<CHierarchyNode*>::const_iterator it = childNodeSet->constBegin(), itEnd = childNodeSet->constEnd(); it != itEnd; ++it) {
							CHierarchyNode* childNode = *it;
							qint32 childPosition = nodePositionHash.value(childNode, -1);
							if (childPosition < 0) {
								childPosition = nodeList.size();
								nodePositionHash.insert(childNode, childPosition);
								nodeList.append(childNode);
							}
							edgeVector.append(position);
							edgeVector.append(childPosition);
						}
					}
					return true;
				}


				bool CJNIQueryProcessor::queryOntologySubClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperClassesResultVisitCallbackQuery* subSuperClassQuery = new CSubSuperClassesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,className,true,false,direct);
						calculateNamedClassQuery(subSuperClassQuery,concept);
						CQueryResult* result = subSuperClassQuery->getQueryResult();
						if (result) {
							subSuperClassQuery->callbackVisitingClasses(visitingCallback);
						}
						delete subSuperClassQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologySuperClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperClassesResultVisitCallbackQuery* subSuperClassQuery = new CSubSuperClassesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,className,false,true,direct);
						calculateNamedClassQuery(subSuperClassQuery,concept);
						CQueryResult* result = subSuperClassQuery->getQueryResult();
						if (result) {
							subSuperClassQuery->callbackVisitingClasses(visitingCallback);
						}
						delete subSuperClassQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologyEquivalentClasses(const QString& className, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CEquivalentClassesResultVisitCallbackQuery* equivClassQuery = new CEquivalentClassesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,className);
						calculateNamedClassQuery(equivClassQuery,concept);
						CQueryResult* result = equivClassQuery->getQueryResult();
						if (result) {
							equivClassQuery->callbackVisitingClasses(visitingCallback);
						}
						delete equivClassQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve concept for '%1'.").arg(className),0);
					}
					return false;
				}










				bool CJNIQueryProcessor::queryOntologySubObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperPropertiesResultVisitCallbackQuery* subSuperPropertiesQuery = new CSubSuperPropertiesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,role,propertyName,true,false,direct,false);
						mJNICommandProcessor->calculateOntologyQuery(subSuperPropertiesQuery);
						CQueryResult* result = subSuperPropertiesQuery->getQueryResult();
						if (result) {
							subSuperPropertiesQuery->callbackVisitingProperties(visitingCallback);
						}
						delete subSuperPropertiesQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologySuperObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperPropertiesResultVisitCallbackQuery* subSuperPropertiesQuery = new CSubSuperPropertiesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,role,propertyName,false,true,direct,false);
						mJNICommandProcessor->calculateOntologyQuery(subSuperPropertiesQuery);
						CQueryResult* result = subSuperPropertiesQuery->getQueryResult();
						if (result) {
							subSuperPropertiesQuery->callbackVisitingProperties(visitingCallback);
						}
						delete subSuperPropertiesQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologyEquivalentObjectProperties(const QString& propertyName, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CEquivalentPropertiesResultVisitCallbackQuery* equivPropQuery = new CEquivalentPropertiesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,role,false,propertyName);
						mJNICommandProcessor->calculateOntologyQuery(equivPropQuery);
						CQueryResult* result = equivPropQuery->getQueryResult();
						if (result) {
							equivPropQuery->callbackVisitingProperties(visitingCallback);
						}
						delete equivPropQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve role for '%1'.").arg(propertyName),0);
					}
					return false;
				}







				bool CJNIQueryProcessor::queryOntologyInstances(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CInstancesResultVisitCallbackQuery* instancesQuery = new CInstancesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,direct,className);
						mJNICommandProcessor->calculateOntologyQuery(instancesQuery);
						CQueryResult* result = instancesQuery->getQueryResult();
						if (result) {
							instancesQuery->callbackVisitingInstances(visitingCallback);
						}
						delete instancesQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve concept for '%1'.").arg(className),0);
					}
					return false;
				}




				bool CJNIQueryProcessor::queryOntologyTypes(const QString& indiName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					if (indi) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CTypesResultVisitCallbackQuery* typesQuery = new CTypesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,direct,indiName);
						mJNICommandProcessor->calculateOntologyQuery(typesQuery);
						CQueryResult* result = typesQuery->getQueryResult();
						if (result) {
							typesQuery->callbackVisitingTypes(visitingCallback);
						}
						delete typesQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve individual for '%1'.").arg(indiName),0);
					}
					return false;
				}



				bool CJNIQueryProcessor::queryOntologySameIndividuals(const QString& indiName, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					if (indi) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSameIndividualsResultVisitCallbackQuery* sameIndiQuery = new CSameIndividualsResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,indiName);
						mJNICommandProcessor->calculateOntologyQuery(sameIndiQuery);
						CQueryResult* result = sameIndiQuery->getQueryResult();
						if (result) {
							sameIndiQuery->callbackVisitingInstances(visitingCallback);
						}
						delete sameIndiQuery;
						return result != nullptr;
					}
					return false;
				}




				bool CJNIQueryProcessor::queryOntologyObjectPropertySources(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (indi && role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CObjectPropertySourcesTargetsResultVisitCallbackQuery* propertySouTarQuery = new CObjectPropertySourcesTargetsResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,role,true,false);
						mJNICommandProcessor->calculateOntologyQuery(propertySouTarQuery);
						CQueryResult* result = propertySouTarQuery->getQueryResult();
						if (result) {
							propertySouTarQuery->callbackVisitingIndividuals(visitingCallback);
						}
						delete propertySouTarQuery;
						return result != nullptr;
					}
					return false;
				}




				bool CJNIQueryProcessor::queryOntologyObjectPropertyTargets(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (indi && role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CObjectPropertySourcesTargetsResultVisitCallbackQuery* propertySouTarQuery = new CObjectPropertySourcesTargetsResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,role,false,true);
						mJNICommandProcessor->calculateOntologyQuery(propertySouTarQuery);
						CQueryResult* result = propertySouTarQuery->getQueryResult();
						if (result) {
							propertySouTarQuery->callbackVisitingIndividuals(visitingCallback);
						}
						delete propertySouTarQuery;
						return result != nullptr;
					}
					return false;
				}





				CJNIAxiomExpressionVisitingLoader* CJNIQueryProcessor::getClassExpressionBuilder() {
					if (!mExpressionBuilder) {
						COntologyRevision* installedOntRev = mOntRevData->getOntologyRevision();
						mExpressionOntRev = mJNICommandProcessor->getNewOntologyRevision(installedOntRev);
						if (!mExpressionOntRev) {
							LOG(ERROR,"::Konclude::JNIQueryProcessor",logTr("Could not create the ontology revision for the class expressions of the queries."),0);
							return nullptr;
						}
						// the installed revision data keeps the entity mapping, so an entity that is
						// first mentioned in a query is recorded where the results are resolved
						mExpressionBuilder = new CJNIQueryExpressionBuildingLoader(mJNIInstanceManager,mOntRevData,mExpressionOntRev->getOntology());
						mExpressionBuilder->initializeBuilding();
					}
					return mExpressionBuilder;
				}


				CClassTermExpression* CJNIQueryProcessor::getQueryClassTermExpression(CBuildExpression* classExpression, QString& errorMessage) {
					if (!classExpression) {
						errorMessage = QString("No class expression was handed over for the query.");
						return nullptr;
					}
					if (!mExpressionBuilder) {
						errorMessage = QString("The class expression builder of the querying bridge has not been initialised, so the class expression cannot belong to it.");
						return nullptr;
					}
					CClassTermExpression* classTermExp = dynamic_cast<CClassTermExpression*>(classExpression);
					if (!classTermExp) {
						errorMessage = QString("The expression handed over for the query is not a class expression.");
						return nullptr;
					}
					return classTermExp;
				}


				CQueryResult* CJNIQueryProcessor::calculateClassExpressionQuery(CComplexConceptAnsweringQuery* query, QString& errorMessage) {
					mJNICommandProcessor->calculateOntologyQuery(query);
					CQueryResult* result = query->getQueryResult();
					if (query->hasError() || !result) {
						QStringList errorStringList = query->getErrorStringList();
						if (errorStringList.isEmpty()) {
							errorMessage = QString("The query '%1' about a class expression was not answered.").arg(query->getQueryName());
						} else {
							errorMessage = QString("The query '%1' about a class expression failed: %2").arg(query->getQueryName()).arg(errorStringList.join("; "));
						}
						return nullptr;
					}
					return result;
				}


				void CJNIQueryProcessor::visitClassName(const QString& className, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CClassExpression* classExp = ont->getBuildData()->getClassEntityBuildHash()->value(className,nullptr);
					if (classExp) {
						visitingCallback->visitEntityExpression(classExp,ont);
					}
				}


				void CJNIQueryProcessor::visitIndividualName(const QString& individualName, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CNamedIndividualExpression* indiExp = ont->getBuildData()->getIndividualEntityBuildHash()->value(individualName,nullptr);
					if (indiExp) {
						visitingCallback->visitEntityExpression(indiExp,ont);
					}
				}



				bool CJNIQueryProcessor::checkIsClassExpressionSatisfiable(CBuildExpression* classExpression, bool& satisfiable, QString& errorMessage) {
					CClassTermExpression* classTermExp = getQueryClassTermExpression(classExpression,errorMessage);
					if (!classTermExp) {
						return false;
					}
					COntologyRevision* ontRev = mOntRevData->getOntologyRevision();
					CComplexSatisfiabilityAnsweringQuery* query = new CComplexSatisfiabilityAnsweringQuery(ontRev->getOntology(),mExpressionOntRev->getOntology(),classTermExp,ontRev->getOntologyConfiguration(),QString("JNI-Class-Expression-Satisfiability-Query"));
					CQueryResult* result = calculateClassExpressionQuery(query,errorMessage);
					CBooleanQueryResult* boolResult = dynamic_cast<CBooleanQueryResult*>(result);
					if (boolResult) {
						satisfiable = boolResult->getResult();
					} else if (result) {
						errorMessage = QString("The satisfiability query about a class expression was answered with an unexpected result.");
					}
					delete query;
					return boolResult != nullptr;
				}


				bool CJNIQueryProcessor::computeClassExpressionEquivalentClassNames(CClassTermExpression* classTermExp, QStringList& classNames, QString& errorMessage) {
					COntologyRevision* ontRev = mOntRevData->getOntologyRevision();
					CComplexEquivalentClassesAnsweringQuery* query = new CComplexEquivalentClassesAnsweringQuery(ontRev->getOntology(),mExpressionOntRev->getOntology(),classTermExp,ontRev->getOntologyConfiguration(),QString("JNI-Class-Expression-EquivalentClasses-Query"));
					CQueryResult* result = calculateClassExpressionQuery(query,errorMessage);
					CClassSetResult* classSetResult = dynamic_cast<CClassSetResult*>(result);
					if (classSetResult) {
						for (const QString& className : *classSetResult->getClassesSet()) {
							classNames.append(className);
						}
					} else if (result) {
						errorMessage = QString("The equivalent classes query about a class expression was answered with an unexpected result.");
					}
					delete query;
					return classSetResult != nullptr;
				}


				bool CJNIQueryProcessor::queryClassExpressionSubSuperClasses(CBuildExpression* classExpression, bool subClasses, bool superClasses, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage) {
					CClassTermExpression* classTermExp = getQueryClassTermExpression(classExpression,errorMessage);
					if (!classTermExp) {
						return false;
					}
					COntologyRevision* ontRev = mOntRevData->getOntologyRevision();
					CConcreteOntology* ont = ontRev->getOntology();

					// the complex query answers the strict sub or super classes, without a class that is equivalent to
					// the expression, and its direct ones are minimal (issues #12 and #50), as the OWL API asks for them
					CComplexConceptAnsweringQuery* query = nullptr;
					if (subClasses) {
						query = new CComplexSubClassesAnsweringQuery(ont,mExpressionOntRev->getOntology(),classTermExp,ontRev->getOntologyConfiguration(),QString("JNI-Class-Expression-SubClasses-Query"));
					} else {
						query = new CComplexSuperClassesAnsweringQuery(ont,mExpressionOntRev->getOntology(),classTermExp,ontRev->getOntologyConfiguration(),QString("JNI-Class-Expression-SuperClasses-Query"));
					}
					query->setDirect(direct);
					CQueryResult* result = calculateClassExpressionQuery(query,errorMessage);
					CClassSynsetsResult* synsetsResult = dynamic_cast<CClassSynsetsResult*>(result);
					if (synsetsResult) {
						QList<CHierarchyNode*> nodeList;
						QSet<CHierarchyNode*> nodeSet;
						CTaxonomy* taxonomy = ont->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy();
						for (CClassSynsetResult* synset : *synsetsResult->getClassSynsetList()) {
							CHierarchyNode* node = nullptr;
							for (const QString& className : synset->getEquivalentClassNameList()) {
								CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
								if (concept) {
									node = taxonomy->getHierarchyNode(concept);
									if (node) {
										break;
									}
								}
							}
							if (node && !nodeSet.contains(node)) {
								nodeSet.insert(node);
								nodeList.append(node);
							}
						}
						for (CHierarchyNode* node : nodeList) {
							visitingCallback->startEntityExpressionSet(ont);
							for (CConcept* concept : *node->getEquivalentConceptList()) {
								visitingCallback->visitConceptAssociatedEntityExpression(concept,ont);
							}
							visitingCallback->endEntityExpressionSet(ont);
						}
					} else if (result) {
						errorMessage = QString("The sub/super classes query about a class expression was answered with an unexpected result.");
					}
					delete query;
					return synsetsResult != nullptr;
				}


				bool CJNIQueryProcessor::queryClassExpressionSubClasses(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage) {
					return queryClassExpressionSubSuperClasses(classExpression,true,false,direct,visitingCallback,errorMessage);
				}


				bool CJNIQueryProcessor::queryClassExpressionSuperClasses(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage) {
					return queryClassExpressionSubSuperClasses(classExpression,false,true,direct,visitingCallback,errorMessage);
				}


				bool CJNIQueryProcessor::queryClassExpressionEquivalentClasses(CBuildExpression* classExpression, CEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage) {
					CClassTermExpression* classTermExp = getQueryClassTermExpression(classExpression,errorMessage);
					if (!classTermExp) {
						return false;
					}
					QStringList equivalentClassNames;
					if (!computeClassExpressionEquivalentClassNames(classTermExp,equivalentClassNames,errorMessage)) {
						return false;
					}
					for (const QString& className : equivalentClassNames) {
						visitClassName(className,visitingCallback);
					}
					return true;
				}


				bool CJNIQueryProcessor::queryClassExpressionInstances(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage) {
					CClassTermExpression* classTermExp = getQueryClassTermExpression(classExpression,errorMessage);
					if (!classTermExp) {
						return false;
					}
					COntologyRevision* ontRev = mOntRevData->getOntologyRevision();
					CComplexIndividualInstancesAnsweringQuery* query = new CComplexIndividualInstancesAnsweringQuery(ontRev->getOntology(),mExpressionOntRev->getOntology(),classTermExp,ontRev->getOntologyConfiguration(),QString("JNI-Class-Expression-Instances-Query"));
					query->setDirect(direct);
					query->setFlattened(false);
					CQueryResult* result = calculateClassExpressionQuery(query,errorMessage);
					CIndividualSynsetsResult* synsetsResult = dynamic_cast<CIndividualSynsetsResult*>(result);
					if (synsetsResult) {
						CConcreteOntology* ont = ontRev->getOntology();
						for (CIndividualSynsetResult* synset : *synsetsResult->getIndividualSynsetList()) {
							visitingCallback->startEntityExpressionSet(ont);
							for (const QString& individualName : synset->getEquivalentIndividualNameList()) {
								visitIndividualName(individualName,visitingCallback);
							}
							visitingCallback->endEntityExpressionSet(ont);
						}
					} else if (result) {
						errorMessage = QString("The instances query about a class expression was answered with an unexpected result.");
					}
					delete query;
					return synsetsResult != nullptr;
				}






			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
