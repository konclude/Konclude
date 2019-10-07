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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLEINSTANCESITEM_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLEINSTANCESITEM_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetIndividualItem.h"
#include "COptimizedKPSetIndividualItemPair.h"
#include "COptimizedKPSetIndividualItemReferencePair.h"
#include "COptimizedKPSetRoleInstancesRedirectionItem.h"
#include "COntologyRealizingDynamicRequirmentProcessingContainer.h"
#include "COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData.h"
#include "COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash.h"
#include "COptimizedKPSetIndividualComplexRoleData.h"
#include "COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator.h"
#include "COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"

#include "Reasoner/Taxonomy/CRolePropertiesHierarchyNode.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Realization;
		using namespace Taxonomy;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetRoleInstancesItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetRoleInstancesItem : public COptimizedKPSetRoleInstancesRedirectionItem, public COntologyRealizingDynamicRequirmentProcessingContainer {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetRoleInstancesItem();

					COptimizedKPSetRoleInstancesItem* initInstancesItem(CRole* role);

					COptimizedKPSetRoleInstancesItem* setInitialUnprocessedSuccessorItemCount();
					COptimizedKPSetRoleInstancesItem* setInitialUninitializedSuccessorItemCount();

					CRole* getRole();
					COptimizedKPSetRoleInstancesItem* setRole(CRole* role);


					CRolePropertiesHierarchyNode* getRoleHierarchyNode();
					COptimizedKPSetRoleInstancesItem* setRoleHierarchyNode(CRolePropertiesHierarchyNode* roleHierNode);


					CRolePropertiesHierarchyNode* getInverseRoleHierarchyNode();
					COptimizedKPSetRoleInstancesItem* setInverseRoleHierarchyNode(CRolePropertiesHierarchyNode* roleHierNode);


					COptimizedKPSetRoleInstancesRedirectionItem* getInverseRoleRedirectedItem();
					COptimizedKPSetRoleInstancesItem* setInverseRoleRedirectedItem(COptimizedKPSetRoleInstancesRedirectionItem* redirectedItem);


					QSet<COptimizedKPSetIndividualItemPair>* getKnownInstancesSet();
					QSet<COptimizedKPSetIndividualItemPair>* getPossibleInstancesSet();

					QList<TRoleItemInversionPair>* getParentItemList();
					QList<TRoleItemInversionPair>* getSuccessorItemList();

					bool hasKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair);
					bool hasPossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair);

					COptimizedKPSetRoleInstancesItem* addKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair);
					COptimizedKPSetRoleInstancesItem* removeKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair);
					COptimizedKPSetRoleInstancesItem* addPossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair);
					COptimizedKPSetRoleInstancesItem* removePossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair);


					bool hasKnownInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair);
					bool hasPossibleInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair);

					COptimizedKPSetRoleInstancesItem* addKnownInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair);
					COptimizedKPSetRoleInstancesItem* removeKnownInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair);
					COptimizedKPSetRoleInstancesItem* addPossibleInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair);
					COptimizedKPSetRoleInstancesItem* removePossibleInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair);


					bool hasKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse);
					bool hasPossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse);

					COptimizedKPSetRoleInstancesItem* addKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse);
					COptimizedKPSetRoleInstancesItem* removeKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse);
					COptimizedKPSetRoleInstancesItem* addPossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse);
					COptimizedKPSetRoleInstancesItem* removePossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse);



					COptimizedKPSetRoleInstancesItem* addParentItem(COptimizedKPSetRoleInstancesItem* item, bool inversed);
					COptimizedKPSetRoleInstancesItem* addSuccessorItem(COptimizedKPSetRoleInstancesItem* item, bool inversed);

					cint64 getUnprocessedSuccessorItemCount();
					COptimizedKPSetRoleInstancesItem* setUnprocessedSuccessorItemCount(cint64 unpSuccCount);
					COptimizedKPSetRoleInstancesItem* incUnprocessedSuccessorItemCount(cint64 incCount = 1);
					COptimizedKPSetRoleInstancesItem* decUnprocessedSuccessorItemCount(cint64 decCount = 1);

					cint64 getTestingPossibleInstancesCount();
					bool hasTestingPossibleInstances();
					COptimizedKPSetRoleInstancesItem* setTestingPossibleInstancesCount(cint64 testPossIntCount);
					COptimizedKPSetRoleInstancesItem* incTestingPossibleInstancesCount(cint64 incCount = 1);
					COptimizedKPSetRoleInstancesItem* decTestingPossibleInstancesCount(cint64 decCount = 1);

					bool hasAllSuccessorProcessedFlag();
					COptimizedKPSetRoleInstancesItem* setAllSuccessorProcessedFlag(bool allSuccProcessed);

					bool hasToProcessPossibleInstancesFlag();
					COptimizedKPSetRoleInstancesItem* setToProcessPossibleInstancesFlag(bool toProcess);

					bool hasPossibleInstancesProcessingQueuedFlag();
					COptimizedKPSetRoleInstancesItem* setPossibleInstancesProcessingQueuedFlag(bool processingQueued);


					bool hasComplexCandidateInstancesProcessingQueuedFlag();
					COptimizedKPSetRoleInstancesItem* setComplexCandidateProcessingQueuedFlag(bool processingQueued);


					bool hasSelfSuccessorProcessedFlag();
					COptimizedKPSetRoleInstancesItem* setSelfSuccessorProcessedFlag(bool selfSuccProcessed);

					bool hasPossibleInstances();
					COptimizedKPSetIndividualItemPair takeNextTestingPossibleInstance();

					bool requiresCandidateInitialization();
					COptimizedKPSetRoleInstancesItem* setRequiresCandidateInitialization(bool requiresInit);

					bool hasRemainingCandidateInitializationSuccessors();
					QList<COptimizedKPSetIndividualItem*>* getRemainingCandidateInitializationSuccessorList();
					COptimizedKPSetIndividualItem* takeNextRemainingCandidateInitializationSuccessor();
					COptimizedKPSetRoleInstancesItem* addRemainingCandidateInitializationSuccessor(COptimizedKPSetIndividualItem* item);
					COptimizedKPSetRoleInstancesItem* addRemainingCandidateInitializationSuccessors(QList<COptimizedKPSetIndividualItem*>* itemList);
					bool isCandidateSuccessorInitializationCompleted();
					bool isCandidateSuccessorInitializationStarted();
					COptimizedKPSetRoleInstancesItem* setCandidateSuccessorInitializationStarted(bool initStarted);
					COptimizedKPSetRoleInstancesItem* setCandidateSuccessorInitializationCompleted(bool initCompleted);


					cint64 getUninitializedSuccessorItemCount();
					COptimizedKPSetRoleInstancesItem* setUninitializedSuccessorItemCount(cint64 unintSuccCount);
					COptimizedKPSetRoleInstancesItem* incUninitializedSuccessorItemCount(cint64 incCount = 1);
					COptimizedKPSetRoleInstancesItem* decUninitializedSuccessorItemCount(cint64 decCount = 1);

					bool hasAllSuccessorInitializedFlag();
					COptimizedKPSetRoleInstancesItem* setAllSuccessorInitializedFlag(bool allSuccInitialized);

					bool hasInitializingQueuedFlag();
					COptimizedKPSetRoleInstancesItem* setInitializingQueuedFlag(bool initializingQueued);

					bool hasToInitializeCandidatesFlag();
					COptimizedKPSetRoleInstancesItem* setToInitializeCandidatesFlag(bool toInitialize);


					CConcept* getTemporaryPropagationConcept();
					CConcept* getTemporaryMarkerConcept();
					COptimizedKPSetRoleInstancesItem* setTemporaryPropagationConcept(CConcept* propagationConcept);
					COptimizedKPSetRoleInstancesItem* setTemporaryMarkerConcept(CConcept* markerConcept);

					

					CConcept* getTemporaryInversePropagationConcept();
					CConcept* getTemporaryInverseMarkerConcept();
					COptimizedKPSetRoleInstancesItem* setTemporaryInversePropagationConcept(CConcept* propagationConcept);
					COptimizedKPSetRoleInstancesItem* setTemporaryInverseMarkerConcept(CConcept* markerConcept);

					bool hasInverseRole();
					CRole* getInverseRole();
					COptimizedKPSetRoleInstancesItem* setInverseRole(CRole* role);


					QSet<CRole*>* getComplexRoleStarterCandidateSet();
					COptimizedKPSetRoleInstancesItem* addComplexRoleStarterCandidate(CRole* role);
					bool hasComplexRoleStarterCandidate(CRole* role);

					QSet<CRole*>* getComplexInverseRoleStarterCandidateSet();
					COptimizedKPSetRoleInstancesItem* addComplexInverseRoleStarterCandidate(CRole* role);
					bool hasComplexInverseRoleStarterCandidate(CRole* role);


					bool hasOnlyTransitiveComplexRoleCandidates();
					COptimizedKPSetRoleInstancesItem* setOnlyTransitiveComplexRoleCandidates(bool onlyTransitiveCandidates);

					QSet<TRoleItemInversionPair>* getOnlyTransitiveComplexSubRoleItems();
					COptimizedKPSetRoleInstancesItem* addOnlyTransitiveComplexSubRoleItem(const TRoleItemInversionPair& item);


					bool hasOnlyDeterministicComplexRoleStarterCandidatesUsage();
					COptimizedKPSetRoleInstancesItem* setOnlyDeterministicComplexRoleStarterCandidatesUsage(bool onlyDeterministicComplexRoleStarterCandidatesUsage);



					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* getCombinedNeighbourCacheLabelItemDataHash(bool inversed);
					bool hasCombinedNeighbourCacheLabelItemInversed(CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem);



					bool hasComplexRoleData();
					QHash<cint64, COptimizedKPSetIndividualComplexRoleData*>* getIndividualIdComplexRoleDataHash();
					COptimizedKPSetIndividualComplexRoleData* getIndividualIdComplexRoleData(cint64 individualId, bool createIfNotExists = true);



					COptimizedKPSetRoleInstancesItem* addComplexStarterCandidateCombinedNeighbourRoleLabelCacheItem(bool inversed, CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem);
					QSet<CBackendRepresentativeMemoryLabelCacheItem*>* getComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet(bool inversed);


					COptimizedKPSetRoleInstancesItem* addComplexStarterCandidateCombinedExistentialRoleLabelCacheItem(bool inversed, CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem);
					QSet<CBackendRepresentativeMemoryLabelCacheItem*>* getComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet(bool inversed);


					COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator* getIndividualComplexCandidateIterator();
					COptimizedKPSetRoleInstancesItem* setIndividualComplexCandidateIterator(COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator* iterator);




					COptimizedKPSetRoleInstancesItem* addComplexCandidateInstance(const COptimizedKPSetIndividualItemReferencePair& itemRefPair, bool inverse);
					COptimizedKPSetRoleInstancesItem* removeComplexCandidateInstance(const COptimizedKPSetIndividualItemReferencePair& itemRefPair, bool inverse);

					bool hasComplexInstanceCandidates();
					COptimizedKPSetIndividualItemReferencePair takeNextTestingComplexInstanceCandidate();




					COptimizedKPSetRoleInstancesItem* addExistentialRoleLabelCacheItem(bool inversed, CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem);
					QSet<CBackendRepresentativeMemoryLabelCacheItem*>* getExistentialRoleLabelCacheItemSet(bool inversed);

					
					enum RolePreferredPropagationDirection {
						STRAIGHT,
						INVERSE,
						NONE
					};

					bool hasPropagationDirectionDetermined();
					COptimizedKPSetRoleInstancesItem* setPropagationDirectionDetermined(bool determined);

					RolePreferredPropagationDirection getPreferredPropagationDirection();
					COptimizedKPSetRoleInstancesItem* setPreferredPropagationDirection(RolePreferredPropagationDirection direction);

				// protected methods
				protected:

				// protected variables
				protected:
					CRole* mInvRole;

					CRolePropertiesHierarchyNode* mRoleHierNode;
					CRolePropertiesHierarchyNode* mInvRoleHierNode;

					COptimizedKPSetRoleInstancesRedirectionItem* mInvRedirectedItem;


					CConcept* mTmpPropagationConcept;
					CConcept* mTmpMarkerConcept;

					QSet<COptimizedKPSetIndividualItemPair> mKnownInstancesSet;
					QSet<COptimizedKPSetIndividualItemPair> mPossibleInstancesSet;


					QSet<COptimizedKPSetIndividualItemReferencePair> mComplexRoleCandidateInstancesSet;


					CConcept* mTmpInversePropagationConcept;
					CConcept* mTmpInverseMarkerConcept;

					QList<TRoleItemInversionPair> mParentItemList;
					QList<TRoleItemInversionPair> mSuccessorItemList;

					cint64 mUnprocessedSuccItemCount;
					cint64 mTestingPossInstanceCount;
					bool mAllSuccProcessedFlag;
					bool mToProcessFlag;
					bool mPossibleProcessingQueuedFlag;
					bool mComplexCandidateProcessingQueuedFlag;
					bool mSelfSuccsCompletedFlag;


					bool mRequiresCandidateInitializationFlag;

					QList<COptimizedKPSetIndividualItem*> mRemainingCandidateInitializationSuccessorList;
					bool mInitializationStartedFlag;
					bool mInitializationCompletedFlag;
					cint64 mUninitializedSuccItemCount;
					bool mAllSuccInitializedFlag;
					bool mInitializingQueuedFlag;
					bool mToInitializeFlag;


					QSet<CRole*> mComplexRoleStarterCandidateSet;
					QSet<CRole*> mComplexInverseRoleStarterCandidateSet;

					bool mOnlyTransitiveComplexRoleCandidates;
					bool mOnlyDeterministicComplexRoleStarterCandidatesUsage;
					QSet<TRoleItemInversionPair> mOnlyTransitiveComplexSubRoleItems;


					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash mCombinedNeighbourLabelDataHash;
					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash mInverseCombinedNeighbourLabelDataHash;


					QHash<cint64, COptimizedKPSetIndividualComplexRoleData*> mIndividualIdComplexRoleDataHash;


					QSet<CBackendRepresentativeMemoryLabelCacheItem*> mComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet;
					QSet<CBackendRepresentativeMemoryLabelCacheItem*> mComplexInverseStarterCandidateCombinedNeighbourRoleLabelCacheItemSet;

					QSet<CBackendRepresentativeMemoryLabelCacheItem*> mComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet;
					QSet<CBackendRepresentativeMemoryLabelCacheItem*> mComplexInverseStarterCandidateCombinedExistentialRoleLabelCacheItemSet;


					QSet<CBackendRepresentativeMemoryLabelCacheItem*> mExistentialRoleLabelCacheItemSet;
					QSet<CBackendRepresentativeMemoryLabelCacheItem*> mExistentialInverseRoleLabelCacheItemSet;


					COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator* mComplexIndiCandidateIterator;


					bool mPropagationDirectionDetermined;
					RolePreferredPropagationDirection mPreferredPropagationDirection;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLEINSTANCESITEM_H
