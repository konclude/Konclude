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

				mNextIndividualId = 0;
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
				mNextIndividualId = aBox->mNextIndividualId;
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

			cint64 CABox::getNextIndividualId(bool increment) {
				if (mNextIndividualId == 0) {
					if (mIndiVec) {
						mNextIndividualId = mIndiVec->getItemCount() + 1;
					}
				}
				if (increment) {
					return mNextIndividualId++;
				} else {
					return mNextIndividualId;
				}
			}

			CABox* CABox::setNextIndividualId(cint64 nextIndividualId) {
				mNextIndividualId = nextIndividualId;
				return this;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
