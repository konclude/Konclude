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

#include "CNominalCachingLossReactivationData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CNominalCachingLossReactivationData::CNominalCachingLossReactivationData(CProcessContext* processContext) {
					mProcessContext = processContext;
				}

				CNominalCachingLossReactivationData* CNominalCachingLossReactivationData::initNominalCachingLossReactivationData(cint64 nominalID, CNominalCachingLossReactivationData* data) {
					mReactivated = false;
					mNominalID = nominalID;
					mIndiReactivationLinker = nullptr;
					if (data) {
						mReactivated = data->mReactivated;
						mIndiReactivationLinker = data->mIndiReactivationLinker;
					}
					return this;
				}


				bool CNominalCachingLossReactivationData::getNominalID() {
					return mNominalID;
				}

				bool CNominalCachingLossReactivationData::hasReactivated() {
					return mReactivated;
				}

				CNominalCachingLossReactivationData* CNominalCachingLossReactivationData::setReactivated(bool lostCached) {
					mReactivated = lostCached;
					return this;
				}

				CXLinker<CIndividualProcessNode*>* CNominalCachingLossReactivationData::getReactivationIndividualNodeLinker() {
					return mIndiReactivationLinker;
				}

				CXLinker<CIndividualProcessNode*>* CNominalCachingLossReactivationData::takeReactivationIndividualNodeLinker() {
					CXLinker<CIndividualProcessNode*>* tmpIndiProcNodeLinker = mIndiReactivationLinker;
					mIndiReactivationLinker = nullptr;
					return tmpIndiProcNodeLinker;
				}


				CNominalCachingLossReactivationData* CNominalCachingLossReactivationData::addReactivationIndividualNodeLinker(CXLinker<CIndividualProcessNode*>* indiProcNodeLinker) {
					if (indiProcNodeLinker) {
						mIndiReactivationLinker = indiProcNodeLinker->append(mIndiReactivationLinker);
					}
					return this;
				}

				CNominalCachingLossReactivationData* CNominalCachingLossReactivationData::addReactivationIndividualNode(CIndividualProcessNode* indiProcNode) {
					CXLinker<CIndividualProcessNode*>* indiProcNodeLinker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(mProcessContext->getUsedMemoryAllocationManager());
					indiProcNodeLinker->initLinker(indiProcNode);
					addReactivationIndividualNodeLinker(indiProcNodeLinker);
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
