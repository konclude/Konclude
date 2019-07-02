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

#include "CReasonerEvaluationFilterStringCondition.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationFilterStringCondition::CReasonerEvaluationFilterStringCondition(const QString& statisticNameString, const QString& statisticComperator, const QString& statisticCompareValueString) : CReasonerEvaluationFilterCondition(statisticNameString) {
				mStatisticCompareValue = statisticCompareValueString;
				mStatisticComperator = statisticComperator;
			}




			bool CReasonerEvaluationFilterStringCondition::isStatisticConditionSatisfied(const QString& statisticString, const QString& statisticValueString) {
				if (mStatisticComperator == "=" || mStatisticComperator == "==") {
					if (statisticValueString == mStatisticCompareValue) {
						return true;
					}
				} else if (mStatisticComperator == "!=" || mStatisticComperator == "!") {
					if (statisticValueString != mStatisticCompareValue) {
						return true;
					}
				} else if (mStatisticComperator == "contains") {
					if (statisticValueString.contains(mStatisticCompareValue)) {
						return true;
					}
				} else if (mStatisticComperator == "CONTAINS") {
					if (statisticValueString.toUpper().contains(mStatisticCompareValue)) {
						return true;
					}
				} else if (mStatisticComperator == "startsWith") {
					if (statisticValueString.startsWith(mStatisticCompareValue)) {
						return true;
					}
				} else if (mStatisticComperator == "STARTSWITH") {
					if (statisticValueString.toUpper().startsWith(mStatisticCompareValue)) {
						return true;
					}
				} else if (mStatisticComperator == "endsWith") {
					if (statisticValueString.endsWith(mStatisticCompareValue)) {
						return true;
					}
				} else if (mStatisticComperator == "ENDSWITH") {
					if (statisticValueString.toUpper().endsWith(mStatisticCompareValue)) {
						return true;
					}
				}
				return false;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
