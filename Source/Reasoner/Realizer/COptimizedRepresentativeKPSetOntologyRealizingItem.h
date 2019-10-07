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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETONTOLOGYREALIZINGITEM_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETONTOLOGYREALIZINGITEM_H

// Libraries includes
#include <QAtomicPointer>

// Namespace includes
#include "RealizerSettings.h"
#include "CRealizingTestingItem.h"
#include "COntologyRealizingItem.h"
#include "CRealizingTestingStep.h"
#include "COptimizedKPSetRoleInstancesItem.h"
#include "COptimizedKPSetRoleInstancesRedirectionItem.h"
#include "COptimizedKPSetIndividualItem.h"
#include "COntologyRealizingDynamicRequirmentProcessingData.h"
#include "CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem.h"
#include "CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem.h"
#include "CRealizationEntailmentQueuedIndividualsSameInstanceTestingItem.h"
#include "COptimizedRepresentativeKPSetConceptSetCacheLabelItemData.h"
#include "COptimizedRepresentativeKPSetCacheLabelItemIterator.h"
#include "COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData.h"
#include "COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData.h"
#include "COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData.h"
#include "COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator.h"
#include "COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator.h"
#include "COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData.h"
#include "COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator.h"
#include "COptimizedRepresentativeKPSetIntersectionCombinationIterator.h"
#include "COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator.h"
#include "COntologyRealizingDynamicIteratorRequirmentDataCallback.h"
#include "COptimizedRepresentativeKPSetIndividualCandiatesIterator.h"
#include "COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheReader.h"

#include "Reasoner/Ontology/COntologyProcessingStepRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingConceptRealizationRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingRoleRealizationRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingSameRealizationRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingIteratorRealizationRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingInitializedRealizationRequirement.h"

