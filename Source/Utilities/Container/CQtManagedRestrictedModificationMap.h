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
 * 		This file incorporates work covered by the following copyright and  
 * 		permission notice:   
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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONMAP_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONMAP_H

// Namespace includes
#include "ContainerSettings.h"
#include "CQtManagedRestrictedModificationMapBase.h"
#include "CQtManagedRestrictedModificationMapData.h"
#include "CQtManagedRestrictedModificationList.h"

#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_MAP_CONTAINER

// Library includes
#include <QHash>
#include <map>

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
			 *		\class		CQtManagedRestrictedModificationMap
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			

				/*
					CQtManagedRestrictedModificationMap uses cMapLessThanKey() to compare keys. The default
					implementation uses operator<(). For pointer types,
					cMapLessThanKey() casts the pointers to integers before it
					compares them, because operator<() is undefined on pointers
					that come from different memory blocks. (In practice, this
					is only a problem when running a program such as
					BoundsChecker.)
				*/

				template <class Key> inline bool cMapLessThanKey(const Key &key1, const Key &key2)
				{
					return key1 < key2;
				}

				#ifndef QT_NO_PARTIAL_TEMPLATE_SPECIALIZATION
				template <class Ptr> inline bool cMapLessThanKey(Ptr *key1, Ptr *key2)
				{
					Q_ASSERT(sizeof(quintptr) == sizeof(Ptr *));
					return quintptr(key1) < quintptr(key2);
				}

				template <class Ptr> inline bool cMapLessThanKey(const Ptr *key1, const Ptr *key2)
				{
					Q_ASSERT(sizeof(quintptr) == sizeof(const Ptr *));
					return quintptr(key1) < quintptr(key2);
				}
				#endif // QT_NO_PARTIAL_TEMPLATE_SPECIALIZATION

				template <class Key, class T>
				struct CQtManagedRestrictedModificationMapNode {
					Key key;
					T value;

				private:
					// never access these members through this structure.
					// see below
					CQtManagedRestrictedModificationMapData::Node *backward;
					CQtManagedRestrictedModificationMapData::Node *forward[1];
				};

				template <class Key, class T>
				struct CQtManagedRestrictedModificationMapPayloadNode
				{
					Key key;
					T value;

				private:
					// CQtManagedRestrictedModificationMap::e is a pointer to CQtManagedRestrictedModificationMapData::Node, which matches the member
					// below. However, the memory allocation node in CQtManagedRestrictedModificationMapData::node_create
					// allocates sizeof(CQtManagedRestrictedModificationMapPayloNode) and incorrectly calculates the offset
					// of 'backward' below. If the alignment of CQtManagedRestrictedModificationMapPayloadNode is larger
					// than the alignment of a pointer, the 'backward' member is aligned to
					// the end of this structure, not to 'value' above, and will occupy the
					// tail-padding area.
					//
					//  e.g., on a 32-bit archictecture with Key = int and
					//        sizeof(T) = alignof(T) = 8
					//   0        4        8        12       16       20       24  byte
					//   |   key  |   PAD  |      value      |backward|  PAD   |   correct layout
					//   |   key  |   PAD  |      value      |        |backward|   how it's actually used
					//   |<-----  value of CQtManagedRestrictedModificationMap::payload() = 20 ----->|
					CQtManagedRestrictedModificationMapData::Node *backward;
				};

				template <class Key, class T>
				class CQtManagedRestrictedModificationMap : public CQtManagedRestrictedModificationMapBase
				{
					typedef CQtManagedRestrictedModificationMapNode<Key, T> Node;
					typedef CQtManagedRestrictedModificationMapPayloadNode<Key, T> PayloadNode;

					union {
						CQtManagedRestrictedModificationMapData *d;
						CQtManagedRestrictedModificationMapData::Node *e;
					};
					CContext* mContext;
					CMemoryAllocationManager* mMemMan;


					static inline int payload() { return sizeof(PayloadNode) - sizeof(CQtManagedRestrictedModificationMapData::Node *); }
					static inline int alignment() {
#ifdef Q_ALIGNOF
						return int(qMax(sizeof(void*), Q_ALIGNOF(Node)));
#else
						return 0;
#endif
					}
					static inline Node *concrete(CQtManagedRestrictedModificationMapData::Node *node) {
						return reinterpret_cast<Node *>(reinterpret_cast<char *>(node) - payload());
					}

				public:
					inline CQtManagedRestrictedModificationMap() : d(&CQtManagedRestrictedModificationMapData::shared_null),mContext(nullptr) { 
						mMemMan = CContext::getMemoryAllocationManager(mContext);
						/*d->ref.ref();*/ 
					}
					inline CQtManagedRestrictedModificationMap(CContext* context) : d(&CQtManagedRestrictedModificationMapData::shared_null),mContext(context) { 
						mMemMan = CContext::getMemoryAllocationManager(mContext);
						/*d->ref.ref();*/ 
					}
					inline CQtManagedRestrictedModificationMap(const CQtManagedRestrictedModificationMap<Key, T> &other) : d(other.d),mContext(nullptr) { 
						mMemMan = CContext::getMemoryAllocationManager(mContext);
						/*d->ref.ref();*/ if (!d->sharable) detach(); 
					}
					inline CQtManagedRestrictedModificationMap(const CQtManagedRestrictedModificationMap<Key, T> &other, CContext* context) : d(other.d),mContext(context) { 
						mMemMan = CContext::getMemoryAllocationManager(mContext);
						/*d->ref.ref();*/ if (!d->sharable) detach(); 
					}
					inline ~CQtManagedRestrictedModificationMap() { if (!d) return; if (/*!d->ref.deref()*/ d->refMap == this) freeData(d); }

					CQtManagedRestrictedModificationMap<Key, T> &operator=(const CQtManagedRestrictedModificationMap<Key, T> &other);

					CQtManagedRestrictedModificationMap<Key, T>* init(CContext* context = nullptr);
					CQtManagedRestrictedModificationMap<Key, T>* init(const CQtManagedRestrictedModificationMap<Key, T> &other, CContext* context = nullptr);
					CQtManagedRestrictedModificationMap<Key, T>* init(const CQtManagedRestrictedModificationMap<Key, T>* other, CContext* context = nullptr);


					bool isKeySubSetOf(const CQtManagedRestrictedModificationMap<Key, T>* other, cint64 testDirectSubSetFactorCount = 20);

#ifndef QT_NO_STL
					explicit CQtManagedRestrictedModificationMap(const typename std::map<Key, T> &other);
					std::map<Key, T> toStdMap() const;
#endif

					bool operator==(const CQtManagedRestrictedModificationMap<Key, T> &other) const;
					inline bool operator!=(const CQtManagedRestrictedModificationMap<Key, T> &other) const { return !(*this == other); }

					inline int size() const { return d->size; }

					inline bool isEmpty() const { return d->size == 0; }

					inline void detach() { if (/*d->ref != 1*/ d->refMap != this) detach_helper(); }
					inline bool isDetached() const { return /*d->ref == 1*/ d->refMap == this; }
					inline void setSharable(bool sharable) { if (!sharable) detach(); d->sharable = sharable; }
					inline bool isSharedWith(const CQtManagedRestrictedModificationMap<Key, T> &other) const { return d == other.d; }
					inline void setInsertInOrder(bool ordered) { d->insertInOrder = ordered; }

					void clear();

					int remove(const Key &key);
					T take(const Key &key);

					bool contains(const Key &key) const;
					const Key key(const T &value) const;
					const Key key(const T &value, const Key &defaultKey) const;
					const T value(const Key &key) const;
					const T value(const Key &key, const T &defaultValue) const;
					T &operator[](const Key &key);
					const T operator[](const Key &key) const;

					bool tryGetValue(const Key &key, T* value = nullptr) const;
					bool tryGetValuePointer(const Key &key, T*& valuePointer) const;

					QList<Key> uniqueKeys() const;
					QList<Key> keys() const;
					QList<Key> keys(const T &value) const;
					QList<T> values() const;
					QList<T> values(const Key &key) const;
					int count(const Key &key) const;

					class const_iterator;

					class iterator
					{
						friend class const_iterator;
						CQtManagedRestrictedModificationMapData::Node *i;

					public:
						typedef std::bidirectional_iterator_tag iterator_category;
						typedef qptrdiff difference_type;
						typedef T value_type;
						typedef T *pointer;
						typedef T &reference;

						// ### Qt 5: get rid of 'operator Node *'
						inline operator CQtManagedRestrictedModificationMapData::Node *() const { return i; }
						inline iterator() : i(0) { }
						inline iterator(CQtManagedRestrictedModificationMapData::Node *node) : i(node) { }

						inline const Key &key() const { return concrete(i)->key; }
						inline T &value() const { return concrete(i)->value; }

						inline T &operator*() const { return concrete(i)->value; }
						inline T *operator->() const { return &concrete(i)->value; }
						inline bool operator==(const iterator &o) const { return i == o.i; }
						inline bool operator!=(const iterator &o) const { return i != o.i; }

						inline iterator &operator++() {
							i = i->forward[0];
							return *this;
						}
						inline iterator operator++(int) {
							iterator r = *this;
							i = i->forward[0];
							return r;
						}
						inline iterator &operator--() {
							i = i->backward;
							return *this;
						}
						inline iterator operator--(int) {
							iterator r = *this;
							i = i->backward;
							return r;
						}
						inline iterator operator+(int j) const
						{ iterator r = *this; if (j > 0) while (j--) ++r; else while (j++) --r; return r; }
						inline iterator operator-(int j) const { return operator+(-j); }
						inline iterator &operator+=(int j) { return *this = *this + j; }
						inline iterator &operator-=(int j) { return *this = *this - j; }

						// ### Qt 5: not sure this is necessary anymore
#ifdef QT_STRICT_ITERATORS
					private:
#else
					public:
#endif
						inline bool operator==(const const_iterator &o) const
							{ return i == o.i; }
						inline bool operator!=(const const_iterator &o) const
							{ return i != o.i; }

					private:
						// ### Qt 5: remove
						inline operator bool() const { return false; }
					};
					friend class iterator;

					class const_iterator
					{
						friend class iterator;
						CQtManagedRestrictedModificationMapData::Node *i;

					public:
						typedef std::bidirectional_iterator_tag iterator_category;
						typedef qptrdiff difference_type;
						typedef T value_type;
						typedef const T *pointer;
						typedef const T &reference;

						// ### Qt 5: get rid of 'operator Node *'
						inline operator CQtManagedRestrictedModificationMapData::Node *() const { return i; }
						inline const_iterator() : i(0) { }
						inline const_iterator(CQtManagedRestrictedModificationMapData::Node *node) : i(node) { }
#ifdef QT_STRICT_ITERATORS
						explicit inline const_iterator(const iterator &o)
#else
						inline const_iterator(const iterator &o)
#endif
						{ i = o.i; }

						inline const Key &key() const { return concrete(i)->key; }
						inline const T &value() const { return concrete(i)->value; }
						inline const T &operator*() const { return concrete(i)->value; }
						inline const T *operator->() const { return &concrete(i)->value; }
						inline bool operator==(const const_iterator &o) const { return i == o.i; }
						inline bool operator!=(const const_iterator &o) const { return i != o.i; }

						inline const_iterator &operator++() {
							i = i->forward[0];
							return *this;
						}
						inline const_iterator operator++(int) {
							const_iterator r = *this;
							i = i->forward[0];
							return r;
						}
						inline const_iterator &operator--() {
							i = i->backward;
							return *this;
						}
						inline const_iterator operator--(int) {
							const_iterator r = *this;
							i = i->backward;
							return r;
						}
						inline const_iterator operator+(int j) const
						{ const_iterator r = *this; if (j > 0) while (j--) ++r; else while (j++) --r; return r; }
						inline const_iterator operator-(int j) const { return operator+(-j); }
						inline const_iterator &operator+=(int j) { return *this = *this + j; }
						inline const_iterator &operator-=(int j) { return *this = *this - j; }

						// ### Qt 5: not sure this is necessary anymore
#ifdef QT_STRICT_ITERATORS
					private:
						inline bool operator==(const iterator &o) { return operator==(const_iterator(o)); }
						inline bool operator!=(const iterator &o) { return operator!=(const_iterator(o)); }
#endif

					private:
						// ### Qt 5: remove
						inline operator bool() const { return false; }
					};
					friend class const_iterator;

					// STL style
					inline iterator begin() { detach(); return iterator(e->forward[0]); }
					inline iterator beginNoDetach() { return iterator(e->forward[0]); }
					inline const_iterator begin() const { return const_iterator(e->forward[0]); }
					inline const_iterator constBegin() const { return const_iterator(e->forward[0]); }
					inline iterator end() {
						detach();
						return iterator(e);
					}
					inline iterator endNoDetach() { return iterator(e); }
					inline const_iterator end() const { return const_iterator(e); }
					inline const_iterator constEnd() const { return const_iterator(e); }

					iterator erase(iterator it);

					// more Qt
					typedef iterator Iterator;
					typedef const_iterator ConstIterator;
					inline int count() const { return d->size; }
					iterator find(const Key &key);
					const_iterator find(const Key &key) const;
					const_iterator constFind(const Key &key) const;
					iterator lowerBound(const Key &key);
					const_iterator lowerBound(const Key &key) const;
					iterator upperBound(const Key &key);
					const_iterator upperBound(const Key &key) const;
					iterator insert(const Key &key, const T &value);
					iterator tryInsert(const Key &key, const T &value, bool* containsAlready = nullptr);
					iterator tryInsert(const Key &key, const T &value, bool* containsAlready, T** containedValue);
					iterator insertMulti(const Key &key, const T &value);
					CQtManagedRestrictedModificationMap<Key, T> &unite(const CQtManagedRestrictedModificationMap<Key, T> &other);

					// STL compatibility
					typedef Key key_type;
					typedef T mapped_type;
					typedef qptrdiff difference_type;
					typedef int size_type;
					inline bool empty() const { return isEmpty(); }

#ifdef KONCLUDE_CQTMANAGEDRESTRICTEDMODIFICATIONMAP_DEBUG
					inline void dump() const { d->dump(); }
#endif

				private:
					void detach_helper();
					void freeData(CQtManagedRestrictedModificationMapData *d);
					CQtManagedRestrictedModificationMapData::Node *findNode(const Key &key) const;
					CQtManagedRestrictedModificationMapData::Node *mutableFindNode(CQtManagedRestrictedModificationMapData::Node *update[], const Key &key) const;
					CQtManagedRestrictedModificationMapData::Node *node_create(CQtManagedRestrictedModificationMapData *d, CQtManagedRestrictedModificationMapData::Node *update[], const Key &key,
												const T &value);
				};



		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CQtManagedRestrictedModificationMap.cpp"


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_MAP_CONTAINER

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONMAP_H
