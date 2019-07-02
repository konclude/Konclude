/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CIndividualsResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIndividualsResult::CIndividualsResult() {
			}


			QString CIndividualsResult::getQueryResultString() {
				return QString();
			}

			bool CIndividualsResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				CIndividualsResult* individualsResult = dynamic_cast<CIndividualsResult*>(otherQueryResult);
				if (individualsResult) {
					if (mIndividualSet.count() == individualsResult->mIndividualSet.count()) {
						for (QSet<QString>::const_iterator it = mIndividualSet.constBegin(), itEnd = mIndividualSet.constEnd(); it != itEnd; ++it) {
							const QString& individualString(*it);
							if (!individualsResult->mIndividualSet.contains(individualString)) {
								return false;
							}
						}
						return true;
					}
				}
				return false;
			}

			CIndividualsResult* CIndividualsResult::addIndividual(const QString &individualName) {
				mIndividualSet.insert(individualName);
				return this;
			}

			QSet<QString>* CIndividualsResult::getIndividualsSet() {
				return &mIndividualSet;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
