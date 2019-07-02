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
 
#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONMAP_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONMAP_CPP


#include "CQtManagedRestrictedModificationMap.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_MAP_CONTAINER

			
			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationMap<Key, T> &CQtManagedRestrictedModificationMap<Key, T>::operator=(const CQtManagedRestrictedModificationMap<Key, T> &other)
			{
				if (d != other.d) {
					//other.d->ref.ref();
					//if (!d->ref.deref())
					if (d->refMap == this)
						freeData(d);
					d = other.d;
					if (!d->sharable)
						detach_helper();
				}
				return *this;
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationMap<Key, T>* CQtManagedRestrictedModificationMap<Key, T>::init(const CQtManagedRestrictedModificationMap<Key, T> &other, CContext* context)
			{
				if (d != other.d) {
					//other.d->ref.ref();
					//if (!d->ref.deref())
					if (d->refMap == this)
						freeData(d);
					d = other.d;
					if (!d->sharable)
						detach_helper();
				}
				mContext = context;
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				return this;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationMap<Key, T>* CQtManagedRestrictedModificationMap<Key, T>::init(const CQtManagedRestrictedModificationMap<Key, T>* other, CContext* context)
			{
				if (other) {
					if (d != other->d) {
						//other.d->ref.ref();
						//if (!d->ref.deref())
						if (d->refMap == this)
							freeData(d);
						d = other->d;
						if (!d->sharable)
							detach_helper();
					}
				} else {
					clear();
				}
				mContext = context;
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				return this;
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE bool CQtManagedRestrictedModificationMap<Key, T>::isKeySubSetOf(const CQtManagedRestrictedModificationMap<Key, T>* other, cint64 testDirectSubSetFactorCount) {
				if (count() > other->count()) {
					return false;
				}
				if (count() * testDirectSubSetFactorCount < other->count()) {
					for (const_iterator it = constBegin(),itEnd = constEnd(); it != itEnd; ++it) {
						const Key& key = it.key();
						if (!other->contains(key)) {
							return false;
						}
					}
				} else {
					const_iterator itSub = constBegin(),itSubEnd = constEnd();
					const_iterator itSuper = other->constBegin(),itSuperEnd = other->constEnd();
					while (itSub != itSubEnd) {
						const Key& subKey = itSub.key();
						if (itSuper == itSuperEnd) {
							return false;
						}
						const Key& superKey = itSuper.key();
						if (superKey < subKey) {
							++itSuper;
						} else if (subKey < superKey) {
							return false;
						} else {
							++itSub;
							++itSuper;
						}
					}
				}
				return true;
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationMap<Key, T>* CQtManagedRestrictedModificationMap<Key, T>::init(CContext* context)
			{
				clear();
				mContext = context;
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				return this;
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationMap<Key, T>::clear()
			{
				*this = CQtManagedRestrictedModificationMap<Key, T>(mContext);
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMapData::Node *
			CQtManagedRestrictedModificationMap<Key, T>::node_create(CQtManagedRestrictedModificationMapData *adt, CQtManagedRestrictedModificationMapData::Node *aupdate[], const Key &akey, const T &avalue)
			{
				CQtManagedRestrictedModificationMapData::Node *abstractNode = adt->node_create(aupdate, payload(), alignment(), mMemMan);
				QT_TRY {
					Node *concreteNode = concrete(abstractNode);
					new (&concreteNode->key) Key(akey);
					QT_TRY {
						new (&concreteNode->value) T(avalue);
					} QT_CATCH(...) {
						concreteNode->key.~Key();
						QT_RETHROW;
					}
				} QT_CATCH(...) {
					adt->node_delete(aupdate, payload(), abstractNode, mMemMan);
					QT_RETHROW;
				}

				// clean up the update array for further insertions
				/*
				for (int i = 0; i <= d->topLevel; ++i) {
					if ( aupdate[i]==reinterpret_cast<CQtManagedRestrictedModificationMapData::Node *>(adt) || aupdate[i]->forward[i] != abstractNode)
						break;
					aupdate[i] = abstractNode;
				}
			*/

				return abstractNode;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationMapData::Node *CQtManagedRestrictedModificationMap<Key, T>::findNode(const Key &akey) const
			{
				CQtManagedRestrictedModificationMapData::Node *cur = e;
				CQtManagedRestrictedModificationMapData::Node *next = e;

				for (int i = d->topLevel; i >= 0; i--) {
					while ((next = cur->forward[i]) != e && cMapLessThanKey<Key>(concrete(next)->key, akey))
						cur = next;
				}

				if (next != e && !cMapLessThanKey<Key>(akey, concrete(next)->key)) {
					return next;
				} else {
					return e;
				}
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE const T CQtManagedRestrictedModificationMap<Key, T>::value(const Key &akey) const
			{
				CQtManagedRestrictedModificationMapData::Node *node;
				if (d->size == 0 || (node = findNode(akey)) == e) {
					return T();
				} else {
					return concrete(node)->value;
				}
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE const T CQtManagedRestrictedModificationMap<Key, T>::value(const Key &akey, const T &adefaultValue) const
			{
				CQtManagedRestrictedModificationMapData::Node *node;
				if (d->size == 0 || (node = findNode(akey)) == e) {
					return adefaultValue;
				} else {
					return concrete(node)->value;
				}
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE bool CQtManagedRestrictedModificationMap<Key, T>::tryGetValue(const Key &akey, T* value) const {
				CQtManagedRestrictedModificationMapData::Node *node;
				if (d->size == 0 || (node = findNode(akey)) == e) {
					return false;
				} else {
					*value = concrete(node)->value;
					return true;
				}
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE bool CQtManagedRestrictedModificationMap<Key, T>::tryGetValuePointer(const Key &akey, T*& valuePointer) const {
				CQtManagedRestrictedModificationMapData::Node *node;
				if (d->size == 0 || (node = findNode(akey)) == e) {
					return false;
				} else {
					valuePointer = &(concrete(node)->value);
					return true;
				}
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE const T CQtManagedRestrictedModificationMap<Key, T>::operator[](const Key &akey) const
			{
				return value(akey);
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE T &CQtManagedRestrictedModificationMap<Key, T>::operator[](const Key &akey)
			{
				detach();

				CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				CQtManagedRestrictedModificationMapData::Node *node = mutableFindNode(update, akey);
				if (node == e)
					node = node_create(d, update, akey, T());
				return concrete(node)->value;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE int CQtManagedRestrictedModificationMap<Key, T>::count(const Key &akey) const
			{
				int cnt = 0;
				CQtManagedRestrictedModificationMapData::Node *node = findNode(akey);
				if (node != e) {
					do {
						++cnt;
						node = node->forward[0];
					} while (node != e && !cMapLessThanKey<Key>(akey, concrete(node)->key));
				}
				return cnt;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE bool CQtManagedRestrictedModificationMap<Key, T>::contains(const Key &akey) const
			{
				return findNode(akey) != e;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::iterator CQtManagedRestrictedModificationMap<Key, T>::insert(const Key &akey,
																				   const T &avalue)
			{
				detach();

				CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				CQtManagedRestrictedModificationMapData::Node *node = mutableFindNode(update, akey);
				if (node == e) {
					node = node_create(d, update, akey, avalue);
				} else {
					concrete(node)->value = avalue;
				}
				return iterator(node);
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::iterator CQtManagedRestrictedModificationMap<Key, T>::tryInsert(const Key &akey,
				const T &avalue, bool* containsAlready)
			{
				detach();

				CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				CQtManagedRestrictedModificationMapData::Node *node = mutableFindNode(update, akey);
				if (node == e) {
					node = node_create(d, update, akey, avalue);
					if (containsAlready) {
						*containsAlready = false;
					}
				} else {
					if (containsAlready) {
						*containsAlready = true;
					}
					//concrete(node)->value = avalue;
				}
				return iterator(node);
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::iterator CQtManagedRestrictedModificationMap<Key, T>::tryInsert(const Key &akey,
				const T &avalue, bool* containsAlready, T** containedValue)
			{
				detach();

				CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				CQtManagedRestrictedModificationMapData::Node *node = mutableFindNode(update, akey);
				if (node == e) {
					node = node_create(d, update, akey, avalue);
					if (containsAlready) {
						*containsAlready = false;
					}
				} else {
					if (containsAlready) {
						*containsAlready = true;
					}
					//concrete(node)->value = avalue;
				}
				if (containedValue) {
					*containedValue = &concrete(node)->value;
				}
				return iterator(node);
			}


			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::iterator CQtManagedRestrictedModificationMap<Key, T>::insertMulti(const Key &akey,
																						const T &avalue)
			{
				detach();

				CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				mutableFindNode(update, akey);
				return iterator(node_create(d, update, akey, avalue));
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::const_iterator CQtManagedRestrictedModificationMap<Key, T>::find(const Key &akey) const
			{
				return const_iterator(findNode(akey));
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::const_iterator CQtManagedRestrictedModificationMap<Key, T>::constFind(const Key &akey) const
			{
				return const_iterator(findNode(akey));
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::iterator CQtManagedRestrictedModificationMap<Key, T>::find(const Key &akey)
			{
				detach();
				return iterator(findNode(akey));
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationMap<Key, T> &CQtManagedRestrictedModificationMap<Key, T>::unite(const CQtManagedRestrictedModificationMap<Key, T> &other)
			{
				CQtManagedRestrictedModificationMap<Key, T> copy(other);
				const_iterator it = copy.constEnd();
				const const_iterator b = copy.constBegin();
				while (it != b) {
					--it;
					insertMulti(it.key(), it.value());
				}
				return *this;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE void CQtManagedRestrictedModificationMap<Key, T>::freeData(CQtManagedRestrictedModificationMapData *x)
			{
				if (QTypeInfo<Key>::isComplex || QTypeInfo<T>::isComplex) {
					CQtManagedRestrictedModificationMapData *cur = x;
					CQtManagedRestrictedModificationMapData *next = cur->forward[0];
					while (next != x) {
						cur = next;
						next = cur->forward[0];
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#pragma warning(disable:4189)
#endif
						Node *concreteNode = concrete(reinterpret_cast<CQtManagedRestrictedModificationMapData::Node *>(cur));
						concreteNode->key.~Key();
						concreteNode->value.~T();
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#pragma warning(default:4189)
#endif
					}
				}
				x->continueFreeData(payload(), mMemMan);
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE int CQtManagedRestrictedModificationMap<Key, T>::remove(const Key &akey)
			{
				detach();

				CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				CQtManagedRestrictedModificationMapData::Node *cur = e;
				CQtManagedRestrictedModificationMapData::Node *next = e;
				int oldSize = d->size;

				for (int i = d->topLevel; i >= 0; i--) {
					while ((next = cur->forward[i]) != e && cMapLessThanKey<Key>(concrete(next)->key, akey))
						cur = next;
					update[i] = cur;
				}

				if (next != e && !cMapLessThanKey<Key>(akey, concrete(next)->key)) {
					bool deleteNext = true;
					do {
						cur = next;
						next = cur->forward[0];
						deleteNext = (next != e && !cMapLessThanKey<Key>(concrete(cur)->key, concrete(next)->key));
						concrete(cur)->key.~Key();
						concrete(cur)->value.~T();
						d->node_delete(update, payload(), cur, mMemMan);
					} while (deleteNext);
				}
				return oldSize - d->size;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE T CQtManagedRestrictedModificationMap<Key, T>::take(const Key &akey)
			{
				detach();

				CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				CQtManagedRestrictedModificationMapData::Node *cur = e;
				CQtManagedRestrictedModificationMapData::Node *next = e;

				for (int i = d->topLevel; i >= 0; i--) {
					while ((next = cur->forward[i]) != e && cMapLessThanKey<Key>(concrete(next)->key, akey))
						cur = next;
					update[i] = cur;
				}

				if (next != e && !cMapLessThanKey<Key>(akey, concrete(next)->key)) {
					T t = concrete(next)->value;
					concrete(next)->key.~Key();
					concrete(next)->value.~T();
					d->node_delete(update, payload(), next, mMemMan);
					return t;
				}
				return T();
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::iterator CQtManagedRestrictedModificationMap<Key, T>::erase(iterator it)
			{
				CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				CQtManagedRestrictedModificationMapData::Node *cur = e;
				CQtManagedRestrictedModificationMapData::Node *next = e;

				if (it == iterator(e))
					return it;

				for (int i = d->topLevel; i >= 0; i--) {
					while ((next = cur->forward[i]) != e && cMapLessThanKey<Key>(concrete(next)->key, it.key()))
						cur = next;
					update[i] = cur;
				}

				while (next != e) {
					cur = next;
					next = cur->forward[0];
					if (cur == it) {
						concrete(cur)->key.~Key();
						concrete(cur)->value.~T();
						d->node_delete(update, payload(), cur, mMemMan);
						return iterator(next);
					}

					for (int i = 0; i <= d->topLevel; ++i) {
						if (update[i]->forward[i] != cur)
							break;
						update[i] = cur;
					}
				}
				return end();
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE void CQtManagedRestrictedModificationMap<Key, T>::detach_helper()
			{
				union { CQtManagedRestrictedModificationMapData *d; CQtManagedRestrictedModificationMapData::Node *e; } x;
				x.d = CQtManagedRestrictedModificationMapData::createData(alignment(), this, mMemMan);
				if (d->size) {
					x.d->insertInOrder = true;
					CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
					CQtManagedRestrictedModificationMapData::Node *cur = e->forward[0];
					update[0] = x.e;
					while (cur != e) {
						QT_TRY {
							Node *concreteNode = concrete(cur);
							node_create(x.d, update, concreteNode->key, concreteNode->value);
						} QT_CATCH(...) {
							freeData(x.d);
							QT_RETHROW;
						}
						cur = cur->forward[0];
					}
					x.d->insertInOrder = false;
				}
				if (/*!d->ref.deref()*/ d->refMap == this)
					freeData(d);
				d = x.d;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationMapData::Node *CQtManagedRestrictedModificationMap<Key, T>::mutableFindNode(CQtManagedRestrictedModificationMapData::Node *aupdate[],
																			   const Key &akey) const
			{
				CQtManagedRestrictedModificationMapData::Node *cur = e;
				CQtManagedRestrictedModificationMapData::Node *next = e;

				for (int i = d->topLevel; i >= 0; i--) {
					while ((next = cur->forward[i]) != e && cMapLessThanKey<Key>(concrete(next)->key, akey))
						cur = next;
					aupdate[i] = cur;
				}
				if (next != e && !cMapLessThanKey<Key>(akey, concrete(next)->key)) {
					return next;
				} else {
					return e;
				}
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE QList<Key> CQtManagedRestrictedModificationMap<Key, T>::uniqueKeys() const
			{
				QList<Key> res;
				res.reserve(size()); // May be too much, but assume short lifetime
				const_iterator i = begin();
				if (i != end()) {
					for (;;) {
						const Key &aKey = i.key();
						res.append(aKey);
						do {
							if (++i == end())
								goto break_out_of_outer_loop;
						} while (!(aKey < i.key()));   // loop while (key == i.key())
					}
				}
			break_out_of_outer_loop:
				return res;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE QList<Key> CQtManagedRestrictedModificationMap<Key, T>::keys() const
			{
				QList<Key> res;
				res.reserve(size());
				const_iterator i = begin();
				while (i != end()) {
					res.append(i.key());
					++i;
				}
				return res;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE QList<Key> CQtManagedRestrictedModificationMap<Key, T>::keys(const T &avalue) const
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
			Q_OUTOFLINE_TEMPLATE const Key CQtManagedRestrictedModificationMap<Key, T>::key(const T &avalue) const
			{
				return key(avalue, Key());
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE const Key CQtManagedRestrictedModificationMap<Key, T>::key(const T &avalue, const Key &defaultKey) const
			{
				const_iterator i = begin();
				while (i != end()) {
					if (i.value() == avalue)
						return i.key();
					++i;
				}

				return defaultKey;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE QList<T> CQtManagedRestrictedModificationMap<Key, T>::values() const
			{
				QList<T> res;
				res.reserve(size());
				const_iterator i = begin();
				while (i != end()) {
					res.append(i.value());
					++i;
				}
				return res;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE QList<T> CQtManagedRestrictedModificationMap<Key, T>::values(const Key &akey) const
			{
				QList<T> res;
				CQtManagedRestrictedModificationMapData::Node *node = findNode(akey);
				if (node != e) {
					do {
						res.append(concrete(node)->value);
						node = node->forward[0];
					} while (node != e && !cMapLessThanKey<Key>(akey, concrete(node)->key));
				}
				return res;
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::const_iterator
			CQtManagedRestrictedModificationMap<Key, T>::lowerBound(const Key &akey) const
			{
				CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				mutableFindNode(update, akey);
				return const_iterator(update[0]->forward[0]);
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::iterator CQtManagedRestrictedModificationMap<Key, T>::lowerBound(const Key &akey)
			{
				detach();
				return static_cast<CQtManagedRestrictedModificationMapData::Node *>(const_cast<const CQtManagedRestrictedModificationMap *>(this)->lowerBound(akey));
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::const_iterator
			CQtManagedRestrictedModificationMap<Key, T>::upperBound(const Key &akey) const
			{
				CQtManagedRestrictedModificationMapData::Node *update[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				mutableFindNode(update, akey);
				CQtManagedRestrictedModificationMapData::Node *node = update[0]->forward[0];
				while (node != e && !cMapLessThanKey<Key>(akey, concrete(node)->key))
					node = node->forward[0];
				return const_iterator(node);
			}

			template <class Key, class T>
			Q_INLINE_TEMPLATE typename CQtManagedRestrictedModificationMap<Key, T>::iterator CQtManagedRestrictedModificationMap<Key, T>::upperBound(const Key &akey)
			{
				detach();
				return static_cast<CQtManagedRestrictedModificationMapData::Node *>(const_cast<const CQtManagedRestrictedModificationMap *>(this)->upperBound(akey));
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE bool CQtManagedRestrictedModificationMap<Key, T>::operator==(const CQtManagedRestrictedModificationMap<Key, T> &other) const
			{
				if (size() != other.size())
					return false;
				if (d == other.d)
					return true;

				const_iterator it1 = begin();
				const_iterator it2 = other.begin();

				while (it1 != end()) {
					if (!(it1.value() == it2.value()) || cMapLessThanKey(it1.key(), it2.key()) || cMapLessThanKey(it2.key(), it1.key()))
						return false;
					++it2;
					++it1;
				}
				return true;
			}

			#ifndef QT_NO_STL
			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationMap<Key, T>::CQtManagedRestrictedModificationMap(const std::map<Key, T> &other)
			{
				d = CQtManagedRestrictedModificationMapData::createData(alignment(), this, mMemMan);
				d->insertInOrder = true;
				typename std::map<Key,T>::const_iterator it = other.end();
				while (it != other.begin()) {
					--it;
					insert((*it).first, (*it).second);
				}
				d->insertInOrder = false;
			}

			template <class Key, class T>
			Q_OUTOFLINE_TEMPLATE std::map<Key, T> CQtManagedRestrictedModificationMap<Key, T>::toStdMap() const
			{
				std::map<Key, T> map;
				const_iterator it = end();
				while (it != begin()) {
					--it;
					map.insert(std::pair<Key, T>(it.key(), it.value()));
				}
				return map;
			}

			#endif // QT_NO_STL


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_MAP_CONTAINER


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONMAP_CPP

