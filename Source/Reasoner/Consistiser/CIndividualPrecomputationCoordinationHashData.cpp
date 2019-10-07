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

#include "CIndividualPrecomputationCoordinationHashData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CIndividualPrecomputationCoordinationHashData::CIndividualPrecomputationCoordinationHashData() {
				mComputationIntegrated = false;
				mComputationOrdered = false;
				mNewlyRetrieved = false;
				mUsageCount = 0;
			}

			CIndividualPrecomputationCoordinationHashData* CIndividualPrecomputationCoordinationHashData::setComputationOrdered(bool ordered) {
				mComputationOrdered = ordered;
				return this;
			}

			CIndividualPrecomputationCoordinationHashData* CIndividualPrecomputationCoordinationHashData::setComputationIntegrated(bool integrated) {
				mComputationIntegrated = integrated;
				return this;
			}

			bool CIndividualPrecomputationCoordinationHashData::isComputationOrdered() {
				return mComputationOrdered;
			}

			bool CIndividualPrecomputationCoordinationHashData::isComputationIntegrated() {
				return mComputationIntegrated;
			}


			CIndividualPrecomputationCoordinationHashData* CIndividualPrecomputationCoordinationHashData::setNewlyRetrieved(bool retrieved) {
				mNewlyRetrieved = retrieved;
				return this;
			}

			bool CIndividualPrecomputationCoordinationHashData::isNewlyRetrieved() {
				return mNewlyRetrieved;
			}


			CIndividualPrecomputationCoordinationHashData* CIndividualPrecomputationCoordinationHashData::incUsageCount(cint64 count) {
				mUsageCount += count;
				return this;
			}

			CIndividualPrecomputationCoordinationHashData* CIndividualPrecomputationCoordinationHashData::decUsageCount(cint64 count) {
				mUsageCount -= count;
				return this;
			}

			cint64 CIndividualPrecomputationCoordinationHashData::getUsageCount() {
				return mUsageCount;
			}


		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
