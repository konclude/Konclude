/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "COptimizedKPSetRoleSubsumptionClassifierThread.h"

namespace Konclude {

	namespace Reasoner {

		namespace Classifier {





			COptimizedKPSetRoleSubsumptionClassifierThread::COptimizedKPSetRoleSubsumptionClassifierThread(CReasonerManager *reasonerManager) 
					: CSubsumptionClassifierThread(reasonerManager), CLogIdentifier("::Konclude::Reasoner::Kernel::Classifier::OptimizedKPSetRoleSubsumptionClassifierThread",this) {
				satTestedCount = 0;
				totalToSatTestClassCount = 0;
				mClassificationCount = 0;
				subsumTestedCount = 0;
				totalToSubsumTestCount = 0;

				mConfPossSubsumCalcOrderConceptSorted = true;
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
			}


			COptimizedKPSetRoleSubsumptionClassifierThread::~COptimizedKPSetRoleSubsumptionClassifierThread() {
			}


			CTaxonomy *COptimizedKPSetRoleSubsumptionClassifierThread::createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config) {
				CTBox *tBox = ontology->getTBox();
				CConceptVector *conVec = tBox->getConceptVector();
				CConcept *topConcept = conVec->getData(1);
				CConcept *bottomConcept = conVec->getData(0);
				CPartialPruningTaxonomy *tax = new CPartialPruningTaxonomy(topConcept,bottomConcept);
				return tax->readConfig(config);
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
					cint64 multiplicator = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedKPSetClassSubsumptionClassifier.MultipliedUnitsParallelSatisfiableCalculationCount",1,&mulConfigErrorFlag);
					confMaxTestParallelCount = processorCount*multiplicator;
					bool maxConfigErrorFlag = false;
					cint64 maxParallel = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedKPSetClassSubsumptionClassifier.MaximumParallelSatisfiableCalculationCount",1,&maxConfigErrorFlag);
					if (!maxConfigErrorFlag) {
						if (!mulConfigErrorFlag) {
							confMaxTestParallelCount = qMin(confMaxTestParallelCount,maxParallel);
						} else {
							confMaxTestParallelCount = maxParallel;
						}
					}
					double factor = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedKPSetClassSubsumptionClassifier.ParallelSatisfiableCalculationCreationFactor",1000)/1000.;
					confMinTestParallelCount = confMaxTestParallelCount*factor;

					mConfWriteDebuggingData = CConfigDataReader::readConfigBoolean(config,"Konclude.Debugging.WriteDebuggingData",false);

				} else {
					confMaxTestParallelCount = 1;
					confMinTestParallelCount = 1;
					mConfWriteDebuggingData = false;
				}

			}


			
			CSubsumptionClassifierThread *COptimizedKPSetRoleSubsumptionClassifierThread::scheduleOntologyClassification(CConcreteOntology *ontology, CTaxonomy *taxonomy, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config) {

				COptimizedKPSetRoleOntologyClassificationItem *ontClassItem = new COptimizedKPSetRoleOntologyClassificationItem(config,statistics);
				ontClassItem->initTaxonomyConcepts(ontology,taxonomy);
				ontItemList.append(ontClassItem);
				processingOntItemList.append(ontClassItem);
				ontItemHash.insert(ontology,ontClassItem);

				readCalculationConfig(ontClassItem->getCalculationConfiguration());

				if (CConfigDataReader::readConfigBoolean(ontClassItem->getCalculationConfiguration(),"Konclude.Calculation.Classification.IndividualDependenceTracking",true)) {
					ontClassItem->setIndividualDependenceTrackingCollector(new CIndividualDependenceTrackingCollector());
				}


				CPartialPruningTaxonomy *parTax = dynamic_cast<CPartialPruningTaxonomy *>(taxonomy);
				if (parTax) {
					COntologyClassificationItem *ontClassItem = ontItemHash.value(ontology);
					parTax->createStatistics(ontClassItem->getClassifierStatistics());
				}


				if (satTestedCount == totalToSatTestClassCount) {
					satTestedCount = 0;
					totalToSatTestClassCount = ontClassItem->getRemainingSatisfiableTestsCount();
					classStartTime.start();
				} else {
					totalToSatTestClassCount += ontClassItem->getRemainingSatisfiableTestsCount();
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
					mClassifyProgess.setTestedClasses(satTestedCount);
					mClassifyProgess.setTotalClasses(totalToSatTestClassCount);
					mClassifyProgess.setTestedSubsumptions(subsumTestedCount);
					mClassifyProgess.setTotalSubsumptions(totalToSubsumTestCount);
				} else {
					mClassifyProgess.setProgessPercent(0.);
					mClassifyProgess.setRemainingMilliSeconds(0.);
					mClassifyProgess.setTestedClasses(satTestedCount);
					mClassifyProgess.setTotalClasses(totalToSatTestClassCount);
					mClassifyProgess.setTestedSubsumptions(subsumTestedCount);
					mClassifyProgess.setTotalSubsumptions(totalToSubsumTestCount);
				}
				return &mClassifyProgess;
			}

			bool itemSortLessSubsumptionsThan(const COptimizedKPSetRoleTestingItem* item1, const COptimizedKPSetRoleTestingItem* item2) {
				return item1->getSubsumingConceptItemCount() < item2->getSubsumingConceptItemCount();
			}

			bool itemSortMoreSubsumptionsThan(const COptimizedKPSetRoleTestingItem* item1, const COptimizedKPSetRoleTestingItem* item2) {
				return item1->getSubsumingConceptItemCount() > item2->getSubsumingConceptItemCount();
			}



