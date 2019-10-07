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

#include "CRealizingTestingStep.h"
#include "COntologyRealizingItem.h"
#include "COntologyRealizingDynamicRequirmentProcessingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CRealizingTestingStep::CRealizingTestingStep(TESTINGSTEPTYPE testingStepType, COntologyProcessingStepData* ontProcStepData, COntologyRealizingItem* preproItem) {
				mTestingStepType = testingStepType;
				mOntologyPreproItem = preproItem;
				mProcessingStepData = ontProcStepData;
				mStepFinished = false;
			}

			COntologyRealizingItem* CRealizingTestingStep::getOntologyRealizingItem() {
				return mOntologyPreproItem;
			}

			CRealizingTestingStep* CRealizingTestingStep::setOntologyRealizingItem(COntologyRealizingItem* preproItem) {
				mOntologyPreproItem = preproItem;
				return this;
			}

			bool CRealizingTestingStep::isStepFinished() {
				return mStepFinished;
			}

			bool CRealizingTestingStep::areStepProcessingRequirementSatisfied() {
				return mProcessingStepData->getProcessingStep()->areAllRequirementsSatisfied(mOntologyPreproItem->getOntology());
			}


			bool CRealizingTestingStep::hasRequirements() {
				return !mRequirementStepDataList.isEmpty() || !mRequirementProcDataSet.isEmpty();
			}

			CRealizingTestingStep* CRealizingTestingStep::setStepFinished(bool finished) {
				mStepFinished = finished;
				cint64 installFlags = COntologyProcessingStatus::PSCOMPLETELYYPROCESSED;
				mProcessingStepData->getProcessingStatus()->setProcessingFlags(installFlags);
				return this;
			}

			QList<COntologyProcessingRequirement*>* CRealizingTestingStep::getRequirementList() {
				return &mRequirementStepDataList;
			}

			COntologyProcessingStepData* CRealizingTestingStep::getProcessingStepData() {
				return mProcessingStepData;
			}


			CRealizingTestingStep::TESTINGSTEPTYPE CRealizingTestingStep::getRealizingTestingType() {
				return mTestingStepType;
			}

			CRealizingTestingStep* CRealizingTestingStep::setProcessingStepData(COntologyProcessingStepData* processingData) {
				mProcessingStepData = processingData;
				return this;
			}

			CRealizingTestingStep* CRealizingTestingStep::addProcessingRequirement(COntologyProcessingRequirement* processingRequirement) {
				mRequirementStepDataList.append(processingRequirement);
				return this;
			}

			CRealizingTestingStep* CRealizingTestingStep::submitRequirementsUpdate(cint64 flags, bool clearRequirements, bool doCallbacks) {
				mProcessingStepData->getProcessingStatus()->setErrorFlags(flags);

				for (QList<COntologyProcessingRequirement*>::const_iterator it = mRequirementStepDataList.constBegin(), itEnd = mRequirementStepDataList.constEnd(); it != itEnd; ++it) {
					COntologyProcessingRequirement* requirement(*it);
					requirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED,flags);
				}

				if (clearRequirements) {
					mRequirementStepDataList.clear();
				}

				if (doCallbacks) {
					QSet<COntologyRealizingDynamicRequirmentProcessingData*> mUpdatingSet;
					mUpdatingSet.swap(mRequirementProcDataSet);
					for (QSet<COntologyRealizingDynamicRequirmentProcessingData*>::iterator it = mUpdatingSet.begin(), itEnd = mUpdatingSet.end(); it != itEnd; ++it) {
						COntologyRealizingDynamicRequirmentProcessingData* reqProcData(*it);
						reqProcData->decProcessingItemCount(this);
						reqProcData->clearAssociatedRelizationTestingStep(getRealizingTestingType());
						if (!reqProcData->hasCurrentProcessingItemCount()) {
							COntologyProcessingRequirement* procReq = reqProcData->getProcessingRequirement();
							if (procReq) {
								procReq->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, flags);
								reqProcData->setProcessingRequirement(nullptr);
							}
							COntologyRealizingDynamicRequirmentCallbackData* callbackData = reqProcData->getCallbackData();
							if (callbackData) {
								callbackData->decProcessingRequirmentCount();
								if (!callbackData->hasCurrentProcessingRequirmentCount()) {
									CCallbackData* callback = callbackData->getProcessingFinishedCallback();
									callback->doCallback();
									delete callbackData;
								}
								reqProcData->setCallbackData(nullptr);
							}
							delete reqProcData;
						}
					}
				}
				return this;
			}



			CRealizingTestingStep* CRealizingTestingStep::addRequirementProcessingData(COntologyRealizingDynamicRequirmentProcessingData* procData) {
				mRequirementProcDataSet.insert(procData);
				return this;
			}

			CRealizingTestingStep* CRealizingTestingStep::removeRequirementProcessingData(COntologyRealizingDynamicRequirmentProcessingData* procData) {
				if (mRequirementProcDataSet.contains(procData)) {
					mRequirementProcDataSet.remove(procData);
				}
				return this;
			}

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
