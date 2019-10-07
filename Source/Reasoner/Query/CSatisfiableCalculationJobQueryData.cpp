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

#include "CSatisfiableCalculationJobQueryData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSatisfiableCalculationJobQueryData::CSatisfiableCalculationJobQueryData(CQuery* query, bool mustBeSatisfiableForPositive) {
				mQuery = query;
				mMustBeSatisfiableForPositive = mustBeSatisfiableForPositive;
				mJobCalculated = false;
				mJobCalculationError = false;
				mJobCalculationSatisfiable = false;
			}


			CSatisfiableCalculationJobQueryData::~CSatisfiableCalculationJobQueryData() {
			}

			CQuery* CSatisfiableCalculationJobQueryData::getQuery() {
				return mQuery;
			}

			bool CSatisfiableCalculationJobQueryData::needsJobSatisfiabilityForPositiveQueryAnswer() {
				return mMustBeSatisfiableForPositive;
			}

			bool CSatisfiableCalculationJobQueryData::isJobCalculated() {
				return mJobCalculated;
			}

			bool CSatisfiableCalculationJobQueryData::isJobCalculationError() {
				return mJobCalculationError;
			}

			bool CSatisfiableCalculationJobQueryData::isJobCalculationSatisfiable() {
				return mJobCalculationSatisfiable;
			}

			CSatisfiableCalculationJobQueryData* CSatisfiableCalculationJobQueryData::setJobCalculated(bool jobCalculated) {
				mJobCalculated = jobCalculated;
				return this;
			}

			CSatisfiableCalculationJobQueryData* CSatisfiableCalculationJobQueryData::setJobCalculationError(bool jobCalculationError) {
				mJobCalculationError = jobCalculationError;
				return this;
			}

			CSatisfiableCalculationJobQueryData* CSatisfiableCalculationJobQueryData::setJobCalculationSatisfiable(bool jobCalculationSatisfiable) {
				mJobCalculationSatisfiable = jobCalculationSatisfiable;
				return this;
			}

			
		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
