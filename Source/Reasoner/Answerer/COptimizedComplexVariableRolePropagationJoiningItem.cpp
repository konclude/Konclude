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

#include "COptimizedComplexVariableRolePropagationJoiningItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableRolePropagationJoiningItem::COptimizedComplexVariableRolePropagationJoiningItem(COptimizedComplexVariableCompositionItem* baseItem, CRole* role, bool inversed, cint64 propVarIdx, cint64 joinVarIdx) : COptimizedComplexVariableRolePropagationAbstractItem(baseItem, role, inversed, propVarIdx) {
				mJoinVarIdx = joinVarIdx;
			}


			COptimizedComplexVariableRolePropagationJoiningItem::~COptimizedComplexVariableRolePropagationJoiningItem() {
			}


			COptimizedComplexVariableRolePropagationJoiningItem::COMPOSITION_TYPE COptimizedComplexVariableRolePropagationJoiningItem::getCompositionType() {
				return ROLE_PROPAGATION_JOIN;
			}


			cint64 COptimizedComplexVariableRolePropagationJoiningItem::getJoiningVariableIndex() {
				return mJoinVarIdx;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
