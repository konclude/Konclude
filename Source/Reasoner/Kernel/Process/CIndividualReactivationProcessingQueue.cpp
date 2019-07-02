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

#include "CIndividualReactivationProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualReactivationProcessingQueue::CIndividualReactivationProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext),mPriorityIndiReactivationMap(processContext) {

				}

				CIndividualReactivationProcessingQueue* CIndividualReactivationProcessingQueue::initProcessingQueue(CIndividualReactivationProcessingQueue* processingQueue) {
					if (processingQueue) {
						mPriorityIndiReactivationMap = processingQueue->mPriorityIndiReactivationMap;
					} else {
						mPriorityIndiReactivationMap.clear();
					}
					return this;
				}


				bool CIndividualReactivationProcessingQueue::takeNextReactivationIndividual(CIndividualProcessNode*& indiNode, bool& forceReactivation) {
					CIndividualProcessNode* nextNode = nullptr;
					if (!mPriorityIndiReactivationMap.isEmpty()) {
						CPROCESSMAP<CIndividualDepthPriority,CIndividualForceReactivationData>::iterator it = mPriorityIndiReactivationMap.begin();
						CIndividualForceReactivationData& data = it.value();
						forceReactivation = data.mForceReactivation;
						indiNode = data.mIndiNode;
						it = mPriorityIndiReactivationMap.erase(it);
						return true;
					}
					return false;
				}

				bool CIndividualReactivationProcessingQueue::getNextReactivationIndividual(CIndividualProcessNode*& indiNode, bool& forceReactivation) {
					CIndividualProcessNode* nextNode = nullptr;
					if (!mPriorityIndiReactivationMap.isEmpty()) {
						CPROCESSMAP<CIndividualDepthPriority,CIndividualForceReactivationData>::iterator it = mPriorityIndiReactivationMap.begin();
						CIndividualForceReactivationData& data = it.value();
						forceReactivation = data.mForceReactivation;
						indiNode = data.mIndiNode;
						return true;
					}
					return false;
				}


				bool CIndividualReactivationProcessingQueue::hasQueuedIndividual(CIndividualProcessNode* individual) {
					CIndividualProcessNode* nextNode = nullptr;
					if (!mPriorityIndiReactivationMap.isEmpty()) {
						CIndividualDepthPriority priority(individual->getIndividualNominalLevelOrAncestorDepth(),individual->getIndividualID());
						return mPriorityIndiReactivationMap.contains(priority);
					}
					return false;
				}


				bool CIndividualReactivationProcessingQueue::insertReactivationIndiviudal(CIndividualProcessNode* individual, bool forceReactivation) {
					CIndividualDepthPriority priority(individual->getIndividualNominalLevelOrAncestorDepth(),individual->getIndividualID());
					CIndividualForceReactivationData& inFoReDa = mPriorityIndiReactivationMap[priority];
					bool newEntry = inFoReDa.mIndiNode == nullptr || !inFoReDa.mForceReactivation && forceReactivation;
					inFoReDa.mForceReactivation = forceReactivation;
					inFoReDa.mIndiNode = individual;
					return newEntry;
				}


				bool CIndividualReactivationProcessingQueue::isEmpty() {
					return mPriorityIndiReactivationMap.count() <= 0;
				}

				cint64 CIndividualReactivationProcessingQueue::getQueuedIndividualCount() {
					return mPriorityIndiReactivationMap.count();
				}

				bool CIndividualReactivationProcessingQueue::hasQueuedIndividuals() {
					return mPriorityIndiReactivationMap.count() > 0;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
