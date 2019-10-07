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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COptimizedKPSetRoleTestingItem_H
#define KONCLUDE_REASONER_CLASSIFIER_COptimizedKPSetRoleTestingItem_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "COptimizedKPSetRolePossibleSubsumptionMap.h"
#include "CConceptSubsumerObserver.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSatisfiableReferenceLinkingData.h"

#include "Reasoner/Taxonomy/CRolePropertiesHierarchy.h"

#include "Reasoner/Kernel/Cache/CCacheEntry.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Classifier {


			/*! 
			 *
			 *		\class		COptimizedKPSetRoleTestingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetRoleTestingItem : public CConceptSubsumerObserver {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetRoleTestingItem();
					~COptimizedKPSetRoleTestingItem();


					COptimizedKPSetRoleTestingItem* initSatisfiableTestingItem(CRole* role);

					CConcept* getTemporaryMarkerConcept();
					CConcept* getTemporaryPropagationConcept();
					CConcept* getTemporaryExistConcept();

					COptimizedKPSetRoleTestingItem* setTemporaryMarkerConcept(CConcept* markerConcept);
					COptimizedKPSetRoleTestingItem* setTemporaryPropagationConcept(CConcept* propConcept);
					COptimizedKPSetRoleTestingItem* setTemporaryExistConcept(CConcept* existConcept);


					CRole* getTestingRole();

					CRolePropertiesHierarchyNode* getSatisfiableRoleHierarchyNode();
					COptimizedKPSetRoleTestingItem* setSatisfiableRoleHierarchyNode(CRolePropertiesHierarchyNode* hierNode);

					QSet<COptimizedKPSetRoleTestingItem*>* getSubsumerRoleItemSet();
					QList<COptimizedKPSetRoleTestingItem*>* getSubsumerRoleItemList();
					cint64 getSubsumerRoleItemCount() const;
					bool hasSubsumerRoleItem(COptimizedKPSetRoleTestingItem* item);

					QList<COptimizedKPSetRoleTestingItem*>* getSuccessorItemList();
					cint64 getUnprocessedPredecessorItemCount();

					bool hasOnlyProcessedPredecessorItems();
					COptimizedKPSetRoleTestingItem* decUnprocessedPredecessorItems(cint64 decCount = 1);
					COptimizedKPSetRoleTestingItem* incUnprocessedPredecessorItems(cint64 incCount = 1);
					COptimizedKPSetRoleTestingItem* setUnprocessedPredecessorItems(cint64 predCount);

					COptimizedKPSetRoleTestingItem* addSuccessorSatisfiableTestItem(COptimizedKPSetRoleTestingItem* succItem);

					bool isSatisfiableTestOrdered();
					bool isSatisfiableTested();
					bool isResultUnsatisfiableDerivated();
					bool isResultSatisfiableDerivated();
					bool getSatisfiableTestedResult();

					COptimizedKPSetRoleTestingItem* setSatisfiableTestOrdered(bool satTestOrdered);
					COptimizedKPSetRoleTestingItem* setSatisfiableTested(bool satTested);
					COptimizedKPSetRoleTestingItem* setResultUnsatisfiableDerivated(bool unsatDerivated);
					COptimizedKPSetRoleTestingItem* setResultSatisfiableDerivated(bool satDerivated);
					COptimizedKPSetRoleTestingItem* setSatisfiableTestedResult(bool satTestedResult);


					COptimizedKPSetRoleTestingItem* addSubsumerRoleItem(COptimizedKPSetRoleTestingItem* subsumingItem);
					CConceptSubsumerObserver* tellConceptSupsumption(CConcept* subsumedConcept, CConcept* subsumerConcept);


					QList<COptimizedKPSetRoleTestingItem*>* sortSubsumingConceptItemList();

					bool isEquivalentItem();
					COptimizedKPSetRoleTestingItem* setEquivalentItem(bool equivalent);

					bool isPredecessorItem();
					COptimizedKPSetRoleTestingItem* setPredecessorItem(bool isPredecessorOfOneItem);

					bool isMoreConceptClassificationInformationRequired();

					COptimizedKPSetRolePossibleSubsumptionMap* getPossibleSubsumptionMap(bool create = false);


					QSet<COptimizedKPSetRoleTestingItem*>* getUpPropagationItemSet();
					QSet<COptimizedKPSetRoleTestingItem*>* getDownPropagationItemSet();
					COptimizedKPSetRoleTestingItem* addUpPropagationItem(COptimizedKPSetRoleTestingItem* item);
					COptimizedKPSetRoleTestingItem* addDownPropagationItem(COptimizedKPSetRoleTestingItem* item);
					bool isPropagationConnected();
					COptimizedKPSetRoleTestingItem* setPropagationConnected(bool connected);


					virtual CPropertyPossibleSubsumptionMap* getPropertyPossibleSubsumptionMap();
					bool isPossibleSubsumptionMapInitialized();
					COptimizedKPSetRoleTestingItem* setPossibleSubsumptionMapInitialized(bool initialized);



					QSet<COptimizedKPSetRoleTestingItem*>* getPossibleSubsumerSet(bool create);
					QList<COptimizedKPSetRoleTestingItem*>* getPossibleSubsumerList();
					COptimizedKPSetRoleTestingItem* setPossibleSubsumedList(QList<COptimizedKPSetRoleTestingItem*>* possSubsumedList);
					bool hasRemainingPossibleSubsumedItems();




				// protected methods
				protected:

				// protected variables
				protected:
					CRole* mRole;
					CRolePropertiesHierarchyNode* mConSatHierNode;

					QSet<COptimizedKPSetRoleTestingItem*> mSubsumingConceptItemSet;
					QList<COptimizedKPSetRoleTestingItem*> mSubsumingConceptItemList;

					QList<COptimizedKPSetRoleTestingItem*> mSuccList;
					cint64 mPredCounter;

					bool mSatTestOrdered;
					bool mTestedSat;
					bool mUnsatDerivated;
					bool mSatDerivated;
					bool mSatTestedResult;

					bool mPropagationConnected;
					QSet<COptimizedKPSetRoleTestingItem*> mUpPropagationItemSet;
					QSet<COptimizedKPSetRoleTestingItem*> mDownPropagationItemSet;


					QSet<COptimizedKPSetRoleTestingItem*>* mPossibleSubsumedSet;
					QList<COptimizedKPSetRoleTestingItem*>* mPossibleSubsumedList;


					bool mEquiItem;
					bool mPredOfItem;

					COptimizedKPSetRolePossibleSubsumptionMap* mPossibleSubsumptionMap;
					bool mPossSubsumMapInitialized;

					CConcept* mTmpMarkerConcept;
					CConcept* mTmpPropagationConcept;
					CConcept* mTmpExistConcept;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COptimizedKPSetRoleTestingItem_H
