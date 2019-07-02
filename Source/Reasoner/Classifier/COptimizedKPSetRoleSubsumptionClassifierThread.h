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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COptimizedKPSetRoleSubsumptionClassifierThread_H
#define KONCLUDE_REASONER_CLASSIFIER_COptimizedKPSetRoleSubsumptionClassifierThread_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifier.h"
#include "CSubsumptionClassifierThread.h"
#include "CPartialPruningTaxonomy.h"
#include "CClassificationWorkItem.h"
#include "COptimizedKPSetRoleOntologyClassificationItem.h"
#include "CClassificationPseudoModelIdentifierMessageData.h"
#include "COptimizedKPSetRolePossibleSubsumptionData.h"
#include "COptimizedKPSetRoleTestingItem.h"
#include "CClassificationSubsumptionMessageData.h"
#include "CClassificationInitializePossibleSubsumptionMessageData.h"
#include "CClassificationUpdatePossibleSubsumptionMessageData.h"
#include "CPrecomputedSaturationSubsumerExtractor.h"

// Other includes
#include "Context/CContextBase.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"

#include "Utilities/Memory/CTempMemoryPoolContainerAllocationManager.h"

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
		using namespace Task;

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
			 *		\class		COptimizedKPSetRoleSubsumptionClassifierThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetRoleSubsumptionClassifierThread : public CSubsumptionClassifierThread, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetRoleSubsumptionClassifierThread(CReasonerManager *reasonerManager);

					//! Destructor
					virtual ~COptimizedKPSetRoleSubsumptionClassifierThread();

					virtual QString getStatusString();
					virtual CClassificationProgress* getClassificationProgress();

				// protected methods
				protected:
					virtual bool finishOntologyClassification(COntologyClassificationItem *ontClassItem);
					virtual CSubsumptionClassifierThread *scheduleOntologyClassification(CConcreteOntology *ontology, CTaxonomy *taxonomy, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config);

					virtual CSubsumptionClassifierThread *processCalculationJob(CSatisfiableCalculationJob* job, COntologyClassificationItem *ontClassItem, CClassificationWorkItem* workItem);

					virtual void readCalculationConfig(CCalculationConfigurationExtension *config);

					virtual bool interpreteSubsumptionResult(COntologyClassificationItem *ontClassItem, CConcept *subsumerConcept, CConcept *subsumedConcept, bool isSubsumption);
					virtual bool interpreteSatisfiableResult(COntologyClassificationItem *ontClassItem, CConcept *satisfiableConcept, bool isSatis);

					virtual bool interpreteTestResults(CTestCalculatedCallbackEvent *testResult);
					

					virtual bool createNextSubsumtionTest();
					virtual CTaxonomy *createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config);

					virtual bool interpreteToldSubsumptionResult(COntologyClassificationItem *ontClassItem, const QList<QPair<CConcept *,CConcept *> > &subSumRelList, bool isSubSum);

					

					bool calculateSatisfiable(COptimizedKPSetRoleOntologyClassificationItem *optSubClassItem, COptimizedKPSetRoleTestingItem* nextSatTestItem);



					virtual bool processToldClassificationMessage(COntologyClassificationItem *ontClassItem, CClassificationMessageData* messageData, CMemoryPool* memoryPools);



					bool calculateSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* subsumedItem, COptimizedKPSetRoleTestingItem* possSubsumerItem, COptimizedKPSetRolePossibleSubsumptionData* possSubsumData);

					COptimizedKPSetRolePossibleSubsumptionData* getNextPossibleSubsumptionCalculation(COptimizedKPSetRoleOntologyClassificationItem *ontClassItem, COptimizedKPSetRoleTestingItem* classItem);


					bool prunePossibleSubsumptions(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, COptimizedKPSetRolePossibleSubsumptionData* possSubsumData);
					bool propagateDownSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, COptimizedKPSetRoleTestingItem* subsumerItem);
					bool pruneDownSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, CConcept* subsumerConcept);
					bool pruneUpNotSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, CConcept* notSubsumerConcept);
					
					bool propagateUnsatisfiability(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item);

					QString createDebugKPSetString(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, const QString& filename);
					void testDebugPossibleSubsumerCorrectCounted(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem);
					void testDebugPossibleSubsumerCorrectReferenced(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem);

					void incRemainingPossibleSubsumptionTestingCount(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRolePossibleSubsumptionMap* posSubsumMap);
					void decRemainingPossibleSubsumptionTestingCount(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRolePossibleSubsumptionMap* posSubsumMap, bool subsumptionConfirmed);


				// protected variables
				protected:
					QHash<CClassificationWorkItem *, COntologyClassificationItem *> workOntItemHash;

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



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COptimizedKPSetRoleSubsumptionClassifierThread_H
