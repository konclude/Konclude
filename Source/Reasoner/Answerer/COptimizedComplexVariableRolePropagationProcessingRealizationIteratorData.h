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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONPROCESSINGREALIZATIONITERATORDATA_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONPROCESSINGREALIZATIONITERATORDATA_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableCompositionItem.h"
#include "COptimizedComplexConceptItem.h"



// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {


	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData();


					QList<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* getVariableMappingPropagationIteratorList();
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* addVariableMappingPropagationIterator(COptimizedComplexVariableIndividualBindingsCardinalityLinker* iterator);


					QList<COptimizedComplexVariableIndividualUpdateCardinalityLinker*>* getCardinalityUpdatePropagationIteratorList();
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* addCardinalityUpdatePropagationIterator(COptimizedComplexVariableIndividualUpdateCardinalityLinker* iterator);


					CRealizationIndividualInstanceItemReferenceIterator* getRealizationIterator();
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* setRealizationIterator(CRealizationIndividualInstanceItemReferenceIterator* iterator);

					QSet<CRealizationIndividualInstanceItemReference>* getPropagationInstanceItemSet();


					bool isRealizationScheduled();
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* setRealizationScheduled(bool scheduled);


					cint64 getExpectedFillerCount();
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* setExpectedFillerCount(cint64 count);


					//CRealizationIndividualInstanceItemReference debugPropInstItemRef;
				// protected methods
				protected:

				// protected variables
				protected:
					QList<COptimizedComplexVariableIndividualBindingsCardinalityLinker*> mVariableMappingIteratorList;
					QList<COptimizedComplexVariableIndividualUpdateCardinalityLinker*> mUpdateCardinalityIteratorList;
					CRealizationIndividualInstanceItemReferenceIterator* mRealizationIterator;
					QSet<CRealizationIndividualInstanceItemReference> mInstItemRolePropInstItemSet;

					bool mRealizationScheduled;


					cint64 mExpectedFillerCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONPROCESSINGREALIZATIONITERATORDATA_H
