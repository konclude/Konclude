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

#include "CReapplyRoleSuccessorHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CReapplyRoleSuccessorHash::CReapplyRoleSuccessorHash(CProcessContext* context) : mContext(context),mRoleSuccessorDataHash(context) {
					mLinkCount = 0;
				}

				CReapplyRoleSuccessorHash* CReapplyRoleSuccessorHash::initRoleSuccessorHash(CReapplyRoleSuccessorHash* prevRoleSuccHash) {
					if (prevRoleSuccHash) {
						mRoleSuccessorDataHash = prevRoleSuccHash->mRoleSuccessorDataHash;
						mLinkCount = prevRoleSuccHash->mLinkCount;
					} else {
						mRoleSuccessorDataHash.clear();
						mLinkCount = 0;
					}
					return this;
				}


				cint64 CReapplyRoleSuccessorHash::insertRoleSuccessorLink(CRole* role, CIndividualLinkEdge* link, CReapplyQueueIterator* reapplyQueueIterator) {
					CReapplyRoleSuccessorData& roleSuccData = mRoleSuccessorDataHash[role];
					if (roleSuccData.mLinkSet) {
						if (!roleSuccData.mLocatedLinkSet) {
							CPROCESSHASH<cint64,CIndividualLinkEdge*>* newLinkSet = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CIndividualLinkEdge*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
							newLinkSet->init(roleSuccData.mLinkSet,mContext);
							roleSuccData.mLinkSet = newLinkSet;
							roleSuccData.mLocatedLinkSet = true;
						}
						roleSuccData.mLinkSet->insert(getCoupledIndividualID(link),link);
					}
					roleSuccData.mLinkLinker = link->append(roleSuccData.mLinkLinker);
					roleSuccData.mLinkCount++;
					++mLinkCount;
					if (reapplyQueueIterator) {
						*reapplyQueueIterator = roleSuccData.mReapplyQueue.getIterator(true);
					}
					return roleSuccData.mLinkCount;
				}


				CReapplyRoleSuccessorHash* CReapplyRoleSuccessorHash::removeRoleSuccessorLink(CRole* role, CIndividualLinkEdge* link) {
					CReapplyRoleSuccessorData& roleSuccData = mRoleSuccessorDataHash[role];
					if (roleSuccData.mLinkSet) {
						if (!roleSuccData.mLocatedLinkSet) {
							CPROCESSHASH<cint64,CIndividualLinkEdge*>* newLinkSet = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CIndividualLinkEdge*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
							newLinkSet->init(roleSuccData.mLinkSet,mContext);
							roleSuccData.mLinkSet = newLinkSet;
							roleSuccData.mLocatedLinkSet = true;
						}
						roleSuccData.mLinkSet->remove(getCoupledIndividualID(link));
					} else {
						if (roleSuccData.mLinkLinker) {
							// replace by set
							CPROCESSHASH<cint64,CIndividualLinkEdge*>* newLinkSet = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CIndividualLinkEdge*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
							roleSuccData.mLinkSet = newLinkSet;
							roleSuccData.mLocatedLinkSet = true;

							CIndividualLinkEdge* linkIt = roleSuccData.mLinkLinker;
							while (linkIt) {
								if (linkIt != link) {
									newLinkSet->insert(getCoupledIndividualID(linkIt),linkIt);
								}
								linkIt = linkIt->getNext();
							}
						}
					}
					--roleSuccData.mLinkCount;
					--mLinkCount;
					return this;
				}



				CReapplyRoleSuccessorHash* CReapplyRoleSuccessorHash::removeRoleSuccessorLink(CRole* role, cint64 sourceIndiID, cint64 destinationIndiID) {
					CReapplyRoleSuccessorData& roleSuccData = mRoleSuccessorDataHash[role];
					if (roleSuccData.mLinkSet) {
						if (!roleSuccData.mLocatedLinkSet) {
							CPROCESSHASH<cint64,CIndividualLinkEdge*>* newLinkSet = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CIndividualLinkEdge*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
							newLinkSet->init(roleSuccData.mLinkSet,mContext);
							roleSuccData.mLinkSet = newLinkSet;
							roleSuccData.mLocatedLinkSet = true;
						}
						cint64 searchedCoupledID = getCoupledIndividualID(sourceIndiID,destinationIndiID);
						roleSuccData.mLinkSet->remove(searchedCoupledID);
					} else {
						if (roleSuccData.mLinkLinker) {
							// replace by set
							CPROCESSHASH<cint64,CIndividualLinkEdge*>* newLinkSet = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CIndividualLinkEdge*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
							roleSuccData.mLinkSet = newLinkSet;
							roleSuccData.mLocatedLinkSet = true;

							cint64 searchedCoupledID = getCoupledIndividualID(sourceIndiID,destinationIndiID);
							CIndividualLinkEdge* linkIt = roleSuccData.mLinkLinker;
							while (linkIt) {
								cint64 coupledID = getCoupledIndividualID(linkIt);
								if (coupledID != searchedCoupledID) {
									newLinkSet->insert(coupledID,linkIt);
								}
								linkIt = linkIt->getNext();
							}
						}
					}
					--roleSuccData.mLinkCount;
					--mLinkCount;
					return this;
				}



				CIndividualLinkEdge* CReapplyRoleSuccessorHash::getRoleSuccessorToIndividualLink(CRole* role, cint64 sourceIndiID, cint64 destinationIndiID, bool locateable) {
					CReapplyRoleSuccessorData* roleSuccData = mRoleSuccessorDataHash.valuePointer(role);
					if (roleSuccData && roleSuccData->mLinkLinker) {
						if (locateable && !roleSuccData->mLinkSet && roleSuccData->mLinkCount >= 5) {
							CPROCESSHASH<cint64,CIndividualLinkEdge*>* newLinkSet = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CIndividualLinkEdge*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
							roleSuccData->mLinkSet = newLinkSet;
							roleSuccData->mLocatedLinkSet = true;

							CIndividualLinkEdge* linkIt = roleSuccData->mLinkLinker;
							CIndividualLinkEdge* searchedLink = nullptr;
							cint64 searchedCoupledID = getCoupledIndividualID(sourceIndiID,destinationIndiID);
							while (linkIt) {
								cint64 coupledID = getCoupledIndividualID(linkIt);
								newLinkSet->insert(coupledID,linkIt);
								if (coupledID == searchedCoupledID) {
									searchedLink = linkIt;
								}
								linkIt = linkIt->getNext();
							}
							return searchedLink;

						} else if (!roleSuccData->mLinkSet) {
							cint64 searchedCoupledID = getCoupledIndividualID(sourceIndiID,destinationIndiID);
							CIndividualLinkEdge* linkIt = roleSuccData->mLinkLinker;
							while (linkIt) {
								cint64 coupledID = getCoupledIndividualID(linkIt);
								if (coupledID == searchedCoupledID) {
									return linkIt;
								}
								linkIt = linkIt->getNext();
							}

						} else {
							cint64 searchedCoupledID = getCoupledIndividualID(sourceIndiID,destinationIndiID);
							return roleSuccData->mLinkSet->value(searchedCoupledID,nullptr);
						}
					}
					return nullptr;
				}



				bool CReapplyRoleSuccessorHash::hasRoleSuccessorToIndividual(CRole* role, cint64 sourceIndiID, cint64 destinationIndiID, bool locateable) {
					CIndividualLinkEdge* searchedLink = getRoleSuccessorToIndividualLink(role,sourceIndiID,destinationIndiID,locateable);					
					return searchedLink != nullptr;
				}



				bool CReapplyRoleSuccessorHash::hasRoleSuccessor(CRole* role) {
					CReapplyRoleSuccessorData* roleSuccData = nullptr;
					if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
						if (roleSuccData->mLinkSet) {
							return !roleSuccData->mLinkSet->isEmpty();
						} else {
							return roleSuccData->mLinkLinker != nullptr;
						}
					}
					return false;
				}


				CRoleSuccessorLinkIterator CReapplyRoleSuccessorHash::getRoleSuccessorLinkIterator(CRole* role) {
					CReapplyRoleSuccessorData* roleSuccData = nullptr;
					if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
						if (roleSuccData->mLinkSet) {
							return CRoleSuccessorLinkIterator((roleSuccData->mLinkSet)->begin(),(roleSuccData->mLinkSet)->end());
						} else {
							return CRoleSuccessorLinkIterator(roleSuccData->mLinkLinker);
						}
					}
					return CRoleSuccessorLinkIterator(nullptr);
				}


				CRoleSuccessorLinkIterator CReapplyRoleSuccessorHash::getRoleSuccessorLinkIterator(CRole* role, cint64* linkCount, CIndividualLinkEdge*& lastLink) {
					CReapplyRoleSuccessorData* roleSuccData = nullptr;
					if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
						lastLink = roleSuccData->mLinkLinker;
						if (linkCount) {
							*linkCount = roleSuccData->mLinkCount;
						}
						if (roleSuccData->mLinkSet) {
							return CRoleSuccessorLinkIterator((roleSuccData->mLinkSet)->begin(),(roleSuccData->mLinkSet)->end());
						} else {
							return CRoleSuccessorLinkIterator(roleSuccData->mLinkLinker);
						}
					}
					return CRoleSuccessorLinkIterator(nullptr);
				}



				CRoleSuccessorLinkIterator CReapplyRoleSuccessorHash::getRoleSuccessorLinkIterator(CRole* role, cint64* linkCount) {
					CReapplyRoleSuccessorData* roleSuccData = nullptr;
					if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
						if (linkCount) {
							*linkCount = roleSuccData->mLinkCount;
						}
						if (roleSuccData->mLinkSet) {
							return CRoleSuccessorLinkIterator((roleSuccData->mLinkSet)->begin(),(roleSuccData->mLinkSet)->end());
						} else {
							return CRoleSuccessorLinkIterator(roleSuccData->mLinkLinker);
						}
					}
					return CRoleSuccessorLinkIterator(nullptr);
				}


				cint64 CReapplyRoleSuccessorHash::getRoleSuccessorCount(CRole* role) {
					CReapplyRoleSuccessorData* roleSuccData = nullptr;
					if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
						return roleSuccData->mLinkCount;
					}
					return 0;
				}



				CRoleSuccessorLinkIterator CReapplyRoleSuccessorHash::getRoleSuccessorHistoryLinkIterator(CRole* role, CIndividualLinkEdge* lastLink) {
					CReapplyRoleSuccessorData* roleSuccData = nullptr;
					if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
						return CRoleSuccessorLinkIterator(roleSuccData->mLinkLinker,lastLink);
					}
					return CRoleSuccessorLinkIterator(nullptr);
				}


				CRoleSuccessorLinkIterator CReapplyRoleSuccessorHash::getRoleSuccessorHistoryLinkIterator(CRole* role, CIndividualLinkEdge* lastLink, cint64* linkCount) {
					CReapplyRoleSuccessorData* roleSuccData = nullptr;
					if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
						if (linkCount) {
							*linkCount = roleSuccData->mLinkCount;
						}
						return CRoleSuccessorLinkIterator(roleSuccData->mLinkLinker,lastLink);
					}
					return CRoleSuccessorLinkIterator(nullptr);
				}



				CReapplyQueue* CReapplyRoleSuccessorHash::getRoleReapplyQueue(CRole* role, bool create) {
					if (create) {
						CReapplyRoleSuccessorData& roleSuccData = mRoleSuccessorDataHash[role];
						return &roleSuccData.mReapplyQueue;
					} else {
						CReapplyRoleSuccessorData* roleSuccData = nullptr;
						if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
							return &roleSuccData->mReapplyQueue;
						}
					}
					return nullptr;
				}


				bool CReapplyRoleSuccessorHash::containsRoleReapplyQueue(CRole* role) {
					CReapplyRoleSuccessorData* roleSuccData = nullptr;
					if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
						return !roleSuccData->mReapplyQueue.isEmpty();
					}
					return false;
				}


				CReapplyQueueIterator CReapplyRoleSuccessorHash::getRoleReapplyIterator(CRole* role, bool clearDynamicReapplyQueue) {
					CReapplyRoleSuccessorData* roleSuccData = nullptr;
					if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
						return roleSuccData->mReapplyQueue.getIterator(clearDynamicReapplyQueue);
					}
					return CReapplyQueueIterator(nullptr,nullptr);
				}


				cint64 CReapplyRoleSuccessorHash::getCoupledIndividualID(CIndividualLinkEdge* link) {
					return link->getCoupledIndividualID();
				}


				cint64 CReapplyRoleSuccessorHash::getCoupledIndividualID(cint64 indi1ID, cint64 indi2ID) {
					return indi1ID + indi2ID;
				}

				CRoleSuccessorIterator CReapplyRoleSuccessorHash::getRoleIterator() {
					return CRoleSuccessorIterator(mRoleSuccessorDataHash.begin(),mRoleSuccessorDataHash.end());
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
