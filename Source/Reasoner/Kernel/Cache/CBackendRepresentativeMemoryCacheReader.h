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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEREADER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEREADER_H

// Libraries includes
#include <QAtomicPointer>

// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryCacheSlotItem.h"
#include "CBackendRepresentativeMemoryLabelSignatureResolveCacheItem.h"
#include "CBackendRepresentativeMemoryCacheIndividualAssociationData.h"
#include "CBackendRepresentativeMemoryCacheRoleAssertionLinker.h"
#include "CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker.h"
#include "CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData.h"
#include "CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData.h"
#include "CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Kernel/Algorithm/CCalculationErrorProcessingException.h"

#include "Reasoner/Kernel/Process/CConceptSaturationDescriptor.h"
#include "Reasoner/Kernel/Process/CConceptDescriptor.h"
#include "Reasoner/Kernel/Process/CIndividualMergingHash.h"
#include "Reasoner/Kernel/Process/CDistinctHash.h"

#include "Reasoner/Ontology/CIndividualProcessData.h"
#include "Reasoner/Ontology/CIndividual.h"
#include "Reasoner/Ontology/CConcreteOntology.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			using namespace Process;
			using namespace Algorithm;

			namespace Cache {

				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheReader : public CLinkerBase<CBackendRepresentativeMemoryCacheReader*,CBackendRepresentativeMemoryCacheReader> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheReader();


						CBackendRepresentativeMemoryCacheReader* updateSlot(CBackendRepresentativeMemoryCacheSlotItem* updatedSlot);
						CBackendRepresentativeMemoryCacheReader* fixOntologyData(CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						CBackendRepresentativeMemoryCacheReader* checkRecomputationIdUsage(cint64 recomputationId);


						CBackendRepresentativeMemoryCacheReader* setWorkingOntology(cint64 ontologyIdentifier);
						CBackendRepresentativeMemoryCacheReader* setWorkingOntology(CConcreteOntology* ontology);


						bool hasSameIndividualsMergings();


						bool hasCacheEntry(cint64 labelType, cint64 signature);
						CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* getLabelCacheEntry(cint64 labelType, cint64 signature);

						bool visitLabelCacheEntries(cint64 labelType, function<bool(CBackendRepresentativeMemoryLabelCacheItem* labelItem)> visitFunc);


						CBackendRepresentativeMemoryLabelCacheItem* getLabelCacheEntryViaProvidedCacheValues(cint64 labelType, cint64 signature, cint64 count, function<bool(bool, cint64&, CCacheValue&)> provFunc);
						CBackendRepresentativeMemoryLabelCacheItem* getLabelCacheEntryViaRoleLinker(cint64 labelType, cint64 signature, cint64 count, CSortedNegLinker<CRole*>* roleLinker, bool inversed, CRole* assertedRole = nullptr);
						CBackendRepresentativeMemoryLabelCacheItem* getLabelCacheEntryViaRoleAssertionLinker(cint64 labelType, cint64 signature, cint64 count, CBackendRepresentativeMemoryCacheRoleAssertionLinker* roleAssertionLinker);
					

						CBackendRepresentativeMemoryLabelCacheItem* getNeighbourRoleInstantiatedSetCompinationLabelCacheEntry(cint64 signature, cint64 count, CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neigbourRoleInstantiatedSetTmpLabelLinker);

						CBackendRepresentativeMemoryLabelCacheItem* getConceptSetLabelCacheEntry(cint64 labelType, cint64 signature, cint64 conCount, CConceptSaturationDescriptor* conDesLinker);
						CBackendRepresentativeMemoryLabelCacheItem* getDeterministicConceptSetLabelCacheEntry(cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, cint64 maxDeterministicBranchTag, bool excludePositiveNominalConcepts);
						CBackendRepresentativeMemoryLabelCacheItem* getNondeterministicConceptSetLabelCacheEntry(cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, cint64 maxDeterministicBranchTag, bool excludePositiveNominalConcepts);
						CBackendRepresentativeMemoryLabelCacheItem* getConceptSetLabelCacheEntry(cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, bool deterministic, cint64 maxDeterministicBranchTag, bool excludePositiveNominalConcepts);
						CBackendRepresentativeMemoryLabelCacheItem* getConceptSetLabelCacheEntry(cint64 labelType, cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, function<bool(CConcept* concept, bool negation)> exclusionDetermineFunction);
						CBackendRepresentativeMemoryLabelCacheItem* getFullConceptSetLabelCacheEntry(cint64 labelType, cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, function<bool(CConcept* concept, bool negation)> exclusionDetermineFunction, function<bool(CConcept* concept, bool negation, CDependencyTrackPoint* depTrackPoint)> nondeterministicDetermineFunction);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* getIndividualAssociationData(CIndividual* individual);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* getIndividualAssociationData(cint64 indiId);


						CBackendRepresentativeMemoryLabelCacheItem* getIndividualAssociatedCacheLabelItem(cint64 indiId, cint64 labelType);


						CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* getNominalIndirectConnectionData(cint64 indiId);

						CBackendRepresentativeMemoryLabelCacheItem* getIndividualSetLabelCacheEntry(cint64 labelType, cint64 signature, cint64 indiId, CDistinctHash* indiDistinctHash, cint64& count, bool onlyDeterministic, cint64 maxDeterministicBranchTag);
						CBackendRepresentativeMemoryLabelCacheItem* getIndividualSetLabelCacheEntry(cint64 labelType, cint64 signature, cint64 indiId, CIndividualMergingHash* indiMerginghash, cint64& count, bool onlyDeterministic, cint64 maxDeterministicBranchTag);
						CBackendRepresentativeMemoryLabelCacheItem* getIndividualSetLabelCacheEntry(cint64 labelType, cint64 signature, CPROCESSSET<cint64>* individualSet, cint64& count);



						const CCacheValue getCacheValue(cint64 indiId, bool negation);

						const CCacheValue getCacheValue(CConcept* concept, bool negation, bool deterministic = true);
						const CCacheValue getCacheValue(CRole* role);
						const CCacheValue getCacheValue(CRole* role, bool inversed, bool assertionLinkBase = false, bool nominalLinkBase = false, bool nondeterministic = false);
						const CCacheValue getCacheValue(CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neigbourRoleInstantiatedSetTmpLabelLinker);


						bool isCacheValueRoleInverse(const CCacheValue& cacheValue);
						bool isCacheValueRoleNondeterministic(const CCacheValue& cacheValue);
						bool isCacheValueRoleNominal(const CCacheValue& cacheValue);
						bool isCacheValueRoleAssertion(const CCacheValue& cacheValue);


						bool visitNominalIndirectlyConnectedIndividualIds(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* nomConnData, function<bool(cint64 indiId)> visitFunc);
						bool visitIndividualIdsOfAssociatedIndividualSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* indiSetLabel, function<bool(cint64 indiId)> visitFunc);

						bool hasIndividualIdsInAssociatedIndividualSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* indiSetLabel, cint64 indiId);


						bool visitConceptsOfAssociatedDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, function<bool(CConcept*, bool)> visitFunc);
						bool hasConceptInAssociatedDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CConcept* concept, bool negation);

						bool visitConceptsOfAssociatedNonDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, function<bool(CConcept*, bool)> visitFunc);
						bool hasConceptInAssociatedNonDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CConcept* concept, bool negation);

						bool visitConceptsOfAssociatedConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, function<bool(CConcept*, bool)> visitFunc);
						bool hasConceptInAssociatedConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool negation);
						
						bool visitConceptsOfFullConceptSetLabel(CBackendRepresentativeMemoryLabelCacheItem* labelItem, function<bool(CConcept* concept, bool negation, bool deterministic)> visitFunc, bool visitDeterministicConcepts, bool visitNonDeterministicConcepts);
						bool visitConceptsOfAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, function<bool(CConcept* concept, bool negation, bool deterministic)> visitFunc, bool visitDeterministicConcepts = true, bool visitNonDeterministicConcepts = true);
						bool hasConceptInAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool negation);
						bool hasConceptInAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool negation, bool deterministic);

						bool getConceptOccurrenceInAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool& negationFlag, bool& deterministicFlag);



						bool visitNeighbourIndividualIdsForNeighbourArrayIdFromCursor(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 arrayId, function<bool(cint64 neighbourIndividualId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, bool nondeterministic, cint64 nextCursor)> visitFunc, bool visitOnlyDeterministicNeighbours = true, cint64 cursor = 0);
						bool visitNeighbourIndividualIdsForRole(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CRole* role, function<bool(cint64 neighbourIndividualId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, bool nondeterministic)> visitFunc, bool visitOnlyDeterministicNeighbours = true);
						bool visitNeighbourArrayIdsForRole(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CRole* role, function<bool(cint64 neighbourIndividualId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, bool nondeterministic)> visitFunc, bool visitOnlyDeterministicNeighbours = true);

						cint64 getNeighbourCountForRole(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CRole* role);
						cint64 getNeighbourCountForArrayPos(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 pos);


						bool visitRolesOfAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, function<bool(CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)> visitFunc);
						bool hasRoleInAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic);
						bool hasRoleInAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed);
						bool hasRoleInAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed, bool nondeterministic);
			
						bool hasRoleInAssociatedCombinedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed);
					

						bool visitLabelsOfAssociatedNeigbourRoleSetCombinationLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCompinationLabel, function<bool(CBackendRepresentativeMemoryLabelCacheItem* label)> visitFunc);


						bool visitRolesOfAssociatedCompinationRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* combinationRoleSetLabel, function<bool(CRole* role, bool inversed)> visitFunc);
						bool hasRoleInAssociatedCompinationRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* compinationRoleSetLabel, CRole* role, bool inversed);

						bool visitRolesOfAssociatedCombinedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, function<bool(CRole* role, bool inversed)> visitFunc);

						bool hasRoleToNeigbourInAssociatedNeighbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 neighbourIndiId, CRole* role, bool inversed);
						bool visitRolesToNeigbourInAssociatedNeighbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 neighbourIndiId, function<bool(CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)> visitFunc);


						bool visitLabelItemIndividualIdAssociations(CBackendRepresentativeMemoryLabelCacheItem* labelItem, function<bool(cint64 indiId, bool sameIndividualMerged)> visitFunc, bool ascending = true, bool visitBaseIndividual = true, bool visitSameMergedIndividuals = true);


					// protected methods
					protected:
						bool hasUpdatedSlotItem();
						bool switchToUpdatedSlotItem();

					// protected variables
					protected:
						CBackendRepresentativeMemoryCacheSlotItem* mCurrentSlot;
						QAtomicPointer<CBackendRepresentativeMemoryCacheSlotItem> mUpdatedSlot;

						CBackendRepresentativeMemoryLabelSignatureResolveCacheItem mEmptySigResCacheItem;
						CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem mEmptyCardSigResCacheItem;

						cint64 mOntologyIdentifier;
						cint64 mRecomputationId;
						CBackendRepresentativeMemoryCacheOntologyData* mOntologyData;
						CBackendRepresentativeMemoryCacheOntologyData* mFixedOntologyData;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEREADER_H
