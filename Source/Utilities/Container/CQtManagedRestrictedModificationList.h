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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONLIST_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONLIST_H

// Namespace includes
#include "ContainerSettings.h"

#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_LIST_CONTAINER
#include "CQtManagedRestrictedModificationListData.h"
#include "CQtManagedRestrictedModificationListBase.h"

// Library includes
#include <QList>

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
			 *		\class		CQtManagedRestrictedModificationList
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template <typename T>
			class CQtManagedRestrictedModificationList : public CQtManagedRestrictedModificationListBase
			{
				struct Node { void *v;
#if defined(Q_CC_BOR)
				Q_INLINE_TEMPLATE T &t();
#else
				Q_INLINE_TEMPLATE T &t()
				{ return *reinterpret_cast<T*>(QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic
				? v : this); }
#endif
				};

				union { CQtManagedRestrictedModificationListData p; CQtManagedRestrictedModificationListData::Data *d; };
				CContext* mContext;
				CMemoryAllocationManager* mMemMan;

			public:
				inline CQtManagedRestrictedModificationList() : d(&CQtManagedRestrictedModificationListData::shared_null),mContext(nullptr) { 
					mMemMan = CContext::getMemoryAllocationManager(mContext);
					/*d->ref.ref();*/ 
				}
				inline CQtManagedRestrictedModificationList(CContext* context) : d(&CQtManagedRestrictedModificationListData::shared_null),mContext(context) { 
					mMemMan = CContext::getMemoryAllocationManager(mContext);
					/*d->ref.ref();*/ 
				}
				inline CQtManagedRestrictedModificationList(const CQtManagedRestrictedModificationList<T> &l) : d(l.d),mContext(nullptr) { 
					mMemMan = CContext::getMemoryAllocationManager(mContext);
					/*d->ref.ref();*/ if (!d->sharable) detach_helper(); 
				}
				inline CQtManagedRestrictedModificationList(const CQtManagedRestrictedModificationList<T> &l, CContext* context) : d(l.d),mContext(context) { 
					mMemMan = CContext::getMemoryAllocationManager(mContext);
					/*d->ref.ref();*/ if (!d->sharable) detach_helper(); 
				}
	
				~CQtManagedRestrictedModificationList();

				CQtManagedRestrictedModificationList<T>* init(CContext* context = nullptr);
				CQtManagedRestrictedModificationList<T>* init(const CQtManagedRestrictedModificationList<T> &other, CContext* context = nullptr);
				CQtManagedRestrictedModificationList<T>* init(const CQtManagedRestrictedModificationList<T> *other, CContext* context = nullptr);

				CQtManagedRestrictedModificationList<T> &operator=(const CQtManagedRestrictedModificationList<T> &l);



				bool operator==(const CQtManagedRestrictedModificationList<T> &l) const;
				inline bool operator!=(const CQtManagedRestrictedModificationList<T> &l) const { return !(*this == l); }

				inline int size() const { return p.size(); }

				inline void detach() { if (d->refList != this/*d->ref != 1*/) detach_helper(); }

				inline void detachShared()
				{
					// The "this->" qualification is needed for GCCE.
					if (d->refList != this/*d->ref != 1*/ && this->d != &CQtManagedRestrictedModificationListData::shared_null)
						detach_helper();
				}

				inline bool isDetached() const { return /*d->ref == 1*/d->refList == this; }
				inline void setSharable(bool sharable) { if (!sharable) detach(); d->sharable = sharable; }
				inline bool isSharedWith(const CQtManagedRestrictedModificationList<T> &other) const { return d == other.d; }

				inline bool isEmpty() const { return p.isEmpty(); }

				void clear();

				const T &at(int i) const;
				const T &operator[](int i) const;
				T &operator[](int i);

				void reserve(int size);
				void append(const T &t);
				void append(const CQtManagedRestrictedModificationList<T> &t);
				void prepend(const T &t);
				void insert(int i, const T &t);
				void replace(int i, const T &t);
				void removeAt(int i);
				int removeAll(const T &t);
				bool removeOne(const T &t);
				T takeAt(int i);
				T takeFirst();
				T takeLast();
				void move(int from, int to);
				void swap(int i, int j);
				int indexOf(const T &t, int from = 0) const;
				int lastIndexOf(const T &t, int from = -1) const;
                bool contains(const T &t) const;
				int count(const T &t) const;

				class const_iterator;

				class iterator {
				public:
					Node *i;
					typedef std::random_access_iterator_tag  iterator_category;
					typedef qptrdiff difference_type;
					typedef T value_type;
					typedef T *pointer;
					typedef T &reference;

					inline iterator() : i(0) {}
					inline iterator(Node *n) : i(n) {}
					inline iterator(const iterator &o): i(o.i){}
					inline T &operator*() const { return i->t(); }
					inline T *operator->() const { return &i->t(); }
					inline T &operator[](int j) const { return i[j].t(); }
					inline bool operator==(const iterator &o) const { return i == o.i; }
					inline bool operator!=(const iterator &o) const { return i != o.i; }
					inline bool operator<(const iterator& other) const { return i < other.i; }
					inline bool operator<=(const iterator& other) const { return i <= other.i; }
					inline bool operator>(const iterator& other) const { return i > other.i; }
					inline bool operator>=(const iterator& other) const { return i >= other.i; }
#ifndef QT_STRICT_ITERATORS
					inline bool operator==(const const_iterator &o) const
					{ return i == o.i; }
					inline bool operator!=(const const_iterator &o) const
					{ return i != o.i; }
					inline bool operator<(const const_iterator& other) const
					{ return i < other.i; }
					inline bool operator<=(const const_iterator& other) const
					{ return i <= other.i; }
					inline bool operator>(const const_iterator& other) const
					{ return i > other.i; }
					inline bool operator>=(const const_iterator& other) const
					{ return i >= other.i; }
#endif
					inline iterator &operator++() { ++i; return *this; }
					inline iterator operator++(int) { Node *n = i; ++i; return n; }
					inline iterator &operator--() { i--; return *this; }
					inline iterator operator--(int) { Node *n = i; i--; return n; }
					inline iterator &operator+=(int j) { i+=j; return *this; }
					inline iterator &operator-=(int j) { i-=j; return *this; }
					inline iterator operator+(int j) const { return iterator(i+j); }
					inline iterator operator-(int j) const { return iterator(i-j); }
					inline int operator-(iterator j) const { return int(i - j.i); }
				};
				friend class iterator;

				class const_iterator {
				public:
					Node *i;
					typedef std::random_access_iterator_tag  iterator_category;
					typedef qptrdiff difference_type;
					typedef T value_type;
					typedef const T *pointer;
					typedef const T &reference;

					inline const_iterator() : i(0) {}
					inline const_iterator(Node *n) : i(n) {}
					inline const_iterator(const const_iterator &o): i(o.i) {}
#ifdef QT_STRICT_ITERATORS
					inline explicit const_iterator(const iterator &o): i(o.i) {}
#else
					inline const_iterator(const iterator &o): i(o.i) {}
#endif
					inline const T &operator*() const { return i->t(); }
					inline const T *operator->() const { return &i->t(); }
					inline const T &operator[](int j) const { return i[j].t(); }
					inline bool operator==(const const_iterator &o) const { return i == o.i; }
					inline bool operator!=(const const_iterator &o) const { return i != o.i; }
					inline bool operator<(const const_iterator& other) const { return i < other.i; }
					inline bool operator<=(const const_iterator& other) const { return i <= other.i; }
					inline bool operator>(const const_iterator& other) const { return i > other.i; }
					inline bool operator>=(const const_iterator& other) const { return i >= other.i; }
					inline const_iterator &operator++() { ++i; return *this; }
					inline const_iterator operator++(int) { Node *n = i; ++i; return n; }
					inline const_iterator &operator--() { i--; return *this; }
					inline const_iterator operator--(int) { Node *n = i; i--; return n; }
					inline const_iterator &operator+=(int j) { i+=j; return *this; }
					inline const_iterator &operator-=(int j) { i-=j; return *this; }
					inline const_iterator operator+(int j) const { return const_iterator(i+j); }
					inline const_iterator operator-(int j) const { return const_iterator(i-j); }
					inline int operator-(const_iterator j) const { return i - j.i; }
				};
				friend class const_iterator;

				// stl style
				inline iterator begin() { detach(); return reinterpret_cast<Node *>(p.begin()); }
				inline const_iterator begin() const { return reinterpret_cast<Node *>(p.begin()); }
				inline const_iterator constBegin() const { return reinterpret_cast<Node *>(p.begin()); }
				inline iterator end() { detach(); return reinterpret_cast<Node *>(p.end()); }
				inline const_iterator end() const { return reinterpret_cast<Node *>(p.end()); }
				inline const_iterator constEnd() const { return reinterpret_cast<Node *>(p.end()); }
				iterator insert(iterator before, const T &t);
				iterator erase(iterator pos);
				iterator erase(iterator first, iterator last);

				// more Qt
				typedef iterator Iterator;
				typedef const_iterator ConstIterator;
				inline int count() const { return p.size(); }
				inline int length() const { return p.size(); } // Same as count()
				inline T& first() { Q_ASSERT(!isEmpty()); return *begin(); }
				inline const T& first() const { Q_ASSERT(!isEmpty()); return at(0); }
				T& last() { Q_ASSERT(!isEmpty()); return *(--end()); }
				const T& last() const { Q_ASSERT(!isEmpty()); return at(count() - 1); }
				inline void removeFirst() { Q_ASSERT(!isEmpty()); erase(begin()); }
				inline void removeLast() { Q_ASSERT(!isEmpty()); erase(--end()); }
				inline bool startsWith(const T &t) const { return !isEmpty() && first() == t; }
				inline bool endsWith(const T &t) const { return !isEmpty() && last() == t; }
				CQtManagedRestrictedModificationList<T> mid(int pos, int length = -1) const;

				T value(int i) const;
				T value(int i, const T &defaultValue) const;

				// stl compatibility
				inline void push_back(const T &t) { append(t); }
				inline void push_front(const T &t) { prepend(t); }
				inline T& front() { return first(); }
				inline const T& front() const { return first(); }
				inline T& back() { return last(); }
				inline const T& back() const { return last(); }
				inline void pop_front() { removeFirst(); }
				inline void pop_back() { removeLast(); }
				inline bool empty() const { return isEmpty(); }
				typedef int size_type;
				typedef T value_type;
				typedef value_type *pointer;
				typedef const value_type *const_pointer;
				typedef value_type &reference;
				typedef const value_type &const_reference;
				typedef qptrdiff difference_type;

#ifdef QT3_SUPPORT
				inline QT3_SUPPORT iterator remove(iterator pos) { return erase(pos); }
				inline QT3_SUPPORT int remove(const T &t) { return removeAll(t); }
				inline QT3_SUPPORT int findIndex(const T& t) const { return indexOf(t); }
				inline QT3_SUPPORT iterator find(const T& t)
				{ int i = indexOf(t); return (i == -1 ? end() : (begin()+i)); }
				inline QT3_SUPPORT const_iterator find (const T& t) const
				{ int i = indexOf(t); return (i == -1 ? end() : (begin()+i)); }
				inline QT3_SUPPORT iterator find(iterator from, const T& t)
				{ int i = indexOf(t, from - begin()); return i == -1 ? end() : begin()+i; }
				inline QT3_SUPPORT const_iterator find(const_iterator from, const T& t) const
				{ int i = indexOf(t, from - begin()); return i == -1 ? end() : begin()+i; }
#endif

				// comfort
				CQtManagedRestrictedModificationList<T> &operator+=(const CQtManagedRestrictedModificationList<T> &l);
				inline CQtManagedRestrictedModificationList<T> operator+(const CQtManagedRestrictedModificationList<T> &l) const
				{ CQtManagedRestrictedModificationList n = *this; n += l; return n; }
				inline CQtManagedRestrictedModificationList<T> &operator+=(const T &t)
				{ append(t); return *this; }
				inline CQtManagedRestrictedModificationList<T> &operator<< (const T &t)
				{ append(t); return *this; }
				inline CQtManagedRestrictedModificationList<T> &operator<<(const CQtManagedRestrictedModificationList<T> &l)
				{ *this += l; return *this; }

				QVector<T> toVector() const;
				QSet<T> toSet() const;

				static CQtManagedRestrictedModificationList<T> fromVector(const QVector<T> &vector);
				static CQtManagedRestrictedModificationList<T> fromSet(const QSet<T> &set);

#ifndef QT_NO_STL
				static inline CQtManagedRestrictedModificationList<T> fromStdList(const std::list<T> &list)
				{ CQtManagedRestrictedModificationList<T> tmp; qCopy(list.begin(), list.end(), std::back_inserter(tmp)); return tmp; }
				inline std::list<T> toStdList() const
				{ std::list<T> tmp; qCopy(constBegin(), constEnd(), std::back_inserter(tmp)); return tmp; }
#endif

			private:
				Node *detach_helper_grow(int i, int n);
				void detach_helper(int alloc);
				void detach_helper();
				void free(CQtManagedRestrictedModificationListData::Data *d);

				void node_construct(Node *n, const T &t);
				void node_destruct(Node *n);
				void node_copy(Node *from, Node *to, Node *src);
				void node_destruct(Node *from, Node *to);
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CQtManagedRestrictedModificationList.cpp"

#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_LIST_CONTAINER

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONLIST_H
