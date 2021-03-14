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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEREPRESENTATIVEMEMORYBACKENDCACHESYNCHRONISATIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEREPRESENTATIVEMEMORYBACKENDCACHESYNCHRONISATIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualNodeBackendCacheSynchronisationData.h"
#include "CConceptDescriptor.h"
#include "CProcessContext.h"
#include "CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData.h"
#include "CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationNeighbourExpansionData.h"
#include "CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData.h"
#include "CBackendNeighbourExpansionQueue.h"

// Other includes
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheIndividualAssociationData.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Cache;

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData : public CIndividualNodeBackendCacheSynchronisationData {
					// public methods
					public:
						//! Constructor
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData(CProcessContext* context);

						//! Destructor
						virtual ~CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData();

						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* initSynchronisationData(CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* synchData);
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* initSynchronisationData(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData);


						CConceptDescriptor* getLastSynchronizationTestedConceptDescriptor();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastSynchronizationTestedConceptDescriptor(CConceptDescriptor* conDes);

						CIndividualLinkEdge* getLastCriticalCardinalityLinkEdge();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastCriticalCardinalityLinkEdge(CIndividualLinkEdge* linkEdge);
						bool hasCriticalCardinalityInitiallyChecked();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setCriticalCardinalityInitiallyChecked(bool checked);


						CConceptDescriptor* getLastCriticalNeighbourExpansionTestedConceptDescriptor();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastCriticalNeighbourExpansionTestedConceptDescriptor(CConceptDescriptor* conDes);

						CConceptDescriptor* getLastSynchedConceptDescriptor();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastSynchedConceptDescriptor(CConceptDescriptor* conDes);

						CBackendRepresentativeMemoryCacheIndividualAssociationData* getAssocitaionData();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setAssociationData(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData);

						bool isBackendCacheSynchron();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setBackendCacheSynchron(bool synchron);

						bool isCriticalNeighbourExpansionBlocking();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setCriticalNeighbourExpansionBlocking(bool critical);

						bool isCriticalCardinalityExpansionBlocking();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setCriticalCardinalityExpansionBlocking(bool critical);


						CConceptDescriptor* getLastNeighbourInfluenceTestedConceptDescriptor();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastNeighbourInfluenceTestedConceptDescriptor(CConceptDescriptor* conDes);
						CPROCESSHASH< QPair<CRole*, bool>, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData >* getRoleNeighbourExpansionDataHash(bool createIfNotExists = true);
						CPROCESSHASH< cint64, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationNeighbourExpansionData >* getNeighbourExpansionDataHash(bool createIfNotExists = true);
						CPROCESSHASH< CBackendRepresentativeMemoryLabelCacheItem*, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData >* getNeighbourLabelExpansionDataHash(cint64 arrayId, bool createIfNotExists = true);
						bool hasNeighbourInfluenceTestingCriticalCardinalityReset();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* hasNeighbourInfluenceTestingCriticalCardinalityReset(bool reset);

						cint64 getLastMergedIntoIndividualTestingCriticalCardinalityReset();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastMergedIntoIndividualTestingCriticalCardinalityReset(cint64 indiId);



						CIndividualLinkEdge* getLastNewNeighbourLinkEdge();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastNewNeighbourLinkEdge(CIndividualLinkEdge* linkEdge);


						bool hasNominalIndirectConnectionIndividualExpanded();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setNominalIndirectConnectionIndividualExpanded(bool expanded);


						bool isCriticalIndirectConnectionIndividualExpansionBlocking();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setCriticalIndirectConnectionIndividualExpansionBlocking(bool critical);

						CIndividualLinkEdge* getLastIndirectConnectedIndividualExpansionTestedLinkEdge();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastIndirectConnectedIndividualExpansionTestedLinkEdge(CIndividualLinkEdge* linkEdge);

						CConceptDescriptor* getLastIndirectConnectedIndividualExpansionTestedConceptDescriptor();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastIndirectConnectedIndividualExpansionTestedConceptDescriptor(CConceptDescriptor* conDes);

						bool hasDeterministicSameIndividualMerged();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setDeterministicSameIndividualMerged(bool merged);

						bool hasDeterministicDifferentIndividualDifferentiated();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setDeterministicDifferentIndividualDifferentiated(bool differentiated);




						bool hasPrioritziedPropagationLinksEstablished();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setPrioritziedPropagationLinksEstablished(bool established);



						cint64 getLastMergedIndividualCount();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastMergedIndividualCount(cint64 count);

						CXLinker<cint64>* getLastProcessedMergedIndividualLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastProcessedMergedIndividualLinker(CXLinker<cint64>* indiLinker);


						CPROCESSSET<cint64>* getIntegratedMergedIndividualIdSet(bool createIfNotExists = true);
						CPROCESSSET<cint64>* getRemainingIndirectCompatibilityMergedIndividualCheckingSet(bool createIfNotExists = true);

						CXLinker<CIndividualProcessNode*>* getMergedIndividualNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* addMergedIndividualNodeLinker(CXLinker<CIndividualProcessNode*>* linker);


						CXLinker<CIndividualProcessNode*>* getLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker);
						
						CXLinker<CIndividualProcessNode*>* getLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker);



						CXLinker<CIndividualProcessNode*>* getLastSynchronizedConceptsTestedMergedNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastSynchronizedConceptsTestedMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker);

						CXLinker<CIndividualProcessNode*>* getLastCriticalNeighboursTestedMergedNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastCriticalNeighboursTestedMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker);

						CXLinker<CIndividualProcessNode*>* getLastDirectExpansionHandledMergedNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastDirectExpansionHandledMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker);

						CXLinker<CIndividualProcessNode*>* getLastInferringExpansionHandledMergedNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastInferringExpansionHandledMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker);



						CXLinker<CIndividualProcessNode*>* getLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker);


						CXLinker<CIndividualProcessNode*>* getLastIndirectCompatibilityExpansionHandledMergedNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastIndirectCompatibilityExpansionHandledMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker);




						bool hasMergedIndividualsAllNeighbourExpanded();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setMergedIndividualsAllNeighbourExpanded(bool expanded);
						bool hasMergedIndirectlyConnectedNominalIndividuals();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setMergedIndirectlyConnectedNominalIndividuals(bool connection);

						bool hasAllNeighbourExpansion();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setAllNeighbourExpansion(bool expanded);
						bool hasAllNeighbourForcedExpansion();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setAllNeighbourForcedExpansion(bool expanded);


						bool hasAllNeighbourExpansionScheduled();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setAllNeighbourExpansionScheduled(bool expansionScheduled);
						bool hasAllNeighbourForcedExpansionScheduled();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setAllNeighbourForcedExpansionScheduled(bool expansionScheduled);



						CXLinker<CIndividualProcessNode*>* getConceptSetLabelProcessedNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setConceptSetLabelProcessedNodeLinker(CXLinker<CIndividualProcessNode*>* nodeLinker);



						bool hasNonConceptSetBackendLabelRelatedProcessing();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setNonConceptSetBackendLabelRelatedProcessing(bool nonConceptSetRelatedProcessing);



						bool hasNonConceptSetBackendNeighbourLabelRelatedProcessing();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setNonConceptSetBackendNeighbourLabelRelatedProcessing(bool nonConceptSetRelatedProcessing);


						bool hasNeighbourLabelRepresentativeExpansion();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setNeighbourLabelRepresentativeExpansion(bool repExp);

						bool hasNeighbourLabelRepresentativeExpansionInstalled();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* installNeighbourLabelRepresentativeExpansion(CXLinker<CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData*>* linker);
						CXLinker<CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData*>* getNeighbourLabelRepresentativeExpansionLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* clearNeighbourLabelRepresentativeExpansionLinker();



						bool isBackendConceptSetInitialized();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setBackendConceptSetInitialized(bool initialized);

						bool isBackendConceptSetInitializationQueued();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setBackendConceptSetInitializationQueued(bool queued);



						bool isBackendConceptSetInitializationRequired();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setBackendConceptSetInitializationRequired(bool initRequired);

						bool isBackendConceptSetInitializationDelayingRegistered();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setBackendConceptSetInitializationDelayingRegistered(bool registered);



						CNonDeterministicDependencyTrackPoint* getBackendExpansionReuseDependencyTrackPoint();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setBackendExpansionReuseDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint);

						CXLinker<CIndividualProcessNode*>* getLastReuseExpansionHandledMergedNodeLinker();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setLastReuseExpansionHandledMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker);
						
						bool hasReuseNonDeterministicSameIndividualMerged();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setReuseNonDeterministicSameIndividualMerged(bool merged);

						bool hasReuseNonDeterministicDifferentIndividualStated();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setReuseNonDeterministicDifferentIndividualStated(bool stated);

						bool hasReuseNonDeterministicConceptsAdded();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setReuseNonDeterministicConceptsAdded(bool added);

						bool hasNewlyMergedIndividuals();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setNewlyMergedIndividuals(bool newlyMerged);

						bool hasNonDeterministicallyMergedIndividuals();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setNonDeterministicallyMergedIndividuals(bool nonDeterministicallyMerged);

						bool hasNewlyMergedAllNeighbourExpansion();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setNewlyMergedAllNeighbourExpansion(bool allExpanded);

						bool hasNewlyMergedInferringNeighbourExpansion();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setNewlyMergedInferringNeighbourExpansion(bool inferringExpanded);


						CBackendNeighbourExpansionQueue* getBackendNeighbourExpansionQueue(bool create = true);
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* clearBackendNeighbourExpansionQueue();

						CPROCESSHASH< CRole*, cint64 >* getRoleRepresentativeNeighbourCountHash(bool create = true);


						bool isScheduledIndividual();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* setScheduledIndividual(bool scheduled);


					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mContext;

						CConceptDescriptor* mLastCriticalNeighbourTestedConDes;
						CIndividualLinkEdge* mLastCriticalNeighbourLinkEdge;
						bool mCriticalCardinalityInitiallyChecked;
						CConceptDescriptor* mLastSynchronizationTestedConDes;
						CConceptDescriptor* mLastSynchedConDes;
						CBackendRepresentativeMemoryCacheIndividualAssociationData* mAssociationData;
						bool mSynchron;
						bool mCriticalNeighbour;
						bool mCriticalCardinality;
						bool mCriticalIndirectConnectedIndis;



						CConceptDescriptor* mLastNeighbourInfluenceTestedConDes;
						bool mNeighbourInfluenceTestingCriticalCardinalityReset;
						CPROCESSHASH< QPair<CRole*, bool>, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData >* mRoleNeighbourExpansionDataHash;
						CPROCESSHASH< cint64, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationNeighbourExpansionData >* mNeighbourExpansionDataHash;
						CPROCESSHASH< CBackendRepresentativeMemoryLabelCacheItem*, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData >** mNeighbourLabelExpansionDataHashArray;

						CPROCESSHASH< CRole*, cint64 >* mRoleRepNeighbourCountHash;


						CXLinker<CIndividualProcessNode*>* mMergedIndiNodeLinker;
						CPROCESSSET<cint64>* mRemainingIndirectCompatibilityMergedIndividualCheckingSet;
						CPROCESSSET<cint64>* mIntegratedMergedIndividualIdSet;
						bool mMergedIndividualsAllNeighbourExpanded;
						bool mMergedIndirectlyConnectedNominalIndividuals;
						bool mAllNeighbourExpansion;
						bool mAllNeighbourForcedExpansion;
						bool mAllNeighbourExpansionScheduled;
						bool mAllNeighbourForcedExpansionScheduled;

						CXLinker<CIndividualProcessNode*>* mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker;
						CXLinker<CIndividualProcessNode*>* mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker;
						CXLinker<CIndividualProcessNode*>* mLastSynchronizedConceptsTestedMergedNodeLinker;
						CXLinker<CIndividualProcessNode*>* mLastCriticalNeighboursTestedMergedNodeLinker;
						CXLinker<CIndividualProcessNode*>* mLastDirectExpansionHandledMergedNodeLinker;
						CXLinker<CIndividualProcessNode*>* mLastInferringExpansionHandledMergedNodeLinker;
						CXLinker<CIndividualProcessNode*>* mLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker;
						CXLinker<CIndividualProcessNode*>* mLastIndirectCompatibilityExpansionHandledMergedNodeLinker;

						CIndividualLinkEdge* mLastNewNeighbourLinkEdge;


						CConceptDescriptor* mLastIndirectConnectedIndiTestedConDes;
						CIndividualLinkEdge* mIndirectConnectedIndiTestedLinkEdge;


						bool mNewlyMergedIndividuals;
						bool mDeterministicSameIndividualMerged;
						bool mDeterministicDifferentIndividualDifferentiated;
						bool mNomainlIndirectConnectionIndividualExpanded;
						bool mPrioritziedPropagationLinksEstablished;


						cint64 mLastMergedIndiCount;
						CXLinker<cint64>* mLastProcessedMergedIndividualLinker;


						CXLinker<CIndividualProcessNode*>* mConceptSetLabelProcessedIndiNodeLinker;

						bool mNonConceptSetRelatedProcessing;
						bool mBackendConceptSetInitialized;
						bool mBackendConceptSetInitializationQueued;
						bool mBackendConceptSetInitializationRequired;
						bool mBackendConceptSetInitializationDelayingRegistered;


						bool mNonConceptSetNeighbourLabelRelatedProcessing;
						CXLinker<CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData*>* mNeighbourLabelRepresentativeExpansionLinker;
						bool mNeighbourLabelRepresentativeExpansion;


						CXLinker<CIndividualProcessNode*>* mDelayedAdditionalLinkInitializationLinker;

						CNonDeterministicDependencyTrackPoint* mBackendExpansionReuseDependencyTrackPoint;
						CXLinker<CIndividualProcessNode*>* mLastReuseExpansionHandledMergedNodeLinker;
						bool mReuseNonDeterministicSameIndividualMerged;
						bool mReuseNonDeterministicDifferentIndividualStated;
						bool mReuseNonDeterministicConceptsAdded;


						CBackendNeighbourExpansionQueue* mBackendNeighbourExpansionQueue;
						CBackendNeighbourExpansionQueue* mPrevBackendNeighbourExpansion;
						CBackendNeighbourExpansionQueue* mUseBackendNeighbourExpansion;


						bool mNewlyMergedAllNeighbourExpansion;
						bool mNewlyMergedInferringNeighbourExpansion;
						bool mNonDeterministicallyMergedIndividuals;


						cint64 mLastMergedIntoIndividualTestingCriticalCardinalityReset;

						bool mScheduledIndividual;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEREPRESENTATIVEMEMORYBACKENDCACHESYNCHRONISATIONDATA_H
