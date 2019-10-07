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

#ifndef KONCLUDE_REASONER_PREPROCESS_CREQUIREMENTCONFIGPREPROCESSINGITEM_H
#define KONCLUDE_REASONER_PREPROCESS_CREQUIREMENTCONFIGPREPROCESSINGITEM_H

// Libraries includes
#include <QAtomicPointer>

// Namespace includes
#include "PreprocessSettings.h"
#include "CPreprocessingTestingItem.h"
#include "COntologyPreprocessingItem.h"
#include "CPreprocessingTestingStep.h"


// Other includes
#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Ontology/COntologyProcessingStepRequirement.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Ontology;

		namespace Preprocess {


			/*! 
			 *
			 *		\class		CRequirementConfigPreprocessingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRequirementConfigPreprocessingItem : public COntologyPreprocessingItem {
				// public methods
				public:
					//! Constructor
					CRequirementConfigPreprocessingItem();
					~CRequirementConfigPreprocessingItem();

					CRequirementConfigPreprocessingItem* initRequirementConfigPreprocessingItem(CConcreteOntology* ontology, CConfigurationBase* config);


					virtual COntologyPreprocessingItem* addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement);


					CCalculationConfigurationExtension* getCalculationConfiguration();


					CPreprocessingTestingStep* getTriplesMappingProcessingStep();
					bool isTriplesMappingStepFinished();
					bool isTriplesMappingStepRequired();
					bool areTriplesMappingStepProcessingRequirementSatisfied();

					CPreprocessingTestingStep* getActiveCountingProcessingStep();
					bool isActiveCountingStepFinished();
					bool isActiveCountingStepRequired();
					bool areActiveCountingStepProcessingRequirementSatisfied();


					CPreprocessingTestingStep* getBuildProcessingStep();
					bool isBuildStepFinished();
					bool isBuildStepRequired();
					bool areBuildStepProcessingRequirementSatisfied();


					CPreprocessingTestingStep* getPreprocessProcessingStep();
					bool isPreprocessStepFinished();
					bool isPreprocessStepRequired();
					bool arePreprocessStepProcessingRequirementSatisfied();


					CPreprocessingTestingStep* getTriplesIndexingProcessingStep();
					bool isTriplesIndexingStepFinished();
					bool isTriplesIndexingStepRequired();
					bool areTriplesIndexingStepProcessingRequirementSatisfied();



					bool areAllStepFinished();
					bool hasRemainingProcessingRequirements();


					CRequirementConfigPreprocessingItem* setPreprocessing(CPreprocessing* preprocessing);
					CPreprocessing* getPreprocessing();


					bool failAfterPreprocessing();
					bool failAfterBuilt();


				// protected methods
				protected:

				// protected variables
				protected:
					CPreprocessingTestingStep* mTriplesMappingProcessingStep;
					CPreprocessingTestingStep* mTriplesIndexingProcessingStep;
					CPreprocessingTestingStep* mActiveCountingProcessingStep;
					CPreprocessingTestingStep* mBuildProcessingStep;
					CPreprocessingTestingStep* mPreprocessProcessingStep;
					CPreprocessing* mPreprocessing;

					bool mFailAfterPreprocessing;
					bool mFailAfterBuilt;


					QList<CPreprocessingTestingStep*> mProcessingSteps;

					CCalculationConfigurationExtension* mCalculationConfig;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CREQUIREMENTCONFIGPREPROCESSINGITEM_H
