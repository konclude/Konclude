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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETONTOLOGYREALIZINGTHREAD_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETONTOLOGYREALIZINGTHREAD_H

// Libraries includes
#include <QStack>


// Namespace includes
#include "RealizerSettings.h"
#include "CRealizerThread.h"
#include "CRealizingTestingItem.h"
#include "CIndividualConceptInstanceTestingItem.h"
#include "CIndividualPairRoleInstanceTestingItem.h"
#include "CIndividualSameTestingItem.h"
#include "COptimizedRepresentativeKPSetOntologyRealizingItem.h"
#include "COptimizedKPSetIndividualItem.h"
#include "COptimizedKPSetConceptInstancesItem.h"
#include "COptimizedKPSetConceptInstancesData.h"
#include "CRealizationMarkerCandidatesMessageData.h"
#include "CIndividualRoleCandidatePropagationTestingItem.h"
#include "CIndividualsConsistencyTestingItem.h"
#include "CIndividualRoleCandidateTestingData.h"
#include "COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData.h"
#include "CIndividualRoleCandidateConfirmationTestingItem.h"

// Other includes
#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"
#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskIndividualDependenceTrackingAdapter.h"

#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNodeVector.h"

#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCache.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheReader.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"
#include "Reasoner/Generator/CConcreteOntologyUpdateBuilder.h"

#include "Reasoner/Preprocess/CRoleChainAutomataTransformationPreProcess.h"
#include "Reasoner/Preprocess/CPreProcessContextBase.h"

#include "Utilities/Memory/CTempMemoryPoolContainerAllocationManager.h"

// Logger includes
#include "Logger/CLogger.h"


#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

	//#define REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS

#endif



namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Kernel::Process;
		using namespace Kernel::Manager;
		using namespace Kernel::Cache;
		using namespace Generator;
		using namespace Preprocess;

		namespace Realizer {

			using namespace Events;


			/*! 
			 *
			 *		\class		COptimizedRepresentativeKPSetOntologyRealizingThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetOntologyRealizingThread : public CRealizerThread, public CLogDomain {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetOntologyRealizingThread(CReasonerManager* reasonerManager);

					//! Destructor
					~COptimizedRepresentativeKPSetOntologyRealizingThread();


				// protected methods
				protected:
					class CKnownPossibleInstanceHashingData {
						public:
							CKnownPossibleInstanceHashingData() {
								mKnownInstance = false;
								mDirectInserted = false;
								mMostSpecialRetest = false;
								mInstanceItem = nullptr;
							}
							bool mKnownInstance;
							bool mDirectInserted;
							bool mMostSpecialRetest;
							COptimizedKPSetConceptInstancesItem* mInstanceItem;
					};


					class CPossibleConceptInstancesInitializationBatchingData {
						public:
							QList<COptimizedRepresentativeKPSetConceptSetCacheLabelItemData*> mLabelItemDataList;
							cint64 mLabelItemAssociatedIndiCount = 0;

							void addLabelItemData(COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* itemData) {
								mLabelItemDataList.append(itemData);
								mLabelItemAssociatedIndiCount += itemData->getLabelCacheItem()->getIndividualAssociationCount();
							}
					};



					class CPossibleConceptInstanceInitializationLinker : public CLinkerBase<cint64, CPossibleConceptInstanceInitializationLinker> {
					public:
						COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* mLabelItemData = nullptr;

						CPossibleConceptInstanceInitializationLinker* initInitializationLinker(cint64 indiId, COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* labelItemData) {
							setData(indiId);
							mLabelItemData = labelItemData;
							return this;
						}
					};

					class CPossibleConceptInstanceInitializationLinkerData {
					public:
						CPossibleConceptInstanceInitializationLinker* mFirst = nullptr;
						CPossibleConceptInstanceInitializationLinker* mLast = nullptr;

						CPossibleConceptInstanceInitializationLinkerData* addLinker(CPossibleConceptInstanceInitializationLinker* linker) {
							if (!mFirst) {
								mFirst = mLast = linker;
							} else {
								mFirst = linker->append(mFirst);
							}
							return this;
						}

						CPossibleConceptInstanceInitializationLinkerData* addLinkerData(const CPossibleConceptInstanceInitializationLinkerData& data) {
							if (data.mFirst) {
								if (!mFirst) {
									mFirst = data.mFirst;
									mLast = data.mLast;
								} else {
									mLast->setNext(data.mFirst);
									mLast = data.mLast;
								}
							}
							return this;
						}
					};





					class CSameIndividualInstanceInitializationLinker : public CLinkerBase<cint64, CSameIndividualInstanceInitializationLinker> {
					public:
						COptimizedKPSetIndividualItem* mIndiItem = nullptr;

						CSameIndividualInstanceInitializationLinker* initInitializationLinker(cint64 indiId, COptimizedKPSetIndividualItem* indiItem) {
							setData(indiId);
							mIndiItem = indiItem;
							return this;
						}
					};

					class CSameIndividualInstanceInitializationLinkerData {
					public:
						CSameIndividualInstanceInitializationLinker* mFirst = nullptr;
						CSameIndividualInstanceInitializationLinker* mLast = nullptr;

						CSameIndividualInstanceInitializationLinkerData* addLinker(CSameIndividualInstanceInitializationLinker* linker) {
							if (!mFirst) {
								mFirst = mLast = linker;
							} else {
								mFirst = linker->append(mFirst);
							}
							return this;
						}

						CSameIndividualInstanceInitializationLinkerData* addLinkerData(const CSameIndividualInstanceInitializationLinkerData& data) {
							if (data.mFirst) {
								if (!mFirst) {
									mFirst = data.mFirst;
									mLast = data.mLast;
								} else {
									mLast->setNext(data.mFirst);
									mLast = data.mLast;
								}
							}
							return this;
						}
					};



					virtual COntologyRealizingItem* initializeOntologyRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config);
					virtual void readCalculationConfig(CConfigurationBase *config);

					virtual bool createNextTest();

					CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem* getEntailmentIndividualConceptInstanceTestingItem(QList<CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem *>* entIndConInstTestList, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);

					virtual bool finishOntologyRealizing(COptimizedRepresentativeKPSetOntologyRealizingItem* totallyPreCompItem);
					virtual bool realizingTested(COntologyRealizingItem* ontPreCompItem, CRealizingTestingItem* preTestItem, CRealizingCalculatedCallbackEvent* pcce);

					bool interpretConceptInstantiationResult(COntologyRealizingItem* ontPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem, bool isIndividualInstance);

					bool visitRepresentativeCacheNeighbours(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CRole* role, CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, function<bool(cint64 neighbourIndiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssData, bool deterministicNeighbour)> visitFunc);

					bool checkTrivialConceptInstanceModelMerging(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem, bool &isInstance, bool shouldRememberChecking = false);
					bool checkTrivialInstanceConceptCachedCompletionGraphBasedMerging(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CConcept* concept, bool conceptNegation, CIndividualProcessNode* indiNode, CIndividualProcessNodeVector* indiProcVector, bool* mergableFlag, bool* unmergableFlag, cint64* mergingOperationLimit);
					bool checkTrivialInstanceConceptRepresentativeCacheBasedMerging(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CConcept* concept, bool conceptNegation, CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, bool* mergableFlag, bool* unmergableFlag, cint64* mergingOperationLimit);


					bool createNextConceptInstantiationTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem, cint64 remainingTestingCount, COntologyRealizingDynamicRequirmentProcessingData* procData = nullptr);
					bool createNextRoleInstantiationTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetIndividualItemPair itemPair, COntologyRealizingDynamicRequirmentProcessingData* procData = nullptr);
					bool createNextSameIndividualsTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* instantiatedItem1, COptimizedKPSetIndividualItem* instantiatedItem2, COntologyRealizingDynamicRequirmentProcessingData* procData = nullptr);
					bool createIndividualsConsistencyTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);





					bool createNextRoleInitializingTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, const CRealizationIndividualInstanceItemReference& indiRealItemRef);
					bool createNextRoleInitializingTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstancesItem);
					bool createNextRoleInitializingTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, const CRealizationIndividualInstanceItemReference& indiRealItemRef, const QList<CIndividualRoleCandidateTestingData>& indiRoleCandTestDataList);
					bool createNextRoleInitializingTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, const CRealizationIndividualInstanceItemReference& indiRealItemRef);

					bool prepareRoleInitializingTestData(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inverse, const CRealizationIndividualInstanceItemReference& indiRealItemRef, COptimizedKPSetIndividualComplexRoleData* indiComplexRoleData, QList<CIndividualRoleCandidateTestingData>& indiRoleCandTestDataList);

					bool collectTransitiveLinks(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, const CRealizationIndividualInstanceItemReference& indiRealItemRef, COptimizedKPSetIndividualComplexRoleData* indiComplexRoleData);



					bool createNextRoleComplexCandidateConfirmationTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstancesItem, COptimizedKPSetIndividualItemReferencePair candidatePair, COntologyRealizingDynamicRequirmentProcessingData* procData = nullptr);



					bool initializeSamePossibleIndividuals(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* individualItem, const QList<CIndividualReference>& possibleSameIndividualList);

					bool initializeItems(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);
					bool initializeRepresentativeRoleSetCacheLabelItems(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);
					bool initializeRepresentativeConceptSetCacheLabelItems(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);
					bool addRepresentativeCacheLabelItemToConceptInstancesItem(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem, COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* labelCacheItemData, COptimizedKPSetConceptInstancesItem* instancesItem, bool knownInstancesItem);
					virtual bool initializeIndividualProcessingKPSetsFromConsistencyData(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);


					bool initializeRepresentativeIndividualProcessing(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, cint64 indiId, CBackendRepresentativeMemoryLabelCacheItem* conSetLabelItem, COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* conSetLabelCacheItemData, QHash<cint64, QList<CIndividualReference> >** individualPossibleSameIndividualListHash);


					bool extractKnownPossibleIndividualDataFromConsistencyData(CIndividualProcessNode* indiProcNode, QList<COptimizedKPSetConceptInstancesItem*>* knownInstancesList, QList<COptimizedKPSetConceptInstancesItem*>* possibleInstancesList, QList<CIndividualReference>* knownSameIndividualList, QList<CIndividualReference>* possibleSameIndividualList, CIndividualProcessNodeVector* indiProcVector, const QList<COptimizedKPSetConceptInstancesItem*>& equivClassList, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);


					bool initializeEquivalentClassList(QList<COptimizedKPSetConceptInstancesItem*>* equivClassList, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);

					COptimizedKPSetIndividualItem* initializeKPSetsForIndividualConcurrently(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, cint64 indiId, CIndividual* individual, QList<COptimizedKPSetConceptInstancesItem*>& knownInstancesList, QList<COptimizedKPSetConceptInstancesItem*>& possibleInstancesList, QList<CIndividualReference>& knownSameIndividualIdList);

					bool initializeKPSetsForIndividual(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, cint64 indiId, CIndividual* individual, QList<COptimizedKPSetConceptInstancesItem*>& knownInstancesList, QList<COptimizedKPSetConceptInstancesItem*>& possibleInstancesList, QList<CIndividualReference>& knownSameIndividualIdList);
					bool addKPSetDirectSuperInstances(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash, COptimizedKPSetConceptInstancesItem* instanceItem, bool knownInstance);

					CIndividualProcessNode* getMergeCorrectedIndividualProcessNode(CIndividualProcessNode* indiProcNode, CIndividualProcessNodeVector* indiProcVector, bool* nonDeterministicallyMergedFlag, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);


					COptimizedRepresentativeKPSetOntologyRealizingThread* incTestedPossibleConceptInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingThread* incOpenPossibleConceptInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount = 1);

					COptimizedRepresentativeKPSetOntologyRealizingThread* decTestedPossibleConceptInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingThread* decOpenPossibleConceptInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount = 1);



					COptimizedRepresentativeKPSetOntologyRealizingThread* incTestedPossibleRoleInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingThread* incOpenPossibleRoleInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount = 1);

					COptimizedRepresentativeKPSetOntologyRealizingThread* decTestedPossibleRoleInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingThread* decOpenPossibleRoleInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount = 1);




					COptimizedRepresentativeKPSetOntologyRealizingThread* incTestedPossibleSameIndividualsCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingThread* incOpenPossibleSameIndividualsCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount = 1);

					COptimizedRepresentativeKPSetOntologyRealizingThread* decTestedPossibleSameIndividualsCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingThread* decOpenPossibleSameIndividualsCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount = 1);


					virtual COptimizedRepresentativeKPSetOntologyRealizingThread* addRealizationStatistics(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CRealizingTestingStep* ontProcStep);
					virtual COptimizedRepresentativeKPSetOntologyRealizingThread* addIndividualDependencyTrackingStatistics(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CRealizingTestingStep* ontProcStep, const QString& realizationTypeString);


					COptimizedRepresentativeKPSetOntologyRealizingThread* updateParentItemsSuccessorProcessed(COptimizedKPSetConceptInstancesItem* item);
					COptimizedRepresentativeKPSetOntologyRealizingThread* updateParentItemsSuccessorProcessed(COptimizedKPSetRoleInstancesItem* item);
					
					virtual CRealizationProgress* getRealizationProgress();

					COptimizedRepresentativeKPSetOntologyRealizingThread* updateCalculatedKnownConceptInstance(COptimizedKPSetIndividualItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);
					COptimizedRepresentativeKPSetOntologyRealizingThread* updateCalculatedNonConceptInstance(COptimizedKPSetIndividualItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);

					COptimizedRepresentativeKPSetOntologyRealizingThread* mergeSameIndividualItems(COptimizedKPSetIndividualItem* instantiatedItem1, COptimizedKPSetIndividualItem* instantiatedItem2, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);


					COptimizedRepresentativeKPSetOntologyRealizingThread* createTemporarySameRealizationOntology(COptimizedRepresentativeKPSetOntologyRealizingItem* item);

					COptimizedRepresentativeKPSetOntologyRealizingThread* createTemporaryRoleRealizationOntology(COptimizedRepresentativeKPSetOntologyRealizingItem* item);


					CConcept* createTemporaryConcept(COptimizedRepresentativeKPSetOntologyRealizingItem* item, CConcreteOntology* tmpRoleRealOntology);
					void addTemporaryConceptOperand(CConcept* concept, CConcept* opConcept, bool negated, COptimizedRepresentativeKPSetOntologyRealizingItem* item, CConcreteOntology* tmpRoleRealOntology);

					COptimizedRepresentativeKPSetOntologyRealizingThread* updateCalculatedKnownRoleInstance(COptimizedKPSetIndividualItemPair individualItemPair, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);
					COptimizedRepresentativeKPSetOntologyRealizingThread* updateCalculatedNonRoleInstance(COptimizedKPSetIndividualItemPair individualItemPair, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem);

					virtual bool processRealizationMessage(COntologyRealizingItem* ontRealItem, CRealizationMessageData* messageData, CMemoryPool* memoryPools);
					
					bool addKPSetDirectSuperInstances(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstItem, bool inversed, COptimizedKPSetIndividualItem* indiItem1, COptimizedKPSetIndividualItem* indiItem2, bool knownInstance);
					bool addComplexRoleExplicitIndirectNeighbourLink(COptimizedKPSetRoleInstancesItem* roleInstItem, bool inversed,
						const CRealizationIndividualInstanceItemReference& indiRealItemRef, COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData,
						COptimizedKPSetRoleNeighbourInstancesHashData* indiRoleNeighbourHashData, COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* combinedNeighbourCacheLabelItemData, CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* assNeighbourRoleSetHash,
						const CIndividualReference& neighbourIndi, COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* neighbourIndiExplicitIndirectLinkComplexRepresentationData,
						bool deterministic, COptimizedRepresentativeKPSetOntologyRealizingItem* realItem);

					CIndividualProcessNode* getCompletionGraphCachedIndividualProcessNode(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* indiItem, bool* nondeterministicNodeFlag = nullptr);



					bool checkFinishSameIndividualProcessing(COntologyRealizingItem* ontRealItem, COptimizedKPSetIndividualItem* instantiatedItem);
					COptimizedRepresentativeKPSetOntologyRealizingThread* setDynamicRequirementProcessed(COntologyRealizingItem* ontRealItem, CRealizingTestingStep* realizingStep, CLinker<COntologyRealizingDynamicRequirmentProcessingData*>* procDataLinker);
					COptimizedRepresentativeKPSetOntologyRealizingThread* setDynamicRequirementProcessed(COntologyRealizingItem* ontRealItem, CRealizingTestingStep* realizingStep, COntologyRealizingDynamicRequirmentProcessingData* procData);
					COptimizedRepresentativeKPSetOntologyRealizingThread* setDynamicRequirementProcessed(COntologyRealizingItem* ontRealItem, CRealizingTestingStep* realizingStep, const QList<COntologyRealizingDynamicRequirmentProcessingData*>& procDataList);
					bool checkFinishConceptInstancesProcessing(COntologyRealizingItem* ontRealItem, COptimizedKPSetConceptInstancesItem* conceptItem);
					bool checkFinishRoleInstancesProcessing(COntologyRealizingItem* ontRealItem, COptimizedKPSetRoleInstancesItem* roleItem);

					COptimizedKPSetIndividualItem* getMergingResolvedIndividualItem(COptimizedKPSetIndividualItem* indiItem);


					QSet<QString> debugGetIndividualTypesStringSet(COptimizedRepresentativeKPSetOntologyRealizingItem* realizingItem);
					bool debugCheckIndividualTypesFromFile(COptimizedRepresentativeKPSetOntologyRealizingItem* realizingItem);
					bool debugWriteIndividualTypesToFile(COptimizedRepresentativeKPSetOntologyRealizingItem* realizingItem);





					CPossibleInstancesIndividualsMergingData* getPossibleInstancesMergingData(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem, cint64 remainingTestingCount);


				// protected variables
				protected:
					cint64 mTestedPossibleInstancesCount;
					cint64 mOpenPossibleInstancesCount;

					CRealizationProgress mRealProgress;
					QTime mRealStartTime;

					CBackendRepresentativeMemoryCache* mBackendAssocCache;
					//CBackendRepresentativeMemoryCacheReader* mBackendAssocCacheReader;

					bool mAlternateShufflePossibleConceptInstanceTestingItemsOnCalculations;
					bool mConfPossibleInstanceConceptsUpfrontMerging;
					bool mConfPossibleInstanceIndividualsAfterwardsMerging;
					bool mConfPossibleInstanceIndividualsAfterwardsMergingOnlyWithSameRepresentativeLabel;
					cint64 mConfPossibleInstanceIndividualsAfterwardsMergingProvidingCount;
					cint64 mConfPossibleInstanceIndividualsAfterwardsMergingMaximumAttemptCount;
					bool mConfNonDeterministicSatisfiableCalculationContinuation;
					cint64 mConfMinimumPossibleInstanceIndividualsAfterwardsMergingCount;


#ifdef REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS
					QStringList mDebugTransitiveCollectionStringList;
					QString mDebugTransitiveCollectionString;
#endif // REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS


					bool mConfConcurrentIndividualPossibleConceptInstantiationInitialization;
					cint64 mConfConcurrentHandlingVectorSize;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETONTOLOGYREALIZINGTHREAD_H
