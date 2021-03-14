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

#include "CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {


				CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker::CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker() : CLinkerBase<COptimizedComplexVariableIndividualBindings*, CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>(nullptr, nullptr) {
					mNewCardinalites = nullptr;
					mPrevCardinalites = nullptr;
					mKeyIndex = 0;
				}


				CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker* CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker::initMappedKeysBindingsCardinalityData(CRealizationIndividualInstanceItemReference instItemRef, cint64 mappedKeyIndex, COptimizedComplexVariableIndividualBindings* varMapping, COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites) {
					setData(varMapping);
					mNewCardinalites = newCardinalites;
					mPrevCardinalites = prevCardinalites;
					mInstItemRef = instItemRef;
					mKeyIndex = mappedKeyIndex;
					return this;
				}


				cint64 CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker::getMappedKeyIndex() {
					return mKeyIndex;
				}


				COptimizedComplexVariableIndividualBindings* CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker::getVariableMapping() {
					return getData();
				}

				COptimizedComplexVariableIndividualBindingsCardinality* CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker::getNewCardinalites() {
					return mNewCardinalites;
				}

				COptimizedComplexVariableIndividualBindingsCardinality* CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker::getPreviousCardinalites() {
					return mPrevCardinalites;
				}

				CRealizationIndividualInstanceItemReference CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker::getInstanceItemReference() {
					return mInstItemRef;
				}


			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
