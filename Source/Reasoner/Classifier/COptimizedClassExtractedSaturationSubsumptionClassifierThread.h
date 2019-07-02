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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSEXTRACTEDSATURATIONSUBSUMPTIONCLASSIFIERTHREAD_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSEXTRACTEDSATURATIONSUBSUMPTIONCLASSIFIERTHREAD_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifier.h"
#include "CSubsumptionClassifierThread.h"
#include "CPartialPruningTaxonomy.h"
#include "CPrecomputedSaturationSubsumerExtractor.h"
#include "COptimizedClassExtractedSaturationOntologyClassificationItem.h"

// Other includes
#include "Reasoner/Classifier/Events/CSaturationCalculatedCallbackEvent.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"


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

		using namespace Kernel;

		namespace Classifier {

			using namespace Events;


			/*! 
			 *
			 *		\class		COptimizedClassExtractedSaturationSubsumptionClassifierThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedClassExtractedSaturationSubsumptionClassifierThread : public CSubsumptionClassifierThread, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					COptimizedClassExtractedSaturationSubsumptionClassifierThread(CReasonerManager *reasonerManager);

					//! Destructor
					virtual ~COptimizedClassExtractedSaturationSubsumptionClassifierThread();

					virtual QString getStatusString();
					virtual CClassificationProgress* getClassificationProgress();

				// protected methods
				protected:
					virtual bool finishOntologyClassification(COntologyClassificationItem *ontClassItem);
					virtual CSubsumptionClassifierThread *scheduleOntologyClassification(CConcreteOntology *ontology, CTaxonomy *taxonomy, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config);

					virtual void readCalculationConfig(CCalculationConfigurationExtension *config);

					virtual bool interpreteSubsumptionResult(COntologyClassificationItem *ontClassItem, CConcept *subsumerConcept, CConcept *subsumedConcept, bool isSubsumption);
					virtual bool interpreteSatisfiableResult(COntologyClassificationItem *ontClassItem, CConcept *satisfiableConcept, bool isSatis);

					virtual bool interpreteTestResults(CTestCalculatedCallbackEvent *testResult);
					virtual bool interceptTestResults(CInterceptOntologyTestResultEvent *interceptResult);


					virtual bool createNextSubsumtionTest();
					virtual CTaxonomy *createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config);


					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

					void readSubsumptionFromPrecomputedSaturation(COptimizedClassExtractedSaturationOntologyClassificationItem* ontClassItem);

					cint64 makeParentAddPredeccessors(COptimizedClassExtractedSaturationSatisfiableTestingItem* conceptItem, COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumedConceptItem, cint64 remainingSubsumingResolvingCount, bool firstResolve);


				// protected variables
				protected:
					QString statusString;
					QMutex statUpdateMutex;

					bool mTransitiveReductionAddAllPredecessors;

					CClassificationProgress mClassifyProgess;
					QTime classStartTime;

					cint64 satTestedCount;
					cint64 totalToTestCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSEXTRACTEDSATURATIONSUBSUMPTIONCLASSIFIERTHREAD_H
