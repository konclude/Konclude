/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CRealizerThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CRealizerThread::CRealizerThread(CReasonerManager *reasonerManager) : CThread("Realizer-Thread") {
				mReasoner = reasonerManager;
				mCalculationManager = 0;
				mCurrRunningTestParallelCount = 0;
				mConfMaxTestParallelCount = 1;
				mStatCalculatingJobs = 0;

				startThread(QThread::HighPriority);
			}


			CRealizerThread::~CRealizerThread() {
				stopThread();
			}


			bool CRealizerThread::realize(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callback) {
				postEvent(new CRealizeOntologyEvent(ontology,config,requirementList,callback));
				return true;
			}


			bool CRealizerThread::realize(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList) {
				CBlockingCallbackData callback;
				postEvent(new CRealizeOntologyEvent(ontology,config,requirementList,&callback));
				callback.waitForCallback();
				return true;
			}

			bool CRealizerThread::callbackRealized(CConcreteOntology* ontology, CCallbackData* callback) {
				bool callbackRealizedOnto = false;
				CRealization* realization = ontology->getRealization();
				if (realization && realization->isRealized()) {
					callbackRealizedOnto = true;
					if (callback) {
						callback->doCallback();
					}
				} else {
					CBlockingCallbackData callbackBlock;
					postEvent(new CCallbackRealizedOntologyEvent(ontology,&callbackBlock,callback));
					callbackBlock.waitForCallback();
					CCallbackDataContext* callbackContext = callbackBlock.getCallbackDataContext();
					if (callbackContext) {
						CRealizingCallbackDataContext* possCallbackContext = dynamic_cast<CRealizingCallbackDataContext*>(callbackContext);
						callbackRealizedOnto = possCallbackContext->isRealized();
					}
				}
				return callbackRealizedOnto;
			}



			bool CRealizerThread::canProcessMoreTests() {
				return mCurrRunningTestParallelCount < mConfMaxTestParallelCount;
			}


			bool CRealizerThread::doNextPendingTests() {
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

			CRealizerThread* CRealizerThread::processCalculationJob(CSatisfiableCalculationJob* job, COntologyRealizingItem* preCompItem, CRealizingTestingItem* preTestItem) {

				CRealizingCalculatedCallbackEvent* callbackEvent = new CRealizingCalculatedCallbackEvent(this,job,preTestItem);
				preCompItem->getRealizingTestingItemSet()->insert(preTestItem);

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


			COntologyRealizingItem* CRealizerThread::initializeOntologyRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				COntologyRealizingItem* item = new COntologyRealizingItem();
				item->initRealizingItem(ontology,config);
				return item;
			}

			bool CRealizerThread::addOntologyRealizingCallback(COntologyRealizingItem* item, CCallbackData* callback) {
				item->addRealizingFinishedCallback(callback);
				return true;
			}


			bool CRealizerThread::addOntologyRealizingRequirements(COntologyRealizingItem* item, COntologyProcessingRequirement* requirement) {
				item->addProcessingRequirement(requirement);
				return true;
			}


			bool CRealizerThread::processCustomsEvents(QEvent::Type type, CCustomEvent* event) {
				if (CThread::processCustomsEvents(type,event)) {
					return true;
				} else if (type == CRealizeOntologyEvent::EVENTTYPE) {
					CRealizeOntologyEvent* poe = (CRealizeOntologyEvent*)event;

					CCallbackData* callbackData = poe->getCallbackData();
					CConfigurationBase* configBase = poe->getConfiguration();
					CConcreteOntology* ontology = poe->getOntology();
					QList<COntologyProcessingRequirement*>* requirementList = poe->getRealizingRequirementList();


					COntologyRealizingItem* item = mOntItemHash.value(ontology);
					if (!item) {						
						item = initializeOntologyRealizingItem(ontology,configBase);
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
						addOntologyRealizingRequirements(item,requ);
					}

					if (callbackData) {
						addOntologyRealizingCallback(item,callbackData);
					}
					doNextPendingTests();
					return true;

				} else if (type == CCallbackRealizedOntologyEvent::EVENTTYPE) {
					CCallbackRealizedOntologyEvent* cpoe = (CCallbackRealizedOntologyEvent*)event;

					CConcreteOntology* ontology = cpoe->getOntology();
					CCallbackData* realizeedCallbackData = cpoe->getRealizedCallbackData();
					CCallbackData* installedCallbackData = cpoe->getCallbackPossibleCallbackData();
					COntologyRealizingItem* item = mOntItemHash.value(ontology);
					if (item) {
						addOntologyRealizingCallback(item,realizeedCallbackData);
						if (installedCallbackData) {
							CRealizingCallbackDataContext* callbackDataContext = new CRealizingCallbackDataContext(true,ontology);
							installedCallbackData->setCallbackDataContext(callbackDataContext);
							installedCallbackData->doCallback();
						}						
					} else {
						if (installedCallbackData) {
							CRealizingCallbackDataContext* callbackDataContext = new CRealizingCallbackDataContext(false,ontology);
							installedCallbackData->setCallbackDataContext(callbackDataContext);
							installedCallbackData->doCallback();
						}						
					}
					return true;

				} else if (type == CRealizingCalculatedCallbackEvent::EVENTTYPE) {
					CRealizingCalculatedCallbackEvent* pcce = (CRealizingCalculatedCallbackEvent*)event;

					--mCurrRunningTestParallelCount;

					CRealizingTestingItem* testingItem = pcce->getTestingItem();
					COntologyRealizingItem* ontPreproItem = testingItem->getOntologyRealizingItem();

					ontPreproItem->getRealizingTestingItemSet()->remove(testingItem);
					realizingTested(ontPreproItem,testingItem,pcce);
					doNextPendingTests();

					return true;
				}
				return false;
			}



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
