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

#include "CRBox.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CRBox::CRBox(CBoxContext* boxContext) {
				mBoxContext = boxContext;
				mMemMan = CContext::getMemoryAllocationManager(mBoxContext);
				roles = nullptr;
				mRoleChainVec = nullptr;
				mDeleteBoxContext = nullptr;

				mActiveRoleSet = nullptr;

				mBottomObjectRoleTag = 0;
				mTopObjectRoleTag = 1;

				mBottomDataRoleTag = 2;
				mTopDataRoleTag = 3;
			}


			CRBox::~CRBox() {
				delete mDeleteBoxContext;
				COPADestroyAndRelease(roles,mMemMan);
				COPADestroyAndRelease(mRoleChainVec,mMemMan);
				COPADestroyAndRelease(mActiveRoleSet,mMemMan);
			}


			CRBox *CRBox::setRoleVector(CRoleVector* roleVec) {
				COPADestroyAndRelease(roles,mMemMan);
				roles = roleVec;
				return this;
			}

			CRoleVector* CRBox::getRoleVector(bool create) {
				if (!roles && create) {
					roles = CObjectParameterizingAllocator<CRoleVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return roles;
			}

			CRBox *CRBox::setRoleChainVector(CRoleChainVector* roleChainVec) {
				COPADestroyAndRelease(mRoleChainVec,mMemMan);
				mRoleChainVec = roleChainVec;
				return this;
			}

			CRoleChainVector* CRBox::getRoleChainVector(bool create) {
				if (!mRoleChainVec && create) {
					mRoleChainVec = CObjectParameterizingAllocator<CRoleChainVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mRoleChainVec;
			}

			qint64 CRBox::getRoleCount() {
				cint64 roleCount = 0;
				if (roles) {
					roleCount = roles->getItemCount();
				}
				return roleCount;
			}

			qint64 CRBox::getRoleChainCount() {
				cint64 roleChainCount = 0;
				if (mRoleChainVec) {
					roleChainCount = mRoleChainVec->getItemCount();
				}
				return roleChainCount;
			}


			CRBox *CRBox::getRBoxReferenceCopy(CBoxContext *boxContext) {
				CRBox *rBox = CObjectParameterizingAllocator<CRBox,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(boxContext),boxContext);
				rBox->referenceRBox(this);
				return rBox;
			}


			CRBox *CRBox::referenceRBox(CRBox *rBox) {
				if (rBox->roles) {
					getRoleVector();
					roles->referenceVector(rBox->roles);
				} else if (roles) {					
					roles->clear();
				}
				if (rBox->mRoleChainVec) {
					getRoleChainVector();
					mRoleChainVec->referenceVector(rBox->mRoleChainVec);
				} else if (mRoleChainVec) {					
					mRoleChainVec->clear();
				}
				if (rBox->mActiveRoleSet) {
					if (!mActiveRoleSet) {
						mActiveRoleSet = CObjectParameterizingAllocator< CBOXSET<CRole*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
					}
					mActiveRoleSet->init(rBox->mActiveRoleSet,mBoxContext);
				} else if (mActiveRoleSet) {
					mActiveRoleSet->clear();
				}

				return this;
			}


			CBoxContext *CRBox::getBoxContext(bool create) {
				if (!mBoxContext && create) {
					mBoxContext = new CConcreteOntologyContextBase();
					mDeleteBoxContext = mBoxContext;
				}
				return mBoxContext;
			}



			CRole* CRBox::getTopObjectRole() {
				CRole* role = nullptr;
				if (roles) {
					role = roles->getData(mTopObjectRoleTag);
				}
				return role;
			}

			CRBox* CRBox::setTopObjectRole(CRole* role) {
				getRoleVector()->setData(mTopObjectRoleTag,role);
				return this;
			}

			CRole* CRBox::getBottomObjectRole() {
				CRole* role = nullptr;
				if (roles) {
					role = roles->getData(mBottomObjectRoleTag);
				}
				return role;
			}

			CRBox* CRBox::setBottomObjectRole(CRole* role) {
				getRoleVector()->setData(mBottomObjectRoleTag,role);
				return this;
			}


			cint64 CRBox::getTopObjectRoleIndex() {
				return mTopObjectRoleTag;
			}

			cint64 CRBox::getBottomObjectRoleIndex() {
				return mBottomObjectRoleTag;
			}





			CRole* CRBox::getTopDataRole() {
				CRole* role = nullptr;
				if (roles) {
					role = roles->getData(mTopDataRoleTag);
				}
				return role;
			}

			CRBox* CRBox::setTopDataRole(CRole* role) {
				getRoleVector()->setData(mTopDataRoleTag,role);
				return this;
			}

			CRole* CRBox::getBottomDataRole() {
				CRole* role = nullptr;
				if (roles) {
					role = roles->getData(mBottomDataRoleTag);
				}
				return role;
			}

			CRBox* CRBox::setBottomDataRole(CRole* role) {
				getRoleVector()->setData(mBottomDataRoleTag,role);
				return this;
			}


			cint64 CRBox::getTopDataRoleIndex() {
				return mTopDataRoleTag;
			}

			cint64 CRBox::getBottomDataRoleIndex() {
				return mBottomDataRoleTag;
			}



			CBOXSET<CRole*>* CRBox::getActivePropertyRoleSet(bool create) {
				if (!mActiveRoleSet && create) {
					mActiveRoleSet = CObjectParameterizingAllocator< CBOXSET<CRole*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mActiveRoleSet;
			}

			CRBox* CRBox::setActivePropertyRoleSet(CBOXSET<CRole*>* takeActiveRoleSet) {
				COPADestroyAndRelease(mActiveRoleSet,mMemMan);
				mActiveRoleSet = takeActiveRoleSet;
				return this;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
