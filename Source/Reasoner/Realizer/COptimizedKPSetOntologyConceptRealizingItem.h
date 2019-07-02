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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGITEM_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGITEM_H

// Libraries includes
#include <QAtomicPointer>

// Namespace includes
#include "RealizerSettings.h"
#include "CRealizingTestingItem.h"
#include "COntologyRealizingItem.h"
#include "CRealizingTestingStep.h"
#include "COptimizedKPSetConceptInstancesItem.h"


// Other includes
#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Ontology/COntologyProcessingStepRequirement.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetOntologyConceptRealizingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetOntologyConceptRealizingItem : public COntologyRealizingItem, public CConceptRealization {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetOntologyConceptRealizingItem();
					~COptimizedKPSetOntologyConceptRealizingItem();

					COptimizedKPSetOntologyConceptRealizingItem* initRequirementConfigRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config);
					
					COptimizedKPSetOntologyConceptRealizingItem* initItemsFromHierarchy();

					bool hasItemsInitialized();
					COptimizedKPSetOntologyConceptRealizingItem* setItemsInitialized(bool initialized);

					bool hasRealizationSameIndividualsInitialized();
					COptimizedKPSetOntologyConceptRealizingItem* setRealizationSameIndividualsInitialized(bool initialized);

					bool hasRealizationConceptsInitialized();
					COptimizedKPSetOntologyConceptRealizingItem* setRealizationConceptsInitialized(bool initialized);

					virtual COntologyRealizingItem* addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement);

					CCalculationConfigurationExtension* getCalculationConfiguration();
					COptimizedKPSetOntologyConceptRealizingItem* markIntanceItemForConceptRealization(CConcept* concept);
					COptimizedKPSetOntologyConceptRealizingItem* addInstanceItemToProcessingPossibleInstances(COptimizedKPSetConceptInstancesItem* instanceItem);

					COptimizedKPSetOntologyConceptRealizingItem* markInstantiatedItemForSameIndividualsRealization();
					COptimizedKPSetOntologyConceptRealizingItem* addInstantiatedItemToProcessingPossibleSameIndividuals(COptimizedKPSetConceptInstantiatedItem* instantiatedItem);


					CRealizingTestingStep* getRealizeConceptProcessingStep();
					bool isRealizeConceptStepFinished();
					bool isRealizeConceptStepRequired();
					bool areRealizeConceptStepProcessingRequirementSatisfied();

					CRealizingTestingStep* getRealizeSameIndividualsProcessingStep();
					bool isRealizeSameIndividualsStepFinished();
					bool isRealizeSameIndividualsStepRequired();
					bool areRealizeSameIndividualsStepProcessingRequirementSatisfied();


					bool areAllStepFinished();
					bool hasRemainingProcessingRequirements();


					COptimizedKPSetOntologyConceptRealizingItem* setRealization(CRealization* realization);
					CRealization* getRealization();


					bool hasRemainingProcessingInstanceItems();
					bool hasRemainingProcessingSameIndividualsItems();


					QHash<CHierarchyNode*,COptimizedKPSetConceptInstancesItem*>* getHierarchyNodeInstancesItemHash();
					QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* getConceptInstancesItemHash();

					QHash<CIndividual*,COptimizedKPSetConceptInstantiatedItem*>* getIndividualInstantiatedItemHash();

					COptimizedKPSetConceptInstantiatedItem* getIndividualInstantiatedItem(CIndividual* individual, bool directCreate = true);
					bool hasIndividualInstantiatedItem(CIndividual* individual);

					QList<COptimizedKPSetConceptInstancesItem*>* getProcessingPossibleInstancesItemList();
					QList<COptimizedKPSetConceptInstantiatedItem*>* getProcessingPossibleSameIndividualsItemList();


					cint64 getTestedPossibleInstancesCount();
					cint64 getOpenPossibleInstancesCount();

					COptimizedKPSetOntologyConceptRealizingItem* incTestedPossibleInstancesCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* incOpenPossibleInstancesCount(cint64 incCount = 1);

					COptimizedKPSetOntologyConceptRealizingItem* decTestedPossibleInstancesCount(cint64 decCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decOpenPossibleInstancesCount(cint64 decCount = 1);


					virtual bool visitDirectInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitDirectTypes(CConceptInstanceItem* item, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool visitTypes(CConceptInstanceItem* item, CConceptRealizationInstantiatedVisitor* visitor);

					virtual bool visitIndividuals(CConceptInstanceItem* item, CConceptRealizationIndividualVisitor* visitor);
					virtual bool visitConcepts(CConceptInstantiatedItem* item, CConceptRealizationConceptVisitor* visitor);

					virtual CConceptInstantiatedItem* getInstantiatedItem(CConcept* concept);
					virtual CConceptInstanceItem* getInstanceItem(CIndividual* individual);

					COptimizedKPSetConceptInstancesItem* getTopInstancesItem();
					COptimizedKPSetConceptInstancesItem* getBottomInstancesItem();

					cint64 getTestingPossibleSameIndividualCount();
					cint64 getTestingPossibleInstanceCount();

					bool hasTestingPossibleSameIndividual();
					bool hasTestingPossibleInstances();

					COptimizedKPSetOntologyConceptRealizingItem* setTestingPossibleSameIndividualCount(cint64 testCount);
					COptimizedKPSetOntologyConceptRealizingItem* setTestingPossibleInstanceCount(cint64 testCount);

					COptimizedKPSetOntologyConceptRealizingItem* incTestingPossibleSameIndividualCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decTestingPossibleSameIndividualCount(cint64 decCount = 1);
					
					COptimizedKPSetOntologyConceptRealizingItem* incTestingPossibleInstanceCount(cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingItem* decTestingPossibleInstanceCount(cint64 decCount = 1);

				// protected methods
				protected:

				// protected variables
				protected:
					CRealizingTestingStep* mRealizeConceptProcessingStep;
					CRealizingTestingStep* mRealizeSameIndividualsProcessingStep;
					CRealization* mRealization;

					QList<CRealizingTestingStep*> mProcessingSteps;

					CCalculationConfigurationExtension* mCalculationConfig;

					bool mItemsInitialized;
					bool mRealizationSameIndividualsInitialized;
					bool mRealizationConceptsInitialized;

					QHash<CHierarchyNode*,COptimizedKPSetConceptInstancesItem*> mHierNodeInstancesItemHash;
					QHash<CConcept*,COptimizedKPSetConceptInstancesItem*> mConceptInstancesItemHash;
					QList<COptimizedKPSetConceptInstancesItem*> mInstancesItemContainer;

					QHash<CIndividual*,COptimizedKPSetConceptInstantiatedItem*> mIndividualInstantiatedItemHash;
					QList<COptimizedKPSetConceptInstantiatedItem*> mInstantiatedItemContainer;


					QList<COptimizedKPSetConceptInstancesItem*> mProcessingInstancesItemList;
					QList<COptimizedKPSetConceptInstantiatedItem*> mProcessingSameIndividualsItemList;

					cint64 mTestedPossibleInstancesCount;
					cint64 mOpenPossibleInstancesCount;

					COptimizedKPSetConceptInstancesItem* mTopInstancesItem;
					COptimizedKPSetConceptInstancesItem* mBottomInstancesItem;

					cint64 mTestingPossibleSameIndividualCount;
					cint64 mTestingPossibleInstanceCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGITEM_H
