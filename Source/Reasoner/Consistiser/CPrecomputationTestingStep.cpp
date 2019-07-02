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

#include "CPrecomputationTestingStep.h"
#include "COntologyPrecomputationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CPrecomputationTestingStep::CPrecomputationTestingStep(TESTINGSTEPTYPE testingStepType, COntologyProcessingStepData* processingStepData, COntologyPrecomputationItem* precompItem) {
				mTestingStepType = testingStepType;
				mOntologyPrecompItem = precompItem;
				mProcessingStepData = processingStepData;
				mStepFinished = false;
				mCombineLastFlags = 0;
			}

			COntologyPrecomputationItem* CPrecomputationTestingStep::getOntologyPrecomputationItem() {
				return mOntologyPrecompItem;
			}

			CPrecomputationTestingStep* CPrecomputationTestingStep::setOntologyPrecomputationItem(COntologyPrecomputationItem* precompItem) {
				mOntologyPrecompItem = precompItem;
				return this;
			}

			bool CPrecomputationTestingStep::isStepFinished() {
				return mStepFinished;
			}

			bool CPrecomputationTestingStep::areStepProcessingRequirementSatisfied() {
				return mProcessingStepData->getProcessingStep()->areAllRequirementsSatisfied(mOntologyPrecompItem->getOntology());
			}

			bool CPrecomputationTestingStep::hasRequirements() {
				return !mRequirementStepDataList.isEmpty();
			}

			CPrecomputationTestingStep* CPrecomputationTestingStep::setStepFinished(bool finished) {
				mStepFinished = finished;
				cint64 installFlags = COntologyProcessingStatus::PSCOMPLETELYYPROCESSED;
				mProcessingStepData->getProcessingStatus()->setProcessingFlags(installFlags);
				return this;
			}

			QList<COntologyProcessingRequirement*>* CPrecomputationTestingStep::getRequirementList() {
				return &mRequirementStepDataList;
			}

			COntologyProcessingStepData* CPrecomputationTestingStep::getProcessingStepData() {
				return mProcessingStepData;
			}


			CPrecomputationTestingStep::TESTINGSTEPTYPE CPrecomputationTestingStep::getPrecomputationTestingType() {
				return mTestingStepType;
			}

			CPrecomputationTestingStep* CPrecomputationTestingStep::setProcessingStepData(COntologyProcessingStepData* processingData) {
				mProcessingStepData = processingData;
				return this;
			}

			CPrecomputationTestingStep* CPrecomputationTestingStep::addProcessingRequirement(COntologyProcessingRequirement* processingRequirement) {
				mRequirementStepDataList.append(processingRequirement);
				return this;
			}

			CPrecomputationTestingStep* CPrecomputationTestingStep::submitRequirementsUpdate(bool clearRequirements) {
				submitRequirementsUpdate(mCombineLastFlags,true);
				return this;
			}


			CPrecomputationTestingStep* CPrecomputationTestingStep::submitRequirementsUpdate(cint64 flags, bool clearRequirements) {
				mCombineLastFlags |= flags;
				mProcessingStepData->getProcessingStatus()->setErrorFlags(mCombineLastFlags);

				for (QList<COntologyProcessingRequirement*>::const_iterator it = mRequirementStepDataList.constBegin(), itEnd = mRequirementStepDataList.constEnd(); it != itEnd; ++it) {
					COntologyProcessingRequirement* requirement(*it);
					requirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED,mCombineLastFlags);
				}

				if (clearRequirements) {
					mRequirementStepDataList.clear();
				}
				return this;
			}

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
