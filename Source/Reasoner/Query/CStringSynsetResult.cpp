/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_QUERY_CSTRINGSYNSETRESULT_CPP
#define KONCLUDE_REASONER_QUERY_CSTRINGSYNSETRESULT_CPP


#include "CStringSynsetResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			template<class T>
			CStringSynsetResult<T>::CStringSynsetResult() {
			}

			template<class T>
			CStringSynsetResult<T>::CStringSynsetResult(const QString &stringName) {
				addEquivalentStringName(stringName);
			}

			template<class T>
			CStringSynsetResult<T>::CStringSynsetResult(const QStringList &stringNameList) {
				addEquivalentStringNames(stringNameList);
			}


			template<class T>
			CStringSynsetResult<T>::~CStringSynsetResult() {
			}


			template<class T>
			QStringList CStringSynsetResult<T>::getEquivalentStringNameList() {
				return mStringNameList;
			}

			template<class T>
			T* CStringSynsetResult<T>::addEquivalentStringName(const QString &stringName) {
				mStringNameList.append(stringName);
				return (T*)this;
			}

			template<class T>
			T* CStringSynsetResult<T>::addEquivalentStringNames(const QStringList &stringNameList) {
				mStringNameList += stringNameList;
				return (T*)this;
			}

			template<class T>
			qint64 CStringSynsetResult<T>::getStringCount() {
				return mStringNameList.count();
			}


			template<class T>
			bool CStringSynsetResult<T>::hasStringName(const QString &stringName) {
				return mStringNameList.contains(stringName);
			}

			template<class T>
			bool CStringSynsetResult<T>::hasStringNames(const QStringList &stringNameList) {
				if (mStringNameList.size() <= 1) {
					return mStringNameList.first() == stringNameList.first();
				} else if (mStringNameList.size() <= 20) {
					QStringList remEqStringNameList(mStringNameList);
					foreach (const QString& stringName, stringNameList) {
						bool contained = false;
						for (qint64 cnt = remEqStringNameList.count(); cnt > 0; --cnt) {
							contained |= remEqStringNameList.first() == stringName;
							if (contained) {
								remEqStringNameList.takeFirst();
								break;
							} else {
								remEqStringNameList.append(remEqStringNameList.takeFirst());
							}
						}
						if (!contained) {
							return false;
						}
					}
				} else {
					QHash<QString,cint64> stringNameCountHash;
					foreach (const QString& stringName, stringNameList) {
						if (stringNameCountHash.contains(stringName)) {
							++stringNameCountHash[stringName];
						} else {
							stringNameCountHash[stringName] = 1;
						}
					}
					foreach (const QString& stringName, mStringNameList) {
						if (!stringNameCountHash.contains(stringName)) {
							return false;
						} else {
							--stringNameCountHash[stringName];
						}
					}
					for (QHash<QString,cint64>::const_iterator it = stringNameCountHash.constBegin(), itEnd = stringNameCountHash.constEnd(); it != itEnd; ++it) {
						cint64 diff(it.value());
						if (diff != 0) {
							return false;
						}
					}
				}
				return true;
			}

			template<class T>
			QString CStringSynsetResult<T>::getQueryResultString(const QString& stringTypeName) {
				QString stringName = mStringNameList.join(", ");
				if (mStringNameList.count() > 1) {
					return QString("Equivalent %1 Set '%2'").arg(stringTypeName).arg(stringName);
				} else if (mStringNameList.count() > 0) {
					return QString("%1 '%2'").arg(stringTypeName).arg(stringName);
				} else {
					return QString("Empty %1 Set").arg(stringTypeName);
				}
			}

			template<class T>
			bool CStringSynsetResult<T>::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (!otherQueryResult) {
					return false;
				}
				T* otherClassSynset = dynamic_cast<T*>(otherQueryResult);
				if (!otherClassSynset) {
					return false;
				}
				if (getStringCount() != otherClassSynset->getStringCount()) {
					return false;
				}
				if (!hasStringNames(otherClassSynset->getEquivalentStringNameList())) {
					return false;
				}
				return true;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // KONCLUDE_REASONER_QUERY_CSTRINGSYNSETRESULT_CPP
