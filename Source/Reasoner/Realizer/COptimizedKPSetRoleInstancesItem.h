/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLEINSTANCESITEM_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLEINSTANCESITEM_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetIndividualItem.h"
#include "COptimizedKPSetIndividualItemPair.h"
#include "COptimizedKPSetRoleInstancesRedirectionItem.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Realization;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetRoleInstancesItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetRoleInstancesItem : public COptimizedKPSetRoleInstancesRedirectionItem {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetRoleInstancesItem();

					COptimizedKPSetRoleInstancesItem* initInstancesItem(CRole* role);

					COptimizedKPSetRoleInstancesItem* setInitialUnprocessedSuccessorItemCount();
					COptimizedKPSetRoleInstancesItem* setInitialUninitializedSuccessorItemCount();

					CRole* getRole();
					COptimizedKPSetRoleInstancesItem* setRole(CRole* role);

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



				// protected methods
				protected:

				// protected variables
				protected:
					CRole* mInvRole;

					CConcept* mTmpPropagationConcept;
					CConcept* mTmpMarkerConcept;

					QSet<COptimizedKPSetIndividualItemPair> mKnownInstancesSet;
					QSet<COptimizedKPSetIndividualItemPair> mPossibleInstancesSet;


					CConcept* mTmpInversePropagationConcept;
					CConcept* mTmpInverseMarkerConcept;

					QSet<COptimizedKPSetIndividualItemPair> mKnownInverseInstancesSet;
					QSet<COptimizedKPSetIndividualItemPair> mPossibleInverseInstancesSet;

					QList<TRoleItemInversionPair> mParentItemList;
					QList<TRoleItemInversionPair> mSuccessorItemList;

					cint64 mUnprocessedSuccItemCount;
					cint64 mTestingPossInstanceCount;
					bool mAllSuccProcessedFlag;
					bool mToProcessFlag;
					bool mProcessingQueuedFlag;
					bool mSelfSuccsCompletedFlag;


					bool mRequiresCandidateInitializationFlag;

					QList<COptimizedKPSetIndividualItem*> mRemainingCandidateInitializationSuccessorList;
					bool mInitializationStartedFlag;
					cint64 mUninitializedSuccItemCount;
					bool mAllSuccInitializedFlag;
					bool mInitializingQueuedFlag;
					bool mToInitializeFlag;


					QSet<CRole*> mComplexRoleStarterCandidateSet;
					QSet<CRole*> mComplexInverseRoleStarterCandidateSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLEINSTANCESITEM_H
