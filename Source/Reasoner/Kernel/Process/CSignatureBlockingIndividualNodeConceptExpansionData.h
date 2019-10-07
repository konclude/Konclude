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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CSIGNATUREBLOCKINGINDIVIDUALNODECONCEPTEXPANSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CSIGNATUREBLOCKINGINDIVIDUALNODECONCEPTEXPANSIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptor.h"
#include "CProcessContext.h"

// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CSignatureBlockingIndividualNodeConceptExpansionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSignatureBlockingIndividualNodeConceptExpansionData {
					// public methods
					public:
						//! Constructor
						CSignatureBlockingIndividualNodeConceptExpansionData();

						CSignatureBlockingIndividualNodeConceptExpansionData* initBlockingExpansionData(CSignatureBlockingIndividualNodeConceptExpansionData* prevData);

						CConceptDescriptor* getLastSubsetTestedConceptDescriptor();
						cint64 setBlockingConceptSignature();
						cint64 getBlockingConceptCount();
						cint64 getContinuousExpandedContainedConceptCount();
						cint64 getLastUpdatedConceptExpansionCount();
						cint64 getLastUpdatedConceptCount();
						CIndividualProcessNode* getBlockerIndividualNode();
						bool isBlockingReviewMarked();
						bool isBlockingSubsetReviewMarked();
						bool isIdenticConceptSetRequired();
						bool isConceptSetStillSubset();


						CSignatureBlockingIndividualNodeConceptExpansionData* setLastSubsetTestedConceptDescriptor(CConceptDescriptor* conDes);
						CSignatureBlockingIndividualNodeConceptExpansionData* setBlockingConceptSignature(cint64 signature);
						CSignatureBlockingIndividualNodeConceptExpansionData* setBlockingConceptCount(cint64 conCount);
						CSignatureBlockingIndividualNodeConceptExpansionData* setContinuousExpandedContainedConceptCount(cint64 conCount);
						CSignatureBlockingIndividualNodeConceptExpansionData* setLastUpdatedConceptExpansionCount(cint64 conCount);
						CSignatureBlockingIndividualNodeConceptExpansionData* setLastUpdatedConceptCount(cint64 conCount);
						CSignatureBlockingIndividualNodeConceptExpansionData* setBlockerIndividualNode(CIndividualProcessNode* node);
						CSignatureBlockingIndividualNodeConceptExpansionData* setBlockingReviewMarked(bool marked);
						CSignatureBlockingIndividualNodeConceptExpansionData* setBlockingSubsetReviewMarked(bool marked);
						CSignatureBlockingIndividualNodeConceptExpansionData* setIdenticConceptSetRequired(bool identicConceptSetRequired);
						CSignatureBlockingIndividualNodeConceptExpansionData* setConceptSetStillSubset(bool stillSubset);


					// protected methods
					protected:

					// protected variables
					protected:
						CConceptDescriptor* mSubsetTestedConDes;
						CIndividualProcessNode* blockerIndiNode;
						cint64 mBlockingConceptSignature;
						cint64 mBlockingConceptCount;
						cint64 mExpandedContainedConceptCount;
						cint64 mLastUpdatedConExpCount;
						cint64 mLastUpdatedConCount;
						bool mReviewMarked;
						bool mReviewSubsetMarked;
						bool mIdenticConceptSetRequired;
						bool mStillConceptSetSubset;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CSIGNATUREBLOCKINGINDIVIDUALNODECONCEPTEXPANSIONDATA_H
