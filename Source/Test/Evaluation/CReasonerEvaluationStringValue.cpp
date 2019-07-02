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

#include "CReasonerEvaluationStringValue.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationStringValue::CReasonerEvaluationStringValue(const QString& value) {
				mValue = value;
			}


			CReasonerEvaluationStringValue::~CReasonerEvaluationStringValue() {
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationStringValue::getCopy() {
				return new CReasonerEvaluationStringValue(*this);
			}

			QString CReasonerEvaluationStringValue::generateString() {
				return mValue;
			}


			const QString& CReasonerEvaluationStringValue::getValue() {
				return mValue;
			}

			CReasonerEvaluationStringValue* CReasonerEvaluationStringValue::setValue(const QString& value) {
				mValue = value;
				return this;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
