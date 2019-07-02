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

#include "COntologyDataBoxes.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyDataBoxes::COntologyDataBoxes(CBoxContext* boxContext) {
				mBoxContext = boxContext;
				mTBox = CObjectParameterizingAllocator< CTBox,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				mRBox = CObjectParameterizingAllocator< CRBox,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				mABox = CObjectParameterizingAllocator< CABox,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				mMBox = CObjectParameterizingAllocator< CMBox,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				mExpDataBoxMapping = CObjectParameterizingAllocator< CExpressionDataBoxMapping,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);

				mBasicBuild = false;
				mInitialBuild = false;
				mIterationBuild = false;
				mPrevOntBasicBuild = false;
				mPrevOntInitialBuild = false;
				mPrevOntIterationBuild = false;

				mValueSpaceTypes = CDatatypeValueSpaceTypes::getValueSpaceTypes();
			}



			COntologyDataBoxes::~COntologyDataBoxes() {
				COPADestroyAndRelease(mTBox,CContext::getMemoryAllocationManager(mBoxContext));
				COPADestroyAndRelease(mRBox,CContext::getMemoryAllocationManager(mBoxContext));
				COPADestroyAndRelease(mABox,CContext::getMemoryAllocationManager(mBoxContext));
				COPADestroyAndRelease(mMBox,CContext::getMemoryAllocationManager(mBoxContext));
				COPADestroyAndRelease(mExpDataBoxMapping,CContext::getMemoryAllocationManager(mBoxContext));
			}


			COntologyDataBoxes* COntologyDataBoxes::referenceDataBoxes(COntologyDataBoxes* dataBoxes) {
				mTBox->referenceTBox(dataBoxes->mTBox);
				mRBox->referenceRBox(dataBoxes->mRBox);
				mABox->referenceABox(dataBoxes->mABox);
				mMBox->referenceMBox(dataBoxes->mMBox);
				mExpDataBoxMapping->referenceDataBoxMapping(dataBoxes->mExpDataBoxMapping);
				mBasicBuild = dataBoxes->mBasicBuild;
				mInitialBuild = dataBoxes->mInitialBuild;
				mIterationBuild = dataBoxes->mIterationBuild;
				mPrevOntBasicBuild = mBasicBuild;
				mPrevOntInitialBuild = mInitialBuild;
				mPrevOntIterationBuild = mIterationBuild;
				return this;
			}

			CConcept *COntologyDataBoxes::getTopConcept() {
				return mTBox->getTopConcept();
			}

			CConcept *COntologyDataBoxes::getBottomConcept() {
				return mTBox->getBottomConcept();
			}

			CTBox* COntologyDataBoxes::getTBox() {
				return mTBox;
			}

			CRBox* COntologyDataBoxes::getRBox() {
				return mRBox;
			}

			CABox* COntologyDataBoxes::getABox() {
				return mABox;
			}

			CMBox* COntologyDataBoxes::getMBox() {
				return mMBox;
			}

			CBoxContext* COntologyDataBoxes::getBoxContext() {
				return mBoxContext;
			}


			CExpressionDataBoxMapping* COntologyDataBoxes::getExpressionDataBoxMapping() {
				return mExpDataBoxMapping;
			}

			COntologyDataBoxes* COntologyDataBoxes::setExpressionDataBoxMapping(CExpressionDataBoxMapping* expDatBoxMapping) {
				mExpDataBoxMapping = expDatBoxMapping;
				return this;
			}

			bool COntologyDataBoxes::isBasicBuild() {
				return mBasicBuild;
			}

			bool COntologyDataBoxes::isInitialBuild() {
				return mInitialBuild;
			}

			bool COntologyDataBoxes::isIterationBuild() {
				return mIterationBuild;
			}


			COntologyDataBoxes* COntologyDataBoxes::setBasicBuild(bool basicBuild) {
				mBasicBuild = basicBuild;
				return this;
			}

			COntologyDataBoxes* COntologyDataBoxes::setInitialBuild(bool initialBuild) {
				mInitialBuild = initialBuild;
				return this;
			}

			COntologyDataBoxes* COntologyDataBoxes::setIterationBuild(bool iterationBuild) {
				mIterationBuild = iterationBuild;
				return this;
			}

			COntologyDataBoxes* COntologyDataBoxes::setBuildContinuation() {
				mBasicBuild = false;
				mInitialBuild = false;
				mIterationBuild = false;
				if (mPrevOntIterationBuild) {
					mIterationBuild = true;
				} else if (mPrevOntInitialBuild) {
					mIterationBuild = true;
				} else if (mPrevOntBasicBuild) {
					mInitialBuild = true;
				}
				return this;
			}


			CDatatypeValueSpaceTypes* COntologyDataBoxes::getDatatypeValueSpaceTypes() {
				return mValueSpaceTypes;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
