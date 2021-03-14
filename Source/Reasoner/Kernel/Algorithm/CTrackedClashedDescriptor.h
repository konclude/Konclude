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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDESCRIPTOR_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDESCRIPTOR_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"

// Other includes
#include "Reasoner/Kernel/Process/CClashedDependencyDescriptor.h"
#include "Reasoner/Kernel/Process/CConceptDescriptor.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CVariableBindingPath.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CTrackedClashedDescriptor
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CTrackedClashedDescriptor : public CClashedDependencyDescriptor {
					// public methods
					public:
						//! Constructor
						CTrackedClashedDescriptor();

						CTrackedClashedDescriptor* initTrackedClashedDescriptor(CIndividualProcessNode* indiNode, CConceptDescriptor* conDes, CVariableBindingPath* varBindPath, CDependencyTrackPoint* depTrackPoint);
						CTrackedClashedDescriptor* initTrackedClashedDescriptor(CTrackedClashedDescriptor* trackedClashDes);
						CTrackedClashedDescriptor* initTrackedClashedDescriptor(CTrackedClashedDescriptor* trackedClashDesForIndiNode, CConceptDescriptor* conDes, CVariableBindingPath* varBindPath, CDependencyTrackPoint* depTrackPoint);
						CTrackedClashedDescriptor* initTrackedClashedDescriptor(CTrackedClashedDescriptor* trackedClashDesForIndiNode, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint);

						bool isAppropriatedIndividualNominal();
						cint64 getAppropriatedIndividualID();
						CIndividualProcessNode* getAppropriatedIndividual();


						cint64 getAppropriatedIndividualLevel();

						cint64 getBranchingLevelTag();
						cint64 getProcessingTag();

						bool isPointingToDeterministicDependencyNode();
						bool isPointingToNonDeterministicDependencyNode();
						bool isTrackingError();
						bool isPointingToIndependentDependencyNode();

						CConceptDescriptor* getConceptDescriptor();
						CTrackedClashedDescriptor* setConceptDescriptor(CConceptDescriptor* conDes);

						bool candidateTracking(bool* trackingError, bool* nominalOccured, cint64* maxBranchLevel, cint64* minIndividualLevel);
						bool isProcessedAfter(cint64 processingTag);


						CTrackedClashedDescriptor* append(CTrackedClashedDescriptor* trackedClashedDescriptor);
						CTrackedClashedDescriptor* getNextDescriptor();

						CVariableBindingPath* getVariableBindingPath();


					// protected methods
					protected:

					// protected variables
					protected:
						CIndividualProcessNode* mIndiNode;
						cint64 mIndiNodeID;
						cint64 mIndiNodeLevel;
						cint64 mBranchingLevelTag;
						cint64 mProcessingTag;
						bool mDetermisticFlag;
						bool mNominalIndiFlag;
						bool mErrorFlag;
						bool mIndepenentFlag;

						CConceptDescriptor* mConceptDescriptor;
						CVariableBindingPath* mVarBindPath;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDESCRIPTOR_H
