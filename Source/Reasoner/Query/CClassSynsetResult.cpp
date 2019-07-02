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


			CClassSynsetResult::~CClassSynsetResult() {
			}


			QStringList CClassSynsetResult::getEquivalentClassNameList() {
				return eqClassNameList;
			}

			CClassSynsetResult *CClassSynsetResult::addEquivalentClassName(const QString &className) {
				eqClassNameList.append(className);
				return this;
			}

			CClassSynsetResult *CClassSynsetResult::addEquivalentClassNames(const QStringList &classNameList) {
				eqClassNameList += classNameList;
				return this;
			}

			qint64 CClassSynsetResult::getClassCount() {
				return eqClassNameList.count();
			}


			bool CClassSynsetResult::hasClassName(const QString &className) {
				return eqClassNameList.contains(className);
			}

			bool CClassSynsetResult::hasClassNames(const QStringList &classNameList) {
				if (eqClassNameList.size() <= 1) {
					return eqClassNameList.first() == classNameList.first();
				} else if (eqClassNameList.size() <= 20) {
					QStringList remEqClassNameList(eqClassNameList);
					foreach (const QString& className, classNameList) {
						bool contained = false;
						for (qint64 cnt = remEqClassNameList.count(); cnt > 0; --cnt) {
							contained |= remEqClassNameList.first() == className;
							if (contained) {
								remEqClassNameList.takeFirst();
								break;
							} else {
								remEqClassNameList.append(remEqClassNameList.takeFirst());
							}
						}
						if (!contained) {
							return false;
						}
					}
				} else {
					QHash<QString,cint64> classNameCountHash;
					foreach (const QString& className, classNameList) {
						if (classNameCountHash.contains(className)) {
							++classNameCountHash[className];
						} else {
							classNameCountHash[className] = 1;
						}
					}
					foreach (const QString& className, eqClassNameList) {
						if (!classNameCountHash.contains(className)) {
							return false;
						} else {
							--classNameCountHash[className];
						}
					}
					for (QHash<QString,cint64>::const_iterator it = classNameCountHash.constBegin(), itEnd = classNameCountHash.constEnd(); it != itEnd; ++it) {
						cint64 diff(it.value());
						if (diff != 0) {
							return false;
						}
					}
				}
				return true;
			}

			QString CClassSynsetResult::getQueryResultString() {
				QString classNames = eqClassNameList.join(", ");
				if (eqClassNameList.count() > 1) {
					return QString("Equivalent Class Set '%1'").arg(classNames);
				} else if (eqClassNameList.count() > 0) {
					return QString("Class '%1'").arg(classNames);
				} else {
					return QString("Empty Class Set");
				}
			}

			bool CClassSynsetResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (!otherQueryResult) {
					return false;
				}
				CClassSynsetResult *otherClassSynset = dynamic_cast<CClassSynsetResult *>(otherQueryResult);
				if (!otherClassSynset) {
					return false;
				}
				if (getClassCount() != otherClassSynset->getClassCount()) {
					return false;
				}
				if (!hasClassNames(otherClassSynset->getEquivalentClassNameList())) {
					return false;
				}
				return true;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
