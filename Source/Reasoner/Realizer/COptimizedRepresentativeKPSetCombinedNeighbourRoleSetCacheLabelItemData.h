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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCOMBINEDNEIGHBOURROLESETCACHELABELITEMDATA_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCOMBINEDNEIGHBOURROLESETCACHELABELITEMDATA_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetRoleInstancesItem.h"
#include "COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData(CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleLabelCacheItem);

					bool hasPossibleInstances();
					COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* setPossibleInstances(bool possibleInstances);


					QHash<COptimizedKPSetRoleInstancesItem*, COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData*>* getInstancesItemDataHash();
					COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData* getInstancesItemData(COptimizedKPSetRoleInstancesItem* instanceItem, bool createIfNotExists = true);

					QSet<COptimizedKPSetRoleInstancesItem*>* getKnownInstancesItemSet(bool inverse);
					QSet<COptimizedKPSetRoleInstancesItem*>* getPossibleInstancesItemSet(bool inverse);


					COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* addKnownInstancesItem(COptimizedKPSetRoleInstancesItem* roleItem, bool inverse);
					COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* addPossibleInstancesItem(COptimizedKPSetRoleInstancesItem* roleItem, bool inverse);



					CBackendRepresentativeMemoryLabelCacheItem* getCombinedNeighbourRoleLabelCacheItem();


					QSet<COptimizedKPSetRoleInstancesItem*>* getComplexCandiateInstancesItemSet(bool inverse);
					COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* addComplexCandiateInstancesItem(COptimizedKPSetRoleInstancesItem* roleItem, bool inverse);

					bool hasComplexCandiateInstancesItems();


				// protected methods
				protected:

				// protected variables
				protected:
					CBackendRepresentativeMemoryLabelCacheItem* mCombinedNeighbourRoleLabelCacheItem;

					bool mPossibleInstances;

					QHash<COptimizedKPSetRoleInstancesItem*, COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData*> mInstancesItemDataHash;
					QSet<COptimizedKPSetRoleInstancesItem*> mKnownInstancesItemSet;
					QSet<COptimizedKPSetRoleInstancesItem*> mPossibleInstancesItemSet;

					QSet<COptimizedKPSetRoleInstancesItem*> mInverseKnownInstancesItemSet;
					QSet<COptimizedKPSetRoleInstancesItem*> mInversePossibleInstancesItemSet;


					QSet<COptimizedKPSetRoleInstancesItem*> mComplexCandiateInstancesItemSet;
					QSet<COptimizedKPSetRoleInstancesItem*> mInverseComplexCandiateInstancesItemSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCOMBINEDNEIGHBOURROLESETCACHELABELITEMDATA_H
