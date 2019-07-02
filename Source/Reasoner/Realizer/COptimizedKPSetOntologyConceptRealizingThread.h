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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGTHREAD_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGTHREAD_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "CRealizerThread.h"
#include "CRealizingTestingItem.h"
#include "CIndividualConceptInstanceTestingItem.h"
#include "CIndividualPairRoleInstanceTestingItem.h"
#include "CIndividualSameTestingItem.h"
#include "COptimizedKPSetOntologyConceptRealizingItem.h"
#include "COptimizedKPSetIndividualItem.h"
#include "COptimizedKPSetConceptInstancesItem.h"
#include "COptimizedKPSetConceptInstancesData.h"
#include "CRealizationMarkerCandidatesMessageData.h"
#include "CIndividualRoleCandidateTestingItem.h"
#include "CIndividualsConsistencyTestingItem.h"

// Other includes
#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"
#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskIndividualDependenceTrackingAdapter.h"

#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNodeVector.h"

#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCache.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheReader.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"
#include "Reasoner/Generator/CConcreteOntologyUpdateBuilder.h"

#include "Reasoner/Preprocess/CRoleChainAutomataTransformationPreProcess.h"
#include "Reasoner/Preprocess/CPreProcessContextBase.h"

#include "Utilities/Memory/CTempMemoryPoolContainerAllocationManager.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Kernel::Process;
		using namespace Kernel::Manager;
		using namespace Kernel::Cache;
		using namespace Generator;
		using namespace Preprocess;

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
					class CKnownPossibleInstanceHashingData {
						public:
							CKnownPossibleInstanceHashingData() {
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
					virtual void readCalculationConfig(CConfigurationBase *config);

					virtual bool createNextTest();

					virtual bool finishOntologyRealizing(COptimizedKPSetOntologyConceptRealizingItem* totallyPreCompItem);
					virtual bool realizingTested(COntologyRealizingItem* ontPreCompItem, CRealizingTestingItem* preTestItem, CRealizingCalculatedCallbackEvent* pcce);

					bool createNextConceptInstantiationTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem);
					bool createNextRoleInstantiationTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetIndividualItemPair itemPair);
					bool createNextSameIndividualsTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* instantiatedItem1, COptimizedKPSetIndividualItem* instantiatedItem2);
					bool createIndividualsConsistencyTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);

					bool createNextRoleInitializingTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetIndividualItem* individualItem);
					bool createNextRoleInitializingTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* individualItem);

					bool initializeSamePossibleIndividuals(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* individualItem, const QList<CIndividual*>& possibleSameIndividualList);

					bool initializeItems(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);

					virtual bool initializeKPSetsFromConsistencyData(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);
					bool extractKnownPossibleIndividualDataFromConsistencyData(CIndividual* individual, QList<COptimizedKPSetConceptInstancesItem*>* knownInstancesList, QList<COptimizedKPSetConceptInstancesItem*>* possibleInstancesList, QList<CIndividual*>* knownSameIndividualList, QList<CIndividual*>* possibleSameIndividualList, CIndividualProcessNodeVector* indiProcVector, const QList<COptimizedKPSetConceptInstancesItem*>& equivClassList, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);


					bool initializeEquivalentClassList(QList<COptimizedKPSetConceptInstancesItem*>* equivClassList, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);

					bool initializeKPSetsForIndividual(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, CIndividual* individual, QList<COptimizedKPSetConceptInstancesItem*>& knownInstancesList, QList<COptimizedKPSetConceptInstancesItem*>& possibleInstancesList, QList<CIndividual*>& knownSameIndividualList);
					bool addKPSetDirectSuperInstances(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash, COptimizedKPSetConceptInstancesItem* instanceItem, bool knownInstance);

					CIndividualProcessNode* getMergeCorrectedIndividualProcessNode(CIndividualProcessNode* indiProcNode, CIndividualProcessNodeVector* indiProcVector, bool* nonDeterministicallyMergedFlag, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);


					COptimizedKPSetOntologyConceptRealizingThread* incTestedPossibleConceptInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingThread* incOpenPossibleConceptInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount = 1);

					COptimizedKPSetOntologyConceptRealizingThread* decTestedPossibleConceptInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount = 1);
					COptimizedKPSetOntologyConceptRealizingThread* decOpenPossibleConceptInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount = 1);



					COptimizedKPSetOntologyConceptRealizingThread* incTestedPossibleRoleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingThread* incOpenPossibleRoleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount = 1);

					COptimizedKPSetOntologyConceptRealizingThread* decTestedPossibleRoleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount = 1);
					COptimizedKPSetOntologyConceptRealizingThread* decOpenPossibleRoleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount = 1);




					COptimizedKPSetOntologyConceptRealizingThread* incTestedPossibleSameIndividualsCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount = 1);
					COptimizedKPSetOntologyConceptRealizingThread* incOpenPossibleSameIndividualsCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount = 1);

					COptimizedKPSetOntologyConceptRealizingThread* decTestedPossibleSameIndividualsCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount = 1);
					COptimizedKPSetOntologyConceptRealizingThread* decOpenPossibleSameIndividualsCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount = 1);


					virtual COptimizedKPSetOntologyConceptRealizingThread* addRealizationStatistics(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, CRealizingTestingStep* ontProcStep);
					virtual COptimizedKPSetOntologyConceptRealizingThread* addIndividualDependencyTrackingStatistics(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, CRealizingTestingStep* ontProcStep, const QString& realizationTypeString);


					COptimizedKPSetOntologyConceptRealizingThread* updateParentItemsSuccessorProcessed(COptimizedKPSetConceptInstancesItem* item);
					COptimizedKPSetOntologyConceptRealizingThread* updateParentItemsSuccessorProcessed(COptimizedKPSetRoleInstancesItem* item);
					
					virtual CRealizationProgress* getRealizationProgress();

					COptimizedKPSetOntologyConceptRealizingThread* updateCalculatedKnownConceptInstance(COptimizedKPSetIndividualItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);
					COptimizedKPSetOntologyConceptRealizingThread* updateCalculatedNonConceptInstance(COptimizedKPSetIndividualItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);

					COptimizedKPSetOntologyConceptRealizingThread* mergeSameIndividualItems(COptimizedKPSetIndividualItem* instantiatedItem1, COptimizedKPSetIndividualItem* instantiatedItem2, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);


					COptimizedKPSetOntologyConceptRealizingThread* createTemporarySameRealizationOntology(COptimizedKPSetOntologyConceptRealizingItem* item);

					COptimizedKPSetOntologyConceptRealizingThread* createTemporaryRoleRealizationOntology(COptimizedKPSetOntologyConceptRealizingItem* item);


					CConcept* createTemporaryConcept(COptimizedKPSetOntologyConceptRealizingItem* item, CConcreteOntology* tmpRoleRealOntology);
					void addTemporaryConceptOperand(CConcept* concept, CConcept* opConcept, bool negated, COptimizedKPSetOntologyConceptRealizingItem* item, CConcreteOntology* tmpRoleRealOntology);

					COptimizedKPSetOntologyConceptRealizingThread* updateCalculatedKnownRoleInstance(COptimizedKPSetIndividualItemPair individualItemPair, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);
					COptimizedKPSetOntologyConceptRealizingThread* updateCalculatedNonRoleInstance(COptimizedKPSetIndividualItemPair individualItemPair, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem);

					virtual bool processRealizationMessage(COntologyRealizingItem* ontRealItem, CRealizationMessageData* messageData, CMemoryPool* memoryPools);
					
					bool addKPSetDirectSuperInstances(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstItem, bool inversed, COptimizedKPSetIndividualItem* indiItem1, COptimizedKPSetIndividualItem* indiItem2, bool knownInstance);


					CIndividualProcessNode* getCompletionGraphCachedIndividualProcessNode(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* indiItem, bool* nondeterministicNodeFlag = nullptr);


				// protected variables
				protected:
					cint64 mTestedPossibleInstancesCount;
					cint64 mOpenPossibleInstancesCount;

					CRealizationProgress mRealProgress;
					QTime mRealStartTime;

					CBackendRepresentativeMemoryCache* mBackendAssocCache;
					CBackendRepresentativeMemoryCacheReader* mBackendAssocCacheReader;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETONTOLOGYCONCEPTREALIZINGTHREAD_H
