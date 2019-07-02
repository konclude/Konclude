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

#include "COntologyPreprocessingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			COntologyPreprocessingItem::COntologyPreprocessingItem() {
			}

			COntologyPreprocessingItem* COntologyPreprocessingItem::initPreprocessingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				mOntology = ontology;
				mConfig = config;
				mPreprocessingFinishedFlag = false;
				return this;
			}

			COntologyPreprocessingItem* COntologyPreprocessingItem::addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement) {
				ontoRequirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSUNSUPPORTED);
				return this;
			}

			bool COntologyPreprocessingItem::isPreprocessingFinished() {
				return mPreprocessingFinishedFlag;
			}

			COntologyPreprocessingItem* COntologyPreprocessingItem::setPreprocessingFinished(bool finished) {
				mPreprocessingFinishedFlag = finished;
				return this;
			}

			COntologyPreprocessingItem* COntologyPreprocessingItem::doPreprocessingFinishedCallback(bool doCallbacks) {
				if (doCallbacks) {
					mCallbackExecuter.doCallback();
				}
				return this;
			}

			COntologyPreprocessingItem* COntologyPreprocessingItem::addPreprocessingFinishedCallback(CCallbackData* callback) {
				mCallbackExecuter.addCallbackData(callback);
				return this;
			}

			CConcreteOntology* COntologyPreprocessingItem::getOntology() {
				return mOntology;
			}


			CConfigurationBase* COntologyPreprocessingItem::getConfiguration() {
				return mConfig;
			}

			QSet<CPreprocessingTestingItem*>* COntologyPreprocessingItem::getPreprocessingTestingItemSet() {
				return &mTestItemSet;
			}


			cint64 COntologyPreprocessingItem::getCurrentPreprocessingTestingCount() {
				return mTestItemSet.count();
			}


			bool COntologyPreprocessingItem::hasCurrentPreprocessingTestingCount() {
				return mTestItemSet.count() > 0;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
