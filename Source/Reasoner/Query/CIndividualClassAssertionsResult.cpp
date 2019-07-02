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

#include "CIndividualClassAssertionsResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIndividualClassAssertionsResult::CIndividualClassAssertionsResult() {
			}


			QString CIndividualClassAssertionsResult::getQueryResultString() {
				return QString();
			}

			bool CIndividualClassAssertionsResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				CIndividualClassAssertionsResult* individualConceptAssertionsResult = dynamic_cast<CIndividualClassAssertionsResult*>(otherQueryResult);
				if (individualConceptAssertionsResult) {
					if (mIndividualConceptAssertionSet.count() == individualConceptAssertionsResult->mIndividualConceptAssertionSet.count()) {
						for (QSet< QPair<QString,QString> >::const_iterator it = mIndividualConceptAssertionSet.constBegin(), itEnd = mIndividualConceptAssertionSet.constEnd(); it != itEnd; ++it) {
							const QPair<QString,QString>& individualConceptAssertionPairString(*it);
							if (!individualConceptAssertionsResult->mIndividualConceptAssertionSet.contains(individualConceptAssertionPairString)) {
								return false;
							}
						}
						return true;
					}
				}
				return false;
			}

			CIndividualClassAssertionsResult* CIndividualClassAssertionsResult::addIndividualConceptAssertion(const QString& individualName, const QString& conceptName) {
				mIndividualConceptAssertionSet.insert(QPair<QString,QString>(individualName,conceptName));
				return this;
			}

			QSet< QPair<QString,QString> >* CIndividualClassAssertionsResult::getIndividualConceptAssertionSet() {
				return &mIndividualConceptAssertionSet;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
