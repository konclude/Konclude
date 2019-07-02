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

#include "CSuccessorIndividualATMOSTReactivationData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSuccessorIndividualATMOSTReactivationData::CSuccessorIndividualATMOSTReactivationData(CProcessContext* processContext) {
					mProcessContext = processContext;
				}

				CSuccessorIndividualATMOSTReactivationData* CSuccessorIndividualATMOSTReactivationData::initSuccessorIndividualATMOSTReactivationData(CSuccessorIndividualATMOSTReactivationData* data) {
					mDirectReactivationConDes = nullptr;
					mLastDirectIndiReactivationLinker = nullptr;
					mReactivationConDesIndiLinkerHash = nullptr;
					if (data) {
						if (data->mReactivationConDesIndiLinkerHash) {
							mReactivationConDesIndiLinkerHash = CObjectParameterizingAllocator< CPROCESSHASH< CConceptDescriptor*,CXLinker<CIndividualLinkEdge*>* >,CContext* >::allocateAndConstructAndParameterize(mProcessContext->getMemoryAllocationManager(),mProcessContext);
							*mReactivationConDesIndiLinkerHash = *data->mReactivationConDesIndiLinkerHash;
						}
						mLastDirectIndiReactivationLinker = data->mLastDirectIndiReactivationLinker;
						mDirectReactivationConDes = data->mDirectReactivationConDes;
					}
					return this;
				}




				CXLinker<CIndividualLinkEdge*>* CSuccessorIndividualATMOSTReactivationData::getReactivationSuccessorIndividualLinkLinker(CConceptDescriptor* atmostConDes) {
					if (mDirectReactivationConDes == atmostConDes) {
						return mLastDirectIndiReactivationLinker;
					}
					if (mReactivationConDesIndiLinkerHash) {
						return mReactivationConDesIndiLinkerHash->operator[](atmostConDes);
					}
					return nullptr;
				}

				bool CSuccessorIndividualATMOSTReactivationData::hasNewReactivationSuccessorIndividualLinkTriggered(CConceptDescriptor* atmostConDes, CXLinker<CIndividualLinkEdge*>* lastDirectIndiReactivationLinker) {
					return getReactivationSuccessorIndividualLinkLinker(atmostConDes) == lastDirectIndiReactivationLinker;
				}


				CSuccessorIndividualATMOSTReactivationData* CSuccessorIndividualATMOSTReactivationData::addReactivationSuccessorIIndividualLinkLinker(CConceptDescriptor* atmostConDes, CXLinker<CIndividualLinkEdge*>* indiProcNodeLinker) {
					if (!mDirectReactivationConDes || mDirectReactivationConDes == atmostConDes) {
						mLastDirectIndiReactivationLinker = indiProcNodeLinker->append(mLastDirectIndiReactivationLinker);
						mDirectReactivationConDes = atmostConDes;
					} else {
						if (!mReactivationConDesIndiLinkerHash) {
							mReactivationConDesIndiLinkerHash = CObjectParameterizingAllocator< CPROCESSHASH< CConceptDescriptor*,CXLinker<CIndividualLinkEdge*>* >,CContext* >::allocateAndConstructAndParameterize(mProcessContext->getMemoryAllocationManager(),mProcessContext);
						}
						CXLinker<CIndividualLinkEdge*>*& prevIndiLinker = mReactivationConDesIndiLinkerHash->operator[](atmostConDes);
						prevIndiLinker = indiProcNodeLinker->append(prevIndiLinker);
					}
					return this;
				}

				CSuccessorIndividualATMOSTReactivationData* CSuccessorIndividualATMOSTReactivationData::addReactivationSuccessorIndividualLink(CConceptDescriptor* atmostConDes, CIndividualLinkEdge* indiProcNode) {
					CXLinker<CIndividualLinkEdge*>* indiLinker = CObjectAllocator< CXLinker<CIndividualLinkEdge*> >::allocateAndConstruct(mProcessContext->getMemoryAllocationManager());
					indiLinker->initLinker(indiProcNode);
					return addReactivationSuccessorIIndividualLinkLinker(atmostConDes,indiLinker);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
