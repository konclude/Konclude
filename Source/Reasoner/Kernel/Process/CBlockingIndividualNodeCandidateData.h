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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGINDIVIDUALNODECANDIDATEDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGINDIVIDUALNODECANDIDATEDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualProcessNode.h"
#include "CProcessContext.h"
#include "CConceptLabelSetModificationTag.h"
#include "CNodeSwitchTag.h"
#include "CBlockingIndividualNodeCandidateIterator.h"


// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CBlockingIndividualNodeCandidateData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBlockingIndividualNodeCandidateData : public CConceptLabelSetModificationTag, public CNodeSwitchTag {
					// public methods
					public:
						//! Constructor
						CBlockingIndividualNodeCandidateData(CProcessContext* context = nullptr);

						//! Destructor
						virtual ~CBlockingIndividualNodeCandidateData();

						CBlockingIndividualNodeCandidateData* initBlockingCandidateData(CBlockingIndividualNodeCandidateData* prevBlockCandHash);

						CBlockingIndividualNodeCandidateData* insertBlockingCandidateIndividualNode(CIndividualProcessNode* candidateIndi);

						CBlockingIndividualNodeCandidateIterator getBlockingCandidatesIndividualNodeIterator(cint64 candidateIndividualID);
						CBlockingIndividualNodeCandidateIterator getBlockingCandidatesIndividualNodeIterator(CIndividualProcessNode* candidateIndi);

						cint64 getMaxValidIndividualID();
						CBlockingIndividualNodeCandidateData* setMaxValidIndividualID(cint64 indiID);


					// protected methods
					protected:

					// protected variables
					protected:
						CPROCESSMAP<cint64,CIndividualProcessNode*> mCandidateIndiMap;
						cint64 mMaxValidIndividualID;
						CProcessContext* mContext;
						CMemoryAllocationManager* mMemMan;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGINDIVIDUALNODECANDIDATEDATA_H
