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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELROLENEIGHBOURINGITEMITERATOR_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELROLENEIGHBOURINGITEMITERATOR_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetRoleInstancesItem.h"
#include "COptimizedRepresentativeKPSetCacheLabelItemIterator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Cache;
		using namespace Realization;

		namespace Realizer {

			typedef QPair<COptimizedKPSetRoleInstancesItem*, bool> TRoleInstancesItemInversedFlagPair;

			/*! 
			 *
			 *		\class		COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator : public COptimizedRepresentativeKPSetCacheLabelItemIterator {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList, COptimizedKPSetIndividualInstantiatedItemMultiHash* individualInstantiatedItemHash, CIndividualVector* individualVector, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor = true);
					COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList, COptimizedKPSetIndividualInstantiatedItemMultiHash* individualInstantiatedItemHash, CIndividualVector* individualVector, const QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TRoleInstancesItemInversedFlagPair>>& pssibleLabelRoleInstancesItemInversionPairSetHash, const QSet<TRoleInstancesItemInversedFlagPair>& roleInstancesItemInversionPairSet, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor = true);


					bool isValidIteratorPosition(CAssociationMapIteratorData* iteratorData, cint64 indiId);

					QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TRoleInstancesItemInversedFlagPair>>* getPossibleLabelRoleInstancesItemInversionPairSetHash();
					QSet<TRoleInstancesItemInversedFlagPair>* getRoleInstancesItemInversionPairSet();
					QSet<TRoleInstancesItemInversedFlagPair>* currentRoleInstancesItemInversionPairSet();

				// protected methods
				protected:
					class CAssociationMapIteratorDataWithRoleInstancesItems : public CAssociationMapIteratorData {
					public:
						CAssociationMapIteratorDataWithRoleInstancesItems(CBackendRepresentativeMemoryLabelCacheItem* labelItem, const CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator& iterator, bool possibleInstances) : CAssociationMapIteratorData(labelItem, iterator, possibleInstances) {
						}

						QSet<TRoleInstancesItemInversedFlagPair> mRoleInstancesItemInversionPairSet;
					};

					void checkValidIteratorPosition(CAssociationMapIteratorData* iteratorData, cint64 indiId, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, bool& isValidInstanceFlag, bool& isValidKnownInstanceFlag, bool& isValidPossibleInstanceFlag);

					CAssociationMapIteratorData* createAssociationMapIteratorData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, const CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator& iterator, bool possibleInstances);

					CRealizationIndividualInstanceItemReferenceIterator* getCopy();
					CRealizationIndividualInstanceItemReferenceIterator* initCopy(CRealizationIndividualInstanceItemReferenceIterator* iterator);

				// protected variables
				protected:
					QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TRoleInstancesItemInversedFlagPair>> mPossibleLabelRoleInstancesItemInversionPairSetHash;
					QSet<TRoleInstancesItemInversedFlagPair> mRoleInstancesItemInversionPairSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELROLENEIGHBOURINGITEMITERATOR_H
