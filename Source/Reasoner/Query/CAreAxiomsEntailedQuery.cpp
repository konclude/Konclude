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

#include "CAreAxiomsEntailedQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CAreAxiomsEntailedQuery::CAreAxiomsEntailedQuery(QStringList axiomTypeNames, const QString& queryName) {
				cint64 axiomsCount = axiomTypeNames.count();
				QString axiomTypeNamesString;
				bool hasClassName = false;
				while (!axiomTypeNames.isEmpty()) {
					QString nextClassName = axiomTypeNames.takeFirst();
					if (hasClassName) {
						if (axiomTypeNames.isEmpty()) {
							axiomTypeNamesString += QString(" and ");
						} else {
							axiomTypeNamesString += QString(", ");
						}
					}
					axiomTypeNamesString += nextClassName;
					hasClassName = true;
				}
				CSatisfiableCalculationJobsQuery::setQueryName(queryName);
				QString queryString;
				if (axiomsCount > 1) {
					queryString = QString("Are '%1'-axioms entailed?").arg(axiomTypeNamesString);
				} else {
					queryString = QString("Is '%1'-axiom entailed?").arg(axiomTypeNamesString);
				}
				CSatisfiableCalculationJobsQuery::setQueryString(queryString);
				QString answerStringYes = QString("Yes");
				QString answerStringNo = QString("No");
				QString answerStringUnknown = QString("Unknown");
				CSatisfiableCalculationJobsQuery::setAnswerString(answerStringYes,answerStringNo,answerStringUnknown);
			}


			bool CAreAxiomsEntailedQuery::areClassesEquivalent() {
				return getResult();
			}


			CAreAxiomsEntailedQuery::~CAreAxiomsEntailedQuery() {
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
