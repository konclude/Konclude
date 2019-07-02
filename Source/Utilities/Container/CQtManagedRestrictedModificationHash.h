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
 *		Note, this is a modified version of the QHash from the Qt framework.
 *
 */

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONHASH_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONHASH_H

// Namespace includes
#include "ContainerSettings.h"
#include "CQtManagedRestrictedModificationHashBase.h"
#include "CQtManagedRestrictedModificationHashData.h"
#include "CQtManagedRestrictedModificationList.h"

#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_HASH_CONTAINER

// Library includes
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
			 *		\class		CQtManagedRestrictedModificationHash
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */


			template <class Key, class T>
			struct CQtManagedRestrictedModificationHashNode
			{
				CQtManagedRestrictedModificationHashNode *next;
				uint h;
				Key key;
				T value;

				inline CQtManagedRestrictedModificationHashNode(const Key &key0) : key(key0) {} // ### remove in 5.0
				inline CQtManagedRestrictedModificationHashNode(const Key &key0, const T &value0) : key(key0), value(value0) {}
				inline bool same_key(uint h0, const Key &key0) { return h0 == h && key0 == key; }
			};


			template <class Key, class T>
			struct CQtManagedRestrictedModificationHashDummyNode
			{
				CQtManagedRestrictedModificationHashDummyNode *next;
				uint h;
				Key key;

				inline CQtManagedRestrictedModificationHashDummyNode(const Key &key0) : key(key0) {}
			};

			struct CQtManagedRestrictedModificationHashDummyValue
			{
			};

			inline bool operator==(const CQtManagedRestrictedModificationHashDummyValue & /* v1 */, const CQtManagedRestrictedModificationHashDummyValue & /* v2 */)
			{
				return true;
			}

			template <class Key, class T>
			class CQtManagedRestrictedModificationHash : public CQtManagedRestrictedModificationHashBase
			{
				typedef CQtManagedRestrictedModificationHashDummyNode<Key, T> DummyNode;
				typedef CQtManagedRestrictedModificationHashNode<Key, T> Node;

				union {
					CQtManagedRestrictedModificationHashData *d;
					CQtManagedRestrictedModificationHashNode<Key, T> *e;
				};
				CContext* mContext;
				CMemoryAllocationManager* mMemMan;

				static inline Node *concrete(CQtManagedRestrictedModificationHashData::Node *node) {
					return reinterpret_cast<Node *>(node);
				}

#ifdef Q_ALIGNOF
				static inline int alignOfNode() { return qMax<int>(sizeof(void*), Q_ALIGNOF(Node)); }
				static inline int alignOfDummyNode() { return qMax<int>(sizeof(void*), Q_ALIGNOF(DummyNode)); }
#else
				static inline int alignOfNode() { return 0; }
				static inline int alignOfDummyNode() { return 0; }
#endif

			public:
				inline CQtManagedRestrictedModificationHash() : d(&CQtManagedRestrictedModificationHashData::shared_null),mContext(nullptr) { 
					mMemMan = CContext::getMemoryAllocationManager(mContext);
					/*d->ref.ref();*/ 
				}
				inline CQtManagedRestrictedModificationHash(CContext* context) : d(&CQtManagedRestrictedModificationHashData::shared_null),mContext(context) { 
					mMemMan = CContext::getMemoryAllocationManager(mContext);
					/*d->ref.ref();*/ 
				}
				inline CQtManagedRestrictedModificationHash(const CQtManagedRestrictedModificationHash<Key, T> &other) : d(other.d),mContext(nullptr) {
					mMemMan = CContext::getMemoryAllocationManager(mContext);
					/* d->ref.ref();*/ if (!d->sharable) detach(); 
				}
				inline CQtManagedRestrictedModificationHash(const CQtManagedRestrictedModificationHash<Key, T> &other, CContext* context) : d(other.d),mContext(context) {
					mMemMan = CContext::getMemoryAllocationManager(mContext);
					/* d->ref.ref();*/ if (!d->sharable) detach(); 
				}
				inline ~CQtManagedRestrictedModificationHash() { 
					if (/*!d->ref.deref()*/ d->refHash == this) freeData(d); 
				}


				CQtManagedRestrictedModificationHash<Key, T>* init(CContext* context = nullptr);
				CQtManagedRestrictedModificationHash<Key, T>* init(const CQtManagedRestrictedModificationHash<Key, T> &other, CContext* context = nullptr);
				CQtManagedRestrictedModificationHash<Key, T>* init(CQtManagedRestrictedModificationHash<Key, T> *other, CContext* context = nullptr);


				CQtManagedRestrictedModificationHash<Key, T> &operator=(const CQtManagedRestrictedModificationHash<Key, T> &other);

				bool operator==(const CQtManagedRestrictedModificationHash<Key, T> &other) const;
				inline bool operator!=(const CQtManagedRestrictedModificationHash<Key, T> &other) const { return !(*this == other); }

				inline int size() const { return d->size; }

				inline bool isEmpty() const { return d->size == 0; }

				inline int capacity() const { return d->numBuckets; }
				void reserve(int size);
				inline void squeeze() { reserve(1); }

				inline void detach() { if (/*d->ref != 1*/d->refHash != this) detach_helper(); }
				inline void detachRestrictedCopy(void* assistValue) { if (/*d->ref != 1*/d->refHash != this) detach_helper_RestrictedCopy(assistValue); }
				inline bool isDetached() const { return /*d->ref == 1;*/ d->refHash == this; }
				inline void setSharable(bool sharable) { if (!sharable) detach(); d->sharable = sharable; }

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

				bool tryGetValuePointer(const Key& key, T*& valuePointer) const;
				bool tryGetValue(const Key& key, T* value = nullptr) const;
				T* valuePointer(const Key& key) const;

				QList<Key> uniqueKeys() const;
				QList<Key> keys() const;
				QList<Key> keys(const T &value) const;
				QList<T> values() const;
				QList<T> values(const Key &key) const;

				CQtManagedRestrictedModificationList<Key> uniqueKeysInManagedRestrictedModificationList(CContext* context = nullptr) const;
				CQtManagedRestrictedModificationList<Key> keysInManagedRestrictedModificationList(CContext* context = nullptr) const;
				CQtManagedRestrictedModificationList<Key> keysInManagedRestrictedModificationList(const T &value, CContext* context = nullptr) const;
				CQtManagedRestrictedModificationList<T> valuesInManagedRestrictedModificationList(CContext* context = nullptr) const;
				CQtManagedRestrictedModificationList<T> valuesInManagedRestrictedModificationList(const Key &key, CContext* context = nullptr) const;

				int count(const Key &key) const;

				class const_iterator;

				class iterator
				{
					friend class const_iterator;
					CQtManagedRestrictedModificationHashData::Node *i;

				public:
					typedef std::bidirectional_iterator_tag iterator_category;
					typedef ptrdiff_t difference_type;
					typedef T value_type;
					typedef T *pointer;
					typedef T &reference;

					// ### Qt 5: get rid of 'operator Node *'
					inline operator Node *() const { return concrete(i); }
					inline iterator() : i(0) { }
					explicit inline iterator(void *node) : i(reinterpret_cast<CQtManagedRestrictedModificationHashData::Node *>(node)) { }

					inline const Key &key() const { return concrete(i)->key; }
					inline T &value() const { return concrete(i)->value; }
					inline T &operator*() const { return concrete(i)->value; }
					inline T *operator->() const { return &concrete(i)->value; }
					inline bool operator==(const iterator &o) const { return i == o.i; }
					inline bool operator!=(const iterator &o) const { return i != o.i; }

					inline iterator &operator++() {
						i = CQtManagedRestrictedModificationHashData::nextNode(i);
						return *this;
					}
					inline iterator operator++(int) {
						iterator r = *this;
						i = CQtManagedRestrictedModificationHashData::nextNode(i);
						return r;
					}
					inline iterator &operator--() {
						i = CQtManagedRestrictedModificationHashData::previousNode(i);
						return *this;
					}
					inline iterator operator--(int) {
						iterator r = *this;
						i = CQtManagedRestrictedModificationHashData::previousNode(i);
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
					CQtManagedRestrictedModificationHashData::Node *i;

				public:
					typedef std::bidirectional_iterator_tag iterator_category;
					typedef ptrdiff_t difference_type;
					typedef T value_type;
					typedef const T *pointer;
					typedef const T &reference;

					// ### Qt 5: get rid of 'operator Node *'
					inline operator Node *() const { return concrete(i); }
					inline const_iterator() : i(0) { }
					explicit inline const_iterator(void *node)
						: i(reinterpret_cast<CQtManagedRestrictedModificationHashData::Node *>(node)) { }
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
						i = CQtManagedRestrictedModificationHashData::nextNode(i);
						return *this;
					}
					inline const_iterator operator++(int) {
						const_iterator r = *this;
						i = CQtManagedRestrictedModificationHashData::nextNode(i);
						return r;
					}
					inline const_iterator &operator--() {
						i = CQtManagedRestrictedModificationHashData::previousNode(i);
						return *this;
					}
					inline const_iterator operator--(int) {
						const_iterator r = *this;
						i = CQtManagedRestrictedModificationHashData::previousNode(i);
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
					inline bool operator==(const iterator &o) const { return operator==(const_iterator(o)); }
					inline bool operator!=(const iterator &o) const { return operator!=(const_iterator(o)); }
#endif

				private:
					// ### Qt 5: remove
					inline operator bool() const { return false; }
				};
				friend class const_iterator;

				// STL style
				inline iterator begin() { detach(); return iterator(d->firstNode()); }
				inline const_iterator begin() const { return const_iterator(d->firstNode()); }
				inline const_iterator constBegin() const { return const_iterator(d->firstNode()); }
				inline iterator end() { detach(); return iterator(e); }
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
				iterator insert(const Key &key, const T &value);
				iterator insertMulti(const Key &key, const T &value);
				CQtManagedRestrictedModificationHash<Key, T> &unite(const CQtManagedRestrictedModificationHash<Key, T> &other);

				// STL compatibility
				typedef T mapped_type;
				typedef Key key_type;
				typedef ptrdiff_t difference_type;
				typedef int size_type;

				inline bool empty() const { return isEmpty(); }

#ifdef QT_CQTMANAGEDONEWAYMUTATIONHASH_DEBUG
				inline void dump() const { d->dump(); }
				inline void checkSanity() const { d->checkSanity(); }
#endif

			private:
				void detach_helper();
				void detach_helper_RestrictedCopy(void* assistValue);
				void freeData(CQtManagedRestrictedModificationHashData *d);
				Node **findNode(const Key &key, uint *hp = 0) const;
				Node *createNode(uint h, const Key &key, const T &value, Node **nextNode);
				void deleteNode(Node *node);
				static void deleteNode2(CQtManagedRestrictedModificationHashData::Node *node);

				static void duplicateNode(CQtManagedRestrictedModificationHashData::Node *originalNode, void *newNode);
				static bool copyRestricted_helper(CQtManagedRestrictedModificationHashData::Node *originalNode, void* assistValue);
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CQtManagedRestrictedModificationHash.cpp"


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_HASH_CONTAINER

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONHASH_H
