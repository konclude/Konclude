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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECOMPOSITIONITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECOMPOSITIONITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableIndividualMappings.h"
#include "COptimizedComplexVariableJoiningBindingPositionMapping.h"
#include "COptimizedComplexVariableCompositionItemRoleSamplingData.h"
#include "CCacheAnswersWeightedUsageCostItem.h"
#include "COptimizedComplexVariableCompositionItemUpdateData.h"



// Other includes
#include "Reasoner/Ontology/CRole.h"

#include "Utilities/CVariantTrible.hpp"
#include "Utilities/CVariantQuad.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Answerer {



			/*! 
			 *
			 *		\class		COptimizedComplexVariableCompositionItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableCompositionItem : public CCacheAnswersWeightedUsageCostItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableCompositionItem();
					virtual ~COptimizedComplexVariableCompositionItem();

					enum COMPOSITION_TYPE {
						CONCEPT_BASE, ROLE_PROPAGATION, JOINING, DATA_LITERAL_BASE, DATA_LITERAL_EXTENSION, ABSOROPTION_BASED_EXTENSION, BINDING_REDUCTION, ROLE_PROPAGATION_JOIN, ROLE_PROPAGATION_REPLACEMENT, BINDING_EXTRACTION, 
						CONCEPT_SUB_SUPER_CLASSES, SUB_SUPER_CLASSES_PROPAGATION,
						ROLE_SUB_SUPER_PROPERTIES, SUB_SUPER_PROPERTIES_PROPAGATION,
						INDIVIDUAL_TYPE_EXTENSION, INDIVIDUAL_PROPERTY_EXTENSION
					};


					virtual COMPOSITION_TYPE getCompositionType() = 0;
					


					bool isVariableMappingsComputed();
					COptimizedComplexVariableCompositionItem* setVariableMappingsComputed(bool computed);

					bool isVariableMappingsInitialized();
					COptimizedComplexVariableCompositionItem* setVariableMappingsInitialized(bool initialized);


					double repeatedRequirementInsufficientDependencyComputationIncreaseFactor();
					COptimizedComplexVariableCompositionItem* setRepeatedRequirementInsufficientDependencyComputationIncreaseFactor(double factor);
					COptimizedComplexVariableCompositionItem* increaseRepeatedRequirementInsufficientDependencyComputationIncreaseFactor(double increaseFactor);




					bool isComputationQueued();
					COptimizedComplexVariableCompositionItem* setComputationQueued(bool queued);


					QHash<CVariantTrible<CRole*, bool, cint64>, COptimizedComplexVariableRolePropagationItem*>* getRoleInverseItemHash();
					QHash<COptimizedComplexVariableJoiningBindingPositionMapping, COptimizedComplexVariableJoiningItem*>* getJoiningItemHash();

					
					COptimizedComplexVariableRolePropagationItem*& getRoleInversePropagationItem(CRole* role, bool inversed, cint64 varIdx);
					COptimizedComplexVariableRolePropagationJoiningItem*& getRoleInversePropagationJoiningItem(CRole* role, bool inversed, cint64 varIdx, cint64 joinVarIdx);
					COptimizedComplexVariableRolePropagationReplacementItem*& getRoleInversePropagationReplacementItem(CRole* role, bool inversed, cint64 varIdx, cint64 replaceVarIdx);


					COptimizedComplexVariableJoiningItem*& getJoiningItem(const COptimizedComplexVariableJoiningBindingPositionMapping& bindingPosMapping);

					bool hasJoiningItem(const COptimizedComplexVariableJoiningBindingPositionMapping& bindingPosMapping);
					bool hasRoleInversePropagationItem(CRole* role, bool inversed, cint64 varIdx);


					COptimizedComplexVariableIndividualMappings* getVariableMapping();


					COptimizedComplexVariableCompositionItem* setVariableMapping(COptimizedComplexVariableIndividualMappings* variableMapping);

					COptimizedComplexVariableSubSuperClassesPropagationItem*& getSubSuperClassesPropagationItem(bool superClassesProp, cint64 varIdx);
					COptimizedComplexVariableSubSuperPropertiesPropagationItem*& getSubSuperPropertiesPropagationItem(bool superPropertiesProp, cint64 varIdx);



					COptimizedComplexVariableIndividualTypeExtensionItem*& getIndividualTypeExtensionItem(cint64 indiVarIdx, COptimizedComplexVariableCompositionItem* classVarCompItem, cint64 classVarIdx);
					COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem*& getIndividualNeighboringPropertyExtensionItem(cint64 indiVarIdx, COptimizedComplexVariableCompositionItem* propVarCompItem, cint64 classVarIdx);


					COptimizedComplexVariableDataLiteralExtensionItem*& getDataLiteralExtensionItem(CRole* dataRole, cint64 varIdx);
					COptimizedComplexVariableReductionItem*& getReductionItem(cint64 varIdx);
					COptimizedComplexVariableExtractionItem*& getExtractionItem(cint64 varIdx);


					QList<COptimizedComplexVariableCompositionItem*>* getComputationDependentItemList();
					COptimizedComplexVariableCompositionItem* addComputationDependentItem(COptimizedComplexVariableCompositionItem* depCompItem);
					COptimizedComplexVariableCompositionItem* setComputationDependentItemList(const QList<COptimizedComplexVariableCompositionItem*>& computationDependentItemList);


					QList<COptimizedComplexVariableCompositionItemUpdateData>* getComputationUpdateItemList();
					COptimizedComplexVariableCompositionItem* addComputationUpdateItem(COptimizedComplexVariableCompositionItem* updateCompItem);
					COptimizedComplexVariableCompositionItem* addComputationUpdateItem(COptimizedComplexVariableCompositionItem* updateCompItem, COptimizedComplexBuildingVariableCompositionsItem* buildingItem);
					COptimizedComplexVariableCompositionItem* setComputationUpdateItemList(const QList<COptimizedComplexVariableCompositionItemUpdateData>& computationUpdateItemList);



					bool requiresMoreVariableMappingsComputation();
					cint64 getVariableMappingsComputationRequirement();
					COptimizedComplexVariableCompositionItem* setVariableMappingsComputationRequirement(cint64 limit);

					cint64 getVariableMappingsCurrentCount();
					double getVariableMappingsRemainingCount();
					double getVariableMappingsExpectedCount();
					COptimizedComplexVariableCompositionItem* setVariableMappingsExpectedCount(double count);


					COptimizedComplexVariableCompositionItem* decDependencyUpdatingCount(cint64 count = 1);
					COptimizedComplexVariableCompositionItem* incDependencyUpdatingCount(cint64 count = 1);
					cint64 getDependencyUpdatingCount();



					cint64 getComputationStepId();
					COptimizedComplexVariableCompositionItem* setComputationStepId(cint64 step);

					cint64 getComputationAttempt(bool increase = true);
					COptimizedComplexVariableCompositionItem* setComputationAttempt(cint64 attempt);


					COptimizedComplexVariableCompositionItemRoleSamplingData& getRoleSamplingData(CRole* role);


					bool isLastComputation();
					COptimizedComplexVariableCompositionItem* setLastComputation(bool last);


					bool hasSplitComputations();
					COptimizedComplexVariableCompositionItem* setSplitComputations(bool splitComputations);
					

					bool isSplitComputationMode();
					COptimizedComplexVariableCompositionItem* setSplitComputationMode(bool splitMode);
					bool requiresSplitComputationActivation();
					COptimizedComplexVariableCompositionItem* setRequiresSplitComputationActivation(bool splitActivation);

					cint64 getCurrentSplitComputationPosition();
					COptimizedComplexVariableCompositionItem* setCurrentSplitComputationPosition(cint64 pos);


					cint64 getNextSplitComputationPosition();
					COptimizedComplexVariableCompositionItem* setNextSplitComputationPosition(cint64 pos);
					COptimizedComplexVariableCompositionItem* getCurrentSplitComputationItem();
					COptimizedComplexVariableCompositionItem* setCurrentSplitComputationItem(COptimizedComplexVariableCompositionItem* splitItem);

					cint64 getTotalSplitComputationCount();
					COptimizedComplexVariableCompositionItem* setTotalSplitComputationCount(cint64 count);

					cint64 getCompletedSplitComputationCount();
					cint64 getRemainingSplitComputationCount();
					COptimizedComplexVariableCompositionItem* setCompletedSplitComputationCount(cint64 count);
					COptimizedComplexVariableCompositionItem* incCompletedSplitComputationCount(cint64 count = 1);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
					QStringList debugVariableNameStringList;
					QString debugCreationString;
					COptimizedComplexVariableCompositionItemVariableIndexMapping* debugVarItemIndexMapping;
#endif


					cint64 getLastVariableMappingCacheReportedCount();
					cint64 getLastVariableMappingCacheReportedSize();

					COptimizedComplexVariableCompositionItem* setLastVariableMappingCacheReportedCount(cint64 count);
					COptimizedComplexVariableCompositionItem* setLastVariableMappingCacheReportedSize(cint64 size);




					QSet<COptimizedComplexVariableCompositionItem*>* getComputationSuccessorItemSet();
					COptimizedComplexVariableCompositionItem* addComputationSuccessorItem(COptimizedComplexVariableCompositionItem* succItem);

					cint64 getComputationItemDepth();
					COptimizedComplexVariableCompositionItem* setComputationItemDepth(cint64 depth);



					double getCacheDescendantsIncompleteComputationCostSum();
					COptimizedComplexVariableCompositionItem* setCacheDescendantsIncompleteComputationCostSum(double cost);

					bool isVariableMappingsComputationStarted();
					COptimizedComplexVariableCompositionItem* setVariableMappingsComputationStarted(bool started);

					virtual bool clearComputation();


				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableIndividualMappings* mVariableMapping;

					bool mVariableMappingsComputed;
					bool mVariableMappingsInitialized;
					bool mComputationQueued;
					bool mVariableMappingsComputationStarted;

					double mRepeatedRequirementInsufficientDependencyComputationIncreaseFactor;



					QHash<CVariantQuad<CRole*, bool, cint64, cint64>, COptimizedComplexVariableRolePropagationJoiningItem*> mRoleInverseJoinItemHash;
					QHash<CVariantQuad<CRole*, bool, cint64, cint64>, COptimizedComplexVariableRolePropagationReplacementItem*> mRoleInverseReplaceItemHash;

					QHash<CVariantTrible<CRole*, bool, cint64>, COptimizedComplexVariableRolePropagationItem*> mRoleInverseItemHash;
					QHash<COptimizedComplexVariableJoiningBindingPositionMapping, COptimizedComplexVariableJoiningItem*> mJoiningItemHash;
					QHash< QPair<CRole*, cint64>, COptimizedComplexVariableDataLiteralExtensionItem* > mDataLiteralExtensionItemHash;
					QHash< QPair<bool, cint64>, COptimizedComplexVariableSubSuperClassesPropagationItem*> mSubSuperClassesPropagationItem;
					QHash< QPair<bool, cint64>, COptimizedComplexVariableSubSuperPropertiesPropagationItem*> mSubSuperPropertiesPropagationItem;

					QHash<CVariantTrible<cint64, COptimizedComplexVariableCompositionItem*, cint64>, COptimizedComplexVariableIndividualTypeExtensionItem*> mIndividualTypeExtensionItemHash;
					QHash<CVariantTrible<cint64, COptimizedComplexVariableCompositionItem*, cint64>, COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem*> mIndividualPropertyExtensionItemHash;

					QHash< cint64, COptimizedComplexVariableReductionItem* > mReductionItemHash;
					QHash< cint64, COptimizedComplexVariableExtractionItem* > mExtractionItemHash;

					QList<COptimizedComplexVariableCompositionItem*> mComputationDependentItemList;
					QList<COptimizedComplexVariableCompositionItemUpdateData> mComputationUpdateItemList;
					QSet<COptimizedComplexVariableCompositionItem*> mComputationSuccessorItemSet;
					cint64 mComputationItemDepth;

					cint64 mVariableMappingsComputationRequirement;

					double mVariableMappingsExpectedCount;
					double mComputedVariableMappingsCount;
					cint64 mDependencyUpdatingCount;


					cint64 mComputationStepId;
					cint64 mComputationAttempt;

					COptimizedComplexVariableCompositionItem* mSplitItem;
					bool mLastComputation;
					bool mSplitComputationMode;
					bool mSplitComputations;
					bool mRequiresSplitComputationActivation;
					bool mHadSplitComputations;
					cint64 mCurrentSplitComputationPos;
					cint64 mNextSplitComputationPos;
					cint64 mTotalSplitComputationCount;
					cint64 mCompletedSplitComputationCount;

					QHash<CRole*, COptimizedComplexVariableCompositionItemRoleSamplingData> mRoleSamplingDataHash;

					cint64 mLastVariableMappingCacheReportedCount;
					cint64 mLastVariableMappingCacheReportedSize;


					double mCacheDescendantsIncompleteComputationCostSum;





				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECOMPOSITIONITEM_H
