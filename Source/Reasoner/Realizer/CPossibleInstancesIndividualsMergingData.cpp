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

#include "CPossibleInstancesIndividualsMergingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CPossibleInstancesIndividualsMergingData::CPossibleInstancesIndividualsMergingData(cint64 maxMergingAttemptCount) {
				mMaxMergingAttemptCount = maxMergingAttemptCount;
				mPossibleInstancesIndividualsMergingLinker = nullptr;
				mPossibleMergingIndiCount = 0;
				mAllPossibleInstancesIndividualsNonMerging = false;
				mMergingAttemptCount.store(0);
				mMergingSuccessCount.store(0);
				mMergingStreakFailCount.store(0);
			}

			CPossibleInstancesIndividualsMergingLinker* CPossibleInstancesIndividualsMergingData::getPossibleInstancesIndividualsMergingLinker() {
				return mPossibleInstancesIndividualsMergingLinker;
			}

			CPossibleInstancesIndividualsMergingData* CPossibleInstancesIndividualsMergingData::addPossibleInstancesIndividualsMergingLinker(CPossibleInstancesIndividualsMergingLinker* linker) {
				mPossibleMergingIndiCount += linker->getCount();
				mPossibleInstancesIndividualsMergingLinker = (CPossibleInstancesIndividualsMergingLinker*)linker->append(mPossibleInstancesIndividualsMergingLinker);
				return this;
			}

			cint64 CPossibleInstancesIndividualsMergingData::getPossibleMergingIndividualCount() {
				return mPossibleMergingIndiCount;
			}

			cint64 CPossibleInstancesIndividualsMergingData::getMergingAttemptCount() {
				return mMergingAttemptCount;
			}

			cint64 CPossibleInstancesIndividualsMergingData::getMergingSuccessCount() {
				return mMergingSuccessCount;
			}

			cint64 CPossibleInstancesIndividualsMergingData::getMaxMergingAttemptCount() {
				return mMaxMergingAttemptCount;
			}

			cint64 CPossibleInstancesIndividualsMergingData::getMergingStreakFailCount() {
				return mMergingStreakFailCount;
			}

			CPossibleInstancesIndividualsMergingData* CPossibleInstancesIndividualsMergingData::incMergingAttempt() {
				mMergingAttemptCount.ref();
				return this;
			}

			CPossibleInstancesIndividualsMergingData* CPossibleInstancesIndividualsMergingData::incMergingSuccess() {
				mMergingSuccessCount.ref();
				return this;
			}

			CPossibleInstancesIndividualsMergingData* CPossibleInstancesIndividualsMergingData::incMergingStreakFails() {
				mMergingStreakFailCount.ref();
				return this;
			}


			bool CPossibleInstancesIndividualsMergingData::areAllPossibleInstancesIndividualsNonMerging() {
				return mAllPossibleInstancesIndividualsNonMerging;
			}

			CPossibleInstancesIndividualsMergingData* CPossibleInstancesIndividualsMergingData::setAllPossibleInstancesIndividualsNonMerging(bool allNonMergeable) {
				mAllPossibleInstancesIndividualsNonMerging = allNonMergeable;
				return this;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
