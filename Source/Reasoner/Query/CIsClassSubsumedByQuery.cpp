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

#include "CIsClassSubsumedByQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIsClassSubsumedByQuery::CIsClassSubsumedByQuery(const QString& subsumedClassName, const QString& subsumerClassName, const QString& queryName) {
				subsumedName = subsumedClassName;
				subsumerName = subsumerClassName;
				CSatisfiableCalculationJobsQuery::setQueryName(queryName);
				QString queryString = QString("Is class '%1' subsumed by '%2'?").arg(subsumerClassName).arg(subsumedClassName);
				CSatisfiableCalculationJobsQuery::setQueryString(queryString);
				QString answerStringYes = QString("Yes");
				QString answerStringNo = QString("No");
				QString answerStringUnknown = QString("Unknown");
				CSatisfiableCalculationJobsQuery::setAnswerString(answerStringYes,answerStringNo,answerStringUnknown);
			}

			bool CIsClassSubsumedByQuery::isSubsumed() {
				return getResult();
			}

			CIsClassSubsumedByQuery::~CIsClassSubsumedByQuery() {
			}

			QString CIsClassSubsumedByQuery::getSubsumerClassName() {
				return subsumerName;
			}

			QString CIsClassSubsumedByQuery::getSubsumedClassName() {
				return subsumedName;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
