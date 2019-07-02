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

#include "CReasonerEvaluationDataValue.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationDataValue::CReasonerEvaluationDataValue() : CLinkerBase<CReasonerEvaluationDataValue*,CReasonerEvaluationDataValue>(this) {
				mDataFlags = 0;
			}


			CReasonerEvaluationDataValue::~CReasonerEvaluationDataValue() {
			}

			bool CReasonerEvaluationDataValue::hasDataFlags(cint64 flags, bool testPartial) {
				if (testPartial) {
					return (mDataFlags & flags) != 0;
				} else {
					return (mDataFlags & flags) == flags;
				}
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationDataValue::setDataFlags(cint64 flags) {
				mDataFlags |= flags;
				return this;
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationDataValue::clearDataFlags(cint64 flags) {
				mDataFlags = mDataFlags & ~flags;
				return this;
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationDataValue::setDataFlags(cint64 flags, bool value) {
				if (value) {
					return setDataFlags(flags);
				} else {
					return clearDataFlags(flags);
				}
			}


			bool CReasonerEvaluationDataValue::hasErrorOccuredFlag() {
				return hasDataFlags(REDFDATAERROROCCURED,true);
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationDataValue::setErrorOccuredFlag(bool value) {
				return setDataFlags(REDFDATAERROROCCURED,value);
			}

			bool CReasonerEvaluationDataValue::hasDataNotAvailableFlag() {
				return hasDataFlags(REDFDATANOTAVAILABLE,true);
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationDataValue::setDataNotAvailableFlag(bool value) {
				return setDataFlags(REDFDATANOTAVAILABLE,value);
			}


			cint64 CReasonerEvaluationDataValue::getDataFlags() {
				return mDataFlags;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
