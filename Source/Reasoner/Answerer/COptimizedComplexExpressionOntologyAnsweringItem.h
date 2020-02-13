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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXEXPRESSIONONTOLOGYANSWERINGITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXEXPRESSIONONTOLOGYANSWERINGITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COntologyAnsweringItem.h"
#include "COptimizedComplexConceptItem.h"
#include "COptimizedComplexConceptStepAnsweringItem.h"
#include "CComplexConceptStepComputationProcess.h"
#include "COptimizedComplexVariableCompositionItem.h"
#include "COptimizedComplexBuildingVariableCompositionsItem.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem.h"
#include "CAnsweringCalculationStatisticsCollectionStrings.h"
#include "CAnsweringStatisticsCollectionStrings.h"
#include "CAnsweringHandlingStatistics.h"
#include "CComputedItemDataNotificationLinker.h"


// Other includes
#include "Reasoner/Ontology/CConceptReferenceLinking.h"
#include "Reasoner/Preprocess/COntologyQueryExtendedConfigDependedPreProcesser.h"

#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Ontology;

		namespace Answerer {


			typedef QPair<CConcept*, bool> TConceptNegPair;



			/*! 
			 *
			 *		\class		COptimizedComplexConceptOntologyAnsweringItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexExpressionOntologyAnsweringItem : public COntologyAnsweringItem {
				// public methods
			public:
				//! Constructor
				COptimizedComplexExpressionOntologyAnsweringItem(CConcreteOntology* ontology, CConfigurationBase* configuration);

				//! Destructor
				virtual ~COptimizedComplexExpressionOntologyAnsweringItem();


				bool isOntologyBuildingAndPreprocessingPrepared();
				COptimizedComplexExpressionOntologyAnsweringItem* setOntologyBuildingAndPreprocessingPrepared(bool prepared);


				CConcreteOntology* getTestingOntology(bool create = true);
				COptimizedComplexExpressionOntologyAnsweringItem* takeTestingOntology(CConcreteOntology* ontology);


				COptimizedComplexConceptItem* getComplexConceptItem(CConcept* concept, bool negation, bool create = true, bool* created = nullptr);
				QHash< TConceptNegPair, COptimizedComplexConceptItem* >* getComplexConceptNegationItemHash();

				CComputedItemDataNotificationLinker* createComputedItemDataNotificationLinker();
				COptimizedComplexExpressionOntologyAnsweringItem* releaseComputedItemDataNotificationLinker(CComputedItemDataNotificationLinker* itemLinker);

				CXLinker<CComplexQueryExpressionProcessingData*>* createQueryProcessingLinker();
				COptimizedComplexExpressionOntologyAnsweringItem* releaseQueryProcessingLinker(CXLinker<CComplexQueryExpressionProcessingData*>* queryLinker);

				CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* createBuildingVariableItemProcessingLinker();
				COptimizedComplexExpressionOntologyAnsweringItem* releaseBuildingVariableItemProcessingLinker(CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* varBuildItemLinker);


				CIndividual* takeTemporaryTestingIndividual();
				COptimizedComplexExpressionOntologyAnsweringItem* releaseTemporaryTestingIndividual(CIndividual* indi);



				COptimizedComplexConceptStepAnsweringItem* getConceptProcessingStepItem(ComplexConceptStepComputationProcessType type);
				COptimizedComplexConceptStepAnsweringItem* getNextConceptProcessingStepItem(ComplexConceptStepComputationProcessType type, bool onlyRequired = true);
				COptimizedComplexConceptStepAnsweringItem* getNextConceptProcessingStepItem(COptimizedComplexConceptStepAnsweringItem* item, bool onlyRequired = true);
				COptimizedComplexConceptStepAnsweringItem* getConceptSatisfiabilityProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* getConceptSuperClassesProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* getConceptSubClassesProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* getConceptSubClassesRealizationProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* getConceptEquivalentClassesProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* getConceptInstancesProcessingStepItem();

				COptimizedComplexExpressionOntologyAnsweringItem* incCurrentlyAnsweringQueryCount(cint64 incCount = 1);
				COptimizedComplexExpressionOntologyAnsweringItem* decCurrentlyAnsweringQueryCount(cint64 decCount = 1);

				bool hasCurrentlyAnsweringQueries();
				cint64 getCurrentlyAnsweringQueryCount();


				QList<COptimizedComplexConceptItem*>* getConceptClassItemReorderingList();

				COptimizedComplexConceptItem* getTopConceptItem(bool create = true);




				QList<COptimizedComplexVariableCompositionItem*>* getVariableCompositionItemProcessingList();
				COptimizedComplexVariableCompositionItem* takeNextProcessingVariableCompositionItem();
				COptimizedComplexExpressionOntologyAnsweringItem* addProcessingVariableCompositionItem(COptimizedComplexVariableCompositionItem* varCompItem);


				QList<COptimizedComplexBuildingVariableCompositionsItem*>* getVariableBuildingItemProcessingList();
				COptimizedComplexBuildingVariableCompositionsItem* takeNextProcessingVariableBuildingItem();
				COptimizedComplexExpressionOntologyAnsweringItem* addProcessingVariableBuildingItem(COptimizedComplexBuildingVariableCompositionsItem* varBuildItem);


				cint64 getNextVariableId();
				QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* getPropagationFinalizationConceptAbsorptionDataHash();
				QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* getPropagationFinalizationConceptAbsorptionItemHash();


				QHash<TConceptNegPair, TConceptNegPair>* getIndividualTriggerSingleConceptUpgradeHash();
				QHash< QSet<TConceptNegPair> , TConceptNegPair>* getIndividualTriggerMultiConceptUpgradeHash();


				CConcept* getTopRolePropagationExistingIndividualCreationConcept();
				COptimizedComplexExpressionOntologyAnsweringItem* setTopRolePropagationExistingIndividualCreationConcept(CConcept* concept);

				CIndividual* getTopRolePropagationExistingIndividual();
				COptimizedComplexExpressionOntologyAnsweringItem* setTopRolePropagationExistingIndividual(CIndividual* indi);

				cint64 getNextComplexConceptItemId(bool moveNext = true);




				CAnsweringCalculationStatisticsCollectionStrings* getAnsweringStatisticsCollectionStrings();
				CAnsweringStatisticsCollectionStrings* getDifferenceStoredAnsweringStatisticsCollectionStrings();
				COptimizedComplexExpressionOntologyAnsweringItem* setDifferenceStoredAnsweringStatisticsCollectionStrings(CAnsweringStatisticsCollectionStrings* stats);


				CAnsweringCalculationStatisticsCollection* getCalculationStatisticsCollection();

				COptimizedComplexExpressionOntologyAnsweringItem* reuseCalculationStatisticsCollection(CAnsweringCalculationStatisticsCollection* statColl);

				bool isProcessStatisticsCollectionConfigured();

				CAnsweringHandlingStatistics* getAnsweringHandlingStatistics();


				cint64 getNextComputationStepId(bool next = true);


				bool hasRequirementProcessingSchedulingReported();
				COptimizedComplexExpressionOntologyAnsweringItem* setRequirementProcessingSchedulingReported(bool reported);


				QHash<QPair<QString, CDatatype*>, CDataLiteral*>* getDataValueDatatypeSingleLiteralHash();

				CContext* getDataValueMemoryManagementContext();


				QHash<cint64, CComplexQueryExpressionProcessingData*>* getQueryExtensionProcessingDataHash();
				cint64 getNextQueryExtensionId(bool moveNext = true);


				// protected methods
			protected:

				// protected variables
			protected:
				CConcreteOntology* mTestingOntology;

				cint64 mNextComputationStepId;

				CComputedItemDataNotificationLinker* mItemNotificationLinker;
				CXLinker<CComplexQueryExpressionProcessingData*>* mQueryProcessingLinker;
				CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* mBuildingVarItemProcessingLinker;

				QHash< TConceptNegPair, COptimizedComplexConceptItem* > mConceptNegItemHash;
				COptimizedComplexConceptItem* mTopConceptItem;
				QList<COptimizedComplexConceptItem*> mConceptClassItemReorderingList;


				QList<COptimizedComplexVariableCompositionItem*> mVarCompItemProcessingList;
				QList<COptimizedComplexBuildingVariableCompositionsItem*> mVarBuildItemProcessingList;


				COptimizedComplexConceptStepAnsweringItem mConceptProcessingStepItems[COMPLEXCONCEPTSTEPCOMPUTATIONPROCESSTYPECOUNT];

				cint64 mCurrentlyAnsweringQueryCount;

				cint64 mNextVariableId;

				QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> mPropagationFinalizationConceptAbsorptionDataHash;
				QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*> mPropagationFinalizationConceptAbsorptionItemHash;

				QHash<TConceptNegPair, TConceptNegPair> mIndividualTriggerSingleConceptUpgradeHash;
				QHash< QSet<TConceptNegPair>, TConceptNegPair> mIndividualTriggerMultiConceptUpgradeHash;

				CConcept* mTopRolePropagationExistingIndividualCreationConcept;
				CIndividual* mTopRolePropagationExistingIndividual;


				cint64 mNextComplexConceptItemId;
				bool mOntologyBuildingAndPreprocessingPrepared;


				CAnsweringHandlingStatistics* mHandlingStats;
				CAnsweringStatisticsCollectionStrings* mDiffStoredStatCollStrings;
				CAnsweringCalculationStatisticsCollectionStrings* mCalcStatCollStrings;
				QList<CAnsweringCalculationStatisticsCollection*> mReuseStatCollList;

				bool mConfCollectProcessStatistics;

				CAnsweringHandlingStatistics mAnsweringHandlingStatistics;

				QList<CIndividual*> mTestingIndiList;
				cint64 mNextTestingIndiId = 0;

				bool mRequirementProcessingSchedulingReported;

				QHash<QPair<QString, CDatatype*>, CDataLiteral*> mDataValueDatatypeSingleLiteralHash;

				CMemoryPoolNewAllocationIncreasingContext mDataValueMemoryManagementContext;
				QHash<cint64, CComplexQueryExpressionProcessingData*> mQueryExtensionProcessingDataHash;
				cint64 mNextQueryExtensionId;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXEXPRESSIONONTOLOGYANSWERINGITEM_H
