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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONABSTRACTITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONABSTRACTITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableCompositionItem.h"
#include "COptimizedComplexConceptItem.h"
#include "COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData.h"



// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {


	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableRolePropagationAbstractItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableRolePropagationAbstractItem : public COptimizedComplexVariableCompositionItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableRolePropagationAbstractItem(COptimizedComplexVariableCompositionItem* baseItem, CRole* role, bool inversed, cint64 propVarIdx);

					virtual ~COptimizedComplexVariableRolePropagationAbstractItem();

					
					COptimizedComplexVariableCompositionItem* getBaseItem();
					CRole* getRole();
					bool getInversed();
					cint64 getPropagationVariableIndex();


					COptimizedComplexVariableIndividualMapping::const_iterator getVariableMappingPropagationIterator();
					COptimizedComplexVariableRolePropagationAbstractItem* setVariableMappingPropagationIterator(COptimizedComplexVariableIndividualMapping::const_iterator iterator);

					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* getWaitingProcessedRealizationIteratorDataList();
					QHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* getInstanceItemRolePropagationInstanceIterationDataHash();


				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableCompositionItem* mBaseItem;
					CRole* mRole;
					bool mInversed;
					cint64 mPropagationVarIdx;

					COptimizedComplexVariableIndividualMapping::const_iterator mVariableMappingIterator;
					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*> mWaitingProcessedRealizationIteratorDataList;
					QHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*> mInstItemRolePropInstIterationDataHash;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONABSTRACTITEM_H
