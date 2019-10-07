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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALNODEBACKENDCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALNODEBACKENDCACHEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"
#include "CBackendAssociationCacheHandler.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNodeLinker.h"
#include "Reasoner/Kernel/Process/CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData.h"

#include "Reasoner/Kernel/Process/Dependency/CROLEASSERTIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CVALUEDependencyNode.h"

#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheReader.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheWriter.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryLabelCacheItem.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheLabelAssociationWriteData.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheUtilities.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker.h"


#include "Reasoner/Kernel/Task/CSatisfiableTaskRepresentativeBackendUpdatingAdapter.h"


#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			using namespace Process;
			using namespace Dependency;
			using namespace Cache;
			using namespace Task;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CIndividualNodeBackendCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualNodeBackendCacheHandler : public CBackendAssociationCacheHandler {
					// public methods
					public:
						//! Constructor
						CIndividualNodeBackendCacheHandler(CBackendRepresentativeMemoryCacheReader* backAssCacheReader, CBackendRepresentativeMemoryCacheWriter* backAssCacheWriter);



						bool tryAssociateNodesWithBackendCache(CXLinker<CIndividualProcessNode*>* indiNodeLinker, CSatisfiableTaskRepresentativeBackendUpdatingAdapter* repbackUpdAdapter, CCalculationAlgorithmContext* calcAlgContext);
						

					// protected methods
					protected:

						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher getIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher(CIndividualProcessNode* indiNode, CCalculationAlgorithmContext* calcAlgContext);


						bool determineSameIndividualSetLabelAssociationBackendItem(CIndividualProcessNode* indiNode, cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, bool* integratedAllMergingsFlag, CCalculationAlgorithmContext* calcAlgContext);
						bool determineDifferentIndividualSetLabelAssociationBackendItem(CIndividualProcessNode* indiNode, cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, bool* integratedAllDifferentFlag, CCalculationAlgorithmContext* calcAlgContext);



						bool determineConceptSetLabelAssociationBackendItem(CIndividualProcessNode* indiNode, cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);
						bool determineAssociationBackendCardinality(CIndividualProcessNode* indiNode, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);


						bool determineRoleInstantiatedSetLabelAssociationBackendItems(CIndividualProcessNode* extractionIndiNode, CIndividualProcessNode* indiNode, bool onlyNondeterministic, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CPROCESSSET<cint64>** existentialIndirectlyConnectedNominalIndividualSet, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, CCalculationAlgorithmContext* calcAlgContext);


						void collectRoleSuccessorData(CIndividualLinkEdge* link, bool inversed, cint64 maxDetBranchTag, bool isNominalNeighbour, bool isDataNeighbour, CPROCESSSET<TRoleInversionPair>* combinedNeighbourDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedNeighbourDetRoleInstantiatedLinker,
							CPROCESSSET<TRoleInversionPair>* combinedNeighbourNonDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedNeighbourNonDetRoleInstantiatedLinker, CPROCESSSET<TRoleInversionPair>* combinedExistentialDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedExistentialDetRoleInstantiatedLinker, 
							CPROCESSSET<TRoleInversionPair>* combinedExistentialNonDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedExistentialNonDetRoleInstantiatedLinker, CPROCESSSET<TRoleInversionPair>* combinedDataDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedDataDetRoleInstantiatedLinker,
							CPROCESSSET<TRoleInversionPair>* combinedDataNonDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedDataNonDetRoleInstantiatedLinker, CRole* role, CCalculationAlgorithmContext* calcAlgContext, bool &collected, CIndividualProcessNode* indiNode, CIndividualProcessNode* connIndiNode,
							cint64 connIndiMergedNominalId, bool connIndiNominalDeterministicallyMerged, CBackendRepresentativeMemoryCacheRoleAssertionLinker* &roleAssertionLinke, CPROCESSSET<cint64>** existentialIndirectlyConnectedNominalIndividualSet, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash);


						CIndividualProcessNode* getSuccessorIndividual(CIndividualProcessNode*& indi, CIndividualLinkEdge* link, CCalculationAlgorithmContext* calcAlgContext);
						CIndividualProcessNode* getMergedIntoIndividualNode(CIndividualProcessNode* indi, bool onlyDeterministic, CCalculationAlgorithmContext* calcAlgContext);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* getIndividualAssociationDataFromIndividualNode(CIndividualProcessNode* indiNode, CCalculationAlgorithmContext* calcAlgContext);


						bool determineNondeterministicConceptSetLabelAssociationBackendItemWithExcludedConceptsFromDeterministicLabel(CIndividualProcessNode* extractionIndiNode, CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);


						bool determineFullConceptSetLabelAssociationBackendItemWithSeparatedDeterministicNode(CIndividualProcessNode* extractionIndiNode, CIndividualProcessNode* deterministicConSetNode, CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);


						bool visitMergedIndividualNodesAssociationData(CIndividualProcessNode* baseIndiNode, CBackendRepresentativeMemoryCacheIndividualAssociationData* excludeIndiAssData, CBackendRepresentativeMemoryLabelCacheItem* excludeIndiSetLabel, bool visitBaseIndiAssoData, cint64 maxDetBranchTag, function<bool(cint64 mergedIndiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* mergedIndiAssData, bool deterministicallyMerged)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);


					// protected variables
					protected:
						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mSignatureTmpDetConSetRefLabelHash;
						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mSignatureTmpNonDetConSetRefLabelHash;

						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mSignatureTmpDetSameIndiSetRefLabelHash;
						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mSignatureTmpNonDetSameIndiSetRefLabelHash;


						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mSignatureTmpDetDiffIndiSetRefLabelHash;
						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mSignatureTmpNonDetDiffIndiSetRefLabelHash;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALNODEBACKENDCACHEHANDLER_H
