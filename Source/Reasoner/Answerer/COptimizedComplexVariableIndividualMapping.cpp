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

#include "COptimizedComplexVariableIndividualMapping.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualMapping::COptimizedComplexVariableIndividualMapping(cint64 bindingSize) {
				mBindingSize = bindingSize;
				mBindingMapping = new VARIABLE_TYPE[bindingSize];
				mLastAddedBindingsCardinalityLinker = nullptr;
			}


			COptimizedComplexVariableIndividualMapping::~COptimizedComplexVariableIndividualMapping() {
				delete[] mBindingMapping;
			}


			COptimizedComplexVariableIndividualMapping::VARIABLE_TYPE COptimizedComplexVariableIndividualMapping::getBindingMapping(cint64 idx) {
				return mBindingMapping[idx];
			}

			COptimizedComplexVariableIndividualMapping* COptimizedComplexVariableIndividualMapping::setBindingMapping(cint64 idx, COptimizedComplexVariableIndividualMapping::VARIABLE_TYPE varType) {
				mBindingMapping[idx] = varType;
				return this;
			}

			cint64 COptimizedComplexVariableIndividualMapping::getBindingSize() {
				return mBindingSize;
			}

			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualMapping::getLastAddedBindingsCardinalityLinker() {
				return mLastAddedBindingsCardinalityLinker;
			}

			COptimizedComplexVariableIndividualMapping* COptimizedComplexVariableIndividualMapping::setLastAddedBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker) {
				mLastAddedBindingsCardinalityLinker = linker->append(mLastAddedBindingsCardinalityLinker);
				return this;
			}

			bool COptimizedComplexVariableIndividualMapping::addInsertingBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker, bool addCardinalitiesIfAlreadyPresent) {
				COptimizedComplexVariableIndividualBindingsHasher hasher(linker);
				COptimizedComplexVariableIndividualBindingsCardinalityLinker*& hashedLinker = QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>::operator[](hasher);
				if (!hashedLinker) {
					hashedLinker = linker;
					setLastAddedBindingsCardinalityLinker(linker);
					return true;
				} else if (addCardinalitiesIfAlreadyPresent) {
					COptimizedComplexVariableIndividualBindingsCardinality* cardinalities = linker->getCardinalities();
					if (cardinalities) {
						hashedLinker->getCardinalities()->addCardinalities(cardinalities);
					}
				}
				return false;
			}


			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualMapping::takeBindingsCardinalityLinker() {
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = mLastAddedBindingsCardinalityLinker;
				if (mLastAddedBindingsCardinalityLinker) {
					mLastAddedBindingsCardinalityLinker = mLastAddedBindingsCardinalityLinker->getNext();
				}
				if (linker) {
					remove(linker);
				}
				return linker;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
