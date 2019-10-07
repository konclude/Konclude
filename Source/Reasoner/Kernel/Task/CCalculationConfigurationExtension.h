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

#ifndef KONCLUDE_REASONER_KERNEL_TASK_CCALCULATIONCONFIGURATIONEXTENSION_H
#define KONCLUDE_REASONER_KERNEL_TASK_CCALCULATIONCONFIGURATIONEXTENSION_H

// Libraries includes


// Namespace includes


// Other includes
#include "Config/CConfigurationBase.h"
#include "Config/CLocalConfigurationExtension.h"
#include "Config/CLocalConfigurationFixedExtension.h"
#include "Config/CConfigDataReader.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				/*! 
				 *
				 *		\class		CCalculationConfigurationExtension
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCalculationConfigurationExtension : public CLocalConfigurationFixedExtension {
					// public methods
					public:
						//! Constructor
						CCalculationConfigurationExtension(CConfigurationBase *fixedCollectingConfiguration, qint64 configID = 0);
						CCalculationConfigurationExtension(CCalculationConfigurationExtension *previousLocalCalcExtension, CConfigurationBase *fixedCollectingConfiguration, qint64 configID = 0);

						//! Destructor
						virtual ~CCalculationConfigurationExtension();

						bool isDependencyTrackingActivated();
						bool isBackjumpingActivated();
						bool isUnsatisfiableCacheRetrievalActivated();
						bool isSatisfiableCacheRetrievalActivated();
						bool isSingleLevelUnsatisfiableCacheWritingActivated();
						bool isTestedConceptUnsatisfiableCacheWritingActivated();
						bool isSingleLevelSatisfiableCacheWritingActivated();
						bool isProxyIndividualsActivated();
						bool isMinimizeMergingBranchesActivated();
						bool isPseudoModelRuleEssentialCheckingActivated();
						bool isClassificationPseudoModelSubsumptionMergingActivated();
						bool isSpecializedAutomateRuleActivated();
						bool isSubSetBlockingActivated();
						bool isOptimizedBlockingActivated();
						bool isEqualSetBlockingActivated();
						bool isPairwiseEqualSetBlockingActivated();
						bool isAncestorBlockingSearchActivated();
						bool isAnywhereBlockingSearchActivated();
						bool isAnywhereBlockingCandidateHashSearchActivated();
						bool isSemanticBranchingActivated();
						bool isAtomicSemanticBranchingActivated();
						bool isBranchTriggeringActivated();
						bool isStrictIndiNodeProcessingActivated();
						bool isIDIndiPriorizationActivated();
						bool isPropagateNodeProcessedActivated();
						bool isDirectRulePreprocessingActivated();
						bool isLazyNewNominalCreationActivated();
						bool isConsistenceRestrictedNonStictProcessingActivated();
						bool isUniqueNameAssumptionActivated();

						bool isSatisfiableExpansionCacheRetrievalActivated();
						bool isSatisfiableExpansionCacheConceptExpansionActivated();
						bool isSatisfiableExpansionCacheSatisfiableBlockingActivated();
						bool isSatisfiableExpansionCacheWritingActivated();

						bool isSignatureMirroringBlockingActivated();
						bool isSignatureSavingActivated();
						bool isSkipANDConceptsActivated();

						bool isCompletionGraphCachingActivated();
						bool isAvoidRepeatedIndividualProcessingActivated();
						bool isDelayedCompletionGraphCachingReactivationActivated();
						bool isForceNodesRecreationForRepeatedIndividualProcessingActivated();
						bool isIndividualsBackendCacheLoadingActivated();

						bool isUnsatisfiableCachingFullDependencyActivated();
						bool isUnsatisfiableCachingFullSignatureActivated();

						bool isPairwiseMergingActivated();

						bool isSaturationPilingActivated();

						bool isCompletionGraphReuseCachingRetrievalActivated();
						bool isCompletionGraphDeterministicReuseActivated();
						bool isCompletionGraphNonDeterministicReuseActivated();

						bool isAnywhereBlockingCoreConceptCandidateHashSearchActivated();

						bool isRepresentativePropagationActivated();

						bool isDebuggingWriteDataActivated();
						bool isDebuggingWriteDataSaturationTasksActivated();
						bool isDebuggingWriteDataCompletionTasksActivated();
						bool isDebuggingWriteDataCompletionTasksOnlyOnSatisfiabilityActivated();
						bool isDebuggingWriteDataCompletionTasksForConsistencyTestsActivated();
						bool isDebuggingWriteDataCompletionTasksForClassificationTestsActivated();
						bool isDebuggingWriteDataCompletionTasksForAnsweringPropagationTestsActivated();
						bool isDebuggingWriteDataCompletionTasksForIncrementalExpansionTestsActivated();
						bool isDebuggingWriteDataCompletionTasksForRepresentativeCacheRecomputationTestsActivated();
						bool isDebuggingWriteDataCompletionTasksForAllTestsActivated();


						bool isSuccessorConceptSaturationExpansionActivated();
						bool isSaturationCachingActivated();
						bool isSaturationCriticalConceptTestingActivated();
						bool isSaturationDirectCriticalToInsufficientActivated();
						bool isSaturationCachingWithNominalsByReactivationActivated();

						bool isSaturationSuccessorExtensionActivated();
						bool isNominalSaturationActivated();
						bool isSaturationExpansionSatisfiabilityCacheWritingActivated();
						bool isSaturationUnsatisfiabilityCacheWritingActivated();

						bool isEquivalentAlternativesSaturationMergingActivated();

						bool isDatatypeReasoningActivated();

						bool isComputedTypesCachingActivated();

						bool isConstructionIndividualNodeMergingActivated();


						cint64 getMaximumRecursiveProcessingConceptCount();


						cint64 getSaturationReferredNodeManyConceptCount();
						cint64 getSaturationManyConceptReferredNodeCountProcessLimit();
						cint64 getSaturationReferredNodeConceptCountProcessLimit();
						cint64 getSaturationReferredNodeUnprocessedCountProcessLimit();
						cint64 getSaturationReferredNodeCheckingDepth();


						bool isForceManyConceptSaturationActivated();

						bool isAllowBackendNeighbourExpansionBlockingActivated();
						bool isAllowBackendSuccessorExpansionBlockingActivated();


						bool isOccurrenceStatisticsCollectionActivated();

						bool isGeneratingTestQueriesActivated();

						bool isBlockingTestsIgnoringCompletionGraphCachedNonBlockedNodesActivated();

					// protected methods
					protected:

					// protected variables
					protected:
						bool mConfDependencyTrackingActivated;
						bool mConfBackjumpingActivated;
						bool mConfUnsatCachingActivated;
						bool mConfSatCachingActivated;
						bool mConfUnsatCacheSingleLevelWritingActivated;
						bool mConfUnsatCacheTestedConceptWritingActivated;
						bool mConfSatCacheSingleLevelWritingActivated;
						bool mConfProxyIndividualsActivated;
						bool mConfMinimizeMergingBranchesActivated;
						bool mConfPseudoModelRuleEssentialCheckingActivated;
						bool mConfClassPseudoModelSubsumptionMergingActivated;
						bool mConfSpecializedAutomateRuleActivated;
						bool mConfSubSetBlockingActivated;
						bool mConfOptimizedBlockingActivated;
						bool mConfEqualSetBlockingActivated;
						bool mConfPairwiseEqualSetBlockingActivated;
						bool mConfAncestorBlockingSearchActivated;
						bool mConfAnywhereBlockingSearchActivated;
						bool mConfAnywhereBlockingCandidateHashSearchActivated;
						bool mConfSemanticBranchingActivated;
						bool mConfAtomicSemanticBranchingActivated;
						bool mConfBranchTriggeringActivated;
						bool mConfStrictIndiNodeProcessingActivated;
						bool mConfIDIndiPriorizationActivated;
						bool mConfPropagateNodeProcessedActivated;
						bool mConfDirectRulePreprocessingActivated;
						bool mConfLazyNewNominalCreationActivated;
						bool mConfConsistenceRestrictedNonStictProcessingActivated;
						bool mConfUniqueNameAssumptionActivated;

						bool mConfSatisfiableExpansionCacheRetrievalActivated;
						bool mConfSatisfiableExpansionCacheConceptExpansionActivated;
						bool mConfSatisfiableExpansionCacheSatisfiableBlockingActivated;
						bool mConfSatisfiableExpansionCacheWritingActivated;

						bool mConfSignatureMirroringBlockingActivated;
						bool mConfSignatureSavingActivated;
						bool mConfSkipANDConceptsActivated;

						bool mConfCompletionGraphCachingActivated;
						bool mConfAvoidRepeatedIndividualProcessingActivated;
						bool mConfDelayedCompletionGraphCachingReactivationActivated;
						bool mConfForceNodesRecreationForRepeatedIndividualProcessingActivated;


						bool mConfUnsatCachingUseFullNodeDependencyActivated;
						bool mConfUnsatCachingUseNodeSignatureSetActivated;

						bool mConfPairwiseMergingActivated;

						bool mConfSaturationPilingActivated;

						bool mConfCompGraphReuseCacheRetrievalActivated;
						bool mConfCompGraphDeterministicReuseActivated;
						bool mConfCompGraphNonDeterministicReuseActivated;

						bool mConfAnywhereBlockingCoreConceptCandidateHashSearchActivated;
						bool mConfRepresentativePropagationActivated;

						bool mConfDebuggingWriteDataActivated;
						bool mConfDebuggingWriteDataCompletionTasksActivated;
						bool mConfDebuggingWriteDataSaturationTasksActivated;
						bool mConfDebuggingWriteDataOnlyOnSatisfiabilityActivated;
						bool mConfDebuggingWriteDataForConsistencyTestsActivated;
						bool mConfDebuggingWriteDataForClassificationTestsActivated;
						bool mConfDebuggingWriteDataForAnsweringPropagationTestsActivated;
						bool mConfDebuggingWriteDataForIncrementalExpansionTestsActivated;
						bool mConfDebuggingWriteDataForRepresentativeCacheRecomputationTestsActivated;
						bool mConfDebuggingWriteDataForAllTestsActivated;

						bool mConfSuccessorConceptSaturationExpansionActivated;
						bool mConfSaturationCachingActivated;
						bool mConfSaturationCriticalConceptTestingActivated;
						bool mConfSaturationDirectCriticalToInsufficientActivated;

						bool mConfSaturationSuccessorExtensionActivated;
						bool mConfSaturationCachingWithNominalsByReactivationActivated;
						bool mConfNominalSaturationActivated;
						bool mConfSaturationExpansionSatisfiabilityCacheWritingActivated;
						bool mConfSaturationUnsatisfiabilityCacheWritingActivated;
						bool mConfIndividualsBackendCacheLoadingActivated;

						bool mEquivalentAlternativesSaturationMergingActivated;
						bool mDatatypeReasoningActivated;
						bool mComputedTypesCachingActivated;
						bool mConstructionIndividualNodeMergingActivated;

						cint64 mSaturationReferredNodeManyConceptCount;
						cint64 mSaturationManyConceptReferredNodeCountProcessLimit;
						cint64 mSaturationReferredNodeConceptCountProcessLimit;
						cint64 mSaturationReferredNodeUnprocessedCountProcessLimit;
						cint64 mSaturationReferredNodeCheckingDepth;

						bool mConfForceManyConceptSaturationActivated;

						bool mAllowBackendNeighbourExpansionBlockingActivated;
						bool mAllowBackendSuccessorExpansionBlockingActivated;

						cint64 mMaxRecProConceptCount;
						bool mOccurrenceStatisticsCollectionActivated;
						bool mGeneratingTestQueriesActivated;
						bool mBlockingTestsIgnoringCompletionGraphCachedNonBlockedNodesActivated;


						bool mConfDependencyTrackingChecked;
						bool mConfBackjumpingChecked;
						bool mConfUnsatCachingChecked;
						bool mConfSatCachingChecked;
						bool mConfUnsatCacheSingleLevelWritingChecked;
						bool mConfUnsatCacheTestedConceptWritingChecked;
						bool mConfSatCacheSingleLevelWritingChecked;
						bool mConfProxyIndividualsChecked;
						bool mConfMinimizeMergingBranchesChecked;
						bool mConfPseudoModelRuleEssentialCheckingChecked;
						bool mConfClassPseudoModelSubsumptionMergingChecked;
						bool mConfSpecializedAutomateRuleChecked;
						bool mConfSubSetBlockingChecked;
						bool mConfOptimizedBlockingChecked;
						bool mConfEqualSetBlockingChecked;
						bool mConfPairwiseEqualSetBlockingChecked;
						bool mConfAncestorBlockingSearchChecked;
						bool mConfAnywhereBlockingSearchChecked;
						bool mConfAnywhereBlockingCandidateHashSearchChecked;
						bool mConfSemanticBranchingChecked;
						bool mConfAtomicSemanticBranchingChecked;
						bool mConfBranchTriggeringChecked;
						bool mConfStrictIndiNodeProcessingChecked;
						bool mConfIDIndiPriorizationChecked;
						bool mConfPropagateNodeProcessedChecked;
						bool mConfDirectRulePreprocessingChecked;
						bool mConfLazyNewNominalCreationChecked;
						bool mConfConsistenceRestrictedNonStictProcessingChecked;
						bool mConfUniqueNameAssumptionChecked;

						bool mConfSatisfiableExpansionCacheRetrievalChecked;
						bool mConfSatisfiableExpansionCacheConceptExpansionChecked;
						bool mConfSatisfiableExpansionCacheSatisfiableBlockingChecked;
						bool mConfSatisfiableExpansionCacheWritingChecked;

						bool mConfSignatureMirroringBlockingChecked;
						bool mConfSignatureSavingChecked;
						bool mConfSkipANDConceptsChecked;

						bool mConfCompletionGraphCachingChecked;
						bool mConfAvoidRepeatedIndividualProcessingChecked;
						bool mConfDelayedCompletionGraphCachingReactivationChecked;
						bool mConfForceNodesRecreationForRepeatedIndividualProcessingChecked;
						bool mConfIndividualsBackendCacheLoadingChecked;

						bool mConfUnsatCachingUseFullNodeDependencyChecked;
						bool mConfUnsatCachingUseNodeSignatureSetChecked;

						bool mConfPairwiseMergingChecked;

						bool mConfSaturationPilingChecked;

						bool mConfCompGraphReuseCacheRetrievalChecked;
						bool mConfCompGraphDeterministicReuseChecked;
						bool mConfCompGraphNonDeterministicReuseChecked;

						bool mConfAnywhereBlockingCoreConceptCandidateHashSearchChecked;
						bool mConfRepresentativePropagationChecked;
						bool mConfDebuggingWriteDataChecked;
						bool mConfDebuggingWriteDataSaturationTasksChecked;
						bool mConfDebuggingWriteDataCompletionTasksChecked;
						bool mConfDebuggingWriteDataOnlyOnSatisfiabilityChecked;
						bool mConfDebuggingWriteDataForConsistencyTestsChecked;
						bool mConfDebuggingWriteDataForClassificationTestsChecked;
						bool mConfDebuggingWriteDataForAnsweringPropagationTestsChecked;
						bool mConfDebuggingWriteDataForIncrementalExpansionTestsChecked;
						bool mConfDebuggingWriteDataForRepresentativeCacheRecomputationTestsChecked;
						bool mConfDebuggingWriteDataForAllTestsChecked;

						bool mConfSuccessorConceptSaturationExpansionChecked;
						bool mConfSaturationCachingChecked;
						bool mConfSaturationCriticalConceptTestingChecked;
						bool mConfSaturationDirectCriticalToInsufficientChecked;
						bool mConfSaturationSuccessorExtensionChecked;
						bool mConfSaturationCachingWithNominalsByReactivationChecked;
						bool mConfNominalSaturationChecked;
						bool mConfSaturationExpansionSatisfiabilityCacheWritingChecked;
						bool mConfSaturationUnsatisfiabilityCacheWritingChecked;

						bool mEquivalentAlternativesSaturationMergingChecked;
						bool mDatatypeReasoningChecked;

						bool mComputedTypesCachingChecked;
						bool mConstructionIndividualNodeMergingChecked;

						bool mSaturationReferredNodeManyConceptCountChecked;
						bool mSaturationManyConceptReferredNodeCountProcessLimitChecked;
						bool mSaturationReferredNodeConceptCountProcessLimitChecked;
						bool mSaturationReferredNodeUnprocessedCountProcessLimitChecked;
						bool mSaturationReferredNodeCheckingDepthChecked;

						bool mConfForceManyConceptSaturationChecked;


						bool mAllowBackendNeighbourExpansionBlockingChecked;
						bool mAllowBackendSuccessorExpansionBlockingChecked;

						bool mMaxRecProConceptCountChecked;
						bool mOccurrenceStatisticsCollectionChecked;
						bool mGeneratingTestQueriesChecked;

						bool mBlockingTestsIgnoringCompletionGraphCachedNonBlockedNodesChecked;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_TASK_CCALCULATIONCONFIGURATIONEXTENSION_H
