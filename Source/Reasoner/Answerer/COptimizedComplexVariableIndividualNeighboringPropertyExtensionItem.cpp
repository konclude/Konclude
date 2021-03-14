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

#include "COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem(COptimizedComplexVariableCompositionItem* indiItem, COptimizedComplexVariableCompositionItem* propertyItem, cint64 extIndiVarId, cint64 extPropertyVarId) : COptimizedComplexVariableCompositionSingleDependenceItem(indiItem) {
				mIndiItem = indiItem;
				mPropertyItem = propertyItem;
				mExtIndiVarId = extIndiVarId;
				mExtPropertyVarId = extPropertyVarId;
				mScheduledRealizationCount = 0;
			}


			COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::~COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem() {
			}

			COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::COMPOSITION_TYPE COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::getCompositionType() {
				return INDIVIDUAL_PROPERTY_EXTENSION;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::getIndividualVariableItem() {
				return mIndiItem;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::getPropertyVariableItem() {
				return mPropertyItem;
			}

			cint64 COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::getExtensionIndividualVariableId() {
				return mExtIndiVarId;
			}

			cint64 COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::getExtensionPropertyVariableId() {
				return mExtPropertyVarId;
			}



			bool COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::clearComputation() {
				COptimizedComplexVariableCompositionItem::clearComputation();
				return true;
			}

			QList<COptimizedComplexVariableIndividualPropertyExtensionProcessingRealizationData>* COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::getWaitingProcessedRealizationBindingsCardinalityDataList() {
				return &mWaitingProcessedRealizationBindingsCardinalityDataList;
			}


			COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem* COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::incScheduledRealizationCount(cint64 incCount) {
				mScheduledRealizationCount += incCount;
				return this;
			}

			COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem* COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::decScheduledRealizationCount(cint64 decCount) {
				mScheduledRealizationCount -= decCount;
				return this;
			}

			cint64 COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem::getScheduledRealizationCount() {
				return mScheduledRealizationCount;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
