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

#ifndef KONCLUDE_REASONER_QUERY_CSTRINGSYNSETSRESULT_CPP
#define KONCLUDE_REASONER_QUERY_CSTRINGSYNSETSRESULT_CPP


#include "CStringSynsetsResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			template<class T, class E>
			CStringSynsetsResult<T,E>::CStringSynsetsResult() {
			}



			template<class T, class E>
			CStringSynsetsResult<T,E>::~CStringSynsetsResult() {
				qDeleteAll(mStringSynsetContainer);
			}


			template<class T, class E>
			E *CStringSynsetsResult<T,E>::getStringSynset(const QString &stringName, bool create) {
				E *stringSynset = mStringStringSynsetHash.value(stringName,0);
				if (!stringSynset && create) {
					stringSynset = new E(stringName);
					mStringStringSynsetHash.insert(stringName,stringSynset);
					mStringSynsetContainer.append(stringSynset);
				}
				return stringSynset;
			}

			template<class T, class E>
			E *CStringSynsetsResult<T,E>::getStringSynset(const QStringList &stringNames, bool create) {
				E *stringSynset = 0;
				if (!stringNames.isEmpty()) {
					stringSynset = mStringStringSynsetHash.value(stringNames.first(),0);
					if (!stringSynset && create) {
						stringSynset = new E(stringNames);
						foreach (QString stringName, stringNames) {
							mStringStringSynsetHash.insert(stringName,stringSynset);
						}
						mStringSynsetContainer.append(stringSynset);
					}
				}
				return stringSynset;
			}

			template<class T, class E>
			E *CStringSynsetsResult<T,E>::addStringSynset(E *takeStringSynset) {
				foreach (QString stringName, takeStringSynset->getEquivalentStringNameList()) {
					mStringStringSynsetHash.insert(stringName,takeStringSynset);
				}
				mStringSynsetContainer.append(takeStringSynset);
				return takeStringSynset;
			} 
 
			template<class T, class E>
			QList<E*>* CStringSynsetsResult<T,E>::getStringSynsetList() {
				return &mStringSynsetContainer;
			}


			template<class T, class E>
			QString CStringSynsetsResult<T,E>::getQueryResultString() {
				QString string;
				return string;
			}
 

			template<class T, class E>
			bool CStringSynsetsResult<T,E>::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (!otherQueryResult) {
					return false;
				}
				T *otherStringSynsets = dynamic_cast<T *>(otherQueryResult);
				if (!otherStringSynsets) {
					return false;
				}

				if (mStringStringSynsetHash.count() != otherStringSynsets->mStringStringSynsetHash.count()) {
					return false;
				}

				for (typename QList<E*>::const_iterator it = mStringSynsetContainer.constBegin(), itEnd = mStringSynsetContainer.constEnd(); it != itEnd; ++it) {
					E* stringSynRes(*it);
					E* otherStringSynRes = nullptr;
					QStringList stringNameList(stringSynRes->getEquivalentStringNameList());
					if (!stringNameList.isEmpty()) {
						const QString& className = stringNameList.first();
						otherStringSynRes = otherStringSynsets->mStringStringSynsetHash.value(className);
					}
					if (!otherStringSynRes && !stringNameList.isEmpty()) {
						return false;
					}
					if (!stringSynRes->isResultEquivalentTo(otherStringSynRes)) {
						return false;
					}
				}
				return true;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSTRINGSYNSETSRESULT_CPP
