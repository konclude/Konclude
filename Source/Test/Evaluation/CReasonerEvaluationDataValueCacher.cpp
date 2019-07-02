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

#include "CReasonerEvaluationDataValueCacher.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationDataValueCacher::CReasonerEvaluationDataValueCacher() {
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationDataValueCacher::getCachedDataValue(const QString& responseName, cint64 extractionType) {
				CReasonerEvaluationDataValue* dataValue = nullptr;
				dataValue = mResponseTypeDataValueHash.value(QPair<QString,cint64>(responseName,extractionType));
				return dataValue;
			}

			bool CReasonerEvaluationDataValueCacher::addDataValueToCache(const QString& responseName, cint64 extractionType, CReasonerEvaluationDataValue* dataValue) {
				mResponseTypeDataValueHash.insert(QPair<QString,cint64>(responseName,extractionType),dataValue);
				return true;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