			void COptimizedKPSetRoleSubsumptionClassifierThread::testDebugPossibleSubsumerCorrectCounted(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem) {
				QList<COptimizedKPSetRoleTestingItem*>* classList = optKPSetClassificationItem->getConceptSatisfiableTestItemContainer();
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
				QList<COptimizedKPSetRoleTestingItem*>* classList = optKPSetClassificationItem->getSatisfiableConceptItemList();
				if (mConfPossSubsumCalcOrderBottomUp) {
					QSet<COptimizedKPSetRoleTestingItem*>* remItemSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionTestingSet();
					foreach (COptimizedKPSetRoleTestingItem* item, *classList) {
						COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
						if (possSubsumMap) {
							for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetRolePossibleSubsumptionData* data = it.value();
								if (data->isSubsumptionUnknown() || data->isUpdateRequired()) {				
									COptimizedKPSetRoleTestingItem* possSubsumerItem = data->getTestingItem();
									if (!possSubsumerItem->getPossibleSubsumedSet(false)->contains(item) && !optKPSetClassificationItem->getCurrentPossibleSubsumptionTestingItemSet()->contains(possSubsumerItem)) {
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
				QList<COptimizedKPSetRoleTestingItem*>* classList = optKPSetClassificationItem->getSatisfiableConceptItemList();
				foreach (COptimizedKPSetRoleTestingItem* item, *classList) {
					QString iriClassNameString = CIRIName::getRecentIRIName(item->getTestingConcept()->getClassNameLinker());
					QString classDebugString = QString("Class: %1\r\n").arg(iriClassNameString);
					QString subsumString;
					foreach (COptimizedKPSetRoleTestingItem* subsumItem, *item->getSubsumingConceptItemList()) {
						QString subsumIRIClassNameString = CIRIName::getRecentIRIName(subsumItem->getTestingConcept()->getClassNameLinker());
						if (subsumString.isEmpty()) {
							subsumString += subsumIRIClassNameString;
						} else {
							subsumString += QString(", %1").arg(subsumIRIClassNameString);
						}
					}
					classDebugString += QString("Subsumer Roles: %1\r\n").arg(subsumString);

					QString possSubsumString;
					COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
					if (possSubsumMap) {
						for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetRolePossibleSubsumptionData* data = it.value();
							QString subsumIRIClassNameString = CIRIName::getRecentIRIName(data->getTestingItem()->getTestingConcept()->getClassNameLinker());
							QString possString;
							if (data->isSubsumptionConfirmed()) {
								possString = "s";
							} else if (data->isSubsumptionInvalided()) {
								possString = "n";
							} else {
								possString = "u";
							}

							if (possSubsumString.isEmpty()) {
								possSubsumString += QString("%1(%2)").arg(subsumIRIClassNameString).arg(possString);
							} else {
								possSubsumString += QString(", %1(%2)").arg(subsumIRIClassNameString).arg(possString);
							}
						}
					}
					classDebugString += QString("Possible Subsumer Roles: %1\r\n\r\n\r\n").arg(possSubsumString);

					debugString += classDebugString;
				}

				QFile file(filename);
				if (file.open(QIODevice::WriteOnly)) {
					file.write(debugString.toLocal8Bit());
					file.close();
				}

				return debugString;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::createNextSubsumtionTest() {


				COntologyClassificationItem *loopOntClassItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !processingOntItemList.isEmpty()) {
					COntologyClassificationItem *ontClassItem = processingOntItemList.first();

					if (ontClassItem == loopOntClassItem) {
						// don't run into infinite loop without doing something
						break;
					}

					COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontClassItem;

					if (!optKPSetClassificationItem->hasSatisfiableTestingPhaseFinished() && !ontClassItem->isTaxonomyConstructionFailed()) {

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
									interpreteSatisfiableResult(optKPSetClassificationItem,nextSatTestItem->getTestingConcept(),nextSatTestItem->getSatisfiableTestedResult());
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

								COptimizedKPSetRoleTestingItem* topItem = optKPSetClassificationItem->getTopConceptSatisfiableTestItem();
								COptimizedKPSetRoleTestingItem* bottomItem = optKPSetClassificationItem->getBottomConceptSatisfiableTestItem();

								topItem->setPropagationConnected(true);

								QList<COptimizedKPSetRoleTestingItem*> classList(*optKPSetClassificationItem->getSatisfiableConceptItemList());
								qSort(classList.begin(),classList.end(),itemSortLessSubsumptionsThan);
								QList<COptimizedKPSetRoleTestingItem*> resevePossSubsumClassList;
								foreach (COptimizedKPSetRoleTestingItem* item, classList) {
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

									QList<COptimizedKPSetRoleTestingItem*>* subsumerList = item->getSubsumingConceptItemList();
									COptimizedKPSetRoleTestingItem* maxSubsumerItem = nullptr;
									cint64 maxSubsumerCount = 0;
									for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator it = subsumerList->constBegin(), itEnd = subsumerList->constEnd(); it != itEnd; ++it) {
										COptimizedKPSetRoleTestingItem* subsumerItem = *it;
										if (!maxSubsumerItem || subsumerItem->getSubsumingConceptItemCount() > maxSubsumerCount) {
											maxSubsumerCount = subsumerItem->getSubsumingConceptItemCount();
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
											if (propItem == subsumerItem || propItem->hasSubsumerConceptItem(subsumerItem)) {
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


								foreach (COptimizedKPSetRoleTestingItem* item, classList) {
									COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
									QSet<COptimizedKPSetRoleTestingItem*>* upPropSet = item->getUpPropagationItemSet();
									if (!possSubsumMap && item->isPossibleSubsumptionMapInitialized()) {
										for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator itProp = upPropSet->constBegin(), itPropEnd = upPropSet->constEnd(); itProp != itPropEnd; ++itProp) {
											COptimizedKPSetRoleTestingItem* upPropItem = *itProp;

											COptimizedKPSetRolePossibleSubsumptionMap* upPropItemPossSubsumMap = upPropItem->getPossibleSubsumptionMap(false);
											if (upPropItemPossSubsumMap) {
												for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator itUpPoss = upPropItemPossSubsumMap->constBegin(), itUpPossEnd = upPropItemPossSubsumMap->constEnd(); itUpPoss != itUpPossEnd; ++itUpPoss) {
													COptimizedKPSetRolePossibleSubsumptionData* upPossData = itUpPoss.value();
													if (!item->hasSubsumerConceptItem(upPossData->getTestingItem()) && item != upPossData->getTestingItem()) {
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
														CConcept* possCon = itPoss.key().getConcept();
														CConcept* possUpCon = itUpPoss.key().getConcept();
														if (possCon->getConceptTag() == possUpCon->getConceptTag()) {
															++itPoss;
															++itUpPoss;
														} else if (possCon->getConceptTag() < possUpCon->getConceptTag()) {
															++itPoss;
														} else if (possCon->getConceptTag() > possUpCon->getConceptTag()) {
															COptimizedKPSetRolePossibleSubsumptionData* upPossData = itUpPoss.value();
															if (!item->hasSubsumerConceptItem(upPossData->getTestingItem()) && item != upPossData->getTestingItem()) {
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
													if (!item->hasSubsumerConceptItem(upPossData->getTestingItem()) && item != upPossData->getTestingItem()) {
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
														possSubsumerItem->getPossibleSubsumedSet(true)->insert(item);
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
										QList<COptimizedKPSetRoleTestingItem*>* subsumersSortedList = new QList<COptimizedKPSetRoleTestingItem*>(possSubsumerItem->getPossibleSubsumedSet(false)->toList());										
										qSort(subsumersSortedList->begin(),subsumersSortedList->end(),itemSortMoreSubsumptionsThan);
										possSubsumerItem->setPossibleSubsumedList(subsumersSortedList);
									}
								}

								if (mConfWriteDebuggingData) {
									createDebugKPSetString(optKPSetClassificationItem,"classkpsets-inital-pruned.txt");
								}

							}
						}
					}

					if (optKPSetClassificationItem->hasSatisfiableTestingPhaseFinished() && !optKPSetClassificationItem->hasPossibleSubsumptionTestingPhaseFinished() && !ontClassItem->isTaxonomyConstructionFailed()) {

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
												interpreteSubsumptionResult(optKPSetClassificationItem,nextPossSubsumTestItem->getTestingConcept(),possSubsumCalcData->getTestingItem()->getTestingConcept(),possSubsumCalcData->isSubsumptionConfirmed());
											}
										}
									} else {
										QSet<COptimizedKPSetRoleTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionTestingSet();
										remTestingSet->remove(nextPossSubsumTestItem);
									}

								} else if (mConfPossSubsumCalcOrderBottomUp) {
									COptimizedKPSetRolePossibleSubsumptionData* possSubsumCalcData = nullptr;
									COptimizedKPSetRoleTestingItem* possSubsumedItem = nullptr;


									CConcept* possSubsumerConcept = nextPossSubsumTestItem->getTestingConcept();
									CConcept* candidateConcept = possSubsumerConcept;

									QList<COptimizedKPSetRoleTestingItem*>* possSubsumedSortedList = nextPossSubsumTestItem->getPossibleSubsumedList();
									if (possSubsumedSortedList && !possSubsumedSortedList->isEmpty()) {										
										while (!possSubsumCalcData && !possSubsumedSortedList->isEmpty()) {
											possSubsumedItem = possSubsumedSortedList->takeFirst();
											if (nextPossSubsumTestItem->getPossibleSubsumedSet(false)->contains(possSubsumedItem)) {
												nextPossSubsumTestItem->getPossibleSubsumedSet(false)->remove(possSubsumedItem);
												COptimizedKPSetRolePossibleSubsumptionMap* possSubsumerMap = possSubsumedItem->getPossibleSubsumptionMap(false);
												if (possSubsumerMap && possSubsumerMap->hasRemainingPossibleSubsumptions()) {
													possSubsumCalcData = possSubsumerMap->value(CConceptTagComparer(candidateConcept));
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
												interpreteSubsumptionResult(optKPSetClassificationItem,possSubsumedItem->getTestingConcept(),nextPossSubsumTestItem->getTestingConcept(),possSubsumCalcData->isSubsumptionConfirmed());
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
						if (optKPSetClassificationItem->hasPossibleSubsumptionTestingPhaseFinished() || ontClassItem->isTaxonomyConstructionFailed()) {
							finishOntologyClassification(ontClassItem);
							processingOntItemList.removeFirst();
						} else {
							processingOntItemList.removeFirst();
							processingOntItemList.append(ontClassItem);
							loopOntClassItem = ontClassItem;
						}
					}
				}
				return workTestCreated;
			}


			COptimizedKPSetRolePossibleSubsumptionData* COptimizedKPSetRoleSubsumptionClassifierThread::getNextPossibleSubsumptionCalculation(COptimizedKPSetRoleOntologyClassificationItem *ontClassItem, COptimizedKPSetRoleTestingItem* classItem) {
				COptimizedKPSetRolePossibleSubsumptionData* possSubsumer = nullptr;
				// I don't know which is the best, simply return the first one
				// sort by subsumer count?
				COptimizedKPSetRolePossibleSubsumptionMap* possSubsumerMap = classItem->getPossibleSubsumptionMap(false);
				if (possSubsumerMap) {
					if (mConfPossSubsumCalcOrderConceptSorted) {
						for (COptimizedKPSetRolePossibleSubsumptionMap::const_iterator it = possSubsumerMap->constBegin(), itEnd = possSubsumerMap->constEnd(); !possSubsumer && it != itEnd; ++it) {
							COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = it.value();
							if (possSubsumData->isSubsumptionUnknown()) {
								possSubsumer = possSubsumData;
							} else if (possSubsumData->isUpdateRequired()) {
								prunePossibleSubsumptions(ontClassItem,classItem,possSubsumData);							
							}
						}
					}
				}
				return possSubsumer;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::calculateSatisfiable(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* nextSatTestItem) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CClassificationWorkItem *workItem = 0;

				CConcept* concept = nextSatTestItem->getTestingConcept();
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
				CPrecomputedSaturationSubsumerExtractor* precSatSubsumerExtractor = optKPSetClassificationItem->getPrecomputedSaturationSubsumerExtractor(false);
				bool unsatisfiableFlag = false;
				if (precSatSubsumerExtractor && precSatSubsumerExtractor->getConceptFlags(concept,&unsatisfiableFlag,nullptr,nullptr) && unsatisfiableFlag) {
					satisfiableFlag = false;
					nextSatTestItem->setSatisfiableTested(true);
					nextSatTestItem->setSatisfiableTestedResult(false);
					return false;
				}

				CSatisfiableCalculationJobGenerator satCalcJobGen(optKPSetClassificationItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(concept);
				QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *> *workHash = optKPSetClassificationItem->getWorkItemHash();

				CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
				if (ontClassStat) {
					ontClassStat->incCalculatedTestedSatisfiableCount(1);
				}

				workItem = new CClassificationWorkItem(satCalcJob,concept);
				workHash->insertMulti(satCalcJob,workItem);
				workOntItemHash.insert(workItem,optKPSetClassificationItem);

				QHash<CConcept*,CClassificationSatisfiableCalculationConceptReferenceLinking*>* conRefLinkDataHash = optKPSetClassificationItem->getConceptReferenceLinkingDataHash();
				cint64 extFlags = CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTALL;
				// TODO: set extraction flags


				satCalcJob->setSatisfiableClassificationMessageAdapter(new CSatisfiableTaskClassificationMessageAdapter(concept,optKPSetClassificationItem->getOntology(),this,conRefLinkDataHash,extFlags));

				optKPSetClassificationItem->incCurrentCalculatingCount();
				processCalculationJob(satCalcJob,optKPSetClassificationItem,workItem);
				if (optKPSetClassificationItem->getIndividualDependenceTrackingCollector()) {
					satCalcJob->setSatisfiableTaskIndividualDependenceTrackingAdapter(new CSatisfiableTaskIndividualDependenceTrackingAdapter(optKPSetClassificationItem->getIndividualDependenceTrackingCollector()));
				}
				++mCreatedCalculationTaskCount;
				return true;
			}





			bool COptimizedKPSetRoleSubsumptionClassifierThread::calculateSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* subsumedItem, COptimizedKPSetRoleTestingItem* possSubsumerItem, COptimizedKPSetRolePossibleSubsumptionData* possSubsumData) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CClassificationWorkItem *workItem = 0;
				++mOrderedSubsumptionCalculationCount;

				CConcept* subsumedConcept = subsumedItem->getTestingConcept();
				CConcept* subsumerConcept = possSubsumerItem->getTestingConcept();

				bool isSubsumptionFlag = false;
				bool precheckedSubsum = false;


				CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
				if (possSubsumData && possSubsumData->isSubsumptionKnown()) {
					return false;
				} else if (possSubsumData && subsumedItem->hasSubsumerConceptItem(possSubsumerItem)) {					
					possSubsumData->setSubsumptionConfirmed(true);
					return false;
				}

				CSatisfiableCalculationJobGenerator satCalcJobGen(optKPSetClassificationItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(subsumedConcept,false,subsumerConcept,true);
				QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *> *workHash = optKPSetClassificationItem->getWorkItemHash();

				if (ontClassStat) {
					ontClassStat->incCalculatedTestedSubsumptionCount(1);
					ontClassStat->incToldSubsumptionCount(1);
				}

				workItem = new CClassificationWorkItem(satCalcJob,subsumerConcept,subsumedConcept);
				workHash->insertMulti(satCalcJob,workItem);
				workOntItemHash.insert(workItem,optKPSetClassificationItem);

				QHash<CConcept*,CClassificationSatisfiableCalculationConceptReferenceLinking*>* conRefLinkDataHash = optKPSetClassificationItem->getConceptReferenceLinkingDataHash();
				cint64 extFlags = CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTSUBSUMERSOTHERNODES | 
						CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTPOSSIBLESUBSUMERSROOTNODE | 
						CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTPOSSIBLESUBSUMERSOTHERNODES | 
						CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTOTHERNODESMULTIPLEDEPENDENCY;
				// TODO: set extraction flags

				satCalcJob->setSatisfiableClassificationMessageAdapter(new CSatisfiableTaskClassificationMessageAdapter(subsumedConcept,optKPSetClassificationItem->getOntology(),this,conRefLinkDataHash,extFlags));
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




			CSubsumptionClassifierThread *COptimizedKPSetRoleSubsumptionClassifierThread::processCalculationJob(CSatisfiableCalculationJob* job, COntologyClassificationItem *ontClassItem, CClassificationWorkItem* workItem) {
				CClassificationCalculationStatisticsCollection* statColl =  nullptr;
				if (ontClassItem->isCollectProcessStatisticsActivated()) {
					statColl = ontClassItem->getCalculationStatisticsCollection();
				}
				job->setCalclulationStatisticsCollector(statColl);
				CTestCalculatedCallbackEvent *testResultCallback = new CTestCalculatedCallbackEvent(this,job,workItem,statColl);
				job->setCalculationConfiguration(ontClassItem->getCalculationConfiguration());
				CSubsumptionClassifierThread::processCalculationJob(job,ontClassItem,testResultCallback,false);
				return this;
			}



			bool COptimizedKPSetRoleSubsumptionClassifierThread::processToldClassificationMessage(COntologyClassificationItem *ontClassItem, CClassificationMessageData* messageDataLinker, CMemoryPool* memoryPools) {
				COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontClassItem;
				CClassificationMessageData* messageDataLinkerIt = messageDataLinker;
				while (messageDataLinkerIt) {
					CClassificationMessageData* messageData = messageDataLinkerIt;
					if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLCLASSSUBSUMPTION) {
						++mStatProcesedSubsumMessCount;
						CClassificationSubsumptionMessageData* subsumMessageData = (CClassificationSubsumptionMessageData*)messageData;
						CConcept* subsumedConcept = subsumMessageData->getSubsumedConcept();
						CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* subsumerList = subsumMessageData->getClassSubsumerList();

						COptimizedKPSetRoleTestingItem* subsumedItem = nullptr;
						subsumedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumedConcept);

						COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = subsumedItem->getPossibleSubsumptionMap(false);
						if (subsumerList) {
							for (CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>::const_iterator it = subsumerList->constBegin(), itEnd = subsumerList->constEnd(); it != itEnd; ++it) {
								CConcept* subsumerConcept(*it);
								COptimizedKPSetRoleTestingItem* subsumerItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumerConcept);
								if (subsumedItem != subsumerItem) {
									subsumedItem->addSubsumingConceptItem(subsumerItem);
									if (possSubsumMap) {
										COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = possSubsumMap->value(subsumerConcept,nullptr);
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
						//if (!possSubsumMap || !possSubsumMap->hasRemainingPossibleSubsumptions()) {
						subsumedItem->setResultSatisfiableDerivated(true);
						//}

					} else if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLCLASSINITIALIZEPOSSIBLESUBSUM) {
						++mStatProcesedPossSubsumInitMessCount;
						CClassificationInitializePossibleSubsumptionMessageData* possSubsumMessageData = (CClassificationInitializePossibleSubsumptionMessageData*)messageData;
						CConcept* subsumedConcept = possSubsumMessageData->getSubsumedConcept();
						CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>* possSubsumerList = possSubsumMessageData->getClassPossibleSubsumerList();


						COptimizedKPSetRoleTestingItem* subsumedItem = nullptr;
						subsumedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumedConcept);

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
										for (CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>::const_iterator it = possSubsumerList->constBegin(), itEnd = possSubsumerList->constEnd(); it != itEnd; ++it) {
											CClassificationInitializePossibleSubsumptionData* possSubsumDataIt = *it;
											if (possSubsumDataIt->isPossibleSubsumerValid()) {
												CConcept* possSubsumConcept = possSubsumDataIt->getPossibleSubsumerConcept();
												COptimizedKPSetRoleTestingItem* possSubsumItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(possSubsumConcept);
												if (!subsumedItem->hasSubsumerConceptItem(possSubsumItem) && subsumedItem != possSubsumItem) {
													COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = new COptimizedKPSetRolePossibleSubsumptionData(possSubsumItem);
													possSubsumMap->insert(CConceptTagComparer(possSubsumConcept),possSubsumData);
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
												CConcept* possCon = itPoss.key().getConcept();
												CConcept* possUpCon = itUpPoss.key().getConcept();
												if (possCon->getConceptTag() == possUpCon->getConceptTag()) {
													++itPoss;
													++itUpPoss;
												} else if (possCon->getConceptTag() < possUpCon->getConceptTag()) {
													++itPoss;
												} else if (possCon->getConceptTag() > possUpCon->getConceptTag()) {
													COptimizedKPSetRolePossibleSubsumptionData* upPossData = itUpPoss.value();
													if (!subsumedItem->hasSubsumerConceptItem(upPossData->getTestingItem()) && subsumedItem != upPossData->getTestingItem()) {
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
												if (!subsumedItem->hasSubsumerConceptItem(upPossData->getTestingItem()) && subsumedItem != upPossData->getTestingItem()) {
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
												CConcept* possCon = itPoss.key().getConcept();
												CConcept* possDownCon = itDownPoss.key().getConcept();
												if (possCon->getConceptTag() == possDownCon->getConceptTag()) {
													++itPoss;
													++itDownPoss;
												} else if (possDownCon->getConceptTag() < possCon->getConceptTag()) {
													++itDownPoss;
												} else if (possDownCon->getConceptTag() > possCon->getConceptTag()) {
													COptimizedKPSetRolePossibleSubsumptionData* possData = itPoss.value();
													if (!downPropItem->hasSubsumerConceptItem(possData->getTestingItem()) && downPropItem != possData->getTestingItem()) {
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
												if (!downPropItem->hasSubsumerConceptItem(possData->getTestingItem()) && downPropItem != possData->getTestingItem()) {
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
												if (!downPropItem->hasSubsumerConceptItem(possData->getTestingItem()) && downPropItem != possData->getTestingItem()) {
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
										CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>::const_iterator itNew = possSubsumerList->constBegin(), itNewEnd = possSubsumerList->constEnd();
										while (itPoss != itPossEnd && itNew != itNewEnd) {
											COptimizedKPSetRolePossibleSubsumptionData* possData = itPoss.value();
											CConcept* possCon = itPoss.key().getConcept();
											CClassificationInitializePossibleSubsumptionData* possNewSubsumDataIt = *itNew;
											if (possNewSubsumDataIt->isPossibleSubsumerValid()) {
												CConcept* newCon = possNewSubsumDataIt->getPossibleSubsumerConcept();
												if (possCon->getConceptTag() == newCon->getConceptTag()) {
													++itPoss;
													++itNew;
												} else if (possCon->getConceptTag() < newCon->getConceptTag()) {
													if (!possData->isSubsumptionInvalided()) {
														possData->setSubsumptionInvalid(true);
														if (possData->isUpdateRequired()) {
															prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
														}
													}
													++itPoss;
												} else if (possCon->getConceptTag() > newCon->getConceptTag()) {
													++itNew;
												}
											} else {
												++itNew;
											}
										}
									}
									while (itPoss != itPossEnd) {
										COptimizedKPSetRolePossibleSubsumptionData* possData = itPoss.value();
										CConcept* possCon = itPoss.key().getConcept();
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


					} else if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLCLASSUPDATEPOSSIBLESUBSUM) {
						++mStatProcesedPossSubsumUpdateMessCount;
						CClassificationUpdatePossibleSubsumptionMessageData* possSubsumMessageData = (CClassificationUpdatePossibleSubsumptionMessageData*)messageData;
						CConcept* subsumedConcept = possSubsumMessageData->getSubsumedConcept();

						COptimizedKPSetRoleTestingItem* subsumedItem = nullptr;
						subsumedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumedConcept);

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


			bool COptimizedKPSetRoleSubsumptionClassifierThread::interpreteToldSubsumptionResult(COntologyClassificationItem *ontClassItem, const QList<QPair<CConcept *,CConcept *> > &subSumRelList, bool isSubSum) {
				CConcept *lastConcept = nullptr;
				COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontClassItem;
				COptimizedKPSetRoleTestingItem* subsumerItem = nullptr;

				for (QList<QPair<CConcept *,CConcept *> >::const_iterator it = subSumRelList.constBegin(), itEnd = subSumRelList.constEnd(); it != itEnd; ++it) {
					CConcept *subsumerConcept = it->first;
					if (lastConcept != subsumerConcept) {
						lastConcept = subsumerConcept;
						subsumerItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumerConcept);
					}
					CConcept *subsumedConcept = it->second;
					if (isSubSum) {
						COptimizedKPSetRoleTestingItem* subsumedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumedConcept);
						subsumerItem->addSubsumingConceptItem(subsumedItem);
					}
				}
				return true;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::interpreteSubsumptionResult(COntologyClassificationItem *ontClassItem, CConcept *subsumedConcept, CConcept *subsumerConcept, bool isSubsumption) {
				COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontClassItem;

				++mInterpretedSubsumptionCalculationCount;
				CConcept* candidateConcept = subsumerConcept;

				optKPSetClassificationItem->decRunningPossibleSubsumptionTestsCount();

				COptimizedKPSetRoleTestingItem* subsumedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumedConcept);
				COptimizedKPSetRoleTestingItem* subsumerItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumerConcept);



				//QString iriClassNameString1 = CIRIName::getRecentIRIName(subsumedConcept->getClassNameLinker());
				//QString iriClassNameString2 = CIRIName::getRecentIRIName(subsumerConcept->getClassNameLinker());
				//if (iriClassNameString1 == "http://www.bootstrep.eu/ontology/GRO#BindingOfProteinToProteinBindingSiteOfDNA" && iriClassNameString2 == "http://www.bootstrep.eu/ontology/GRO#DNAProteinInteraction" && !isSubsumption) {
				//	bool bug = true;
				//	calculateSubsumption(optKPSetClassificationItem,subsumedItem,subsumerItem,nullptr);
				//}


				COptimizedKPSetRoleTestingItem* topItem = optKPSetClassificationItem->getTopConceptSatisfiableTestItem();

				COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = subsumedItem->getPossibleSubsumptionMap(false);
				COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = nullptr;
				if (possSubsumMap) {
					possSubsumData = possSubsumMap->value(candidateConcept);
				}

				if (isSubsumption) {
					optKPSetClassificationItem->incCalculatedTruePossibleSubsumerCount();
					if (possSubsumData) {
						possSubsumData->setSubsumptionConfirmed(true);
					}
					subsumedItem->addSubsumingConceptItem(subsumerItem);
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
							QSet<COptimizedKPSetRoleTestingItem*>* remPossSubsumedItemSet = subsumerItem->getPossibleSubsumedSet(false);
							QList<COptimizedKPSetRoleTestingItem*>* remPossSubsumedItemList = subsumerItem->getPossibleSubsumedList();
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
						CConcept* subsumerConcept = subsumerItem->getTestingConcept();
						// establish the subsumption for all successor nodes
						QSet<COptimizedKPSetRoleTestingItem*>* downPropSet = item->getDownPropagationItemSet();
						for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = downPropSet->constBegin(), itEnd = downPropSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetRoleTestingItem* downPropItem = *it;
							pruneDownSubsumption(optKPSetClassificationItem,downPropItem,subsumerConcept);
						}
						return true;
					} else {
						// is not subsuming
						COptimizedKPSetRoleTestingItem* notSubsumerItem = possSubsumData->getTestingItem();
						CConcept* notSubsumerConcept = notSubsumerItem->getTestingConcept();
						// establish the subsumption for all successor nodes
						QSet<COptimizedKPSetRoleTestingItem*>* upPropSet = item->getUpPropagationItemSet();
						for (QSet<COptimizedKPSetRoleTestingItem*>::const_iterator it = upPropSet->constBegin(), itEnd = upPropSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetRoleTestingItem* upPropItem = *it;
							pruneUpNotSubsumption(optKPSetClassificationItem,upPropItem,notSubsumerConcept);
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
					if (!downPropItem->hasSubsumerConceptItem(subsumerItem) && downPropItem != subsumerItem) {
						downPropItem->addSubsumingConceptItem(subsumerItem);
						propagateDownSubsumption(optKPSetClassificationItem,downPropItem,subsumerItem);
						propagated = true;
					}
				}
				return propagated;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::pruneDownSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, CConcept* subsumerConcept) {		
				COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
				if (possSubsumMap) {
					COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = possSubsumMap->value(subsumerConcept);
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
							pruneDownSubsumption(optKPSetClassificationItem,downPropItem,subsumerConcept);
						}
						return true;
					}
				}
				return false;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::pruneUpNotSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, CConcept* notSubsumerConcept) {		
				COptimizedKPSetRolePossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
				if (possSubsumMap) {
					COptimizedKPSetRolePossibleSubsumptionData* possSubsumData = possSubsumMap->value(notSubsumerConcept);
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
							pruneUpNotSubsumption(optKPSetClassificationItem,upPropItem,notSubsumerConcept);
						}
						return true;
					}
				}
				return false;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::interpreteSatisfiableResult(COntologyClassificationItem *ontClassItem, CConcept *satisfiableConcept, bool isSatis) {
				COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontClassItem;

				CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
				if (ontClassStat) {
					ontClassStat->incToldSatisfiableCount(1);
				}

				++satTestedCount;

				optKPSetClassificationItem->decRunningSatisfiableTestsCount();

				COptimizedKPSetRoleTestingItem* satTestedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(satisfiableConcept);
				//if (CIRIName::getRecentIRIName(satisfiableConcept->getClassNameLinker()) == "http://www.gdst.uqam.ca/Documents/Ontologies/HIT/Equipment_SH_Ontology.owl#Folding_chair" && !isSatis) {
				//	bool bug = true;
				//	calculateSatisfiable(optKPSetClassificationItem,satTestedItem);
				//	return false;
				//}


				satTestedItem->setSatisfiableTested(true);
				satTestedItem->setSatisfiableTestedResult(isSatis);

				COptimizedKPSetRoleTestingItem* topItem = optKPSetClassificationItem->getTopConceptSatisfiableTestItem();

				if (isSatis) {
					optKPSetClassificationItem->addSatisfiableConceptItem(satTestedItem);

				} else {
					CTaxonomy *tax = optKPSetClassificationItem->getTaxonomy();
					if (topItem) {
						tax->setConceptSatisfiable(satisfiableConcept,false);
					} else {
						tax->getBottomHierarchyNode()->addEquivalentConcept(satisfiableConcept);
						tax->updateNodeEquivalences(tax->getBottomHierarchyNode());
					}
				}

				QList<COptimizedKPSetRoleTestingItem*>* nextItemList = optKPSetClassificationItem->getNextSatisfiableTestingItemList();
				QSet<COptimizedKPSetRoleTestingItem*>* nextCandItemSet = optKPSetClassificationItem->getNextCandidateSatisfiableTestingItemSet();
				QSet<COptimizedKPSetRoleTestingItem*>* remainingCandItemSet = optKPSetClassificationItem->getRemainingCandidateSatisfiableTestingItemSet();


				QList<COptimizedKPSetRoleTestingItem*>* subSumItemList = satTestedItem->getSubsumingConceptItemList();
				QList<COptimizedKPSetRoleTestingItem*>* succItemList = satTestedItem->getSuccessorItemList();
				for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator it = succItemList->constBegin(), itEnd = succItemList->constEnd(); it != itEnd; ++it) {
					COptimizedKPSetRoleTestingItem* succItem = *it;
					if (!isSatis) {
						succItem->setResultUnsatisfiableDerivated(true);
					} else {
						for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator itSubsum = subSumItemList->constBegin(), itSubsumEnd = subSumItemList->constEnd(); itSubsum != itSubsumEnd; ++itSubsum) {
							COptimizedKPSetRoleTestingItem* subusmerItem = *itSubsum;
							if (succItem != subusmerItem) {
								succItem->addSubsumingConceptItem(subusmerItem);
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
				CClassificationWorkItem *workItem = testResult->getClassificationWorkItem();
				++mRecievedCallbackCount;


				COntologyClassificationItem *ontClassItem = workOntItemHash.value(workItem);
				if (ontClassItem) {

					QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *> *workHash = ontClassItem->getWorkItemHash();

					CTaxonomy *taxonomy = ontClassItem->getTaxonomy();

					if (workItem) {
						ontClassItem->decCurrentCalculatingCount();
						if (workItem->isTestValid()) {

							if (testResult->hasCalculationError()) {
								LOG(ERROR,getLogDomain(),logTr("Error in computation, classification for ontology '%1' failed.").arg(ontClassItem->getOntology()->getTerminologyName()),getLogObject());
								ontClassItem->setTaxonomyConstructionFailed();

							} else {

								if (workItem->isConceptSatisfiableTest()) {
									interpreteSatisfiableResult(ontClassItem,workItem->getSatisfiableTestedConcept(),testSat);
								} else if (workItem->isConceptSubsumptionTest()) {
									interpreteSubsumptionResult(ontClassItem,workItem->getSubsumedTestedConcept(),workItem->getSubsumerTestedConcept(),!testSat);
								}
							}
						}
						QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *>::iterator itWorkItem = workHash->find(satCalcJob);
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
						ontClassItem->reuseCalculationStatisticsCollection(statisticCollection);
					}

				} 


				return true;
			}


			bool COptimizedKPSetRoleSubsumptionClassifierThread::finishOntologyClassification(COntologyClassificationItem *ontClassItem) {

				if (!ontClassItem->isTaxonomyConstructed() && !ontClassItem->isTaxonomyConstructionFailed()) {

					CTaxonomy *taxonomy = ontClassItem->getTaxonomy();
					CConcreteOntology *ontology = ontClassItem->getOntology();
					LOG(INFO,getLogDomain(),logTr("Starting Transitive Reduction for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

					// build taxonomy

					COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetRoleOntologyClassificationItem *)ontClassItem;
					QList<COptimizedKPSetRoleTestingItem*>* satItemList = optKPSetClassificationItem->getSatisfiableConceptItemList();

					CTaxonomy *tax = optKPSetClassificationItem->getTaxonomy();

					QList<COptimizedKPSetRoleTestingItem*> itemList;

					COptimizedKPSetRoleTestingItem* topItem = optKPSetClassificationItem->getTopConceptSatisfiableTestItem();
					CHierarchyNode* topHierNode = tax->getTopHierarchyNode();
					CHierarchyNode* bottomHierNode = tax->getBottomHierarchyNode();



					cint64 topSubsumingItemCount = topItem->getSubsumingConceptItemCount();
					topItem->setSatisfiableConceptHierarchyNode(topHierNode);
					if (topSubsumingItemCount) {
						QList<COptimizedKPSetRoleTestingItem*>* subsumingConceptItemList = topItem->sortSubsumingConceptItemList();
						for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
							COptimizedKPSetRoleTestingItem* subsumedConceptItem = *subsumedIt;
							// mark as equivalent
							subsumedConceptItem->setEquivalentItem(true);
							topHierNode->addEquivalentConcept(subsumedConceptItem->getTestingConcept());
							tax->updateNodeEquivalences(topHierNode);
						}
					}

					LOG(INFO,getLogDomain(),logTr("Sorting classes for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

					for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator it = satItemList->constBegin(), itEnd = satItemList->constEnd(); it != itEnd; ++it) {
						COptimizedKPSetRoleTestingItem* item = *it;

						if (!item->isEquivalentItem() && item != topItem) {
							CHierarchyNode* itemNode = tax->getHierarchyNode(item->getTestingConcept(),true);
							item->setSatisfiableConceptHierarchyNode(itemNode);
							cint64 itemSubsumingCount = item->getSubsumingConceptItemCount();
							itemList.append(item);
							if (itemSubsumingCount > topSubsumingItemCount) {
								QList<COptimizedKPSetRoleTestingItem*>* subsumingConceptItemList = item->sortSubsumingConceptItemList();
								for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
									COptimizedKPSetRoleTestingItem* subsumedConceptItem = *subsumedIt;
									cint64 subsumedItemSubsumingCount = subsumedConceptItem->getSubsumingConceptItemCount();
									if (subsumedItemSubsumingCount == itemSubsumingCount) {
										// mark as equivalent
										subsumedConceptItem->setEquivalentItem(true);
										itemNode->addEquivalentConcept(subsumedConceptItem->getTestingConcept());
										tax->updateNodeEquivalences(itemNode);
									} else {
										break;
									}
								}
							}
						}
					}

					LOG(INFO,getLogDomain(),logTr("Transitive Reduction for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

					for (QList<COptimizedKPSetRoleTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedKPSetRoleTestingItem* item = *it;

						if (!item->isEquivalentItem() && item != topItem) {
							cint64 itemSubsumingCount = item->getSubsumingConceptItemCount();
							CHierarchyNode* itemHierNode = item->getSatisfiableConceptHierarchyNode();

							QList<COptimizedKPSetRoleTestingItem*>* subsumingConceptItemList = item->getSubsumingConceptItemList();
							QList<COptimizedKPSetRoleTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd();

							cint64 eqCount = 0;
							cint64 remainingSubsumingCount = itemSubsumingCount;
							// remove direct equivalences
							while (subsumedIt != subsumedItEnd) {
								COptimizedKPSetRoleTestingItem* subsumedConceptItem = *subsumedIt;
								cint64 subsumedItemSubsumingCount = subsumedConceptItem->getSubsumingConceptItemCount();
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
										CHierarchyNode* subsumedHierNode = subsumedConceptItem->getSatisfiableConceptHierarchyNode();
										itemHierNode->makeParent(subsumedHierNode);
										parentItemList.append(subsumedConceptItem);
										remainingSubsumingCount -= subsumedConceptItem->getSubsumingConceptItemCount()+1;
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
												if (parentItem->getSubsumingConceptItemSet()->contains(subsumedConceptItem)) {
													predOfOnePrevParent = true;
												}
											}
										}
										subsumedConceptItem->setPredecessorItem(true);
										CHierarchyNode* subsumedHierNode = subsumedConceptItem->getSatisfiableConceptHierarchyNode();
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
							CHierarchyNode* itemHierNode = item->getSatisfiableConceptHierarchyNode();
							bottomHierNode->makeParent(itemHierNode);
						}
					}

					LOG(INFO,getLogDomain(),logTr("Collect statistics for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());


#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
					statUpdateMutex.lock();
					statusString = QString("Finished ontology classification");
					statUpdateMutex.unlock();
#endif


					CClassification* classification = ontology->getClassification();
					if (classification) {
						CClassConceptClassification* classConClassification = ontClassItem->getClassConceptClassification();
						CClassificationStatisticsCollectionStrings* classifStatCollStrings = ontClassItem->getClassificationStatisticsCollectionStrings();
						CClassifierStatistics* classifierStats = ontClassItem->getClassifierStatistics();
						classifStatCollStrings->addProcessingStatistics("role-classification-total-satisfiable-test-count",classifierStats->getTotalSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-tested-count",classifierStats->getTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-pseudo-model-merged-tested-count",classifierStats->getPseudoModelMergedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-derivate-tested-count",classifierStats->getDerivatedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-calculated-tested-count",classifierStats->getCalculatedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-pruned-tested-count",classifierStats->getPrunedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-satisfiable-told-tested-count",classifierStats->getToldSatisfiableCount()-classifierStats->getCalculatedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-total-subsumption-test-count",classifierStats->getTotalSatisfiableCount()*(classifierStats->getTotalSatisfiableCount()-1));
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-tested-count",classifierStats->getTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-pseudo-model-merged-tested-count",classifierStats->getPseudoModelMergedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-subclass-tested-count",classifierStats->getSubclassTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-subclass-identifier-occur-tested-count",classifierStats->getSubclassIdentifierOccurTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-derivate-tested-count",classifierStats->getDerivatedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-calculated-tested-count",classifierStats->getCalculatedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-pruned-tested-count",classifierStats->getPrunedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("role-classification-subsumption-told-tested-count",classifierStats->getToldSubsumptionCount()-classifierStats->getCalculatedTestedSubsumptionCount());
						classConClassification->setClassConceptTaxonomy(taxonomy);
						classConClassification->setClassificationStatistics(classifStatCollStrings);
						classification->setClassConceptClassification(classConClassification);
					}
					ontology->setConceptTaxonomy(taxonomy);

					ontClassItem->setGoneOutRemainingTests(false);
					taxonomy->setTaxonomyComplete(true);

					--mClassificationCount;
					LOG(INFO,getLogDomain(),logTr("Ontology '%1' classified.").arg(ontology->getTerminologyName()),getLogObject());
					LOG(INFO,getLogDomain(),logTr("Used %1 satisfiable tests and %3 calculated subsumption tests.").arg(ontClassItem->getCalcedSatisfiableCount()).arg(ontClassItem->getClassifierStatistics()->getCalculatedTestedSubsumptionCount()),getLogObject());
					LOG(INFO,getLogDomain(),logTr("Confirmed %1 of %2 possible subsumer concepts.").arg(optKPSetClassificationItem->getTruePossibleSubsumerCount()).arg(optKPSetClassificationItem->getPossibleSubsumerCount()),getLogObject());
					LOG(INFO,getLogDomain(),logTr("Confirmed %1 and invalidated %2 possible subsumer concepts with calculation.").arg(optKPSetClassificationItem->getCalculatedTruePossibleSubsumerCount()).arg(optKPSetClassificationItem->getFalsePossibleSubsumerCount()),getLogObject());
				}
				ontClassItem->submitTaxonomyConstructed();
				ontClassItem->doClassifiedCallback();
				return true;
			}



		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
