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

#include "CVariableBindingsAnswerResultMapOrdering.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CVariableBindingsAnswerResultMapOrdering::CVariableBindingsAnswerResultMapOrdering(cint64 orderingVarCount) {
				mVarIdxArray = new cint64[orderingVarCount];
				mAscendingArray = new bool[orderingVarCount];
				mOrderingVarCount = orderingVarCount;
				mDistinct = false;
			}

			CVariableBindingsAnswerResultMapOrdering::~CVariableBindingsAnswerResultMapOrdering() {
				delete[] mVarIdxArray;
				delete[] mAscendingArray;
			}


			cint64 CVariableBindingsAnswerResultMapOrdering::getVariableIndex(cint64 varNumber) {
				return mVarIdxArray[varNumber];
			}

			CVariableBindingsAnswerResultMapOrdering* CVariableBindingsAnswerResultMapOrdering::setVariableIndex(cint64 varNumber, cint64 index) {
				mVarIdxArray[varNumber] = index;
				return this;
			}

			cint64 CVariableBindingsAnswerResultMapOrdering::getVariableCount() {
				return mOrderingVarCount;
			}

			bool CVariableBindingsAnswerResultMapOrdering::isDistinct() {
				return mDistinct;
			}


			bool CVariableBindingsAnswerResultMapOrdering::isAscending(cint64 varNumber) {
				return mAscendingArray[varNumber];
			}

			CVariableBindingsAnswerResultMapOrdering* CVariableBindingsAnswerResultMapOrdering::setDistinct(bool distinct) {
				mDistinct = distinct;
				return this;
			}

			CVariableBindingsAnswerResultMapOrdering* CVariableBindingsAnswerResultMapOrdering::setAscending(cint64 varNumber, bool ascending) {
				mAscendingArray[varNumber] = ascending;
				return this;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
