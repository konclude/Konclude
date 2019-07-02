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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CNONDETERMINISTICDEPENDENCYNODE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CNONDETERMINISTICDEPENDENCYNODE_H

// Libraries includes


// Namespace includes
#include "DependencySettings.h"
#include "CDependencyNode.h"
#include "CNonDeterministicDependencyTrackPoint.h"


// Other includes
#include "Reasoner/Kernel/Process/CClashedDependencyDescriptor.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {

					/*! 
					 *
					 *		\class		CNonDeterministicDependencyNode
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CNonDeterministicDependencyNode : public CDependencyNode {
						// public methods
						public:
							//! Constructor
							CNonDeterministicDependencyNode(CProcessContext* processContext = nullptr);


							CNonDeterministicDependencyNode* initNonDeterministicDependencyNode(DEPENDENCNODEYTYPE depType, CBranchTreeNode* branchNode, CConceptDescriptor* conceptDescriptor = nullptr);
							CNonDeterministicDependencyNode* initNonDeterministicDependencyNode(DEPENDENCNODEYTYPE depType, CBranchTreeNode* branchNode, CIndividualProcessNode* individualNode, CConceptDescriptor* conceptDescriptor = nullptr);
							
							virtual CDependencyTrackPoint* getContinueDependencyTrackPoint();
							virtual bool isDeterministiDependencyNode();


							CNonDeterministicDependencyTrackPoint* getDependencyTrackPointBranch();


							bool hasDependencyClashes();
							CClashedDependencyDescriptor* getDependencyClashes();
							CNonDeterministicDependencyNode* setDependencyClash(CClashedDependencyDescriptor* clash);

							CNonDeterministicDependencyTrackPoint* getBranchTrackPoints();
							CNonDeterministicDependencyTrackPoint* getClashTrackPoint();
							CNonDeterministicDependencyTrackPoint* addBranchClashes(CClashedDependencyDescriptor* clash);

							CNonDeterministicDependencyTrackPoint* getClosingDependencyTrackPoint();
							CNonDeterministicDependencyTrackPoint* getClosedDependencyTrackPoint();
							CNonDeterministicDependencyNode* setClosingDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint);
							CNonDeterministicDependencyNode* setClosedDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint);
							bool hasClosingDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint);
							bool hasClosedDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint);
							bool hasClosingDependencyTrackPoint();
							bool hasClosedDependencyTrackPoint();

							cint64 getOpenedDependencyTrackingPointsCount();
							bool hasMultipleOpenedDependencyTrackingPoints();
							bool hasOtherOpenedDependencyTrackingPoints(CNonDeterministicDependencyTrackPoint* depTrackPoint);

							CBranchTreeNode* getBranchNode();

						// protected methods
						protected:
							bool updateBranchingTags();


						// protected variables
						protected:
							CNonDeterministicDependencyTrackPoint* mBranchTrackPoints;
							CNonDeterministicDependencyTrackPoint mClashTrackPoint;
							CClashedDependencyDescriptor* mDependencyClashes;
							CBranchTreeNode* mBranchNode;
							cint64 mBranchTag;


							CNonDeterministicDependencyTrackPoint* mClosingTrackPoint;
							CNonDeterministicDependencyTrackPoint* mClosedTrackPoint;


						// private methods
						private:

						// private variables
						private:


					};

				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CNONDETERMINISTICDEPENDENCYNODE_H
