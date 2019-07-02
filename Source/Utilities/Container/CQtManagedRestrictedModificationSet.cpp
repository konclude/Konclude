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
 * 		This file incorporates work covered by the following copyright and  
 * 		permission notice (to be compatible with LGPLv2.1,
 *		this code is available/released under GPLv3, 
 * 		see https://www.gnu.org/licenses/gpl-faq.html#AllCompatibility):
 *
 *
 * 			Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 * 			Contact: http://www.qt-project.org/legal
 *
 * 			This file is part of the QtCore module of the Qt Toolkit.
 *
 * 			$QT_BEGIN_LICENSE:LGPL$
 * 			Commercial License Usage
 * 			Licensees holding valid commercial Qt licenses may use this file in
 * 			accordance with the commercial license agreement provided with the
 * 			Software or, alternatively, in accordance with the terms contained in
 * 			a written agreement between you and Digia.  For licensing terms and
 * 			conditions see http://qt.digia.com/licensing.  For further information
 * 			use the contact form at http://qt.digia.com/contact-us.
 *
 * 			GNU Lesser General Public License Usage
 * 			Alternatively, this file may be used under the terms of the GNU Lesser
 * 			General Public License version 2.1 as published by the Free Software
 * 			Foundation and appearing in the file LICENSE.LGPL included in the
 *			packaging of this file.  Please review the following information to
 * 			ensure the GNU Lesser General Public License version 2.1 requirements
 * 			will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * 			In addition, as a special exception, Digia gives you certain additional
 * 			rights.  These rights are described in the Digia Qt LGPL Exception
 * 			version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 *
 * 			GNU General Public License Usage
 * 			Alternatively, this file may be used under the terms of the GNU
 * 			General Public License version 3.0 as published by the Free Software
 * 			Foundation and appearing in the file LICENSE.GPL included in the
 * 			packaging of this file.  Please review the following information to
 * 			ensure the GNU General Public License version 3.0 requirements will be
 * 			met: http://www.gnu.org/copyleft/gpl.html.
 *
 *
 * 			$QT_END_LICENSE$
 *
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
