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
 */

#include "CCentralizedAllocationConfigProvidedDependendLimitation.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CCentralizedAllocationConfigProvidedDependendLimitation::CCentralizedAllocationConfigProvidedDependendLimitation(CConfigurationProvider* configProvider, const QString& configString) {
				mConfigString = configString;
				mConfigProvider = configProvider;
				mMaxMemAllocSize = 0;
				mCurrMemAllocSize = 0;
				mAddMemAllocSize = 1024*1024*512;
				mCriticalMemAllocSize = 1024*1024*1024;
				mMemAllocLimited = false;
				mLimitatorLinker = nullptr;
				mLastConfig = nullptr;
				mStatMaxAllocatedMemSize = 0;
			}

			CCentralizedAllocationConfigProvidedDependendLimitation::~CCentralizedAllocationConfigProvidedDependendLimitation() {
			}


			CCentralizedAllocationLimitator* CCentralizedAllocationConfigProvidedDependendLimitation::getLimitator() {
				QMutexLocker locker(&mSyncMutex);
				CCentralizedAllocationLimitator* limitator = new CCentralizedAllocationLimitator(this);
				mLimitatorLinker = limitator->append(mLimitatorLinker);
				return limitator;
			}


			cint64 CCentralizedAllocationConfigProvidedDependendLimitation::getAllocatedMemorySize() {
				cint64 allocatedMemSize = 0;
				CCentralizedAllocationLimitator* limIt = mLimitatorLinker;
				while (limIt) {
					allocatedMemSize += limIt->getMemoryAllocatedSize();
					limIt = limIt->getNext();
				}
				return allocatedMemSize;
			}


			cint64 CCentralizedAllocationConfigProvidedDependendLimitation::getAllocatedMemorySizeMaximum() {
				mStatMaxAllocatedMemSize = qMax(mStatMaxAllocatedMemSize,getAllocatedMemorySize());
				return mStatMaxAllocatedMemSize;
			}

			cint64 CCentralizedAllocationConfigProvidedDependendLimitation::getReservedMemorySizeMaximum() {
				return mCurrMemAllocSize;
			}

			bool CCentralizedAllocationConfigProvidedDependendLimitation::updateMaxMemoryAllocationSize() {
				CConfiguration* currConfig = mConfigProvider->getCurrentConfiguration();
				if (mLastConfig != currConfig) {
					QMutexLocker locker(&mSyncMutex);
					if (mLastConfig != currConfig) {
						mLastConfig = currConfig;
						mAddMemAllocSize = CConfigDataReader::readConfigInteger(currConfig,mConfigString+".IncreaseAllocationSize",1024*1024*25 /*25 MByte*/);


						bool configReadError = false;
						bool limited = CConfigDataReader::readConfigBoolean(currConfig,mConfigString+".AllocationLimitation",false,&configReadError);
						if (!configReadError && limited) {
							configReadError = false;
							mCriticalMemAllocSize = CConfigDataReader::readConfigInteger(currConfig,mConfigString+".CriticalAllocationSize",1024*1024*1024 /*1 GByte*/);
							cint64 configMaxMemAllocSize = CConfigDataReader::readConfigInteger(currConfig,mConfigString+".MaximumAllocationSize",0,&configReadError);
							if (configReadError) {
								mMemAllocLimited = false;
							} else {
								mMemAllocLimited = true;
								mMaxMemAllocSize = configMaxMemAllocSize;
								return true;
							}
						} else {
							mMemAllocLimited = false;
						}
					}
				}

				return false;
			}

			

			bool CCentralizedAllocationConfigProvidedDependendLimitation::requestLimitatorMemoryAllocationSizeUpdate() {
				CCentralizedAllocationLimitator* limIt = mLimitatorLinker;
				while (limIt) {
					if (limIt->getRemainingMemoryAllocationSize() > 2*mAddMemAllocSize) {
						limIt->notifyRequireUpdateLimit();
					}
					limIt = limIt->getNext();
				}
				return true;
			}

			
			cint64 CCentralizedAllocationConfigProvidedDependendLimitation::getUpdatedMemoryAllocationLimit(CAllocationLimitator* limitator) {
				mStatMaxAllocatedMemSize = qMax(mStatMaxAllocatedMemSize,getAllocatedMemorySize());
				bool updated = updateMaxMemoryAllocationSize();
				cint64 remSize = limitator->getRemainingMemoryAllocationSize();
				if (remSize < mAddMemAllocSize) {
					// add some memory
					QMutexLocker locker(&mSyncMutex);
					if (!mMemAllocLimited) {
						mCurrMemAllocSize += mAddMemAllocSize;
						return limitator->getMaximumMemoryAllocationSize() + mAddMemAllocSize;
					} else {
						cint64 tmpAlloc = mMaxMemAllocSize-mCurrMemAllocSize;
						tmpAlloc = qMin(tmpAlloc,mAddMemAllocSize);
						tmpAlloc = qMax(tmpAlloc,(cint64)0);
						mCurrMemAllocSize += tmpAlloc;

						if (mCurrMemAllocSize > mCriticalMemAllocSize || tmpAlloc != mAddMemAllocSize) {
							requestLimitatorMemoryAllocationSizeUpdate();
						}

						return limitator->getMaximumMemoryAllocationSize() + tmpAlloc;
					}
				} else if (remSize > 2*mAddMemAllocSize) {
					// get some memory
					QMutexLocker locker(&mSyncMutex);
					cint64 tmpAllocGet = remSize-mAddMemAllocSize;
					mCurrMemAllocSize -= tmpAllocGet;
					return limitator->getMaximumMemoryAllocationSize() - tmpAllocGet;
				} 
				return limitator->getMaximumMemoryAllocationSize();
			}


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

