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

#include "CReasonerEvaluationDataValueGroup.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationDataValueGroup::CReasonerEvaluationDataValueGroup() {
			}


			CReasonerEvaluationDataValueGroup::~CReasonerEvaluationDataValueGroup() {
			}


			CReasonerEvaluationDataValueGroup* CReasonerEvaluationDataValueGroup::setGroupName(const QString& string) {
				mGroupName = string;
				return this;
			}

			CReasonerEvaluationDataValueGroup* CReasonerEvaluationDataValueGroup::addRequestDataValue(const QString& requestString, CReasonerEvaluationDataValue* dataValue) {
				mRequestDataValueMap.insert(requestString,dataValue);
				return this;
			}

			QMap<QString,CReasonerEvaluationDataValue*>* CReasonerEvaluationDataValueGroup::getRequestDataValueMap() {
				return &mRequestDataValueMap;
			}

			QString CReasonerEvaluationDataValueGroup::getGroupName() {
				return mGroupName;
			}

			bool CReasonerEvaluationDataValueGroup::isEmpty() {
				return mRequestDataValueMap.isEmpty();
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
