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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBackendRepresentativeMemoryCacheOntologyData_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBackendRepresentativeMemoryCacheOntologyData_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryCacheBaseContext.h"
#include "CBackendRepresentativeMemoryCacheOntologyContext.h"
#include "CBackendRepresentativeMemoryLabelCacheItem.h"
#include "CBackendRepresentativeMemoryCardinalityCacheItem.h"
#include "CBackendRepresentativeMemoryCachingFlags.h"
#include "CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash.h"
#include "CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray.h"
#include "CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem.h"
#include "CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData.h"


// Other includes
#include "Reasoner/Ontology/CIndividualBackendCachingData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheOntologyData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheOntologyData {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheOntologyData(CBackendRepresentativeMemoryCacheBaseContext* baseContext);

						CBackendRepresentativeMemoryCacheOntologyData* initOntologyData(cint64 ontologyIdentifer);

						cint64 getOntologyIdentifer();
						cint64 getUsageCount();
						CBackendRepresentativeMemoryCacheOntologyData* incUsageCount(cint64 count = 1);
						CBackendRepresentativeMemoryCacheOntologyData* decUsageCount(cint64 count = 1);

						CBackendRepresentativeMemoryCacheOntologyData* setSignatureLabelItemHash(cint64 labelType, CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigItemHash);
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* getSignatureLabelItemHash(cint64 labelType);
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* getNominaIIndividualdIndirectConnectionDataHash();

						bool hasSameIndividualsMergings();
						CBackendRepresentativeMemoryCacheOntologyData* setSameIndividualsMergings(bool sameIndisMergings);


						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* getIndividualIdAssociationDataHash();
						CBackendRepresentativeMemoryCacheOntologyData* setIndividualIdAssociationDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* indiIdAssoDataHash);
						CBackendRepresentativeMemoryCacheOntologyData* setNominaIIndividualdIndirectConnectionDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* hash);

						cint64 getIndividualIdAssoiationDataVectorSize();
						CBackendRepresentativeMemoryCacheIndividualAssociationData** getIndividualIdAssoiationDataVector();
						CBackendRepresentativeMemoryCacheOntologyData* setIndividualIdAssoiationDataVector(cint64 indiIdAssoDataVectorSize, CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector);
						CBackendRepresentativeMemoryCacheOntologyData* setIndividualIdAssoiationDataVectorSize(cint64 size);


						cint64 getNextEntryID(bool moveNext = true);
						CBackendRepresentativeMemoryCacheOntologyData* setNextEntryID(cint64 nextEntryID);



						cint64 getLastMinIncompletelyHandledIndvidualiId();
						CBackendRepresentativeMemoryCacheOntologyData* setLastMinIncompletelyHandledIndvidualiId(cint64 id);
						CBackendRepresentativeMemoryCacheOntologyData* incLastMinIncompletelyHandledIndvidualiId(cint64 count = 1);

						cint64 getIncompletelyHandledIndividualIdCount();
						CBackendRepresentativeMemoryCacheOntologyData* setIncompletelyHandledIndividualIdCount(cint64 count);
						CBackendRepresentativeMemoryCacheOntologyData* incIncompletelyHandledIndividualIdCount(cint64 count = 1);
						CBackendRepresentativeMemoryCacheOntologyData* decIncompletelyHandledIndividualIdCount(cint64 count = 1);

						bool getZeroIncompletelyHandledIndividualIdCountDebugWritten();
						CBackendRepresentativeMemoryCacheOntologyData* setZeroIncompletelyHandledIndividualIdCountDebugWritten(bool written);

						cint64 getMaxIndividualAssociationDataUpdateCount();
						CBackendRepresentativeMemoryCacheOntologyData* setMaxIndividualAssociationDataUpdateCount(cint64 count);


						CBackendRepresentativeMemoryCacheOntologyContext* getTemporaryContext();
						CBackendRepresentativeMemoryCacheOntologyContext* getOntologyContext();
						CBackendRepresentativeMemoryCacheOntologyData* setOntologyContext(CBackendRepresentativeMemoryCacheOntologyContext* ontContext);

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mOntologyIdentifer;
						cint64 mUsageCount;


						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* mSigLabelItemHash[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT];
						cint64 mNextEntryID;

						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* mIndiIdAssoDataHash;
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* mNominalIndiIdIndirectConnectionDataHash;



						cint64 mIndiIdAssoDataVectorSize;
						CBackendRepresentativeMemoryCacheIndividualAssociationData** mIndiIdAssoDataVector;

						cint64 mLastMinIncompletelyHandledIndiId;
						cint64 mIncompletelyHandledIndiIdCount;
						bool mZeroIncompletelyHandledIndiIdCountDebugWritten;

						cint64 mMaxIndiAssocDataUpdateCount;

						bool mSameMergedIndisInCache;


						CBackendRepresentativeMemoryCacheOntologyContext mTemporaryContext;
						CBackendRepresentativeMemoryCacheOntologyContext* mOntologyContext;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBackendRepresentativeMemoryCacheOntologyData_H
