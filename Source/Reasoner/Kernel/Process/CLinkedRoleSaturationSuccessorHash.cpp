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

#include "CLinkedRoleSaturationSuccessorHash.h"
#include "CIndividualSaturationProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CLinkedRoleSaturationSuccessorHash::CLinkedRoleSaturationSuccessorHash(CProcessContext* context) : mContext(context),mRoleSuccDataHash(context) {
				}


				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::initRoleSuccessorHash() {
					mLastExaminedConDes = nullptr;
					mLastExaminedRoleAssLinker = nullptr;
					mRoleSuccDataHash.clear();
					return this;
				}

				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::copyRoleSuccessorHash(CLinkedRoleSaturationSuccessorHash* copyRoleSuccHash) {
					mRoleSuccDataHash = copyRoleSuccHash->mRoleSuccDataHash;
					return this;
				}


				bool CLinkedRoleSaturationSuccessorHash::hasActiveLinkedSuccessor(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole, cint64 linkCount) {
					CLinkedRoleSaturationSuccessorData** roleSuccData = mRoleSuccDataHash.valuePointer(role);
					if (roleSuccData && *roleSuccData) {
						CSaturationSuccessorData** succData = nullptr;
						if ((*roleSuccData)->mSuccNodeDataMap.tryGetValuePointer(linkedIndi->getIndividualID(),succData)) {
							if (succData && (*succData)->mActiveCount >= 1 && (*succData)->mSuccCount >= linkCount) {
								if (!creationRole) {
									return true;
								}
								for (CXNegLinker<CRole*>* creationRoleIt = (*succData)->mCreationRoleLinker; creationRoleIt; creationRoleIt = creationRoleIt->getNext()) {
									if (!creationRoleIt->isNegated() && creationRole == creationRoleIt->getData()) {
										return true;
									}
								}
							}
						}
					}
					return false;
				} 

				bool CLinkedRoleSaturationSuccessorHash::hasLinkedSuccessor(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole, cint64 linkCount) {
					CLinkedRoleSaturationSuccessorData** roleSuccData = mRoleSuccDataHash.valuePointer(role);
					if (roleSuccData && *roleSuccData) {
						CSaturationSuccessorData** succData = nullptr;
						if ((*roleSuccData)->mSuccNodeDataMap.tryGetValuePointer(linkedIndi->getIndividualID(),succData)) {
							if (succData && (*succData)->mSuccCount >= linkCount) {
								if (!creationRole) {
									return true;
								}
								for (CXNegLinker<CRole*>* creationRoleIt = (*succData)->mCreationRoleLinker; creationRoleIt; creationRoleIt = creationRoleIt->getNext()) {
									if (creationRole == creationRoleIt->getData()) {
										return true;
									}
								}
							}
						}
					}
					return false;
				} 


				CSaturationSuccessorData*& CLinkedRoleSaturationSuccessorHash::getLinkedRoleSuccessorData(CLinkedRoleSaturationSuccessorData*& roleSuccData, cint64 linkedIndiID, bool forceNewCreation) {
					CSaturationSuccessorData*& succData = roleSuccData->mSuccNodeDataMap[linkedIndiID];
					if (!succData || forceNewCreation) {
						CSaturationSuccessorData* prevSuccData = succData;
						succData = CObjectAllocator<CSaturationSuccessorData>::allocateAndConstruct(mContext->getUsedMemoryAllocationManager());
						if (prevSuccData) {
							succData->mSuccCount = prevSuccData->mSuccCount;
							succData->mActiveCount = prevSuccData->mActiveCount;
							succData->mExtension = prevSuccData->mExtension;
							succData->mSuccIndiNode = prevSuccData->mSuccIndiNode;
							succData->mCreationRoleLinker = prevSuccData->mCreationRoleLinker;
							succData->mVALUENominalID = prevSuccData->mVALUENominalID;
							succData->mVALUENominalConnection = prevSuccData->mVALUENominalConnection;
							prevSuccData->mActiveCount = 0;
						}
						succData->mNextLink = roleSuccData->mLastLink;
						roleSuccData->mLastLink = succData;

					}
					return succData;
				}




				bool CLinkedRoleSaturationSuccessorHash::hasActiveCreationRole(CSaturationSuccessorData* succData, CRole* creationRole) {
					for (CXNegLinker<CRole*>* creationRoleLinkerIt = succData->mCreationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
						if (!creationRoleLinkerIt->isNegated()) {
							if (creationRoleLinkerIt->getData() == creationRole) {
								return true;
							}
						}
					}
					return false;
				}



				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::addLinkedSuccessor(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole, cint64 linkCount, bool forceAddLink) {
					CLinkedRoleSaturationSuccessorData*& roleSuccData = getLinkedRoleSuccessorData(role,true);
					if (roleSuccData) {
						CSaturationSuccessorData*& succData = getLinkedRoleSuccessorData(roleSuccData,linkedIndi->getIndividualID(),true);
						bool creationRoleAlreadyExists = hasActiveCreationRole(succData,creationRole);
						if (!creationRoleAlreadyExists) {
							++succData->mActiveCount;
						}
						succData->mExtension = false;
						succData->mSuccIndiNode = linkedIndi;
						if (!creationRoleAlreadyExists) {
							succData->mCreationRoleLinker = CObjectAllocator< CXNegLinker<CRole*> >::allocateAndConstruct(mContext->getUsedMemoryAllocationManager())->initNegLinker(creationRole,false)->append(succData->mCreationRoleLinker);
						}
						cint64 linkCountDiff = 0;
						if (forceAddLink) {
							linkCountDiff += linkCount;
						} else {
							if (succData->mSuccCount < linkCount) {
								linkCountDiff = linkCount-succData->mSuccCount;
							}
						}
						succData->mSuccCount += linkCountDiff;
						roleSuccData->mSuccCount += linkCountDiff;
					}
					return this;
				}



				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::addLinkedVALUESuccessor(CRole* role, cint64 nominalID, CRole* creationRole) {
					CLinkedRoleSaturationSuccessorData*& roleSuccData = getLinkedRoleSuccessorData(role,true);
					if (roleSuccData) {
						CSaturationSuccessorData*& succData = getLinkedRoleSuccessorData(roleSuccData,-nominalID,true);
						bool creationRoleAlreadyExists = hasActiveCreationRole(succData,creationRole);
						if (!creationRoleAlreadyExists) {
							++succData->mActiveCount;
						}
						succData->mExtension = false;
						succData->mVALUENominalConnection = true;
						succData->mVALUENominalID = nominalID;
						if (!creationRoleAlreadyExists) {
							succData->mCreationRoleLinker = CObjectAllocator< CXNegLinker<CRole*> >::allocateAndConstruct(mContext->getUsedMemoryAllocationManager())->initNegLinker(creationRole,false)->append(succData->mCreationRoleLinker);
						}
						cint64 linkCountDiff = 0;
						if (succData->mSuccCount < 1) {
							linkCountDiff = 1;
						}
						succData->mSuccCount += linkCountDiff;
						roleSuccData->mSuccCount += linkCountDiff;
					}
					return this;
				}





				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::addExtensionSuccessor(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole, cint64 linkCount) {
					CLinkedRoleSaturationSuccessorData*& roleSuccData = getLinkedRoleSuccessorData(role,true);
					if (roleSuccData) {
						CSaturationSuccessorData*& succData = getLinkedRoleSuccessorData(roleSuccData,linkedIndi->getIndividualID(),true);
						bool creationRoleAlreadyExists = hasActiveCreationRole(succData,creationRole);
						if (!creationRoleAlreadyExists) {
							++succData->mActiveCount;
						}
						succData->mExtension = true;
						succData->mSuccIndiNode = linkedIndi;
						if (!creationRoleAlreadyExists) {
							succData->mCreationRoleLinker = CObjectAllocator< CXNegLinker<CRole*> >::allocateAndConstruct(mContext->getUsedMemoryAllocationManager())->initNegLinker(creationRole,false)->append(succData->mCreationRoleLinker);
						}
						cint64 linkCountDiff = 0;
						if (succData->mSuccCount < linkCount) {
							linkCountDiff = linkCount-succData->mSuccCount;
						}
						succData->mSuccCount += linkCountDiff;
						roleSuccData->mSuccCount += linkCountDiff;
					}
					return this;
				}



				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::setSuccessorMergedCreation(CRole* role, CIndividualSaturationProcessNode* linkedIndi) {
					CLinkedRoleSaturationSuccessorData*& roleSuccData = getLinkedRoleSuccessorData(role,true);
					if (roleSuccData) {
						CSaturationSuccessorData*& succData = getLinkedRoleSuccessorData(roleSuccData,linkedIndi->getIndividualID());
						//succData->mMergedCreation = true;
					}
					return this;
				}



				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::deactivateLinkedSuccessor(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole) {
					CLinkedRoleSaturationSuccessorData*& roleSuccData = getLinkedRoleSuccessorData(role,true);
					if (roleSuccData) {
						CSaturationSuccessorData*& succData = getLinkedRoleSuccessorData(roleSuccData,linkedIndi->getIndividualID());
						if (succData->mActiveCount > 0) {
							--succData->mActiveCount;
							for (CXNegLinker<CRole*>* creationRoleIt = succData->mCreationRoleLinker; creationRoleIt; creationRoleIt = creationRoleIt->getNext()) {
								if (creationRoleIt->getData() == creationRole && !creationRoleIt->isNegated()) {
									creationRoleIt->setNegation(true);
									break;
								}
							}
							if (succData->mActiveCount <= 0) {
								roleSuccData->mSuccCount -= succData->mSuccCount;
								succData->mSuccCount = 0;
							}
						}
					}
					return this;
				}


				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::reduceLinkedSuccessorCount(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole, cint64 expectedLinkCount, cint64 remainingLinkCount) {
					CLinkedRoleSaturationSuccessorData*& roleSuccData = getLinkedRoleSuccessorData(role,true);
					if (roleSuccData) {
						CSaturationSuccessorData*& succData = getLinkedRoleSuccessorData(roleSuccData,linkedIndi->getIndividualID());
						if (succData->mActiveCount > 0) {
							if (expectedLinkCount == roleSuccData->mSuccCount) {
								if (remainingLinkCount == 0) {
									--succData->mActiveCount;
									for (CXNegLinker<CRole*>* creationRoleIt = succData->mCreationRoleLinker; creationRoleIt; creationRoleIt = creationRoleIt->getNext()) {
										if (creationRoleIt->getData() == creationRole && !creationRoleIt->isNegated()) {
											creationRoleIt->setNegation(true);
											break;
										}
									}
								}
								if (succData->mActiveCount <= 0) {
									roleSuccData->mSuccCount -= succData->mSuccCount;
									succData->mSuccCount = 0;
								} else {
									succData->mSuccCount = remainingLinkCount;
								}
							}

						}
					}
					return this;
				}



				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::increaseLinkedSuccessorCount(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole, cint64 incLinkCount) {
					CLinkedRoleSaturationSuccessorData*& roleSuccData = getLinkedRoleSuccessorData(role,true);
					if (roleSuccData) {
						CSaturationSuccessorData*& succData = getLinkedRoleSuccessorData(roleSuccData,linkedIndi->getIndividualID());
						if (succData->mActiveCount > 0) {
							succData->mSuccCount += incLinkCount;
							roleSuccData->mSuccCount += incLinkCount;
						}
					}
					return this;
				}


				CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* CLinkedRoleSaturationSuccessorHash::getLinkedRoleSuccessorHash() {
					return &mRoleSuccDataHash;
				}

				CLinkedRoleSaturationSuccessorData*& CLinkedRoleSaturationSuccessorHash::getLinkedRoleSuccessorData(CRole* role, bool create) {
					CLinkedRoleSaturationSuccessorData*& roleSuccData = mRoleSuccDataHash[role];
					if (!roleSuccData) {
						roleSuccData = CObjectAllocator<CLinkedRoleSaturationSuccessorData>::allocateAndConstruct(mContext->getUsedMemoryAllocationManager());
					}
					return roleSuccData;
				}

				bool CLinkedRoleSaturationSuccessorHash::hasLinkedRoleSuccessorData(CRole* role) {
					CLinkedRoleSaturationSuccessorData* roleSuccData = mRoleSuccDataHash.value(role);
					if (roleSuccData && roleSuccData->mSuccCount > 0) {
						return true;
					}
					return false;
				}

				CConceptSaturationDescriptor* CLinkedRoleSaturationSuccessorHash::getLastExaminedConceptDescriptor() {
					return mLastExaminedConDes;
				}

				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::setLastExaminedConceptDescriptor(CConceptSaturationDescriptor* conDes) {
					mLastExaminedConDes = conDes;
					return this;
				}

				CSaturationSuccessorRoleAssertionLinker* CLinkedRoleSaturationSuccessorHash::getLastExaminedRoleAssertionLinker() {
					return mLastExaminedRoleAssLinker;
				}

				CLinkedRoleSaturationSuccessorHash* CLinkedRoleSaturationSuccessorHash::setLastExaminedRoleAssertionLinker(CSaturationSuccessorRoleAssertionLinker* roleAssLinker) {
					mLastExaminedRoleAssLinker = roleAssLinker;
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
