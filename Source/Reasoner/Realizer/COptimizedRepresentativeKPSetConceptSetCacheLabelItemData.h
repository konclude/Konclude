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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCONCEPTSETCACHELABELITEMDATA_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCONCEPTSETCACHELABELITEMDATA_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetConceptInstancesItem.h"
#include "COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedRepresentativeKPSetConceptSetCacheLabelItemData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetConceptSetCacheLabelItemData {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetConceptSetCacheLabelItemData();

					bool hasPossibleInstances();
					COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* setPossibleInstances(bool possibleInstances);


					QHash<COptimizedKPSetConceptInstancesItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData*>* getInstancesItemDataHash();
					COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData* getInstancesItemData(COptimizedKPSetConceptInstancesItem* instanceItem, bool createIfNotExists = true);

					QList<COptimizedKPSetConceptInstancesItem*>* getKnownInstancesItemList();
					QList<COptimizedKPSetConceptInstancesItem*>* getPossibleInstancesItemList();

				// protected methods
				protected:

				// protected variables
				protected:
					bool mPossibleInstances;

					QHash<COptimizedKPSetConceptInstancesItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData*> mInstancesItemDataHash;
					QList<COptimizedKPSetConceptInstancesItem*> mKnownInstancesItemList;
					QList<COptimizedKPSetConceptInstancesItem*> mPossibleInstancesItemList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCONCEPTSETCACHELABELITEMDATA_H
