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

#include "CActiveEntityCountVector.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CActiveEntityCountVector::CActiveEntityCountVector(CContext* context) {
				mContext = context;
				mCountVector = nullptr;
				mVectorSize = 0;
			}


			CActiveEntityCountVector* CActiveEntityCountVector::initActiveEntityCountVector(CActiveEntityCountVector* vector) {
				mVectorSize = 0;
				mCountVector = nullptr;
				if (vector) {
					increaseVectorSize(vector->getVectorSize());
					for (cint64 i = 0; i < vector->mVectorSize; ++i) {
						mCountVector[i] = vector->mCountVector[i];
					}
				}
				return this;
			}


			CActiveEntityCountVector* CActiveEntityCountVector::incActiveEntityCount(CExpressionEntity* entity, cint64 incCount) {
				CActiveEntityCountItem* item = getActiveEntityCountItem(entity);
				if (item) {
					item->incActiveCount(incCount);
				}
				return this;
			}

			CActiveEntityCountVector* CActiveEntityCountVector::decActiveEntityCount(CExpressionEntity* entity, cint64 decCount) {
				CActiveEntityCountItem* item = getActiveEntityCountItem(entity);
				if (item) {
					item->decActiveCount(decCount);
				}
				return this;
			}

			CActiveEntityCountItem* CActiveEntityCountVector::getActiveEntityCountItem(CExpressionEntity* entity) {
				if (entity) {
					cint64 itemID = entity->getEntityID();
					if (itemID < mVectorSize) {
						CActiveEntityCountItem* item = &mCountVector[itemID];
						item->setEntity(entity);
						return item;
					}
				}
				return nullptr;
			}

			bool CActiveEntityCountVector::increaseVectorSize(cint64 newSize) {
				if (newSize > mVectorSize) {
					CActiveEntityCountItem* tmpCountVector = CObjectAllocator<CActiveEntityCountItem>::allocateAndConstructArray(mContext->getMemoryAllocationManager(),newSize);
					if (mCountVector) {
						for (cint64 i = 0; i < mVectorSize; ++i) {
							tmpCountVector[i] = mCountVector[i];
						}
						CObjectAllocator<CActiveEntityCountItem>::destroyAndReleaseArray(mCountVector,mContext->getMemoryAllocationManager());
					}
					mCountVector = tmpCountVector;
					mVectorSize = newSize;
					return true;
				}
				return false;
			}

			cint64 CActiveEntityCountVector::getVectorSize() {
				return mVectorSize;
			}

			CActiveEntityCountVector* CActiveEntityCountVector::clearActiveEntityCount() {
				for (cint64 i = 0; i < mVectorSize; ++i) {
					mCountVector[i].setActiveCount(0);
				}
				return this;
			}


			CActiveEntityIterator CActiveEntityCountVector::getActiveEntityIterator() {
				return CActiveEntityIterator(mCountVector,mVectorSize);
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
