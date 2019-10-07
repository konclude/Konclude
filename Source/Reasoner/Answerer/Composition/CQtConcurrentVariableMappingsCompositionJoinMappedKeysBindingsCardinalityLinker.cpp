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

#include "CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {


				CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker() : mBindingHasher(nullptr, nullptr), CLinkerBase<COptimizedComplexVariableIndividualBindings*, CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>(nullptr, nullptr) {
					mNewCardinalites = nullptr;
					mPrevCardinalites = nullptr;

					mInserting = false;
					mLeftSide = false;

					mKeyIndex = 0;
				}


				CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker* CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::initMappedKeysBindingsCardinalityData(const COptimizedComplexVariableJoiningHasher& bindingHasher, cint64 mappedKeyIndex, COptimizedComplexVariableIndividualBindings* varMapping, COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites, bool leftSide, bool inserting) {
					setData(varMapping);
					mNewCardinalites = newCardinalites;
					mPrevCardinalites = prevCardinalites;
					mInserting = inserting;
					mLeftSide = leftSide;
					mBindingHasher = bindingHasher;
					mKeyIndex = mappedKeyIndex;
					return this;
				}


				cint64 CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::getMappedKeyIndex() {
					return mKeyIndex;
				}


				COptimizedComplexVariableIndividualBindings* CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::getVariableMapping() {
					return getData();
				}

				COptimizedComplexVariableIndividualBindingsCardinality* CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::getNewCardinalites() {
					return mNewCardinalites;
				}

				COptimizedComplexVariableIndividualBindingsCardinality* CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::getPreviousCardinalites() {
					return mPrevCardinalites;
				}


				bool CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::isInserting() {
					return mInserting;
				}

				bool CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::isLeftSide() {
					return mLeftSide;
				}


				CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker* CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::setInserting(bool inserting) {
					mInserting = inserting;
					return this;
				}

				CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker* CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::setLeftSide(bool leftSide) {
					mLeftSide = leftSide;
					return this;
				}

				COptimizedComplexVariableJoiningHasher CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker::getBindingHasher() {
					return mBindingHasher;
				}


			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
