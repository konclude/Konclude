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

#include "COptimizedComplexVariableIndividualTypeExtensionItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualTypeExtensionItem::COptimizedComplexVariableIndividualTypeExtensionItem(COptimizedComplexVariableCompositionItem* indiItem, COptimizedComplexVariableCompositionItem* classItem, cint64 extIndiVarId, cint64 extClassVarId) : COptimizedComplexVariableCompositionSingleDependenceItem(indiItem) {
				mIndiItem = indiItem;
				mClassItem = classItem;
				mExtIndiVarId = extIndiVarId;
				mExtClassVarId = extClassVarId;
				mScheduledRealizationCount = 0;
			}


			COptimizedComplexVariableIndividualTypeExtensionItem::~COptimizedComplexVariableIndividualTypeExtensionItem() {
			}

			COptimizedComplexVariableIndividualTypeExtensionItem::COMPOSITION_TYPE COptimizedComplexVariableIndividualTypeExtensionItem::getCompositionType() {
				return INDIVIDUAL_TYPE_EXTENSION;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableIndividualTypeExtensionItem::getIndividualVariableItem() {
				return mIndiItem;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableIndividualTypeExtensionItem::getClassVariableItem() {
				return mClassItem;
			}

			cint64 COptimizedComplexVariableIndividualTypeExtensionItem::getExtensionIndividualVariableId() {
				return mExtIndiVarId;
			}

			cint64 COptimizedComplexVariableIndividualTypeExtensionItem::getExtensionClassVariableId() {
				return mExtClassVarId;
			}



			bool COptimizedComplexVariableIndividualTypeExtensionItem::clearComputation() {
				COptimizedComplexVariableCompositionItem::clearComputation();
				return true;
			}

			QList<COptimizedComplexVariableIndividualTypeExtensionProcessingRealizationData>* COptimizedComplexVariableIndividualTypeExtensionItem::getWaitingProcessedRealizationBindingsCardinalityDataList() {
				return &mWaitingProcessedRealizationBindingsCardinalityDataList;
			}


			COptimizedComplexVariableIndividualTypeExtensionItem* COptimizedComplexVariableIndividualTypeExtensionItem::incScheduledRealizationCount(cint64 incCount) {
				mScheduledRealizationCount += incCount;
				return this;
			}

			COptimizedComplexVariableIndividualTypeExtensionItem* COptimizedComplexVariableIndividualTypeExtensionItem::decScheduledRealizationCount(cint64 decCount) {
				mScheduledRealizationCount -= decCount;
				return this;
			}

			cint64 COptimizedComplexVariableIndividualTypeExtensionItem::getScheduledRealizationCount() {
				return mScheduledRealizationCount;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
