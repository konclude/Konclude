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


#include "CQtManagedRestrictedModificationHashData.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_HASH_CONTAINER

			static const uchar prime_deltas[] = {
				0,  0,  1,  3,  1,  5,  3,  3,  1,  9,  7,  5,  3,  9, 25,  3,
				1, 21,  3, 21,  7, 15,  9,  5,  3, 29, 15,  0,  0,  0,  0,  0
			};

			static inline int primeForNumBits(int numBits)
			{
				return (1 << numBits) + prime_deltas[numBits];
			}

			/*
			Returns the smallest integer n such that
			primeForNumBits(n) >= hint.
			*/
			static int countBits(int hint)
			{
				int numBits = 0;
				int bits = hint;

				while (bits > 1) {
					bits >>= 1;
					numBits++;
				}

				if (numBits >= (int)sizeof(prime_deltas)) {
					numBits = sizeof(prime_deltas) - 1;
				} else if (primeForNumBits(numBits) < hint) {
					++numBits;
				}
				return numBits;
			}

			const int MinNumBits = 4;

			CQtManagedRestrictedModificationHashData CQtManagedRestrictedModificationHashData::shared_null = {
				0, 0, /*Q_BASIC_ATOMIC_INITIALIZER(1)*/0, 0, 0, MinNumBits, 0, 0, true, false, 0
			};

			void *CQtManagedRestrictedModificationHashData::allocateNode(CMemoryAllocationManager* memMan)
			{
				return allocateNode(0,memMan);
			}

			void *CQtManagedRestrictedModificationHashData::allocateNode(int nodeAlign, CMemoryAllocationManager* memMan)
			{
				//void *ptr = strictAlignment ? qMallocAligned(nodeSize, nodeAlign) : qMalloc(nodeSize);
				//void *ptr = strictAlignment ? memMan->allocateAligned(nodeSize,nodeAlign) : memMan->allocate(nodeSize);
				void *ptr = strictAlignment ? CMemoryAllocator::allocateAlignedMemory(memMan,nodeSize,nodeAlign) : CMemoryAllocator::allocateMemory(memMan,nodeSize);
				Q_CHECK_PTR(ptr);
				return ptr;
			}

			void CQtManagedRestrictedModificationHashData::freeNode(void *node, CMemoryAllocationManager* memMan)
			{
				if (strictAlignment)
					//qFreeAligned(node);
					CMemoryAllocator::releaseAlignedMemory(node,memMan);
				else
					//qFree(node);
					CMemoryAllocator::releaseMemory(node,memMan);
			}

			CQtManagedRestrictedModificationHashData *CQtManagedRestrictedModificationHashData::detach_helper(void (*node_duplicate)(Node *, void *), int nodeSize)
			{
				return detach_helper2( node_duplicate, 0, nodeSize, 0, 0, 0);
			}

			CQtManagedRestrictedModificationHashData *CQtManagedRestrictedModificationHashData::detach_helper2(void (*node_duplicate)(Node *, void *),
				void (*node_delete)(Node *),
				int nodeSize,
				int nodeAlign, CQtManagedRestrictedModificationHashBase* referencedHash, CMemoryAllocationManager* memMan)
			{
				union {
					CQtManagedRestrictedModificationHashData *d;
					Node *e;
				};
				//d = new CQtManagedRestrictedModificationHashData;
				d = CObjectAllocator<CQtManagedRestrictedModificationHashData>::allocate(memMan);
				d->fakeNext = 0;
				d->buckets = 0;
				//d->ref = 1;
				d->refHash = referencedHash;
				d->size = size;
				d->nodeSize = nodeSize;
				d->userNumBits = userNumBits;
				d->numBits = numBits;
				d->numBuckets = numBuckets;
				d->sharable = true;
				d->strictAlignment = nodeAlign > 8;
				d->reserved = 0;

				if (numBuckets) {
					QT_TRY {
						//d->buckets = new Node *[numBuckets];
						d->buckets = CObjectAllocator<Node*>::allocateArray(memMan,numBuckets);
					} QT_CATCH(...) {
						// restore a consistent state for d
						d->numBuckets = 0;
						// roll back
						d->free_helper(node_delete,memMan);
						QT_RETHROW;
					}

					Node *this_e = reinterpret_cast<Node *>(this);
					for (int i = 0; i < numBuckets; ++i) {
						Node **nextNode = &d->buckets[i];
						Node *oldNode = buckets[i];
						while (oldNode != this_e) {
							QT_TRY {
								Node *dup = static_cast<Node *>(allocateNode(nodeAlign,memMan));

								QT_TRY {
									node_duplicate(oldNode, dup);
								} QT_CATCH(...) {
									freeNode(dup,memMan);
									QT_RETHROW;
								}

								dup->h = oldNode->h;
								*nextNode = dup;
								nextNode = &dup->next;
								oldNode = oldNode->next;
							} QT_CATCH(...) {
								// restore a consistent state for d
								*nextNode = e;
								d->numBuckets = i+1;
								// roll back
								d->free_helper(node_delete,memMan);
								QT_RETHROW;
							}
						}
						*nextNode = e;
					}
				}
				return d;
			}

			CQtManagedRestrictedModificationHashData *CQtManagedRestrictedModificationHashData::restricted_detach_helper2(bool (*copy_restricted)(Node *, void *), 
				void *assistValue, void (*node_duplicate)(Node *, void *), void (*node_delete)(Node *), 
				int nodeSize,
				int nodeAlign, CQtManagedRestrictedModificationHashBase* referencedHash, CMemoryAllocationManager* memMan)
			{
				union {
					CQtManagedRestrictedModificationHashData *d;
					Node *e;
				};
				//d = new CQtManagedRestrictedModificationHashData;
				d = CObjectAllocator<CQtManagedRestrictedModificationHashData>::allocate(memMan);
				d->fakeNext = 0;
				d->buckets = 0;
				//d->ref = 1;
				d->refHash = referencedHash;
				d->size = size;
				d->nodeSize = nodeSize;
				d->userNumBits = userNumBits;
				d->numBits = numBits;
				d->numBuckets = numBuckets;
				d->sharable = true;
				d->strictAlignment = nodeAlign > 8;
				d->reserved = 0;

				if (numBuckets) {
					QT_TRY {
						//d->buckets = new Node *[numBuckets];
						d->buckets = CObjectAllocator<Node*>::allocateArray(memMan,numBuckets);
					} QT_CATCH(...) {
						// restore a consistent state for d
						d->numBuckets = 0;
						// roll back
						d->free_helper(node_delete,memMan);
						QT_RETHROW;
					}

					Node *this_e = reinterpret_cast<Node *>(this);
					for (int i = 0; i < numBuckets; ++i) {
						Node **nextNode = &d->buckets[i];
						Node *oldNode = buckets[i];
						while (oldNode != this_e) {
							if (copy_restricted(oldNode,assistValue)) {
								QT_TRY {
									Node *dup = static_cast<Node *>(allocateNode(nodeAlign,memMan));

									QT_TRY {
										node_duplicate(oldNode, dup);
									} QT_CATCH(...) {
										freeNode(dup,memMan);
										QT_RETHROW;
									}

									dup->h = oldNode->h;
									*nextNode = dup;
									nextNode = &dup->next;
									oldNode = oldNode->next;
								} QT_CATCH(...) {
									// restore a consistent state for d
									*nextNode = e;
									d->numBuckets = i+1;
									// roll back
									d->free_helper(node_delete,memMan);
									QT_RETHROW;
								}
							} else {
								d->size = d->size - 1;
								oldNode = oldNode->next;
							}
						}
						*nextNode = e;
					}
				}
				return d;
			}

			void CQtManagedRestrictedModificationHashData::free_helper(void (*node_delete)(Node *), CMemoryAllocationManager* memMan)
			{
				if (node_delete) {
					Node *this_e = reinterpret_cast<Node *>(this);
					Node **bucket = reinterpret_cast<Node **>(this->buckets);

					int n = numBuckets;
					while (n--) {
						Node *cur = *bucket++;
						while (cur != this_e) {
							Node *next = cur->next;
							node_delete(cur);
							freeNode(cur,memMan);
							cur = next;
						}
					}
				}
				//delete [] buckets;
				CObjectAllocator<Node*>::releaseArray(buckets,memMan);
				//delete this;
				CObjectAllocator<CQtManagedRestrictedModificationHashData>::release(this,memMan);
			}

			CQtManagedRestrictedModificationHashData::Node *CQtManagedRestrictedModificationHashData::nextNode(Node *node)
			{
				union {
					Node *next;
					Node *e;
					CQtManagedRestrictedModificationHashData *d;
				};
				next = node->next;
				Q_ASSERT_X(next, "QHash", "Iterating beyond end()");
				if (next->next)
					return next;

				int start = (node->h % d->numBuckets) + 1;
				Node **bucket = d->buckets + start;
				int n = d->numBuckets - start;
				while (n--) {
					if (*bucket != e)
						return *bucket;
					++bucket;
				}
				return e;
			}

			CQtManagedRestrictedModificationHashData::Node *CQtManagedRestrictedModificationHashData::previousNode(Node *node)
			{
				union {
					Node *e;
					CQtManagedRestrictedModificationHashData *d;
				};

				e = node;
				while (e->next)
					e = e->next;

				int start;
				if (node == e)
					start = d->numBuckets - 1;
				else
					start = node->h % d->numBuckets;

				Node *sentinel = node;
				Node **bucket = d->buckets + start;
				while (start >= 0) {
					if (*bucket != sentinel) {
						Node *prev = *bucket;
						while (prev->next != sentinel)
							prev = prev->next;
						return prev;
					}

					sentinel = e;
					--bucket;
					--start;
				}
				Q_ASSERT_X(start >= 0, "QHash", "Iterating backward beyond begin()");
				return e;
			}

			/*
			If hint is negative, -hint gives the approximate number of
			buckets that should be used for the hash table. If hint is
			nonnegative, (1 << hint) gives the approximate number
			of buckets that should be used.
			*/
			void CQtManagedRestrictedModificationHashData::rehash(int hint, CMemoryAllocationManager* memMan)
			{
				if (hint < 0) {
					hint = countBits(-hint);
					if (hint < MinNumBits)
						hint = MinNumBits;
					userNumBits = hint;
					while (primeForNumBits(hint) < (size >> 1))
						++hint;
				} else if (hint < MinNumBits) {
					hint = MinNumBits;
				}

				if (numBits != hint) {
					Node *e = reinterpret_cast<Node *>(this);
					Node **oldBuckets = buckets;
					int oldNumBuckets = numBuckets;

					int nb = primeForNumBits(hint);
					//buckets = new Node *[nb];
					buckets = CObjectAllocator<Node*>::allocateArray(memMan,nb);
					numBits = hint;
					numBuckets = nb;
					for (int i = 0; i < numBuckets; ++i)
						buckets[i] = e;

					for (int i = 0; i < oldNumBuckets; ++i) {
						Node *firstNode = oldBuckets[i];
						while (firstNode != e) {
							uint h = firstNode->h;
							Node *lastNode = firstNode;
							while (lastNode->next != e && lastNode->next->h == h)
								lastNode = lastNode->next;

							Node *afterLastNode = lastNode->next;
							Node **beforeFirstNode = &buckets[h % numBuckets];
							while (*beforeFirstNode != e)
								beforeFirstNode = &(*beforeFirstNode)->next;
							lastNode->next = *beforeFirstNode;
							*beforeFirstNode = firstNode;
							firstNode = afterLastNode;
						}
					}
					//delete [] oldBuckets;
					CObjectAllocator<Node*>::releaseArray(oldBuckets,memMan);
				}
			}

			void CQtManagedRestrictedModificationHashData::destroyAndFree(CMemoryAllocationManager* memMan)
			{
				free_helper(0,memMan);
			}

