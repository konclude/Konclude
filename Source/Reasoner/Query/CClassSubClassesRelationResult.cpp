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

#include "CClassSubClassesRelationResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CClassSubClassesRelationResult::CClassSubClassesRelationResult() {
			}

			CClassSubClassesRelationResult::CClassSubClassesRelationResult(CClassSynsetResult *superClassSynset) : CStringSubStringsRelationResult<CClassSubClassesRelationResult,CClassSynsetResult>(superClassSynset) {
			}



			CClassSynsetResult *CClassSubClassesRelationResult::getSuperClassSynset() {
				return getSuperStringSynset();
			}

			CClassSubClassesRelationResult *CClassSubClassesRelationResult::setSuperClassSynset(CClassSynsetResult *superClassSynset) {
				return setSuperStringSynset(superClassSynset);
			}


			QSet<CClassSynsetResult *>* CClassSubClassesRelationResult::getSubClassesSynsetSet() {
				return getSubStringsSynsetSet();
			}

			QList<CClassSynsetResult *> CClassSubClassesRelationResult::getSubClassesSynsetList() {
				return getSubStringsSynsetList();
			}

			CClassSubClassesRelationResult *CClassSubClassesRelationResult::addSubClassesSynset(CClassSynsetResult *subClassSynset) {
				return addSubStringsSynset(subClassSynset);
			}

			CClassSubClassesRelationResult *CClassSubClassesRelationResult::addSubClassesSynsets(const QList<CClassSynsetResult *> &subClassesSynsets) {
				return addSubStringsSynsets(subClassesSynsets);
			}

			qint64 CClassSubClassesRelationResult::getSubClassesSynsetCount() {
				return getSubStringsSynsetCount();
			}


			bool CClassSubClassesRelationResult::hasSubClassesSynset(CClassSynsetResult *subClassSynset) {
				return hasSubStringsSynset(subClassSynset);
			}


			QString CClassSubClassesRelationResult::getQueryResultString() {
				return CStringSubStringsRelationResult<CClassSubClassesRelationResult,CClassSynsetResult>::getQueryResultString("Class");
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
