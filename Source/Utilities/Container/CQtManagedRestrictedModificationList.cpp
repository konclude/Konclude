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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONLIST_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONLIST_CPP


#include "CQtManagedRestrictedModificationList.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_LIST_CONTAINER


#if defined(Q_CC_BOR)
			template <typename T>
			Q_INLINE_TEMPLATE T &CQtManagedRestrictedModificationList<T>::Node::t()
			{ return QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic ? *(T*)v:*(T*)this; }
#endif

			template <typename T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationList<T>::node_construct(Node *n, const T &t)
			{
				//if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) n->v = new T(t);
				//else if (QTypeInfo<T>::isComplex) new (n) T(t);
				if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) {
					n->v = CObjectAllocator<T>::allocate(mMemMan);
					new (n->v) T(t);
				} else if (QTypeInfo<T>::isComplex) new (n) T(t);

#if (defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__IBMCPP__)) && !defined(__OPTIMIZE__)
				// This violates pointer aliasing rules, but it is known to be safe (and silent)
				// in unoptimized GCC builds (-fno-strict-aliasing). The other compilers which
				// set the same define are assumed to be safe.
				else *reinterpret_cast<T*>(n) = t;
#else
				// This is always safe, but penaltizes unoptimized builds a lot.
				else ::memcpy(n, &t, sizeof(T));
