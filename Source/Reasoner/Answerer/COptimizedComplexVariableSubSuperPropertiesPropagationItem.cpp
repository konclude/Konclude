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

#include "COptimizedComplexVariableSubSuperPropertiesPropagationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableSubSuperPropertiesPropagationItem::COptimizedComplexVariableSubSuperPropertiesPropagationItem(COptimizedComplexVariableCompositionItem* baseItem, bool superPropertiesPropagation, cint64 varIdx) : COptimizedComplexVariableCompositionSingleDependenceItem(baseItem) {
				mBaseItem = baseItem;
				mSuperPropertiesProp = superPropertiesPropagation;
				mVarIdx = varIdx;
			}


			COptimizedComplexVariableSubSuperPropertiesPropagationItem::~COptimizedComplexVariableSubSuperPropertiesPropagationItem() {
			}


			COptimizedComplexVariableSubSuperPropertiesPropagationItem::COMPOSITION_TYPE COptimizedComplexVariableSubSuperPropertiesPropagationItem::getCompositionType() {
				return SUB_SUPER_PROPERTIES_PROPAGATION;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableSubSuperPropertiesPropagationItem::getBaseItem() {
				return mBaseItem;
			}

			bool COptimizedComplexVariableSubSuperPropertiesPropagationItem::isSuperPropertiesPropagation() {
				return mSuperPropertiesProp;
			}

			cint64 COptimizedComplexVariableSubSuperPropertiesPropagationItem::getVariableIndex() {
				return mVarIdx;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
