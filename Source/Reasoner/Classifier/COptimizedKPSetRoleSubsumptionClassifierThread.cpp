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

#include "COptimizedKPSetRoleSubsumptionClassifierThread.h"

namespace Konclude {

	namespace Reasoner {

		namespace Classifier {





			COptimizedKPSetRoleSubsumptionClassifierThread::COptimizedKPSetRoleSubsumptionClassifierThread(CReasonerManager *reasonerManager, bool dataRoleClassification) 
					: CSubsumptionClassifierThread(reasonerManager), CLogIdentifier("::Konclude::Reasoner::Kernel::Classifier::OptimizedKPSetRoleSubsumptionClassifierThread",this) {
				satTestedCount = 0;
				totalToSatTestClassCount = 0;
				mClassificationCount = 0;
				subsumTestedCount = 0;
				totalToSubsumTestCount = 0;

				mConfPossSubsumCalcOrderRoleSorted = true;
				mConfPossSubsumCalcOrderFewestSubsumptionSorted = false;

				mConfPossSubsumCalcOrderTopDown = true;
				mConfPossSubsumCalcOrderBottomUp = false;

				mConfPossSubsumPseudoModelPretest = true;
				mConfPossSubsumPseudoModelTest = true;
				mConfWriteDebuggingData = false;

				mStatProcesedSubsumMessCount = 0;
				mStatProcesedPossSubsumInitMessCount = 0;
				mStatProcesedPossSubsumUpdateMessCount = 0;
				mStatProcesedPseudoModelMessCount = 0;

				mInterpretedSubsumptionCalculationCount = 0;
				mOrderedSubsumptionCalculationCount = 0;
				mPseudoModelPretestSubsumptionCalculationCount = 0;
				mCreatedCalculationTaskCount = 0;
				mRecievedCallbackCount = 0;

				mDataRolesClassification = dataRoleClassification;
			}


			COptimizedKPSetRoleSubsumptionClassifierThread::~COptimizedKPSetRoleSubsumptionClassifierThread() {
			}


			CRolePropertiesHierarchy *COptimizedKPSetRoleSubsumptionClassifierThread::createInitialHierarchyForOntology(CConcreteOntology *ontology, CConfigurationBase *config) {
				CRBox* rBox = ontology->getRBox();
				CRole* topRole = nullptr;
				CRole* bottomRole = nullptr;
				bool classifyDataRoles = mDataRolesClassification;
				if (classifyDataRoles) {
					topRole = rBox->getTopDataRole();
					bottomRole = rBox->getBottomDataRole();
				} else {
					topRole = rBox->getTopObjectRole();
					bottomRole = rBox->getBottomObjectRole();
				}
				CRolePropertiesHierarchy *hierarchy = new CRolePropertiesHierarchy(topRole,bottomRole);
				return hierarchy;
			}


			void COptimizedKPSetRoleSubsumptionClassifierThread::readCalculationConfig(CCalculationConfigurationExtension *config) {
				if (config) {
					bool configErrorFlag = false;
					cint64 processorCount = 1;
					QString processorCountString = CConfigDataReader::readConfigString(config,"Konclude.Calculation.ProcessorCount",QString(),&configErrorFlag);
					if (!configErrorFlag) {
						if (processorCountString == "AUTO") {
							processorCount = CThread::idealThreadCount();
						} else {
							qint64 convertedWorkerCount = processorCountString.toInt(&configErrorFlag);
							if (configErrorFlag) {
								processorCount = convertedWorkerCount;
							}
						}
					}

					bool mulConfigErrorFlag = false;
					cint64 multiplicator = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedKPSetRoleSubsumptionClassifier.MultipliedUnitsParallelSatisfiableCalculationCount",1,&mulConfigErrorFlag);
					confMaxTestParallelCount = processorCount*multiplicator;
					bool maxConfigErrorFlag = false;
					cint64 maxParallel = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedKPSetRoleSubsumptionClassifier.MaximumParallelSatisfiableCalculationCount",1,&maxConfigErrorFlag);
					if (!maxConfigErrorFlag) {
						if (!mulConfigErrorFlag) {
							confMaxTestParallelCount = qMin(confMaxTestParallelCount,maxParallel);
						} else {
							confMaxTestParallelCount = maxParallel;
						}
					}
					double factor = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedKPSetRoleSubsumptionClassifier.ParallelSatisfiableCalculationCreationFactor",1000)/1000.;
					confMinTestParallelCount = confMaxTestParallelCount*factor;

					mConfWriteDebuggingData = CConfigDataReader::readConfigBoolean(config,"Konclude.Debugging.WriteDebuggingData",false);

				} else {
					confMaxTestParallelCount = 1;
					confMinTestParallelCount = 1;
					mConfWriteDebuggingData = false;
				}

			}





			COptimizedKPSetRoleSubsumptionClassifierThread* COptimizedKPSetRoleSubsumptionClassifierThread::createInitialSubsumptionSatisfiabilityTestingOrderFromBuildData(COptimizedKPSetRoleOntologyClassificationItem* ontPropItem) {
				CConcreteOntology* ontology = ontPropItem->getOntology();
				CConcept* topConcept = ontology->getTBox()->getTopConcept();

				CRBox *rBox = ontology->getRBox();

				CRole* topRole = nullptr;
				CRole* bottomRole = nullptr;

				bool classifyDataRoles = ontPropItem->isDataRolesClassification();
				if (classifyDataRoles) {
					topRole = rBox->getTopDataRole();
					bottomRole = rBox->getBottomDataRole();
				} else {
					topRole = rBox->getTopObjectRole();
					bottomRole = rBox->getBottomObjectRole();
				}

				cint64 satRoleCount = 1;
				CBOXSET<CRole*>* activeRoleHash = rBox->getActivePropertyRoleSet(false);
				if (activeRoleHash) {
					for (CBOXSET<CRole*>::const_iterator it = activeRoleHash->constBegin(), itEnd = activeRoleHash->constEnd(); it != itEnd; ++it) {
						CRole *role = (*it);
						if (role->isDataRole() == classifyDataRoles) {
							if (role && role != topRole && role != bottomRole) {
								++satRoleCount;
								ontPropItem->getRoleSatisfiableTestItem(role,true);

							}
						}
					}
				}

				CClassifierStatistics* statistics = ontPropItem->getClassifierStatistics();
				statistics->incTotalSatisfiableTestCount(satRoleCount);


				COptimizedKPSetRoleTestingItem* topRoleItem = ontPropItem->getRoleSatisfiableTestItem(topRole,true);
				COptimizedKPSetRoleTestingItem* bottomRoleItem = ontPropItem->getRoleSatisfiableTestItem(bottomRole,true);

				ontPropItem->initTopBottomSatisfiableTestingItems(topRoleItem,bottomRoleItem);

				CConcept* univConnNomValueConcept = ontology->getTBox()->getUniversalConnectionNominalValueConcept();

				QList<COptimizedKPSetRoleTestingItem*>* nextItemList = ontPropItem->getNextSatisfiableTestingItemList();
				QSet<COptimizedKPSetRoleTestingItem*>* nextCandItemSet = ontPropItem->getNextCandidateSatisfiableTestingItemSet();
				QSet<COptimizedKPSetRoleTestingItem*>* remainingCandItemSet = ontPropItem->getRemainingCandidateSatisfiableTestingItemSet();

				QList<COptimizedKPSetRoleTestingItem*>* roleItemList = ontPropItem->getRoleSatisfiableTestItemList();
				for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator it = roleItemList->constBegin(), itEnd = roleItemList->constEnd(); it != itEnd; ++it) {
					COptimizedKPSetRoleTestingItem* roleItem(*it);
					if (roleItem != bottomRoleItem) {

						CRole* role = roleItem->getTestingRole();
						bool complex = false;
						for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
							CRole* superRole = superRoleIt->getData();

							if (!superRoleIt->isNegated() && superRole != topRole && superRole != role) {
								COptimizedKPSetRoleTestingItem* superRoleItem = ontPropItem->getRoleSatisfiableTestItem(superRole);
								if (superRoleItem) {
									roleItem->addSubsumerRoleItem(superRoleItem);
								}
							}

							if (superRole->getDomainConceptList()) {
								complex = true;
							}
							if (superRole->getRangeConceptList()) {
								complex = true;
							}
							if (topConcept->getOperandList() || univConnNomValueConcept) {
								complex = true;
							}
						}

						cint64 foundSubsumerCount = 0;
						ontPropItem->incRemainingSatisfiableTestsCount();
						if (!complex) {
							roleItem->setResultSatisfiableDerivated(true);
							roleItem->setPossibleSubsumptionMapInitialized(true);
						} else {

							for (CSortedNegLinker<CRole*>* superRoleIt = role->getSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
								CRole* superRole = superRoleIt->getData();
								if (!superRoleIt->isNegated()) {
									COptimizedKPSetRoleTestingItem* superRoleItem = ontPropItem->getRoleSatisfiableTestItem(superRole);
									if (superRoleItem) {
										foundSubsumerCount++;
										superRoleItem->addSuccessorSatisfiableTestItem(roleItem);
										roleItem->incUnprocessedPredecessorItems();
									}
								}
							}
						}

						if (foundSubsumerCount) {
							nextItemList->append(roleItem);
						} else {
							nextCandItemSet->insert(roleItem);
						}
					}
				}			
				return this;
			}







			CConcept* COptimizedKPSetRoleSubsumptionClassifierThread::createTemporaryConcept(CConcreteOntology* tmpRoleRealOntology) {
				CConceptVector* conVec = tmpRoleRealOntology->getTBox()->getConceptVector();
				CConcept* concept = CObjectAllocator< CConcept >::allocateAndConstruct(tmpRoleRealOntology->getOntologyContext()->getMemoryAllocationManager());
				concept->initConcept();

				cint64 newConTag = tmpRoleRealOntology->getTBox()->getNextConceptID();

				concept->setConceptTag(newConTag);
				conVec->setData(concept->getConceptTag(),concept);
				return concept;
			}

			void COptimizedKPSetRoleSubsumptionClassifierThread::addTemporaryConceptOperand(CConcept* concept, CConcept* opConcept, bool negated, CConcreteOntology* tmpRoleRealOntology) {
				CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(tmpRoleRealOntology->getOntologyContext()->getMemoryAllocationManager());
				opConLinker->init(opConcept,negated);
				concept->addOperandLinker(opConLinker);
				concept->incOperandCount();
			}