#endif
			}

			template <typename T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationList<T>::node_destruct(Node *n)
			{
				if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) {
					//delete reinterpret_cast<T*>(n->v);
					T* ob = reinterpret_cast<T*>(n->v);
					CObjectAllocator<T>::destroyAndRelease(ob,mMemMan);
				} else if (QTypeInfo<T>::isComplex) reinterpret_cast<T*>(n)->~T();
			}

			template <typename T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationList<T>::node_copy(Node *from, Node *to, Node *src)
			{
				Node *current = from;
				if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) {
					QT_TRY {
						while(current != to) {
							//current->v = new T(*reinterpret_cast<T*>(src->v));
							current->v = CObjectAllocator<T>::allocate(mMemMan);
							new (current->v) T(*reinterpret_cast<T*>(src->v));
							++current;
							++src;
						}
					} QT_CATCH(...) {
						while (current-- != from) {
							//delete reinterpret_cast<T*>(current->v);
							T* ob = reinterpret_cast<T*>(current->v);
							CObjectAllocator<T>::destroyAndRelease(ob,mMemMan);
						}

						QT_RETHROW;
					}

				} else if (QTypeInfo<T>::isComplex) {
					QT_TRY {
						while(current != to) {
							new (current) T(*reinterpret_cast<T*>(src));
							++current;
							++src;
						}
					} QT_CATCH(...) {
						while (current-- != from)
							(reinterpret_cast<T*>(current))->~T();
						QT_RETHROW;
					}
				} else {
					if (src != from && to - from > 0)
						memcpy(from, src, (to - from) * sizeof(Node *));
				}
			}

			template <typename T>
			Q_INLINE_TEMPLATE void CQtManagedRestrictedModificationList<T>::node_destruct(Node *from, Node *to)
			{
				if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic)
					while(from != to) {
						--to;
						//delete reinterpret_cast<T*>(to->v);
						T* ob = reinterpret_cast<T*>(to->v);
						CObjectAllocator<T>::destroyAndRelease(ob,mMemMan);
					}
				else if (QTypeInfo<T>::isComplex)
					while (from != to) --to, reinterpret_cast<T*>(to)->~T();
			}

			template <typename T>
			Q_INLINE_TEMPLATE CQtManagedRestrictedModificationList<T> &CQtManagedRestrictedModificationList<T>::operator=(const CQtManagedRestrictedModificationList<T> &l)
			{
				if (d != l.d) {
					//l.d->ref.ref();
					//if (!d->ref.deref())
					//	free(d);
					//d = l.d;
					//if (!d->sharable)
					//	detach_helper();

					if (d->refList == this)
						free(d);
					d = l.d;
					if (!d->sharable)
						detach_helper();
				}
				return *this;
			}


			template <typename T>
			CQtManagedRestrictedModificationList<T>* CQtManagedRestrictedModificationList<T>::init(CContext* context) {
				clear();
				mContext = context;
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				return this;
			}

			template <typename T>
			CQtManagedRestrictedModificationList<T>* CQtManagedRestrictedModificationList<T>::init(const CQtManagedRestrictedModificationList<T> &other, CContext* context) {
				if (d != other.d) {
					if (d->refList == this)
						free(d);
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

			template <typename T>
			CQtManagedRestrictedModificationList<T>* CQtManagedRestrictedModificationList<T>::init(const CQtManagedRestrictedModificationList<T> *other, CContext* context) {
				if (!other) {
					if (d != other->d) {
						if (d->refList == this)
							free(d);
						mContext = context;
						mMemMan = CContext::getMemoryAllocationManager(mContext);
						if (other) {
							d = other->d;
							if (!d->sharable)
								detach_helper();
						}
					}
				} else {
					if (d->refList == this)
						free(d);
					d = &CQtManagedRestrictedModificationListData::shared_null;
				}
				mContext = context;
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				return this;
			}



			template <typename T>
			inline typename CQtManagedRestrictedModificationList<T>::iterator CQtManagedRestrictedModificationList<T>::insert(iterator before, const T &t)
			{
				int iBefore = int(before.i - reinterpret_cast<Node *>(p.begin()));
				Node *n = reinterpret_cast<Node *>(p.insert(iBefore,mMemMan));
				QT_TRY {
					node_construct(n, t);
				} QT_CATCH(...) {
					p.remove(iBefore);
					QT_RETHROW;
				}
				return n;
			}
			template <typename T>
			inline typename CQtManagedRestrictedModificationList<T>::iterator CQtManagedRestrictedModificationList<T>::erase(iterator it)
			{ node_destruct(it.i);
			return reinterpret_cast<Node *>(p.erase(reinterpret_cast<void**>(it.i))); }
			template <typename T>
			inline const T &CQtManagedRestrictedModificationList<T>::at(int i) const
			{ Q_ASSERT_X(i >= 0 && i < p.size(), "CQtManagedRestrictedModificationList<T>::at", "index out of range");
			return reinterpret_cast<Node *>(p.at(i))->t(); }
			template <typename T>
			inline const T &CQtManagedRestrictedModificationList<T>::operator[](int i) const
			{ Q_ASSERT_X(i >= 0 && i < p.size(), "CQtManagedRestrictedModificationList<T>::operator[]", "index out of range");
			return reinterpret_cast<Node *>(p.at(i))->t(); }
			template <typename T>
			inline T &CQtManagedRestrictedModificationList<T>::operator[](int i)
			{ Q_ASSERT_X(i >= 0 && i < p.size(), "CQtManagedRestrictedModificationList<T>::operator[]", "index out of range");
			detach(); return reinterpret_cast<Node *>(p.at(i))->t(); }
			template <typename T>
			inline void CQtManagedRestrictedModificationList<T>::removeAt(int i)
			{ if(i >= 0 && i < p.size()) { detach();
			node_destruct(reinterpret_cast<Node *>(p.at(i))); p.remove(i); } }
			template <typename T>
			inline T CQtManagedRestrictedModificationList<T>::takeAt(int i)
			{ Q_ASSERT_X(i >= 0 && i < p.size(), "CQtManagedRestrictedModificationList<T>::take", "index out of range");
			detach(); Node *n = reinterpret_cast<Node *>(p.at(i)); T t = n->t(); node_destruct(n);
			p.remove(i); return t; }
			template <typename T>
			inline T CQtManagedRestrictedModificationList<T>::takeFirst()
			{ T t = first(); removeFirst(); return t; }
			template <typename T>
			inline T CQtManagedRestrictedModificationList<T>::takeLast()
			{ T t = last(); removeLast(); return t; }

			template <typename T>
			Q_OUTOFLINE_TEMPLATE void CQtManagedRestrictedModificationList<T>::reserve(int alloc)
			{
				if (d->alloc < alloc) {
					//if (d->ref != 1)
					if (d->refList != this)
						detach_helper(alloc);
					else
						p.realloc(alloc,mMemMan);
				}
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE void CQtManagedRestrictedModificationList<T>::append(const T &t)
			{
				//if (d->ref != 1) {
				if (d->refList != this) {
					Node *n = detach_helper_grow(INT_MAX, 1);
					QT_TRY {
						node_construct(n, t);
					} QT_CATCH(...) {
						--d->end;
						QT_RETHROW;
					}
				} else {
					if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) {
						Node *n = reinterpret_cast<Node *>(p.append(mMemMan));
						QT_TRY {
							node_construct(n, t);
						} QT_CATCH(...) {
							--d->end;
							QT_RETHROW;
						}
					} else {
						Node *n, copy;
						node_construct(&copy, t); // t might be a reference to an object in the array
						QT_TRY {
							n = reinterpret_cast<Node *>(p.append(mMemMan));;
						} QT_CATCH(...) {
							node_destruct(&copy);
							QT_RETHROW;
						}
						*n = copy;
					}
				}
			}

			template <typename T>
			inline void CQtManagedRestrictedModificationList<T>::prepend(const T &t)
			{
				//if (d->ref != 1) {
				if (d->refList != this) {
					Node *n = detach_helper_grow(0, 1);
					QT_TRY {
						node_construct(n, t);
					} QT_CATCH(...) {
						++d->begin;
						QT_RETHROW;
					}
				} else {
					if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) {
						Node *n = reinterpret_cast<Node *>(p.prepend(mMemMan));
						QT_TRY {
							node_construct(n, t);
						} QT_CATCH(...) {
							++d->begin;
							QT_RETHROW;
						}
					} else {
						Node *n, copy;
						node_construct(&copy, t); // t might be a reference to an object in the array
						QT_TRY {
							n = reinterpret_cast<Node *>(p.prepend(mMemMan));;
						} QT_CATCH(...) {
							node_destruct(&copy);
							QT_RETHROW;
						}
						*n = copy;
					}
				}
			}

			template <typename T>
			inline void CQtManagedRestrictedModificationList<T>::insert(int i, const T &t)
			{
				//if (d->ref != 1) {
				if (d->refList != this) {
					Node *n = detach_helper_grow(i, 1);
					QT_TRY {
						node_construct(n, t);
					} QT_CATCH(...) {
						p.remove(i);
						QT_RETHROW;
					}
				} else {
					if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) {
						Node *n = reinterpret_cast<Node *>(p.insert(i,mMemMan));
						QT_TRY {
							node_construct(n, t);
						} QT_CATCH(...) {
							p.remove(i);
							QT_RETHROW;
						}
					} else {
						Node *n, copy;
						node_construct(&copy, t); // t might be a reference to an object in the array
						QT_TRY {
							n = reinterpret_cast<Node *>(p.insert(i,mMemMan));;
						} QT_CATCH(...) {
							node_destruct(&copy);
							QT_RETHROW;
						}
						*n = copy;
					}
				}
			}

			template <typename T>
			inline void CQtManagedRestrictedModificationList<T>::replace(int i, const T &t)
			{
				Q_ASSERT_X(i >= 0 && i < p.size(), "CQtManagedRestrictedModificationList<T>::replace", "index out of range");
				detach();
				reinterpret_cast<Node *>(p.at(i))->t() = t;
			}

			template <typename T>
			inline void CQtManagedRestrictedModificationList<T>::swap(int i, int j)
			{
				Q_ASSERT_X(i >= 0 && i < p.size() && j >= 0 && j < p.size(),
					"CQtManagedRestrictedModificationList<T>::swap", "index out of range");
				detach();
				void *t = d->array[d->begin + i];
				d->array[d->begin + i] = d->array[d->begin + j];
				d->array[d->begin + j] = t;
			}

			template <typename T>
			inline void CQtManagedRestrictedModificationList<T>::move(int from, int to)
			{
				Q_ASSERT_X(from >= 0 && from < p.size() && to >= 0 && to < p.size(),
					"CQtManagedRestrictedModificationList<T>::move", "index out of range");
				detach();
				p.move(from, to);
			}

			template<typename T>
			Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationList<T> CQtManagedRestrictedModificationList<T>::mid(int pos, int alength) const
			{
				if (alength < 0 || pos + alength > size())
					alength = size() - pos;
				if (pos == 0 && alength == size())
					return *this;
				CQtManagedRestrictedModificationList<T> cpy;
				cpy.reserve(alength);
				cpy.d->end = alength;
				QT_TRY {
					cpy.node_copy(reinterpret_cast<Node *>(cpy.p.begin()),
						reinterpret_cast<Node *>(cpy.p.end()),
						reinterpret_cast<Node *>(p.begin() + pos));
				} QT_CATCH(...) {
					// restore the old end
					cpy.d->end = 0;
					QT_RETHROW;
				}
				return cpy;
			}

			template<typename T>
			Q_OUTOFLINE_TEMPLATE T CQtManagedRestrictedModificationList<T>::value(int i) const
			{
				if (i < 0 || i >= p.size()) {
					return T();
				}
				return reinterpret_cast<Node *>(p.at(i))->t();
			}

			template<typename T>
			Q_OUTOFLINE_TEMPLATE T CQtManagedRestrictedModificationList<T>::value(int i, const T& defaultValue) const
			{
				return ((i < 0 || i >= p.size()) ? defaultValue : reinterpret_cast<Node *>(p.at(i))->t());
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE typename CQtManagedRestrictedModificationList<T>::Node *CQtManagedRestrictedModificationList<T>::detach_helper_grow(int i, int c)
			{
				Node *n = reinterpret_cast<Node *>(p.begin());
				CQtManagedRestrictedModificationListData::Data *x = p.detach_grow(&i, c,mMemMan,this);
				QT_TRY {
					node_copy(reinterpret_cast<Node *>(p.begin()),
						reinterpret_cast<Node *>(p.begin() + i), n);
				} QT_CATCH(...) {
					//qFree(d);
					CObjectAllocator<CQtManagedRestrictedModificationListData::Data>::destroyAndRelease(d,mMemMan);
					d = x;
					QT_RETHROW;
				}
				QT_TRY {
					node_copy(reinterpret_cast<Node *>(p.begin() + i + c),
						reinterpret_cast<Node *>(p.end()), n + i);
				} QT_CATCH(...) {
					node_destruct(reinterpret_cast<Node *>(p.begin()),
						reinterpret_cast<Node *>(p.begin() + i));
					//qFree(d);
					CObjectAllocator<CQtManagedRestrictedModificationListData::Data>::destroyAndRelease(d,mMemMan);
					d = x;
					QT_RETHROW;
				}

				//if (!x->ref.deref())
				if (d->refList == this) {
					free(x);
				}

				return reinterpret_cast<Node *>(p.begin() + i);
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE void CQtManagedRestrictedModificationList<T>::detach_helper(int alloc)
			{
				Node *n = reinterpret_cast<Node *>(p.begin());
				CQtManagedRestrictedModificationListData::Data *x = p.detach(alloc,mMemMan,this);
				QT_TRY {
					node_copy(reinterpret_cast<Node *>(p.begin()), reinterpret_cast<Node *>(p.end()), n);
				} QT_CATCH(...) {
					//qFree(d);
					CObjectAllocator<CQtManagedRestrictedModificationListData::Data>::destroyAndRelease(d,mMemMan);
					d = x;
					QT_RETHROW;
				}

				//if (!x->ref.deref())
				if (d->refList == this) {
					free(x);
				}
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE void CQtManagedRestrictedModificationList<T>::detach_helper()
			{
				detach_helper(d->alloc);
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationList<T>::~CQtManagedRestrictedModificationList()
			{
				//if (d && !d->ref.deref())
				if (d && d->refList == this)
					free(d);
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE bool CQtManagedRestrictedModificationList<T>::operator==(const CQtManagedRestrictedModificationList<T> &l) const
			{
				if (p.size() != l.p.size())
					return false;
				if (d == l.d)
					return true;
				Node *i = reinterpret_cast<Node *>(p.end());
				Node *b = reinterpret_cast<Node *>(p.begin());
				Node *li = reinterpret_cast<Node *>(l.p.end());
				while (i != b) {
					--i; --li;
					if (!(i->t() == li->t()))
						return false;
				}
				return true;
			}

			// ### Qt 5: rename freeData() to avoid confusion with std::free()
			template <typename T>
			Q_OUTOFLINE_TEMPLATE void CQtManagedRestrictedModificationList<T>::free(CQtManagedRestrictedModificationListData::Data *data)
			{
				node_destruct(reinterpret_cast<Node *>(data->array + data->begin),
					reinterpret_cast<Node *>(data->array + data->end));
				//if (data->ref == 0)
				if (data->refList == this)
					//qFree(data);
					CObjectAllocator<CQtManagedRestrictedModificationListData::Data>::destroyAndRelease(data,mMemMan);
			}


			template <typename T>
			Q_OUTOFLINE_TEMPLATE void CQtManagedRestrictedModificationList<T>::clear()
			{
				*this = CQtManagedRestrictedModificationList<T>(mContext);
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE int CQtManagedRestrictedModificationList<T>::removeAll(const T &_t)
			{
				detachShared();
				const T t = _t;
				int removedCount=0, i=0;
				Node *n;
				while (i < p.size())
					if ((n = reinterpret_cast<Node *>(p.at(i)))->t() == t) {
						node_destruct(n);
						p.remove(i);
						++removedCount;
					} else {
						++i;
					}
				return removedCount;
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE bool CQtManagedRestrictedModificationList<T>::removeOne(const T &_t)
			{
				detachShared();
				int index = indexOf(_t);
				if (index != -1) {
					removeAt(index);
					return true;
				}
				return false;
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE typename CQtManagedRestrictedModificationList<T>::iterator CQtManagedRestrictedModificationList<T>::erase(typename CQtManagedRestrictedModificationList<T>::iterator afirst,
				typename CQtManagedRestrictedModificationList<T>::iterator alast)
			{
				for (Node *n = afirst.i; n < alast.i; ++n)
					node_destruct(n);
				int idx = afirst - begin();
				p.remove(idx, alast - afirst);
				return begin() + idx;
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE CQtManagedRestrictedModificationList<T> &CQtManagedRestrictedModificationList<T>::operator+=(const CQtManagedRestrictedModificationList<T> &l)
			{
				if (!l.isEmpty()) {
					if (isEmpty()) {
						*this = l;
					} else {
						//Node *n = (d->ref != 1)
						Node *n = (d->refList != this)
							? detach_helper_grow(INT_MAX, l.size())
							: reinterpret_cast<Node *>(p.append2(l.p,mMemMan));
						QT_TRY {
							node_copy(n, reinterpret_cast<Node *>(p.end()),
								reinterpret_cast<Node *>(l.p.begin()));
						} QT_CATCH(...) {
							// restore the old end
							d->end -= int(reinterpret_cast<Node *>(p.end()) - n);
							QT_RETHROW;
						}
					}
				}
				return *this;
			}

			template <typename T>
			inline void CQtManagedRestrictedModificationList<T>::append(const CQtManagedRestrictedModificationList<T> &t)
			{
				*this += t;
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE int CQtManagedRestrictedModificationList<T>::indexOf(const T &t, int from) const
			{
				if (from < 0)
					from = qMax(from + p.size(), 0);
				if (from < p.size()) {
					Node *n = reinterpret_cast<Node *>(p.at(from -1));
					Node *e = reinterpret_cast<Node *>(p.end());
					while (++n != e)
						if (n->t() == t)
							return int(n - reinterpret_cast<Node *>(p.begin()));
				}
				return -1;
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE int CQtManagedRestrictedModificationList<T>::lastIndexOf(const T &t, int from) const
			{
				if (from < 0)
					from += p.size();
				else if (from >= p.size())
					from = p.size()-1;
				if (from >= 0) {
					Node *b = reinterpret_cast<Node *>(p.begin());
					Node *n = reinterpret_cast<Node *>(p.at(from + 1));
					while (n-- != b) {
						if (n->t() == t)
							return n - b;
					}
				}
				return -1;
			}

			template <typename T>
            Q_OUTOFLINE_TEMPLATE bool CQtManagedRestrictedModificationList<T>::contains(const T &t) const
			{
				Node *b = reinterpret_cast<Node *>(p.begin());
				Node *i = reinterpret_cast<Node *>(p.end());
				while (i-- != b)
					if (i->t() == t)
                        return true;
                return false;
			}

			template <typename T>
			Q_OUTOFLINE_TEMPLATE int CQtManagedRestrictedModificationList<T>::count(const T &t) const
			{
				int c = 0;
				Node *b = reinterpret_cast<Node *>(p.begin());
				Node *i = reinterpret_cast<Node *>(p.end());
				while (i-- != b)
					if (i->t() == t)
						++c;
				return c;
			}


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_LIST_CONTAINER


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONLIST_CPP
