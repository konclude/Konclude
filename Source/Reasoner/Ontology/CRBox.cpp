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

				mBottomRoleTag = 0;
				mTopRoleTag = 1;
			}


			CRBox::~CRBox() {
				delete mDeleteBoxContext;
				COPADestroyAndRelease(roles,mMemMan);
				COPADestroyAndRelease(mRoleChainVec,mMemMan);
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
				return this;
			}


			CBoxContext *CRBox::getBoxContext(bool create) {
				if (!mBoxContext && create) {
					mBoxContext = new CConcreteOntologyContextBase();
					mDeleteBoxContext = mBoxContext;
				}
				return mBoxContext;
			}



			CRole* CRBox::getTopRole() {
				CRole* role = nullptr;
				if (roles) {
					role = roles->getData(mTopRoleTag);
				}
				return role;
			}

			CRBox* CRBox::setTopRole(CRole* role) {
				getRoleVector()->setData(mTopRoleTag,role);
				return this;
			}

			CRole* CRBox::getBottomRole() {
				CRole* role = nullptr;
				if (roles) {
					role = roles->getData(mBottomRoleTag);
				}
				return role;
			}

			CRBox* CRBox::setBottomRole(CRole* role) {
				getRoleVector()->setData(mBottomRoleTag,role);
				return this;
			}




		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
