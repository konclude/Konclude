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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSET_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSET_H

// Namespace includes
#include "ContainerSettings.h"
#include "CQtManagedRestrictedModificationHash.h"

#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_SET_CONTAINER

// Library includes
#include <QSet>
#include <QHash>

// Other includes
#include "Context/CContext.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Context;

	namespace Utilities {

		namespace Container {

			/*! 
			 *
			 *		\class		CQtManagedRestrictedModificationSet
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template <class T>
			class CQtManagedRestrictedModificationSet
			{
				typedef CQtManagedRestrictedModificationHash<T, CQtManagedRestrictedModificationHashDummyValue> Hash;

			public:
				inline CQtManagedRestrictedModificationSet(CContext* context = nullptr) : mContext(context),mHash(context) {}
				inline CQtManagedRestrictedModificationSet(const CQtManagedRestrictedModificationSet<T> &other) : mHash(other.mHash,nullptr),mContext(nullptr) {}
				inline CQtManagedRestrictedModificationSet(const CQtManagedRestrictedModificationSet<T> &other, CContext* context) : mContext(context),mHash(other.mHash,context) {}


				CQtManagedRestrictedModificationSet<T>* init(CContext* context = nullptr);
				CQtManagedRestrictedModificationSet<T>* init(const CQtManagedRestrictedModificationSet<T> &other, CContext* context = nullptr);
				CQtManagedRestrictedModificationSet<T>* init(CQtManagedRestrictedModificationSet<T> *other, CContext* context = nullptr);


				inline CQtManagedRestrictedModificationSet<T> &operator=(const CQtManagedRestrictedModificationSet<T> &other)
				{ mHash.init(other.mHash,mContext); return *this; }

				inline bool operator==(const CQtManagedRestrictedModificationSet<T> &other) const
				{ return mHash == other.mHash; }
				inline bool operator!=(const CQtManagedRestrictedModificationSet<T> &other) const
				{ return mHash != other.mHash; }

				inline int size() const { return mHash.size(); }

				inline bool isEmpty() const { return mHash.isEmpty(); }

				inline int capacity() const { return mHash.capacity(); }
				inline void reserve(int size);
				inline void squeeze() { mHash.squeeze(); }

				inline void detach() { mHash.detach(); }
				inline bool isDetached() const { return mHash.isDetached(); }
				inline void setSharable(bool sharable) { mHash.setSharable(sharable); }

				inline void clear() { mHash.clear(); }

				inline bool remove(const T &value) { return mHash.remove(value) != 0; }

				inline bool contains(const T &value) const { return mHash.contains(value); }

				bool contains(const CQtManagedRestrictedModificationSet<T> &set) const;

				class const_iterator;

				class iterator
				{
					typedef CQtManagedRestrictedModificationHash<T, CQtManagedRestrictedModificationHashDummyValue> Hash;
					typename Hash::iterator i;
					friend class const_iterator;

				public:
					typedef std::bidirectional_iterator_tag iterator_category;
					typedef qptrdiff difference_type;
					typedef T value_type;
					typedef const T *pointer;
					typedef const T &reference;

					inline iterator() {}
					inline iterator(typename Hash::iterator o) : i(o) {}
					inline iterator(const iterator &o) : i(o.i) {}
					inline iterator &operator=(const iterator &o) { i = o.i; return *this; }
					inline const T &operator*() const { return i.key(); }
					inline const T *operator->() const { return &i.key(); }
					inline bool operator==(const iterator &o) const { return i == o.i; }
					inline bool operator!=(const iterator &o) const { return i != o.i; }
					inline bool operator==(const const_iterator &o) const
					{ return i == o.i; }
					inline bool operator!=(const const_iterator &o) const
					{ return i != o.i; }
					inline iterator &operator++() { ++i; return *this; }
					inline iterator operator++(int) { iterator r = *this; ++i; return r; }
					inline iterator &operator--() { --i; return *this; }
					inline iterator operator--(int) { iterator r = *this; --i; return r; }
					inline iterator operator+(int j) const { return i + j; }
					inline iterator operator-(int j) const { return i - j; }
					inline iterator &operator+=(int j) { i += j; return *this; }
					inline iterator &operator-=(int j) { i -= j; return *this; }
				};

				class const_iterator
				{
					typedef CQtManagedRestrictedModificationHash<T, CQtManagedRestrictedModificationHashDummyValue> Hash;
					typename Hash::const_iterator i;
					friend class iterator;

				public:
					typedef std::bidirectional_iterator_tag iterator_category;
					typedef qptrdiff difference_type;
					typedef T value_type;
					typedef const T *pointer;
					typedef const T &reference;

					inline const_iterator() {}
					inline const_iterator(typename Hash::const_iterator o) : i(o) {}
					inline const_iterator(const const_iterator &o) : i(o.i) {}
					inline const_iterator(const iterator &o)
						: i(o.i) {}
					inline const_iterator &operator=(const const_iterator &o) { i = o.i; return *this; }
					inline const T &operator*() const { return i.key(); }
					inline const T *operator->() const { return &i.key(); }
					inline bool operator==(const const_iterator &o) const { return i == o.i; }
					inline bool operator!=(const const_iterator &o) const { return i != o.i; }
					inline const_iterator &operator++() { ++i; return *this; }
					inline const_iterator operator++(int) { const_iterator r = *this; ++i; return r; }
					inline const_iterator &operator--() { --i; return *this; }
					inline const_iterator operator--(int) { const_iterator r = *this; --i; return r; }
					inline const_iterator operator+(int j) const { return i + j; }
					inline const_iterator operator-(int j) const { return i - j; }
					inline const_iterator &operator+=(int j) { i += j; return *this; }
					inline const_iterator &operator-=(int j) { i -= j; return *this; }
				};

				// STL style
				inline iterator begin() { return mHash.begin(); }
				inline const_iterator begin() const { return mHash.begin(); }
				inline const_iterator constBegin() const { return mHash.constBegin(); }
				inline iterator end() { return mHash.end(); }
				inline const_iterator end() const { return mHash.end(); }
				inline const_iterator constEnd() const { return mHash.constEnd(); }
				iterator erase(iterator i)
				{ return mHash.erase(reinterpret_cast<typename Hash::iterator &>(i)); }

				// more Qt
				typedef iterator Iterator;
				typedef const_iterator ConstIterator;
				inline int count() const { return mHash.count(); }
				inline const_iterator insert(const T &value) // ### Qt 5: should return an 'iterator'
				{ return static_cast<typename Hash::const_iterator>(mHash.insert(value,
				CQtManagedRestrictedModificationHashDummyValue())); }
				iterator find(const T &value) { return mHash.find(value); }
				const_iterator find(const T &value) const { return mHash.find(value); }
				inline const_iterator constFind(const T &value) const { return find(value); }
				CQtManagedRestrictedModificationSet<T> &unite(const CQtManagedRestrictedModificationSet<T> &other);
				CQtManagedRestrictedModificationSet<T> &intersect(const CQtManagedRestrictedModificationSet<T> &other);
				CQtManagedRestrictedModificationSet<T> &subtract(const CQtManagedRestrictedModificationSet<T> &other);

				// STL compatibility
				typedef T key_type;
				typedef T value_type;
				typedef value_type *pointer;
				typedef const value_type *const_pointer;
				typedef value_type &reference;
				typedef const value_type &const_reference;
				typedef qptrdiff difference_type;
				typedef int size_type;

				inline bool empty() const { return isEmpty(); }
				// comfort
				inline CQtManagedRestrictedModificationSet<T> &operator<<(const T &value) { insert(value); return *this; }
				inline CQtManagedRestrictedModificationSet<T> &operator|=(const CQtManagedRestrictedModificationSet<T> &other) { unite(other); return *this; }
				inline CQtManagedRestrictedModificationSet<T> &operator|=(const T &value) { insert(value); return *this; }
				inline CQtManagedRestrictedModificationSet<T> &operator&=(const CQtManagedRestrictedModificationSet<T> &other) { intersect(other); return *this; }
				inline CQtManagedRestrictedModificationSet<T> &operator&=(const T &value)
				{ CQtManagedRestrictedModificationSet<T> result; if (contains(value)) result.insert(value); return (*this = result); }
				inline CQtManagedRestrictedModificationSet<T> &operator+=(const CQtManagedRestrictedModificationSet<T> &other) { unite(other); return *this; }
				inline CQtManagedRestrictedModificationSet<T> &operator+=(const T &value) { insert(value); return *this; }
				inline CQtManagedRestrictedModificationSet<T> &operator-=(const CQtManagedRestrictedModificationSet<T> &other) { subtract(other); return *this; }
				inline CQtManagedRestrictedModificationSet<T> &operator-=(const T &value) { remove(value); return *this; }
				inline CQtManagedRestrictedModificationSet<T> operator|(const CQtManagedRestrictedModificationSet<T> &other) const
				{ CQtManagedRestrictedModificationSet<T> result = *this; result |= other; return result; }
				inline CQtManagedRestrictedModificationSet<T> operator&(const CQtManagedRestrictedModificationSet<T> &other) const
				{ CQtManagedRestrictedModificationSet<T> result = *this; result &= other; return result; }
				inline CQtManagedRestrictedModificationSet<T> operator+(const CQtManagedRestrictedModificationSet<T> &other) const
				{ CQtManagedRestrictedModificationSet<T> result = *this; result += other; return result; }
				inline CQtManagedRestrictedModificationSet<T> operator-(const CQtManagedRestrictedModificationSet<T> &other) const
				{ CQtManagedRestrictedModificationSet<T> result = *this; result -= other; return result; }
#if QT_VERSION < 0x050000
				// ### Qt 5: remove
				inline CQtManagedRestrictedModificationSet<T> operator|(const CQtManagedRestrictedModificationSet<T> &other)
				{ CQtManagedRestrictedModificationSet<T> result = *this; result |= other; return result; }
				inline CQtManagedRestrictedModificationSet<T> operator&(const CQtManagedRestrictedModificationSet<T> &other)
				{ CQtManagedRestrictedModificationSet<T> result = *this; result &= other; return result; }
				inline CQtManagedRestrictedModificationSet<T> operator+(const CQtManagedRestrictedModificationSet<T> &other)
				{ CQtManagedRestrictedModificationSet<T> result = *this; result += other; return result; }
				inline CQtManagedRestrictedModificationSet<T> operator-(const CQtManagedRestrictedModificationSet<T> &other)
				{ CQtManagedRestrictedModificationSet<T> result = *this; result -= other; return result; }
#endif

				QList<T> toList() const;
				inline QList<T> values() const { return toList(); }

				static CQtManagedRestrictedModificationSet<T> fromList(const QList<T> &list);

			private:
				Hash mHash;
				CContext* mContext;
			};



		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CQtManagedRestrictedModificationSet.cpp"

#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_SET_CONTAINER

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSET_H
