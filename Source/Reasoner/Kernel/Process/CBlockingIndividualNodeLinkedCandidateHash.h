/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGINDIVIDUALNODELINKEDCANDIDATEHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGINDIVIDUALNODELINKEDCANDIDATEHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CIndividualProcessNode.h"
#include "CBlockingIndividualNodeLinkedCandidateData.h"


// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CBlockingIndividualNodeLinkedCandidateHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBlockingIndividualNodeLinkedCandidateHash {
					// public methods
					public:
						//! Constructor
						CBlockingIndividualNodeLinkedCandidateHash(CProcessContext* context = nullptr);

						//! Destructor
						virtual ~CBlockingIndividualNodeLinkedCandidateHash();

						CBlockingIndividualNodeLinkedCandidateHash* initBlockingIndividualNodeCandidateHash(CBlockingIndividualNodeLinkedCandidateHash* prevBlockCandHash);

						CBlockingIndividualNodeLinkedCandidateData* getBlockingIndividualCandidateData(CConcept* initializationConcept, bool conceptNegation, bool create = true);
						CBlockingIndividualNodeLinkedCandidateData* getBlockingIndividualCandidateData(CConceptDescriptor* initializationConDes, bool create = true);

						
					// protected methods
					protected:

					// protected variables
					protected:
						class CBlockingLinkedCandidateHashData {
							public:
								inline CBlockingLinkedCandidateHashData() {
									mCandidateIndiData = nullptr;
									mPrevCandidateIndiData = nullptr;
								}
								inline CBlockingLinkedCandidateHashData(const CBlockingLinkedCandidateHashData& data) {
									mCandidateIndiData = nullptr;
									mPrevCandidateIndiData = data.mPrevCandidateIndiData;
								}
								CBlockingIndividualNodeLinkedCandidateData* mCandidateIndiData;
								CBlockingIndividualNodeLinkedCandidateData* mPrevCandidateIndiData;
						};
						CProcessContext* mContext;
						CMemoryAllocationManager* mMemMan;

						CPROCESSHASH< QPair<CConcept*,bool> ,CBlockingLinkedCandidateHashData>* mBlockCandidateHash;
						CPROCESSHASH< QPair<CConcept*,bool> ,CBlockingLinkedCandidateHashData>* mUseBlockCandidateHash;
						CPROCESSHASH< QPair<CConcept*,bool> ,CBlockingLinkedCandidateHashData>* mPrevBlockCandidateHash;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGINDIVIDUALNODELINKEDCANDIDATEHASH_H
