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

#ifndef KONCLUDE_REASONER_QUERY_CSTRINGSRESULT_CPP
#define KONCLUDE_REASONER_QUERY_CSTRINGSRESULT_CPP


#include "CStringsResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			template<class T>
			CStringsResult<T>::CStringsResult() {
			}


			template<class T>
			QString CStringsResult<T>::getQueryResultString() {
				return QString();
			}

			template<class T>
			bool CStringsResult<T>::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				T* stringsResult = dynamic_cast<T*>(otherQueryResult);
				if (stringsResult) {
					if (mStringSet.count() == stringsResult->mStringSet.count()) {
						for (QSet<QString>::const_iterator it = mStringSet.constBegin(), itEnd = mStringSet.constEnd(); it != itEnd; ++it) {
							const QString& stringString(*it);
							if (!stringsResult->mStringSet.contains(stringString)) {
								return false;
							}
						}
						return true;
					}
				}
				return false;
			}

			template<class T>
			T* CStringsResult<T>::addString(const QString &stringName) {
				mStringSet.insert(stringName);
				return (T*)this;
			}

			template<class T>
			QSet<QString>* CStringsResult<T>::getStringsSet() {
				return &mStringSet;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSTRINGSRESULT_CPP
