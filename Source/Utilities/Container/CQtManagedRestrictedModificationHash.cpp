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
#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONHASH_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONHASH_CPP


#include "CQtManagedRestrictedModificationHash.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_HASH_CONTAINER


			template <class Key, class T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationHash<Key, T>::deleteNode(Node *node)
			{
				deleteNode2(reinterpret_cast<CQtManagedRestrictedModificationHashData::Node*>(node));
				d->freeNode(node,mMemMan);
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationHash<Key, T>::deleteNode2(CQtManagedRestrictedModificationHashData::Node *node)
			{
#ifdef Q_CC_BOR
				concrete(node)->~CQtManagedRestrictedModificationHashNode<Key, T>();
#elif defined(QT_NO_PARTIAL_TEMPLATE_SPECIALIZATION)
				concrete(node)->~CQtManagedRestrictedModificationHashNode();
#else
				concrete(node)->~Node();
#endif
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationHash<Key, T>::duplicateNode(CQtManagedRestrictedModificationHashData::Node *node, void *newNode)
			{
				Node *concreteNode = concrete(node);
				if (QTypeInfo<T>::isDummy) {
					(void) new (newNode) DummyNode(concreteNode->key);
				} else {
					(void) new (newNode) Node(concreteNode->key, concreteNode->value);
				}
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationHash<Key, T>::Node *
				CQtManagedRestrictedModificationHash<Key, T>::createNode(uint ah, const Key &akey, const T &avalue, Node **anextNode)
			{
				Node *node;

				if (QTypeInfo<T>::isDummy) {
					node = reinterpret_cast<Node *>(new (d->allocateNode(alignOfDummyNode(),mMemMan)) DummyNode(akey));
				} else {
					node = new (d->allocateNode(alignOfNode(),mMemMan)) Node(akey, avalue);
				}

				node->h = ah;
				node->next = *anextNode;
				*anextNode = node;
				++d->size;
				return node;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationHash<Key, T> &CQtManagedRestrictedModificationHash<Key, T>::unite(const CQtManagedRestrictedModificationHash<Key, T> &other)
			{
				CQtManagedRestrictedModificationHash<Key, T> copy(other);
				const_iterator it = copy.constEnd();
				while (it != copy.constBegin()) {
					--it;
					insertMulti(it.key(), it.value());
				}
				return *this;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE void CQtManagedRestrictedModificationHash<Key, T>::freeData(CQtManagedRestrictedModificationHashData *x)
			{
				x->free_helper(deleteNode2,mMemMan);
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationHash<Key, T>::clear()
			{
				*this = CQtManagedRestrictedModificationHash<Key,T>(mContext);
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE void CQtManagedRestrictedModificationHash<Key, T>::detach_helper()
			{
				CQtManagedRestrictedModificationHashData *x = d->detach_helper2(duplicateNode, deleteNode2,
					QTypeInfo<T>::isDummy ? sizeof(DummyNode) : sizeof(Node),
					QTypeInfo<T>::isDummy ? alignOfDummyNode() : alignOfNode(),this,mMemMan);
				//if (!d->ref.deref())
				if (d->refHash == this)
					freeData(d);
				d = x;
			}



			template <class Key, class T>
			Q_INLINE_TEMPLATE bool CQtManagedRestrictedModificationHash<Key, T>::copyRestricted_helper(CQtManagedRestrictedModificationHashData::Node *node, void* assistValue)
			{
				Node *concreteNode = concrete(node);
				return concreteNode->value.restrictedCopy(assistValue);
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationHash<Key, T>::detach_helper_RestrictedCopy(void* assistValue) {
				CQtManagedRestrictedModificationHashData *x = d->restricted_detach_helper2(copyRestricted_helper,assistValue,duplicateNode, deleteNode2,
					QTypeInfo<T>::isDummy ? sizeof(DummyNode) : sizeof(Node),
					QTypeInfo<T>::isDummy ? alignOfDummyNode() : alignOfNode(),this,mMemMan);
				//if (!d->ref.deref())
				if (d->refHash == this)
					freeData(d);
				d = x;
			}



			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationHash<Key, T> &CQtManagedRestrictedModificationHash<Key, T>::operator=(const CQtManagedRestrictedModificationHash<Key, T> &other)
			{
				if (d != other.d) {
					//other.d->ref.ref();
					//if (!d->ref.deref())
					if (d->refHash == this)
						freeData(d);
					d = other.d;
					if (!d->sharable)
						detach_helper();
				}
				return *this;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationHash<Key, T>* CQtManagedRestrictedModificationHash<Key, T>::init(const CQtManagedRestrictedModificationHash<Key, T>& other, CContext* context)
			{
				if (d != other.d) {
					if (d->refHash == this)
						freeData(d);
					mContext = context;
					mMemMan = CContext::getMemoryAllocationManager(mContext);
					d = other.d;
					if (!d->sharable)
						detach_helper();
				}
				mContext = context;
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				return this;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationHash<Key, T>* CQtManagedRestrictedModificationHash<Key, T>::init(CContext* context)
			{
				if (d->refHash == this)
					freeData(d);
				mContext = context;					
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				d = &CQtManagedRestrictedModificationHashData::shared_null;
				return this;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationHash<Key, T>* CQtManagedRestrictedModificationHash<Key, T>::init(CQtManagedRestrictedModificationHash<Key, T>* other, CContext* context)
			{
				if (other) {
					if (d != other->d) {
						if (d->refHash ==  this)
							freeData(d);
						mContext = context;
						mMemMan = CContext::getMemoryAllocationManager(mContext);
						if (other) {
							d = other->d;
							if (!d->sharable)
								detach_helper();
						}
					}
				} else {
					if (d->refHash == this)
						freeData(d);
					d = &CQtManagedRestrictedModificationHashData::shared_null;
				}
				mContext = context;
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				return this;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE const T CQtManagedRestrictedModificationHash<Key, T>::value(const Key &akey) const
			{
				Node *node;
				if (d->size == 0 || (node = *findNode(akey)) == e) {
					return T();
				} else {
					return node->value;
				}
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE T* CQtManagedRestrictedModificationHash<Key, T>::valuePointer(const Key& akey) const {
				Node *node;
				if (d->size == 0 || (node = *findNode(akey)) == e) {
					return nullptr;
				} else {
					return &node->value;
				}
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE const T CQtManagedRestrictedModificationHash<Key, T>::value(const Key &akey, const T &adefaultValue) const
			{
				Node *node;
				if (d->size == 0 || (node = *findNode(akey)) == e) {
					return adefaultValue;
				} else {
					return node->value;
				}
			}


			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE bool CQtManagedRestrictedModificationHash<Key, T>::tryGetValuePointer(const Key& akey, T*& valuePointer) const {
				Node *node;
				if (d->size == 0 || (node = *findNode(akey)) == e) {
					return false;
				} else {
					valuePointer = &node->value;
					return true;
				}
			}


			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE bool CQtManagedRestrictedModificationHash<Key, T>::tryGetValue(const Key& akey, T* value) const {
				Node *node;
				if (d->size == 0 || (node = *findNode(akey)) == e) {
					return false;
				} else {
					*value = node->value;
					return true;
				}
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE QList<Key> CQtManagedRestrictedModificationHash<Key, T>::uniqueKeys() const
			{
				QList<Key> res;
				const_iterator i = begin();
				if (i != end()) {
					for (;;) {
						const Key &aKey = i.key();
						res.append(aKey);
						do {
							if (++i == end())
								goto break_out_of_outer_loop;
						} while (aKey == i.key());
					}
				}
break_out_of_outer_loop:
				return res;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE QList<Key> CQtManagedRestrictedModificationHash<Key, T>::keys() const
			{
				QList<Key> res;
				const_iterator i = begin();
				while (i != end()) {
					res.append(i.key());
					++i;
				}
				return res;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE QList<Key> CQtManagedRestrictedModificationHash<Key, T>::keys(const T &avalue) const
			{
				QList<Key> res;
				const_iterator i = begin();
				while (i != end()) {
					if (i.value() == avalue)
						res.append(i.key());
					++i;
				}
				return res;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE const Key CQtManagedRestrictedModificationHash<Key, T>::key(const T &avalue) const
			{
				return key(avalue, Key());
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE const Key CQtManagedRestrictedModificationHash<Key, T>::key(const T &avalue, const Key &defaultValue) const
			{
				const_iterator i = begin();
				while (i != end()) {
					if (i.value() == avalue)
						return i.key();
					++i;
				}

				return defaultValue;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE QList<T> CQtManagedRestrictedModificationHash<Key, T>::values() const
			{
				QList<T> res;
				const_iterator i = begin();
				while (i != end()) {
					res.append(i.value());
					++i;
				}
				return res;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE QList<T> CQtManagedRestrictedModificationHash<Key, T>::values(const Key &akey) const
			{
				QList<T> res;
				Node *node = *findNode(akey);
				if (node != e) {
					do {
						res.append(node->value);
					} while ((node = node->next) != e && node->key == akey);
				}
				return res;
			}



			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationList<Key> CQtManagedRestrictedModificationHash<Key, T>::uniqueKeysInManagedRestrictedModificationList(CContext* context) const
			{
				CQtManagedRestrictedModificationList<Key> res(context);
				const_iterator i = begin();
				if (i != end()) {
					for (;;) {
						const Key &aKey = i.key();
						res.append(aKey);
						do {
							if (++i == end())
								goto break_out_of_outer_loop;
						} while (aKey == i.key());
					}
				}
break_out_of_outer_loop:
				return res;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationList<Key> CQtManagedRestrictedModificationHash<Key, T>::keysInManagedRestrictedModificationList(CContext* context) const
			{
				CQtManagedRestrictedModificationList<Key> res(context);
				const_iterator i = begin();
				while (i != end()) {
					res.append(i.key());
					++i;
				}
				return res;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationList<Key> CQtManagedRestrictedModificationHash<Key, T>::keysInManagedRestrictedModificationList(const T &avalue, CContext* context) const
			{
				CQtManagedRestrictedModificationList<Key> res(context);
				const_iterator i = begin();
				while (i != end()) {
					if (i.value() == avalue)
						res.append(i.key());
					++i;
				}
				return res;
			}


			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationList<T> CQtManagedRestrictedModificationHash<Key, T>::valuesInManagedRestrictedModificationList(CContext* context) const
			{
				CQtManagedRestrictedModificationList<T> res(context);
				const_iterator i = begin();
				while (i != end()) {
					res.append(i.value());
					++i;
				}
				return res;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationList<T> CQtManagedRestrictedModificationHash<Key, T>::valuesInManagedRestrictedModificationList(const Key &akey, CContext* context) const
			{
				CQtManagedRestrictedModificationList<T> res(context);
				Node *node = *findNode(akey);
				if (node != e) {
					do {
						res.append(node->value);
					} while ((node = node->next) != e && node->key == akey);
				}
				return res;
			}


			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE int CQtManagedRestrictedModificationHash<Key, T>::count(const Key &akey) const
			{
				int cnt = 0;
				Node *node = *findNode(akey);
				if (node != e) {
					do {
						++cnt;
					} while ((node = node->next) != e && node->key == akey);
				}
				return cnt;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE const T CQtManagedRestrictedModificationHash<Key, T>::operator[](const Key &akey) const
			{
				return value(akey);
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE T &CQtManagedRestrictedModificationHash<Key, T>::operator[](const Key &akey)
			{
				detach();

				uint h;
				Node **node = findNode(akey, &h);
				if (*node == e) {
					if (d->willGrow(mMemMan))
						node = findNode(akey, &h);
					return createNode(h, akey, T(), node)->value;
				}
				return (*node)->value;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationHash<Key, T>::iterator CQtManagedRestrictedModificationHash<Key, T>::insert(const Key &akey,
				const T &avalue)
			{
				detach();

				uint h;
				Node **node = findNode(akey, &h);
				if (*node == e) {
					if (d->willGrow(mMemMan))
						node = findNode(akey, &h);
					return iterator(createNode(h, akey, avalue, node));
				}

				if (!QTypeInfo<T>::isDummy)
					(*node)->value = avalue;
				return iterator(*node);
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationHash<Key, T>::iterator CQtManagedRestrictedModificationHash<Key, T>::insertMulti(const Key &akey,
				const T &avalue)
			{
				detach();
				d->willGrow(mMemMan);

				uint h;
				Node **nextNode = findNode(akey, &h);
				return iterator(createNode(h, akey, avalue, nextNode));
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE int CQtManagedRestrictedModificationHash<Key, T>::remove(const Key &akey)
			{
				if (isEmpty()) // prevents detaching shared null
					return 0;
				detach();

				int oldSize = d->size;
				Node **node = findNode(akey);
				if (*node != e) {
					bool deleteNext = true;
					do {
						Node *next = (*node)->next;
						deleteNext = (next != e && next->key == (*node)->key);
						deleteNode(*node);
						*node = next;
						--d->size;
					} while (deleteNext);
					d->hasShrunk(mMemMan);
				}
				return oldSize - d->size;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE T CQtManagedRestrictedModificationHash<Key, T>::take(const Key &akey)
			{
				if (isEmpty()) // prevents detaching shared null
					return T();
				detach();

				Node **node = findNode(akey);
				if (*node != e) {
					T t = (*node)->value;
					Node *next = (*node)->next;
					deleteNode(*node);
					*node = next;
					--d->size;
					d->hasShrunk(mMemMan);
					return t;
				}
				return T();
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE typename CQtManagedRestrictedModificationHash<Key, T>::iterator CQtManagedRestrictedModificationHash<Key, T>::erase(iterator it)
			{
				if (it == iterator(e))
					return it;

				iterator ret = it;
				++ret;

				Node *node = it;
				Node **node_ptr = reinterpret_cast<Node **>(&d->buckets[node->h % d->numBuckets]);
				while (*node_ptr != node)
					node_ptr = &(*node_ptr)->next;
				*node_ptr = node->next;
				deleteNode(node);
				--d->size;
				return ret;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationHash<Key, T>::reserve(int asize)
			{
				detach();
				d->rehash(-qMax(asize, 1),mMemMan);
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator CQtManagedRestrictedModificationHash<Key, T>::find(const Key &akey) const
			{
				return const_iterator(*findNode(akey));
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator CQtManagedRestrictedModificationHash<Key, T>::constFind(const Key &akey) const
			{
				return const_iterator(*findNode(akey));
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationHash<Key, T>::iterator CQtManagedRestrictedModificationHash<Key, T>::find(const Key &akey)
			{
				detach();
				return iterator(*findNode(akey));
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE bool CQtManagedRestrictedModificationHash<Key, T>::contains(const Key &akey) const
			{
				return *findNode(akey) != e;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE typename CQtManagedRestrictedModificationHash<Key, T>::Node **CQtManagedRestrictedModificationHash<Key, T>::findNode(const Key &akey,
				uint *ahp) const
			{
				Node **node;
				uint h = qHash(akey);

				if (d->numBuckets) {
					node = reinterpret_cast<Node **>(&d->buckets[h % d->numBuckets]);
					Q_ASSERT(*node == e || (*node)->next);
					while (*node != e && !(*node)->same_key(h, akey))
						node = &(*node)->next;
				} else {
					node = const_cast<Node **>(reinterpret_cast<const Node * const *>(&e));
				}
				if (ahp)
					*ahp = h;
				return node;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE bool CQtManagedRestrictedModificationHash<Key, T>::operator==(const CQtManagedRestrictedModificationHash<Key, T> &other) const
			{
				if (size() != other.size())
					return false;
				if (d == other.d)
					return true;

				const_iterator it = begin();

				while (it != end()) {
					const Key &akey = it.key();

					const_iterator it2 = other.find(akey);
					do {
						if (it2 == other.end() || !(it2.key() == akey))
							return false;
						if (!QTypeInfo<T>::isDummy && !(it.value() == it2.value()))
							return false;
						++it;
						++it2;
					} while (it != end() && it.key() == akey);
				}
				return true;
			}


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_HASH_CONTAINER


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONHASH_CPP

