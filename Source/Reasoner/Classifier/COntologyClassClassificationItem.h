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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYCLASSCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYCLASSCLASSIFICATIONITEM_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "ClassifierSettings.h"
#include "CClassClassificationComputationItem.h"
#include "CClassifiedCallbackDataContext.h"
#include "CClassifierStatistics.h"
#include "CPrecomputedSaturationSubsumerExtractor.h"
#include "COntologyClassificationItem.h"

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
			 *		\class		COntologyClassClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyClassClassificationItem : public CClassConceptClassification, public COntologyClassificationItem {
				// public methods
				public:
					//! Constructor
					COntologyClassClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics = 0);

					//! Destructor
					virtual ~COntologyClassClassificationItem();

					virtual COntologyClassClassificationItem *initTaxonomyConcepts(CConcreteOntology *ontology, CTaxonomy *taxonomy);

					CPrecomputedSaturationSubsumerExtractor* getPrecomputedSaturationSubsumerExtractor(bool create = true);


					virtual CTaxonomy *getTaxonomy();
					virtual CTBox *getTBox();

					virtual QHash<CSatisfiableCalculationJob *, CClassClassificationComputationItem *> *getWorkItemHash();



					CClassConceptClassification* getClassConceptClassification();

					bool isTaxonomyConstructed();
					COntologyClassClassificationItem* submitTaxonomyConstructed();
					COntologyClassClassificationItem* setTaxonomyConstructionFailed();
					bool isTaxonomyConstructionFailed();


				// protected methods
				protected:

				// protected variables
				protected:
					CTaxonomy *tax;

					CPrecomputedSaturationSubsumerExtractor* mPrecSatSubsumerExtractor;

					QHash<CSatisfiableCalculationJob*, CClassClassificationComputationItem*> workItemHash;

					bool mTaxonomyConstructed;
					bool mTaxonomyConstructionFailed;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYCLASSCLASSIFICATIONITEM_H
