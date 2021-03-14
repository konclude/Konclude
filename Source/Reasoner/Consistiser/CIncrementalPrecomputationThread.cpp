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

#include "CIncrementalPrecomputationThread.h"



#ifndef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
	#define KONCLUDE_PRECOMPUTATION_SATURATION_STATISTIC_OUTPUT
#endif


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CIncrementalPrecomputationThread::CIncrementalPrecomputationThread(CReasonerManager *reasonerManager) : CPrecomputationThread(reasonerManager),CLogIdentifier("::Konclude::Reasoner::Kernel::Precomputator",this) {
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


			CIncrementalPrecomputationThread::~CIncrementalPrecomputationThread() {
			}


			COntologyPrecomputationItem* CIncrementalPrecomputationThread::initializeOntologyPrecomputionItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				CIncrementalOntologyPrecomputationItem* item = new CIncrementalOntologyPrecomputationItem();
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


				return item;
			}



			bool CIncrementalPrecomputationThread::createNextTest() {


				COntologyPrecomputationItem* loopOntPreCompItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !mProcessingOntItemList.isEmpty()) {
					COntologyPrecomputationItem* ontPreCompItem = mProcessingOntItemList.first();

					if (ontPreCompItem == loopOntPreCompItem) {
						// don't run into infinite loop without doing something
						break;
					}

					CIncrementalOntologyPrecomputationItem* incPreCompItem = (CIncrementalOntologyPrecomputationItem*)ontPreCompItem;
					

					bool failDebug = false;
					if (!workTestCreated && incPreCompItem->isConsistenceStepRequired()) {
						if (!incPreCompItem->isConsistenceStepFinished()) {
							if (incPreCompItem->areConsistenceStepProcessingRequirementSatisfied()) {


								if (!failDebug && !incPreCompItem->hasConsistenceCheckCreated()) {
									LOG(INFO,getLogDomain(),logTr("Incrementally precompute ontology consistency."),this);
									incPreCompItem->setConsistenceCheckCreated(true);
									workTestCreated = createConsistencePrecomputationCheck(incPreCompItem);
								}

								if (incPreCompItem->hasConsistenceCheched()) {
									incPreCompItem->getConsistencePrecomputationStep()->setStepFinished(true);

									if (incPreCompItem->getOntology()->getConsistence()->isOntologyConsistent()) {
										incPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
									} else {
										incPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL | COntologyProcessingStatus::PSINCONSITENT);
									}

								}

							} else {
								incPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
								incPreCompItem->getConsistencePrecomputationStep()->setStepFinished(true);
							}
						} else {
							incPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate();
						}
					}







					if (!workTestCreated && incPreCompItem->isIndividualStepRequired() && incPreCompItem->isConsistenceStepFinished()) {
						if (!incPreCompItem->isIndividualStepFinished()) {
							if (incPreCompItem->areIndividualStepProcessingRequirementSatisfied()) {


								if (!incPreCompItem->hasIndividualPrecomputationCreated()) {
									incPreCompItem->setIndividualPrecomputationCreated(true);
									workTestCreated = createIndividualPrecomputationCheck(incPreCompItem);
								}


								if (incPreCompItem->hasIndividualPrecomputationChecked() && !incPreCompItem->isIndividualComputationRunning()) {
									incPreCompItem->setIndividualStepRunning(false);
									incPreCompItem->getIndividualPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
									incPreCompItem->getIndividualPrecomputationStep()->setStepFinished(true);
								}

							} else {
								incPreCompItem->setIndividualStepRunning(false);
								incPreCompItem->getIndividualPrecomputationStep()->setStepFinished(true);
								incPreCompItem->getIndividualPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							incPreCompItem->getIndividualPrecomputationStep()->submitRequirementsUpdate();
						}
					}








					if (!workTestCreated && incPreCompItem->isSaturationStepRequired() && incPreCompItem->isConsistenceStepFinished() && incPreCompItem->isIndividualStepFinished()) {
						if (!incPreCompItem->isSaturationStepFinished()) {
							if (incPreCompItem->areSaturationStepProcessingRequirementSatisfied()) {

								incPreCompItem->getSaturationPrecomputationStep()->setStepFinished(true);
								incPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);

							} else {
								incPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							incPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate();
						}
					}







					if (!workTestCreated && incPreCompItem->isCycleStepRequired() && incPreCompItem->isConsistenceStepFinished()) {
						if (!incPreCompItem->isCycleStepFinished()) {
							if (incPreCompItem->areCycleStepProcessingRequirementSatisfied()) {
								incPreCompItem->getCyclePrecomputationStep()->setStepFinished(true);
								incPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
							} else {
								incPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
								incPreCompItem->getCyclePrecomputationStep()->setStepFinished(true);
							}
						} else {
							incPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate();
						}
					}


					if (!workTestCreated) {
						if (!incPreCompItem->hasRemainingProcessingRequirements()) {
							finishOntologyPrecomputation(incPreCompItem);
							mProcessingOntItemList.removeFirst();
						} else {
							mProcessingOntItemList.removeFirst();
							mProcessingOntItemList.append(incPreCompItem);
							loopOntPreCompItem = ontPreCompItem;
						}
					}
				}
				return workTestCreated;
			}









			bool CIncrementalPrecomputationThread::createIndividualPrecomputationCheck(CIncrementalOntologyPrecomputationItem* incPreCompItem) {
				CConcreteOntology* onto = incPreCompItem->getOntology();
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				return false;
			}




			QSet<CIndividual*>* CIncrementalPrecomputationThread::getDirectlyChangedIndividualsFromLastConsistentlyTestedOntology(CIncrementalOntologyPrecomputationItem* incPreCompItem) {
				CConcreteOntology* lastConOntVer = getLastConsistentlyTestedOntologyVersion(incPreCompItem);
				return getDirectlyChangedIndividuals(lastConOntVer,incPreCompItem);
			}



			QSet<CIndividual*>* CIncrementalPrecomputationThread::getDirectlyChangedIndividuals(CConcreteOntology* lastOntologyVersion, CIncrementalOntologyPrecomputationItem* incPreCompItem) {
				QSet<CIndividual*>* indiSet = incPreCompItem->getDirectlyChangedIndividualSet();
				if (!incPreCompItem->hasDirectlyChangedIndividualsIdentified()) {
					CConcreteOntology* ontology = incPreCompItem->getOntology();
					CIndividualVector* indiVec = ontology->getABox()->getIndividualVector(false);
					if (indiVec) {
						cint64 indiCount = indiVec->getItemCount();
						CIndividualVector* prevIndiVec = lastOntologyVersion->getABox()->getIndividualVector(false);
						if (!prevIndiVec) {
							for (cint64 i = 0; i < indiCount; ++i) {
								CIndividual* individual = indiVec->getData(i);
								if (individual) {
									indiSet->insert(individual);
								}
							}
						} else {
							for (cint64 i = 0; i < indiCount; ++i) {
								CIndividual* individual = indiVec->getData(i);
								if (individual) {
									CIndividual* prevIndividual = prevIndiVec->getData(i);
									if (individual != prevIndividual) {
										indiSet->insert(individual);
									}
								}
							}
						}
					}
					incPreCompItem->setDirectlyChangedIndividualsIdentified(true);
				}
				return indiSet;
			}




			CConcreteOntology* CIncrementalPrecomputationThread::getLastConsistentlyTestedOntologyVersion(CIncrementalOntologyPrecomputationItem* incPreCompItem) {
				CConcreteOntology* lastConsOntology = incPreCompItem->getLastConsistentlyTestedVersion();
				if (!lastConsOntology) {
					CConcreteOntology* ontology = incPreCompItem->getOntology();
					CConcreteOntology* prevOntology = ontology->getIncrementalRevisionData()->getPreviousOntologyVersion();
					while (prevOntology && !lastConsOntology) {
						CConsistence* prevConsistenceData = prevOntology->getConsistence();
						if (prevConsistenceData && prevConsistenceData->isOntologyConsistent()) {
							if (prevConsistenceData->areAllIndividualsRepresentativelyStored() || prevConsistenceData->hasConsistenceModelData()) {
								lastConsOntology = prevOntology;
							}
						}
						if (!lastConsOntology) {
							prevOntology = prevOntology->getIncrementalRevisionData()->getPreviousOntologyVersion();
						}
					}
					incPreCompItem->setLastConsistentlyTestedVersion(lastConsOntology);
				}
				return lastConsOntology;
			}



			
			bool CIncrementalPrecomputationThread::createConsistencePrecomputationCheck(CIncrementalOntologyPrecomputationItem* incPreCompItem) {
				CConcreteOntology* onto = incPreCompItem->getOntology();
				CSatisfiableCalculationJob* satCalcJob = nullptr;

				CConcreteOntology* lastConsistentlyTestedOnto = getLastConsistentlyTestedOntologyVersion(incPreCompItem);

				CIndividualVector* indiVec = onto->getABox()->getIndividualVector(false);
				cint64 indiCount = 0;
				if (indiVec) {
					indiCount = indiVec->getItemCount();
				}

				bool consistencyDetected = false;
				bool detectedConsistency = true;
				if (false) {
					consistencyDetected = true;
					detectedConsistency = true;
					LOG(INFO,getLogDomain(),logTr("Trivial consistency detected with merged individual."),getLogObject());
				} else {

					QSet<CIndividual*>* changedIndividualSet = getDirectlyChangedIndividuals(lastConsistentlyTestedOnto,incPreCompItem);

					QList<CIndividual*> indiList;
					for (QSet<CIndividual*>::const_iterator it = changedIndividualSet->constBegin(), itEnd = changedIndividualSet->constEnd(); it != itEnd; ++it) {
						CIndividual* individual(*it);
						indiList.append(individual);
					}

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
				incPreCompItem->setConsistence(consistence);

				if (consistencyDetected) {
					consistence->setOntologyConsistent(detectedConsistency);
					incPreCompItem->setConsistenceCheched(true);
					return false;
				} else {
					cint64 incRevID = incPreCompItem->getIncrementalExpansionID();
					CIncrementalConsistencePrecomputationTestingItem* consTestItem = new CIncrementalConsistencePrecomputationTestingItem(incPreCompItem,lastConsistentlyTestedOnto,incRevID,incPreCompItem);
					CCalculationConfigurationExtension* calcConfig = incPreCompItem->getCalculationConfiguration();
					satCalcJob->setCalculationConfiguration(calcConfig);
					satCalcJob->setSatisfiableTaskIncrementalConsistencyTestingAdapter(consTestItem->getIncrementalConsistencyTestingAdapter());

					processCalculationJob(satCalcJob,incPreCompItem,consTestItem);
					return true;
				}
			}









			bool CIncrementalPrecomputationThread::precomputationIndividualsRetrieved(COntologyPrecomputationItem* ontPreCompItem, CRetrievedPrecomputationIndividualsCallbackEvent* pcce) {
				return false;
			}


			bool CIncrementalPrecomputationThread::precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CSaturationPrecomputationCalculatedCallbackEvent* pcce) {
				return false;
			}


			bool CIncrementalPrecomputationThread::precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CPrecomputationCalculatedCallbackEvent* pcce) {
				CIncrementalOntologyPrecomputationItem* incPreCompItem = (CIncrementalOntologyPrecomputationItem*)ontPreCompItem;
				if (preTestItem->getPrecomputationTestingType() == CPrecomputationTestingItem::CONSISTENCEPRECOMPUTATIONTYPE) {
					CIncrementalConsistencePrecomputationTestingItem* consPreTestItem = (CIncrementalConsistencePrecomputationTestingItem*)preTestItem;
					incPreCompItem->setIndividualStepRunning(false);
					incPreCompItem->setIndividualPrecomputationChecked(true);
					incPreCompItem->setConsistenceCheched(true);

					if (pcce->hasCalculationError()) {
						LOG(ERROR,getLogDomain(),logTr("Error in computation, consistence testing for ontology '%1' failed.").arg(ontPreCompItem->getOntology()->getTerminologyName()),getLogObject());
						incPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
						incPreCompItem->getIndividualPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
					} else {
						CConsistence* consistence = incPreCompItem->getConsistence();
						consistence->setConsistenceModelData(incPreCompItem->getConsistenceData());
						consistence->setOntologyConsistent(pcce->getTestResultSatisfiable());
					}
					logIncrementalReasoningInfos(incPreCompItem);
					return true;
				}
				return false;
			}




			bool CIncrementalPrecomputationThread::logIncrementalReasoningInfos(CIncrementalOntologyPrecomputationItem* incPreCompItem) {
				CConsistenceData* consData = incPreCompItem->getConsistenceData();
				if (consData) {
					CIncrementalConsistenceTaskData* incConsTaskData = dynamic_cast<CIncrementalConsistenceTaskData*>(consData);
					if (incConsTaskData) {
						cint64 directlyChangedNodeCount = incPreCompItem->getDirectlyChangedIndividualSet()->count();
						cint64 deterministicallyChangedNodeCount = incConsTaskData->getDeterministicallyChangedNodeCount();
						cint64 indirectlyChangedNodeCount = incConsTaskData->getIndirectlyChangedCount();
						cint64 changedCompatibleNodeCount = incConsTaskData->getChangedCompatibleNodeCount();
						COntologyProcessingStatistics* ontProcStats = incPreCompItem->getConsistencePrecomputationStep()->getProcessingStepData()->getProcessingStatistics(true);
						ontProcStats->setProcessingCountStatisticValue("incremental-consistency-checking-directly-changed-individual-count",directlyChangedNodeCount);
						ontProcStats->setProcessingCountStatisticValue("incremental-consistency-checking-deterministically-changed-node-count",deterministicallyChangedNodeCount);
						ontProcStats->setProcessingCountStatisticValue("incremental-consistency-checking-indirectly-changed-node-count",indirectlyChangedNodeCount);
						ontProcStats->setProcessingCountStatisticValue("incremental-consistency-checking-changed-compatible-node-count",changedCompatibleNodeCount);
						ontProcStats->setProcessingCountStatisticValue("incremental-consistency-checking-added-node-count",incConsTaskData->getAddedNodeCount());
						ontProcStats->setProcessingCountStatisticValue("incremental-consistency-checking-new-total-node-count",incConsTaskData->getTotalNodeCount());
						ontProcStats->setProcessingCountStatisticValue("incremental-consistency-checking-previous-total-node-count",incConsTaskData->getPreviousNodeCount());
						ontProcStats->setProcessingCountStatisticValue("incremental-consistency-checking-time",incPreCompItem->getInitializationTime()->elapsed());
						LOG(INFO,getLogDomain(),logTr("Established consistency incrementally, %1 individuals directly changed, %2 nodes deterministically changed, %3 nodes indirectly changed.").arg(directlyChangedNodeCount).arg(deterministicallyChangedNodeCount).arg(indirectlyChangedNodeCount),this);
					}
				}
				return false;
			}


			bool CIncrementalPrecomputationThread::finishOntologyPrecomputation(CIncrementalOntologyPrecomputationItem* incPreCompItem) {
				if (incPreCompItem->areAllStepFinished()) {
					incPreCompItem->setPrecomputationFinished(true);
					CConcreteOntology* ontology = incPreCompItem->getOntology();

					CPrecomputation* precomputation = ontology->getPrecomputation();
					precomputation->setPrecomputed(true);
				}

				incPreCompItem->doPrecomputationFinishedCallback(true);

				mActiveOntItemSet.remove(incPreCompItem);
				mInactiveOntItemSet.insert(incPreCompItem);
				return true;
			}


		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
