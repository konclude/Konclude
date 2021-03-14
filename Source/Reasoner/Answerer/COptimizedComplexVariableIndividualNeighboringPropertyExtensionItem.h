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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALNEIGHBORINGPROPERTYEXTENSIONITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALNEIGHBORINGPROPERTYEXTENSIONITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableCompositionSingleDependenceItem.h"
#include "COptimizedComplexConceptItem.h"
#include "COptimizedComplexConceptInstanziatedIndividualItemLinker.h"
#include "COptimizedComplexVariableIndividualPropertyExtensionProcessingRealizationData.h"



// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {


	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem : public COptimizedComplexVariableCompositionSingleDependenceItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem(COptimizedComplexVariableCompositionItem* indiItem, COptimizedComplexVariableCompositionItem* classItem, cint64 extIndiVarId, cint64 extPropVarId);

					virtual ~COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem();

					virtual COMPOSITION_TYPE getCompositionType();

					COptimizedComplexVariableCompositionItem* getIndividualVariableItem();
					COptimizedComplexVariableCompositionItem* getPropertyVariableItem();
					cint64 getExtensionIndividualVariableId();
					cint64 getExtensionPropertyVariableId();


					virtual bool clearComputation();

					QList<COptimizedComplexVariableIndividualPropertyExtensionProcessingRealizationData>* getWaitingProcessedRealizationBindingsCardinalityDataList();


					COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem* incScheduledRealizationCount(cint64 incCount = 1);
					COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem* decScheduledRealizationCount(cint64 decCount = 1);
					cint64 getScheduledRealizationCount();

				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableCompositionItem* mIndiItem;
					COptimizedComplexVariableCompositionItem* mPropertyItem;
					cint64 mExtIndiVarId;
					cint64 mExtPropertyVarId;

					QList<COptimizedComplexVariableIndividualPropertyExtensionProcessingRealizationData> mWaitingProcessedRealizationBindingsCardinalityDataList;

					cint64 mScheduledRealizationCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALNEIGHBORINGPROPERTYEXTENSIONITEM_H
