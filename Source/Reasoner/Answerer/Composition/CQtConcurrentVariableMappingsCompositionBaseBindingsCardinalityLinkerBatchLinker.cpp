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

#include "CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker::CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker() : CLinkerBase<COptimizedComplexVariableIndividualBindingsCardinalityLinker*, CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker>(nullptr, nullptr) {
					mBatchBindingsCardinalityStartLinker = nullptr;
					mBatchBindingsCardinalityEndLinker = nullptr;

					mBatchCardinalityUpdateCurrentLinker = nullptr;
					mBatchCardinalityUpdateStartLinker = nullptr;
					mBatchCardinalityUpdateEndLinker = nullptr;
				}



				bool CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker::hasMoreLinkers() {
					return getNextBatchBindingsCardinalityLinker(false) || getNextCardinalityUpdateLinker(false);
				}


				CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker* CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker::setBatchBindingsCardinalityLinkers(COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindingsCardinalityStartLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindingsCardinalityEndLinker) {
					setData(batchBindingsCardinalityStartLinker);
					mBatchBindingsCardinalityStartLinker = batchBindingsCardinalityStartLinker;
					mBatchBindingsCardinalityEndLinker = batchBindingsCardinalityEndLinker;
					return this;
				}


				CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker* CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker::setBatchCardinalityUpdateLinkers(COptimizedComplexVariableIndividualUpdateCardinalityLinker* batchCardinalityUpdateStartLinker, COptimizedComplexVariableIndividualUpdateCardinalityLinker* batchCardinalityUpdateEndLinker) {
					mBatchCardinalityUpdateCurrentLinker = batchCardinalityUpdateStartLinker;
					mBatchCardinalityUpdateStartLinker = batchCardinalityUpdateStartLinker;
					mBatchCardinalityUpdateEndLinker = batchCardinalityUpdateEndLinker;
					return this;
				}



				COptimizedComplexVariableIndividualBindingsCardinalityLinker* CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker::getNextBatchBindingsCardinalityLinker(bool moveNext) {
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


				COptimizedComplexVariableIndividualUpdateCardinalityLinker* CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker::getNextCardinalityUpdateLinker(bool moveNext) {
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




			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
