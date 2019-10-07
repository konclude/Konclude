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

#include "CSubsumptionClassifierThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CSubsumptionClassifierThread::CSubsumptionClassifierThread(CReasonerManager *reasonerManager) : CThread("Classifier-Thread") {
				reasoner = reasonerManager;
				calculationManager = 0;
				currRunningTestParallelCount = 0;
				confMaxTestParallelCount = 1;
				confMinTestParallelCount = 15;
				mStatCalculatingJobs = 0;
				mClassifierActive = false;
				statistics = new CClassifierStatistics();
				startThread(QThread::HighPriority);
			}


			CSubsumptionClassifierThread::~CSubsumptionClassifierThread() {
				stopThread();
			}


			bool CSubsumptionClassifierThread::classify(CConcreteOntology *ontology, CConfigurationBase *config, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callback) {				
				postEvent(new CClassifyOntologyEvent(ontology,config,requirementList,callback));
				return true;
			}


			bool CSubsumptionClassifierThread::classify(CConcreteOntology *ontology, CConfigurationBase *config, const QList<COntologyProcessingRequirement*>& requirementList) {
				CBlockingCallbackData callback;
				postEvent(new CClassifyOntologyEvent(ontology,config,requirementList,&callback));
				callback.waitForCallback();
				return true;
			}


			bool CSubsumptionClassifierThread::callbackClassified(CConcreteOntology *ontology, CCallbackData *callback) {
				bool isClassifyOnto = false;
				CTaxonomy *taxonomy = ontology->getConceptTaxonomy();
				if (taxonomy && taxonomy->isComplete()) {
					isClassifyOnto = true;
					if (callback) {
						callback->doCallback();
					}
				} else {
					CBlockingCallbackData callbackBlock;
					postEvent(new CCallbackClassifiedOntologyEvent(ontology,&callbackBlock,callback));
					callbackBlock.waitForCallback();
					CClassifyingCallbackDataContext *classifyingContext = dynamic_cast<CClassifyingCallbackDataContext *>(callbackBlock.getCallbackDataContext());
					if (classifyingContext) {
						isClassifyOnto = classifyingContext->isClassifying();
					}
				}
				return isClassifyOnto;
			}


			CClassificationMessageDataObserver* CSubsumptionClassifierThread::tellClassificationMessage(CConcreteOntology *ontology, CClassificationMessageData* messageData, CMemoryPool* memoryPool) {
				postEvent(new COntologyTellClassificationMessageEvent(ontology,messageData,memoryPool));
				return this;
			}




			CSubsumptionClassifierThread *CSubsumptionClassifierThread::addOntologyClassifiedCallback(CConcreteOntology *ontology, CCallbackData *callback) {
				COntologyClassificationItem *ontClassItem = ontItemHash.value(ontology,0);
				if (ontClassItem) {
					ontClassItem->addClassifiedCallback(callback);
				}
				return this;
			}

			CSubsumptionClassifierThread* CSubsumptionClassifierThread::addOntologyClassifyRequirements(CConcreteOntology *ontology, const QList<COntologyProcessingRequirement*>& requirementList) {
				COntologyClassificationItem *ontClassItem = ontItemHash.value(ontology,nullptr);
				if (ontClassItem) {
					ontClassItem->addOntologyProcessingRequirements(requirementList);
				}
				return this;
			}

			CSubsumptionClassifierThread *CSubsumptionClassifierThread::scheduleOntologyClassification(CConcreteOntology *ontology, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config) {
				COntologyClassificationItem *ontClassItem = new COntologyClassificationItem(config,statistics);
				ontItemList.append(ontClassItem);
				processingOntItemList.append(ontClassItem);
				ontItemHash.insert(ontology,ontClassItem);
				return this;
			}



			CSubsumptionClassifierThread* CSubsumptionClassifierThread::rescheduleOntologyClassification(CConcreteOntology *ontology) {
				COntologyClassificationItem *ontClassItem = ontItemHash.value(ontology);
				processingOntItemList.append(ontClassItem);
				return this;
			}


			bool CSubsumptionClassifierThread::isOntologyClassificationScheduled(CConcreteOntology *ontology) {
				return ontItemHash.contains(ontology);
			}



			bool CSubsumptionClassifierThread::canProcessMoreTests() {
				return currRunningTestParallelCount < confMaxTestParallelCount;
			}

			bool CSubsumptionClassifierThread::startProcessMoreTests() {
				return currRunningTestParallelCount <= confMinTestParallelCount;
			}

			bool CSubsumptionClassifierThread::doNextPendingTests() {
				bool createdNewTests = false;
				if (startProcessMoreTests()) {
					bool nextTestCreated = true;
					while (canProcessMoreTests() && nextTestCreated) {
						nextTestCreated = false;
						bool workCreated = createNextSubsumtionTest();
						if (workCreated) {
							nextTestCreated = true;
							createdNewTests = true;
						} else {
							nextTestCreated = false;
						}
					}
				}
				submitCalculationJobs();
				return createdNewTests;
			}

			CSubsumptionClassifierThread* CSubsumptionClassifierThread::submitCalculationJobs() {
				if (!mJobCallbackList.isEmpty()) {
					calculationManager->calculateJobs(mJobCallbackList);
					mJobCallbackList.clear();
				}
				return this;
			}


			CSubsumptionClassifierThread* CSubsumptionClassifierThread::processCalculationJob(CSatisfiableCalculationJob* job, COntologyClassificationItem *ontClassItem, CCallbackData* callback, bool directlySubmit) {
				currRunningTestParallelCount++;
				++mStatCalculatingJobs;
				if (!calculationManager) {
					calculationManager = reasoner->getCalculationManager();
				}
				if (!calculationManager || directlySubmit || currRunningTestParallelCount-1 < confMinTestParallelCount/2) {
					if (calculationManager) {
						calculationManager->calculateJob(job,callback);
					} else {
						reasoner->reasoningSatisfiableCalcualtionJob(job,callback);
					}
				} else {
					mJobCallbackList.append(QPair<CCalculationJob*,CCallbackData*>(job,callback));
				}
				return this;
			}

			bool CSubsumptionClassifierThread::processToldClassificationMessage(COntologyClassificationItem *ontClassItem, CClassificationMessageData* messageData, CMemoryPool* memoryPools) {
				return false;
			}


			bool CSubsumptionClassifierThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (CThread::processCustomsEvents(type,event)) {
					return true;
				} else if (type == CClassifyOntologyEvent::EVENTTYPE) {
					CClassifyOntologyEvent *coe = (CClassifyOntologyEvent *)event;
					CConcreteOntology* onto = coe->getOntology();
					CCallbackData *callback = coe->getCallbackData();
					CConfigurationBase* config = coe->getConfiguration();
					QList<COntologyProcessingRequirement*>* reqList = coe->getClassificationRequirementList();
					if (!isOntologyClassificationScheduled(onto)) {
						scheduleOntologyClassification(onto,nullptr,config);
						addOntologyClassifyRequirements(onto,*reqList);
					} else {
						rescheduleOntologyClassification(onto);
					}

					if (reqList) {
						addOntologyClassifyRequirements(onto,*reqList);
					}
					if (callback) {
						addOntologyClassifiedCallback(onto,callback);
					}
					doNextPendingTests();
					return true;
				} else if (type == COntologyTellClassificationMessageEvent::EVENTTYPE) {
					COntologyTellClassificationMessageEvent *otcme = (COntologyTellClassificationMessageEvent *)event;
					CConcreteOntology *ontology = otcme->getOntology();
					COntologyClassificationItem *ontClassItem = ontItemHash.value(ontology,0);
					if (ontClassItem) {
						CClassificationMessageData* messageData = otcme->getMessageData();
						CMemoryPool* memoryPools = otcme->getMemoryPool();
						processToldClassificationMessage(ontClassItem,messageData,memoryPools);
						mContext.getMemoryPoolAllocationManager()->releaseTemporaryMemoryPools(memoryPools);
					}
					doNextPendingTests();
					return true;
				} else if (type == CTestCalculatedCallbackEvent::EVENTTYPE) {
					CTestCalculatedCallbackEvent *tcue = (CTestCalculatedCallbackEvent *)event;
					interpreteTestResults(tcue);
					currRunningTestParallelCount--;
					doNextPendingTests();
					return true;
				} else if (type == CCallbackClassifiedOntologyEvent::EVENTTYPE) {
					CCallbackClassifiedOntologyEvent *ccoe = (CCallbackClassifiedOntologyEvent *)event;
					CCallbackData *possCallback = ccoe->getCallbackPossibleCallbackData();
					COntologyClassificationItem *ontClassItem = ontItemHash.value(ccoe->getOntology(),0);
					if (possCallback) {
						CClassifyingCallbackDataContext *possCallbackContext = new CClassifyingCallbackDataContext(ontClassItem,ccoe->getOntology());
						possCallback->setCallbackDataContext(possCallbackContext);
						possCallback->doCallback();
					}
					CCallbackData *classifiedCallback = ccoe->getClassifiedCallbackData();
					if (classifiedCallback) {
						addOntologyClassifiedCallback(ccoe->getOntology(),classifiedCallback);
						rescheduleOntologyClassification(ccoe->getOntology());
					}
					return true;
				}
				return false;
			}


			CClassifierStatistics *CSubsumptionClassifierThread::getClassificationStatistics() {
				return statistics;
			}

			CClassificationProgress* CSubsumptionClassifierThread::getClassificationProgress() {
				return nullptr;
			}


			bool CSubsumptionClassifierThread::isClassifierActive() {
				return isThreadProcessingEvents();
			}

			


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
