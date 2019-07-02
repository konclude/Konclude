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
#ifndef KONCLUDE_REASONER_QUERY_CSTRINGSETRESULT_CPP
#define KONCLUDE_REASONER_QUERY_CSTRINGSETRESULT_CPP



#include "CStringSetResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			template<class T>
			CStringSetResult<T>::CStringSetResult() {
			}


			template<class T>
			QString CStringSetResult<T>::getQueryResultString() {
				return QString();
			}

			template<class T>
			bool CStringSetResult<T>::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				T* stringSetResult = dynamic_cast<T*>(otherQueryResult);
				if (stringSetResult) {
					if (mStringsSet.count() == stringSetResult->mStringsSet.count()) {
						for (QSet<QString>::const_iterator it = mStringsSet.constBegin(), itEnd = mStringsSet.constEnd(); it != itEnd; ++it) {
							const QString& stringName(*it);
							if (!stringSetResult->mStringsSet.contains(stringName)) {
								return false;
							}
						}
						return true;
					}
				}
				return false;
			}

			template<class T>
			T* CStringSetResult<T>::addString(const QString &stringName) {
				mStringsSet.insert(stringName);
				return (T*)this;
			}

			template<class T>
			QSet<QString>* CStringSetResult<T>::getStringsSet() {
				return &mStringsSet;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSTRINGSETRESULT_CPP

