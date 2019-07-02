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

#include "CBranchTreeNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {


					CBranchTreeNode::CBranchTreeNode(CProcessContext* processContext) {
						mProcessContext = processContext;
						mBranchedDepTrackPoint = nullptr;
						mSatCalcTask = nullptr;

						KONCLUCE_TASK_ALGORITHM_BRANCH_STRING_INSTRUCTION(mBranchString = nullptr);
					}

					CBranchTreeNode* CBranchTreeNode::initBranchingRootNode(Task::CSatisfiableCalculationTask* satCalcTask) {
						mParentNode = nullptr;
						mRootNode = this;
						initBranchingTag((cint64)0);
						mSatCalcTask = satCalcTask;
						return this;
					}


					CBranchTreeNode* CBranchTreeNode::initBranchingChildNode(CBranchTreeNode* parentBranchTreeNode, Task::CSatisfiableCalculationTask* satCalcTask) {
						mSatCalcTask = satCalcTask;
						mBranchedDepTrackPoint = nullptr;
						mParentNode = parentBranchTreeNode;
						mRootNode = nullptr;
						initBranchingTag((cint64)0);
						if (parentBranchTreeNode) {
							mRootNode = parentBranchTreeNode->mRootNode;
							setBranchingTag(parentBranchTreeNode->getBranchingLevel());
						}
						return this;
					}


					CBranchTreeNode* CBranchTreeNode::branchingIncrement(CNonDeterministicDependencyTrackPoint* depTrackPoint) {
						incBranchingTag();
						mBranchedDepTrackPoint = depTrackPoint;
						return this;
					}


					CNonDeterministicDependencyTrackPoint* CBranchTreeNode::getDependencyTrackPoint() {
						return mBranchedDepTrackPoint;
					}



					CBranchTreeNode* CBranchTreeNode::initBranchingCopyNode(CBranchTreeNode* copyBranchTreeNode, Task::CSatisfiableCalculationTask* satCalcTask) {
						mSatCalcTask = satCalcTask;
						mBranchedDepTrackPoint = copyBranchTreeNode->mBranchedDepTrackPoint;
						mParentNode = copyBranchTreeNode->mParentNode;
						mRootNode = copyBranchTreeNode->mRootNode;
						setBranchingTag(copyBranchTreeNode->getBranchingLevel());
						if (copyBranchTreeNode->isRootNode()) {
							mRootNode = this;
						}
						return this;
					}


					CBranchTreeNode* CBranchTreeNode::getParentNode() {
						return mParentNode;
					}

					CBranchTreeNode* CBranchTreeNode::getRootNode() {
						return mRootNode;
					}

					bool CBranchTreeNode::isRootNode() {
						return mRootNode == this;
					}


					cint64 CBranchTreeNode::getBranchingLevel() {
						return getBranchingTag();
					}


					Task::CSatisfiableCalculationTask* CBranchTreeNode::getSatisfiableCalculationTask() {
						return mSatCalcTask;
					}

					KONCLUCE_TASK_ALGORITHM_BRANCH_STRING_INSTRUCTION(CBranchTreeNode* CBranchTreeNode::setBranchString(QString* string) {\
						mBranchString = string;\
						return this;\
					})

				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
