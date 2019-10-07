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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CBRANCHTREENODE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CBRANCHTREENODE_H

// Libraries includes


// Namespace includes
#include "DependencySettings.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Kernel/Process/CProcessContext.h"
#include "Reasoner/Kernel/Process/CBranchingTag.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Task {
				// forward declaration
				class CSatisfiableCalculationTask;
			};

			namespace Process {

				namespace Dependency {


//#define KONCLUCE_TASK_ALGORITHM_BRANCH_STRING_INSTRUCTION(a) a
#define KONCLUCE_TASK_ALGORITHM_BRANCH_STRING_INSTRUCTION(a)


					/*! 
					 *
					 *		\class		CBranchTreeNode
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CBranchTreeNode : public CBranchingTag {
						// public methods
						public:
							//! Constructor
							CBranchTreeNode(CProcessContext* processContext = nullptr);

							CBranchTreeNode* initBranchingRootNode(Task::CSatisfiableCalculationTask* satCalcTask);
							CBranchTreeNode* initBranchingChildNode(CBranchTreeNode* parentBranchTreeNode, Task::CSatisfiableCalculationTask* satCalcTask);
							CBranchTreeNode* initBranchingCopyNode(CBranchTreeNode* copyBranchTreeNode, Task::CSatisfiableCalculationTask* satCalcTask);

							CBranchTreeNode* getParentNode();
							CBranchTreeNode* getRootNode();
							bool isRootNode();

							cint64 getBranchingLevel();

							CBranchTreeNode* branchingIncrement(CNonDeterministicDependencyTrackPoint* depTrackPoint);
							CNonDeterministicDependencyTrackPoint* getDependencyTrackPoint();

							Task::CSatisfiableCalculationTask* getSatisfiableCalculationTask();

							KONCLUCE_TASK_ALGORITHM_BRANCH_STRING_INSTRUCTION(CBranchTreeNode* setBranchString(QString* string));


						// protected methods
						protected:


						// protected variables
						protected:
							CProcessContext* mProcessContext;
							CBranchTreeNode* mParentNode;
							CBranchTreeNode* mRootNode;

							CNonDeterministicDependencyTrackPoint* mBranchedDepTrackPoint;
							Task::CSatisfiableCalculationTask* mSatCalcTask;

							KONCLUCE_TASK_ALGORITHM_BRANCH_STRING_INSTRUCTION(QString* mBranchString);


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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CBRANCHTREENODE_H
