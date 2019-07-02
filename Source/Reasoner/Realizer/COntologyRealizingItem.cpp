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

#include "COntologyRealizingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COntologyRealizingItem::COntologyRealizingItem() {
				mTmpProcDataLinker = nullptr;
			}

			COntologyRealizingItem* COntologyRealizingItem::initRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				mOntology = ontology;
				mConfig = config;
				mRealizingFinishedFlag = false;
				mTmpProcDataLinker = nullptr;
				return this;
			}

			COntologyRealizingItem* COntologyRealizingItem::addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement, COntologyRealizingDynamicRequirmentProcessingData* procData) {
				ontoRequirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSUNSUPPORTED);
				return this;
			}

			COntologyRealizingItem* COntologyRealizingItem::logRequirementProcessingStartStatistics(COntologyRealizingDynamicRequirmentProcessingStatistics* procStats) {
				return this;
			}

			COntologyRealizingItem* COntologyRealizingItem::logRequirementProcessingFinishStatistics(COntologyRealizingDynamicRequirmentProcessingStatistics* procStats) {
				return this;
			}


			bool COntologyRealizingItem::isRealizingFinished() {
				return mRealizingFinishedFlag;
			}

			COntologyRealizingItem* COntologyRealizingItem::setRealizingFinished(bool finished) {
				mRealizingFinishedFlag = finished;
				return this;
			}

			COntologyRealizingItem* COntologyRealizingItem::doRealizingFinishedCallback(bool doCallbacks) {
				if (doCallbacks) {
					mCallbackExecuter.doCallback();
				}
				return this;
			}

			COntologyRealizingItem* COntologyRealizingItem::addRealizingFinishedCallback(CCallbackData* callback) {
				mCallbackExecuter.addCallbackData(callback);
				return this;
			}

			CConcreteOntology* COntologyRealizingItem::getOntology() {
				return mOntology;
			}


			CConfigurationBase* COntologyRealizingItem::getConfiguration() {
				return mConfig;
			}

			QSet<CRealizingTestingItem*>* COntologyRealizingItem::getRealizingTestingItemSet() {
				return &mTestItemSet;
			}


			cint64 COntologyRealizingItem::getCurrentRealizingTestingCount() {
				return mTestItemSet.count();
			}


			bool COntologyRealizingItem::hasCurrentRealizingTestingCount() {
				return mTestItemSet.count() > 0;
			}


			CLinker<COntologyRealizingDynamicRequirmentProcessingData*>* COntologyRealizingItem::createRequirementProcessingDataLinker(COntologyRealizingDynamicRequirmentProcessingData* procData) {
				CLinker<COntologyRealizingDynamicRequirmentProcessingData*>* procDataLinker = mTmpProcDataLinker;
				if (procDataLinker) {
					mTmpProcDataLinker = mTmpProcDataLinker->getNext();
				}
				if (!procDataLinker) {
					procDataLinker = new CLinker<COntologyRealizingDynamicRequirmentProcessingData *>();
				}
				procDataLinker->setNext(nullptr);
				if (procData) {
					procDataLinker->init(procData);
				}
				return procDataLinker;
			}

			COntologyRealizingItem* COntologyRealizingItem::releaseRequirementProcessingDataLinker(CLinker<COntologyRealizingDynamicRequirmentProcessingData*>* procDataLinker) {
				mTmpProcDataLinker = procDataLinker->append(mTmpProcDataLinker);
				return this;
			}



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
