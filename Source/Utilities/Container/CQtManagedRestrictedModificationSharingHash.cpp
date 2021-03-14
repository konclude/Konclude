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
 
#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSHARINGHASH_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSHARINGHASH_CPP


#include "CQtManagedRestrictedModificationSharingHash.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {




			template <class Key, class T>
			CQtManagedRestrictedModificationSharingHash<Key, T>::CQtManagedRestrictedModificationSharingHash(CContext* context) : mContext(context) {
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				mCurrentHash = CObjectParameterizingAllocator< CQtManagedRestrictedModificationHash<Key, T>, CContext* >::allocateAndConstructAndParameterize(mMemMan, mContext);
				mSharingHash = nullptr;
				mSharingDuplicatedCount = 0;
			}

			template <class Key, class T>
			CQtManagedRestrictedModificationSharingHash<Key, T>* CQtManagedRestrictedModificationSharingHash<Key, T>::init(CContext* context) {
				mContext = context;
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				mCurrentHash->init(mContext);
				mSharingHash = nullptr;
				mSharingDuplicatedCount = 0;
				return this;
			}

			template <class Key, class T>
			CQtManagedRestrictedModificationSharingHash<Key, T>* CQtManagedRestrictedModificationSharingHash<Key, T>::init(const CQtManagedRestrictedModificationSharingHash<Key, T> &other, CContext* context) {
				mContext = context;
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				initSharing(other);
				return this;
			}

			template <class Key, class T>
			CQtManagedRestrictedModificationSharingHash<Key, T>* CQtManagedRestrictedModificationSharingHash<Key, T>::init(CQtManagedRestrictedModificationSharingHash<Key, T> *other, CContext* context) {
				mContext = context;
				mMemMan = CContext::getMemoryAllocationManager(mContext);
				if (other) {
					initSharing(*other);
				} else {
					mCurrentHash->clear();
					mSharingHash = nullptr;
					mSharingDuplicatedCount = 0;
				}
				return this;
			}



			template <class Key, class T>
			CQtManagedRestrictedModificationSharingHash<Key, T>& CQtManagedRestrictedModificationSharingHash<Key, T>::operator=(const CQtManagedRestrictedModificationSharingHash<Key, T> &other) {
				initSharing(other);
				return *this;
			}


			template <class Key, class T>
			CQtManagedRestrictedModificationSharingHash<Key, T>* CQtManagedRestrictedModificationSharingHash<Key, T>::initSharing(const CQtManagedRestrictedModificationSharingHash<Key, T> &other) {

				mCurrentHash->clear();
				mSharingHash = nullptr;
				mSharingDuplicatedCount = 0;

				if (!other.mSharingHash && other.mCurrentHash->size() <= 100) {
					*mCurrentHash = *other.mCurrentHash;
					mSharingHash = nullptr;
				} else if (!other.mSharingHash && other.mCurrentHash->size() > 100) {
					mCurrentHash->clear();
					mSharingHash = other.mCurrentHash;
				} else {

					if (other.mCurrentHash->size() * 3 > other.mSharingHash->size()) {
						*mCurrentHash = *other.mCurrentHash;
						cint64 remainShareCount = other.mSharingDuplicatedCount;
						for (typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator it = other.mSharingHash->constBegin(), itEnd = other.mSharingHash->constEnd(); it != itEnd;) {
							if (remainShareCount <= 0 || !other.mCurrentHash->contains(it.key())) {
								typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator itBase = it;
								while (it != itEnd && it.key() == itBase.key()) {
									mCurrentHash->insertMulti(it.key(), it.value());
									++it;
								}
							} else {
								remainShareCount--;
								++it;
							}
						}

						mSharingHash = nullptr;
					} else {
						*mCurrentHash = *other.mCurrentHash;
						mSharingHash = other.mSharingHash;
					}

				}

				return this;
			}

			template <class Key, class T>
			void CQtManagedRestrictedModificationSharingHash<Key, T>::unshareAll() {
				if (mSharingHash) {
					cint64 remainShareCount = mSharingDuplicatedCount;
					for (typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator it = mSharingHash->constBegin(), itEnd = mSharingHash->constEnd(); it != itEnd;) {
						if (remainShareCount <= 0 || !mCurrentHash->contains(it.key())) {
							typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator itBase = it;
							while (it != itEnd && it.key() == itBase.key()) {
								mCurrentHash->insertMulti(it.key(), it.value());
								++it;
							}
						} else  {
							remainShareCount--;
							++it;
						}
					}
					mSharingDuplicatedCount = 0;
					mSharingHash = nullptr;
				}
			}

			template <class Key, class T>
			int CQtManagedRestrictedModificationSharingHash<Key, T>::size() const {
				cint64 count = mCurrentHash->count();
				if (mSharingHash) {
					count += mSharingHash->count();
					count -= mSharingDuplicatedCount;
				}
				return count;
			}

			template <class Key, class T>
			bool CQtManagedRestrictedModificationSharingHash<Key, T>::isEmpty() const {
				return size() == 0;
			}

			template <class Key, class T>
			int CQtManagedRestrictedModificationSharingHash<Key, T>::count() const {
				return size();
			}

			template <class Key, class T>
			void CQtManagedRestrictedModificationSharingHash<Key, T>::clear() {
				mCurrentHash->clear();
				mSharingHash = nullptr;
				mSharingDuplicatedCount = 0;
			}

			template <class Key, class T>
			bool CQtManagedRestrictedModificationSharingHash<Key, T>::contains(const Key &key) const {
				if (mCurrentHash->contains(key)) {
					return true;
				}
				if (mSharingHash && mSharingHash->contains(key)) {
					return true;
				}
				return false;
			}

			template <class Key, class T>
			T& CQtManagedRestrictedModificationSharingHash<Key, T>::operator[](const Key &key) {
				unshare(key);
				return (*mCurrentHash)[key];
			}


			template <class Key, class T>
			const T CQtManagedRestrictedModificationSharingHash<Key, T>::operator[](const Key &key) const {
				return value(key);
			}

			template <class Key, class T>
			const T CQtManagedRestrictedModificationSharingHash<Key, T>::value(const Key &key) const {
				if (mCurrentHash->contains(key)) {
					return mCurrentHash->value(key);
				}
				if (mSharingHash) {
					return mSharingHash->value(key);
				}
				return mCurrentHash->value(key);
			}

			template <class Key, class T>
			const T CQtManagedRestrictedModificationSharingHash<Key, T>::value(const Key &key, const T &defaultValue) const {
				if (mCurrentHash->contains(key)) {
					return mCurrentHash->value(key);
				}
				if (mSharingHash && mSharingHash->contains(key)) {
					return mSharingHash->value(key);
				}
				return defaultValue;
			}

			template <class Key, class T>
			CQtManagedRestrictedModificationSharingHash<Key, T>* CQtManagedRestrictedModificationSharingHash<Key, T>::insert(const Key &key, const T &value) {
				unshare(key);
				mCurrentHash->insert(key, value);
				return this;
			}

			template <class Key, class T>
			CQtManagedRestrictedModificationSharingHash<Key, T>* CQtManagedRestrictedModificationSharingHash<Key, T>::insertMulti(const Key &key, const T &value) {
				unshare(key);
				mCurrentHash->insertMulti(key, value);
				return this;
			}

			template <class Key, class T>
			void CQtManagedRestrictedModificationSharingHash<Key, T>::unshare(const Key & key) {
				if (mSharingHash && mSharingHash->contains(key) && !mCurrentHash->contains(key)) {
					for (typename CQtManagedRestrictedModificationHash<Key, T>::const_iterator it = mSharingHash->find(key), itEnd = mSharingHash->constEnd(); it != itEnd && it.key() == key; ++it) {
						mCurrentHash->insertMulti(key, it.value());
						mSharingDuplicatedCount++;
					}
				}
			}





		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONSHARINGHASH_CPP

