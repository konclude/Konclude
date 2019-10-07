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

#include "CIndividualDependenceTrackingCollector.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CIndividualDependenceTrackingCollector::CIndividualDependenceTrackingCollector() {
			}


			CIndividualDependenceTrackingCollector::~CIndividualDependenceTrackingCollector() {
			}


			CIndividualDependenceTracking* CIndividualDependenceTrackingCollector::getExtendingIndividualDependenceTracking() {
                return mIndiDepTrackingPointer.fetchAndAddRelaxed(0);
			}

			CIndividualDependenceTracking* CIndividualDependenceTrackingCollector::installIndividualDependenceTracking(CIndividualDependenceTracking* indDepTrack) {
				if (!mIndiDepTrackingPointer.testAndSetOrdered(nullptr,indDepTrack)) {
					delete indDepTrack;
				}
                return mIndiDepTrackingPointer.fetchAndAddRelaxed(0);
			}



		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
