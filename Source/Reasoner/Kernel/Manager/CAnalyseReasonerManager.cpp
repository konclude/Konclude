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

#include "CAnalyseReasonerManager.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {


				CAnalyseReasonerManager::CAnalyseReasonerManager(CWatchDog *threadWatchDog) : CReasonerManagerThread(threadWatchDog) {
					mWatchDog = threadWatchDog;
				}


				CAnalyseReasonerManager::~CAnalyseReasonerManager() {
				}



				void CAnalyseReasonerManager::logQueryProgressUpdates() {
					bool activeCalculation = false;
					CConcurrentTaskCalculationEnvironment* calcEnv = dynamic_cast<CConcurrentTaskCalculationEnvironment*>(mCalculationManager->getCalculationContext());
					if (calcEnv) {

						double approxRemainingTasksCount = calcEnv->getCalculationApproximatedRemainingTasksCount();
						cint64 computedTasksCount = calcEnv->getCalculationStatisticTasksProcessedCount();
						double computedTime = calcEnv->getCalculationComputionTime()/(double)mWorkControllerCount;
						if (approxRemainingTasksCount >= 1 && computedTasksCount >= 1 && computedTime >= 1) {

							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Calculation progress:"),this);

							QHash<QString,cint64>* mCalcStatHash = mCalculationManager->getCalculationStatistics();
							if (mCalcStatHash) {
								QString compTime = getLoggingCalculationStatisticsTimeSizeString(mCalcStatHash->value("calculation-computing-time")/(double)mWorkControllerCount);
								QString memCons = getLoggingCalculationStatisticsMemorySizeString(mCalcStatHash->value("calculation-memory-consumption"));
								QString statString = QString("Up to now calculation computing time: %1, calculation memory consumption: %2")
									.arg(compTime)
									.arg(memCons);
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString,this);
								delete mCalcStatHash;
							}


							double timePerTask = (double)computedTime/(double)computedTasksCount;
							double approxRemainingTime = approxRemainingTasksCount*timePerTask;
							QString approxRemainingTimeString;
							if (approxRemainingTime > 0) {
								approxRemainingTimeString = getLoggingCalculationStatisticsTimeSizeString(approxRemainingTime);
							} else {
								approxRemainingTimeString = QString("very very long");
							}
							double progress = computedTasksCount/(approxRemainingTasksCount+computedTasksCount)*100.;

							QString statString1 = QString("Processed tasks: %1, approximated remaining tasks: %2")
								.arg(computedTasksCount)
								.arg(approxRemainingTasksCount);
							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString1,this);
							QString statString2 = QString("Approximated remaining calculation time: %1, approximated calculation progress: %2 %\r\n")
								.arg(approxRemainingTimeString)
								.arg(progress);
							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString2,this);

							activeCalculation = true;

						}
					}
					if (classificationMan) {
						CClassificationProgress* classifProgress = classificationMan->getClassificationProgress();
						if (classifProgress) {
							if (classifProgress->getTestedClasses() != classifProgress->getTotalClasses() || classifProgress->getTestedSubsumptions() != classifProgress->getTotalSubsumptions()) {
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Classification progress:"),this);

								QString remainingTimeString = getLoggingCalculationStatisticsTimeSizeString(classifProgress->getRemainingMilliSeconds());
								QString statString1 = QString("Satisfiable tested classes: %1, approximated remaining classes to test: %2")
									.arg(classifProgress->getTestedClasses())
									.arg(classifProgress->getTotalClasses()-classifProgress->getTestedClasses());
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString1,this);

								if (classifProgress->getTotalSubsumptions() >= 0) {
									QString statString2 = QString("Tested subsumptions: %1, approximated remaining subsumptions to test: %2")
										.arg(classifProgress->getTestedSubsumptions())
										.arg(classifProgress->getTotalSubsumptions()-classifProgress->getTestedSubsumptions());
									LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString2,this);
								}

								QString statString3 = QString("Approximated remaining classification time: %1, approximated classification progress: %2 %\r\n")
									.arg(remainingTimeString)
									.arg(classifProgress->getProgessPercent());
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString3,this);

								activeCalculation = true;
							}
						}
					}

					if (mRealizationManager) {
						CRealizationProgress* realProgress = mRealizationManager->getRealizationProgress();
						if (realProgress) {
							if (realProgress->getTestedClasses() != realProgress->getTotalClasses()) {
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Realization progress:"),this);

								QString remainingTimeString = getLoggingCalculationStatisticsTimeSizeString(realProgress->getRemainingMilliSeconds());
								QString statString1 = QString("Tested instantiations: %1, approximated remaining instantiations to test: %2")
									.arg(realProgress->getTestedClasses())
									.arg(realProgress->getTotalClasses()-realProgress->getTestedClasses());
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString1,this);

								QString statString3 = QString("Approximated remaining realization time: %1, approximated realization progress: %2 %\r\n")
									.arg(remainingTimeString)
									.arg(realProgress->getProgessPercent());
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString3,this);

								activeCalculation = true;
							}
						}
					}

					if (mAnswererManager) {
						CAnsweringProgress* answeringProgress = mAnswererManager->getAnsweringProgress();
						if (answeringProgress) {
							if (answeringProgress->getTestedInstances() != answeringProgress->getTotalInstances()) {
								LOG(INFO, "::Konclude::Reasoner::Kernel::ReasonerManager", logTr("Answering progress:"), this);

								QString remainingTimeString = getLoggingCalculationStatisticsTimeSizeString(answeringProgress->getRemainingMilliSeconds());
								QString statString1 = QString("Tested instantiations: %1, approximated remaining instantiations to test: %2")
									.arg(answeringProgress->getTestedInstances())
									.arg(answeringProgress->getTotalInstances() - answeringProgress->getTestedInstances());
								LOG(INFO, "::Konclude::Reasoner::Kernel::ReasonerManager", statString1, this);

								QString statString3 = QString("Approximated remaining answering time: %1, approximated answering progress: %2 %\r\n")
									.arg(remainingTimeString)
									.arg(answeringProgress->getProgessPercent());
								LOG(INFO, "::Konclude::Reasoner::Kernel::ReasonerManager", statString3, this);

								activeCalculation = true;
							}
						}
					}

					if (activeCalculation) {

						if (mSatExpCache) {
							CCacheStatistics* cacheStats = mSatExpCache->getCacheStatistics();
							QString statString3 = QString("Satisfiable-Expander-Cache statistics: %1 cache entries, %2 memory consumption")
								.arg(cacheStats->getCacheEntriesCount())
								.arg(getLoggingCalculationStatisticsMemorySizeString(cacheStats->getMemoryConsumption()));
							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString3,this);

						}
						if (unsatCache) {
							CCacheStatistics* cacheStats = unsatCache->getCacheStatistics();
							QString statString3 = QString("Unsatisfiable-Cache statistics: %1 cache entries, %2 memory consumption\r\n")
								.arg(cacheStats->getCacheEntriesCount())
								.arg(getLoggingCalculationStatisticsMemorySizeString(cacheStats->getMemoryConsumption()));
							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString3,this);

						}

					}
				}



				double CAnalyseReasonerManager::getQueryProgress() {
					double progressPercent = 100;
					//double allTotalWork = 0;
					//double allClosedWork = 0;
					//CConcurrentWorkCalculationEnvironment *conCalcContext = dynamic_cast<CConcurrentWorkCalculationEnvironment *>(mCalculationManager->getCalculationContext());
					//CWorkRegistry *registry = conCalcContext->getWorkRegistry();
					//workSyncMutex.lock();
					//QList<CQuery *> queryList = mCalculatingQuerySet.values();
					//workSyncMutex.unlock();
					//foreach (CQuery *query, queryList) {
					//	CWorkProvidedQuery *workQuery = dynamic_cast<CWorkProvidedQuery *>(query);
					//	if (workQuery) {
					//		double totWorkCount = 0;
					//		double closWorkCount = 0;
					//		QLinkedList<CWork *> workList = workQuery->getWorkList();
					//		foreach (CWork *work, workList) {
					//			CWorkLock *workLock = registry->getWorkLock(work);
					//			if (workLock) {
					//				CCalcBoxProcessWork *calcWork = dynamic_cast<CCalcBoxProcessWork *>(work);
					//				if (calcWork) {
					//					closWorkCount += calcWork->getApproximatedClosedWorkCount();
					//					totWorkCount += calcWork->getApproximatedTotalWorkCount();
					//				}
					//				workLock->releaseLock();
					//				delete workLock;
					//			}
					//		}
					//		allTotalWork += totWorkCount;	
					//		allClosedWork += closWorkCount;	
					//	}
					//}
					//if (allTotalWork > 0) {
					//	progressPercent = allClosedWork / allTotalWork * 100.;
					//}			
					return progressPercent;
				}

				double CAnalyseReasonerManager::getQueryTotalWorkCount() {
					double allTotalWork = 0;
					//double allClosedWork = 0;
					//CConcurrentWorkCalculationEnvironment *conCalcContext = dynamic_cast<CConcurrentWorkCalculationEnvironment *>(mCalculationManager->getCalculationContext());
					//CWorkRegistry *registry = conCalcContext->getWorkRegistry();
					//workSyncMutex.lock();
					//QList<CQuery *> queryList = mCalculatingQuerySet.values();
					//workSyncMutex.unlock();
					//foreach (CQuery *query, queryList) {
					//	CWorkProvidedQuery *workQuery = dynamic_cast<CWorkProvidedQuery *>(query);
					//	if (workQuery) {
					//		double totWorkCount = 0;
					//		double closWorkCount = 0;
					//		QLinkedList<CWork *> workList = workQuery->getWorkList();
					//		foreach (CWork *work, workList) {
					//			CWorkLock *workLock = registry->getWorkLock(work);
					//			if (workLock) {
					//				CCalcBoxProcessWork *calcWork = dynamic_cast<CCalcBoxProcessWork *>(work);
					//				if (calcWork) {
					//					closWorkCount += calcWork->getApproximatedClosedWorkCount();
					//					totWorkCount += calcWork->getApproximatedTotalWorkCount();
					//				}
					//				workLock->releaseLock();
					//				delete workLock;
					//			}
					//		}
					//		allTotalWork += totWorkCount;	
					//		allClosedWork += closWorkCount;	
					//	}
					//}
					return allTotalWork;
				}

				double CAnalyseReasonerManager::getQueryCalculatedWorkCount() {
					double allTotalWork = 0;
					double allClosedWork = 0;
					//CConcurrentWorkCalculationEnvironment *conCalcContext = dynamic_cast<CConcurrentWorkCalculationEnvironment *>(mCalculationManager->getCalculationContext());
					//CWorkRegistry *registry = conCalcContext->getWorkRegistry();
					//workSyncMutex.lock();
					//QList<CQuery *> queryList = mCalculatingQuerySet.values();
					//workSyncMutex.unlock();
					//foreach (CQuery *query, queryList) {
					//	CWorkProvidedQuery *workQuery = dynamic_cast<CWorkProvidedQuery *>(query);
					//	if (workQuery) {
					//		double totWorkCount = 0;
					//		double closWorkCount = 0;
					//		QLinkedList<CWork *> workList = workQuery->getWorkList();
					//		foreach (CWork *work, workList) {
					//			CWorkLock *workLock = registry->getWorkLock(work);
					//			if (workLock) {
					//				CCalcBoxProcessWork *calcWork = dynamic_cast<CCalcBoxProcessWork *>(work);
					//				if (calcWork) {
					//					closWorkCount += calcWork->getApproximatedClosedWorkCount();
					//					totWorkCount += calcWork->getApproximatedTotalWorkCount();
					//				}
					//				workLock->releaseLock();
					//				delete workLock;
					//			}
					//		}
					//		allTotalWork += totWorkCount;	
					//		allClosedWork += closWorkCount;	
					//	}
					//}
					return allClosedWork;
				}



			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
