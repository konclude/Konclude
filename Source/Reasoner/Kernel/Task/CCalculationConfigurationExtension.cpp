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

#include "CCalculationConfigurationExtension.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				CCalculationConfigurationExtension::CCalculationConfigurationExtension(CConfigurationBase *fixedCollectingConfiguration, qint64 configID) 
						: CLocalConfigurationFixedExtension((fixedCollectingConfiguration)?fixedCollectingConfiguration->getConfigurationGroup():0,configID) {
					addCollectedConfigurations(fixedCollectingConfiguration);
					mConfDependencyTrackingChecked = false;
					mConfBackjumpingChecked = false;
					mConfSatCachingChecked = false;
					mConfUnsatCachingChecked = false;
					mConfProxyIndividualsChecked = false;
					mConfMinimizeMergingBranchesChecked = false;
					mConfSatCacheSingleLevelWritingChecked = false;
					mConfUnsatCacheSingleLevelWritingChecked = false;
					mConfUnsatCacheTestedConceptWritingChecked = false;
					mConfPseudoModelRuleEssentialCheckingChecked = false;
					mConfClassPseudoModelSubsumptionMergingChecked = false;
					mConfSpecializedAutomateRuleChecked = false;
					mConfSubSetBlockingChecked = false;
					mConfOptimizedBlockingChecked = false;
					mConfEqualSetBlockingChecked = false;
					mConfPairwiseEqualSetBlockingChecked = false;
					mConfAncestorBlockingSearchChecked = false;
					mConfAnywhereBlockingSearchChecked = false;
					mConfAnywhereBlockingCandidateHashSearchChecked = false;
					mConfSemanticBranchingChecked = false;
					mConfAtomicSemanticBranchingChecked = false;
					mConfBranchTriggeringChecked = false;
					mConfStrictIndiNodeProcessingChecked = false;
					mConfIDIndiPriorizationChecked = false;
					mConfPropagateNodeProcessedChecked = false;
					mConfDirectRulePreprocessingChecked = false;
					mConfLazyNewNominalCreationChecked = false;
					mConfConsistenceRestrictedNonStictProcessingChecked = false;
					mConfUniqueNameAssumptionChecked = false;

					mConfSatisfiableExpansionCacheRetrievalChecked = false;
					mConfSatisfiableExpansionCacheConceptExpansionChecked = false;
					mConfSatisfiableExpansionCacheSatisfiableBlockingChecked = false;
					mConfSatisfiableExpansionCacheWritingChecked = false;

					mConfSignatureMirroringBlockingChecked = false;
					mConfSignatureSavingChecked = false;
					mConfSkipANDConceptsChecked = false;
					mConfCompletionGraphCachingChecked = false;
					mConfDelayedCompletionGraphCachingReactivationChecked = false;
					mConfForceNodesRecreationForRepeatedIndividualProcessingChecked = false;
					mConfAvoidRepeatedIndividualProcessingChecked = false;

					mConfUnsatCachingUseNodeSignatureSetChecked = false;
					mConfUnsatCachingUseFullNodeDependencyChecked = false;
					mConfPairwiseMergingChecked = false;
					mConfSaturationPilingChecked = false;
					mConfCompGraphReuseCacheRetrievalChecked = false;
					mConfCompGraphDeterministicReuseChecked = false;
					mConfCompGraphNonDeterministicReuseChecked = false;
					mConfAnywhereBlockingCoreConceptCandidateHashSearchChecked = false;
					mConfRepresentativePropagationChecked = false;
					mConfDebuggingWriteDataChecked = false;
					mConfDebuggingWriteDataCompletionTasksChecked = false;
					mConfDebuggingWriteDataSaturationTasksChecked = false;
					mConfDebuggingWriteDataOnlyOnSatisfiabilityChecked = false;
					mConfDebuggingWriteDataForConsistencyTestsChecked = false;
					mConfDebuggingWriteDataForClassificationTestsChecked = false;
					mConfDebuggingWriteDataForAnsweringPropagationTestsChecked = false;
					mConfDebuggingWriteDataForIncrementalExpansionTestsChecked = false;
					mConfDebuggingWriteDataForRepresentativeCacheRecomputationTestsChecked = false;
					mConfDebuggingWriteDataForAllTestsChecked = false;

					mConfSuccessorConceptSaturationExpansionChecked = false;
					mConfSaturationCachingChecked = false;
					mConfSaturationCriticalConceptTestingChecked = false;
					mConfSaturationDirectCriticalToInsufficientChecked = false;
					mConfSaturationSuccessorExtensionChecked = false;
					mConfSaturationCachingWithNominalsByReactivationChecked = false;
					mConfNominalSaturationChecked = false;
					mEquivalentAlternativesSaturationMergingChecked = false;
					mConfSaturationExpansionSatisfiabilityCacheWritingChecked = false;
					mConfSaturationUnsatisfiabilityCacheWritingChecked = false;
					mDatatypeReasoningChecked = false;
					mComputedTypesCachingChecked = false;
					mConstructionIndividualNodeMergingChecked = false;
					mConfIndividualsBackendCacheLoadingChecked = false;

					mSaturationReferredNodeManyConceptCountChecked = false;
					mSaturationManyConceptReferredNodeCountProcessLimitChecked = false;
					mSaturationReferredNodeConceptCountProcessLimitChecked = false;
					mSaturationReferredNodeUnprocessedCountProcessLimitChecked = false;
					mSaturationReferredNodeCheckingDepthChecked = false;
					mConfForceManyConceptSaturationChecked = false;
					mMaxRecProConceptCountChecked = false;

					mAllowBackendNeighbourExpansionBlockingChecked = false;
					mAllowBackendSuccessorExpansionBlockingChecked = false;
					mOccurrenceStatisticsCollectionChecked = false;
					mGeneratingTestQueriesChecked = false;
				}


				CCalculationConfigurationExtension::CCalculationConfigurationExtension(CCalculationConfigurationExtension *previousLocalCalcExtension, CConfigurationBase *fixedCollectingConfiguration, qint64 configID) 
						: CLocalConfigurationFixedExtension(previousLocalCalcExtension,configID) {
					addCollectedConfigurations(fixedCollectingConfiguration);
				}

				CCalculationConfigurationExtension::~CCalculationConfigurationExtension() {
				}



				bool CCalculationConfigurationExtension::isDependencyTrackingActivated() {
					if (!mConfDependencyTrackingChecked) {
						bool tmpConfig = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.DependencyTracking",true);
						tmpConfig |= isBackjumpingActivated() | isSingleLevelUnsatisfiableCacheWritingActivated();
						mConfDependencyTrackingActivated = tmpConfig;
						mConfDependencyTrackingChecked = true;
					}
					return mConfDependencyTrackingActivated;
				}

				bool CCalculationConfigurationExtension::isBackjumpingActivated() {
					if (!mConfBackjumpingChecked) {
						mConfBackjumpingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.Backjumping",true);
						mConfBackjumpingChecked = true;
					}
					return mConfBackjumpingActivated;
				}

				bool CCalculationConfigurationExtension::isSatisfiableCacheRetrievalActivated() {
					if (!mConfSatCachingChecked) {
						mConfSatCachingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SatisfiableCacheRetrieval",true);
						mConfSatCachingChecked = true;
					}
					return mConfSatCachingActivated;
				}

				bool CCalculationConfigurationExtension::isUnsatisfiableCacheRetrievalActivated() {
					if (!mConfUnsatCachingChecked) {
						mConfUnsatCachingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.UnsatisfiableCacheRetrieval",true);
						mConfUnsatCachingChecked = true;
					}
					return mConfUnsatCachingActivated;
				}

				bool CCalculationConfigurationExtension::isProxyIndividualsActivated() {
					if (!mConfProxyIndividualsChecked) {
						mConfProxyIndividualsActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.ProxyIndividuals",true);
						mConfProxyIndividualsChecked = true;
					}
					return mConfProxyIndividualsActivated;
				}

				bool CCalculationConfigurationExtension::isMinimizeMergingBranchesActivated() {
					if (!mConfMinimizeMergingBranchesChecked) {
						mConfMinimizeMergingBranchesActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.MinimizedMergingBranches",true);
						mConfMinimizeMergingBranchesChecked = true;
					}
					return mConfMinimizeMergingBranchesActivated;
				}

				bool CCalculationConfigurationExtension::isSingleLevelUnsatisfiableCacheWritingActivated() {
					if (!mConfUnsatCacheSingleLevelWritingChecked) {
						mConfUnsatCacheSingleLevelWritingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.UnsatisfiableCacheSingleLevelWriting",true);
						mConfUnsatCacheSingleLevelWritingChecked = true;
					}
					return mConfUnsatCacheSingleLevelWritingActivated;
				}


				bool CCalculationConfigurationExtension::isTestedConceptUnsatisfiableCacheWritingActivated() {
					if (!mConfUnsatCacheTestedConceptWritingChecked) {
						mConfUnsatCacheTestedConceptWritingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.UnsatisfiableCacheTestingConceptWriting",true);
						mConfUnsatCacheTestedConceptWritingChecked = true;
					}
					return mConfUnsatCacheTestedConceptWritingActivated;
				}

				bool CCalculationConfigurationExtension::isSingleLevelSatisfiableCacheWritingActivated() {
					if (!mConfSatCacheSingleLevelWritingChecked) {
						mConfSatCacheSingleLevelWritingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SatisfiableCacheSingleLevelWriting",true);
						mConfSatCacheSingleLevelWritingChecked = true;
					}
					return mConfSatCacheSingleLevelWritingActivated;
				}

				bool CCalculationConfigurationExtension::isPseudoModelRuleEssentialCheckingActivated() {
					if (!mConfPseudoModelRuleEssentialCheckingChecked) {
						mConfPseudoModelRuleEssentialCheckingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.PseudoModelEssentialRuleChecking",true);
						mConfPseudoModelRuleEssentialCheckingChecked = true;
					}
					return mConfPseudoModelRuleEssentialCheckingActivated;
				}

				bool CCalculationConfigurationExtension::isClassificationPseudoModelSubsumptionMergingActivated() {
					if (!mConfClassPseudoModelSubsumptionMergingChecked) {
						mConfClassPseudoModelSubsumptionMergingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.PseudoModelSubsumptionMerging",true);
						mConfClassPseudoModelSubsumptionMergingChecked = true;
					}
					return mConfClassPseudoModelSubsumptionMergingActivated;
				}


				bool CCalculationConfigurationExtension::isSpecializedAutomateRuleActivated() {
					if (!mConfSpecializedAutomateRuleChecked) {
						mConfSpecializedAutomateRuleActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SpezializedAutomateRule",true);
						mConfSpecializedAutomateRuleChecked = true;
					}
					return mConfSpecializedAutomateRuleActivated;
				}



				bool CCalculationConfigurationExtension::isSubSetBlockingActivated() {
					if (!mConfSubSetBlockingChecked) {
						mConfSubSetBlockingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SubSetBlockingTest",false);
						mConfSubSetBlockingChecked = true;
					}
					return mConfSubSetBlockingActivated;
				}

				bool CCalculationConfigurationExtension::isOptimizedBlockingActivated() {
					if (!mConfOptimizedBlockingChecked) {
						mConfOptimizedBlockingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.OptimizedBlockingTest",true);
						if (!isSubSetBlockingActivated() && !isEqualSetBlockingActivated() && !isPairwiseEqualSetBlockingActivated()) {
							mConfOptimizedBlockingActivated = true;
						}
						mConfOptimizedBlockingChecked = true;
					}
					return mConfOptimizedBlockingActivated;
				}

				bool CCalculationConfigurationExtension::isEqualSetBlockingActivated() {
					if (!mConfEqualSetBlockingChecked) {
						mConfEqualSetBlockingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.EqualSetBlockingTest",false);
						mConfEqualSetBlockingChecked = true;
					}
					return mConfEqualSetBlockingActivated;
				}

				bool CCalculationConfigurationExtension::isPairwiseEqualSetBlockingActivated() {
					if (!mConfPairwiseEqualSetBlockingChecked) {
						mConfPairwiseEqualSetBlockingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.PairwiseEqualSetBlockingTest",false);
						mConfPairwiseEqualSetBlockingChecked = true;
					}
					return mConfPairwiseEqualSetBlockingActivated;
				}

				bool CCalculationConfigurationExtension::isAncestorBlockingSearchActivated() {
					if (!mConfAncestorBlockingSearchChecked) {
						mConfAncestorBlockingSearchActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.AncestorBlockingSearch",false);
						if (!isAnywhereBlockingSearchActivated() && !isAnywhereBlockingCandidateHashSearchActivated() && !isAnywhereBlockingCoreConceptCandidateHashSearchActivated()) {
							mConfAncestorBlockingSearchActivated = true;
						}
						mConfAncestorBlockingSearchChecked = true;
					}
					return mConfAncestorBlockingSearchActivated;
				}

				bool CCalculationConfigurationExtension::isAnywhereBlockingSearchActivated() {
					if (!mConfAnywhereBlockingSearchChecked) {
						mConfAnywhereBlockingSearchActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.AnywhereBlockingSearch",false);
						mConfAnywhereBlockingSearchChecked = true;
					}
					return mConfAnywhereBlockingSearchActivated;
				}

				bool CCalculationConfigurationExtension::isAnywhereBlockingCandidateHashSearchActivated() {
					if (!mConfAnywhereBlockingCandidateHashSearchChecked) {
						mConfAnywhereBlockingCandidateHashSearchActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.AnywhereCandidateHashBlockingSearch",true);
						mConfAnywhereBlockingCandidateHashSearchChecked = true;
					}
					return mConfAnywhereBlockingCandidateHashSearchActivated;
				}

				bool CCalculationConfigurationExtension::isSemanticBranchingActivated() {
					if (!mConfSemanticBranchingChecked) {
						mConfSemanticBranchingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SemanticBranching",false);
						mConfSemanticBranchingChecked = true;
					}
					return mConfSemanticBranchingActivated;
				}

				bool CCalculationConfigurationExtension::isAtomicSemanticBranchingActivated() {
					if (!mConfAtomicSemanticBranchingChecked) {
						mConfAtomicSemanticBranchingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.AtomicSemanticBranching",true);
						mConfAtomicSemanticBranchingChecked = true;
					}
					return mConfAtomicSemanticBranchingActivated;
				}

				bool CCalculationConfigurationExtension::isBranchTriggeringActivated() {
					if (!mConfBranchTriggeringChecked) {
						mConfBranchTriggeringActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.BranchTriggering",true);
						mConfBranchTriggeringChecked = true;
					}
					return mConfBranchTriggeringActivated;
				}

				bool CCalculationConfigurationExtension::isStrictIndiNodeProcessingActivated() {
					if (!mConfStrictIndiNodeProcessingChecked) {
						mConfStrictIndiNodeProcessingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.StrictIndividualNodeProcessingOrder",true);
						mConfStrictIndiNodeProcessingChecked = true;
					}
					return mConfStrictIndiNodeProcessingActivated;
				}

				bool CCalculationConfigurationExtension::isIDIndiPriorizationActivated() {
					if (!mConfIDIndiPriorizationChecked) {
						mConfIDIndiPriorizationActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.IndividualNodeIDPriorization",true);
						mConfIDIndiPriorizationChecked = true;
					}
					return mConfIDIndiPriorizationActivated;
				}

				bool CCalculationConfigurationExtension::isPropagateNodeProcessedActivated() {
					if (!mConfPropagateNodeProcessedChecked) {
						mConfPropagateNodeProcessedActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.ProcessedIndividualNodePropagation",false);
						if (!isStrictIndiNodeProcessingActivated()) {
							mConfPropagateNodeProcessedActivated = true;
						}
						mConfPropagateNodeProcessedChecked = true;
					}
					return mConfPropagateNodeProcessedActivated;
				}


				bool CCalculationConfigurationExtension::isDirectRulePreprocessingActivated() {
					if (!mConfDirectRulePreprocessingChecked) {
						mConfDirectRulePreprocessingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.DirectRulePreprocessing",true);
						mConfDirectRulePreprocessingChecked = true;
					}
					return mConfDirectRulePreprocessingActivated;
				}


				bool CCalculationConfigurationExtension::isLazyNewNominalCreationActivated() {
					if (!mConfLazyNewNominalCreationChecked) {
						mConfLazyNewNominalCreationActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.LazyNewMergingNominalCreation",true);
						mConfLazyNewNominalCreationChecked = true;
					}
					return mConfLazyNewNominalCreationActivated;
				}

				bool CCalculationConfigurationExtension::isConsistenceRestrictedNonStictProcessingActivated() {
					if (!mConfConsistenceRestrictedNonStictProcessingChecked) {
						mConfConsistenceRestrictedNonStictProcessingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.ConsistenceRestrictedNonStrictProcessing",true);
						mConfConsistenceRestrictedNonStictProcessingChecked = true;
					}
					return mConfConsistenceRestrictedNonStictProcessingActivated;
				}

				bool CCalculationConfigurationExtension::isUniqueNameAssumptionActivated() {
					if (!mConfUniqueNameAssumptionChecked) {
						mConfUniqueNameAssumptionActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.UniqueNameAssumption",false);
						mConfUniqueNameAssumptionChecked = true;
					}
					return mConfUniqueNameAssumptionActivated;
				}

				bool CCalculationConfigurationExtension::isSatisfiableExpansionCacheRetrievalActivated() {
					if (!mConfSatisfiableExpansionCacheRetrievalChecked) {
						mConfSatisfiableExpansionCacheRetrievalActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SatisfiableExpansionCacheRetrieval",true);
						mConfSatisfiableExpansionCacheRetrievalChecked = true;
					}
					return mConfSatisfiableExpansionCacheRetrievalActivated;
				}

				bool CCalculationConfigurationExtension::isSatisfiableExpansionCacheConceptExpansionActivated() {
					if (!mConfSatisfiableExpansionCacheConceptExpansionChecked) {
						mConfSatisfiableExpansionCacheConceptExpansionActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SatisfiableExpansionCacheConceptExpansion",true);
						mConfSatisfiableExpansionCacheConceptExpansionChecked = true;
					}
					return mConfSatisfiableExpansionCacheConceptExpansionActivated;
				}

				bool CCalculationConfigurationExtension::isSatisfiableExpansionCacheSatisfiableBlockingActivated() {
					if (!mConfSatisfiableExpansionCacheSatisfiableBlockingChecked) {
						mConfSatisfiableExpansionCacheSatisfiableBlockingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SatisfiableExpansionCacheSatisfiableBlocking",true);
						mConfSatisfiableExpansionCacheSatisfiableBlockingChecked = true;
					}
					return mConfSatisfiableExpansionCacheSatisfiableBlockingActivated;
				}

				bool CCalculationConfigurationExtension::isSatisfiableExpansionCacheWritingActivated() {
					if (!mConfSatisfiableExpansionCacheWritingChecked) {
						mConfSatisfiableExpansionCacheWritingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SatisfiableExpansionCacheWriting",true);
						mConfSatisfiableExpansionCacheWritingChecked = true;
					}
					return mConfSatisfiableExpansionCacheWritingActivated;
				}

				bool CCalculationConfigurationExtension::isSignatureMirroringBlockingActivated() {
					if (!mConfSignatureMirroringBlockingChecked) {
						mConfSignatureMirroringBlockingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SignatureMirroringBlocking",true);
						mConfSignatureMirroringBlockingChecked = true;
					}
					return mConfSignatureMirroringBlockingActivated;
				}

				bool CCalculationConfigurationExtension::isSignatureSavingActivated() {
					if (!mConfSignatureSavingChecked) {
						mConfSignatureSavingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SignatureSaving",true);
						mConfSignatureSavingChecked = true;
					}
					return mConfSignatureSavingActivated;
				}

				bool CCalculationConfigurationExtension::isSkipANDConceptsActivated() {
					if (!mConfSkipANDConceptsChecked) {
						mConfSkipANDConceptsActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.ANDConceptSkipping",false);
						mConfSkipANDConceptsChecked = true;
					}
					return mConfSkipANDConceptsActivated;
				}

				bool CCalculationConfigurationExtension::isCompletionGraphCachingActivated() {
					if (!mConfCompletionGraphCachingChecked) {
						mConfCompletionGraphCachingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.CompletionGraphCaching",true);
						mConfCompletionGraphCachingChecked = true;
					}
					return mConfCompletionGraphCachingActivated;
				}



				bool CCalculationConfigurationExtension::isDelayedCompletionGraphCachingReactivationActivated() {
					if (!mConfDelayedCompletionGraphCachingReactivationChecked) {
						mConfDelayedCompletionGraphCachingReactivationActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.DelayedCompletionGraphCachingReactivation",false);
						mConfDelayedCompletionGraphCachingReactivationChecked = true;
					}
					return mConfDelayedCompletionGraphCachingReactivationActivated;
				}



				bool CCalculationConfigurationExtension::isForceNodesRecreationForRepeatedIndividualProcessingActivated() {
					if (!mConfForceNodesRecreationForRepeatedIndividualProcessingChecked) {
						mConfForceNodesRecreationForRepeatedIndividualProcessingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.ForceNodesRecreationForRepeatedIndividualProcessing",true);
						mConfForceNodesRecreationForRepeatedIndividualProcessingChecked = true;
					}
					return mConfForceNodesRecreationForRepeatedIndividualProcessingActivated;
				}





				bool CCalculationConfigurationExtension::isAvoidRepeatedIndividualProcessingActivated() {
					if (!mConfAvoidRepeatedIndividualProcessingChecked) {
						mConfAvoidRepeatedIndividualProcessingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.AvoidRepeatedIndividualProcessing",true);
						mConfAvoidRepeatedIndividualProcessingChecked = true;
					}
					return mConfAvoidRepeatedIndividualProcessingActivated;
				}







				bool CCalculationConfigurationExtension::isIndividualsBackendCacheLoadingActivated() {
					if (!mConfIndividualsBackendCacheLoadingChecked) {
						mConfIndividualsBackendCacheLoadingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.IndividualsBackendCacheLoading",true);
						mConfIndividualsBackendCacheLoadingChecked = true;
					}
					return mConfIndividualsBackendCacheLoadingActivated;
				}





				bool CCalculationConfigurationExtension::isUnsatisfiableCachingFullDependencyActivated() {
					if (!mConfUnsatCachingUseFullNodeDependencyChecked) {
						mConfUnsatCachingUseFullNodeDependencyActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.UnsatisfiableCachingFullDependency",false);
						mConfUnsatCachingUseFullNodeDependencyChecked = true;
					}
					return mConfUnsatCachingUseFullNodeDependencyActivated;
				}

				bool CCalculationConfigurationExtension::isUnsatisfiableCachingFullSignatureActivated() {
					if (!mConfUnsatCachingUseNodeSignatureSetChecked) {
						mConfUnsatCachingUseNodeSignatureSetActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.UnsatisfiableCachingFullSignature",false);
						mConfUnsatCachingUseNodeSignatureSetChecked = true;
					}
					return mConfUnsatCachingUseNodeSignatureSetActivated;
				}

				bool CCalculationConfigurationExtension::isPairwiseMergingActivated() {
					if (!mConfPairwiseMergingChecked) {
						mConfPairwiseMergingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.PairwiseMerging",false);
						mConfPairwiseMergingChecked = true;
					}
					return mConfPairwiseMergingActivated;
				}

				bool CCalculationConfigurationExtension::isSaturationPilingActivated() {
					if (!mConfSaturationPilingChecked) {
						mConfSaturationPilingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SaturationPiling",false);
						mConfSaturationPilingChecked = true;
					}
					return mConfSaturationPilingActivated;
				}



				bool CCalculationConfigurationExtension::isCompletionGraphReuseCachingRetrievalActivated() {
					if (!mConfCompGraphReuseCacheRetrievalChecked) {
						mConfCompGraphReuseCacheRetrievalActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.CompletionGraphReuseCachingRetrieval",true);
						mConfCompGraphReuseCacheRetrievalChecked = true;
					}
					return mConfCompGraphReuseCacheRetrievalActivated;
				}


				bool CCalculationConfigurationExtension::isCompletionGraphDeterministicReuseActivated() {
					if (!mConfCompGraphDeterministicReuseChecked) {
						mConfCompGraphDeterministicReuseActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.CompletionGraphDeterministicReuse",true);
						mConfCompGraphDeterministicReuseChecked = true;
					}
					return mConfCompGraphDeterministicReuseActivated;
				}


				bool CCalculationConfigurationExtension::isCompletionGraphNonDeterministicReuseActivated() {
					if (!mConfCompGraphNonDeterministicReuseChecked) {
						mConfCompGraphNonDeterministicReuseActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.CompletionGraphNonDeterministicReuse",true);
						mConfCompGraphNonDeterministicReuseChecked = true;
					}
					return mConfCompGraphNonDeterministicReuseActivated;
				}


				bool CCalculationConfigurationExtension::isAnywhereBlockingCoreConceptCandidateHashSearchActivated() {
					if (!mConfAnywhereBlockingCoreConceptCandidateHashSearchChecked) {
						mConfAnywhereBlockingCoreConceptCandidateHashSearchActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.AnywhereCoreConceptCandidateHashBlockingSearch",true);
						mConfAnywhereBlockingCoreConceptCandidateHashSearchChecked = true;
					}
					return mConfAnywhereBlockingCoreConceptCandidateHashSearchActivated;
				}


				bool CCalculationConfigurationExtension::isRepresentativePropagationActivated() {
					if (!mConfRepresentativePropagationChecked) {
						mConfRepresentativePropagationActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.RepresentativePropagation",true);
						mConfRepresentativePropagationChecked = true;
					}
					return mConfRepresentativePropagationActivated;
				}


				bool CCalculationConfigurationExtension::isDebuggingWriteDataActivated() {
					if (!mConfDebuggingWriteDataChecked) {
						mConfDebuggingWriteDataActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Debugging.WriteDebuggingData",false);
						mConfDebuggingWriteDataChecked = true;
					}
					return mConfDebuggingWriteDataActivated;
				}

				bool CCalculationConfigurationExtension::isDebuggingWriteDataSaturationTasksActivated() {
					if (!mConfDebuggingWriteDataSaturationTasksChecked) {
						mConfDebuggingWriteDataSaturationTasksActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Debugging.WriteDebuggingDataSaturationTasks", false);
						mConfDebuggingWriteDataSaturationTasksChecked = true;
					}
					return mConfDebuggingWriteDataSaturationTasksActivated;
				}

				bool CCalculationConfigurationExtension::isDebuggingWriteDataCompletionTasksActivated() {
					if (!mConfDebuggingWriteDataCompletionTasksChecked) {
						mConfDebuggingWriteDataCompletionTasksActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Debugging.WriteDebuggingDataCompletionTasks", false);
						mConfDebuggingWriteDataCompletionTasksChecked = true;
					}
					return mConfDebuggingWriteDataCompletionTasksActivated;
				}



				bool CCalculationConfigurationExtension::isDebuggingWriteDataCompletionTasksOnlyOnSatisfiabilityActivated() {
					if (!mConfDebuggingWriteDataOnlyOnSatisfiabilityChecked) {
						mConfDebuggingWriteDataOnlyOnSatisfiabilityActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Debugging.WriteDebuggingDataCompletionTasksOnlyOnSatisfiability", false);
						mConfDebuggingWriteDataOnlyOnSatisfiabilityChecked = true;
					}
					return mConfDebuggingWriteDataOnlyOnSatisfiabilityActivated;
				}


				bool CCalculationConfigurationExtension::isDebuggingWriteDataCompletionTasksForConsistencyTestsActivated() {
					if (!mConfDebuggingWriteDataForConsistencyTestsChecked) {
						mConfDebuggingWriteDataForConsistencyTestsActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Debugging.WriteDebuggingDataCompletionTasksForConsistencyTests", false);
						mConfDebuggingWriteDataForConsistencyTestsChecked = true;
					}
					return mConfDebuggingWriteDataForConsistencyTestsActivated;
				}


				bool CCalculationConfigurationExtension::isDebuggingWriteDataCompletionTasksForClassificationTestsActivated() {
					if (!mConfDebuggingWriteDataForClassificationTestsChecked) {
						mConfDebuggingWriteDataForClassificationTestsActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Debugging.WriteDebuggingDataCompletionTasksForClassificationTests", false);
						mConfDebuggingWriteDataForClassificationTestsChecked = true;
					}
					return mConfDebuggingWriteDataForClassificationTestsActivated;
				}


				bool CCalculationConfigurationExtension::isDebuggingWriteDataCompletionTasksForAnsweringPropagationTestsActivated() {
					if (!mConfDebuggingWriteDataForAnsweringPropagationTestsChecked) {
						mConfDebuggingWriteDataForAnsweringPropagationTestsActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Debugging.WriteDebuggingDataCompletionTasksForAnsweringPropagationTests", false);
						mConfDebuggingWriteDataForAnsweringPropagationTestsChecked = true;
					}
					return mConfDebuggingWriteDataForAnsweringPropagationTestsActivated;
				}

				bool CCalculationConfigurationExtension::isDebuggingWriteDataCompletionTasksForIncrementalExpansionTestsActivated() {
					if (!mConfDebuggingWriteDataForIncrementalExpansionTestsChecked) {
						mConfDebuggingWriteDataForIncrementalExpansionTestsActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Debugging.WriteDebuggingDataCompletionTasksForIncrementalExpansionTests", false);
						mConfDebuggingWriteDataForIncrementalExpansionTestsChecked = true;
					}
					return mConfDebuggingWriteDataForIncrementalExpansionTestsActivated;
				}

				bool CCalculationConfigurationExtension::isDebuggingWriteDataCompletionTasksForRepresentativeCacheRecomputationTestsActivated() {
					if (!mConfDebuggingWriteDataForRepresentativeCacheRecomputationTestsChecked) {
						mConfDebuggingWriteDataForRepresentativeCacheRecomputationTestsActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Debugging.WriteDebuggingDataCompletionTasksForRepresentativeCacheRecomputationTests", false);
						mConfDebuggingWriteDataForRepresentativeCacheRecomputationTestsChecked = true;
					}
					return mConfDebuggingWriteDataForRepresentativeCacheRecomputationTestsActivated;
				}

				bool CCalculationConfigurationExtension::isDebuggingWriteDataCompletionTasksForAllTestsActivated() {
					if (!mConfDebuggingWriteDataForAllTestsChecked) {
						mConfDebuggingWriteDataForAllTestsActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Debugging.WriteDebuggingDataForAllTests", false);
						mConfDebuggingWriteDataForAllTestsChecked = true;
					}
					return mConfDebuggingWriteDataForAllTestsActivated;
				}











				bool CCalculationConfigurationExtension::isSuccessorConceptSaturationExpansionActivated() {
					if (!mConfSuccessorConceptSaturationExpansionChecked) {
						mConfSuccessorConceptSaturationExpansionActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SuccessorConceptSaturationExpansion",true);
						mConfSuccessorConceptSaturationExpansionChecked = true;
					}
					return mConfSuccessorConceptSaturationExpansionActivated;
				}


				bool CCalculationConfigurationExtension::isSaturationCachingActivated() {
					if (!mConfSaturationCachingChecked) {
						mConfSaturationCachingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SaturationCaching",true);
						mConfSaturationCachingChecked = true;
					}
					return mConfSaturationCachingActivated;
				}


				bool CCalculationConfigurationExtension::isSaturationCriticalConceptTestingActivated() {
					if (!mConfSaturationCriticalConceptTestingChecked) {
						mConfSaturationCriticalConceptTestingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SaturationCriticalConceptTesting",true);
						mConfSaturationCriticalConceptTestingChecked = true;
					}
					return mConfSaturationCriticalConceptTestingActivated;
				}


				bool CCalculationConfigurationExtension::isSaturationDirectCriticalToInsufficientActivated() {
					if (!mConfSaturationDirectCriticalToInsufficientChecked) {
						mConfSaturationDirectCriticalToInsufficientActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SaturationDirectCriticalToInsufficient",false);
						mConfSaturationDirectCriticalToInsufficientChecked = true;
					}
					return mConfSaturationDirectCriticalToInsufficientActivated;
				}


				bool CCalculationConfigurationExtension::isSaturationSuccessorExtensionActivated() {
					if (!mConfSaturationSuccessorExtensionChecked) {
						mConfSaturationSuccessorExtensionActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SaturationSuccessorExtension",false);
						mConfSaturationSuccessorExtensionChecked = true;
					}
					return mConfSaturationSuccessorExtensionActivated;
				}


				bool CCalculationConfigurationExtension::isSaturationCachingWithNominalsByReactivationActivated() {
					if (!mConfSaturationCachingWithNominalsByReactivationChecked) {
						mConfSaturationCachingWithNominalsByReactivationActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.NominalSaturationCachingByNodeReactivation",false);
						mConfSaturationCachingWithNominalsByReactivationChecked = true;
					}
					return mConfSaturationCachingWithNominalsByReactivationActivated;
				}


				bool CCalculationConfigurationExtension::isNominalSaturationActivated() {
					if (!mConfNominalSaturationChecked) {
						mConfNominalSaturationActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.NominalSaturation",true);
						mConfNominalSaturationChecked = true;
					}
					return mConfNominalSaturationActivated;
				}


				bool CCalculationConfigurationExtension::isEquivalentAlternativesSaturationMergingActivated() {
					if (!mEquivalentAlternativesSaturationMergingChecked) {
						mEquivalentAlternativesSaturationMergingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.EquivalentAlternativesSaturationMerging",true);
						mEquivalentAlternativesSaturationMergingChecked = true;
					}
					return mEquivalentAlternativesSaturationMergingActivated;
				}


				bool CCalculationConfigurationExtension::isSaturationExpansionSatisfiabilityCacheWritingActivated() {
					if (!mConfSaturationExpansionSatisfiabilityCacheWritingChecked) {
						mConfSaturationExpansionSatisfiabilityCacheWritingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SaturationExpansionSatisfiabilityCacheWriting",true);
						mConfSaturationExpansionSatisfiabilityCacheWritingChecked = true;
					}
					return mConfSaturationExpansionSatisfiabilityCacheWritingActivated;
				}

				bool CCalculationConfigurationExtension::isSaturationUnsatisfiabilityCacheWritingActivated() {
					if (!mConfSaturationUnsatisfiabilityCacheWritingChecked) {
						mConfSaturationUnsatisfiabilityCacheWritingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.SaturationExpansionSatisfiabilityCacheWriting",true);
						mConfSaturationUnsatisfiabilityCacheWritingChecked = true;
					}
					return mConfSaturationUnsatisfiabilityCacheWritingActivated;
				}


				bool CCalculationConfigurationExtension::isDatatypeReasoningActivated() {
					if (!mDatatypeReasoningChecked) {
						mDatatypeReasoningActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.ComputedTypesCaching",true);
						mDatatypeReasoningChecked = true;
					}
					return mDatatypeReasoningActivated;
				}


				bool CCalculationConfigurationExtension::isComputedTypesCachingActivated() {
					if (!mComputedTypesCachingChecked) {
						mComputedTypesCachingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.DatatypeReasoning",true);
						mComputedTypesCachingChecked = true;
					}
					return mComputedTypesCachingActivated;
				}



				bool CCalculationConfigurationExtension::isConstructionIndividualNodeMergingActivated() {
					if (!mConstructionIndividualNodeMergingChecked) {
						mConstructionIndividualNodeMergingActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.ConstructionIndividualNodeMerging",true);
						mConstructionIndividualNodeMergingChecked = true;
					}
					return mConstructionIndividualNodeMergingActivated;
				}




				cint64 CCalculationConfigurationExtension::getMaximumRecursiveProcessingConceptCount() {
					if (!mMaxRecProConceptCountChecked) {
						mMaxRecProConceptCount = CConfigDataReader::readConfigInteger(this,"Konclude.Calculation.MaximumRecursiveProcessingConceptCount",true);
						mMaxRecProConceptCountChecked = true;
					}
					return mMaxRecProConceptCount;
				}



				cint64 CCalculationConfigurationExtension::getSaturationReferredNodeManyConceptCount() {
					if (!mSaturationReferredNodeManyConceptCountChecked) {
						mSaturationReferredNodeManyConceptCount = CConfigDataReader::readConfigInteger(this,"Konclude.Calculation.Optimization.SaturationReferredNodeManyConceptCount",true);
						mSaturationReferredNodeManyConceptCountChecked = true;
					}
					return mSaturationReferredNodeManyConceptCount;
				}

				cint64 CCalculationConfigurationExtension::getSaturationManyConceptReferredNodeCountProcessLimit() {
					if (!mSaturationManyConceptReferredNodeCountProcessLimitChecked) {
						mSaturationManyConceptReferredNodeCountProcessLimit = CConfigDataReader::readConfigInteger(this,"Konclude.Calculation.Optimization.SaturationManyConceptReferredNodeCountProcessLimit",true);
						mSaturationManyConceptReferredNodeCountProcessLimitChecked = true;
					}
					return mSaturationManyConceptReferredNodeCountProcessLimit;
				}

				cint64 CCalculationConfigurationExtension::getSaturationReferredNodeConceptCountProcessLimit() {
					if (!mSaturationReferredNodeConceptCountProcessLimitChecked) {
						mSaturationReferredNodeConceptCountProcessLimit = CConfigDataReader::readConfigInteger(this,"Konclude.Calculation.Optimization.SaturationReferredNodeConceptCountProcessLimit",true);
						mSaturationReferredNodeConceptCountProcessLimitChecked = true;
					}
					return mSaturationReferredNodeConceptCountProcessLimit;
				}

				cint64 CCalculationConfigurationExtension::getSaturationReferredNodeUnprocessedCountProcessLimit() {
					if (!mSaturationReferredNodeUnprocessedCountProcessLimitChecked) {
						mSaturationReferredNodeUnprocessedCountProcessLimit = CConfigDataReader::readConfigInteger(this,"Konclude.Calculation.Optimization.SaturationReferredNodeUnprocessedCountProcessLimit",true);
						mSaturationReferredNodeUnprocessedCountProcessLimitChecked = true;
					}
					return mSaturationReferredNodeUnprocessedCountProcessLimit;
				}

				cint64 CCalculationConfigurationExtension::getSaturationReferredNodeCheckingDepth() {
					if (!mSaturationReferredNodeCheckingDepthChecked) {
						mSaturationReferredNodeCheckingDepth = CConfigDataReader::readConfigInteger(this,"Konclude.Calculation.Optimization.SaturationReferredNodeCheckingDepth",true);
						mSaturationReferredNodeCheckingDepthChecked = true;
					}
					return mSaturationReferredNodeCheckingDepth;
				}


				bool CCalculationConfigurationExtension::isForceManyConceptSaturationActivated() {
					if (!mConfForceManyConceptSaturationChecked) {
						mConfForceManyConceptSaturationActivated = CConfigDataReader::readConfigBoolean(this,"Konclude.Calculation.Optimization.ForceManyConceptNodeSaturation",true);
						mConfForceManyConceptSaturationChecked = true;
					}
					return mConfForceManyConceptSaturationActivated;
				}


				bool CCalculationConfigurationExtension::isAllowBackendNeighbourExpansionBlockingActivated() {
					if (!mAllowBackendNeighbourExpansionBlockingChecked) {
						mAllowBackendNeighbourExpansionBlockingActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Calculation.Optimization.AllowBackendNeighbourExpansionBlocking", true);
						mAllowBackendNeighbourExpansionBlockingChecked = true;
					}
					return mAllowBackendNeighbourExpansionBlockingActivated;
				}

				bool CCalculationConfigurationExtension::isAllowBackendSuccessorExpansionBlockingActivated() {
					if (!mAllowBackendSuccessorExpansionBlockingChecked) {
						mAllowBackendSuccessorExpansionBlockingActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Calculation.Optimization.AllowBackendSuccessorExpansionBlocking", true);
						mAllowBackendSuccessorExpansionBlockingChecked = true;
					}
					return mAllowBackendSuccessorExpansionBlockingActivated;
				}


				bool CCalculationConfigurationExtension::isOccurrenceStatisticsCollectionActivated() {
					if (!mOccurrenceStatisticsCollectionChecked) {
						mOccurrenceStatisticsCollectionActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Calculation.Optimization.OccurrenceStatisticsCollecting", true);
						mOccurrenceStatisticsCollectionChecked = true;
					}
					return mOccurrenceStatisticsCollectionActivated;
				}


				bool CCalculationConfigurationExtension::isGeneratingTestQueriesActivated() {
					if (!mGeneratingTestQueriesChecked) {
						mGeneratingTestQueriesActivated = CConfigDataReader::readConfigBoolean(this, "Konclude.Test.ConjunctiveQueryGeneration.CompletionGraphRandomWalks", true);
						mGeneratingTestQueriesChecked = true;
					}
					return mGeneratingTestQueriesActivated;
				}

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
