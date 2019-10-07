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

#include "COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker::COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker() {
				initBindingsCardinalityBatchLinker(nullptr, nullptr, 0);
			}

			COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker::COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* startBindingsCardLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker* endBindingsCardLinker, cint64 linkerCount) {
				initBindingsCardinalityBatchLinker(startBindingsCardLinker, endBindingsCardLinker, linkerCount);
			}

			COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker::initBindingsCardinalityBatchLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* startBindingsCardLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker* endBindingsCardLinker, cint64 linkerCount) {
				mStartBindingsCardLinker = startBindingsCardLinker;
				mEndBindingsCardLinker = endBindingsCardLinker;
				setData(linkerCount);
				return this;
			}


			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker::getStartBindingsCardinalityLinker() const {
				return mStartBindingsCardLinker;
			}

			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker::getEndBindingsCardinalityLinker() const {
				return mEndBindingsCardLinker;
			}

			cint64 COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker::getLinkerCount() {
				return getData();
			}


			COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker::addStartBindingsCardLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* startBindingsCardLinker, cint64 addedCount) {
				setData(addedCount + getData());
				mStartBindingsCardLinker = startBindingsCardLinker;
				return this;
			}




		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
