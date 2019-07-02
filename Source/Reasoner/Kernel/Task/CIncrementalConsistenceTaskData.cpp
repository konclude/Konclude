/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CIncrementalConsistenceTaskData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				CIncrementalConsistenceTaskData::CIncrementalConsistenceTaskData(CSatisfiableCalculationTask* detSatTask, CSatisfiableCalculationTask* graphCachedSatTask, CConcreteOntology* prevOntology, CConsistenceTaskData* prevConsData) : CConsistenceTaskData(detSatTask,graphCachedSatTask) {
					mPrevOntology = prevOntology;
					mPrevConsData = prevConsData;
					mTotalNodeCount = 0;
					mAddedNodeCount = 0;
					mPreviousNodeCount = 0;
				}


				CIncrementalConsistenceTaskData::~CIncrementalConsistenceTaskData() {
				}


				QSet<cint64>* CIncrementalConsistenceTaskData::getIndirectlyChangedNodeSet() {
					return &mIndirectlyChangedNodeSet;
				}

				QSet<cint64>* CIncrementalConsistenceTaskData::getDeterministicallyChangedNodeSet() {
					return &mDeterministicallyChangedNodeSet;
				}


				QSet<cint64>* CIncrementalConsistenceTaskData::getChangedCompatibleNodeSet() {
					return &mChangedCompatibleNodeSet;
				}

				CConcreteOntology* CIncrementalConsistenceTaskData::getPreviousOntology() {
					return mPrevOntology;
				}

				CConsistenceTaskData* CIncrementalConsistenceTaskData::getPreviousConsistenceData() {
					return mPrevConsData;
				}

				CIncrementalConsistenceTaskData* CIncrementalConsistenceTaskData::setPreviousOntology(CConcreteOntology* prevOntology) {
					mPrevOntology = prevOntology;
					return this;
				}

				CIncrementalConsistenceTaskData* CIncrementalConsistenceTaskData::setPreviousConsistenceData(CConsistenceTaskData* prevConsData) {
					mPrevConsData = prevConsData;
					return this;
				}

				cint64 CIncrementalConsistenceTaskData::getAddedNodeCount() {
					return mAddedNodeCount;
				}

				cint64 CIncrementalConsistenceTaskData::getTotalNodeCount() {
					return mTotalNodeCount;
				}

				cint64 CIncrementalConsistenceTaskData::getPreviousNodeCount() {
					return mPreviousNodeCount;
				}

				cint64 CIncrementalConsistenceTaskData::getChangedCompatibleNodeCount() {
					return mChangedCompatibleNodeSet.count();
				}

				cint64 CIncrementalConsistenceTaskData::getDeterministicallyChangedNodeCount() {
					return mDeterministicallyChangedNodeSet.count();
				}

				cint64 CIncrementalConsistenceTaskData::getIndirectlyChangedCount() {
					return mIndirectlyChangedNodeSet.count();
				}


				CIncrementalConsistenceTaskData* CIncrementalConsistenceTaskData::setAddedNodeCount(cint64 addedNodeCount) {
					mAddedNodeCount = addedNodeCount;
					return this;
				}

				CIncrementalConsistenceTaskData* CIncrementalConsistenceTaskData::setTotalNodeCount(cint64 totalNodeCount) {
					mTotalNodeCount = totalNodeCount;
					return this;
				}

				CIncrementalConsistenceTaskData* CIncrementalConsistenceTaskData::setPreviousNodeCount(cint64 prevNodeCount) {
					mPreviousNodeCount = prevNodeCount;
					return this;
				}

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