#include "Reasoner/Realization/CRoleRealizationInstanceVisitor.h"
#include "Reasoner/Realization/CRoleRealizationIndividualVisitor.h"
#include "Reasoner/Realization/CRoleRealization.h"
#include "Reasoner/Realization/CConceptRealization.h"
#include "Reasoner/Realization/CSameRealization.h"
#include "Reasoner/Realization/CPossibleAssertionsCollectionSet.h"
#include "Reasoner/Realization/CRealizationIndividualInstanceItemReference.h"
#include "Reasoner/Realization/CRealizationIndividualSorting.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Kernel::Cache;
		using namespace Ontology;
		using namespace Realization;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedRepresentativeKPSetOntologyRealizingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetOntologyRealizingItem : public COntologyRealizingItem, public CRoleRealization, public CConceptRealization, public CSameRealization {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetOntologyRealizingItem();
					~COptimizedRepresentativeKPSetOntologyRealizingItem();

					virtual COptimizedRepresentativeKPSetOntologyRealizingItem* initRequirementConfigRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader);
					
					COptimizedRepresentativeKPSetOntologyRealizingItem* initConceptItemsFromHierarchy();
					COptimizedRepresentativeKPSetOntologyRealizingItem* initRoleItemsFromHierarchy();
					COptimizedRepresentativeKPSetOntologyRealizingItem* initRoleCandidateInitializingIndividualItems();

					COptimizedRepresentativeKPSetOntologyRealizingItem* initComplexRolesStarterCandidates();

					bool hasItemsInitialized();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setItemsInitialized(bool initialized);

					bool hasRealizationSameIndividualsInitialized();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setRealizationSameIndividualsInitialized(bool initialized);

					bool hasRealizationConceptsInitialized();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setRealizationConceptsInitialized(bool initialized);

					bool hasRealizationRolesInitialized();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setRealizationRolesInitialized(bool initialized);

					COptimizedRepresentativeKPSetOntologyRealizingItem* setTemporaryRoleRealizationOntology(CConcreteOntology* tmpOntology);
					CConcreteOntology* getTemporaryRoleRealizationOntology();

					virtual COntologyRealizingItem* addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement, COntologyRealizingDynamicRequirmentProcessingData* procData);
					virtual COntologyRealizingItem* logRequirementProcessingStartStatistics(COntologyRealizingDynamicRequirmentProcessingStatistics* procStats);
					virtual COntologyRealizingItem* logRequirementProcessingFinishStatistics(COntologyRealizingDynamicRequirmentProcessingStatistics* procStats);

					CCalculationConfigurationExtension* getCalculationConfiguration();



					bool handleIteratorRequirement(COntologyRealizingDynamicIteratorRequirmentDataCallback* iteratorReqDataCallback, COntologyRealizingDynamicRequirmentProcessingData* newReqProcDat, COntologyRealizingDynamicRequirmentProcessingData* procData);
					bool ensureIteratorPositionRealization(COntologyRealizingDynamicIteratorRequirmentDataCallback* iteratorReqDataCallback, COntologyRealizingDynamicRequirmentProcessingData* newReqProcDat, CRealizationIndividualInstanceItemReferenceIterator* iterator, const QList<CRealizationIndividualInstanceItemReferenceIterator*>& initializationCheckIteratorList);


					bool queueSameInstanceRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, const CIndividualReference& indi1, const CIndividualReference& indi2);
					bool queueConceptInstanceRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, const CIndividualReference& indi, CConcept* concept, bool subconceptRealizationRequired = true);
					bool queueRoleInstanceRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, const CIndividualReference& indiSource, const CIndividualReference& indiDestination, CRole* role, bool roleInversed, bool subRoleRealization = true);
					bool queueRoleFillerInstanceRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, const CIndividualReference& indiSource, CRole* role, bool roleInversed, bool subRoleRealization = true);

					COptimizedRepresentativeKPSetOntologyRealizingItem* markIntanceItemForConceptRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, CConcept* concept, bool subconceptRealizationRequired = true);
					COptimizedRepresentativeKPSetOntologyRealizingItem* addConceptInstanceItemToProcessPossibleInstances(COptimizedKPSetConceptInstancesItem* instanceItem);

					COptimizedRepresentativeKPSetOntologyRealizingItem* markInstantiatedItemForSameIndividualsRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, const CIndividualReference&  individual);
					COptimizedRepresentativeKPSetOntologyRealizingItem* addInstantiatedItemToProcessPossibleSameIndividuals(COptimizedKPSetIndividualItem* instantiatedItem);

					COptimizedRepresentativeKPSetOntologyRealizingItem* markIntanceItemForRoleRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, CRole* role, bool subroleRealizationRequired = true);
					COptimizedRepresentativeKPSetOntologyRealizingItem* addRoleInstanceItemToProcessPossibleInstances(COptimizedKPSetRoleInstancesItem* instanceItem);
					COptimizedRepresentativeKPSetOntologyRealizingItem* addRoleInstanceItemToInitializeCandidates(COptimizedKPSetRoleInstancesItem* instanceItem);
					COptimizedRepresentativeKPSetOntologyRealizingItem* addRoleInstanceIndividualItemToInitializeCandidates(COptimizedKPSetIndividualItem* indiItem);
					COptimizedRepresentativeKPSetOntologyRealizingItem* addRoleInstanceIndividualItemToInitializeCandidates(const CRealizationIndividualInstanceItemReference& indiRealItemRef);
					COptimizedRepresentativeKPSetOntologyRealizingItem* addRoleInstanceItemToProcessComplexCandidateInstances(COptimizedKPSetRoleInstancesItem* instanceItem);

					COptimizedRepresentativeKPSetOntologyRealizingItem* addRoleSuccessorInstanceIndividualItemPairToInitializeCandidates(const CRealizationIndividualInstanceItemReference& indiRealItemRef, COptimizedKPSetRoleInstancesItem* instanceItem);
					COptimizedRepresentativeKPSetOntologyRealizingItem* addRolePredecessorInstanceIndividualItemPairToInitializeCandidates(const CRealizationIndividualInstanceItemReference& indiRealItemRef, COptimizedKPSetRoleInstancesItem* instanceItem);


					QHash<CConcept*,COptimizedKPSetRoleInstancesItem*>* getMarkerConceptInstancesItemHash();



					bool isExtraConsistencyTestingStepRequired();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setExtraConsistencyTestingStepRequired(bool required);
					bool isExtraConsistencyTesting();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setExtraConsistencyTesting(bool testing);
					bool isExtraConsistencyTested();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setExtraConsistencyTested(bool tested);


					CRealizingTestingStep* getInitRealizeProcessingStep();
					bool isInitRealizeStepFinished();
					bool isInitRealizeStepRequired();
					bool areInitRealizeStepProcessingRequirementSatisfied();


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


					COptimizedRepresentativeKPSetOntologyRealizingItem* setRealization(CRealization* realization);
					CRealization* getRealization();



					bool hasRemainingCandidateConfirmationRoleInstanceItems();




					bool hasRemainingInitializingRoleInstanceItems();
					bool hasRemainingProcessingRoleInstanceItems();
					bool hasRemainingInitializingRoleInstanceIndividualItemReferences();
					bool hasRemainingInitializingRoleSuccessorInstanceIndividualItemReferencePairs();
					bool hasRemainingInitializingRolePredeccessorInstanceIndividualItemReferencePairs();

					bool hasRemainingProcessingConceptInstanceItems();
					bool hasRemainingProcessingConceptInstanceIndividualItems();
					bool hasRemainingProcessingSameIndividualsItems();


					COptimizedRepresentativeKPSetOntologyRealizingItem* addRoleCandidateInitalizingIndividualsItem(COptimizedKPSetIndividualItem* indiItem);
					QList<COptimizedKPSetIndividualItem*>* getRoleCandidateInitalizingIndividualsItemList();


					QHash<CHierarchyNode*,COptimizedKPSetConceptInstancesItem*>* getHierarchyNodeInstancesItemHash();
					QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* getConceptInstancesItemHash();

					QHash<CRole*,COptimizedKPSetRoleInstancesItem*>* getRoleInstancesItemHash();
					QList<COptimizedKPSetRoleInstancesItem*>* getRoleInstancesItemList();
					QList<COptimizedKPSetRoleInstancesItem*>* getComplexRoleInstancesItemList();

					QHash<cint64,COptimizedKPSetIndividualItem*>* getIndividualInstantiatedItemHash();

					COptimizedKPSetIndividualItem* getIndividualInstantiatedItem(CIndividual* individual, bool directCreate = true);
					bool hasIndividualInstantiatedItem(CIndividual* individual);

					COptimizedKPSetIndividualItem* getIndividualInstantiatedItem(cint64 indiId, bool directCreate = true, CIndividual* individual = nullptr);
					bool hasIndividualInstantiatedItem(cint64 indiId);


					QList<COptimizedKPSetConceptInstancesItem*>* getProcessingPossibleConceptInstancesItemList();

					COptimizedRepresentativeKPSetOntologyRealizingItem* addPossibleConceptInstancesIndividualItem(COptimizedKPSetIndividualItem* indiItem);
					QList<COptimizedKPSetIndividualItem*>* getPossibleConceptInstancesIndividualItemList();


					QList<COptimizedKPSetRoleInstancesItem*>* getProcessingPossibleRoleInstancesItemList();
					QList<COptimizedKPSetRoleInstancesItem*>* getInitializingRoleInstancesItemList();
					QList<COptimizedKPSetIndividualItem*>* getProcessingPossibleSameIndividualsItemList();
					QList<COptimizedKPSetRoleInstancesItem*>* getCandidateConfirmationRoleInstancesItemList();


					QList<CRealizationIndividualInstanceItemReference>* getInitializingRoleInstancesIndividualItemReferenceList();
					QList<QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*>>* getInitializingRoleSuccessorInstancesIndividualItemReferencePairList();
					QList<QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*>>* getInitializingRolePredecessorInstancesIndividualItemReferencePairList();


					cint64 getTestedPossibleConceptInstancesCount();
					cint64 getOpenPossibleConceptInstancesCount();

					COptimizedRepresentativeKPSetOntologyRealizingItem* incTestedPossibleConceptInstancesCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* incOpenPossibleConceptInstancesCount(cint64 incCount = 1);

					COptimizedRepresentativeKPSetOntologyRealizingItem* decTestedPossibleConceptInstancesCount(cint64 decCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decOpenPossibleConceptInstancesCount(cint64 decCount = 1);


					cint64 getTestedPossibleSameIndividualsCount();
					cint64 getOpenPossibleSameIndividualsCount();

					COptimizedRepresentativeKPSetOntologyRealizingItem* incTestedPossibleSameIndividualsCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* incOpenPossibleSameIndividualsCount(cint64 incCount = 1);

					COptimizedRepresentativeKPSetOntologyRealizingItem* decTestedPossibleSameIndividualsCount(cint64 decCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decOpenPossibleSameIndividualsCount(cint64 decCount = 1);



					COptimizedRepresentativeKPSetOntologyRealizingItem* incProvidedPossibleInstancesMergingIndividualsCount(cint64 incCount = 1);
					cint64 getProvidedPossibleInstancesMergingIndividualsCount();

					COptimizedRepresentativeKPSetOntologyRealizingItem* incProvidedPossibleInstancesMergingsCount(cint64 incCount = 1);
					cint64 getProvidedPossibleInstancesMergingsCount();

					COptimizedRepresentativeKPSetOntologyRealizingItem* incAttemptedPossibleInstancesMergingsCount(cint64 incCount = 1);
					cint64 getAttemptedPossibleInstancesMergingsCount();

					COptimizedRepresentativeKPSetOntologyRealizingItem* incSucceddedPossibleInstancesMergingsCount(cint64 incCount = 1);
					cint64 getSucceddedPossibleInstancesMergingsCount();


					COptimizedRepresentativeKPSetOntologyRealizingItem* incModelMergingsTriedCount(cint64 incCount = 1);
					cint64 getModelMergingsTriedCount();

					COptimizedRepresentativeKPSetOntologyRealizingItem* incModelMergingsSucessCount(cint64 incCount = 1);
					cint64 getModelMergingsSucessCount();

					COptimizedRepresentativeKPSetOntologyRealizingItem* incModelMergingsInstanceFoundCount(cint64 incCount = 1);
					cint64 getModelMergingsInstanceFoundCount();

					COptimizedRepresentativeKPSetOntologyRealizingItem* incModelMergingsNonInstanceFoundCount(cint64 incCount = 1);
					cint64 getModelMergingsNonInstanceFoundCount();



					virtual COptimizedKPSetIndividualItem* getInstanceItem(const CIndividualReference& indiRef, bool correctSameIndividualsMerging);


					virtual QString getIndividualName(const CIndividualReference& indiRef, bool abbreviated);
					virtual CRealizationIndividualInstanceItemReference getInstanceItemReference(const CIndividualReference& indiRef);
					virtual CRealizationIndividualInstanceItemReference getInstanceItemReference(const CIndividualReference& indiRef, bool correctSameIndividualsMerging);
					virtual CRealizationIndividualInstanceItemReference getInstanceItemReference(cint64 indiId, bool correctSameIndividualsMerging);

					virtual bool isConceptInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConcept* concept);





					virtual bool visitDirectTypes(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool visitAllTypes(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealizationInstantiatedVisitor* visitor);



					virtual bool visitTypes(const CRealizationIndividualInstanceItemReference& indiRealItemRef, function<bool(COptimizedKPSetConceptInstancesItem* conceptInstancesItem)> visitFunc, bool visitDirectTypesOnly);


					virtual bool visitDirectInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitAllInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitInstances(CConceptInstantiatedItem* item, function<bool(const CRealizationIndividualInstanceItemReference& indiInstItemRef)> visitFunc, bool visitDirectInstancesOnly, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference());


					virtual CSameInstanceItem* getSameInstanceItem(const CRealizationIndividualInstanceItemReference& indiRealItemRef);




					virtual bool visitIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealizationIndividualVisitor* visitor);
					virtual bool visitConcepts(CConceptInstantiatedItem* item, CConceptRealizationConceptVisitor* visitor);

					virtual CConceptInstantiatedItem* getInstantiatedItem(CConcept* concept);


					virtual bool visitSameIndividuals(CSameInstanceItem* item, CSameRealizationIndividualVisitor* visitor);
					virtual bool visitSameIndividuals(const CRealizationIndividualInstanceItemReference& itemRef, CSameRealizationIndividualVisitor* visitor);

					virtual CRealizationIndividualInstanceItemReference getSameInstanceItemReference(const CIndividualReference& indiRef);

					virtual bool hasPotentiallySameIndividuals();
					virtual COptimizedRepresentativeKPSetOntologyRealizingItem* setPotentiallySameIndividuals(bool potentiallySameIndividuals);


					bool visitAllIndividuals(function<bool(const CRealizationIndividualInstanceItemReference& indiInstItemRef)> visitFunc, bool visitSameIndividualMerged);

					bool isSameIndividualsMerged(const CRealizationIndividualInstanceItemReference& indiInstItemRef);


					virtual bool getPreferredRolePropagationDirection(CRole* role, bool& inversedPreferred);


					virtual CRealizationIndividualInstanceItemReferenceIterator* getConceptInstancesIterator(CConcept* concept, bool onlyDirect, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true);
					virtual CRealizationIndividualInstanceItemReferenceIterator* getRoleNeighbouringInstancesIterator(CRole* role, bool inversed, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true);
					virtual CRealizationIndividualInstanceItemReferenceIterator* getRoleFillerInstancesIterator(const CRealizationIndividualInstanceItemReference& indiInstItemRef, CRole* role, bool inversed, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true);
					virtual CRealizationIndividualInstanceItemReferenceIterator* getRoleFillerInstancesIterator(const CRealizationIndividualInstanceItemReference& indiInstItemRef, CRoleInstantiatedItem* roleInstancesItem, bool inversed, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true);
					virtual CRealizationIndividualInstanceItemReferenceIterator* getIterationIntersectionIterator(QList<CRealizationIndividualInstanceItemReferenceIterator*> takeIteratorList);
					virtual CRealizationIndividualInstanceItemReferenceIterator* getIterationCandidateIndividualIdIterator(const QMap<cint64, IndividualIdCandidateData>& candidateIndiIdsMap, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true);


					virtual CRealizationIndividualInstanceItemReferenceIterator* getRoleExistentialInstancesIterator(CRole* role, bool inversed, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true);
					virtual CRealizationIndividualInstanceItemReferenceIterator* getRoleDataInstancesIterator(CRole* role, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true);
					virtual bool hasOnlyDeterministicRoleDataInstances(CRole* role);
					virtual bool hasExistentiallyLinkedRoleInstances(CRole* role, bool inversed);




					virtual bool isRoleInstance(CRole* role, const CRealizationIndividualInstanceItemReference& sourceIndiRealItemRef, const CRealizationIndividualInstanceItemReference& targetIndiRealItemRef);
					virtual bool visitTargetIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleInstantiatedItem* roleItem, CRoleRealizationInstanceVisitor* visitor);
					virtual bool visitSourceIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleInstantiatedItem* roleItem, CRoleRealizationInstanceVisitor* visitor);
					virtual bool visitSourceIndividualRoles(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealizationInstantiatedVisitor* visitor);
					virtual bool visitTargetIndividualRoles(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealizationInstantiatedVisitor* visitor);
					virtual bool visitBetweeenIndividualRoles(const CRealizationIndividualInstanceItemReference& sourceIndiRealItemRef, const CRealizationIndividualInstanceItemReference& targetIndiRealItemRef, CRoleRealizationInstantiatedVisitor* visitor);


					bool visitSourceTargetIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleInstantiatedItem* roleItem, bool target, CRoleRealizationInstanceVisitor* visitor);
					bool visitSourceTargetIndividualRoles(const CRealizationIndividualInstanceItemReference& indiRealItemRef, bool target, CRoleRealizationInstantiatedVisitor* visitor);

					virtual CRealizationIndividualInstanceItemReference getRoleInstanceItemReference(const CIndividualReference& indiRef);



					virtual bool visitIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealizationIndividualVisitor* visitor);


					virtual bool visitRoles(CRoleInstantiatedItem* item, CRoleRealizationRoleVisitor* visitor);

					virtual CRoleInstantiatedItem* getRoleInstantiatedItem(CRole* role);
					virtual CRoleInstanceItem* getRoleInstanceItem(CIndividual* individual);
					virtual CSameInstanceItem* getSameInstanceItem(CRoleInstanceItem* instanceItem);

					virtual CRoleInstanceItem* getRoleInstanceItem(CSameInstanceItem* instanceItem);
					virtual CRealizationIndividualInstanceItemReference getRoleInstanceItemReference(CSameInstanceItem* instanceItem);


					COptimizedKPSetConceptInstancesItem* getTopInstancesItem();
					COptimizedKPSetConceptInstancesItem* getBottomInstancesItem();

					cint64 getTestingPossibleSameIndividualCount();
					cint64 getTestingPossibleConceptInstanceCount();

					bool hasTestingPossibleSameIndividual();
					bool hasTestingPossibleConceptInstances();

					COptimizedRepresentativeKPSetOntologyRealizingItem* setTestingPossibleSameIndividualCount(cint64 testCount);
					COptimizedRepresentativeKPSetOntologyRealizingItem* setTestingPossibleConceptInstanceCount(cint64 testCount);

					COptimizedRepresentativeKPSetOntologyRealizingItem* incTestingPossibleSameIndividualCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decTestingPossibleSameIndividualCount(cint64 decCount = 1);
					
					COptimizedRepresentativeKPSetOntologyRealizingItem* incTestingPossibleConceptInstanceCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decTestingPossibleConceptInstanceCount(cint64 decCount = 1);






					COptimizedKPSetRoleInstancesItem* getTopRoleInstancesItem();
					COptimizedKPSetRoleInstancesItem* getBottomRoleInstancesItem();

					bool hasTestingPossibleRoleInstances();
					cint64 getTestingPossibleRoleInstanceCount();
					cint64 getTestedPossibleRoleInstancesCount();
					cint64 getInitializedRoleInstancesCount();
					cint64 getOpenPossibleRoleInstancesCount();
					cint64 getRemaningInitalizingRoleInstancesCount();

					COptimizedRepresentativeKPSetOntologyRealizingItem* setTestingPossibleRoleInstanceCount(cint64 testCount);
					COptimizedRepresentativeKPSetOntologyRealizingItem* incTestingPossibleRoleInstanceCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decTestingPossibleRoleInstanceCount(cint64 decCount = 1);

					COptimizedRepresentativeKPSetOntologyRealizingItem* incTestedPossibleRoleInstancesCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decTestedPossibleRoleInstancesCount(cint64 decCount = 1);

					COptimizedRepresentativeKPSetOntologyRealizingItem* incOpenPossibleRoleInstancesCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decOpenPossibleRoleInstancesCount(cint64 decCount = 1);

					COptimizedRepresentativeKPSetOntologyRealizingItem* incInitializedRoleInstancesCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decInitializedRoleInstancesCount(cint64 decCount = 1);

					COptimizedRepresentativeKPSetOntologyRealizingItem* incRemaningInitalizingInstancesCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decRemaningInitalizingInstancesCount(cint64 decCount = 1);


					bool hasPropagatingRoleInstanceCandidates();
					cint64 getPropagatingRoleInstanceCandidatesCount();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setPropagatingRoleInstanceCandidatesCount(cint64 testCount);
					COptimizedRepresentativeKPSetOntologyRealizingItem* incPropagatingRoleInstanceCandidatesCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decPropagatingRoleInstanceCandidatesCount(cint64 decCount = 1);




					bool hasTestingRoleInstanceCandidates();
					cint64 getTestingRoleInstanceCandidatesCount();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setTestingRoleInstanceCandidatesCount(cint64 testCount);
					COptimizedRepresentativeKPSetOntologyRealizingItem* incTestingRoleInstanceCandidatesCount(cint64 incCount = 1);
					COptimizedRepresentativeKPSetOntologyRealizingItem* decTestingRoleInstanceCandidatesCount(cint64 decCount = 1);





					bool isConceptRealizationInstalled();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setConceptRealizationInstalled(bool installed);

					bool isRoleRealizationInstalled();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setRoleRealizationInstalled(bool installed);


					bool isSameRealizationInstalled();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setSameRealizationInstalled(bool installed);

					bool isCountingPossibleConceptInstancesTesting();
					bool isCountingPossibleRoleInstancesTesting();
					bool isCountingPossibleSameInstancesTesting();

					COptimizedRepresentativeKPSetOntologyRealizingItem* setCountingPossibleConceptInstancesTesting(bool counting);
					COptimizedRepresentativeKPSetOntologyRealizingItem* setCountingPossibleRoleInstancesTesting(bool counting);
					COptimizedRepresentativeKPSetOntologyRealizingItem* setCountingPossibleSameInstancesTesting(bool counting);



					COptimizedRepresentativeKPSetOntologyRealizingItem* setTemporarySameRealizationOntology(CConcreteOntology* tmpOntology);
					CConcreteOntology* getTemporarySameRealizationOntology();


					QList<COptimizedKPSetIndividualItem*>* getPossibleSameIndividualsItemList();
					COptimizedRepresentativeKPSetOntologyRealizingItem* addPossibleSameIndividualsItem(COptimizedKPSetIndividualItem* indiItem);

					virtual CPossibleAssertionsCollectionSet* getPossibleAssertionCollectionSet();



					bool requiresIndividualDependenceTracking();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setIndividualDependenceTrackingRequired(bool indiDepTrackingRequired);

					QList<COptimizedKPSetIndividualItem*>* getInstantiatedItemList();

					QTime* getInitializationTime();


					bool isAllSameIndividualsProcessing();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setAllSameIndividualsProcessing(bool processing);

					bool isAllConceptInstancesProcessing();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setAllConceptInstancesProcessing(bool processing);
					
					bool isAllRoleInstancesProcessing();
					COptimizedRepresentativeKPSetOntologyRealizingItem* setAllRoleInstancesProcessing(bool processing);

					COptimizedRepresentativeKPSetOntologyRealizingItem* addEntailmentIndividualConceptInstanceTestingItem(CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem* queueItem);
					QList<CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem*>* getEntailmentIndividualConceptInstanceTestingItemList();
					bool hasEntailmentIndividualConceptInstanceTestingItems();



					COptimizedRepresentativeKPSetOntologyRealizingItem* addEntailmentIndividualsRoleInstanceTestingItem(CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem* queueItem);
					QList<CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem*>* getEntailmentIndividualsRoleInstanceTestingItemList();
					bool hasEntailmentIndividualsRoleInstanceTestingItems();



					COptimizedRepresentativeKPSetOntologyRealizingItem* addEntailmentIndividualsSameInstanceTestingItem(CRealizationEntailmentQueuedIndividualsSameInstanceTestingItem* queueItem);
					QList<CRealizationEntailmentQueuedIndividualsSameInstanceTestingItem*>* getEntailmentIndividualsSameInstanceTestingItemList();
					bool hasEntailmentIndividualsSameInstanceTestingItems();



					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemData*>* getRepresentativeCacheConceptSetLabelItemDataHash();
					COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* getRepresentativeCacheConceptSetLabelItemData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, bool createIfNotExists = true);



					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData*>* getRepresentativeCacheSingleNeighbourRoleSetLabelItemDataHash();
					COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData* getRepresentativeCacheSingleNeighbourRoleSetLabelItemData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, bool createIfNotExists = true);

					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData*>* getRepresentativeCacheCombinedNeighbourRoleSetLabelItemDataHash();
					COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* getRepresentativeCacheCombinedNeighbourRoleSetLabelItemData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, bool createIfNotExists = true);






					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData*>* getRepresentativeCacheCombinedExistentialRoleSetLabelItemDataHash();
					COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData* getRepresentativeCacheCombinedExistentialRoleSetLabelItemData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, bool createIfNotExists = true);


					QList<COptimizedKPSetRoleInstancesItem*> getComplexStarterCandidateRoleInstancesItems(CRole* role);
					QList<COptimizedKPSetRoleInstancesItem*> getInverseComplexStarterCandidateRoleInstancesItems(CRole* role);



					COptimizedRepresentativeKPSetOntologyRealizingItem* addDataRoleCacheLabelItem(CRole* role, CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem, bool deterministic);
					QHash<CRole*, QSet<CBackendRepresentativeMemoryLabelCacheItem*>* >* getDataRoleDeterministicCacheLabelItemSetHash();
					QHash<CRole*, QSet<CBackendRepresentativeMemoryLabelCacheItem*>* >* getDataRoleNondeterministicCacheLabelItemSetHash();
					QSet<CBackendRepresentativeMemoryLabelCacheItem*>* getDataRoleCacheLabelItemSet(CRole* role, bool deterministic);


					COptimizedRepresentativeKPSetOntologyRealizingItem* addDataRoleNondeterministicCacheLabelItems(CRole* role);

					CBackendRepresentativeMemoryCacheReader* getBackendAssociationCacheReader();

				// protected methods
				protected:

				// protected variables
				protected:
					CRealizingTestingStep* mInitRealizeProcessingStep;
					CRealizingTestingStep* mRealizeConceptProcessingStep;
					CRealizingTestingStep* mRealizeRoleProcessingStep;
					CRealizingTestingStep* mRealizeSameIndividualsProcessingStep;
					CRealization* mRealization;

					CBackendRepresentativeMemoryCacheReader* mBackendAssocCacheReader;
					bool mSameMergedIndisInCache;

					QList<CRealizingTestingStep*> mProcessingSteps;

					CCalculationConfigurationExtension* mCalculationConfig;

					bool mItemsInitialized;
					bool mRealizationSameIndividualsInitialized;
					bool mRealizationConceptsInitialized;
					bool mRealizationRolesInitialized;


					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemData*> mRepresentativeCacheConceptSetLabelItemDataHash;
					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData*> mRepresentativeCacheSingleNeighbourRoleSetLabelItemDataHash;
					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData*> mRepresentativeCacheCombinedNeighbourRoleSetLabelItemDataHash;
					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData*> mRepresentativeCacheCombinedExistentialRoleSetLabelItemDataHash;


					QHash<CHierarchyNode*,COptimizedKPSetConceptInstancesItem*> mHierNodeInstancesItemHash;
					QHash<CConcept*,COptimizedKPSetConceptInstancesItem*> mConceptInstancesItemHash;
					QList<COptimizedKPSetConceptInstancesItem*> mConceptInstancesItemContainer;

					QHash<CRole*,COptimizedKPSetRoleInstancesItem*> mRoleInstancesItemHash;
					QHash<CRole*,COptimizedKPSetRoleInstancesRedirectionItem*> mRedirectedRoleInstancesItemHash;


					QHash<CRole*, COptimizedKPSetRoleInstancesItem*> mComplexStarterCandidateRoleInstancesItemHash;
					QHash<CRole*, COptimizedKPSetRoleInstancesItem*> mInverseComplexStarterCandidateRoleInstancesItemHash;



					QHash<CRole*, QSet<CBackendRepresentativeMemoryLabelCacheItem*>* > mDataRoleDetCacheLabelItemSetHash;
					QHash<CRole*, QSet<CBackendRepresentativeMemoryLabelCacheItem*>* > mDataRoleNonDetCacheLabelItemSetHash;
					QSet<CRole*> mDataRoleNondeterministicCacheLabelsSet;




					QList<COptimizedKPSetRoleInstancesItem*> mRoleInstancesItemContainer;
					QList<COptimizedKPSetRoleInstancesItem*> mComplexRoleInstancesItemContainer;

					QHash<CConcept*,COptimizedKPSetRoleInstancesItem*> mMarkerConceptInstancesItemHash;

					QHash<cint64, COptimizedKPSetIndividualItem*> mIndividualInstantiatedItemHash;
					QList<COptimizedKPSetIndividualItem*> mInstantiatedItemContainer;


					QList<COptimizedKPSetConceptInstancesItem*> mProcessingConceptInstancesItemList;
					QList<COptimizedKPSetIndividualItem*> mProcessingPossibleConceptInstanceItemList;
					QList<COptimizedKPSetIndividualItem*> mProcessingSameIndividualsItemList;
					bool mPotentiallySameIndividuals;


					cint64 mTestedPossibleConceptInstancesCount;
					cint64 mOpenPossibleConceptInstancesCount;

					COptimizedKPSetConceptInstancesItem* mTopConceptInstancesItem;
					COptimizedKPSetConceptInstancesItem* mBottomConceptInstancesItem;

					cint64 mTestingPossibleSameIndividualCount;
					cint64 mTestingPossibleConceptInstanceCount;
					cint64 mTestingPossibleRoleInstanceCount;
					cint64 mPropagatingRoleInstanceCandidateCount;
					cint64 mTestingRoleInstanceCandidateCount;



					CConcreteOntology* mTempRoleRealizationOntology;
					CConcreteOntology* mTempSameRealizationOntology;


					QList<COptimizedKPSetIndividualItem*> mRoleCandidateInitalizingIndividualsItemList;
					QList<COptimizedKPSetRoleInstancesItem*> mInitializingRoleInstancesItemList;
					QList<COptimizedKPSetRoleInstancesItem*> mCandidateConfirmationRoleInstancesItemList;
					QList<COptimizedKPSetRoleInstancesItem*> mProcessingPossibleRoleInstancesItemList;

					QList<CRealizationIndividualInstanceItemReference> mInitializingRoleInstancesIndividualItemReferenceList;
					QList< QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*> > mInitializingRoleSuccessorInstancesIndividualItemReferencePairList;
					QList< QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*> > mInitializingRolePredecessorInstancesIndividualItemReferencePairList;

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

					bool mAllSameIndividualsProcessing;
					bool mAllConceptInstancesProcessing;
					bool mAllRoleInstancesProcessing;


					QList<CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem*> mEntIndConInstTestList;
					QList<CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem*> mEntIndsRoleInstTestList;
					QList<CRealizationEntailmentQueuedIndividualsSameInstanceTestingItem*> mEntIndsSameInstTestList;


					bool mIteratorNothingToDoWarning;

					cint64 mProvidedPossibleInstancesMergingIndividualsCount;
					cint64 mProvidedPossibleInstancesMergingsCount;
					cint64 mAttemptedPossibleInstancesMergingsCount;
					cint64 mSucceddedPossibleInstancesMergingsCount;

					cint64 mModelMergingsTriedCount;
					cint64 mModelMergingsSucessCount;
					cint64 mModelMergingsInstanceFoundCount;
					cint64 mModelMergingsNonInstanceFoundCount;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETONTOLOGYREALIZINGITEM_H
