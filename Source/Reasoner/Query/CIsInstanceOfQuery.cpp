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

#include "CIsInstanceOfQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIsInstanceOfQuery::CIsInstanceOfQuery(const QString& individualName, const QString& className, const QString& queryName) {
				CSatisfiableCalculationJobsQuery::setQueryName(queryName);
				QString queryString = QString("Is individual '%1' instance of class '%2'?").arg(individualName).arg(className);
				CSatisfiableCalculationJobsQuery::setQueryString(queryString);
				QString answerStringYes = QString("Yes.");
				QString answerStringNo = QString("No.");
				QString answerStringUnknown = QString("Unknown.");
				CSatisfiableCalculationJobsQuery::setAnswerString(answerStringYes,answerStringNo,answerStringUnknown);
			}

			bool CIsInstanceOfQuery::isInstance() {
				return getResult();
			}

			CIsInstanceOfQuery::~CIsInstanceOfQuery() {
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
