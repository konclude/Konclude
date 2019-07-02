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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSSATURATIONSUBSUMPTIONCLASSIFIERTHREAD_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSSATURATIONSUBSUMPTIONCLASSIFIERTHREAD_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifier.h"
#include "CSubsumptionClassifierThread.h"
#include "CPartialPruningTaxonomy.h"
#include "CClassificationWorkItem.h"
#include "CInterceptResultCallbackDataContext.h"
#include "COptimizedClassSaturationOntologyClassificationItem.h"
#include "CPrecomputedSaturationSubsumerExtractor.h"

// Other includes
#include "Reasoner/Classifier/Events/CSaturationCalculatedCallbackEvent.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Generator/CSaturationCalculationJobGenerator.h"

#include "KoncludeSettings.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"

#include <iostream>

using namespace std;


namespace Konclude {


	namespace Reasoner {

		using namespace Revision;
		using namespace Generator;
		using namespace Ontology;

		using namespace Kernel;

		namespace Classifier {

			using namespace Events;

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

	//#define OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGCOUTOUTPUT 
	//#define OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGTAXONOMYCOUTOUTPUT
	//#define OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGLOGOUTPUT 
	//#define OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGBOXDESCRIPTION 
	#define OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGSTATUSDESCRIPTION

#endif


			/*! 
			 *
			 *		\class		COptimizedClassSaturationSubsumptionClassifierThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedClassSaturationSubsumptionClassifierThread : public CSubsumptionClassifierThread, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					COptimizedClassSaturationSubsumptionClassifierThread(CReasonerManager *reasonerManager);

					//! Destructor
					virtual ~COptimizedClassSaturationSubsumptionClassifierThread();

					virtual QString getStatusString();
					virtual CClassificationProgress* getClassificationProgress();

				// protected methods
				protected:
					virtual bool finishOntologyClassification(COntologyClassificationItem *ontClassItem);
					virtual CSubsumptionClassifierThread *scheduleOntologyClassification(CConcreteOntology *ontology, CTaxonomy *taxonomy, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config);

					virtual CSubsumptionClassifierThread *processCalculationJob(CSaturationCalculationJob* job, COntologyClassificationItem *ontClassItem);

					virtual void readCalculationConfig(CCalculationConfigurationExtension *config);

					virtual bool interpreteSubsumptionResult(COntologyClassificationItem *ontClassItem, CConcept *subsumerConcept, CConcept *subsumedConcept, bool isSubsumption);
					virtual bool interpreteSatisfiableResult(COntologyClassificationItem *ontClassItem, CConcept *satisfiableConcept, bool isSatis);

					virtual bool interpreteTestResults(CTestCalculatedCallbackEvent *testResult);
					virtual bool interpreteTestResults(CSaturationCalculatedCallbackEvent *testResult);
					
					virtual bool interceptTestResults(CInterceptOntologyTestResultEvent *interceptResult);

					virtual bool createNextSubsumtionTest();
					virtual CTaxonomy *createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config);

					virtual bool interpreteToldSubsumptionResult(COntologyClassificationItem *ontClassItem, const QList<QPair<CConcept *,CConcept *> > &subSumRelList, bool isSubSum);

					void createObviousSubsumptionPilingSatisfiableTestingOrder(COptimizedClassSaturationOntologyClassificationItem* ontClassItem);
					void createObviousSubsumptionSatisfiableTestingOrder(COptimizedClassSaturationOntologyClassificationItem* ontClassItem);
					void analyseConceptSatisfiableSubsumerExistItems(COptimizedClassSaturationOntologyClassificationItem* ontClassItem, COptimizedConceptSaturationSatisfiableTestingItem* ontConSatTestItem, CConcept* concept, bool negated);

					void propagateExistInitializationFlag(COptimizedConceptSaturationSatisfiableTestingItem* ontClassSatTestItem);
					
					void orderItemsSaturationTesting(COptimizedClassSaturationOntologyClassificationItem* ontClassItem, COptimizedConceptSaturationSatisfiableTestingItem* ontConSatTestItem, QList<COptimizedConceptSaturationSatisfiableTestingItem*>& orderedItemList);


					void propagateSubsumerItemFlag(COptimizedConceptSaturationSatisfiableTestingItem* ontClassSatTestItem);

					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

					void readSubsumptionFromPrecomputedSaturation(COptimizedClassSaturationOntologyClassificationItem* ontClassItem);

				// protected variables
				protected:
					QHash<CSaturationCalculationJob *, COntologyClassificationItem *> workOntItemHash;

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

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSSATURATIONSUBSUMPTIONCLASSIFIERTHREAD_H
