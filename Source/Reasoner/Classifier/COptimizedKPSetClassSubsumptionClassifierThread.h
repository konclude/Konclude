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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSSUBSUMPTIONCLASSIFIERTHREAD_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSSUBSUMPTIONCLASSIFIERTHREAD_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifier.h"
#include "CSubsumptionClassifierThread.h"
#include "CPartialPruningTaxonomy.h"
#include "CClassClassificationComputationItem.h"
#include "COptimizedKPSetClassOntologyClassificationItem.h"
#include "CClassificationPseudoModelIdentifierMessageData.h"
#include "COptimizedKPSetClassPossibleSubsumptionData.h"
#include "COptimizedKPSetClassTestingItem.h"
#include "CClassificationClassSubsumptionMessageData.h"
#include "CClassificationInitializePossibleClassSubsumptionMessageData.h"
#include "CClassificationUpdatePossibleClassSubsumptionMessageData.h"
#include "CPrecomputedSaturationSubsumerExtractor.h"

// Other includes
#include "Context/CContextBase.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Consistiser/CSaturationConceptReferenceLinking.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"

#include "Utilities/Memory/CTempMemoryPoolContainerAllocationManager.h"

#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCache.h"
#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCacheReader.h"


#include "KoncludeSettings.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"

#include <iostream>

using namespace std;


namespace Konclude {

	using namespace Context;
	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Revision;
		using namespace Ontology;
		using namespace Generator;

		using namespace Kernel;
		using namespace Cache;
		using namespace Task;
		using namespace Consistiser;

		namespace Classifier {

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

	//#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCOUTOUTPUT 
	//#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGTAXONOMYCOUTOUTPUT
	//#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGLOGOUTPUT 
	//#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGBOXDESCRIPTION 
	#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION

#endif


