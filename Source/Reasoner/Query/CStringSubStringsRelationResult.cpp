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

#ifndef KONCLUDE_REASONER_QUERY_CSTRINGSUBSTRINGSRELATIONRESULT_CPP
#define KONCLUDE_REASONER_QUERY_CSTRINGSUBSTRINGSRELATIONRESULT_CPP


#include "CStringSubStringsRelationResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			template<class T, class E>
			CStringSubStringsRelationResult<T,E>::CStringSubStringsRelationResult() {
				mSuperString = 0;
			}

			template<class T, class E>
			CStringSubStringsRelationResult<T,E>::CStringSubStringsRelationResult(E *superStringSynset) {
				mSuperString = superStringSynset;
			}

			template<class T, class E>
			CStringSubStringsRelationResult<T,E>::~CStringSubStringsRelationResult() {
			}


			template<class T, class E>
			E* CStringSubStringsRelationResult<T,E>::getSuperStringSynset() {
				return mSuperString;
			}

			template<class T, class E>
			T *CStringSubStringsRelationResult<T,E>::setSuperStringSynset(E* superStringSynset) {
				mSuperString = superStringSynset;
				return (T*)this;
			}


			template<class T, class E>
			QSet<E*>* CStringSubStringsRelationResult<T,E>::getSubStringsSynsetSet() {
				return &mSubStringSynsetSet;
			}

			template<class T, class E>
			QList<E*> CStringSubStringsRelationResult<T,E>::getSubStringsSynsetList() {
				return mSubStringSynsetSet.toList();
			}

			template<class T, class E>
			T *CStringSubStringsRelationResult<T,E>::addSubStringsSynset(E *subStringSynset) {
				mSubStringSynsetSet.insert(subStringSynset);
				return (T*)this;
			}

			template<class T, class E>
			T *CStringSubStringsRelationResult<T,E>::addSubStringsSynsets(const QList<E*> &subStringsSynsets) {
				foreach (E* stringSynRes, subStringsSynsets) {
					mSubStringSynsetSet.insert(stringSynRes);
				}
				return (T*)this;
			}

			template<class T, class E>
			qint64 CStringSubStringsRelationResult<T,E>::getSubStringsSynsetCount() {
				return mSubStringSynsetSet.count();
			}


			template<class T, class E>
			bool CStringSubStringsRelationResult<T,E>::hasSubStringsSynset(E *subStringSynset) {
				return mSubStringSynsetSet.contains(subStringSynset);
			}


			template<class T, class E>
			QString CStringSubStringsRelationResult<T,E>::getQueryResultString(const QString& typeStringName) {
				QString string;
				if (mSuperString && getSubStringsSynsetCount() > 0) {
					string += mSuperString->getQueryResultString();
				}
				if (!string.isEmpty()) {
					QString subStringString;
					foreach (E *subStringSynset, mSubStringSynsetSet) {
						if (subStringString.isEmpty()) {
							subStringString += QString(" has Sub-%1-Relation to %2").arg(typeStringName).arg(subStringSynset->getQueryResultString());
						} else {
							subStringString += QString(" and to %1").arg(subStringSynset->getQueryResultString());
						}
					}
					string += subStringString;
				}
				return string;
			}

			template<class T, class E>
			bool CStringSubStringsRelationResult<T,E>::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (!otherQueryResult) {
					return false;
				}
				T *otherSubClassRelation = dynamic_cast<T *>(otherQueryResult);
				if (!otherSubClassRelation) {
					return false;
				}
				if (!mSuperString->isResultEquivalentTo(otherSubClassRelation->getSuperStringSynset())) {
					return false;
				}
				QList<E *> otherSubStringList(otherSubClassRelation->getSubStringsSynsetList());				
				foreach (E *stringSynset, otherSubStringList) {
					bool contained = false;					
					for (qint64 cnt = otherSubStringList.count(); cnt > 0; --cnt) {
						contained |= otherSubStringList.first()->isResultEquivalentTo(stringSynset);
						if (contained) {
							otherSubStringList.takeFirst();
							break;
						} else {
							otherSubStringList.append(otherSubStringList.takeFirst());
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

#endif // KONCLUDE_REASONER_QUERY_CSTRINGSUBSTRINGSRELATIONRESULT_CPP
