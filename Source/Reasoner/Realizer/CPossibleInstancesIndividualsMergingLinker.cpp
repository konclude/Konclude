/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CPossibleInstancesIndividualsMergingLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CPossibleInstancesIndividualsMergingLinker::CPossibleInstancesIndividualsMergingLinker() {
			}


			CPossibleInstancesIndividualsMergingLinker* CPossibleInstancesIndividualsMergingLinker::initPossibleInstancesIndividualsMergingLinker(cint64 mergingIndiId, CPossibleInstancesIndividualsMergingData* mergingData) {
				initLinker(mergingIndiId);
				mSatisfiableMerged = false;
				mFullTestingScheduled = false;
				mMergingAttemptCount.store(0);
				mMergingData = mergingData;
				return this;
			}


			CPossibleInstancesIndividualsMergingLinker* CPossibleInstancesIndividualsMergingLinker::getNextPossibleInstancesIndividualsMergingLinker() {
				return (CPossibleInstancesIndividualsMergingLinker*)getNext();
			}

			cint64 CPossibleInstancesIndividualsMergingLinker::getMergingIndividualId() {
				return getData();
			}

			bool CPossibleInstancesIndividualsMergingLinker::isSatisfiableMerged() {
				return mSatisfiableMerged;
			}

			CPossibleInstancesIndividualsMergingLinker* CPossibleInstancesIndividualsMergingLinker::incMergingAttempt() {
				mMergingAttemptCount.ref();
				return this;
			}

			cint64 CPossibleInstancesIndividualsMergingLinker::getMergingAttemptCount() {
				return mMergingAttemptCount;
			}

			bool CPossibleInstancesIndividualsMergingLinker::isFullTestingScheduled() {
				return mFullTestingScheduled;
			}

			CPossibleInstancesIndividualsMergingLinker* CPossibleInstancesIndividualsMergingLinker::setFullTestingScheduled() {
				mFullTestingScheduled = true;
				return this;
			}

			CPossibleInstancesIndividualsMergingLinker* CPossibleInstancesIndividualsMergingLinker::setSatisfiableMerged() {
				mSatisfiableMerged = true;
				return this;
			}


			CPossibleInstancesIndividualsMergingData* CPossibleInstancesIndividualsMergingLinker::getMergingData() {
				return mMergingData;
			}



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