			/*! 
			 *
			 *		\class		COptimizedKPSetClassSubsumptionClassifierThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetClassSubsumptionClassifierThread : public CSubsumptionClassifierThread, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetClassSubsumptionClassifierThread(CReasonerManager *reasonerManager);

					//! Destructor
					virtual ~COptimizedKPSetClassSubsumptionClassifierThread();

					virtual QString getStatusString();
					virtual CClassificationProgress* getClassificationProgress();

				// protected methods
				protected:

					virtual bool addClassificationStatistics(COntologyClassClassificationItem *ontClassItem, CClassConceptClassification* classConClassification);
					COptimizedKPSetClassSubsumptionClassifierThread* addIndividualDependencyTrackingStatistics(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, const QString& classificationTypeString = "class-classification");

					virtual bool finishOntologyClassification(COntologyClassClassificationItem *ontClassItem);
					virtual CSubsumptionClassifierThread* scheduleOntologyClassification(CConcreteOntology *ontology, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config);
					virtual COptimizedKPSetClassOntologyClassificationItem* createOntologyClassificationItem(CConcreteOntology *ontology, CConfigurationBase *config);

					virtual CSubsumptionClassifierThread *processCalculationJob(CSatisfiableCalculationJob* job, COntologyClassClassificationItem *ontClassItem, CClassClassificationComputationItem* workItem);

					virtual void readCalculationConfig(CCalculationConfigurationExtension *config);

					virtual bool interpreteSubsumptionResult(COntologyClassClassificationItem *ontClassItem, CConcept *subsumerConcept, CConcept *subsumedConcept, bool isSubsumption);
					virtual bool interpreteSatisfiableResult(COntologyClassClassificationItem *ontClassItem, CConcept *satisfiableConcept, bool isSatis);

					virtual bool interpreteTestResults(CTestCalculatedCallbackEvent *testResult);
					
					virtual bool createNextSubsumtionTest();
					virtual CTaxonomy *createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config);

					virtual bool interpreteToldSubsumptionResult(COntologyClassClassificationItem *ontClassItem, const QList<QPair<CConcept *,CConcept *> > &subSumRelList, bool isSubSum);

					void createObviousSubsumptionSatisfiableTestingOrder(COptimizedKPSetClassOntologyClassificationItem* ontClassItem);
					
					QSet<CConcept*> getSubsumerSetFromBuildData(CConcept* concept, COptimizedKPSetClassOntologyClassificationItem* ontClassItem);
					void createObviousSubsumptionSatisfiableTestingOrderFromBuildData(COptimizedKPSetClassOntologyClassificationItem* ontClassItem);
					void createObviousSubsumptionSatisfiableTestingOrderFromSaturationData(COptimizedKPSetClassOntologyClassificationItem* ontClassItem);

					bool calculateSatisfiable(COptimizedKPSetClassOntologyClassificationItem *optSubClassItem, COptimizedKPSetClassTestingItem* nextSatTestItem);
					bool fastSatisfiableOnlySubClassPrecheckTest(COptimizedKPSetClassOntologyClassificationItem *optSubClassItem, COptimizedKPSetClassTestingItem* nextSatTestItem, bool* isSatisfiableFlag);

					bool fastPseudoModelSubsumptionClassPrecheckTest(COptimizedKPSetClassOntologyClassificationItem *optSubClassItem, COptimizedKPSetClassTestingItem* subsumedItem, COptimizedKPSetClassTestingItem* subsumerItem, bool* isSubsumptionFlag);
					bool isPseudoModelSubsumerPossible(COptimizedKPSetClassOntologyClassificationItem *optSubClassItem, CClassificationClassPseudoModelData* subsumedPMData, CClassificationClassPseudoModelHash* subsumedPMHash, CClassificationClassPseudoModelData* subsumerPMData, CClassificationClassPseudoModelHash* subsumerPMHash);


					virtual bool processToldClassificationMessage(COntologyClassificationItem *ontClassItem, CClassificationMessageData* messageData, CMemoryPool* memoryPools);


					bool calculateSubsumption(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* subsumedItem, COptimizedKPSetClassTestingItem* possSubsumerItem, COptimizedKPSetClassPossibleSubsumptionData* possSubsumData);

					COptimizedKPSetClassPossibleSubsumptionData* getNextPossibleSubsumptionCalculation(COptimizedKPSetClassOntologyClassificationItem *ontClassItem, COptimizedKPSetClassTestingItem* classItem);


					bool prunePossibleSubsumptions(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* item, COptimizedKPSetClassPossibleSubsumptionData* possSubsumData);
					bool propagateDownSubsumption(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* item, COptimizedKPSetClassTestingItem* subsumerItem);
					bool pruneDownSubsumption(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* item, CConcept* subsumerConcept);
					bool pruneUpNotSubsumption(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* item, CConcept* notSubsumerConcept);
					
					bool propagateUnsatisfiability(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* item);

					QString createDebugKPSetString(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, const QString& filename);
					void testDebugPossibleSubsumerCorrectCounted(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem);
					void testDebugPossibleSubsumerCorrectReferenced(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem);

					void incRemainingPossibleSubsumptionTestingCount(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassPossibleSubsumptionMap* posSubsumMap);
					void decRemainingPossibleSubsumptionTestingCount(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassPossibleSubsumptionMap* posSubsumMap, bool subsumptionConfirmed);


					CIndividualSaturationProcessNode* getSaturationIndividualNodeForConcept(CConcept* concept, bool negated);
					bool hasCachedSaturationIndividualNodeAssociatedExpansionProplematicConcept(CCacheEntry* cacheEntry, CConcept* testingConcept);
					CCacheEntry* getAssociatedSaturationCacheEntry(COptimizedKPSetClassTestingItem* classConItem);

				// protected variables
				protected:
					QHash<CClassClassificationComputationItem *, COntologyClassClassificationItem *> workOntItemHash;

					QStringList satTestedConList;
					QStringList possSubsumInitConList;
					QString statusString;
					QMutex statUpdateMutex;

					CClassificationProgress mClassifyProgess;
					cint64 satTestedCount;
					cint64 totalToSatTestClassCount;

					cint64 subsumTestedCount;
					cint64 totalToSubsumTestCount;
					QTime classStartTime;

					QSet<QPair<CConcept *, CConcept *> > subsumCalcedSet;

					bool mConfPossSubsumCalcOrderTopDown;
					bool mConfPossSubsumCalcOrderBottomUp;

					bool mConfPossSubsumCalcOrderConceptSorted;
					bool mConfPossSubsumCalcOrderFewestSubsumptionSorted;

					bool mConfPossSubsumPseudoModelPretest;

					bool mConfPossSubsumPseudoModelTest;
					bool mConfWriteDebuggingData;

					cint64 mStatProcesedSubsumMessCount;
					cint64 mStatProcesedPossSubsumInitMessCount;
					cint64 mStatProcesedPossSubsumUpdateMessCount;
					cint64 mStatProcesedPseudoModelMessCount;


					cint64 mInterpretedSubsumptionCalculationCount;
					cint64 mOrderedSubsumptionCalculationCount;
					cint64 mPseudoModelPretestSubsumptionCalculationCount;
					cint64 mCreatedCalculationTaskCount;
					cint64 mRecievedCallbackCount;


					CSaturationNodeAssociatedExpansionCache* mSatNodeExpCache;
					CSaturationNodeAssociatedExpansionCacheReader* mSatNodeExpCacheReader;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSSUBSUMPTIONCLASSIFIERTHREAD_H
