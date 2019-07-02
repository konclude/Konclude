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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYCLASSIFICATIONITEM_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "ClassifierSettings.h"
#include "CClassificationWorkItem.h"
#include "CClassifiedCallbackDataContext.h"
#include "CClassifierStatistics.h"
#include "CPrecomputedSaturationSubsumerExtractor.h"

// Other includes
#include "Reasoner/Classification/CClassConceptClassification.h"
#include "Reasoner/Classification/CClassificationCalculationStatisticsCollection.h"
#include "Reasoner/Classification/CClassificationStatisticsCollectionStrings.h"

#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyProcessingStep.h"
#include "Reasoner/Ontology/COntologyProcessingSteps.h"
#include "Reasoner/Ontology/COntologyProcessingStepData.h"
#include "Reasoner/Ontology/CConceptNegationPair.h"

#include "Reasoner/Consistiser/CIndividualDependenceTrackingCollector.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Concurrent/Callback/CCallbackListExecuter.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;
		using namespace Classification;
		using namespace Consistiser;

		using namespace Kernel::Task;

		namespace Classifier {


			/*! 
			 *
			 *		\class		COntologyClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyClassificationItem : public CClassConceptClassification {
				// public methods
				public:
					//! Constructor
					COntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics = 0);

					//! Destructor
					virtual ~COntologyClassificationItem();

					virtual COntologyClassificationItem *initTaxonomyConcepts(CConcreteOntology *ontology, CTaxonomy *taxonomy);

					CPrecomputedSaturationSubsumerExtractor* getPrecomputedSaturationSubsumerExtractor(bool create = true);


					virtual CConcreteOntology *getOntology();
					virtual CTaxonomy *getTaxonomy();
					virtual CTBox *getTBox();
					virtual CCalculationConfigurationExtension *getCalculationConfiguration();

					virtual QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *> *getWorkItemHash();

					virtual QList<CHierarchyNode *> *getAllNodeList();
					virtual QList<CHierarchyNode *> *getProcessingNodeList();

					virtual COntologyClassificationItem *incCurrentCalculatingCount(qint64 incCount = 1);
					virtual COntologyClassificationItem *decCurrentCalculatingCount(qint64 decCount = 1);
					virtual qint64 getCurrentCalculatingCount();

					virtual COntologyClassificationItem *setGoneOutRemainingTests(bool moreTests = false);
					virtual bool hasRemainingTests();

					virtual COntologyClassificationItem *doClassifiedCallback();
					virtual COntologyClassificationItem *addClassifiedCallback(CCallbackData *callback);

					virtual qint64 getCalcedSatisfiableCount();
					virtual qint64 getCalcedSubsumptionCount();

					virtual CClassifierStatistics *getClassifierStatistics();


					CClassConceptClassification* getClassConceptClassification();
					CClassificationStatisticsCollectionStrings* getClassificationStatisticsCollectionStrings();

					CClassificationCalculationStatisticsCollection* getCalculationStatisticsCollection();
					COntologyClassificationItem* reuseCalculationStatisticsCollection(CClassificationCalculationStatisticsCollection* statColl);


					bool isCollectProcessStatisticsActivated();

					QList<COntologyProcessingRequirement*> getOntologyProcessingRequirementList();
					COntologyClassificationItem* addOntologyProcessingRequirements(const QList<COntologyProcessingRequirement*>& reqList);
					bool isTaxonomyConstructed();
					COntologyClassificationItem* submitTaxonomyConstructed();
					COntologyClassificationItem* setTaxonomyConstructionFailed();
					bool isTaxonomyConstructionFailed();

					CIndividualDependenceTrackingCollector* getIndividualDependenceTrackingCollector();
					COntologyClassificationItem* setIndividualDependenceTrackingCollector(CIndividualDependenceTrackingCollector* indiDepTrackColl);


					COntologyProcessingStepData* getClassificationProcessingStep();
					QTime* getInitializationTime();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology *onto;
					CTaxonomy *tax;
					CClassificationStatisticsCollectionStrings* mClassificationStatCollStrings;
					QList<CClassificationCalculationStatisticsCollection*> mReuseStatCollList;

					CPrecomputedSaturationSubsumerExtractor* mPrecSatSubsumerExtractor;

					QHash<CSatisfiableCalculationJob*, CClassificationWorkItem*> workItemHash;

					QList<CHierarchyNode *> allNodeList;
					QList<CHierarchyNode *> processingNodeList;

					CCallbackListExecuter classifiedCallbackList;

					qint64 currentCalculatingCount;

					bool remainingTests;

					CClassifierStatistics *statistics;
					CCalculationConfigurationExtension *config;

					bool mConfCollectProcessStatistics;

					QList<COntologyProcessingRequirement*> mRequirementList;
					bool mTaxonomyConstructed;
					bool mTaxonomyConstructionFailed;
					COntologyProcessingStepData* mClassifyProcessingStepData;


					CIndividualDependenceTrackingCollector* mIndiDepTrackingCollector;

					QTime mInitTime;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYCLASSIFICATIONITEM_H
