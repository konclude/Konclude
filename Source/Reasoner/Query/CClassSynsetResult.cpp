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

#include "CClassSynsetResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CClassSynsetResult::CClassSynsetResult() {
			}

			CClassSynsetResult::CClassSynsetResult(const QString &className) {
				addEquivalentClassName(className);
			}

			CClassSynsetResult::CClassSynsetResult(const QStringList &classNameList) {
				addEquivalentClassNames(classNameList);
			}


			QStringList CClassSynsetResult::getEquivalentClassNameList() {
				return getEquivalentStringNameList();
			}

			CClassSynsetResult *CClassSynsetResult::addEquivalentClassName(const QString &className) {
				return addEquivalentStringName(className);
			}

			CClassSynsetResult *CClassSynsetResult::addEquivalentClassNames(const QStringList &classNameList) {
				return addEquivalentStringNames(classNameList);
			}

			qint64 CClassSynsetResult::getClassCount() {
				return getStringCount();
			}


			bool CClassSynsetResult::hasClassName(const QString &className) {
				return hasStringName(className);
			}

			bool CClassSynsetResult::hasClassNames(const QStringList &classNameList) {
				return hasStringNames(classNameList);
			}

			QString CClassSynsetResult::getQueryResultString() {
				return CStringSynsetResult<CClassSynsetResult>::getQueryResultString("Class");
			}




		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
