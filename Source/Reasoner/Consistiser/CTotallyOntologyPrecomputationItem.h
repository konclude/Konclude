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

#ifndef KONCLUDE_REASONER_CONSISTISER_CTOTALLYONTOLOGYPRECOMPUTATIONITEM_H
#define KONCLUDE_REASONER_CONSISTISER_CTOTALLYONTOLOGYPRECOMPUTATIONITEM_H

// Libraries includes
#include <QAtomicPointer>

// Namespace includes
#include "ConsistiserSettings.h"
#include "CConsistenceObserver.h"
#include "CSaturationObserver.h"
#include "COntologyPrecomputationItem.h"
#include "CPrecomputationTestingStep.h"
#include "CSaturationConceptDataItem.h"
#include "CSaturationIndividualDataItem.h"
#include "CSaturationIndividualAnalysationObserver.h"
#include "CIndividualPrecomputationCoordinationHash.h"


// Other includes
#include "Reasoner/Consistence/CConsistenceData.h"
#include "Reasoner/Consistence/CSaturationData.h"
#include "Reasoner/Consistence/CConsistence.h"

#include "Reasoner/Ontology/OntologySettings.h"
#include "Reasoner/Ontology/COntologyProcessingStepRequirement.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CIndividualProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"
#include "Reasoner/Ontology/COntologyProcessingConceptSaturationRequirement.h"
#include "Reasoner/Ontology/CIndividualReference.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Query/CApproximatedSaturationCalculationJob.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Consistence;
		using namespace Ontology;
		using namespace Query;

		namespace Consistiser {


			typedef QPair<CRole*,TConceptNegPair> TRoleConceptNegTriple;


			/*! 
			 *
			 *		\class		CTotallyOntologyPrecomputationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTotallyOntologyPrecomputationItem : public COntologyPrecomputationItem, public CConsistenceObserver, public CSaturationObserver, public CSaturationIndividualAnalysationObserver {
				// public methods
				public:
					//! Constructor
					CTotallyOntologyPrecomputationItem();
					~CTotallyOntologyPrecomputationItem();

					CTotallyOntologyPrecomputationItem* initTotallyPrecomputationItem(CConcreteOntology* ontology, CConfigurationBase* config);

					virtual COntologyPrecomputationItem* addPrecomputationRequirement(COntologyProcessingRequirement* ontoRequirement);

					CTotallyOntologyPrecomputationItem* setConsistenceCheched(bool consistenceChecked);
					bool hasConsistenceCheched();

					CTotallyOntologyPrecomputationItem* setConsistenceCheckCreated(bool consistenceCheckCreated);
					bool hasConsistenceCheckCreated();

					virtual CConsistenceObserver* tellConsistenceData(CConsistenceData* consData);
					CConsistenceData* getConsistenceData();
					virtual CSaturationObserver* tellSaturationData(CSaturationData* saturationData);
					CSaturationData* getSaturationData();

					CConsistence* getConsistence();
					CTotallyOntologyPrecomputationItem* setConsistence(CConsistence* consistence);

					CCalculationConfigurationExtension* getCalculationConfiguration();

					CTotallyOntologyPrecomputationItem* setPrecomputation(CPrecomputation* precomputation);
					CPrecomputation* getPrecomputation();
					CTotallyOntologyPrecomputationItem* setConceptCyclePrecomputationInitialized(bool initialized);
					bool hasConceptCyclePrecomputationInitialized();
					CTotallyOntologyPrecomputationItem* setConceptCyclePrecomputationChecked(bool checked);
					bool hasConceptCyclePrecomputationChecked();
					QList<CConceptCycleData*>* getRemainingConceptCyclePrecomputationList();
					CTotallyOntologyPrecomputationItem* addRemainingConceptCyclePrecomputation(CConceptCycleData* conceptCycleData);
					bool hasRemainingConceptCyclePrecomputation();

					CTotallyOntologyPrecomputationItem* setConceptSaturationPrecomputationCreated(bool initialized);
					bool hasConceptSaturationPrecomputationCreated();
					CTotallyOntologyPrecomputationItem* setConceptSaturationPrecomputationChecked(bool checked);
					bool hasConceptSaturationPrecomputationChecked();





					CSaturationIndividualAnalysationObserver* notifyClashedIndividual();
					CSaturationIndividualAnalysationObserver* notifyInsufficientIndividual();


					bool hasClashedSaturationIndividuals();
					bool hasInsufficientSaturationIndividuals();


					bool isForceCompletionGraphConstruction();
					CTotallyOntologyPrecomputationItem* setForceCompletionGraphConstruction(bool forceCompletionGraphConstruction);
					bool isFullCompletionGraphConstruction();
					CTotallyOntologyPrecomputationItem* setFullCompletionGraphConstruction(bool fullCompletionGraphConstruction);
					bool isFullCompletionGraphConstructed();
					CTotallyOntologyPrecomputationItem* setFullCompletionGraphConstructed(bool fullCompletionGraphConstructed);



					bool hasIndividualsSaturated();
					CTotallyOntologyPrecomputationItem* setIndividualsSaturated(bool saturated);

					bool hasALLIndividualsSaturationOrderd();
					CTotallyOntologyPrecomputationItem* setALLIndividualsSaturationOrderd(bool allSaturationOrderd);


					bool failAfterConsistencyConceptSaturation();
					bool failAfterConsistencyChecking();
					bool failAfterConceptSaturation();



					bool requiresAllConceptsSaturation();
					bool hasAllConceptsSaturationOrdered();
					CTotallyOntologyPrecomputationItem* setAllConceptsSaturationOrdered(bool ordered);
					QList<CSaturationConceptDataItem*>* getSaturationOrderedItemList();
					bool hasRemainingRequiredSaturationConcepts();
					QList<TConceptNegPair>* getRemainingRequiredSaturationConceptList();
					CTotallyOntologyPrecomputationItem* addRequiredSaturationConcept(CConcept* concept, bool negation);

					bool requiresNominalDelayedConceptsSaturationUpdate();
					CTotallyOntologyPrecomputationItem* setNominalDelayedConceptsSaturationUpdateRequired(bool updateRequired);

					bool hasNominalDelayedConceptsSaturationUpdated();
					CTotallyOntologyPrecomputationItem* setNominalDelayedConceptsSaturationUpdated(bool updated);

					bool hasRemainingConsistencyRequiredSaturationConcepts();
					QList<TConceptNegPair>* getRemainingConsistencyRequiredSaturationConceptList();
					CTotallyOntologyPrecomputationItem* addConsistencyRequiredSaturationConcept(CConcept* concept, bool negation);


					bool hasRemainingRequiredSaturationIndividuals();
					bool hasRemainingRequiredABoxSaturationIndividuals();
					QList<CIndividual*>* getRemainingRequiredABoxSaturationIndividuals();
					CTotallyOntologyPrecomputationItem* addRequiredABoxSaturationIndividual(CIndividual* individual);
					CTotallyOntologyPrecomputationItem* setTripleIndexedIndividualSaturated(bool saturated);


					cint64 getHandledTriplesIndividualSaturatedId();
					CTotallyOntologyPrecomputationItem* setHandledTriplesIndividualSaturatedId(cint64 handledId);



					cint64 getMinConceptCycleTestSize();
					CTotallyOntologyPrecomputationItem* setMinConceptCycleTestSize(cint64 minSize);

					CPrecomputationTestingStep* getConsistencePrecomputationStep();
					bool isConsistenceStepFinished();
					bool isConsistenceStepRequired();
					bool areConsistenceStepProcessingRequirementSatisfied();


					CPrecomputationTestingStep* getCyclePrecomputationStep();
					bool isCycleStepFinished();
					bool isCycleStepRequired();
					bool areCycleStepProcessingRequirementSatisfied();

					CPrecomputationTestingStep* getSaturationPrecomputationStep();
					bool isSaturationStepRunning();
					bool isSaturationStepFinished();
					bool isSaturationStepRequired();
					bool areSaturationStepProcessingRequirementSatisfied();



					CTotallyOntologyPrecomputationItem* setSaturationStepRunning(bool satStepRunning);
				
					bool isSaturationComputationRunning();
					CTotallyOntologyPrecomputationItem* setSaturationComputationRunning(bool satCompRunning);


					CTotallyOntologyPrecomputationItem* setSaturationOccurrenceStatisticsCollectingInitialized(bool initialized);
					bool hasSaturationOccurrenceStatisticsCollectingInitialized();
					CTotallyOntologyPrecomputationItem* setSaturationOccurrenceStatisticsCollected(bool checked);
					bool hasSaturationOccurrenceStatisticsCollected();

					CPrecomputationTestingStep* getOccurrenceStatisticsPrecomputationStep();
					bool isOccurrenceStatisticsStepFinished();
					bool isOccurrenceStatisticsStepRequired();
					bool areOccurrenceStatisticsStepProcessingRequirementSatisfied();




					CPrecomputationTestingStep* getIndividualPrecomputationStep();
					bool isIndividualStepRunning();
					bool isIndividualStepFinished();
					bool isIndividualStepRequired();
					bool areIndividualStepProcessingRequirementSatisfied();
					CTotallyOntologyPrecomputationItem* setIndividualStepRunning(bool satStepRunning);


					bool isIndividualComputationRunning();
					CTotallyOntologyPrecomputationItem* setIndividualComputationRunning(bool indiCompRunning);
					CTotallyOntologyPrecomputationItem* incIndividualComputationRunningCount(cint64 incCount = 1);
					CTotallyOntologyPrecomputationItem* decIndividualComputationRunningCount(cint64 decCount = 1);


					CTotallyOntologyPrecomputationItem* setAllIndividualPrecomputationCreated(bool initialized);
					bool hasAllIndividualPrecomputationCreated();
					CTotallyOntologyPrecomputationItem* setIndividualPrecomputationChecked(bool checked);
					bool hasIndividualPrecomputationChecked();

					CTotallyOntologyPrecomputationItem* setIndividualPrecomputationClashed(bool checked);
					bool hasIndividualPrecomputationClashed();
					bool* getIndividualPrecomputationClashedPointer();




					CTotallyOntologyPrecomputationItem* incIndividualSaturationRunningCount(cint64 incCount = 1);
					CTotallyOntologyPrecomputationItem* decIndividualSaturationRunningCount(cint64 decCount = 1);
					cint64 getIndividualSaturationRunningCount();
					bool hasIndividualSaturationRunning();


					bool areAllStepFinished();
					bool hasRemainingProcessingRequirements();

					CSaturationConceptDataItem* getSaturationConceptDataItem(CConcept* satConcept, bool negated, bool create);
					CSaturationConceptDataItem* getSaturationRoleSuccessorConceptDataItem(CRole* role, CConcept* satConcept, bool negated, bool create);

					CApproximatedSaturationCalculationJob* getSaturationCalculationJob();
					CTotallyOntologyPrecomputationItem* setSaturationCalculationJob(CApproximatedSaturationCalculationJob* calculationJob);


					QSet<CConcept*>* getSaturatedDisjunctionSet();
					CTotallyOntologyPrecomputationItem* addSaturatedDisjunction(CConcept* disjunctionConcept);

					CIndividual* getAllAssertionIndividual();
					CTotallyOntologyPrecomputationItem* setAllAssertionIndividual(CIndividual* individual);

					bool isAllAssertionIndividualSaturated();
					CTotallyOntologyPrecomputationItem* setAllAssertionIndividualSaturated(bool saturated);

					bool hasAllAssertionIndividualSufficientSaturationChecked();
					CTotallyOntologyPrecomputationItem* setAllAssertionIndividualSufficientSaturationChecked(bool sufficientSaturatedChecked);

					bool isAllAssertionIndividualSaturationSufficient();
					CTotallyOntologyPrecomputationItem* setAllAssertionIndividualSaturationSufficient(bool sufficientSaturated);


					CSaturationIndividualDataItem* takeFreeSaturationIndividualDataItem(bool create = true);
					CTotallyOntologyPrecomputationItem* addFreeSaturationIndividualDataItem(CSaturationIndividualDataItem* item);

					CTotallyOntologyPrecomputationItem* setSaturationIDIndividualDataItems(cint64 saturationID, CSaturationIndividualDataItem* items);
					CTotallyOntologyPrecomputationItem* releaseSaturationIDIndividualDataItems(cint64 saturationID);
					cint64 getNextSaturationID(bool moveNext = true);



					bool hasIndividualsSaturationCacheSynchronisation();
					CTotallyOntologyPrecomputationItem* setIndividualsSaturationCacheSynchronisation(bool synchronized);


					CIndividualPrecomputationCoordinationHash* getCurrentIndividualComputationCoordinationHash();
					CTotallyOntologyPrecomputationItem* setCurrentIndividualComputationCoordinationHash(CIndividualPrecomputationCoordinationHash* coordHash);
					QSet<CIndividualReference>* getCurrentIndividualComputationSet();

					QSet<CIndividualReference>* getIncompletelyHandledIndividualSet();
					cint64 getCurrentIncompletelyHandledIndividualRetrievalLimit();
					CTotallyOntologyPrecomputationItem* setCurrentIncompletelyHandledIndividualRetrievalLimit(cint64 limit);
					bool hasAllIncompletelyHandledIndividualsRetrieved();
					CTotallyOntologyPrecomputationItem* setAllIncompletelyHandledIndividualsRetrieved(bool allRetrieved);


					cint64 getLastMinimumRetrievedIncompletelyHandledIndividualId();
					CTotallyOntologyPrecomputationItem* setLastMinimumRetrievedIncompletelyHandledIndividualId(cint64 minId);


					cint64 getCurrentIncompletelyHandledIndividualComputationLimit();
					CTotallyOntologyPrecomputationItem* setCurrentIncompletelyHandledIndividualComputationLimit(cint64 limit);


					QTime* getInitializationTime();


					cint64 getNextRepresentativeCacheRecomputationId();



				// protected methods
				protected:

				// protected variables
				protected:
					CConsistence* mConsistence;
					bool mConsistenceCheckCreated;
					bool mConsistenceCheched;
					QAtomicPointer<CConsistenceData> mConsData;
					QAtomicPointer<CSaturationData> mSaturData;
					QMutex mSaturDataAddMutex;
					QList<CSaturationData*> mSaturDataList;
					CSaturationData* mLastSaturData;

					CPrecomputation* mPrecomputation;
					bool mConceptCyclePrecomputationInitialized;
					bool mConceptCyclePrecomputationChecked;
					QList<CConceptCycleData*> mRemainingConceptCyclePrecomputationList;
					cint64 mMinConceptCycleTestSize;

					bool mFailAfterConsistencyConceptSaturation;
					bool mFailAfterConsistencyChecking;
					bool mFailAfterConceptSaturation;

					bool mSaturationTestRunning;
					bool mSaturationComputationRunning;
					bool mIndividualTestRunning;

					cint64 mIndividualSaturationRunningCount;
					cint64 mIndividualComputationRunningCount;
					bool mIndividualComputationRunning;
					bool mIndividualPrecomputationCreated;
					bool mIndividualPrecomputationChecked;
					bool mIndividualPrecomputationClashed;

					CPrecomputationTestingStep* mConsistencePrecomputationStep;
					CPrecomputationTestingStep* mCyclePrecomputationStep;
					CPrecomputationTestingStep* mOccurrenceStatisticsPrecomputationStep;
					CPrecomputationTestingStep* mSaturationPrecomputationStep;
					CPrecomputationTestingStep* mIndividualPrecomputationStep;
					QList<CPrecomputationTestingStep*> mProcessingSteps;

					QSet<CConcept*> mDisjunctionSaturationSet;


					bool mConceptSaturationPrecomputationCreated;
					bool mConceptSaturationPrecomputationChecked;
					QHash<TConceptNegPair,CSaturationConceptDataItem*> mConceptNegationSatItemHash;
					QHash<TRoleConceptNegTriple,CSaturationConceptDataItem*> mRoleConceptNegationSatItemHash;
					QList<CSaturationConceptDataItem*> mSatItemContainer;
					QList<CSaturationConceptDataItem*> mSatOrderedItemList;
					bool mAllConSatRequired;
					bool mAllConSatOrdered;
					QList<TConceptNegPair> mRemainingReqSatConList;
					QList<TConceptNegPair> mRemainingConsReqSatConList;
					bool mNomDelayedConSatUpRequired;
					bool mNomDelayedConSatUpdated;

					QList<CIndividual*> mRemainingABoxIndiSatList;
					cint64 mHandledTriplesIndiSaturatedId;
					bool mTriplesIndexedIndisSaturated;


					CApproximatedSaturationCalculationJob* mApproxSatCalcJob;

					CCalculationConfigurationExtension* mCalculationConfig;

					CIndividual* mAllAssertionIndividual;
					bool mAllAssertionIndividualSaturated;
					bool mAllAssertionIndividualSufficientSaturationChecked;
					bool mAllAssertionIndividualSaturationSufficient;

					CSaturationIndividualDataItem* mFreeItemLinker;
					QHash<cint64,CSaturationIndividualDataItem*> mSatIDIndividualItemsHash;
					cint64 mNextSaturationID;

					bool mIndividualSaturationClashed;
					bool mIndividualSaturationInsufficient;
					bool mIndividualSaturated;
					bool mAllIndividualSaturaturationOrderd;

					bool mIndividualsSaturationCacheSynchronisation;
					QSet<CIndividualReference> mIncompHandledIndiSet;
					QSet<CIndividualReference> mCurrentIndiComputationSet;
					CIndividualPrecomputationCoordinationHash* mCurrentIndiCompCoordHash;
					cint64 mCurrentIncompIndiRetrievalLimit;
					bool mAllIncompIndiRetrieved;
					cint64 mLastMinRetrievedIncompIndiId;
					cint64 mCurrentIncompIndiComputationLimit;

					cint64 mNextRepresentativeCacheRecomputationId = 1;


					QTime mInitTime;


					bool mSaturationOccurrenceStatisticsCollected;
					bool mSaturationOccurrenceStatisticsCollectingInitialized;


					bool mForceCompletionGraphConstruction;
					bool mFullCompletionGraphConstruction;
					bool mFullCompletionGraphConstructed;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CTOTALLYONTOLOGYPRECOMPUTATIONITEM_H
