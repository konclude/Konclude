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
 */

#include "COptimizedKPSetRoleNeighbourInstancesHashData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetRoleNeighbourInstancesHashData::COptimizedKPSetRoleNeighbourInstancesHashData() {
				mRoleSuccessorInstancesHash = nullptr;
				mRolePredecessorInstancesHash = nullptr;

				mRoleSuccessorInstancesInitialized = false;
				mRolePredecessorInstancesInitialized = false;

				mRoleSuccessorInstancesInitializing = false;
				mRolePredecessorInstancesInitializing = false;
			}


			COptimizedKPSetRoleInstancesHash* COptimizedKPSetRoleNeighbourInstancesHashData::getRoleSuccessorInstancesHash(bool forceCreated) {
				if (forceCreated && !mRoleSuccessorInstancesHash) {
					mRoleSuccessorInstancesHash = new COptimizedKPSetRoleInstancesHash();
				}
				return mRoleSuccessorInstancesHash;
			}

			COptimizedKPSetRoleInstancesHash* COptimizedKPSetRoleNeighbourInstancesHashData::getRolePredecessorInstancesHash(bool forceCreated) {
				if (forceCreated && !mRolePredecessorInstancesHash) {
					mRolePredecessorInstancesHash = new COptimizedKPSetRoleInstancesHash();
				}
				return mRolePredecessorInstancesHash;
			}


			COptimizedKPSetRoleInstancesHash* COptimizedKPSetRoleNeighbourInstancesHashData::getRoleNeighbourInstancesHash(bool inversed, bool forceCreated) {
				if (inversed) {
					return getRolePredecessorInstancesHash(forceCreated);
				} else {
					return getRoleSuccessorInstancesHash(forceCreated);
				}
			}


			COptimizedKPSetRoleInstancesData* COptimizedKPSetRoleNeighbourInstancesHashData::getRoleSuccessorInstanceItemData(COptimizedKPSetIndividualItem* item) {
				if (mRoleSuccessorInstancesHash) {
					return mRoleSuccessorInstancesHash->getInstanceItemData(item);
				}
				return nullptr;
			}

			COptimizedKPSetRoleInstancesData* COptimizedKPSetRoleNeighbourInstancesHashData::getRolePredecessorInstanceItemData(COptimizedKPSetIndividualItem* item) {
				if (mRolePredecessorInstancesHash) {
					return mRolePredecessorInstancesHash->getInstanceItemData(item);
				}
				return nullptr;
			}


			COptimizedKPSetRoleInstancesData* COptimizedKPSetRoleNeighbourInstancesHashData::getRoleSuccessorInstanceItemData(cint64 indiId, bool forceCreated) {
				if (getRoleSuccessorInstancesHash(forceCreated)) {
					return mRoleSuccessorInstancesHash->getInstanceItemData(indiId, forceCreated);
				}
				return nullptr;
			}

			COptimizedKPSetRoleInstancesData* COptimizedKPSetRoleNeighbourInstancesHashData::getRolePredecessorInstanceItemData(cint64 indiId, bool forceCreated) {
				if (getRolePredecessorInstancesHash(forceCreated)) {
					return mRolePredecessorInstancesHash->getInstanceItemData(indiId, forceCreated);
				}
				return nullptr;
			}


			COptimizedKPSetRoleInstancesData* COptimizedKPSetRoleNeighbourInstancesHashData::getRoleNeighbourInstanceItemData(bool inversed, cint64 indiId, bool forceCreated) {
				if (!inversed) {
					return getRoleSuccessorInstanceItemData(indiId, forceCreated);
				} else {
					return getRolePredecessorInstanceItemData(indiId, forceCreated);
				}
			}

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
