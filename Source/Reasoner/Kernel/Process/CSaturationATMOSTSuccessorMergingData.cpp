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

#include "CSaturationATMOSTSuccessorMergingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationATMOSTSuccessorMergingData::CSaturationATMOSTSuccessorMergingData(CProcessContext* processContext) : mProcessContext(processContext) {
				}

				CSaturationATMOSTSuccessorMergingData* CSaturationATMOSTSuccessorMergingData::initSuccessorMergingData(CIndividualSaturationProcessNode* indiProcessNode) {
					mNewSuccessorHash = nullptr;
					mMergingIndiProcessLinker.initProcessNodeLinker(indiProcessNode,false);

					mConceptMergingDataHash = nullptr;
					mMergingConceptLinker = nullptr;
					mRemainMergeableCardHash = nullptr;
					mMergeDistintHash = nullptr;
					mMergeDistintSet = nullptr;
					return this;
				}


				CIndividualSaturationProcessNodeLinker* CSaturationATMOSTSuccessorMergingData::getMergingIndividualProcessLinker() {
					return &mMergingIndiProcessLinker;
				}

				bool CSaturationATMOSTSuccessorMergingData::isMergingProcessingQueued() {
					return mMergingIndiProcessLinker.isProcessingQueued();
				}








				CSaturationATMOSTSuccessorMergingHash* CSaturationATMOSTSuccessorMergingData::getATMOSTConceptMergingDataHash(bool create) {
					if (!mConceptMergingDataHash && create) {
						mConceptMergingDataHash = CObjectParameterizingAllocator< CSaturationATMOSTSuccessorMergingHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mConceptMergingDataHash->initATMOSTConceptDescriptorMergingHash();
					}
					return mConceptMergingDataHash;
				}

				CConceptSaturationProcessLinker* CSaturationATMOSTSuccessorMergingData::getMergingConceptLinker() {
					return mMergingConceptLinker;
				}


				CSaturationATMOSTSuccessorMergingData* CSaturationATMOSTSuccessorMergingData::setMergingConceptLinker(CConceptSaturationProcessLinker* conceptLinker) {
					mMergingConceptLinker = conceptLinker;
					return this;
				}

				CSaturationATMOSTSuccessorMergingData* CSaturationATMOSTSuccessorMergingData::addMergingProcessingConcept(CConceptSaturationDescriptor* conSatDes) {
					CConceptSaturationProcessLinker* conSatProLinker = CObjectAllocator< CConceptSaturationProcessLinker >::allocateAndConstruct(mProcessContext->getUsedMemoryAllocationManager());
					conSatProLinker->initConceptSaturationProcessLinker(conSatDes);
					mMergingConceptLinker = conSatProLinker->append(mMergingConceptLinker);
					return this;
				}



				CLinkedRoleSaturationSuccessorHash* CSaturationATMOSTSuccessorMergingData::getMergedLinkedRoleSaturationSuccessorHash(bool create) {
					if (!mNewSuccessorHash && create) {
						mNewSuccessorHash = CObjectParameterizingAllocator< CLinkedRoleSaturationSuccessorHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mNewSuccessorHash->initRoleSuccessorHash();
					}
					return mNewSuccessorHash;
				}


				CConceptSaturationProcessLinker* CSaturationATMOSTSuccessorMergingData::takeNextMergingConceptLinker() {
					if (mMergingConceptLinker) {
						mMergingConceptLinker = mMergingConceptLinker->getNext();
					}
					return mMergingConceptLinker;
				}

				CSaturationATMOSTSuccessorMergingHashData& CSaturationATMOSTSuccessorMergingData::getATMOSTConceptMergingData(CConceptSaturationDescriptor* conSatDes) {
					if (!mConceptMergingDataHash) {
						getATMOSTConceptMergingDataHash(true);
					}
					return mConceptMergingDataHash->getATMOSTConceptMergingData(conSatDes);
				}

				CPROCESSHASH<CSaturationSuccessorData*,cint64>* CSaturationATMOSTSuccessorMergingData::getRemainingMergeableCardinalityHash(bool create) {
					if (!mRemainMergeableCardHash && create) {
						mRemainMergeableCardHash = CObjectParameterizingAllocator< CPROCESSHASH<CSaturationSuccessorData*,cint64>,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					}
					return mRemainMergeableCardHash;
				}

				CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>* CSaturationATMOSTSuccessorMergingData::getMergingDistintHash(bool create) {
					if (!mMergeDistintHash && create) {
						mMergeDistintHash = CObjectParameterizingAllocator< CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					}
					return mMergeDistintHash;
				}

				CPROCESSSET< QPair<CSaturationSuccessorData*,CSaturationSuccessorData*> >* CSaturationATMOSTSuccessorMergingData::getMergingDistintSet(bool create) {
					if (!mMergeDistintSet && create) {
						mMergeDistintSet = CObjectParameterizingAllocator< CPROCESSSET< QPair<CSaturationSuccessorData*,CSaturationSuccessorData*> >,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					}
					return mMergeDistintSet;
				}





			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
