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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANTIATEDITEM_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANTIATEDITEM_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetConceptInstancesHash.h"
#include "COptimizedKPSetConceptInstancesData.h"
#include "COptimizedKPSetRoleNeighbourInstancesHash.h"
#include "COptimizedKPSetRoleInstancesData.h"
#include "COntologyRealizingDynamicRequirmentProcessingContainer.h"

// Other includes
#include "Reasoner/Ontology/CIndividual.h"

#include "Reasoner/Realization/CConceptInstanceItem.h"
#include "Reasoner/Realization/CRoleInstanceItem.h"
#include "Reasoner/Realization/CSameInstanceItem.h"

#include "Reasoner/Consistiser/CIndividualDependenceTrackingCollector.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Realization;
		using namespace Consistiser;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetConceptInstantiatedItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetIndividualItem : public CConceptInstanceItem, public CRoleInstanceItem, public CSameInstanceItem, public COntologyRealizingDynamicRequirmentProcessingContainer {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetIndividualItem();

					COptimizedKPSetIndividualItem* initInstantiatedItem(cint64 indiId, CIndividual* individual);

					cint64 getIndividualId();
					CIndividual* getIndividual();
					CIndividualReference getIndividualReference();
					COptimizedKPSetConceptInstancesHash* getKnownPossibleInstancesHash();
					COptimizedKPSetRoleNeighbourInstancesHash* getKnownPossibleRoleNeighboursInstancesHash();

					COptimizedKPSetIndividualItem* addKnownSameIndividual(const CIndividualReference& individualRef);
					bool hasKnownSameIndividual(const CIndividualReference& individualRef);

					COptimizedKPSetIndividualItem* addPossibleSameIndividualItem(COptimizedKPSetIndividualItem* individualItem);
					COptimizedKPSetIndividualItem* removePossibleSameIndividualItem(COptimizedKPSetIndividualItem* individualItem);
					bool hasPossibleSameIndividualItem(COptimizedKPSetIndividualItem* individualItem);

					QSet<CIndividualReference>* getKnownSameIndividualSet();
					QSet<COptimizedKPSetIndividualItem*>* getPossibleSameInstantiatedItemSet();
					cint64 getPossibleSameInstantiatedItemCount();


					cint64 getTestingPossibleInstantiatedCount();
					COptimizedKPSetIndividualItem* setTestingPossibleInstantiatedCount(cint64 testCount);
					COptimizedKPSetIndividualItem* incTestingPossibleInstantiatedCount(cint64 incCount = 1);
					COptimizedKPSetIndividualItem* decTestingPossibleInstantiatedCount(cint64 decCount = 1);

					COptimizedKPSetIndividualItem* setTestingPossibleInstantiated(COptimizedKPSetConceptInstancesItem* item);
					COptimizedKPSetIndividualItem* setTestingPossibleInstantiated(COptimizedKPSetRoleInstancesItem* roleItem, COptimizedKPSetIndividualItem* successorItem);

					cint64 getPossibleInstantiatedCount();
					COptimizedKPSetIndividualItem* setPossibleInstantiatedCount(cint64 testCount);
					COptimizedKPSetIndividualItem* incPossibleInstantiatedCount(cint64 incCount = 1);
					COptimizedKPSetIndividualItem* decPossibleInstantiatedCount(cint64 decCount = 1);

					cint64 getTestingPossibleRoleInstantiatedCount();
					COptimizedKPSetIndividualItem* setTestingPossibleRoleInstantiatedCount(cint64 testCount);
					COptimizedKPSetIndividualItem* incTestingPossibleRoleInstantiatedCount(cint64 incCount = 1);
					COptimizedKPSetIndividualItem* decTestingPossibleRoleInstantiatedCount(cint64 decCount = 1);

					cint64 getPossibleRoleInstantiatedCount();
					COptimizedKPSetIndividualItem* setPossibleRoleInstantiatedCount(cint64 testCount);
					COptimizedKPSetIndividualItem* incPossibleRoleInstantiatedCount(cint64 incCount = 1);
					COptimizedKPSetIndividualItem* decPossibleRoleInstantiatedCount(cint64 decCount = 1);


					cint64 getTestingPossibleSameIndividualCount();
					COptimizedKPSetIndividualItem* setTestingPossibleSameIndividualCount(cint64 testCount);
					COptimizedKPSetIndividualItem* incTestingPossibleSameIndividualCount(cint64 incCount = 1);
					COptimizedKPSetIndividualItem* decTestingPossibleSameIndividualCount(cint64 decCount = 1);

					bool hasPossibleInstantiatedTesting();
					bool hasPossibleInstantiated();

					bool hasPossibleRoleInstantiatedTesting();
					bool hasPossibleRoleInstantiated();

					bool hasPossibleSameIndividuals();
					bool hasPossibleSameIndividualsTesting();

					COptimizedKPSetIndividualItem* takeTestingPossibleSameIndividualItem();

					bool hasToProcessPossibleSameIndividualsFlag();
					COptimizedKPSetIndividualItem* setToProcessPossibleSameIndividualsFlag(bool toProcess);

					bool hasPossibleSameIndividualsProcessingQueuedFlag();
					COptimizedKPSetIndividualItem* setPossibleSameIndividualsProcessingQueuedFlag(bool processingQueued);

					bool isItemSameIndividualMerged();
					COptimizedKPSetIndividualItem* setItemSameIndividualMerged(COptimizedKPSetIndividualItem* mergedIndi);
					COptimizedKPSetIndividualItem* getItemSameIndividualMerged();


					cint64 getInitializingRoleCandidateCount();
					COptimizedKPSetIndividualItem* setInitializingRoleCandidateCount(cint64 testCount);
					COptimizedKPSetIndividualItem* incInitializingRoleCandidateCount(cint64 incCount = 1);
					COptimizedKPSetIndividualItem* decInitializingRoleCandidateCount(cint64 decCount = 1);



					bool hasInitializingRoleCandidatesQueuedFlag();
					COptimizedKPSetIndividualItem* setInitializingRoleCandidatesQueuedFlag(bool initializingQueued);


					bool hasAllRoleCandidatesInitializedFlag();
					COptimizedKPSetIndividualItem* setAllRoleCandidatesInitializedFlag(bool allInitalized);


					CConcept* getTemporaryIndividualNominalConcept();
					COptimizedKPSetIndividualItem* setTemporaryIndividualNominalConcept(CConcept* concept);


					CIndividualDependenceTrackingCollector* getIndividualDependenceTrackingCollector();
					COptimizedKPSetIndividualItem* setIndividualDependenceTrackingCollector(CIndividualDependenceTrackingCollector* indiDepTrackColl);



				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mIndividualId;
					CIndividual* mIndividual;
					COptimizedKPSetConceptInstancesHash mKnownPossibleInstancesHash;
					COptimizedKPSetRoleNeighbourInstancesHash mKnownPossibleRoleNeigbourInstancesHash;
					QSet<CIndividualReference> mKnownSameIndividualSet;
					QSet<COptimizedKPSetIndividualItem*> mPossibleSameInstantiatedItemSet;
					cint64 mTestingPossibleInstantiatedCount;
					cint64 mPossibleInstantiatedCount;

					cint64 mTestingPossibleRoleInstantiatedCount;
					cint64 mPossibleRoleInstantiatedCount;

					cint64 mPossibleSameIndividualCount;

					COptimizedKPSetIndividualItem* mItemSameIndividualMerged;
					bool mToProcessPossibleSameIndividualsFlag;
					bool mPossibleSameIndividualsProcessingQueuedFlag;

					cint64 mInitializingRoleCandidateCount;
					bool mInitializingRoleCandidatesQueuedFlag;
					bool mAllRoleCandidatesInitializedFlag;

					CConcept* mTmpNominalConcept;
					CIndividualDependenceTrackingCollector* mIndiDepTrackingCollector;




				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANTIATEDITEM_H
