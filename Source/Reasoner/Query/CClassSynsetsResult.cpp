/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CClassSynsetsResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CClassSynsetsResult::CClassSynsetsResult() {
			}



			CClassSynsetsResult::~CClassSynsetsResult() {
				qDeleteAll(mClassSynsetContainer);
			}


			CClassSynsetResult *CClassSynsetsResult::getClassSynset(const QString &className, bool create) {
				CClassSynsetResult *classSynset = mClassClassSynsetHash.value(className,0);
				if (!classSynset && create) {
					classSynset = new CClassSynsetResult(className);
					mClassClassSynsetHash.insert(className,classSynset);
					mClassSynsetContainer.append(classSynset);
				}
				return classSynset;
			}

			CClassSynsetResult *CClassSynsetsResult::getClassSynset(const QStringList &classNames, bool create) {
				CClassSynsetResult *classSynset = 0;
				if (!classNames.isEmpty()) {
					classSynset = mClassClassSynsetHash.value(classNames.first(),0);
					if (!classSynset && create) {
						classSynset = new CClassSynsetResult(classNames);
						foreach (QString className, classNames) {
							mClassClassSynsetHash.insert(className,classSynset);
						}
						mClassSynsetContainer.append(classSynset);
					}
				}
				return classSynset;
			}

			CClassSynsetResult *CClassSynsetsResult::addClassSynset(CClassSynsetResult *takeClassSynset) {
				foreach (QString className, takeClassSynset->getEquivalentClassNameList()) {
					mClassClassSynsetHash.insert(className,takeClassSynset);
				}
				mClassSynsetContainer.append(takeClassSynset);
				return takeClassSynset;
			} 
 
			QList<CClassSynsetResult*>* CClassSynsetsResult::getClassSynsetList() {
				return &mClassSynsetContainer;
			}


			QString CClassSynsetsResult::getQueryResultString() {
				QString string;
				return string;
			}
 

			bool CClassSynsetsResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (!otherQueryResult) {
					return false;
				}
				CClassSynsetsResult *otherClassSynsets = dynamic_cast<CClassSynsetsResult *>(otherQueryResult);
				if (!otherClassSynsets) {
					return false;
				}

				if (mClassClassSynsetHash.count() != otherClassSynsets->mClassClassSynsetHash.count()) {
					return false;
				}

				for (QList<CClassSynsetResult*>::const_iterator it = mClassSynsetContainer.constBegin(), itEnd = mClassSynsetContainer.constEnd(); it != itEnd; ++it) {
					CClassSynsetResult* classSynRes(*it);
					CClassSynsetResult* otherClassSynRes = nullptr;
					QStringList classNameList(classSynRes->getEquivalentClassNameList());
					if (!classNameList.isEmpty()) {
						const QString& className = classNameList.first();
						otherClassSynRes = otherClassSynsets->mClassClassSynsetHash.value(className);
					}
					if (!otherClassSynRes && !classNameList.isEmpty()) {
						return false;
					}
					if (!classSynRes->isResultEquivalentTo(otherClassSynRes)) {
						return false;
					}
				}
				return true;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
