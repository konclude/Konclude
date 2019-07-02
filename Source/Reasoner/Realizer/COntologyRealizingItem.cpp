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

#include "COntologyRealizingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COntologyRealizingItem::COntologyRealizingItem() {
			}

			COntologyRealizingItem* COntologyRealizingItem::initRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				mOntology = ontology;
				mConfig = config;
				mRealizingFinishedFlag = false;
				return this;
			}

			COntologyRealizingItem* COntologyRealizingItem::addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement) {
				ontoRequirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSUNSUPPORTED);
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


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
