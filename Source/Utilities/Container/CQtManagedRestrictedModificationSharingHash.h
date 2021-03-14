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
 
#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSHARINGHASH_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSHARINGHASH_H

// Namespace includes
#include "ContainerSettings.h"
#include "CQtManagedRestrictedModificationHash.h"



// Library includes

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
			 *		\class		CQtManagedRestrictedModificationSharingHash
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */


			template <class Key, class T>
			class CQtManagedRestrictedModificationSharingHash {

			public:
				

				CQtManagedRestrictedModificationSharingHash(CContext* context);


				CQtManagedRestrictedModificationSharingHash<Key, T>* init(CContext* context = nullptr);
				CQtManagedRestrictedModificationSharingHash<Key, T>* init(const CQtManagedRestrictedModificationSharingHash<Key, T> &other, CContext* context = nullptr);
				CQtManagedRestrictedModificationSharingHash<Key, T>* init(CQtManagedRestrictedModificationSharingHash<Key, T> *other, CContext* context = nullptr);


				CQtManagedRestrictedModificationSharingHash<Key, T>* initSharing(const CQtManagedRestrictedModificationSharingHash<Key, T> &other);
				
				CQtManagedRestrictedModificationSharingHash<Key, T>& operator=(const CQtManagedRestrictedModificationSharingHash<Key, T> &other);


				int size() const;
				int count() const;
				bool isEmpty() const;
				void clear();
				bool contains(const Key &key) const;
				T &operator[](const Key &key);
				const T operator[](const Key &key) const;
				const T value(const Key &key) const;
				const T value(const Key &key, const T &defaultValue) const;


				CQtManagedRestrictedModificationSharingHash<Key, T>* insert(const Key &key, const T &value);
				CQtManagedRestrictedModificationSharingHash<Key, T>* insertMulti(const Key &key, const T &value);


				class const_iterator {
					bool mCurrentIterator = false;
					typename  CQtManagedRestrictedModificationHash<Key, T>::const_iterator mItCurrentBegin;
					typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator mItCurrentEnd;

					bool mSharingIterator = false;
					typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator mItSharingBegin;
					typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator mItSharingEnd;

					cint64 mShareValidatingCount;
					CQtManagedRestrictedModificationHash<Key, T>* mShareValidatingHash;

				public:
					inline const_iterator() : mCurrentIterator(false), mSharingIterator(false) {
					}
					inline const_iterator(typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator itCurrentBegin, typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator itCurrentEnd) {
						mCurrentIterator = true;
						mItCurrentBegin = itCurrentBegin;
						mItCurrentEnd = itCurrentEnd;
						mSharingIterator = false;
					}

					inline const_iterator(typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator itCurrentBegin, typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator itCurrentEnd,
											typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator itSharingBegin, typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator itSharingEnd,
											CQtManagedRestrictedModificationHash<Key, T>* validatingHash = nullptr, cint64 validatingCount = 0) {
						mCurrentIterator = true;
						mItCurrentBegin = itCurrentBegin;
						mItCurrentEnd = itCurrentEnd;
						mSharingIterator = true;
						mItSharingBegin = itSharingBegin;
						mItSharingEnd = itSharingEnd;
						mShareValidatingHash = validatingHash;
						mShareValidatingCount = validatingCount;

						while (mShareValidatingHash && mShareValidatingCount > 0 && mItSharingBegin != mItSharingEnd && mShareValidatingHash->contains(mItSharingBegin.key())) {		
							++mItSharingBegin;
							--mShareValidatingCount;
						}
					}

					inline const Key &key() const {
						if (mCurrentIterator && mItCurrentBegin != mItCurrentEnd) {
							return mItCurrentBegin.key();
						}
						if (mSharingIterator && mItSharingBegin != mItSharingEnd) {
							return mItSharingBegin.key();
						}
						return mItCurrentBegin.key();
					}

					inline const T &value() const {
						if (mCurrentIterator && mItCurrentBegin != mItCurrentEnd) {
							return mItCurrentBegin.value();
						}
						if (mSharingIterator && mItSharingBegin != mItSharingEnd) {
							return mItSharingBegin.value();
						}
						return mItCurrentBegin.value();
					}

					inline const T &operator*() const {
						return value();
					}

					inline const T *operator->() const {
						return value();
					}

					inline bool operator==(const const_iterator &o) const {
						if (mCurrentIterator != o.mCurrentIterator) {
							return false;
						}
						if (mCurrentIterator && mItCurrentBegin != o.mItCurrentBegin) {
							return false;
						}
						if (mCurrentIterator && mItCurrentEnd != o.mItCurrentEnd) {
							return false;
						}
						if (mSharingIterator != o.mSharingIterator) {
							return false;
						}
						if (mSharingIterator && mItSharingBegin != o.mItSharingBegin) {
							return false;
						}
						if (mSharingIterator && mItSharingEnd != o.mItSharingEnd) {
							return false;
						}
						return true;
					}

					inline bool operator!=(const const_iterator &o) const {
						return !this->operator==(o);
					}

					inline const_iterator &operator++() {
						if (mCurrentIterator && mItCurrentBegin != mItCurrentEnd) {
							++mItCurrentBegin;
						} else if (mSharingIterator && mItSharingBegin != mItSharingEnd) {
							++mItSharingBegin;
							while (mShareValidatingHash && mShareValidatingCount > 0 && mItSharingBegin != mItSharingEnd && mShareValidatingHash->contains(mItSharingBegin.key())) {
								++mItSharingBegin;
								--mShareValidatingCount;
							}
						}
						return *this;
					}

					inline const_iterator operator++(int) {
						const_iterator r = *this;
						this->operator++();
						return r;
					}


				private:
					inline operator bool() const { return false; }
				};
				typedef const_iterator ConstIterator;


				inline const_iterator constBegin() const { 
					if (mSharingHash) {
						return const_iterator(mCurrentHash->constBegin(), mCurrentHash->constEnd(), mSharingHash->constBegin(), mSharingHash->constEnd(), mCurrentHash, mSharingDuplicatedCount);
					}
					return const_iterator(mCurrentHash->constBegin(), mCurrentHash->constEnd());
				}
				inline const_iterator constEnd() const { 
					if (mSharingHash) {
						return const_iterator(mCurrentHash->constEnd(), mCurrentHash->constEnd(), mSharingHash->constEnd(), mSharingHash->constEnd());
					}
					return const_iterator(mCurrentHash->constEnd(), mCurrentHash->constEnd());
				}


				void unshare(const Key & key);
				void unshareAll();

				protected:
					CQtManagedRestrictedModificationHash<Key, T>* mCurrentHash;
					CQtManagedRestrictedModificationHash<Key, T>* mSharingHash;
					cint64 mSharingDuplicatedCount;
					CContext* mContext;
					CMemoryAllocationManager* mMemMan;
				
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CQtManagedRestrictedModificationSharingHash.cpp"


#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSHARINGHASH_H

