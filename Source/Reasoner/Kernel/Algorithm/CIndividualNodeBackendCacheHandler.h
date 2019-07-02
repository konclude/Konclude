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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALNODEBACKENDCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALNODEBACKENDCACHEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNodeLinker.h"

#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheReader.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheWriter.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryLabelCacheItem.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheLabelAssociationWriteData.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker.h"

#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			using namespace Process;
			using namespace Cache;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CIndividualNodeBackendCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualNodeBackendCacheHandler {
					// public methods
					public:
						//! Constructor
						CIndividualNodeBackendCacheHandler(CBackendRepresentativeMemoryCacheReader* backAssCacheReader, CBackendRepresentativeMemoryCacheWriter* backAssCacheWriter);


						CBackendRepresentativeMemoryCacheIndividualAssociationData* getIndividualAssociationData(CIndividual* individual);
						bool hasConceptIndividualAssociation(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext);


						bool tryAssociateNodesWithBackendCache(CIndividualProcessNodeLinker* indiNodeLinker, CCalculationAlgorithmContext* calcAlgContext);
						
						bool commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext);




					// protected methods
					protected:
						bool prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext);
						bool addCacheMessages(CBackendRepresentativeMemoryCacheLabelAssociationWriteData* backAssWriteData, CCalculationAlgorithmContext* calcAlgContext);
						const CCacheValue getCacheValue(CConcept* concept, bool negation);

						bool findAssociationBackendLabel(CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);
						bool findAssociationBackendCardinality(CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext);

					// protected variables
					protected:
						CBackendRepresentativeMemoryCacheReader* mAssBackCacheReader;
						CBackendRepresentativeMemoryCacheWriter* mAssBackCacheWriter;

						CPROCESSHASH< CIndividualProcessNode*,QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*> >* mIndiNodeRefLabelHash;
						CPROCESSHASH< cint64,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mSignatureTmpRefLabelHash;

						CPROCESSHASH< CIndividualProcessNode*,QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker*> >* mIndiNodeRefCardHash;
						CPROCESSHASH< cint64,CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >* mSignatureTmpRefCardHash;


						CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* mNewTmpLabelList;
						CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >* mNewTmpCardList;


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

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALNODEBACKENDCACHEHANDLER_H
