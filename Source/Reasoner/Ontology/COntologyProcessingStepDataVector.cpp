/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "COntologyProcessingStepDataVector.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyProcessingStepDataVector::COntologyProcessingStepDataVector(COntologyContext* context) {
				mContext = context;
				mPrStepVector = COntologyProcessingStepVector::getProcessingStepVectorInstance();

				CMemoryAllocationManager* memMan = mContext->getMemoryAllocationManager();
				cint64 stepCount = mPrStepVector->getProcessingStepCount();
				mPrStCount = stepCount;
				mPrStepDataVector = CObjectAllocator<COntologyProcessingStepData*>::allocateArray(memMan,stepCount);
				for (cint64 i = 0; i < stepCount; ++i) {
					COntologyProcessingStepData* stepData = CObjectAllocator<COntologyProcessingStepData>::allocateAndConstruct(memMan);
					stepData->initProcessingStepData(mPrStepVector->getProcessingStep(i));
					mPrStepDataVector[i] = stepData;
				}
			}

			COntologyProcessingStepData* COntologyProcessingStepDataVector::getProcessingStepData(cint64 stepID) {
				if (stepID < 0 || stepID >= mPrStCount) {
					return nullptr;
				}
				return mPrStepDataVector[stepID];
			}

			COntologyProcessingStepData* COntologyProcessingStepDataVector::getProcessingStepData(COntologyProcessingStep::PROCESSINGSTEPTYPE stepType) {
				return getProcessingStepData((cint64)stepType);
			}
		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
