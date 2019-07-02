/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSSUBSUMPTIONCLASSIFIERTHREAD_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSSUBSUMPTIONCLASSIFIERTHREAD_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifier.h"
#include "CSubsumptionClassifierThread.h"
#include "CPartialPruningTaxonomy.h"
#include "CClassificationWorkItem.h"
#include "CInterceptResultCallbackDataContext.h"
#include "COptimizedSubClassOntologyClassificationItem.h"
#include "CClassificationSubsumptionMessageData.h"
#include "CPrecomputedSaturationSubsumerExtractor.h"

// Other includes
#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSatisfiableReferenceLinkingData.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"

#include "KoncludeSettings.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"

#include <iostream>

using namespace std;


namespace Konclude {


	namespace Reasoner {

		using namespace Revision;
		using namespace Ontology;
		using namespace Generator;

		using namespace Kernel;
		using namespace Task;

		namespace Classifier {

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

	//#define OPTIMIZEDSUBCLASSCLASSIFIERDEBUGCOUTOUTPUT 
	//#define OPTIMIZEDSUBCLASSCLASSIFIERDEBUGTAXONOMYCOUTOUTPUT
	//#define OPTIMIZEDSUBCLASSCLASSIFIERDEBUGLOGOUTPUT 
	//#define OPTIMIZEDSUBCLASSCLASSIFIERDEBUGBOXDESCRIPTION 
	#define OPTIMIZEDSUBCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION

#endif


			/*! 
			 *
			 *		\class		COptimizedSubClassSubsumptionClassifierThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedSubClassSubsumptionClassifierThread : public CSubsumptionClassifierThread, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					COptimizedSubClassSubsumptionClassifierThread(CReasonerManager *reasonerManager);

					//! Destructor
					virtual ~COptimizedSubClassSubsumptionClassifierThread();

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
					
					virtual bool interceptTestResults(CInterceptOntologyTestResultEvent *interceptResult);

					virtual bool createNextSubsumtionTest();
					virtual CTaxonomy *createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config);

					virtual bool interpreteToldSubsumptionResult(COntologyClassificationItem *ontClassItem, const QList<QPair<CConcept *,CConcept *> > &subSumRelList, bool isSubSum);

					void createObviousSubsumptionSatisfiableTestingOrder(COptimizedSubClassOntologyClassificationItem* ontClassItem);

					void createObviousSubsumptionSatisfiableTestingOrderFromBuildData(COptimizedSubClassOntologyClassificationItem* ontClassItem);
					void createObviousSubsumptionSatisfiableTestingOrderFromSaturationData(COptimizedSubClassOntologyClassificationItem* ontClassItem);

					bool calculateSatisfiable(COptimizedSubClassOntologyClassificationItem *optSubClassItem, COptimizedSubClassSatisfiableTestingItem* nextSatTestItem);
					bool fastSatisfiableOnlySubClassPrecheckTest(COptimizedSubClassOntologyClassificationItem *optSubClassItem, COptimizedSubClassSatisfiableTestingItem* nextSatTestItem, bool* isSatisfiableFlag);

					virtual bool processToldClassificationMessage(COntologyClassificationItem *ontClassItem, CClassificationMessageData* messageData, CMemoryPool* memoryPools);

				// protected variables
				protected:
					QHash<CClassificationWorkItem *, COntologyClassificationItem *> workOntItemHash;

					QStringList satTestedConList;
					QString statusString;
					QMutex statUpdateMutex;

					CClassificationProgress mClassifyProgess;
					cint64 satTestedCount;
					cint64 totalToTestCount;
					QTime classStartTime;

					QSet<QPair<CConcept *, CConcept *> > subsumCalcedSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSSUBSUMPTIONCLASSIFIERTHREAD_H
