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

#include "COptimizedComplexVariableConceptBaseItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableConceptBaseItem::COptimizedComplexVariableConceptBaseItem(COptimizedComplexConceptItem* conceptItem) {
				mConceptItem = conceptItem;
				mInstanceItemProcessingLinker = nullptr;
				mInstanceItemProcessedLinker = nullptr;
				mInstanceItemRetrievedLinker = nullptr;
			}


			COptimizedComplexVariableConceptBaseItem::~COptimizedComplexVariableConceptBaseItem() {
			}

			COptimizedComplexVariableConceptBaseItem::COMPOSITION_TYPE COptimizedComplexVariableConceptBaseItem::getCompositionType() {
				return CONCEPT_BASE;
			}


			COptimizedComplexConceptItem* COptimizedComplexVariableConceptBaseItem::getConceptItem() {
				return mConceptItem;
			}

			COptimizedComplexConceptInstanziatedIndividualItemLinker* COptimizedComplexVariableConceptBaseItem::getInstanceItemProcessingLinker() {
				return mInstanceItemProcessingLinker;
			}

			COptimizedComplexConceptInstanziatedIndividualItemLinker* COptimizedComplexVariableConceptBaseItem::getInstanceItemProcessedLinker() {
				return mInstanceItemProcessedLinker;
			}

			COptimizedComplexConceptInstanziatedIndividualItemLinker* COptimizedComplexVariableConceptBaseItem::getInstanceItemRetrievedLinker() {
				return mInstanceItemRetrievedLinker;
			}
			
			COptimizedComplexVariableConceptBaseItem* COptimizedComplexVariableConceptBaseItem::setInstanceItemProcessingLinker(COptimizedComplexConceptInstanziatedIndividualItemLinker* linker) {
				mInstanceItemProcessingLinker = linker;
				return this;
			}

			COptimizedComplexVariableConceptBaseItem* COptimizedComplexVariableConceptBaseItem::setInstanceItemProcessedLinker(COptimizedComplexConceptInstanziatedIndividualItemLinker* linker) {
				mInstanceItemProcessedLinker = linker;
				return this;
			}

			COptimizedComplexVariableConceptBaseItem* COptimizedComplexVariableConceptBaseItem::setInstanceItemRetrievedLinker(COptimizedComplexConceptInstanziatedIndividualItemLinker* linker) {
				mInstanceItemRetrievedLinker = linker;
				return this;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
