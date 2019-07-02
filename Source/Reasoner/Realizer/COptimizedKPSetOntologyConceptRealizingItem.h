/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGITEM_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGITEM_H

// Libraries includes
#include <QAtomicPointer>

// Namespace includes
#include "RealizerSettings.h"
#include "CRealizingTestingItem.h"
#include "COntologyRealizingItem.h"
#include "CRealizingTestingStep.h"
#include "COptimizedKPSetRoleInstancesItem.h"
#include "COptimizedKPSetRoleInstancesRedirectionItem.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Ontology/COntologyProcessingStepRequirement.h"

#include "Reasoner/Realization/CRoleRealizationInstanceVisitor.h"
#include "Reasoner/Realization/CRoleRealizationIndividualVisitor.h"
#include "Reasoner/Realization/CRoleRealization.h"
#include "Reasoner/Realization/CConceptRealization.h"
#include "Reasoner/Realization/CSameRealization.h"
#include "Reasoner/Realization/CPossibleAssertionsCollectionSet.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Ontology;
		using namespace Realization;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetOntologyConceptRealizingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetOntologyConceptRealizingItem : public COntologyRealizingItem, public CRoleRealization, public CConceptRealization, public CSameRealization {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetOntologyConceptRealizingItem();
					~COptimizedKPSetOntologyConceptRealizingItem();

					virtual COptimizedKPSetOntologyConceptRealizingItem* initRequirementConfigRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config);
					
					COptimizedKPSetOntologyConceptRealizingItem* initConceptItemsFromHierarchy();
					COptimizedKPSetOntologyConceptRealizingItem* initRoleItemsFromHierarchy();
					COptimizedKPSetOntologyConceptRealizingItem* initRoleCandidateInitializingIndividualItems();

					COptimizedKPSetOntologyConceptRealizingItem* initComplexRolesStarterCandidates();

					bool hasItemsInitialized();
					COptimizedKPSetOntologyConceptRealizingItem* setItemsInitialized(bool initialized);

					bool hasRealizationSameIndividualsInitialized();
					COptimizedKPSetOntologyConceptRealizingItem* setRealizationSameIndividualsInitialized(bool initialized);

					bool hasRealizationConceptsInitialized();
					COptimizedKPSetOntologyConceptRealizingItem* setRealizationConceptsInitialized(bool initialized);

					bool hasRealizationRolesInitialized();
					COptimizedKPSetOntologyConceptRealizingItem* setRealizationRolesInitialized(bool initialized);

					COptimizedKPSetOntologyConceptRealizingItem* setTemporaryRoleRealizationOntology(CConcreteOntology* tmpOntology);
					CConcreteOntology* getTemporaryRoleRealizationOntology();

					virtual COntologyRealizingItem* addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement);

					CCalculationConfigurationExtension* getCalculationConfiguration();

					COptimizedKPSetOntologyConceptRealizingItem* markIntanceItemForConceptRealization(CConcept* concept, bool subconceptRealizationRequired = true);
					COptimizedKPSetOntologyConceptRealizingItem* addConceptInstanceItemToProcessPossibleInstances(COptimizedKPSetConceptInstancesItem* instanceItem);

					COptimizedKPSetOntologyConceptRealizingItem* markInstantiatedItemForSameIndividualsRealization();
					COptimizedKPSetOntologyConceptRealizingItem* addInstantiatedItemToProcessPossibleSameIndividuals(COptimizedKPSetIndividualItem* instantiatedItem);

					COptimizedKPSetOntologyConceptRealizingItem* markIntanceItemForRoleRealization(CRole* role, bool subroleRealizationRequired = true);
					COptimizedKPSetOntologyConceptRealizingItem* addRoleInstanceItemToProcessPossibleInstances(COptimizedKPSetRoleInstancesItem* instanceItem);
					COptimizedKPSetOntologyConceptRealizingItem* addRoleInstanceItemToInitializeCandidates(COptimizedKPSetRoleInstancesItem* instanceItem);
					COptimizedKPSetOntologyConceptRealizingItem* addRoleInstanceIndividualItemToInitializeCandidates(COptimizedKPSetIndividualItem* indiItem);



					QHash<CConcept*,COptimizedKPSetRoleInstancesItem*>* getMarkerConceptInstancesItemHash();



					bool isExtraConsistencyTestingStepRequired();
					COptimizedKPSetOntologyConceptRealizingItem* setExtraConsistencyTestingStepRequired(bool required);
					bool isExtraConsistencyTesting();
					COptimizedKPSetOntologyConceptRealizingItem* setExtraConsistencyTesting(bool testing);
					bool isExtraConsistencyTested();
					COptimizedKPSetOntologyConceptRealizingItem* setExtraConsistencyTested(bool tested);



					CRealizingTestingStep* getRealizeConceptProcessingStep();
					bool isRealizeConceptStepFinished();
					bool isRealizeConceptStepRequired();
					bool areRealizeConceptStepProcessingRequirementSatisfied();


					CRealizingTestingStep* getRealizeRoleProcessingStep();
					bool isRealizeRoleStepFinished();
					bool isRealizeRoleStepRequired();
					bool areRealizeRoleStepProcessingRequirementSatisfied();


					CRealizingTestingStep* getRealizeSameIndividualsProcessingStep();
					bool isRealizeSameIndividualsStepFinished();
					bool isRealizeSameIndividualsStepRequired();
					bool areRealizeSameIndividualsStepProcessingRequirementSatisfied();


					bool areAllStepFinished();
					bool hasRemainingProcessingRequirements();


					COptimizedKPSetOntologyConceptRealizingItem* setRealization(CRealization* realization);
					CRealization* getRealization();


					bool hasRemainingInitializingRoleInstanceItems();
					bool hasRemainingProcessingRoleInstanceItems();
					bool hasRemainingInitializingRoleInstanceIndividualItems();

					bool hasRemainingProcessingConceptInstanceItems();
					bool hasRemainingProcessingSameIndividualsItems();


					COptimizedKPSetOntologyConceptRealizingItem* addRoleCandidateInitalizingIndividualsItem(COptimizedKPSetIndividualItem* indiItem);
					QList<COptimizedKPSetIndividualItem*>* getRoleCandidateInitalizingIndividualsItemList();


					QHash<CHierarchyNode*,COptimizedKPSetConceptInstancesItem*>* getHierarchyNodeInstancesItemHash();
					QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* getConceptInstancesItemHash();

					QHash<CRole*,COptimizedKPSetRoleInstancesItem*>* getRoleInstancesItemHash();
					QList<COptimizedKPSetRoleInstancesItem*>* getRoleInstancesItemList();
					QList<COptimizedKPSetRoleInstancesItem*>* getComplexRoleInstancesItemList();

					QHash<CIndividual*,COptimizedKPSetIndividualItem*>* getIndividualInstantiatedItemHash();

					COptimizedKPSetIndividualItem* getIndividualInstantiatedItem(CIndividual* individual, bool directCreate = true);
					bool hasIndividualInstantiatedItem(CIndividual* individual);

					QList<COptimizedKPSetConceptInstancesItem*>* getProcessingPossibleConceptInstancesItemList();
					QList<COptimizedKPSetRoleInstancesItem*>* getProcessingPossibleRoleInstancesItemList();
					QList<COptimizedKPSetRoleInstancesItem*>* getInitializingRoleInstancesItemList();
					QList<COptimizedKPSetIndividualItem*>* getProcessingPossibleSameIndividualsItemList();

					QList<COptimizedKPSetIndividualItem*>* getInitializingRoleInstancesIndividualItemList();


					cint64 getTestedPossibleConceptInstancesCount();
					cint64 getOpenPossibleConceptInstancesCount();

					COptimizedKPSetOntologyConceptRealizingItem* incTestedPossibleConceptInstancesCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* incOpenPossibleConceptInstancesCount(cint64 incCount = 1);

					COptimizedKPSetOntologyConceptRealizingItem* decTestedPossibleConceptInstancesCount(cint64 decCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decOpenPossibleConceptInstancesCount(cint64 decCount = 1);


					cint64 getTestedPossibleSameIndividualsCount();
					cint64 getOpenPossibleSameIndividualsCount();

					COptimizedKPSetOntologyConceptRealizingItem* incTestedPossibleSameIndividualsCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* incOpenPossibleSameIndividualsCount(cint64 incCount = 1);

					COptimizedKPSetOntologyConceptRealizingItem* decTestedPossibleSameIndividualsCount(cint64 decCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decOpenPossibleSameIndividualsCount(cint64 decCount = 1);


					virtual bool visitDirectInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitDirectTypes(CConceptInstanceItem* item, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool visitTypes(CConceptInstanceItem* item, CConceptRealizationInstantiatedVisitor* visitor);

					virtual bool visitIndividuals(CConceptInstanceItem* item, CConceptRealizationIndividualVisitor* visitor);
					virtual bool visitConcepts(CConceptInstantiatedItem* item, CConceptRealizationConceptVisitor* visitor);

					virtual CConceptInstantiatedItem* getInstantiatedItem(CConcept* concept);
					virtual CConceptInstanceItem* getInstanceItem(CIndividual* individual);


					virtual bool visitSameIndividuals(CSameInstanceItem* item, CSameRealizationIndividualVisitor* visitor);

					virtual CSameInstanceItem* getSameInstanceItem(CIndividual* individual);



					virtual bool visitTargetIndividuals(CRoleInstanceItem* individualItem, CRoleInstantiatedItem* roleItem, CRoleRealizationInstanceVisitor* visitor);

					virtual bool visitIndividuals(CRoleInstanceItem* item, CRoleRealizationIndividualVisitor* visitor);

					virtual CRoleInstantiatedItem* getRoleInstantiatedItem(CRole* role);
					virtual CRoleInstanceItem* getRoleInstanceItem(CIndividual* individual);



					COptimizedKPSetConceptInstancesItem* getTopInstancesItem();
					COptimizedKPSetConceptInstancesItem* getBottomInstancesItem();

					cint64 getTestingPossibleSameIndividualCount();
					cint64 getTestingPossibleConceptInstanceCount();

					bool hasTestingPossibleSameIndividual();
					bool hasTestingPossibleConceptInstances();

					COptimizedKPSetOntologyConceptRealizingItem* setTestingPossibleSameIndividualCount(cint64 testCount);
					COptimizedKPSetOntologyConceptRealizingItem* setTestingPossibleConceptInstanceCount(cint64 testCount);

					COptimizedKPSetOntologyConceptRealizingItem* incTestingPossibleSameIndividualCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decTestingPossibleSameIndividualCount(cint64 decCount = 1);
					
					COptimizedKPSetOntologyConceptRealizingItem* incTestingPossibleConceptInstanceCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decTestingPossibleConceptInstanceCount(cint64 decCount = 1);






					COptimizedKPSetRoleInstancesItem* getTopRoleInstancesItem();
					COptimizedKPSetRoleInstancesItem* getBottomRoleInstancesItem();

					bool hasTestingPossibleRoleInstances();
					cint64 getTestingPossibleRoleInstanceCount();
					cint64 getTestedPossibleRoleInstancesCount();
					cint64 getInitializedRoleInstancesCount();
					cint64 getOpenPossibleRoleInstancesCount();
					cint64 getRemaningInitalizingRoleInstancesCount();

					COptimizedKPSetOntologyConceptRealizingItem* setTestingPossibleRoleInstanceCount(cint64 testCount);
					COptimizedKPSetOntologyConceptRealizingItem* incTestingPossibleRoleInstanceCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decTestingPossibleRoleInstanceCount(cint64 decCount = 1);

					COptimizedKPSetOntologyConceptRealizingItem* incTestedPossibleRoleInstancesCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decTestedPossibleRoleInstancesCount(cint64 decCount = 1);

					COptimizedKPSetOntologyConceptRealizingItem* incOpenPossibleRoleInstancesCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decOpenPossibleRoleInstancesCount(cint64 decCount = 1);

					COptimizedKPSetOntologyConceptRealizingItem* incInitializedRoleInstancesCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decInitializedRoleInstancesCount(cint64 decCount = 1);

					COptimizedKPSetOntologyConceptRealizingItem* incRemaningInitalizingInstancesCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decRemaningInitalizingInstancesCount(cint64 decCount = 1);


					bool hasTestingRoleInstanceCandidates();
					cint64 getTestingRoleInstanceCandidatesCount();
					COptimizedKPSetOntologyConceptRealizingItem* setTestingRoleInstanceCandidatesCount(cint64 testCount);
					COptimizedKPSetOntologyConceptRealizingItem* incTestingRoleInstanceCandidatesCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decTestingRoleInstanceCandidatesCount(cint64 decCount = 1);


					bool isConceptRealizationInstalled();
					COptimizedKPSetOntologyConceptRealizingItem* setConceptRealizationInstalled(bool installed);

					bool isRoleRealizationInstalled();
					COptimizedKPSetOntologyConceptRealizingItem* setRoleRealizationInstalled(bool installed);


					bool isSameRealizationInstalled();
					COptimizedKPSetOntologyConceptRealizingItem* setSameRealizationInstalled(bool installed);

					bool isCountingPossibleConceptInstancesTesting();
					bool isCountingPossibleRoleInstancesTesting();
					bool isCountingPossibleSameInstancesTesting();

					COptimizedKPSetOntologyConceptRealizingItem* setCountingPossibleConceptInstancesTesting(bool counting);
					COptimizedKPSetOntologyConceptRealizingItem* setCountingPossibleRoleInstancesTesting(bool counting);
					COptimizedKPSetOntologyConceptRealizingItem* setCountingPossibleSameInstancesTesting(bool counting);



					COptimizedKPSetOntologyConceptRealizingItem* setTemporarySameRealizationOntology(CConcreteOntology* tmpOntology);
					CConcreteOntology* getTemporarySameRealizationOntology();


					QList<COptimizedKPSetIndividualItem*>* getPossibleSameIndividualsItemList();
					COptimizedKPSetOntologyConceptRealizingItem* addPossibleSameIndividualsItem(COptimizedKPSetIndividualItem* indiItem);

					virtual CPossibleAssertionsCollectionSet* getPossibleAssertionCollectionSet();



					bool requiresIndividualDependenceTracking();
					COptimizedKPSetOntologyConceptRealizingItem* setIndividualDependenceTrackingRequired(bool indiDepTrackingRequired);

					QList<COptimizedKPSetIndividualItem*>* getInstantiatedItemList();

					QTime* getInitializationTime();

				// protected methods
				protected:

				// protected variables
				protected:
					CRealizingTestingStep* mRealizeConceptProcessingStep;
					CRealizingTestingStep* mRealizeRoleProcessingStep;
					CRealizingTestingStep* mRealizeSameIndividualsProcessingStep;
					CRealization* mRealization;

					QList<CRealizingTestingStep*> mProcessingSteps;

					CCalculationConfigurationExtension* mCalculationConfig;

					bool mItemsInitialized;
					bool mRealizationSameIndividualsInitialized;
					bool mRealizationConceptsInitialized;
					bool mRealizationRolesInitialized;

					QHash<CHierarchyNode*,COptimizedKPSetConceptInstancesItem*> mHierNodeInstancesItemHash;
					QHash<CConcept*,COptimizedKPSetConceptInstancesItem*> mConceptInstancesItemHash;
					QList<COptimizedKPSetConceptInstancesItem*> mConceptInstancesItemContainer;

					QHash<CRole*,COptimizedKPSetRoleInstancesItem*> mRoleInstancesItemHash;
					QHash<CRole*,COptimizedKPSetRoleInstancesRedirectionItem*> mRedirectedRoleInstancesItemHash;
					QList<COptimizedKPSetRoleInstancesItem*> mRoleInstancesItemContainer;
					QList<COptimizedKPSetRoleInstancesItem*> mComplexRoleInstancesItemContainer;

					QHash<CConcept*,COptimizedKPSetRoleInstancesItem*> mMarkerConceptInstancesItemHash;

					QHash<CIndividual*,COptimizedKPSetIndividualItem*> mIndividualInstantiatedItemHash;
					QList<COptimizedKPSetIndividualItem*> mInstantiatedItemContainer;


					QList<COptimizedKPSetConceptInstancesItem*> mProcessingConceptInstancesItemList;
					QList<COptimizedKPSetIndividualItem*> mProcessingSameIndividualsItemList;

					cint64 mTestedPossibleConceptInstancesCount;
					cint64 mOpenPossibleConceptInstancesCount;

					COptimizedKPSetConceptInstancesItem* mTopConceptInstancesItem;
					COptimizedKPSetConceptInstancesItem* mBottomConceptInstancesItem;

					cint64 mTestingPossibleSameIndividualCount;
					cint64 mTestingPossibleConceptInstanceCount;
					cint64 mTestingPossibleRoleInstanceCount;
					cint64 mTestingRoleInstanceCandidateCount;



					CConcreteOntology* mTempRoleRealizationOntology;
					CConcreteOntology* mTempSameRealizationOntology;


					QList<COptimizedKPSetIndividualItem*> mRoleCandidateInitalizingIndividualsItemList;
					QList<COptimizedKPSetRoleInstancesItem*> mInitializingRoleInstancesItemList;
					QList<COptimizedKPSetRoleInstancesItem*> mProcessingRoleInstancesItemList;

					QList<COptimizedKPSetIndividualItem*> mInitializingRoleInstancesIndividualItemList;

					COptimizedKPSetRoleInstancesItem* mTopRoleInstancesItem;
					COptimizedKPSetRoleInstancesItem* mBottomRoleInstancesItem;

					cint64 mTestedPossibleRoleInstancesCount;
					cint64 mInitializedRoleInstancesCount;
					cint64 mOpenPossibleRoleInstancesCount;
					cint64 mRemainingInitalizingRoleInstancesCount;


					bool mConceptRealizationInstalled;
					bool mRoleRealizationInstalled;
					bool mSameRealizationInstalled;


					bool mCountingPossibleConceptInstancesTesting;
					bool mCountingPossibleRoleInstancesTesting;
					bool mCountingPossibleSameInstancesTesting;

					QList<COptimizedKPSetIndividualItem*> mPossibleSameIndiItemList;

					cint64 mTestedPossibleSameIndividualsCount;
					cint64 mOpenPossibleSameIndividualsCount;

					CPossibleAssertionsCollectionSet* mPossAssCollSet;
					bool mIndiDepTrackReq;



					bool mExtraConsistencyTestingRequired;
					bool mExtraConsistencyTesting;
					bool mExtraConsistencyTested;

					QTime mInitTime;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGITEM_H
