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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGTHREAD_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGTHREAD_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "CRealizerThread.h"
#include "CRealizingTestingItem.h"
#include "CIndividualInstanceTestingItem.h"
#include "CIndividualSameTestingItem.h"
#include "COptimizedKPSetOntologyConceptRealizingItem.h"
#include "COptimizedKPSetConceptInstantiatedItem.h"
#include "COptimizedKPSetConceptInstancesItem.h"
#include "COptimizedKPSetConceptInstancesData.h"

// Other includes
#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"
#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"

#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNodeVector.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"
#include "Reasoner/Generator/CConcreteOntologyUpdateBuilder.h"

#include "Utilities/Memory/CTempMemoryPoolContainerAllocationManager.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Kernel::Process;
		using namespace Kernel::Manager;
		using namespace Generator;

		namespace Realizer {

			using namespace Events;


			/*! 
			 *
			 *		\class		COptimizedKPSetOntologyConceptRealizingThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetOntologyConceptRealizingThread : public CRealizerThread, public CLogDomain {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetOntologyConceptRealizingThread(CReasonerManager* reasonerManager);

					//! Destructor
					~COptimizedKPSetOntologyConceptRealizingThread();


				// protected methods
				protected:
					class CKnownPossibleInstancHashingData {
						public:
							CKnownPossibleInstancHashingData() {
								mKnownInstance = false;
								mDirectInserted = false;
								mMostSpecialRetest = false;
								mInstanceItem = nullptr;
							}
							bool mKnownInstance;
							bool mDirectInserted;
							bool mMostSpecialRetest;
							COptimizedKPSetConceptInstancesItem* mInstanceItem;
					};


					virtual COntologyRealizingItem* initializeOntologyRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config);
					void readCalculationConfig(CConfigurationBase *config);

					virtual bool createNextTest();

					virtual bool finishOntologyRealizing(COptimizedKPSetOntologyConceptRealizingItem* totallyPreCompItem);
					virtual bool realizingTested(COntologyRealizingItem* ontPreCompItem, CRealizingTestingItem* preTestItem, CRealizingCalculatedCallbackEvent* pcce);

					bool createNextInstantiationTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetConceptInstantiatedItem* instantiatedItem);
					bool createNextSameIndividualsTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstantiatedItem* instantiatedItem1, COptimizedKPSetConceptInstantiatedItem* instantiatedItem2);

					bool initializeSamePossibleIndividuals(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstantiatedItem* individualItem, const QList<CIndividual*>& possibleSameIndividualList);

					bool initializeItems(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);
					bool initializeKPSetsFromCompletionGraph(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);
					bool initializeEquivalentClassList(QList<COptimizedKPSetConceptInstancesItem*>* equivClassList, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);

					bool initializeKPSetsForIndividual(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, CIndividual* individual, QList<COptimizedKPSetConceptInstancesItem*>& knownInstancesList, QList<COptimizedKPSetConceptInstancesItem*>& possibleInstancesList, QList<CIndividual*>& knownSameIndividualList);
					bool addKPSetDirectSuperInstances(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash, COptimizedKPSetConceptInstancesItem* instanceItem, bool knownInstance);

					CIndividualProcessNode* getMergeCorrectedIndividualProcessNode(CIndividualProcessNode* indiProcNode, CIndividualProcessNodeVector* indiProcVector, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);


					COptimizedKPSetOntologyConceptRealizingThread* incTestedPossibleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingThread* incOpenPossibleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount = 1);

					COptimizedKPSetOntologyConceptRealizingThread* decTestedPossibleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount = 1);
					COptimizedKPSetOntologyConceptRealizingThread* decOpenPossibleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount = 1);

					COptimizedKPSetOntologyConceptRealizingThread* updateParentItemsSuccessorProcessed(COptimizedKPSetConceptInstancesItem* item);
					
					virtual CRealizationProgress* getRealizationProgress();

					COptimizedKPSetOntologyConceptRealizingThread* updateCalculatedKnownInstance(COptimizedKPSetConceptInstantiatedItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);
					COptimizedKPSetOntologyConceptRealizingThread* updateCalculatedNotInstance(COptimizedKPSetConceptInstantiatedItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);

					COptimizedKPSetOntologyConceptRealizingThread* mergeSameIndividualItems(COptimizedKPSetConceptInstantiatedItem* instantiatedItem1, COptimizedKPSetConceptInstantiatedItem* instantiatedItem2, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);

				// protected variables
				protected:
					cint64 mTestedPossibleInstancesCount;
					cint64 mOpenPossibleInstancesCount;

					CRealizationProgress mRealProgress;
					QTime mRealStartTime;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGTHREAD_H