#ifdef KONCLUDE_CQTMANAGEDRESTRICTEDMODIFICATIONHASH_DEBUG

			void CQtManagedRestrictedModificationHashData::dump()
			{
				qDebug("Hash data (ref = %d, size = %d, nodeSize = %d, userNumBits = %d, numBits = %d, numBuckets = %d)",
					/*int(ref)*/(int)refHash, size, nodeSize, userNumBits, numBits,
					numBuckets);
				qDebug("    %p (fakeNode = %p)", this, fakeNext);
				for (int i = 0; i < numBuckets; ++i) {
					QString line;
					Node *n = buckets[i];
					if (n != reinterpret_cast<Node *>(this)) {
						line.sprintf("%d:", i);
						while (n != reinterpret_cast<Node *>(this)) {
							line += QString().sprintf(" -> [%p]", n);
							if (!n) {
								line += " (CORRUPT)";
								break;
							}
							n = n->next;
						}
						qDebug(qPrintable(line));
					}
				}
			}

			void CQtManagedRestrictedModificationHashData::checkSanity()
			{
				if (fakeNext)
					qFatal("Fake next isn't 0");

				for (int i = 0; i < numBuckets; ++i) {
					Node *n = buckets[i];
					Node *p = n;
					if (!n)
						qFatal("%d: Bucket entry is 0", i);
					if (n != reinterpret_cast<Node *>(this)) {
						while (n != reinterpret_cast<Node *>(this)) {
							if (!n->next)
								qFatal("%d: Next of %p is 0, should be %p", i, n, this);
							n = n->next;
						}
					}
				}
			}