			COptimizedKPSetRoleSubsumptionClassifierThread* COptimizedKPSetRoleSubsumptionClassifierThread::createTemporaryRoleClassificationOntology(COptimizedKPSetRoleOntologyClassificationItem* item) {
				CConcreteOntology* ontology = item->getOntology();
				CConcreteOntology* tmpRoleRealOntology = item->getTemporaryRoleClassificationOntology();
				if (!tmpRoleRealOntology) {
					tmpRoleRealOntology = new CConcreteOntology(ontology,ontology->getConfiguration());		
					tmpRoleRealOntology->setOntologyID(ontology->getOntologyID());
					tmpRoleRealOntology->setConsistence(ontology->getConsistence());
					tmpRoleRealOntology->getTBox()->setMinimalNextConceptID(ontology->getTBox()->getMinimalNextConceptID());

					QSet<CConcept*> compTransformConceptSet;
					QHash<CConcept*,COptimizedKPSetRoleTestingItem*>* markerConRolInsItemHash = item->getMarkerConceptInstancesItemHash();

					CABox* abox = item->getOntology()->getABox();

					CABox* tempRoleRealAbox = tmpRoleRealOntology->getABox();
					CIndividualVector* indiVec = tempRoleRealAbox->getIndividualVector(true);

					CIndividual* tmpIndiProp = new CIndividual();
					tmpIndiProp->initIndividual(tempRoleRealAbox->getNextIndividualId(true));
					indiVec->setData(tmpIndiProp->getIndividualID(),tmpIndiProp);
					CIndividual* tmpIndiMarker = new CIndividual();
					tmpIndiMarker->initIndividual(tempRoleRealAbox->getNextIndividualId(true));
					indiVec->setData(tmpIndiMarker->getIndividualID(),tmpIndiMarker);
					tmpIndiProp->setTemporaryFakeIndividual(true);
					tmpIndiMarker->setTemporaryFakeIndividual(true);


					item->setTemporaryMarkerIndividual(tmpIndiMarker);
					item->setTemporaryPropagationIndividual(tmpIndiProp);

					CConcept* allPropConcept = createTemporaryConcept(tmpRoleRealOntology);
					allPropConcept->setOperatorCode(CCAND);


					COptimizedKPSetRoleTestingItem* topRoleInstItem = item->getTopRoleSatisfiableTestItem();
					COptimizedKPSetRoleTestingItem* bottomRoleInstItem = item->getBottomRoleSatisfiableTestItem();
					QList<COptimizedKPSetRoleTestingItem*>* roleItemList = item->getRoleSatisfiableTestItemList();
					for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator itemIt = roleItemList->constBegin(), itemItEnd = roleItemList->constEnd(); itemIt != itemItEnd; ++itemIt) {
						COptimizedKPSetRoleTestingItem* roleItem(*itemIt);
						if (roleItem != bottomRoleInstItem) {
							CRole* role = roleItem->getTestingRole();

							CConcept* markerConcept = createTemporaryConcept(tmpRoleRealOntology);
							markerConcept->setOperatorCode(CCMARKER);
							markerConcept->setRole(role);

							CConcept* existConcept = createTemporaryConcept(tmpRoleRealOntology);
							existConcept->setOperatorCode(CCVALUE);
							existConcept->setRole(role);
							existConcept->setNominalIndividual(tmpIndiMarker);
							roleItem->setTemporaryExistConcept(existConcept);

							CConcept* propConcept = createTemporaryConcept(tmpRoleRealOntology);
							propConcept->setOperatorCode(CCALL);
							propConcept->setRole(role);
							addTemporaryConceptOperand(propConcept,markerConcept,false,tmpRoleRealOntology);

							addTemporaryConceptOperand(allPropConcept,propConcept,false,tmpRoleRealOntology);
							roleItem->setTemporaryMarkerConcept(markerConcept);
							markerConRolInsItemHash->insert(markerConcept,roleItem);
							roleItem->setTemporaryPropagationConcept(propConcept);
							if (role->isComplexRole()) {
								compTransformConceptSet.insert(propConcept);
							}
						}
					}

					if (!compTransformConceptSet.isEmpty()) {
						CPreProcessContextBase* preprocessContext = new CPreProcessContextBase(tmpRoleRealOntology,ontology->getConfiguration());
						CRoleChainAutomataTransformationPreProcess* roleChainAutomataTransformPreprocessor = new CRoleChainAutomataTransformationPreProcess();
						roleChainAutomataTransformPreprocessor->preprocess(tmpRoleRealOntology,&compTransformConceptSet,preprocessContext);
						delete preprocessContext;
						delete roleChainAutomataTransformPreprocessor;						
					}

					item->setTemporaryRoleClassificationOntology(tmpRoleRealOntology);
					item->setTemporaryAllPropagationConcept(allPropConcept);
				}
				return this;
			}



			
			CSubsumptionClassifierThread *COptimizedKPSetRoleSubsumptionClassifierThread::scheduleOntologyClassification(CConcreteOntology *ontology, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config) {


				CRolePropertiesHierarchy* hierarchy = createInitialHierarchyForOntology(ontology,config);

				COptimizedKPSetRoleOntologyClassificationItem *ontPropItem = new COptimizedKPSetRoleOntologyClassificationItem(config,statistics);
				ontPropItem->initClassificationItem(ontology,hierarchy,mDataRolesClassification);
				ontItemList.append(ontPropItem);
				processingOntItemList.append(ontPropItem);
				ontItemHash.insert(ontology,ontPropItem);

				readCalculationConfig(ontPropItem->getCalculationConfiguration());

				if (CConfigDataReader::readConfigBoolean(ontPropItem->getCalculationConfiguration(),"Konclude.Calculation.Classification.IndividualDependenceTracking",true)) {
					ontPropItem->setIndividualDependenceTrackingCollector(new CIndividualDependenceTrackingCollector());
				}



				createInitialSubsumptionSatisfiabilityTestingOrderFromBuildData(ontPropItem);
				createTemporaryRoleClassificationOntology(ontPropItem);

				if (satTestedCount == totalToSatTestClassCount) {
					satTestedCount = 0;
					totalToSatTestClassCount = ontPropItem->getRemainingSatisfiableTestsCount();
					classStartTime.start();
				} else {
					totalToSatTestClassCount += ontPropItem->getRemainingSatisfiableTestsCount();
				}
				++mClassificationCount;

				return this;
			}






			QString COptimizedKPSetRoleSubsumptionClassifierThread::getStatusString() {
				statUpdateMutex.lock();
				QString string = statusString;
				string.detach();
				statUpdateMutex.unlock();
				return string;
			}



			CClassificationProgress* COptimizedKPSetRoleSubsumptionClassifierThread::getClassificationProgress() {
				if (satTestedCount != totalToSatTestClassCount || subsumTestedCount != totalToSubsumTestCount) {					
					cint64 tested = satTestedCount + subsumTestedCount;
					cint64 total = totalToSatTestClassCount + totalToSubsumTestCount;
					cint64 elapsedTime = classStartTime.elapsed();
					double remainingMilliSeconds = 0.;
					double progessPercent = 0.;
					if (elapsedTime <= 0 && total > 0) {
						remainingMilliSeconds = 0.;
					} else {
						remainingMilliSeconds = ((double)elapsedTime / (double)tested) * (total-tested);
					}
					if (total <= 0) {
						progessPercent = 0.;
					} else {
						progessPercent = ((double)tested / (double)total) * 100.;
					}
					cint64 classificCount = mClassificationCount;
					mClassifyProgess.setClassificationCount(classificCount);
					mClassifyProgess.setProgessPercent(progessPercent);
					mClassifyProgess.setRemainingMilliSeconds(remainingMilliSeconds);
					mClassifyProgess.setTestedSatisfiable(satTestedCount);
					mClassifyProgess.setTotalSatisfiable(totalToSatTestClassCount);
					mClassifyProgess.setTestedSubsumptions(subsumTestedCount);
					mClassifyProgess.setTotalSubsumptions(totalToSubsumTestCount);
				} else {
					mClassifyProgess.setProgessPercent(0.);
					mClassifyProgess.setRemainingMilliSeconds(0.);
					mClassifyProgess.setTestedSatisfiable(satTestedCount);
					mClassifyProgess.setTotalSatisfiable(totalToSatTestClassCount);
					mClassifyProgess.setTestedSubsumptions(subsumTestedCount);
					mClassifyProgess.setTotalSubsumptions(totalToSubsumTestCount);
				}
				return &mClassifyProgess;
			}

			bool itemSortLessSubsumptionsThan(const COptimizedKPSetRoleTestingItem* item1, const COptimizedKPSetRoleTestingItem* item2) {
				return item1->getSubsumerRoleItemCount() < item2->getSubsumerRoleItemCount();
			}

			bool itemSortMoreSubsumptionsThan(const COptimizedKPSetRoleTestingItem* item1, const COptimizedKPSetRoleTestingItem* item2) {
				return item1->getSubsumerRoleItemCount() > item2->getSubsumerRoleItemCount();
			}



			void COptimizedKPSetRoleSubsumptionClassifierThread::testDebugPossibleSubsumerCorrectCounted(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem) {
				QList<COptimizedKPSetRoleTestingItem*>* classList = optKPSetClassificationItem->getRoleSatisfiableTestItemList();
				cint64 totalCount = 0;
				foreach (COptimizedKPSetRoleTestingItem* item, *classList) {
					COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
					if (possSubsumMap) {
						cint64 mapCount = 0;
						for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetRolePossibleSubsumptionData* data = it.value();
							if (data->isSubsumptionUnknown() || data->isUpdateRequired()) {
								mapCount++;
								totalCount++;
							}
						}
						if (mapCount != possSubsumMap->getRemainingPossibleSubsumptionCount()) {
							bool bug = true;
						}
					}
				}
				if (totalCount != optKPSetClassificationItem->getRemainingPossibleSubsumptionTestsCount()) {
					bool bug = true;
				}

			}

			void COptimizedKPSetRoleSubsumptionClassifierThread::testDebugPossibleSubsumerCorrectReferenced(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem) {
				QList<COptimizedKPSetRoleTestingItem*>* classList = optKPSetClassificationItem->getRoleSatisfiableTestItemList();
				if (mConfPossSubsumCalcOrderBottomUp) {
					QSet<COptimizedKPSetRoleTestingItem*>* remItemSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionTestingSet();
					foreach (COptimizedKPSetRoleTestingItem* item, *classList) {
						COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
						if (possSubsumMap) {
							for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetRolePossibleSubsumptionData* data = it.value();
								if (data->isSubsumptionUnknown() || data->isUpdateRequired()) {				
									COptimizedKPSetRoleTestingItem* possSubsumerItem = data->getTestingItem();
									if (!possSubsumerItem->getPossibleSubsumerSet(false)->contains(item) && !optKPSetClassificationItem->getCurrentPossibleSubsumptionTestingItemSet()->contains(possSubsumerItem)) {
										bool bug = true;
									}
									if (!remItemSet->contains(possSubsumerItem) && !optKPSetClassificationItem->getCurrentPossibleSubsumptionTestingItemSet()->contains(possSubsumerItem)) {
										bool bug = true;
									}
								}
							}
						}
					}
				}
			}


			QString COptimizedKPSetRoleSubsumptionClassifierThread::createDebugKPSetString(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, const QString& filename) {
				QString debugString;
				QList<COptimizedKPSetRoleTestingItem*>* propertyList = optKPSetClassificationItem->getRoleSatisfiableTestItemList();
				foreach (COptimizedKPSetRoleTestingItem* item, *propertyList) {
					QString iriPropertyNameString = CIRIName::getRecentIRIName(item->getTestingRole()->getPropertyNameLinker());
					QString propertyDebugString = QString("Property: %1\r\n").arg(iriPropertyNameString);
					QString subsumString;
					foreach (COptimizedKPSetRoleTestingItem* subsumItem, *item->getSubsumerRoleItemList()) {
						QString subsumIRIPropertyNameString = CIRIName::getRecentIRIName(subsumItem->getTestingRole()->getPropertyNameLinker());
						if (subsumString.isEmpty()) {
							subsumString += subsumIRIPropertyNameString;
						} else {
							subsumString += QString(", %1").arg(subsumIRIPropertyNameString);
						}
					}
					propertyDebugString += QString("Subsumer Roles: %1\r\n").arg(subsumString);

					QString possSubsumString;
					COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
					if (possSubsumMap) {
						for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetRolePossibleSubsumptionData* data = it.value();
							QString subsumIRIPropertyNameString = CIRIName::getRecentIRIName(data->getTestingItem()->getTestingRole()->getPropertyNameLinker());
							QString possString;
							if (data->isSubsumptionConfirmed()) {
								possString = "s";
							} else if (data->isSubsumptionInvalided()) {
								possString = "n";
							} else {
								possString = "u";
							}

							if (possSubsumString.isEmpty()) {
								possSubsumString += QString("%1(%2)").arg(subsumIRIPropertyNameString).arg(possString);
							} else {
								possSubsumString += QString(", %1(%2)").arg(subsumIRIPropertyNameString).arg(possString);
							}
						}
					}
					propertyDebugString += QString("Possible Subsumer Roles: %1\r\n\r\n\r\n").arg(possSubsumString);

					debugString += propertyDebugString;
				}

				QFile file(filename);
				if (file.open(QIODevice::WriteOnly)) {
					file.write(debugString.toLocal8Bit());
					file.close();
				}

				return debugString;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::createNextSubsumtionTest() {


				COntologyPropertyRoleClassificationItem *loopontPropItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !processingOntItemList.isEmpty()) {
					COptimizedKPSetRoleOntologyClassificationItem *ontPropItem = (COptimizedKPSetRoleOntologyClassificationItem*)processingOntItemList.first();

					if (ontPropItem == loopontPropItem) {
						// don't run into infinite loop without doing something
						break;
					}

					COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontPropItem;

					if (!optKPSetClassificationItem->hasSatisfiableTestingPhaseFinished() && !ontPropItem->isHierarchyConstructionFailed()) {

						while (!workTestCreated && optKPSetClassificationItem->hasRemainingSatisfiableTests()) {
							// get next satisfiable test

							QList<COptimizedKPSetRoleTestingItem*>* nextItemList = optKPSetClassificationItem->getNextSatisfiableTestingItemList();
							QSet<COptimizedKPSetRoleTestingItem*>* nextCandItemSet = optKPSetClassificationItem->getNextCandidateSatisfiableTestingItemSet();
							QSet<COptimizedKPSetRoleTestingItem*>* remainingCandItemSet = optKPSetClassificationItem->getRemainingCandidateSatisfiableTestingItemSet();


							COptimizedKPSetRoleTestingItem* nextSatTestItem = nullptr;
							if (!nextItemList->isEmpty()) {
								nextSatTestItem = nextItemList->takeFirst();
								remainingCandItemSet->remove(nextSatTestItem);
								nextCandItemSet->remove(nextSatTestItem);

							} else if (!nextCandItemSet->isEmpty()) {
								COptimizedKPSetRoleTestingItem* minItem = nullptr;
								cint64 minUnpPredCount = 0;
								for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = nextCandItemSet->constBegin(), itEnd = nextCandItemSet->constEnd(); it != itEnd; ++it) {
									COptimizedKPSetRoleTestingItem* item = (*it);
									cint64 predCount = item->getUnprocessedPredecessorItemCount();
									if (!minItem || minUnpPredCount < predCount) {
										minUnpPredCount = predCount;
										minItem = item;
									}
								}
								nextSatTestItem = minItem;
								remainingCandItemSet->remove(nextSatTestItem);
								nextCandItemSet->remove(nextSatTestItem);

							} else if (!remainingCandItemSet->isEmpty()) {
								COptimizedKPSetRoleTestingItem* minItem = nullptr;
								QSet<COptimizedKPSetRoleTestingItem*>::const_iterator itNext = remainingCandItemSet->constBegin();
								nextSatTestItem = *itNext;
								remainingCandItemSet->remove(nextSatTestItem);
							}


							if (nextSatTestItem && !nextSatTestItem->isSatisfiableTestOrdered()) {
								optKPSetClassificationItem->decRemainingSatisfiableTestsCount();
								optKPSetClassificationItem->incRunningSatisfiableTestsCount();
								if (calculateSatisfiable(optKPSetClassificationItem,nextSatTestItem)) {
									workTestCreated = true;
								} else {
									interpreteSatisfiableResult(optKPSetClassificationItem,nextSatTestItem->getTestingRole(),nextSatTestItem->getSatisfiableTestedResult());
								}
							}
						}


						if (!workTestCreated) {
							if (optKPSetClassificationItem->hasAllSatisfiableTestsCompleted()) {
								optKPSetClassificationItem->setSatisfiableTestingPhaseFinished(true);
								// initialize possible subsumption testing phase


								cint64 remainingSubSumTestCount = 0;

								QSet<COptimizedKPSetRoleTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionTestingSet();

								QList<COptimizedKPSetRoleTestingItem*>* nextItemList = optKPSetClassificationItem->getNextPossibleSubsumptionTestingItemList();

								COptimizedKPSetRoleTestingItem* topItem = optKPSetClassificationItem->getTopRoleSatisfiableTestItem();
								COptimizedKPSetRoleTestingItem* bottomItem = optKPSetClassificationItem->getBottomRoleSatisfiableTestItem();

								topItem->setPropagationConnected(true);

								QList<COptimizedKPSetRoleTestingItem*> propList(*optKPSetClassificationItem->getRoleSatisfiableTestItemList());
								qSort(propList.begin(),propList.end(),itemSortLessSubsumptionsThan);
								QList<COptimizedKPSetRoleTestingItem*> resevePossSubsumClassList;
								foreach (COptimizedKPSetRoleTestingItem* item, propList) {
									COptimizedKPSetRolePossibleSubsumptionMap* possSubSumMap = item->getPossibleSubsumptionMap(false);
									if (possSubSumMap && possSubSumMap->hasRemainingPossibleSubsumptions()) {	
										if (mConfPossSubsumCalcOrderTopDown) {
											nextItemList->append(item);
											remTestingSet->insert(item);
										}
										remainingSubSumTestCount += possSubSumMap->getRemainingPossibleSubsumptionCount();
										resevePossSubsumClassList.append(item);
									}
									if (!item->isPossibleSubsumptionMapInitialized()) {
										bool bug = true;
									}

									QList<COptimizedKPSetRoleTestingItem*>* subsumerList = item->getSubsumerRoleItemList();
									COptimizedKPSetRoleTestingItem* maxSubsumerItem = nullptr;
									cint64 maxSubsumerCount = 0;
									for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator it = subsumerList->constBegin(), itEnd = subsumerList->constEnd(); it != itEnd; ++it) {
										COptimizedKPSetRoleTestingItem* subsumerItem = *it;
										if (!maxSubsumerItem || subsumerItem->getSubsumerRoleItemCount() > maxSubsumerCount) {
											maxSubsumerCount = subsumerItem->getSubsumerRoleItemCount();
											maxSubsumerItem = subsumerItem;
										}
									}
									if (!maxSubsumerItem) {
										maxSubsumerItem = topItem;
									}

									bool upPropConnected = false;
									QSet<COptimizedKPSetRoleTestingItem*>* upPropSet = item->getUpPropagationItemSet();
									if (item != topItem) {
										upPropSet->insert(maxSubsumerItem);
										maxSubsumerItem->addDownPropagationItem(item);
										upPropConnected |= maxSubsumerItem->isPropagationConnected();
									}

									for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator itSub = subsumerList->constBegin(), itSubEnd = subsumerList->constEnd(); itSub != itSubEnd; ++itSub) {
										COptimizedKPSetRoleTestingItem* subsumerItem = *itSub;	
										bool alreadySubsumed = false;
										for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator itProp = upPropSet->constBegin(), itPropEnd = upPropSet->constEnd(); !alreadySubsumed && itProp != itPropEnd; ++itProp) {
											COptimizedKPSetRoleTestingItem* propItem = *itProp;
											if (propItem == subsumerItem || propItem->hasSubsumerRoleItem(subsumerItem)) {
												alreadySubsumed = true;
											}
										}
										if (!alreadySubsumed) {
											upPropSet->insert(subsumerItem);
											subsumerItem->addDownPropagationItem(item);
											upPropConnected |= subsumerItem->isPropagationConnected();
										}
									}
									if (!upPropConnected) {
										for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator itSub = subsumerList->constBegin(), itSubEnd = subsumerList->constEnd(); itSub != itSubEnd && !upPropConnected; ++itSub) {
											COptimizedKPSetRoleTestingItem* subsumerItem = *itSub;	
											if (subsumerItem->isPropagationConnected()) {
												upPropSet->insert(subsumerItem);
												subsumerItem->addDownPropagationItem(item);
												upPropConnected |= subsumerItem->isPropagationConnected();
											}
										}
										if (!upPropConnected) {
											upPropSet->insert(topItem);
											topItem->addDownPropagationItem(item);
											upPropConnected |= topItem->isPropagationConnected();
										}
									}
									item->setPropagationConnected(upPropConnected);

								}


								foreach (COptimizedKPSetRoleTestingItem* item, propList) {
									COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
									QSet<COptimizedKPSetRoleTestingItem*>* upPropSet = item->getUpPropagationItemSet();
									if (!possSubsumMap && item->isPossibleSubsumptionMapInitialized()) {
										for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator itProp = upPropSet->constBegin(), itPropEnd = upPropSet->constEnd(); itProp != itPropEnd; ++itProp) {
											COptimizedKPSetRoleTestingItem* upPropItem = *itProp;

											COptimizedKPSetRolePossibleSubsumptionMap* upPropItemPossSubsumMap = upPropItem->getPossibleSubsumptionMap(false);
											if (upPropItemPossSubsumMap) {
												for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator itUpPoss = upPropItemPossSubsumMap->constBegin(), itUpPossEnd = upPropItemPossSubsumMap->constEnd(); itUpPoss != itUpPossEnd; ++itUpPoss) {
													COptimizedKPSetRolePossibleSubsumptionData* upPossData = itUpPoss.value();
													if (!item->hasSubsumerRoleItem(upPossData->getTestingItem()) && item != upPossData->getTestingItem()) {
														if (!upPossData->isSubsumptionInvalided()) {
															upPossData->setSubsumptionInvalid(true);
															if (upPossData->isUpdateRequired()) {
																prunePossibleSubsumptions(optKPSetClassificationItem,upPropItem,upPossData);
															}
														}
													}
												}
											}
										}
									} else {
										for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator itProp = upPropSet->constBegin(), itPropEnd = upPropSet->constEnd(); itProp != itPropEnd; ++itProp) {
											COptimizedKPSetRoleTestingItem* upPropItem = *itProp;

											COptimizedKPSetRolePossibleSubsumptionMap* upPropItemPossSubsumMap = upPropItem->getPossibleSubsumptionMap(false);
											if (upPropItemPossSubsumMap) {
												COptimizedKPSetRolePossibleSubsumptionMap::const_iterator itUpPoss = upPropItemPossSubsumMap->constBegin(), itUpPossEnd = upPropItemPossSubsumMap->constEnd();
												if (possSubsumMap) {
													COptimizedKPSetRolePossibleSubsumptionMap::const_iterator itPoss = possSubsumMap->constBegin(), itPossEnd = possSubsumMap->constEnd();
													while (itPoss != itPossEnd && itUpPoss != itUpPossEnd) {
														CRole* possRole = itPoss.key().getRole();
														CRole* possUpRole = itUpPoss.key().getRole();
														if (possRole->getRoleTag() == possUpRole->getRoleTag()) {
															++itPoss;
															++itUpPoss;
														} else if (possRole->getRoleTag() < possUpRole->getRoleTag()) {
															++itPoss;
														} else if (possRole->getRoleTag() > possUpRole->getRoleTag()) {
															COptimizedKPSetRolePossibleSubsumptionData* upPossData = itUpPoss.value();
															if (!item->hasSubsumerRoleItem(upPossData->getTestingItem()) && item != upPossData->getTestingItem()) {
																if (!upPossData->isSubsumptionInvalided()) {
																	upPossData->setSubsumptionInvalid(true);
																	if (upPossData->isUpdateRequired()) {
																		prunePossibleSubsumptions(optKPSetClassificationItem,upPropItem,upPossData);
																	}
																}
															}
															++itUpPoss;
														}
													}
												}
												while (itUpPoss != itUpPossEnd) {
													COptimizedKPSetRolePossibleSubsumptionData* upPossData = itUpPoss.value();
													if (!item->hasSubsumerRoleItem(upPossData->getTestingItem()) && item != upPossData->getTestingItem()) {
														if (!upPossData->isSubsumptionInvalided()) {
															upPossData->setSubsumptionInvalid(true);
															if (upPossData->isUpdateRequired()) {
																prunePossibleSubsumptions(optKPSetClassificationItem,upPropItem,upPossData);
															}
														}
													}
													++itUpPoss;
												}
											}
										}
									}
								}

								foreach (COptimizedKPSetRoleTestingItem* item, resevePossSubsumClassList) {
									COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
									if (possSubsumMap) {
										if (possSubsumMap->hasRemainingPossibleSubsumptions()) {
											for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
												COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = it.value();
												
												if (mConfPossSubsumCalcOrderBottomUp) {
													if (possSubsumData->isSubsumptionUnknown() || possSubsumData->isUpdateRequired()) {
														COptimizedKPSetRoleTestingItem* possSubsumerItem = possSubsumData->getTestingItem();
														possSubsumerItem->getPossibleSubsumerSet(true)->insert(item);
														if (!remTestingSet->contains(possSubsumerItem)) {
															remTestingSet->insert(possSubsumerItem);
															nextItemList->append(possSubsumerItem);
														}
													}
												}
											}
										}
									}
								}


								if (mConfPossSubsumCalcOrderBottomUp) {
									for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = remTestingSet->constBegin(), itEnd = remTestingSet->constEnd(); it != itEnd; ++it) {
										COptimizedKPSetRoleTestingItem* possSubsumerItem = *it;
										QList<COptimizedKPSetRoleTestingItem*>* subsumersSortedList = new QList<COptimizedKPSetRoleTestingItem*>(possSubsumerItem->getPossibleSubsumerSet(false)->toList());										
										qSort(subsumersSortedList->begin(),subsumersSortedList->end(),itemSortMoreSubsumptionsThan);
										possSubsumerItem->setPossibleSubsumedList(subsumersSortedList);
									}
								}

								if (mConfWriteDebuggingData) {
									createDebugKPSetString(optKPSetClassificationItem,"./Debugging/Classification/propertykpsets-inital-pruned.txt");
								}

							}
						}
					}

					if (optKPSetClassificationItem->hasSatisfiableTestingPhaseFinished() && !optKPSetClassificationItem->hasPossibleSubsumptionTestingPhaseFinished() && !ontPropItem->isHierarchyConstructionFailed()) {

						QList<COptimizedKPSetRoleTestingItem*>* nextItemList = optKPSetClassificationItem->getNextPossibleSubsumptionTestingItemList();
						QSet<COptimizedKPSetRoleTestingItem*>* currentItemSet = optKPSetClassificationItem->getCurrentPossibleSubsumptionTestingItemSet();

						bool canContinueSearchingForTests = true;

						while (!workTestCreated && optKPSetClassificationItem->hasRemainingPossibleSubsumptionTests() && canContinueSearchingForTests) {
							// get next possible subsumption test
							canContinueSearchingForTests = false;

							COptimizedKPSetRoleTestingItem* nextPossSubsumTestItem = nullptr;
							if (!nextItemList->isEmpty()) {
								nextPossSubsumTestItem = nextItemList->takeFirst();
							}

							if (nextPossSubsumTestItem) {
								canContinueSearchingForTests = true;

								if (mConfPossSubsumCalcOrderTopDown) {

									COptimizedKPSetRolePossibleSubsumptionMap* possSubsumerMap = nextPossSubsumTestItem->getPossibleSubsumptionMap(false);
									if (possSubsumerMap && possSubsumerMap->hasRemainingPossibleSubsumptions()) {

										COptimizedKPSetRolePossibleSubsumptionData* possSubsumCalcData = getNextPossibleSubsumptionCalculation(optKPSetClassificationItem,nextPossSubsumTestItem);
										if (possSubsumCalcData) {
											currentItemSet->insert(nextPossSubsumTestItem);
											optKPSetClassificationItem->incRunningPossibleSubsumptionTestsCount();

											if (calculateSubsumption(optKPSetClassificationItem,nextPossSubsumTestItem,possSubsumCalcData->getTestingItem(),possSubsumCalcData)) {
												workTestCreated = true;
											} else {
												interpreteSubsumptionResult(optKPSetClassificationItem,nextPossSubsumTestItem->getTestingRole(),possSubsumCalcData->getTestingItem()->getTestingRole(),possSubsumCalcData->isSubsumptionConfirmed());
											}
										}
									} else {
										QSet<COptimizedKPSetRoleTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionTestingSet();
										remTestingSet->remove(nextPossSubsumTestItem);
									}

								} else if (mConfPossSubsumCalcOrderBottomUp) {
									COptimizedKPSetRolePossibleSubsumptionData* possSubsumCalcData = nullptr;
									COptimizedKPSetRoleTestingItem* possSubsumedItem = nullptr;


									CRole* possSubsumerRole = nextPossSubsumTestItem->getTestingRole();
									CRole* candidateRole = possSubsumerRole;

									QList<COptimizedKPSetRoleTestingItem*>* possSubsumedSortedList = nextPossSubsumTestItem->getPossibleSubsumerList();
									if (possSubsumedSortedList && !possSubsumedSortedList->isEmpty()) {										
										while (!possSubsumCalcData && !possSubsumedSortedList->isEmpty()) {
											possSubsumedItem = possSubsumedSortedList->takeFirst();
											if (nextPossSubsumTestItem->getPossibleSubsumerSet(false)->contains(possSubsumedItem)) {
												nextPossSubsumTestItem->getPossibleSubsumerSet(false)->remove(possSubsumedItem);
												COptimizedKPSetRolePossibleSubsumptionMap* possSubsumerMap = possSubsumedItem->getPossibleSubsumptionMap(false);
												if (possSubsumerMap && possSubsumerMap->hasRemainingPossibleSubsumptions()) {
													possSubsumCalcData = possSubsumerMap->value(CRoleTagComparer(candidateRole));
												}
											}
										}
									}

									if (possSubsumCalcData) {
										//if ((possSubsumCalcData->isUpdateRequired() || possSubsumCalcData->isSubsumptionUnknown())) {
											currentItemSet->insert(nextPossSubsumTestItem);
											optKPSetClassificationItem->incRunningPossibleSubsumptionTestsCount();

											if (calculateSubsumption(optKPSetClassificationItem,possSubsumedItem,nextPossSubsumTestItem,possSubsumCalcData)) {
												workTestCreated = true;
											} else {
												interpreteSubsumptionResult(optKPSetClassificationItem,possSubsumedItem->getTestingRole(),nextPossSubsumTestItem->getTestingRole(),possSubsumCalcData->isSubsumptionConfirmed());
											}
										//}
									} else {
										QSet<COptimizedKPSetRoleTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionTestingSet();
										remTestingSet->remove(nextPossSubsumTestItem);
									}

								}
							}
						}

						if (!workTestCreated && currentItemSet->isEmpty()) {
							optKPSetClassificationItem->setPossibleSubsumptionTestingPhaseFinished(true);
						}
					}


					if (!workTestCreated) {
						if (optKPSetClassificationItem->hasPossibleSubsumptionTestingPhaseFinished() || ontPropItem->isHierarchyConstructionFailed()) {
							finishOntologyClassification(ontPropItem);
							processingOntItemList.removeFirst();
						} else {
							processingOntItemList.removeFirst();
							processingOntItemList.append(ontPropItem);
							loopontPropItem = ontPropItem;
						}
					}
				}
				return workTestCreated;
			}


			COptimizedKPSetRolePossibleSubsumptionData* COptimizedKPSetRoleSubsumptionClassifierThread::getNextPossibleSubsumptionCalculation(COptimizedKPSetRoleOntologyClassificationItem *ontPropItem, COptimizedKPSetRoleTestingItem* classItem) {
				COptimizedKPSetRolePossibleSubsumptionData* possSubsumer = nullptr;
				// I don't know which is the best, simply return the first one
				// sort by subsumer count?
				COptimizedKPSetRolePossibleSubsumptionMap* possSubsumerMap = classItem->getPossibleSubsumptionMap(false);
				if (possSubsumerMap) {
					if (mConfPossSubsumCalcOrderRoleSorted) {
						for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator it = possSubsumerMap->constBegin(), itEnd = possSubsumerMap->constEnd(); !possSubsumer && it != itEnd; ++it) {
							COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = it.value();
							if (possSubsumData->isSubsumptionUnknown()) {
								possSubsumer = possSubsumData;
							} else if (possSubsumData->isUpdateRequired()) {
								prunePossibleSubsumptions(ontPropItem,classItem,possSubsumData);							
							}
						}
					}
				}
				return possSubsumer;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::calculateSatisfiable(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* nextSatTestItem) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CPropertyClassificationComputationItem *workItem = 0;


				//if (CIRIName::getRecentIRIName(nextSatTestItem->getTestingRole()->getPropertyNameLinker()) == "http://www.owllink.org/testsuite/particle-D#Neutrino" || CIRIName::getRecentIRIName(nextSatTestItem->getTestingRole()->getPropertyNameLinker()) == "http://www.co-ode.org/roberts/family-tree.owl#isBrotherInLawOf") {
				//	bool bug = true;
				//}


				CConcept* existConcept = nextSatTestItem->getTemporaryExistConcept();
				CConcept* allPropConcept = optKPSetClassificationItem->getTemporaryAllPropagationConcept();


				nextSatTestItem->setSatisfiableTestOrdered(true);

				bool satisfiableFlag = false;

				if (nextSatTestItem->isResultSatisfiableDerivated()) {
					satisfiableFlag = true;
					nextSatTestItem->setSatisfiableTested(true);
					nextSatTestItem->setSatisfiableTestedResult(true);
					return false;
				}
				if (nextSatTestItem->isResultUnsatisfiableDerivated()) {
					satisfiableFlag = false;
					nextSatTestItem->setSatisfiableTested(true);
					nextSatTestItem->setSatisfiableTestedResult(false);
					return false;
				}


				CIndividual* tmpIndiPropagation = optKPSetClassificationItem->getTemporaryPropagationIndividual();
				CIndividual* tmpIndiMarker = optKPSetClassificationItem->getTemporaryMarkerIndividual();
				CRole* role = nextSatTestItem->getTestingRole();



				CSatisfiableCalculationJobGenerator satCalcJobGen(optKPSetClassificationItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(existConcept,false,tmpIndiPropagation,satCalcJob);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(allPropConcept,false,tmpIndiPropagation,satCalcJob);
				if (optKPSetClassificationItem->isDataRolesClassification()) {
					CConcept* topDataConcept = optKPSetClassificationItem->getOntology()->getTBox()->getTopDataRangeConcept();
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topDataConcept,false,tmpIndiMarker,satCalcJob);
				} else {
					CConcept* topConcept = optKPSetClassificationItem->getOntology()->getTBox()->getTopConcept();
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topConcept,false,tmpIndiMarker,satCalcJob);
				}

				QHash<CSatisfiableCalculationJob*,CPropertyClassificationComputationItem*> *workHash = optKPSetClassificationItem->getComputationItemHash();

				CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
				if (ontClassStat) {
					ontClassStat->incCalculatedTestedSatisfiableCount(1);
				}

				workItem = new CPropertyClassificationComputationItem(satCalcJob,role);
				workHash->insertMulti(satCalcJob,workItem);
				workOntItemHash.insert(workItem,optKPSetClassificationItem);

				satCalcJob->setSatisfiableClassificationRoleMarkedMessageAdapter(new CSatisfiableTaskClassificationRoleMarkedMessageAdapter(role,tmpIndiPropagation,tmpIndiMarker,optKPSetClassificationItem->getOntology(),this));

				optKPSetClassificationItem->incCurrentCalculatingCount();
				if (optKPSetClassificationItem->getIndividualDependenceTrackingCollector()) {
					satCalcJob->setSatisfiableTaskIndividualDependenceTrackingAdapter(new CSatisfiableTaskIndividualDependenceTrackingAdapter(optKPSetClassificationItem->getIndividualDependenceTrackingCollector()));
				}

				processCalculationJob(satCalcJob,optKPSetClassificationItem,workItem);


				++mCreatedCalculationTaskCount;
				return true;
			}





			bool COptimizedKPSetRoleSubsumptionClassifierThread::calculateSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* subsumedItem, COptimizedKPSetRoleTestingItem* possSubsumerItem, COptimizedKPSetRolePossibleSubsumptionData* possSubsumData) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CPropertyClassificationComputationItem *workItem = 0;
				++mOrderedSubsumptionCalculationCount;

				CRole* subsumedRole = subsumedItem->getTestingRole();
				CRole* subsumerRole = possSubsumerItem->getTestingRole();

				bool isSubsumptionFlag = false;
				bool precheckedSubsum = false;


				CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
				if (possSubsumData && possSubsumData->isSubsumptionKnown()) {
					return false;
				} else if (possSubsumData && subsumedItem->hasSubsumerRoleItem(possSubsumerItem)) {					
					possSubsumData->setSubsumptionConfirmed(true);
					return false;
				}

				CIndividual* tmpIndiPropagation = optKPSetClassificationItem->getTemporaryPropagationIndividual();
				CIndividual* tmpIndiMarker = optKPSetClassificationItem->getTemporaryMarkerIndividual();
				CConcept* existConcept = subsumedItem->getTemporaryExistConcept();
				CConcept* propagationConcept = possSubsumerItem->getTemporaryPropagationConcept();

				CSatisfiableCalculationJobGenerator satCalcJobGen(optKPSetClassificationItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(existConcept,false,tmpIndiPropagation,satCalcJob);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(propagationConcept,false,tmpIndiPropagation,satCalcJob);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(possSubsumerItem->getTemporaryMarkerConcept(),true,tmpIndiMarker,satCalcJob);
				if (optKPSetClassificationItem->isDataRolesClassification()) {
					CConcept* topDataConcept = optKPSetClassificationItem->getOntology()->getTBox()->getTopDataRangeConcept();
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topDataConcept,false,tmpIndiMarker,satCalcJob);
				} else {
					CConcept* topConcept = optKPSetClassificationItem->getOntology()->getTBox()->getTopConcept();
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topConcept,false,tmpIndiMarker,satCalcJob);
				}


				QHash<CSatisfiableCalculationJob *, CPropertyClassificationComputationItem *> *workHash = optKPSetClassificationItem->getComputationItemHash();

				if (ontClassStat) {
					ontClassStat->incCalculatedTestedSubsumptionCount(1);
					ontClassStat->incToldSubsumptionCount(1);
				}

				workItem = new CPropertyClassificationComputationItem(satCalcJob,subsumerRole,subsumedRole);
				workHash->insertMulti(satCalcJob,workItem);
				workOntItemHash.insert(workItem,optKPSetClassificationItem);


				if (optKPSetClassificationItem->getIndividualDependenceTrackingCollector()) {
					satCalcJob->setSatisfiableTaskIndividualDependenceTrackingAdapter(new CSatisfiableTaskIndividualDependenceTrackingAdapter(optKPSetClassificationItem->getIndividualDependenceTrackingCollector()));
				}

				optKPSetClassificationItem->incCurrentCalculatingCount();
				processCalculationJob(satCalcJob,optKPSetClassificationItem,workItem);
				optKPSetClassificationItem->incCalculatedPossibleSubsumerCount();
				++mCreatedCalculationTaskCount;
				return true;
			}







			void COptimizedKPSetRoleSubsumptionClassifierThread::incRemainingPossibleSubsumptionTestingCount(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRolePossibleSubsumptionMap* posSubsumMap) {
				optKPSetClassificationItem->incPossibleSubsumerCount();
				optKPSetClassificationItem->incRemainingPossibleSubsumptionTestsCount();
				posSubsumMap->incRemainingPossibleSubsumptionCount();
				++totalToSubsumTestCount;
			}


			void COptimizedKPSetRoleSubsumptionClassifierThread::decRemainingPossibleSubsumptionTestingCount(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRolePossibleSubsumptionMap* posSubsumMap, bool subsumptionConfirmed) {
				if (subsumptionConfirmed) {
					optKPSetClassificationItem->incTruePossibleSubsumerCount();
				} else {
					optKPSetClassificationItem->incFalsePossibleSubsumerCount();
				}
				optKPSetClassificationItem->decRemainingPossibleSubsumptionTestsCount();
				posSubsumMap->decRemainingPossibleSubsumptionCount();
				++subsumTestedCount;
			}




			CSubsumptionClassifierThread *COptimizedKPSetRoleSubsumptionClassifierThread::processCalculationJob(CSatisfiableCalculationJob* job, COntologyPropertyRoleClassificationItem *ontPropItem, CPropertyClassificationComputationItem* workItem) {
				CClassificationCalculationStatisticsCollection* statColl =  nullptr;
				if (ontPropItem->isCollectProcessStatisticsActivated()) {
					statColl = ontPropItem->getCalculationStatisticsCollection();
				}
				job->setCalclulationStatisticsCollector(statColl);
				CTestCalculatedCallbackEvent *testResultCallback = new CTestCalculatedCallbackEvent(this,job,workItem,statColl);
				job->setCalculationConfiguration(ontPropItem->getCalculationConfiguration());
				CSubsumptionClassifierThread::processCalculationJob(job,ontPropItem,testResultCallback,false);
				return this;
			}



			bool COptimizedKPSetRoleSubsumptionClassifierThread::processToldClassificationMessage(COntologyClassificationItem *ontPropItem, CClassificationMessageData* messageDataLinker, CMemoryPool* memoryPools) {
				COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontPropItem;
				CClassificationMessageData* messageDataLinkerIt = messageDataLinker;
				while (messageDataLinkerIt) {
					CClassificationMessageData* messageData = messageDataLinkerIt;
					if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLPROPERTYSUBSUMPTION) {
						++mStatProcesedSubsumMessCount;
						CClassificationPropertySubsumptionMessageData* subsumMessageData = (CClassificationPropertySubsumptionMessageData*)messageData;
						CRole* subsumedRole = subsumMessageData->getSubsumedRole();
						CPROPERTYSUBSUMPTIONMESSAGELIST<CRole*>* subsumerList = subsumMessageData->getPropertySubsumerList();

						COptimizedKPSetRoleTestingItem* subsumedItem = nullptr;
						subsumedItem = optKPSetClassificationItem->getRoleSatisfiableTestItem(subsumedRole);

						CRole* topRole = optKPSetClassificationItem->getTopRoleSatisfiableTestItem()->getTestingRole();

						COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = subsumedItem->getPossibleSubsumptionMap(false);
						if (subsumerList) {
							for (CPROPERTYSUBSUMPTIONMESSAGELIST<CRole*>::const_iterator it = subsumerList->constBegin(), itEnd = subsumerList->constEnd(); it != itEnd; ++it) {
								CRole* subsumerRole(*it);
								if (subsumerRole != topRole) {
									COptimizedKPSetRoleTestingItem* subsumerItem = optKPSetClassificationItem->getRoleSatisfiableTestItem(subsumerRole);
									if (subsumedItem != subsumerItem) {
										subsumedItem->addSubsumerRoleItem(subsumerItem);
										if (possSubsumMap) {
											COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = possSubsumMap->value(subsumerRole,nullptr);
											if (possSubsumData) {
												if (!possSubsumData->isSubsumptionConfirmed()) {
													possSubsumData->setSubsumptionConfirmed(true);

													if (possSubsumData->isUpdateRequired()) {
														prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possSubsumData);
													}
												}
											}
										}
										propagateDownSubsumption(optKPSetClassificationItem,subsumedItem,subsumerItem);
									}
								}
							}
						}
						subsumedItem->setResultSatisfiableDerivated(true);

					} else if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLCLASSINITIALIZEPOSSIBLESUBSUM) {
						++mStatProcesedPossSubsumInitMessCount;
						CClassificationInitializePossiblePropertySubsumptionMessageData* possSubsumMessageData = (CClassificationInitializePossiblePropertySubsumptionMessageData*)messageData;
						CRole* subsumedRole = possSubsumMessageData->getSubsumedRole();
						CPROPERTYPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossiblePropertySubsumptionData*>* possSubsumerList = possSubsumMessageData->getPropertyPossibleSubsumerList();


						COptimizedKPSetRoleTestingItem* subsumedItem = nullptr;
						subsumedItem = optKPSetClassificationItem->getRoleSatisfiableTestItem(subsumedRole);
						CRole* topRole = optKPSetClassificationItem->getTopRoleSatisfiableTestItem()->getTestingRole();

						COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = subsumedItem->getPossibleSubsumptionMap(false);
						if (!possSubsumerList || possSubsumerList->empty()) {
							if (possSubsumMap) {
								for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
									COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = it.value();
									if (possSubsumData->isSubsumptionUnknown()) {
										possSubsumData->setSubsumptionInvalid(true);
										if (possSubsumData->isUpdateRequired()) {
											prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possSubsumData);
										}
									}
								}
							}
						} else {
							if (!subsumedItem->isPossibleSubsumptionMapInitialized() || possSubsumMap) {
								possSubsumMap = subsumedItem->getPossibleSubsumptionMap(true);
								if (possSubsumMap->isEmpty()) {
									// initialize the possible subsumption map
									if (possSubsumerList) {
										for (CPROPERTYPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossiblePropertySubsumptionData*>::const_iterator it = possSubsumerList->constBegin(), itEnd = possSubsumerList->constEnd(); it != itEnd; ++it) {
											CClassificationInitializePossiblePropertySubsumptionData* possSubsumDataIt = *it;
											if (possSubsumDataIt->isPossibleSubsumerValid()) {
												CRole* possSubsumRole = possSubsumDataIt->getPossibleSubsumerRole();
												COptimizedKPSetRoleTestingItem* possSubsumItem = optKPSetClassificationItem->getRoleSatisfiableTestItem(possSubsumRole);
												if (possSubsumRole != topRole && subsumedItem != possSubsumItem && !subsumedItem->hasSubsumerRoleItem(possSubsumItem)) {
													COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = new COptimizedKPSetRolePossibleSubsumptionData(possSubsumItem);
													possSubsumMap->insert(CRoleTagComparer(possSubsumRole),possSubsumData);
													incRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,possSubsumMap);
												}
											}
										}
									}


									// prune ancestor items
									QSet<COptimizedKPSetRoleTestingItem*>* upPropSet = subsumedItem->getUpPropagationItemSet();
									for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = upPropSet->constBegin(), itEnd = upPropSet->constEnd(); it != itEnd; ++it) {
										COptimizedKPSetRoleTestingItem* upPropItem = *it;
										COptimizedKPSetRolePossibleSubsumptionMap* upPropItemPossSubsumMap = upPropItem->getPossibleSubsumptionMap(false);
										if (upPropItemPossSubsumMap) {
											COptimizedKPSetRolePossibleSubsumptionMap::const_iterator itPoss = possSubsumMap->constBegin(), itPossEnd = possSubsumMap->constEnd();
											COptimizedKPSetRolePossibleSubsumptionMap::const_iterator itUpPoss = upPropItemPossSubsumMap->constBegin(), itUpPossEnd = upPropItemPossSubsumMap->constEnd();
											while (itPoss != itPossEnd && itUpPoss != itUpPossEnd) {
												CRole* possRole = itPoss.key().getRole();
												CRole* possUpRole = itUpPoss.key().getRole();
												if (possRole->getRoleTag() == possUpRole->getRoleTag()) {
													++itPoss;
													++itUpPoss;
												} else if (possRole->getRoleTag() < possUpRole->getRoleTag()) {
													++itPoss;
												} else if (possRole->getRoleTag() > possUpRole->getRoleTag()) {
													COptimizedKPSetRolePossibleSubsumptionData* upPossData = itUpPoss.value();
													if (!subsumedItem->hasSubsumerRoleItem(upPossData->getTestingItem()) && subsumedItem != upPossData->getTestingItem()) {
														if (!upPossData->isSubsumptionInvalided()) {
															upPossData->setSubsumptionInvalid(true);
															if (upPossData->isUpdateRequired()) {
																prunePossibleSubsumptions(optKPSetClassificationItem,upPropItem,upPossData);
															}
														}
													}
													++itUpPoss;
												}
											}
											while (itUpPoss != itUpPossEnd) {
												COptimizedKPSetRolePossibleSubsumptionData* upPossData = itUpPoss.value();
												if (!subsumedItem->hasSubsumerRoleItem(upPossData->getTestingItem()) && subsumedItem != upPossData->getTestingItem()) {
													if (!upPossData->isSubsumptionInvalided()) {
														upPossData->setSubsumptionInvalid(true);
														if (upPossData->isUpdateRequired()) {
															prunePossibleSubsumptions(optKPSetClassificationItem,upPropItem,upPossData);
														}
													}
												}
												++itUpPoss;
											}
										}
									}

									// prune descendant items
									QSet<COptimizedKPSetRoleTestingItem*>* downPropSet = subsumedItem->getDownPropagationItemSet();
									for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = downPropSet->constBegin(), itEnd = downPropSet->constEnd(); it != itEnd; ++it) {
										COptimizedKPSetRoleTestingItem* downPropItem = *it;
										COptimizedKPSetRolePossibleSubsumptionMap* downPropItemPossSubsumMap = downPropItem->getPossibleSubsumptionMap(false);
										if (downPropItemPossSubsumMap) {
											COptimizedKPSetRolePossibleSubsumptionMap::const_iterator itPoss = possSubsumMap->constBegin(), itPossEnd = possSubsumMap->constEnd();
											COptimizedKPSetRolePossibleSubsumptionMap::const_iterator itDownPoss = downPropItemPossSubsumMap->constBegin(), itDownPossEnd = downPropItemPossSubsumMap->constEnd();
											while (itPoss != itPossEnd && itDownPoss != itDownPossEnd) {
												CRole* possRole = itPoss.key().getRole();
												CRole* possDownRole = itDownPoss.key().getRole();
												if (possRole->getRoleTag() == possDownRole->getRoleTag()) {
													++itPoss;
													++itDownPoss;
												} else if (possDownRole->getRoleTag() < possRole->getRoleTag()) {
													++itDownPoss;
												} else if (possDownRole->getRoleTag() > possRole->getRoleTag()) {
													COptimizedKPSetRolePossibleSubsumptionData* possData = itPoss.value();
													if (!downPropItem->hasSubsumerRoleItem(possData->getTestingItem()) && downPropItem != possData->getTestingItem()) {
														if (!possData->isSubsumptionInvalided()) {
															possData->setSubsumptionInvalid(true);
															if (possData->isUpdateRequired()) {
																prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
															}
														}
													}
													++itPoss;
												}
											}
											while (itPoss != itPossEnd) {
												COptimizedKPSetRolePossibleSubsumptionData* possData = itDownPoss.value();
												if (!downPropItem->hasSubsumerRoleItem(possData->getTestingItem()) && downPropItem != possData->getTestingItem()) {
													if (!possData->isSubsumptionInvalided()) {
														possData->setSubsumptionInvalid(true);
														if (possData->isUpdateRequired()) {
															prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
														}
													}
												}
												++itPoss;
											}
										} else if (downPropItem->isPossibleSubsumptionMapInitialized()) {
											for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator itPoss = possSubsumMap->constBegin(), itPossEnd = possSubsumMap->constEnd(); itPoss != itPossEnd; ++itPoss) {
												COptimizedKPSetRolePossibleSubsumptionData* possData = itPoss.value();
												if (!downPropItem->hasSubsumerRoleItem(possData->getTestingItem()) && downPropItem != possData->getTestingItem()) {
													if (!possData->isSubsumptionInvalided()) {
														possData->setSubsumptionInvalid(true);
														if (possData->isUpdateRequired()) {
															prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
														}
													}
												}
											}
										}
									}

								} else {
									// prune the possible subsumption map
									COptimizedKPSetRolePossibleSubsumptionMap::const_iterator itPoss = possSubsumMap->constBegin(), itPossEnd = possSubsumMap->constEnd();
									if (possSubsumerList) {
										CPROPERTYPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossiblePropertySubsumptionData*>::const_iterator itNew = possSubsumerList->constBegin(), itNewEnd = possSubsumerList->constEnd();
										while (itPoss != itPossEnd && itNew != itNewEnd) {
											COptimizedKPSetRolePossibleSubsumptionData* possData = itPoss.value();
											CRole* possRole = itPoss.key().getRole();
											CClassificationInitializePossiblePropertySubsumptionData* possNewSubsumDataIt = *itNew;
											if (possNewSubsumDataIt->isPossibleSubsumerValid()) {
												CRole* newRole = possNewSubsumDataIt->getPossibleSubsumerRole();
												if (possRole->getRoleTag() == newRole->getRoleTag()) {
													++itPoss;
													++itNew;
												} else if (possRole->getRoleTag() < newRole->getRoleTag()) {
													if (!possData->isSubsumptionInvalided()) {
														possData->setSubsumptionInvalid(true);
														if (possData->isUpdateRequired()) {
															prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
														}
													}
													++itPoss;
												} else if (possRole->getRoleTag() > newRole->getRoleTag()) {
													++itNew;
												}
											} else {
												++itNew;
											}
										}
									}
									while (itPoss != itPossEnd) {
										COptimizedKPSetRolePossibleSubsumptionData* possData = itPoss.value();
										CRole* possRole = itPoss.key().getRole();
										if (!possData->isSubsumptionInvalided()) {
											possData->setSubsumptionInvalid(true);
											if (possData->isUpdateRequired()) {
												prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
											}
										}
										++itPoss;
									}
								}
							}
						}
						subsumedItem->setPossibleSubsumptionMapInitialized(true);


					} else if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLPROPERTYUPDATEPOSSIBLESUBSUM) {
						++mStatProcesedPossSubsumUpdateMessCount;
						CClassificationUpdatePossiblePropertySubsumptionMessageData* possSubsumMessageData = (CClassificationUpdatePossiblePropertySubsumptionMessageData*)messageData;
						CRole* subsumedRole = possSubsumMessageData->getSubsumedRole();

						COptimizedKPSetRoleTestingItem* subsumedItem = nullptr;
						subsumedItem = optKPSetClassificationItem->getRoleSatisfiableTestItem(subsumedRole);

						COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = subsumedItem->getPossibleSubsumptionMap(false);
						if (possSubsumMap && !possSubsumMap->empty()) {
							possSubsumMap = subsumedItem->getPossibleSubsumptionMap(true);
							for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = it.value();
								if (possSubsumData->isUpdateRequired()) {
									prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possSubsumData);
								}
							}
						}
					}
					messageDataLinkerIt = messageDataLinkerIt->getNext();
				}
				return true;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::interpreteToldSubsumptionResult(COntologyPropertyRoleClassificationItem *ontPropItem, const QList< QPair<CRole*,CRole*> > &subSumRelList, bool isSubSum) {
				CRole *lastRole = nullptr;
				COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontPropItem;
				COptimizedKPSetRoleTestingItem* subsumerItem = nullptr;

				for (QList< QPair<CRole*,CRole*> >::const_iterator it = subSumRelList.constBegin(), itEnd = subSumRelList.constEnd(); it != itEnd; ++it) {
					CRole *subsumerRole = it->first;
					if (lastRole != subsumerRole) {
						lastRole = subsumerRole;
						subsumerItem = optKPSetClassificationItem->getRoleSatisfiableTestItem(subsumerRole);
					}
					CRole *subsumedRole = it->second;
					if (isSubSum) {
						COptimizedKPSetRoleTestingItem* subsumedItem = optKPSetClassificationItem->getRoleSatisfiableTestItem(subsumedRole);
						subsumerItem->addSubsumerRoleItem(subsumedItem);
					}
				}
				return true;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::interpreteSubsumptionResult(COntologyPropertyRoleClassificationItem *ontPropItem, CRole *subsumedRole, CRole *subsumerRole, bool isSubsumption) {
				COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontPropItem;

				++mInterpretedSubsumptionCalculationCount;

				optKPSetClassificationItem->decRunningPossibleSubsumptionTestsCount();

				COptimizedKPSetRoleTestingItem* subsumedItem = optKPSetClassificationItem->getRoleSatisfiableTestItem(subsumedRole);
				COptimizedKPSetRoleTestingItem* subsumerItem = optKPSetClassificationItem->getRoleSatisfiableTestItem(subsumerRole);


				//QString iriPropertyNameString1 = CIRIName::getRecentIRIName(subsumedRole->getPropertyNameLinker());
				//QString iriPropertyNameString2 = CIRIName::getRecentIRIName(subsumerRole->getPropertyNameLinker());
				//if (iriPropertyNameString1 == "http://www.bootstrep.eu/ontology/GRO#BindingOfProteinToProteinBindingSiteOfDNA" && iriPropertyNameString2 == "http://www.bootstrep.eu/ontology/GRO#DNAProteinInteraction" && !isSubsumption) {
				//	bool bug = true;
				//	calculateSubsumption(optKPSetPropertyificationItem,subsumedItem,subsumerItem,nullptr);
				//}


				COptimizedKPSetRoleTestingItem* topItem = optKPSetClassificationItem->getTopRoleSatisfiableTestItem();

				COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = subsumedItem->getPossibleSubsumptionMap(false);
				COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = nullptr;
				if (possSubsumMap) {
					possSubsumData = possSubsumMap->value(subsumerRole);
				}

				if (isSubsumption) {
					optKPSetClassificationItem->incCalculatedTruePossibleSubsumerCount();
					if (possSubsumData) {
						possSubsumData->setSubsumptionConfirmed(true);
					}
					subsumedItem->addSubsumerRoleItem(subsumerItem);
					subsumedItem->addUpPropagationItem(subsumerItem);
					subsumerItem->addDownPropagationItem(subsumedItem);
					propagateDownSubsumption(optKPSetClassificationItem,subsumedItem,subsumerItem);
				} else {

					optKPSetClassificationItem->incCalculatedFalsePossibleSubsumerCount();
					if (possSubsumData) {
						possSubsumData->setSubsumptionInvalid(true);
					}
				}

				if (possSubsumData && possSubsumData->isUpdateRequired()) {
					prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possSubsumData);
				}


				QList<COptimizedKPSetRoleTestingItem*>* nextItemList = optKPSetClassificationItem->getNextPossibleSubsumptionTestingItemList();
				QSet<COptimizedKPSetRoleTestingItem*>* currentItemSet = optKPSetClassificationItem->getCurrentPossibleSubsumptionTestingItemSet();
				if (mConfPossSubsumCalcOrderTopDown) {
					if (currentItemSet->contains(subsumedItem)) {
						currentItemSet->remove(subsumedItem);
						if (possSubsumMap) {
							if (possSubsumMap->hasRemainingPossibleSubsumptions()) {
								nextItemList->prepend(subsumedItem);
							} else {
								QSet<COptimizedKPSetRoleTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionTestingSet();
								remTestingSet->remove(subsumedItem);
							}
						}
					}
				} 
				if (mConfPossSubsumCalcOrderBottomUp) {
					if (currentItemSet->contains(subsumerItem)) {
						currentItemSet->remove(subsumerItem);
						if (subsumerItem->hasRemainingPossibleSubsumedItems()) {
							QSet<COptimizedKPSetRoleTestingItem*>* remPossSubsumedItemSet = subsumerItem->getPossibleSubsumerSet(false);
							QList<COptimizedKPSetRoleTestingItem*>* remPossSubsumedItemList = subsumerItem->getPossibleSubsumerList();
							nextItemList->prepend(subsumerItem);

							QSet<COptimizedKPSetRoleTestingItem*>* upPropSet = subsumedItem->getUpPropagationItemSet();
							for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator itUp = upPropSet->constBegin(), itUpEnd = upPropSet->constEnd(); itUp != itUpEnd; ++itUp) {
								COptimizedKPSetRoleTestingItem* upItem = *itUp;
								if (remPossSubsumedItemSet->contains(upItem)) {
									remPossSubsumedItemList->prepend(upItem);
								}
							}

						} else {
							QSet<COptimizedKPSetRoleTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionTestingSet();
							remTestingSet->remove(subsumerItem);
						}
					}
				}
				return true;
			}



			bool COptimizedKPSetRoleSubsumptionClassifierThread::prunePossibleSubsumptions(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, COptimizedKPSetRolePossibleSubsumptionData* possSubsumData) {

				if (possSubsumData->isUpdateRequired()) {
					COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
					possSubsumData->setSubsumptionUpdated(true);
					decRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,possSubsumMap,true);
					if (possSubsumData->isSubsumptionConfirmed()) {
						// is subsuming
						COptimizedKPSetRoleTestingItem* subsumerItem = possSubsumData->getTestingItem();
						CRole* subsumerRole = subsumerItem->getTestingRole();
						// establish the subsumption for all successor nodes
						QSet<COptimizedKPSetRoleTestingItem*>* downPropSet = item->getDownPropagationItemSet();
						for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = downPropSet->constBegin(), itEnd = downPropSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetRoleTestingItem* downPropItem = *it;
							pruneDownSubsumption(optKPSetClassificationItem,downPropItem,subsumerRole);
						}
						return true;
					} else {
						// is not subsuming
						COptimizedKPSetRoleTestingItem* notSubsumerItem = possSubsumData->getTestingItem();
						CRole* notSubsumerRole = notSubsumerItem->getTestingRole();
						// establish the subsumption for all successor nodes
						QSet<COptimizedKPSetRoleTestingItem*>* upPropSet = item->getUpPropagationItemSet();
						for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = upPropSet->constBegin(), itEnd = upPropSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetRoleTestingItem* upPropItem = *it;
							pruneUpNotSubsumption(optKPSetClassificationItem,upPropItem,notSubsumerRole);
						}

						return true;

					}
				}
				return false;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::propagateDownSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, COptimizedKPSetRoleTestingItem* subsumerItem) {
				bool propagated = false;
				QSet<COptimizedKPSetRoleTestingItem*>* downPropSet = item->getDownPropagationItemSet();
				for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = downPropSet->constBegin(), itEnd = downPropSet->constEnd(); it != itEnd; ++it) {
					COptimizedKPSetRoleTestingItem* downPropItem = *it;
					if (!downPropItem->hasSubsumerRoleItem(subsumerItem) && downPropItem != subsumerItem) {
						downPropItem->addSubsumerRoleItem(subsumerItem);
						propagateDownSubsumption(optKPSetClassificationItem,downPropItem,subsumerItem);
						propagated = true;
					}
				}
				return propagated;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::pruneDownSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, CRole* subsumerRole) {		
				COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
				if (possSubsumMap) {
					COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = possSubsumMap->value(subsumerRole);
					if (possSubsumData && !possSubsumData->isSubsumptionConfirmed()) {
						possSubsumData->setSubsumptionConfirmed(true);
						possSubsumData->setSubsumptionUpdated(true);
						decRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,possSubsumMap,true);

						CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
						if (ontClassStat) {
							ontClassStat->incDerivatedTestedSubsumptionCount(1);
						}

						QSet<COptimizedKPSetRoleTestingItem*>* downPropSet = item->getDownPropagationItemSet();
						for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = downPropSet->constBegin(), itEnd = downPropSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetRoleTestingItem* downPropItem = *it;
							pruneDownSubsumption(optKPSetClassificationItem,downPropItem,subsumerRole);
						}
						return true;
					}
				}
				return false;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::pruneUpNotSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, CRole* notSubsumerRole) {		
				COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
				if (possSubsumMap) {
					COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = possSubsumMap->value(notSubsumerRole);
					if (possSubsumData && !possSubsumData->isSubsumptionInvalided()) {
						possSubsumData->setSubsumptionInvalid(true);
						possSubsumData->setSubsumptionUpdated(true);
						decRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,possSubsumMap,false);

						CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
						if (ontClassStat) {
							ontClassStat->incPrunedTestedSubsumptionCount(1);
						}

						QSet<COptimizedKPSetRoleTestingItem*>* upPropSet = item->getUpPropagationItemSet();
						for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = upPropSet->constBegin(), itEnd = upPropSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetRoleTestingItem* upPropItem = *it;
							pruneUpNotSubsumption(optKPSetClassificationItem,upPropItem,notSubsumerRole);
						}
						return true;
					}
				}
				return false;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::interpreteSatisfiableResult(COntologyPropertyRoleClassificationItem *ontPropItem, CRole *satisfiableRole, bool isSatis) {
				COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontPropItem;

				CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
				if (ontClassStat) {
					ontClassStat->incToldSatisfiableCount(1);
				}

				++satTestedCount;

				optKPSetClassificationItem->decRunningSatisfiableTestsCount();

				COptimizedKPSetRoleTestingItem* satTestedItem = optKPSetClassificationItem->getRoleSatisfiableTestItem(satisfiableRole);
				//if (CIRIName::getRecentIRIName(satisfiableRole->getPropertyNameLinker()) == "http://www.gdst.uqam.ca/Documents/Ontologies/HIT/Equipment_SH_Ontology.owl#Folding_chair" && !isSatis) {
				//	bool bug = true;
				//	calculateSatisfiable(optKPSetClassificationItem,satTestedItem);
				//	return false;
				//}


				satTestedItem->setSatisfiableTested(true);
				satTestedItem->setSatisfiableTestedResult(isSatis);


				if (isSatis) {
					optKPSetClassificationItem->addSatisfiableRoleItem(satTestedItem);

				} else {
					CRolePropertiesHierarchy* hierarchy = optKPSetClassificationItem->getHierarchy();
					hierarchy->getBottomHierarchyNode()->addEquivalentElement(satisfiableRole);
					hierarchy->updateNodeEquivalentElements(hierarchy->getBottomHierarchyNode());
				}

				QList<COptimizedKPSetRoleTestingItem*>* nextItemList = optKPSetClassificationItem->getNextSatisfiableTestingItemList();
				QSet<COptimizedKPSetRoleTestingItem*>* nextCandItemSet = optKPSetClassificationItem->getNextCandidateSatisfiableTestingItemSet();
				QSet<COptimizedKPSetRoleTestingItem*>* remainingCandItemSet = optKPSetClassificationItem->getRemainingCandidateSatisfiableTestingItemSet();


				QList<COptimizedKPSetRoleTestingItem*>* subSumItemList = satTestedItem->getSubsumerRoleItemList();
				QList<COptimizedKPSetRoleTestingItem*>* succItemList = satTestedItem->getSuccessorItemList();
				for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator it = succItemList->constBegin(), itEnd = succItemList->constEnd(); it != itEnd; ++it) {
					COptimizedKPSetRoleTestingItem* succItem = *it;
					if (!isSatis) {
						succItem->setResultUnsatisfiableDerivated(true);
					} else {
						for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator itSubsum = subSumItemList->constBegin(), itSubsumEnd = subSumItemList->constEnd(); itSubsum != itSubsumEnd; ++itSubsum) {
							COptimizedKPSetRoleTestingItem* subusmerItem = *itSubsum;
							if (succItem != subusmerItem) {
								succItem->addSubsumerRoleItem(subusmerItem);
							}
						}
					}
					succItem->decUnprocessedPredecessorItems();
					if (succItem->hasOnlyProcessedPredecessorItems()) {
						nextItemList->append(succItem);
					} else {
						nextCandItemSet->insert(succItem);
					}
				}

				return false;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::interpreteTestResults(CTestCalculatedCallbackEvent *testResult) {
				CSatisfiableCalculationJob *satCalcJob = testResult->getSatisfiableCalculationJob();
				bool testSat = testResult->getTestResultSatisfiable();
				CPropertyClassificationComputationItem *workItem = (CPropertyClassificationComputationItem*)testResult->getClassificationWorkItem();
				++mRecievedCallbackCount;


				COntologyPropertyRoleClassificationItem *ontPropItem = workOntItemHash.value(workItem);
				if (ontPropItem) {

					QHash<CSatisfiableCalculationJob *, CPropertyClassificationComputationItem *> *workHash = ontPropItem->getComputationItemHash();

					if (workItem) {
						ontPropItem->decCurrentCalculatingCount();

						if (testResult->hasCalculationError()) {
							LOG(ERROR,getLogDomain(),logTr("Error in computation, role classification for ontology '%1' failed.").arg(ontPropItem->getOntology()->getTerminologyName()),getLogObject());
							ontPropItem->setHierarchyConstructionFailed();

						} else {

							if (workItem->isRoleSatisfiableTest()) {
								interpreteSatisfiableResult(ontPropItem,workItem->getSatisfiableTestedRole(),testSat);
							} else if (workItem->isRoleSubsumptionTest()) {
								interpreteSubsumptionResult(ontPropItem,workItem->getSubsumedTestedRole(),workItem->getSubsumerTestedRole(),!testSat);
							}
						}
						QHash<CSatisfiableCalculationJob *, CPropertyClassificationComputationItem *>::iterator itWorkItem = workHash->find(satCalcJob);
						while (itWorkItem != workHash->end()) {						
							if (itWorkItem.value() == workItem) {
								workHash->erase(itWorkItem);
								break;
							}
							++itWorkItem;
						}
						delete workItem;
						delete satCalcJob;
					}
					workOntItemHash.remove(workItem);


					CClassificationCalculationStatisticsCollection* statisticCollection = testResult->getUsedStatisticsCollection();
					if (statisticCollection) {
						ontPropItem->reuseCalculationStatisticsCollection(statisticCollection);
					}
				}

				return true;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::finishOntologyClassification(COntologyPropertyRoleClassificationItem *ontPropItem) {

				if (!ontPropItem->isHierarchyConstructed() && !ontPropItem->isHierarchyConstructionFailed()) {

					CConcreteOntology *ontology = ontPropItem->getOntology();
					LOG(INFO,getLogDomain(),logTr("Starting Transitive Reduction for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

					// build taxonomy

					COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontPropItem;
					QList<COptimizedKPSetRoleTestingItem*>* satItemList = optKPSetClassificationItem->getSatisfiableRoleItemList();

					CRolePropertiesHierarchy* hierarchy = optKPSetClassificationItem->getHierarchy();

					QList<COptimizedKPSetRoleTestingItem*> itemList;

					COptimizedKPSetRoleTestingItem* topItem = optKPSetClassificationItem->getTopRoleSatisfiableTestItem();
					CRolePropertiesHierarchyNode* topHierNode = hierarchy->getTopHierarchyNode();
					CRolePropertiesHierarchyNode* bottomHierNode = hierarchy->getBottomHierarchyNode();



					cint64 topSubsumingItemCount = topItem->getSubsumerRoleItemCount();
					topItem->setSatisfiableRoleHierarchyNode(topHierNode);
					if (topSubsumingItemCount) {
						QList<COptimizedKPSetRoleTestingItem*>* subsumingConceptItemList = topItem->sortSubsumingConceptItemList();
						for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
							COptimizedKPSetRoleTestingItem* subsumedConceptItem = *subsumedIt;
							// mark as equivalent
							subsumedConceptItem->setEquivalentItem(true);
							topHierNode->addEquivalentElement(subsumedConceptItem->getTestingRole());
						}
						hierarchy->updateNodeEquivalentElements(topHierNode);
					}

					LOG(INFO,getLogDomain(),logTr("Sorting roles for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

					for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator it = satItemList->constBegin(), itEnd = satItemList->constEnd(); it != itEnd; ++it) {
						COptimizedKPSetRoleTestingItem* item = *it;

						if (!item->isEquivalentItem() && item != topItem) {
							CRolePropertiesHierarchyNode* itemNode = hierarchy->getHierarchyNode(item->getTestingRole(),true);
							item->setSatisfiableRoleHierarchyNode(itemNode);
							cint64 itemSubsumingCount = item->getSubsumerRoleItemCount();
							itemList.append(item);
							if (itemSubsumingCount > topSubsumingItemCount) {
								QList<COptimizedKPSetRoleTestingItem*>* subsumingConceptItemList = item->sortSubsumingConceptItemList();
								for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
									COptimizedKPSetRoleTestingItem* subsumedConceptItem = *subsumedIt;
									cint64 subsumedItemSubsumingCount = subsumedConceptItem->getSubsumerRoleItemCount();
									if (subsumedItemSubsumingCount == itemSubsumingCount) {
										// mark as equivalent
										subsumedConceptItem->setEquivalentItem(true);
										itemNode->addEquivalentElement(subsumedConceptItem->getTestingRole());
									} else {
										break;
									}
								}
							}
							hierarchy->updateNodeEquivalentElements(itemNode);
						}
					}

					LOG(INFO,getLogDomain(),logTr("Transitive Reduction for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

					for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedKPSetRoleTestingItem* item = *it;

						if (!item->isEquivalentItem() && item != topItem) {
							cint64 itemSubsumingCount = item->getSubsumerRoleItemCount();
							CRolePropertiesHierarchyNode* itemHierNode = item->getSatisfiableRoleHierarchyNode();

							QList<COptimizedKPSetRoleTestingItem*>* subsumingConceptItemList = item->getSubsumerRoleItemList();
							QList<COptimizedKPSetRoleTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd();

							cint64 eqCount = 0;
							cint64 remainingSubsumingCount = itemSubsumingCount;
							// remove direct equivalences
							while (subsumedIt != subsumedItEnd) {
								COptimizedKPSetRoleTestingItem* subsumedConceptItem = *subsumedIt;
								cint64 subsumedItemSubsumingCount = subsumedConceptItem->getSubsumerRoleItemCount();
								if (itemSubsumingCount != subsumedItemSubsumingCount) {
									break;
								} else {
									++eqCount;
									++subsumedIt;
									--remainingSubsumingCount;
								}
							}

							while (subsumedIt != subsumedItEnd && (*subsumedIt)->isEquivalentItem()) {
								++subsumedIt;
							}

							// find first parent
							if (subsumedIt == subsumedItEnd) {
								// no parent, use top as parent
								itemHierNode->makeParent(topHierNode);
							} else {
								itemHierNode->addPredecessorNode(topHierNode);
								QList<COptimizedKPSetRoleTestingItem*> parentItemList;
								while (subsumedIt != subsumedItEnd) {
									COptimizedKPSetRoleTestingItem* subsumedConceptItem = *subsumedIt;
									if (!subsumedConceptItem->isEquivalentItem()) {
										subsumedConceptItem->setPredecessorItem(true);
										CRolePropertiesHierarchyNode* subsumedHierNode = subsumedConceptItem->getSatisfiableRoleHierarchyNode();
										itemHierNode->makeParent(subsumedHierNode);
										parentItemList.append(subsumedConceptItem);
										remainingSubsumingCount -= subsumedConceptItem->getSubsumerRoleItemCount()+1;
										++subsumedIt;
										break;
									} else {
										++subsumedIt;
									}
								}

								bool searchForMoreParents = remainingSubsumingCount > 0;
								while (subsumedIt != subsumedItEnd) {
									COptimizedKPSetRoleTestingItem* subsumedConceptItem = *subsumedIt;
									if (!subsumedConceptItem->isEquivalentItem()) {
										bool predOfOnePrevParent = false;
										if (searchForMoreParents) {
											for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator parentIt = parentItemList.constBegin(), parentItEnd = parentItemList.constEnd(); parentIt != parentItEnd && !predOfOnePrevParent; ++parentIt) {
												COptimizedKPSetRoleTestingItem* parentItem = *parentIt;
												if (parentItem->getSubsumerRoleItemSet()->contains(subsumedConceptItem)) {
													predOfOnePrevParent = true;
												}
											}
										}
										subsumedConceptItem->setPredecessorItem(true);
										CRolePropertiesHierarchyNode* subsumedHierNode = subsumedConceptItem->getSatisfiableRoleHierarchyNode();
										if (!predOfOnePrevParent && searchForMoreParents) {
											itemHierNode->makeParent(subsumedHierNode);
											parentItemList.append(subsumedConceptItem);
										} else {
											itemHierNode->addPredecessorNode(subsumedHierNode);
										}
									}
									++subsumedIt;
								}
							}
						}
					}

					for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedKPSetRoleTestingItem* item = *it;
						if (!item->isPredecessorItem() && !item->isEquivalentItem()) {
							CRolePropertiesHierarchyNode* itemHierNode = item->getSatisfiableRoleHierarchyNode();
							bottomHierNode->makeParent(itemHierNode);
						}
					}

					LOG(INFO,getLogDomain(),logTr("Collect statistics for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());


#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
					statUpdateMutex.lock();
					statusString = QString("Finished ontology role classification");
					statUpdateMutex.unlock();
#endif


					CClassification* classification = ontology->getClassification();
					if (classification) {
						CPropertyRoleClassification* propRoleClassification = ontPropItem->getPropertyRoleClassification();
						CClassificationStatisticsCollectionStrings* classifStatCollStrings = ontPropItem->getClassificationStatisticsCollectionStrings();
						CClassifierStatistics* classifierStats = ontPropItem->getClassifierStatistics();
						classifStatCollStrings->addProcessingStatistics("role-classification-total-satisfiable-test-count",classifierStats->getTotalSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-tested-count",classifierStats->getTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-derivate-tested-count",classifierStats->getDerivatedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-calculated-tested-count",classifierStats->getCalculatedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-pruned-tested-count",classifierStats->getPrunedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-told-tested-count",classifierStats->getToldSatisfiableCount()-classifierStats->getCalculatedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-total-subsumption-test-count",classifierStats->getTotalSatisfiableCount()*(classifierStats->getTotalSatisfiableCount()-1));
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-tested-count",classifierStats->getTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-subproperty-tested-count",classifierStats->getSubclassTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-subproperty-identifier-occur-tested-count",classifierStats->getSubclassIdentifierOccurTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-derivate-tested-count",classifierStats->getDerivatedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-calculated-tested-count",classifierStats->getCalculatedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-pruned-tested-count",classifierStats->getPrunedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-told-tested-count",classifierStats->getToldSubsumptionCount()-classifierStats->getCalculatedTestedSubsumptionCount());
						propRoleClassification->setRolePropertiesHierarchy(hierarchy);
						propRoleClassification->setClassificationStatistics(classifStatCollStrings);
						classification->setPropertyRoleClassification(propRoleClassification,optKPSetClassificationItem->isDataRolesClassification());
					}

					ontPropItem->setHasRemainingTests(false);

					--mClassificationCount;
					LOG(INFO,getLogDomain(),logTr("Roles of ontology '%1' classified.").arg(ontology->getTerminologyName()),getLogObject());
					LOG(INFO,getLogDomain(),logTr("Used %1 satisfiable tests and %3 calculated subsumption tests.").arg(ontPropItem->getCalcedSatisfiableCount()).arg(ontPropItem->getClassifierStatistics()->getCalculatedTestedSubsumptionCount()),getLogObject());
					LOG(INFO,getLogDomain(),logTr("Confirmed %1 of %2 possible subsumer roles.").arg(optKPSetClassificationItem->getTruePossibleSubsumerCount()).arg(optKPSetClassificationItem->getPossibleSubsumerCount()),getLogObject());
					LOG(INFO,getLogDomain(),logTr("Confirmed %1 and invalidated %2 possible subsumer roles with calculation.").arg(optKPSetClassificationItem->getCalculatedTruePossibleSubsumerCount()).arg(optKPSetClassificationItem->getFalsePossibleSubsumerCount()),getLogObject());
				}
				ontPropItem->submitHierarchyConstructed();
				ontPropItem->doClassifiedCallback();
				return true;
			}



		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
