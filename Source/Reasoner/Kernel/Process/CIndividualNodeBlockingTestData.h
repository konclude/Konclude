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

#ifndef KONCLUDE_REASONER_KERNEL_BLOCKER_CINDIVIDUALNODEBLOCKINGTESTDATA_H
#define KONCLUDE_REASONER_KERNEL_BLOCKER_CINDIVIDUALNODEBLOCKINGTESTDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualNodeBlockData.h"
#include "CNodeSwitchTag.h"
#include "CConceptDescriptor.h"
#include "CIndividualProcessNode.h"
#include "CConceptLabelSetModificationTag.h"
#include "CCoreConceptDescriptor.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualNodeBlockingTestData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualNodeBlockingTestData : public CIndividualNodeBlockData, public CNodeSwitchTag, public CConceptLabelSetModificationTag {
					// public methods
					public:
						//! Constructor
						CIndividualNodeBlockingTestData();

						//! Destructor
						virtual ~CIndividualNodeBlockingTestData();

						CIndividualNodeBlockingTestData* initBlockData(CIndividualNodeBlockingTestData* prevBlockData);

						CIndividualProcessNode* getBlockingIndividualNode();
						CIndividualNodeBlockingTestData* clearBlockingIndividualNode();
						CIndividualNodeBlockingTestData* setBlockingIndividualNode(CIndividualProcessNode* blockingIndividualNode);

						CConceptDescriptor* getLastCoreBlockingCandidateConceptDescriptor();
						CIndividualNodeBlockingTestData* setLastCoreBlockingCandidateConceptDescriptor(CConceptDescriptor* lastCoreBlockCandConDes);

						cint64 getLastCoreBlockingCandidateConceptNodeDifference();
						CIndividualNodeBlockingTestData* setLastCoreBlockingCandidateConceptNodeDifference(cint64 diffCount);

						CCoreConceptDescriptor* getLastAddedCoreConceptDescriptor();
						CIndividualNodeBlockingTestData* setLastAddedCoreConceptDescriptor(CCoreConceptDescriptor* lastCoreConDes);


					// protected methods
					protected:

					// protected variables
					protected:
						//CConceptDescriptor* mConceptDes;
						//cint64 mFirstEntailedAncestorDepth;
						CIndividualProcessNode* mBlockingIndiNode;
						CConceptDescriptor* mLastCoreBlockCandConDes;
						CCoreConceptDescriptor* mLastAddedCoreConDes;
						cint64 mLastCoreBlockCandNodeDiff;
						
					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_BLOCKER_CINDIVIDUALNODEBLOCKINGTESTDATA_H