#endif

			void CQtManagedRestrictedModificationHashData::mightGrow(CMemoryAllocationManager* memMan) // ### Qt 5: eliminate
			{
				if (size >= numBuckets)
					rehash(numBits + 1,memMan);
			}

			//inline bool CQtManagedRestrictedModificationHashData::willGrow()
			//{
			//	if (size >= numBuckets) {
			//		rehash(numBits + 1);
			//		return true;
			//	} else {
			//		return false;
			//	}
			//}

			void CQtManagedRestrictedModificationHashData::hasShrunk(CMemoryAllocationManager* memMan)
			{
				if (size <= (numBuckets >> 3) && numBits > userNumBits) {
					QT_TRY {
						rehash(qMax(int(numBits) - 2, int(userNumBits)),memMan);
					} QT_CATCH(const std::bad_alloc &) {
						// ignore bad allocs - shrinking shouldn't throw. rehash is exception safe.
					}
				}
			}

			CQtManagedRestrictedModificationHashData::Node *CQtManagedRestrictedModificationHashData::firstNode()
			{
				Node *e = reinterpret_cast<Node *>(this);
				Node **bucket = buckets;
				int n = numBuckets;
				while (n--) {
					if (*bucket != e)
						return *bucket;
					++bucket;
				}
				return e;
			}


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_HASH_CONTAINER


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

