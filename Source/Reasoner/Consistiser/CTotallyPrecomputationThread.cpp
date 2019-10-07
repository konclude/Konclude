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

#include "CTotallyPrecomputationThread.h"



#ifndef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
	#define KONCLUDE_PRECOMPUTATION_SATURATION_STATISTIC_OUTPUT
#endif


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CTotallyPrecomputationThread::CTotallyPrecomputationThread(CReasonerManager *reasonerManager) : CPrecomputationThread(reasonerManager),CLogIdentifier("::Konclude::Reasoner::Kernel::Precomputator",this) {
				mReuseCompletionGraphCacheWriter = nullptr;
				CCompletionGraphCache* complGraphCache = reasonerManager->getCompletionGraphCache();
				if (complGraphCache) {
					CReuseCompletionGraphCache* reuseComplGraphCache = dynamic_cast<CReuseCompletionGraphCache*>(complGraphCache);
					if (reuseComplGraphCache) {
						mReuseCompletionGraphCacheWriter = reuseComplGraphCache->createCacheWriter();
					}
				}
				mBackendAssocCache = nullptr;
				CBackendCache* backendCache = reasonerManager->getBackendAssociationCache();
				if (backendCache) {
					mBackendAssocCache = dynamic_cast<CBackendRepresentativeMemoryCache*>(backendCache);
				}
			}


			CTotallyPrecomputationThread::~CTotallyPrecomputationThread() {
			}


			COntologyPrecomputationItem* CTotallyPrecomputationThread::initializeOntologyPrecomputionItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				CTotallyOntologyPrecomputationItem* item = new CTotallyOntologyPrecomputationItem();
				item->initTotallyPrecomputationItem(ontology,config);


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
					cint64 multiplicator = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Precomputation.TotalPrecomputor.MultipliedUnitsParallelCalculationCount",1,&mulConfigErrorFlag);
					mConfMaxTestParallelCount = processorCount*multiplicator;
					bool maxConfigErrorFlag = false;
					cint64 maxParallel = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Precomputation.TotalPrecomputor.MaximumParallelCalculationCount",1,&maxConfigErrorFlag);
					if (!maxConfigErrorFlag) {
						if (!mulConfigErrorFlag) {
							mConfMaxTestParallelCount = qMin(mConfMaxTestParallelCount,maxParallel);
						} else {
							mConfMaxTestParallelCount = maxParallel;
						}
					}

				} else {
					mConfMaxTestParallelCount = 1;
				}

				mIndividualSaturationCount = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Precomputation.TotalPrecomputor.IndividualsSaturationSize",1000);
				mConfForceFullCompletionGraphConstruction = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Precomputation.ForceFullCompletionGraphConstruction",false);


				mConfConditionalFullCompletionGraphConstruction = CConfigDataReader::readConfigBoolean(config, "Konclude.Calculation.Precomputation.ConditionalFullCompletionGraphConstruction", false);

				mConfFullCGCExclusionConditionMaximumIndividualLimit = CConfigDataReader::readConfigInteger(config, "Konclude.Calculation.Precomputation.FullCompletionGraphConstruction.ExclusionCondition.MaximumIndividualLimit", 300000);
				mConfFullCGCSuggestionConditionMaximumIndividualLimit = CConfigDataReader::readConfigInteger(config, "Konclude.Calculation.Precomputation.FullCompletionGraphConstruction.SuggestionCondition.MaximumIndividualLimit", 10000);
				
				mConfFullCGCExclusionConditionMaximumIndividualConceptRatio = CConfigDataReader::readConfigInteger(config, "Konclude.Calculation.Precomputation.FullCompletionGraphConstruction.ExclusionCondition.MaximumIndividualConceptRatio", 5000) / 100;
				mConfFullCGCSuggestionConditionMaximumIndividualConceptRatio = CConfigDataReader::readConfigInteger(config, "Konclude.Calculation.Precomputation.FullCompletionGraphConstruction.SuggestionCondition.MaximumIndividualConceptRatio", 500) / 100;


				return item;
			}



			bool CTotallyPrecomputationThread::createNextTest() {


				COntologyPrecomputationItem* loopOntPreCompItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !mProcessingOntItemList.isEmpty()) {
					COntologyPrecomputationItem* ontPreCompItem = mProcessingOntItemList.first();

					if (ontPreCompItem == loopOntPreCompItem) {
						// don't run into infinite loop without doing something
						break;
					}

					CTotallyOntologyPrecomputationItem* totallyPreCompItem = (CTotallyOntologyPrecomputationItem*)ontPreCompItem;
					

					bool failDebug = false;
					if (!workTestCreated && totallyPreCompItem->isConsistenceStepRequired()) {
						if (!totallyPreCompItem->isConsistenceStepFinished()) {
							if (totallyPreCompItem->areConsistenceStepProcessingRequirementSatisfied()) {

								if (!totallyPreCompItem->hasConceptSaturationPrecomputationCreated()) {
									LOG(INFO,getLogDomain(),logTr("Preparing concept saturation."),this);

									CConcreteOntology* ontology = totallyPreCompItem->getOntology();
									totallyPreCompItem->setSaturationStepRunning(true);
									createSaturationConstructionJob(totallyPreCompItem);
									totallyPreCompItem->setSaturationComputationRunning(true);
									totallyPreCompItem->setConceptSaturationPrecomputationCreated(true);

									addIdentifiedRemainingConsistencyRequiredConcepts(totallyPreCompItem);
									addRequiredSaturationIndividuals(totallyPreCompItem);

									bool fullCompletionGraphConstruction = false;
									bool forceCompletionGraphConstruction = false;
									if (mConfForceFullCompletionGraphConstruction || !totallyPreCompItem->hasIndividualsSaturated()) {
										forceCompletionGraphConstruction = fullCompletionGraphConstruction = true;
									}
									if (!fullCompletionGraphConstruction) {
										forceCompletionGraphConstruction = fullCompletionGraphConstruction |= totallyPreCompItem->getOntology()->getStructureSummary()->hasGroundingOccurrence();
									}

									if (!mConfForceFullCompletionGraphConstruction && mConfConditionalFullCompletionGraphConstruction) {
										cint64 indiCount = ontology->getABox()->getNextIndividualId();
										cint64 conceptCount = ontology->getTBox()->getConceptCount();
										double indiConRatio = (double)indiCount / (double)conceptCount;

										// check whether no exclusion limit/condition is satisfied
										if (indiCount < mConfFullCGCExclusionConditionMaximumIndividualLimit && indiConRatio < mConfFullCGCExclusionConditionMaximumIndividualConceptRatio) {
											// check whether at least one suggestion condition is satisfied
											if (indiCount < mConfFullCGCSuggestionConditionMaximumIndividualLimit || indiConRatio < mConfFullCGCSuggestionConditionMaximumIndividualConceptRatio) {
												fullCompletionGraphConstruction = true;
												LOG(INFO, getLogDomain(), logTr("Enabling full completion graph construction for %1 individuals with %2 individuals to concepts ratio.").arg(indiCount).arg(indiConRatio), this);
											}
										}

									}

									totallyPreCompItem->setFullCompletionGraphConstruction(fullCompletionGraphConstruction);
									totallyPreCompItem->setForceCompletionGraphConstruction(forceCompletionGraphConstruction);

								}


								if (totallyPreCompItem->hasRemainingConsistencyRequiredSaturationConcepts() && !totallyPreCompItem->isSaturationComputationRunning()) {
									if (saturateRemainingConsistencyRequiredConcepts(totallyPreCompItem)) {
										totallyPreCompItem->setSaturationComputationRunning(true);
										workTestCreated = true;
									}
								}


								if (totallyPreCompItem->hasRemainingRequiredSaturationIndividuals() && !totallyPreCompItem->hasClashedSaturationIndividuals() && (!totallyPreCompItem->isSaturationComputationRunning() || totallyPreCompItem->hasIndividualSaturationRunning())) {
									if (!isAllAssertionIndividualSaturationSufficient(totallyPreCompItem) || totallyPreCompItem->isIndividualStepRequired() || totallyPreCompItem->isFullCompletionGraphConstruction() || totallyPreCompItem->isForceCompletionGraphConstruction()) {
										if (saturateRemainingRequiredSaturationIndividuals(totallyPreCompItem)) {
											totallyPreCompItem->incIndividualSaturationRunningCount();
											totallyPreCompItem->setSaturationComputationRunning(true);
											totallyPreCompItem->setIndividualStepRunning(true);
											workTestCreated = true;
										}
									}
								}



								if (!totallyPreCompItem->isSaturationComputationRunning()) {
									if (totallyPreCompItem->failAfterConsistencyConceptSaturation()) {
										totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
										totallyPreCompItem->getConsistencePrecomputationStep()->setStepFinished(true);
										failDebug |= true;
									} else {
										if (!isAllAssertionIndividualSaturationSufficient(totallyPreCompItem) || totallyPreCompItem->isIndividualStepRequired()) {
											if (!totallyPreCompItem->hasAllIndividualPrecomputationCreated() && !totallyPreCompItem->hasClashedSaturationIndividuals() && !totallyPreCompItem->hasIndividualPrecomputationClashed() && !totallyPreCompItem->isSaturationComputationRunning() && !totallyPreCompItem->hasIndividualSaturationRunning()) {
												if (createIndividualPrecomputationCheck(totallyPreCompItem)) {
													totallyPreCompItem->setIndividualStepRunning(true);
													totallyPreCompItem->setIndividualComputationRunning(true);
													totallyPreCompItem->incIndividualComputationRunningCount();
													workTestCreated = true;
												} else {
													if (totallyPreCompItem->hasAllIncompletelyHandledIndividualsRetrieved()) {
														totallyPreCompItem->setAllIndividualPrecomputationCreated(true);
														if (!totallyPreCompItem->isIndividualComputationRunning()) {
															totallyPreCompItem->setIndividualPrecomputationChecked(true);
														}
													}
												}
											}
										}
									}
								}


								if (!failDebug && !totallyPreCompItem->hasConsistenceCheckCreated() && !totallyPreCompItem->isSaturationComputationRunning() && !totallyPreCompItem->isIndividualComputationRunning()) {
									LOG(INFO,getLogDomain(),logTr("Precompute ontology consistency."),this);
									totallyPreCompItem->setConsistenceCheckCreated(true);
									workTestCreated = createConsistencePrecomputationCheck(totallyPreCompItem);
								}

								if (totallyPreCompItem->hasConsistenceCheched()) {
									LOG(INFO,getLogDomain(),logTr("Consistency computation completed."),this);
									totallyPreCompItem->getConsistencePrecomputationStep()->setStepFinished(true);

									COntologyProcessingStatistics* ontProcStats = totallyPreCompItem->getConsistencePrecomputationStep()->getProcessingStepData()->getProcessingStatistics(true);
									ontProcStats->setProcessingCountStatisticValue("consistency-checking-time",totallyPreCompItem->getInitializationTime()->elapsed());

									if (totallyPreCompItem->failAfterConsistencyChecking()) {
										totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
										failDebug |= true;
									} else {

										if (totallyPreCompItem->getOntology()->getConsistence()->isOntologyConsistent()) {
											totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
										} else {
											totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL | COntologyProcessingStatus::PSINCONSITENT);
										}
									}

								}

							} else {
								totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
								totallyPreCompItem->getConsistencePrecomputationStep()->setStepFinished(true);
							}
						} else {
							totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate();
						}
					}







					if (!workTestCreated && totallyPreCompItem->isIndividualStepRequired() && totallyPreCompItem->isConsistenceStepFinished()) {
						if (!totallyPreCompItem->isIndividualStepFinished()) {
							if (totallyPreCompItem->areIndividualStepProcessingRequirementSatisfied()) {

								if (totallyPreCompItem->hasRemainingRequiredSaturationIndividuals() && !totallyPreCompItem->hasClashedSaturationIndividuals() && (!totallyPreCompItem->isSaturationComputationRunning() || totallyPreCompItem->hasIndividualSaturationRunning())) {
									if (saturateRemainingRequiredSaturationIndividuals(totallyPreCompItem)) {
										totallyPreCompItem->incIndividualSaturationRunningCount();
										totallyPreCompItem->setSaturationComputationRunning(true);
										totallyPreCompItem->setIndividualStepRunning(true);
										workTestCreated = true;
									}
								}


								if (!totallyPreCompItem->hasAllIndividualPrecomputationCreated() && !totallyPreCompItem->hasClashedSaturationIndividuals() && !totallyPreCompItem->hasIndividualPrecomputationClashed() && !totallyPreCompItem->isSaturationComputationRunning() && !totallyPreCompItem->hasIndividualSaturationRunning()) {
									if (createIndividualPrecomputationCheck(totallyPreCompItem)) {
										totallyPreCompItem->setIndividualStepRunning(true);
										totallyPreCompItem->setIndividualComputationRunning(true);
										totallyPreCompItem->incIndividualComputationRunningCount(1);
									} else {
										if (totallyPreCompItem->hasAllIncompletelyHandledIndividualsRetrieved()) {
											totallyPreCompItem->setAllIndividualPrecomputationCreated(true);
											if (!totallyPreCompItem->isIndividualComputationRunning()) {
												totallyPreCompItem->setIndividualPrecomputationChecked(true);
											}
										}
									}
								}
								

								if ((totallyPreCompItem->hasIndividualPrecomputationChecked() || (totallyPreCompItem->hasClashedSaturationIndividuals() || totallyPreCompItem->hasIndividualPrecomputationClashed())) && !totallyPreCompItem->isIndividualComputationRunning()) {
									totallyPreCompItem->setIndividualStepRunning(false);
									totallyPreCompItem->getIndividualPrecomputationStep()->setStepFinished(true);
									if (totallyPreCompItem->hasClashedSaturationIndividuals() || totallyPreCompItem->hasIndividualPrecomputationClashed()) {
										totallyPreCompItem->getIndividualPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
									} else {
										totallyPreCompItem->getIndividualPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
									}
								}

							} else {
								totallyPreCompItem->setIndividualStepRunning(false);
								totallyPreCompItem->getIndividualPrecomputationStep()->setStepFinished(true);
								totallyPreCompItem->getIndividualPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							totallyPreCompItem->getIndividualPrecomputationStep()->submitRequirementsUpdate();
						}
					}








					if (!workTestCreated && totallyPreCompItem->isSaturationStepRequired() && totallyPreCompItem->isConsistenceStepFinished() && !totallyPreCompItem->hasIndividualSaturationRunning() && !totallyPreCompItem->isIndividualStepRunning()) {
						if (!totallyPreCompItem->isSaturationStepFinished()) {
							if (totallyPreCompItem->areSaturationStepProcessingRequirementSatisfied()) {


								if (totallyPreCompItem->hasRemainingRequiredSaturationConcepts() && !totallyPreCompItem->isSaturationComputationRunning()) {
									if (saturateRemainingRequiredItems(totallyPreCompItem)) {
										totallyPreCompItem->setSaturationComputationRunning(true);
										workTestCreated = true;
									}
								}


								if (totallyPreCompItem->hasConceptSaturationPrecomputationChecked() && !totallyPreCompItem->isSaturationComputationRunning()) {
									totallyPreCompItem->setSaturationStepRunning(false);
									if (totallyPreCompItem->requiresAllConceptsSaturation()) {
										LOG(INFO,getLogDomain(),logTr("Concept saturation finished."),this);
										totallyPreCompItem->getSaturationPrecomputationStep()->setStepFinished(true);
									}

									CPrecomputation* precomputation = totallyPreCompItem->getPrecomputation();
									CSaturationData* saturationData = totallyPreCompItem->getSaturationData();


									if (totallyPreCompItem->failAfterConceptSaturation()) {
										totallyPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
										failDebug |= true;
									} else {

										if (!saturationData) {
											LOG(ERROR,getLogDomain(),logTr("Unknow error in computation, tableau saturation for ontology '%1' failed.").arg(ontPreCompItem->getOntology()->getTerminologyName()),getLogObject());
											totallyPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
										} else {

#ifdef KONCLUDE_PRECOMPUTATION_SATURATION_STATISTIC_OUTPUT
											logSaturationInfos(totallyPreCompItem);
#endif
											extractCommonDisjunctConceptsFromPrecomputedSaturation(totallyPreCompItem);

											totallyPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
										}
									}
								}

							} else {
								totallyPreCompItem->setSaturationStepRunning(false);
								totallyPreCompItem->getSaturationPrecomputationStep()->setStepFinished(true);
								totallyPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							totallyPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate();
						}
					}






					if (!workTestCreated && totallyPreCompItem->isOccurrenceStatisticsStepRequired() && totallyPreCompItem->isConsistenceStepFinished() && !totallyPreCompItem->isSaturationStepRunning() && !totallyPreCompItem->isIndividualComputationRunning()) {
						if (!totallyPreCompItem->isOccurrenceStatisticsStepFinished()) {
							if (totallyPreCompItem->areOccurrenceStatisticsStepProcessingRequirementSatisfied()) {

								if (!totallyPreCompItem->getCalculationConfiguration()->isOccurrenceStatisticsCollectionActivated()) {
									totallyPreCompItem->setSaturationOccurrenceStatisticsCollectingInitialized(true);
									totallyPreCompItem->setSaturationOccurrenceStatisticsCollected(true);
								}

								if (!totallyPreCompItem->hasSaturationOccurrenceStatisticsCollectingInitialized()) {
									// determine concept and role occurrences in saturation graph
									LOG(INFO, getLogDomain(), logTr("Collecting remaining occurrence statistics from saturation for ontology '%1'.").arg(ontPreCompItem->getOntology()->getTerminologyName()), getLogObject());
									createOccurrenceStatisticsCollectingSaturationProcessingJob(totallyPreCompItem);
									totallyPreCompItem->setSaturationOccurrenceStatisticsCollectingInitialized(true);
								}

								if (totallyPreCompItem->hasSaturationOccurrenceStatisticsCollected()) {
									totallyPreCompItem->getOccurrenceStatisticsPrecomputationStep()->setStepFinished(true);
									totallyPreCompItem->getOccurrenceStatisticsPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
								}
							} else {
								totallyPreCompItem->getOccurrenceStatisticsPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
								totallyPreCompItem->getOccurrenceStatisticsPrecomputationStep()->setStepFinished(true);
							}
						} else {
							totallyPreCompItem->getOccurrenceStatisticsPrecomputationStep()->submitRequirementsUpdate();
						}
					}








					if (!workTestCreated && totallyPreCompItem->isCycleStepRequired() && totallyPreCompItem->isConsistenceStepFinished() && !totallyPreCompItem->isSaturationStepRunning() && !totallyPreCompItem->isIndividualComputationRunning()) {
						if (!totallyPreCompItem->isCycleStepFinished()) {
							if (totallyPreCompItem->areCycleStepProcessingRequirementSatisfied()) {
								if (!totallyPreCompItem->hasConceptCyclePrecomputationChecked()) {
									if (!totallyPreCompItem->hasConceptCyclePrecomputationInitialized()) {
										CConcreteOntology* ontology = totallyPreCompItem->getOntology();
										COntologyCoreConceptCyclesData* coreConceptCycles = ontology->getCoreConceptCyclesData();
										bool hasConceptCycles = false;
										if (coreConceptCycles) {
											CBOXLIST<CConceptCycleData*>* coreConceptCycleList = coreConceptCycles->getConceptCycleList();
											for (CBOXLIST<CConceptCycleData*>::const_iterator it = coreConceptCycleList->constBegin(), itEnd = coreConceptCycleList->constEnd(); it != itEnd; ++it) {
												CConceptCycleData* conCycleData = *it;
												if (mReuseCompletionGraphCacheWriter) {
													totallyPreCompItem->addRemainingConceptCyclePrecomputation(conCycleData);
													hasConceptCycles = true;
												}
											}
										}

										cint64 conceptCount = totallyPreCompItem->getOntology()->getTBox()->getConceptCount();
										cint64 minSizeCount = qMax<cint64>(conceptCount*0.001,50);
										totallyPreCompItem->setMinConceptCycleTestSize(minSizeCount);
										totallyPreCompItem->setConceptCyclePrecomputationInitialized(true);
										if (!hasConceptCycles) {
											totallyPreCompItem->setConceptCyclePrecomputationChecked(true);
										}
									}

									if (totallyPreCompItem->hasRemainingConceptCyclePrecomputation()) {
										CConcreteOntology* ontology = totallyPreCompItem->getOntology();
										COntologyCoreConceptCyclesData* coreConceptCycles = ontology->getCoreConceptCyclesData();
										QList<CConceptCycleData*>* remConCycPreTestList = totallyPreCompItem->getRemainingConceptCyclePrecomputationList();
										while (!workTestCreated && !remConCycPreTestList->isEmpty()) {
											CConceptCycleData* conceptCycleData = remConCycPreTestList->takeFirst();
											if (conceptCycleData->hasNominalConnection()) {
												workTestCreated = createConceptCyclePrecomputation(conceptCycleData,totallyPreCompItem);
												if (workTestCreated) {
													LOG(INFO,getLogDomain(),logTr("Precompute core concept cycle '%1' of '%2' with '%3' concepts.").arg(conceptCycleData->getCycleID()).arg(coreConceptCycles->getConceptCycleCount()).arg(conceptCycleData->getConceptCount()),this);
												}
											}
										}
									}

									if (!workTestCreated) {
										if (!totallyPreCompItem->hasRemainingConceptCyclePrecomputation() && !totallyPreCompItem->hasCurrentPrecomputationTesting()) {
											totallyPreCompItem->setConceptCyclePrecomputationChecked(true);
										}
									}
								}

								if (!workTestCreated) {
									if (totallyPreCompItem->hasConceptCyclePrecomputationChecked()) {
										totallyPreCompItem->getCyclePrecomputationStep()->setStepFinished(true);
										totallyPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
									}
								}
							} else {
								totallyPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
								totallyPreCompItem->getCyclePrecomputationStep()->setStepFinished(true);
							}
						} else {
							totallyPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate();
						}
					}


					if (!workTestCreated) {
						if (!totallyPreCompItem->hasRemainingProcessingRequirements()) {
							finishOntologyPrecomputation(totallyPreCompItem);
							mProcessingOntItemList.removeFirst();
						} else {
							mProcessingOntItemList.removeFirst();
							mProcessingOntItemList.append(totallyPreCompItem);
							loopOntPreCompItem = ontPreCompItem;
						}
					}
				}
				return workTestCreated;
			}






			void CTotallyPrecomputationThread::extractCommonDisjunctConceptsFromPrecomputedSaturation(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				LOG(INFO,getLogDomain(),logTr("Extract common disjunct concepts."),this);
				CSaturationCommonDisjunctConceptsExtractor satCommonDisjConExt;
				satCommonDisjConExt.extractCommonDisjunctConcepts(totallyPreCompItem->getOntology(),totallyPreCompItem->getSaturationData(),totallyPreCompItem->getSaturatedDisjunctionSet());
			}



			bool CTotallyPrecomputationThread::isAllAssertionIndividualSaturationSufficient(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				if (!totallyPreCompItem->hasAllAssertionIndividualSufficientSaturationChecked()) {
					CIndividual* allAssertionIndi = totallyPreCompItem->getAllAssertionIndividual();
					bool saturationSufficient = false;

					if (allAssertionIndi) {
						CSaturationData* saturationData = totallyPreCompItem->getSaturationData();
						CSaturationTaskData* satTaskData = (CSaturationTaskData*)saturationData;
						if (satTaskData) {
							CSatisfiableCalculationTask* satCalcTask = satTaskData->getSaturationTask();
							if (satCalcTask) {
								CProcessingDataBox* procDataBox = satCalcTask->getProcessingDataBox();
								if (procDataBox) {
									CIndividualSaturationProcessNodeVector* satIndiNodeVec = procDataBox->getIndividualSaturationProcessNodeVector(false);
									if (satIndiNodeVec) {
										CIndividualSaturationProcessNode* allAssertionIndiNode = satIndiNodeVec->getData(allAssertionIndi->getIndividualID());
										if (allAssertionIndiNode) {
											CIndividualSaturationProcessNodeStatusFlags* indFlags = allAssertionIndiNode->getIndirectStatusFlags();
											CIndividualSaturationProcessNodeStatusFlags* dirFlags = allAssertionIndiNode->getDirectStatusFlags();
											if (!indFlags->hasClashedFlag() && indFlags->hasCompletedFlag() && dirFlags->hasCompletedFlag() && !indFlags->hasInsufficientFlag()) {
												saturationSufficient = true;
											}
										}
									}
								}
							}
						}
					}
					if (saturationSufficient) {
						totallyPreCompItem->setAllAssertionIndividualSaturationSufficient(true);
					}
					totallyPreCompItem->setAllAssertionIndividualSufficientSaturationChecked(true);
				}
				return totallyPreCompItem->isAllAssertionIndividualSaturationSufficient();
			}


			void CTotallyPrecomputationThread::logSaturationInfos(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CConcreteOntology* ontology = totallyPreCompItem->getOntology();
				CSaturationData* saturationData = totallyPreCompItem->getSaturationData();
				CSaturationTaskData* satTaskData = (CSaturationTaskData*)saturationData;
				if (satTaskData) {
					CSatisfiableCalculationTask* satCalcTask = satTaskData->getSaturationTask();
					if (satCalcTask) {
						CProcessingDataBox* procDataBox = satCalcTask->getProcessingDataBox();
						if (procDataBox) {
							CIndividualSaturationProcessNodeVector* satIndiNodeVec = procDataBox->getIndividualSaturationProcessNodeVector(false);
							if (satIndiNodeVec) {
								cint64 saturatedCount = 0;
								cint64 insufficientCount = 0;
								cint64 criticalCount = 0;
								cint64 clashedCount = 0;
								cint64 indiCount = satIndiNodeVec->getItemCount();
								for (cint64 indiID = 0; indiID < indiCount; ++indiID) {
									CIndividualSaturationProcessNode* indiNode = satIndiNodeVec->getData(indiID);
									if (indiNode && indiNode->isCompleted()) {
										++saturatedCount;
										CIndividualSaturationProcessNodeStatusFlags* flags = indiNode->getIndirectStatusFlags();
										if (flags->hasClashedFlag()) {
											++clashedCount;
										}
										if (flags->hasInsufficientFlag()) {
											++insufficientCount;
										}
										if (flags->hasCriticalFlag()) {
											++criticalCount;
										}
									}
								}
								LOG(INFO,getLogDomain(),logTr("Saturated %1 individual nodes, thereof %2 with clashes, %3 critical and %4 insufficient.").arg(saturatedCount).arg(clashedCount).arg(criticalCount).arg(insufficientCount),this);
							}
						}
					}
				}
			}


			bool CTotallyPrecomputationThread::createConceptCyclePrecomputation(CConceptCycleData* conceptCycleData, CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CConcreteOntology* onto = totallyPreCompItem->getOntology();
				CConcept* repCycleClassConcept = conceptCycleData->getRepresentativeCycleClass();

				if (repCycleClassConcept) {

					CSaturationConceptDataItem* conSatItem = totallyPreCompItem->getSaturationConceptDataItem(repCycleClassConcept,false,false);
					if (conSatItem) {
						CIndividualSaturationProcessNode* satNode = (CIndividualSaturationProcessNode*)conSatItem->getIndividualProcessNodeForConcept();
						if (!satNode->getIndirectStatusFlags()->hasInsufficientFlag()) {
							return false;
						}
					}

					if (conceptCycleData->getConceptCount() >= totallyPreCompItem->getMinConceptCycleTestSize()) {

						CConceptCyclePrecomputationTestingItem* conCycPrecTestItem = new CConceptCyclePrecomputationTestingItem(conceptCycleData,totallyPreCompItem);

						CSatisfiableCalculationJobGenerator satCalcJobGen(onto);
						CSatisfiableCalculationJob* satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(repCycleClassConcept);
						CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
						satCalcJob->setCalculationConfiguration(calcConfig);
						satCalcJob->setSatisfiableTaskPreyingAdapter(conCycPrecTestItem->getTaskPreyingAdapter());

						processCalculationJob(satCalcJob,totallyPreCompItem,conCycPrecTestItem);
						return true;
					}
				}
				return false;
			}





			QList<CIndividualReference> CTotallyPrecomputationThread::getIndividualComputationList(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				QList<CIndividualReference> indiList;
				CConcreteOntology* onto = totallyPreCompItem->getOntology();

				CIndividualVector* indiVec = onto->getABox()->getIndividualVector(false);
				QSet<CIndividual*>* activeIndiSet = onto->getABox()->getActiveIndividualSet(false);
				cint64 indiCount = 0;
				if (indiVec) {
					indiCount = indiVec->getItemCount();
				}

				bool allIndiProRequired = true;
				QSet<CIndividualReference>* procReqIndiSet = procReqIndiSet = totallyPreCompItem->getIncompletelyHandledIndividualSet();
				cint64 maxTriplesIndexedIndiId = -1;
				cint64 maxABoxIndiId = 0;
				COntologyTriplesAssertionsAccessor* triplesAccessor = totallyPreCompItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (triplesAccessor) {
					maxTriplesIndexedIndiId = totallyPreCompItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor()->getMaxIndexedIndividualId();
				}
				if (indiVec) {
					cint64 indiCount = indiVec->getItemCount();
					for (cint64 idx = 0; idx < indiCount; ++idx) {
						CIndividual* indi = indiVec->getData(idx);
						if (allIndiProRequired || procReqIndiSet && procReqIndiSet->contains(CIndividualReference(idx))) {
							if (indi && activeIndiSet->contains(indi)) {
								indiList.append(CIndividualReference(indi));
							}
							else if (idx <= maxTriplesIndexedIndiId) {
								indiList.append(CIndividualReference(idx));
							}
						}
						maxABoxIndiId = qMax(idx, maxABoxIndiId);
					}
				}
				for (cint64 idx = maxABoxIndiId + 1; idx <= maxTriplesIndexedIndiId; ++idx) {
					if (allIndiProRequired || procReqIndiSet && procReqIndiSet->contains(CIndividualReference(idx))) {
						indiList.append(CIndividualReference(idx));
					}
				}

				return indiList;
			}


			bool CTotallyPrecomputationThread::decrementUsageFromIndividualPrecomputationCoordinationHash(CIndividualPrecomputationCoordinationHash* indiPrecompCoordHash, CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				indiPrecompCoordHash->detUsageCount();
				if (indiPrecompCoordHash->getUsageCount() == 0) {
					for (CIndividualPrecomputationCoordinationHash::iterator it = indiPrecompCoordHash->begin(), itEnd = indiPrecompCoordHash->end(); it != itEnd; ++it) {
						CIndividualPrecomputationCoordinationHashData*& data = it.value();
						data->decUsageCount();
						if (data->getUsageCount() == 0) {
							//delete data;
							data = nullptr;
						}
					}
					//delete indiPrecompCoordHash;
					return true;
				}
				return false;
			}



			QList<CIndividualReference> CTotallyPrecomputationThread::getNextIndividualComputationPartList(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				QList<CIndividualReference> indiList;
				QSet<CIndividualReference>* procReqIndiSet = procReqIndiSet = totallyPreCompItem->getIncompletelyHandledIndividualSet();
				bool indisRetrieved = true;
				bool newIndisRetrieved = true;
				while (indiList.isEmpty() && !totallyPreCompItem->hasAllIncompletelyHandledIndividualsRetrieved() && indisRetrieved && newIndisRetrieved) {
					indisRetrieved = false;
					newIndisRetrieved = false;
					if (procReqIndiSet->isEmpty() && !totallyPreCompItem->hasAllIncompletelyHandledIndividualsRetrieved()) {
						cint64 limit = totallyPreCompItem->getCurrentIncompletelyHandledIndividualRetrievalLimit();
						mBackendAssocCache->getIncompletlyAssociationCachedIndividuals(totallyPreCompItem->getOntology()->getOntologyID(), procReqIndiSet, limit);
						if (procReqIndiSet->size() > 0) {
							indisRetrieved = true;
							LOG(INFO, getLogDomain(), logTr("Retrieved %1 more insufficiently handled individuals from representative cache, scheduling processing.").arg(procReqIndiSet->size()), getLogObject());

							CIndividualPrecomputationCoordinationHash* lastIndiCompCoordHash = totallyPreCompItem->getCurrentIndividualComputationCoordinationHash();
							QSet<CIndividualReference>* currentCompIndiSet = totallyPreCompItem->getCurrentIndividualComputationSet();
							CIndividualPrecomputationCoordinationHash* newIndiCompCoordHash = new CIndividualPrecomputationCoordinationHash();
							if (lastIndiCompCoordHash) {
								for (CIndividualReference indiRef : *currentCompIndiSet) {
									CIndividualPrecomputationCoordinationHashData* data = lastIndiCompCoordHash->value(indiRef.getIndividualID());
									if (data) {
										data->incUsageCount();
										data->setNewlyRetrieved(false);
										(*newIndiCompCoordHash)[indiRef.getIndividualID()] = data;
									}
								}
								decrementUsageFromIndividualPrecomputationCoordinationHash(lastIndiCompCoordHash, totallyPreCompItem);
							}

							newIndiCompCoordHash->incUsageCount();
							for (CIndividualReference indiRef : *procReqIndiSet) {
								CIndividualPrecomputationCoordinationHashData*& data = (*newIndiCompCoordHash)[indiRef.getIndividualID()];
								if (!data) {
									newIndisRetrieved = true;
									data = new CIndividualPrecomputationCoordinationHashData();
									data->incUsageCount();
								}
								data->setNewlyRetrieved(true);
							}
							totallyPreCompItem->setCurrentIndividualComputationCoordinationHash(newIndiCompCoordHash);

						} else {
							LOG(INFO, getLogDomain(), logTr("Retrieved no more insufficiently handled individuals from representative cache, finishing processing."), getLogObject());
							if (!totallyPreCompItem->isIndividualComputationRunning()) {
								LOG(INFO, getLogDomain(), logTr("Finished insufficiently handled individual reprocessing with representative cache."), getLogObject());
								totallyPreCompItem->setAllIncompletelyHandledIndividualsRetrieved(true);
								CIndividualPrecomputationCoordinationHash* lastIndiCompCoordHash = totallyPreCompItem->getCurrentIndividualComputationCoordinationHash();
								if (lastIndiCompCoordHash) {
									decrementUsageFromIndividualPrecomputationCoordinationHash(lastIndiCompCoordHash, totallyPreCompItem);
								}
							}
						}
					}

					if (!procReqIndiSet->isEmpty()) {
						cint64 computationLimit = totallyPreCompItem->getCurrentIncompletelyHandledIndividualComputationLimit();
						CIndividualPrecomputationCoordinationHash* currentIndiCompCoordHash = totallyPreCompItem->getCurrentIndividualComputationCoordinationHash();
						cint64 currentCount = 0;
						for (QSet<CIndividualReference>::iterator it = procReqIndiSet->begin(); it != procReqIndiSet->end() && currentCount < computationLimit;) {
							CIndividualPrecomputationCoordinationHashData* data = currentIndiCompCoordHash->value(it->getIndividualID());
							if (!data->isComputationIntegrated() && !data->isComputationOrdered()) {
								++currentCount;
								indiList.append(*it);
								data->setComputationOrdered(true);
							}
							it = procReqIndiSet->erase(it);
						}
					}
				}
				return indiList;
			}



			bool CTotallyPrecomputationThread::createIndividualPrecomputationCheck(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CConcreteOntology* onto = totallyPreCompItem->getOntology();
				CSatisfiableCalculationJob* satCalcJob = nullptr;

				bool fullCompletionGraphConstruction = totallyPreCompItem->isFullCompletionGraphConstruction();
				if (!fullCompletionGraphConstruction && !totallyPreCompItem->hasIndividualsSaturated()) {
					fullCompletionGraphConstruction = true;
					totallyPreCompItem->setFullCompletionGraphConstruction(true);
				}
				if (totallyPreCompItem->hasIndividualsSaturated() && totallyPreCompItem->hasALLIndividualsSaturationOrderd() && totallyPreCompItem->hasInsufficientSaturationIndividuals()) {
					if (!fullCompletionGraphConstruction) {
						if (!totallyPreCompItem->hasIndividualPrecomputationChecked()) {
							QSet<CIndividualReference>* procReqIndiSet = totallyPreCompItem->getIncompletelyHandledIndividualSet();

							if (!procReqIndiSet->isEmpty() || !totallyPreCompItem->hasAllIncompletelyHandledIndividualsRetrieved()) {
								QList<CIndividualReference> indiList;
								indiList = getNextIndividualComputationPartList(totallyPreCompItem);

								if (!indiList.isEmpty()) {

									cint64 minIndiId = CINT64_MAX;

									QList<CIndividualReference> filteredIndiList;
									CIndividualPrecomputationCoordinationHash* currentIndiCompCoordHash = totallyPreCompItem->getCurrentIndividualComputationCoordinationHash();
									QSet<CIndividualReference>* currentIndiComputationSet = totallyPreCompItem->getCurrentIndividualComputationSet();
									for (CIndividualReference indiRef : indiList) {
										minIndiId = qMin(minIndiId, indiRef.getIndividualID());
										filteredIndiList.append(indiRef);
										currentIndiComputationSet->insert(indiRef);
									}

									CSatisfiableCalculationJobGenerator satCalcJobGen(onto);
									satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(filteredIndiList);

									CIndividualPrecomputationTestingItem* indiTestItem = new CIndividualPrecomputationTestingItem(totallyPreCompItem, currentIndiCompCoordHash, totallyPreCompItem, totallyPreCompItem->getIndividualPrecomputationClashedPointer());
									if (currentIndiCompCoordHash) {
										currentIndiCompCoordHash->incUsageCount();
									}
									indiTestItem->addIndividualComputations(filteredIndiList);
									CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
									satCalcJob->setCalculationConfiguration(calcConfig);
									CSatisfiableTaskRepresentativeBackendUpdatingAdapter* repCacheUpdAdapter = indiTestItem->getRepresentativeCacheUpdatingAdapter();
									repCacheUpdAdapter->setFirstIndividualRecomputationId(minIndiId);
									repCacheUpdAdapter->setRepresentativeCacheRecomputationId(totallyPreCompItem->getNextRepresentativeCacheRecomputationId());
									satCalcJob->setSatisfiableRepresentativeBackendCacheUpdatingAdapter(repCacheUpdAdapter);
									CIndividual* allAssertionIndi = totallyPreCompItem->getAllAssertionIndividual();
									if (allAssertionIndi) {
										repCacheUpdAdapter->setAllAssertionIndividualId(allAssertionIndi->getIndividualID());
									}

									processCalculationJob(satCalcJob, totallyPreCompItem, indiTestItem);
									return true;
								}
							}
						}
					}
				}
				return false;
			}








			
			bool CTotallyPrecomputationThread::createConsistencePrecomputationCheck(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CConcreteOntology* onto = totallyPreCompItem->getOntology();
				CSatisfiableCalculationJob* satCalcJob = nullptr;

				CIndividualVector* indiVec = onto->getABox()->getIndividualVector(false);
				QSet<CIndividual*>* activeIndiSet = onto->getABox()->getActiveIndividualSet(false);
				cint64 indiCount = 0;
				if (indiVec) {
					indiCount = indiVec->getItemCount();
				}
				cint64 maxTriplesIndexedIndiId = -1;
				COntologyTriplesAssertionsAccessor* triplesAccessor = totallyPreCompItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (triplesAccessor) {
					maxTriplesIndexedIndiId = totallyPreCompItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor()->getMaxIndexedIndividualId();
				}


				bool forceCompletionGraphConstruction = totallyPreCompItem->isForceCompletionGraphConstruction();
				bool fullCompletionGraphConstruction = totallyPreCompItem->isFullCompletionGraphConstruction();
				bool consistencyDetected = false;
				bool detectedConsistency = true;
				QSet<CIndividualReference>* procReqIndiSet = procReqIndiSet = totallyPreCompItem->getIncompletelyHandledIndividualSet();
				if (!procReqIndiSet->isEmpty()) {
					forceCompletionGraphConstruction = true;
				}
				if (!forceCompletionGraphConstruction && isAllAssertionIndividualSaturationSufficient(totallyPreCompItem)) {
					consistencyDetected = true;
					detectedConsistency = true;
					LOG(INFO,getLogDomain(),logTr("Trivial consistency detected with merged individual."),getLogObject());
				} else if (totallyPreCompItem->hasClashedSaturationIndividuals()) {
					consistencyDetected = true;
					detectedConsistency = false;
					LOG(INFO,getLogDomain(),logTr("Trivial inconsistency detected with individual saturation."),getLogObject());
				} else if (!forceCompletionGraphConstruction && totallyPreCompItem->hasIndividualsSaturated() && !totallyPreCompItem->hasInsufficientSaturationIndividuals() && (indiCount > 0 || maxTriplesIndexedIndiId > 0)) {
					consistencyDetected = true;
					detectedConsistency = true;
					LOG(INFO,getLogDomain(),logTr("Trivial consistency detected with individual saturation."),getLogObject());
				} else if (!fullCompletionGraphConstruction && totallyPreCompItem->hasIndividualsSaturated() && totallyPreCompItem->hasAllIncompletelyHandledIndividualsRetrieved() && (indiCount > 0 || maxTriplesIndexedIndiId > 0)) {
					consistencyDetected = true;
					detectedConsistency = !totallyPreCompItem->hasIndividualPrecomputationClashed();
					if (detectedConsistency) {
						LOG(INFO, getLogDomain(), logTr("Consistency detected with individual precomputation."), getLogObject());
					} else {
						LOG(INFO, getLogDomain(), logTr("Inconsistency detected with individual precomputation."), getLogObject());
					}
				} else if (totallyPreCompItem->hasIndividualPrecomputationClashed()) {
					consistencyDetected = true;
					detectedConsistency = false;
					LOG(INFO, getLogDomain(), logTr("Inconsistency detected with individual precomputation."), getLogObject());
				} else {
					QList<CIndividualReference> indiList;
					if (!fullCompletionGraphConstruction && procReqIndiSet) {
						indiList = procReqIndiSet->toList();
						procReqIndiSet->clear();
					} else {
						indiList = getIndividualComputationList(totallyPreCompItem);
					}
					totallyPreCompItem->setAllIncompletelyHandledIndividualsRetrieved(true);
					totallyPreCompItem->setFullCompletionGraphConstructed(true);

					CSatisfiableCalculationJobGenerator satCalcJobGen(onto);
					if (indiList.isEmpty()) {
						CConcept* topConcept = onto->getTBox()->getTopConcept();
						if (topConcept->getOperandCount() > 0) {
							satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topConcept);
						} else {
							consistencyDetected = true;
							detectedConsistency = true;
						}
					} else {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(indiList);
					}
				}


				CConsistence* consistence = onto->getConsistence();
				if (!consistence) {
					consistence = new CConsistence();
					onto->setConsistence(consistence);
				}
				if (totallyPreCompItem->hasIndividualsSaturated() && totallyPreCompItem->hasALLIndividualsSaturationOrderd()) {
					consistence->setIndividualsRepresentativelyStored(true);
					if (!totallyPreCompItem->hasInsufficientSaturationIndividuals()) {
						consistence->setAllIndividualsRepresentativelyStored(true);
					} else if (totallyPreCompItem->hasAllIndividualPrecomputationCreated()) {
						consistence->setAllIndividualsRepresentativelyStored(true);
					}
				}
				totallyPreCompItem->setConsistence(consistence);

				if (consistencyDetected) {
					consistence->setOntologyConsistent(detectedConsistency);
					totallyPreCompItem->setConsistenceCheched(true);
					return false;
				} else {
					CConsistencePrecomputationTestingItem* consTestItem = new CConsistencePrecomputationTestingItem(totallyPreCompItem,totallyPreCompItem);
					CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
					satCalcJob->setCalculationConfiguration(calcConfig);
					satCalcJob->setSatisfiableTaskPreyingAdapter(consTestItem->getTaskPreyingAdapter());

					processCalculationJob(satCalcJob,totallyPreCompItem,consTestItem);
					return true;
				}
			}





			void CTotallyPrecomputationThread::propagateExistInitializationFlag(CSaturationConceptDataItem* ontConSatDataItem) {
				if (ontConSatDataItem->isPotentiallyExistInitializationConcept()) {
					CSaturationConceptDataItem* parentOntConceptSatTestItem = ontConSatDataItem->getSpecialItemReference();
					while (parentOntConceptSatTestItem && !parentOntConceptSatTestItem->isPotentiallyExistInitializationConcept()) {
						parentOntConceptSatTestItem->setPotentiallyExistInitializationConcept(true);
						parentOntConceptSatTestItem = parentOntConceptSatTestItem->getSpecialItemReference();
					}
				}
			}


			void CTotallyPrecomputationThread::propagateSubsumerItemFlag(CSaturationConceptDataItem* ontClassSatTestItem) {
				if (ontClassSatTestItem->hasIndirectSuccessorsItems()) {
					CSaturationConceptDataItem* subsumerItem = ontClassSatTestItem->getSpecialItemReference();
					while (subsumerItem && !subsumerItem->hasIndirectSuccessorsItems()) {
						subsumerItem->setIndirectSuccessorsItems(true);
						subsumerItem = subsumerItem->getSpecialItemReference();
					}
				}
			}







			void CTotallyPrecomputationThread::analyseConceptSaturationSubsumerExistItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem, QList<CConceptNegationSaturationItem>& processList) {

				QSet<CConceptNegationSaturationItem> processSet;
				for (QList<CConceptNegationSaturationItem>::const_iterator it = processList.constBegin(), itEnd = processList.constEnd(); it != itEnd; ++it) {
					CConceptNegationSaturationItem conNegSatItem(*it);
					processSet.insert(conNegSatItem);
				}

				while (!processList.isEmpty()) {
					CConceptNegationSaturationItem conNegSatItem(processList.takeLast());
					CConcept* concept = conNegSatItem.getConcept();
					bool negated = conNegSatItem.getNegation();
					CSaturationConceptDataItem* ontConSatDataItem = conNegSatItem.getSaturationConceptDataItem();
					cint64 nOpCode = concept->getOperatorCode();
					cint64 opCount = concept->getOperandCount();
					if (!negated && (nOpCode == CCAND || nOpCode == CCSUB || nOpCode == CCEQ || ((nOpCode == CCOR) && opCount == 1)) || negated && (nOpCode == CCOR || ((nOpCode == CCAND || nOpCode == CCEQ) && opCount == 1))) {

						CSortedNegLinker<CConcept*>* opConIt = concept->getOperandList();
						while (opConIt) {
							CConcept* opConcept = opConIt->getData();
							bool opNegation = opConIt->isNegated()^negated;
							cint64 opConOpCode = opConcept->getOperatorCode();
							if (!opNegation && (opConOpCode == CCEQ || opConOpCode == CCSUB || opConOpCode == CCATOM && opConcept->hasClassName())) {
								CSaturationConceptDataItem* subsumerClassTestItem = totallyPreCompItem->getSaturationConceptDataItem(opConcept,opNegation,false);
								subsumerClassTestItem->setIndirectSuccessorsItems(true);
								if (!ontConSatDataItem->getSpecialItemReference() && !ontConSatDataItem->isInvalidSpecialItemReference()) {
									ontConSatDataItem->setSpecialItemReference(subsumerClassTestItem);
								} else {
									ontConSatDataItem->setMultipleDirectPredecessorsItems(true);
								}
							} else if (!opNegation && opConOpCode == CCAND || opNegation && opConOpCode == CCOR) {
								if (opConcept->getOperandCount() > 1) {
									ontConSatDataItem->setMultipleDirectPredecessorsItems(true);
								}
								if (!processSet.contains(CConceptNegationSaturationItem(opConcept,opNegation,ontConSatDataItem))) {
									processSet.insert(CConceptNegationSaturationItem(opConcept,opNegation,ontConSatDataItem));
									processList.append(CConceptNegationSaturationItem(opConcept,opNegation,ontConSatDataItem));
								}

							} else if ((!opNegation && (opConOpCode == CCSOME || opConOpCode == CCAQSOME)) || opNegation && opConOpCode == CCALL) {
								CRole* role = opConcept->getRole();
								CConcept* existConcept = opConcept->getOperandList()->getData();
								bool existConNegation = opConcept->getOperandList()->isNegated()^opNegation;
								CSaturationConceptDataItem* existConceptTestItem =  nullptr;
								if (hasRoleRanges(totallyPreCompItem,role)) {
									existConceptTestItem = totallyPreCompItem->getSaturationRoleSuccessorConceptDataItem(role,existConcept,existConNegation,false);
								} else {
									existConceptTestItem = totallyPreCompItem->getSaturationConceptDataItem(existConcept,existConNegation,false);
								}
								ontConSatDataItem->addExistReferenceConceptItemList(existConceptTestItem);
								ontConSatDataItem->setMultipleDirectPredecessorsItems(true);

							} else if (negated && opConOpCode == CCATMOST || !negated && opConOpCode == CCATLEAST) {
								CRole* role = opConcept->getRole();
								CConcept* existConcept = totallyPreCompItem->getOntology()->getTBox()->getTopConcept();
								bool existConceptNegation = false;
								if (opConcept->getOperandList()) {
									existConcept = opConcept->getOperandList()->getData();
									existConceptNegation = opConcept->getOperandList()->isNegated();
								}
								CSaturationConceptDataItem* existConceptTestItem =  nullptr;
								if (hasRoleRanges(totallyPreCompItem,role)) {
									existConceptTestItem = totallyPreCompItem->getSaturationRoleSuccessorConceptDataItem(role,existConcept,existConceptNegation,false);
								} else {
									existConceptTestItem = totallyPreCompItem->getSaturationConceptDataItem(existConcept,existConceptNegation,false);
								}
								ontConSatDataItem->addExistReferenceConceptItemList(existConceptTestItem);
								ontConSatDataItem->setMultipleDirectPredecessorsItems(true);

							} else if (opConOpCode == CCAQCHOOCE) {
								CSortedNegLinker<CConcept*>* opOpConIt = opConcept->getOperandList();
								while (opOpConIt) {
									if (opNegation == opOpConIt->isNegated()) {
										CConcept* opOpConcept = opOpConIt->getData();
										if (opOpConcept->getOperatorCode() == CCAQSOME) {
											CConcept* existConcept = opOpConcept->getOperandList()->getData();
											bool existConNegation = opOpConcept->getOperandList()->isNegated();
											CRole* role = opOpConcept->getRole();
											CSaturationConceptDataItem* existConceptTestItem =  nullptr;
											if (hasRoleRanges(totallyPreCompItem,role)) {
												existConceptTestItem = totallyPreCompItem->getSaturationRoleSuccessorConceptDataItem(role,existConcept,existConNegation,false);
											} else {
												existConceptTestItem = totallyPreCompItem->getSaturationConceptDataItem(existConcept,existConNegation,false);
											}
											ontConSatDataItem->addExistReferenceConceptItemList(existConceptTestItem);
											ontConSatDataItem->setMultipleDirectPredecessorsItems(true);
										} else if (opOpConcept->getOperatorCode() == CCAQAND) {
											if (opConcept->getOperandCount() > 1) {
												ontConSatDataItem->setMultipleDirectPredecessorsItems(true);
											}
											if (!processSet.contains(CConceptNegationSaturationItem(opOpConcept,false,ontConSatDataItem))) {
												processSet.insert(CConceptNegationSaturationItem(opOpConcept,false,ontConSatDataItem));
												processList.append(CConceptNegationSaturationItem(opOpConcept,false,ontConSatDataItem));
											}
										}
									}
									opOpConIt = opOpConIt->getNext();
								}
							} else {
								ontConSatDataItem->setMultipleDirectPredecessorsItems(true);
							}
							opConIt = opConIt->getNext();
						}
					}
				}
			}



			CConcept* CTotallyPrecomputationThread::getDisjunctCheckingConcept(CConcept* opConcept, bool opConNegation, bool* checkingNegation) {
				if (opConcept->getOperatorCode() == CCAQCHOOCE) {
					cint64 replaceCount = 0;
					CConcept* replaceCheckingConcept = nullptr;
					CSortedNegLinker<CConcept*>* opConceptOpLinkerIt = opConcept->getOperandList();
					while (opConceptOpLinkerIt) {
						CConcept* opOpConcept = opConceptOpLinkerIt->getData();
						bool opOpNegation = opConceptOpLinkerIt->isNegated();
						if (opOpNegation == opConNegation) {
							replaceCheckingConcept = opOpConcept;
							replaceCount++;
						}
						opConceptOpLinkerIt = opConceptOpLinkerIt->getNext();
					}

					if (replaceCount == 1 && replaceCheckingConcept) {
						if (checkingNegation) {
							*checkingNegation = false;
						}
						return replaceCheckingConcept;
					}
				}
				return opConcept;
			}


			void CTotallyPrecomputationThread::extendDisjunctionsCandidateAlternativesItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CSaturationConceptDataItem* ontConSatDataItem, QList<CSaturationConceptDataItem*>* newDisjunctionCandidateAlternativeList) {
				CConcept* concept = ontConSatDataItem->getSaturationConcept();
				bool negation = ontConSatDataItem->getSaturationNegation();
				QList<TConceptNegPair> conNegPairList;

				bool directExamine = true;
				bool candidateAlternativeExtraction = false;
				if (!negation && concept->hasClassName()) {
					if (concept->getOperatorCode() == CCEQ) {
						//CBOXHASH<CConcept*,CConcept*>* eqConHash = totallyPreCompItem->getOntology()->getTBox()->getEquivalentConceptCandidateHash(false);
						//if (eqConHash->contains(concept)) {
							candidateAlternativeExtraction = true;
						//}
					}
					for (CSortedNegLinker<CConcept*>* opConIt = concept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
						CConcept* opConcept = opConIt->getData();
						bool opNegation = opConIt->isNegated()^negation;
						conNegPairList.append(TConceptNegPair(opConcept,opNegation));
						directExamine = false;
					}
				}
				if (directExamine) {
					conNegPairList.append(TConceptNegPair(concept,negation));
				}

				while (!conNegPairList.isEmpty()) {
					TConceptNegPair nextConNegPair(conNegPairList.takeFirst());
					CConcept* nextConcept = nextConNegPair.first;
					bool nextNegation = nextConNegPair.second;
					cint64 nOpCode = nextConcept->getOperatorCode();
					cint64 opCount = nextConcept->getOperandCount();
					if (!nextNegation && (nOpCode == CCAND || (nOpCode == CCOR && opCount == 1)) || nextNegation && (nOpCode == CCOR || ((nOpCode == CCAND) && opCount == 1))) {
						for (CSortedNegLinker<CConcept*>* opConIt = nextConcept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
							CConcept* opConcept = opConIt->getData();
							bool opNegation = opConIt->isNegated()^nextNegation;
							conNegPairList.append(TConceptNegPair(opConcept,opNegation));
						}

					} else if (nOpCode == CCAQCHOOCE) {
						for (CSortedNegLinker<CConcept*>* opConIt = nextConcept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
							CConcept* opConcept = opConIt->getData();
							bool opNegation = opConIt->isNegated();
							if (nextNegation == opNegation) {
								conNegPairList.append(TConceptNegPair(opConcept,opNegation));
							}
							if (candidateAlternativeExtraction && nextNegation != opNegation) {
								conNegPairList.append(TConceptNegPair(opConcept,!opNegation));
							}
						}

					} else if (nextNegation && ((nOpCode == CCAND || nOpCode == CCEQ) && opCount > 1) || !nextNegation && nOpCode == CCOR) {
						// saturate all disjuncts to extract common concepts from all disjunctions

						totallyPreCompItem->addSaturatedDisjunction(nextConcept);

						for (CSortedNegLinker<CConcept*>* opConIt = nextConcept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
							CConcept* opConcept = opConIt->getData();
							bool opNegation = opConIt->isNegated()^nextNegation;

							bool opCheckingNegation = opNegation;
							CConcept* opCheckConcept = getDisjunctCheckingConcept(opConcept, opNegation, &opCheckingNegation);

							CConceptProcessData* conProcData = (CConceptProcessData*)opCheckConcept->getConceptData();
							CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
							if (!conRefSatLinking) {
								conRefSatLinking = new CConceptSaturationReferenceLinkingData();
								conProcData->setConceptReferenceLinking(conRefSatLinking);
							}
							if (conRefSatLinking->getConceptSaturationReferenceLinkingData(opCheckingNegation) == nullptr) {
								CSaturationConceptDataItem* conItem = totallyPreCompItem->getSaturationConceptDataItem(opCheckConcept, opCheckingNegation,true);
								conRefSatLinking->setSaturationReferenceLinkingData(conItem, opCheckingNegation);
								newDisjunctionCandidateAlternativeList->append(conItem);
								if (!opCheckConcept->hasClassName() || opCheckingNegation) {
									conItem->setInvalidSpecialItemReference(true);
								}
							}
						}


						CConceptProcessData* conProcData = (CConceptProcessData*)nextConcept->getConceptData();
						CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
						if (!conRefSatLinking) {
							conRefSatLinking = new CConceptSaturationReferenceLinkingData();
							conProcData->setConceptReferenceLinking(conRefSatLinking);
						}
						if (conRefSatLinking->getConceptSaturationReferenceLinkingData(nextNegation) == nullptr) {
							CSaturationConceptDataItem* conItem = totallyPreCompItem->getSaturationConceptDataItem(nextConcept,nextNegation,true);
							conRefSatLinking->setSaturationReferenceLinkingData(conItem,nextNegation);
							newDisjunctionCandidateAlternativeList->append(conItem);
							conItem->setInvalidSpecialItemReference(true);
						}



					} else if (!nextNegation && (nOpCode == CCALL) || nextNegation && (nOpCode == CCSOME || nOpCode == CCAQSOME)) {
						if (candidateAlternativeExtraction) {
							// saturate pseudo models to invalidate candidates concepts

							bool saturateNegation = !nextNegation;

							CConceptProcessData* conProcData = (CConceptProcessData*)nextConcept->getConceptData();
							CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
							if (!conRefSatLinking) {
								conRefSatLinking = new CConceptSaturationReferenceLinkingData();
								conProcData->setConceptReferenceLinking(conRefSatLinking);
							}
							if (conRefSatLinking->getConceptSaturationReferenceLinkingData(saturateNegation) == nullptr) {
								CSaturationConceptDataItem* conItem = totallyPreCompItem->getSaturationConceptDataItem(nextConcept,saturateNegation,true);
								conRefSatLinking->setSaturationReferenceLinkingData(conItem,saturateNegation);
								newDisjunctionCandidateAlternativeList->append(conItem);
								conItem->setInvalidSpecialItemReference(true);
							}
						}
					}
				}
			}




			bool CTotallyPrecomputationThread::saturateRemainingRequiredItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				cint64 saturatingConceptCount = 0;
				bool allConSaturReq = totallyPreCompItem->requiresAllConceptsSaturation();
				QList<TConceptNegPair>* reqConSatList = totallyPreCompItem->getRemainingRequiredSaturationConceptList();
				if (allConSaturReq) {
					totallyPreCompItem->setAllConceptsSaturationOrdered(true);
					QList<CSaturationConceptDataItem*>* satOrderdItemList = totallyPreCompItem->getSaturationOrderedItemList();
					for (QList<CSaturationConceptDataItem*>::const_iterator it = satOrderdItemList->constBegin(), itEnd = satOrderdItemList->constEnd(); it != itEnd; ++it) {
						CSaturationConceptDataItem* satConItem(*it);
						if (!satConItem->isItemProcessingMarked()) {
							++saturatingConceptCount;
							satConItem->setItemProcessingMarked(true);
						}
					}
				} else {
					for (QList<TConceptNegPair>::const_iterator it = reqConSatList->constBegin(), itEnd = reqConSatList->constEnd(); it != itEnd; ++it) {
						const TConceptNegPair& conNegPair(*it);
						CConcept* concept(conNegPair.first);
						bool negation = conNegPair.second;
						saturatingConceptCount += markSaturationProcessingItems(totallyPreCompItem,nullptr,concept,negation);
					}
				}
				reqConSatList->clear();
				if (saturatingConceptCount > 0 || totallyPreCompItem->requiresNominalDelayedConceptsSaturationUpdate() && !totallyPreCompItem->hasNominalDelayedConceptsSaturationUpdated()) {
					totallyPreCompItem->setNominalDelayedConceptsSaturationUpdateRequired(false);
					totallyPreCompItem->setNominalDelayedConceptsSaturationUpdated(true);
					createMarkedConceptSaturationProcessingJob(totallyPreCompItem,true);
					return true;
				}
				return false;
			}



			bool CTotallyPrecomputationThread::addRequiredSaturationIndividuals(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CIndividualVector* indiVec = totallyPreCompItem->getOntology()->getABox()->getIndividualVector(false);
				QSet<CIndividual*>* activeIndiSet = totallyPreCompItem->getOntology()->getABox()->getActiveIndividualSet(false);
				bool individualsAdded = false;
				bool saturateConcepts = CConfigDataReader::readConfigBoolean(totallyPreCompItem->getCalculationConfiguration(),"Konclude.Calculation.Optimization.ConceptSaturation",true);
				bool individualSaturation = CConfigDataReader::readConfigBoolean(totallyPreCompItem->getCalculationConfiguration(),"Konclude.Calculation.Optimization.IndividualSaturation",false);
				if (saturateConcepts && individualSaturation) {
					if (indiVec) {
						QSet<TConceptNegPair> assConNegPairSet;
						cint64 indiCount = indiVec->getItemCount();
						for (cint64 indiId = 0; indiId < indiCount; ++indiId) {
							CIndividual* indi = indiVec->getData(indiId);
							if (indi && activeIndiSet->contains(indi)) {
								totallyPreCompItem->addRequiredABoxSaturationIndividual(indi);
								individualsAdded = true; 
							}
						}
					}
					totallyPreCompItem->setIndividualsSaturated(true);
				} else {
					totallyPreCompItem->setIndividualsSaturated(false);
				}
				return individualsAdded;
			}



			bool CTotallyPrecomputationThread::addIdentifiedRemainingConsistencyRequiredConcepts(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {

				bool constConReqSatAdded = false;
				bool saturateConcepts = CConfigDataReader::readConfigBoolean(totallyPreCompItem->getCalculationConfiguration(),"Konclude.Calculation.Optimization.ConceptSaturation",true);

				if (saturateConcepts) {
					CConcept* topConcept = totallyPreCompItem->getOntology()->getTBox()->getTopConcept();
					if (topConcept->getOperandList() != nullptr) {
						totallyPreCompItem->addConsistencyRequiredSaturationConcept(topConcept,false);
						constConReqSatAdded = true;
					}
					CIndividualVector* indiVec = totallyPreCompItem->getOntology()->getABox()->getIndividualVector(false);
					QSet<CIndividual*>* activeIndiSet = totallyPreCompItem->getOntology()->getABox()->getActiveIndividualSet(false);
					QSet<TConceptNegPair> assConNegPairSet;
					QSet<CRole*> assRoleSet;
					bool createAllAssertionIndi = false;
					if (indiVec) {
						cint64 indiCount = indiVec->getItemCount();

						for (cint64 indiId = 0; indiId < indiCount; ++indiId) {
							CIndividual* indi = indiVec->getData(indiId);
							if (indi && activeIndiSet->contains(indi)) {
								createAllAssertionIndi = true;
								for (CConceptAssertionLinker* conAssLinkIt = indi->getAssertionConceptLinker(); conAssLinkIt; conAssLinkIt = conAssLinkIt->getNext()) {
									CConcept* assCon = conAssLinkIt->getData();
									bool assConNeg = conAssLinkIt->isNegated();
									if (assCon->getOperatorCode() == CCNOMINAL) {
										if (assCon->getNominalIndividual() != indi || assConNeg && assCon->getNominalIndividual() == indi) {
											assConNegPairSet.insert(TConceptNegPair(assCon,assConNeg));
										}
									} else {
										assConNegPairSet.insert(TConceptNegPair(assCon,assConNeg));
									}
								}
								for (CRoleAssertionLinker* roleAssLinkIt = indi->getAssertionRoleLinker(); roleAssLinkIt; roleAssLinkIt = roleAssLinkIt->getNext()) {
									CRole* role = roleAssLinkIt->getRole();
									assRoleSet.insert(role);
								}

							}
						}
					}

					COntologyTriplesData* ontTripleData = totallyPreCompItem->getOntology()->getOntologyTriplesData();
					if (ontTripleData && ontTripleData->getTripleAssertionAccessor()) {
						COntologyTriplesAssertionsAccessor* assertionAccessor = ontTripleData->getTripleAssertionAccessor();
						if (assertionAccessor->getIndexedIndividualCount() > 0) {
							createAllAssertionIndi = true;
							CTriplesAssertedConceptsExtendingSetVisitor assertedConceptsVisitor(&assConNegPairSet);
							assertionAccessor->visitAllAssertedConcepts(&assertedConceptsVisitor);
							CTriplesAssertedRolesExtendingSetVisitor assertedRolesVisitor(&assRoleSet);
							assertionAccessor->visitAllAssertedObjectRoles(&assertedRolesVisitor);

							QSet<CRole*> dataRoleSet;
							CTriplesAssertedRolesExtendingSetVisitor assertedDataRolesVisitor(&dataRoleSet);
							assertionAccessor->visitAllAssertedDataRoles(&assertedDataRolesVisitor);
							for (CRole* dataRole : dataRoleSet) {
								for (CSortedNegLinker<CRole*>* superDataRoleIt = dataRole->getIndirectSuperRoleList(); superDataRoleIt; superDataRoleIt = superDataRoleIt->getNext()) {
									CRole* superDataRole = superDataRoleIt->getData();
									for (CSortedNegLinker<CConcept*>* domainConLinkerIt = superDataRole->getDomainRangeConceptList(superDataRoleIt->isNegated()); domainConLinkerIt; domainConLinkerIt = domainConLinkerIt->getNext()) {
										CConcept* domainCon = domainConLinkerIt->getData();
										bool domainConNegation = domainConLinkerIt->isNegated();
										assConNegPairSet.insert(TConceptNegPair(domainCon, domainConNegation));
									}
								}
							}
						}
					}



					if (createAllAssertionIndi && (!assConNegPairSet.isEmpty() || !assRoleSet.isEmpty())) {
						CIndividual* tmpAllAssertionIndi = new CIndividual();
						tmpAllAssertionIndi->initIndividual(totallyPreCompItem->getOntology()->getABox()->getNextIndividualId(false));
						tmpAllAssertionIndi->setTemporaryFakeIndividual(true);

						for (QSet<TConceptNegPair>::const_iterator it = assConNegPairSet.constBegin(), itEnd = assConNegPairSet.constEnd(); it != itEnd; ++it) {
							constConReqSatAdded = true;
							TConceptNegPair assConNegPair(*it);
							totallyPreCompItem->addConsistencyRequiredSaturationConcept(assConNegPair.first, assConNegPair.second);
							CConceptAssertionLinker* tmpConAssLinker = new CConceptAssertionLinker();
							tmpConAssLinker->initNegLinker(assConNegPair.first, assConNegPair.second);
							tmpAllAssertionIndi->addAssertionConceptLinker(tmpConAssLinker);
						}
						for (QSet<CRole*>::const_iterator it = assRoleSet.constBegin(), itEnd = assRoleSet.constEnd(); it != itEnd; ++it) {
							CRole* role(*it);
							CRoleAssertionLinker* tmpRoleAssLinker = new CRoleAssertionLinker();
							tmpRoleAssLinker->initRoleAssertionLinker(role, tmpAllAssertionIndi);
							tmpAllAssertionIndi->addAssertionRoleLinker(tmpRoleAssLinker);
						}
						totallyPreCompItem->setAllAssertionIndividual(tmpAllAssertionIndi);
					}
				}
				
				return constConReqSatAdded;
			}



			bool CTotallyPrecomputationThread::saturateRemainingConsistencyRequiredConcepts(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				cint64 saturatingConceptCount = 0;
				QList<TConceptNegPair>* reqConSatList = totallyPreCompItem->getRemainingConsistencyRequiredSaturationConceptList();
				for (QList<TConceptNegPair>::const_iterator it = reqConSatList->constBegin(), itEnd = reqConSatList->constEnd(); it != itEnd; ++it) {
					const TConceptNegPair& conNegPair(*it);
					CConcept* concept(conNegPair.first);
					bool negation = conNegPair.second;
					saturatingConceptCount += markSaturationProcessingItems(totallyPreCompItem,nullptr,concept,negation);
				}
				reqConSatList->clear();
				if (saturatingConceptCount > 0) {
					createMarkedConceptSaturationProcessingJob(totallyPreCompItem,false);
					return true;
				}
				return false;
			}



			bool CTotallyPrecomputationThread::saturateRemainingRequiredSaturationIndividuals(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				cint64 saturatingIndividualCount = 0;
				QList<CIndividual*> indiSatList;
				QList<CIndividual*>* reqIndiSatList = totallyPreCompItem->getRemainingRequiredABoxSaturationIndividuals();
				while (!reqIndiSatList->isEmpty() && saturatingIndividualCount < mIndividualSaturationCount) {
					CIndividual* individual = reqIndiSatList->takeFirst();
					indiSatList.append(individual);
					totallyPreCompItem->setHandledTriplesIndividualSaturatedId(qMax(individual->getIndividualID(), totallyPreCompItem->getHandledTriplesIndividualSaturatedId()));
					++saturatingIndividualCount;
				}
				cint64 maxTriplesIndexedIndiId = -1;
				COntologyTriplesAssertionsAccessor* triplesAccessor = totallyPreCompItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (triplesAccessor) {
					maxTriplesIndexedIndiId = totallyPreCompItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor()->getMaxIndexedIndividualId();
				}
				cint64 handledTriplesIndexedIndis = totallyPreCompItem->getHandledTriplesIndividualSaturatedId();
				bool remainingTripleIndexedIndis = handledTriplesIndexedIndis < maxTriplesIndexedIndiId;


				if (saturatingIndividualCount <= 0 && remainingTripleIndexedIndis) {
					cint64 newHandledCount = handledTriplesIndexedIndis + mIndividualSaturationCount;
					newHandledCount = qMin(newHandledCount, maxTriplesIndexedIndiId);

					createTripleIndexedIndividualsSaturationProcessingJob(totallyPreCompItem, handledTriplesIndexedIndis + 1, newHandledCount);

					handledTriplesIndexedIndis = newHandledCount;
					totallyPreCompItem->setHandledTriplesIndividualSaturatedId(handledTriplesIndexedIndis);
					remainingTripleIndexedIndis = handledTriplesIndexedIndis < maxTriplesIndexedIndiId;

					if (!totallyPreCompItem->hasRemainingRequiredABoxSaturationIndividuals() && !remainingTripleIndexedIndis) {
						totallyPreCompItem->setALLIndividualsSaturationOrderd(true);
						totallyPreCompItem->setTripleIndexedIndividualSaturated(true);
					}
					return true;
				} else {
					if (!totallyPreCompItem->hasRemainingRequiredABoxSaturationIndividuals() && !remainingTripleIndexedIndis) {
						totallyPreCompItem->setALLIndividualsSaturationOrderd(true);
						totallyPreCompItem->setTripleIndexedIndividualSaturated(true);
					}
					createIndividualSaturationProcessingJob(totallyPreCompItem, indiSatList);
					return true;
				}

				return false;
			}

			cint64 CTotallyPrecomputationThread::markSaturationProcessingItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CSaturationConceptDataItem* startMarkingItem, CConcept* startMarkingConcept, bool startMarkingConceptNegation) {

				cint64 markedConcepts = 0;
				QList<CSaturationConceptDataItem*> itemList;
				QList<TConceptNegPair> conNegPairList;

				if (startMarkingItem && !startMarkingItem->isItemProcessingMarked()) {
					++markedConcepts;
					startMarkingItem->setItemProcessingMarked(true);
					itemList.append(startMarkingItem);
				}

				if (startMarkingConcept) {
					CSaturationConceptDataItem* opMarkingItem = totallyPreCompItem->getSaturationConceptDataItem(startMarkingConcept,startMarkingConceptNegation,false);
					if (opMarkingItem) {
						if (!opMarkingItem->isItemProcessingMarked()) {
							++markedConcepts;
							opMarkingItem->setItemProcessingMarked(true);
							itemList.append(opMarkingItem);
						}		
					} else {
						conNegPairList.append(TConceptNegPair(startMarkingConcept,startMarkingConceptNegation));
					}
				}


				while (!itemList.isEmpty() || !conNegPairList.isEmpty()) {

					while (!itemList.isEmpty()) {
						CSaturationConceptDataItem* item = itemList.takeFirst();
						CConcept* nextConcept = item->getSaturationConcept();
						bool nextNegation = item->getSaturationNegation();
						conNegPairList.append(TConceptNegPair(nextConcept,nextNegation));

						QList<CSaturationConceptDataItem*>* existRefItemList = item->getExistReferenceConceptItemList();
						for (QList<CSaturationConceptDataItem*>::const_iterator it2 = existRefItemList->constBegin(), it2End = existRefItemList->constEnd(); it2 != it2End; ++it2) {
							CSaturationConceptDataItem* existRefItem = *it2;
							if (!existRefItem->isItemProcessingMarked()) {
								++markedConcepts;
								existRefItem->setItemProcessingMarked(true);
								itemList.append(existRefItem);
							}
						}
					}


					while (!conNegPairList.isEmpty()) {
						TConceptNegPair nextConNegPair(conNegPairList.takeFirst());
						CConcept* nextConcept = nextConNegPair.first;
						bool nextNegation = nextConNegPair.second;
						cint64 nOpCode = nextConcept->getOperatorCode();
						cint64 opCount = nextConcept->getOperandCount();
						if (!nextNegation && (nOpCode == CCAND || nOpCode == CCSUB || nOpCode == CCIMPLTRIG || nOpCode == CCEQ || (nOpCode == CCOR && opCount == 1) || nOpCode == CCAQSOME || nOpCode == CCSOME || nOpCode == CCATLEAST) || nextNegation && (nOpCode == CCOR || ((nOpCode == CCAND || nOpCode == CCEQ) && opCount == 1) || nOpCode == CCALL || nOpCode == CCATMOST)) {
							for (CSortedNegLinker<CConcept*>* opConIt = nextConcept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
								CConcept* opConcept = opConIt->getData();
								bool influenceNegation = true;
								if (nOpCode == CCAQSOME || nOpCode == CCATLEAST || nOpCode == CCATMOST) {
									influenceNegation = false;
								}
								bool opNegation = opConIt->isNegated();
								if (influenceNegation && nextNegation) {
									opNegation = !opNegation;
								}
								bool opConceptMarked = false;
								if (nOpCode != CCAQCHOOCE || !opNegation) {
									CSaturationConceptDataItem* opMarkingItem = totallyPreCompItem->getSaturationConceptDataItem(opConcept,opNegation,false);
									CSaturationConceptDataItem* roleOpMarkingItem = nullptr;
									if (nextConcept->getRole()) {
										CRole* role = nextConcept->getRole();
										roleOpMarkingItem = totallyPreCompItem->getSaturationRoleSuccessorConceptDataItem(role,opConcept,opNegation,false);
									}
									if (opMarkingItem || roleOpMarkingItem) {
										if (opMarkingItem && !opMarkingItem->isItemProcessingMarked()) {
											++markedConcepts;
											opMarkingItem->setItemProcessingMarked(true);
											itemList.append(opMarkingItem);
										}		
										if (roleOpMarkingItem && !roleOpMarkingItem->isItemProcessingMarked()) {
											++markedConcepts;
											roleOpMarkingItem->setItemProcessingMarked(true);
											itemList.append(roleOpMarkingItem);
										}		
									} else {
										conNegPairList.append(TConceptNegPair(opConcept,opNegation));
									}
								}
							}
						} else if (nOpCode == CCAQCHOOCE) {
							for (CSortedNegLinker<CConcept*>* opConIt = nextConcept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
								CConcept* opConcept = opConIt->getData();
								if (opConIt->isNegated() == nextNegation) {
									CSaturationConceptDataItem* opMarkingItem = totallyPreCompItem->getSaturationConceptDataItem(opConcept,false,false);
									if (opMarkingItem) {
										if (!opMarkingItem->isItemProcessingMarked()) {
											++markedConcepts;
											opMarkingItem->setItemProcessingMarked(true);
											itemList.append(opMarkingItem);
										}		
									} else {
										conNegPairList.append(TConceptNegPair(opConcept,false));
									}
								}
							}
						}
					}
				}
				return markedConcepts;
			}




			void CTotallyPrecomputationThread::orderItemsSaturationTesting(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CSaturationConceptDataItem* ontConSatDataItem, QList<CSaturationConceptDataItem*>& orderedItemList, QStack<CSaturationSaturationListItem>& saturationItemStack) {


				if (!ontConSatDataItem->isOrderingQueued()) {

					saturationItemStack.push(CSaturationSaturationListItem(ontConSatDataItem));


					while (!saturationItemStack.isEmpty()) {
						CSaturationSaturationListItem& topSatSatListItem = saturationItemStack.top();
						CSaturationConceptDataItem* topOntConSatDataItem = topSatSatListItem.mSaturationItem;
						QList<CSaturationConceptDataItem*>& markOderingSubsumerList = topSatSatListItem.mSaturationItemList;

						if (!topOntConSatDataItem->isOrderingQueued()) {
							if (!topSatSatListItem.mSubsumerOrdered) {
								topSatSatListItem.mSubsumerOrdered = true;

								// first mark all subsumer items
								CSaturationConceptDataItem* subsumerItem = topOntConSatDataItem;
								while (subsumerItem && !subsumerItem->isOrderingQueued()) {
									subsumerItem->setOrderingQueued(true);
									markOderingSubsumerList.prepend(subsumerItem);
									subsumerItem = subsumerItem->getSpecialItemReference();
								}
							}
						}

						if (topSatSatListItem.mLastSubsumerSaturationItem) {
							orderedItemList.append(topSatSatListItem.mLastSubsumerSaturationItem);
							topSatSatListItem.mLastSubsumerSaturationItem = nullptr;
						}

						if (!markOderingSubsumerList.isEmpty()) {
							CSaturationConceptDataItem* subsumerItem = markOderingSubsumerList.takeFirst();
							topSatSatListItem.mLastSubsumerSaturationItem = subsumerItem;

							QList<CSaturationConceptDataItem*>* existRefItemList = subsumerItem->getExistReferenceConceptItemList();
							for (QList<CSaturationConceptDataItem*>::const_iterator it2 = existRefItemList->constBegin(), it2End = existRefItemList->constEnd(); it2 != it2End; ++it2) {
								CSaturationConceptDataItem* existRefItem = *it2;
								saturationItemStack.push(CSaturationSaturationListItem(existRefItem));
							}
							//QListIterator<CSaturationConceptDataItem*> it2(*existRefItemList);
							//it2.toBack();
							//while (it2.hasPrevious()) {
							//	CSaturationConceptDataItem* existRefItem = it2.previous();
							//	saturationItemStack.push(CSaturationSaturationListItem(existRefItem));
							//}


						} else {
							saturationItemStack.pop();
						}
					}	
				}

				//if (!ontConSatDataItem->isOrderingQueued()) {


				//	// first mark all subsumer items
				//	QList<CSaturationConceptDataItem*> markOderingSubsumerList;
				//	CSaturationConceptDataItem* subsumerItem = ontConSatDataItem;
				//	while (subsumerItem && !subsumerItem->isOrderingQueued()) {
				//		subsumerItem->setOrderingQueued(true);
				//		markOderingSubsumerList.prepend(subsumerItem);
				//		subsumerItem = subsumerItem->getSpecialItemReference();
				//	}



				//	for (QList<CSaturationConceptDataItem*>::const_iterator it1 = markOderingSubsumerList.constBegin(), it1End = markOderingSubsumerList.constEnd(); it1 != it1End; ++it1) {
				//		CSaturationConceptDataItem* subsumerItem = *it1;

				//		QList<CSaturationConceptDataItem*>* existRefItemList = subsumerItem->getExistReferenceConceptItemList();
				//		for (QList<CSaturationConceptDataItem*>::const_iterator it2 = existRefItemList->constBegin(), it2End = existRefItemList->constEnd(); it2 != it2End; ++it2) {
				//			CSaturationConceptDataItem* existRefItem = *it2;
				//			orderItemsSaturationTesting(totallyPreCompItem,existRefItem,orderedItemList);
				//		}

				//		orderedItemList.append(subsumerItem);
				//	}

				//}
			}


			bool CTotallyPrecomputationThread::hasRoleRanges(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CRole* role) {
				for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
					CRole* superRole = superRoleIt->getData();
					bool superRoleNegation = superRoleIt->isNegated();
					CSortedNegLinker<CConcept*>* rangeConceptLinker = superRole->getDomainRangeConceptList(!superRoleNegation);
					if (rangeConceptLinker) {
						return true;
					}
				}
				return false;
			}








			void CTotallyPrecomputationThread::createMarkedConceptSaturationProcessingJob(CTotallyOntologyPrecomputationItem* totallyPreCompItem, bool allowAllSaturation) {
				CApproximatedSaturationCalculationJob* satCalculationJob = nullptr;
				CConcreteOntology *onto = totallyPreCompItem->getOntology();
				CApproximatedSaturationCalculationJobGenerator satCalculationJobGenerator(onto);
				satCalculationJob = satCalculationJobGenerator.getApproximatedSaturationCalculationJob(0, totallyPreCompItem->getSaturationData());


				bool imiadelyProcessAllConcepts = totallyPreCompItem->requiresAllConceptsSaturation();
				if (!allowAllSaturation) {
					imiadelyProcessAllConcepts = false;
				}

				cint64 saturationConceptCount = 0;
				QList<CSaturationConceptDataItem*>* orderedItemList = totallyPreCompItem->getSaturationOrderedItemList();
				for (QList<CSaturationConceptDataItem*>::const_iterator it = orderedItemList->constBegin(), itEnd = orderedItemList->constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conItem = *it;
					CConcept* concept = conItem->getSaturationConcept();
					bool negation = conItem->getSaturationNegation();
					CRole* role = conItem->getSaturationRoleRanges();
					if (conItem->isItemProcessingMarked() && !conItem->isItemProcessingQueued() || imiadelyProcessAllConcepts) {
						conItem->setItemProcessingQueued(true);
						satCalculationJob = satCalculationJobGenerator.extendApproximatedSaturationCalculationJobProcessing(conItem, satCalculationJob);
						saturationConceptCount++;
					}
				}
				if (!totallyPreCompItem->isAllAssertionIndividualSaturated()) {
					CIndividual* allAssertionIndi = totallyPreCompItem->getAllAssertionIndividual();
					if (allAssertionIndi) {
						satCalculationJob = satCalculationJobGenerator.extendApproximatedSaturationCalculationJobProcessing(allAssertionIndi, nullptr, satCalculationJob);
					}
					totallyPreCompItem->setAllAssertionIndividualSaturated(true);
				}

				LOG(INFO, getLogDomain(), logTr("Scheduled %1 concepts for saturation for ontology '%2'.").arg(saturationConceptCount).arg(totallyPreCompItem->getOntology()->getTerminologyName()), this);

				CSaturationPrecomputationTestingItem* satTestingItem = new CSaturationPrecomputationTestingItem(totallyPreCompItem, CPrecomputationTestingItem::CONCEPTSATURATIONPRECOMPUTATIONTYPE);
				CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
				satCalculationJob->setCalculationConfiguration(calcConfig);
				satCalculationJob->setSaturationTaskPreyingAdapter(new CTaskPreyingAdapter((CSaturationObserver*)totallyPreCompItem));

				totallyPreCompItem->setSaturationCalculationJob(satCalculationJob);
				processCalculationJob(satCalculationJob, totallyPreCompItem, satTestingItem);
			}









			void CTotallyPrecomputationThread::createOccurrenceStatisticsCollectingSaturationProcessingJob(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CApproximatedSaturationCalculationJob* satCalculationJob = nullptr;
				CConcreteOntology *onto = totallyPreCompItem->getOntology();
				CApproximatedSaturationCalculationJobGenerator satCalculationJobGenerator(onto);
				satCalculationJob = satCalculationJobGenerator.getApproximatedSaturationCalculationJob(0,totallyPreCompItem->getSaturationData());

				
				CSaturationPrecomputationTestingItem* satTestingItem = new CSaturationPrecomputationTestingItem(totallyPreCompItem,CPrecomputationTestingItem::OCCURRENCESTATISTICSSATURATIONPRECOMPUTATIONTYPE);
				CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
				satCalculationJob->setCalculationConfiguration(calcConfig);
				satCalculationJob->setOccurrenceStatisticsCollectingAdapter(new CSaturationOccurrenceStatisticsCollectingAdapter());

				totallyPreCompItem->setSaturationCalculationJob(satCalculationJob);
				processCalculationJob(satCalculationJob,totallyPreCompItem,satTestingItem);
			}








			void CTotallyPrecomputationThread::createIndividualSaturationProcessingJob(CTotallyOntologyPrecomputationItem* totallyPreCompItem, const QList<CIndividual*>& individualList) {
				CApproximatedSaturationCalculationJob* satCalculationJob = nullptr;
				CConcreteOntology *onto = totallyPreCompItem->getOntology();
				CApproximatedSaturationCalculationJobGenerator satCalculationJobGenerator(onto);
				satCalculationJob = satCalculationJobGenerator.getApproximatedSaturationCalculationJob(0,totallyPreCompItem->getSaturationData());
				satCalculationJob->setSeparateSaturation(true);

				cint64 nextSaturationID = totallyPreCompItem->getNextSaturationID();
				CSaturationIndividualDataItem* satIndiDataItemLinker = nullptr;

				cint64 saturationIndividualCount = 0;
				for (QList<CIndividual*>::const_iterator it = individualList.constBegin(), itEnd = individualList.constEnd(); it != itEnd; ++it) {
					CIndividual* individual = *it;
					CSaturationIndividualDataItem* satIndiDataItem = totallyPreCompItem->takeFreeSaturationIndividualDataItem();
					satIndiDataItem->initConceptSaturationTestingItem(individual,nextSaturationID);
					satIndiDataItemLinker = satIndiDataItem->append(satIndiDataItemLinker);
					CIndividualProcessData* indiProcessData = (CIndividualProcessData*)individual->getIndividualData();
					if (indiProcessData) {
						indiProcessData->setSaturationReferenceLinkingData(satIndiDataItem);
					}
					satCalculationJob = satCalculationJobGenerator.extendApproximatedSaturationCalculationJobProcessing(individual,satIndiDataItem,satCalculationJob);
					++saturationIndividualCount;
				}
				totallyPreCompItem->setSaturationIDIndividualDataItems(nextSaturationID,satIndiDataItemLinker);

				LOG(INFO,getLogDomain(),logTr("Scheduled %1 individuals for saturation for ontology '%2'.").arg(saturationIndividualCount).arg(totallyPreCompItem->getOntology()->getTerminologyName()),this);

				CSaturationPrecomputationTestingItem* satTestingItem = new CSaturationPrecomputationTestingItem(totallyPreCompItem,CPrecomputationTestingItem::INDIVIDUALSATURATIONPRECOMPUTATIONTYPE,nextSaturationID);
				CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
				satCalculationJob->setCalculationConfiguration(calcConfig);
				satCalculationJob->setSaturationIndividualsAnalysationObserver(new CSaturationIndividualsAnalysingAdapter((CSaturationIndividualAnalysationObserver*)totallyPreCompItem));

				totallyPreCompItem->setSaturationCalculationJob(satCalculationJob);
				processCalculationJob(satCalculationJob,totallyPreCompItem,satTestingItem);
			}





			void CTotallyPrecomputationThread::createTripleIndexedIndividualsSaturationProcessingJob(CTotallyOntologyPrecomputationItem* totallyPreCompItem, cint64 startIndiId, cint64 endIndiId) {
				CApproximatedSaturationCalculationJob* satCalculationJob = nullptr;
				CConcreteOntology *onto = totallyPreCompItem->getOntology();
				CApproximatedSaturationCalculationJobGenerator satCalculationJobGenerator(onto);
				satCalculationJob = satCalculationJobGenerator.getApproximatedSaturationCalculationJob(0,totallyPreCompItem->getSaturationData());
				satCalculationJob->setSeparateSaturation(true);

				cint64 nextSaturationID = totallyPreCompItem->getNextSaturationID();
				CSaturationIndividualDataItem* satIndiDataItemLinker = nullptr;

				cint64 saturationIndividualCount = 0;
				for (cint64 indiId = startIndiId; indiId <= endIndiId; ++indiId) {

					CSaturationIndividualDataItem* satIndiDataItem = totallyPreCompItem->takeFreeSaturationIndividualDataItem();
					satIndiDataItem->initConceptSaturationTestingItem(nullptr, nextSaturationID);
					satIndiDataItemLinker = satIndiDataItem->append(satIndiDataItemLinker);

					satCalculationJob = satCalculationJobGenerator.extendApproximatedSaturationCalculationJobProcessing(indiId, satIndiDataItem, satCalculationJob);
					++saturationIndividualCount;
				}
				totallyPreCompItem->setSaturationIDIndividualDataItems(nextSaturationID,satIndiDataItemLinker);

				LOG(INFO,getLogDomain(),logTr("Scheduled %1 triple indexed individuals for saturation for ontology '%2'.").arg(saturationIndividualCount).arg(totallyPreCompItem->getOntology()->getTerminologyName()),this);

				CSaturationPrecomputationTestingItem* satTestingItem = new CSaturationPrecomputationTestingItem(totallyPreCompItem,CPrecomputationTestingItem::INDIVIDUALSATURATIONPRECOMPUTATIONTYPE,nextSaturationID);
				CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
				satCalculationJob->setCalculationConfiguration(calcConfig);
				satCalculationJob->setSaturationIndividualsAnalysationObserver(new CSaturationIndividualsAnalysingAdapter((CSaturationIndividualAnalysationObserver*)totallyPreCompItem));

				totallyPreCompItem->setSaturationCalculationJob(satCalculationJob);
				processCalculationJob(satCalculationJob,totallyPreCompItem,satTestingItem);
			}



			void CTotallyPrecomputationThread::createSaturationConstructionJob(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {

				CTBox* tBox = totallyPreCompItem->getOntology()->getDataBoxes()->getTBox();

				CConcreteOntology *onto = totallyPreCompItem->getOntology();
				CConcept* topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept* bottomConcept = onto->getDataBoxes()->getBottomConcept();
				CConcept* topDataRangeConcept = tBox->getTopDataRangeConcept();

				CConceptVector* conVec = tBox->getConceptVector();

				LOG(INFO,getLogDomain(),logTr("Determine concepts for saturation for ontology '%1'.").arg(totallyPreCompItem->getOntology()->getTerminologyName()),this);


				CApproximatedSaturationCalculationJob* satCalculationJob = nullptr;
				CApproximatedSaturationCalculationJobGenerator satCalculationJobGenerator(onto);



				bool saturateConcepts = CConfigDataReader::readConfigBoolean(totallyPreCompItem->getCalculationConfiguration(),"Konclude.Calculation.Optimization.ConceptSaturation",true);

				cint64 statExistsConceptCount = 0;
				cint64 statClassCount = 0;

				QList<CSaturationConceptDataItem*> existConList;
				QList<CSaturationConceptDataItem*> analyseExistSubsumerList;
				QList<CSaturationConceptDataItem*> allConItemList;
				QList<CSaturationConceptDataItem*> disjunctionCandidateAlternativeList;

				cint64 conCount = conVec->getItemCount();
				if (saturateConcepts) {
					for (cint64 conIdx = 1; conIdx < conCount; ++conIdx) {
						CConcept* concept = conVec->getData(conIdx);

						//if (CIRIName::getRecentIRIName(concept->getClassNameLinker()) == "file:F:/Projects/OIL/DAMLOilEd/ontologies/ka.daml#Publication") {
						//	bool bug = true;
						//}

						if (concept) {
							cint64 opCode = concept->getOperatorCode();
							if (opCode == CCSOME || opCode == CCALL || opCode == CCAQSOME || opCode == CCATLEAST || opCode == CCATMOST) {
								++statExistsConceptCount;
								bool negation = opCode == CCALL;
								CRole* role = concept->getRole();
								CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
								CConcept* opConcept = nullptr;
								bool opNegation = false;
								if (opLinker) {
									opConcept = opLinker->getData();
									if (opCode == CCATLEAST || opCode == CCATMOST) {
										opNegation = opLinker->isNegated();
									} else {
										opNegation = opLinker->isNegated()^negation;
									}
								} else {
									opConcept = topConcept;
									if (opCode == CCATLEAST || opCode == CCATMOST) {
										opNegation = false;
									} else {
										opNegation = negation;
									}
								}


								if (hasRoleRanges(totallyPreCompItem,role)) {
									CSaturationConceptDataItem* conItem = nullptr;
									conItem = totallyPreCompItem->getSaturationRoleSuccessorConceptDataItem(role,opConcept,opNegation,true);
									conItem->setPotentiallyExistInitializationConcept(true);
									existConList.append(conItem);
									if (role->isDataRole()) {
										conItem->setDataRangeConcept(true);
									}

									CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
									CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
									if (!conRefSatLinking) {
										conRefSatLinking = new CConceptSaturationReferenceLinkingData();
										conProcData->setConceptReferenceLinking(conRefSatLinking);
									}
									if (conRefSatLinking->getExistentialSuccessorConceptSaturationReferenceLinkingData() == nullptr) {
										conRefSatLinking->setExistentialSuccessorConceptSaturationReferenceLinkingData(conItem);
										allConItemList.append(conItem);
									}

								} else {
									CSaturationConceptDataItem* conItem = nullptr;
									conItem = totallyPreCompItem->getSaturationConceptDataItem(opConcept,opNegation,true);
									conItem->setPotentiallyExistInitializationConcept(true);
									existConList.append(conItem);
									if (role->isDataRole()) {
										conItem->setDataRangeConcept(true);
									}

									CConceptProcessData* opConProcData = (CConceptProcessData*)opConcept->getConceptData();
									CConceptSaturationReferenceLinkingData* opConRefSatLinking = (CConceptSaturationReferenceLinkingData*)opConProcData->getConceptReferenceLinking();
									if (!opConRefSatLinking) {
										opConRefSatLinking = new CConceptSaturationReferenceLinkingData();
										opConProcData->setConceptReferenceLinking(opConRefSatLinking);
									}
									if (opConRefSatLinking->getConceptSaturationReferenceLinkingData(opNegation) == nullptr) {
										opConRefSatLinking->setSaturationReferenceLinkingData(conItem,opNegation);
										allConItemList.append(conItem);
									}
								}

							} else if (concept->hasClassName() || concept == topDataRangeConcept) {

								++statClassCount;
								CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
								CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
								if (!conRefSatLinking) {
									conRefSatLinking = new CConceptSaturationReferenceLinkingData();
									conProcData->setConceptReferenceLinking(conRefSatLinking);
								}
								if (conRefSatLinking->getConceptSaturationReferenceLinkingData(false) == nullptr) {
									CSaturationConceptDataItem* conItem = totallyPreCompItem->getSaturationConceptDataItem(concept,false,true);
									conRefSatLinking->setSaturationReferenceLinkingData(conItem,false);
									allConItemList.append(conItem);
								}
							}
						}
					}
				}

				disjunctionCandidateAlternativeList = allConItemList;
				while (!disjunctionCandidateAlternativeList.isEmpty()) {
					CSaturationConceptDataItem* conTestItem = disjunctionCandidateAlternativeList.takeFirst();
					QList<CSaturationConceptDataItem*> tempNewItems;
					extendDisjunctionsCandidateAlternativesItems(totallyPreCompItem,conTestItem,&tempNewItems);
					if (!tempNewItems.isEmpty()) {
						disjunctionCandidateAlternativeList.append(tempNewItems);
						allConItemList.append(tempNewItems);
					}
				}


				analyseExistSubsumerList = allConItemList;

				QList<CConceptNegationSaturationItem> processList;
				// identify all items, which aren't subsumers of other items
				for (QList<CSaturationConceptDataItem*>::const_iterator it = analyseExistSubsumerList.constBegin(), itEnd = analyseExistSubsumerList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					CConcept* concept = conTestItem->getSaturationConcept();
					bool conceptNegation = conTestItem->getSaturationNegation();
					processList.append(CConceptNegationSaturationItem(concept,conceptNegation,conTestItem));
				}
				analyseConceptSaturationSubsumerExistItems(totallyPreCompItem,processList);

				// find all potentially subsumer items
				for (QList<CSaturationConceptDataItem*>::const_iterator it = analyseExistSubsumerList.constBegin(), itEnd = analyseExistSubsumerList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					propagateSubsumerItemFlag(conTestItem);
				}

				// find all potentially role successors, for which back propagation is necessary
				for (QList<CSaturationConceptDataItem*>::const_iterator it = existConList.constBegin(), itEnd = existConList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					propagateExistInitializationFlag(conTestItem);
				}

				QList<CSaturationConceptDataItem*> leafConList;
				QList<CSaturationConceptDataItem*> leafExistConList;
				// collect all leaf nodes
				for (QList<CSaturationConceptDataItem*>::const_iterator it = allConItemList.constBegin(), itEnd = allConItemList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					if (!conTestItem->hasIndirectSuccessorsItems()) {
						if (!conTestItem->isPotentiallyExistInitializationConcept()) {
							leafConList.append(conTestItem);
						} else {
							leafExistConList.append(conTestItem);
						}
					}
				}



				QList<CSaturationConceptDataItem*>* orderedItemList = totallyPreCompItem->getSaturationOrderedItemList();
				QStack<CSaturationSaturationListItem> saturationItemStack;
				// order all concepts associated to the items for efficient Saturation testing, start with the real leaf items
				for (QList<CSaturationConceptDataItem*>::const_iterator it = leafConList.constBegin(), itEnd = leafConList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					if (!conTestItem->isOrderingQueued()) {
						orderItemsSaturationTesting(totallyPreCompItem,conTestItem,*orderedItemList,saturationItemStack);
					}
				}
				for (QList<CSaturationConceptDataItem*>::const_iterator it = leafExistConList.constBegin(), itEnd = leafExistConList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					if (!conTestItem->isOrderingQueued()) {
						orderItemsSaturationTesting(totallyPreCompItem,conTestItem,*orderedItemList,saturationItemStack);
					}
				}
				for (QList<CSaturationConceptDataItem*>::const_iterator it = allConItemList.constBegin(), itEnd = allConItemList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					if (!conTestItem->isOrderingQueued()) {
						orderItemsSaturationTesting(totallyPreCompItem,conTestItem,*orderedItemList,saturationItemStack);
					}
				}


				// set reference mode for saturation: substitute or copy 
				QListIterator<CSaturationConceptDataItem*> reverseOrderedIt(*orderedItemList);
				reverseOrderedIt.toBack();
				CBOXHASH<CConcept*,CConcept*>* triggerImpHash = totallyPreCompItem->getOntology()->getTBox()->getTriggerImplicationHash(false);
				while (reverseOrderedIt.hasPrevious()) {
					CSaturationConceptDataItem* conTestItem = reverseOrderedIt.previous();
					CSaturationConceptDataItem* referencedTestItem = conTestItem->getSpecialItemReference();
					if (referencedTestItem) {
						if (!conTestItem->isPotentiallyExistInitializationConcept() && !conTestItem->hasMultipleDirectPredecessorsItems() && (!triggerImpHash || !triggerImpHash->contains(conTestItem->getSaturationConcept()))) {
							conTestItem->setSpecialItemReferenceMode(CSaturationConceptDataItem::SATURATIONSUBSTITUTEMODE);
						} else {
							conTestItem->setSpecialItemReferenceMode(CSaturationConceptDataItem::SATURATIONCOPYMODE);
						}
					}
				}

				satCalculationJob = satCalculationJobGenerator.getApproximatedSaturationCalculationJob(0);


				cint64 mSaturationConceptCount = 0;
				for (QList<CSaturationConceptDataItem*>::const_iterator it = orderedItemList->constBegin(), itEnd = orderedItemList->constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conItem = *it;
					CConcept* concept = conItem->getSaturationConcept();
					bool negation = conItem->getSaturationNegation();
					CRole* role = conItem->getSaturationRoleRanges();
					conItem->setItemProcessingQueued(false);
					satCalculationJob = satCalculationJobGenerator.extendApproximatedSaturationCalculationJobConstruction(role,concept,negation,false,conItem,satCalculationJob);
					mSaturationConceptCount++;
				}

				LOG(INFO,getLogDomain(),logTr("Preparing saturation of %1 concepts for ontology '%2'.").arg(mSaturationConceptCount).arg(totallyPreCompItem->getOntology()->getTerminologyName()),this);

				CSaturationPrecomputationTestingItem* satTestingItem = new CSaturationPrecomputationTestingItem(totallyPreCompItem,CPrecomputationTestingItem::CONCEPTSATURATIONPRECOMPUTATIONTYPE);
				CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
				satCalculationJob->setCalculationConfiguration(calcConfig);
				satCalculationJob->setSaturationTaskPreyingAdapter(new CTaskPreyingAdapter((CSaturationObserver*)totallyPreCompItem));

				totallyPreCompItem->setSaturationCalculationJob(satCalculationJob);
				processCalculationJob(satCalculationJob,totallyPreCompItem,satTestingItem);
			}
















			bool CTotallyPrecomputationThread::precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CSaturationPrecomputationCalculatedCallbackEvent* pcce) {
				CTotallyOntologyPrecomputationItem* totallyPreCompItem = (CTotallyOntologyPrecomputationItem*)ontPreCompItem;
				if (preTestItem->getPrecomputationTestingType() == CPrecomputationTestingItem::OCCURRENCESTATISTICSSATURATIONPRECOMPUTATIONTYPE) {
					totallyPreCompItem->setSaturationOccurrenceStatisticsCollected(true);
					return true;
				} else if (preTestItem->getPrecomputationTestingType() == CPrecomputationTestingItem::CONCEPTSATURATIONPRECOMPUTATIONTYPE) {
					CSaturationPrecomputationTestingItem* satPreTestItem = (CSaturationPrecomputationTestingItem*)preTestItem;
					totallyPreCompItem->setConceptSaturationPrecomputationChecked(true);
					CPrecomputation* precomputation = totallyPreCompItem->getPrecomputation();
					CSaturationData* saturationData = totallyPreCompItem->getSaturationData();
					precomputation->setSaturationModelData(saturationData);
					totallyPreCompItem->setSaturationComputationRunning(false);

					CSaturationTaskData* satTaskData = (CSaturationTaskData*)saturationData;
					if (satTaskData) {
						CSatisfiableCalculationTask* satCalcTask = satTaskData->getSaturationTask();
						if (satCalcTask) {
							CProcessingDataBox* procDataBox = satCalcTask->getProcessingDataBox();
							if (procDataBox) {
								totallyPreCompItem->setNominalDelayedConceptsSaturationUpdateRequired(procDataBox->isDelayedNominalProcessingOccured());
							}
						}
					}

					return true;
				} else if (preTestItem->getPrecomputationTestingType() == CPrecomputationTestingItem::INDIVIDUALSATURATIONPRECOMPUTATIONTYPE) {
					CSaturationPrecomputationTestingItem* satPreTestItem = (CSaturationPrecomputationTestingItem*)preTestItem;
					cint64 saturationID = satPreTestItem->getSaturationID();
					totallyPreCompItem->releaseSaturationIDIndividualDataItems(saturationID);
					totallyPreCompItem->decIndividualSaturationRunningCount();
					if (!totallyPreCompItem->hasIndividualSaturationRunning()) {
						totallyPreCompItem->setSaturationComputationRunning(false);

						if (totallyPreCompItem->hasIndividualsSaturated() && totallyPreCompItem->hasALLIndividualsSaturationOrderd()) {

							if (!totallyPreCompItem->hasIndividualsSaturationCacheSynchronisation()) {
								totallyPreCompItem->setIndividualsSaturationCacheSynchronisation(true);
								QSet<CIndividualReference>* procReqIndiSet = totallyPreCompItem->getIncompletelyHandledIndividualSet();
								cint64 limit = totallyPreCompItem->getCurrentIncompletelyHandledIndividualRetrievalLimit();

								bool fullCompletionGraphConstruction = totallyPreCompItem->isFullCompletionGraphConstruction();
								if (fullCompletionGraphConstruction) {
									limit = -1;
								}
								mBackendAssocCache->getIncompletlyAssociationCachedIndividuals(ontPreCompItem->getOntology()->getOntologyID(), procReqIndiSet, limit);
								if (procReqIndiSet->size() > 0) {
									LOG(INFO, getLogDomain(), logTr("Preparing processing of first %1 insufficiently saturated individuals.").arg(procReqIndiSet->size()), getLogObject());

									CIndividualPrecomputationCoordinationHash* newIndiCompCoordHash = new CIndividualPrecomputationCoordinationHash();
									newIndiCompCoordHash->incUsageCount();
									for (CIndividualReference indiRef : *procReqIndiSet) {
										CIndividualPrecomputationCoordinationHashData*& data = (*newIndiCompCoordHash)[indiRef.getIndividualID()];
										if (!data) {
											data = new CIndividualPrecomputationCoordinationHashData();
											data->incUsageCount();
										}
										data->setNewlyRetrieved(true);
									}
									totallyPreCompItem->setCurrentIndividualComputationCoordinationHash(newIndiCompCoordHash);

								} else {
									totallyPreCompItem->setAllIncompletelyHandledIndividualsRetrieved(true);
								}
							}


							CConsistence* consistence = totallyPreCompItem->getConsistence();
							if (consistence) {
								consistence->setIndividualsRepresentativelyStored(true);
								if (!totallyPreCompItem->hasInsufficientSaturationIndividuals()) {
									consistence->setAllIndividualsRepresentativelyStored(true);
								}
							}
						}
					}
					return true;
				}
				return false;
			}



			bool CTotallyPrecomputationThread::determineMinimumNextConceptID(CConsistence* consistence, COntologyPrecomputationItem* ontPreCompItem) {
				CConsistenceTaskData* consTaskData = (CConsistenceTaskData*)consistence->getConsistenceModelData();
				if (consTaskData) {
					CSatisfiableCalculationTask* detTask = consTaskData->getDeterministicSatisfiableTask();
					CSatisfiableCalculationTask* nonDetTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
					CConceptVector* extendedConVec = nonDetTask->getProcessingDataBox()->getExtendedConceptVector(false);
					if (extendedConVec) {
						cint64 minNextId = extendedConVec->getItemCount();
						ontPreCompItem->getOntology()->getTBox()->setMinimalNextConceptID(minNextId);
						return true;
					}
				}
				return false;
			}


			bool CTotallyPrecomputationThread::precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CPrecomputationCalculatedCallbackEvent* pcce) {
				CTotallyOntologyPrecomputationItem* totallyPreCompItem = (CTotallyOntologyPrecomputationItem*)ontPreCompItem;
				if (preTestItem->getPrecomputationTestingType() == CPrecomputationTestingItem::CONSISTENCEPRECOMPUTATIONTYPE) {
					CConsistencePrecomputationTestingItem* consPreTestItem = (CConsistencePrecomputationTestingItem*)preTestItem;
					totallyPreCompItem->setIndividualStepRunning(false);
					totallyPreCompItem->setIndividualPrecomputationChecked(true);
					totallyPreCompItem->setConsistenceCheched(true);

					if (pcce->hasCalculationError()) {
						LOG(ERROR,getLogDomain(),logTr("Error in computation, consistency testing for ontology '%1' failed.").arg(ontPreCompItem->getOntology()->getTerminologyName()),getLogObject());
						totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
						totallyPreCompItem->getIndividualPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
					} else {
						CConsistence* consistence = totallyPreCompItem->getConsistence();
						consistence->setConsistenceModelData(totallyPreCompItem->getConsistenceData());
						consistence->setOntologyConsistent(pcce->getTestResultSatisfiable());
						determineMinimumNextConceptID(consistence, ontPreCompItem);

						if (consistence->isOntologyConsistent()) {
							QSet<CIndividualReference> procReqIndiSet;
							mBackendAssocCache->getIncompletlyAssociationCachedIndividuals(totallyPreCompItem->getOntology()->getOntologyID(), &procReqIndiSet, -1);
							if (!procReqIndiSet.isEmpty()) {
								LOG(WARN, getLogDomain(), logTr("Representative cache has incompletely processed individuals although individual computation finished and ontology is consistent."), getLogObject());
							}
						}
					}
					return true;

				} else if (preTestItem->getPrecomputationTestingType() == CPrecomputationTestingItem::INDIVIDUALPRECOMPUTATIONTYPE) {
					CIndividualPrecomputationTestingItem* indiPreTestItem = (CIndividualPrecomputationTestingItem*)preTestItem;
					totallyPreCompItem->decIndividualComputationRunningCount();

					if (indiPreTestItem->getIndividualComputationCoordinationHash()) {
						decrementUsageFromIndividualPrecomputationCoordinationHash(indiPreTestItem->getIndividualComputationCoordinationHash(), totallyPreCompItem);
					}

					QSet<CIndividualReference>* currentIndiComputationSet = totallyPreCompItem->getCurrentIndividualComputationSet();
					for (CIndividualReference indiRef : *indiPreTestItem->getIndividualComputationList()) {
						currentIndiComputationSet->remove(indiRef);
					}

					if (!pcce->getTestResultSatisfiable()) {
						totallyPreCompItem->setIndividualPrecomputationClashed(true);
					}

					if (!totallyPreCompItem->isIndividualComputationRunning() && totallyPreCompItem->hasAllIndividualPrecomputationCreated()) {
						totallyPreCompItem->setIndividualStepRunning(false);
						totallyPreCompItem->setIndividualComputationRunning(false);
						totallyPreCompItem->setIndividualPrecomputationChecked(true);
						totallyPreCompItem->setConsistenceCheched(true);

						CConsistence* consistence = totallyPreCompItem->getConsistence();
						if (consistence) {
							consistence->setAllIndividualsRepresentativelyStored(true);
						}
					}

					if (pcce->hasCalculationError()) {
						if (!totallyPreCompItem->hasIndividualPrecomputationClashed()) {
							LOG(ERROR, getLogDomain(), logTr("Error in computation, individual precomputation for ontology '%1' failed.").arg(ontPreCompItem->getOntology()->getTerminologyName()), getLogObject());
							totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
							totallyPreCompItem->getIndividualPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
						}
					} else if (indiPreTestItem->getTaskPreyingAdapter()) {
						CConsistence* consistence = totallyPreCompItem->getConsistence();
						if (consistence) {
							consistence->setConsistenceModelData(totallyPreCompItem->getConsistenceData());
							consistence->setOntologyConsistent(!totallyPreCompItem->hasIndividualPrecomputationClashed());
							determineMinimumNextConceptID(consistence, ontPreCompItem);
						}
					}
					return true;

				} else if (preTestItem->getPrecomputationTestingType() == CPrecomputationTestingItem::CONCEPTCYCLEPRECOMPUTATIONTYPE) {
					CConceptCyclePrecomputationTestingItem* conCyclePreTestItem = (CConceptCyclePrecomputationTestingItem*)preTestItem;
					CConceptCycleData* conCycleData = conCyclePreTestItem->getConceptCycleData();
					CBOXSET<CConcept*>* classCycleSet = conCycleData->getCycleClassSet();

					if (pcce->hasCalculationError()) {
						LOG(ERROR,getLogDomain(),logTr("Error in computation, cyclic concept graph caching for ontology '%1' failed.").arg(ontPreCompItem->getOntology()->getTerminologyName()),getLogObject());
						totallyPreCompItem->setConsistenceCheched(true);
						totallyPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
					} else {
						if (pcce->getTestResultSatisfiable()) {
							// insert into cache
							CTempMemoryPoolContainerAllocationManager tmpMemMan(mContext.getMemoryPoolProvider());
							CContextBase* tmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(&tmpMemMan,&tmpMemMan);

							CCACHINGLIST<CCacheValue>* cacheValueList = CObjectParameterizingAllocator< CCACHINGLIST<CCacheValue>,CContext* >::allocateAndConstructAndParameterize(&tmpMemMan,tmpContext);
							for (CBOXSET<CConcept*>::const_iterator it = classCycleSet->constBegin(), itEnd = classCycleSet->constEnd(); it != itEnd; ++it) {
								CConcept* classConcept(*it);
								CCacheValue cacheValue(classConcept->getConceptTag(),(cint64)classConcept,CCacheValue::CACHEVALTAGANDCONCEPT);
								cacheValueList->append(cacheValue);
							}

							CReuseCompletionGraphCacheEntryExpandWriteData* writeData = CObjectAllocator<CReuseCompletionGraphCacheEntryExpandWriteData>::allocateAndConstruct(&tmpMemMan);
							writeData->initExpandWriteData(cacheValueList,cacheValueList,conCyclePreTestItem->getJobInstantiation());
							mReuseCompletionGraphCacheWriter->writeExpandCache(writeData,tmpMemMan.takeMemoryPools());

						}
					}

					if (!totallyPreCompItem->hasRemainingConceptCyclePrecomputation() && !totallyPreCompItem->hasCurrentPrecomputationTesting()) {
						totallyPreCompItem->setConceptCyclePrecomputationChecked(true);
					}

				}
				return false;
			}


			bool CTotallyPrecomputationThread::finishOntologyPrecomputation(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CConcreteOntology* ontology = totallyPreCompItem->getOntology();
				if (totallyPreCompItem->areAllStepFinished()) {
					totallyPreCompItem->setPrecomputationFinished(true);

					CPrecomputation* precomputation = ontology->getPrecomputation();
					precomputation->setPrecomputed(true);
				}

				CConsistenceStatisticsCollectionStrings* classifStatCollStrings = totallyPreCompItem->getConsistenceStatisticsCollectionStrings();
				CPrecomputation* precomputation = ontology->getPrecomputation();
				if (precomputation) {
					precomputation->setPrecomputationStatistics(classifStatCollStrings);
				}

				CConsistence* consistence = ontology->getConsistence();
				if (consistence) {
					consistence->setConsistenceStatistics(classifStatCollStrings);
				}

				totallyPreCompItem->doPrecomputationFinishedCallback(true);

				mActiveOntItemSet.remove(totallyPreCompItem);
				mInactiveOntItemSet.insert(totallyPreCompItem);
				return true;
			}






		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
