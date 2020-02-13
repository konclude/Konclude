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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELCONCEPTSETITEMITERATOR_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELCONCEPTSETITEMITERATOR_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetConceptInstancesItem.h"
#include "COptimizedRepresentativeKPSetCacheLabelItemIterator.h"


// Other includes
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheReader.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Cache;
		using namespace Realization;

		namespace Realizer {


			typedef QPair<COptimizedKPSetConceptInstancesItem*, bool> TConceptInstancesItemMostSpecificFlagPair;


			/*! 
			 *
			 *		\class		COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator : public COptimizedRepresentativeKPSetCacheLabelItemIterator {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList, QHash<cint64, COptimizedKPSetIndividualItem*>* individualInstantiatedItemHash, CIndividualVector* individualVector, COptimizedKPSetConceptInstancesItem* conceptInstancesItem, bool onlyMostSpecificInstances, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor = true, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader = nullptr);
					COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList, QHash<cint64, COptimizedKPSetIndividualItem*>* individualInstantiatedItemHash, CIndividualVector* individualVector, const QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TConceptInstancesItemMostSpecificFlagPair>>& possibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash, const QSet<TConceptInstancesItemMostSpecificFlagPair>& onceptInstancesItemOnlyMostSpecificFlagPairSet, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor = true, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader = nullptr);



					bool isValidIteratorPosition(CAssociationMapIteratorData* iteratorData, cint64 indiId);


					QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TConceptInstancesItemMostSpecificFlagPair>>* getPossibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash();
					QSet<TConceptInstancesItemMostSpecificFlagPair>* getConceptInstancesItemOnlyMostSpecificFlagPairSet();
					QSet<TConceptInstancesItemMostSpecificFlagPair>* currentConceptInstancesItemOnlyMostSpecificFlagPairSet();


				// protected methods
				protected:
					class CAssociationMapIteratorDataWithConceptInstancesItems : public CAssociationMapIteratorData {
					public:
						CAssociationMapIteratorDataWithConceptInstancesItems(CBackendRepresentativeMemoryLabelCacheItem* labelItem, const CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator& iterator, bool possibleInstances) : CAssociationMapIteratorData(labelItem, iterator, possibleInstances) {
						}

						QSet<TConceptInstancesItemMostSpecificFlagPair> mConceptInstancesItemOnlyMostSpecificFlagPairSet;
					};



					CRealizationIndividualInstanceItemReferenceIterator* getCopy();
					CRealizationIndividualInstanceItemReferenceIterator* initCopy(CRealizationIndividualInstanceItemReferenceIterator* iterator);

					CAssociationMapIteratorData* createAssociationMapIteratorData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, const CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator& iterator, bool possibleInstances);

				// protected variables
				protected:
					QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TConceptInstancesItemMostSpecificFlagPair>> mPossibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash;
					QSet<TConceptInstancesItemMostSpecificFlagPair> mConceptInstancesItemOnlyMostSpecificFlagPairSet;
					CBackendRepresentativeMemoryCacheReader* mBackendAssocCacheReader;
				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELCONCEPTSETITEMITERATOR_H
