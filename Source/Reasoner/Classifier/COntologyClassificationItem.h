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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYCLASSIFICATIONITEM_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "ClassifierSettings.h"
#include "CClassifiedCallbackDataContext.h"
#include "CClassifierStatistics.h"

// Other includes
#include "Reasoner/Classification/CClassificationCalculationStatisticsCollection.h"
#include "Reasoner/Classification/CClassificationStatisticsCollectionStrings.h"

#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyProcessingStep.h"
#include "Reasoner/Ontology/COntologyProcessingSteps.h"
#include "Reasoner/Ontology/COntologyProcessingStepData.h"

#include "Reasoner/Consistiser/CIndividualDependenceTrackingCollector.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Concurrent/Callback/CCallbackListExecuter.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;
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
			class COntologyClassificationItem {
				// public methods
				public:
					//! Constructor
					COntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics = 0);

					//! Destructor
					virtual ~COntologyClassificationItem();


					virtual CConcreteOntology *getOntology();

					virtual CCalculationConfigurationExtension *getCalculationConfiguration();


					virtual COntologyClassificationItem *incCurrentCalculatingCount(qint64 incCount = 1);
					virtual COntologyClassificationItem *decCurrentCalculatingCount(qint64 decCount = 1);
					virtual qint64 getCurrentCalculatingCount();

					virtual COntologyClassificationItem *setHasRemainingTests(bool moreTests = false);
					virtual bool hasRemainingTests();

					virtual COntologyClassificationItem *doClassifiedCallback();
					virtual COntologyClassificationItem *addClassifiedCallback(CCallbackData *callback);

					virtual qint64 getCalcedSatisfiableCount();
					virtual qint64 getCalcedSubsumptionCount();

					virtual CClassifierStatistics *getClassifierStatistics();

					CClassificationStatisticsCollectionStrings* getClassificationStatisticsCollectionStrings();

					CClassificationCalculationStatisticsCollection* getCalculationStatisticsCollection();
					COntologyClassificationItem* reuseCalculationStatisticsCollection(CClassificationCalculationStatisticsCollection* statColl);


					bool isCollectProcessStatisticsActivated();

					QList<COntologyProcessingRequirement*> getOntologyProcessingRequirementList();
					COntologyClassificationItem* addOntologyProcessingRequirements(const QList<COntologyProcessingRequirement*>& reqList);

					CIndividualDependenceTrackingCollector* getIndividualDependenceTrackingCollector();
					COntologyClassificationItem* setIndividualDependenceTrackingCollector(CIndividualDependenceTrackingCollector* indiDepTrackColl);


					COntologyProcessingStepData* getClassificationProcessingStep();
					QTime* getInitializationTime();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology *onto;

					CClassificationStatisticsCollectionStrings* mClassificationStatCollStrings;
					QList<CClassificationCalculationStatisticsCollection*> mReuseStatCollList;


					CCallbackListExecuter classifiedCallbackList;

					qint64 currentCalculatingCount;

					bool remainingTests;

					CClassifierStatistics *statistics;
					CCalculationConfigurationExtension *config;

					bool mConfCollectProcessStatistics;

					QList<COntologyProcessingRequirement*> mRequirementList;
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
