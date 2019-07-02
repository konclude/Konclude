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

#include "CPreprocessingThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CPreprocessingThread::CPreprocessingThread(CReasonerManager *reasonerManager) : CThread("Preprocessing-Thread") {
				mReasoner = reasonerManager;
				mCalculationManager = 0;
				mCurrRunningTestParallelCount = 0;
				mConfMaxTestParallelCount = 1;
				mStatCalculatingJobs = 0;

				startThread(QThread::HighPriority);
			}


			CPreprocessingThread::~CPreprocessingThread() {
				stopThread();
			}


			bool CPreprocessingThread::preprocess(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callback) {
				postEvent(new CPreprocessOntologyEvent(ontology,config,requirementList,callback));
				return true;
			}


			bool CPreprocessingThread::preprocess(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList) {
				CBlockingCallbackData callback;
				postEvent(new CPreprocessOntologyEvent(ontology,config,requirementList,&callback));
				callback.waitForCallback();
				return true;
			}

			bool CPreprocessingThread::callbackPreprocessed(CConcreteOntology* ontology, CCallbackData* callback) {
				bool callbackPreprocessedOnto = false;
				CPreprocessing* preprocessing = ontology->getPreprocessing();
				if (preprocessing && preprocessing->isPreprocessed()) {
					callbackPreprocessedOnto = true;
					if (callback) {
						callback->doCallback();
					}
				} else {
					CBlockingCallbackData callbackBlock;
					postEvent(new CCallbackPreprocessedOntologyEvent(ontology,&callbackBlock,callback));
					callbackBlock.waitForCallback();
					CCallbackDataContext* callbackContext = callbackBlock.getCallbackDataContext();
					if (callbackContext) {
						CPreprocessingCallbackDataContext* possCallbackContext = dynamic_cast<CPreprocessingCallbackDataContext*>(callbackContext);
						callbackPreprocessedOnto = possCallbackContext->isPreprocessed();
					}
				}
				return callbackPreprocessedOnto;
			}



			bool CPreprocessingThread::canProcessMoreTests() {
				return mCurrRunningTestParallelCount < mConfMaxTestParallelCount;
			}


			bool CPreprocessingThread::doNextPendingTests() {
				bool createdNewTests = false;
				bool nextTestCreated = true;
				while (canProcessMoreTests() && nextTestCreated) {
					nextTestCreated = false;
					bool workCreated = createNextTest();
					if (workCreated) {
						nextTestCreated = true;
						createdNewTests = true;
					} else {
						nextTestCreated = false;
					}
				}
				return createdNewTests;
			}

			CPreprocessingThread* CPreprocessingThread::processCalculationJob(CSatisfiableCalculationJob* job, COntologyPreprocessingItem* preCompItem, CPreprocessingTestingItem* preTestItem) {

				CPreprocessingCalculatedCallbackEvent* callbackEvent = new CPreprocessingCalculatedCallbackEvent(this,job,preTestItem);
				preCompItem->getPreprocessingTestingItemSet()->insert(preTestItem);

				mCurrRunningTestParallelCount++;
				++mStatCalculatingJobs;
				if (!mCalculationManager) {
					mCalculationManager = mReasoner->getCalculationManager();
				}
				if (mCalculationManager) {
					mCalculationManager->calculateJob(job,callbackEvent);
				} else {
					mReasoner->reasoningSatisfiableCalcualtionJob(job,callbackEvent);
				}
				return this;
			}


			COntologyPreprocessingItem* CPreprocessingThread::initializeOntologyPreprocessingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				COntologyPreprocessingItem* item = new COntologyPreprocessingItem();
				item->initPreprocessingItem(ontology,config);
				return item;
			}

			bool CPreprocessingThread::addOntologyPreprocessingCallback(COntologyPreprocessingItem* item, CCallbackData* callback) {
				item->addPreprocessingFinishedCallback(callback);
				return true;
			}


			bool CPreprocessingThread::addOntologyPreprocessingRequirements(COntologyPreprocessingItem* item, COntologyProcessingRequirement* requirement) {
				item->addProcessingRequirement(requirement);
				return true;
			}


			bool CPreprocessingThread::processCustomsEvents(QEvent::Type type, CCustomEvent* event) {
				if (CThread::processCustomsEvents(type,event)) {
					return true;
				} else if (type == CPreprocessOntologyEvent::EVENTTYPE) {
					CPreprocessOntologyEvent* poe = (CPreprocessOntologyEvent*)event;

					CCallbackData* callbackData = poe->getCallbackData();
					CConfigurationBase* configBase = poe->getConfiguration();
					CConcreteOntology* ontology = poe->getOntology();
					QList<COntologyProcessingRequirement*>* requirementList = poe->getPreprocessingRequirementList();


					COntologyPreprocessingItem* item = mOntItemHash.value(ontology);
					if (!item) {						
						item = initializeOntologyPreprocessingItem(ontology,configBase);
						mOntItemHash.insert(ontology,item);
						mActiveOntItemSet.insert(item);
						mProcessingOntItemList.append(item);
					} else {
						if (!mActiveOntItemSet.contains(item)) {
							mInactiveOntItemSet.remove(item);
							mActiveOntItemSet.insert(item);
							mProcessingOntItemList.append(item);
						}
					}

					for (QList<COntologyProcessingRequirement*>::const_iterator it = requirementList->constBegin(), itEnd = requirementList->constEnd(); it != itEnd; ++it) {
						COntologyProcessingRequirement* requ(*it);
						addOntologyPreprocessingRequirements(item,requ);
					}

					if (callbackData) {
						addOntologyPreprocessingCallback(item,callbackData);
					}
					doNextPendingTests();
					return true;

				} else if (type == CCallbackPreprocessedOntologyEvent::EVENTTYPE) {
					CCallbackPreprocessedOntologyEvent* cpoe = (CCallbackPreprocessedOntologyEvent*)event;

					CConcreteOntology* ontology = cpoe->getOntology();
					CCallbackData* preprocessedCallbackData = cpoe->getPreprocessedCallbackData();
					CCallbackData* installedCallbackData = cpoe->getCallbackPossibleCallbackData();
					COntologyPreprocessingItem* item = mOntItemHash.value(ontology);
					if (item) {
						addOntologyPreprocessingCallback(item,preprocessedCallbackData);
						if (installedCallbackData) {
							CPreprocessingCallbackDataContext* callbackDataContext = new CPreprocessingCallbackDataContext(true,ontology);
							installedCallbackData->setCallbackDataContext(callbackDataContext);
							installedCallbackData->doCallback();
						}						
					} else {
						if (installedCallbackData) {
							CPreprocessingCallbackDataContext* callbackDataContext = new CPreprocessingCallbackDataContext(false,ontology);
							installedCallbackData->setCallbackDataContext(callbackDataContext);
							installedCallbackData->doCallback();
						}						
					}
					return true;

				} else if (type == CPreprocessingCalculatedCallbackEvent::EVENTTYPE) {
					CPreprocessingCalculatedCallbackEvent* pcce = (CPreprocessingCalculatedCallbackEvent*)event;

					--mCurrRunningTestParallelCount;

					CPreprocessingTestingItem* testingItem = pcce->getTestingItem();
					COntologyPreprocessingItem* ontPreproItem = testingItem->getOntologyPreprocessingItem();

					ontPreproItem->getPreprocessingTestingItemSet()->remove(testingItem);
					preprocessingTested(ontPreproItem,testingItem,pcce);
					doNextPendingTests();

					return true;
				}
				return false;
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
