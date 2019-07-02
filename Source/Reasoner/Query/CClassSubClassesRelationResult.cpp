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

#include "CClassSubClassesRelationResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CClassSubClassesRelationResult::CClassSubClassesRelationResult() {
				superClass = 0;
			}

			CClassSubClassesRelationResult::CClassSubClassesRelationResult(CClassSynsetResult *superClassSynset) {
				superClass = superClassSynset;
			}

			CClassSubClassesRelationResult::~CClassSubClassesRelationResult() {
			}


			CClassSynsetResult *CClassSubClassesRelationResult::getSuperClassSynset() {
				return superClass;
			}

			CClassSubClassesRelationResult *CClassSubClassesRelationResult::setSuperClassSynset(CClassSynsetResult *superClassSynset) {
				superClass = superClassSynset;
				return this;
			}


			QSet<CClassSynsetResult *>* CClassSubClassesRelationResult::getSubClassesSynsetSet() {
				return &subClassSynsetSet;
			}

			QList<CClassSynsetResult *> CClassSubClassesRelationResult::getSubClassesSynsetList() {
				return subClassSynsetSet.toList();
			}

			CClassSubClassesRelationResult *CClassSubClassesRelationResult::addSubClassesSynset(CClassSynsetResult *subClassSynset) {
				subClassSynsetSet.insert(subClassSynset);
				return this;
			}

			CClassSubClassesRelationResult *CClassSubClassesRelationResult::addSubClassesSynsets(const QList<CClassSynsetResult *> &subClassesSynsets) {
				foreach (CClassSynsetResult* classSynRes, subClassesSynsets) {
					subClassSynsetSet.insert(classSynRes);
				}
				return this;
			}

			qint64 CClassSubClassesRelationResult::getSubClassesSynsetCount() {
				return subClassSynsetSet.count();
			}


			bool CClassSubClassesRelationResult::hasSubClassesSynset(CClassSynsetResult *subClassSynset) {
				return subClassSynsetSet.contains(subClassSynset);
			}


			QString CClassSubClassesRelationResult::getQueryResultString() {
				QString string;
				if (superClass && getSubClassesSynsetCount() > 0) {
					string += superClass->getQueryResultString();
				}
				if (!string.isEmpty()) {
					QString subClassString;
					foreach (CClassSynsetResult *subClassSynset, subClassSynsetSet) {
						if (subClassString.isEmpty()) {
							subClassString += QString(" has Sub-Class-Relation to %1").arg(subClassSynset->getQueryResultString());
						} else {
							subClassString += QString(" and to %1").arg(subClassSynset->getQueryResultString());
						}
					}
					string += subClassString;
				}
				return string;
			}

			bool CClassSubClassesRelationResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (!otherQueryResult) {
					return false;
				}
				CClassSubClassesRelationResult *otherSubClassRelation = dynamic_cast<CClassSubClassesRelationResult *>(otherQueryResult);
				if (!otherSubClassRelation) {
					return false;
				}
				if (!superClass->isResultEquivalentTo(otherSubClassRelation->getSuperClassSynset())) {
					return false;
				}
				QList<CClassSynsetResult *> otherSubClassList(otherSubClassRelation->getSubClassesSynsetList());				
				foreach (CClassSynsetResult *classSynset, otherSubClassList) {
					bool contained = false;					
					for (qint64 cnt = otherSubClassList.count(); cnt > 0; --cnt) {
						contained |= otherSubClassList.first()->isResultEquivalentTo(classSynset);
						if (contained) {
							otherSubClassList.takeFirst();
							break;
						} else {
							otherSubClassList.append(otherSubClassList.takeFirst());
						}
					}
					if (!contained) {
						return false;
					}
				}
				return true;
			}



		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
