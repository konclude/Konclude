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
 *		Note, this is a modified version of the QList from the Qt framework.
 *
 */


#include "CQtManagedRestrictedModificationListData.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_LIST_CONTAINER



			CQtManagedRestrictedModificationListData::Data CQtManagedRestrictedModificationListData::shared_null = { nullptr, 0, 0, 0, true, { 0 } };



			int cAllocMore(int alloc, int extra)
			{
				if (alloc == 0 && extra == 0)
					return 0;
				const int page = 1 << 12;
				int nalloc;
				alloc += extra;
				if (alloc < 1<<6) {
					nalloc = (1<<3) + ((alloc >>3) << 3);
				} else  {
					// don't do anything if the loop will overflow signed int.
					if (alloc >= INT_MAX/2)
						return INT_MAX;
					nalloc = (alloc < page) ? 1 << 3 : page;
					while (nalloc < alloc) {
						if (nalloc <= 0)
							return INT_MAX;
						nalloc *= 2;
					}
				}
				return nalloc - extra;
			}


			static int grow(int size, CMemoryAllocationManager* memMan)
			{
				// dear compiler: don't optimize me out.
				//if (!memMan) {
				volatile int x = cAllocMore(size * sizeof(void *), CQtManagedRestrictedModificationListData::DataHeaderSize) / sizeof(void *);
				return x;
				/*} else {
					return memMan->getOptimizedMoreAllocationSize(size * sizeof(void *)+ CQtManagedRestrictedModificationListData::DataHeaderSize) / sizeof(void *);
				}*/
			}

			/*!
			*  Detaches the CQtManagedRestrictedModificationListData by allocating new memory for a list which will be bigger
			*  than the copied one and is expected to grow further.
			*  *idx is the desired insertion point and is clamped to the actual size of the list.
			*  num is the number of new elements to insert at the insertion point.
			*  Returns the old (shared) data, it is up to the caller to deref() and free().
			*  For the new data node_copy needs to be called.
			*
			*  \internal
			*/
			CQtManagedRestrictedModificationListData::Data *CQtManagedRestrictedModificationListData::detach_grow(int *idx, int num, CMemoryAllocationManager* memMan, CQtManagedRestrictedModificationListBase* listBase)
			{
				Data *x = d;
				int l = x->end - x->begin;
				int nl = l + num;
				int alloc = grow(nl,memMan);
				Data* t = static_cast<Data *>(CMemoryAllocator::allocateMemory(memMan,DataHeaderSize + alloc * sizeof(void *)));
				Q_CHECK_PTR(t);

				//t->ref = 1;
				t->refList = listBase;
				t->sharable = true;
				t->alloc = alloc;
				// The space reservation algorithm's optimization is biased towards appending:
				// Something which looks like an append will put the data at the beginning,
				// while something which looks like a prepend will put it in the middle
				// instead of at the end. That's based on the assumption that prepending
				// is uncommon and even an initial prepend will eventually be followed by
				// at least some appends.
				int bg;
				if (*idx < 0) {
					*idx = 0;
					bg = (alloc - nl) >> 1;
				} else if (*idx > l) {
					*idx = l;
					bg = 0;
				} else if (*idx < (l >> 1)) {
					bg = (alloc - nl) >> 1;
				} else {
					bg = 0;
				}
				t->begin = bg;
				t->end = bg + nl;
				d = t;

				return x;
			}


			/*!
			*  Detaches the CQtManagedRestrictedModificationListData by allocating new memory for a list which possibly
			*  has a different size than the copied one.
			*  Returns the old (shared) data, it is up to the caller to deref() and free()
			*  For the new data node_copy needs to be called.
			*
			*  \internal
			*/
			CQtManagedRestrictedModificationListData::Data *CQtManagedRestrictedModificationListData::detach(int alloc, CMemoryAllocationManager* memMan, CQtManagedRestrictedModificationListBase* listBase)
			{
				Data *x = d;
				Data* t = nullptr;
				//t = static_cast<Data *>(qMalloc(DataHeaderSize + alloc * sizeof(void *)));
				t = static_cast<Data *>(CMemoryAllocator::allocateMemory(memMan,DataHeaderSize + alloc * sizeof(void *)));
				Q_CHECK_PTR(t);

				//t->ref = 1;
				t->refList = listBase;
				t->sharable = true;
				t->alloc = alloc;
				if (!alloc) {
					t->begin = 0;
					t->end = 0;
				} else {
					t->begin = x->begin;
					t->end   = x->end;
				}
				d = t;

				return x;
			}


			// ensures that enough space is available to append n elements
			void **CQtManagedRestrictedModificationListData::append(int n, CMemoryAllocationManager* memMan)
			{
				//Q_ASSERT(d->ref == 1);
				int e = d->end;
				if (e + n > d->alloc) {
					int b = d->begin;
					if (b - n >= 2 * d->alloc / 3) {
						// we have enough space. Just not at the end -> move it.
						e -= b;
						::memcpy(d->array, d->array + b, e * sizeof(void *));
						d->begin = 0;
					} else {
						realloc(grow(d->alloc + n,memMan),memMan);
					}
				}
				d->end = e + n;
				return d->array + e;
			}

			// ensures that enough space is available to append one element
			void **CQtManagedRestrictedModificationListData::append(CMemoryAllocationManager* memMan)
			{
				return append(1,memMan);
			}

			// ensures that enough space is available to append the list
			void **CQtManagedRestrictedModificationListData::append2(const CQtManagedRestrictedModificationListData& l, CMemoryAllocationManager* memMan)
			{
				return append(l.d->end - l.d->begin,memMan);
			}

			void **CQtManagedRestrictedModificationListData::append(const CQtManagedRestrictedModificationListData& l, CMemoryAllocationManager* memMan)
			{
				//Q_ASSERT(d->ref == 1);
				int e = d->end;
				int n = l.d->end - l.d->begin;
				if (n) {
					if (e + n > d->alloc)
						realloc(grow(e + n,memMan),memMan);
					::memcpy(d->array + d->end, l.d->array + l.d->begin, n*sizeof(void*));
					d->end += n;
				}
				return d->array + e;
			}


			void **CQtManagedRestrictedModificationListData::prepend(CMemoryAllocationManager* memMan)
			{
				//Q_ASSERT(d->ref == 1);
				if (d->begin == 0) {
					if (d->end >= d->alloc / 3)
						realloc(grow(d->alloc + 1,memMan),memMan);

					if (d->end < d->alloc / 3)
						d->begin = d->alloc - 2 * d->end;
					else
						d->begin = d->alloc - d->end;

					::memmove(d->array + d->begin, d->array, d->end * sizeof(void *));
					d->end += d->begin;
				}
				return d->array + --d->begin;
			}

			void **CQtManagedRestrictedModificationListData::insert(int i, CMemoryAllocationManager* memMan)
			{
				//Q_ASSERT(d->ref == 1);
				if (i <= 0)
					return prepend(memMan);
				int size = d->end - d->begin;
				if (i >= size)
					return append(memMan);

				bool leftward = false;

				if (d->begin == 0) {
					if (d->end == d->alloc) {
						// If the array is full, we expand it and move some items rightward
						realloc(grow(d->alloc + 1,memMan),memMan);
					} else {
						// If there is free space at the end of the array, we move some items rightward
					}
				} else {
					if (d->end == d->alloc) {
						// If there is free space at the beginning of the array, we move some items leftward
						leftward = true;
					} else {
						// If there is free space at both ends, we move as few items as possible
						leftward = (i < size - i);
					}
				}

				if (leftward) {
					--d->begin;
					::memmove(d->array + d->begin, d->array + d->begin + 1, i * sizeof(void *));
				} else {
					::memmove(d->array + d->begin + i + 1, d->array + d->begin + i,
						(size - i) * sizeof(void *));
					++d->end;
				}
				return d->array + d->begin + i;
			}

			void CQtManagedRestrictedModificationListData::remove(int i)
			{
				//Q_ASSERT(d->ref == 1);
				i += d->begin;
				if (i - d->begin < d->end - i) {
					if (int offset = i - d->begin)
						::memmove(d->array + d->begin + 1, d->array + d->begin, offset * sizeof(void *));
					d->begin++;
				} else {
					if (int offset = d->end - i - 1)
						::memmove(d->array + i, d->array + i + 1, offset * sizeof(void *));
					d->end--;
				}
			}

			void CQtManagedRestrictedModificationListData::remove(int i, int n)
			{
				//Q_ASSERT(d->ref == 1);
				i += d->begin;
				int middle = i + n/2;
				if (middle - d->begin < d->end - middle) {
					::memmove(d->array + d->begin + n, d->array + d->begin,
						(i - d->begin) * sizeof(void*));
					d->begin += n;
				} else {
					::memmove(d->array + i, d->array + i + n,
						(d->end - i - n) * sizeof(void*));
					d->end -= n;
				}
			}

			void CQtManagedRestrictedModificationListData::move(int from, int to)
			{
				//Q_ASSERT(d->ref == 1);
				if (from == to)
					return;

				from += d->begin;
				to += d->begin;
				void *t = d->array[from];

				if (from < to) {
					if (d->end == d->alloc || 3 * (to - from) < 2 * (d->end - d->begin)) {
						::memmove(d->array + from, d->array + from + 1, (to - from) * sizeof(void *));
					} else {
						// optimization
						if (int offset = from - d->begin)
							::memmove(d->array + d->begin + 1, d->array + d->begin, offset * sizeof(void *));
						if (int offset = d->end - (to + 1))
							::memmove(d->array + to + 2, d->array + to + 1, offset * sizeof(void *));
						++d->begin;
						++d->end;
						++to;
					}
				} else {
					if (d->begin == 0 || 3 * (from - to) < 2 * (d->end - d->begin)) {
						::memmove(d->array + to + 1, d->array + to, (from - to) * sizeof(void *));
					} else {
						// optimization
						if (int offset = to - d->begin)
							::memmove(d->array + d->begin - 1, d->array + d->begin, offset * sizeof(void *));
						if (int offset = d->end - (from + 1))
							::memmove(d->array + from, d->array + from + 1, offset * sizeof(void *));
						--d->begin;
						--d->end;
						--to;
					}
				}
				d->array[to] = t;
			}

			void **CQtManagedRestrictedModificationListData::erase(void **xi)
			{
				//Q_ASSERT(d->ref == 1);
				int i = xi - (d->array + d->begin);
				remove(i);
				return d->array + d->begin + i;
			}

			void CQtManagedRestrictedModificationListData::realloc(int alloc, CMemoryAllocationManager* memMan)
			{
				//Q_ASSERT(d->ref == 1);
				//Data *x = static_cast<Data *>(qRealloc(d, DataHeaderSize + alloc * sizeof(void *)));
				Data *x = nullptr;
				x = static_cast<Data *>(CMemoryAllocator::reallocateMemory(d,memMan, DataHeaderSize + d->alloc * sizeof(void *), DataHeaderSize + alloc * sizeof(void *)));
				Q_CHECK_PTR(x);

				d = x;
				d->alloc = alloc;
				if (!alloc)
					d->begin = d->end = 0;
			}


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_LIST_CONTAINER


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

