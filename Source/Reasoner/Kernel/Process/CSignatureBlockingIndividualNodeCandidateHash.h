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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CSignatureBlockingIndividualNodeCandidateHash_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CSignatureBlockingIndividualNodeCandidateHash_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CIndividualProcessNode.h"
#include "CSignatureBlockingIndividualNodeCandidateData.h"


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
				 *		\class		CSignatureBlockingIndividualNodeCandidateHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSignatureBlockingIndividualNodeCandidateHash {
					// public methods
					public:
						//! Constructor
						CSignatureBlockingIndividualNodeCandidateHash(CProcessContext* context = nullptr);

						//! Destructor
						virtual ~CSignatureBlockingIndividualNodeCandidateHash();

						CSignatureBlockingIndividualNodeCandidateHash* initBlockingIndividualNodeCandidateHash(CSignatureBlockingIndividualNodeCandidateHash* prevBlockCandHash);

						CSignatureBlockingIndividualNodeCandidateData* getBlockingIndividualCandidateData(cint64 signature, bool create = true);

						
					// protected methods
					protected:

					// protected variables
					protected:
						class CSignatureBlockingCandidateHashData {
							public:
								inline CSignatureBlockingCandidateHashData() {
									mCandidateIndiData = nullptr;
									mPrevCandidateIndiData = nullptr;
								}
								inline CSignatureBlockingCandidateHashData(const CSignatureBlockingCandidateHashData& data) {
									mCandidateIndiData = nullptr;
									mPrevCandidateIndiData = data.mPrevCandidateIndiData;
								}
								CSignatureBlockingIndividualNodeCandidateData* mCandidateIndiData;
								CSignatureBlockingIndividualNodeCandidateData* mPrevCandidateIndiData;
						};
						CProcessContext* mContext;
						CMemoryAllocationManager* mMemMan;

						CPROCESSHASH<cint64,CSignatureBlockingCandidateHashData>* mBlockCandidateHash;
						CPROCESSHASH<cint64,CSignatureBlockingCandidateHashData>* mUseBlockCandidateHash;
						CPROCESSHASH<cint64,CSignatureBlockingCandidateHashData>* mPrevBlockCandidateHash;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CSignatureBlockingIndividualNodeCandidateHash_H
