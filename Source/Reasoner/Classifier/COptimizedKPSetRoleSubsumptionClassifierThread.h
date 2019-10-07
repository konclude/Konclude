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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COptimizedKPSetRoleSubsumptionClassifierThread_H
#define KONCLUDE_REASONER_CLASSIFIER_COptimizedKPSetRoleSubsumptionClassifierThread_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifier.h"
#include "CSubsumptionClassifierThread.h"
#include "CPropertyClassificationComputationItem.h"
#include "COptimizedKPSetRoleOntologyClassificationItem.h"
#include "COptimizedKPSetRolePossibleSubsumptionData.h"
#include "COptimizedKPSetRoleTestingItem.h"
#include "CClassificationPropertySubsumptionMessageData.h"
#include "CClassificationInitializePossiblePropertySubsumptionMessageData.h"
#include "CClassificationUpdatePossiblePropertySubsumptionMessageData.h"

// Other includes
#include "Context/CContextBase.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"


#include "Reasoner/Preprocess/CPreProcessContextBase.h"
#include "Reasoner/Preprocess/CRoleChainAutomataTransformationPreProcess.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"

#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"


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
		using namespace Preprocess;

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
					COptimizedKPSetRoleSubsumptionClassifierThread(CReasonerManager *reasonerManager, bool dataRoleClassification);

					//! Destructor
					virtual ~COptimizedKPSetRoleSubsumptionClassifierThread();

					virtual QString getStatusString();
					virtual CClassificationProgress* getClassificationProgress();

				// protected methods
				protected:
					virtual bool finishOntologyClassification(COntologyPropertyRoleClassificationItem *ontPropItem);
					virtual CSubsumptionClassifierThread *scheduleOntologyClassification(CConcreteOntology *ontology, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config);


					COptimizedKPSetRoleSubsumptionClassifierThread* createTemporaryRoleClassificationOntology(COptimizedKPSetRoleOntologyClassificationItem* item);
					void addTemporaryConceptOperand(CConcept* concept, CConcept* opConcept, bool negated, CConcreteOntology* tmpRoleRealOntology);
					CConcept* createTemporaryConcept(CConcreteOntology* tmpRoleRealOntology);
					COptimizedKPSetRoleSubsumptionClassifierThread* createInitialSubsumptionSatisfiabilityTestingOrderFromBuildData(COptimizedKPSetRoleOntologyClassificationItem* ontPropItem);


					virtual CSubsumptionClassifierThread *processCalculationJob(CSatisfiableCalculationJob* job, COntologyPropertyRoleClassificationItem *ontPropItem, CPropertyClassificationComputationItem* workItem);

					virtual void readCalculationConfig(CCalculationConfigurationExtension *config);

					virtual bool interpreteSubsumptionResult(COntologyPropertyRoleClassificationItem *ontPropItem, CRole *subsumerRole, CRole *subsumedRole, bool isSubsumption);
					virtual bool interpreteSatisfiableResult(COntologyPropertyRoleClassificationItem *ontPropItem, CRole *satisfiableRole, bool isSatis);

					virtual bool interpreteTestResults(CTestCalculatedCallbackEvent *testResult);
					

					virtual bool createNextSubsumtionTest();
					virtual CRolePropertiesHierarchy *createInitialHierarchyForOntology(CConcreteOntology *ontology, CConfigurationBase *config);

					virtual bool interpreteToldSubsumptionResult(COntologyPropertyRoleClassificationItem *ontPropItem, const QList< QPair<CRole*,CRole*> > &subSumRelList, bool isSubSum);

					

					bool calculateSatisfiable(COptimizedKPSetRoleOntologyClassificationItem *optSubClassItem, COptimizedKPSetRoleTestingItem* nextSatTestItem);



					virtual bool processToldClassificationMessage(COntologyClassificationItem *ontPropItem, CClassificationMessageData* messageData, CMemoryPool* memoryPools);



					bool calculateSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* subsumedItem, COptimizedKPSetRoleTestingItem* possSubsumerItem, COptimizedKPSetRolePossibleSubsumptionData* possSubsumData);

					COptimizedKPSetRolePossibleSubsumptionData* getNextPossibleSubsumptionCalculation(COptimizedKPSetRoleOntologyClassificationItem *ontPropItem, COptimizedKPSetRoleTestingItem* classItem);


					bool prunePossibleSubsumptions(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, COptimizedKPSetRolePossibleSubsumptionData* possSubsumData);
					bool propagateDownSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, COptimizedKPSetRoleTestingItem* subsumerItem);
					bool pruneDownSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, CRole* subsumerRole);
					bool pruneUpNotSubsumption(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item, CRole* notSubsumerRole);
					
					bool propagateUnsatisfiability(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRoleTestingItem* item);

					QString createDebugKPSetString(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, const QString& filename);
					void testDebugPossibleSubsumerCorrectCounted(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem);
					void testDebugPossibleSubsumerCorrectReferenced(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem);

					void incRemainingPossibleSubsumptionTestingCount(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRolePossibleSubsumptionMap* posSubsumMap);
					void decRemainingPossibleSubsumptionTestingCount(COptimizedKPSetRoleOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetRolePossibleSubsumptionMap* posSubsumMap, bool subsumptionConfirmed);


				// protected variables
				protected:
					QHash<CPropertyClassificationComputationItem *, COntologyPropertyRoleClassificationItem *> workOntItemHash;

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

					bool mConfPossSubsumCalcOrderRoleSorted;
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

					bool mDataRolesClassification;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COptimizedKPSetRoleSubsumptionClassifierThread_H
