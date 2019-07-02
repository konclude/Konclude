/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "COptimizedComplexConceptAnsweringHandler.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			



			COptimizedComplexConceptAnsweringHandler::COptimizedComplexConceptAnsweringHandler(COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem) : CAnsweringHandler(), CLogDomain("::Konclude::Reasoner::Kernel::Answerer") {
				mOntoAnsweringItem = ontoAnsweringItem;
				mConfDebugTestingOntologyWriting = false;
				mConfUseNonAnswerVariablesAsAnonymousVariables = true;
				mConfDirectPossibleRealizationRequesting = false;
				mConfFailOnUnknownEntity = false;
				mConfAnonymousToDistinguishedVariablesConversion = true;
				mConfAnonymousToDistinguishedVariablesOnlyFullConversion = true;
				mConfUnsatisfiableNonInstanceStatisticsChecking = false;
				mConfOccurrenceStatisticsCollection = false;
				mConfRedundantlyRestrictedVariablesElimination = true;
				mConfAllowBindingReduction = true;
				mConfMappingSizeBindingReduction = 5000;
				mConfDebugWriteVariableCompositionItems = false;
				mConfSamplingBasedJoinMappingSize = 1000;
				mConfBindingReducingForNonDistinctAnswerQueries = true;
				mConfDetailedMissingEntityReporting = false;
				
				mConfDebugWriteVariableCompositionItems = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Debugging.WriteAnsweringVariableCompositionItems", false);

				mConfSamplingBasedJoinMappingSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.SamplingBasedJoinMappingSize", 1000);

				mConfFailOnUnknownEntity = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.FailOnUnknownEntity", false);
				mConfExtendedLogging = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ExtendedLogging", false);
				mConfVariablePreAbsorption = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.QueryAbsorption.Preabsorption", true);
				mConfRedundantTermReduction = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.RedundantTermElimination", true);

				mConfAllowBindingReduction = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AllowBindingReduction", true);
				mConfBindingReducingForNonDistinctAnswerQueries = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.BindingReducingForNonDistinctAnswerQueries", true);
				mConfMappingSizeBindingReduction = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.BindingReductionMappingSize", 5000);
				mConfPropagationJoining = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.PropagationJoining", true);

				mConfAnonymousToDistinguishedVariablesConversion = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AnonymousToDistinguishedVariableConversion", true);
				mConfAnonymousToDistinguishedVariablesOnlyFullConversion = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AnonymousToDistinguishedVariableOnlyFullConversion", true);
				mConfOccurrenceStatisticsCollection = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Calculation.Optimization.OccurrenceStatisticsCollecting", true);
				if (mConfOccurrenceStatisticsCollection) {
					mConfUnsatisfiableNonInstanceStatisticsChecking = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.UnsatisfiableNonInstanceStatisticsChecking", true);
				}
				mConfRedundantlyRestrictedVariablesElimination = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.RedundantlyRestrictedVariablesElimination", true);


				mConfConceptItemInstanceCandiateRetrievingSizeIncreaseFactor = 1.3;
				mConfConceptItemInstanceCandiateRetrievingMaxSize = 10000;

				mAnsweringStartTime.start();
				mTestingOntologyPreprocessor = new COntologyQueryExtendedConfigDependedPreProcesser();
				mTestingOntologyBuilder = new CConcreteOntologyRebuildingPreparationBuilder(ontoAnsweringItem->getTestingOntology(true));

				mDefaultRealizationSorting = CRealizationIndividualSorting(CRealizationIndividualSorting::SORT_TYPE_INDIVIDUAL_ID, CRealizationIndividualSorting::SORT_ORDER_ASCENDING, CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE);
				mRoleFillerRealizationSorting = CRealizationIndividualSorting(CRealizationIndividualSorting::SORT_TYPE_NOTHING, CRealizationIndividualSorting::SORT_ORDER_NOTHING, CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE);

				readConfig();
			}


			COptimizedComplexConceptAnsweringHandler::~COptimizedComplexConceptAnsweringHandler() {
				delete mOntoAnsweringItem;
				delete mTestingOntologyPreprocessor;
				delete mTestingOntologyBuilder;
			}



			bool COptimizedComplexConceptAnsweringHandler::addAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback) {
				mPendingQueryProcessingList.append(new CComplexQueryProcessingData(complexAnsweringQuery, callback));
				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::initializeProcessing(CAnswererContext* answererContext) {
				if (!mOntoAnsweringItem->isOntologyBuildingAndPreprocessingPrepared()) {
					prepareProcessing(answererContext);
				}
				while (!mPendingQueryProcessingList.isEmpty()) {
					CComplexQueryProcessingData* queryData = mPendingQueryProcessingList.takeFirst();
					initializeQueryProcessing(queryData,answererContext);
				}
				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::prepareProcessing(CAnswererContext* answererContext) {
				if (!mOntoAnsweringItem->isOntologyBuildingAndPreprocessingPrepared()) {
					LOG(INFO, getDomain(), logTr("Preparing query answering handler."), this);
					mTestingOntologyBuilder->prepareRebuildingUpdates();
					CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology(true);
					mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());
					LOG(INFO, getDomain(), logTr("Query answering hanlder prepared."), this);
					mOntoAnsweringItem->setOntologyBuildingAndPreprocessingPrepared(true);
				}
				return true;
			}


			bool nodeSortLessPredecessorsThan(const CHierarchyNode* item1, const CHierarchyNode* item2) {
				return item1->getPredecessorNodeCount() < item2->getPredecessorNodeCount();
			}

			bool nodeSortMorePredecessorsThan(const CHierarchyNode* item1, const CHierarchyNode* item2) {
				return item1->getPredecessorNodeCount() > item2->getPredecessorNodeCount();
			}





			bool COptimizedComplexConceptAnsweringHandler::initializeComplexConceptQueryProcessing(CComplexQueryProcessingData* queryProcessingData, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CClassTermExpression* classTermExpOfInt, bool satisfiableComputationRequired, bool superClassNodesComputationRequired, bool subClassNodesComputationRequired, bool equivalentClassNodesComputationRequired, bool subClassRealizationRequired, cint64 instanceComputationRequiredCount, COptimizedComplexConceptItem** assocConceptItem) {
				CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classTermExpOfInt);
				bool testingNegation = testingConcept->hasMappingNegation();
				return initializeComplexConceptQueryProcessing(queryProcessingData, buildingVarItem, testingConcept, testingNegation, satisfiableComputationRequired, superClassNodesComputationRequired, subClassNodesComputationRequired, equivalentClassNodesComputationRequired, subClassRealizationRequired, instanceComputationRequiredCount, assocConceptItem);
			}



			bool COptimizedComplexConceptAnsweringHandler::initializeComplexConceptQueryProcessing(CComplexQueryProcessingData* queryProcessingData, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CConcept* testingConcept, bool testingNegation, bool satisfiableComputationRequired, bool superClassNodesComputationRequired, bool subClassNodesComputationRequired, bool equivalentClassNodesComputationRequired, bool subClassRealizationRequired, cint64 instanceComputationRequiredCount, COptimizedComplexConceptItem** assocConceptItem) {
				bool created = false;
				COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(testingConcept, testingNegation, true, &created);
				if (assocConceptItem) {
					*assocConceptItem = conceptItem;
				}
				if (created) {

					mOntoAnsweringItem->getAnsweringHandlingStatistics()->incCreatedComplexConceptItemsCount();

					if (!mOntoAnsweringItem->getTestingOntology()->getABox()->hasUniversalConnectionIndividual()) {
						bool usesTopRolePropagation = checkHasNewTopRolePropagation(testingConcept, testingNegation);
						if (usesTopRolePropagation) {
							conceptItem->setTopObjectPropertyUsage(usesTopRolePropagation);
						}
					}
					extractAtomicSubsumerExpressions(conceptItem);
					searchTrivialSuperSubConceptItems(conceptItem);
				}
				if (queryProcessingData) {
					queryProcessingData->setConceptItem(conceptItem);
				}
				CComplexConceptItemComputationProcess* computationProcess = conceptItem->getComputationProcess();

				bool instancesDirectResolveable = true;
				if (instanceComputationRequiredCount != 0) {
					instancesDirectResolveable = false;
					if (checkConceptInstancesCompletelyResolveable(conceptItem, nullptr)) {
						instancesDirectResolveable = true;
					}
				}


				bool requiresProcessing = false;
				subClassNodesComputationRequired |= equivalentClassNodesComputationRequired || subClassRealizationRequired;
				superClassNodesComputationRequired |= subClassNodesComputationRequired || !instancesDirectResolveable;
				satisfiableComputationRequired |= superClassNodesComputationRequired;
				if (satisfiableComputationRequired) {
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getSatisfiableClassNodesComputationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				}
				if (superClassNodesComputationRequired) {
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getSuperClassNodesComputationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				}
				if (subClassNodesComputationRequired) {
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getSubClassNodesComputationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				}
				if (equivalentClassNodesComputationRequired) {
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getEquivalentClassNodesComputationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				}
				if (subClassRealizationRequired) {
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getSubClassRealizationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				}
				if (instanceComputationRequiredCount == -1) {
					CComplexConceptStepInstanceComputationProcess* instanceCompStep = computationProcess->getInstancesComputationProcess(true);
					instanceCompStep->setAllInstanceComputationRequired(true);
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getInstancesComputationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				} else if (instanceComputationRequiredCount > 0) {
					CComplexConceptStepInstanceComputationProcess* instanceCompStep = computationProcess->getInstancesComputationProcess(true);
					if (instanceComputationRequiredCount > instanceCompStep->getComputedInstancesCount()) {
						CXLinker<CComplexQueryProcessingData*>* queryLinker = nullptr;
						if (queryProcessingData) {
							queryLinker = mOntoAnsweringItem->createQueryProcessingLinker();
							queryLinker->initLinker(queryProcessingData);
							queryProcessingData->setComputationStepWaiting(instanceCompStep);
						}
						CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* buildVarItemLinker = nullptr;
						if (buildingVarItem) {
							buildVarItemLinker = mOntoAnsweringItem->createBuildingVariableItemProcessingLinker();
							buildVarItemLinker->initLinker(buildingVarItem);
							buildingVarItem->setComputationStepWaiting(instanceCompStep);
						}
						instanceCompStep->addComputedInstancesCountRequirement(instanceComputationRequiredCount, queryLinker, buildVarItemLinker);
						queueConceptItemProcessing(conceptItem, instanceCompStep);
						requiresProcessing = true;
					}
				}
				return requiresProcessing;

			}




			bool COptimizedComplexConceptAnsweringHandler::initializeQueryProcessingStep(CComplexConceptStepComputationProcess* compStep, COptimizedComplexConceptItem* conceptItem, CComplexQueryProcessingData* queryProcessingData, COptimizedComplexBuildingVariableCompositionsItem* buildVarItem) {
				if (compStep->isComputationProcessFinished()) {
					return false;
				} else {
					compStep->setComputationProcessRequired(true);
					if (queryProcessingData) {
						CXLinker<CComplexQueryProcessingData*>* queryLinker = mOntoAnsweringItem->createQueryProcessingLinker();
						queryLinker->initLinker(queryProcessingData);
						compStep->addQueryProcessingLinker(queryLinker);
						queryProcessingData->setComputationStepWaiting(compStep);
					}
					if (buildVarItem) {
						CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* buildVarItemLinker = mOntoAnsweringItem->createBuildingVariableItemProcessingLinker();
						buildVarItemLinker->initLinker(buildVarItem);
						compStep->addBuildingVariableItemProcessingLinker(buildVarItemLinker);
						buildVarItem->setComputationStepWaiting(compStep);
					}
					queueConceptItemProcessing(conceptItem, compStep);
					return true;
				}
			}



			bool COptimizedComplexConceptAnsweringHandler::checkHasNewTopRolePropagation(CConcept* concept, bool conceptNegation) {
				CRole* topRole = mOntoAnsweringItem->getTestingOntology()->getRBox()->getTopObjectRole();
				QList<CConcept*> testingList;
				QSet<CConcept*> testedSet;
				testingList.append(concept);
				testedSet.insert(concept);
				while (!testingList.isEmpty()) {
					CConcept* testingConcept = testingList.takeFirst();
					cint64 opCode = testingConcept->getOperatorCode();

					CConceptOperator* conceptOperator = testingConcept->getConceptOperator();
					if (conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_TYPE | CConceptOperator::CCFS_SOME_TYPE)) {
						CRole* role = testingConcept->getRole();
						if (role == topRole) {
							return true;
						}
					}

					if (opCode != CCEQ && opCode != CCSUB) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = testingConcept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							if (!testedSet.contains(opConcept)) {
								testedSet.insert(opConcept);
								testingList.append(opConcept);
							}
						}
					}

				}
				return false;
			}


			COptimizedComplexConceptAnsweringHandler* COptimizedComplexConceptAnsweringHandler::readConfig() {
				CCalculationConfigurationExtension* config = mOntoAnsweringItem->getCalculationConfiguration();
				mConfDebugTestingOntologyWriting = config->isDebuggingWriteDataActivated();
				return this;
			}



			bool COptimizedComplexConceptAnsweringHandler::checkUsesQueryOnlyKnownEntities(CConcreteOntology* expressionOntology) {
				COntologyBuildData* expressionOntologyBuildData = expressionOntology->getBuildData();
				COntologyBuildData* baseOntologyBuildData = mOntoAnsweringItem->getOntology()->getBuildData();
				if (baseOntologyBuildData->getClassEntityBuildHash()->size() != expressionOntologyBuildData->getClassEntityBuildHash()->size()) {
					return false;
				}
				if (baseOntologyBuildData->getDataPropertyEntityBuildHash()->size() != expressionOntologyBuildData->getDataPropertyEntityBuildHash()->size()) {
					return false;
				}
				if (baseOntologyBuildData->getObjectPropertyEntityBuildHash()->size() != expressionOntologyBuildData->getObjectPropertyEntityBuildHash()->size()) {
					if (mConfDetailedMissingEntityReporting) {
						for (CBUILDHASH<CStringRefStringHasher, CObjectPropertyExpression*>::const_iterator it = expressionOntologyBuildData->getObjectPropertyEntityBuildHash()->constBegin(), itEnd = expressionOntologyBuildData->getObjectPropertyEntityBuildHash()->constEnd(); it != itEnd; ++it) {
							CObjectPropertyExpression* objPropEx = it.value();
							if (!baseOntologyBuildData->getObjectPropertyEntityBuildHash()->contains(objPropEx->getName())) {
								LOG(WARN, getDomain(), logTr("Object property %1 not used in queried ontology.").arg(objPropEx->getName()), this);
								return false;
							}
						}
					}
					return false;
				}
				if (baseOntologyBuildData->getIndividualEntityBuildHash()->size() != expressionOntologyBuildData->getIndividualEntityBuildHash()->size()) {
					COntologyTriplesAssertionsAccessor* indexedTriplesAssertionAccessor = mOntoAnsweringItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
					if (indexedTriplesAssertionAccessor) {
						CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>* baseIndiEntHash = baseOntologyBuildData->getIndividualEntityBuildHash();
						CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>* expIndiEntHash = expressionOntologyBuildData->getIndividualEntityBuildHash();

						for (CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>::const_iterator it = expIndiEntHash->constBegin(), itEnd = expIndiEntHash->constEnd(); it != itEnd; ++it) {
							if (!baseIndiEntHash->contains(it.key())) {
								// check whether the individual is indexed in the triple stored data
								CNamedIndividualExpression* namedIndiEntExp = it.value();

								if (!indexedTriplesAssertionAccessor->hasIndividualId(namedIndiEntExp->getName())) {
									return false;
								}

							}
						}						
					} else {
						CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>* baseIndiEntHash = baseOntologyBuildData->getIndividualEntityBuildHash();
						CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>* expIndiEntHash = expressionOntologyBuildData->getIndividualEntityBuildHash();
						COntologyStringMapping* stringMapping = mOntoAnsweringItem->getOntology()->getStringMapping();

						for (CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>::const_iterator it = expIndiEntHash->constBegin(), itEnd = expIndiEntHash->constEnd(); it != itEnd; ++it) {
							if (!baseIndiEntHash->contains(it.key())) {
								// check whether the individual is indexed in the triple stored data
								CNamedIndividualExpression* namedIndiEntExp = it.value();

								if (!stringMapping->getIndividualFromName(namedIndiEntExp->getName())) {
									return false;
								}

							}
						}
					}


				}
				if (baseOntologyBuildData->getDatatypeIRIBuildHash()->size() != expressionOntologyBuildData->getDatatypeIRIBuildHash()->size()) {
					return false;
				}
				if (baseOntologyBuildData->getAnonymousIndividualBuildHash()->size() != expressionOntologyBuildData->getAnonymousIndividualBuildHash()->size()) {
					return false;
				}
				return true;
			}




			static bool itemSortMoreHierachyDepthThan(const TDepthTermpPair& item1, const TDepthTermpPair& item2) {
				return item1.first > item2.first;
			}





			bool COptimizedComplexConceptAnsweringHandler::tryEmbedVariableRestrictions(CExpressionVariable* varTesting, CExpressionVariable* varEmbedding, QHash<CExpressionVariable*, CExpressionVariable*>& varExpEmbeddingHash, 
					const QHash<CExpressionVariable*, CAxiomExpression*>& varExpAxiomExpHash, const QSet<CExpressionVariable*>& anonymousIndiVariableSet, const QSet<CExpressionVariable*>& answerIndiVariableSet, CAnswererContext* answererContext) {

				if (varExpEmbeddingHash.value(varTesting) == varEmbedding) {
					return true;
				} else if (varExpEmbeddingHash.contains(varTesting)) {
					return false;
				}
				if (varExpEmbeddingHash.contains(varEmbedding)) {
					return false;
				}

				if (answerIndiVariableSet.contains(varTesting)) {
					return false;
				}

				if (!anonymousIndiVariableSet.contains(varTesting)) {
					if (anonymousIndiVariableSet.contains(varEmbedding)) {
						return false;
					}
				}

				QSet<QPair<CObjectPropertyTermExpression*, bool>> testingObjPropExpSet;
				QSet<CClassTermExpression*> testingClassExpSet;
				QList<CObjectPropertyAssertionExpression*> remainingEmbedObjPropList;
				QList<CObjectPropertyAssertionExpression*> availEmbedObjPropList;

				CTaxonomy* classHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy();
				CRolePropertiesHierarchy* rolePropertyHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getObjectPropertyRoleClassification()->getRolePropertiesHierarchy();


				for (CAxiomExpression* axExp : varExpAxiomExpHash.values(varTesting)) {
					CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(axExp);
					if (classAssExp) {
						CClassTermExpression* classExp = classAssExp->getClassTermExpression();
						testingClassExpSet.insert(classExp);
					} else {
						CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(axExp);
						if (propAssExp) {
							CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
							if (varTesting == dynamic_cast<CExpressionVariable*>(propAssExp->getFirstIndividualTermExpression())) {
								testingObjPropExpSet.insert(QPair<CObjectPropertyTermExpression*, bool>(propExp, false));
							} else {
								testingObjPropExpSet.insert(QPair<CObjectPropertyTermExpression*, bool>(propExp, true));
							}
							remainingEmbedObjPropList.append(propAssExp);
						} else {
							// TODO: extend to data values/assertions
							return false;
						}
					}
				}

				if (!testingObjPropExpSet.isEmpty() || !testingClassExpSet.isEmpty()) {
					QSet<QPair<CObjectPropertyTermExpression*, bool>> embeddingObjPropExpSet;
					QSet<CClassTermExpression*> embeddingClassExpSet;

					for (CAxiomExpression* axExp : varExpAxiomExpHash.values(varEmbedding)) {
						CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(axExp);
						if (classAssExp) {
							if (!testingClassExpSet.isEmpty()) {
								CClassTermExpression* classExp = classAssExp->getClassTermExpression();
								embeddingClassExpSet.insert(classExp);
							}
						} else {
							CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(axExp);
							if (propAssExp) {
								if (!testingObjPropExpSet.isEmpty()) {
									CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
									if (varEmbedding == dynamic_cast<CExpressionVariable*>(propAssExp->getFirstIndividualTermExpression())) {
										embeddingObjPropExpSet.insert(QPair<CObjectPropertyTermExpression*, bool>(propExp, false));
									} else {
										embeddingObjPropExpSet.insert(QPair<CObjectPropertyTermExpression*, bool>(propExp, true));
									}
									availEmbedObjPropList.append(propAssExp);
								}
							} else {
								// TODO: extend to data values/assertions
							}
						}
					}


					for (QSet<CClassTermExpression*>::iterator it = testingClassExpSet.begin(), itEnd = testingClassExpSet.end(); it != itEnd; ) {
						CClassTermExpression* testingClassExp = *it;
						if (embeddingClassExpSet.contains(testingClassExp)) {
							it = testingClassExpSet.erase(it);
						} else {
							++it;
						}
					}

					for (QSet<QPair<CObjectPropertyTermExpression*, bool>>::iterator it = testingObjPropExpSet.begin(), itEnd = testingObjPropExpSet.end(); it != itEnd; ) {
						QPair<CObjectPropertyTermExpression*, bool> propInvPair = *it;
						CObjectPropertyTermExpression* testingObjPropExp = propInvPair.first;
						bool inversed = propInvPair.second;
						if (embeddingObjPropExpSet.contains(propInvPair)) {
							it = testingObjPropExpSet.erase(it);
						} else {
							++it;
						}
					}

					if (!testingClassExpSet.isEmpty()) {

						QSet<CHierarchyNode*> embeddingHierNodeSet;
						for (QSet<CClassTermExpression*>::iterator it = embeddingClassExpSet.begin(), itEnd = embeddingClassExpSet.end(); it != itEnd; ++it) {
							CClassTermExpression* classExp = *it;
							if (classHierarchy && dynamic_cast<CClassExpression*>(classExp)) {
								CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classExp);
								if (concept) {
									CHierarchyNode* hierNode = classHierarchy->getHierarchyNode(concept, false);
									if (hierNode) {
										embeddingHierNodeSet.insert(hierNode);
										for (CHierarchyNode* predHierNode : *hierNode->getPredecessorNodeSet()) {
											embeddingHierNodeSet.insert(predHierNode);
										}
									}
								}
							}
						}


						for (QSet<CClassTermExpression*>::iterator it = testingClassExpSet.begin(), itEnd = testingClassExpSet.end(); classHierarchy && it != itEnd; ++it) {
							CClassTermExpression* classExp = *it;
							if (!dynamic_cast<CClassExpression*>(classExp)) {
								return false;
							} else {
								CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classExp);
								if (!concept) {
									return false;
								} else {
									CHierarchyNode* hierNode = classHierarchy->getHierarchyNode(concept, false);
									if (!hierNode) {
										return false;
									} else if (!embeddingHierNodeSet.contains(hierNode)) {
										return false;
									}
								}
							}
						}
					}





					if (!remainingEmbedObjPropList.isEmpty()) {


						QHash<CRolePropertiesHierarchyNode*, CObjectPropertyAssertionExpression*> embeddingHierNodeObjectPropAssHash;
						QHash<CRolePropertiesHierarchyNode*, CObjectPropertyAssertionExpression*> embeddingHierNodeInvObjectPropAssHash;
						for (CObjectPropertyAssertionExpression* propAssExp : availEmbedObjPropList) {
							CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
							bool inversed = true;
							if (varEmbedding == dynamic_cast<CExpressionVariable*>(propAssExp->getFirstIndividualTermExpression())) {
								inversed = false;
							}

							if (rolePropertyHierarchy) {

								CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
								if (inversed && role) {
									role = role->getInverseRole();
								}
								if (role) {
									CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);
									if (hierNode) {
										embeddingHierNodeObjectPropAssHash.insertMulti(hierNode, propAssExp);
										for (CRolePropertiesHierarchyNode* predHierNode : *hierNode->getPredecessorNodeSet()) {
											embeddingHierNodeObjectPropAssHash.insertMulti(predHierNode, propAssExp);
										}
									}
									CRolePropertiesHierarchyNode* invHierNode = rolePropertyHierarchy->getHierarchyNode(role->getInverseRole());
									if (invHierNode) {
										embeddingHierNodeInvObjectPropAssHash.insertMulti(invHierNode, propAssExp);
										for (CRolePropertiesHierarchyNode* predHierNode : *invHierNode->getPredecessorNodeSet()) {
											embeddingHierNodeInvObjectPropAssHash.insertMulti(predHierNode, propAssExp);
										}
									}
								}
							}
						}


						if (!testingObjPropExpSet.isEmpty()) {
							for (QSet<QPair<CObjectPropertyTermExpression*, bool>>::iterator it = testingObjPropExpSet.begin(), itEnd = testingObjPropExpSet.end(); it != itEnd; ++it) {
								QPair<CObjectPropertyTermExpression*, bool> propInvPair = *it;
								CObjectPropertyTermExpression* testingObjPropExp = propInvPair.first;
								bool inversed = propInvPair.second;
								if (!dynamic_cast<CObjectPropertyExpression*>(testingObjPropExp)) {
									return false;
								} else {
									CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(testingObjPropExp);
									if (inversed && role) {
										role = role->getInverseRole();
									}
									if (!role) {
										return false;
									} else {
										CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);
										if (hierNode) {
											if (!embeddingHierNodeObjectPropAssHash.contains(hierNode)) {
												return false;
											}
										} else {
											CRolePropertiesHierarchyNode* invHierNode = rolePropertyHierarchy->getHierarchyNode(role->getInverseRole());
											if (invHierNode) {
												if (!embeddingHierNodeInvObjectPropAssHash.contains(invHierNode)) {
													return false;
												}
											}
										}
									}
								}
							}
						}

						// assume embedding 
						QHash<CExpressionVariable*, CExpressionVariable*> newVarExpEmbeddingHash(varExpEmbeddingHash);
						newVarExpEmbeddingHash.insert(varTesting, varEmbedding);
						if (!tryEmbedVariablesObjectPropertyAssertionList(varTesting, varEmbedding, remainingEmbedObjPropList, embeddingHierNodeObjectPropAssHash, embeddingHierNodeInvObjectPropAssHash, newVarExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
							return false;
						} 
						varExpEmbeddingHash = newVarExpEmbeddingHash;
					}
				}

				return true;
			}


			bool COptimizedComplexConceptAnsweringHandler::tryEmbedVariablesObjectPropertyAssertionList(CExpressionVariable* varTesting, CExpressionVariable* varEmbedding, const QList<CObjectPropertyAssertionExpression*>& remainingEmbedObjPropList, const QHash<CRolePropertiesHierarchyNode*, CObjectPropertyAssertionExpression*>& embeddingHierNodeObjectPropAssHash, const QHash<CRolePropertiesHierarchyNode*, CObjectPropertyAssertionExpression*>& embeddingHierNodeInvObjectPropAssHash,
					QHash<CExpressionVariable*, CExpressionVariable*>& varExpEmbeddingHash, const QHash<CExpressionVariable*, CAxiomExpression*>& varExpAxiomExpHash, const QSet<CExpressionVariable*>& anonymousIndiVariableSet, const QSet<CExpressionVariable*>& answerIndiVariableSet, CAnswererContext* answererContext) {

				if (!remainingEmbedObjPropList.isEmpty()) {
					QList<CObjectPropertyAssertionExpression*> embedObjPropList(remainingEmbedObjPropList);
					CRolePropertiesHierarchy* rolePropertyHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getObjectPropertyRoleClassification()->getRolePropertiesHierarchy();

					CObjectPropertyAssertionExpression* propAssExp = embedObjPropList.takeFirst();
					CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
					CIndividualTermExpression* otherIndiExp = propAssExp->getFirstIndividualTermExpression();
					bool inversed = true;
					if (varTesting == dynamic_cast<CExpressionVariable*>(propAssExp->getFirstIndividualTermExpression())) {
						inversed = false;
						otherIndiExp = propAssExp->getSecondIndividualTermExpression();
					}
					CExpressionVariable* otherVarExp = dynamic_cast<CExpressionVariable*>(otherIndiExp);
					CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
					if (inversed && role) {
						role = role->getInverseRole();
					}
					CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);
					QList<CObjectPropertyAssertionExpression*> embeddingPropAssExpList;
					if (hierNode) {
						embeddingPropAssExpList = embeddingHierNodeObjectPropAssHash.values(hierNode);
					} else {
						CRolePropertiesHierarchyNode* invHierNode = rolePropertyHierarchy->getHierarchyNode(role->getInverseRole());
						if (invHierNode) {
							embeddingPropAssExpList = embeddingHierNodeInvObjectPropAssHash.values(invHierNode);
						}
					}



					for (CObjectPropertyAssertionExpression* embeddingPropAssExp : embeddingPropAssExpList) {
						CObjectPropertyTermExpression* embeddingObjPropExp = embeddingPropAssExp->getObjectPropertyTermExpression();
						bool embeddingInversed = true;
						CIndividualTermExpression* embeddingOtherIndiExp = embeddingPropAssExp->getFirstIndividualTermExpression();
						if (varEmbedding == dynamic_cast<CExpressionVariable*>(embeddingPropAssExp->getFirstIndividualTermExpression())) {
							embeddingInversed = false;
							embeddingOtherIndiExp = embeddingPropAssExp->getSecondIndividualTermExpression();
						}
						CExpressionVariable* embeddingOtherVarExp = dynamic_cast<CExpressionVariable*>(embeddingOtherIndiExp);

						if (embeddingOtherVarExp && otherVarExp) {
							if (embeddingOtherVarExp == otherVarExp) {
								if (tryEmbedVariablesObjectPropertyAssertionList(varTesting, varEmbedding, embedObjPropList, embeddingHierNodeObjectPropAssHash, embeddingHierNodeInvObjectPropAssHash, varExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
									return true;
								}
							} else {
								QHash<CExpressionVariable*, CExpressionVariable*> newVarExpEmbeddingHash(varExpEmbeddingHash);
								if (tryEmbedVariableRestrictions(otherVarExp, embeddingOtherVarExp, newVarExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
									if (tryEmbedVariablesObjectPropertyAssertionList(varTesting, varEmbedding, embedObjPropList, embeddingHierNodeObjectPropAssHash, embeddingHierNodeInvObjectPropAssHash, newVarExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
										varExpEmbeddingHash = newVarExpEmbeddingHash;
										return true;
									}
								}
							}
						} else if (otherIndiExp == embeddingOtherIndiExp) {
							if (tryEmbedVariablesObjectPropertyAssertionList(varTesting, varEmbedding, embedObjPropList, embeddingHierNodeObjectPropAssHash, embeddingHierNodeInvObjectPropAssHash, varExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
								return true;
							}
						}
						
					}

				} else {
					return true;
				}

				return false;

			}




			QList<CExpressionVariable*> COptimizedComplexConceptAnsweringHandler::getRedundantlyEmbeddedVariables(QHash<CExpressionVariable*, CAxiomExpression*>& varExpAxiomExpHash, const QSet<CExpressionVariable*>& anonymousIndiVariableSet, const QSet<CExpressionVariable*>& answerIndiVariableSet, CComplexQueryProcessingData* queryProcessingData, CAnswererContext* answererContext) {

				CRolePropertiesHierarchy* rolePropertyHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getObjectPropertyRoleClassification()->getRolePropertiesHierarchy();
				QHash<CExpressionVariable*, CExpressionVariable*> varExpEmbeddingHash;

				QSet<CExpressionVariable*> redundantVariableSet;
				CExpressionVariable* testingVarExp = nullptr;
				QList<TDepthTermpPair> propAssExpList;
				for (QHash<CExpressionVariable*, CAxiomExpression*>::iterator it = varExpAxiomExpHash.begin(), itEnd = varExpAxiomExpHash.end(); it != itEnd; ++it) {
					CExpressionVariable* varExp = it.key();
					if (varExp != testingVarExp) {
						if (testingVarExp && propAssExpList.size() >= 2) {

							qSort(propAssExpList.begin(), propAssExpList.end(), itemSortMoreHierachyDepthThan);
							QList<TDepthTermpPair>::const_iterator itProp1 = propAssExpList.constBegin(), itPropEnd = propAssExpList.constEnd();
							while (itProp1 != itPropEnd) {
								TDepthTermpPair item1 = *itProp1;
								++itProp1;
								CObjectPropertyAssertionExpression* propAssExp1 = (CObjectPropertyAssertionExpression*)item1.second;
								CObjectPropertyTermExpression* propExp1 = propAssExp1->getObjectPropertyTermExpression();
								CRole* role1 = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp1);
								CExpressionVariable* otherVarExp1 = dynamic_cast<CExpressionVariable*>(propAssExp1->getSecondIndividualTermExpression());
								bool inversed1 = false;
								if (testingVarExp == otherVarExp1) {
									inversed1 = true;
									otherVarExp1 = dynamic_cast<CExpressionVariable*>(propAssExp1->getFirstIndividualTermExpression());
									role1 = role1->getInverseRole();
								}

								if (!varExpEmbeddingHash.contains(otherVarExp1)) {
									CRolePropertiesHierarchyNode* hierNode1 = rolePropertyHierarchy->getHierarchyNode(role1);
									CRolePropertiesHierarchyNode* invHierNode1 = rolePropertyHierarchy->getHierarchyNode(role1->getInverseRole());


									QList<TDepthTermpPair>::const_iterator itProp2 = propAssExpList.constBegin();
									while (itProp2 != itPropEnd) {

										TDepthTermpPair item2 = *itProp2;
										++itProp2;
										CObjectPropertyAssertionExpression* propAssExp2 = (CObjectPropertyAssertionExpression*)item2.second;
										if (propAssExp2 != propAssExp1) {
											CObjectPropertyTermExpression* propExp2 = propAssExp2->getObjectPropertyTermExpression();
											CRole* role2 = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp2);
											CExpressionVariable* otherVarExp2 = dynamic_cast<CExpressionVariable*>(propAssExp2->getSecondIndividualTermExpression());
											bool inversed2 = false;
											if (testingVarExp == otherVarExp2) {
												inversed2 = true;
												otherVarExp2 = dynamic_cast<CExpressionVariable*>(propAssExp2->getFirstIndividualTermExpression());
												role2 = role2->getInverseRole();
											}
											CRolePropertiesHierarchyNode* hierNode2 = rolePropertyHierarchy->getHierarchyNode(role2);
											CRolePropertiesHierarchyNode* invHierNode2 = rolePropertyHierarchy->getHierarchyNode(role2->getInverseRole());


											if (!varExpEmbeddingHash.contains(otherVarExp2)) {
												if (otherVarExp1 != otherVarExp2 && (hierNode1 && hierNode2 && hierNode2->hasPredecessorNode(hierNode1) || invHierNode1 && invHierNode2 && invHierNode2->hasPredecessorNode(invHierNode1))) {
													if (tryEmbedVariableRestrictions(otherVarExp1, otherVarExp2, varExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
														break;
													}
												}
											}
										}


									}
								}

							}


						}
						propAssExpList.clear();
						testingVarExp = varExp;
					}
					CAxiomExpression* axExp = it.value();
					CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(axExp);
					if (propAssExp) {
						CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
						CExpressionVariable* otherVarExp1 = dynamic_cast<CExpressionVariable*>(propAssExp->getSecondIndividualTermExpression());
						CExpressionVariable* otherVarExp2 = dynamic_cast<CExpressionVariable*>(propAssExp->getFirstIndividualTermExpression());
						if (otherVarExp1 && otherVarExp2 && otherVarExp1 != otherVarExp2) {
							CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
							if (role && rolePropertyHierarchy) {
								CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);
								if (hierNode) {
									propAssExpList.append(TDepthTermpPair(hierNode->getPredecessorNodeSet()->count(), propAssExp));
								}
							}
						}
					}
				}

				return varExpEmbeddingHash.keys();
			}





			QList<CAxiomExpression*> COptimizedComplexConceptAnsweringHandler::eliminateRedundantQueryTerms(const QList<CAxiomExpression*>& assExps, CComplexQueryProcessingData* queryProcessingData, CAnswererContext* answererContext) {
				QList<CAxiomExpression*> redElimAssExpList;
				typedef QPair<CHierarchyNode*, CIndividualTermExpression*> TConceptIndiTermPair;
				typedef QPair<CRole*, QPair<CIndividualTermExpression*, CIndividualTermExpression* > > TRoleIndiTermsPair;
				QSet< TConceptIndiTermPair > redundantClassAssertionSet;
				QSet< TRoleIndiTermsPair > redundantRoleAssertionSet;



				QList<TDepthTermpPair> depthTermAxiomPairList;

				CTaxonomy* classHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy();
				CRolePropertiesHierarchy* rolePropertyHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getObjectPropertyRoleClassification()->getRolePropertiesHierarchy();

				for (CAxiomExpression* assExp : assExps) {
					CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
					if (classAssExp) {
						CClassTermExpression* classExp = classAssExp->getClassTermExpression();
						bool foundHierNode = false;;
						if (classHierarchy && dynamic_cast<CClassExpression*>(classExp)) {
							CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classExp);
							if (concept) {
								CHierarchyNode* hierNode = classHierarchy->getHierarchyNode(concept, false);
								if (hierNode) {
									depthTermAxiomPairList.append(TDepthTermpPair(hierNode->getPredecessorNodeCount(), classAssExp));
									foundHierNode = true;
								}
							}
						} 
						if (!foundHierNode) {
							redElimAssExpList.append(classAssExp);
						}

					} else {
						CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
						if (propAssExp) {
							CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
							CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
							bool foundHierNode = false;
							if (role && rolePropertyHierarchy) {
								CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);
								if (hierNode) {
									depthTermAxiomPairList.append(TDepthTermpPair(hierNode->getPredecessorNodeSet()->count(), propAssExp));
									foundHierNode = true;
								}
							}
							if (!foundHierNode) {
								redElimAssExpList.append(classAssExp);
							}

						} else {
							redElimAssExpList.append(assExp);
						}
					}
				}

				qSort(depthTermAxiomPairList.begin(), depthTermAxiomPairList.end(), itemSortMoreHierachyDepthThan);

				for (TDepthTermpPair depthTermPair : depthTermAxiomPairList) {
					CAxiomExpression* assExp = depthTermPair.second;
					CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
					if (classAssExp) {
						CClassTermExpression* classExp = classAssExp->getClassTermExpression();
						CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classExp);
						CHierarchyNode* hierNode = classHierarchy->getHierarchyNode(concept, false);
						if (!redundantClassAssertionSet.contains(TConceptIndiTermPair(hierNode, classAssExp->getIndividualTermExpression()))) {
							redundantClassAssertionSet.insert(TConceptIndiTermPair(hierNode, classAssExp->getIndividualTermExpression()));
							redElimAssExpList.append(assExp);
							for (CHierarchyNode* predHierNode : *hierNode->getPredecessorNodeSet()) {
								redundantClassAssertionSet.insert(TConceptIndiTermPair(predHierNode, classAssExp->getIndividualTermExpression()));
							}
						}
					} else {
						CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
						if (propAssExp) {
							CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
							CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
							CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);

							TRoleIndiTermsPair roleIndisPair(hierNode->getOneEquivalentElement(), QPair<CIndividualTermExpression*, CIndividualTermExpression* >(propAssExp->getFirstIndividualTermExpression(), propAssExp->getSecondIndividualTermExpression()));
							if (!redundantRoleAssertionSet.contains(roleIndisPair)) {
								redundantRoleAssertionSet.insert(roleIndisPair);
								CRole* invRole = hierNode->getOneEquivalentElement()->getInverseRole();
								if (invRole) {
									redundantRoleAssertionSet.insert(TRoleIndiTermsPair(invRole, QPair<CIndividualTermExpression*, CIndividualTermExpression* >(propAssExp->getSecondIndividualTermExpression(), propAssExp->getFirstIndividualTermExpression())));
								}

								redElimAssExpList.append(assExp);
								for (CRolePropertiesHierarchyNode* predHierNode : *hierNode->getPredecessorNodeSet()) {
									CRole* invPredRole = predHierNode->getOneEquivalentElement()->getInverseRole();
									redundantRoleAssertionSet.insert(TRoleIndiTermsPair(predHierNode->getOneEquivalentElement(), QPair<CIndividualTermExpression*, CIndividualTermExpression* >(propAssExp->getFirstIndividualTermExpression(), propAssExp->getSecondIndividualTermExpression())));
									if (invPredRole) {
										redundantRoleAssertionSet.insert(TRoleIndiTermsPair(invPredRole, QPair<CIndividualTermExpression*, CIndividualTermExpression* >(propAssExp->getSecondIndividualTermExpression(), propAssExp->getFirstIndividualTermExpression())));
									}
								}
							}
						}
					}
				}


				return redElimAssExpList;
			}




			COptimizedComplexConceptAnsweringHandler* COptimizedComplexConceptAnsweringHandler::initializeQueryProcessing(CComplexQueryProcessingData* queryProcessingData, CAnswererContext* answererContext) {
				CComplexAnsweringQuery* query = queryProcessingData->getQuery();
				CConcreteOntology* expOnto = query->getExrpessionOntology();
				CConcreteOntology* baseOnto = query->getBaseOntology();
				
				mAnswererContext = answererContext;

				mOntoAnsweringItem->incCurrentlyAnsweringQueryCount();

				bool queryProcessing = false;
				bool queryCompleted = false;
				if (!queryProcessingData->isProcessingStarted()) {
					queryProcessingData->setProcessingStarted();
				}

				CComplexAnsweringQuery* complAnsQuery = dynamic_cast<CComplexAnsweringQuery*>(query);
				if (!checkUsesQueryOnlyKnownEntities(complAnsQuery->getExrpessionOntology())) {
					if (mConfFailOnUnknownEntity) {
						failQueryProcessing(queryProcessingData, new CQueryUnknownEntityReferenceError(baseOnto));
						queryCompleted = true;
					} else {
						finishQueryProcessingAsUnsatisfaible(queryProcessingData);
						queryCompleted = true;
					}
				} else {
					CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology();
					if (testingOnto) {
						CComplexConceptAnsweringQuery* compConQuery = dynamic_cast<CComplexConceptAnsweringQuery*>(query);
						if (compConQuery) {
							CBuildExpression* expOfInt = compConQuery->getExpressionOfInterest();
							mTestingOntologyBuilder->initializeBuilding();
							CBuildExpression* testingExpOfInt = mTestingOntologyBuilder->rebuildExpression(expOfInt);
							mTestingOntologyBuilder->completeBuilding();
							mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());
							CClassTermExpression* classTermExpOfInt = dynamic_cast<CClassTermExpression*>(testingExpOfInt);
							if (classTermExpOfInt) {
								bool instanceComputationRequired = compConQuery->isInstancesComputationRequired();
								bool equivalentClassNodesComputationRequired = compConQuery->isEquivalentClassNodesComputationRequired();
								bool subClassNodesComputationRequired = compConQuery->isSubClassNodesComputationRequired();
								bool superClassNodesComputationRequired = compConQuery->isSuperClassNodesComputationRequired();
								bool satisfiableComputationRequired = compConQuery->isSatisfiableComputationRequired();
								bool direct = compConQuery->isDirect();

								queryProcessing |= initializeComplexConceptQueryProcessing(queryProcessingData, nullptr, classTermExpOfInt, satisfiableComputationRequired, superClassNodesComputationRequired, subClassNodesComputationRequired, equivalentClassNodesComputationRequired, direct, instanceComputationRequired? -1 : 0, nullptr);
							}
						}
						CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringQuery*>(query);
						if (compAssIndVarQuery) {
							QList<CAxiomExpression*> assExps = *compAssIndVarQuery->getAxiomExpressions();
							QList<CExpressionVariable*>* disVarList = compAssIndVarQuery->getDistinguishedVariableExpressions();
							QSet<CExpressionVariable*> varExpSet;
							QList<CClassAssertionExpression*> classAssExpsEntList;
							QList<CClassAssertionExpression*> compClassAssExpsEntList;
							QList<CObjectPropertyAssertionExpression*> roleAssExpsEntList;
							QList<CDataPropertyAssertionExpression*> dataRoleAssExpsEntList;
							QList<CAxiomExpression*> assExpsVarList;
							CExpressionVariable* lastVarExpression = nullptr;

							QSet<CExpressionVariable*> answerIndiVariableSet(disVarList->toSet());




							bool nonInstantiatedTerm = false;
							if (mConfUnsatisfiableNonInstanceStatisticsChecking) {
								// identify those anonymous variables for which the related concept and role terms only occur on individual nodes
								COccurrenceStatisticsCacheReader* occStatsCacheReader = answererContext->getAnsweringCalculationHandler()->getOccurrenceStatisticsCacheReader();
								cint64 ontologyId = mOntoAnsweringItem->getOntology()->getOntologyID();
								for (CAxiomExpression* assExp : assExps) {
									CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
									if (propAssExp) {
										CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
										if (dynamic_cast<CObjectPropertyExpression*>(propExp)) {
											CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
											CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
											// TODO: correctly resolve inverses
											CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propExp);
											if (role && !role->isComplexRole()) {
												CRole* inverseRole = role->getInverseRole();
												COccurrenceStatisticsRoleData roleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, role->getRoleTag());
												if (roleOccStatsData.getExistentialInstanceOccurrencesCount() <= 0 && roleOccStatsData.getIndividualInstanceOccurrencesCount() <= 0) {
													nonInstantiatedTerm = true;
													break;
												}
												if (roleOccStatsData.getIndividualInstanceOccurrencesCount() <= 0 && firstVarExp && answerIndiVariableSet.contains(firstVarExp)) {
													nonInstantiatedTerm = true;
													break;
												}
												if (secondVarExp && answerIndiVariableSet.contains(secondVarExp)) {
													COccurrenceStatisticsRoleData roleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, inverseRole->getRoleTag());
													if (roleOccStatsData.getIndividualInstanceOccurrencesCount() <= 0) {
														nonInstantiatedTerm = true;
														break;
													}
												}
											}
										}
									}

									CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
									if (classAssExp) {
										CClassTermExpression* classExp = classAssExp->getClassTermExpression();
										if (dynamic_cast<CClassExpression*>(classExp)) {
											CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)classExp);
											if (concept->getOperatorCode() == CCEQ) {
												concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getTBox()->getEquivalentConceptCandidateHash(false)->value(concept);
											}
											COccurrenceStatisticsConceptData conceptOccStatsData = occStatsCacheReader->getAccummulatedConceptDataOccurrenceStatistics(ontologyId, concept->getConceptTag());
											if (conceptOccStatsData.getExistentialInstanceOccurrencesCount() <= 0 && conceptOccStatsData.getIndividualInstanceOccurrencesCount() <= 0) {
												nonInstantiatedTerm = true;
												break;
											}
											CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
											if (conceptOccStatsData.getIndividualInstanceOccurrencesCount() <= 0 && varExp && answerIndiVariableSet.contains(varExp)) {
												nonInstantiatedTerm = true;
												break;
											}
										}
									}

								}
							}

							if (nonInstantiatedTerm) {
								finishQueryProcessingAsUnsatisfaible(queryProcessingData);
								queryCompleted = true;
							} else {





								if (mConfRedundantTermReduction) {
									cint64 prevCount = assExps.count();
									assExps = eliminateRedundantQueryTerms(assExps, queryProcessingData, answererContext);
									cint64 newCount = assExps.count();
									if (prevCount != newCount && mConfExtendedLogging) {
										LOG(INFO, getDomain(), logTr("Eliminated %1 redundant query terms.").arg(prevCount - newCount), this);
									}
								}



								for (CAxiomExpression* assExp : assExps) {
									CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
									if (classAssExp) {
										CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
										if (!varExp) {
											if (classAssExp->getClassTermExpression()->getType() == CBuildExpression::BETCLASS) {
												classAssExpsEntList.append(classAssExp);
											}
											else {
												compClassAssExpsEntList.append(classAssExp);
											}
										}
										else {
											lastVarExpression = varExp;
											assExpsVarList.append(classAssExp);
										}
									}
									CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
									if (propAssExp) {
										bool hasVariable = false;
										CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
										if (firstVarExp) {
											hasVariable = true;
											lastVarExpression = firstVarExp;
										}
										CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
										if (secondVarExp) {
											hasVariable = true;
											lastVarExpression = secondVarExp;
										}
										if (!hasVariable) {
											roleAssExpsEntList.append(propAssExp);
										}
										else {
											assExpsVarList.append(propAssExp);
										}
									}
									CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
									if (dataPropAssExp) {
										bool hasVariable = false;
										CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
										if (indiVarExp) {
											hasVariable = true;
											lastVarExpression = indiVarExp;
										}
										CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
										if (dataVarExp) {
											hasVariable = true;
											lastVarExpression = dataVarExp;
										}
										if (!hasVariable) {
											dataRoleAssExpsEntList.append(dataPropAssExp);
										}
										else {
											assExpsVarList.append(dataPropAssExp);
										}

									}
								}


								QList<CVariableBindingFilteringLiteralComparison*> dataLitCompFilterList;
								for (CVariableBindingFiltering* filteringLinker = compAssIndVarQuery->getResultFilteringLinker(); filteringLinker; filteringLinker = filteringLinker->getNext()) {
									CVariableBindingFilteringLiteralComparison* dataLiteralCompFilter = dynamic_cast<CVariableBindingFilteringLiteralComparison*>(filteringLinker);
									if (dataLiteralCompFilter && !dataLiteralCompFilter->getDataLiteral()) {
										dataLitCompFilterList.append(dataLiteralCompFilter);
									}
								}


								if (!classAssExpsEntList.isEmpty() || !compClassAssExpsEntList.isEmpty() || !roleAssExpsEntList.isEmpty() || !dataRoleAssExpsEntList.isEmpty() || !dataLitCompFilterList.isEmpty()) {
									QList<QPair<CClassTermExpression*, CIndividualTermExpression*>> indClassInstTestExpList;
									QList<QPair<CVariableBindingFilteringLiteralComparison*, CDataLiteralExpression*>> dataFilterDataLiteralExpList;
									if (!compClassAssExpsEntList.isEmpty() || !dataRoleAssExpsEntList.isEmpty() || !dataLitCompFilterList.isEmpty()) {
										mTestingOntologyBuilder->initializeBuilding();
										for (CClassAssertionExpression* assExp : compClassAssExpsEntList) {
											CClassTermExpression* classTermExp = assExp->getClassTermExpression();
											indClassInstTestExpList.append(QPair<CClassTermExpression*, CIndividualTermExpression*>((CClassTermExpression*)mTestingOntologyBuilder->rebuildExpression(classTermExp), assExp->getIndividualTermExpression()));
										}
										for (CDataPropertyAssertionExpression* dataRoleAssExp : dataRoleAssExpsEntList) {
											CDataPropertyTermExpression* dataPropTermExp = dataRoleAssExp->getDataPropertyTermExpression();
											CIndividualTermExpression* indiTermExp = dataRoleAssExp->getIndividualTermExpression();
											CDataLiteralExpression* dataLiteralExp = (CDataLiteralExpression*)dataRoleAssExp->getDataLiteralTermExpression();
											dataLiteralExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLiteralExp);
											indClassInstTestExpList.append(QPair<CClassTermExpression*, CIndividualTermExpression*>(mTestingOntologyBuilder->getDataHasValue(dataPropTermExp, dataLiteralExp), indiTermExp));
										}
										for (CVariableBindingFilteringLiteralComparison* dataLiteralCompFilter : dataLitCompFilterList) {
											CDataLiteralExpression* dataLitExp = dataLiteralCompFilter->getDataLiteralExpression();
											dataLitExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLitExp);
											dataFilterDataLiteralExpList.append(QPair<CVariableBindingFilteringLiteralComparison*, CDataLiteralExpression*>(dataLiteralCompFilter, dataLitExp));
										}

										mTestingOntologyBuilder->completeBuilding();
										mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());
									}
									if (!dataFilterDataLiteralExpList.isEmpty()) {
										CPreProcessContextBase preprocessingContext(mOntoAnsweringItem->getTestingOntology(), mOntoAnsweringItem->getTestingOntology()->getConfiguration());
										CDataLiteralNormalizerPreProcess dataLitNormalizer;
										for (QPair<CVariableBindingFilteringLiteralComparison*, CDataLiteralExpression*> dataFilterDataLiteralExpPair : dataFilterDataLiteralExpList) {
											CVariableBindingFilteringLiteralComparison* dataLiteralCompFilter = dataFilterDataLiteralExpPair.first;
											CDataLiteralExpression* dataLitExp = dataFilterDataLiteralExpPair.second;
											CConcept* dataConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getDataRangeTermConceptMappingHash()->value(dataLitExp);
											CDataLiteral* dataLiteral = dataConcept->getDataLiteral();
											if (dataLiteral) {
												CDataLiteralValue* dataLiteralValue = dataLiteral->getDataLiteralValue();
												if (!dataLiteralValue) {
													dataLitNormalizer.preprocessDataConcept(dataConcept, mOntoAnsweringItem->getTestingOntology(), &preprocessingContext);
												}
												dataLiteralCompFilter->setDataLiteral(dataLiteral);
											}
										}
									}
									if (!classAssExpsEntList.isEmpty()) {
										for (CClassAssertionExpression* assExp : classAssExpsEntList) {
											indClassInstTestExpList.append(QPair<CClassTermExpression*, CIndividualTermExpression*>(assExp->getClassTermExpression(), assExp->getIndividualTermExpression()));
										}
									}
									for (QPair<CClassTermExpression*, CIndividualTermExpression*> classIndiPair : indClassInstTestExpList) {
										CClassTermExpression* classTermExp = classIndiPair.first;
										CIndividualTermExpression* indiTermExp = classIndiPair.second;
										CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classTermExp);
										CIndividual* testingIndividual = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(indiTermExp);
										bool testingNegation = testingConcept->hasMappingNegation();
										queryProcessing |= checkEntailmentConceptAssertion(testingConcept, testingNegation, testingIndividual, queryProcessingData, answererContext);
									}
									for (CObjectPropertyAssertionExpression* roleAssExp : roleAssExpsEntList) {
										CObjectPropertyTermExpression* objPropTermExp = roleAssExp->getObjectPropertyTermExpression();
										CIndividualTermExpression* indiSourceTermExp = roleAssExp->getFirstIndividualTermExpression();
										CIndividualTermExpression* indiDestTermExp = roleAssExp->getSecondIndividualTermExpression();
										bool inversed = false;
										while (objPropTermExp->getType() == CBuildExpression::BETINVERSEOBJECTPROPERTYOF) {
											CInverseObjectPropertyOfExpression* invPropExp = (CInverseObjectPropertyOfExpression*)objPropTermExp;
											objPropTermExp = invPropExp->getInverseOfExpression();
											inversed = !inversed;
										}
										if (inversed) {
											CIndividualTermExpression* indiTermExp = indiSourceTermExp;
											indiSourceTermExp = indiDestTermExp;
											indiDestTermExp = indiTermExp;
										}
										CRole* testingRole = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(objPropTermExp);
										CIndividual* testingIndividualSource = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(indiSourceTermExp);
										CIndividual* testingIndividualDestination = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(indiDestTermExp);
										queryProcessing |= checkEntailmentRoleAssertion(testingRole, testingIndividualSource, testingIndividualDestination, queryProcessingData, answererContext);
									}

								}


								for (CAxiomExpression* assExp : assExpsVarList) {
									CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
									if (classAssExp) {
										CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
										if (varExp) {
											varExpSet.insert(varExp);
										}
									}
									CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
									if (propAssExp) {
										CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
										if (firstVarExp) {
											varExpSet.insert(firstVarExp);
										}
										CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
										if (secondVarExp) {
											varExpSet.insert(secondVarExp);
										}
									}
									CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
									if (dataPropAssExp) {
										CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
										if (indiVarExp) {
											varExpSet.insert(indiVarExp);
										}
										CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
										if (dataVarExp) {
											varExpSet.insert(dataVarExp);
										}
									}

								}



								QSet<CExpressionVariable*> initialAnonymousIndiVariableSet;
								for (CExpressionVariable* varExp : varExpSet) {
									CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);
									if (indiVarExp && indiVarExp->isAnonymousVariable()) {
										initialAnonymousIndiVariableSet.insert(varExp);
									}
									if (mConfUseNonAnswerVariablesAsAnonymousVariables || !answerIndiVariableSet.contains(varExp)) {
										if (varExp->getName().startsWith("_")) {
											initialAnonymousIndiVariableSet.insert(varExp);
										}
									}
								}
								bool distinct = compAssIndVarQuery->isDistinctRequired();
								QSet<CExpressionVariable*> anonymousIndiVariableSet(initialAnonymousIndiVariableSet);
								QSet<CExpressionVariable*> nonEliminatableIndiVariableSet(answerIndiVariableSet);
								if (!distinct) {
									for (CExpressionVariable* varExp : varExpSet) {
										if (!anonymousIndiVariableSet.contains(varExp) && !answerIndiVariableSet.contains(varExp)) {
											nonEliminatableIndiVariableSet.insert(varExp);
										}
									}
								}





								QList<CObjectPropertyAssertionExpression*> bothVarPropAssPropagationList;
								QList<CDataPropertyAssertionExpression*> bothVarDataPropAssPropagationList;
								QList<CDataPropertyAssertionExpression*> onlyDataVarDataPropAssList;
								QHash<CExpressionVariable*, CAxiomExpression*> varExpAxiomExpHash;
								bool unsupportedWarning = false;
								for (CAxiomExpression* assExp : assExpsVarList) {
									CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
									if (classAssExp) {
										CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
										if (varExp) {
											varExpAxiomExpHash.insertMulti(varExp, assExp);
										}
									}
									CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
									if (propAssExp) {
										CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
										if (firstVarExp) {
											varExpAxiomExpHash.insertMulti(firstVarExp, assExp);
										}
										CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
										if (secondVarExp && secondVarExp != firstVarExp) {
											varExpAxiomExpHash.insertMulti(secondVarExp, assExp);
										}
										if (firstVarExp && secondVarExp) {
											if (!anonymousIndiVariableSet.contains(firstVarExp) && !anonymousIndiVariableSet.contains(secondVarExp)) {
												bothVarPropAssPropagationList.append(propAssExp);
											}
										}
									}
									CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
									if (dataPropAssExp) {
										CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
										if (firstVarExp) {
											varExpAxiomExpHash.insertMulti(firstVarExp, assExp);
										}
										CDataLiteralVariableExpression* dataLitVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
										if (firstVarExp && dataLitVarExp) {
											if (!anonymousIndiVariableSet.contains(firstVarExp) && !anonymousIndiVariableSet.contains(dataLitVarExp)) {
												bothVarDataPropAssPropagationList.append(dataPropAssExp);
											}
										}
										if (!firstVarExp && dataLitVarExp) {
											onlyDataVarDataPropAssList.append(dataPropAssExp);
										}
									}
									if (!classAssExp && !propAssExp && !dataPropAssExp) {
										if (!unsupportedWarning) {
											LOG(WARN, getDomain(), logTr("Query uses unsupported axiom types, which will be ignored."), this);
										}
										unsupportedWarning = true;
									}
								}
								bool updateIndiVarAssPropLists = false;

							

								// TODO: check whether all role and concept terms of an instance query are indeed instantiated via the occurrence statistics cache


								QSet<CExpressionVariable*> backupAnonymousIndiVariableSet(anonymousIndiVariableSet);
								cint64 originalAnonymousVariableCount = anonymousIndiVariableSet.size();
								bool allAnonymousVariables = varExpSet.size() == anonymousIndiVariableSet.size();
								if (mConfAnonymousToDistinguishedVariablesConversion && mConfOccurrenceStatisticsCollection && (allAnonymousVariables || distinct)) {
									// identify those anonymous variables for which the related concept and role terms only occur on individual nodes
									COccurrenceStatisticsCacheReader* occStatsCacheReader = answererContext->getAnsweringCalculationHandler()->getOccurrenceStatisticsCacheReader();
									cint64 ontologyId = mOntoAnsweringItem->getOntology()->getOntologyID();
									for (QSet<CExpressionVariable*>::iterator itAnonIndiVar = anonymousIndiVariableSet.begin(); itAnonIndiVar != anonymousIndiVariableSet.end(); ) {

										CExpressionVariable* annonyomousIndiVar(*itAnonIndiVar);
										QList<CAxiomExpression*> expressions = varExpAxiomExpHash.values(annonyomousIndiVar);
										bool convertAnonymousVarToIndiVar = false;
										for (CAxiomExpression* assExp : expressions) {
											CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
											if (propAssExp) {
												CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
												if (dynamic_cast<CObjectPropertyExpression*>(propExp)) {
													CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
													CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
													// TODO: correctly resolve inverses
													CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propExp);
													if (!role->isComplexRole()) {
														bool inversed = true;
														CIndividualVariableExpression* otherVarExp = firstVarExp;
														if (firstVarExp == annonyomousIndiVar) {
															otherVarExp = secondVarExp;
															inversed = false;
														}
														if (inversed) {
															role = role->getInverseRole();
														}
														COccurrenceStatisticsRoleData roleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, role->getRoleTag());
														if (roleOccStatsData.getExistentialInstanceOccurrencesCount() <= 0) {
															convertAnonymousVarToIndiVar = true;
															break;
														}
													}
												}
											}

											CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
											if (classAssExp) {
												CClassTermExpression* classExp = classAssExp->getClassTermExpression();
												if (dynamic_cast<CClassExpression*>(classExp)) {
													CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)classExp);
													if (concept->getOperatorCode() == CCEQ) {
														concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getTBox()->getEquivalentConceptCandidateHash(false)->value(concept);
													}
													COccurrenceStatisticsConceptData conceptOccStatsData = occStatsCacheReader->getAccummulatedConceptDataOccurrenceStatistics(ontologyId, concept->getConceptTag());
													if (conceptOccStatsData.getExistentialInstanceOccurrencesCount() <= 0) {
														convertAnonymousVarToIndiVar = true;
														break;
													}
												}											
											}

										}

										if (convertAnonymousVarToIndiVar) {
											itAnonIndiVar = anonymousIndiVariableSet.erase(itAnonIndiVar);
										} else {
											++itAnonIndiVar;
										}

									}
								}


								QSet<CExpressionVariable*> convertedVariableSet;
								CRoleRealization* roleReal = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
								if (mConfAnonymousToDistinguishedVariablesConversion && roleReal && (allAnonymousVariables || distinct)) {
									for (QSet<CExpressionVariable*>::iterator itAnonIndiVar = anonymousIndiVariableSet.begin(); itAnonIndiVar != anonymousIndiVariableSet.end(); ) {
										bool convertToDistinguishedVariable = false;

										CExpressionVariable* annonyomousIndiVar(*itAnonIndiVar);
										QList<CAxiomExpression*> expressions = varExpAxiomExpHash.values(annonyomousIndiVar);
										for (CAxiomExpression* assExp : expressions) {
											CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
											if (propAssExp) {
												CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
												CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
												if (firstVarExp && secondVarExp) {
													bool inversed = true;
													CIndividualVariableExpression* otherVarExp = firstVarExp;
													if (firstVarExp == annonyomousIndiVar) {
														otherVarExp = secondVarExp;
														inversed = false;
													}
													if (!anonymousIndiVariableSet.contains(otherVarExp)) {
														// check whether role is linking to existential individuals
														CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
														if (dynamic_cast<CObjectPropertyExpression*>(propExp)) {
															CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propExp);
															if (!roleReal->hasExistentiallyLinkedRoleInstances(role, inversed)) {
																convertToDistinguishedVariable = true;
																break;
															}
														}
													}
												}
											}
										}

										if (convertToDistinguishedVariable) {
											convertedVariableSet.insert(annonyomousIndiVar);
											anonymousIndiVariableSet.erase(itAnonIndiVar);
											itAnonIndiVar = anonymousIndiVariableSet.begin();
										} else {
											++itAnonIndiVar;
										}
									}
								}

								cint64 reducedAnonymousVariableCount = anonymousIndiVariableSet.size();
								if (originalAnonymousVariableCount != reducedAnonymousVariableCount) {
									updateIndiVarAssPropLists = true;
									if (allAnonymousVariables && mConfAnonymousToDistinguishedVariablesOnlyFullConversion && reducedAnonymousVariableCount > 0) {
										anonymousIndiVariableSet = backupAnonymousIndiVariableSet;
										updateIndiVarAssPropLists = false;
										if (mConfExtendedLogging) {
											LOG(INFO, getDomain(), logTr("No converstion of anonymous variables to distinguished (non-answer) variables since it is only possible for %1.").arg(originalAnonymousVariableCount - reducedAnonymousVariableCount), this);
										}
									} else if (mConfExtendedLogging) {
										LOG(INFO, getDomain(), logTr("Interpreting %1 anonymous variables as distinguished (non-answer) variables.").arg(originalAnonymousVariableCount - reducedAnonymousVariableCount), this);
									}
								}


								QList<CExpressionVariable*> indiIgnoreVarSet;
								if (mConfRedundantlyRestrictedVariablesElimination) {
									QList<CExpressionVariable*> redEmbVarSet = getRedundantlyEmbeddedVariables(varExpAxiomExpHash, anonymousIndiVariableSet, nonEliminatableIndiVariableSet, queryProcessingData, answererContext);
									if (!redEmbVarSet.isEmpty()) {
										updateIndiVarAssPropLists = true;
										indiIgnoreVarSet = redEmbVarSet;

										if (mConfExtendedLogging) {
											QStringList varList;
											for (CExpressionVariable* var : redEmbVarSet) {
												varList.append(var->getName());
											}
											LOG(INFO, getDomain(), logTr("Eliminated %1 redundantly restricted variables: %2.").arg(varList.size()).arg(varList.join(", ")), this);
										}

										for (CExpressionVariable* var : redEmbVarSet) {
											varExpSet.remove(var);
										}

										for (QSet<CExpressionVariable*>::iterator it = anonymousIndiVariableSet.begin(), itEnd = anonymousIndiVariableSet.end(); it != itEnd; ) {
											CExpressionVariable* varExp = *it;
											if (redEmbVarSet.contains(varExp)) {
												it = anonymousIndiVariableSet.erase(it);
											} else {
												++it;
											}
										}
									}
								}


								if (updateIndiVarAssPropLists) {
									bothVarPropAssPropagationList.clear();
									bothVarDataPropAssPropagationList.clear();
									onlyDataVarDataPropAssList.clear();
									varExpAxiomExpHash.clear();
									for (CAxiomExpression* assExp : assExpsVarList) {
										CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
										if (classAssExp) {
											CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
											if (varExp && !indiIgnoreVarSet.contains(varExp)) {
												varExpAxiomExpHash.insertMulti(varExp, assExp);
											}
										}
										CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
										if (propAssExp) {
											CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
											CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
											if (firstVarExp && !indiIgnoreVarSet.contains(firstVarExp) && (!secondVarExp || !indiIgnoreVarSet.contains(secondVarExp))) {
												varExpAxiomExpHash.insertMulti(firstVarExp, assExp);
											}
											if (secondVarExp && secondVarExp != firstVarExp && !indiIgnoreVarSet.contains(secondVarExp) && (!firstVarExp || !indiIgnoreVarSet.contains(firstVarExp))) {
												varExpAxiomExpHash.insertMulti(secondVarExp, assExp);
											}
											if (firstVarExp && secondVarExp && !indiIgnoreVarSet.contains(firstVarExp) && !indiIgnoreVarSet.contains(secondVarExp)) {
												if (!anonymousIndiVariableSet.contains(firstVarExp) && !anonymousIndiVariableSet.contains(secondVarExp)) {
													bothVarPropAssPropagationList.append(propAssExp);
												}
											}
										}
										CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
										if (dataPropAssExp) {
											CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
											if (firstVarExp && !indiIgnoreVarSet.contains(firstVarExp)) {
												varExpAxiomExpHash.insertMulti(firstVarExp, assExp);
											}
											CDataLiteralVariableExpression* dataLitVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
											if (firstVarExp && !indiIgnoreVarSet.contains(firstVarExp) && dataLitVarExp) {
												if (!anonymousIndiVariableSet.contains(firstVarExp) && !anonymousIndiVariableSet.contains(dataLitVarExp)) {
													bothVarDataPropAssPropagationList.append(dataPropAssExp);
												}
											}
											if (!firstVarExp && dataLitVarExp) {
												onlyDataVarDataPropAssList.append(dataPropAssExp);
											}
										}
									}
								}





								if (varExpSet.size() == 1 && dynamic_cast<CIndividualVariableExpression*>(lastVarExpression) && !anonymousIndiVariableSet.contains(lastVarExpression)) {
									// reduce to instance test
									//TODO: make sure expression ontology contains inverses for all roles
									LOG(INFO, getDomain(), logTr("Reducing query to single (complex) instance retrieval task."), this);
									CBuildExpression* instTestingExp = nullptr;
									QList<CBuildExpression*> instTestingExpList;
									mTestingOntologyBuilder->initializeBuilding();
									for (CAxiomExpression* assExp : assExpsVarList) {
										CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
										if (classAssExp) {
											instTestingExpList.append(mTestingOntologyBuilder->rebuildExpression(classAssExp->getClassTermExpression()));
										}
										CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
										if (propAssExp) {
											CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
											CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
											CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
											CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
											CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
											if (firstVarExp && secondVarExp) {
												instTestingExpList.append(mTestingOntologyBuilder->getObjectHasSelf(propExp));
											} else if (firstVarExp) {
												instTestingExpList.append(mTestingOntologyBuilder->getObjectHasValue(propExp, secondIndiExp));
											} else {
												instTestingExpList.append(mTestingOntologyBuilder->getObjectHasValue(mTestingOntologyBuilder->getInverseObjectPropertyOf(propExp), firstIndiExp));
											}
										}
										CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
										if (dataPropAssExp) {
											CDataPropertyTermExpression* dataPropTermExp = dataPropAssExp->getDataPropertyTermExpression();
											CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
											CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
											if (indiVarExp && !dataVarExp) {
												CDataLiteralExpression* dataLiteralExp = (CDataLiteralExpression*)dataPropAssExp->getDataLiteralTermExpression();
												dataLiteralExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLiteralExp);
												instTestingExpList.append(mTestingOntologyBuilder->getDataHasValue(dataPropTermExp, dataLiteralExp));
											}
										}

										//TODO: handle data assertions and data variables
									}
									instTestingExp = mTestingOntologyBuilder->getObjectIntersectionOf(instTestingExpList);
									mTestingOntologyBuilder->completeBuilding();
									mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());

									CClassTermExpression* classTermExpOfInt = dynamic_cast<CClassTermExpression*>(instTestingExp);
									if (classTermExpOfInt) {
										COptimizedComplexConceptItem* assocConceptItem = nullptr;
										queryProcessing |= initializeComplexConceptQueryProcessing(queryProcessingData, nullptr, classTermExpOfInt, false, false, false, false, false, -1, &assocConceptItem);

										if (assocConceptItem && mConfExtendedLogging) {
											LOG(INFO, getDomain(), logTr("Using complex concept item %2 for computing bounds for variable %1.").arg(lastVarExpression->getName()).arg(assocConceptItem->getConceptItemId()), this);
										}
									}




								} else {


									COptimizedComplexBuildingVariableCompositionsItem* varBuildItem = new COptimizedComplexBuildingVariableCompositionsItem(queryProcessingData);
									varBuildItem->setAllVariableSet(varExpSet);
									varBuildItem->setDistinguishedVariableSet(disVarList->toSet());
									varBuildItem->setReductionDeniedVariableSet(disVarList->toSet());
									for (CVariableBindingOrdering* resultOrderingLinkerIt = compAssIndVarQuery->getResultOrderingLinker(); resultOrderingLinkerIt; resultOrderingLinkerIt = resultOrderingLinkerIt->getNext()) {
										varBuildItem->getReductionDeniedVariableSet()->insert(resultOrderingLinkerIt->getOrderingVariableExpression());
									}
									if (allAnonymousVariables || distinct || mConfBindingReducingForNonDistinctAnswerQueries) {
										varBuildItem->setBindingsReducible(true);
									}
									mTestingOntologyBuilder->initializeBuilding();


									*varBuildItem->getVariableSingleCardinalitySet() = convertedVariableSet;






									// do rolling up of assertions with anonymous variables
									CClassTermExpression* topClassTermExp = testingOnto->getBuildData()->getTopClassExpression();
									CObjectPropertyTermExpression* topObjPropExp = testingOnto->getBuildData()->getTopObjectPropertyExpression();
									QList<CExpressionVariable*> anonymousIndiVariableProcessingList = anonymousIndiVariableSet.toList();
									QSet<CExpressionVariable*> anonymousIndiVariableProcessingSet;

									QHash<CExpressionVariable*, CBuildExpression*> varRolledUpClassExpHash;
									QSet<CExpressionVariable*> rolledVarExpSet;

									while (!anonymousIndiVariableProcessingList.isEmpty()) {

										CExpressionVariable* anonyIndiVarExp = anonymousIndiVariableProcessingList.takeFirst();
										anonymousIndiVariableProcessingSet.remove(anonyIndiVarExp);

										if (!rolledVarExpSet.contains(anonyIndiVarExp)) {
											cint64 propAssPropagationCount = 0;
											bool rollingUpPossible = true;
											QList<CAxiomExpression*> axiomList = varExpAxiomExpHash.values(anonyIndiVarExp);
											for (CAxiomExpression* axiomExp : axiomList) {
												CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(axiomExp);
												if (propAssExp) {
													CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
													CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
													CIndividualVariableExpression* otherVarExp = firstVarExp;
													if (anonyIndiVarExp == firstVarExp) {
														otherVarExp = secondVarExp;
													}
													if (firstVarExp && secondVarExp && firstVarExp != secondVarExp && !rolledVarExpSet.contains(otherVarExp)) {
														propAssPropagationCount++;
													}
													if (firstVarExp == secondVarExp) {
														CRole* role = testingOnto->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propAssExp->getObjectPropertyTermExpression());
														if (role && role->isComplexRole()) {
															rollingUpPossible = false;
														}
													}
												}
											}
											if (propAssPropagationCount == 1 && rollingUpPossible) {

												rolledVarExpSet.insert(anonyIndiVarExp);
												CClassTermExpression* rollingUpExp = nullptr;
												QList<CBuildExpression*> rollingUpExpList(varRolledUpClassExpHash.values(anonyIndiVarExp));

												bool inversedRollingUp = false;
												CObjectPropertyTermExpression* rollingUpPropExpression = nullptr;
												CExpressionVariable* rollingUpVarExpression = nullptr;

												for (CAxiomExpression* assExp : axiomList) {
													CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
													if (classAssExp) {
														rollingUpExpList.append(mTestingOntologyBuilder->rebuildExpression(classAssExp->getClassTermExpression()));
													}
													CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
													if (propAssExp) {
														CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
														CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
														CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
														CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
														CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
														if (firstVarExp == anonyIndiVarExp && secondVarExp == anonyIndiVarExp) {
															rollingUpExpList.append(mTestingOntologyBuilder->getObjectHasSelf(propExp));
														} else if (firstVarExp == anonyIndiVarExp) {
															if (secondVarExp == nullptr) {
																rollingUpExpList.append(mTestingOntologyBuilder->getObjectHasValue(propExp, secondIndiExp));
															} else if (!rolledVarExpSet.contains(secondVarExp)) {
																rollingUpPropExpression = propExp;
																inversedRollingUp = true;
																rollingUpVarExpression = secondVarExp;
															}
														} else if (secondVarExp == anonyIndiVarExp) {
															if (firstVarExp == nullptr) {
																rollingUpExpList.append(mTestingOntologyBuilder->getObjectHasValue(mTestingOntologyBuilder->getInverseObjectPropertyOf(propExp), firstIndiExp));
															} else if (!rolledVarExpSet.contains(firstVarExp)) {
																rollingUpPropExpression = propExp;
																inversedRollingUp = false;
																rollingUpVarExpression = firstVarExp;
															}
														}
													}

													CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
													if (dataPropAssExp) {
														CDataPropertyTermExpression* dataPropTermExp = dataPropAssExp->getDataPropertyTermExpression();
														CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
														CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
														if (indiVarExp && !dataVarExp) {
															CDataLiteralExpression* dataLiteralExp = (CDataLiteralExpression*)dataPropAssExp->getDataLiteralTermExpression();
															dataLiteralExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLiteralExp);
															rollingUpExpList.append(mTestingOntologyBuilder->getDataHasValue(dataPropTermExp, dataLiteralExp));
														}

													}
												}
												if (rollingUpExpList.size() > 1) {
													rollingUpExp = (CClassTermExpression*)mTestingOntologyBuilder->getObjectIntersectionOf(rollingUpExpList);
												} else if (rollingUpExpList.size() > 0) {
													rollingUpExp = (CClassTermExpression*)rollingUpExpList.first();
												} else {
													rollingUpExp = topClassTermExp;
												}

												CClassTermExpression* rolledUpExp = nullptr;
												if (inversedRollingUp) {
													rolledUpExp = mTestingOntologyBuilder->getObjectSomeValuesFrom(mTestingOntologyBuilder->getInverseObjectPropertyOf(rollingUpPropExpression), rollingUpExp);
												} else {
													rolledUpExp = mTestingOntologyBuilder->getObjectSomeValuesFrom(rollingUpPropExpression, rollingUpExp);
												}
												varRolledUpClassExpHash.insertMulti(rollingUpVarExpression, rolledUpExp);
												if (anonymousIndiVariableSet.contains(rollingUpVarExpression) && !anonymousIndiVariableProcessingSet.remove(anonyIndiVarExp)) {
													anonymousIndiVariableProcessingSet.insert(anonyIndiVarExp);
													anonymousIndiVariableProcessingList.append(rollingUpVarExpression);
												}
											}
										}

									}
									if (mConfExtendedLogging && !rolledVarExpSet.isEmpty()) {
										QStringList varList;
										for (CExpressionVariable* var : rolledVarExpSet) {
											varList.append(var->getName());
										}
										LOG(INFO, getDomain(), logTr("Rolled-up %1 variables: %2.").arg(rolledVarExpSet.size()).arg(varList.join(", ")), this);
									}





									// building complex concept expressions for concept items
									QSet<CExpressionVariable*> conceptTermOmittedIndiVarSet;
									QSet<CExpressionVariable*> prepareIndiVarSet;
									QHash<CExpressionVariable*, CBuildExpression*> individualTriggerUpdatableHash;
									QList<QPair<CIndividualVariableExpression*, CClassTermExpression*>> varClassInstTestExpList;
									for (CExpressionVariable* varExp : varExpSet) {

										CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);
										if (indiVarExp && !rolledVarExpSet.contains(varExp) && !anonymousIndiVariableSet.contains(varExp)) {
											CClassTermExpression* instTestingExp = nullptr;
											QList<CBuildExpression*> instTestingExpList(varRolledUpClassExpHash.values(indiVarExp));


											bool conceptTermRequired = true;
											QList<CAxiomExpression*> assExplist = varExpAxiomExpHash.values(varExp);
											if (assExplist.size() == 1) {
												CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExplist.first());
												if (propAssExp) {
													CIndividualVariableExpression* otherVarExp = nullptr;
													CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
													CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
													CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
													CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
													if (firstVarExp && secondVarExp) {
														otherVarExp = firstVarExp;
														if (firstVarExp == indiVarExp) {
															otherVarExp = secondVarExp;
														}

														QList<CAxiomExpression*> otherAssExplist = varExpAxiomExpHash.values(otherVarExp);

														// currently only works if propagated correctly
														if (otherAssExplist.size() == 1 && !conceptTermOmittedIndiVarSet.contains(otherVarExp)) {
															conceptTermOmittedIndiVarSet.insert(indiVarExp);
															conceptTermRequired = false;
														}

													}

												}
											}

											if (conceptTermRequired) {
												for (CAxiomExpression* assExp : assExplist) {
													CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
													if (classAssExp) {
														instTestingExpList.append(mTestingOntologyBuilder->rebuildExpression(classAssExp->getClassTermExpression()));
													}
													CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
													if (propAssExp) {
														CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
														if (propExp != topObjPropExp) {
															CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
															CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
															CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
															CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
															if (firstVarExp == varExp && secondVarExp == varExp) {
																instTestingExpList.append(mTestingOntologyBuilder->getObjectHasSelf(propExp));
															} else if (firstVarExp == varExp) {
																if (secondVarExp == nullptr) {
																	instTestingExpList.append(mTestingOntologyBuilder->getObjectHasValue(propExp, secondIndiExp));
																} else {
																	if (!rolledVarExpSet.contains(secondVarExp)) {
																		CObjectSomeValuesFromExpression* someValuesFromExp = mTestingOntologyBuilder->getObjectSomeValuesFrom(propExp, topClassTermExp);
																		if (!anonymousIndiVariableSet.contains(secondVarExp)) {
																			individualTriggerUpdatableHash.insertMulti(varExp, someValuesFromExp);
																		}
																		instTestingExpList.append(someValuesFromExp);
																	}
																}
															} else if (secondVarExp == varExp) {
																CObjectPropertyTermExpression* invPropExp = nullptr;
																CRole* role = testingOnto->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
																if (!role || !role->isSymmetric()) {
																	invPropExp = mTestingOntologyBuilder->getInverseObjectPropertyOf(propExp);
																} else {
																	invPropExp = propExp;
																}
																if (firstVarExp == nullptr) {
																	instTestingExpList.append(mTestingOntologyBuilder->getObjectHasValue(invPropExp, firstIndiExp));
																} else {
																	if (!rolledVarExpSet.contains(firstVarExp)) {
																		CObjectSomeValuesFromExpression* someValuesFromExp = mTestingOntologyBuilder->getObjectSomeValuesFrom(invPropExp, topClassTermExp);
																		if (!anonymousIndiVariableSet.contains(firstVarExp)) {
																			individualTriggerUpdatableHash.insertMulti(varExp, someValuesFromExp);
																		}
																		instTestingExpList.append(someValuesFromExp);
																	}
																}
															}
														}
													}

													CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
													if (dataPropAssExp) {
														CDataPropertyTermExpression* dataPropTermExp = dataPropAssExp->getDataPropertyTermExpression();
														CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
														CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
														if (indiVarExp && !dataVarExp) {
															CDataLiteralExpression* dataLiteralExp = (CDataLiteralExpression*)dataPropAssExp->getDataLiteralTermExpression();
															dataLiteralExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLiteralExp);
															instTestingExpList.append(mTestingOntologyBuilder->getDataHasValue(dataPropTermExp, dataLiteralExp));
														}
														if (indiVarExp && dataVarExp) {
															instTestingExpList.append(mTestingOntologyBuilder->getDataSomeValuesFrom(dataPropTermExp, mTestingOntologyBuilder->getTopDataRange()));
														}

													}
												}


												instTestingExp = (CClassTermExpression*)mTestingOntologyBuilder->getObjectIntersectionOf(instTestingExpList);
												if (instTestingExpList.size() > 1) {
													instTestingExp = (CClassTermExpression*)mTestingOntologyBuilder->getObjectIntersectionOf(instTestingExpList);
												} else {
													instTestingExp = (CClassTermExpression*)instTestingExpList.first();
												}
												prepareIndiVarSet.insert(indiVarExp);
												varClassInstTestExpList.append(QPair<CIndividualVariableExpression*, CClassTermExpression*>(indiVarExp, instTestingExp));
											}

										}

									}

									if (!varClassInstTestExpList.isEmpty()) {
										LOG(INFO, getDomain(), logTr("Builded %1 class term expressions for complex concept items.").arg(varClassInstTestExpList.size()), this);
									}






									QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> absorptionBasedHandlingQueryPartDatas;
									QSet<CExpressionVariable*> anonymVarExpSubQueryIncludedSet;
									QSet<CExpressionVariable*> absorptionAnonymousInterpretedIndiVariableSet(anonymousIndiVariableSet);
									if (mConfVariableAbsorptionOverFullQuery && allAnonymousVariables && !anonymousIndiVariableSet.isEmpty() && anonymousIndiVariableSet.size() != rolledVarExpSet.size()) {
										absorptionAnonymousInterpretedIndiVariableSet = initialAnonymousIndiVariableSet;
									}
									// identify sub queries with non-distinguished variables
									for (CExpressionVariable* varExp : varExpSet) {

										CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);

										if (indiVarExp && !rolledVarExpSet.contains(indiVarExp) && !anonymVarExpSubQueryIncludedSet.contains(indiVarExp) && absorptionAnonymousInterpretedIndiVariableSet.contains(varExp)) {

											QSet<CExpressionVariable*> anonymVarExpSubQueryProcessingSet;
											QList<CExpressionVariable*> anonymVarExpSubQueryProcessingList;
											anonymVarExpSubQueryIncludedSet.insert(varExp);
											anonymVarExpSubQueryProcessingSet.insert(varExp);
											anonymVarExpSubQueryProcessingList.append(varExp);

											COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionBasedQueryPartData = new COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData(absorptionAnonymousInterpretedIndiVariableSet, prepareIndiVarSet);

											while (!anonymVarExpSubQueryProcessingList.isEmpty()) {

												CExpressionVariable* processingVarExp = anonymVarExpSubQueryProcessingList.takeFirst();
												QList<CBuildExpression*> varClassExpList(varRolledUpClassExpHash.values(processingVarExp));

												if (absorptionAnonymousInterpretedIndiVariableSet.contains(processingVarExp)) {

													QList<CAxiomExpression*> axiomList = varExpAxiomExpHash.values(processingVarExp);
													for (CAxiomExpression* assExp : axiomList) {

														CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
														if (classAssExp) {
															varClassExpList.append(mTestingOntologyBuilder->rebuildExpression(classAssExp->getClassTermExpression()));
														}
														CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
														if (propAssExp) {
															CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
															CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
															CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
															CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
															CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
															if (firstVarExp == processingVarExp && secondVarExp == processingVarExp) {
																CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propExp);
																if (!role->isComplexRole()) {
																	varClassExpList.append(mTestingOntologyBuilder->getObjectHasSelf(propExp));
																} else {
																	absorptionBasedQueryPartData->addVariableSelfPropertyAssertion(propAssExp);
																}
															} else if (firstVarExp == processingVarExp) {
																if (secondVarExp == nullptr) {
																	varClassExpList.append(mTestingOntologyBuilder->getObjectHasValue(propExp, secondIndiExp));
																} else if (!rolledVarExpSet.contains(secondVarExp)) {
																	absorptionBasedQueryPartData->addVariableNeighbouringPropertyAssertion(propAssExp);
																	if (!anonymVarExpSubQueryProcessingSet.contains(secondVarExp)) {
																		anonymVarExpSubQueryProcessingSet.insert(secondVarExp);
																		anonymVarExpSubQueryIncludedSet.insert(secondVarExp);
																		anonymVarExpSubQueryProcessingList.append(secondVarExp);
																	}
																}
															} else if (secondVarExp == processingVarExp) {
																if (firstVarExp == nullptr) {
																	varClassExpList.append(mTestingOntologyBuilder->getObjectHasValue(mTestingOntologyBuilder->getInverseObjectPropertyOf(propExp), firstIndiExp));
																} else if (!rolledVarExpSet.contains(firstVarExp))  {
																	absorptionBasedQueryPartData->addVariableNeighbouringPropertyAssertion(propAssExp);
																	if (!anonymVarExpSubQueryProcessingSet.contains(firstVarExp)) {
																		anonymVarExpSubQueryProcessingSet.insert(firstVarExp);
																		anonymVarExpSubQueryIncludedSet.insert(firstVarExp);
																		anonymVarExpSubQueryProcessingList.append(firstVarExp);
																	}
																}
															}
														}

														CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
														if (dataPropAssExp) {
															CDataPropertyTermExpression* dataPropTermExp = dataPropAssExp->getDataPropertyTermExpression();
															CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
															CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
															if (indiVarExp && !dataVarExp) {
																CDataLiteralExpression* dataLiteralExp = (CDataLiteralExpression*)dataPropAssExp->getDataLiteralTermExpression();
																dataLiteralExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLiteralExp);
																varClassExpList.append(mTestingOntologyBuilder->getDataHasValue(dataPropTermExp, dataLiteralExp));
															}
														}



													}
												}
												absorptionBasedQueryPartData->addVariableClassExpressions(processingVarExp, varClassExpList);


											}

											absorptionBasedHandlingQueryPartDatas.append(absorptionBasedQueryPartData);
										}
									}







									mTestingOntologyBuilder->completeBuilding();


									QList<QPair<CIndividualVariableExpression*, TConceptNegPair>> varConNegInstTestExpList;
									// upgrade \exits r.\top concepts to use individual trigger if possible
									for (QPair<CIndividualVariableExpression*, CClassTermExpression*> varClassPair : varClassInstTestExpList) {
										CClassTermExpression* classTermExp = varClassPair.second;
										CIndividualVariableExpression* varTermExp = varClassPair.first;

										TConceptNegPair upgradedConNegPair = upgradeExistTopToIndividualTrigger(classTermExp, varTermExp, &individualTriggerUpdatableHash, answererContext);
										varConNegInstTestExpList.append(QPair<CIndividualVariableExpression*, TConceptNegPair>(varTermExp, upgradedConNegPair));

									}


									QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* propagationFinalizationConceptAbsorptionDataHash = mOntoAnsweringItem->getPropagationFinalizationConceptAbsorptionDataHash();


									QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> entailmentCheckingAbsorptionBasedHandlingQueryPartDatas;
									for (COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data : absorptionBasedHandlingQueryPartDatas) {

										QStringList varList;
										if (mConfExtendedLogging) {
											for (CExpressionVariable* var : *data->getAbsorptionVariableSet()) {
												varList.append(var->getName());
											}
										}
										COccurrenceStatisticsCacheReader* occStatsCacheReader = answererContext->getAnsweringCalculationHandler()->getOccurrenceStatisticsCacheReader();
										if (mConfVariablePreAbsorption) {
											COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler preAbsorptionHandler(data, mOntoAnsweringItem, occStatsCacheReader);
											preAbsorptionHandler.readConfig(mOntoAnsweringItem->getCalculationConfiguration());
											cint64 prevConCount = mOntoAnsweringItem->getTestingOntology()->getTBox()->getConceptCount();
											preAbsorptionHandler.absorbQueryPart();
											cint64 newConCount = mOntoAnsweringItem->getTestingOntology()->getTBox()->getConceptCount();
											if (mConfExtendedLogging) {
												LOG(INFO, getDomain(), logTr("Preabsorbed existential query part consiting of variables %1 with %2 concepts.").arg(varList.join(", ")).arg(newConCount - prevConCount), this);
											}

										}

										COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler absorptionHandler(data, mOntoAnsweringItem, occStatsCacheReader);
										absorptionHandler.readConfig(mOntoAnsweringItem->getCalculationConfiguration());
										cint64 prevConCount = mOntoAnsweringItem->getTestingOntology()->getTBox()->getConceptCount();
										absorptionHandler.absorbQueryPart();
										cint64 newConCount = mOntoAnsweringItem->getTestingOntology()->getTBox()->getConceptCount();
										if (mConfExtendedLogging) {
											LOG(INFO, getDomain(), logTr("Absorbed existential query part consisting of variables %1 with %2 concepts (%3 is initialization and %4 is first binding variable).").arg(varList.join(", ")).arg(newConCount - prevConCount).arg(data->getInitializerVariableExpression()->getName()).arg(data->getBindingPropagationStartVariableExpression()->getName()), this);
										}

										mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartAbsorptionCount();


										CConcept* propFinalConcept = data->getPropagationFinalizationConcept();
										propagationFinalizationConceptAbsorptionDataHash->insert(propFinalConcept, data);

										if (data->hasPreparationVariables()) {
											varBuildItem->addAbsorbedBasedQueryPartItemExtensionHandling(data);
										} else {
											varBuildItem->addAbsorbedBasedQueryPartEntailmentCheckingHandling(data);
											entailmentCheckingAbsorptionBasedHandlingQueryPartDatas.append(data);
										}

									}
									if (mConfDebugTestingOntologyWriting) {
										COntologyTextFormater::writeOntologyToFile(mOntoAnsweringItem->getTestingOntology(), "Debugging/Answering/Absorbed-Query-Expressions.txt");
									}

									mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());

									for (QPair<CIndividualVariableExpression*, TConceptNegPair> varConNegPair : varConNegInstTestExpList) {
										TConceptNegPair conNegPair = varConNegPair.second;
										CIndividualVariableExpression* varTermExp = varConNegPair.first;

										CConcept* testingConcept = conNegPair.first;
										bool testingNegation = conNegPair.second;

										cint64 requiredInstancesCount = 1;
										if (varConNegInstTestExpList.size() == 1) {
											// request all if the conjunctive query can be reduced to a complex concept instances query
											requiredInstancesCount = -1;
										}

										COptimizedComplexConceptItem* assocConceptItem = nullptr;
										queryProcessing |= initializeComplexConceptQueryProcessing(nullptr, varBuildItem, testingConcept, testingNegation, false, false, false, false, false, requiredInstancesCount, &assocConceptItem);

										if (assocConceptItem && mConfExtendedLogging) {
											LOG(INFO, getDomain(), logTr("Using complex concept item %2 for computing bounds of variable %1.").arg(varTermExp->getName()).arg(assocConceptItem->getConceptItemId()), this);
										}

										COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(testingConcept, testingNegation);
										varBuildItem->addVariableComplexConceptItem(varTermExp, conceptItem);
									}
									varBuildItem->addVariableDataPropertyAssertions(bothVarDataPropAssPropagationList);
									varBuildItem->addVariablePropertyAssertions(bothVarPropAssPropagationList);
									varBuildItem->addBasicDataPropertyAssertions(onlyDataVarDataPropAssList);


									if (mConfDebugTestingOntologyWriting) {
										COntologyTextFormater::writeOntologyToFile(mOntoAnsweringItem->getTestingOntology(), "Debugging/Answering/Absorbed-Preprocessed-Query-Expressions.txt");
									}

									for (COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data : entailmentCheckingAbsorptionBasedHandlingQueryPartDatas) {
										createAbsorbedQueryPartEntailmentTest(data, queryProcessingData, answererContext);
									}

									queryProcessingData->incBuildingVariableItem();
									queryProcessingData->setVariableBuildingItem(varBuildItem);
									if (!queryProcessing) {
										varBuildItem->setProcessingQueued(true);
										mOntoAnsweringItem->addProcessingVariableBuildingItem(varBuildItem);
										queryProcessing = true;
									}



								}

							}
						}
					}
				}

				if (!queryCompleted && !queryProcessing) {
					finishQueryProcessing(queryProcessingData);
				}

				return this;
			}





			TConceptNegPair COptimizedComplexConceptAnsweringHandler::upgradeExistTopToIndividualTrigger(CClassTermExpression* classTermExp, CExpressionVariable* varTermExp, QHash<CExpressionVariable*, CBuildExpression*>* individualTriggerUpdatableHash, CAnswererContext* answererContext) {


				QSet<TConceptNegPair> upgradableConNegSet;
				for (CBuildExpression* upgradableExp : individualTriggerUpdatableHash->values(varTermExp)) {
					CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)upgradableExp);
					bool testingNegation = testingConcept->hasMappingNegation();
					upgradableConNegSet.insert(TConceptNegPair(testingConcept, testingNegation));
				}

				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();
				CConcept* topConcept = mOntoAnsweringItem->getOntology()->getTBox()->getTopConcept();

				CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)classTermExp);
				bool conceptNegation = concept->hasMappingNegation();

				QList<TConceptNegPair> upgradeOpConList;
				QList<TConceptNegPair> remainingOpConList;
				QSet<TConceptNegPair> allUpgradedOpConSet;



				QList<TConceptNegPair> testingConList;
				testingConList.append(TConceptNegPair(concept, conceptNegation));
				while (!testingConList.isEmpty()) {
					TConceptNegPair testingConNegPair = testingConList.takeFirst();
					CConcept* testingConcept = testingConNegPair.first;
					bool testingNegation = testingConNegPair.second;
					cint64 testingConOpCode = testingConcept->getOperatorCode();
					bool upgradable = false;
					bool remaining = true;

					if (!testingNegation && testingConOpCode == CCAND || testingNegation && testingConOpCode == CCOR) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = testingConcept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opConNegation = opConLinkerIt->isNegated() ^ testingNegation;
							testingConList.append(TConceptNegPair(opConcept, opConNegation));
						}
						remaining = false;
					} else if (!testingNegation && testingConOpCode == CCSOME) {
						if (testingConcept->getOperandCount() == 1) {
							if (testingConcept->getOperandList()->getData() == topConcept && !testingConcept->getOperandList()->isNegated()) {
								if (upgradableConNegSet.contains(TConceptNegPair(testingConcept, testingNegation))) {
									upgradable = true;
								}
							}
						}
					} else if (testingNegation && testingConOpCode == CCALL) {
						if (testingConcept->getOperandCount() == 1) {
							if (testingConcept->getOperandList()->getData() == topConcept && testingConcept->getOperandList()->isNegated()) {
								if (upgradableConNegSet.contains(TConceptNegPair(testingConcept, testingNegation))) {
									upgradable = true;
								}
							}
						}
					}
					if (upgradable) {
						upgradeOpConList.append(TConceptNegPair(testingConcept, testingNegation));
					} else if (remaining) {
						remainingOpConList.append(TConceptNegPair(testingConcept, testingNegation));
						bool originalNegation = testingNegation ^ testingConcept->hasMappingNegation();
						allUpgradedOpConSet.insert(TConceptNegPair(testingConcept, originalNegation));
					}

				}


				QHash<TConceptNegPair, TConceptNegPair>* individualTriggerSingleConceptUpgradeHash = mOntoAnsweringItem->getIndividualTriggerSingleConceptUpgradeHash();
				QHash< QSet<TConceptNegPair>, TConceptNegPair>* individualTriggerMultiConceptUpgradeHash = mOntoAnsweringItem->getIndividualTriggerMultiConceptUpgradeHash();


				if (upgradeOpConList.isEmpty()) {
					return TConceptNegPair(concept, conceptNegation);
				} else {

					CMemoryAllocationManager* memMan = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					CConceptVector* conceptVec = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getTBox()->getConceptVector();

					QList<TConceptNegPair> upgradedOpConList;
					bool allExistingUpgrade = true;
					for (TConceptNegPair conNegPar : upgradeOpConList) {
						CConcept* opConcept = conNegPar.first;
						bool opConceptNegation = conNegPar.second;
						bool originalOpConNegation = opConceptNegation ^ opConcept->hasMappingNegation();
						TConceptNegPair opConOriginalNegPair(opConcept, originalOpConNegation);

						if (!individualTriggerSingleConceptUpgradeHash->contains(opConOriginalNegPair)) {
							CConcept* upgradedOpConcept = CObjectAllocator< CConcept >::allocateAndConstruct(memMan);
							upgradedOpConcept->initConcept();
							cint64 nextConTag = conceptVec->getItemCount();
							upgradedOpConcept->initTag(nextConTag);
							upgradedOpConcept->setOperatorCode(opConcept->getOperatorCode());
							upgradedOpConcept->setRole(opConcept->getRole());
							conceptVec->setLocalData(nextConTag, upgradedOpConcept);

							CSortedNegLinker<CConcept*>* newIndiTrigOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
							newIndiTrigOpCon->init(indiTriggerConcept, opConcept->getOperandList()->isNegated(), upgradedOpConcept->getOperandList());
							upgradedOpConcept->setOperandList(newIndiTrigOpCon);
							upgradedOpConcept->incOperandCount();
							TConceptNegPair upgradedOpConNegPair(upgradedOpConcept, opConceptNegation);
							individualTriggerSingleConceptUpgradeHash->insert(opConOriginalNegPair, upgradedOpConNegPair);
							upgradedOpConList.append(upgradedOpConNegPair);
							allUpgradedOpConSet.insert(opConOriginalNegPair);
							allExistingUpgrade = false;
						} else {
							TConceptNegPair upgradedConNegPar = individualTriggerSingleConceptUpgradeHash->value(opConOriginalNegPair);

							CConcept* upgradedOpConcept = upgradedConNegPar.first;
							bool correctedConceptNegation = upgradedConNegPar.second ^ upgradedOpConcept->hasMappingNegation();

							allUpgradedOpConSet.insert(opConOriginalNegPair);
							upgradedOpConList.append(TConceptNegPair(upgradedOpConcept, correctedConceptNegation));
						}
					}

					if (allExistingUpgrade && individualTriggerMultiConceptUpgradeHash->contains(allUpgradedOpConSet)) {
						return individualTriggerMultiConceptUpgradeHash->value(allUpgradedOpConSet);
					} else {

						if (allUpgradedOpConSet.size() == 1) {
							TConceptNegPair allUpgradedConNegPair(*upgradedOpConList.begin());
							individualTriggerMultiConceptUpgradeHash->insert(allUpgradedOpConSet, allUpgradedConNegPair);
							return allUpgradedConNegPair;
						} else {

							CConcept* upgradedAndConcept = CObjectAllocator< CConcept >::allocateAndConstruct(memMan);
							upgradedAndConcept->initConcept();
							cint64 nextConTag = conceptVec->getItemCount();
							upgradedAndConcept->initTag(nextConTag);
							upgradedAndConcept->setOperatorCode(concept->getOperatorCode());
							upgradedAndConcept->setMappingNegation(conceptNegation);
							conceptVec->setLocalData(nextConTag, upgradedAndConcept);

							for (TConceptNegPair conNegPar : remainingOpConList) {
								CConcept* opConcept = conNegPar.first;
								bool opConceptNegation = conNegPar.second;

								CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
								newBaseOpCon->init(opConcept, opConceptNegation, upgradedAndConcept->getOperandList());
								upgradedAndConcept->setOperandList(newBaseOpCon);
								upgradedAndConcept->incOperandCount();
							}


							for (TConceptNegPair upgradedAndConNegPar : upgradedOpConList) {
								CConcept* upgradedOpConcept = upgradedAndConNegPar.first;
								bool opConceptNegation = upgradedAndConNegPar.second;

								CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
								newBaseOpCon->init(upgradedOpConcept, opConceptNegation, upgradedAndConcept->getOperandList());
								upgradedAndConcept->setOperandList(newBaseOpCon);
								upgradedAndConcept->incOperandCount();
							}
							TConceptNegPair allUpgradedConNegPair(upgradedAndConcept, conceptNegation);
							individualTriggerMultiConceptUpgradeHash->insert(allUpgradedOpConSet, allUpgradedConNegPair);
							return allUpgradedConNegPair;
						}
					}


				}
			}






			CVariableBindingResult* createVariableBindingResult(CComplexAssertionsIndividualVariablesAnsweringQuery* query) {
				if (query->getResultOrderingLinker()) {
					return new CVariableBindingStringDataResult();
				}
				else {
					return new CVariableBindingStringResult();
				}
			}


			CVariableBindingResult* createVariableBindingResult(CDataLiteral* dataLiteral, CComplexAssertionsIndividualVariablesAnsweringQuery* query) {
				return createVariableBindingResult(query)->initVariableBinding(dataLiteral);
			}



			CVariableBindingResult* createVariableBindingResult(CIndividual* individual, CComplexAssertionsIndividualVariablesAnsweringQuery* query) {
				return createVariableBindingResult(query)->initVariableBinding(individual);
			}




			CVariableBindingResult* createVariableBindingResult(const CIndividualReference& indiRef, CComplexAssertionsIndividualVariablesAnsweringQuery* query, CIndividualNameResolver* indiNameResolver) {
				return createVariableBindingResult(query)->initVariableBinding(indiRef, indiNameResolver);
			}


			CVariableBindingResult* createVariableBindingResult(const CIndividualReference& indiRef, CComplexAssertionsIndividualVariablesAnsweringQuery* query, const QString& resolvedIndiName) {
				return createVariableBindingResult(query)->initVariableBinding(indiRef, resolvedIndiName);
			}




			bool COptimizedComplexConceptAnsweringHandler::ensureRoleTargetsRealization(CRole* role, bool inversed, const QSet<CIndividualReference>& individualSourceSet, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext) {
				QList<COntologyProcessingRequirement*> reqList;
				QList<CIndividualReference> reqIndiList;
				bool targetsRealizationMissing = false;
				for (CIndividualReference individualSource : individualSourceSet) {
					CRoleTargetsRealizationHasher enHasher(role, individualSource, inversed);
					CEntailmentVariableBuildingItemData*& varBuildData = mRoleTargetsProcDataHash[enHasher];
					if (!varBuildData) {
						varBuildData = new CEntailmentVariableBuildingItemData();
					}
					if (!varBuildData->mHasResult) {
						targetsRealizationMissing = true;
						if (!varBuildData->mHasRequested) {
							COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role, inversed, individualSource);
							reqList.append(roleRealReq);
							reqIndiList.append(individualSource);
						}
						CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* procDataLinker = mOntoAnsweringItem->createBuildingVariableItemProcessingLinker();
						procDataLinker->setData(buildingVarItem);
						varBuildData->mProcDataLinker = procDataLinker->append(varBuildData->mProcDataLinker);
						buildingVarItem->incWaitingRoleTargetsRealizationCount();
					}
				}
				if (!reqList.isEmpty()) {
					CAnsweringMessageDataRequirementCompletedRoleTargetsRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRoleTargetsRealization(role, inversed, reqIndiList);
					processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
				}
				return targetsRealizationMissing;
			}





			bool COptimizedComplexConceptAnsweringHandler::checkEntailmentRoleAssertion(CRole* role, CIndividual* individualSource, CIndividual* individualDest, CComplexQueryProcessingData* procData, CAnswererContext* answererContext) {
				CEntailmentRoleAssertionHasher enHasher(role, individualSource, individualDest);
				CEntailmentQueryProcessingData*& queryProcData = mRoleAssEntProcDataHash[enHasher];
				if (!queryProcData) {
					queryProcData = new CEntailmentQueryProcessingData();
				}
				if (queryProcData->mHasResult) {
					if (!queryProcData->mEntailed) {
						procData->setUnsatisfiable(true);
						return false;
					} else {
						return false;
					}
				} else {
					procData->incEntailmentComputation();
					if (!queryProcData->mHasRequested) {
						CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment* reqCompMess = new CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment(queryProcData, role,individualSource,individualDest);
						QList<COntologyProcessingRequirement*> reqList;
						COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role,false,individualSource,individualDest);
						reqList.append(roleRealReq);
						processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
					}
					CXLinker<CComplexQueryProcessingData*>* procDataLinker = mOntoAnsweringItem->createQueryProcessingLinker();
					procDataLinker->setData(procData);
					queryProcData->mProcDataLinker = procDataLinker->append(queryProcData->mProcDataLinker);
					return true;
				}
			}




			bool COptimizedComplexConceptAnsweringHandler::checkEntailmentConceptAssertion(CConcept* concept, bool negation, CIndividual* individual, CComplexQueryProcessingData* procData, CAnswererContext* answererContext) {
				CEntailmentConceptAssertionHasher enHasher(concept, negation, individual);
				CEntailmentQueryProcessingData*& queryProcData = mConAssEntProcDataHash[enHasher];
				if (!queryProcData) {
					queryProcData = new CEntailmentQueryProcessingData();
				}
				if (queryProcData->mHasResult) {
					if (!queryProcData->mEntailed) {
						procData->setUnsatisfiable(true);
						return false;
					} else {
						return false;
					}
				} else {
					procData->incEntailmentComputation();
					if (concept->hasClassName() && !negation) {
						if (!queryProcData->mHasRequested) {
							CAnsweringMessageDataRequirementCompletedConceptAssertionEntailment* reqCompMess = new CAnsweringMessageDataRequirementCompletedConceptAssertionEntailment(queryProcData, concept, individual);
							QList<COntologyProcessingRequirement*> reqList;
							COntologyProcessingConceptRealizationRequirement* conRealReq = new COntologyProcessingConceptRealizationRequirement(individual, concept);
							reqList.append(conRealReq);
							processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
						} 
						CXLinker<CComplexQueryProcessingData*>* procDataLinker = mOntoAnsweringItem->createQueryProcessingLinker();
						procDataLinker->setData(procData);
						queryProcData->mProcDataLinker = procDataLinker->append(queryProcData->mProcDataLinker);
						return true;
					} else {
						CSatisfiableCalculationJob* satCalcJob = nullptr;
						CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
						bool topObjectRoleUsed = false;
						if (!mOntoAnsweringItem->getTestingOntology()->getABox()->hasUniversalConnectionIndividual()) {
							topObjectRoleUsed = checkHasNewTopRolePropagation(concept, !negation);
						}
						satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, topObjectRoleUsed, answererContext);
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(concept, !negation, individual, satCalcJob);
						CAnsweringMessageDataCalculationCompletedInstanceEntailment* completedMessage = new CAnsweringMessageDataCalculationCompletedInstanceEntailment(satCalcJob, queryProcData, individual, concept, negation);
						mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptAssertionEntailmentTestingCount();
						processCalculationJob(answererContext, satCalcJob, completedMessage);
						return true;
					}
				}
			}





			bool COptimizedComplexConceptAnsweringHandler::queueConceptItemProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep) {
				if (!conceptItem->hasQueuedProcessStep() && !compStep->isComputationProcessQueued() && !compStep->isComputationProcessProcessing()) {
					conceptItem->setQueuedProcessStep(compStep);
					compStep->setComputationProcessQueued(true);
					COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
					processingStep->addQueuedConceptItem(conceptItem);
					return true;
				}
				return false;
			}


			bool COptimizedComplexConceptAnsweringHandler::requeueConceptItemNextProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep) {
				conceptItem->setQueuedProcessStep(nullptr);
				CComplexConceptStepComputationProcess* nextCompStep = conceptItem->getComputationProcess()->getNextConceptStepComputationProcess(compStep, true);
				if (nextCompStep && !conceptItem->hasQueuedProcessStep() && !nextCompStep->isComputationProcessQueued() && !nextCompStep->isComputationProcessProcessing()) {
					conceptItem->setQueuedProcessStep(nextCompStep);
					nextCompStep->setComputationProcessQueued(true);
					COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(nextCompStep->getComputationType());
					processingStep->addQueuedConceptItem(conceptItem);
					return true;
				}
				return false;
			}




			CVariableBindingsAnswersResult* addVariableBindingAnswerToResult(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingsListAnswerResult* bindAns, CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping, CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, cint64 cardinality = 1) {
				bool filtered = true;
				for (CVariableBindingFiltering* filteringLinker = compAssIndVarQuery->getResultFilteringLinker(); filteringLinker && filtered; filteringLinker = filteringLinker->getNext()) {
					if (!filteringLinker->isFiltered(filteringAnsweringMapping, bindAns)) {
						filtered = false;
					}
				}
				if (filtered) {
					return bindsAnswersResult->addResultVariableBindings(bindAns, cardinality);
				} else {
					delete bindAns;
					return bindsAnswersResult;
				}
			}



			CVariableBindingsAnswersResult* addReusedVariableBindingAnswerToResult(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingsListAnswerResult* bindAns, CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping, CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, cint64 cardinality = 1) {
				bool filtered = true;
				for (CVariableBindingFiltering* filteringLinker = compAssIndVarQuery->getResultFilteringLinker(); filteringLinker && filtered; filteringLinker = filteringLinker->getNext()) {
					if (!filteringLinker->isFiltered(filteringAnsweringMapping, bindAns)) {
						filtered = false;
					}
				}
				if (filtered) {
					return bindsAnswersResult->addReusedResultVariableBindings(bindAns, cardinality);
				} else {
					return bindsAnswersResult;
				}
			}
















			bool COptimizedComplexConceptAnsweringHandler::failQueryProcessing(CComplexQueryProcessingData* queryProcessingData, CQueryError* queryError) {
				CQuery* query = queryProcessingData->getQuery();
				QString errorString = queryError->getErrorString();
				query->addQueryError(queryError);

				queryProcessingData->setProcessingFinished();
				cint64 processingTime = queryProcessingData->getProcessingTime();
				LOG(WARN, getDomain(), logTr("Complex query processing failed after %1 ms due to error '%2'.").arg(processingTime).arg(errorString), this);

				CCallbackData* callback = queryProcessingData->getCallback();
				if (callback) {
					callback->doCallback();
				}
				delete queryProcessingData;
				mOntoAnsweringItem->decCurrentlyAnsweringQueryCount();
				return true;
			}




			bool COptimizedComplexConceptAnsweringHandler::finishQueryProcessingAsUnsatisfaible(CComplexQueryProcessingData* queryProcessingData) {
				CComplexAnsweringQuery* query = queryProcessingData->getQuery();

				queryProcessingData->setProcessingFinished();
				cint64 processingTime = queryProcessingData->getProcessingTime();
				LOG(INFO, getDomain(), logTr("Finished unsatisfiable complex query processing in %1 ms.").arg(processingTime), this);

				CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringQuery*>(query);
				if (compAssIndVarQuery) {
					if (compAssIndVarQuery->isBooleanEntailmentResultRequired()) {
						CBooleanQueryResult* entailedResult = new CBooleanQueryResult(false);
						query->setQueryResult(entailedResult);
					} else {
						QStringList varList;
						QHash<CExpressionVariable*, cint64> disVarIdHash;
						cint64 nextId = 0;
						for (CExpressionVariable* varExp : *compAssIndVarQuery->getDistinguishedVariableExpressions()) {
							disVarIdHash.insert(varExp, nextId++);
							varList.append(varExp->getName());
						}
						CVariableBindingsAnswersResult* bindsAnswersResult = nullptr;
						CVariableBindingsAnswersStreamingResult* existBindsAnswersStreamingResult = dynamic_cast<CVariableBindingsAnswersStreamingResult*>(query->getQueryResult());
						if (existBindsAnswersStreamingResult) {
							existBindsAnswersStreamingResult->initResult(varList);
							bindsAnswersResult = existBindsAnswersStreamingResult;
						} else {
							CVariableBindingsAnswersResult* bindsAnswersResult = new CVariableBindingsAnswersListResult(varList);
							query->setQueryResult(bindsAnswersResult);
						}
					}
				}


				CComplexConceptAnsweringQuery* compConQuery = dynamic_cast<CComplexConceptAnsweringQuery*>(query);
				if (compConQuery) {
					bool flattened = compConQuery->isFlattened();
					if (compConQuery->isSatisfiableComputationRequired()) {
						CBooleanQueryResult* booleanQueryResult = new CBooleanQueryResult(false);
						query->setQueryResult(booleanQueryResult);
					}
					if (compConQuery->isSuperClassNodesComputationRequired() || compConQuery->isSubClassNodesComputationRequired()) {
						CClassSynsetsResult* classSynsetsResult = new CClassSynsetsResult();
						if (compConQuery->isSuperClassNodesComputationRequired()) {
							CHierarchyNode* nextNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getTopHierarchyNode();
							CClassSynsetResult* nextClassSynset = new CClassSynsetResult(nextNode->getEquivalentConceptStringList(false));
							classSynsetsResult->addClassSynset(nextClassSynset);
						}
						query->setQueryResult(classSynsetsResult);
					}
					if (compConQuery->isEquivalentClassNodesComputationRequired()) {
						CClassSetResult* classesResult = new CClassSetResult();
						query->setQueryResult(classesResult);
					}
					if (compConQuery->isInstancesComputationRequired()) {
						if (flattened) {
							CIndividualsResult* individualResult = new CIndividualsResult();
							query->setQueryResult(individualResult);
						} else {
							CIndividualSynsetsResult* indiSynsResult = new CIndividualSynsetsResult();
							query->setQueryResult(indiSynsResult);
						}
					}
				}


				CCallbackData* callback = queryProcessingData->getCallback();
				if (callback) {
					callback->doCallback();
				}
				delete queryProcessingData;
				mOntoAnsweringItem->decCurrentlyAnsweringQueryCount();
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incTotalAnsweredQueriesCount();
				return true;
			}



			

			bool COptimizedComplexConceptAnsweringHandler::finishQueryProcessing(CComplexQueryProcessingData* queryProcessingData) {
				CComplexAnsweringQuery* query = queryProcessingData->getQuery();
				COptimizedComplexConceptItem* conceptItem = queryProcessingData->getConceptItem();
				COptimizedComplexBuildingVariableCompositionsItem* varBuildItem = queryProcessingData->getVariableBuildingItem();

				queryProcessingData->setProcessingFinished();
				cint64 processingTime = queryProcessingData->getProcessingTime();
				LOG(INFO, getDomain(), logTr("Finished complex query processing in %1 ms.").arg(processingTime), this);

				CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringQuery*>(query);
				if (compAssIndVarQuery) {

					if (compAssIndVarQuery->isBooleanEntailmentResultRequired()) {

						CBooleanQueryResult* entailedResult = new CBooleanQueryResult(false);
						if (!queryProcessingData->isUnsatisfiable()) {
							if (varBuildItem && varBuildItem->isSatisfiable()) {

								CExpressionVariable* lastVarExp = varBuildItem->getLastHandledVariableExpression();
								if (!lastVarExp) {
									entailedResult->setResult(true);
								} else {
									COptimizedComplexVariableCompositionItem* varItem = varBuildItem->getVariableLastCompositionItem(lastVarExp);
									if (varItem->getVariableMapping()->size() > 0) {
										entailedResult->setResult(true);
									}
								}

							}
						}
						query->setQueryResult(entailedResult);

					} else {

						bool abbreviatedIRIs = false;
						QStringList varList;
						QHash<CExpressionVariable*, cint64> disVarIdHash;
						cint64 nextId = 0;
						for (CExpressionVariable* varExp : *compAssIndVarQuery->getDistinguishedVariableExpressions()) {
							disVarIdHash.insert(varExp, nextId++);
							varList.append(varExp->getName());
						}

						CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping = nullptr;
						CVariableBindingFilteringAnswerMapping* lastFilteringAnsweringMapping = nullptr;
						for (CVariableBindingFiltering* filteringLinker = compAssIndVarQuery->getResultFilteringLinker(); filteringLinker; filteringLinker = filteringLinker->getNext()) {
							CVariableBindingFilteringAnswerMapping* tmpFilteringAnsweringMapping = filteringLinker->createFitleringAnswerMapping(disVarIdHash);
							if (!filteringAnsweringMapping) {
								lastFilteringAnsweringMapping = filteringAnsweringMapping = tmpFilteringAnsweringMapping;
							} else {
								lastFilteringAnsweringMapping->append(tmpFilteringAnsweringMapping);
								lastFilteringAnsweringMapping = tmpFilteringAnsweringMapping;
							}
						}

						cint64 variablesCount = 1;
						if (varBuildItem) {
							variablesCount = varBuildItem->getAllVariableSet()->count();
						}
						bool distinct = compAssIndVarQuery->isDistinctRequired();

						
						CVariableBindingsAnswersResult* bindsAnswersResult = nullptr;
						CVariableBindingsAnswersStreamingResult* existBindsAnswersStreamingResult = dynamic_cast<CVariableBindingsAnswersStreamingResult*>(query->getQueryResult());
						if (existBindsAnswersStreamingResult) {
							existBindsAnswersStreamingResult->initResult(varList);
							bindsAnswersResult = existBindsAnswersStreamingResult;
						} else {
							if (compAssIndVarQuery->getResultOrderingLinker()) {
								cint64 orderingVarCount = compAssIndVarQuery->getResultOrderingLinker()->getCount();
								bool mappingDistinct = distinct && varList.count() != variablesCount;
								CVariableBindingsAnswerResultMapOrdering* ordering = new CVariableBindingsAnswerResultMapOrdering(orderingVarCount);
								ordering->setDistinct(mappingDistinct);
								cint64 orderingVarNum = 0;
								for (CVariableBindingOrdering* orderingIt = compAssIndVarQuery->getResultOrderingLinker(); orderingIt; orderingIt = orderingIt->getNext()) {
									CExpressionVariable* orderingVarExp = orderingIt->getOrderingVariableExpression();
									CVariableBindingOrdering::ORDERING_TYPE orderingType = orderingIt->getOrderingType();
									cint64 orderingVarId = disVarIdHash.value(orderingVarExp);
									ordering->setAscending(orderingVarNum, orderingType == CVariableBindingOrdering::ASC);
									ordering->setVariableIndex(orderingVarNum, orderingVarId);
									++orderingVarNum;
								}
								bindsAnswersResult = new CVariableBindingsAnswersOrderedMapResult(varList, ordering);
							} else {
								if (distinct) {
									bindsAnswersResult = new CVariableBindingsAnswersSetResult(varList);
								} else {
									bindsAnswersResult = new CVariableBindingsAnswersListResult(varList);
								}
							}
						}
						if (!queryProcessingData->isUnsatisfiable()) {
							if (varBuildItem && varBuildItem->isSatisfiable()) {
								CExpressionVariable* lastVarExp = varBuildItem->getLastHandledVariableExpression();
								if (!lastVarExp) {
									CVariableBindingsListAnswerResult* bindAnsRes = new CVariableBindingsListAnswerResult();
									for (CExpressionVariable* varExp : *compAssIndVarQuery->getDistinguishedVariableExpressions()) {
										QString varName = varExp->getName();
										QString anonymousBindingName = varName.replace("?", "").replace("$", "") + "_1";
										CVariableBindingStringResult* bindingResult = new CVariableBindingStringResult(anonymousBindingName, CVariableBindingResult::VBTANONYMOUSINDIVIDUAL);
										bindAnsRes->addResultVariableBinding(bindingResult);
									}
									addVariableBindingAnswerToResult(bindsAnswersResult, bindAnsRes, filteringAnsweringMapping, compAssIndVarQuery);
								} else {
									COptimizedComplexVariableCompositionItem* varItem = varBuildItem->getVariableLastCompositionItem(lastVarExp);
									COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = varBuildItem->getVariableItemIndexMapping(lastVarExp).value(varItem);
									QHash<CExpressionVariable*, cint64> varIdxHash;

									QSet<CExpressionVariable*> distinguishedAnswerVariableSet(compAssIndVarQuery->getDistinguishedVariableExpressions()->toSet());
									QList<cint64> nonAnswerVariableMappingIndexList;
									for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
										cint64 varIdx = it.key();
										CExpressionVariable* varExp = it.value();
										if (distinguishedAnswerVariableSet.contains(varExp)) {
											varIdxHash.insert(varExp, varIdx);
										} else {
											nonAnswerVariableMappingIndexList.append(varIdx);
										}
									}

									QList<cint64> answerDistinguishedVariableMappingIndexList;
									for (CExpressionVariable* varExp : *compAssIndVarQuery->getDistinguishedVariableExpressions()) {
										cint64 varIdx = varIdxHash.value(varExp, -1);
										if (varIdx >= 0) {
											answerDistinguishedVariableMappingIndexList.append(varIdx);
										}
									}


									CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();


									//debugCheckVariableMappingFromFile(varBuildItem, varItem);
									//debugWriteVariableMappingToFile(varBuildItem, varItem);

									cint64 answerVarCount = answerDistinguishedVariableMappingIndexList.size();

									class CVariableBindingResultVector : public QVector<CVariableBindingResult*> {
									public:
										cint64 mValidBindingCount = 0;
										cint64 mCurrentBindingIdx = 0;
										CVariableBindingResultVector* mNextIteratingVector = nullptr;
										cint64 mVectorPos = 0;
									};
									CVariableBindingResultVector** bindingResultVectorArray = new CVariableBindingResultVector*[answerVarCount];
									for (cint64 i = 0; i < answerVarCount; ++i) {
										bindingResultVectorArray[i] = new CVariableBindingResultVector();
										bindingResultVectorArray[i]->mVectorPos = i;
									}

									CVariableBindingsListAnswerResult bindResListAnswer(answerVarCount);

									CIndividualNameResolver* indiNameResolver = mOntoAnsweringItem->getOntology()->getIndividualNameResolver();
									COptimizedComplexVariableIndividualMapping* varMapping = varItem->getVariableMapping();

									cint64 explicitMaximalCardinality = 0;
									cint64 explicitTotalCardinality = 0;

									cint64 implicitMaximalCardinality = 0;
									cint64 implicitTotalCardinality = 0;

									cint64 answerNr = 0;
									for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = varMapping->getLastAddedBindingsCardinalityLinker(); linker; linker = linker->getNext()) {
										COptimizedComplexVariableIndividualBindings* bindings = linker->getBindings();
										COptimizedComplexVariableIndividualBindingsCardinality* cardinalities = linker->getCardinalities();

										answerNr++;

										for (cint64 i = 0; i < answerVarCount; ++i) {
											bindingResultVectorArray[i]->mValidBindingCount = 0;
											bindingResultVectorArray[i]->mCurrentBindingIdx = 0;
											bindingResultVectorArray[i]->mNextIteratingVector = nullptr;
										}

										cint64 cardinality = 1;
										if (!distinct) {
											if (cardinalities) {
												cardinality *= cardinalities->getSameIndividualsSeparatlyConsideredCardinality();
											}
											// determine additional cardinality of binding result due to same individuals bound to non-distinguished variables
											for (cint64 varIdx : nonAnswerVariableMappingIndexList) {
												COptimizedComplexVariableIndividualMapping::VARIABLE_TYPE bindingType = varMapping->getBindingMapping(varIdx);
												if (bindingType == COptimizedComplexVariableIndividualMapping::INDIVIDUAL_VARIABLE) {
													CInstanceBindingIndividualCountingVisitor visitor;
													sameRealization->visitSameIndividuals(bindings->getBinding(varIdx).reference, &visitor);
													if (visitor.individualCount > 1) {
														cardinality *= visitor.individualCount;
													}
												}
											}
										}

										implicitMaximalCardinality = qMax(implicitMaximalCardinality, cardinality);
										implicitTotalCardinality += cardinality;

										cint64 vectorIdx = 0;
										for (cint64 varIdx : answerDistinguishedVariableMappingIndexList) {
											COptimizedComplexVariableIndividualMapping::VARIABLE_TYPE bindingType = varMapping->getBindingMapping(varIdx);
											if (bindingType == COptimizedComplexVariableIndividualMapping::INDIVIDUAL_VARIABLE) {

												CInstanceBindingIndividualLambdaCallingVisitor visitor([&](const CIndividualReference& indiRef) -> bool {
													QString indiName = indiNameResolver->getIndividualName(indiRef);
													if (!indiName.isEmpty()) {
														CVariableBindingResultVector* bindingResultVector = bindingResultVectorArray[vectorIdx];
														if (bindingResultVector->size() <= bindingResultVector->mValidBindingCount) {
															bindingResultVector->resize(bindingResultVector->mValidBindingCount + 5);
														}
														CVariableBindingResult*& binding = (*bindingResultVector)[bindingResultVector->mValidBindingCount++];
														if (!binding) {
															binding = createVariableBindingResult(indiRef, compAssIndVarQuery, indiName);
														} else {
															binding->initVariableBinding(indiRef, indiName);
														}
													}
													return true;
												});
												sameRealization->visitSameIndividuals(bindings->getBinding(varIdx).reference, &visitor);
												vectorIdx++;
												
											} else if (bindingType == COptimizedComplexVariableIndividualMapping::DATA_LITERAL_VARIABLE) {

												TIndividualInstanceItemDataBinding& varBinding = bindings->getBinding(varIdx);
												CDataLiteral* dataLiteral = (CDataLiteral*)varBinding.pointer;

												CVariableBindingResultVector* bindingResultVector = bindingResultVectorArray[vectorIdx];
												if (bindingResultVector->size() <= bindingResultVector->mValidBindingCount) {
													bindingResultVector->resize(bindingResultVector->mValidBindingCount + 5);
												}
												CVariableBindingResult*& binding = (*bindingResultVector)[bindingResultVector->mValidBindingCount++];
												if (!binding) {
													binding = createVariableBindingResult(dataLiteral, compAssIndVarQuery);
												} else {
													binding->initVariableBinding(dataLiteral);
												}
												vectorIdx++;

											}
										}


										CVariableBindingResultVector* firstIteratingBindingVec = nullptr;
										CVariableBindingResultVector* lastIteratingBindingVec = nullptr;
										for (cint64 i = 0; i < answerVarCount; ++i) {
											CVariableBindingResultVector* bindingVec = bindingResultVectorArray[i];
											if (bindingVec->mValidBindingCount > 1) {
												if (lastIteratingBindingVec) {
													lastIteratingBindingVec->mNextIteratingVector = bindingVec;
												}
												if (firstIteratingBindingVec == nullptr) {
													firstIteratingBindingVec = bindingVec;
												}
												lastIteratingBindingVec = bindingVec;
											}
											bindResListAnswer.setResult(i, (*bindingVec)[bindingVec->mCurrentBindingIdx]);
										}

										addReusedVariableBindingAnswerToResult(bindsAnswersResult, &bindResListAnswer, filteringAnsweringMapping, compAssIndVarQuery, cardinality);

										cint64 explicitCardinality = 1;

										bool isValidBindingCombination = true;
										while (isValidBindingCombination && firstIteratingBindingVec) {

											firstIteratingBindingVec->mCurrentBindingIdx++;

											if (firstIteratingBindingVec->mCurrentBindingIdx >= firstIteratingBindingVec->mValidBindingCount) {
												firstIteratingBindingVec->mCurrentBindingIdx = 0;
												CVariableBindingResultVector* overflowIteratingBindingVec = firstIteratingBindingVec->mNextIteratingVector;
												if (!overflowIteratingBindingVec) {
													isValidBindingCombination = false;
												}
												while (overflowIteratingBindingVec) {
													overflowIteratingBindingVec->mCurrentBindingIdx++;
													CVariableBindingResultVector* nextOverflowIteratingBindingVec = nullptr;
													if (overflowIteratingBindingVec->mCurrentBindingIdx >= overflowIteratingBindingVec->mValidBindingCount) {
														overflowIteratingBindingVec->mCurrentBindingIdx = 0;
														nextOverflowIteratingBindingVec = overflowIteratingBindingVec->mNextIteratingVector;
														if (!nextOverflowIteratingBindingVec) {
															isValidBindingCombination = false;
														}
													}
													if (isValidBindingCombination) {
														bindResListAnswer.setResult(overflowIteratingBindingVec->mVectorPos, (*overflowIteratingBindingVec)[overflowIteratingBindingVec->mCurrentBindingIdx]);
													}
													overflowIteratingBindingVec = nextOverflowIteratingBindingVec;
												}
											}
											if (isValidBindingCombination) {
												bindResListAnswer.setResult(firstIteratingBindingVec->mVectorPos, (*firstIteratingBindingVec)[firstIteratingBindingVec->mCurrentBindingIdx]);
												addReusedVariableBindingAnswerToResult(bindsAnswersResult, &bindResListAnswer, filteringAnsweringMapping, compAssIndVarQuery, cardinality);
												explicitCardinality++;
											}

										}

										explicitMaximalCardinality = qMax(explicitMaximalCardinality, explicitCardinality);
										explicitTotalCardinality += explicitCardinality;

									}

									delete[] bindingResultVectorArray;

									if (mConfExtendedLogging) {
										cint64 mappingCount = varMapping->size();
										LOG(INFO, getDomain(), logTr("%2 mappings extended to %1 answers with average cardinality of %3 (maximum %4) and, in average, %5 (maximum %6) replicated answers due to same individuals.")
											.arg(bindsAnswersResult->getResultCount()).arg(mappingCount)
											.arg(QString::number(implicitTotalCardinality / (double)mappingCount)).arg(implicitMaximalCardinality)
											.arg(QString::number(explicitTotalCardinality / (double)mappingCount)).arg(explicitMaximalCardinality)
											, this);
									}

								}


							

							} else if (conceptItem) {
								CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
								CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
								QSet<CRealizationIndividualInstanceItemReference>* instanceItemSet = conceptItem->getKnownInstanceItemSet();

								class CInstanceBindingAddingVisitor : public CSameRealizationIndividualVisitor, public CConceptRealizationInstanceToIndividualVisitor {
								public:
									CInstanceBindingAddingVisitor(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping, CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, CIndividualNameResolver* indiNameResolver) {
										mCompAssIndVarQuery = compAssIndVarQuery;
										mBindsAnswersResult = bindsAnswersResult;
										mFilteringAnsweringMapping = filteringAnsweringMapping;
										mIndiNameResolver = indiNameResolver;
									}

									virtual bool visitIndividual(const CIndividualReference& indiRef, CConceptRealization* conRealization) {
										return visitIndividual(indiRef);
									}

									virtual bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealizatio) {
										return visitIndividual(indiRef);
									}


									bool visitIndividual(const CIndividualReference& indiRef) {
										CVariableBindingsListAnswerResult* answerResult = new CVariableBindingsListAnswerResult();
										CVariableBindingResult* binding = createVariableBindingResult(indiRef, mCompAssIndVarQuery, mIndiNameResolver);
										answerResult->addResultVariableBinding(binding);
										addVariableBindingAnswerToResult(mBindsAnswersResult, answerResult, mFilteringAnsweringMapping, mCompAssIndVarQuery);
										return true;
									}
									

									CComplexAssertionsIndividualVariablesAnsweringQuery* mCompAssIndVarQuery;
									CVariableBindingsAnswersResult* mBindsAnswersResult;
									CVariableBindingFilteringAnswerMapping* mFilteringAnsweringMapping;
									CIndividualNameResolver* mIndiNameResolver;

								} resultGenerator(bindsAnswersResult, filteringAnsweringMapping, compAssIndVarQuery, mOntoAnsweringItem->getOntology()->getIndividualNameResolver());
								if (instanceItemSet) {
									for (auto item : *instanceItemSet) {
										sameRealization->visitSameIndividuals(item, &resultGenerator);
									}
								}
								//for (auto node : *conceptItem->getDirectSubClassNodeSet()) {
								//	conceptRealization->visitInstances(node->getOneEquivalentConcept(), false, &resultGenerator);
								//}
							
							}
						}



						//QSet<QString> indi1Set;
						//QSet<QString> indi2Set;
						//QFile file1("./Tests/sparql-response.xml");
						//cint64 firstMissingIndiID1 = -1;
						//cint64 firstMissingIndiID2 = -1;
						//if (file1.open(QIODevice::ReadOnly)) {
						//	while (!file1.atEnd()) {
						//		QString line = file1.readLine().trimmed();
						//		if (line.startsWith("<uri>")) {
						//			QString indiName = line.mid(5, line.length() - 11);
						//			indi1Set.insert(indiName);
						//		}
						//	}

						//	auto it = bindsAnswersResult->getVariableBindingsAnswersIterator();
						//	while (it->hasNext()) {
						//		auto binding = it->getNext();
						//		auto bindIt = binding->getVariableBindingsIterator();
						//		auto bindingData = bindIt->getNext();
						//		QString bindingString = bindingData->getBindingString();
						//		indi2Set.insert(bindingString);
						//		if (!indi1Set.contains(bindingString) && firstMissingIndiID2 < 0) {
						//			CVariableBindingStringDataResult* dataResult = dynamic_cast<CVariableBindingStringDataResult*>(bindingData);
						//			firstMissingIndiID2 = (cint64)dataResult->getData();
						//		}
						//	}


						//	QStringList additionalIndis2;
						//	QStringList additionalIndis1;
						//	for (QString indi2 : indi2Set) {
						//		if (!indi1Set.contains(indi2)) {
						//			additionalIndis2.append(indi2);
						//			bool debug = true;
						//		}
						//	}

						//	for (QString indi1 : indi1Set) {
						//		if (!indi2Set.contains(indi1)) {
						//			if (firstMissingIndiID1 < 0) {
						//				firstMissingIndiID1 = mOntoAnsweringItem->getOntology()->getStringMapping()->getIndividualFromName(indi1)->getIndividualID();
						//			}
						//			additionalIndis1.append(indi1);
						//			bool debug = true;
						//		}
						//	}

						//	if (!additionalIndis1.isEmpty() || !additionalIndis2.isEmpty()) {
						//		bool debug = true;

						//		QHash< TConceptNegPair, COptimizedComplexConceptItem* >* conItemHash = mOntoAnsweringItem->getComplexConceptNegationItemHash();
						//		for (COptimizedComplexConceptItem* conItem : *conItemHash) {
						//			if (conItem->getKnownInstanceItemSet() && conItem->getKnownInstanceItemSet()->size() >= 1400 && conItem->getKnownInstanceItemSet()->size() <= 2000) {

						//				bool abc = true;
						//				while (abc) {
						//					initializeRealizationInstancesIterators(conItem, 1, mAnswererContext);
						//					auto realIt = conItem->getRealizationIterator();
						//					realIt->begin();
						//					cint64 currentId = realIt->currentIndividualInstanceItemReference().getIndividualID();

						//					while (currentId < firstMissingIndiID1 && !realIt->atEnd()) {
						//						auto copyIt = realIt;
						//						currentId = realIt->currentIndividualInstanceItemReference().getIndividualID();
						//						if (currentId <= firstMissingIndiID1) {
						//							copyIt = realIt->getCopy();
						//						}
						//						realIt->moveNext();
						//						cint64 nextId = realIt->currentIndividualInstanceItemReference().getIndividualID();
						//						if (nextId > firstMissingIndiID1) {
						//							copyIt->moveNext();
						//						}
						//					}

						//				}

						//			}
						//		}
						//	}

						//}

						LOG(INFO, getDomain(), logTr("Determined %1 answers for complex ABox query with %2 answer variables (%3).").arg(bindsAnswersResult->getResultCount()).arg(bindsAnswersResult->getVariableNames().count()).arg(bindsAnswersResult->getVariableNames().join(", ")), this);



						if (!existBindsAnswersStreamingResult) {
							query->setQueryResult(bindsAnswersResult);
						}
						for (CVariableBindingFilteringAnswerMapping* filteringAnsweringMappingIt = filteringAnsweringMapping; filteringAnsweringMappingIt; ) {
							CVariableBindingFilteringAnswerMapping* tmpFilteringAnsweringMapping = filteringAnsweringMappingIt;
							filteringAnsweringMappingIt = filteringAnsweringMappingIt->getNext();
							delete tmpFilteringAnsweringMapping;
						}

					}
				}


				CComplexConceptAnsweringQuery* compConQuery = dynamic_cast<CComplexConceptAnsweringQuery*>(query);
				if (conceptItem && compConQuery) {
					bool direct = compConQuery->isDirect();
					bool flattened = compConQuery->isFlattened();
					if (compConQuery->isSatisfiableComputationRequired()) {
						bool satisfiable = conceptItem->isSatisfiable();
						CBooleanQueryResult* booleanQueryResult = new CBooleanQueryResult(satisfiable);
						query->setQueryResult(booleanQueryResult);
					}
					if (compConQuery->isSuperClassNodesComputationRequired() || compConQuery->isSubClassNodesComputationRequired()) {
						CClassSynsetsResult* classSynsetsResult = new CClassSynsetsResult();
						bool subClasses = compConQuery->isSubClassNodesComputationRequired();
						bool superClasses = compConQuery->isSuperClassNodesComputationRequired();
						bool abbreviatedIRIs = false;


						QSet<CHierarchyNode*>* visitedHierNodeSet = nullptr;
						if (!direct) {
							visitedHierNodeSet = new QSet<CHierarchyNode*>();
						}
						QList<CHierarchyNode*> visitHierNodetList;
						if (subClasses) {
							QSet<CHierarchyNode*>* childNodeSet = conceptItem->getDirectSubClassNodeSet();
							for (QSet<CHierarchyNode*>::const_iterator it = childNodeSet->constBegin(), itEnd = childNodeSet->constEnd(); it != itEnd; ++it) {
								CHierarchyNode* childNode(*it);
								visitHierNodetList.append(childNode);
							}
						}
						if (superClasses) {
							QSet<CHierarchyNode*>* parentNodeSet = conceptItem->getDirectSuperClassNodeSet();
							for (QSet<CHierarchyNode*>::const_iterator it = parentNodeSet->constBegin(), itEnd = parentNodeSet->constEnd(); it != itEnd; ++it) {
								CHierarchyNode* parentNode(*it);
								visitHierNodetList.append(parentNode);
							}
						}

						while (!visitHierNodetList.isEmpty()) {
							CHierarchyNode* nextNode = visitHierNodetList.takeFirst();

							CClassSynsetResult* nextClassSynset = new CClassSynsetResult(nextNode->getEquivalentConceptStringList(abbreviatedIRIs));
							classSynsetsResult->addClassSynset(nextClassSynset);


							if (!direct) {
								if (subClasses) {
									QSet<CHierarchyNode*>* tmpChildNodeSet = nextNode->getChildNodeSet();
									for (QSet<CHierarchyNode*>::const_iterator it = tmpChildNodeSet->constBegin(), itEnd = tmpChildNodeSet->constEnd(); it != itEnd; ++it) {
										CHierarchyNode* nextChildNode(*it);
										if (!visitedHierNodeSet->contains(nextChildNode)) {
											visitedHierNodeSet->insert(nextChildNode);
											visitHierNodetList.append(nextChildNode);
										}
									}
								}
								if (superClasses) {
									QSet<CHierarchyNode*>* tmpParentNodeSet = nextNode->getParentNodeSet();
									for (QSet<CHierarchyNode*>::const_iterator it = tmpParentNodeSet->constBegin(), itEnd = tmpParentNodeSet->constEnd(); it != itEnd; ++it) {
										CHierarchyNode* nextParentNode(*it);
										if (!visitedHierNodeSet->contains(nextParentNode)) {
											visitedHierNodeSet->insert(nextParentNode);
											visitHierNodetList.append(nextParentNode);
										}
									}
								}
							}
						}
						delete visitedHierNodeSet;
						query->setQueryResult(classSynsetsResult);
					}
					if (compConQuery->isEquivalentClassNodesComputationRequired()) {
						CClassSetResult* classesResult = new CClassSetResult();
						QSet<CHierarchyNode*>* childNodeSet = conceptItem->getDirectSubClassNodeSet();
						QSet<CHierarchyNode*>* parentNodeSet = conceptItem->getDirectSuperClassNodeSet();
						bool abbreviatedIRIs = false;
						if (parentNodeSet && childNodeSet && parentNodeSet->size() == 1 && childNodeSet->size() == 1) {
							CHierarchyNode* node = (*childNodeSet->constBegin());
							if (parentNodeSet->contains(node)) {
								QStringList eqClassNameList(node->getEquivalentConceptStringList(abbreviatedIRIs));
								for (QStringList::const_iterator it = eqClassNameList.constBegin(), itEnd = eqClassNameList.constEnd(); it != itEnd; ++it) {
									classesResult->addClass(*it);
								}
							}
						}
						query->setQueryResult(classesResult);
					}
					if (compConQuery->isInstancesComputationRequired()) {
						CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
						CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
						QSet<CRealizationIndividualInstanceItemReference>* instanceItemSet = conceptItem->getKnownInstanceItemSet();
						bool abbreviatedIRIs = false;


						if (direct) {
							// TODO: make more efficient
							QSet<CRealizationIndividualInstanceItemReference>* directInstanceItemSet = new QSet<CRealizationIndividualInstanceItemReference>(*instanceItemSet);
							for (auto node : *conceptItem->getDirectSubClassNodeSet()) {
								CRealizationIndividualInstanceItemReferenceIterator* instanceIt = conceptRealization->getConceptInstancesIterator(node->getOneEquivalentConcept(), false, mDefaultRealizationSorting);
								instanceIt->begin();
								while (!instanceIt->atEnd()) {
									CRealizationIndividualInstanceItemReference itemRef = instanceIt->currentIndividualInstanceItemReference();
									directInstanceItemSet->remove(itemRef);
									instanceIt->moveNext();
								}
								delete instanceIt;
							}
							instanceItemSet = directInstanceItemSet;
						}


						if (flattened) {
							CIndividualsResult* individualResult = new CIndividualsResult();
							CIndividualsResultVisitorGenerator resultGenerator(individualResult, abbreviatedIRIs, mOntoAnsweringItem->getOntology()->getIndividualNameResolver());
							if (instanceItemSet) {
								for (auto item : *instanceItemSet) {
									sameRealization->visitSameIndividuals(item, &resultGenerator);
								}
							}
							query->setQueryResult(individualResult);
						} else {
							CIndividualSynsetsResult* indiSynsResult = new CIndividualSynsetsResult();
							if (instanceItemSet) {
								for (auto item : *instanceItemSet) {
									CIndividualSynsetResult* individualSynsetResult = new CIndividualSynsetResult();
									CIndividualSynsetResultVisitorGenerator resultGenerator(individualSynsetResult, abbreviatedIRIs, mOntoAnsweringItem->getOntology()->getIndividualNameResolver());
									sameRealization->visitSameIndividuals(item, &resultGenerator);
									indiSynsResult->addIndividualSynset(individualSynsetResult);
								}
							}
							query->setQueryResult(indiSynsResult);
						}

						if (direct) {
							delete instanceItemSet;
						}

					}
				}
				CCallbackData* callback = queryProcessingData->getCallback();
				if (callback) {
					callback->doCallback();
				}
				delete queryProcessingData;
				mOntoAnsweringItem->decCurrentlyAnsweringQueryCount();
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incTotalAnsweredQueriesCount();
				return true;
			}









































			bool COptimizedComplexConceptAnsweringHandler::continueCalculationCreation(CAnswererContext* answererContext) {
				bool processing = false;
				COptimizedComplexConceptStepAnsweringItem* conSatProcStepItem = mOntoAnsweringItem->getConceptSatisfiabilityProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* conSuperProcStepItem = mOntoAnsweringItem->getConceptSuperClassesProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* conSubProcStepItem = mOntoAnsweringItem->getConceptSubClassesProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* conSubRealStepItem = mOntoAnsweringItem->getConceptSubClassesRealizationProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* conEqProcStepItem = mOntoAnsweringItem->getConceptEquivalentClassesProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* instancesProcStepItem = mOntoAnsweringItem->getConceptInstancesProcessingStepItem();
				QList<COptimizedComplexBuildingVariableCompositionsItem*>* buildingVarItemList = mOntoAnsweringItem->getVariableBuildingItemProcessingList();
				while (!processing && (conSatProcStepItem->hasConceptItemsQueued() || conSuperProcStepItem->hasConceptItemsQueued() || conSubProcStepItem->hasConceptItemsQueued() || conEqProcStepItem->hasConceptItemsQueued() || instancesProcStepItem->hasConceptItemsQueued() || !buildingVarItemList->isEmpty())) {
					// satisfiability
					if (!processing && conSatProcStepItem->hasConceptItemsQueued()) {
						COptimizedComplexConceptItem* conceptItem = conSatProcStepItem->takeNextQueuedConceptItem();
						CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSatisfiableClassNodesComputationProcess(true);
						compStep->setComputationProcessQueued(false);
						if (!compStep->isComputationProcessFinished()) {
							if (searchSuperSubSatisfiabilityResult(conceptItem, answererContext)) {
							} else if (createSatisfiabilityTest(conceptItem, answererContext)) {
								processing = true;
								compStep->setComputationProcessStarted(true);
								compStep->setComputationProcessProcessing(true);
								compStep->incCurrentlyRunningComputationCount();
							}
						}
						if (!processing) {
							finishCalculationStepProcessing(conceptItem, compStep, answererContext);
						}
					}			
					// super classes
					if (!processing && conSuperProcStepItem->hasConceptItemsQueued()) {
						COptimizedComplexConceptItem* conceptItem = conSuperProcStepItem->getNextQueuedConceptItem();
						CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSuperClassNodesComputationProcess(true);
						processing = false;
						if (!compStep->isComputationProcessFinished()) {
							QList<CHierarchyNode*>* possibleClassNodeTestingList = conceptItem->getPossibleSuperClassNodeTestingList();
							if (!possibleClassNodeTestingList && !compStep->isComputationProcessStarted()) {
								compStep->setComputationProcessStarted(true);

								searchSuperSubSuperClassesResult(conceptItem, answererContext);

								QSet<CConcept*>* knowSuperConceptSet = conceptItem->getKnownSuperConceptSet();
								QSet<CConcept*>* possibleSuperConceptSet = conceptItem->getPossibleSuperConceptSet();
								QSet<CHierarchyNode*>* knownSuperClassesSet = conceptItem->getKnownSuperClassNodeSet();
								QSet<CHierarchyNode*>* possibleSuperClassesSet = conceptItem->getPossibleSuperClassNodeSet();


								if (knowSuperConceptSet && possibleSuperConceptSet || knownSuperClassesSet && possibleSuperClassesSet) {
									conceptItem->setSuperClassComputationInitializing(false);
								} else {
									conceptItem->setSuperClassComputationInitializing(true);
									if (createSatisfiabilityTest(conceptItem, answererContext)) {
										processing = true;
										compStep->setComputationProcessProcessing(true);
										compStep->incCurrentlyRunningComputationCount();
									}
								}
							}

							if (!possibleClassNodeTestingList && !conceptItem->isSuperClassComputationInitializing()) {

								QSet<CConcept*>* knowSuperConceptSet = conceptItem->getKnownSuperConceptSet();
								if (knowSuperConceptSet) {
									CClassConceptClassification* classClassification = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification();
									CTaxonomy* taxonomy = classClassification->getClassConceptTaxonomy();
									QSet<CHierarchyNode*>* knownSuperClassesSet = conceptItem->getKnownSuperClassNodeSet();
									if (!knownSuperClassesSet) {
										knownSuperClassesSet = new QSet<CHierarchyNode*>();
										conceptItem->setKnownSuperClassNodeSet(knownSuperClassesSet);
									}
									for (QSet<CConcept*>::const_iterator it = knowSuperConceptSet->constBegin(), itEnd = knowSuperConceptSet->constEnd(); it != itEnd; ++it) {
										CConcept* knownSuperClass(*it);
										CHierarchyNode* hierNode = taxonomy->getHierarchyNode(knownSuperClass, false);
										knownSuperClassesSet->insert(hierNode);
									}
									delete knowSuperConceptSet;
									conceptItem->setKnownSuperConceptSet(nullptr);
								}

								QSet<CConcept*>* possibleSuperConceptSet = conceptItem->getPossibleSuperConceptSet();
								if (possibleSuperConceptSet) {
									CClassConceptClassification* classClassification = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification();
									CTaxonomy* taxonomy = classClassification->getClassConceptTaxonomy();
									QSet<CHierarchyNode*>* knownSuperClassesSet = conceptItem->getKnownSuperClassNodeSet();
									QSet<CHierarchyNode*>* possibleSuperClassesSet = new QSet<CHierarchyNode*>();
									for (QSet<CConcept*>::const_iterator it = possibleSuperConceptSet->constBegin(), itEnd = possibleSuperConceptSet->constEnd(); it != itEnd; ++it) {
										CConcept* possibleSuperClass(*it);
										CHierarchyNode* hierNode = taxonomy->getHierarchyNode(possibleSuperClass, false);
										if (!knownSuperClassesSet->contains(hierNode)) {
											possibleSuperClassesSet->insert(hierNode);
										}
									}
									if (conceptItem->getPossibleSuperClassNodeSet()) {
										conceptItem->getPossibleSuperClassNodeSet()->intersect(*possibleSuperClassesSet);
										delete possibleSuperClassesSet;
									} else {
										conceptItem->setPossibleSuperClassNodeSet(possibleSuperClassesSet);
									}
									delete possibleSuperConceptSet;
									conceptItem->setPossibleSuperConceptSet(nullptr);
								}


								QSet<CHierarchyNode*>* possibleSuperClassNodeSet = conceptItem->getPossibleSuperClassNodeSet();
								if (!possibleSuperClassNodeSet || possibleSuperClassNodeSet->isEmpty()) {
									QSet<CHierarchyNode*>* knownSuperClassSet = conceptItem->getKnownSuperClassNodeSet();
									if (knownSuperClassSet) {
										QSet<CHierarchyNode*>* directSuperClassSet = getDirectReducedSuperNodeSet(*knownSuperClassSet);
										conceptItem->setKnownSuperClassNodeSet(nullptr);
										delete knownSuperClassSet;
										conceptItem->setDirectSuperClassNodeSet(directSuperClassSet);
										delete possibleSuperClassNodeSet;
										conceptItem->setPossibleSuperClassNodeSet(nullptr);
									}
									finishCalculationStepProcessing(conceptItem, compStep, answererContext);
								} else {
									possibleClassNodeTestingList = new QList<CHierarchyNode *>();
									for (QSet<CHierarchyNode*>::const_iterator it = possibleSuperClassNodeSet->constBegin(), itEnd = possibleSuperClassNodeSet->constEnd(); it != itEnd; ++it) {
										CHierarchyNode* hierNode(*it);
										possibleClassNodeTestingList->append(hierNode);
									}
									qSort(possibleClassNodeTestingList->begin(), possibleClassNodeTestingList->end(), nodeSortMorePredecessorsThan);
									conceptItem->setPossibleSuperClassNodeTestingList(possibleClassNodeTestingList);
									delete possibleSuperClassNodeSet;
									conceptItem->setPossibleSuperClassNodeSet(nullptr);
								}
							}
							if (possibleClassNodeTestingList && !possibleClassNodeTestingList->isEmpty()) {
								CHierarchyNode* testingNode = possibleClassNodeTestingList->takeFirst();
								if (createSuperClassSubsumptionTest(conceptItem, testingNode, answererContext)) {
									processing = true;
									compStep->setComputationProcessProcessing(true);
									compStep->incCurrentlyRunningComputationCount();
								}
							} 
							if (!possibleClassNodeTestingList || possibleClassNodeTestingList->isEmpty()) {
								compStep->setComputationProcessQueued(false);
								conSuperProcStepItem->takeNextQueuedConceptItem();
							}
						}
					}

					// sub classes
					if (!processing && conSubProcStepItem->hasConceptItemsQueued()) {
						COptimizedComplexConceptItem* conceptItem = conSubProcStepItem->getNextQueuedConceptItem();
						CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSubClassNodesComputationProcess(true);
						processing = false;
						if (!compStep->isComputationProcessFinished()) {
							if (!compStep->isComputationProcessStarted()) {
								compStep->setComputationProcessStarted(true);

								searchSuperSubSubClassesResult(conceptItem, answererContext);
							}

							QList<CHierarchyNode*>* possibleClassNodeTestingList = conceptItem->getPossibleSubClassNodeTestingList();
							if (possibleClassNodeTestingList && !possibleClassNodeTestingList->isEmpty()) {
								CHierarchyNode* testingNode = possibleClassNodeTestingList->takeFirst();
								QSet<CHierarchyNode*>* maxSubNodeSet = conceptItem->getMaximumSubClassNodeSet();
								if (maxSubNodeSet && maxSubNodeSet->contains(testingNode)) {
									conceptItem->getDirectSubClassNodeSet()->insert(testingNode);
								} else if (createSubClassSubsumptionTest(conceptItem, testingNode, answererContext)) {
									processing = true;
									compStep->setComputationProcessProcessing(true);
									compStep->incCurrentlyRunningComputationCount();
								}
							}
							if (!possibleClassNodeTestingList || possibleClassNodeTestingList->isEmpty()) {
								compStep->setComputationProcessQueued(false);
								conSubProcStepItem->takeNextQueuedConceptItem();
								if (compStep->getCurrentlyRunningComputationCount() == 0) {
									if (!conceptItem->getPossibleSubClassNodeTestingList() || conceptItem->getPossibleSubClassNodeTestingList()->isEmpty()) {
										finishSubClassCalculationStepProcessing(conceptItem, compStep, answererContext);
									}
								}
							}
						}
					}


					if (!processing && conSubRealStepItem->hasConceptItemsQueued()) {
						COptimizedComplexConceptItem* conceptItem = conSubRealStepItem->getNextQueuedConceptItem();
						CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSubClassRealizationProcess(true);
						CHierarchyNode* bottomHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getBottomHierarchyNode();

						if (!compStep->isComputationProcessFinished()) {
							if (!compStep->isComputationProcessStarted()) {
								compStep->setComputationProcessStarted(true);

								QList<COntologyProcessingRequirement*> reqList;
								for (CHierarchyNode* node : *conceptItem->getDirectSubClassNodeSet()) {
									if (node != bottomHierNode) {
										COntologyProcessingConceptRealizationRequirement* conRealReq = new COntologyProcessingConceptRealizationRequirement(CIndividualReference(), node->getOneEquivalentConcept());
										reqList.append(conRealReq);
									}
								}

								if (!reqList.isEmpty()) {
									CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(conceptItem);
									processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
									processing = true;
								} else {
									finishCalculationStepProcessing(conceptItem, compStep, answererContext);
								}
							}
						}

					}

					// equivalent classes
					if (!processing && conEqProcStepItem->hasConceptItemsQueued()) {
						COptimizedComplexConceptItem* conceptItem = conEqProcStepItem->takeNextQueuedConceptItem();
						CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getEquivalentClassNodesComputationProcess(true);
						compStep->setComputationProcessQueued(false);
						if (!compStep->isComputationProcessStarted()) {
							compStep->setComputationProcessStarted(true);
						}
						if (!processing) {
							finishCalculationStepProcessing(conceptItem, compStep, answererContext);
						}
					}


					// instances
					if (!processing && instancesProcStepItem->hasConceptItemsQueued()) {
						COptimizedComplexConceptItem* conceptItem = instancesProcStepItem->getNextQueuedConceptItem();
						CComplexConceptStepInstanceComputationProcess* compStep = conceptItem->getComputationProcess()->getInstancesComputationProcess(true);
						processing = false;
						if (!compStep->isComputationProcessFinished() || compStep->requiresAdditionalInstanceComputation()) {



							if (!conceptItem->isLazyRealizationInitialized()) {
								conceptItem->setLazyRealizationInitialized(true);
								if (initializeRealization(conceptItem, answererContext)) {
									processing = true;
									compStep->setComputationProcessQueued(false);
									instancesProcStepItem->takeNextQueuedConceptItem();
								}
							}
							
							cint64 newRequiredCandidateCount = 0;
							if (compStep->requiresAllInstanceComputation()) {
								newRequiredCandidateCount = -1;
							} else {
								// subtract the eliminated candidates, i.e., those instance candidates that are confirmed non-instances
								cint64 possibleTestedNonInstanceCount = conceptItem->getPossibleTestedNonInstanceCount();
								cint64 realizationRetrievedInstanceCandidateCount = conceptItem->getRealizationRetrievedInstanceCandidateCount();
								cint64 requiredInstancesCount = compStep->getRequiredInstancesCount();
								newRequiredCandidateCount = requiredInstancesCount - realizationRetrievedInstanceCandidateCount + possibleTestedNonInstanceCount;
								if (newRequiredCandidateCount < 0) {
									newRequiredCandidateCount = 0;
								}
							}


							if (conceptItem->isLazyRealizationInitialized() && !conceptItem->isLazyRealizationProcessing() && !compStep->isComputationProcessStarted()) {
								compStep->setComputationProcessStarted(true);

								if (mConfExtendedLogging) {
									LOG(INFO, getDomain(), logTr("Starting instance computation for complex concept item %1.").arg(conceptItem->getConceptItemId()), this);
								}
								

								if (initializeRealizationInstancesIterators(conceptItem, newRequiredCandidateCount, answererContext)) {
									processing = true;
									compStep->setComputationProcessQueued(false);
									instancesProcStepItem->takeNextQueuedConceptItem();
								}
							}



							if (!processing && !conceptItem->isLazyRealizationProcessing() && !conceptItem->isCompletelyHandledChecked()) {
								conceptItem->setCompletelyHandledChecked(true);
								checkPossibleInstancesCompletelyHandled(conceptItem, answererContext);
							}


							if (!processing && compStep->isComputationProcessStarted() && !conceptItem->isLazyRealizationProcessing() && !conceptItem->isLazyRealizationInstancesRetrieved()) {

								QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();

								if ((!possibleInstanceItemSet || possibleInstanceItemSet->isEmpty() || conceptItem->isLazyRealizationInitializationRequested()) && newRequiredCandidateCount != 0) {
									conceptItem->setLazyRealizationInitializationRequested(false);

									cint64 retrievingCount = newRequiredCandidateCount;
									double minRetrievingSize = conceptItem->getMinimalRetrievingInstanceItemSize();
									if (retrievingCount != -1) {
										minRetrievingSize = qMin(minRetrievingSize, mConfConceptItemInstanceCandiateRetrievingMaxSize);
										retrievingCount = qMax(retrievingCount, (cint64)minRetrievingSize);
									}

									if (initializeKnownPossibleInstances(conceptItem, retrievingCount, answererContext)) {
										conceptItem->setLazyRealizationInitializationRequested(true);
										processing = true;
										compStep->setComputationProcessQueued(false);
										instancesProcStepItem->takeNextQueuedConceptItem();
									} else {

										if (retrievingCount != -1 && minRetrievingSize < mConfConceptItemInstanceCandiateRetrievingMaxSize) {
											minRetrievingSize *= mConfConceptItemInstanceCandiateRetrievingSizeIncreaseFactor;
											conceptItem->setMinimalRetrievingInstanceItemSize(minRetrievingSize);
										}

										if (mConfExtendedLogging) {

											cint64 newCertainInstances = conceptItem->getLastRetrievedCertainInstanceItemCount();
											cint64 possibleInstances = 0;
											QString requestedCountString = QString::number(retrievingCount);
											if (newRequiredCandidateCount == -1) {
												requestedCountString = QString("all");
											}
											if (conceptItem->getPossibleInstanceItemSet()) {
												possibleInstances = conceptItem->getPossibleInstanceItemSet()->size();
											}
											LOG(INFO, getDomain(), logTr("Retrieved %2 certain and %3 possible of %4 requested instances for complex concept item %1.").arg(conceptItem->getConceptItemId()).arg(newCertainInstances).arg(possibleInstances).arg(requestedCountString), this);
										}

										conceptItem->setCompletelyHandledChecked(false);
										conceptItem->setCandidatePropagated(false);
									}
								}
							}



							//if (!processing && !conceptItem->isLazyRealizationProcessing() && !compStep->isComputationProcessStarted()) {
							//	compStep->setComputationProcessStarted(true);

							//	searchKnownPossibleInstances(conceptItem, answererContext);
							//}


							//if (!processing && !conceptItem->isLazyRealizationProcessing() && !conceptItem->isLazyNeighbourRealizationRequested()) {
							//	conceptItem->setLazyNeighbourRealizationRequested(true);
							//	if (schedulePossibleInstancesNeighourRealization(conceptItem, answererContext)) {
							//		processing = true;
							//		compStep->setComputationProcessQueued(false);
							//		instancesProcStepItem->takeNextQueuedConceptItem();
							//	}
							//}


							//if (!processing && !conceptItem->isLazyRealizationProcessing() && conceptItem->isLazyNeighbourRealizationPruningRequired()) {
							//	conceptItem->setLazyNeighbourRealizationPruningRequired(false);
							//	neighboursPrunePossibleInstances(conceptItem, answererContext);
							//}





							if (!processing && !conceptItem->isLazyRealizationProcessing() && !conceptItem->isConceptAbsorbed()) {
								QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
								if (possibleInstanceItemSet && !possibleInstanceItemSet->isEmpty()) {
									CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology();

									CConcept* reducedConcept = conceptItem->getCompletelyHandledReducedInstanceTestingConcept();
									bool reducedNegation = conceptItem->getCompletelyHandledReducedInstanceTestingConceptNegation();
									CBOXLIST<TConceptNegPair>* conceptOfInterestList = testingOnto->getTBox()->getConceptOfInterestList(true);
									conceptOfInterestList->append(TConceptNegPair(reducedConcept, reducedNegation));
									mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());
									conceptItem->setConceptAbsorbed(true);
								}
							}


							if (!processing && !conceptItem->isLazyRealizationProcessing() && conceptItem->isConceptAbsorbed() && !conceptItem->isCandidatePropagated()) {
								QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
								if (possibleInstanceItemSet && !possibleInstanceItemSet->isEmpty()) {

									CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology();
									CBOXHASH<TConceptNegPair, CConceptOfInterestActivationTriggeringData>* activationTriggeringDataHash = testingOnto->getTBox()->getConceptOfInterestActivationTriggerDataHash(false);
									if (activationTriggeringDataHash) {
										CConcept* reducedConcept = conceptItem->getCompletelyHandledReducedInstanceTestingConcept();
										bool reducedNegation = conceptItem->getCompletelyHandledReducedInstanceTestingConceptNegation();

										CConceptOfInterestActivationTriggeringData activationTriggeringData = activationTriggeringDataHash->value(TConceptNegPair(reducedConcept, reducedNegation));
										CConcept* candidateConcept = activationTriggeringData.getCandidateConcept();
										CConcept* triggeringConcept = activationTriggeringData.getTriggeringConcept();

										if (triggeringConcept && !conceptItem->isLazyRealizationInstancesRetrieved()) {

											bool createOnePassPropagationTest = true;
											for (CConceptOfInterestActivationLinker* activationLinkerIt = activationTriggeringData.getActivationLinker(); activationLinkerIt; activationLinkerIt = activationLinkerIt->getNext()) {
												if (!activationLinkerIt->getActivationIndividual()) {
													createOnePassPropagationTest = false;
												}
											}


											if (createOnePassPropagationTest) {
												compStep->setAllInstanceComputationRequired(true);
												if (initializeKnownPossibleInstances(conceptItem, -1, answererContext)) {
													processing = true;
													compStep->setComputationProcessQueued(false);
													instancesProcStepItem->takeNextQueuedConceptItem();
												}
											}
										}


										if (!processing && (candidateConcept || triggeringConcept)) {



											if (createCandidatePropagationInstanceTest(conceptItem, activationTriggeringData, answererContext)) {
												processing = true;
												compStep->setComputationProcessProcessing(true);
												compStep->incCurrentlyRunningComputationCount();
												conceptItem->incCandidatePropagationProcessingCount();
												instancesProcStepItem->takeNextQueuedConceptItem();
											}
										}
									}


									conceptItem->setCandidatePropagated(true);
								}
							}



							if (!processing && !conceptItem->isLazyRealizationProcessing() && !conceptItem->isCandidatePropagationProcessing()) {


								QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
								if (possibleInstanceItemSet && !possibleInstanceItemSet->isEmpty()) {
									QSet<CRealizationIndividualInstanceItemReference>::iterator it = possibleInstanceItemSet->begin();
									CRealizationIndividualInstanceItemReference testingItem(*it);
									it = possibleInstanceItemSet->erase(it);
									if (createInvidiualItemInstanceTest(conceptItem, testingItem, answererContext)) {
										processing = true;
										compStep->setComputationProcessProcessing(true);
										compStep->incCurrentlyRunningComputationCount();
									}
								}
								if (!possibleInstanceItemSet || possibleInstanceItemSet->isEmpty()) {

									compStep->setComputationProcessQueued(false);
									instancesProcStepItem->takeNextQueuedConceptItem();
									if (compStep->getCurrentlyRunningComputationCount() == 0) {


										if (mConfExtendedLogging) {
											cint64 knownInstancesCount = 0;
											if (conceptItem->getKnownInstanceItemSet()) {
												knownInstancesCount = conceptItem->getKnownInstanceItemSet()->size();
											}
											LOG(INFO, getDomain(), logTr("Determined %2 known instances for complex concept item %1.").arg(conceptItem->getConceptItemId()).arg(knownInstancesCount), this);
										}


										conceptItem->setLastRetrievedCertainInstanceItemCount(conceptItem->getKnownInstanceItemSet()->size());
										updateComputedInstancesCount(conceptItem, compStep, answererContext);

										if (conceptItem->isLazyRealizationInstancesRetrieved()) {
											delete conceptItem->getPossibleInstanceItemSet();
											conceptItem->setPossibleInstanceItemSet(nullptr);
											finishCalculationStepProcessing(conceptItem, compStep, answererContext);
										} else {
											compStep->setComputationProcessQueued(false);
											conceptItem->setQueuedProcessStep(nullptr);
										}
									}

								}
							}
						}
					}














					// variable items
					if (!processing && !buildingVarItemList->isEmpty()) {
						bool continueProcessingBuildingVarItem = false;
						COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem = buildingVarItemList->first();






						QList<COptimizedComplexVariableCompositionItem*>* compositionVarItemList = buildingVarItem->getComputeVariableMappingItemList();
						// variable composition items
						if (!processing && !compositionVarItemList->isEmpty()) {
							COptimizedComplexVariableCompositionItem* compVarItem = compositionVarItemList->takeFirst();
							if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::CONCEPT_BASE) {
								computeVariableCompositionItemFromConceptItemBase(compVarItem, buildingVarItem);


							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::BINDING_REDUCTION) {
								computeVariableCompositionItemReduction(compVarItem, buildingVarItem);



							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::JOINING) {
								computeVariableCompositionItemJoin(compVarItem, buildingVarItem, processing);


							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::ROLE_PROPAGATION || compVarItem->getCompositionType() == COptimizedComplexVariableRolePropagationJoiningItem::ROLE_PROPAGATION_JOIN || compVarItem->getCompositionType() == COptimizedComplexVariableRolePropagationReplacementItem::ROLE_PROPAGATION_REPLACEMENT) {
								computeVariableCompositionItemPropagation(compVarItem, buildingVarItem, answererContext, processing);



							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::DATA_LITERAL_BASE) {
								computeVariableCompositionItemFromDataLiteralBase(compVarItem, answererContext, buildingVarItem);


							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::ABSOROPTION_BASED_EXTENSION) {
								computeVariableCompositionItemAbsorptionBasedExtension(compVarItem, processing, buildingVarItem, answererContext, continueProcessingBuildingVarItem);



							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::DATA_LITERAL_EXTENSION) {
								computeVariableCompositionItemDataLiteralExtension(compVarItem, answererContext, buildingVarItem);

							}

						}

























						// building variable items
						if (!processing && compositionVarItemList->isEmpty()) {

							QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
							QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
							QSet<CIndividualVariableExpression*>* reuseVarExpComputationsCheckSet = buildingVarItem->getReuseVariableExpressionComputationsCheckSet();

							if (!buildingVarItem->isInitializedBaseConceptVariableItems()) {

								for (QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>::const_iterator it = varConItemHash->constBegin(), itEnd = varConItemHash->constEnd(); it != itEnd && buildingVarItem->isSatisfiable(); ++it) {
									CIndividualVariableExpression* varExp = it.key();
									COptimizedComplexConceptItem* conItem = it.value();
									if (!conItem->getKnownInstanceItemSet() || conItem->getKnownInstanceItemSet()->isEmpty()) {
										buildingVarItem->setSatisfiability(false);
										break;
									}
								}

								if (buildingVarItem->isSatisfiable()) {

									for (QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>::const_iterator it = varConItemHash->constBegin(), itEnd = varConItemHash->constEnd(); it != itEnd && buildingVarItem->isSatisfiable(); ++it) {
										CIndividualVariableExpression* varExp = it.key();
										COptimizedComplexConceptItem* conItem = it.value();


										if (!conItem->isLazyRealizationInstancesRetrieved()) {
											CComplexConceptStepInstanceComputationProcess* instComStep = conItem->getComputationProcess()->getInstancesComputationProcess(true);
											instComStep->setAllInstanceComputationRequired(true);
											processing |= initializeQueryProcessingStep(instComStep, conItem, nullptr, buildingVarItem);
										}


										COptimizedComplexVariableConceptBaseItem* varConBaseItem = conItem->getVariableConceptBaseItem();
										if (!varConBaseItem) {
											varConBaseItem = new COptimizedComplexVariableConceptBaseItem(conItem);
											conItem->setVariableConceptBaseItem(varConBaseItem);
											reuseVarExpComputationsCheckSet->remove(varExp);
										}
										if (!varConBaseItem->isVariableMappingComputed()) {

											COptimizedComplexVariableIndividualMapping* varMapping = new COptimizedComplexVariableIndividualMapping(1);
											varMapping->setBindingMapping(0, COptimizedComplexVariableIndividualMapping::INDIVIDUAL_VARIABLE);
											varConBaseItem->setVariableMapping(varMapping);

											queueVariableItemComputation(buildingVarItem, varConBaseItem);
											processing = true;
										}

										COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);
										COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
										varItemIndexMapping.insert(varConBaseItem, itemIndexMapping);
										itemIndexMapping->insert(0, varExp);

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
										QStringList debugVariableNameStringList = debugGetItemVariableNames(buildingVarItem, varExp, varConBaseItem);
										varConBaseItem->debugVariableNameStringList = debugVariableNameStringList;
										varConBaseItem->debugCreationString = "[" + debugVariableNameStringList.join(",")+"] initialized";
										debugCreationStringList += varConBaseItem->debugCreationString;
	#endif

										QSet<COptimizedComplexVariableCompositionItem*>* varConBaseItemSet = new QSet<COptimizedComplexVariableCompositionItem*>();
										varConBaseItemSet->insert(varConBaseItem);
										varExpVarComItemHash->insert(varExp, varConBaseItemSet);


										buildingVarItem->setLastHandledVariableExpression(varExp);
										buildingVarItem->updateLastHandledVariableItemAssociation(varExp, varConBaseItem);
									}












									while (!reuseVarExpComputationsCheckSet->isEmpty()) {

										QSet<CIndividualVariableExpression*>::iterator it = reuseVarExpComputationsCheckSet->begin();
										CIndividualVariableExpression* varExp = *it;
										reuseVarExpComputationsCheckSet->erase(it);

										COptimizedComplexVariableCompositionItem* varCompItem = nullptr;

										// join items
										QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(varExp);
										if (joiningVarItemSet->size() == 1) {
											varCompItem = *joiningVarItemSet->constBegin();
										} else {
											QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> joinedVarCompItems = reuseJoinedVariableCompositionItems(buildingVarItem, varExp);
											if (joinedVarCompItems.size() == 1) {
												varCompItem = joinedVarCompItems.first().first;
											}
										}

										if (varCompItem) {
											QList<CObjectPropertyAssertionExpression*> propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(varExp);
											cint64 reusedCount = 0;
											for (CObjectPropertyAssertionExpression* propAss : propAssList) {

												if (tryReuseVariableRolePropagationItem(buildingVarItem, varExp, varCompItem, nullptr, propAss)) {
													if (propAssList.size()-reusedCount == 1 || !buildingVarItem->isRemovingPropertyAssertionDisconnecting(varExp, propAss)) {
														reusedCount++;
														buildingVarItem->setPropertyAssertionHandled(propAss);
														reuseVarExpComputationsCheckSet->insert(buildingVarItem->getOtherVariableExpression(varExp, propAss));
														buildingVarItem->setVariableExpressionUnhandled(buildingVarItem->getOtherVariableExpression(varExp, propAss));
													}
												}
											}
											if (reusedCount == propAssList.size()) {
												buildingVarItem->setVariableExpressionHandled(varExp);
												buildingVarItem->setLastHandledVariableExpression(varExp);
											}

										}

									}


									buildingVarItem->setInitializedBaseConceptVariableItems(true);
								}
							}







							// initialize basic data literals items
							if (!processing && compositionVarItemList->isEmpty()) {


								QList<CDataPropertyAssertionExpression*>* basicDataPropAssList = buildingVarItem->getBasicDataPropertyAssertionList();

								QHash<CDataLiteralVariableExpression*, CIndividualVariableExpression*>* dataVarIndiVarHash = buildingVarItem->getDataLiteralVariableIndidualVariableMappingHash();
								QSet<CIndividualVariableExpression*>* remainingDataVarExpSet = buildingVarItem->getRemainingExtendingDataVariableSet();

								if (!buildingVarItem->isInitializedBasicDataVariableItems() && buildingVarItem->isSatisfiable()) {

									for (CDataPropertyAssertionExpression* dataPropAssExp : *basicDataPropAssList) {
										CDataLiteralVariableExpression* dataVarExp = (CDataLiteralVariableExpression*)dataPropAssExp->getDataLiteralTermExpression();
										CIndividualTermExpression* indiTermExp = dataPropAssExp->getIndividualTermExpression();
										CDataPropertyTermExpression* dataPropertyExp = dataPropAssExp->getDataPropertyTermExpression();


										CRole* dataRole = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash()->value(dataPropertyExp);
										CIndividual* indi = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(indiTermExp);


										COptimizedComplexVariableDataLiteralBaseItem* dataLiteralBaseItem = new COptimizedComplexVariableDataLiteralBaseItem(indi, dataRole);
										COptimizedComplexVariableIndividualMapping* varMapping = new COptimizedComplexVariableIndividualMapping(1);
										varMapping->setBindingMapping(0, COptimizedComplexVariableIndividualMapping::DATA_LITERAL_VARIABLE);
										dataLiteralBaseItem->setVariableMapping(varMapping);

										queueVariableItemComputation(buildingVarItem, dataLiteralBaseItem);
										processing = true;


										COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(dataVarExp);
										COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
										varItemIndexMapping.insert(dataLiteralBaseItem, itemIndexMapping);
										itemIndexMapping->insert(0, dataVarExp);

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
										QStringList debugVariableNameStringList = debugGetItemVariableNames(buildingVarItem, dataVarExp, dataLiteralBaseItem);
										dataLiteralBaseItem->debugVariableNameStringList = debugVariableNameStringList;
										dataLiteralBaseItem->debugCreationString = "[" + debugVariableNameStringList.join(",") + "] initialized";
										debugCreationStringList += dataLiteralBaseItem->debugCreationString;
#endif
										QSet<COptimizedComplexVariableCompositionItem*>* varConBaseItemSet = new QSet<COptimizedComplexVariableCompositionItem*>();
										varConBaseItemSet->insert(dataLiteralBaseItem);
										varExpVarComItemHash->insert(dataVarExp, varConBaseItemSet);

										buildingVarItem->setLastHandledVariableExpression(dataVarExp);
										buildingVarItem->updateLastHandledVariableItemAssociation(dataVarExp, dataLiteralBaseItem);

									}

									buildingVarItem->setInitializedBasicDataVariableItems(true);
								}

								basicDataPropAssList->clear();
							}






							// data literals extension of variable items 
							if (!processing && compositionVarItemList->isEmpty()) {


								QHash<CDataLiteralVariableExpression*, CIndividualVariableExpression*>* dataVarIndiVarHash = buildingVarItem->getDataLiteralVariableIndidualVariableMappingHash();
								QSet<CIndividualVariableExpression*>* remainingDataVarExpSet = buildingVarItem->getRemainingExtendingDataVariableSet();

								if (!buildingVarItem->isInitializedConceptDataVariableExtensionItems() && buildingVarItem->isSatisfiable()) {

									for (CIndividualVariableExpression* indiVarExp : *remainingDataVarExpSet) {
										QList<CDataPropertyAssertionExpression*> unDataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(indiVarExp);


										QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
										QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
										COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(indiVarExp);
										COptimizedComplexVariableCompositionItem* varCompItem = *varExpVarComItemHash->value(indiVarExp)->begin();
										COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = varItemIndexMapping.value(varCompItem);


										for (CDataPropertyAssertionExpression* dataPropAss : unDataPropAssList) {
											CDataLiteralVariableExpression* dataVarExp = (CDataLiteralVariableExpression*)dataPropAss->getDataLiteralTermExpression();
											CDataPropertyTermExpression* dataPropExp = dataPropAss->getDataPropertyTermExpression();

											buildVariableRolePropagationItem(buildingVarItem, indiVarExp, varCompItem, itemIndexMapping, dataVarExp, dataPropExp, &processing, answererContext);

											if (dataVarIndiVarHash->count(dataVarExp) <= 1) {
												buildingVarItem->setDataPropertyAssertionHandled(dataPropAss);
											}
										}
									}

									buildingVarItem->setInitializedConceptDataVariableExtensionItems(true);
								}



							}




							// joining basic data literals items and combining them with data extension items
							if (!processing && compositionVarItemList->isEmpty()) {


								QSet<CDataLiteralVariableExpression*>* basicDataVarJoiningSet = buildingVarItem->getBasicDataVariableJoiningSet();
								if (!buildingVarItem->isJoinedBasicDataVariableItems() && buildingVarItem->isSatisfiable()) {

									for (CDataLiteralVariableExpression* dataVarExp : *basicDataVarJoiningSet) {
										QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(dataVarExp);
										COptimizedComplexVariableCompositionItem* dataVarCompItem = *joiningVarItemSet->constBegin();
										if (joiningVarItemSet->size() > 1) {
											dataVarCompItem = buildJoinedVariableJoiningItems(buildingVarItem, dataVarExp, &processing).first;
										}

										QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(dataVarExp);

										for (QList<CDataPropertyAssertionExpression*>::iterator it = dataPropAssList.begin(), itEnd = dataPropAssList.end(); it != itEnd; ++it) {
											CDataPropertyAssertionExpression* dataPropAssExp(*it);

											CIndividualVariableExpression* indiVar = (CIndividualVariableExpression*)dataPropAssExp->getIndividualTermExpression();
											COptimizedComplexVariableCompositionItem* indiVarCompItem = *varExpVarComItemHash->value(indiVar)->begin();
											buildVariableJoiningItem(buildingVarItem, indiVar, false, indiVar, indiVarCompItem, dataVarExp, dataVarCompItem, nullptr, nullptr, &processing);

										}
									}
									buildingVarItem->setJoinedBasicDataVariableItems(true);
								}
							}







							QSet<CIndividualVariableExpression*>* remainVarExpSet = buildingVarItem->getRemainingVariableExpressionSet();
							if (!processing && !remainVarExpSet->isEmpty() && buildingVarItem->isSatisfiable() && (!buildingVarItem->isWaitingComputation() || continueProcessingBuildingVarItem)) {

								//TODO: sort remainVarExpList by number of instances
								QList<CIndividualVariableExpression*> remainVarExpList = getSortedRemainingVariableExpressionList(buildingVarItem, remainVarExpSet);

								while (!remainVarExpList.isEmpty() && !processing) {
									CIndividualVariableExpression* varExp = remainVarExpList.takeFirst();

									COptimizedComplexVariableCompositionItem* varCompItem = nullptr;
									// join items
									QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(varExp);
									if (joiningVarItemSet->size() == 1) {
										varCompItem = *joiningVarItemSet->constBegin();
									} else {
										varCompItem = buildJoinedVariableJoiningItems(buildingVarItem, varExp, &processing).first;
									}


									cint64 propagatedCount = 0;
									QList<CObjectPropertyAssertionExpression*> propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(varExp);
									QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(varExp);
									if (!processing) {

										if (!dataPropAssList.isEmpty() && propAssList.isEmpty()) {
											for (QList<CDataPropertyAssertionExpression*>::iterator it = dataPropAssList.begin(), itEnd = dataPropAssList.end(); it != itEnd; ++it) {
												CDataPropertyAssertionExpression* dataPropAssExp(*it);
												QList<CIndividualVariableExpression*> indiBackDataPropAssList = buildingVarItem->getUnhandledDataExtensionIndividualTermExpressions(dataPropAssExp,varExp);
												if (indiBackDataPropAssList.size() <= 0) {
													buildingVarItem->setDataPropertyAssertionHandled(dataPropAssExp);
												} else if (indiBackDataPropAssList.size() == 1 || !buildingVarItem->isRemovingDataPropertyAssertionDisconnecting(varExp, dataPropAssExp)) {

													CIndividualVariableExpression* connIndiVarExp = *indiBackDataPropAssList.begin();
													COptimizedComplexVariableCompositionItem* connVarCompItem = *varExpVarComItemHash->value(connIndiVarExp)->begin();

													buildVariableJoiningItem(buildingVarItem, connIndiVarExp, false, varExp, varCompItem, connIndiVarExp, connVarCompItem, nullptr, nullptr, &processing);

													propagatedCount++;
													buildingVarItem->setDataPropertyAssertionHandled(dataPropAssExp);
													buildingVarItem->setVariableExpressionUnhandled(connIndiVarExp);
												}
											}

										} else {

											for (CObjectPropertyAssertionExpression* propAss : propAssList) {
												if (propAssList.size() - propagatedCount == 1 || !buildingVarItem->isRemovingPropertyAssertionDisconnecting(varExp, propAss)) {

													QHash<CExpressionVariable*, cint64> reductionVarExpIndHash = getReducableVariables(buildingVarItem, varExp, varCompItem, nullptr, propAss);
													varCompItem = buildVariableReductionItem(buildingVarItem, varExp, varCompItem, nullptr, reductionVarExpIndHash, &processing).first;

													buildVariableRolePropagationItem(buildingVarItem, varExp, varCompItem, nullptr, reductionVarExpIndHash, propAss, &processing, answererContext);
													propagatedCount++;
													buildingVarItem->setPropertyAssertionHandled(propAss);
													buildingVarItem->setVariableExpressionUnhandled(buildingVarItem->getOtherVariableExpression(varExp, propAss));
												}
											}
										}
									}

									if (propagatedCount == propAssList.size()+ dataPropAssList.size()) {
										buildingVarItem->setVariableExpressionHandled(varExp);
										buildingVarItem->setLastHandledVariableExpression(varExp);
										if (!processing) {
											remainVarExpList = getSortedRemainingVariableExpressionList(buildingVarItem, remainVarExpSet);
										}
									}

								}
							}





							QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* absorbedQueryPartsItemExtensionList = buildingVarItem->getAbsorbedQueryPartItemExtensionHandlingList();
							if (!processing && absorbedQueryPartsItemExtensionList && !absorbedQueryPartsItemExtensionList->isEmpty() && buildingVarItem->isSatisfiable()) {

								COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart = absorbedQueryPartsItemExtensionList->takeFirst();
								COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem = new COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem(absorptionHanldingQueryPart);
								

								QSet<CExpressionVariable*>* preparationVarSet = absorptionHanldingQueryPart->getPrepareVariableSet();
								QSet<CExpressionVariable*>* indiVarSet = absorptionHanldingQueryPart->getIndividualBindingsVariableSet();

								QSet<CExpressionVariable*> useVarSet(*preparationVarSet);
								useVarSet = useVarSet.unite(*indiVarSet);

								QList<CExpressionVariable*> indiIntVarList = indiVarSet->toList();
								CExpressionVariable* initializationVar = absorptionHanldingQueryPart->getInitializerVariableExpression();
								if (!useVarSet.contains(initializationVar)) {
									indiIntVarList.append(initializationVar);
								}

								QHash<CExpressionVariable*, CVariable*>* variableExpressionVariableHash = absorptionHanldingQueryPart->getVariableExpressionVariableHash();


								CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
								QList<CIndividualReference> indiList;


								QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* propagationFinalizationConceptAbsorptionItemHash = mOntoAnsweringItem->getPropagationFinalizationConceptAbsorptionItemHash();
								COptimizedComplexVariableCompositionItemVariableIndexMapping* extendItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
								cint64 nextVariableIdx = 0;
								COptimizedComplexVariableIndividualMapping* newPropVarMapping = new COptimizedComplexVariableIndividualMapping(indiVarSet->size());

								QHash<CVariable*, cint64>* variableIndexHash = absorptionPropagationItem->getVariableIndexHash();

								QSet<COptimizedComplexVariableCompositionItem*>* integratedItemSet = absorptionPropagationItem->getIntegratedVariableCompositionItemSet();

								CExpressionVariable* initializingVarExp = absorptionHanldingQueryPart->getInitializerVariableExpression();

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
								QStringList prevItemsStringList;
#endif

								for (CExpressionVariable* indiVarExp : useVarSet) {
									COptimizedComplexVariableCompositionItem* varCompItem = buildingVarItem->getVariableLastCompositionItem(indiVarExp);
									CExpressionVariable* associatedVariableExpression = buildingVarItem->getVariableLastCompositionItemAssociatedVariableExpression(indiVarExp);

									if (!integratedItemSet->contains(varCompItem) && indiVarSet->contains(indiVarExp)) {
										integratedItemSet->insert(varCompItem);
									}

									CVariable* variable = variableExpressionVariableHash->value(indiVarExp);
									absorptionPropagationItem->setVariableCompositionItem(variable, varCompItem);
									absorptionPropagationItem->setVariableExpressionCompositionItem(indiVarExp, varCompItem);

									QSet<CIndividualReference>*& variableSteeringIndividualBinding = absorptionPropagationItem->getVariableSteeringIndividualBindingSet(variable);
									if (!variableSteeringIndividualBinding) {
										variableSteeringIndividualBinding = new QSet<CIndividualReference>();
									}

									propagationFinalizationConceptAbsorptionItemHash->insert(absorptionHanldingQueryPart->getPropagationFinalizationConcept(), absorptionPropagationItem);


									// TODO: use latest item for variable to identify allowed bindings
									COptimizedComplexVariableCompositionItemVariableIndexMapping* varIndexMapping = buildingVarItem->getVariableItemIndexMapping(associatedVariableExpression).value(varCompItem);
									cint64 varPos = -1;
									if (varIndexMapping) {
										for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = varIndexMapping->constBegin(), itEnd = varIndexMapping->constEnd(); it != itEnd && varPos < 0; ++it) {
											if (it.value() == indiVarExp) {
												varPos = it.key();
											}
										}
									}
									if (varPos >= 0) {
										for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinkerIt = varCompItem->getVariableMapping()->getLastAddedBindingsCardinalityLinker(); bindingLinkerIt; bindingLinkerIt = bindingLinkerIt->getNext()) {
											COptimizedComplexVariableIndividualBindings* bindings = bindingLinkerIt->getBindings();
											TIndividualInstanceItemDataBinding& instItem = bindings->getBinding(varPos);

											indiList.clear();
											CInstanceBindingIndividualCollectionVisitor visitor(&indiList);
											sameRealization->visitSameIndividuals(instItem.reference, &visitor);

											for (CIndividualReference indiRef : indiList) {
												variableSteeringIndividualBinding->insert(indiRef);
											}
										}
									}

									COptimizedComplexVariableIndividualMapping* varMapping = varCompItem->getVariableMapping();
									if (varMapping->size() > 0) {
										if (indiVarSet->contains(indiVarExp)) {
											variableIndexHash->insertMulti(variable, nextVariableIdx);
											newPropVarMapping->setBindingMapping(nextVariableIdx, varMapping->getBindingMapping(varPos));
											extendItemIndexMapping->insert(nextVariableIdx++, indiVarExp);
										}
									} else {
										buildingVarItem->setSatisfiability(false);
									}


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
									QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, associatedVariableExpression, varCompItem);
									prevItemsStringList += "[" + debugVariableNamePrevItemStringList.join(",") + "]";
#endif

								}
								absorptionPropagationItem->setVariableMapping(newPropVarMapping);

								COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(initializingVarExp);
								varItemIndexMapping.insert(absorptionPropagationItem, extendItemIndexMapping);
								varExpVarComItemHash->value(initializingVarExp)->insert(absorptionPropagationItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
								QStringList debugVariableNameNewItemStringList = debugGetItemVariableNames(buildingVarItem, initializingVarExp, absorptionPropagationItem);
								absorptionPropagationItem->debugVariableNameStringList = debugVariableNameNewItemStringList;
								absorptionPropagationItem->debugCreationString = "[" + debugVariableNameNewItemStringList.join(",") + "] absorption propagation based on { " + prevItemsStringList.join(" }, { ") + " } ";
								debugCreationStringList += absorptionPropagationItem->debugCreationString;
#endif



								if (buildingVarItem->isSatisfiable()) {


									if (absorptionHanldingQueryPart->hasIndividualVariables()) {
										if (mConfExtendedLogging) {
											LOG(INFO, getDomain(), logTr("Scheduling marker propagation for absorbed query part."), this);
										}
										processing = createVariableBindingPropagationTest(absorptionPropagationItem, buildingVarItem, answererContext);
									} else {
										if (mConfExtendedLogging) {
											LOG(INFO, getDomain(), logTr("Testing entailment with prepared upper bounds for absorbed query part."), this);
										}
										processing = createAbsorbedQueryPartEntailmentTest(absorptionPropagationItem, buildingVarItem, answererContext);
									}
								}


							}




							QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* absorptionHanldingQueryExtensionJoiningList = buildingVarItem->getAbsorptionBasedHandlingExtensionItemList();
							if (!processing && absorptionHanldingQueryExtensionJoiningList && !absorptionHanldingQueryExtensionJoiningList->isEmpty() && buildingVarItem->isSatisfiable()) {

								COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem = absorptionHanldingQueryExtensionJoiningList->takeFirst();
								COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHandlingData = absorptionPropagationItem->getAbsorptionBasedHandlingData();
								CExpressionVariable* initializerVariableExp = absorptionHandlingData->getInitializerVariableExpression();


								QList< QPair<COptimizedComplexVariableCompositionItem*, CExpressionVariable*> >* variableExtendingItemList = absorptionPropagationItem->getExtendingCompositionItemAssociatedVariablePairList();
								QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();

								// TODO: check, may has to be extended/adapted for cases where same item is used for several variables


								QSet<CExpressionVariable*>* indiVarSet = absorptionHandlingData->getIndividualVariableSet();
								QSet<COptimizedComplexVariableCompositionItem*> integratedItemSet;

								for (CExpressionVariable* indiVarExp : *indiVarSet) {
									COptimizedComplexVariableCompositionItem* varCompItem = buildingVarItem->getVariableLastCompositionItem(indiVarExp);
									CExpressionVariable* associatedVariableExpression = buildingVarItem->getVariableLastCompositionItemAssociatedVariableExpression(indiVarExp);

									if (!integratedItemSet.contains(varCompItem)) {
										integratedItemSet.insert(varCompItem);
										if (varCompItem->getVariableMapping()->getBindingSize() > 1) {
											variableExtendingItemList->append(QPair<COptimizedComplexVariableCompositionItem*, CExpressionVariable*>(varCompItem, associatedVariableExpression));
										}
									}


								}



								COptimizedComplexVariableCompositionItem* lastCompItem = absorptionPropagationItem;
								CExpressionVariable* lastVariableExp = initializerVariableExp;
								for (QPair<COptimizedComplexVariableCompositionItem*, CExpressionVariable*> extendingItemAssociatedVariablePair : *variableExtendingItemList) {
									COptimizedComplexVariableCompositionItem* extendingItem = extendingItemAssociatedVariablePair.first;
									CExpressionVariable* associatedVariableExpression = extendingItemAssociatedVariablePair.second;
									lastCompItem = buildVariableJoiningItem(buildingVarItem, initializerVariableExp, false, lastVariableExp, lastCompItem, associatedVariableExpression, extendingItem, nullptr, nullptr, &processing).first;
								}


								for (CExpressionVariable* indiVarExp : *indiVarSet) {
									QSet<COptimizedComplexVariableCompositionItem*>* compItems = varExpVarComItemHash->value(indiVarExp);
									if (compItems->size() > 0) {
										compItems->clear();
										compItems->insert(absorptionPropagationItem);
									}
								}


								buildingVarItem->updateLastHandledVariableItemAssociation(initializerVariableExp, lastCompItem);
								buildingVarItem->setLastHandledVariableExpression(initializerVariableExp);


							}

						}


						if (processing && buildingVarItem->isWaitingComputation() && !continueProcessingBuildingVarItem) {
							buildingVarItemList->removeFirst();
							buildingVarItem->setProcessingQueued(false);
						}


						if (!processing) {
							if (!buildingVarItem->isWaitingComputation() && (!buildingVarItem->isSatisfiable() || buildingVarItem->getComputeVariableMappingItemList()->isEmpty())) {
								buildingVarItemList->removeFirst();
								buildingVarItem->setProcessingQueued(false);

								CComplexQueryProcessingData* queryProcData = buildingVarItem->getQueryProcessingData();
								queryProcData->decBuildingVariableItem();
								if (!queryProcData->hasComputation()) {
									finishQueryProcessing(queryProcData);
								}
							}
						}




					}

				}

				return processing;
			}





			bool COptimizedComplexConceptAnsweringHandler::finishVariableCompositionItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				varCompItem->setVariableMappingComputed(true);
				if (varCompItem->getVariableMapping()->isEmpty()) {
					buildingVarItem->setSatisfiability(false);
				}
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				if (mConfDebugWriteVariableCompositionItems) {
					debugWriteVariableMappingToFileWithHeader(buildingVarItem, varCompItem);
				}
#endif
				return true;
			}



			static bool itemSortLessVariableMappingsThan(const TRemainingVariableMappingCountSortingItem& item1, const TRemainingVariableMappingCountSortingItem& item2) {
				return item1.second < item2.second;
			}



			QList<CIndividualVariableExpression*> COptimizedComplexConceptAnsweringHandler::getSortedRemainingVariableExpressionList(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, QSet<CIndividualVariableExpression*>* remainVarExpSet) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				QList<TRemainingVariableMappingCountSortingItem> remainVarExpSortList;


				for (CIndividualVariableExpression* remainVarExp : *remainVarExpSet) {
					QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(remainVarExp);

					cint64 minBindingSize = CINT64_MAX;
					for (COptimizedComplexVariableCompositionItem* joiningVarItem : *joiningVarItemSet) {
						minBindingSize = qMin(minBindingSize, (cint64)joiningVarItem->getVariableMapping()->size());
					}


					TRemainingVariableMappingCountSortingItem remVarSortItem(remainVarExp, minBindingSize);
					remainVarExpSortList.append(remVarSortItem);
				}




				qSort(remainVarExpSortList.begin(), remainVarExpSortList.end(), itemSortLessVariableMappingsThan);

				QStringList remainingVarStringList;

				QList<CIndividualVariableExpression*> remainVarExpList;
				for (TRemainingVariableMappingCountSortingItem remVarSortItem : remainVarExpSortList) {
					remainVarExpList.append(remVarSortItem.first);

					if (mConfExtendedLogging) {
						remainingVarStringList.append(QString("%1 with %2 mappings").arg(remVarSortItem.first->getName()).arg(remVarSortItem.second));
					}
				}

				if (mConfExtendedLogging) {
					LOG(INFO, getDomain(), logTr("Continue propagation for remaining variables: %1.").arg(remainingVarStringList.join(", ")), this);
				}

				return remainVarExpList;
			}









			bool COptimizedComplexConceptAnsweringHandler::processExtractedInstanceCandidatePossiblePropagations(CAnsweringMessageDataInstancePossiblePropagationsData* instanceCandidatePropagationMessage, CAnswererContext* answererContext) {
				CConcept* concept = instanceCandidatePropagationMessage->getTestingConcept();
				bool negation = instanceCandidatePropagationMessage->getTestingConceptNegation();
				COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(concept, negation);
				CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>* indiList = instanceCandidatePropagationMessage->getIndividualReferenceList();

				QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
				if (!indiList || indiList->isEmpty()) {
					mTestedPossibleInstancesCount += possibleInstanceItemSet->size();
					delete possibleInstanceItemSet;
					conceptItem->setPossibleInstanceItemSet(nullptr);
					conceptItem->incPossibleTestedNonInstanceCount(possibleInstanceItemSet->size());
				} else {
					QSet<CRealizationIndividualInstanceItemReference>* newPossibleInstanceItemSet = new QSet<CRealizationIndividualInstanceItemReference>();
					CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>::const_iterator it = indiList->constBegin(), itEnd = indiList->constEnd(); it != itEnd; ++it) {
						CRealizationIndividualInstanceItemReference indiItemRef = sameRealization->getSameInstanceItemReference(*it);
						if (possibleInstanceItemSet->contains(indiItemRef)) {
							newPossibleInstanceItemSet->insert(indiItemRef);
						} else {
							mTestedPossibleInstancesCount++;
						}
					}
					conceptItem->incPossibleTestedNonInstanceCount(newPossibleInstanceItemSet->size() - possibleInstanceItemSet->size());
					delete possibleInstanceItemSet;
					conceptItem->setPossibleInstanceItemSet(newPossibleInstanceItemSet);
				}
				conceptItem->setCandidateReceived(true);
				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::processExtractedInstanceCertainPropagations(CAnsweringMessageDataInstanceCertainPropagationsData* instanceCandidatePropagationMessage, CAnswererContext* answererContext) {

				CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology(false);
				CConcept* concept = instanceCandidatePropagationMessage->getTestingConcept();
				bool negation = instanceCandidatePropagationMessage->getTestingConceptNegation();
				CConcept* propagatedConcept = instanceCandidatePropagationMessage->getPropagatedConcept();
				COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(concept, negation);
				CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>* indiList = instanceCandidatePropagationMessage->getIndividualReferenceList();

				QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
				QSet<CRealizationIndividualInstanceItemReference>* directInstanceItemSet = conceptItem->getKnownInstanceItemSet();

				if (indiList && !indiList->isEmpty()) {
					CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>::const_iterator it = indiList->constBegin(), itEnd = indiList->constEnd(); it != itEnd; ++it) {
						CRealizationIndividualInstanceItemReference indiItemRef = sameRealization->getSameInstanceItemReference(*it);
						if (possibleInstanceItemSet->contains(indiItemRef)) {
							possibleInstanceItemSet->remove(indiItemRef);
							mTestedPossibleInstancesCount++;
							directInstanceItemSet->insert(indiItemRef);
						}
					}
				}
				return true;
			}






			bool COptimizedComplexConceptAnsweringHandler::visitAllIndividuals(function<bool(const CIndividualReference& indiRef)> visitFunc) {

				CConcreteOntology* ontology = mOntoAnsweringItem->getOntology();

				CBOXSET<CIndividual*>* activeIndiSet = ontology->getABox()->getActiveIndividualSet();
				CIndividualVector* indiVec = ontology->getABox()->getIndividualVector(false);
				cint64 indiCount = 0;
				if (indiVec) {
					indiCount = indiVec->getItemCount();
				}

				bool visited = false;
				bool continueVisiting = true;
				cint64 maxTriplesIndexedIndiId = 0;
				COntologyTriplesAssertionsAccessor* triplesAccessor = ontology->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (triplesAccessor) {
					maxTriplesIndexedIndiId = ontology->getOntologyTriplesData()->getTripleAssertionAccessor()->getMaxIndexedIndividualId();
				}
				if (indiVec) {
					cint64 indiCount = indiVec->getItemCount();
					for (cint64 idx = 0; idx < indiCount && continueVisiting; ++idx) {
						CIndividual* indi = indiVec->getData(idx);

						CIndividualReference indiRef;
						if (indi && activeIndiSet->contains(indi)) {
							indiRef = CIndividualReference(indi);
							visited = true;
						} else if (idx <= maxTriplesIndexedIndiId) {
							visited = true;
							indiRef = CIndividualReference(idx);
						}
						continueVisiting = visitFunc(indiRef);						

					}
				}
				return visited;
			}





			bool COptimizedComplexConceptAnsweringHandler::createAbsorbedQueryPartEntailmentTest(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart, CComplexQueryProcessingData* procData, CAnswererContext* answererContext) {
				procData->incEntailmentComputation();
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CExpressionVariable* initializerVariable = absorptionHanldingQueryPart->getInitializerVariableExpression();
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				CVariable* variable = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(initializerVariable);
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, absorptionHanldingQueryPart->isTopObjectPropertyUsed(), answererContext);

				visitAllIndividuals([&](const CIndividualReference& indiRef)->bool {
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, indiRef, satCalcJob);
					return true;
				});

				CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* completedMessage = new CAnsweringMessageDataCalculationCompletedVariableBindingEntailment(satCalcJob, absorptionHanldingQueryPart, procData);
				CAnsweringPropagationSteeringAbsorptionEntailmentController* propagationSteeringController = new CAnsweringPropagationSteeringAbsorptionEntailmentController();
				satCalcJob->setSatisfiableAnswererBindingPropagationAdapter(new CSatisfiableTaskAnswererBindingPropagationAdapter(absorptionHanldingQueryPart->getInitializerConcept(), absorptionHanldingQueryPart->getPropagationFinalizationConcept(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propagationSteeringController));
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartEntailmentTestingCount();
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}





			bool COptimizedComplexConceptAnsweringHandler::createAbsorbedQueryPartEntailmentTest(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext) {
				buildingVarItem->getQueryProcessingData()->incEntailmentComputation();
				buildingVarItem->getQueryProcessingData()->decBuildingVariableItem();
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart = absorptionPropagationExtension->getAbsorptionBasedHandlingData();
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				buildingVarItem->incVariableBindingsPropagationCount();
				CExpressionVariable* initializerVariable = absorptionHanldingQueryPart->getInitializerVariableExpression();
				if (!absorptionHanldingQueryPart->getPrepareVariableSet()->contains(initializerVariable)) {
					initializerVariable = *absorptionHanldingQueryPart->getPrepareVariableSet()->begin();
				}
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				CVariable* variable = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(initializerVariable);
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, absorptionHanldingQueryPart->isTopObjectPropertyUsed(), answererContext);
				QSet<CIndividualReference>* indiSet = absorptionPropagationExtension->getVariableSteeringIndividualBindingSet(variable);
				for (QSet<CIndividualReference>::const_iterator it = indiSet->constBegin(), itEnd = indiSet->constEnd(); it != itEnd; ++it) {
					CIndividualReference individual(*it);
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, individual, satCalcJob);
				}

				CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* completedMessage = new CAnsweringMessageDataCalculationCompletedVariableBindingEntailment(satCalcJob, absorptionHanldingQueryPart, buildingVarItem->getQueryProcessingData());
				CAnsweringPropagationSteeringAbsorptionExtensionItemEntailmentController* propagationSteeringController = new CAnsweringPropagationSteeringAbsorptionExtensionItemEntailmentController(absorptionPropagationExtension);
				satCalcJob->setSatisfiableAnswererBindingPropagationAdapter(new CSatisfiableTaskAnswererBindingPropagationAdapter(absorptionHanldingQueryPart->getInitializerConcept(), absorptionHanldingQueryPart->getPropagationFinalizationConcept(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propagationSteeringController));
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartEntailmentTestingCount();
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}






			bool COptimizedComplexConceptAnsweringHandler::createVariableBindingConfirmationTest(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableIndividualBindingsCardinalityLinker* testingVarIndiBindingCardLinker, CSameRealization* sameRealization, CAnswererContext* answererContext) {
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart = absorptionPropagationExtension->getAbsorptionBasedHandlingData();
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				buildingVarItem->incVariableBindingsConfirmationCount();
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());

				CVariable* initializingVariable = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(absorptionHanldingQueryPart->getInitializerVariableExpression());
				CRealizationIndividualInstanceItemReference instanceItemRef = testingVarIndiBindingCardLinker->getBindings()->getBinding(absorptionPropagationExtension->getVariableIndexHash()->value(initializingVariable)).reference;
				QList<CIndividualReference> sameIndiList;
				CInstanceBindingIndividualCollectionVisitor visitor(&sameIndiList);
				sameRealization->visitSameIndividuals(instanceItemRef, &visitor);
				CIndividualReference individualRef = sameIndiList.first();

				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, absorptionHanldingQueryPart->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, individualRef, satCalcJob);
				CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation* completedMessage = new CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation(satCalcJob, absorptionPropagationExtension, buildingVarItem, testingVarIndiBindingCardLinker);
				CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController* propagationSteeringController = new CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController(absorptionPropagationExtension, *testingVarIndiBindingCardLinker->getBindings(), sameRealization);
				satCalcJob->setSatisfiableAnswererBindingPropagationAdapter(new CSatisfiableTaskAnswererBindingPropagationAdapter(absorptionHanldingQueryPart->getInitializerConcept(), absorptionHanldingQueryPart->getPropagationFinalizationConcept(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propagationSteeringController));
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartVariableBindingConfirmationCount();
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}










			bool COptimizedComplexConceptAnsweringHandler::createVariableBindingPropagationTest(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext) {
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart = absorptionPropagationExtension->getAbsorptionBasedHandlingData();
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				buildingVarItem->incVariableBindingsPropagationCount();
				CExpressionVariable* initializerVariable = absorptionHanldingQueryPart->getInitializerVariableExpression();
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				CVariable* variable = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(initializerVariable);
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, absorptionHanldingQueryPart->isTopObjectPropertyUsed(), answererContext);
				QSet<CIndividualReference>* indiSet = absorptionPropagationExtension->getVariableSteeringIndividualBindingSet(variable);
				for (QSet<CIndividualReference>::const_iterator it = indiSet->constBegin(), itEnd = indiSet->constEnd(); it != itEnd; ++it) {
					CIndividualReference individual(*it);
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, individual, satCalcJob);
				}

				CAnsweringMessageDataCalculationCompletedVariableBindingPropagations* completedMessage = new CAnsweringMessageDataCalculationCompletedVariableBindingPropagations(satCalcJob, absorptionPropagationExtension, buildingVarItem);
				CAnsweringPropagationSteeringAbsorptionExtensionItemController* propagationSteeringController = new CAnsweringPropagationSteeringAbsorptionExtensionItemController(absorptionPropagationExtension);
				satCalcJob->setSatisfiableAnswererBindingPropagationAdapter(new CSatisfiableTaskAnswererBindingPropagationAdapter(absorptionHanldingQueryPart->getInitializerConcept(), absorptionHanldingQueryPart->getPropagationFinalizationConcept(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propagationSteeringController));
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartVariableBindingPropagationCount();
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}









			bool COptimizedComplexConceptAnsweringHandler::processVariableBindingsPropagationItemCalculationCompleted(CAnsweringMessageDataCalculationCompletedVariableBindingPropagations* message, CAnswererContext* answererContext) {
				COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem = message->getAbsorptionPropagationItem();
				COptimizedComplexBuildingVariableCompositionsItem* buildVarItem = message->getVariableBuildingItem();

				buildVarItem->decVariableBindingsPropagationCount();

				if (!absorptionPropagationItem->isVariableMappingComputed()) {
					// schedule item processing
					queueVariableItemComputation(buildVarItem, absorptionPropagationItem);
				}
				mOntoAnsweringItem->addProcessingVariableBuildingItem(buildVarItem);

				return true;
			}






			bool COptimizedComplexConceptAnsweringHandler::processVariableBindingsConfirmationCalculationCompleted(CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation* message, CAnswererContext* answererContext) {
				COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem = message->getAbsorptionPropagationItem();
				COptimizedComplexBuildingVariableCompositionsItem* buildVarItem = message->getVariableBuildingItem();

				COptimizedComplexVariableIndividualBindingsCardinalityLinker* variableBindingCardLinker = message->getPropagatedBindingCardinalityLinker();

				COptimizedComplexVariableIndividualMapping* testingVariableMapping = absorptionPropagationItem->getTestingVariableMapping();
				testingVariableMapping->remove(variableBindingCardLinker);

				bool variableBindingEntailed = !message->isSatisfiable();
				if (variableBindingEntailed) {
					absorptionPropagationItem->getVariableMapping()->addInsertingBindingsCardinalityLinker(variableBindingCardLinker, true);
				}

				buildVarItem->decVariableBindingsConfirmationCount();

				COptimizedComplexVariableIndividualMapping* possibleVarIndiBindings = absorptionPropagationItem->getPossibleVariableMapping();
				if (!buildVarItem->isProcessingQueued() && possibleVarIndiBindings->isEmpty() && !buildVarItem->isWaitingVariableBindingsConfirmation()) {
					buildVarItem->setProcessingQueued(true);
					mOntoAnsweringItem->addProcessingVariableBuildingItem(buildVarItem);
				}

				return true;
			}





			bool COptimizedComplexConceptAnsweringHandler::processVariableBindingsEntailmentCalculationCompleted(CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* message, CAnswererContext* answererContext) {
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorbedQueryPartData = message->getAbsorbedQueryPartData();
				CComplexQueryProcessingData* queryProcData = message->getQueryProcessingData();

				if (message->isSatisfiable()) {
					queryProcData->setUnsatisfiable(true);
				}
				queryProcData->decEntailmentComputation();
				if (!queryProcData->hasComputation()) {
					finishQueryProcessing(queryProcData);
				}
				return true;
			}




			bool COptimizedComplexConceptAnsweringHandler::processExtractedVariableBindingPropagations(CAnsweringMessageDataVariableBindingPropagations* variableBindingsPropagationMessage, CAnswererContext* answererContext) {


				CVARIABLEBINDINGMESSAGELIST<CAnsweringMessageDataVariableBindingPropagationsData*>* individualBindingList = variableBindingsPropagationMessage->getIndividualBindingList();
				CVARIABLEBINDINGMESSAGELIST<CVariable*>* variablesList = variableBindingsPropagationMessage->getVariablesList();

				CConcept* propagationFinalizationConcept = variableBindingsPropagationMessage->getPropagationFinalizationConcept();
				QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* propagationFinalizationConceptAbsorptionDataHash = mOntoAnsweringItem->getPropagationFinalizationConceptAbsorptionDataHash();
				QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* propagationFinalizationConceptAbsorptionItemHash = mOntoAnsweringItem->getPropagationFinalizationConceptAbsorptionItemHash();


				COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionItem = propagationFinalizationConceptAbsorptionItemHash->value(propagationFinalizationConcept);
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionData = propagationFinalizationConceptAbsorptionDataHash->value(propagationFinalizationConcept);

				CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
				QHash<CVariable*, cint64>* variableIndexHash = absorptionItem->getVariableIndexHash();
				COptimizedComplexVariableIndividualMapping* variableMapping = absorptionItem->getVariableMapping();



				QHash<CExpressionVariable*, cint64> variablePosHash;
				cint64 varPosCount = 0;
				QList<QList<cint64>> variableIndexPosList;
				for (CVariable* variable : *variablesList) {
					QList<cint64> indexs = variableIndexHash->values(variable);
					variableIndexPosList.append(indexs);
					varPosCount += indexs.size();
				}

				COptimizedComplexVariableIndividualMapping* possibleVariableMapping = absorptionItem->getPossibleVariableMapping();

				COptimizedComplexVariableIndividualBindings* propagatedBinding = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* propagatedCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* propagatedLinker = nullptr;

				COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem = nullptr;

				cint64 count = 0;
				// first verify that variable binding is valid
				for (CAnsweringMessageDataVariableBindingPropagationsData* individualBindingData : *individualBindingList) {
					bool possiblePropagation = individualBindingData->isPossibleBinding();
					count++;

					if (!propagatedBinding) {
						propagatedBinding = createBindingsForVariableCompositionItems(varPosCount, absorptionItem, buildingVarItem);
					}

					cint64 pos = 0;
					for (QList<cint64> variableIndexPositions : variableIndexPosList) {
						CIndividualReference indiRef = individualBindingData->getBindingIndividualReference(pos++);
						for (cint64 variableIndexPos : variableIndexPositions) {
							if (variableIndexPos >= 0) {
								CRealizationIndividualInstanceItemReference instanceItemRef = sameRealization->getSameInstanceItemReference(indiRef);
								propagatedBinding->setBinding(variableIndexPos, instanceItemRef);
							}
						}
					}

					if (!propagatedLinker) {
						propagatedLinker = createBindingsLinkerForVariableCompositionItems(propagatedBinding, nullptr, absorptionItem, buildingVarItem);
					}


					if (!possiblePropagation) {
						if (variableMapping->addInsertingBindingsCardinalityLinker(propagatedLinker, true)) {
							propagatedBinding = nullptr;
							propagatedLinker = nullptr;
						}

					} else {
						if (!possibleVariableMapping) {
							possibleVariableMapping = new COptimizedComplexVariableIndividualMapping(variablesList->size());
							absorptionItem->setPossibleVariableMapping(possibleVariableMapping);
						}
						if (possibleVariableMapping->addInsertingBindingsCardinalityLinker(propagatedLinker, true)) {
							propagatedBinding = nullptr;
							propagatedLinker = nullptr;
						}
					}
				}

				if (mConfExtendedLogging) {
					cint64 possMappingCount = 0;
					if (possibleVariableMapping) {
						possMappingCount = possibleVariableMapping->size();
					}
					cint64 certMappingCount = variableMapping->size();
					LOG(INFO, getDomain(), logTr("Extracted %1 certain and %2 possible mappings from propagated marker for absorbed query part.").arg(certMappingCount).arg(possMappingCount), this);
				}


				return true;
			}







			bool COptimizedComplexConceptAnsweringHandler::canRetrieveInstantiatingDataValueIndividuals(CRole* dataRole, CDataLiteral* dataLiteral) {
				COntologyTriplesAssertionsAccessor* triplesAssertionAccessor = mOntoAnsweringItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (triplesAssertionAccessor && dataRole->hasPropertyName()) {
					CDatatype* datatype = dataLiteral->getDatatype();
					if (datatype) {
						if (datatype->getDatatypeType() == CDatatype::DT_STRING) {
							return true;
						}
					}
				}
				return false;
			}


			QMap<cint64, IndividualIdCandidateData>* COptimizedComplexConceptAnsweringHandler::getInstantiatingDataValueIndividualIdMap(CRole* dataRole, CDataLiteral* dataLiteral, QMap<cint64, IndividualIdCandidateData>* instantiatingDataValueIndividualIdMap, CAnswererContext* answererContext) {
				if (!instantiatingDataValueIndividualIdMap) {
					instantiatingDataValueIndividualIdMap = new QMap<cint64, IndividualIdCandidateData>();
				}
				COntologyTriplesAssertionsAccessor* triplesAssertionAccessor = mOntoAnsweringItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
				triplesAssertionAccessor->visitDataValueIndividuals(dataRole, dataLiteral, [&](cint64 indiId)->bool {
					instantiatingDataValueIndividualIdMap->insert(indiId, IndividualIdCandidateData());
					return true;
				});
				return instantiatingDataValueIndividualIdMap;
			}






			QSet<CDataLiteral*>* COptimizedComplexConceptAnsweringHandler::getInstantiatedDataLiterals(const CIndividualReference& indiRef, CRole* dataRole, QSet<CDataLiteral*>* dataLiteralSet, CAnswererContext* answererContext) {

				if (!dataLiteralSet) {
					dataLiteralSet = new QSet<CDataLiteral*>();
				}

				CIndividualProcessNodeVector* indiProcVector = nullptr;
				CConsistence* consistence = mOntoAnsweringItem->getOntology()->getConsistence();
				if (consistence) {
					CConsistenceData* consData = consistence->getConsistenceModelData();
					if (consData) {
						CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
						CSatisfiableCalculationTask* satConsTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
						if (!satConsTask) {
							satConsTask = consTaskData->getDeterministicSatisfiableTask();
						}
						if (satConsTask) {
							CProcessingDataBox* procDataBox = satConsTask->getProcessingDataBox();
							if (procDataBox) {
								indiProcVector = procDataBox->getIndividualProcessNodeVector();
							}
						}
					}
				}


				if (indiProcVector) {
					bool nondeterministicNodeFlag = true;
					CIndividualProcessNode* indiProcNode = getNominalIndividualProcessNode(indiRef, indiProcVector, &nondeterministicNodeFlag, answererContext);
					if (indiProcNode) {
						CRoleSuccessorLinkIterator succRoleIt = indiProcNode->getRoleSuccessorLinkIterator(dataRole);
						while (succRoleIt.hasNext()) {
							bool nonDeterministicEdgeFlag = true;
							CIndividualLinkEdge* linkEdge = succRoleIt.next(true);
							CDependencyTrackPoint* linkEdgeDepPoint = linkEdge->getDependencyTrackPoint();
							if (linkEdgeDepPoint) {
								nonDeterministicEdgeFlag = linkEdgeDepPoint->getBranchingTag() > 0;
							}
							cint64 succIndiId = linkEdge->getOppositeIndividualID(indiProcNode);
							CIndividualProcessNode* succNode = indiProcVector->getData(succIndiId);

							for (CConceptDescriptor* conDesIt = succNode->getReapplyConceptLabelSet(false)->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNextConceptDesciptor()) {
								CConcept* concept = conDesIt->getConcept();
								bool negated = conDesIt->isNegated();
								CDependencyTrackPoint* conDepTrackPoint = conDesIt->getDependencyTrackPoint();
								bool nondeterministicConcept = true;
								if (!negated && concept->getOperatorCode() == CCDATALITERAL) {

									if (conDepTrackPoint) {
										nondeterministicConcept = conDepTrackPoint->getBranchingTag() > 0;
									}


									CDataLiteral* dataLiteral = concept->getDataLiteral();
									if (!nondeterministicNodeFlag && !nonDeterministicEdgeFlag && !nondeterministicConcept) {
										dataLiteralSet->insert(dataLiteral);
									}
								}
							}
						}
					}
				}


				COntologyTriplesAssertionsAccessor* triplesAssertionAccessor = mOntoAnsweringItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (triplesAssertionAccessor) {
					class COntologyTriplesIndividualDataAssertionsVisitor : public COntologyTriplesIndividualAssertionsVisitor {
					public:
						function<bool(CDataLiteral* dataLiteral, CRole* dataRole)> mDataLiteralHandleFunc;

						COntologyTriplesIndividualDataAssertionsVisitor(function<bool(CDataLiteral* dataLiteral, CRole* dataRole)> dataLiteralHandleFunc) {
							mDataLiteralHandleFunc = dataLiteralHandleFunc;
						}

						bool visitIndividualName(const QString& indiName, COntologyTriplesAssertionsAccessor* accessor) {
							// for debugging, it is visiting the individual name
							return true;
						}

						bool visitRoleAssertion(CRole* role, cint64 otherIndividualId, COntologyTriplesAssertionsAccessor* accessor) {
							return false;
						}

						bool visitReverseRoleAssertion(CRole* role, cint64 otherIndividualId, COntologyTriplesAssertionsAccessor* accessor) {
							return false;
						}

						bool visitDataAssertion(CRole* role, CDataLiteral* dataLiteral, COntologyTriplesAssertionsAccessor* accessor) {
							return mDataLiteralHandleFunc(dataLiteral, role);
						}

						bool visitConceptAssertion(CConcept* concept, COntologyTriplesAssertionsAccessor* accessor) {
							return false;
						}
					} visitor([&](CDataLiteral* dataLiteral, CRole* dataLiteralDataRole)->bool {
						if (dataRole == dataLiteralDataRole) {
							CDataLiteral* copiedDataLiteral = CObjectParameterizingAllocator<CDataLiteral, CContext*>::allocateAndConstructAndParameterize(nullptr, nullptr);
							copiedDataLiteral->initDataLiteral(dataLiteral->getLexicalDataLiteralValueString(), dataLiteral->getDatatype());
							dataLiteralSet->insert(copiedDataLiteral);
						}
						return true;
					});



					triplesAssertionAccessor->visitIndividualAssertions(indiRef.getIndividualID(), &visitor, false, false, false, true);
				} else {

					CIndividual* indi = indiRef.getIndividual();
					if (indi) {
						for (CDataAssertionLinker* dataAssertionLinkerIt = indi->getAssertionDataLinker(); dataAssertionLinkerIt; dataAssertionLinkerIt = dataAssertionLinkerIt->getNext()) {
							if (dataRole == dataAssertionLinkerIt->getRole()) {
								dataLiteralSet->insert(dataAssertionLinkerIt->getDataLiteral());
							}
						}
					}
				}

				return dataLiteralSet;
			}







			CIndividualProcessNode* COptimizedComplexConceptAnsweringHandler::getMergeCorrectedIndividualProcessNode(CIndividualProcessNode* indiProcNode, CIndividualProcessNodeVector* indiProcVector, bool* nonDeterministicallyMergedFlag, CAnswererContext* answererContext) {
				bool nonDeterministicMerged = false;
				while (indiProcNode && indiProcNode->getMergedIntoIndividualNodeID() != indiProcNode->getIndividualNodeID()) {
					cint64 mergeID = indiProcNode->getMergedIntoIndividualNodeID();
					CDependencyTrackPoint* mergedDepTrackPoint = indiProcNode->getMergedDependencyTrackPoint();
					indiProcNode = indiProcVector->getData(mergeID);
					if (!mergedDepTrackPoint || mergedDepTrackPoint->getBranchingTag() > 0) {
						nonDeterministicMerged = true;
					}
				}
				if (nonDeterministicallyMergedFlag) {
					*nonDeterministicallyMergedFlag = nonDeterministicMerged;
				}
				return indiProcNode;
			}




			CIndividualProcessNode* COptimizedComplexConceptAnsweringHandler::getNominalIndividualProcessNode(const CIndividualReference& indiRef, CIndividualProcessNodeVector* indiProcVector, bool* nondeterministicNodeFlag, CAnswererContext* answererContext) {

				cint64 indiID = -indiRef.getIndividualID();

				if (indiProcVector) {
					CIndividualProcessNode* baseIndiProcNode = indiProcVector->getData(indiID);
					CIndividualProcessNode* indiProcNode = baseIndiProcNode;
					bool nondeterministicallyMergedFlag = false;
					if (indiProcNode) {
						indiProcNode = getMergeCorrectedIndividualProcessNode(indiProcNode, indiProcVector, &nondeterministicallyMergedFlag, answererContext);
					}
					if (nondeterministicNodeFlag) {
						*nondeterministicNodeFlag = nondeterministicallyMergedFlag;
					}
					return indiProcNode;
				}
				return nullptr;
			}






			bool COptimizedComplexConceptAnsweringHandler::queueVariableItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				if (!varCompItem->isComputationQueued()) {
					varCompItem->setComputationQueued(true);
					buildingVarItem->addComputeVariableMappingItem(varCompItem);
					return true;
				}
				return false;
			}




			bool COptimizedComplexConceptAnsweringHandler::debugCheckVariableMappingContainsSolution(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();

				QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItemVariableExpressionMapping>* varItemIndexMappingHash = buildingVarItem->getVariableItemIndexMappingHash();

				bool allContainingSolution = true;
				for (QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItemVariableExpressionMapping>::const_iterator it = varItemIndexMappingHash->constBegin(), itEnd = varItemIndexMappingHash->constEnd(); it != itEnd; ++it) {
					CExpressionVariable* varExp = it.key();
					const COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = it.value();
					if (varItemIndexMapping.contains(varCompItem)) {
						allContainingSolution &= debugCheckVariableMappingContainsSolution(buildingVarItem, varExp, varCompItem);
					}
				}

				return allContainingSolution;

			}



			QString COptimizedComplexConceptAnsweringHandler::debugGetVariableMappingsHeaderString(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();

				QString varHeaderString;
				for (auto it = varExpVarComItemHash->constBegin(), itEnd = varExpVarComItemHash->constEnd(); it != itEnd; ++it) {
					CExpressionVariable* var = it.key();
					QSet<COptimizedComplexVariableCompositionItem*>* compItemSet = it.value();
					if (compItemSet && compItemSet->contains(varCompItem)) {
						QStringList varNameStrings = debugGetItemVariableNames(buildingVarItem, var, varCompItem);
						varHeaderString += varNameStrings.join("\t") + "\r\n";
					}
				}
				return varHeaderString;
			}


			QSet<QString> COptimizedComplexConceptAnsweringHandler::debugGetVariableMappingStringSet(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				QSet<QString> bindingStringSet;
				COptimizedComplexVariableIndividualMapping* varMapping = varCompItem->getVariableMapping();

				for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = varMapping->getLastAddedBindingsCardinalityLinker(); linker; linker = linker->getNext()) {
					COptimizedComplexVariableIndividualBindings* binding = linker->getBindings();

					QString bindingString;
					for (cint64 i = 0; i < binding->getBindingSize(); ++i) {

						COptimizedComplexVariableIndividualMapping::VARIABLE_TYPE bindingType = varMapping->getBindingMapping(i);
						if (bindingType == COptimizedComplexVariableIndividualMapping::INDIVIDUAL_VARIABLE) {

							CRealizationIndividualInstanceItemReference instItemRef = binding->getBinding(i).reference;
							COptimizedKPSetIndividualItem* indiItem = (COptimizedKPSetIndividualItem*)instItemRef.getRealizationInstanceItem();
							CIndividual* individual = instItemRef.getIndividual();
							QString individualString = QString::number(instItemRef.getIndividualID());
							if (individual) {
								individualString = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
							} else {
								individualString = mOntoAnsweringItem->getOntology()->getIndividualNameResolver()->getIndividualName(instItemRef);
							}
							if (!bindingString.isEmpty()) {
								bindingString.append("\t");
							}
							bindingString.append(individualString);

						}
					}
					bindingStringSet.insert(bindingString);
				}
				return bindingStringSet;
			}


			bool COptimizedComplexConceptAnsweringHandler::debugWriteVariableMappingToFileWithHeader(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				COptimizedComplexVariableIndividualMapping* varMapping = varCompItem->getVariableMapping();				 
				QString nextFileName = QString("Debugging/Answering/item-bindings-%1.tsv").arg(nextBindingsFileId++);
				QFile bindingsFile(nextFileName);
				if (bindingsFile.open(QIODevice::WriteOnly)) {
					QString varHeaderString = debugGetVariableMappingsHeaderString(buildingVarItem, varCompItem);
					bindingsFile.write(varHeaderString.toLocal8Bit());

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
					cint64 debugId = 0;
					for (auto debugString : debugCreationStringList) {
						if (debugString == varCompItem->debugCreationString) {
							bindingsFile.write(QString("#-%1 [current] >> %2\r\n").arg(debugId++).arg(debugString).toLocal8Bit());
						} else {
							bindingsFile.write(QString("#-%1 --------- >> %2\r\n").arg(debugId++).arg(debugString).toLocal8Bit());
						}
					}

#endif

					QSet<QString> bindingsStringSet = debugGetVariableMappingStringSet(buildingVarItem, varCompItem);
					for (QString bindingString : bindingsStringSet) {
						bindingsFile.write(bindingString.toLocal8Bit());
						bindingsFile.write("\r\n");
					}
					bindingsFile.close();
				}
				return true;
			}




			bool COptimizedComplexConceptAnsweringHandler::debugWriteVariableMappingToFile(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				COptimizedComplexVariableIndividualMapping* varMapping = varCompItem->getVariableMapping();
				QFile bindingsFile("Debugging/Answering/item-bindings.tsv");
				QString bindingsString;
				QSet<QString> bindingsStringSet = debugGetVariableMappingStringSet(buildingVarItem, varCompItem);
				for (QString bindingString : bindingsStringSet) {
					if (!bindingsString.isEmpty()) {
						bindingsString.append("\n");
					}
					bindingsString.append(bindingString);
				}
				if (bindingsFile.open(QIODevice::WriteOnly)) {
					bindingsFile.write(bindingsString.toLocal8Bit());
					bindingsFile.close();
				}
				return true;
			}


			bool COptimizedComplexConceptAnsweringHandler::debugCheckVariableMappingFromFile(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				COptimizedComplexVariableIndividualMapping* varMapping = varCompItem->getVariableMapping();
				QFile bindingsFile("Debugging/Answering/item-bindings.tsv");

				QSet<QString> itemBindingsStringSet = debugGetVariableMappingStringSet(buildingVarItem, varCompItem);
				QSet<QString> fileBindingsStringSet;

				if (bindingsFile.open(QIODevice::ReadOnly)) {
					while (!bindingsFile.atEnd()) {
						fileBindingsStringSet.insert(bindingsFile.readLine().trimmed());
					}
					bindingsFile.close();
				}

				QStringList itemAdditionalBindings;
				for (QString itemBinding: itemBindingsStringSet) {
					if (!fileBindingsStringSet.contains(itemBinding.trimmed())) {
						itemAdditionalBindings.append(itemBinding.trimmed());
					}
				}

				QStringList fileAdditionalBindings;
				for (QString fileBinding : fileBindingsStringSet) {
					if (!itemBindingsStringSet.contains(fileBinding.trimmed())) {
						fileAdditionalBindings.append(fileBinding.trimmed());
					}
				}

				if (fileAdditionalBindings.size() > 0 || itemAdditionalBindings.size() > 0) {
					bool debug = true;
				}
				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::debugCheckVariableMappingContainsSolution(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, COptimizedComplexVariableCompositionItem* varCompItem) {
				QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);


				QHash<QString, QString> varSolutionHash;
				varSolutionHash.insert("X", "http://www.Department0.University0.edu/GraduateStudent112");
				varSolutionHash.insert("Y", "http://www.Department0.University0.edu/AssociateProfessor9");
				varSolutionHash.insert("Z", "http://www.Department0.University0.edu/GraduateCourse31");

				COptimizedComplexVariableCompositionItemVariableIndexMapping* itemMapping = varItemIndexMapping.value(varCompItem);
				QVector<QString> nameStringVec(itemMapping->count());

				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemMapping->constBegin(), itEnd = itemMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* expression = it.value();
					QString varName = expression->getName();
					nameStringVec[idx] = varName;
				}

				bool contained = false;
				COptimizedComplexVariableIndividualMapping* varMapping = varCompItem->getVariableMapping();
				for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = varMapping->getLastAddedBindingsCardinalityLinker(); linker; linker = linker->getNext()) {
					COptimizedComplexVariableIndividualBindings* binding = linker->getBindings();

					bool allVariableBindingsMatchSolution = true;
					for (cint64 i = 0; allVariableBindingsMatchSolution && i < itemMapping->count(); ++i) {



						QString varString = nameStringVec[i];
						if (varSolutionHash.contains(varString)) {
							QString solution = varSolutionHash[varString];

							COptimizedComplexVariableIndividualMapping::VARIABLE_TYPE bindingType = varMapping->getBindingMapping(i);
							if (bindingType == COptimizedComplexVariableIndividualMapping::INDIVIDUAL_VARIABLE) {

								CRealizationIndividualInstanceItemReference instItemRef = binding->getBinding(i).reference;
								COptimizedKPSetIndividualItem* indiItem = (COptimizedKPSetIndividualItem*)instItemRef.getRealizationInstanceItem();
								CIndividual* individual = indiItem->getIndividual();
								QString individualString = QString::number(indiItem->getIndividualId());
								if (individual) {
									individualString = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
								} else {
									individualString = mOntoAnsweringItem->getOntology()->getIndividualNameResolver()->getIndividualName(indiItem->getIndividualReference());
								}

								if (individualString != solution) {
									allVariableBindingsMatchSolution = false;
								}

							} else if (bindingType == COptimizedComplexVariableIndividualMapping::DATA_LITERAL_VARIABLE) {

								CDataLiteral* dataLiteral = (CDataLiteral*)binding->getBinding(i).pointer;
								QString literalString = "\"" + dataLiteral->getDataLiteralValue()->getValueString() + "\"^^" + dataLiteral->getDatatype()->getDatatypeIRI();
								if (literalString != solution) {
									allVariableBindingsMatchSolution = false;
								}

							}


						}
					}

					if (allVariableBindingsMatchSolution) {
						contained = true;
						break;
					}

				}
				if (!contained) {
					bool debug = false;
					if (debug) {
						debugCheckVariableMappingContainsSolution(buildingVarItem, varExp, varCompItem);
					}
				}
				return contained;

			}




			QStringList COptimizedComplexConceptAnsweringHandler::debugGetItemVariableNames(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, COptimizedComplexVariableCompositionItem* varCompItem) {
				QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);


				QStringList varNameList;
				COptimizedComplexVariableCompositionItemVariableIndexMapping* itemMapping = varItemIndexMapping.value(varCompItem);
				if (itemMapping) {
					QVector<QString> nameStringVec(itemMapping->count());

					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemMapping->constBegin(), itEnd = itemMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						CExpressionVariable* expression = it.value();
						QString varName = expression->getName();
						nameStringVec[idx] = varName;
					}
					for (cint64 i = 0; i < itemMapping->count(); ++i) {
						varNameList.append(nameStringVec[i]);
					}
				} else {
					bool debug = true;
				}
				return varNameList;
			}




			bool COptimizedComplexConceptAnsweringHandler::tryReuseVariableRolePropagationItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CObjectPropertyAssertionExpression* propAss) {

				QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);

				CRole* role = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propAss->getObjectPropertyTermExpression());
				bool inversed = false;
				if (propAss->getFirstIndividualTermExpression() != varExp) {
					inversed = true;
				}
				CIndividualVariableExpression* otherExp = buildingVarItem->getOtherVariableExpression(varExp, propAss);
				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				bool propVarExpAlreadyPresent = false;
				cint64 varIdx = 0;
				cint64 propVarIdx = itemIndexMapping->size();
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* testVarExp = it.value();
					if (testVarExp == otherExp) {
						propVarExpAlreadyPresent = true;
						propVarIdx = idx;
					}
					else if (varExp == testVarExp) {
						varIdx = idx;
					}
				}

				COptimizedComplexVariableIndividualMapping* varMapping = varCompItem->getVariableMapping();

				if (varCompItem->hasRoleInversePropagationItem(role, inversed, varIdx)) {
					COptimizedComplexVariableRolePropagationItem* varRolePropItem = varCompItem->getRoleInversePropagationItem(role, inversed, varIdx);



					COptimizedComplexVariableCompositionItemVariableExpressionMapping& propVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(otherExp);
					COptimizedComplexVariableCompositionItemVariableIndexMapping* propItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
					propVarItemIndexMapping.insertMulti(varRolePropItem, propItemIndexMapping);

					cint64 newIdx = itemIndexMapping->size();
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						CExpressionVariable* propVarExp = it.value();
						propItemIndexMapping->insert(idx, propVarExp);
					}
					propItemIndexMapping->insert(newIdx, otherExp);

					varExpVarComItemHash->value(otherExp)->insert(varRolePropItem);
					buildingVarItem->updateLastHandledVariableItemAssociation(otherExp, varRolePropItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
					QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varCompItem);
					QStringList debugVariableNamePropItemStringList = debugGetItemVariableNames(buildingVarItem, otherExp, varRolePropItem);
					varRolePropItem->debugVariableNameStringList = debugVariableNamePropItemStringList;
					QString roleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
					if (inversed) {
						roleName = CIRIName::getRecentIRIName(role->getInverseRole()->getPropertyNameLinker()) + "^-";
					}
					varRolePropItem->debugCreationString = "[" + debugVariableNamePropItemStringList.join(",") + "] obtained by propagating " + varExp->getName() + " from [" + debugVariableNamePrevItemStringList.join(",") + "] over role " + roleName;
					debugCreationStringList += varRolePropItem->debugCreationString;
#endif


					return true;
				} else {
					return false;
				}
			}






			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexConceptAnsweringHandler::buildVariableRolePropagationItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CDataLiteralVariableExpression* dataVariable, CDataPropertyTermExpression* dataPropTerm, bool* processing, CAnswererContext* answererContext) {
				QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);

				CRole* dataRole = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash()->value(dataPropTerm);
				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				bool propVarExpAlreadyPresent = false;
				cint64 varIdx = 0;
				cint64 propVarIdx = itemIndexMapping->size();
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* testVarExp = it.value();
					if (testVarExp == dataVariable) {
						propVarExpAlreadyPresent = true;
						propVarIdx = idx;
					}
					else if (varExp == testVarExp) {
						varIdx = idx;
					}
				}

				COptimizedComplexVariableIndividualMapping* varMapping = varCompItem->getVariableMapping();

				COptimizedComplexVariableDataLiteralExtensionItem*& varDataRoleExtItem = varCompItem->getDataLiteralExtensionItem(dataRole, varIdx);
				bool created = false;
				if (!varDataRoleExtItem) {
					varDataRoleExtItem = new COptimizedComplexVariableDataLiteralExtensionItem(varCompItem, dataRole, varIdx);
					created = true;
				}
				if (!varDataRoleExtItem->isVariableMappingComputed()) {
					// schedule item processing
					queueVariableItemComputation(buildingVarItem, varDataRoleExtItem);
					if (processing) {
						*processing = true;
					}

				}



				COptimizedComplexVariableCompositionItemVariableIndexMapping* extendItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();

				cint64 newIdx = itemIndexMapping->size();
				COptimizedComplexVariableIndividualMapping* propVarMapping = nullptr;
				if (created) {
					propVarMapping = new COptimizedComplexVariableIndividualMapping(newIdx + 1);
				}
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* propVarExp = it.value();
					extendItemIndexMapping->insert(idx, propVarExp);
					if (created) {
						propVarMapping->setBindingMapping(idx, varMapping->getBindingMapping(idx));
					}
				}
				extendItemIndexMapping->insert(newIdx, dataVariable);
				if (created) {
					propVarMapping->setBindingMapping(newIdx, COptimizedComplexVariableIndividualMapping::DATA_LITERAL_VARIABLE);
					varDataRoleExtItem->setVariableMapping(propVarMapping);
				}



#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varCompItem);
#endif

				varItemIndexMapping.insert(varDataRoleExtItem, extendItemIndexMapping);
				varExpVarComItemHash->value(varExp)->insert(varDataRoleExtItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNameNewItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varDataRoleExtItem);
				varDataRoleExtItem->debugVariableNameStringList = debugVariableNameNewItemStringList;
				QString roleName = CIRIName::getRecentIRIName(dataRole->getPropertyNameLinker());
				varDataRoleExtItem->debugCreationString = "[" + debugVariableNameNewItemStringList.join(",") + "] obtained by extending " + varExp->getName() + " from [" + debugVariableNamePrevItemStringList.join(",") + "] with data values for data role " + roleName;
				debugCreationStringList += varDataRoleExtItem->debugCreationString;
#endif

				buildingVarItem->updateLastHandledVariableItemAssociation(varExp, varDataRoleExtItem);

				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(varDataRoleExtItem, extendItemIndexMapping);
			}









			QHash<CExpressionVariable*, cint64> COptimizedComplexConceptAnsweringHandler::getReducableVariables(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* propVarExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CObjectPropertyAssertionExpression* propAss) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(propVarExp);

				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* absorbedQueryPartsItemExtensionList = buildingVarItem->getAbsorbedQueryPartItemExtensionHandlingList();

				QHash<CExpressionVariable*, cint64> reductionVarIdxSet;
				if (mConfAllowBindingReduction && buildingVarItem->isBindingsReducible() && varCompItem->getVariableMapping()->size() > mConfMappingSizeBindingReduction && absorbedQueryPartsItemExtensionList->isEmpty()) {
					// test whether there is no absorption propagation
					// test whether there exists a variable that is not distinguished, has no remaining properties, and is not used by a remaining item for any variable
					QSet<CExpressionVariable*>* allVarExpSet = buildingVarItem->getAllVariableSet();
					QSet<CExpressionVariable*>* reductionDeniedVarExpSet = buildingVarItem->getReductionDeniedVariableSet();

					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						CExpressionVariable* varExp = it.value();
						CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);

						if (!reductionDeniedVarExpSet->contains(varExp) && indiVarExp) {

							QList<CObjectPropertyAssertionExpression*> propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(indiVarExp);
							QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(indiVarExp);
							if (dataPropAssList.isEmpty() && (propVarExp != varExp && propAssList.isEmpty() || propAssList.size() == 1 && propAssList.first() == propAss)) {

								bool varExpStillRequired = false;
								for (CExpressionVariable* otherVarExp : *allVarExpSet) {
									//QSet<COptimizedComplexVariableCompositionItem*>* remainingItemSet = varExpVarComItemHash->value(otherVarExp);
									//for (COptimizedComplexVariableCompositionItem* remainingItem : *remainingItemSet) {
									//}
									//CExpressionVariable* associatedVariableExpression = buildingVarItem->getVariableLastCompositionItemAssociatedVariableExpression(otherVarExp);
									COptimizedComplexVariableCompositionItem* otherVarCompItem = buildingVarItem->getVariableLastCompositionItem(otherVarExp);
									if (otherVarCompItem != varCompItem && otherVarCompItem) {
										COptimizedComplexVariableCompositionItemVariableIndexMapping* otherItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(otherVarExp).value(otherVarCompItem);
										if (otherItemIndexMapping) {
											for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator otherIt = otherItemIndexMapping->constBegin(), otherItEnd = otherItemIndexMapping->constEnd(); otherIt != otherItEnd && !varExpStillRequired; ++otherIt) {
												CExpressionVariable* testVarExp = otherIt.value();
												if (testVarExp == varExp) {
													varExpStillRequired = true;
												}
											}
										}
									}
									if (varExpStillRequired) {
										break;
									}
								}

								if (!varExpStillRequired) {
									reductionVarIdxSet.insertMulti(varExp, idx);
								}

							}

						}
					}
				}
				return reductionVarIdxSet;
			}




			QHash<CExpressionVariable*, cint64> COptimizedComplexConceptAnsweringHandler::getReducableVariables(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* propVarExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(propVarExp);

				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* absorbedQueryPartsItemExtensionList = buildingVarItem->getAbsorbedQueryPartItemExtensionHandlingList();

				QHash<CExpressionVariable*, cint64> reductionVarIdxSet;


				if (mConfAllowBindingReduction && buildingVarItem->isBindingsReducible() && varCompItem->getVariableMapping()->size() > mConfMappingSizeBindingReduction && absorbedQueryPartsItemExtensionList->isEmpty()) {
					// test whether there is no absorption propagation
					// test whether there exists a variable that is not distinguished, has no remaining properties, and is not used by a remaining item for any variable
					QSet<CExpressionVariable*>* allVarExpSet = buildingVarItem->getAllVariableSet();
					QSet<CExpressionVariable*>* reductionDeniedVarExpSet = buildingVarItem->getReductionDeniedVariableSet();

					QHash<CExpressionVariable*, cint64> varUseCountHash;

					for (CExpressionVariable* varExp : *allVarExpSet) {

						CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);
						if (indiVarExp) {
							QList<CObjectPropertyAssertionExpression*> propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(indiVarExp);
							QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(indiVarExp);
							QSet<COptimizedComplexVariableCompositionItem*>* remainingItemSet = varExpVarComItemHash->value(varExp);

							if (remainingItemSet && (!dataPropAssList.isEmpty() || !propAssList.isEmpty() || remainingItemSet->size() > 1)) {

								for (COptimizedComplexVariableCompositionItem* remainingItem : *remainingItemSet) {
									if (remainingItem) {
										COptimizedComplexVariableCompositionItemVariableIndexMapping* otherItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp).value(remainingItem);
										if (otherItemIndexMapping) {
											for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator otherIt = otherItemIndexMapping->constBegin(), otherItEnd = otherItemIndexMapping->constEnd(); otherIt != otherItEnd; ++otherIt) {
												CExpressionVariable* otherVarExp = otherIt.value();
												varUseCountHash[otherVarExp]++;
											}
										}
									}
								}

							}
						}
					}


					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						CExpressionVariable* varExp = it.value();
						CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);

						if (!reductionDeniedVarExpSet->contains(varExp) && indiVarExp) {

							cint64 useCount = varUseCountHash.value(varExp);
							if (useCount <= 1) {
								reductionVarIdxSet.insertMulti(varExp, idx);
							}
						}
					}
				}
				return reductionVarIdxSet;
			}




			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexConceptAnsweringHandler::buildVariableReductionItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* propVarExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, QHash<CExpressionVariable*, cint64>& reductionVarExpIndHash, bool* processing) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(propVarExp);

				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* absorbedQueryPartsItemExtensionList = buildingVarItem->getAbsorbedQueryPartItemExtensionHandlingList();

				if (!reductionVarExpIndHash.isEmpty()) {

					QMap<cint64,cint64> reductionVarIdxMap;
					cint64 nextRedNumber = 0;
					for (QHash<CExpressionVariable*, cint64>::iterator it = reductionVarExpIndHash.begin(); it != reductionVarExpIndHash.end(); ) {
						CExpressionVariable* expVar = it.key();
						cint64 varIdx = it.value();
						if (expVar != propVarExp) {
							reductionVarIdxMap.insert(varIdx, nextRedNumber++);
							it = reductionVarExpIndHash.erase(it);
						} else {
							++it;
						}
					}



					cint64 redNumber = 0;
					while (!reductionVarIdxMap.isEmpty()) {

						QMap<cint64, cint64>::iterator redIt = reductionVarIdxMap.begin();
						cint64 nextRedIdx = redIt.key();
						cint64 correctedRedIdx = nextRedIdx - redNumber++;
						redIt = reductionVarIdxMap.erase(redIt);


						COptimizedComplexVariableReductionItem*& varRedItem = varCompItem->getReductionItem(correctedRedIdx);
						bool created = false;
						if (!varRedItem) {
							varRedItem = new COptimizedComplexVariableReductionItem(varCompItem, correctedRedIdx);
							created = true;
						}

						COptimizedComplexVariableCompositionItemVariableIndexMapping* redItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
						varItemIndexMapping.insertMulti(varRedItem, redItemIndexMapping);

						cint64 newIdx = itemIndexMapping->size();
						CExpressionVariable* redVarExp = nullptr;
						COptimizedComplexVariableIndividualMapping* redVarMapping = nullptr;
						if (created) {
							redVarMapping = new COptimizedComplexVariableIndividualMapping(newIdx - 1);
						}

						cint64 nextIdx = 0;
						COptimizedComplexVariableIndividualMapping* varMapping = varCompItem->getVariableMapping();
						for (cint64 i = 0; i < itemIndexMapping->size(); ++i) {
							CExpressionVariable* indVarExp = itemIndexMapping->value(i);
							if (i != correctedRedIdx) {
								redItemIndexMapping->insert(nextIdx, indVarExp);
								if (created) {
									redVarMapping->setBindingMapping(nextIdx++, varMapping->getBindingMapping(i));
								} else {
									nextIdx++;
								}
							} else {
								redVarExp = indVarExp;
							}
						}


						if (!varRedItem->isVariableMappingComputed()) {
							// schedule item processing
							queueVariableItemComputation(buildingVarItem, varRedItem);
							if (processing) {
								*processing = true;
							}
						}

						if (created) {
							varRedItem->setVariableMapping(redVarMapping);
						}

						varExpVarComItemHash->value(propVarExp)->remove(varCompItem);
						varExpVarComItemHash->value(propVarExp)->insert(varRedItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
						QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, propVarExp, varCompItem);
						QStringList debugVariableNameRedItemStringList = debugGetItemVariableNames(buildingVarItem, propVarExp, varRedItem);
						varRedItem->debugVariableNameStringList = debugVariableNameRedItemStringList;
						varRedItem->debugCreationString = "[" + debugVariableNameRedItemStringList.join(",") + "] obtained by reducing " + redVarExp->getName() + " from [" + debugVariableNamePrevItemStringList.join(",") + "]";
						debugCreationStringList += varRedItem->debugCreationString;
#endif


						buildingVarItem->updateLastHandledVariableItemAssociation(propVarExp, varRedItem);
						buildingVarItem->clearLastHandledVariableItemAssociation(redVarExp);

						varCompItem = varRedItem;
						itemIndexMapping = redItemIndexMapping;

					}
				}

				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(varCompItem, itemIndexMapping);
			}




			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexConceptAnsweringHandler::buildVariableRolePropagationItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, QHash<CExpressionVariable*, cint64>& reductionVarExpIndHash, CObjectPropertyAssertionExpression* propAss, bool* processing, CAnswererContext* answererContext) {

				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);

				CRole* role = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propAss->getObjectPropertyTermExpression());
				bool inversed = false;
				if (propAss->getFirstIndividualTermExpression() != varExp) {
					inversed = true;
				}


				if (mConfExtendedLogging) {
					QString roleName = "?";
					if (role->getPropertyNameLinker()) {
						roleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
						if (inversed) {
							roleName = roleName + " inversed";
						}
					} else if (role->getInverseRole() && role->getInverseRole()->getPropertyNameLinker()) {
						roleName = CIRIName::getRecentIRIName(role->getInverseRole()->getPropertyNameLinker());
						if (!inversed) {
							roleName = roleName + " inversed";
						}
					}
					LOG(INFO, getDomain(), logTr("Scheduling propagation of mappings associated with %1 over role %2.").arg(varExp->getName()).arg(roleName), this);
				}


				CIndividualVariableExpression* otherExp = buildingVarItem->getOtherVariableExpression(varExp, propAss);
				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				bool propVarExpAlreadyPresent = false;
				cint64 varIdx = 0;
				cint64 propVarIdx = itemIndexMapping->size();
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* testVarExp = it.value();
					if (testVarExp == otherExp) {
						propVarExpAlreadyPresent = true;
						propVarIdx = idx;
					} else if (varExp == testVarExp) {
						varIdx = idx;
					}
				}

				bool replacement = false;
				if (!propVarExpAlreadyPresent && reductionVarExpIndHash.value(varExp, -1) == varIdx) {
					replacement = true;
				}
				if (propVarExpAlreadyPresent && !mConfPropagationJoining) {
					propVarExpAlreadyPresent = false;
				}

				cint64 newVarIdx = varIdx;
				cint64 newVarCount = itemIndexMapping->size();

				COptimizedComplexVariableIndividualMapping* varMapping = varCompItem->getVariableMapping();

				bool created = false;
				COptimizedComplexVariableRolePropagationAbstractItem* varRolePropItem = nullptr;
				if (!propVarExpAlreadyPresent) {
					newVarIdx = itemIndexMapping->size();
					newVarCount = itemIndexMapping->size() + 1;
					COptimizedComplexVariableRolePropagationItem*& newVarRolePropItem = varCompItem->getRoleInversePropagationItem(role, inversed, varIdx);
					if (!newVarRolePropItem) {
						newVarRolePropItem = new COptimizedComplexVariableRolePropagationItem(varCompItem, role, inversed, varIdx);
						created = true;
					}
					varRolePropItem = newVarRolePropItem;
				} else if (replacement) {
					COptimizedComplexVariableRolePropagationReplacementItem*& replacementVarRolePropItem = varCompItem->getRoleInversePropagationReplacementItem(role, inversed, varIdx, varIdx);
					if (!replacementVarRolePropItem) {
						replacementVarRolePropItem = new COptimizedComplexVariableRolePropagationReplacementItem(varCompItem, role, inversed, varIdx, varIdx);
						created = true;
					}
					varRolePropItem = replacementVarRolePropItem;
				} else {
					COptimizedComplexVariableRolePropagationJoiningItem*& joinVarRolePropItem = varCompItem->getRoleInversePropagationJoiningItem(role, inversed, varIdx, propVarIdx);
					newVarIdx = propVarIdx;
					if (!joinVarRolePropItem) {
						joinVarRolePropItem = new COptimizedComplexVariableRolePropagationJoiningItem(varCompItem, role, inversed, varIdx, propVarIdx);
						created = true;
					}
					varRolePropItem = joinVarRolePropItem;
				}
				if (!varRolePropItem->isVariableMappingComputed()) {
					// schedule item processing
					queueVariableItemComputation(buildingVarItem, varRolePropItem);
					if (processing) {
						*processing = true;
					}

					//// schedule realization requirement processing
					//CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
					//QSet<CIndividualReference> individualSet;
					//class CRoleTargetsRealizationRequirementInstanceItemVisitor : public CSameRealizationIndividualVisitor {
					//public:
					//	QSet<CIndividualReference>* mInstanceItemSet;

					//	CRoleTargetsRealizationRequirementInstanceItemVisitor(QSet<CIndividualReference>* instanceItemSet) {
					//		mInstanceItemSet = instanceItemSet;
					//	}

					//	bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
					//		mInstanceItemSet->insert(indiRef);
					//		return false;
					//	}
					//} visitor(&individualSet);
					//for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = varMapping->getLastAddedBindingsCardinalityLinker(); linker; linker = linker->getNext()) {
					//	COptimizedComplexVariableIndividualBindings* binding = linker->getBindings();
					//	CRealizationIndividualInstanceItemReference instItemRef = bindings->getBinding(varIdx).reference;
					//	sameRealization->visitSameIndividuals(instItemRef, &visitor);
					//}
					//if (!individualSet.isEmpty()) {
					//	ensureRoleTargetsRealization(role, inversed, individualSet, buildingVarItem, answererContext);
					//}
				}


				COptimizedComplexVariableCompositionItemVariableExpressionMapping& propVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(otherExp);
				COptimizedComplexVariableCompositionItemVariableIndexMapping* propItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
				propVarItemIndexMapping.insertMulti(varRolePropItem, propItemIndexMapping);

				COptimizedComplexVariableIndividualMapping* propVarMapping = nullptr;
				if (created) {
					propVarMapping = new COptimizedComplexVariableIndividualMapping(newVarCount);
				}
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* propVarExp = it.value();
					propItemIndexMapping->insert(idx, propVarExp);
					if (created) {
						propVarMapping->setBindingMapping(idx, varMapping->getBindingMapping(idx));
					}
				}
				propItemIndexMapping->insert(newVarIdx, otherExp);
				if (created) {
					propVarMapping->setBindingMapping(newVarIdx, COptimizedComplexVariableIndividualMapping::INDIVIDUAL_VARIABLE);
					varRolePropItem->setVariableMapping(propVarMapping);
				}

				QSet<COptimizedComplexVariableCompositionItem*>*& otherVarCompItemSet = (*varExpVarComItemHash)[otherExp];
				if (!otherVarCompItemSet) {
					otherVarCompItemSet = new QSet<COptimizedComplexVariableCompositionItem*>();
				}
				otherVarCompItemSet->insert(varRolePropItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varCompItem);
				QStringList debugVariableNamePropItemStringList = debugGetItemVariableNames(buildingVarItem, otherExp, varRolePropItem);
				varRolePropItem->debugVariableNameStringList = debugVariableNamePropItemStringList;
				QString roleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
				if (inversed) {
					if (role->getInverseRole()->getPropertyNameLinker()) {
						roleName = CIRIName::getRecentIRIName(role->getInverseRole()->getPropertyNameLinker());
					} else {
						roleName += "^-";
					}
				}
				varRolePropItem->debugCreationString = "[" + debugVariableNamePropItemStringList.join(",") + "] obtained by propagating " + varExp->getName() + " from ["+debugVariableNamePrevItemStringList.join(",")+"] over role "+ roleName;
				debugCreationStringList += varRolePropItem->debugCreationString;
#endif

				buildingVarItem->updateLastHandledVariableItemAssociation(otherExp, varRolePropItem);

				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(varRolePropItem, propItemIndexMapping);

			}





			bool itemMappingSortLessBindingsThan(QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMapping1Pair, QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMapping2Pair) {
				return compItemMapping1Pair.first->getVariableMapping()->size() < compItemMapping2Pair.first->getVariableMapping()->size();
			}




			QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> COptimizedComplexConceptAnsweringHandler::reuseJoinedVariableCompositionItems(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp) {


				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);


				QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> compItemMappingPairList;
				for (COptimizedComplexVariableCompositionItemVariableExpressionMapping::const_iterator it = varItemIndexMapping.constBegin(), itEnd = varItemIndexMapping.constEnd(); it != itEnd; ++it) {
					COptimizedComplexVariableCompositionItem* joiningItem = it.key();
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = it.value();
					compItemMappingPairList.append(QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(joiningItem, itemIndexMapping));
				}

				// sort list
				qSort(compItemMappingPairList.begin(), compItemMappingPairList.end(), itemMappingSortLessBindingsThan);

				QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> compItemMappingPairPreJoinedList;
				while (!compItemMappingPairList.isEmpty()) {
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMappingPair = compItemMappingPairList.takeFirst();
					COptimizedComplexVariableCompositionItem* joiningItem1 = compItemMappingPair.first;
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1 = compItemMappingPair.second;

					bool joined = false;

					QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>>::iterator it = compItemMappingPairPreJoinedList.begin();
					while (it != compItemMappingPairPreJoinedList.end() && !joined) {
						QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> testedCompItemMappingPair(*it);
						COptimizedComplexVariableCompositionItem* joiningItem2 = testedCompItemMappingPair.first;
						COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2 = testedCompItemMappingPair.second;

						if (hasVariableJoiningItem(buildingVarItem, varExp, false, joiningItem1, joiningItem2, itemIndexMapping1, itemIndexMapping2)) {
							joined = true;


							QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> joinedItemMappingPair = buildVariableJoiningItem(buildingVarItem, varExp, false, joiningItem1, joiningItem2, itemIndexMapping1, itemIndexMapping2, nullptr);
							compItemMappingPairList.prepend(joinedItemMappingPair);
							compItemMappingPairPreJoinedList.erase(it);

						} else {
							++it;
						}
					}
					if (!joined) {
						compItemMappingPairPreJoinedList.append(compItemMappingPair);
					}
				}

				return compItemMappingPairPreJoinedList;

			}



			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexConceptAnsweringHandler::buildJoinedVariableJoiningItems(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool* processing) {

				cint64 joinCount = 0;
				cint64 reductionCount = 0;

				QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> compItemMappingPairPreJoinedList = reuseJoinedVariableCompositionItems(buildingVarItem, varExp);


				for (cint64 i = 0; i < compItemMappingPairPreJoinedList.size(); ) {

					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMappingPair = compItemMappingPairPreJoinedList.at(i);
					COptimizedComplexVariableCompositionItem* joiningItem = compItemMappingPair.first;
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = compItemMappingPair.second;
					QSet<CExpressionVariable*> varExpSet;
					bool sameVarExpJoiningRequired = false;
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator itMap = itemIndexMapping->constBegin(), itMapEnd = itemIndexMapping->constEnd(); !sameVarExpJoiningRequired && itMap != itMapEnd; ++itMap) {
						CExpressionVariable* usedVarExp(itMap.value());
						if (varExpSet.contains(usedVarExp)) {
							sameVarExpJoiningRequired = true;
						} else {
							varExpSet.insert(usedVarExp);
						}
					}
					if (sameVarExpJoiningRequired) {
						QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> joinedItemMappingPair = buildVariableJoiningItem(buildingVarItem, varExp, true, joiningItem, joiningItem, itemIndexMapping, itemIndexMapping, processing);
						compItemMappingPairPreJoinedList.replace(i, joinedItemMappingPair);
						joinCount++;
					} else {
						++i;
					}
				}

				QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> reducedCompItemMappingPairPreJoinedList;
				while (!compItemMappingPairPreJoinedList.isEmpty()) {
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMappingPair = compItemMappingPairPreJoinedList.takeFirst();
					COptimizedComplexVariableCompositionItem* joiningItem = compItemMappingPair.first;
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = compItemMappingPair.second;

					QHash<CExpressionVariable*, cint64> reductionVarExpIndHash = getReducableVariables(buildingVarItem, varExp, joiningItem, itemIndexMapping);
					compItemMappingPair = buildVariableReductionItem(buildingVarItem, varExp, joiningItem, itemIndexMapping, reductionVarExpIndHash, processing);

					if (joiningItem != compItemMappingPair.first) {
						++reductionCount;
					}

					reducedCompItemMappingPairPreJoinedList.append(compItemMappingPair);
				}



				QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMapping1Pair = reducedCompItemMappingPairPreJoinedList.takeFirst();
				COptimizedComplexVariableCompositionItem* joiningItem1 = compItemMapping1Pair.first;
				COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1 = compItemMapping1Pair.second;

				while (!reducedCompItemMappingPairPreJoinedList.isEmpty()) {
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMapping2Pair = reducedCompItemMappingPairPreJoinedList.takeFirst();
					COptimizedComplexVariableCompositionItem* joiningItem2 = compItemMapping2Pair.first;
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2 = compItemMapping2Pair.second;

					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> joinedItemMappingPair = buildVariableJoiningItem(buildingVarItem, varExp, false, joiningItem1, joiningItem2, itemIndexMapping1, itemIndexMapping2, processing);

					QHash<CExpressionVariable*, cint64> reductionVarExpIndHash = getReducableVariables(buildingVarItem, varExp, joinedItemMappingPair.first, joinedItemMappingPair.second);
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> reducedJoinedItemMappingPair = buildVariableReductionItem(buildingVarItem, varExp, joinedItemMappingPair.first, joinedItemMappingPair.second, reductionVarExpIndHash, processing);

					if (joinedItemMappingPair.first != reducedJoinedItemMappingPair.first) {
						++reductionCount;
					}

					joiningItem1 = reducedJoinedItemMappingPair.first;
					itemIndexMapping1 = reducedJoinedItemMappingPair.second;
					joinCount++;
				}

				if (mConfExtendedLogging) {
					LOG(INFO, getDomain(), logTr("Scheduling %2 joins with %3 reductions for items associated with variable %1.").arg(varExp->getName()).arg(joinCount).arg(reductionCount), this);
				}

				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(joiningItem1, itemIndexMapping1);
			}











			bool COptimizedComplexConceptAnsweringHandler::hasVariableJoiningItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool itemSameVarReduction, COptimizedComplexVariableCompositionItem* joiningItem1, COptimizedComplexVariableCompositionItem* joiningItem2, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2) {

				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();



				COptimizedComplexVariableCompositionItem* minLeftItem = qMin(joiningItem1, joiningItem2);
				COptimizedComplexVariableCompositionItem* maxRightItem = qMax(joiningItem1, joiningItem2);
				COptimizedComplexVariableJoiningBindingPositionMapping joiningBindingPositionMapping(maxRightItem);


				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);


				COptimizedComplexVariableCompositionItemVariableIndexMapping* minLeftItemIndexMapping = itemIndexMapping1;
				COptimizedComplexVariableCompositionItemVariableIndexMapping* maxRightItemIndexMapping = itemIndexMapping2;

				if (minLeftItem == joiningItem2) {
					minLeftItemIndexMapping = itemIndexMapping2;
					maxRightItemIndexMapping = itemIndexMapping1;
				}



				QHash<cint64, cint64> minLeftVarIdsCorrectionHash;
				QSet<CExpressionVariable*> sameItemVarExpMergedSet;
				QHash<CExpressionVariable*, cint64> minLeftItemVarExpIdxHash;
				QHash<CExpressionVariable*, cint64> maxRightItemVarExpIdxHash;
				QSet<CExpressionVariable*> commonVarExpSet;
				cint64 minLeftEliminationCount = 0;
				for (cint64 idx = 0; idx < minLeftItemIndexMapping->size(); ++idx) {
					CExpressionVariable* refVarExp = minLeftItemIndexMapping->value(idx);
					if (!itemSameVarReduction || !minLeftItemVarExpIdxHash.contains(refVarExp)) {
						minLeftItemVarExpIdxHash.insert(refVarExp, idx);
						minLeftVarIdsCorrectionHash.insert(idx, idx - minLeftEliminationCount);
					} else if (itemSameVarReduction && minLeftItem == maxRightItem) {
						if (!sameItemVarExpMergedSet.contains(refVarExp)) {
							sameItemVarExpMergedSet.insert(refVarExp);
							cint64 leftIdx = minLeftItemVarExpIdxHash.value(refVarExp);
							joiningBindingPositionMapping.addBindingPositionMapping(idx, leftIdx);
							minLeftEliminationCount++;
						} else {
							minLeftVarIdsCorrectionHash.insert(idx, idx - minLeftEliminationCount);
						}
					}
				}
				if (!itemSameVarReduction) {
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = maxRightItemIndexMapping->constBegin(), itEnd = maxRightItemIndexMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						CExpressionVariable* refVarExp = it.value();
						if (minLeftItemVarExpIdxHash.contains(refVarExp)) {
							commonVarExpSet.insert(refVarExp);
						}
						maxRightItemVarExpIdxHash.insert(refVarExp, idx);
					}
					for (CExpressionVariable* commVarExp : commonVarExpSet) {
						cint64 rightIdx = maxRightItemVarExpIdxHash.value(commVarExp);
						cint64 leftIdx = minLeftItemVarExpIdxHash.value(commVarExp);
						joiningBindingPositionMapping.addBindingPositionMapping(rightIdx, leftIdx);
					}
				}

				cint64 nextVarIdx = minLeftItemIndexMapping->count() - minLeftEliminationCount;
				for (QHash<CExpressionVariable*, cint64>::const_iterator it = maxRightItemVarExpIdxHash.constBegin(), itEnd = maxRightItemVarExpIdxHash.constEnd(); it != itEnd; ++it) {
					CExpressionVariable* refVarExp = it.key();
					cint64 idx = it.value();
					if (!commonVarExpSet.contains(refVarExp)) {
						joiningBindingPositionMapping.addBindingPositionMapping(idx, nextVarIdx++);
					}
				}
				joiningBindingPositionMapping.setSelfSameVariableReduction(itemSameVarReduction);


				return minLeftItem->hasJoiningItem(joiningBindingPositionMapping);
			}






			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexConceptAnsweringHandler::buildVariableJoiningItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool itemSameVarReduction, COptimizedComplexVariableCompositionItem* joiningItem1, COptimizedComplexVariableCompositionItem* joiningItem2, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2, bool* processing) {
				return buildVariableJoiningItem(buildingVarItem, varExp, itemSameVarReduction, varExp, joiningItem1, varExp, joiningItem2, itemIndexMapping1, itemIndexMapping2, processing);
			}



			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexConceptAnsweringHandler::buildVariableJoiningItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* destVarExp, bool itemSameVarReduction, CExpressionVariable* varExp1, COptimizedComplexVariableCompositionItem* joiningItem1, CExpressionVariable* varExp2, COptimizedComplexVariableCompositionItem* joiningItem2, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2, bool* processing) {


				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();



				COptimizedComplexVariableCompositionItem* minLeftItem = joiningItem1;
				COptimizedComplexVariableCompositionItem* maxRightItem = joiningItem2;

				CExpressionVariable* minLeftVarExp = varExp1;
				CExpressionVariable* maxRightVarExp = varExp2;

				COptimizedComplexVariableCompositionItemVariableIndexMapping* minLeftItemIndexMapping = itemIndexMapping1;
				COptimizedComplexVariableCompositionItemVariableIndexMapping* maxRightItemIndexMapping = itemIndexMapping2;


				COptimizedComplexVariableCompositionItemVariableExpressionMapping& destVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(destVarExp);

				if (joiningItem2 < joiningItem1) {
					minLeftItem = joiningItem2;
					maxRightItem = joiningItem1;

					minLeftVarExp = varExp2;
					maxRightVarExp = varExp1;

					minLeftItemIndexMapping = itemIndexMapping2;
					maxRightItemIndexMapping = itemIndexMapping1;

				}

				COptimizedComplexVariableCompositionItemVariableExpressionMapping& minLeftVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(minLeftVarExp);
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& maxRightVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(maxRightVarExp);


				QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> minLeftItemIndexMappingList = minLeftVarItemIndexMapping.values(minLeftItem);
				QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> maxRightItemIndexMappingList = maxRightVarItemIndexMapping.values(maxRightItem);

				COptimizedComplexVariableJoiningBindingPositionMapping joiningBindingPositionMapping(maxRightItem);


				if (!minLeftItemIndexMapping) {
					minLeftItemIndexMapping = minLeftItemIndexMappingList.takeFirst();
				} else {
					minLeftItemIndexMappingList.removeOne(minLeftItemIndexMapping);
				}
				if (!maxRightItemIndexMapping) {
					maxRightItemIndexMapping = maxRightItemIndexMappingList.takeFirst();
				} else {
					maxRightItemIndexMappingList.removeOne(maxRightItemIndexMapping);
				}



				QHash<cint64, cint64> minLeftVarIdsCorrectionHash;
				QSet<CExpressionVariable*> sameItemVarExpMergedSet;
				QHash<CExpressionVariable*, cint64> minLeftItemVarExpIdxHash;
				QHash<CExpressionVariable*,cint64> maxRightItemVarExpIdxHash;
				QSet<CExpressionVariable*> commonVarExpSet;
				cint64 minLeftEliminationCount = 0;
				for (cint64 idx = 0; idx < minLeftItemIndexMapping->size(); ++idx) {
					CExpressionVariable* refVarExp = minLeftItemIndexMapping->value(idx);
					if (!itemSameVarReduction || !minLeftItemVarExpIdxHash.contains(refVarExp)) {
						minLeftItemVarExpIdxHash.insert(refVarExp, idx);
						minLeftVarIdsCorrectionHash.insert(idx, idx - minLeftEliminationCount);
					} else if (itemSameVarReduction && minLeftItem == maxRightItem) {
						if (!sameItemVarExpMergedSet.contains(refVarExp)) {
							sameItemVarExpMergedSet.insert(refVarExp);
							cint64 leftIdx = minLeftItemVarExpIdxHash.value(refVarExp);
							joiningBindingPositionMapping.addBindingPositionMapping(idx, leftIdx);
							minLeftEliminationCount++;
						} else {
							minLeftVarIdsCorrectionHash.insert(idx, idx - minLeftEliminationCount);
						}
					}
				}
				if (!itemSameVarReduction) {
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = maxRightItemIndexMapping->constBegin(), itEnd = maxRightItemIndexMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						CExpressionVariable* refVarExp = it.value();
						if (minLeftItemVarExpIdxHash.contains(refVarExp)) {
							commonVarExpSet.insert(refVarExp);
						}
						maxRightItemVarExpIdxHash.insert(refVarExp, idx);
					}
					for (CExpressionVariable* commVarExp : commonVarExpSet) {
						cint64 rightIdx = maxRightItemVarExpIdxHash.value(commVarExp);
						cint64 leftIdx = minLeftItemVarExpIdxHash.value(commVarExp);
						joiningBindingPositionMapping.addBindingPositionMapping(rightIdx, leftIdx);
					}
				}

				cint64 nextVarIdx = minLeftItemIndexMapping->count() - minLeftEliminationCount;
				for (QHash<CExpressionVariable*, cint64>::const_iterator it = maxRightItemVarExpIdxHash.constBegin(), itEnd = maxRightItemVarExpIdxHash.constEnd(); it != itEnd; ++it) {
					CExpressionVariable* refVarExp = it.key();
					cint64 idx = it.value();
					if (!commonVarExpSet.contains(refVarExp)) {
						joiningBindingPositionMapping.addBindingPositionMapping(idx, nextVarIdx++);
					}
				}
				joiningBindingPositionMapping.setSelfSameVariableReduction(itemSameVarReduction);



				COptimizedComplexVariableJoiningItem*& joinedItem = minLeftItem->getJoiningItem(joiningBindingPositionMapping);

				bool createdJoinedItem = false;
				if (!joinedItem) {
					createdJoinedItem = true;
					joinedItem = new COptimizedComplexVariableJoiningItem(minLeftItem, maxRightItem, joiningBindingPositionMapping);
				}

				COptimizedComplexVariableCompositionItemVariableIndexMapping* joinedItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
				COptimizedComplexVariableIndividualMapping* joinedVarMapping = nullptr;
				if (createdJoinedItem) {
					joinedVarMapping = new COptimizedComplexVariableIndividualMapping(nextVarIdx);
				}

				COptimizedComplexVariableIndividualMapping* minLeftVarMapping = minLeftItem->getVariableMapping();
				COptimizedComplexVariableIndividualMapping* maxRightVarMapping = maxRightItem->getVariableMapping();
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = minLeftItemIndexMapping->constBegin(), itEnd = minLeftItemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					if (minLeftVarIdsCorrectionHash.contains(idx)) {
						cint64 corrIdx = minLeftVarIdsCorrectionHash.value(idx);
						CExpressionVariable* joinVarExp = it.value();
						joinedItemIndexMapping->insert(corrIdx, joinVarExp);
						if (createdJoinedItem) {
							joinedVarMapping->setBindingMapping(corrIdx, minLeftVarMapping->getBindingMapping(idx));
						}
					}
				}
				if (!itemSameVarReduction) {
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = maxRightItemIndexMapping->constBegin(), itEnd = maxRightItemIndexMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						CExpressionVariable* joinVarExp = it.value();
						if (!commonVarExpSet.contains(joinVarExp)) {
							cint64 mappedVarIdx = joiningBindingPositionMapping.getMappedBindingPosition(idx);
							joinedItemIndexMapping->insert(mappedVarIdx, joinVarExp);
							if (createdJoinedItem) {
								joinedVarMapping->setBindingMapping(mappedVarIdx, maxRightVarMapping->getBindingMapping(idx));
							}
						}
					}
				}
				// insert new mapping
				if (createdJoinedItem) {
					joinedItem->setVariableMapping(joinedVarMapping);
				}
				destVarItemIndexMapping.insertMulti(joinedItem, joinedItemIndexMapping);


				varExpVarComItemHash->value(destVarExp)->insert(joinedItem);

				if (!joinedItem->isVariableMappingComputed()) {
					if (processing) {
						*processing = true;
					}
					// schedule item processing
					queueVariableItemComputation(buildingVarItem, joinedItem);
				}

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNameLeftItemStringList = debugGetItemVariableNames(buildingVarItem, minLeftVarExp, minLeftItem);
				QStringList debugVariableNameRightItemStringList = debugGetItemVariableNames(buildingVarItem, maxRightVarExp, maxRightItem);

				QStringList debugVariableNameJoinedItemStringList = debugGetItemVariableNames(buildingVarItem, destVarExp, joinedItem);
				joinedItem->debugVariableNameStringList = debugVariableNameJoinedItemStringList;
				joinedItem->debugCreationString = "[" + debugVariableNameJoinedItemStringList.join(",") + "] obtained by joining " + destVarExp->getName() + " from [" + debugVariableNameLeftItemStringList.join(",") + "] and [" + debugVariableNameRightItemStringList.join(", ") + "]";
				debugCreationStringList += joinedItem->debugCreationString;
#endif


				// re-add remaining mappings
				if (minLeftVarExp == destVarExp) {
					minLeftVarItemIndexMapping.remove(minLeftItem);
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping* mapping : minLeftItemIndexMappingList) {
						minLeftVarItemIndexMapping.insertMulti(minLeftItem, mapping);
					}
					varExpVarComItemHash->value(minLeftVarExp)->remove(minLeftItem);
				}
				if (maxRightVarExp == destVarExp) {
					maxRightVarItemIndexMapping.remove(maxRightItem);
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping* mapping : maxRightItemIndexMappingList) {
						maxRightVarItemIndexMapping.insertMulti(maxRightItem, mapping);
					}
					varExpVarComItemHash->value(maxRightVarExp)->remove(maxRightItem);
				}
				buildingVarItem->updateLastHandledVariableItemAssociation(destVarExp, joinedItem);


				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(joinedItem, joinedItemIndexMapping);

			}








			bool COptimizedComplexConceptAnsweringHandler::finishSubClassCalculationStepProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep, CAnswererContext* answererContext) {
				delete conceptItem->getPossibleSubClassNodeTestingList();
				conceptItem->setPossibleSubClassNodeTestingList(nullptr);
				delete conceptItem->getPossibleSubClassTestingNodeSet();
				conceptItem->setPossibleSubClassTestingNodeSet(nullptr);
				delete conceptItem->getMaximumSubClassNodeSet();
				conceptItem->setMaximumSubClassNodeSet(nullptr);
				return finishCalculationStepProcessing(conceptItem, compStep, answererContext);
			}





			bool COptimizedComplexConceptAnsweringHandler::initializeRealization(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {
				CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				if (!conceptRealization || !roleRealization) {
					QList<COntologyProcessingRequirement*> reqList;
					COntologyProcessingInitializedRealizationRequirement* initRealReq = new COntologyProcessingInitializedRealizationRequirement();
					reqList.append(initRealReq);
					CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(conceptItem);
					processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
					conceptItem->incLazyRealizationProcessingCount();
					return true;
				}
				return false;
			}



			bool COptimizedComplexConceptAnsweringHandler::initializeRealizationInstancesIterators(COptimizedComplexConceptItem* conceptItem, cint64 requiredInstancesCount, CAnswererContext* answererContext) {
				QSet<CHierarchyNode*>* superHierNodeSet = conceptItem->getDirectSuperClassNodeSet();
				QSet<CHierarchyNode*>* subHierNodeSet = conceptItem->getDirectSubClassNodeSet();
				CHierarchyNode* topHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getTopHierarchyNode();
				CHierarchyNode* bottomHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getBottomHierarchyNode();

				CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();

				QList<CRealizationIndividualInstanceItemReferenceIterator*> realizationIteratorList;
				bool unsatisfiable = false;

				QList<COntologyProcessingRequirement*> reqList;
				bool subInsteadOfTopHierNodeRealization = false;
				if (superHierNodeSet) {
					for (CHierarchyNode* node : *superHierNodeSet) {
						if (node != topHierNode) {
							if (mConfDirectPossibleRealizationRequesting) {
								COntologyProcessingConceptRealizationRequirement* conRealReq = new COntologyProcessingConceptRealizationRequirement(CIndividualReference(), node->getOneEquivalentConcept());
								reqList.append(conRealReq);
							}
							CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptRealization->getConceptInstancesIterator(node->getOneEquivalentConcept(), false, mDefaultRealizationSorting);
							realizationIteratorList.append(iterator);
							if (iterator->atEnd() && !iterator->requiresInitialization()) {
								unsatisfiable = true;
								break;
							}
						} else {
							subInsteadOfTopHierNodeRealization = true;
						}
					}
				} else {
					for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {
						CConcept* concept = conNegPair.first;
						bool negation = conNegPair.second;
						if (!negation && (concept->getOperatorCode() == CCATOM || concept->getOperatorCode() == CCSUB || concept->getOperatorCode() == CCEQ)) {
							if (mConfDirectPossibleRealizationRequesting) {
								COntologyProcessingConceptRealizationRequirement* conRealReq = new COntologyProcessingConceptRealizationRequirement(CIndividualReference(), concept);
								reqList.append(conRealReq);
							}
							CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptRealization->getConceptInstancesIterator(concept, false, mDefaultRealizationSorting);
							realizationIteratorList.append(iterator);
							if (iterator->atEnd() && !iterator->requiresInitialization()) {
								unsatisfiable = true;
								break;
							}

						}
					}
				}
				if (subInsteadOfTopHierNodeRealization && subHierNodeSet && !unsatisfiable) {
					for (CHierarchyNode* node : *subHierNodeSet) {
						if (node != bottomHierNode) {
							if (mConfDirectPossibleRealizationRequesting) {
								COntologyProcessingConceptRealizationRequirement* conRealReq = new COntologyProcessingConceptRealizationRequirement(CIndividualReference(), node->getOneEquivalentConcept());
								reqList.append(conRealReq);
							}
						}
					}
				}

				typedef QPair<CRole*, CDataLiteral*> TRoleDataLiteralPair;

				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();
				CConcept* topDataRangeConcept = mOntoAnsweringItem->getOntology()->getTBox()->getTopDataRangeConcept();

				QSet<CRole*> roleRequiredNeighboursSet;
				QSet<CRole*> roleRequiredExistentialSet;
				QSet<TRoleIndividualPair> roleRequiredSuccIndiSet;
				QSet<TRoleDataLiteralPair> roleDataLiteralRequiredInstantiatingSet;
				QSet<CRole*> roleRequiredDataSet;

				if (!unsatisfiable) {
					for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {
						CConcept* concept = conNegPair.first;
						bool negation = conNegPair.second;
						if (!negation && concept->getOperatorCode() == CCVALUE) {
							CIndividual* nomIndividual = concept->getNominalIndividual();
							CRole* role = concept->getRole();
							roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndividual));

						} else if (!negation && concept->getOperatorCode() == CCSOME) {
							CRole* role = concept->getRole();
							if (!role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
								if (concept->getOperandList()->getData() == indiTriggerConcept && !concept->getOperandList()->isNegated()) {
									roleRequiredNeighboursSet.insert(role);
								} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !concept->getOperandList()->isNegated()) {
									CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
									roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
								} else {
									roleRequiredExistentialSet.insert(role);
								}
							}

							if (role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
								CConcept* opConcept = concept->getOperandList()->getData();
								bool opConNegation = concept->getOperandList()->isNegated();

								if (opConcept == topDataRangeConcept && !opConNegation) {
									roleRequiredDataSet.insert(role);
								} else if (opConcept->getOperatorCode() == CCDATALITERAL && !opConNegation) {
									if (canRetrieveInstantiatingDataValueIndividuals(role, opConcept->getDataLiteral())) {
										roleDataLiteralRequiredInstantiatingSet.insert(TRoleDataLiteralPair(role, opConcept->getDataLiteral()));
									} else {
										roleRequiredDataSet.insert(role);
									}
								}
							}

						} else if (negation && concept->getOperatorCode() == CCALL) {
							CRole* role = concept->getRole();
							if (!role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
								if (concept->getOperandList()->getData() == indiTriggerConcept && concept->getOperandList()->isNegated()) {
									roleRequiredNeighboursSet.insert(role);
								} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && concept->getOperandList()->isNegated()) {
									CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
									roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
								} else {
									roleRequiredExistentialSet.insert(role);
								}
							}


							if (role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
								CConcept* opConcept = concept->getOperandList()->getData();
								bool opConNegation = concept->getOperandList()->isNegated();

								if (opConcept == topDataRangeConcept && opConNegation) {
									roleRequiredDataSet.insert(role);
								} else if (opConcept->getOperatorCode() == CCDATALITERAL && opConNegation) {
									if (canRetrieveInstantiatingDataValueIndividuals(role, opConcept->getDataLiteral())) {
										roleDataLiteralRequiredInstantiatingSet.insert(TRoleDataLiteralPair(role, opConcept->getDataLiteral()));									
									} else {
										roleRequiredDataSet.insert(role);
									}
								}
							}

						} else if (!negation && concept->getOperatorCode() == CCAQCHOOCE) {
							CRole* role = nullptr;
							if (concept->getOperandCount() >= 1) {
								CConcept* aqChOpConcept = concept->getOperandList()->getData();
								if (aqChOpConcept && aqChOpConcept->getOperatorCode() == CCAQSOME && !concept->getOperandList()->isNegated()) {
									role = aqChOpConcept->getRole();

									if (aqChOpConcept->getOperandCount() == 1 && aqChOpConcept->getOperandList()->getData()) {
										if (aqChOpConcept->getOperandList()->getData() == indiTriggerConcept && !aqChOpConcept->getOperandList()->isNegated()) {
											roleRequiredNeighboursSet.insert(role);
										} else if (aqChOpConcept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !aqChOpConcept->getOperandList()->isNegated()) {
											CIndividual* nomIndi = aqChOpConcept->getOperandList()->getData()->getNominalIndividual();
											roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
										} else {
											roleRequiredExistentialSet.insert(role);
										}
									}

								}
							}
						}
					}
				}
				if (!roleDataLiteralRequiredInstantiatingSet.isEmpty() && !unsatisfiable) {
					for (const TRoleDataLiteralPair& roleDataLiteralPair : roleDataLiteralRequiredInstantiatingSet) {
						CRole* role = roleDataLiteralPair.first;
						CDataLiteral* dataLiteral = roleDataLiteralPair.second;
						QMap<cint64, IndividualIdCandidateData>* instantiatingDataValueIndividualIdMap = getInstantiatingDataValueIndividualIdMap(role, dataLiteral, nullptr, answererContext);
						if (!instantiatingDataValueIndividualIdMap->isEmpty()) {
							CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptRealization->getIterationCandidateIndividualIdIterator(*instantiatingDataValueIndividualIdMap, mDefaultRealizationSorting);
							realizationIteratorList.append(iterator);
							if (iterator->atEnd() && !iterator->requiresInitialization()) {
								unsatisfiable = true;
								break;
							}
						} else {
							unsatisfiable = true;
							break;
						}
						delete instantiatingDataValueIndividualIdMap;
					}
				}
				if (!roleRequiredDataSet.isEmpty() && !unsatisfiable) {
					for (CRole* role : roleRequiredDataSet) {
						CRealizationIndividualInstanceItemReferenceIterator* iterator = roleRealization->getRoleDataInstancesIterator(role, mDefaultRealizationSorting);
						realizationIteratorList.append(iterator);
						if (iterator->atEnd() && !iterator->requiresInitialization()) {
							unsatisfiable = true;
						}
					}
				}
				if (!roleRequiredNeighboursSet.isEmpty() || !roleRequiredSuccIndiSet.isEmpty() || !roleRequiredExistentialSet.isEmpty()) {
					if (!unsatisfiable) {
						for (CRole* role : roleRequiredNeighboursSet) {
							if (mConfDirectPossibleRealizationRequesting) {
								COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role, false);
								reqList.append(roleRealReq);
							}
							CRealizationIndividualInstanceItemReferenceIterator* iterator = roleRealization->getRoleNeighbouringInstancesIterator(role, false, mDefaultRealizationSorting);
							realizationIteratorList.append(iterator);
							if (iterator->atEnd() && !iterator->requiresInitialization()) {
								unsatisfiable = true;
								break;
							}
						}
					}
					if (!unsatisfiable) {
						for (const TRoleIndividualPair& roleIndiPair : roleRequiredSuccIndiSet) {
							CRole* role = roleIndiPair.first;
							CIndividual* nomIndividual = roleIndiPair.second;
							if (mConfDirectPossibleRealizationRequesting) {
								COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role, true, nomIndividual);
								reqList.append(roleRealReq);
							}
							CRealizationIndividualInstanceItemReferenceIterator* iterator = roleRealization->getRoleFillerInstancesIterator(roleRealization->getRoleInstanceItemReference(nomIndividual), role, true, mDefaultRealizationSorting);
							realizationIteratorList.append(iterator);
							if (iterator->atEnd() && !iterator->requiresInitialization()) {
								unsatisfiable = true;
								break;
							}
						}
					}
					if (!unsatisfiable && !roleRequiredExistentialSet.isEmpty()) {
						for (CRole* role : roleRequiredExistentialSet) {
							if (!roleRequiredNeighboursSet.contains(role)) {
								CRealizationIndividualInstanceItemReferenceIterator* iterator = roleRealization->getRoleExistentialInstancesIterator(role, false, mDefaultRealizationSorting);
								realizationIteratorList.append(iterator);
								if (iterator->atEnd() && !iterator->requiresInitialization()) {
									unsatisfiable = true;
									break;
								}
							}
						}
					}
				}


				if (!unsatisfiable && realizationIteratorList.isEmpty()) {
					CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptRealization->getConceptInstancesIterator(topHierNode->getOneEquivalentConcept(), false, mDefaultRealizationSorting);
					realizationIteratorList.append(iterator);
					if (iterator->atEnd() && !iterator->requiresInitialization()) {
						unsatisfiable = true;
					}
				}


				if (!realizationIteratorList.isEmpty()) {
					if (!unsatisfiable) {
						CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptRealization->getIterationIntersectionIterator(realizationIteratorList);
						if (iterator->requiresInitialization()) {
							COntologyProcessingIteratorRealizationRequirement* roleRealReq = new COntologyProcessingIteratorRealizationRequirement(iterator, requiredInstancesCount);
							reqList.append(roleRealReq);
						}
						conceptItem->setRealizationIterator(iterator);
					} else {
						qDeleteAll(realizationIteratorList);
					}
				}


				if (!reqList.isEmpty()) {
					if (!unsatisfiable) {
						CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(conceptItem);
						processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
						conceptItem->incLazyRealizationProcessingCount();
						return true;
					} else {
						qDeleteAll(reqList);
					}
				}
				return false;
			}



			bool COptimizedComplexConceptAnsweringHandler::initializeKnownPossibleInstances(COptimizedComplexConceptItem* conceptItem, cint64 requiredInstancesCount, CAnswererContext* answererContext) {

				QSet<CRealizationIndividualInstanceItemReference>* directInstanceItemSet = conceptItem->getKnownInstanceItemSet();
				if (!directInstanceItemSet) {
					directInstanceItemSet = new QSet<CRealizationIndividualInstanceItemReference>();
					conceptItem->setKnownInstanceItemSet(directInstanceItemSet);
				}

				bool completelyHandled = conceptItem->getAllAtomicConceptsCompletelyHandled();
				cint64 certainInstances = conceptItem->getKnownInstanceItemSet()->size();

				CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptItem->getRealizationIterator();
				if (iterator) {
					iterator->begin();

					QSet<CRealizationIndividualInstanceItemReference>* possiblenstanceItemSet = conceptItem->getPossibleInstanceItemSet();
					if (!possiblenstanceItemSet) {
						possiblenstanceItemSet = new QSet<CRealizationIndividualInstanceItemReference>();
						conceptItem->setPossibleInstanceItemSet(possiblenstanceItemSet);
					}
					cint64 visitedIndis = 0;
					while (!iterator->atEnd() && !iterator->requiresInitialization() && (requiredInstancesCount < 0 || visitedIndis < requiredInstancesCount)) {
						CRealizationIndividualInstanceItemReference indiItemRef = iterator->currentIndividualInstanceItemReference();
						if (completelyHandled && iterator->isCurrentInstanceCertain()) {
							directInstanceItemSet->insert(indiItemRef);
						} else {
							possiblenstanceItemSet->insert(indiItemRef);
						}
						iterator->moveNext();
						conceptItem->inRealizationRetrievedInstanceCandidateCount();
						++visitedIndis;
					}


					cint64 newCertainInstances = conceptItem->getKnownInstanceItemSet()->size() - certainInstances;
					conceptItem->setLastRetrievedCertainInstanceItemCount(newCertainInstances);

					if (iterator->requiresInitialization()) {
						QList<COntologyProcessingRequirement*> reqList;
						COntologyProcessingIteratorRealizationRequirement* roleRealReq = new COntologyProcessingIteratorRealizationRequirement(iterator, requiredInstancesCount);
						reqList.append(roleRealReq);

						CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(conceptItem);
						processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
						conceptItem->incLazyRealizationProcessingCount();
						return true;
					} else if (conceptItem->getRealizationIterator()->atEnd()) {
						conceptItem->setLazyRealizationInstancesRetrieved(true);
					}
				} else {
					conceptItem->setLazyRealizationInstancesRetrieved(true);
				}

				return false;
			}






			bool COptimizedComplexConceptAnsweringHandler::schedulePossibleInstancesNeighourRealization(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {
				QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();

				QSet<CRole*> roleRequiredNeighboursSet;
				QSet<TRoleIndividualPair> roleRequiredSuccIndiSet;
				for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {
					CConcept* concept = conNegPair.first;
					bool negation = conNegPair.second;
					if (!negation && concept->getOperatorCode() == CCSOME) {
						CRole* role = concept->getRole();
						if (concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && !concept->getOperandList()->isNegated()) {
								roleRequiredNeighboursSet.insert(role);
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !concept->getOperandList()->isNegated()) {
								CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
								roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
							}
						}

					} else if (negation && concept->getOperatorCode() == CCALL) {
						CRole* role = concept->getRole();
						if (concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && concept->getOperandList()->isNegated()) {
								roleRequiredNeighboursSet.insert(role);
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && concept->getOperandList()->isNegated()) {
								CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
								roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
							}
						}

					} else if (!negation && concept->getOperatorCode() == CCAQCHOOCE) {
						CRole* role = nullptr;
						if (concept->getOperandCount() >= 1) {
							CConcept* aqChOpConcept = concept->getOperandList()->getData();
							if (aqChOpConcept && aqChOpConcept->getOperatorCode() == CCAQSOME && !concept->getOperandList()->isNegated()) {
								role = aqChOpConcept->getRole();

								if (aqChOpConcept->getOperandCount() == 1 && aqChOpConcept->getOperandList()->getData()) {
									if (aqChOpConcept->getOperandList()->getData() == indiTriggerConcept && !concept->getOperandList()->isNegated()) {
										roleRequiredNeighboursSet.insert(role);
									} else if (aqChOpConcept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !aqChOpConcept->getOperandList()->isNegated()) {
										CIndividual* nomIndi = aqChOpConcept->getOperandList()->getData()->getNominalIndividual();
										roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
									}
								}

							}
						}

					}

				}
				if (!roleRequiredNeighboursSet.isEmpty() || !roleRequiredSuccIndiSet.isEmpty()) {
					conceptItem->setLazyNeighbourRealizationPruningRequired(true);
					CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();

					class CIndividualTestingVisitor : public CSameRealizationIndividualVisitor {
					public:
						CIndividualReference testingIndiRef;

						bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
							testingIndiRef = indiRef;
							return false;
						}
					} visitor;

					QList<COntologyProcessingRequirement*> reqList;
					for (CRole* role : roleRequiredNeighboursSet) {
						for (CRealizationIndividualInstanceItemReference possInstItem : *possibleDirectInstanceItemSet) {
							sameRealization->visitSameIndividuals(possInstItem, &visitor);
							COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role, false, visitor.testingIndiRef);
							reqList.append(roleRealReq);
						}
					}
					for (const TRoleIndividualPair& roleIndiPair : roleRequiredSuccIndiSet) {
						CRole* role = roleIndiPair.first;
						CIndividual* succIndi = roleIndiPair.second;
						for (CRealizationIndividualInstanceItemReference possInstItem : *possibleDirectInstanceItemSet) {
							sameRealization->visitSameIndividuals(possInstItem, &visitor);
							COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role, false, visitor.testingIndiRef, succIndi);
							reqList.append(roleRealReq);
						}
					}
					CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(conceptItem);
					processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
					conceptItem->incLazyRealizationProcessingCount();
					return true;
				}
				return false;
			}









			bool COptimizedComplexConceptAnsweringHandler::checkConceptInstancesCompletelyResolveable(COptimizedComplexConceptItem* conceptItem, QSet<TConceptNegPair>* remainingConNegPair) {


				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();

				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();
				CConcept* topDataRangeConcept = mOntoAnsweringItem->getOntology()->getTBox()->getTopDataRangeConcept();
				CConcept* topConcept = mOntoAnsweringItem->getOntology()->getTBox()->getTopConcept();

				bool allConceptCompletelyHandled = true;


				for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {

					bool conceptCompletelyHandled = false;

					CConcept* concept = conNegPair.first;
					bool negation = conNegPair.second;
					if (!negation && concept->getOperatorCode() == CCSOME) {
						// TODO: we should check for the special concept O here instead of top
						CRole* role = concept->getRole();
						if (!role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && !concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							} else if (concept->getOperandList()->getData() == topConcept && !concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							}
						}

						if (role->isDataRole() && !conceptCompletelyHandled && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {

							CConcept* opConcept = concept->getOperandList()->getData();
							bool opConNegation = concept->getOperandList()->isNegated();

							if (opConcept == topDataRangeConcept && !opConNegation) {
								conceptCompletelyHandled = true;
							} else if (opConcept->getOperatorCode() == CCDATALITERAL && !opConNegation) {
								if (canRetrieveInstantiatingDataValueIndividuals(role, opConcept->getDataLiteral())) {
									conceptCompletelyHandled = true;
								}
							}
						}

					} else if (negation && concept->getOperatorCode() == CCALL) {
						// TODO: we should check for the special concept O here instead of top
						CRole* role = concept->getRole();
						if (!role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							} else if (concept->getOperandList()->getData() == topConcept && concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							}
						}

						if (role->isDataRole() && !conceptCompletelyHandled && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {

							CConcept* opConcept = concept->getOperandList()->getData();
							bool opConNegation = concept->getOperandList()->isNegated();

							if (opConcept == topDataRangeConcept && opConNegation) {
								conceptCompletelyHandled = true;
							} else if (opConcept->getOperatorCode() == CCDATALITERAL && opConNegation) {
								if (canRetrieveInstantiatingDataValueIndividuals(role, opConcept->getDataLiteral())) {
									conceptCompletelyHandled = true;
								}
							}
						}
					} else if (!negation && concept->getOperatorCode() == CCVALUE) {
						conceptCompletelyHandled = true;
					} else if (!negation && concept->hasClassName()) {
						conceptCompletelyHandled = true;
					} else if (!negation && concept->getOperatorCode() == CCAQCHOOCE) {
						bool indiTriggerConceptRestriction = false;
						if (concept->getOperandCount() >= 1) {
							CConcept* aqChOpConcept = concept->getOperandList()->getData();
							if (aqChOpConcept && aqChOpConcept->getOperatorCode() == CCAQSOME && !concept->getOperandList()->isNegated()) {

								if (aqChOpConcept->getOperandCount() == 1) {
									if (aqChOpConcept->getOperandList()->getData() == indiTriggerConcept && !aqChOpConcept->getOperandList()->isNegated()) {
										indiTriggerConceptRestriction = true;
									} else if (aqChOpConcept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !aqChOpConcept->getOperandList()->isNegated()) {
										indiTriggerConceptRestriction = true;
									} else if (aqChOpConcept->getOperandList()->getData() == topConcept && !aqChOpConcept->getOperandList()->isNegated()) {
										conceptCompletelyHandled = true;
									}
								}

							}
						}
						if (indiTriggerConceptRestriction) {
							conceptCompletelyHandled = true;
						}
					}

					if (!conceptCompletelyHandled) {
						allConceptCompletelyHandled = false;
						if (remainingConNegPair) {
							remainingConNegPair->insert(conNegPair);
						}
					}
				}

				return allConceptCompletelyHandled;
			}




			bool COptimizedComplexConceptAnsweringHandler::checkPossibleInstancesCompletelyHandled(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {

				if (!conceptItem->getCompletelyHandledReducedInstanceTestingConcept()) {

					QSet<TConceptNegPair> remainingConNegPair;
					bool allConceptCompletelyHandled = checkConceptInstancesCompletelyResolveable(conceptItem, &remainingConNegPair);
					conceptItem->setCompletelyHandledReducedInstanceTestingConcept(conceptItem->getConcept());
					conceptItem->setCompletelyHandledReducedInstanceTestingConceptNegation(conceptItem->getConceptNegation());

					conceptItem->setAllAtomicConceptsCompletelyHandled(allConceptCompletelyHandled);
					if (allConceptCompletelyHandled) {
						QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
						QSet<CRealizationIndividualInstanceItemReference>* knownDirectInstanceItemSet = conceptItem->getKnownInstanceItemSet();
						if (possibleDirectInstanceItemSet) {
							mTestedPossibleInstancesCount += possibleDirectInstanceItemSet->size();
							knownDirectInstanceItemSet->operator+=(*possibleDirectInstanceItemSet);
							possibleDirectInstanceItemSet->clear();
						}
						return true;
					} else {
						// create reduced testing concept
						CMemoryAllocationManager* memMan = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
						CConceptVector* conceptVec = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getTBox()->getConceptVector();

						CConcept* reducedConcept = CObjectAllocator< CConcept >::allocateAndConstruct(memMan);
						reducedConcept->initConcept();
						cint64 nextConTag = conceptVec->getItemCount();
						reducedConcept->initTag(nextConTag);
						reducedConcept->setOperatorCode(CCAND);
						conceptVec->setLocalData(nextConTag, reducedConcept);

						for (TConceptNegPair conNegPar : remainingConNegPair) {
							CConcept* opConcept = conNegPar.first;
							bool opConceptNegation = conNegPar.second;

							CSortedNegLinker<CConcept*>* newIndiTrigOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
							newIndiTrigOpCon->init(opConcept, opConceptNegation, reducedConcept->getOperandList());
							reducedConcept->setOperandList(newIndiTrigOpCon);
							reducedConcept->incOperandCount();
						}
						conceptItem->setCompletelyHandledReducedInstanceTestingConcept(reducedConcept);
						conceptItem->setCompletelyHandledReducedInstanceTestingConceptNegation(false);

					}
				} else if (conceptItem->getAllAtomicConceptsCompletelyHandled()) {
					QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
					QSet<CRealizationIndividualInstanceItemReference>* knownDirectInstanceItemSet = conceptItem->getKnownInstanceItemSet();
					if (possibleDirectInstanceItemSet) {
						mTestedPossibleInstancesCount += possibleDirectInstanceItemSet->size();
						knownDirectInstanceItemSet->operator+=(*possibleDirectInstanceItemSet);
						possibleDirectInstanceItemSet->clear();
					}
					return true;
				}
				return false;
			}







			bool COptimizedComplexConceptAnsweringHandler::neighboursPrunePossibleInstances(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {
				CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();

				QSet<CRole*> roleRequiredNeighboursSet;
				QSet<TRoleIndividualPair> roleRequiredSuccIndiSet;
				for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {
					CConcept* concept = conNegPair.first;
					bool negation = conNegPair.second;
					if (!negation && concept->getOperatorCode() == CCSOME) {
						CRole* role = concept->getRole();
						if (concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && !concept->getOperandList()->isNegated()) {
								roleRequiredNeighboursSet.insert(role);
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !concept->getOperandList()->isNegated()) {
								CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
								roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
							}
						}
					} else if (negation && concept->getOperatorCode() == CCALL) {
						CRole* role = concept->getRole();
						if (concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && concept->getOperandList()->isNegated()) {
								roleRequiredNeighboursSet.insert(role);
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && concept->getOperandList()->isNegated()) {
								CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
								roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
							}
						}
					} else if (!negation && concept->getOperatorCode() == CCAQCHOOCE) {
						CRole* role = nullptr;
						if (concept->getOperandCount() >= 1) {
							CConcept* aqChOpConcept = concept->getOperandList()->getData();
							if (aqChOpConcept && aqChOpConcept->getOperatorCode() == CCAQSOME && !concept->getOperandList()->isNegated()) {
								role = aqChOpConcept->getRole();

								if (aqChOpConcept->getOperandCount() == 1 && aqChOpConcept->getOperandList()->getData()) {
									if (aqChOpConcept->getOperandList()->getData() == indiTriggerConcept && !aqChOpConcept->getOperandList()->isNegated()) {
										roleRequiredNeighboursSet.insert(role);
									} else if (aqChOpConcept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !aqChOpConcept->getOperandList()->isNegated()) {
										CIndividual* nomIndi = aqChOpConcept->getOperandList()->getData()->getNominalIndividual();
										roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
									}
								}

							}
						}
					}
				}
				

				if (roleRealization && sameRealization) {
					class CPossibleInstancesRoleNeighbourPruningVisitor : public CRoleRealizationInstanceVisitor {
					public:
						bool mHasRoleNeighbourFlag = false;

						bool visitRoleInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealization* roleRealization) {
							mHasRoleNeighbourFlag = true;
							return false;
						}
					};

					class CIndividualTestingVisitor : public CSameRealizationIndividualVisitor {
					public:
						CIndividualReference testingIndiRef;

						bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
							testingIndiRef = indiRef;
							return false;
						}
					} sameInstanceVisitor;

					CPossibleInstancesRoleNeighbourPruningVisitor roleInstanceVisitor;
					for (QSet<CRealizationIndividualInstanceItemReference>::iterator it = possibleDirectInstanceItemSet->begin(); it != possibleDirectInstanceItemSet->end(); ) {
						CRealizationIndividualInstanceItemReference possInstItem(*it);
						bool hasRoleNeighbour = true;

						for (QSet<TRoleIndividualPair>::const_iterator itRoleSucc = roleRequiredSuccIndiSet.constBegin(), itRoleSuccEnd = roleRequiredSuccIndiSet.constEnd(); hasRoleNeighbour && itRoleSucc != itRoleSuccEnd; ++itRoleSucc) {
							CRole* role = itRoleSucc->first;
							CIndividual* succIndi = itRoleSucc->second;
							roleInstanceVisitor.mHasRoleNeighbourFlag = false;
							sameRealization->visitSameIndividuals(possInstItem, &sameInstanceVisitor);
							hasRoleNeighbour = roleRealization->isRoleInstance(role, sameInstanceVisitor.testingIndiRef, succIndi);
						}

						for (QSet<CRole*>::const_iterator itRole = roleRequiredNeighboursSet.constBegin(), itRoleEnd = roleRequiredNeighboursSet.constEnd(); hasRoleNeighbour && itRole != itRoleEnd; ++itRole) {
							CRole* role(*itRole);
							roleInstanceVisitor.mHasRoleNeighbourFlag = false;
							roleRealization->visitTargetIndividuals(roleRealization->getRoleInstanceItemReference(possInstItem), roleRealization->getRoleInstantiatedItem(role), &roleInstanceVisitor);
							hasRoleNeighbour = roleInstanceVisitor.mHasRoleNeighbourFlag;
						}

						if (!hasRoleNeighbour) {
							mTestedPossibleInstancesCount++;
							it = possibleDirectInstanceItemSet->erase(it);
						} else {
							++it;
						}
					}
				}
				return true;
			}


			bool COptimizedComplexConceptAnsweringHandler::searchKnownPossibleInstances(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {
				CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet = new QSet<CRealizationIndividualInstanceItemReference>();
				QSet<CRealizationIndividualInstanceItemReference>* knownDirectInstanceItemSet = new QSet<CRealizationIndividualInstanceItemReference>();
				bool possibleInstancesInitialized = false;

				class CPossibleInstancesOptimizedComplexConceptItemVisitor : public CConceptRealizationInstanceVisitor {
				public:
					QSet<CRealizationIndividualInstanceItemReference>* mInstanceItemSet;

					CPossibleInstancesOptimizedComplexConceptItemVisitor(QSet<CRealizationIndividualInstanceItemReference>* instanceItemSet) {
						mInstanceItemSet = instanceItemSet;
					}

					bool visitInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealization* conRealization) {
						mInstanceItemSet->insert(indiRealItemRef);
						return true;
					}
				};

				if (conceptRealization) {
					CHierarchyNode* topHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getTopHierarchyNode();
					CHierarchyNode* bottomHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getBottomHierarchyNode();

					QSet<CHierarchyNode*>* superClassNodeSet = conceptItem->getDirectSuperClassNodeSet();
					QSet<CHierarchyNode*>* subClassNodeSet = conceptItem->getDirectSubClassNodeSet();
					QSet<CHierarchyNode*> searchingInstanceNodeSet;
					QList<CHierarchyNode*> searchingInstanceNodeList;
					for (auto node : *superClassNodeSet) {
						searchingInstanceNodeSet.insert(node);
						if (!subClassNodeSet->contains(node)) {
							searchingInstanceNodeList.append(node);
						}
						QSet<CRealizationIndividualInstanceItemReference> tmpDirectInstanceItemSet;
						QSet<CRealizationIndividualInstanceItemReference>* fillDirectInstanceItemSet = possibleDirectInstanceItemSet;
						if (possibleInstancesInitialized) {
							fillDirectInstanceItemSet = &tmpDirectInstanceItemSet;
						}
						while (!searchingInstanceNodeList.isEmpty()) {
							auto node = searchingInstanceNodeList.takeFirst();

							if (node == topHierNode) {

								QSet<CRealizationIndividualInstanceItemReference> excludeDirectInstanceItemSet;
								for (auto subNode : *subClassNodeSet) {
									if (subNode != bottomHierNode) {
										CPossibleInstancesOptimizedComplexConceptItemVisitor possibleRealizationVisitor(&excludeDirectInstanceItemSet);
										conceptRealization->visitDirectInstances(subNode->getOneEquivalentConcept(), &possibleRealizationVisitor);
									}
								}



								visitAllIndividuals([&](const CIndividualReference& indiRef)->bool {
									CRealizationIndividualInstanceItemReference indiItemRef = conceptRealization->getInstanceItemReference(indiRef);
									if (!excludeDirectInstanceItemSet.contains(indiItemRef)) {
										fillDirectInstanceItemSet->insert(indiItemRef);
									}
									return true;
								});


							} else {

								CPossibleInstancesOptimizedComplexConceptItemVisitor possibleRealizationVisitor(fillDirectInstanceItemSet);
								conceptRealization->visitDirectInstances(node->getOneEquivalentConcept(), &possibleRealizationVisitor);

								for (auto childNode : *node->getChildNodeSet()) {
									if (!subClassNodeSet->contains(childNode)) {
										searchingInstanceNodeList.append(childNode);
										searchingInstanceNodeSet.insert(childNode);
									}
								}
							}
						}
						if (!possibleInstancesInitialized) {
							possibleInstancesInitialized = true;
						} else {
							possibleDirectInstanceItemSet->intersect(tmpDirectInstanceItemSet);
						}

					}

				}



				if (roleRealization) {
					class CPossibleInstancesOptimizedComplexRoleItemVisitor : public CRoleRealizationInstanceVisitor {
					public:
						QSet<CRealizationIndividualInstanceItemReference>* mInstanceItemSet;

						CPossibleInstancesOptimizedComplexRoleItemVisitor(QSet<CRealizationIndividualInstanceItemReference>* instanceItemSet) {
							mInstanceItemSet = instanceItemSet;
						}

						bool visitRoleInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealization* roleRealization) {
							mInstanceItemSet->insert(indiRealItemRef);
							return true;
						}
					};

					for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {
						CConcept* concept = conNegPair.first;
						bool negation = conNegPair.second;
						if (!negation && concept->getOperatorCode() == CCVALUE) {

							CIndividual* nomIndividual = concept->getNominalIndividual();
							CRole* role = concept->getRole();

							if (!possibleInstancesInitialized) {
								possibleInstancesInitialized = true;
								CPossibleInstancesOptimizedComplexRoleItemVisitor possibleRealizationVisitor(possibleDirectInstanceItemSet);
								roleRealization->visitSourceIndividuals(nomIndividual, role, &possibleRealizationVisitor);
							} else {
								QSet<CRealizationIndividualInstanceItemReference> tmpDirectInstanceItemSet;
								CPossibleInstancesOptimizedComplexRoleItemVisitor possibleRealizationVisitor(&tmpDirectInstanceItemSet);
								roleRealization->visitSourceIndividuals(nomIndividual, role, &possibleRealizationVisitor);
								possibleDirectInstanceItemSet->intersect(tmpDirectInstanceItemSet);
							}
						}
					}


				}
					
				class CSuperInstancesOptimizedComplexConceptItemVisitor : public COptimizedComplexConceptItemVisitor {
				public:
					QSet<CRealizationIndividualInstanceItemReference>* mPossibleDirectInstanceItemSet;
					CConceptRealization* mConceptRealization;

					CSuperInstancesOptimizedComplexConceptItemVisitor(QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet, CConceptRealization* conceptRealization) {
						mPossibleDirectInstanceItemSet = possibleDirectInstanceItemSet;
						mConceptRealization = conceptRealization;
					}

					cint64 mSuperItemVisitingLimit = 50;
					cint64 mSuperItemResultVisitingLimit = 5;


					bool visitComplexSuperConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSuperItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getInstancesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							QSet<CRealizationIndividualInstanceItemReference>* superInstanceItemSet = item->getKnownInstanceItemSet();
							if (superInstanceItemSet) {
								QSet<CRealizationIndividualInstanceItemReference> superIndirectAllInstanceItemSet(*superInstanceItemSet);
								// may be a little bit inefficient
								CPossibleInstancesOptimizedComplexConceptItemVisitor possibleRealizationVisitor(&superIndirectAllInstanceItemSet);
								for (auto node : *item->getDirectSubClassNodeSet()) {
									mConceptRealization->visitInstances(node->getOneEquivalentConcept(), false, &possibleRealizationVisitor);
								}
								mPossibleDirectInstanceItemSet->intersect(superIndirectAllInstanceItemSet);
								continueVisitingSuperItems = false;
								if (--mSuperItemResultVisitingLimit < 0) {
									return false;
								}
							}
						}
						if (--mSuperItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}
				} superItemVisitor(possibleDirectInstanceItemSet, conceptRealization);
				visitSuperSubConceptItems(conceptItem, false, true, &superItemVisitor);




				class CSubInstancesOptimizedComplexConceptItemVisitor : public COptimizedComplexConceptItemVisitor {
				public:
					QSet<CRealizationIndividualInstanceItemReference>* mPossibleDirectInstanceItemSet;
					QSet<CRealizationIndividualInstanceItemReference>* mKnownDirectInstanceItemSet;

					CSubInstancesOptimizedComplexConceptItemVisitor(QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet, QSet<CRealizationIndividualInstanceItemReference>* knownDirectInstanceItemSet) {
						mPossibleDirectInstanceItemSet = possibleDirectInstanceItemSet;
						mKnownDirectInstanceItemSet = knownDirectInstanceItemSet;
					}

					cint64 mSubItemVisitingLimit = 50;
					cint64 mSubItemResultVisitingLimit = 5;

					bool visitComplexSuperConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSuperItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getInstancesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							QSet<CRealizationIndividualInstanceItemReference>* subInstanceItemSet = item->getKnownInstanceItemSet();
							if (subInstanceItemSet) {
								for (auto item : *subInstanceItemSet) {
									if (mPossibleDirectInstanceItemSet->contains(item)) {
										mKnownDirectInstanceItemSet->insert(item);
										mPossibleDirectInstanceItemSet->remove(item);
									}
								}
								continueVisitingSuperItems = false;
								if (--mSubItemResultVisitingLimit < 0) {
									return false;
								}
							}
						}
						if (--mSubItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}
				} subItemVisitor(possibleDirectInstanceItemSet, knownDirectInstanceItemSet);
				visitSuperSubConceptItems(conceptItem, true, false, &subItemVisitor);



				conceptItem->setKnownInstanceItemSet(knownDirectInstanceItemSet);
				conceptItem->setPossibleInstanceItemSet(possibleDirectInstanceItemSet);

				mTotalPossibleInstancesCount += possibleDirectInstanceItemSet->size();

				return true;
			}




			bool COptimizedComplexConceptAnsweringHandler::createSuperClassSubsumptionTest(COptimizedComplexConceptItem* conceptItem, CHierarchyNode* testingNode, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				CConcept* superClassConcept = testingNode->getOneEquivalentConcept();
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(conceptItem->getConcept(), conceptItem->getConceptNegation(), superClassConcept, true, nullptr, satCalcJob);
				CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass* completedMessage = new CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass(satCalcJob, conceptItem, testingNode);
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsSuperClassSubsumptionTestingCount();
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::createSubClassSubsumptionTest(COptimizedComplexConceptItem* conceptItem, CHierarchyNode* testingNode, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				CConcept* subClassConcept = testingNode->getOneEquivalentConcept();
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(subClassConcept, false, conceptItem->getConcept(), !conceptItem->getConceptNegation(), nullptr, satCalcJob);
				CAnsweringMessageDataCalculationCompletedSubsumptionSubClass* completedMessage = new CAnsweringMessageDataCalculationCompletedSubsumptionSubClass(satCalcJob, conceptItem, testingNode);
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsSubClassSubsumptionTestingCount();
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::createInvidiualInstanceTest(COptimizedComplexConceptItem* conceptItem, const CIndividualReference& testingIndiRef, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(conceptItem->getConcept(), !conceptItem->getConceptNegation(), testingIndiRef, satCalcJob);
				CAnsweringMessageDataCalculationCompletedInstanceIndividual* completedMessage = new CAnsweringMessageDataCalculationCompletedInstanceIndividual(satCalcJob, conceptItem, testingIndiRef);

				mOntoAnsweringItem->getAnsweringHandlingStatistics()->getComplexConceptItemsPossibleInstanceVerificationCount();

				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}





			bool COptimizedComplexConceptAnsweringHandler::createQueryMaterializationTest(QHash<CExpressionVariable*, CAxiomExpression*>* varExpAxiomExpHash, CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				CAnsweringMessageDataCalculationCompletedInstanceIndividual* completedMessage = new CAnsweringMessageDataCalculationCompletedInstanceIndividual(satCalcJob, nullptr, CIndividualReference());

				mOntoAnsweringItem->getAnsweringHandlingStatistics()->getComplexConceptItemsPossibleInstanceVerificationCount();

				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}





			bool COptimizedComplexConceptAnsweringHandler::createCandidatePropagationInstanceTest(COptimizedComplexConceptItem* conceptItem, CConceptOfInterestActivationTriggeringData activationTriggeringData, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());

				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				for (CConceptOfInterestActivationLinker* activationLinkerIt = activationTriggeringData.getActivationLinker(); activationLinkerIt; activationLinkerIt = activationLinkerIt->getNext()) {
					if (activationLinkerIt->getActivationIndividual()) {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(activationLinkerIt->getActivationConcept(), false, activationLinkerIt->getActivationIndividual(), satCalcJob);
					} else {
						class CIndividualTestingVisitor : public CSameRealizationIndividualVisitor {
						public:
							CIndividualReference testingIndiRef;

							bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
								testingIndiRef = indiRef;
								return false;
							}
						} visitor;
						CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();

						for (CRealizationIndividualInstanceItemReference possibleInstanceItem : *conceptItem->getPossibleInstanceItemSet()) {
							sameRealization->visitSameIndividuals(possibleInstanceItem, &visitor);
							satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(activationLinkerIt->getActivationConcept(), false, visitor.testingIndiRef, satCalcJob);
						}
					}
				}


				CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem* completedMessage = new CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem(satCalcJob, conceptItem);
				satCalcJob->setSatisfiableAnswererInstancePropagationMessageAdapter(new CSatisfiableTaskAnswererInstancePropagationMessageAdapter(conceptItem->getConcept(), conceptItem->getConceptNegation(), activationTriggeringData.getTriggeringConcept() == nullptr, this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler()));

				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsCandidatePropagationCount();
	
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}




			bool COptimizedComplexConceptAnsweringHandler::createInvidiualItemInstanceTest(COptimizedComplexConceptItem* conceptItem, const CRealizationIndividualInstanceItemReference& testingItemRef, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());

				//if (visitor.testingIndi) {
				//	if (CIRIName::getRecentIRIName(visitor.testingIndi->getIndividualNameLinker()) == "http://www.Department1.University0.edu/UndergraduateStudent79") {
				//		bool debug = true;
				//	}
				//}
				//if (visitor.testingIndi) {
				//	for (auto it = visitor.testingIndi->getAssertionRoleLinker(); it; it = it->getNext()) {
				//		CIndividual* othIndi = it->getIndividual();
				//		if (CIRIName::getRecentIRIName(othIndi->getIndividualNameLinker()) == "http://www.Department0.University0.edu/GraduateCourse0" && CIRIName::getRecentIRIName(it->getRole()->getPropertyNameLinker()) == "http://swat.cse.lehigh.edu/onto/University1-fully.owl#takesCourse") {
				//			bool debug = true;
				//		}
				//	}
				//}

				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsPossibleInstanceVerificationCount();
				
				CConcept* reducedConcept = conceptItem->getCompletelyHandledReducedInstanceTestingConcept();
				bool reducedNegation = conceptItem->getCompletelyHandledReducedInstanceTestingConceptNegation();
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(reducedConcept, !reducedNegation, testingItemRef, satCalcJob);

				CAnsweringMessageDataCalculationCompletedInstanceItem* completedMessage = new CAnsweringMessageDataCalculationCompletedInstanceItem(satCalcJob, conceptItem, testingItemRef);
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}


			bool COptimizedComplexConceptAnsweringHandler::createSatisfiabilityTest(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(conceptItem->getConcept(), conceptItem->getConceptNegation(), nullptr, satCalcJob);
				CAnsweringMessageDataCalculationCompletedSatisfiable* completedMessage = new CAnsweringMessageDataCalculationCompletedSatisfiable(satCalcJob, conceptItem);
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsSatisfiabilityTestingCount();
				satCalcJob->setSatisfiableAnswererSubsumptionMessageAdapter(new CSatisfiableTaskAnswererSubsumptionMessageAdapter(conceptItem->getConcept(), conceptItem->getConceptNegation(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler()));
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}


			CSatisfiableCalculationJob* COptimizedComplexConceptAnsweringHandler::extendProcessingByTopPropagation(CSatisfiableCalculationJobGenerator& satCalcJobGen, CSatisfiableCalculationJob* satCalcJob, bool hasTopObjectPropertyPropagation, CAnswererContext* answererContext) {
				if (hasTopObjectPropertyPropagation) {
					CConcept* topRolePropagationExistingIndividualCreationConcept = mOntoAnsweringItem->getTopRolePropagationExistingIndividualCreationConcept();
					if (!topRolePropagationExistingIndividualCreationConcept) {
						CMemoryAllocationManager* memMan = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
						CConceptVector* conceptVec = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getTBox()->getConceptVector();

						topRolePropagationExistingIndividualCreationConcept = CObjectAllocator<CConcept>::allocateAndConstruct(memMan);

						CIndividualVector* indiVec = mOntoAnsweringItem->getTestingOntology(false)->getABox()->getIndividualVector();
						cint64 univConnIndiID = mOntoAnsweringItem->getTestingOntology(false)->getABox()->getNextIndividualId(true);
						CIndividual* universalConnIndi = CObjectAllocator< CIndividual >::allocateAndConstruct(memMan);
						universalConnIndi->initIndividual(univConnIndiID);
						indiVec->setData(univConnIndiID, universalConnIndi);

						topRolePropagationExistingIndividualCreationConcept->initConcept();
						cint64 nextConTag = conceptVec->getItemCount();
						topRolePropagationExistingIndividualCreationConcept->initTag(nextConTag);
						topRolePropagationExistingIndividualCreationConcept->setOperatorCode(CCVALUE);
						topRolePropagationExistingIndividualCreationConcept->setRole(mOntoAnsweringItem->getTestingOntology(false)->getRBox()->getTopObjectRole());
						topRolePropagationExistingIndividualCreationConcept->incOperandCount();
						topRolePropagationExistingIndividualCreationConcept->setNominalIndividual(universalConnIndi);
						conceptVec->setLocalData(nextConTag, topRolePropagationExistingIndividualCreationConcept);

						mOntoAnsweringItem->setTopRolePropagationExistingIndividual(universalConnIndi);
						mOntoAnsweringItem->setTopRolePropagationExistingIndividualCreationConcept(topRolePropagationExistingIndividualCreationConcept);
					}



					visitAllIndividuals([&](const CIndividualReference& indiRef)->bool {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topRolePropagationExistingIndividualCreationConcept, false, indiRef, satCalcJob);
						return true;
					});
					
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topRolePropagationExistingIndividualCreationConcept, false, mOntoAnsweringItem->getTopRolePropagationExistingIndividual(), satCalcJob);

				}
				return satCalcJob;
			}




			bool COptimizedComplexConceptAnsweringHandler::processRequirements(CAnswererContext* answererContext, CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& reqList, CAnsweringMessageDataRequirementCompleted* completedMessage) {
				LOG(INFO, getDomain(), logTr("Scheduling processing of %1 requirements.").arg(reqList.size()), this);
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incRequestedRealizationRequirementCount(reqList.size());
				answererContext->getAnsweringCalculationHandler()->ensureRequirements(this, ontology, reqList, completedMessage);
				return true;
			}


			bool COptimizedComplexConceptAnsweringHandler::processCalculationJob(CAnswererContext* answererContext, CSatisfiableCalculationJob* satCalcJob, CAnsweringMessageDataCalculationCompleted* completedMessage) {
				satCalcJob->setCalculationConfiguration(mOntoAnsweringItem->getCalculationConfiguration());

				CAnsweringCalculationStatisticsCollection* statColl = nullptr;
				if (mOntoAnsweringItem->isProcessStatisticsCollectionConfigured()) {
					statColl = mOntoAnsweringItem->getCalculationStatisticsCollection();
					completedMessage->setStatisticsCollection(statColl);
				}
				satCalcJob->setCalclulationStatisticsCollector(statColl);

				answererContext->getAnsweringCalculationHandler()->postCalculation(this, satCalcJob, completedMessage, completedMessage);
				return true;
			}








			bool COptimizedComplexConceptAnsweringHandler::processCalculationMessage(CAnsweringMessageData* message, CAnswererContext* answererContext) {
				while (message) {
					CAnsweringMessageData::ANSWERINGMESSAGEDATA messageType = message->getAnsweringMessageDataType();
					if (messageType == CAnsweringMessageData::CALCULATIONCOMPLETED) {
						CAnsweringMessageDataCalculationCompleted* calcCompMessage = (CAnsweringMessageDataCalculationCompleted*)message;
						CAnsweringMessageDataCalculationCompleted::CALCULATIONTYPE calcType = calcCompMessage->getCalculationType();
						if (calcType == CAnsweringMessageDataCalculationCompleted::SATISFIABLECALCULATION) {
							CAnsweringMessageDataCalculationCompletedSatisfiable* satCalcCompMessage = (CAnsweringMessageDataCalculationCompletedSatisfiable*)calcCompMessage;
							processSatisfiableCalculationCompleted(satCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::SUPERCLASSSUBSUMPTIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass* subCalcCompMessage = (CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass*)calcCompMessage;
							processSuperClassSubsumptionCalculationCompleted(subCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::SUBCLASSSUBSUMPTIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedSubsumptionSubClass* subCalcCompMessage = (CAnsweringMessageDataCalculationCompletedSubsumptionSubClass*)calcCompMessage;
							processSubClassSubsumptionCalculationCompleted(subCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::INDIVIDUALINSTANCECALCULATION) {
							CAnsweringMessageDataCalculationCompletedInstanceIndividual* instCalcCompMessage = (CAnsweringMessageDataCalculationCompletedInstanceIndividual*)calcCompMessage;
							processIndividualInstanceCalculationCompleted(instCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::INSTANCEENTAILMENTCALCULATION) {
							CAnsweringMessageDataCalculationCompletedInstanceEntailment* instCalcCompMessage = (CAnsweringMessageDataCalculationCompletedInstanceEntailment*)calcCompMessage;
							processInstanceEntailmentCalculationCompleted(instCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::ITEMINSTANCECALCULATION) {
							CAnsweringMessageDataCalculationCompletedInstanceItem* instCalcCompMessage = (CAnsweringMessageDataCalculationCompletedInstanceItem*)calcCompMessage;
							processIndividualItemCalculationCompleted(instCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::VARIABLEBINDINGPPROPAGATIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedVariableBindingPropagations* varBindPropCompMessage = (CAnsweringMessageDataCalculationCompletedVariableBindingPropagations*)calcCompMessage;
							processVariableBindingsPropagationItemCalculationCompleted(varBindPropCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::VARIABLEBINDINGCONFIRMATIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation* varBindConfCompMessage = (CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation*)calcCompMessage;
							processVariableBindingsConfirmationCalculationCompleted(varBindConfCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::VARIABLEBINDINGENTAILMENTCALCULATION) {
							CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* varBindEntCompMessage = (CAnsweringMessageDataCalculationCompletedVariableBindingEntailment*)calcCompMessage;
							processVariableBindingsEntailmentCalculationCompleted(varBindEntCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::ITEMINSTANCECANDIDATEPROPAGATIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem* instCandPropMessage = (CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem*)calcCompMessage;
							processIndividualCandidatePropagationCalculationCompleted(instCandPropMessage, answererContext);
						}

						CAnsweringCalculationStatisticsCollection* statColl = calcCompMessage->getStatisticsCollection();
						if (statColl) {
							mOntoAnsweringItem->reuseCalculationStatisticsCollection(statColl);
						}

					} else if (messageType == CAnsweringMessageData::CALCULATIONADAPTER) {
						CAnsweringMessageDataCalculationAdapter* calcAdapMessage = (CAnsweringMessageDataCalculationAdapter*)message;
						CAnsweringMessageDataCalculationAdapter::CALCULATIONADAPTERTYPE calcType = calcAdapMessage->getCalculationAdapterType();
						if (calcType == CAnsweringMessageDataCalculationAdapter::CLASSSUBSUMERS) {
							CAnsweringMessageDataCalculationClassSubsumers* classSubsumersMessage = (CAnsweringMessageDataCalculationClassSubsumers*)calcAdapMessage;
							processExtractedClassSubsumers(classSubsumersMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationAdapter::POSSIBLECLASSSUBSUMERS) {
							CAnsweringMessageDataCalculationPossibleClassSubsumers* possibleClassSubsumersMessage = (CAnsweringMessageDataCalculationPossibleClassSubsumers*)calcAdapMessage;
							processExtractedPossibleClassSubsumers(possibleClassSubsumersMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationAdapter::PROPAGATION_BINDINGS) {
							CAnsweringMessageDataVariableBindingPropagations* variableBindingsPropagationMessage = (CAnsweringMessageDataVariableBindingPropagations*)calcAdapMessage;
							processExtractedVariableBindingPropagations(variableBindingsPropagationMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationAdapter::INSTANCE_CERTAIN_PROPAGATIONS) {
							CAnsweringMessageDataInstanceCertainPropagationsData* instanceCandidatePropagationMessage = (CAnsweringMessageDataInstanceCertainPropagationsData*)calcAdapMessage;
							processExtractedInstanceCertainPropagations(instanceCandidatePropagationMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationAdapter::INSTANCE_POSSIBLE_PROPAGATIONS) {
							CAnsweringMessageDataInstancePossiblePropagationsData* instanceCandidatePossiblePropagationMessage = (CAnsweringMessageDataInstancePossiblePropagationsData*)calcAdapMessage;
							processExtractedInstanceCandidatePossiblePropagations(instanceCandidatePossiblePropagationMessage, answererContext);
						}
					}
					message = message->getNext();
				}
				return false;
			}



			bool COptimizedComplexConceptAnsweringHandler::processRequirementMessage(CAnsweringMessageData* message, CAnswererContext* answererContext) {
				while (message) {
					CAnsweringMessageData::ANSWERINGMESSAGEDATA messageType = message->getAnsweringMessageDataType();
					if (messageType == CAnsweringMessageData::REQUIREMENTCOMPLETED) {
						LOG(INFO, getDomain(), logTr("Finished processing requirements, continuing query answering."), this);

						CAnsweringMessageDataRequirementCompleted* reqCompMessage = (CAnsweringMessageDataRequirementCompleted*)message;
						CAnsweringMessageDataRequirementCompleted::REQUIREMENTTYPE reqType = reqCompMessage->getRequirementType();
						if (reqType == CAnsweringMessageDataRequirementCompleted::REALIZATIONREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedRealization* compRealReqMess = (CAnsweringMessageDataRequirementCompletedRealization*)reqCompMessage;
							COptimizedComplexConceptItem* conItem = compRealReqMess->getConceptItem();
							if (conItem) {
								conItem->decLazyRealizationProcessingCount();
								if (conItem->getLazyRealizationProcessingCount() <= 0) {
									CComplexConceptStepComputationProcess* compStep = conItem->getComputationProcess()->getInstancesComputationProcess(false);
									conItem->setQueuedProcessStep(compStep);
									compStep->setComputationProcessQueued(true);
									COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
									processingStep->addQueuedConceptItem(conItem);
								}
							}
							COptimizedComplexVariableCompositionItem* variableCompositionItem = compRealReqMess->getVariableCompositionItem();
							COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem = compRealReqMess->getBuildingVariableCompositionItem();
							if (buildingVarItem && variableCompositionItem) {
								buildingVarItem->decVariableCompositionItemWaitingCount();
								buildingVarItem->addComputeVariableMappingItem(variableCompositionItem);
								if (buildingVarItem->getVariableCompositionItemWaitingCount() <= 0) {
									mOntoAnsweringItem->addProcessingVariableBuildingItem(buildingVarItem);
								}
							}
						} else if (reqType == CAnsweringMessageDataRequirementCompleted::SUBCLASSREALIZATIONREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedSubClassRealization* subClassRealReqMess = (CAnsweringMessageDataRequirementCompletedSubClassRealization*)reqCompMessage;
							COptimizedComplexConceptItem* conItem = subClassRealReqMess->getConceptItem();
							CComplexConceptStepComputationProcess* compStep = conItem->getComputationProcess()->getSubClassRealizationProcess(false);
							finishCalculationStepProcessing(conItem, compStep, answererContext);

						} else if (reqType == CAnsweringMessageDataRequirementCompleted::CONCEPTASSERTIONENTAILMENTREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedConceptAssertionEntailment* entReqMes = (CAnsweringMessageDataRequirementCompletedConceptAssertionEntailment*)reqCompMessage;
							CEntailmentQueryProcessingData* procData = entReqMes->getEntailmentProcessingData();
							procData->mHasResult = true;
							CConcept* concept = entReqMes->getConcept();
							CIndividual* individual = entReqMes->getIndividual();
							CConceptRealization* conReal = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
							procData->mEntailed = conReal->isConceptInstance(individual,concept);
							for (CXLinker<CComplexQueryProcessingData*>* procDataLinkerIt = procData->mProcDataLinker; procDataLinkerIt; procDataLinkerIt = procDataLinkerIt->getNext()) {
								CComplexQueryProcessingData* queryProcData = procDataLinkerIt->getData();
								queryProcData->decEntailmentComputation();
								if (!queryProcData->hasComputation()) {
									finishQueryProcessing(queryProcData);
								}
							}
							mOntoAnsweringItem->releaseQueryProcessingLinker(procData->mProcDataLinker);
							procData->mProcDataLinker = nullptr;

						} else if (reqType == CAnsweringMessageDataRequirementCompleted::ROLEASSERTIONENTAILMENTREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment* entReqMes = (CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment*)reqCompMessage;
							CEntailmentQueryProcessingData* procData = entReqMes->getEntailmentProcessingData();
							procData->mHasResult = true;
							CRole* role = entReqMes->getRole();
							CIndividual* individualSource = entReqMes->getIndividualSource();
							CIndividual* individualDest = entReqMes->getIndividualDestination();
							CRoleRealization* roleReal = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
							procData->mEntailed = roleReal->isRoleInstance(role,individualSource,individualDest);
							for (CXLinker<CComplexQueryProcessingData*>* procDataLinkerIt = procData->mProcDataLinker; procDataLinkerIt; procDataLinkerIt = procDataLinkerIt->getNext()) {
								CComplexQueryProcessingData* queryProcData = procDataLinkerIt->getData();
								queryProcData->decEntailmentComputation();
								if (!queryProcData->hasComputation()) {
									finishQueryProcessing(queryProcData);
								}
							}
							mOntoAnsweringItem->releaseQueryProcessingLinker(procData->mProcDataLinker);
							procData->mProcDataLinker = nullptr;

						} else if (reqType == CAnsweringMessageDataRequirementCompletedRoleTargetsRealization::ROLETARGETSREALIZATIONREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedRoleTargetsRealization* roleTarRealMes = (CAnsweringMessageDataRequirementCompletedRoleTargetsRealization*)reqCompMessage;
							CRole* role = roleTarRealMes->getRole();
							bool inversed = roleTarRealMes->getInversed();
							QList<CIndividualReference>* individualSourceList = roleTarRealMes->getIndividualSourceList();

							for (CIndividualReference individualSource : *individualSourceList) {

								CRoleTargetsRealizationHasher enHasher(role, individualSource, inversed);
								CEntailmentVariableBuildingItemData* varBuildData = mRoleTargetsProcDataHash[enHasher];

								for (CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* procDataLinkerIt = varBuildData->mProcDataLinker; procDataLinkerIt; procDataLinkerIt = procDataLinkerIt->getNext()) {
									COptimizedComplexBuildingVariableCompositionsItem* varBuildItem = procDataLinkerIt->getData();
									varBuildItem->decWaitingRoleTargetsRealizationCount();
									if (!varBuildItem->isWaitingRoleTargetsRealization()) {
										varBuildItem->setProcessingQueued(true);
										mOntoAnsweringItem->addProcessingVariableBuildingItem(varBuildItem);
									}
								}
								mOntoAnsweringItem->releaseBuildingVariableItemProcessingLinker(varBuildData->mProcDataLinker);
								varBuildData->mProcDataLinker = nullptr;
							}

						}
					}
					message = message->getNext();
				}
				return false;				
			}






			QSet<CHierarchyNode*>* COptimizedComplexConceptAnsweringHandler::getDirectReducedSuperNodeSet(const QSet<CHierarchyNode*>& superClassesSet) {
				QSet<CHierarchyNode*>* directSuperClassesSet = new QSet<CHierarchyNode*>();
				QList<CHierarchyNode*> predCountSortedSuperClassNodeList;
				for (QSet<CHierarchyNode*>::const_iterator it = superClassesSet.constBegin(), itEnd = superClassesSet.constEnd(); it != itEnd; ++it) {
					CHierarchyNode* superClassNode(*it);
					if (superClassNode) {
						predCountSortedSuperClassNodeList.append(superClassNode);
					}
				}
				qSort(predCountSortedSuperClassNodeList.begin(), predCountSortedSuperClassNodeList.end(), nodeSortMorePredecessorsThan);
				for (QList<CHierarchyNode*>::const_iterator it = predCountSortedSuperClassNodeList.constBegin(), itEnd = predCountSortedSuperClassNodeList.constEnd(); it != itEnd; ++it) {
					CHierarchyNode* superClassNode(*it);
					bool alreadyContained = false;
					for (QSet<CHierarchyNode*>::const_iterator itDirect = directSuperClassesSet->constBegin(), itDirectEnd = directSuperClassesSet->constEnd(); !alreadyContained && itDirect != itDirectEnd; ++itDirect) {
						CHierarchyNode* directSuperNode(*itDirect);
						if (directSuperNode->hasPredecessorNode(superClassNode)) {
							alreadyContained = true;
						}
					}
					if (!alreadyContained) {
						directSuperClassesSet->insert(superClassNode);
						if (directSuperClassesSet->count() == 1 && superClassNode->getPredecessorNodeCount()+1 == predCountSortedSuperClassNodeList.count()) {
							break;
						}
					}
				}
				return directSuperClassesSet;
			}





			bool COptimizedComplexConceptAnsweringHandler::processExtractedClassSubsumers(CAnsweringMessageDataCalculationClassSubsumers* message, CAnswererContext* answererContext) {
				CConcept* concept = message->getSubsumedConcept();
				bool negation = message->getSubsumedConceptNegation();
				COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(concept,negation);
				CClassConceptClassification* classClassification = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification();
				CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* superClassesList = message->getClassSubsumerList();
				if (classClassification) {
					QSet<CHierarchyNode*>* superClassesSet = new QSet<CHierarchyNode*>();
					CTaxonomy* taxonomy = classClassification->getClassConceptTaxonomy();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>::const_iterator it = superClassesList->constBegin(), itEnd = superClassesList->constEnd(); it != itEnd; ++it) {
						CConcept* superClass(*it);
						CHierarchyNode* hierNode = taxonomy->getHierarchyNode(superClass, false);
						if (hierNode) {
							superClassesSet->insert(hierNode);
						}
					}
					conceptItem->setKnownSuperClassNodeSet(superClassesSet);
				} else {
					QSet<CConcept*>* knownSuperConceptSet = new QSet<CConcept*>();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>::const_iterator it = superClassesList->constBegin(), itEnd = superClassesList->constEnd(); it != itEnd; ++it) {
						CConcept* superClass(*it);
						knownSuperConceptSet->insert(superClass);
					}
					conceptItem->setKnownSuperConceptSet(knownSuperConceptSet);
				}
				return true;
			}




			bool COptimizedComplexConceptAnsweringHandler::processExtractedPossibleClassSubsumers(CAnsweringMessageDataCalculationPossibleClassSubsumers* message, CAnswererContext* answererContext) {
				CConcept* concept = message->getSubsumedConcept();
				bool negation = message->getSubsumedConceptNegation();
				COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(concept, negation);
				CClassConceptClassification* classClassification = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification();
				if (classClassification) {
					QSet<CHierarchyNode*>* possSuperClassesSet = new QSet<CHierarchyNode*>();
					QSet<CHierarchyNode*>* knownSuperClassesSet = conceptItem->getKnownSuperClassNodeSet();
					QSet<CConcept*>* knowSuperConceptSet = conceptItem->getKnownSuperConceptSet();
					CTaxonomy* taxonomy = classClassification->getClassConceptTaxonomy();
					CCLASSSUBSUMPTIONMESSAGELIST<CAnsweringMessageDataCalculationPossibleClassSubsumersData*>* possSuperClassesList = message->getPossibleClassSubsumerList();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CAnsweringMessageDataCalculationPossibleClassSubsumersData*>::const_iterator it = possSuperClassesList->constBegin(), itEnd = possSuperClassesList->constEnd(); it != itEnd; ++it) {
						CAnsweringMessageDataCalculationPossibleClassSubsumersData* possSuperClassData(*it);
						if (possSuperClassData->isPossibleSubsumerValid()) {
							CConcept* possSuperClass = possSuperClassData->getPossibleSubsumerConcept();
							CHierarchyNode* hierNode = taxonomy->getHierarchyNode(possSuperClass, false);
							if (hierNode) {
								if (knownSuperClassesSet && !knownSuperClassesSet->contains(hierNode) || knowSuperConceptSet && !knowSuperConceptSet->contains(possSuperClass)) {
									possSuperClassesSet->insert(hierNode);
								}
							}
						}
					}
					conceptItem->setPossibleSuperClassNodeSet(possSuperClassesSet);
				} else {
					QSet<CConcept*>* knowSuperConceptSet = conceptItem->getKnownSuperConceptSet();
					QSet<CConcept*>* possibleSuperConceptSet = new QSet<CConcept*>();
					CCLASSSUBSUMPTIONMESSAGELIST<CAnsweringMessageDataCalculationPossibleClassSubsumersData*>* possSuperClassesList = message->getPossibleClassSubsumerList();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CAnsweringMessageDataCalculationPossibleClassSubsumersData*>::const_iterator it = possSuperClassesList->constBegin(), itEnd = possSuperClassesList->constEnd(); it != itEnd; ++it) {
						CAnsweringMessageDataCalculationPossibleClassSubsumersData* possSuperClassData(*it);
						if (possSuperClassData->isPossibleSubsumerValid()) {
							CConcept* possSuperClass = possSuperClassData->getPossibleSubsumerConcept();
							if (knowSuperConceptSet && !knowSuperConceptSet->contains(possSuperClass)) {
								possibleSuperConceptSet->insert(possSuperClass);
							}
						}
					}
					conceptItem->setPossibleSuperConceptSet(possibleSuperConceptSet);
				}
				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::processSatisfiableCalculationCompleted(CAnsweringMessageDataCalculationCompletedSatisfiable* message, CAnswererContext* answererContext) {
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				if (conceptItem->isSuperClassComputationInitializing()) {
					conceptItem->setSuperClassComputationInitializing(false);
					CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSuperClassNodesComputationProcess(true);
					compStep->setComputationProcessProcessing(false);
					compStep->decCurrentlyRunningComputationCount();
					COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
					processingStep->addQueuedConceptItem(conceptItem);
				} else {
					bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
					conceptItem->setSatisfiability(satisfiable);
					CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSatisfiableClassNodesComputationProcess(true);
					compStep->setComputationProcessProcessing(false);
					compStep->decCurrentlyRunningComputationCount();
					compStep->incFinishedComputationCount();
					finishCalculationStepProcessing(conceptItem, compStep, answererContext);
				}
				return true;
			}


			bool COptimizedComplexConceptAnsweringHandler::processSuperClassSubsumptionCalculationCompleted(CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass* message, CAnswererContext* answererContext) {
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CHierarchyNode* superClassNode = message->getSuperClassNode();
				CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSuperClassNodesComputationProcess(true);
				if (!satisfiable) {
					conceptItem->getKnownSuperClassNodeSet()->insert(superClassNode);
				}
				compStep->decCurrentlyRunningComputationCount();
				compStep->incFinishedComputationCount();
				if (compStep->getCurrentlyRunningComputationCount() == 0) {
					compStep->setComputationProcessProcessing(false);
					if (!conceptItem->getPossibleSuperClassNodeTestingList() || conceptItem->getPossibleSuperClassNodeTestingList()->isEmpty()) {
						delete conceptItem->getPossibleSuperClassNodeTestingList();
						conceptItem->setPossibleSuperClassNodeTestingList(nullptr);
						QSet<CHierarchyNode*>* knownSuperClassSet = conceptItem->getKnownSuperClassNodeSet();
						if (knownSuperClassSet) {
							QSet<CHierarchyNode*>* directSuperClassSet = getDirectReducedSuperNodeSet(*knownSuperClassSet);
							conceptItem->setKnownSuperClassNodeSet(nullptr);
							delete knownSuperClassSet;
							conceptItem->setDirectSuperClassNodeSet(directSuperClassSet);
						}
						finishCalculationStepProcessing(conceptItem, compStep, answererContext);
					}
				}
				return true;
			}


			bool COptimizedComplexConceptAnsweringHandler::processSubClassSubsumptionCalculationCompleted(CAnsweringMessageDataCalculationCompletedSubsumptionSubClass* message, CAnswererContext* answererContext) {
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CHierarchyNode* subClassNode = message->getSubClassNode();
				CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSubClassNodesComputationProcess(true);
				if (!satisfiable) {
					conceptItem->getDirectSubClassNodeSet()->insert(subClassNode);
				} else {
					for (QSet<CHierarchyNode*>::const_iterator it = subClassNode->getChildNodeSet()->constBegin(), itEnd = subClassNode->getChildNodeSet()->constEnd(); it != itEnd; ++it) {
						CHierarchyNode* childNode(*it);
						if (!conceptItem->getPossibleSubClassTestingNodeSet()->contains(childNode)) {
							conceptItem->getPossibleSubClassTestingNodeSet()->insert(childNode);
							conceptItem->getPossibleSubClassNodeTestingList()->append(childNode);
							if (conceptItem->getPossibleSubClassNodeTestingList()->size() == 1) {
								if (!compStep->isComputationProcessQueued()) {
									compStep->setComputationProcessQueued(true);
									COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
									processingStep->addQueuedConceptItem(conceptItem);
								}
							}
						}
					}
				}
				compStep->incFinishedComputationCount();
				compStep->decCurrentlyRunningComputationCount();
				if (compStep->getCurrentlyRunningComputationCount() == 0) {
					compStep->setComputationProcessProcessing(false);
					if (!conceptItem->getPossibleSubClassNodeTestingList() || conceptItem->getPossibleSubClassNodeTestingList()->isEmpty()) {
						finishSubClassCalculationStepProcessing(conceptItem, compStep, answererContext);
					}
				}
				return true;
			}





			bool COptimizedComplexConceptAnsweringHandler::processIndividualCandidatePropagationCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem* message, CAnswererContext* answererContext) {
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				conceptItem->decCandidatePropagationProcessingCount();
				CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getInstancesComputationProcess(true);

				if (!conceptItem->isCandidateReceived()) {
					QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
					mTestedPossibleInstancesCount += possibleInstanceItemSet->size();
					conceptItem->incPossibleTestedNonInstanceCount(possibleInstanceItemSet->size());
					conceptItem->setPossibleInstanceItemSet(nullptr);
					delete possibleInstanceItemSet;
				}

				compStep->decCurrentlyRunningComputationCount();
				compStep->incFinishedComputationCount();
				if (compStep->getCurrentlyRunningComputationCount() == 0) {
					compStep->setComputationProcessProcessing(false);
				}


				COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
				processingStep->addQueuedConceptItem(conceptItem);
				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::processIndividualItemCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceItem* message, CAnswererContext* answererContext) {
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CRealizationIndividualInstanceItemReference testingItemRef = message->getInstanceItemReference();
				CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getInstancesComputationProcess(true);
				if (!satisfiable) {
					conceptItem->getKnownInstanceItemSet()->insert(testingItemRef);
				} else {
					mTestedPossibleInstancesCount++;
					conceptItem->incPossibleTestedNonInstanceCount(1);
				}
				compStep->decCurrentlyRunningComputationCount();
				compStep->incFinishedComputationCount();
				mTestedPossibleInstancesCount++;
				if (compStep->getCurrentlyRunningComputationCount() == 0) {
					compStep->setComputationProcessProcessing(false);
				}

				COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
				processingStep->addQueuedConceptItem(conceptItem);
				return true;
			}




			bool COptimizedComplexConceptAnsweringHandler::processIndividualInstanceCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceIndividual* message, CAnswererContext* answererContext) {
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CIndividualReference testingIndiRef = message->getIndividualReference();
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				if (conceptItem) {
					CComplexConceptStepInstanceComputationProcess* compStep = conceptItem->getComputationProcess()->getInstancesComputationProcess(true);
					if (!satisfiable) {
						conceptItem->getKnownInstanceIndividualSet()->insert(testingIndiRef);
					} else {
						conceptItem->incPossibleTestedNonInstanceCount();
					}
					compStep->decCurrentlyRunningComputationCount();
					compStep->incFinishedComputationCount();
					if (compStep->getCurrentlyRunningComputationCount() == 0) {
						compStep->setComputationProcessProcessing(false);

						if (!conceptItem->getPossibleInstanceIndividualSet() || conceptItem->getPossibleInstanceIndividualSet()->isEmpty()) {
							updateComputedInstancesCount(conceptItem, compStep, answererContext);

							if (conceptItem->isLazyRealizationInstancesRetrieved()) {
								delete conceptItem->getPossibleInstanceIndividualSet();
								finishCalculationStepProcessing(conceptItem, compStep, answererContext);
							}
						}
					}
				}
				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::processInstanceEntailmentCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceEntailment* message, CAnswererContext* answererContext) {
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CIndividualReference testingIndiRef = message->getIndividualReference();
				CConcept* concept = message->getConcept();
				bool negation = message->getNegation();
				CEntailmentQueryProcessingData* entailmentData = message->getEntailmentData();
				entailmentData->mEntailed = !satisfiable;
				for (CXLinker<CComplexQueryProcessingData*>* procDataLinkerIt = entailmentData->mProcDataLinker; procDataLinkerIt; procDataLinkerIt = procDataLinkerIt->getNext()) {
					CComplexQueryProcessingData* queryProcData = procDataLinkerIt->getData();
					queryProcData->decEntailmentComputation();
					if (!queryProcData->hasComputation()) {
						finishQueryProcessing(queryProcData);
					}
				}
				mOntoAnsweringItem->releaseQueryProcessingLinker(entailmentData->mProcDataLinker);
				entailmentData->mProcDataLinker = nullptr;
				return true;
			}




			bool COptimizedComplexConceptAnsweringHandler::notifyWaitingComplexQueryProcessingData(CXLinker<CComplexQueryProcessingData*>* queryProcDataLinker, CComplexConceptStepComputationProcess* compStep, CAnswererContext* answererContext) {
				if (queryProcDataLinker) {
					for (CXLinker<CComplexQueryProcessingData*>* queryProcDataLinkerIt = queryProcDataLinker; queryProcDataLinkerIt; queryProcDataLinkerIt = queryProcDataLinkerIt->getNext()) {
						CComplexQueryProcessingData* queryProcData = queryProcDataLinkerIt->getData();
						queryProcData->setComputationStepFinished(compStep);
						if (!queryProcData->hasComputation()) {
							finishQueryProcessing(queryProcData);
						}
					}
					mOntoAnsweringItem->releaseQueryProcessingLinker(queryProcDataLinker);
					return true;
				}
				return false;
			}


			bool COptimizedComplexConceptAnsweringHandler::notifyWaitingComplexBuildingVariableCompositionsItems(CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* buildVarItemProcDataLinker, CComplexConceptStepComputationProcess* compStep, CAnswererContext* answererContext) {
				if (buildVarItemProcDataLinker) {
					for (CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* buildVarItemProcDataLinkerIt = buildVarItemProcDataLinker; buildVarItemProcDataLinkerIt; buildVarItemProcDataLinkerIt = buildVarItemProcDataLinkerIt->getNext()) {
						COptimizedComplexBuildingVariableCompositionsItem* buildVarItem = buildVarItemProcDataLinkerIt->getData();
						buildVarItem->setComputationStepFinished(compStep);
						if (!buildVarItem->isWaitingComputationStep()) {
							buildVarItem->setProcessingQueued(true);
							mOntoAnsweringItem->addProcessingVariableBuildingItem(buildVarItem);
						}
					}
					mOntoAnsweringItem->releaseBuildingVariableItemProcessingLinker(buildVarItemProcDataLinker);
					return true;
				}
				return false;
			}


			bool COptimizedComplexConceptAnsweringHandler::finishCalculationStepProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep, CAnswererContext* answererContext) {
				compStep->setComputationProcessFinished(true);

				notifyWaitingComplexQueryProcessingData(compStep->takeQueryProcessingLinker(), compStep, answererContext);
				notifyWaitingComplexBuildingVariableCompositionsItems(compStep->takeBuildingVariableItemProcessingLinker(), compStep, answererContext);

				requeueConceptItemNextProcessing(conceptItem, compStep);
				return true;
			}




			bool COptimizedComplexConceptAnsweringHandler::updateComputedInstancesCount(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepInstanceComputationProcess* compStep, CAnswererContext* answererContext) {
				compStep->updateComputedInstancesCount(conceptItem->getKnownInstanceItemSet()->size(), [&](CXLinker<CComplexQueryProcessingData*>* waitingQueryProcessingLinker, CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* waitingBuildingVariableItemProcessingLinker) {
					notifyWaitingComplexQueryProcessingData(waitingQueryProcessingLinker, compStep, answererContext);
					notifyWaitingComplexBuildingVariableCompositionsItems(waitingBuildingVariableItemProcessingLinker, compStep, answererContext);
				});
				if (conceptItem->isLazyRealizationInstancesRetrieved()) {
					compStep->updateAllInstancesComputed([&](CXLinker<CComplexQueryProcessingData*>* waitingQueryProcessingLinker, CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* waitingBuildingVariableItemProcessingLinker) {
						notifyWaitingComplexQueryProcessingData(waitingQueryProcessingLinker, compStep, answererContext);
						notifyWaitingComplexBuildingVariableCompositionsItems(waitingBuildingVariableItemProcessingLinker, compStep, answererContext);
					});
				}
				return true;
			}

			QSet<COptimizedComplexConceptItem*>* COptimizedComplexConceptAnsweringHandler::collectComplexConceptItemPredecessors(COptimizedComplexConceptItem* conceptItem, QSet<COptimizedComplexConceptItem*>* predItemSet) {
				if (!predItemSet) {
					predItemSet = new QSet<COptimizedComplexConceptItem *>();
				}
				QList<COptimizedComplexConceptItem*> addingList;
				addingList.append(conceptItem);
				while (!addingList.isEmpty()) {
					COptimizedComplexConceptItem* testingItem = addingList.takeFirst();
					if (!predItemSet->contains(testingItem)) {
						predItemSet->insert(testingItem);
						for (auto superItem : *testingItem->getSuperComplexConceptItemSet()) {
							addingList.append(superItem);
						}
					}
				}
				return predItemSet;
			}



			QSet<COptimizedComplexConceptItem*>* COptimizedComplexConceptAnsweringHandler::collectComplexConceptItemSucessors(COptimizedComplexConceptItem* conceptItem, QSet<COptimizedComplexConceptItem*>* succItemSet) {
				if (!succItemSet) {
					succItemSet = new QSet<COptimizedComplexConceptItem *>();
				}
				QList<COptimizedComplexConceptItem*> addingList;
				addingList.append(conceptItem);
				while (!addingList.isEmpty()) {
					COptimizedComplexConceptItem* testingItem = addingList.takeFirst();
					if (!succItemSet->contains(testingItem)) {
						succItemSet->insert(testingItem);
						for (auto superItem : *testingItem->getSubComplexConceptItemSet()) {
							addingList.append(superItem);
						}
					}
				}
				return succItemSet;
			}



			bool COptimizedComplexConceptAnsweringHandler::reorderConceptItemParentAddition(COptimizedComplexConceptItem* conceptItem, COptimizedComplexConceptItem* newParentConceptItem) {
				QSet<COptimizedComplexConceptItem*>* newParentPredItemSet = collectComplexConceptItemPredecessors(newParentConceptItem);
				QSet<COptimizedComplexConceptItem*>* conceptItemSuccItemSet = collectComplexConceptItemSucessors(conceptItem);
				QList<COptimizedComplexConceptItem*> removeParentItemList;

				for (QSet<COptimizedComplexConceptItem*>::iterator it = conceptItem->getSuperComplexConceptItemSet()->begin(); it != conceptItem->getSuperComplexConceptItemSet()->end();) {
					COptimizedComplexConceptItem* parentItem(*it);
					if (newParentPredItemSet->contains(parentItem)) {
						parentItem->getSubComplexConceptItemSet()->remove(conceptItem);
						it = conceptItem->getSuperComplexConceptItemSet()->erase(it);
					} else {
						++it;
					}
				}
				for (QSet<COptimizedComplexConceptItem*>::iterator it = newParentConceptItem->getSubComplexConceptItemSet()->begin(); it != newParentConceptItem->getSubComplexConceptItemSet()->end();) {
					COptimizedComplexConceptItem* childItem(*it);
					if (conceptItemSuccItemSet->contains(childItem)) {
						it = newParentConceptItem->getSubComplexConceptItemSet()->erase(it);
						childItem->getSuperComplexConceptItemSet()->remove(newParentConceptItem);
					} else {
						++it;
					}
				}
				delete conceptItemSuccItemSet;
				delete newParentPredItemSet;
				
				conceptItem->getSuperComplexConceptItemSet()->insert(newParentConceptItem);
				newParentConceptItem->getSubComplexConceptItemSet()->insert(conceptItem);

				return true;
			}


			bool COptimizedComplexConceptAnsweringHandler::reorderConceptClassItems() {
				QList<COptimizedComplexConceptItem*>* classItemReorderingList = mOntoAnsweringItem->getConceptClassItemReorderingList();
				CClassConceptClassification* classClassification = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification();
				if (classClassification) {
					CTaxonomy* taxonomy = classClassification->getClassConceptTaxonomy();
					while (!classItemReorderingList->isEmpty()) {
						COptimizedComplexConceptItem* classItem = classItemReorderingList->takeFirst();
						CConcept* classConcept = classItem->getConcept();
						CHierarchyNode* classHierNode = taxonomy->getHierarchyNode(classConcept, false);
						if (classHierNode) {
							QList<CHierarchyNode*> predTestingList;
							for (auto parentHierNode : *classHierNode->getParentNodeSet()) {
								predTestingList.append(parentHierNode);
							}
							while (!predTestingList.isEmpty()) {
								auto parentHierNode = predTestingList.takeFirst();
								bool foundParent = false;
								for (auto parentConcept : *parentHierNode->getEquivalentConceptList()) {
									COptimizedComplexConceptItem* parentClassItem = mOntoAnsweringItem->getComplexConceptItem(parentConcept, false, false);
									if (parentClassItem) {
										foundParent = true;
										if (!classItem->hasSuperComplexConceptItem(parentClassItem)) {
											reorderConceptItemParentAddition(classItem, parentClassItem);
										}
									}
								}
								if (!foundParent) {
									for (auto nextParentHierNode : *parentHierNode->getParentNodeSet()) {
										predTestingList.append(nextParentHierNode);
									}
								}
							}
						}
					}
				}
				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::extractAtomicSubsumerExpressions(COptimizedComplexConceptItem* conceptItem) {
				CConcept* concept = conceptItem->getConcept();
				bool conceptNegation = conceptItem->getConceptNegation();
				QList<TConceptNegPair> testingList;
				testingList.append(TConceptNegPair(concept,conceptNegation));
				QSet<TConceptNegPair>* atomicConceptSet = conceptItem->getAtomicConcpetSet();
				while (!testingList.isEmpty()) {
					TConceptNegPair testingConNegPair = testingList.takeFirst();
					CConcept* testingConcept = testingConNegPair.first;
					bool testingNegation = testingConNegPair.second;
					cint64 opCode = testingConcept->getOperatorCode();
					if (!testingNegation && opCode == CCAND || testingNegation && opCode == CCOR) {
						for (CSortedNegLinker<CConcept*>* opLinkerIt = testingConcept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
							CConcept* opConcept = opLinkerIt->getData();
							bool opNegation = opLinkerIt->isNegated() ^ testingNegation;
							testingList.append(TConceptNegPair(opConcept, opNegation));
						}
					} else {
						atomicConceptSet->insert(testingConNegPair);
					}
				}
				return !atomicConceptSet->isEmpty();
			}


			bool COptimizedComplexConceptAnsweringHandler::isTriviallySubsumedConceptItem(COptimizedComplexConceptItem* subsumedConceptItem, COptimizedComplexConceptItem* subsumerConceptItem) {
				if (subsumedConceptItem->getAtomicConcpetSet()->contains(subsumerConceptItem->getConceptNegationPair())) {
					return true;
				}
				if (subsumedConceptItem->getAtomicConcpetSet()->contains(*subsumerConceptItem->getAtomicConcpetSet())) {
					return true;
				}
				return false;
			}





			bool COptimizedComplexConceptAnsweringHandler::searchSuperSubSuperClassesResult(COptimizedComplexConceptItem* startConceptItem, CAnswererContext* answererContext) {
				class CSuperSubsumptionOptimizedComplexConceptItemVisitor : public COptimizedComplexConceptItemVisitor {
				public:
					QSet<CHierarchyNode*>* mFoundKnwonSuperClassNodeSet = nullptr;
					QSet<CHierarchyNode*>* mFoundPossibleSuperClassNodeSet = nullptr;

					cint64 mSuperItemVisitingLimit = 50;
					cint64 mSuperItemResultVisitingLimit = 10;

					cint64 mSubItemVisitingLimit = 50;
					cint64 mSubItemResultVisitingLimit = 10;

					bool visitComplexSuperConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSuperItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getSuperClassNodesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							QSet<CHierarchyNode*>* superClassNodeSet = item->getDirectSuperClassNodeSet();
							if (!mFoundKnwonSuperClassNodeSet) {
								mFoundKnwonSuperClassNodeSet = new QSet<CHierarchyNode*>();
							}
							mFoundKnwonSuperClassNodeSet->operator+=(*superClassNodeSet);
							continueVisitingSuperItems = false;
							if (--mSuperItemResultVisitingLimit < 0) {
								return false;
							}
						}
						if (--mSuperItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}

					bool visitComplexSubConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSubItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getSuperClassNodesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							QSet<CHierarchyNode*>* superClassNodeSet = item->getDirectSuperClassNodeSet();
							if (!mFoundPossibleSuperClassNodeSet) {
								mFoundPossibleSuperClassNodeSet = new QSet<CHierarchyNode*>();
								if (mFoundKnwonSuperClassNodeSet && !mFoundKnwonSuperClassNodeSet->isEmpty()) {
									for (auto node : *superClassNodeSet) {
										if (!mFoundKnwonSuperClassNodeSet->contains(node)) {
											mFoundPossibleSuperClassNodeSet->insert(node);
										}
									}
								} else {
									*mFoundPossibleSuperClassNodeSet = *superClassNodeSet;
								}
							} else {
								mFoundPossibleSuperClassNodeSet->intersect(*superClassNodeSet);
							}
							if (mFoundPossibleSuperClassNodeSet->isEmpty()) {
								return false;
							}
							continueVisitingSubItems = false;
							if (--mSubItemResultVisitingLimit < 0) {
								return false;
							}
						}
						if (--mSubItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}

				} visitor;
				visitSuperSubConceptItems(startConceptItem, false, true, &visitor);
				if (visitor.mFoundKnwonSuperClassNodeSet) {
					startConceptItem->setKnownSuperClassNodeSet(visitor.mFoundKnwonSuperClassNodeSet);
					visitSuperSubConceptItems(startConceptItem, true, false, &visitor);
					if (visitor.mFoundPossibleSuperClassNodeSet) {
						startConceptItem->setPossibleSuperClassNodeSet(visitor.mFoundPossibleSuperClassNodeSet);
						return true;
					}
				}
				return false;
			}


















			bool COptimizedComplexConceptAnsweringHandler::searchSuperSubSubClassesResult(COptimizedComplexConceptItem* startConceptItem, CAnswererContext* answererContext) {
				class CSuperSubsumptionOptimizedComplexConceptItemVisitor : public COptimizedComplexConceptItemVisitor {
				public:
					QSet<CHierarchyNode*>* mFoundMinimumSubClassNodeSet = nullptr;
					QSet<CHierarchyNode*>* mFoundMaximumSubClassNodeSet = nullptr;

					cint64 mSuperItemVisitingLimit = 50;
					cint64 mSuperItemResultVisitingLimit = 10;

					cint64 mSubItemVisitingLimit = 50;
					cint64 mSubItemResultVisitingLimit = 10;

					bool visitComplexSuperConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSuperItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getSubClassNodesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							QSet<CHierarchyNode*>* superClassNodeSet = item->getDirectSubClassNodeSet();
							if (!mFoundMinimumSubClassNodeSet) {
								mFoundMinimumSubClassNodeSet = new QSet<CHierarchyNode*>();
							}
							mFoundMinimumSubClassNodeSet->operator+=(*superClassNodeSet);
							continueVisitingSuperItems = false;
							if (--mSuperItemResultVisitingLimit < 0) {
								return false;
							}
						}
						if (--mSuperItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}

					bool visitComplexSubConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSubItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getSubClassNodesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							QSet<CHierarchyNode*>* superClassNodeSet = item->getDirectSubClassNodeSet();
							if (!mFoundMaximumSubClassNodeSet) {
								mFoundMaximumSubClassNodeSet = new QSet<CHierarchyNode*>();
							}
							mFoundMaximumSubClassNodeSet->operator+=(*superClassNodeSet);
							continueVisitingSubItems = false;
							if (--mSuperItemResultVisitingLimit < 0) {
								return false;
							}
							continueVisitingSubItems = false;
							if (--mSubItemResultVisitingLimit < 0) {
								return false;
							}
						}
						if (--mSubItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}

				} visitor;
				visitSuperSubConceptItems(startConceptItem, false, true, &visitor);



				QSet<CHierarchyNode*> testingSubClassCandidateSet;
				if (visitor.mFoundMinimumSubClassNodeSet) {
					visitor.mFoundMinimumSubClassNodeSet;
					for (auto node : *visitor.mFoundMinimumSubClassNodeSet) {
						testingSubClassCandidateSet.insert(node);
					}
				}
				QSet<CHierarchyNode*>* directSuperClassNodeSet = startConceptItem->getDirectSuperClassNodeSet();
				if (directSuperClassNodeSet) {
					for (QSet<CHierarchyNode*>::const_iterator it = directSuperClassNodeSet->constBegin(), itEnd = directSuperClassNodeSet->constEnd(); it != itEnd; ++it) {
						CHierarchyNode* hierNode(*it);
						testingSubClassCandidateSet.insert(hierNode);
					}
				}
				QSet<CHierarchyNode*>* possibleClassNodeTestingSet = getDirectReducedSuperNodeSet(testingSubClassCandidateSet);
				startConceptItem->setPossibleSubClassTestingNodeSet(possibleClassNodeTestingSet);
				QList<CHierarchyNode*>* possibleClassNodeTestingList = new QList<CHierarchyNode *>();
				startConceptItem->setPossibleSubClassNodeTestingList(possibleClassNodeTestingList);


				for (auto node : *possibleClassNodeTestingSet) {
					possibleClassNodeTestingList->append(node);
				}

				QSet<CHierarchyNode*>* directSubClassNodeSet = new QSet<CHierarchyNode*>();
				startConceptItem->setDirectSubClassNodeSet(directSubClassNodeSet);


				visitSuperSubConceptItems(startConceptItem, true, false, &visitor);
				if (visitor.mFoundMaximumSubClassNodeSet) {
					startConceptItem->setMaximumSubClassNodeSet(visitor.mFoundMaximumSubClassNodeSet);
					return true;
				}
				return false;
			}















			bool COptimizedComplexConceptAnsweringHandler::searchSuperSubSatisfiabilityResult(COptimizedComplexConceptItem* startConceptItem, CAnswererContext* answererContext) {
				class CSatisfiabilityOptimizedComplexConceptItemVisitor : public COptimizedComplexConceptItemVisitor {
				public: 
					bool mSatisfiabilityResult = false;
					bool mFoundSatisfiability = false;
					bool visitComplexConceptItem(COptimizedComplexConceptItem* item, bool superItem, bool subItem) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getSatisfiableClassNodesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							if (superItem) {
								if (!item->isSatisfiable()) {
									mSatisfiabilityResult = false;
									mFoundSatisfiability = true;
									return false;
								}
							}
							if (subItem) {
								if (item->isSatisfiable()) {
									mSatisfiabilityResult = true;
									mFoundSatisfiability = true;
									return false;
								}
							}
						}
						return true;
					}

				} visitor;
				visitSuperSubConceptItems(startConceptItem, true, true, &visitor);
				if (visitor.mFoundSatisfiability) {
					startConceptItem->setSatisfiability(visitor.mSatisfiabilityResult);
					startConceptItem->getComputationProcess()->getSatisfiableClassNodesComputationProcess(true)->incDerivedComputationCount()->setComputationProcessFinished(true);
					return true;
				}
				return false;
			}



			bool COptimizedComplexConceptAnsweringHandler::visitSuperSubConceptItems(COptimizedComplexConceptItem* startConceptItem, bool visitSubItems, bool visitSuperItems, COptimizedComplexConceptItemVisitor* visitor) {
				reorderConceptClassItems();
				if (visitSuperItems) {
					QList<COptimizedComplexConceptItem*> superVisitingList;
					QSet<COptimizedComplexConceptItem*> superVisitingSet;
					superVisitingList.append(startConceptItem);
					while (!superVisitingList.isEmpty()) {
						COptimizedComplexConceptItem* visitingItem = superVisitingList.takeFirst();
						bool continueVisiting = true;
						if (!visitor->visitComplexSuperConceptItem(visitingItem, continueVisiting)) {
							return false;
						}
						if (continueVisiting) {
							for (COptimizedComplexConceptItem* superItem : *visitingItem->getSuperComplexConceptItemSet()) {
								if (!superVisitingSet.contains(superItem)) {
									superVisitingSet.insert(superItem);
									superVisitingList.append(superItem);
								}
							}
						}
					}
				}
				if (visitSubItems) {
					QList<COptimizedComplexConceptItem*> subVisitingList;
					QSet<COptimizedComplexConceptItem*> subVisitingSet;
					subVisitingList.append(startConceptItem);
					while (!subVisitingList.isEmpty()) {
						COptimizedComplexConceptItem* visitingItem = subVisitingList.takeFirst();
						bool continueVisiting = true;
						if (!visitor->visitComplexSubConceptItem(visitingItem, continueVisiting)) {
							return false;
						}
						if (continueVisiting) {
							for (COptimizedComplexConceptItem* subItem : *visitingItem->getSubComplexConceptItemSet()) {
								if (!subVisitingSet.contains(subItem)) {
									subVisitingSet.insert(subItem);
									subVisitingList.append(subItem);
								}
							}
						}
					}
				}
				return true;
			}


			bool itemSortMorePredecessorsThan(const TConceptItemPredecessorSet& item1, const TConceptItemPredecessorSet& item2) {
				return item1.second->count() > item1.second->count();
			}


			bool COptimizedComplexConceptAnsweringHandler::searchTrivialSuperSubConceptItems(COptimizedComplexConceptItem* conceptItem) {
				QSet<TConceptNegPair>* atomicConceptSet = conceptItem->getAtomicConcpetSet();
				QList<COptimizedComplexConceptItem*> testingConceptItemList;
				for (TConceptNegPair atomicConNegPair : *atomicConceptSet) {
					bool atomicConceptClass = false;
					if (atomicConNegPair.first->hasClassName() && !atomicConNegPair.second) {						
						atomicConceptClass = true;
					}
					bool created = false;
					COptimizedComplexConceptItem* atomicConceptItem = mOntoAnsweringItem->getComplexConceptItem(atomicConNegPair.first, atomicConNegPair.second, atomicConceptClass, &created);
					if (created) {
						COptimizedComplexConceptItem* topConceptItem = mOntoAnsweringItem->getTopConceptItem();
						topConceptItem->getSubComplexConceptItemSet()->insert(atomicConceptItem);
						atomicConceptItem->getSuperComplexConceptItemSet()->insert(topConceptItem);
					}
					if (atomicConceptItem && atomicConceptItem != conceptItem) {
						testingConceptItemList.append(atomicConceptItem);
					}
				}
				reorderConceptClassItems();

				//TODO: sorting super concept list??, remove indirect super concepts
				QSet<COptimizedComplexConceptItem*>* superConceptItemSet = conceptItem->getSuperComplexConceptItemSet();
				QSet<COptimizedComplexConceptItem*>* subConceptItemSet = conceptItem->getSubComplexConceptItemSet();

				if (testingConceptItemList.isEmpty()) {
					testingConceptItemList.append(mOntoAnsweringItem->getTopConceptItem());
				}
				
				QList<TConceptItemPredecessorSet> testingConceptItemPredSetList;
				while (!testingConceptItemList.isEmpty()) {
					COptimizedComplexConceptItem* superConceptItem = testingConceptItemList.takeFirst();
					testingConceptItemPredSetList.append(TConceptItemPredecessorSet(superConceptItem, collectComplexConceptItemPredecessors(superConceptItem)));
				}
				qSort(testingConceptItemPredSetList.begin(), testingConceptItemPredSetList.end(), itemSortMorePredecessorsThan);

				QSet<COptimizedComplexConceptItem*>* commonPredItemSet = nullptr;
				while (!testingConceptItemPredSetList.isEmpty()) {
                    TConceptItemPredecessorSet conceptItemPredecessorSet = testingConceptItemPredSetList.takeFirst();
					if (!commonPredItemSet) {
						testingConceptItemList.append(conceptItemPredecessorSet.first);
						commonPredItemSet = conceptItemPredecessorSet.second;
					} else {
						if (!commonPredItemSet->contains(conceptItemPredecessorSet.first)) {
							testingConceptItemList.append(conceptItemPredecessorSet.first);
							collectComplexConceptItemPredecessors(conceptItemPredecessorSet.first, commonPredItemSet);
							delete conceptItemPredecessorSet.second;
						}
					}
				}
				delete commonPredItemSet;

				while (!testingConceptItemList.isEmpty()) {
					COptimizedComplexConceptItem* superConceptItem = testingConceptItemList.takeFirst();
					bool foundSubsumption = false;
					for (COptimizedComplexConceptItem* testingConceptItem : *superConceptItem->getSubComplexConceptItemSet()) {
						if (isTriviallySubsumedConceptItem(conceptItem,testingConceptItem)) {
							testingConceptItemList.append(testingConceptItem);
							foundSubsumption = true;
						}
					}
					if (!foundSubsumption) {
						superConceptItemSet->insert(superConceptItem);
					}
				}

				for (auto superConceptItem : *superConceptItemSet) {
					//TODO: handle equivalence, merge with other items
					for (auto childSuperConceptItem : *superConceptItem->getSubComplexConceptItemSet()) {
						if (isTriviallySubsumedConceptItem(childSuperConceptItem, conceptItem)) {
							testingConceptItemList.append(superConceptItem);
						}
					}
				}

				while (!testingConceptItemList.isEmpty()) {
					COptimizedComplexConceptItem* subConceptItem = testingConceptItemList.takeFirst();
					bool foundSubsumption = false;
					for (COptimizedComplexConceptItem* testingConceptItem : *subConceptItem->getSubComplexConceptItemSet()) {
						if (isTriviallySubsumedConceptItem(testingConceptItem, conceptItem)) {
							testingConceptItemList.append(testingConceptItem);
							foundSubsumption = true;
						}
					}
					if (!foundSubsumption) {
						subConceptItemSet->insert(subConceptItem);
					}
				}

				for (COptimizedComplexConceptItem* subConceptItem : *conceptItem->getSubComplexConceptItemSet()) {
					subConceptItem->getSuperComplexConceptItemSet()->insert(conceptItem);
					for (COptimizedComplexConceptItem* superConceptItem : *conceptItem->getSuperComplexConceptItemSet()) {
						if (subConceptItem->hasSuperComplexConceptItem(superConceptItem)) {
							subConceptItem->getSuperComplexConceptItemSet()->remove(superConceptItem);
							superConceptItem->getSuperComplexConceptItemSet()->remove(subConceptItem);
						}
					}
				}
				for (COptimizedComplexConceptItem* superConceptItem : *conceptItem->getSuperComplexConceptItemSet()) {
					superConceptItem->getSubComplexConceptItemSet()->insert(conceptItem);
				}

				return true;
			}



			bool COptimizedComplexConceptAnsweringHandler::isAnsweringFinished(CAnswererContext* answererContext) {
				return !mOntoAnsweringItem->hasCurrentlyAnsweringQueries();
			}





			CAnsweringProgress* COptimizedComplexConceptAnsweringHandler::getAnsweringProgress() {


				if (mTotalPossibleInstancesCount != mTestedPossibleInstancesCount) {
					cint64 tested = mTestedPossibleInstancesCount;
					cint64 total = mTotalPossibleInstancesCount;
					cint64 elapsedTime = mAnsweringStartTime.elapsed();
					double remainingMilliSeconds = 0.;
					double progessPercent = 0.;
					if (elapsedTime <= 0 && total > 0) {
						remainingMilliSeconds = 0.;
					} else {
						remainingMilliSeconds = ((double)elapsedTime / (double)tested) * (total - tested);
					}
					if (total <= 0) {
						progessPercent = 0.;
					} else {
						progessPercent = ((double)tested / (double)total) * 100.;
					}
					mAnsweringProgress.setProgessPercent(progessPercent);
					mAnsweringProgress.setRemainingMilliSeconds(remainingMilliSeconds);
					mAnsweringProgress.setTestedInstances(tested);
					mAnsweringProgress.setTotalInstances(total);
				} else {
					mAnsweringProgress.setProgessPercent(0.);
					mAnsweringProgress.setRemainingMilliSeconds(0.);
					mAnsweringProgress.setTestedInstances(mTestedPossibleInstancesCount);
					mAnsweringProgress.setTotalInstances(mTotalPossibleInstancesCount);
				}
				return &mAnsweringProgress;
			}



			CAnsweringHandler* COptimizedComplexConceptAnsweringHandler::storeAnsweringStatistics() {
				CAnsweringStatisticsCollectionStrings* stats = new CAnsweringStatisticsCollectionStrings();
				addAnsweringStatistics(stats, false);
				mOntoAnsweringItem->setDifferenceStoredAnsweringStatisticsCollectionStrings(stats);
				return this;
			}


			CAnsweringHandler* COptimizedComplexConceptAnsweringHandler::addAnsweringStatistics(CAnsweringStatistics* stats, bool lastStoredDiff) {
				CAnsweringCalculationStatisticsCollectionStrings* calcStatsColl = mOntoAnsweringItem->getAnsweringStatisticsCollectionStrings();
				CAnsweringStatisticsCollectionStrings* diffStats = mOntoAnsweringItem->getDifferenceStoredAnsweringStatisticsCollectionStrings();
				if (calcStatsColl) {
					for (const QString& statName : calcStatsColl->getStatisticsNameStringList()) {
						cint64 value = calcStatsColl->getStatisticIntegerValue(statName);
						if (lastStoredDiff && diffStats) {
							value = value - diffStats->getStatisticIntegerValue(statName);
						}
						stats->addProcessingStatistics(statName, value);
					}
				}

				CAnsweringStatistics* tmpStats = stats;
				if (lastStoredDiff && diffStats) {
					tmpStats = new CAnsweringStatisticsCollectionStrings();
				}
				CAnsweringHandlingStatistics* handlingStats = mOntoAnsweringItem->getAnsweringHandlingStatistics();
				if (handlingStats) {
					stats->addProcessingStatistics("answering-processed-query-count", handlingStats->getTotalAnsweredQueriesCount());
					stats->addProcessingStatistics("answering-existential-query-part-absorption-count", handlingStats->getExistentialQueryPartAbsorptionCount());
					stats->addProcessingStatistics("answering-complex-concept-item-creation-count", handlingStats->getCreatedComplexConceptItemsCount());
					stats->addProcessingStatistics("answering-requested-realization-requirements-count", handlingStats->getRequestedRealizationRequirementCount());
					stats->addProcessingStatistics("answering-complex-concept-item-candidate-propagation-count", handlingStats->getComplexConceptItemsCandidatePropagationCount());
					stats->addProcessingStatistics("answering-complex-concept-item-sub-class-subsumption-count", handlingStats->getComplexConceptItemsSubClassSubsumptionTestingCount());
					stats->addProcessingStatistics("answering-complex-concept-item-super-class-subsumption-count", handlingStats->getComplexConceptItemsSuperClassSubsumptionTestingCount());
					stats->addProcessingStatistics("answering-existential-query-part-entailment-testing-count", handlingStats->getExistentialQueryPartEntailmentTestingCount());
					stats->addProcessingStatistics("answering-existential-query-part-variable-binding-propagation-count", handlingStats->getExistentialQueryPartVariableBindingPropagationCount());
					stats->addProcessingStatistics("answering-existential-query-part-variable-binding-confirmation-count", handlingStats->getExistentialQueryPartVariableBindingConfirmationCount());
					stats->addProcessingStatistics("answering-complex-concept-assertion-entailment-testing-count", handlingStats->getComplexConceptAssertionEntailmentTestingCount());
				}
				if (lastStoredDiff && diffStats) {
					for (const QString& statName : tmpStats->getStatisticsNameStringList()) {
						cint64 value = tmpStats->getStatisticIntegerValue(statName);
						value = value - diffStats->getStatisticIntegerValue(statName);
						stats->addProcessingStatistics(statName, value);
					}
				}

				return this;
			}































			COptimizedComplexVariableIndividualBindings* COptimizedComplexConceptAnsweringHandler::createBindingsForVariableCompositionItems(cint64 bindingSize, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				return new COptimizedComplexVariableIndividualBindings(bindingSize);
			}



			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexConceptAnsweringHandler::createBindingsCardinalitesForVariableCompositionItems(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				return new COptimizedComplexVariableIndividualBindingsCardinality();
			}



			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexConceptAnsweringHandler::createBindingsLinkerForVariableCompositionItems(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinality, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				return new COptimizedComplexVariableIndividualBindingsCardinalityLinker(bindings, cardinality);
			}





			void COptimizedComplexConceptAnsweringHandler::computeVariableCompositionItemFromConceptItemBase(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableConceptBaseItem* conceptBaseItem = (COptimizedComplexVariableConceptBaseItem*)compVarItem;
				COptimizedComplexConceptItem* conItem = conceptBaseItem->getConceptItem();
				COptimizedComplexVariableIndividualMapping* variableMapping = conceptBaseItem->getVariableMapping();

				CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
				QSet<CRealizationIndividualInstanceItemReference>* instanceItemSet = conItem->getKnownInstanceItemSet();

				if (instanceItemSet) {
					COptimizedComplexVariableIndividualBindings* binding = nullptr;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;
					for (auto item : *instanceItemSet) {
						if (!binding) {
							binding = createBindingsForVariableCompositionItems(1, compVarItem, buildingVarItem);
						}
						binding->setBinding(0, item);
						if (!linker) {
							linker = createBindingsLinkerForVariableCompositionItems(binding, nullptr, compVarItem, buildingVarItem);
						}
						if (variableMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
							binding = nullptr;
							linker = nullptr;
						}
					}
				}

				finishVariableCompositionItemComputation(buildingVarItem, compVarItem);


				//debugCheckVariableMappingContainsSolution(buildingVarItem, compVarItem);
			}

			void COptimizedComplexConceptAnsweringHandler::computeVariableCompositionItemReduction(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableReductionItem* reductionItem = (COptimizedComplexVariableReductionItem*)compVarItem;
				cint64 redIdx = reductionItem->getReductionVariableIndex();
				COptimizedComplexVariableIndividualMapping* reducedVarMapping = reductionItem->getVariableMapping();

				CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();

				COptimizedComplexVariableCompositionItem* baseItem = reductionItem->getBaseItem();
				COptimizedComplexVariableIndividualMapping* baseVarMapping = baseItem->getVariableMapping();
				cint64 baseSize = baseVarMapping->getBindingSize();
				cint64 reducedSize = baseSize - 1;
				COptimizedComplexVariableIndividualBindings* reducedBindings = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* reducedCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;
				cint64 sameIndividualsCount = 0;
				for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinkerIt = baseVarMapping->getLastAddedBindingsCardinalityLinker(); bindingLinkerIt; bindingLinkerIt = bindingLinkerIt->getNext()) {
					COptimizedComplexVariableIndividualBindings* bindings = bindingLinkerIt->getBindings();
					COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinkerIt->getCardinalities();

					if (!reducedBindings) {
						reducedBindings = createBindingsForVariableCompositionItems(reducedSize, compVarItem, buildingVarItem);
					}
					if (!reducedCardinalites) {
						reducedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
					}
					if (reducedCardinalites) {
						if (cardinalites) {
							*reducedCardinalites = *cardinalites;
						} else {
							reducedCardinalites->initCardinalities(1, 1);
						}
					}

					TIndividualInstanceItemDataBinding& reducedBinding = bindings->getBinding(redIdx);
					CInstanceBindingIndividualCountingVisitor visitor;
					sameRealization->visitSameIndividuals(reducedBinding.reference, &visitor);
					if (visitor.individualCount > 1) {
						reducedCardinalites->setSameIndividualsSeparatlyConsideredCardinality(visitor.individualCount * reducedCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
					}
					sameIndividualsCount += visitor.individualCount;

					cint64 nextIdx = 0;
					for (cint64 i = 0; i < baseSize; ++i) {
						if (i != redIdx) {
							reducedBindings->setBinding(nextIdx++, bindings->getBinding(i));
						}
					}
					if (!linker) {
						linker = createBindingsLinkerForVariableCompositionItems(reducedBindings, reducedCardinalites, compVarItem, buildingVarItem);
					}
					if (reducedVarMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
						reducedBindings = nullptr;
						reducedCardinalites = nullptr;
						linker = nullptr;
					}

				}

				finishVariableCompositionItemComputation(buildingVarItem, compVarItem);

				if (mConfExtendedLogging) {
					LOG(INFO, getDomain(), logTr("Reduce computation from %1 mappings resulted in %2 mappings.").arg(baseVarMapping->size()).arg(compVarItem->getVariableMapping()->size()), this);
				}


				//cint64 totalSeparatlyConsideredCardinality = 0;
				//cint64 totalJointlyConsideredCardinality = 0;

				//cint64 maxSeparatlyConsideredCardinality = 0;
				//cint64 maxJointlyConsideredCardinality = 0;

				//for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinkerIt = reducedVarMapping->getLastAddedBindingsCardinalityLinker(); bindingLinkerIt; bindingLinkerIt = bindingLinkerIt->getNext()) {
				//	COptimizedComplexVariableIndividualBindings* bindings = bindingLinkerIt->getBindings();
				//	COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinkerIt->getCardinalities();

				//	maxSeparatlyConsideredCardinality = qMax(maxSeparatlyConsideredCardinality, cardinalites->getSameIndividualsSeparatlyConsideredCardinality());
				//	maxJointlyConsideredCardinality = qMax(maxJointlyConsideredCardinality, cardinalites->getSameIndividualsJointlyConsideredCardinality());

				//	totalSeparatlyConsideredCardinality += cardinalites->getSameIndividualsSeparatlyConsideredCardinality();
				//	totalJointlyConsideredCardinality += cardinalites->getSameIndividualsJointlyConsideredCardinality();
				//}

				//LOG(INFO, getDomain(), logTr("Reduce computation has total cardinality %1 (separately) and %2 (jointly).").arg(totalSeparatlyConsideredCardinality).arg(totalJointlyConsideredCardinality), this);
				//if (sameIndividualsCount != totalSeparatlyConsideredCardinality || totalJointlyConsideredCardinality != baseVarMapping->size()) {
				//	bool debug = true;
				//}

			}

			COptimizedComplexConceptAnsweringHandler* COptimizedComplexConceptAnsweringHandler::computeVariableCompositionItemJoin(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, bool& processing) {
				COptimizedComplexVariableJoiningItem* joiningItem = (COptimizedComplexVariableJoiningItem*)compVarItem;

				COptimizedComplexVariableCompositionItem* leftItem = joiningItem->getLeftItem();
				COptimizedComplexVariableCompositionItem* rightItem = joiningItem->getRightItem();

				COptimizedComplexVariableIndividualMapping* leftVarMapping = leftItem->getVariableMapping();
				COptimizedComplexVariableIndividualMapping* rightVarMapping = rightItem->getVariableMapping();
				COptimizedComplexVariableIndividualMapping* joinedVarMapping = joiningItem->getVariableMapping();

				cint64 leftSize = leftVarMapping->getBindingSize();
				cint64 rightSize = rightVarMapping->getBindingSize();
				cint64 joinSize = joinedVarMapping->getBindingSize();


				CXLinker<cint64>* leftKeyBindingLinker = nullptr;
				CXLinker<cint64>* rightKeyBindingLinker = nullptr;

				COptimizedComplexVariableJoiningBindingPositionMapping* rightPositionMapping = joiningItem->getPositionMapping();
				QHash<cint64, cint64>* rightPositionMappingHash = rightPositionMapping->getJoiningItemPositionMapping();
				bool itemSameVarReductionJoining = rightPositionMapping->isSelfSameVariableReduction();
				cint64* rightPosMapArray = new cint64[rightSize];
				cint64* leftPosMapArray = new cint64[leftSize];
				bool* sameItemVarPosUsedArray = new bool[leftSize];
				for (cint64 i = 0; i < leftSize; ++i) {
					leftPosMapArray[i] = -1;
					sameItemVarPosUsedArray[i] = false;
				}
				for (cint64 i = 0; i < rightSize; ++i) {
					rightPosMapArray[i] = -1;
				}
				if (!itemSameVarReductionJoining) {
					for (cint64 i = 0; i < leftSize; ++i) {
						leftPosMapArray[i] = i;
					}
				} else {
					cint64 leftNextIdx = 0;
					for (cint64 i = 0; i < leftSize; ++i) {
						if (!rightPositionMappingHash->contains(i)) {
							leftPosMapArray[i] = leftNextIdx++;
						}
					}
				}
				for (QHash<cint64, cint64>::const_iterator it = rightPositionMappingHash->constBegin(), itEnd = rightPositionMappingHash->constEnd(); it != itEnd; ++it) {
					cint64 rightIdx = it.key();
					cint64 mappedJoinedIdx = it.value();
					if (mappedJoinedIdx < leftSize) {
						sameItemVarPosUsedArray[rightIdx] = true;
						sameItemVarPosUsedArray[mappedJoinedIdx] = true;
						CXLinker<cint64>* rightKeyLinker = new CXLinker<cint64>(rightIdx);
						rightKeyBindingLinker = rightKeyLinker->append(rightKeyBindingLinker);
						CXLinker<cint64>* leftKeyLinker = new CXLinker<cint64>(mappedJoinedIdx);
						leftKeyBindingLinker = leftKeyLinker->append(leftKeyBindingLinker);
					}
					rightPosMapArray[rightIdx] = mappedJoinedIdx;
				}
				if (itemSameVarReductionJoining) {
					for (cint64 i = 0; i < leftSize; ++i) {
						if (!sameItemVarPosUsedArray[i]) {
							CXLinker<cint64>* rightKeyLinker = new CXLinker<cint64>(i);
							rightKeyBindingLinker = rightKeyLinker->append(rightKeyBindingLinker);
							CXLinker<cint64>* leftKeyLinker = new CXLinker<cint64>(i);
							leftKeyBindingLinker = leftKeyLinker->append(leftKeyBindingLinker);
						}
					}
				}

				COptimizedComplexVariableJoiningHash joiningHash(leftKeyBindingLinker, rightKeyBindingLinker);

				if (leftVarMapping->size() > mConfSamplingBasedJoinMappingSize || rightVarMapping->size() > mConfSamplingBasedJoinMappingSize) {

					cint64 leftSampleKeyCount = 0;
					cint64 rightSampleKeyCount = 0;

					cint64 sampleLimit = 100;

					function<void(bool left, CXLinker<cint64>* keyBindingLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker*& varMappingIt, COptimizedComplexVariableIndividualBindingsCardinalityLinker* varMappingItEnd, cint64& sampleLimit)> joiningHashInserterFunc = [&](bool left, CXLinker<cint64>* keyBindingLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker*& varMappingIt, COptimizedComplexVariableIndividualBindingsCardinalityLinker* varMappingItEnd, cint64& sampleCount) -> void {
						for (cint64 i = 0; varMappingIt && varMappingIt != varMappingItEnd && (i < sampleLimit || sampleLimit == -1); ++i) {
							COptimizedComplexVariableIndividualBindings* bindings = varMappingIt->getBindings();
							COptimizedComplexVariableJoiningHasher bindingHasher(bindings->getBindingArray(), keyBindingLinker);
							COptimizedComplexVariableJoiningData& joiningData = joiningHash[bindingHasher];
							if (!joiningData.hasBindings(left)) {
								++sampleCount;
							}
							CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* linker = new CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>(varMappingIt);
							joiningData.addBindingLinker(left, linker);
							varMappingIt = varMappingIt->getNext();
						}
					};

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* leftVarMappingIt = leftVarMapping->getLastAddedBindingsCardinalityLinker();
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* leftVarMappingItEnd = nullptr;

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* rightVarMappingIt = rightVarMapping->getLastAddedBindingsCardinalityLinker();
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* rightVarMappingItEnd = nullptr;

					joiningHashInserterFunc(true, leftKeyBindingLinker, leftVarMappingIt, leftVarMappingItEnd, leftSampleKeyCount);
					joiningHashInserterFunc(false, rightKeyBindingLinker, rightVarMappingIt, rightVarMappingItEnd, rightSampleKeyCount);






					COptimizedComplexVariableIndividualBindingsCardinalityLinker* insertingVarMappingIt = leftVarMappingIt;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* insertingVarMappingItEnd = leftVarMappingItEnd;

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* checkingVarMappingIt = rightVarMapping->getLastAddedBindingsCardinalityLinker();
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* checkingVarMappingItEnd = rightVarMappingItEnd;

					CXLinker<cint64>* insertingKeyBindingLinker = leftKeyBindingLinker;
					CXLinker<cint64>* checkingKeyBindingLinker = rightKeyBindingLinker;

					cint64 leftEstimatedTotal = (double)leftSampleKeyCount / (double)sampleLimit * (double)leftVarMapping->size();
					cint64 rightEstimatedTotal = (double)rightSampleKeyCount / (double)sampleLimit * (double)rightVarMapping->size();
					cint64 estimatedTotal = leftEstimatedTotal;
					cint64 remainingSampleKeyCount = leftSampleKeyCount;

					bool insertedLeft = true;

					if (rightEstimatedTotal < leftEstimatedTotal) {
						estimatedTotal = rightEstimatedTotal;
						remainingSampleKeyCount = rightSampleKeyCount;

						insertingVarMappingIt = rightVarMappingIt;
						insertingVarMappingItEnd = rightVarMappingItEnd;

						insertingKeyBindingLinker = rightKeyBindingLinker;
						checkingKeyBindingLinker = leftKeyBindingLinker;
						checkingVarMappingIt = leftVarMapping->getLastAddedBindingsCardinalityLinker();
						checkingVarMappingItEnd = leftVarMappingItEnd;

						insertedLeft = false;
					}


					joiningHash.reserve(estimatedTotal);
					sampleLimit = -1;
					joiningHashInserterFunc(insertedLeft, insertingKeyBindingLinker, insertingVarMappingIt, insertingVarMappingItEnd, remainingSampleKeyCount);


					COptimizedComplexVariableIndividualBindings* joinedBindings = nullptr;
					COptimizedComplexVariableIndividualBindingsCardinality* joinedCardinalites = nullptr;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker = nullptr;

					for (; checkingVarMappingIt != checkingVarMappingItEnd && checkingVarMappingIt; checkingVarMappingIt = checkingVarMappingIt->getNext()) {
						COptimizedComplexVariableIndividualBindings* checkingBindings = checkingVarMappingIt->getBindings();
						COptimizedComplexVariableIndividualBindingsCardinality* checkingCardinalites = checkingVarMappingIt->getCardinalities();
						COptimizedComplexVariableJoiningHasher bindingHasher(checkingBindings->getBindingArray(), checkingKeyBindingLinker);
						const COptimizedComplexVariableJoiningData& joiningData = joiningHash.value(bindingHasher);

						CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* insertedBindingLinker = joiningData.getBindingLinker(insertedLeft);
						for (CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* insertedBindingLinkerIt = insertedBindingLinker; insertedBindingLinkerIt; insertedBindingLinkerIt = insertedBindingLinkerIt->getNext()) {


							COptimizedComplexVariableIndividualBindingsCardinalityLinker* insertedBindingCardinalitiesLinker = insertedBindingLinkerIt->getData();
							TIndividualInstanceItemDataBinding* insertedBinding = insertedBindingCardinalitiesLinker->getBindings()->getBindingArray();
							COptimizedComplexVariableIndividualBindingsCardinality* insertedCardinalites = insertedBindingCardinalitiesLinker->getCardinalities();

							TIndividualInstanceItemDataBinding* leftBinding = insertedBinding;
							TIndividualInstanceItemDataBinding* rightBinding = checkingBindings->getBindingArray();
							if (!insertedLeft) {
								leftBinding = checkingBindings->getBindingArray();
								rightBinding = insertedBinding;
							}

							if (!joinedBindings) {
								joinedBindings = createBindingsForVariableCompositionItems(joinSize, compVarItem, buildingVarItem);
							}
							if (!joinedCardinalites && (checkingCardinalites || insertedCardinalites)) {
								joinedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
							}
							if (checkingCardinalites && insertedCardinalites) {
								*joinedCardinalites = *insertedCardinalites;
								joinedCardinalites->setSameIndividualsJointlyConsideredCardinality(joinedCardinalites->getSameIndividualsJointlyConsideredCardinality() * checkingCardinalites->getSameIndividualsJointlyConsideredCardinality());
								joinedCardinalites->setSameIndividualsSeparatlyConsideredCardinality(joinedCardinalites->getSameIndividualsSeparatlyConsideredCardinality() * checkingCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
							} else if (insertedCardinalites) {
								*joinedCardinalites = *insertedCardinalites;
							} else if (checkingCardinalites) {
								*joinedCardinalites = *checkingCardinalites;
							}


							for (cint64 i = 0; i < leftSize; ++i) {
								if (leftPosMapArray[i] != -1) {
									joinedBindings->setBinding(leftPosMapArray[i], leftBinding[i]);
								}
							}
							for (cint64 i = 0; i < rightSize; ++i) {
								if (rightPosMapArray[i] != -1 && rightPosMapArray[i] >= leftSize) {
									joinedBindings->setBinding(rightPosMapArray[i], rightBinding[i]);
								}
							}

							if (!joinedLinker) {
								joinedLinker = createBindingsLinkerForVariableCompositionItems(joinedBindings, joinedCardinalites, compVarItem, buildingVarItem);
							}
							if (joinedVarMapping->addInsertingBindingsCardinalityLinker(joinedLinker, true)) {
								joinedBindings = nullptr;
								joinedCardinalites = nullptr;
								joinedLinker = nullptr;
							}
						}
					}



				} else {


					for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingsLinkerIt = leftVarMapping->getLastAddedBindingsCardinalityLinker(); bindingsLinkerIt; bindingsLinkerIt = bindingsLinkerIt->getNext()) {
						COptimizedComplexVariableIndividualBindings* bindings = bindingsLinkerIt->getBindings();
						COptimizedComplexVariableJoiningHasher bindingHasher(bindings->getBindingArray(), leftKeyBindingLinker);
						COptimizedComplexVariableJoiningData& joiningData = joiningHash[bindingHasher];
						CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* linker = new CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>(bindingsLinkerIt);
						joiningData.addLeftBindingLinker(linker);
					}

					for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingsLinkerIt = rightVarMapping->getLastAddedBindingsCardinalityLinker(); bindingsLinkerIt; bindingsLinkerIt = bindingsLinkerIt->getNext()) {
						COptimizedComplexVariableIndividualBindings* bindings = bindingsLinkerIt->getBindings();
						COptimizedComplexVariableJoiningHasher bindingHasher(bindings->getBindingArray(), rightKeyBindingLinker);
						COptimizedComplexVariableJoiningData& joiningData = joiningHash[bindingHasher];
						CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* linker = new CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>(bindingsLinkerIt);
						joiningData.addRightBindingLinker(linker);
					}


					COptimizedComplexVariableIndividualBindings* joinedBindings = nullptr;
					COptimizedComplexVariableIndividualBindingsCardinality* joinedCardinalites = nullptr;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker = nullptr;

					for (COptimizedComplexVariableJoiningHash::const_iterator it = joiningHash.constBegin(), itEnd = joiningHash.constEnd(); it != itEnd; ++it) {
						const COptimizedComplexVariableJoiningData& joiningData = it.value();
						CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* leftBindingLinker = joiningData.getLeftBindingLinker();
						CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* rightBindingLinker = joiningData.getRightBindingLinker();
						if (leftBindingLinker && rightBindingLinker) {
							for (CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* leftBindingLinkerIt = leftBindingLinker; leftBindingLinkerIt; leftBindingLinkerIt = leftBindingLinkerIt->getNext()) {
								TIndividualInstanceItemDataBinding* leftBinding = leftBindingLinkerIt->getData()->getBindings()->getBindingArray();
								COptimizedComplexVariableIndividualBindingsCardinality* leftCardinalites = leftBindingLinkerIt->getData()->getCardinalities();
								for (CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* rightBindingLinkerIt = rightBindingLinker; rightBindingLinkerIt; rightBindingLinkerIt = rightBindingLinkerIt->getNext()) {
									TIndividualInstanceItemDataBinding* rightBinding = rightBindingLinkerIt->getData()->getBindings()->getBindingArray();
									COptimizedComplexVariableIndividualBindingsCardinality* rightCardinalites = rightBindingLinkerIt->getData()->getCardinalities();

									if (!joinedBindings) {
										joinedBindings = createBindingsForVariableCompositionItems(joinSize, compVarItem, buildingVarItem);
									}
									if (!joinedCardinalites && (leftCardinalites || rightCardinalites)) {
										joinedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
									}
									if (leftCardinalites && rightCardinalites) {
										*joinedCardinalites = *rightCardinalites;
										joinedCardinalites->setSameIndividualsJointlyConsideredCardinality(joinedCardinalites->getSameIndividualsJointlyConsideredCardinality() * leftCardinalites->getSameIndividualsJointlyConsideredCardinality());
										joinedCardinalites->setSameIndividualsSeparatlyConsideredCardinality(joinedCardinalites->getSameIndividualsSeparatlyConsideredCardinality() * leftCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
									} else if (rightCardinalites) {
										*joinedCardinalites = *rightCardinalites;
									} else if (leftCardinalites) {
										*joinedCardinalites = *leftCardinalites;
									}

									for (cint64 i = 0; i < leftSize; ++i) {
										if (leftPosMapArray[i] != -1) {
											joinedBindings->setBinding(leftPosMapArray[i], leftBinding[i]);
										}
									}
									for (cint64 i = 0; i < rightSize; ++i) {
										if (rightPosMapArray[i] != -1 && rightPosMapArray[i] >= leftSize) {
											joinedBindings->setBinding(rightPosMapArray[i], rightBinding[i]);
										}
									}

									if (!joinedLinker) {
										joinedLinker = createBindingsLinkerForVariableCompositionItems(joinedBindings, joinedCardinalites, compVarItem, buildingVarItem);
									}
									if (joinedVarMapping->addInsertingBindingsCardinalityLinker(joinedLinker, true)) {
										joinedBindings = nullptr;
										joinedCardinalites = nullptr;
										joinedLinker = nullptr;
									}

									//QList<CSameInstanceItem*> instList;
									//QList<CIndividual*> indiList;
									//QStringList indiStringList;
									//for (cint64 i = 0; i < joinSize; ++i) {
									//	CSameInstanceItem* instItem = CSameInstanceItem*)joinedBinding.getBinding(i);
									//	COptimizedKPSetIndividualItem* indiItem = (COptimizedKPSetIndividualItem*)instItem;
									//	CIndividual* individual = indiItem->getIndividual();
									//	QString individualString = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
									//	if (individualString == "http://www.Department0.University0.edu/GraduateStudent29") {
									//		bool bug = true;
									//	}
									//	instList.append(instItem);
									//	indiStringList.append(individualString);
									//	indiList.append(individual);
									//}


								}
							}
						}
					}

				}



				finishVariableCompositionItemComputation(buildingVarItem, compVarItem);

				if (mConfExtendedLogging) {
					LOG(INFO, getDomain(), logTr("Join computation for %1 and %2 mappings resulted in %3 mappings.").arg(leftVarMapping->size()).arg(rightVarMapping->size()).arg(compVarItem->getVariableMapping()->size()), this);
				}

				//if (joinedVarMapping->count() == 0) {
				//	bool debug = true;
				//}
				//debugCheckVariableMappingContainsSolution(buildingVarItem, compVarItem);
				return this;
			}





			COptimizedComplexConceptAnsweringHandler* COptimizedComplexConceptAnsweringHandler::computeVariableCompositionItemPropagation(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing) {
				COptimizedComplexVariableRolePropagationAbstractItem* rolePropItem = (COptimizedComplexVariableRolePropagationAbstractItem*)compVarItem;
				COptimizedComplexVariableCompositionItem* baseVarItem = rolePropItem->getBaseItem();
				CRole* role = rolePropItem->getRole();
				bool inversed = rolePropItem->getInversed();
				COptimizedComplexVariableIndividualMapping* baseVariableMapping = baseVarItem->getVariableMapping();
				COptimizedComplexVariableIndividualMapping* variableMapping = rolePropItem->getVariableMapping();
				CRoleRealization* roleReal = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();

				QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* waitingProcessedRealizationIteratorDataList = rolePropItem->getWaitingProcessedRealizationIteratorDataList();
				QHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* instItemRolePropInstIterationDataHash = rolePropItem->getInstanceItemRolePropagationInstanceIterationDataHash();

				// get variable index
				cint64 varIdx = rolePropItem->getPropagationVariableIndex();
				cint64 baseBindingSize = baseVariableMapping->getBindingSize();
				cint64 newBindingSize = baseVariableMapping->getBindingSize();
				if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::ROLE_PROPAGATION) {
					newBindingSize = baseVariableMapping->getBindingSize() + 1;
				}
				cint64 newVarIdx = baseBindingSize;
				bool joining = false;
				bool replacing = false;
				if (compVarItem->getCompositionType() == COptimizedComplexVariableRolePropagationJoiningItem::ROLE_PROPAGATION_JOIN) {
					COptimizedComplexVariableRolePropagationJoiningItem* joinRolePropItem = (COptimizedComplexVariableRolePropagationJoiningItem*)compVarItem;
					joining = true;
					newVarIdx = joinRolePropItem->getJoiningVariableIndex();
				}
				if (compVarItem->getCompositionType() == COptimizedComplexVariableRolePropagationReplacementItem::ROLE_PROPAGATION_REPLACEMENT) {
					COptimizedComplexVariableRolePropagationReplacementItem* replRolePropItem = (COptimizedComplexVariableRolePropagationReplacementItem*)compVarItem;
					replacing = true;
					newVarIdx = replRolePropItem->getReplacementVariableIndex();
				}

				cint64* baseBindingMapping = new cint64[newBindingSize];
				for (cint64 i = 0; i < baseBindingSize; ++i) {
					baseBindingMapping[i] = i;
				}

				QList<COntologyProcessingRequirement*> reqList;
				QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*> newWaitingProcessedRealizationIteratorDataList;

				CRoleInstantiatedItem* roleItem = roleReal->getRoleInstantiatedItem(role);

				COptimizedComplexVariableIndividualBindings* propagedBindings = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* propagedCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* propagedLinker = nullptr;


				function<void(COptimizedComplexVariableIndividualBindingsCardinalityLinker* varBindingCardLinker, const CRealizationIndividualInstanceItemReference& propInstItem)> variableMappingCreationFunc = [&](COptimizedComplexVariableIndividualBindingsCardinalityLinker* varBindingCardLinker, const CRealizationIndividualInstanceItemReference& propInstItem) {

					COptimizedComplexVariableIndividualBindings* varBinding = varBindingCardLinker->getBindings();
					COptimizedComplexVariableIndividualBindingsCardinality* varCardinalities = varBindingCardLinker->getCardinalities();
					if (!propagedBindings) {
						propagedBindings = createBindingsForVariableCompositionItems(newBindingSize, compVarItem, buildingVarItem);
					}
					if (!propagedCardinalites && varCardinalities) {
						propagedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
					}
					if (varCardinalities) {
						*propagedCardinalites = *varCardinalities;
					}


					COptimizedComplexVariableIndividualBindings nextNewBinding(newBindingSize);
					for (cint64 i = 0; i < baseBindingSize; ++i) {
						propagedBindings->setBinding(baseBindingMapping[i], varBinding->getBinding(i));
					}
					bool inserting = true;
					if (joining) {
						if (propInstItem == varBinding->getBinding(newVarIdx).reference) {
							inserting = true;
						} else {
							inserting = false;
						}
					}
					propagedBindings->setBinding(newVarIdx, propInstItem);

					if (!propagedLinker) {
						propagedLinker = createBindingsLinkerForVariableCompositionItems(propagedBindings, propagedCardinalites, compVarItem, buildingVarItem);
					}

					if (inserting) {
						if (variableMapping->addInsertingBindingsCardinalityLinker(propagedLinker, true)) {
							propagedBindings = nullptr;
							propagedCardinalites = nullptr;
							propagedLinker = nullptr;
						}
					}

				};


				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				CRoleInstantiatedItem* roleInstancesItem = roleRealization->getRoleInstantiatedItem(role);

				function<void(COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* iteratorData)> iterationDataHandleFunc = [&](COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* iteratorData) {
					QSet<CRealizationIndividualInstanceItemReference>* rolePropInstItemSet = iteratorData->getPropagationInstanceItemSet();
					CRealizationIndividualInstanceItemReferenceIterator* fillerIterator = iteratorData->getRealizationIterator();

					while (!fillerIterator->atEnd() && !fillerIterator->requiresInitialization()) {
						CRealizationIndividualInstanceItemReference fillerInstanceItemRef = fillerIterator->currentIndividualInstanceItemReference();
						iteratorData->getPropagationInstanceItemSet()->insert(fillerInstanceItemRef);
						fillerIterator->moveNext();
					}

					if (fillerIterator->requiresInitialization()) {
						COntologyProcessingIteratorRealizationRequirement* roleRealReq = new COntologyProcessingIteratorRealizationRequirement(fillerIterator);
						newWaitingProcessedRealizationIteratorDataList.append(iteratorData);
						reqList.append(roleRealReq);
					} else if (fillerIterator->atEnd()) {
						delete fillerIterator;
						iteratorData->setRealizationIterator(nullptr);
						QList<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* variableMappingIteratorList = iteratorData->getVariableMappingPropagationIteratorList();
						while (!variableMappingIteratorList->isEmpty()) {
							COptimizedComplexVariableIndividualBindingsCardinalityLinker* varBindingCardLinker = variableMappingIteratorList->takeFirst();
							if (!rolePropInstItemSet->isEmpty()) {
								for (const CRealizationIndividualInstanceItemReference& propInstItem : *rolePropInstItemSet) {
									variableMappingCreationFunc(varBindingCardLinker, propInstItem);
								}
							}
						}
					}
				};

				bool successorCollectionContinued = false;
				while (!waitingProcessedRealizationIteratorDataList->isEmpty()) {
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* iteratorData = waitingProcessedRealizationIteratorDataList->takeFirst();
					iteratorData->getRealizationIterator()->begin();
					iterationDataHandleFunc(iteratorData);
					successorCollectionContinued = true;
				}



				if (!successorCollectionContinued) {
					for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = baseVariableMapping->getLastAddedBindingsCardinalityLinker(); linker; linker = linker->getNext()) {

						COptimizedComplexVariableIndividualBindings* varBinding = linker->getBindings();
						CRealizationIndividualInstanceItemReference instItemRef = varBinding->getBinding(varIdx).reference;

						//if (CIRIName::getRecentIRIName(instItemRef.getIndividual()->getIndividualNameLinker()) == "http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#PauillacRegion") {
						//	bool debug = true;
						//}

						COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*& iterationData = (*instItemRolePropInstIterationDataHash)[instItemRef];
						if (!iterationData) {
							iterationData = new COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData();
							iterationData->setRealizationIterator(roleRealization->getRoleFillerInstancesIterator(instItemRef, roleInstancesItem, inversed, mRoleFillerRealizationSorting));
							iterationData->getVariableMappingPropagationIteratorList()->append(linker);
							iterationDataHandleFunc(iterationData);
						} else if (!iterationData->getRealizationIterator()) {
							QSet<CRealizationIndividualInstanceItemReference>* rolePropInstItemSet = iterationData->getPropagationInstanceItemSet();
							for (const CRealizationIndividualInstanceItemReference& propInstItem : *rolePropInstItemSet) {
								variableMappingCreationFunc(linker, propInstItem);
							}
						} else {
							iterationData->addVariableMappingPropagationIterator(linker);
						}

					}
				}

				if (reqList.isEmpty()) {
					finishVariableCompositionItemComputation(buildingVarItem, compVarItem);

					if (mConfExtendedLogging) {
						QString roleName = "?";
						if (role->getPropertyNameLinker()) {
							roleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
							if (inversed) {
								roleName = roleName + " inversed";
							}
						} else if (role->getInverseRole() && role->getInverseRole()->getPropertyNameLinker()) {
							roleName = CIRIName::getRecentIRIName(role->getInverseRole()->getPropertyNameLinker());
							if (!inversed) {
								roleName = roleName + " inversed";
							}
						}
						LOG(INFO, getDomain(), logTr("Propagation of %1 mappings over role %2 resulted in %3 mappings.").arg(baseVariableMapping->size()).arg(roleName).arg(compVarItem->getVariableMapping()->size()), this);
					}

				} else {
					CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(rolePropItem, buildingVarItem);
					buildingVarItem->incVariableCompositionItemWaitingCount();
					processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
					*waitingProcessedRealizationIteratorDataList = newWaitingProcessedRealizationIteratorDataList;
					processing = true;
				}

				//if (variableMapping->count() == 0) {
				//	bool debug = true;
				//}
				//debugCheckVariableMappingContainsSolution(buildingVarItem, compVarItem);

				return this;
			}

			void COptimizedComplexConceptAnsweringHandler::computeVariableCompositionItemFromDataLiteralBase(COptimizedComplexVariableCompositionItem* compVarItem, CAnswererContext* answererContext, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableDataLiteralBaseItem* dataBaseItem = (COptimizedComplexVariableDataLiteralBaseItem*)compVarItem;
				CRole* dataRole = dataBaseItem->getDataRole();
				CIndividualReference indiRef = dataBaseItem->getIndividualReference();
				COptimizedComplexVariableIndividualMapping* variableMapping = dataBaseItem->getVariableMapping();

				cint64 varIdx = 0;

				QSet<CDataLiteral*> dataLiteralSet;
				getInstantiatedDataLiterals(indiRef, dataRole, &dataLiteralSet, answererContext);

				COptimizedComplexVariableIndividualBindings* binding = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;

				for (CDataLiteral* dataLiteral : dataLiteralSet) {
					if (!binding) {
						binding = createBindingsForVariableCompositionItems(1, compVarItem, buildingVarItem);
					}

					binding->setBinding(varIdx, dataLiteral);

					if (!linker) {
						linker = createBindingsLinkerForVariableCompositionItems(binding, nullptr, compVarItem, buildingVarItem);
					}
					if (variableMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
						binding = nullptr;
						linker = nullptr;
					}
				}

				finishVariableCompositionItemComputation(buildingVarItem, compVarItem);

				if (mConfExtendedLogging) {
					QString roleName = CIRIName::getRecentIRIName(dataRole->getPropertyNameLinker());
					LOG(INFO, getDomain(), logTr("Initialized %1 mappings for data role %2.").arg(variableMapping->size()).arg(roleName), this);
				}
			}

			void COptimizedComplexConceptAnsweringHandler::computeVariableCompositionItemAbsorptionBasedExtension(COptimizedComplexVariableCompositionItem* compVarItem, bool &processing, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool &continueProcessingBuildingVarItem) {
				COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absBasedItem = (COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*)compVarItem;

				COptimizedComplexVariableIndividualMapping* possibleVarIndiBindings = absBasedItem->getPossibleVariableMapping();
				if (possibleVarIndiBindings && !possibleVarIndiBindings->isEmpty()) {
					COptimizedComplexVariableIndividualMapping* testingVarIndiBindings = absBasedItem->getTestingVariableMapping();
					if (!testingVarIndiBindings) {
						testingVarIndiBindings = new COptimizedComplexVariableIndividualMapping(possibleVarIndiBindings->getBindingSize());
						absBasedItem->setTestingVariableMapping(testingVarIndiBindings);
					}

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* testingVarIndiBindingCardLinker = possibleVarIndiBindings->takeBindingsCardinalityLinker();
					testingVarIndiBindings->insert(testingVarIndiBindingCardLinker, testingVarIndiBindingCardLinker);

					CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
					processing = createVariableBindingConfirmationTest(absBasedItem, buildingVarItem, testingVarIndiBindingCardLinker, sameRealization, answererContext);
					buildingVarItem->addComputeVariableMappingItem(absBasedItem);
					if (possibleVarIndiBindings->size() > 0) {
						continueProcessingBuildingVarItem = true;
					}

				} else if (!buildingVarItem->isWaitingVariableBindingsConfirmation()) {
					finishVariableCompositionItemComputation(buildingVarItem, compVarItem);
					if (!compVarItem->getVariableMapping()->isEmpty()) {
						QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* absorptionHanldingQueryExtensionJoiningList = buildingVarItem->getAbsorptionBasedHandlingExtensionItemList();
						absorptionHanldingQueryExtensionJoiningList->append(absBasedItem);
					}

					if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("Obtained %1 mappings via absorbed query entailment tests.").arg(compVarItem->getVariableMapping()->size()), this);
					}

				}
			}

			void COptimizedComplexConceptAnsweringHandler::computeVariableCompositionItemDataLiteralExtension(COptimizedComplexVariableCompositionItem* compVarItem, CAnswererContext* answererContext, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableDataLiteralExtensionItem* dataExtItem = (COptimizedComplexVariableDataLiteralExtensionItem*)compVarItem;
				COptimizedComplexVariableCompositionItem* baseVarItem = dataExtItem->getBaseItem();
				CRole* dataRole = dataExtItem->getRole();
				COptimizedComplexVariableIndividualMapping* baseVariableMapping = baseVarItem->getVariableMapping();
				COptimizedComplexVariableIndividualMapping* variableMapping = dataExtItem->getVariableMapping();

				CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();

				// get variable index
				cint64 varIdx = dataExtItem->getVariableIndex();
				cint64 baseBindingSize = baseVariableMapping->getBindingSize();
				cint64 newBindingSize = baseVariableMapping->getBindingSize() + 1;

				cint64* baseBindingMapping = new cint64[newBindingSize];
				for (cint64 i = 0; i < baseBindingSize; ++i) {
					baseBindingMapping[i] = i;
				}
				cint64 newVarIdx = baseBindingSize;

				COptimizedComplexVariableIndividualBindings* extBindings = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* extCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* extLinker = nullptr;


				for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = baseVariableMapping->getLastAddedBindingsCardinalityLinker(); linker; linker = linker->getNext()) {

					COptimizedComplexVariableIndividualBindings* varBinding = linker->getBindings();
					COptimizedComplexVariableIndividualBindingsCardinality* varCardinalities = linker->getCardinalities();

					CRealizationIndividualInstanceItemReference instItem = varBinding->getBinding(varIdx).reference;


					QList<CIndividualReference> indiList;
					CInstanceBindingIndividualCollectionVisitor visitor(&indiList);
					sameRealization->visitSameIndividuals(instItem, &visitor);

					QSet<CDataLiteral*> dataLiteralSet;
					for (CIndividualReference indi : indiList) {
						getInstantiatedDataLiterals(indi, dataRole, &dataLiteralSet, answererContext);
					}

					for (CDataLiteral* dataLiteral : dataLiteralSet) {

						if (!extBindings) {
							extBindings = createBindingsForVariableCompositionItems(newBindingSize, compVarItem, buildingVarItem);
						}
						if (!extCardinalites && varCardinalities) {
							extCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
						}
						if (varCardinalities) {
							*extCardinalites = *varCardinalities;
						}

						for (cint64 i = 0; i < baseBindingSize; ++i) {
							extBindings->setBinding(baseBindingMapping[i], varBinding->getBinding(i));
						}
						extBindings->setBinding(newVarIdx, dataLiteral);

						if (!extLinker) {
							extLinker = createBindingsLinkerForVariableCompositionItems(extBindings, extCardinalites, compVarItem, buildingVarItem);
						}
						if (variableMapping->addInsertingBindingsCardinalityLinker(extLinker, true)) {
							extBindings = nullptr;
							extCardinalites = nullptr;
							extLinker = nullptr;
						}

					}

				}
				finishVariableCompositionItemComputation(buildingVarItem, compVarItem);

				if (mConfExtendedLogging) {
					QString roleName = CIRIName::getRecentIRIName(dataRole->getPropertyNameLinker());
					LOG(INFO, getDomain(), logTr("Extending %1 mappings via data role %2 resulted in %3 mappings.").arg(variableMapping->size()).arg(roleName).arg(compVarItem->getVariableMapping()->size()), this);
				}


				//if (variableMapping->count() == 0) {
				//	bool debug = true;
				//}
				//debugCheckVariableMappingContainsSolution(buildingVarItem, compVarItem);
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
