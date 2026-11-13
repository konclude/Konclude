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
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTSATURATIONLABELHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTSATURATIONLABELHASH_H

// Libraries includes
#include <cstring>

// Namespace includes
#include "ProcessSettings.h"
#include "CConceptSaturationDescriptorReapplyData.h"
#include "CProcessContext.h"

// Other includes
#include "Utilities/Memory/CMemoryAllocationManager.h"
#include "Utilities/Memory/CMemoryAllocator.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*!
				 *
				 *		\class		CReapplyConceptSaturationLabelHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		The hash from concept tags to the saturation descriptors and reapplication queues that
				 *					make up a saturation node's label (CReapplyConceptSaturationLabelSet).
				 *
				 *					Open addressing with linear probing over one array of entries, power-of-two capacity,
				 *					Fibonacci hashing of the tag, at most three quarters full. A lookup therefore touches one
				 *					cache line in the common case, an insertion needs no allocation, and copying a label is a
				 *					memcpy. The chained CPROCESSHASH it replaces allocated every node from the pools and
				 *					duplicated them one by one on each copy; those hash operations were a third of the
				 *					saturation's time on SNOMED CT.
				 *
				 *					The memory comes from the process context's allocation manager, like all saturation data,
				 *					and is never freed individually (the pools are released as a whole, their release is a
				 *					no-op), so an outgrown array goes to a free list in the process context and serves the
				 *					next label that grows to that capacity. Tags are never negative, so -1 marks a free slot.
				 *
				 */
				class CReapplyConceptSaturationLabelHash {
					// public methods
					public:
						struct Entry {
							cint64 key;
							CConceptSaturationDescriptorReapplyData value;
						};

						static const cint64 EMPTYKEY = -1;
						static const cint64 INITIALCAPACITY = 16;

						//! Constructor
						inline CReapplyConceptSaturationLabelHash(CProcessContext* processContext = nullptr) : mEntries(nullptr), mCapacity(0), mSize(0), mShift(64), mMemMan(nullptr), mFreeArrays(nullptr) {
							if (processContext) {
								mMemMan = processContext->getUsedMemoryAllocationManager();
								mFreeArrays = processContext->getSaturationLabelHashFreeArrays();
							}
						}

						inline cint64 count() const {
							return mSize;
						}

						inline bool isEmpty() const {
							return mSize == 0;
						}

						inline cint64 capacity() const {
							return mCapacity;
						}

						//! The data for the tag, created with null pointers if the tag is new.
						inline CConceptSaturationDescriptorReapplyData& operator[](cint64 key) {
							Q_ASSERT(key >= 0);
							if ((mSize + 1) * 4 > mCapacity * 3) {
								grow();
							}
							Entry* entry = probe(key);
							if (entry->key == EMPTYKEY) {
								entry->key = key;
								++mSize;
							}
							return entry->value;
						}

						inline bool tryGetValuePointer(cint64 key, CConceptSaturationDescriptorReapplyData*& valuePointer) const {
							if (mSize == 0) {
								return false;
							}
							Entry* entry = probe(key);
							if (entry->key == EMPTYKEY) {
								return false;
							}
							valuePointer = &entry->value;
							return true;
						}

						inline bool contains(cint64 key) const {
							CConceptSaturationDescriptorReapplyData* valuePointer = nullptr;
							return tryGetValuePointer(key,valuePointer);
						}

						//! Replaces the contents with a copy of the other hash's entries, keeping its layout, so no rehashing.
						inline CReapplyConceptSaturationLabelHash* copyFrom(const CReapplyConceptSaturationLabelHash& other) {
							if (this == &other) {
								return this;
							}
							if (mEntries) {
								releaseArray(mEntries,mCapacity);
								mEntries = nullptr;
							}
							mCapacity = other.mCapacity;
							mSize = other.mSize;
							mShift = other.mShift;
							if (mCapacity > 0) {
								mEntries = allocateArray(mCapacity);
								memcpy((void*)mEntries,(const void*)other.mEntries,sizeof(Entry)*mCapacity);
							}
							return this;
						}

						//! Makes room for at least count entries without growing again, so a label copied with additions does not rehash.
						//! Parallel saturation: the thread that processes the node first allocates the growth of the hash.
						inline void setProcessContext(CProcessContext* processContext) {
							mMemMan = processContext->getUsedMemoryAllocationManager();
							mFreeArrays = processContext->getSaturationLabelHashFreeArrays();
						}
						inline void reserve(cint64 count) {
							cint64 capacity = mCapacity > 0 ? mCapacity : INITIALCAPACITY;
							while (count * 4 > capacity * 3) {
								capacity *= 2;
							}
							if (capacity != mCapacity) {
								rehash(capacity);
							}
						}


						class const_iterator {
							public:
								inline const_iterator() : mCurrent(nullptr), mEnd(nullptr) {
								}
								inline const_iterator(Entry* current, Entry* end) : mCurrent(current), mEnd(end) {
									skipEmpty();
								}
								inline cint64 key() const {
									return mCurrent->key;
								}
								inline const CConceptSaturationDescriptorReapplyData& value() const {
									return mCurrent->value;
								}
								inline const CConceptSaturationDescriptorReapplyData& operator*() const {
									return mCurrent->value;
								}
								inline const CConceptSaturationDescriptorReapplyData* operator->() const {
									return &mCurrent->value;
								}
								inline bool operator==(const const_iterator& other) const {
									return mCurrent == other.mCurrent;
								}
								inline bool operator!=(const const_iterator& other) const {
									return mCurrent != other.mCurrent;
								}
								inline const_iterator& operator++() {
									if (mCurrent != mEnd) {
										++mCurrent;
										skipEmpty();
									}
									return *this;
								}
								inline const_iterator operator++(int) {
									const_iterator previous = *this;
									++(*this);
									return previous;
								}
							private:
								inline void skipEmpty() {
									while (mCurrent != mEnd && mCurrent->key == EMPTYKEY) {
										++mCurrent;
									}
								}
								Entry* mCurrent;
								Entry* mEnd;
						};

						inline const_iterator constBegin() const {
							return const_iterator(mEntries,mEntries+mCapacity);
						}
						inline const_iterator constEnd() const {
							return const_iterator(mEntries+mCapacity,mEntries+mCapacity);
						}
						inline const_iterator begin() const {
							return constBegin();
						}
						inline const_iterator end() const {
							return constEnd();
						}

					// protected methods
					protected:
						//! The slot of the key, or the free slot where it would be inserted; the array is never full.
						inline Entry* probe(cint64 key) const {
							cint64 index = (cint64)(((quint64)key * Q_UINT64_C(0x9E3779B97F4A7C15)) >> mShift);
							const cint64 mask = mCapacity - 1;
							for (;;) {
								Entry* entry = mEntries + index;
								if (entry->key == key || entry->key == EMPTYKEY) {
									return entry;
								}
								index = (index + 1) & mask;
							}
						}

						inline void grow() {
							rehash(mCapacity > 0 ? mCapacity * 2 : INITIALCAPACITY);
						}

						inline void rehash(cint64 newCapacity) {
							Entry* oldEntries = mEntries;
							cint64 oldCapacity = mCapacity;
							mEntries = allocateArray(newCapacity);
							mCapacity = newCapacity;
							mShift = 64 - log2OfCapacity(newCapacity);
							for (cint64 i = 0; i < newCapacity; ++i) {
								mEntries[i].key = EMPTYKEY;
								mEntries[i].value.mConSatDes = nullptr;
								mEntries[i].value.mImpReapplyConSatDes = nullptr;
							}
							for (cint64 i = 0; i < oldCapacity; ++i) {
								if (oldEntries[i].key != EMPTYKEY) {
									*probe(oldEntries[i].key) = oldEntries[i];
								}
							}
							if (oldEntries) {
								releaseArray(oldEntries,oldCapacity);
							}
						}

						static inline int log2OfCapacity(cint64 capacity) {
							int bits = 0;
							for (; capacity > 1; capacity >>= 1) {
								++bits;
							}
							return bits;
						}

						//! An array for the capacity, from the context's free list if one was outgrown earlier, else from the pools.
						inline Entry* allocateArray(cint64 capacity) {
							if (mFreeArrays) {
								void** freeList = mFreeArrays + log2OfCapacity(capacity);
								if (*freeList) {
									Entry* entries = (Entry*)*freeList;
									*freeList = *(void**)entries;
									return entries;
								}
							}
							return (Entry*)CMemoryAllocator::allocateMemory(mMemMan,sizeof(Entry)*capacity);
						}

						//! Keeps the outgrown array for the next hash that needs this capacity (the pools cannot take it back).
						inline void releaseArray(Entry* entries, cint64 capacity) {
							if (mFreeArrays) {
								void** freeList = mFreeArrays + log2OfCapacity(capacity);
								*(void**)entries = *freeList;
								*freeList = entries;
							} else {
								CMemoryAllocator::releaseMemory(entries,mMemMan);
							}
						}

					// protected variables
					protected:
						Entry* mEntries;
						cint64 mCapacity;
						cint64 mSize;
						int mShift;
						CMemoryAllocationManager* mMemMan;
						void** mFreeArrays;

					// private methods
					private:
						CReapplyConceptSaturationLabelHash(const CReapplyConceptSaturationLabelHash& other);
						CReapplyConceptSaturationLabelHash& operator=(const CReapplyConceptSaturationLabelHash& other);

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTSATURATIONLABELHASH_H
