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

#include "CAnswererThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnswererThread::CAnswererThread(CReasonerManager *reasonerManager) : CThread("Answerer-Thread") {
				mReasonerManager = reasonerManager;
				mCalculationManager = 0;
				mCurrRunningTestParallelCount = 0;
				mCurrRunningRequirementParallelCount = 0;
				mConfMaxTestParallelCount = 1;
				mConfMinTestParallelCount = 15;
				mStatCalculationJobsSubmitted = 0;
				mConfDirectSubmission = false;
				mOccStatsCacheReader = nullptr;
				mAnswererContext = new CAnswererContext(this);
				startThread(QThread::HighPriority);
			}




			CAnswererThread::~CAnswererThread() {
				stopThread();
			}




			CAnswererThread* CAnswererThread::initializeAnswerer(CConfiguration* config) {


				bool configErrorFlag = false;
				cint64 processorCount = 1;
				QString processorCountString = CConfigDataReader::readConfigString(config, "Konclude.Calculation.ProcessorCount", QString(), &configErrorFlag);
				if (!configErrorFlag) {
					if (processorCountString == "AUTO") {
						processorCount = CThread::idealThreadCount();
					}
					else {
						qint64 convertedWorkerCount = processorCountString.toInt(&configErrorFlag);
						if (configErrorFlag) {
							processorCount = convertedWorkerCount;
						}
					}
				}

				bool mulConfigErrorFlag = false;
				cint64 multiplicator = CConfigDataReader::readConfigInteger(config, "Konclude.Calculation.Answering.MultipliedUnitsParallelTestingCalculationCount", 1, &mulConfigErrorFlag);
				mConfMaxTestParallelCount = processorCount*multiplicator;
				bool maxConfigErrorFlag = false;
				cint64 maxParallel = CConfigDataReader::readConfigInteger(config, "Konclude.Calculation.Answering.MaximumParallelTestingCalculationCount", 1, &maxConfigErrorFlag);
				if (!maxConfigErrorFlag) {
					if (!mulConfigErrorFlag) {
						mConfMaxTestParallelCount = qMin(mConfMaxTestParallelCount, maxParallel);
					}
					else {
						mConfMaxTestParallelCount = maxParallel;
					}
				}

				return this;
			}






			bool CAnswererThread::handleAnswering(CAnsweringHandler* answeringHandler, CComplexAnsweringQuery* query, CCallbackData* callback) {
				postEvent(new CAnsweringInitializingEvent(answeringHandler, query, callback));
				return true;
			}







			bool CAnswererThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (CThread::processCustomsEvents(type,event)) {
					return true;
				} else if (type == CAnsweringInitializingEvent::EVENTTYPE) {
					CAnsweringInitializingEvent* aie = (CAnsweringInitializingEvent *)event;
					CAnsweringHandler* answeringHandler = aie->getAnsweringHandler();
					CCallbackData* callback = aie->getCallbackData();
					CComplexAnsweringQuery* query = aie->getQuery();
					CAnsweringHandlerData* answeringHandlingData = new CAnsweringHandlerData(answeringHandler, callback);
					mHandlerHandlerDataHash.insert(answeringHandler, answeringHandlingData);
					mQueuedAnsweringHandlerList.append(answeringHandlingData);
					mReadWriteLock.lockForWrite();
					mManagedAnsweringHandlerSet.insert(answeringHandlingData);
					mReadWriteLock.unlock();
					answeringHandlingData->mQueued = true;
					answeringHandler->addAnsweringComplexQuery(query);
					answeringHandler->initializeProcessing(mAnswererContext);
					continueCalculationCreation();
					return true;
				} else if (type == CAnsweringMessageEvent::EVENTTYPE) {
					CAnsweringMessageEvent* ame = (CAnsweringMessageEvent *)event;
					CAnsweringHandler* answeringHandler = ame->getAnsweringHandler();
					CAnsweringMessageData* messageData = ame->getMessageData();
					CMemoryPool* memoryPool = ame->getMemoryPool();
					answeringHandler->processCalculationMessage(messageData, mAnswererContext);
					if (!memoryPool) {
						//delete messageData;
					} else {
						mAnswererContext->getMemoryPoolProvider()->releaseMemoryPool(memoryPool);
					}
					continueCalculationCreation();
					return true;
				} else if (type == CAnsweringCalculationCompletedEvent::EVENTTYPE) {
					--mCurrRunningTestParallelCount;
					CAnsweringCalculationCompletedEvent* ame = (CAnsweringCalculationCompletedEvent *)event;
					CAnsweringHandler* answeringHandler = ame->getAnsweringHandler();
					CAnsweringMessageData* messageData = ame->getMessageData();
					answeringHandler->processCalculationMessage(messageData, mAnswererContext);
					CAnsweringHandlerData* handlerData = mHandlerHandlerDataHash.value(answeringHandler);
					handlerData->mJobProcessingCount--;
					if (!handlerData->mQueued) {
						handlerData->mQueued = true;
						mQueuedAnsweringHandlerList.prepend(handlerData);
					}
					//delete messageData;
					continueCalculationCreation();
					return true;
				} else if (type == CAnsweringRequirementCompletedEvent::EVENTTYPE) {
					//sleep(6);
					--mCurrRunningRequirementParallelCount;
					CAnsweringRequirementCompletedEvent* ame = (CAnsweringRequirementCompletedEvent *)event;
					CAnsweringHandler* answeringHandler = ame->getAnsweringHandler();
					CAnsweringMessageData* messageData = ame->getMessageData();
					answeringHandler->processRequirementMessage(messageData, mAnswererContext);
					CAnsweringHandlerData* handlerData = mHandlerHandlerDataHash.value(answeringHandler);
					handlerData->mRequirmentProcessingCount--;
					if (!handlerData->mQueued) {
						handlerData->mQueued = true;
						mQueuedAnsweringHandlerList.prepend(handlerData);
					}
					//delete messageData;
					continueCalculationCreation();
					return true;			
				} else if (type == CAnsweringHandlerPreparationEvent::EVENTTYPE) {
					CAnsweringHandlerPreparationEvent* ape = (CAnsweringHandlerPreparationEvent *)event;
					CAnsweringHandler* answeringHandler = ape->getAnsweringHandler();
					if (answeringHandler) {
						answeringHandler->prepareProcessing(mAnswererContext);
					}
					ape->getCallbackData()->doCallback();
					return true;
				}
				return false;
			}


			CAnsweringMessageDataObserver* CAnswererThread::sendMessage(CAnsweringHandler* handler, CAnsweringMessageData* message, CMemoryPool* memoryPool) {
				postEvent(new CAnsweringMessageEvent(handler, message, memoryPool));
				return this;
			}



			CAnsweringCalculationHandler* CAnswererThread::postMessage(CAnsweringHandler* handler, CAnsweringMessageData* message) {
				postEvent(new CAnsweringMessageEvent(handler, message, nullptr));
				return this;
			}


			bool CAnswererThread::continueCalculationCreation() {
				bool createdCalculationJobs = false;
				if (!mQueuedAnsweringHandlerList.isEmpty() && mCurrRunningTestParallelCount < mConfMaxTestParallelCount) {
					CAnsweringHandlerData* handlerData = mQueuedAnsweringHandlerList.first();
					bool calculationCreated = false;
					bool calculationCreationContinued = true;
					while (calculationCreationContinued && mCurrRunningTestParallelCount < mConfMaxTestParallelCount) {
						calculationCreationContinued = handlerData->mAnsweringHandler->continueCalculationCreation(mAnswererContext);
						calculationCreated |= calculationCreationContinued;
					}
					if (!calculationCreationContinued) {
						handlerData->mQueued = false;
						mQueuedAnsweringHandlerList.removeFirst();
						if (handlerData->mAnsweringHandler->isAnsweringFinished(mAnswererContext)) {
							mHandlerHandlerDataHash.remove(handlerData->mAnsweringHandler);
							if (handlerData->mCallbackData) {
								handlerData->mCallbackData->doCallback();
							}
							mProcessingAnsweringHandlerSet.remove(handlerData);
							mReadWriteLock.lockForWrite();
							mManagedAnsweringHandlerSet.remove(handlerData);
							mReadWriteLock.unlock();
							delete handlerData;
						}
					} else {
						mProcessingAnsweringHandlerSet.remove(handlerData);
					}
					if (calculationCreated) {
						handlerData->mProcessing = true;
						mProcessingAnsweringHandlerSet.insert(handlerData);
					}

				}
				submitCalculationJobs();
				return createdCalculationJobs;
			}



			CAnswererThread* CAnswererThread::submitCalculationJobs() {
				if (!mJobCallbackList.isEmpty()) {
					mCalculationManager->calculateJobs(mJobCallbackList);
					mJobCallbackList.clear();
				}
				return this;
			}



			CAnsweringCalculationHandler* CAnswererThread::ensureRequirements(CAnsweringHandler* handler, CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& requirementList, CAnsweringMessageData* finishedMessage) {
				++mStatRequirmentSubmitted;
				++mCurrRunningRequirementParallelCount;
				CAnsweringHandlerData* handlerData = mHandlerHandlerDataHash.value(handler);
				handlerData->mRequirmentProcessingCount++;
				CAnsweringRequirementCompletedEvent* callbackEvent = new CAnsweringRequirementCompletedEvent(this, handler, finishedMessage);
				mReasonerManager->prepareOntology(ontology,requirementList,callbackEvent);
				return this;
			}

			CAnsweringCalculationHandler* CAnswererThread::answerSubQuery(CAnsweringHandler* handler, CQuery* query, CAnsweringMessageData* finishedMessage) {
				++mStatRequirmentSubmitted;
				++mCurrRunningRequirementParallelCount;
				CAnsweringHandlerData* handlerData = mHandlerHandlerDataHash.value(handler);
				handlerData->mRequirmentProcessingCount++;
				CAnsweringRequirementCompletedEvent* callbackEvent = new CAnsweringRequirementCompletedEvent(this, handler, finishedMessage);
				mReasonerManager->reasoningQuery(query, callbackEvent);
				return this;
			}

			CAnsweringCalculationHandler* CAnswererThread::postCalculation(CAnsweringHandler* handler, CSatisfiableCalculationJob* satCalcJob, CAnsweringMessageData* finishedMessage, CCallbackDataContext* callbackContext) {
				++mStatCalculationJobsSubmitted;
				CAnsweringHandlerData* handlerData = mHandlerHandlerDataHash.value(handler);
				handlerData->mJobProcessingCount++;
				CAnsweringCalculationCompletedEvent* callbackEvent = new CAnsweringCalculationCompletedEvent(this,handler,finishedMessage, callbackContext);
				if (!mCalculationManager) {
					mCalculationManager = mReasonerManager->getCalculationManager();
				}
				++mCurrRunningTestParallelCount;
				if (!mCalculationManager || mConfDirectSubmission || mCurrRunningTestParallelCount - 1 < mConfMinTestParallelCount / 2) {
					if (mCalculationManager) {
						mCalculationManager->calculateJob(satCalcJob, callbackEvent);
					} else {
						mReasonerManager->reasoningSatisfiableCalcualtionJob(satCalcJob, callbackEvent);
					}
				} else {
					mJobCallbackList.append(QPair<CCalculationJob*, CCallbackData*>(satCalcJob, callbackEvent));
				}
				return this;
			}


			COccurrenceStatisticsCacheReader* CAnswererThread::getOccurrenceStatisticsCacheReader() {
				if (!mOccStatsCacheReader) {
					COccurrenceStatisticsCache* occStatsCache = mReasonerManager->getOccurrenceStatisticsCache();
					if (occStatsCache) {
						mOccStatsCacheReader = occStatsCache->createCacheReader();
					}
				}
				return mOccStatsCacheReader;
			}


			CAnswererThread* CAnswererThread::prepareAnswering(CAnsweringHandler* answeringHandler, CCallbackData* callback) {
				postEvent(new CAnsweringHandlerPreparationEvent(answeringHandler, callback));
				return this;
			}



			CAnsweringProgress* CAnswererThread::getAnsweringProgress() {
				CAnsweringProgress newAnsweringProg;
				double percentAvg = 0;
				cint64 percentCount = 0;
				mReadWriteLock.lockForRead();

				for (CAnsweringHandlerData* answeringHandlerData : mManagedAnsweringHandlerSet) {
					CAnsweringProgress* answeringProgress = answeringHandlerData->mAnsweringHandler->getAnsweringProgress();
					if (answeringProgress) {
						newAnsweringProg.setTotalInstances(newAnsweringProg.getTotalInstances() + answeringProgress->getTotalInstances());
						newAnsweringProg.setTestedInstances(newAnsweringProg.getTestedInstances() + answeringProgress->getTestedInstances());
						newAnsweringProg.setRemainingMilliSeconds(qMax(newAnsweringProg.getRemainingMilliSeconds(), answeringProgress->getRemainingMilliSeconds()));
						percentAvg += answeringProgress->getProgessPercent();
						++percentCount;
					}
				}
				mReadWriteLock.unlock();
				if (percentCount != 0) {
					percentAvg /= (double)percentCount;
				}
				newAnsweringProg.setProgessPercent(percentAvg);
				mAnsweringProgress = newAnsweringProg;
				return &mAnsweringProgress;

			}


			CAnswererThread* CAnswererThread::addAnsweringStatistics(CAnsweringStatistics* stats) {
				mReadWriteLock.lockForRead();
				for (CAnsweringHandlerData* answeringHandlerData : mManagedAnsweringHandlerSet) {
					answeringHandlerData->mAnsweringHandler->addAnsweringStatistics(stats, true);
				}
				mReadWriteLock.unlock();
				return this;
			}




		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
