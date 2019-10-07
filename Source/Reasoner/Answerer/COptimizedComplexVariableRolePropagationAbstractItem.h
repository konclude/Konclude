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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONABSTRACTITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONABSTRACTITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableCompositionSingleDependenceItem.h"
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
			class COptimizedComplexVariableRolePropagationAbstractItem : public COptimizedComplexVariableCompositionSingleDependenceItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableRolePropagationAbstractItem(COptimizedComplexVariableCompositionItem* baseItem, CRole* role, bool inversed, cint64 propVarIdx);

					virtual ~COptimizedComplexVariableRolePropagationAbstractItem();

					
					COptimizedComplexVariableCompositionItem* getBaseItem();
					CRole* getRole();
					bool getInversed();
					cint64 getPropagationVariableIndex();



					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* getWaitingProcessedRealizationIteratorDataList();
					QHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* getInstanceItemRolePropagationInstanceIterationDataHash();

					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* getSchedulingRealizationIteratorDataList();
					QList<COntologyProcessingRequirement*>* geSchedulingRealizationRequirementIteratorDataList();


					cint64 getFillerInstanceItemCount();
					cint64 getPropagationInstanceItemCount();
					cint64 getRealizationFinishedFillerInstanceItemCount();
					cint64 getPropagationHandledInstanceItemCount();
					cint64 getPropagatedInstanceItemCount();
					cint64 getScheduledRealizationCount();



					COptimizedComplexVariableRolePropagationAbstractItem* setFillerInstanceItemCount(cint64 count);
					COptimizedComplexVariableRolePropagationAbstractItem* setRealizationFinishedFillerInstanceItemCount(cint64 count);
					COptimizedComplexVariableRolePropagationAbstractItem* setPropagationHandledInstanceItemCount(cint64 count);
					COptimizedComplexVariableRolePropagationAbstractItem* setScheduledRealizationCount(cint64 count);


					COptimizedComplexVariableRolePropagationAbstractItem* incFillerInstanceItemCount(cint64 count = 1);
					COptimizedComplexVariableRolePropagationAbstractItem* incRealizationFinishedFillerInstanceItemCount(cint64 count = 1);
					COptimizedComplexVariableRolePropagationAbstractItem* incPropagationHandledInstanceItemCount(cint64 count = 1);
					COptimizedComplexVariableRolePropagationAbstractItem* incScheduledRealizationCount(cint64 count = 1);
					COptimizedComplexVariableRolePropagationAbstractItem* decScheduledRealizationCount(cint64 count = 1);
					COptimizedComplexVariableRolePropagationAbstractItem* incPropagatedInstanceItemCount(cint64 count = 1);


					COptimizedComplexVariableRolePropagationAbstractItem* incExpectedFillerAllPropagationItemCount(double count = 1);
					double getExpectedFillerAllPropagationItemCount();
					double getExpectedFillerPerPropagationItemCount();


				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableCompositionItem* mBaseItem;
					CRole* mRole;
					bool mInversed;
					cint64 mPropagationVarIdx;

					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*> mWaitingProcessedRealizationIteratorDataList;
					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*> mSchedulingRealizationIteratorDataList;
					QHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*> mInstItemRolePropInstIterationDataHash;

					QList<COntologyProcessingRequirement*> mSchedulingRealizationRequirementIteratorDataList;

					cint64 mFillerInstanceItemCount;

					cint64 mRealizationFinishedFillerInstanceItemCount;
					cint64 mPropagationHandledInstanceItemCount;

					cint64 mScheduledRealizationCount;

					double mExpectedFillerAllPropagationItemCount;

					cint64 mPropagatedInstanceItemCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONABSTRACTITEM_H
