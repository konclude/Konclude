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

#include "CPrecomputationThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CPrecomputationThread::CPrecomputationThread(CReasonerManager *reasonerManager) : CThread("Precomputation-Thread") {
				mReasoner = reasonerManager;
				mCalculationManager = 0;
				mCurrRunningTestParallelCount = 0;
				mConfMaxTestParallelCount = 1;
				mStatCalculatingJobs = 0;

				startThread(QThread::HighPriority);
			}


			CPrecomputationThread::~CPrecomputationThread() {
				stopThread();
			}


			bool CPrecomputationThread::precompute(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callback) {
				postEvent(new CPrecomputeOntologyEvent(ontology,config,requirementList,callback));
				return true;
			}


			bool CPrecomputationThread::precompute(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList) {
				CBlockingCallbackData callback;
				postEvent(new CPrecomputeOntologyEvent(ontology,config,requirementList,&callback));
				callback.waitForCallback();
				return true;
			}

			bool CPrecomputationThread::callbackPrecomputed(CConcreteOntology* ontology, CCallbackData* callback) {
				bool callbackPecomputedOnto = false;
				CPrecomputation* precomputation = ontology->getPrecomputation();
				if (precomputation && precomputation->isPrecomputed()) {
					callbackPecomputedOnto = true;
					if (callback) {
						callback->doCallback();
					}
				} else {
					CBlockingCallbackData callbackBlock;
					postEvent(new CCallbackPrecomputedOntologyEvent(ontology,&callbackBlock,callback));
					callbackBlock.waitForCallback();
					CCallbackDataContext* callbackContext = callbackBlock.getCallbackDataContext();
					if (callbackContext) {
						CPrecomputationCallbackDataContext* possCallbackContext = dynamic_cast<CPrecomputationCallbackDataContext*>(callbackContext);
						callbackPecomputedOnto = possCallbackContext->isPrecomputing();
					}
				}
				return callbackPecomputedOnto;
			}



			bool CPrecomputationThread::canProcessMoreTests() {
				return mCurrRunningTestParallelCount < mConfMaxTestParallelCount;
			}


			bool CPrecomputationThread::doNextPendingTests() {
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

			CPrecomputationThread* CPrecomputationThread::processCalculationJob(CSatisfiableCalculationJob* job, COntologyPrecomputationItem* preCompItem, CPrecomputationTestingItem* preTestItem) {

				CConsistenceCalculationStatisticsCollection* statColl = nullptr;
				if (preCompItem->isProcessStatisticsCollectionConfigured()) {
					statColl = preCompItem->getCalculationStatisticsCollection();
					preTestItem->setStatisticCollection(statColl);
				}
				job->setCalclulationStatisticsCollector(statColl);

				CPrecomputationCalculatedCallbackEvent* callbackEvent = new CPrecomputationCalculatedCallbackEvent(this,job,preTestItem);
				preCompItem->getPrecomputationTestingItemSet()->insert(preTestItem);

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


			CPrecomputationThread* CPrecomputationThread::processCalculationJob(CApproximatedSaturationCalculationJob* job, COntologyPrecomputationItem* preCompItem, CPrecomputationTestingItem* preTestItem) {

				CConsistenceCalculationStatisticsCollection* statColl = nullptr;
				if (preCompItem->isProcessStatisticsCollectionConfigured()) {
					statColl = preCompItem->getCalculationStatisticsCollection();
					preTestItem->setStatisticCollection(statColl);
				}
				job->setCalclulationStatisticsCollector(statColl);

				CSaturationPrecomputationCalculatedCallbackEvent* callbackEvent = new CSaturationPrecomputationCalculatedCallbackEvent(this,job,preTestItem);
				preCompItem->getPrecomputationTestingItemSet()->insert(preTestItem);

				mCurrRunningTestParallelCount++;
				++mStatCalculatingJobs;
				if (!mCalculationManager) {
					mCalculationManager = mReasoner->getCalculationManager();
				}
				if (mCalculationManager) {
					mCalculationManager->calculateJob(job,callbackEvent);
				}
				return this;
			}


			COntologyPrecomputationItem* CPrecomputationThread::initializeOntologyPrecomputionItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				COntologyPrecomputationItem* item = new COntologyPrecomputationItem();
				item->initPrecomputationItem(ontology,config);
				return item;
			}

			bool CPrecomputationThread::addOntologyPrecomputionCallback(COntologyPrecomputationItem* item, CCallbackData* callback) {
				item->addPrecomputationFinishedCallback(callback);
				return this;
			}

			bool CPrecomputationThread::addOntologyPrecomputionRequirements(COntologyPrecomputationItem* item, COntologyProcessingRequirement* requirement) {
				item->addPrecomputationRequirement(requirement);
				return true;
			}

			bool CPrecomputationThread::processCustomsEvents(QEvent::Type type, CCustomEvent* event) {
				if (CThread::processCustomsEvents(type,event)) {
					return true;
				} else if (type == CPrecomputeOntologyEvent::EVENTTYPE) {
					CPrecomputeOntologyEvent* poe = (CPrecomputeOntologyEvent*)event;

					CCallbackData* callbackData = poe->getCallbackData();
					CConfigurationBase* configBase = poe->getConfiguration();
					CConcreteOntology* ontology = poe->getOntology();
					QList<COntologyProcessingRequirement*>* requirementList = poe->getProcessingRequirementList();


					COntologyPrecomputationItem* item = mOntItemHash.value(ontology);
					if (!item) {
						item = initializeOntologyPrecomputionItem(ontology,configBase);
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
						addOntologyPrecomputionRequirements(item,requ);
					}

					if (callbackData) {
						addOntologyPrecomputionCallback(item,callbackData);
					}
					doNextPendingTests();
					return true;

				} else if (type == CCallbackPrecomputedOntologyEvent::EVENTTYPE) {
					CCallbackPrecomputedOntologyEvent* cpoe = (CCallbackPrecomputedOntologyEvent*)event;

					CConcreteOntology* ontology = cpoe->getOntology();
					CCallbackData* precomputedCallbackData = cpoe->getPrecomputedCallbackData();
					CCallbackData* installedCallbackData = cpoe->getCallbackPossibleCallbackData();
					COntologyPrecomputationItem* item = mOntItemHash.value(ontology);
					if (item) {
						addOntologyPrecomputionCallback(item,precomputedCallbackData);
						if (installedCallbackData) {
							CPrecomputationCallbackDataContext* callbackDataContext = new CPrecomputationCallbackDataContext(true,ontology);
							installedCallbackData->setCallbackDataContext(callbackDataContext);
							installedCallbackData->doCallback();
						}						
					} else {
						if (installedCallbackData) {
							CPrecomputationCallbackDataContext* callbackDataContext = new CPrecomputationCallbackDataContext(false,ontology);
							installedCallbackData->setCallbackDataContext(callbackDataContext);
							installedCallbackData->doCallback();
						}						
					}
					return true;

				} else if (type == CPrecomputationCalculatedCallbackEvent::EVENTTYPE) {
					CPrecomputationCalculatedCallbackEvent* pcce = (CPrecomputationCalculatedCallbackEvent*)event;

					--mCurrRunningTestParallelCount;

					CPrecomputationTestingItem* testingItem = pcce->getTestingItem();
					COntologyPrecomputationItem* ontPreCompItem = testingItem->getOntologyPrecomputationItem();

					ontPreCompItem->getPrecomputationTestingItemSet()->remove(testingItem);
					CConsistenceCalculationStatisticsCollection* statisticCollection = testingItem->getUsedStatisticsCollection();
					precomputationTested(ontPreCompItem, testingItem, pcce);

					if (statisticCollection) {
						ontPreCompItem->reuseCalculationStatisticsCollection(statisticCollection);
					}

					doNextPendingTests();

					return true;

				} else if (type == CSaturationPrecomputationCalculatedCallbackEvent::EVENTTYPE) {
					CSaturationPrecomputationCalculatedCallbackEvent* spcce = (CSaturationPrecomputationCalculatedCallbackEvent*)event;

					--mCurrRunningTestParallelCount;

					CPrecomputationTestingItem* testingItem = spcce->getPrecomputationTestingItem();
					COntologyPrecomputationItem* ontPreCompItem = testingItem->getOntologyPrecomputationItem();

					ontPreCompItem->getPrecomputationTestingItemSet()->remove(testingItem);

					CConsistenceCalculationStatisticsCollection* statisticCollection = testingItem->getUsedStatisticsCollection();

					precomputationTested(ontPreCompItem,testingItem,spcce);

					if (statisticCollection) {
						ontPreCompItem->reuseCalculationStatisticsCollection(statisticCollection);
					}


					doNextPendingTests();

					return true;
				}
				return false;
			}



		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
