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
