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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CBRANCHINGMERGINGPROCESSINGRESTRICTIONSPECIFICATION_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CBRANCHINGMERGINGPROCESSINGRESTRICTIONSPECIFICATION_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CProcessingRestrictionSpecification.h"
#include "CBranchingMergingIndividualNodeCandidateLinker.h"
#include "CIndividualProcessNode.h"
#include "CDependencyTracker.h"

// Other includes
#include "Utilities/Memory/CObjectParameterizingAllocator.h"

// Logger includes
#include "Logger/CLogger.h"

#include <iostream>
using namespace std;

namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CBranchingMergingProcessingRestrictionSpecification
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBranchingMergingProcessingRestrictionSpecification : public CProcessingRestrictionSpecification, public CDependencyTracker {
					// public methods
					public:
						//! Constructor
						CBranchingMergingProcessingRestrictionSpecification(CProcessContext* processContext = nullptr);

						CBranchingMergingProcessingRestrictionSpecification* initBranchingMergingProcessingRestriction(CBranchingMergingProcessingRestrictionSpecification* prevRest = nullptr);
					
						CPROCESSSET<cint64>* getDistinctMergedNodesSet(bool create = true);

						CBranchingMergingIndividualNodeCandidateLinker* getMergingCandidateNodeLinker();
						CBranchingMergingIndividualNodeCandidateLinker* takeNextMergingCandidateNodeLinker();
						CBranchingMergingProcessingRestrictionSpecification* addMergingCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker);


						CBranchingMergingIndividualNodeCandidateLinker* getMergingInitializationCandidateNodeLinker();
						CBranchingMergingIndividualNodeCandidateLinker* takeNextMergingInitializationCandidateNodeLinker();
						CBranchingMergingProcessingRestrictionSpecification* addMergingInitializationCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker);


						CBranchingMergingIndividualNodeCandidateLinker* getOnlyPosQualifyCandidateNodeLinker();
						CBranchingMergingProcessingRestrictionSpecification* addOnlyPosQualifyCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker);
						CBranchingMergingProcessingRestrictionSpecification* clearOnlyPosQualifyCandidateNodeLinker();

						CBranchingMergingIndividualNodeCandidateLinker* getOnlyNegQualifyCandidateNodeLinker();
						CBranchingMergingProcessingRestrictionSpecification* addOnlyNegQualifyCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker);
						CBranchingMergingProcessingRestrictionSpecification* clearOnlyNegQualifyCandidateNodeLinker();

						CBranchingMergingIndividualNodeCandidateLinker* getBothQualifyCandidateNodeLinker();
						CBranchingMergingProcessingRestrictionSpecification* addBothQualifyCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker);
						CBranchingMergingProcessingRestrictionSpecification* setBothQualifyCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker);

						CBranchingMergingProcessingRestrictionSpecification* addDistinctMergedNode(cint64 mergedIndiNode);
						CBranchingMergingProcessingRestrictionSpecification* removeDistinctMergedNode(cint64 mergedIndiNode);


						cint64 getRemainingLinkerMergingCandidateIndividualNodeCount();
						CBranchingMergingProcessingRestrictionSpecification* setRemainingLinkerMergingCandidateIndividualNodeCount(cint64 remainingCanidateCount);

						cint64 getRemainingValidMergingCandidateIndividualNodeCount();
						CBranchingMergingProcessingRestrictionSpecification* setRemainingValidMergingCandidateIndividualNodeCount(cint64 remainingCanidateCount);
						CBranchingMergingProcessingRestrictionSpecification* incRemainingValidMergingCandidateIndividualNodeCount();

						bool hasValidRemainingMergingCandidates();

						CIndividualLinkEdge* getLastIndividualLink();
						CBranchingMergingProcessingRestrictionSpecification* setLastIndividualLink(CIndividualLinkEdge* indiLink);

						bool hasRemainingMergingCandidates();
						bool hasMergingInitializationCandidates();
						bool hasRemainingMergingInitializationCandidates();


						CBranchingMergingProcessingRestrictionSpecification* setDistinctSetFixed(bool fixed);
						bool isDistinctSetFixed();

						CBranchingMergingProcessingRestrictionSpecification* setRemainingNominalCreationCount(cint64 nomCount);
						cint64 getRemainingNominalCreationCount();


						bool hasAddedBlockablePredecessorMergingNodeCandidate();
						CDependencyTrackPoint* getAddedBlockablePredecessorDependencyTrackPoint();



						CBranchingMergingProcessingRestrictionSpecification* initMergingDependencyNode(CNonDeterministicDependencyNode* depNode);
						CNonDeterministicDependencyNode* getMergingDependencyNode();

						CBranchingMergingProcessingRestrictionSpecification* setMultipleMergingNodesInitializationClashesDescriptors(CClashedDependencyDescriptor* clashes);
						CClashedDependencyDescriptor* getMultipleMergingNodesInitializationClashesDescriptors();

						CBranchingMergingProcessingRestrictionSpecification* setMergingNodesInitializationClashesDescriptors(CClashedDependencyDescriptor* clashes);
						CClashedDependencyDescriptor* getMergingNodesInitializationClashesDescriptors();

						//bool addIndividualToContainer(cint64 container, cint64 individual);
						//void printIndividualContainer();

					// protected methods
					protected:
						CPROCESSSET<cint64>* createLocalizedDistinctMergedNodeSet();

					// protected variables
					protected:
						//QStringList mList;
						cint64 mRemainingNominalCreationCount;
						CIndividualLinkEdge* mIndiLink;
						cint64 mRemainingLinkerMergingCandidateIndiNodeCount;
						cint64 mRemainingValidMergingCandidateIndiNodeCount;
						bool mDistinctSetFixed;
						bool mHasMergingInitCandidates;
						CPROCESSSET<cint64>* mDistinctMergedNodesSet;
						CPROCESSSET<cint64>* mLastDistinctMergedNodesSet;
						CBranchingMergingIndividualNodeCandidateLinker* mMergingNodesLinker;
						CBranchingMergingIndividualNodeCandidateLinker* mMergingInitNodesLinker;
						CBranchingMergingIndividualNodeCandidateLinker* mOnlyPosQualifyNodesLinker;
						CBranchingMergingIndividualNodeCandidateLinker* mOnlyNegQualifyNodesLinker;
						CBranchingMergingIndividualNodeCandidateLinker* mBothQualifyNodesLinker;
						CNonDeterministicDependencyNode* mMergingDependencyNode;
						CClashedDependencyDescriptor* mInitMergingNodesClashes;
						CClashedDependencyDescriptor* mMultipleInitMergingNodesClashes;
						CProcessContext* mProcessContext;
						bool mAddedBlockablePredMergingNodeCandidate;
						CDependencyTrackPoint* mAddedBlockablePredDepTrackPoint;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CBRANCHINGMERGINGPROCESSINGRESTRICTIONSPECIFICATION_H
