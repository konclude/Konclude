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

#include "COptimizedKPSetIndividualInstantiatedItemMultiHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetIndividualInstantiatedItemMultiHash::COptimizedKPSetIndividualInstantiatedItemMultiHash(cint64 concurrentMultiSize) {
				mConcurrentMultiSize = concurrentMultiSize;
				mIndividualInstantiatedItemMultiHash = new QHash<cint64, COptimizedKPSetIndividualItem*>*[mConcurrentMultiSize];
				mContainerMultiList = new QList<COptimizedKPSetIndividualItem *>*[mConcurrentMultiSize];
				for (cint64 i = 0; i < mConcurrentMultiSize; ++i) {
					mIndividualInstantiatedItemMultiHash[i] = nullptr;
					mContainerMultiList[i] = nullptr;
				}
				mContainerList = nullptr;
			}

			COptimizedKPSetIndividualInstantiatedItemMultiHash::~COptimizedKPSetIndividualInstantiatedItemMultiHash() {
				for (cint64 i = 0; i < mConcurrentMultiSize; ++i) {
					if (mIndividualInstantiatedItemMultiHash[i]) {
						delete mIndividualInstantiatedItemMultiHash[i];
					}
					if (mContainerMultiList[i]) {
						delete mContainerMultiList[i];
					}
				}
				delete[] mIndividualInstantiatedItemMultiHash;
				delete[] mContainerMultiList;
				if (mContainerList) {
					delete mContainerList;
				}
			}


			QList<COptimizedKPSetIndividualItem*>* COptimizedKPSetIndividualInstantiatedItemMultiHash::getIndividualInstantiatedItemContainerList() {
				if (!mContainerList) {
					mContainerList = new QList<COptimizedKPSetIndividualItem *>();
					for (cint64 i = 0; i < mConcurrentMultiSize; ++i) {
						if (mContainerMultiList[i]) {
							mContainerList->append(*mContainerMultiList[i]);
							delete mContainerMultiList[i];
							mContainerMultiList[i] = nullptr;
						}
					}
				}
				return mContainerList;
			}



			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualInstantiatedItemMultiHash::getIndividualInstantiatedItem(cint64 indiId, bool directCreate, CIndividual* individual) {
				cint64 indiVecPos = indiId % mConcurrentMultiSize;
				COptimizedKPSetIndividualItem* instantiatedItem = nullptr;
				QHash<cint64, COptimizedKPSetIndividualItem*>*& individualInstantiatedItemHash = mIndividualInstantiatedItemMultiHash[indiVecPos];
				QList<COptimizedKPSetIndividualItem*>*& containerList = mContainerMultiList[indiVecPos];

				if (directCreate) {
					if (!individualInstantiatedItemHash) {
						individualInstantiatedItemHash = new QHash<cint64, COptimizedKPSetIndividualItem *>();
					}
					COptimizedKPSetIndividualItem*& instantiatedItemPointer = (*individualInstantiatedItemHash)[indiId];
					if (!instantiatedItemPointer) {
						if (!containerList) {
							containerList = new QList<COptimizedKPSetIndividualItem *>();
						}
						instantiatedItemPointer = new COptimizedKPSetIndividualItem();
						instantiatedItemPointer->initInstantiatedItem(indiId, individual);
						containerList->append(instantiatedItemPointer);
					}
					instantiatedItem = instantiatedItemPointer;
				} else if (individualInstantiatedItemHash) {
					instantiatedItem = individualInstantiatedItemHash->value(indiId);
				}
				return instantiatedItem;
			}


			COptimizedKPSetIndividualInstantiatedItemMultiHash* COptimizedKPSetIndividualInstantiatedItemMultiHash::updateIndividualInstantiatedItem(cint64 indiId, COptimizedKPSetIndividualItem* item) {
				cint64 indiVecPos = indiId % mConcurrentMultiSize;
				QHash<cint64, COptimizedKPSetIndividualItem*>*& individualInstantiatedItemHash = mIndividualInstantiatedItemMultiHash[indiVecPos];
				if (!individualInstantiatedItemHash) {
					individualInstantiatedItemHash = new QHash<cint64, COptimizedKPSetIndividualItem *>();
				}
				individualInstantiatedItemHash->insert(indiId, item);
				return this;
			}


			bool COptimizedKPSetIndividualInstantiatedItemMultiHash::hasIndividualInstantiatedItem(cint64 indiId) {
				cint64 indiVecPos = indiId % mConcurrentMultiSize;
				QHash<cint64, COptimizedKPSetIndividualItem*>*& individualInstantiatedItemHash = mIndividualInstantiatedItemMultiHash[indiVecPos];
				if (!individualInstantiatedItemHash) {
					return false;
				}
				return individualInstantiatedItemHash->contains(indiId);
			}


			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualInstantiatedItemMultiHash::getIndividualInstantiatedItem(cint64 indiId) {
				if (indiId < 0) {
					return nullptr;
				}
				cint64 indiVecPos = indiId % mConcurrentMultiSize;
				QHash<cint64, COptimizedKPSetIndividualItem*>*& individualInstantiatedItemHash = mIndividualInstantiatedItemMultiHash[indiVecPos];
				if (!individualInstantiatedItemHash) {
					return nullptr;
				}
				return individualInstantiatedItemHash->value(indiId);
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
