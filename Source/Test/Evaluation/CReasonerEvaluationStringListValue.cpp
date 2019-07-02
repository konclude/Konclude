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

#include "CReasonerEvaluationStringListValue.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationStringListValue::CReasonerEvaluationStringListValue(const QStringList& value) {
				mValue = value;
			}


			CReasonerEvaluationStringListValue::~CReasonerEvaluationStringListValue() {
			}


			CReasonerEvaluationDataValue* CReasonerEvaluationStringListValue::getCopy() {
				return new CReasonerEvaluationStringListValue(*this);
			}


			QString CReasonerEvaluationStringListValue::generateString() {
				return mValue.join("\n");
			}


			const QStringList& CReasonerEvaluationStringListValue::getValue() {
				return mValue;
			}

			CReasonerEvaluationStringListValue* CReasonerEvaluationStringListValue::setValue(const QStringList& value) {
				mValue = value;
				return this;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
