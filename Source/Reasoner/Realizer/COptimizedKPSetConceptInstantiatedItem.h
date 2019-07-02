/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANTIATEDITEM_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANTIATEDITEM_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetConceptInstancesHash.h"
#include "COptimizedKPSetConceptInstancesData.h"


// Other includes
#include "Reasoner/Ontology/CIndividual.h"

#include "Reasoner/Realization/CConceptInstanceItem.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Realization;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetConceptInstantiatedItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetConceptInstantiatedItem : public CConceptInstanceItem {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetConceptInstantiatedItem();

					COptimizedKPSetConceptInstantiatedItem* initInstantiatedItem(CIndividual* individual);

					CIndividual* getIndividual();
					COptimizedKPSetConceptInstancesHash* getKnownPossibleInstancesHash();

					COptimizedKPSetConceptInstantiatedItem* addKnownSameIndividual(CIndividual* individual);
					bool hasKnownSameIndividual(CIndividual* individual);

					COptimizedKPSetConceptInstantiatedItem* addPossibleSameIndividualItem(COptimizedKPSetConceptInstantiatedItem* individualItem);
					COptimizedKPSetConceptInstantiatedItem* removePossibleSameIndividualItem(COptimizedKPSetConceptInstantiatedItem* individualItem);
					bool hasPossibleSameIndividualItem(COptimizedKPSetConceptInstantiatedItem* individualItem);

					QSet<CIndividual*>* getKnownSameIndividualSet();
					QSet<COptimizedKPSetConceptInstantiatedItem*>* getPossibleSameInstantiatedItemSet();
					cint64 getPossibleSameInstantiatedItemCount();


					cint64 getTestingPossibleInstantiatedCount();
					COptimizedKPSetConceptInstantiatedItem* setTestingPossibleInstantiatedCount(cint64 testCount);
					COptimizedKPSetConceptInstantiatedItem* incTestingPossibleInstantiatedCount(cint64 incCount = 1);
					COptimizedKPSetConceptInstantiatedItem* decTestingPossibleInstantiatedCount(cint64 decCount = 1);

					COptimizedKPSetConceptInstantiatedItem* setTestingPossibleInstantiated(COptimizedKPSetConceptInstancesItem* item);

					cint64 getPossibleInstantiatedCount();
					COptimizedKPSetConceptInstantiatedItem* setPossibleInstantiatedCount(cint64 testCount);
					COptimizedKPSetConceptInstantiatedItem* incPossibleInstantiatedCount(cint64 incCount = 1);
					COptimizedKPSetConceptInstantiatedItem* decPossibleInstantiatedCount(cint64 decCount = 1);


					cint64 getTestingPossibleSameIndividualCount();
					COptimizedKPSetConceptInstantiatedItem* setTestingPossibleSameIndividualCount(cint64 testCount);
					COptimizedKPSetConceptInstantiatedItem* incTestingPossibleSameIndividualCount(cint64 incCount = 1);
					COptimizedKPSetConceptInstantiatedItem* decTestingPossibleSameIndividualCount(cint64 decCount = 1);

					bool hasPossibleInstantiatedTesting();
					bool hasPossibleInstantiated();

					bool hasPossibleSameIndividuals();
					bool hasPossibleSameIndividualsTesting();

					COptimizedKPSetConceptInstantiatedItem* takeTestingPossibleSameIndividualItem();

					bool hasToProcessPossibleSameIndividualsFlag();
					COptimizedKPSetConceptInstantiatedItem* setToProcessPossibleSameIndividualsFlag(bool toProcess);

					bool hasPossibleSameIndividualsProcessingQueuedFlag();
					COptimizedKPSetConceptInstantiatedItem* setPossibleSameIndividualsProcessingQueuedFlag(bool processingQueued);

					bool isItemSameIndividualMerged();
					COptimizedKPSetConceptInstantiatedItem* setItemSameIndividualMerged(bool merged);

				// protected methods
				protected:

				// protected variables
				protected:
					CIndividual* mIndividual;
					COptimizedKPSetConceptInstancesHash mKnownPossibleInstancesHash;
					QSet<CIndividual*> mKnownSameIndividualSet;
					QSet<COptimizedKPSetConceptInstantiatedItem*> mPossibleSameInstantiatedItemSet;
					cint64 mTestingPossibleInstantiatedCount;
					cint64 mPossibleInstantiatedCount;
					cint64 mPossibleSameIndividualCount;

					bool mItemSameIndividualMerged;
					bool mToProcessPossibleSameIndividualsFlag;
					bool mPossibleSameIndividualsProcessingQueuedFlag;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANTIATEDITEM_H
