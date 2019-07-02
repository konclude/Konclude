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

#include "CIndividualSynsetResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIndividualSynsetResult::CIndividualSynsetResult() {
			}

			CIndividualSynsetResult::CIndividualSynsetResult(const QString &individualName) {
				addEquivalentIndividualName(individualName);
			}

			CIndividualSynsetResult::CIndividualSynsetResult(const QStringList &individualNameList) {
				addEquivalentIndividualNames(individualNameList);
			}


			CIndividualSynsetResult::~CIndividualSynsetResult() {
			}


			QStringList CIndividualSynsetResult::getEquivalentIndividualNameList() {
				return eqIndividualNameList;
			}

			CIndividualSynsetResult *CIndividualSynsetResult::addEquivalentIndividualName(const QString &individualName) {
				eqIndividualNameList.append(individualName);
				return this;
			}

			CIndividualSynsetResult *CIndividualSynsetResult::addEquivalentIndividualNames(const QStringList &individualNameList) {
				eqIndividualNameList += individualNameList;
				return this;
			}

			qint64 CIndividualSynsetResult::getIndividualCount() {
				return eqIndividualNameList.count();
			}


			bool CIndividualSynsetResult::hasIndividualName(const QString &individualName) {
				return eqIndividualNameList.contains(individualName);
			}

			bool CIndividualSynsetResult::hasIndividualNames(const QStringList &individualNameList) {
				QStringList remEqIndividualNameList(eqIndividualNameList);
				foreach (QString individualName, individualNameList) {
					bool contained = false;
					for (qint64 cnt = remEqIndividualNameList.count(); cnt > 0; --cnt) {
						contained |= remEqIndividualNameList.first() == individualName;
						if (contained) {
							remEqIndividualNameList.takeFirst();
							break;
						} else {
							remEqIndividualNameList.append(remEqIndividualNameList.takeFirst());
						}
					}
					if (!contained) {
						return false;
					}
				}
				return true;
			}

			QString CIndividualSynsetResult::getQueryResultString() {
				QString individualNames = eqIndividualNameList.join(", ");
				if (eqIndividualNameList.count() > 1) {
					return QString("Equivalent Individual Set '%1'").arg(individualNames);
				} else if (eqIndividualNameList.count() > 0) {
					return QString("Individual '%1'").arg(individualNames);
				} else {
					return QString("Empty Individual Set");
				}
			}

			bool CIndividualSynsetResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (!otherQueryResult) {
					return false;
				}
				CIndividualSynsetResult *otherIndividualSynset = dynamic_cast<CIndividualSynsetResult *>(otherQueryResult);
				if (!otherIndividualSynset) {
					return false;
				}
				if (getIndividualCount() != otherIndividualSynset->getIndividualCount()) {
					return false;
				}
				if (!hasIndividualNames(otherIndividualSynset->getEquivalentIndividualNameList())) {
					return false;
				}
				return true;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
