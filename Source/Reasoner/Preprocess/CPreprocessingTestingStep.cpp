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

#include "CPreprocessingTestingStep.h"
#include "COntologyPreprocessingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CPreprocessingTestingStep::CPreprocessingTestingStep(TESTINGSTEPTYPE testingStepType, COntologyProcessingStepData* ontProcStepData, COntologyPreprocessingItem* preproItem) {
				mTestingStepType = testingStepType;
				mOntologyPreproItem = preproItem;
				mProcessingStepData = ontProcStepData;
				mStepFinished = false;
			}

			COntologyPreprocessingItem* CPreprocessingTestingStep::getOntologyPreprocessingItem() {
				return mOntologyPreproItem;
			}

			CPreprocessingTestingStep* CPreprocessingTestingStep::setOntologyPreprocessingItem(COntologyPreprocessingItem* preproItem) {
				mOntologyPreproItem = preproItem;
				return this;
			}

			bool CPreprocessingTestingStep::isStepFinished() {
				return mStepFinished;
			}

			bool CPreprocessingTestingStep::areStepProcessingRequirementSatisfied() {
				return mProcessingStepData->getProcessingStep()->areAllRequirementsSatisfied(mOntologyPreproItem->getOntology());
			}


			bool CPreprocessingTestingStep::hasRequirements() {
				return !mRequirementStepDataList.isEmpty();
			}

			CPreprocessingTestingStep* CPreprocessingTestingStep::setStepFinished(bool finished) {
				mStepFinished = finished;
				cint64 installFlags = COntologyProcessingStatus::PSCOMPLETELYYPROCESSED;
				mProcessingStepData->getProcessingStatus()->setProcessingFlags(installFlags);
				return this;
			}

			QList<COntologyProcessingRequirement*>* CPreprocessingTestingStep::getRequirementList() {
				return &mRequirementStepDataList;
			}

			COntologyProcessingStepData* CPreprocessingTestingStep::getProcessingStepData() {
				return mProcessingStepData;
			}


			CPreprocessingTestingStep::TESTINGSTEPTYPE CPreprocessingTestingStep::getPreprocessingTestingType() {
				return mTestingStepType;
			}

			CPreprocessingTestingStep* CPreprocessingTestingStep::setProcessingStepData(COntologyProcessingStepData* processingData) {
				mProcessingStepData = processingData;
				return this;
			}

			CPreprocessingTestingStep* CPreprocessingTestingStep::addProcessingRequirement(COntologyProcessingRequirement* processingRequirement) {
				mRequirementStepDataList.append(processingRequirement);
				return this;
			}

			CPreprocessingTestingStep* CPreprocessingTestingStep::submitRequirementsUpdate(cint64 flags, bool clearRequirements) {
				mProcessingStepData->getProcessingStatus()->setErrorFlags(flags);

				for (QList<COntologyProcessingRequirement*>::const_iterator it = mRequirementStepDataList.constBegin(), itEnd = mRequirementStepDataList.constEnd(); it != itEnd; ++it) {
					COntologyProcessingRequirement* requirement(*it);
					requirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED,flags);
				}

				if (clearRequirements) {
					mRequirementStepDataList.clear();
				}
				return this;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
