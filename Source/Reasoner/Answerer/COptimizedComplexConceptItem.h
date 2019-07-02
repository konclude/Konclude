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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CComplexConceptItemComputationProcess.h"
#include "CRequirementWaitingDependencyData.h"



// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/OntologySettings.h"
#include "Reasoner/Ontology/CConceptProcessData.h"

#include "Reasoner/Taxonomy/CHierarchyNode.h"
#include "Reasoner/Realization/CConceptInstanceItem.h"

#include "Parser/Expressions/CClassTermExpression.h"

#include "Reasoner/Realization/CRealizationIndividualInstanceItemReference.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;
		using namespace Realization;

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexConceptItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexConceptItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexConceptItem();
					~COptimizedComplexConceptItem();

					

					COptimizedComplexConceptItem* initComplexConcept(CConcept* concept, bool negated);

					bool getConceptNegation();
					CConcept* getConcept();

					bool representsConceptClass();
					TConceptNegPair getConceptNegationPair();


					CConcept* getCandidateConcept();
					CConcept* getMarkerConcept();
					CConcept* getPropagationConcept();

					COptimizedComplexConceptItem* setCandidateConcept(CConcept* concept);
					COptimizedComplexConceptItem* setMarkerConcept(CConcept* concept);
					COptimizedComplexConceptItem* setPropagationConcept(CConcept* concept);

					COptimizedComplexConceptItem* setCompletelyHandledReducedInstanceTestingConcept(CConcept* concept);
					CConcept* getCompletelyHandledReducedInstanceTestingConcept();
					COptimizedComplexConceptItem* setCompletelyHandledReducedInstanceTestingConceptNegation(bool negation);
					bool getCompletelyHandledReducedInstanceTestingConceptNegation();


					COptimizedComplexConceptItem* setAllAtomicConceptsCompletelyHandled(bool completelyHandled);
					bool getAllAtomicConceptsCompletelyHandled();



					CClassTermExpression* getClassTermExpression();
					COptimizedComplexConceptItem* setClassTermExpression(CClassTermExpression* classExpression);


					CComplexConceptItemComputationProcess* getComputationProcess();

					bool isSatisfiable();
					COptimizedComplexConceptItem* setSatisfiability(bool satisfiable);

					bool isSuperClassComputationInitializing();
					COptimizedComplexConceptItem* setSuperClassComputationInitializing(bool initialized);


					COptimizedComplexConceptItem* setPossibleSuperClassNodeTestingList(QList<CHierarchyNode*>* classNodeList);
					QList<CHierarchyNode*>* getPossibleSuperClassNodeTestingList();


					COptimizedComplexConceptItem* setKnownSuperClassNodeSet(QSet<CHierarchyNode*>* directClassNodeSet);
					COptimizedComplexConceptItem* setPossibleSuperClassNodeSet(QSet<CHierarchyNode*>* directClassNodeSet);
					COptimizedComplexConceptItem* setDirectSuperClassNodeSet(QSet<CHierarchyNode*>* directClassNodeSet);


					QSet<CHierarchyNode*>* getKnownSuperClassNodeSet();
					QSet<CHierarchyNode*>* getPossibleSuperClassNodeSet();
					QSet<CHierarchyNode*>* getDirectSuperClassNodeSet();


					COptimizedComplexConceptItem* setPossibleSubClassTestingNodeSet(QSet<CHierarchyNode*>* directClassNodeSet);
					COptimizedComplexConceptItem* setDirectSubClassNodeSet(QSet<CHierarchyNode*>* directClassNodeSet);


					QSet<CHierarchyNode*>* getPossibleSubClassTestingNodeSet();
					QSet<CHierarchyNode*>* getDirectSubClassNodeSet();



					COptimizedComplexConceptItem* setPossibleSubClassNodeTestingList(QList<CHierarchyNode*>* classNodeList);
					QList<CHierarchyNode*>* getPossibleSubClassNodeTestingList();


					QSet<CConcept*>* getKnownSuperConceptSet();
					QSet<CConcept*>* getPossibleSuperConceptSet();
					COptimizedComplexConceptItem* setKnownSuperConceptSet(QSet<CConcept*>* conceptSet);
					COptimizedComplexConceptItem* setPossibleSuperConceptSet(QSet<CConcept*>* conceptSet);


					bool hasQueuedProcessStep();
					CComplexConceptStepComputationProcess* setQueuedProcessStep();
					COptimizedComplexConceptItem* setQueuedProcessStep(CComplexConceptStepComputationProcess* queuedProcessStep);


					QSet<COptimizedComplexConceptItem*>* getSuperComplexConceptItemSet();
					bool hasSuperComplexConceptItem(COptimizedComplexConceptItem* item);
					QSet<COptimizedComplexConceptItem*>* getSubComplexConceptItemSet();
					bool hasSubComplexConceptItem(COptimizedComplexConceptItem* item);

					QSet<CIndividualReference>* getInstancesItemSet();

					QSet<TConceptNegPair>* getAtomicConcpetSet();





					QSet<CIndividualReference>* getPossibleInstanceIndividualSet();
					QSet<CIndividualReference>* getKnownInstanceIndividualSet();


					COptimizedComplexConceptItem* setPossibleInstanceIndividualSet(QSet<CIndividualReference>* instanceSet);
					COptimizedComplexConceptItem* setKnownInstanceIndividualSet(QSet<CIndividualReference>* instanceSet);

					QSet<CRealizationIndividualInstanceItemReference>* getPossibleInstanceItemSet();
					QSet<CRealizationIndividualInstanceItemReference>* getKnownInstanceItemSet();

					COptimizedComplexConceptItem* setPossibleInstanceItemSet(QSet<CRealizationIndividualInstanceItemReference>* instanceSet);
					COptimizedComplexConceptItem* setKnownInstanceItemSet(QSet<CRealizationIndividualInstanceItemReference>* instanceSet);


					cint64 getPossibleTestedNonInstanceCount();
					COptimizedComplexConceptItem* incPossibleTestedNonInstanceCount(cint64 incCount = 1);

					cint64 getRealizationRetrievedInstanceCandidateCount();
					COptimizedComplexConceptItem* inRealizationRetrievedInstanceCandidateCount(cint64 incCount = 1);



					COptimizedComplexConceptItem* setMaximumSubClassNodeSet(QSet<CHierarchyNode*>* maxSubClassNodeSet);


					QSet<CHierarchyNode*>* getMaximumSubClassNodeSet();


					bool isLazyRealizationInitializationRequested();
					COptimizedComplexConceptItem* setLazyRealizationInitializationRequested(bool requested);



					cint64 getLastRetrievedCertainInstanceItemCount();
					COptimizedComplexConceptItem* setLastRetrievedCertainInstanceItemCount(cint64 certainCount);

					double getMinimalRetrievingInstanceItemSize();
					COptimizedComplexConceptItem* setMinimalRetrievingInstanceItemSize(double newSize);


					bool isLazyRealizationInitialized();
					COptimizedComplexConceptItem* setLazyRealizationInitialized(bool requested);
					bool isLazyNeighbourRealizationPruningRequired();
					COptimizedComplexConceptItem* setLazyNeighbourRealizationPruningRequired(bool requested);
					bool isLazyNeighbourRealizationRequested();
					COptimizedComplexConceptItem* setLazyNeighbourRealizationRequested(bool requested);

					bool isLazyRealizationInstancesRetrieved();
					COptimizedComplexConceptItem* setLazyRealizationInstancesRetrieved(bool requested);

					bool isLazyRealizationProcessing();
					cint64 getLazyRealizationProcessingCount();
					COptimizedComplexConceptItem* incLazyRealizationProcessingCount(cint64 incCount = 1);
					COptimizedComplexConceptItem* decLazyRealizationProcessingCount(cint64 decCount = 1);

					COptimizedComplexConceptItem* setCompletelyHandledChecked(bool requested);
					bool isCompletelyHandledChecked();




					COptimizedComplexConceptItem* setConceptAbsorbed(bool absorbed);
					bool isConceptAbsorbed();

					COptimizedComplexConceptItem* setCandidatePropagated(bool hasCandidate);
					bool isCandidatePropagated();

					COptimizedComplexConceptItem* setCandidateReceived(bool hasCandidate);
					bool isCandidateReceived();

					bool isCandidatePropagationProcessing();
					cint64 getCandidatePropagationProcessingCount();
					COptimizedComplexConceptItem* incCandidatePropagationProcessingCount(cint64 incCount = 1);
					COptimizedComplexConceptItem* decCandidatePropagationProcessingCount(cint64 decCount = 1);





					COptimizedComplexVariableConceptBaseItem* getVariableConceptBaseItem();
					COptimizedComplexConceptItem* setVariableConceptBaseItem(COptimizedComplexVariableConceptBaseItem* baseItem);


					bool isTopObjectPropertyUsed();
					COptimizedComplexConceptItem* setTopObjectPropertyUsage(bool topObjectPropertyUsed);


					CRealizationIndividualInstanceItemReferenceIterator* getRealizationIterator();
					COptimizedComplexConceptItem* setRealizationIterator(CRealizationIndividualInstanceItemReferenceIterator* realizationIterator);


					CRequirementWaitingDependencyData* getInstancesComputationRequirements();


					cint64 getConceptItemId();
					COptimizedComplexConceptItem* setConceptItemId(cint64 id);

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mConceptItemId;

					CConcept* mConcept;
					bool mConceptNegation;
					CConcept* mCandidateConcept;
					CConcept* mMarkerConcept;
					CConcept* mPropagationConcept;

					CConcept* mCompletelyHandledReducedInstanceTestingConcept;
					bool mCompletelyHandledReducedInstanceTestingConceptNegation;
					bool mAllAtomicConceptsCompletelyHandled;

					CClassTermExpression* mClassTermExpression;

					CComplexConceptStepComputationProcess* mQueuedProcessStep;

					bool mSatisfiable;
					bool mSuperClassComputationInitializing;

					CHierarchyNode* mKnownEquivalentClassNode;

					QSet<CHierarchyNode*>* mDirectSuperClassNodeSet;
					QSet<CHierarchyNode*>* mKnownSuperClassNodeSet;
					QSet<CHierarchyNode*>* mPossibleSuperClassNodeSet;
					QList<CHierarchyNode*>* mPossibleSuperClassNodeTestingList;
					QSet<CConcept*>* mKnownSuperConceptSet;
					QSet<CConcept*>* mPossibleSuperConceptSet;

					QSet<CHierarchyNode*>* mDirectSubClassNodeSet;
					QSet<CHierarchyNode*>* mPossibleSubClassTestingNodeSet;
					QList<CHierarchyNode*>* mPossibleSubClassNodeTestingList;
					QSet<CHierarchyNode*>* mMaximumSubClassNodeSet;


					QSet<TConceptNegPair> mAtomicClassConceptSet;

					QSet<COptimizedComplexConceptItem*> mSuperComplexConceptItemSet;
					QSet<COptimizedComplexConceptItem*> mSubComplexConceptItemSet;


					QSet<CIndividualReference>* mKnownInstanceSet;
					QSet<CIndividualReference>* mPossibleInstanceSet;

					QSet<CRealizationIndividualInstanceItemReference>* mPossibleInstanceItemSet;
					QSet<CRealizationIndividualInstanceItemReference>* mKnownInstanceItemSet;
					cint64 mPossibleTestedNonInstanceCount;
					cint64 mRealizationRetrievedInstanceCandidateCount;




					CComplexConceptItemComputationProcess mComputationProcess;

					bool mLazyRealizationInitialized;
					bool mLazyRealizationInitializationRequested;
					bool mLazyNeighbourRealizationPruningRequired;
					bool mLazyNeighbourRealizationRequested;
					bool mLazyRealizationInstancesRetrieved;
					bool mCompletelyHandledChecked;
					cint64 mLazyRealizationProcessingCount;


					COptimizedComplexVariableConceptBaseItem* mVarConBaseItem;


					bool mConceptAbsorbed;
					cint64 mCandidatePropagationProcessingCount;
					bool mCandidatePropagated;
					bool mCandidateReceived;


					bool mTopObjectPropertyUsed;

					CRealizationIndividualInstanceItemReferenceIterator* mRealizationIterator;


					CRequirementWaitingDependencyData mInstancesComputationRequirments;
					cint64 mLastRetrievingCertainInstanceItemCount;
					double mMinimalRetrievingInstanceItemSize;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTITEM_H
