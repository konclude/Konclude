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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONPROCESSINGREALIZATIONITERATORDATAMEMORYMANAGED_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONPROCESSINGREALIZATIONITERATORDATAMEMORYMANAGED_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataBase.h"



// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {


	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged : public COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataBase {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged(CContext* context);


					CRealizationIndividualInstanceItemReference getPropagationInstanceItemReference();
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* initIteratorData(const CRealizationIndividualInstanceItemReference& propagationInstanceItemReference);


 
					CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* getVariableMappingPropagationIteratorLinker();
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* addVariableMappingPropagationIterator(COptimizedComplexVariableIndividualBindingsCardinalityLinker* iterator);
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* clearVariableMappingPropagationIterator();
					cint64 getVariableMappingIteratorCount();

					CXLinker<COptimizedComplexVariableIndividualUpdateCardinalityLinker*>* getCardinalityUpdatePropagationIteratorLinker();
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* addCardinalityUpdatePropagationIterator(COptimizedComplexVariableIndividualUpdateCardinalityLinker* iterator);
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* clearCardinalityUpdatePropagationIterator();

					CXLinker<CRealizationIndividualInstanceItemReference>* getPropagationInstanceItemLinker();
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* addPropagationInstanceItemLinker(CRealizationIndividualInstanceItemReference indiInstItemRef);
					cint64 getPropagationInstanceItemCount();




					bool isRealizationIterationInitialized();
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* setRealizationIterationInitialized(bool initialized);


				// protected methods
				protected:

				// protected variables
				protected:
					CContext* mContext;

					CRealizationIndividualInstanceItemReference mInstItemRef;

					bool mRealizationIterationInitialized;

					CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* mVariableMappingIteratorLinker;
					cint64 mVariableMappingIteratorCount;

					CXLinker<COptimizedComplexVariableIndividualUpdateCardinalityLinker*>* mUpdateCardinalityIteratorLinker;
					CXLinker<CRealizationIndividualInstanceItemReference>* mInstItemRolePropInstItemLinker;
					cint64 mInstItemRolePropInstItemCount;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONPROCESSINGREALIZATIONITERATORDATAMEMORYMANAGED_H
