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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATURATIONNODEBACKENDASSOCIATIONCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATURATIONNODEBACKENDASSOCIATIONCACHEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"
#include "CBackendAssociationCacheHandler.h"

// Other includes
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Consistiser/CSaturationConceptReferenceLinking.h"

#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNode.h"
#include "Reasoner/Kernel/Process/CProcessSetHasher.h"

#include "Reasoner/Kernel/Cache/COccurrenceStatisticsCacheWriter.h"
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
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData.h"

#include "Reasoner/Kernel/Task/CSatisfiableTaskRepresentativeBackendUpdatingAdapter.h"

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
			using namespace Task;

			namespace Algorithm {



				/*! 
				 *
				 *		\class		CSaturationNodeBackendAssociationCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationNodeBackendAssociationCacheHandler : public CBackendAssociationCacheHandler {
					// public methods
					public:
						//! Constructor
						CSaturationNodeBackendAssociationCacheHandler(CBackendRepresentativeMemoryCacheReader* backAssCacheReader, CBackendRepresentativeMemoryCacheWriter* backAssCacheWriter, COccurrenceStatisticsCacheWriter* occStatsCacheWriter);

						void readCalculationConfig(CCalculationConfigurationExtension* config);


						bool tryAssociateNodesWithBackendCache(CIndividualSaturationProcessNodeLinker* indiSaturationAnalysingNodeLinker, CSatisfiableTaskRepresentativeBackendUpdatingAdapter* repbackUpdAdapter, CCalculationAlgorithmContext* calcAlgContext);
						


					// protected classes
					protected:
						class CSuccessorRelatedNodesAlternativeCheckingData {
						public:
							CSuccessorRelatedNodesAlternativeCheckingData(CRole* role = nullptr, bool inversed = false) {
								mRole = role;
								mInversed = inversed;
								mCheckingNodeLinker = nullptr;
								mCheckingNodeCount = 0;
							}

							CRole* mRole;
							bool mInversed;
							CXLinker<CIndividualSaturationProcessNode*>* mCheckingNodeLinker;
							cint64 mCheckingNodeCount;
						};

					// protected methods
					protected:
						bool determineCardinalityAssociationBackendItem(CIndividualSaturationProcessNode* satNode, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);


						bool determineFullConceptSetLabelAssociationBackendItem(CIndividualSaturationProcessNode* satNode, cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);


						bool determineSaturationNodeIndividualCombinedNeighbourRoleInstantiatedSetLabelAssociationBackendItem(CIndividualSaturationProcessNode* satNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);
						bool determineCombinedExistentialRoleInstantiatedSetLabelAssociationBackendItem(CIndividualSaturationProcessNode* satNode, CPROCESSSET<cint64>** existentialIndirectlyConnectedNominalIndividualSet, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);
						bool determineCombinedDataRoleInstantiatedSetLabelAssociationBackendItem(CIndividualSaturationProcessNode* satNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);


						bool determineSaturationNodeIndividualNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(CIndividualSaturationProcessNode* satNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);

						CIndividualSaturationProcessNode* getSuccessorSaturationNode(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode* satNode, CCalculationAlgorithmContext* calcAlgContext);
						bool collectSaturationNodeSuccessorRelatedRoles(cint64 satIndiId, CIndividualSaturationProcessNode* satNode, CPROCESSSET<TRoleInversionPair>** roleSet, CSortedNegLinker<CRole*>** roleLinker,
							bool collectExistentialConnectionRoles, bool collectNominalConnectionRoles, bool collectDataConnectionRoles, bool analyseConcepts, bool analyseSuccessorLinks, 
							CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >* nominalIndiIdRoleConnectionDataHash, CPROCESSSET<cint64>** existentialIndirectlyConnectedNominalIndividualSet, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, 
							CPROCESSHASH<TRoleInversionPair, CSuccessorRelatedNodesAlternativeCheckingData>* successorRelatedNodesAlternativeCheckingHash, CCalculationAlgorithmContext* calcAlgContext);
						bool collectSaturationNodeSuccessorRelatedRoles(cint64 satIndiId, CIndividualSaturationProcessNode* satNode, CPROCESSSET<TRoleInversionPair>** roleSet, CSortedNegLinker<CRole*>** roleLinker,
							CRole* role, CIndividualSaturationProcessNode* succSatNode, CSaturationSuccessorData* succData,
							CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >* nominalIndiIdRoleConnectionDataHash, CPROCESSSET<cint64>** existentialIndirectlyConnectedNominalIndividualSet, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, CCalculationAlgorithmContext* calcAlgContext);


						bool determineNominalIndividualIdNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(cint64 nominalId, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnection, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);


						bool determineNominalIndividualIdCombinedNeighbourRoleInstantiatedSetLabelAssociationBackendItem(cint64 nominalId, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnection, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);


						bool determineNominalIndividualIdPossiblyExistentialRoleInstantiatedSetLabelAssociationBackendItem(cint64 nominalId, CSaturationNominalDependentNodeData* nomDepNodeData, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);


						bool collectNominalRelatedRoles(cint64 satIndiId, cint64 nominalId, CIndividualSaturationProcessNode* satNode, CPROCESSSET<TRoleInversionPair>** roleSet, CSortedNegLinker<CRole*>** roleLinker, CCalculationAlgorithmContext* calcAlgContext);
						bool collectNominalRelatedRoles(cint64 satIndiId, cint64 nominalId, CIndividualSaturationProcessNode* satNode, CPROCESSSET<TRoleInversionPair>** roleSet, CSortedNegLinker<CRole*>** roleLinker, CRole* role, CCalculationAlgorithmContext* calcAlgContext);
						bool collectNominalRelatedRolesFromPredecessors(cint64 satIndiId, cint64 nominalId, CIndividualSaturationProcessNode* satNode, CPROCESSSET<TRoleInversionPair>** roleSet, CSortedNegLinker<CRole*>** roleLinker, CCalculationAlgorithmContext* calcAlgContext);



						bool collectConceptOccurrenceStatistics(CIndividualSaturationProcessNode* indiNode, CCalculationAlgorithmContext* calcAlgContext);


					// protected variables
					protected:

						CPROCESSHASH< CIndividualSaturationProcessNode*,QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*> >* mIndiNodeRefDetConceptSetLabelHash;
						CPROCESSHASH< cint64,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mSignatureTmpRefDetConceptSetLabelHash;
						

						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >* mNominalIndiIdRoleConnectionDataHash;


						bool mConfDirectCardinalityInsufficient;
						bool mConfOccurrenceStatisticsCounting;

						COccurrenceStatisticsCacheWriter* mOccStatsCacheWriter;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATURATIONNODEBACKENDASSOCIATIONCACHEHANDLER_H
