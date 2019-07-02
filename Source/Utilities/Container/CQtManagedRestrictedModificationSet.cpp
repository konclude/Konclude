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
 *		Note, this is a modified version of the QSet from the Qt framework.
 *
 */

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSET_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSET_CPP


#include "CQtManagedRestrictedModificationSet.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_SET_CONTAINER

			template <class T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationSet<T>::reserve(int asize) { mHash.reserve(asize); }

			template <class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationSet<T> &CQtManagedRestrictedModificationSet<T>::unite(const CQtManagedRestrictedModificationSet<T> &other)
			{
				CQtManagedRestrictedModificationSet<T> copy(other);
				typename CQtManagedRestrictedModificationSet<T>::const_iterator i = copy.constEnd();
				while (i != copy.constBegin()) {
					--i;
					insert(*i);
				}
				return *this;
			}

			template <class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationSet<T> &CQtManagedRestrictedModificationSet<T>::intersect(const CQtManagedRestrictedModificationSet<T> &other)
			{
				CQtManagedRestrictedModificationSet<T> copy1(*this);
				CQtManagedRestrictedModificationSet<T> copy2(other);
				typename CQtManagedRestrictedModificationSet<T>::const_iterator i = copy1.constEnd();
				while (i != copy1.constBegin()) {
					--i;
					if (!copy2.contains(*i))
						remove(*i);
				}
				return *this;
			}

			template <class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationSet<T> &CQtManagedRestrictedModificationSet<T>::subtract(const CQtManagedRestrictedModificationSet<T> &other)
			{
				CQtManagedRestrictedModificationSet<T> copy1(*this);
				CQtManagedRestrictedModificationSet<T> copy2(other);
				typename CQtManagedRestrictedModificationSet<T>::const_iterator i = copy1.constEnd();
				while (i != copy1.constBegin()) {
					--i;
					if (copy2.contains(*i))
						remove(*i);
				}
				return *this;
			}

			template <class T>
			Q_INLINE_TEMPLATE bool CQtManagedRestrictedModificationSet<T>::contains(const CQtManagedRestrictedModificationSet<T> &other) const
			{
				typename CQtManagedRestrictedModificationSet<T>::const_iterator i = other.constBegin();
				while (i != other.constEnd()) {
					if (!contains(*i))
						return false;
					++i;
				}
				return true;
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE QList<T> CQtManagedRestrictedModificationSet<T>::toList() const
			{
				QList<T> result;
				result.reserve(size());
				typename CQtManagedRestrictedModificationSet<T>::const_iterator i = constBegin();
				while (i != constEnd()) {
					result.append(*i);
					++i;
				}
				return result;
			}

			//template <typename T>
			//Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationSet<T> QList<T>::toSet() const
			//{
			//	CQtManagedRestrictedModificationSet<T> result;
			//	result.reserve(size());
			//	for (int i = 0; i < size(); ++i)
			//		result.insert(at(i));
			//	return result;
			//}

			template <typename T>
			CQtManagedRestrictedModificationSet<T> CQtManagedRestrictedModificationSet<T>::fromList(const QList<T> &list)
			{
				return list.toSet();
			}

			//template <typename T>
			//QList<T> QList<T>::fromSet(const CQtManagedRestrictedModificationSet<T> &set)
			//{
			//	return set.toList();
			//}


			template <typename T>
			CQtManagedRestrictedModificationSet<T>* CQtManagedRestrictedModificationSet<T>::init(CContext* context) {
				mHash.init(mContext);
				mContext = context;
				return this;
			}

			template <typename T>
			CQtManagedRestrictedModificationSet<T>* CQtManagedRestrictedModificationSet<T>::init(const CQtManagedRestrictedModificationSet<T> &other, CContext* context) {
				mHash.init(other.mHash,mContext);
				mContext = context;
				return this;
			}

			template <typename T>
			CQtManagedRestrictedModificationSet<T>* CQtManagedRestrictedModificationSet<T>::init(CQtManagedRestrictedModificationSet<T> *other, CContext* context) {
				if (other) {
					mHash.init(other->mHash,mContext);
				} else {
					mHash.init(mContext);
				}
				mContext = context;
				return this;
			}



#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_SET_CONTAINER


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSET_CPP
