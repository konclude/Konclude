/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "COntologyPrecomputationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			COntologyPrecomputationItem::COntologyPrecomputationItem() {
			}

			COntologyPrecomputationItem* COntologyPrecomputationItem::initPrecomputationItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				mOntology = ontology;
				mConfig = config;
				mPrecomputationFinishedFlag = false;
				return this;
			}

			COntologyPrecomputationItem* COntologyPrecomputationItem::addPrecomputationRequirement(COntologyProcessingRequirement* ontoRequirement) {
				ontoRequirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSUNSUPPORTED);
				return this;
			}

			bool COntologyPrecomputationItem::isPrecomputationFinished() {
				return mPrecomputationFinishedFlag;
			}

			COntologyPrecomputationItem* COntologyPrecomputationItem::setPrecomputationFinished(bool finished, bool doCallbacks) {
				mPrecomputationFinishedFlag = finished;
				doPrecomputationFinishedCallback(doCallbacks);
				return this;
			}

			COntologyPrecomputationItem* COntologyPrecomputationItem::doPrecomputationFinishedCallback(bool doCallbacks) {
				if (doCallbacks) {
					mCallbackExecuter.doCallback();
				}
				return this;
			}

			COntologyPrecomputationItem* COntologyPrecomputationItem::addPrecomputationFinishedCallback(CCallbackData* callback) {
				mCallbackExecuter.addCallbackData(callback);
				return this;
			}

			CConcreteOntology* COntologyPrecomputationItem::getOntology() {
				return mOntology;
			}


			CConfigurationBase* COntologyPrecomputationItem::getConfiguration() {
				return mConfig;
			}

			QSet<CPrecomputationTestingItem*>* COntologyPrecomputationItem::getPrecomputationTestingItemSet() {
				return &mTestItemSet;
			}


			cint64 COntologyPrecomputationItem::getCurrentPrecomputationTestingCount() {
				return mTestItemSet.count();
			}


			bool COntologyPrecomputationItem::hasCurrentPrecomputationTesting() {
				return mTestItemSet.count() > 0;
			}

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
