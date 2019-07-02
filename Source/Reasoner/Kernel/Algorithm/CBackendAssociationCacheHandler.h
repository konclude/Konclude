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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CBACKENDASSOCIATIONCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CBACKENDASSOCIATIONCACHEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Consistiser/CSaturationConceptReferenceLinking.h"

#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNode.h"
#include "Reasoner/Kernel/Process/CProcessSetHasher.h"

#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheReader.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheWriter.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryLabelCacheItem.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheLabelAssociationWriteData.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheUtilities.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker.h"

#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Consistiser;

		namespace Kernel {

			using namespace Process;
			using namespace Cache;

			namespace Algorithm {


				typedef QPair<CRole*, bool> TRoleInversionPair;


				class CCacheValueTmpLabelReferenceData {
				public:
					CPROCESSSET<CCacheValue>* mCacheValues;
					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* mTmpLabelRef;
					CCacheValueTmpLabelReferenceData* mNext;
				};




				/*! 
				 *
				 *		\class		CBackendAssociationCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendAssociationCacheHandler {
					// public methods
					public:
						//! Constructor
						CBackendAssociationCacheHandler(CBackendRepresentativeMemoryCacheReader* backAssCacheReader, CBackendRepresentativeMemoryCacheWriter* backAssCacheWriter);


						CBackendAssociationCacheHandler* setWorkingOntology(CConcreteOntology* ontology);

						bool commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext);


						bool visitRolesOfAssociatedCompinationRoleSetLabel(CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef, function<bool(CRole* role, bool inversed)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);



						CBackendRepresentativeMemoryCacheIndividualAssociationData* getIndividualAssociationData(CIndividual* individual, CCalculationAlgorithmContext* calcAlgContext);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* getIndividualAssociationData(cint64 indiId, CCalculationAlgorithmContext* calcAlgContext);


						CIndividualRepresentativeBackendCacheLoadedAssociationData* getIndividualAssociationLoadingData(cint64 indiId, CCalculationAlgorithmContext* calcAlgContext);
						CIndividualRepresentativeBackendCacheLoadedAssociationData* getIndividualAssociationLoadingData(CIndividual* individual, CCalculationAlgorithmContext* calcAlgContext);

						CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* getLoadedNominalIndividualIndirectConnectionData(cint64 indiId, CIndividualRepresentativeBackendCacheLoadedAssociationData* indiLoadedAssoData, CCalculationAlgorithmContext* calcAlgContext);

						const CCacheValue getCacheValue(CConcept* concept, bool negation, bool deterministic = true);
						const CCacheValue getCacheValue(CRole* role);


						bool visitNominalIndirectlyConnectedIndividualIds(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 indiId, function<bool(cint64 indiId)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);
						bool visitIndividualIdsOfAssociatedIndividualSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* indiSetLabel, function<bool(cint64 indiId)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);
						bool hasIndividualIdsInAssociatedIndividualSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* indiSetLabel, cint64 indiId);



						bool visitConceptsOfAssociatedConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* conSetLabel, function<bool(CConcept*, bool)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);
						bool visitConceptsOfAssociatedDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, function<bool(CConcept*, bool)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);
						bool hasConceptInAssociatedDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext);


						bool visitConceptsOfAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, function<bool(CConcept* concept, bool negation, bool deterministic)> visitFunc, bool visitDeterministicConcepts, bool visitNonDeterministicConcepts, CCalculationAlgorithmContext* calcAlgContext);
						bool hasConceptInAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext);
						bool hasConceptInAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool negation, bool deterministic, CCalculationAlgorithmContext* calcAlgContext);



						bool visitConceptsOfAssociatedNonDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, function<bool(CConcept*, bool)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);
						bool hasConceptInAssociatedNonDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext);


						bool visitRolesOfAssociatedCombinedNeighbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, function<bool(CRole* role, bool inversed)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);

						bool visitLabelsOfAssociatedNeigbourRoleSetCombinationLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCompinationLabel, function<bool(CBackendRepresentativeMemoryLabelCacheItem* label)> visitFunc);


						bool visitRolesOfAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, function<bool(CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)> visitFunc);
						bool hasRoleInAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic);
						bool hasRoleInAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed, bool nondeterministic);

						bool hasRoleInAssociatedCombinedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed);

						bool hasRoleToNeigbourInAssociatedNeighbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 neighbourIndiId, CRole* role, bool inversed);
						bool visitRolesToNeigbourInAssociatedNeighbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 neighbourIndiId, function<bool(CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);

						bool visitRolesOfAssociatedCompinationRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* combinationRoleSetLabel, function<bool(CRole* role, bool inversed)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);
						bool hasRoleInAssociatedCompinationRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed);



						bool visitNeighbourIndividualIdsForRole(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CRole* role, function<bool(cint64 neighbourIndividualId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, bool nondeterministic)> visitFunc, bool visitOnlyDeterministicNeighbours, CCalculationAlgorithmContext* calcAlgContext);
						cint64 getNeighbourCountForRole(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CRole* role, CCalculationAlgorithmContext* calcAlgContext);


					// protected methods
					protected:
						bool prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext);
						bool addCacheMessages(CBackendRepresentativeMemoryCacheLabelAssociationWriteData* backAssWriteData, CCalculationAlgorithmContext* calcAlgContext);


						class CRoleCardinalityCountData {
						public:
							bool mHasRestrictingCardinality = false;
							cint64 mExistentialUsedCardinality = 0;
							cint64 mRestrictingCardinality = 0;
						};
						bool determineCardinalityAssociationBackendItem(function<void(CPROCESSHASH<cint64, CRoleCardinalityCountData>* superRoleTagUsedCardCountHash)> initFunc, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, bool superRolePropagation, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);


						CBackendRepresentativeMemoryCacheTemporaryLabelReference getRoleInstantiatedSetLabelAssociationBackendItem(cint64 labelType, CSortedNegLinker<CRole*>* roleLinker, CPROCESSSET<TRoleInversionPair>* roleInversionSet, bool extendBySuperRoles, CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >* roleInstantiatedSetLabelHash, CCalculationAlgorithmContext* calcAlgContext);
						CBackendRepresentativeMemoryCacheTemporaryLabelReference getNeighbourRoleInstantiatedSetLabelAssociationBackendItem(CRole* role, bool inversed, CCalculationAlgorithmContext* calcAlgContext);
						CBackendRepresentativeMemoryCacheTemporaryLabelReference getNeighbourRoleInstantiatedSetLabelAssociationBackendItem(CLinkedNeighbourRoleAssertionLinker* linkedRoleAssertedLinker, CBackendRepresentativeMemoryCacheRoleAssertionLinker* linkedRoleAssertionConnectionLinker, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnectionData, CCalculationAlgorithmContext* calcAlgContext);


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* getNeighbourRoleInstantiatedSetLabelFromAssertionLinker(cint64 signature, cint64 superRoleCount, CBackendRepresentativeMemoryCacheRoleAssertionLinker* superRoleAssertionLinker, CCalculationAlgorithmContext* calcAlgContext);

						bool checkLabelContainsCacheValue(CBackendRepresentativeMemoryLabelCacheItem* cacheItemLabel, const CCacheValue& cacheValue, CCalculationAlgorithmContext* calcAlgContext);
						bool checkLabelContainsAllLinkedCacheValues(CBackendRepresentativeMemoryLabelCacheItem* cacheItemLabel, CBackendRepresentativeMemoryLabelValueLinker* cacheValueLinker, CCalculationAlgorithmContext* calcAlgContext);
						bool checkNeighbourRoleInstantiatedSetCompinationLabelContainsNeighbourRoleInstantiatedSet(CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleInstantiatedSetCompinationLabel, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleInstantiatedSetLabel, CCalculationAlgorithmContext* calcAlgContext);
						bool checkNeighbourRoleInstantiatedSetCompinationLabelContainsNeighbourRoleInstantiatedSet(CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleInstantiatedSetCompinationLabel, CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neighbourRoleInstantiatedSetLabelDataLinker, CCalculationAlgorithmContext* calcAlgContext);
						bool checkNeighbourRoleInstantiatedSetCompinationLabelContainsNeighbourRoleInstantiatedSet(CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleInstantiatedSetCompinationLabel, CBackendRepresentativeMemoryCacheTemporaryLabelReference& tempLabelRef, CCalculationAlgorithmContext* calcAlgContext);

						bool determineNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(CPROCESSSET<CBackendRepresentativeMemoryCacheTemporaryLabelReference>* neigbourRoleInstantiatedSetCompination, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);
						bool determineIndirectlyConnectedNominalIndividualSetLabelAssociationBackendItem(CPROCESSSET<cint64>* existentialIndirectlyConnectedNominalIndividualSet, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);

					// protected variables
					protected:
						CBackendRepresentativeMemoryCacheReader* mAssBackCacheReader;
						CBackendRepresentativeMemoryCacheWriter* mAssBackCacheWriter;

						CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mNewTmpLabelList;
						CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >* mNewTmpCardList;


						CPROCESSHASH< TRoleInversionPair, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >* mRoleInversionTmpRefNeighbourInstantiatedRoleSetLabelHash;
						CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >* mRoleInversionSetTmpRefNeighbourInstantiatedRoleSetLabelHash;


						CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >* mRoleInversionSetTmpRefCombinedNeighbourRoleInstantiatedSetLabelHash;
						CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >* mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash;

						CPROCESSHASH< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*, CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >* mTmpConceptLabelTmpCardDataHash;


						CPROCESSHASH< cint64, CCacheValueTmpLabelReferenceData* >* mSuperRoleSignatureCacheValueTmpRefNeighbourInstantiatedRoleSetLabelPairHash;



						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mSignatureTmpRefNeigbourRoleInstantiatedSetCombinationLabelHash;
						
						
						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mSignatureTmpIndiConnIndiSetRefLabelHash;

						CBackendRepresentativeMemoryCacheLabelAssociationWriteData* mWriteData;
						CTaskMemoryPoolAllocationManager* mMemAllocMan;
						CMemoryPoolContainer mMemPoolCon;
						CContextBase* mTmpContext;

						cint64 mStatCreatedTempLabels;
						cint64 mStatReusedTempLabels;
						cint64 mStatReusedCachedLabels;
						cint64 mStatReusedHashedIndiLabels;
						cint64 mStatReusedHashedSignatureLabels;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CBACKENDASSOCIATIONCACHEHANDLER_H
