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

#ifndef KONCLUDE_REASONER_ANSWERER_COptimizedComplexBuildingVariableCompositionsItem_H
#define KONCLUDE_REASONER_ANSWERER_COptimizedComplexBuildingVariableCompositionsItem_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexConceptItem.h"
#include "COptimizedComplexVariableCompositionItemVariableExpressionMapping.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem.h"
#include "CRequirementWaitingDependencyData.h"
#include "CComplexQueryMaterializationData.h"
#include "CComplexQueryFinishingBuildingVariableCompositionsItemData.h"



// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/OntologySettings.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Parser/Expressions/CIndividualVariableExpression.h"
#include "Parser/Expressions/CObjectPropertyAssertionExpression.h"
#include "Parser/Expressions/CDataLiteralVariableExpression.h"
#include "Parser/Expressions/CDataPropertyTermExpression.h"
#include "Parser/Expressions/CDataPropertyAssertionExpression.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexBuildingVariableCompositionsItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexBuildingVariableCompositionsItem : public CRequirementWaitingDependencyData {
				// public methods
				public:
					//! Constructor
					COptimizedComplexBuildingVariableCompositionsItem(CComplexQueryExpressionProcessingData* queryProcessingData);
					virtual ~COptimizedComplexBuildingVariableCompositionsItem();





					QHash<CExpressionVariable*, CBuildExpression*>* getVariableClassTermExpressionHash();
					COptimizedComplexBuildingVariableCompositionsItem* setVariableClassTermExpressionHash(const QHash<CExpressionVariable*, CBuildExpression*>& hash);

					

					QList<COptimizedComplexVariableCompositionItem*>* getComputeVariableMappingItemList();
					COptimizedComplexBuildingVariableCompositionsItem* addComputeVariableMappingItem(COptimizedComplexVariableCompositionItem* item);


					QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItemVariableExpressionMapping>* getVariableItemIndexMappingHash();
					COptimizedComplexVariableCompositionItemVariableExpressionMapping& getVariableItemIndexMapping(CExpressionVariable* varExp);


					bool isProcessingQueued();
					COptimizedComplexBuildingVariableCompositionsItem* setProcessingQueued(bool queued);


					CComplexQueryExpressionProcessingData* getQueryProcessingData();


					bool isSatisfiable();
					COptimizedComplexBuildingVariableCompositionsItem* setSatisfiability(bool satisfiable);

					bool isWaitingSubVariableBuildingItems();
					cint64 getWaitingSubVariableBuildingItemCount();
					COptimizedComplexBuildingVariableCompositionsItem* incWaitingSubVariableBuildingItemCount(cint64 incCount = 1);
					COptimizedComplexBuildingVariableCompositionsItem* decWaitingSubVariableBuildingItemCount(cint64 decCount = 1);

					bool hasWaitingVariableCompositionItems();
					cint64 getVariableCompositionItemWaitingCount();
					COptimizedComplexBuildingVariableCompositionsItem* incVariableCompositionItemWaitingCount(cint64 incCount = 1);
					COptimizedComplexBuildingVariableCompositionsItem* decVariableCompositionItemWaitingCount(cint64 decCount = 1);


					bool isWaitingComputationStep();
					COptimizedComplexBuildingVariableCompositionsItem* setComputationStepFinished(CComplexConceptStepComputationProcess* computationStep);
					COptimizedComplexBuildingVariableCompositionsItem* setComputationStepWaiting(CComplexConceptStepComputationProcess* computationStep);

					virtual bool isWaitingComputation() = 0;


					COptimizedComplexBuildingVariableCompositionsItem* updateLastHandledVariableItemAssociation(CExpressionVariable* lastHandledVarExp, COptimizedComplexVariableCompositionItem* lastHandledItem);
					COptimizedComplexBuildingVariableCompositionsItem* clearLastHandledVariableItemAssociation(CExpressionVariable* lastHandledVarExp);

					COptimizedComplexVariableCompositionItem* getVariableLastCompositionItem(CExpressionVariable* variableExp);
					CExpressionVariable* getVariableLastCompositionItemAssociatedVariableExpression(CExpressionVariable* variableExp);

					CExpressionVariable* getLastHandledVariableExpression();
					COptimizedComplexBuildingVariableCompositionsItem* setLastHandledVariableExpression(CExpressionVariable* varExp);


					QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* getVariableConceptItemHash();

					QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* getVariableExpressionVariableCompositionItemHash();



					QSet<COptimizedComplexVariableCompositionItem*>* getUsedComplexVariableCompositionItemSet();
					bool addUsedComplexVariableCompositionItem(COptimizedComplexVariableCompositionItem* item);
					bool addUsedComplexConceptItem(COptimizedComplexConceptItem* item);


				// protected methods
				protected:

				// protected variables
				protected:
					QList<COptimizedComplexVariableCompositionItem*> mComputeMappingItemList;
					bool mProcessingQueued;
					bool mSatisfiable;
					QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItemVariableExpressionMapping> mVarItemIndexMappingHash;
					QHash<CExpressionVariable*, CBuildExpression*> mVariableClassTermExpressionHash;

					CComplexQueryExpressionProcessingData* mQueryProcessingData;


					QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*> mVarVarCompItemHash;

					cint64 mSubVarBuildItemWaitingCount;
					cint64 mVarCompItemWaitingCount;
					cint64 mWaitingComputationStepsCount;

					QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*> mVarLastItemHash;
					QHash<CExpressionVariable*, CExpressionVariable*> mVarLastItemAssociatedVariableHash;
					CExpressionVariable* mLastHandVarExp;

					QHash<CExpressionVariable*, COptimizedComplexConceptItem*> mVarConItemHash;

					QSet<COptimizedComplexVariableCompositionItem*> mUsedComplexVariableCompositionItemSet;
					QSet<COptimizedComplexConceptItem*> mUsedComplexConceptItemSet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COptimizedComplexBuildingVariableCompositionsItem_H
