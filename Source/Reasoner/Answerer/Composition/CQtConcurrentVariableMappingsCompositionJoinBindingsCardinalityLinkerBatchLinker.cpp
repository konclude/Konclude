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

#include "CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker::CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker() : CLinkerBase<COptimizedComplexVariableIndividualBindingsCardinalityLinker*, CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>(nullptr, nullptr) {
					mBatchBindingsCardinalityStartLinker = nullptr;
					mBatchBindingsCardinalityEndLinker = nullptr;

					mBatchCardinalityUpdateCurrentLinker = nullptr;
					mBatchCardinalityUpdateStartLinker = nullptr;
					mBatchCardinalityUpdateEndLinker = nullptr;

					mInserting = false;
					mLeftSide = false;
				}



				bool CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker::hasMoreLinkers() {
					return getNextBatchBindingsCardinalityLinker(false) || getNextCardinalityUpdateLinker(false);
				}


				CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker::setBatchBindingsCardinalityLinkers(COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindingsCardinalityStartLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindingsCardinalityEndLinker) {
					setData(batchBindingsCardinalityStartLinker);
					mBatchBindingsCardinalityStartLinker = batchBindingsCardinalityStartLinker;
					mBatchBindingsCardinalityEndLinker = batchBindingsCardinalityEndLinker;
					return this;
				}


				CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker::setBatchCardinalityUpdateLinkers(COptimizedComplexVariableIndividualUpdateCardinalityLinker* batchCardinalityUpdateStartLinker, COptimizedComplexVariableIndividualUpdateCardinalityLinker* batchCardinalityUpdateEndLinker) {
					mBatchCardinalityUpdateCurrentLinker = batchCardinalityUpdateStartLinker;
					mBatchCardinalityUpdateStartLinker = batchCardinalityUpdateStartLinker;
					mBatchCardinalityUpdateEndLinker = batchCardinalityUpdateEndLinker;
					return this;
				}



				COptimizedComplexVariableIndividualBindingsCardinalityLinker* CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker::getNextBatchBindingsCardinalityLinker(bool moveNext) {
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* tmpBatchBindingsCardinalityCurrentLinker = getData();
					if (tmpBatchBindingsCardinalityCurrentLinker && moveNext) {
						if (getData() == mBatchBindingsCardinalityEndLinker) {
							setData(nullptr);
						} else {
							setData(getData()->getNext());
						}

					}
					return tmpBatchBindingsCardinalityCurrentLinker;
				}


				COptimizedComplexVariableIndividualUpdateCardinalityLinker* CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker::getNextCardinalityUpdateLinker(bool moveNext) {
					COptimizedComplexVariableIndividualUpdateCardinalityLinker* tmpBatchCardinalityUpdateCurrentLinker = mBatchCardinalityUpdateCurrentLinker;
					if (tmpBatchCardinalityUpdateCurrentLinker && moveNext) {
						if (mBatchCardinalityUpdateCurrentLinker == mBatchCardinalityUpdateEndLinker) {
							mBatchCardinalityUpdateCurrentLinker = nullptr;
						} else {
							mBatchCardinalityUpdateCurrentLinker = mBatchCardinalityUpdateCurrentLinker->getNext();
						}
					}
					return tmpBatchCardinalityUpdateCurrentLinker;
				}

				bool CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker::isInserting() {
					return mInserting;
				}

				bool CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker::isLeftSide() {
					return mLeftSide;
				}


				CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker::setInserting(bool inserting) {
					mInserting = inserting;
					return this;
				}

				CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker::setLeftSide(bool leftSide) {
					mLeftSide = leftSide;
					return this;
				}



			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
