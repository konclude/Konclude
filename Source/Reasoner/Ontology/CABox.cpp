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

#include "CABox.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CABox::CABox(CBoxContext* boxContext) {
				mBoxContext = boxContext;
				mMemMan = CContext::getMemoryAllocationManager(mBoxContext);
				mIndiVec = nullptr;
				mDeleteBoxContext = nullptr;
				mActiveIndividualSet = nullptr;
				mUnivConnIndiID = 0;
				mHasUnivConnIndiID = false;
			}


			CABox::~CABox() {
				delete mDeleteBoxContext;
				COPADestroyAndRelease(mIndiVec,mMemMan);
			}


			CABox *CABox::setIndividualVector(CIndividualVector* roleVec) {
				COPADestroyAndRelease(mIndiVec,mMemMan);
				mIndiVec = roleVec;
				return this;
			}

			CIndividualVector* CABox::getIndividualVector(bool create) {
				if (!mIndiVec && create) {
					mIndiVec = CObjectParameterizingAllocator<CIndividualVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mIndiVec;
			}

			qint64 CABox::getIndividualCount() {
				cint64 indiCount = 0;
				if (mIndiVec) {
					indiCount = mIndiVec->getItemCount();
				}
				return indiCount;
			}



			CABox *CABox::getABoxReferenceCopy(CBoxContext *boxContext) {
				CABox *aBox = CObjectParameterizingAllocator<CABox,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(boxContext),boxContext);
				aBox->referenceABox(this);
				return aBox;
			}


			CABox *CABox::referenceABox(CABox *aBox) {
				mHasUnivConnIndiID = aBox->mHasUnivConnIndiID;
				mUnivConnIndiID = aBox->mUnivConnIndiID;
				if (aBox->mIndiVec) {
					getIndividualVector();
					mIndiVec->referenceVector(aBox->mIndiVec);
				} else if (mIndiVec) {					
					mIndiVec->clear();
				}
				return this;
			}


			CBoxContext *CABox::getBoxContext(bool create) {
				if (!mBoxContext && create) {
					mBoxContext = new CConcreteOntologyContextBase();
					mDeleteBoxContext = mBoxContext;
				}
				return mBoxContext;
			}


			cint64 CABox::getUniversalConnectionIndividualID() {
				return mUnivConnIndiID;
			}

			CABox* CABox::setUniversalConnectionIndividualID(cint64 univConnIndiID) {
				mUnivConnIndiID = univConnIndiID;
				return this;
			}

			bool CABox::hasUniversalConnectionIndividual() {
				return mHasUnivConnIndiID;
			}

			CABox* CABox::setHasUniversalConnectionIndividual(bool hasUnivConnIndi) {
				mHasUnivConnIndiID = hasUnivConnIndi;
				return this;
			}




			CBOXSET<CIndividual*>* CABox::getActiveIndividualSet(bool create) {
				if (!mActiveIndividualSet && create) {
					mActiveIndividualSet = CObjectParameterizingAllocator< CBOXSET<CIndividual*> ,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mActiveIndividualSet;
			}

			CABox* CABox::setActiveIndividualSet(CBOXSET<CIndividual*>* takeActiveIndividualSet) {
				mActiveIndividualSet = takeActiveIndividualSet;
				return this;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
