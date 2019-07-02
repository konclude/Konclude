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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSSUBSUMPTIONCLASSIFIERTHREAD_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSSUBSUMPTIONCLASSIFIERTHREAD_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifier.h"
#include "CSubsumptionClassifierThread.h"
#include "CPartialPruningTaxonomy.h"
#include "CClassClassificationComputationItem.h"
#include "COptimizedSubClassOntologyClassificationItem.h"
#include "CClassificationClassSubsumptionMessageData.h"
#include "CPrecomputedSaturationSubsumerExtractor.h"

// Other includes
#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSatisfiableReferenceLinkingData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"

#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCache.h"
#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCacheReader.h"

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
		using namespace Cache;

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
					virtual bool finishOntologyClassification(COntologyClassClassificationItem *ontClassItem);
					virtual CSubsumptionClassifierThread *scheduleOntologyClassification(CConcreteOntology *ontology, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config);

					virtual CSubsumptionClassifierThread *processCalculationJob(CSatisfiableCalculationJob* job, COntologyClassClassificationItem *ontClassItem, CClassClassificationComputationItem* workItem);

					virtual void readCalculationConfig(CCalculationConfigurationExtension *config);

					virtual bool interpreteSubsumptionResult(COntologyClassClassificationItem *ontClassItem, CConcept *subsumerConcept, CConcept *subsumedConcept, bool isSubsumption);
					virtual bool interpreteSatisfiableResult(COntologyClassClassificationItem *ontClassItem, CConcept *satisfiableConcept, bool isSatis);

					virtual bool interpreteTestResults(CTestCalculatedCallbackEvent *testResult);
					
					virtual bool createNextSubsumtionTest();
					virtual CTaxonomy *createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config);

					virtual bool interpreteToldSubsumptionResult(COntologyClassClassificationItem *ontClassItem, const QList<QPair<CConcept *,CConcept *> > &subSumRelList, bool isSubSum);

					void createObviousSubsumptionSatisfiableTestingOrder(COptimizedSubClassOntologyClassificationItem* ontClassItem);

					void createObviousSubsumptionSatisfiableTestingOrderFromBuildData(COptimizedSubClassOntologyClassificationItem* ontClassItem);
					void createObviousSubsumptionSatisfiableTestingOrderFromSaturationData(COptimizedSubClassOntologyClassificationItem* ontClassItem);

					bool calculateSatisfiable(COptimizedSubClassOntologyClassificationItem *optSubClassItem, COptimizedSubClassSatisfiableTestingItem* nextSatTestItem);
					bool fastSatisfiableOnlySubClassPrecheckTest(COptimizedSubClassOntologyClassificationItem *optSubClassItem, COptimizedSubClassSatisfiableTestingItem* nextSatTestItem, bool* isSatisfiableFlag);

					virtual bool processToldClassificationMessage(COntologyClassificationItem *ontClassItem, CClassificationMessageData* messageData, CMemoryPool* memoryPools);

					CIndividualSaturationProcessNode* getSaturationIndividualNodeForConcept(CConcept* concept, bool negated);
					bool hasCachedSaturationIndividualNodeAssociatedExpansionProplematicConcept(CCacheEntry* cacheEntry, CConcept* testingConcept);
					CCacheEntry* getAssociatedSaturationCacheEntry(COptimizedSubClassSatisfiableTestingItem* classConItem);


				// protected variables
				protected:
					QHash<CClassClassificationComputationItem *, COntologyClassClassificationItem *> workOntItemHash;

					QStringList satTestedConList;
					QString statusString;
					QMutex statUpdateMutex;

					CClassificationProgress mClassifyProgess;
					cint64 satTestedCount;
					cint64 totalToTestCount;
					QTime classStartTime;

					QSet<QPair<CConcept *, CConcept *> > subsumCalcedSet;

					CSaturationNodeAssociatedExpansionCache* mSatNodeExpCache;
					CSaturationNodeAssociatedExpansionCacheReader* mSatNodeExpCacheReader;

					cint64 mStatFastSubClassCheckCacheTryCount;
					cint64 mStatFastSubClassCheckCacheSuccCount;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSSUBSUMPTIONCLASSIFIERTHREAD_H
