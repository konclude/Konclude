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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSORPTIONBASEDHANDLINGEXTENSIONITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSORPTIONBASEDHANDLINGEXTENSIONITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableCompositionItem.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"
#include "CAnsweringSplitEntailmentTestingCancellationAdapter.h"
#include "CAnsweringSplitPropagationTestControllingItem.h"



// Other includes
#include "Parser/Expressions/CExpressionVariable.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem : public COptimizedComplexVariableCompositionItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionData);

					virtual ~COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem();

					virtual COMPOSITION_TYPE getCompositionType();

					
					QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*>* getVariableExpressionCompositionItemHash();

					COptimizedComplexVariableCompositionItem* getVariableCompositionItem(CExpressionVariable* variableExpression);
					COptimizedComplexVariableCompositionItem* getVariableCompositionItem(CVariable* variable);
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* getAbsorptionBasedHandlingData();


					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* setVariableExpressionCompositionItem(CExpressionVariable* variableExpression, COptimizedComplexVariableCompositionItem* compositionItem);
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* setVariableCompositionItem(CVariable* variable, COptimizedComplexVariableCompositionItem* compositionItem);


					QSet<CIndividualReference>*& getVariableSteeringIndividualBindingSet(CVariable* variable);

					QSet<COptimizedComplexVariableCompositionItem*>* getIntegratedVariableCompositionItemSet();
					QList< QPair<COptimizedComplexVariableCompositionItem*, CExpressionVariable*> >* getExtendingCompositionItemAssociatedVariablePairList();

					QHash<CVariable*, cint64>* getVariableIndexHash();


					COptimizedComplexVariableIndividualMappings* getPossibleVariableMapping();
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* setPossibleVariableMapping(COptimizedComplexVariableIndividualMappings* possibleVariableMapping);

					QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* getTestingVariableMapping();
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* setTestingVariableMapping(QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* testingVariableMapping);


					bool isPropatationTestCreated();
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* setPropatationTestCreated(bool created);

					bool isDependentMappingsComputationSchedluled();
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* setDependentMappingsComputationSchedluled(bool scheduled);

					bool isPropagationInitializationIndividualsSplitted();
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* setPropagationInitializationIndividualsSplitted(bool splitted);
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* addPropagationSteeringController(CAnsweringPropagationSteeringController* propagationSteeringController);
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* removePropagationSteeringController(CAnsweringPropagationSteeringController* propagationSteeringController);

					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* setPropagationInitializationIndividualsSplittedTestsRemaining(bool remainingTests);
					bool hasRemainingPropagationInitializationIndividualsSplittedTests();
					cint64 getNextPropagationInitializationIndividualsSplittedTestId(cint64 incCount = 1);


					bool isEntailmentFound();
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* setEntailmentFound(bool entailmentFound);


					CAnsweringSplitEntailmentTestingCancellationAdapter* getEntailmentTestingCancellationAdapter();

					CAnsweringSplitPropagationTestControllingItem* getNextPreparedSplitPropagationTestControllingItem();
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* addReaminingPreparedSplitPropagationTestControllingItem(CAnsweringSplitPropagationTestControllingItem* item);
					bool hasPreparedSplitPropagationTestControllingItems();
					bool hasRemainingPreparedSplitPropagationTestControllingItems();
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* initPreparedSplitPropagationTestControllingItemSize(cint64 preparedSplitSize);
					CAnsweringSplitPropagationTestControllingItem*& getPreparedSplitPropagationTestControllingItem(cint64 preparedSplitId);

					CAnsweringSplitPropagationTestControllingItem* getNextSplitPropagationTestControllingItem();
					CAnsweringSplitPropagationTestControllingItem* getSplitPropagationTestingItem(cint64 splitItemId);

					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* addRemainingRepeatedSplitTestingItem(CAnsweringSplitPropagationTestControllingItem* item);
					QList<CAnsweringSplitPropagationTestControllingItem*>* getRemainingRepeatedSplitTestingItemList();

					bool isSplitPropagationItemProcessingQueued();
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* setSplitPropagationItemProcessingQueued(bool queued);

					virtual bool clearComputation();

				// protected methods
				protected:
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* addSplitPropagationTestingItem(cint64 splitItemId, CAnsweringSplitPropagationTestControllingItem* item);

				// protected variables
				protected:
					QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*> mVariableExpressionBaseItem;
					QHash<CVariable*, COptimizedComplexVariableCompositionItem*> mVariableBaseItem;
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* mAbsorptionData;

					QHash<CVariable*, QSet<CIndividualReference>* > mVariableIndividualBindingSet;

					QSet<COptimizedComplexVariableCompositionItem*> mIntegratedItemSet;

					QList< QPair<COptimizedComplexVariableCompositionItem*, CExpressionVariable*> > mVariableExtendingItemList;


					QHash<CVariable*, cint64> mVariableIndexHash;


					COptimizedComplexVariableIndividualMappings* mPossibleVariableMapping;
					QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* mTestingVariableMapping;

					bool mPropatationTestCreated;
					bool mDependentMappingsComputationScheduled;
					bool mPropagationInitializationIndividualsSplitted;
					bool mPropagationInitializationIndividualsSplittedTestsRemaining;
					QSet<CAnsweringPropagationSteeringController*> mPropagationSteeringControllerSet;

					cint64 mNextPropagationInitializationIndividualsSplittedTestId;

					bool mEntailmentFound;

					CAnsweringSplitEntailmentTestingCancellationAdapter* mCancellationAdapter;

					QHash<cint64, CAnsweringSplitPropagationTestControllingItem*> mSplitPropagationTestingItemHash;
					cint64 mNextSplitPropagationTestingItemId;

					QList<CAnsweringSplitPropagationTestControllingItem*> mRemainingRepeatedSplitTestingItemList;

					QVector<CAnsweringSplitPropagationTestControllingItem*> mPreparedSplitTestingItemVec;
					QList<CAnsweringSplitPropagationTestControllingItem*> mRemainingPreparedSplitTestingItemList;

					bool mSplitPropagationItemProcessingQueued;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSORPTIONBASEDHANDLINGEXTENSIONITEM_H
