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

				mConsistenceStatCollStrings = new CConsistenceStatisticsCollectionStrings();

				mConfCollectProcessStatistics = CConfigDataReader::readConfigBoolean(config, "Konclude.Calculation.Precomputation.CollectProcessStatistics", false);

				return this;
			}


			bool COntologyPrecomputationItem::isProcessStatisticsCollectionConfigured() {
				return mConfCollectProcessStatistics;
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



			CPrecomputatorStatistics* COntologyPrecomputationItem::getPrecomputatorStatistics() {
				return &mPrecomputatorStatistics;
			}



			CConsistenceStatisticsCollectionStrings* COntologyPrecomputationItem::getConsistenceStatisticsCollectionStrings() {
				return mConsistenceStatCollStrings;
			}


			CConsistenceCalculationStatisticsCollection* COntologyPrecomputationItem::getCalculationStatisticsCollection() {
				CConsistenceCalculationStatisticsCollection* statColl = nullptr;
				if (!mReuseStatCollList.isEmpty()) {
					statColl = mReuseStatCollList.takeFirst();
				}
				if (!statColl && mConsistenceStatCollStrings) {
					statColl = mConsistenceStatCollStrings->createCalculationStatisticsCollection();
				}
				return statColl;
			}

			COntologyPrecomputationItem* COntologyPrecomputationItem::reuseCalculationStatisticsCollection(CConsistenceCalculationStatisticsCollection* statColl) {
				mReuseStatCollList.append(statColl);
				return this;
			}

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
