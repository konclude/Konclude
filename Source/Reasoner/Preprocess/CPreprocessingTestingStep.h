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

#ifndef KONCLUDE_REASONER_PREPROCESS_CPREPROCESSINGTESTINGSTEP_H
#define KONCLUDE_REASONER_PREPROCESS_CPREPROCESSINGTESTINGSTEP_H

// Libraries includes


// Namespace includes
#include "PreprocessSettings.h"

// Other includes
#include "Reasoner/Ontology/COntologyProcessingRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingStepData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {


			/*! 
			 *
			 *		\class		CPreprocessingTestingStep
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPreprocessingTestingStep {
				// public methods
				public:
					enum TESTINGSTEPTYPE {
						BUILDPREPROCESSINGSTEP, PREPROCESSPREPROCESSINGSTEP, ACTIVECOUNTINGPREPROCESSINGSTEP, TRIPLESMAPPINGPREPROCESSINGSTEP, TRIPLESINDEXINGPREPROCESSINGSTEP
					};

					//! Constructor
					CPreprocessingTestingStep(TESTINGSTEPTYPE testingStepType, COntologyProcessingStepData* ontProcStepData, COntologyPreprocessingItem* preproItem);

					COntologyPreprocessingItem* getOntologyPreprocessingItem();
					CPreprocessingTestingStep* setOntologyPreprocessingItem(COntologyPreprocessingItem* preproItem);

					bool hasRequirements();
					bool isStepFinished();
					bool areStepProcessingRequirementSatisfied();
					CPreprocessingTestingStep* setStepFinished(bool finished);
					QList<COntologyProcessingRequirement*>* getRequirementList();
					COntologyProcessingStepData* getProcessingStepData();

					CPreprocessingTestingStep* setProcessingStepData(COntologyProcessingStepData* processingData);
					CPreprocessingTestingStep* addProcessingRequirement(COntologyProcessingRequirement* processingRequirement);
					CPreprocessingTestingStep* submitRequirementsUpdate(cint64 flags = COntologyProcessingStatus::PSSUCESSFULL, bool clearRequirements = true);

					TESTINGSTEPTYPE getPreprocessingTestingType();

				// protected methods
				protected:

				// protected variables
				protected:
					TESTINGSTEPTYPE mTestingStepType;
					COntologyPreprocessingItem* mOntologyPreproItem;
					COntologyProcessingStepData* mProcessingStepData;
					QList<COntologyProcessingRequirement*> mRequirementStepDataList;
					bool mStepFinished;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CPREPROCESSINGTESTINGSTEP_H
