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

#ifndef KONCLUDE_REASONER_CONSISTISER_CPRECOMPUTATIONTESTINGSTEP_H
#define KONCLUDE_REASONER_CONSISTISER_CPRECOMPUTATIONTESTINGSTEP_H

// Libraries includes


// Namespace includes
#include "ConsistiserSettings.h"

// Other includes
#include "Reasoner/Ontology/COntologyProcessingRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingStepData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Consistiser {


			/*! 
			 *
			 *		\class		CPrecomputationTestingStep
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPrecomputationTestingStep {
				// public methods
				public:
					enum TESTINGSTEPTYPE { CONSISTENCYPRECOMPUTATIONSTEP, CYCLEPRECOMPUTATIONSTEP, SATURATIONPRECOMPUTATIONSTEP, INDIVIDUALPRECOMPUTATIONSTEP };

					//! Constructor
					CPrecomputationTestingStep(TESTINGSTEPTYPE testingStepType, COntologyProcessingStepData* processingStepData, COntologyPrecomputationItem* precompItem);

					COntologyPrecomputationItem* getOntologyPrecomputationItem();
					CPrecomputationTestingStep* setOntologyPrecomputationItem(COntologyPrecomputationItem* precompItem);

					bool hasRequirements();
					bool isStepFinished();
					bool areStepProcessingRequirementSatisfied();
					CPrecomputationTestingStep* setStepFinished(bool finished);
					QList<COntologyProcessingRequirement*>* getRequirementList();
					COntologyProcessingStepData* getProcessingStepData();

					CPrecomputationTestingStep* setProcessingStepData(COntologyProcessingStepData* processingData);
					CPrecomputationTestingStep* addProcessingRequirement(COntologyProcessingRequirement* processingRequirement);
					CPrecomputationTestingStep* submitRequirementsUpdate(cint64 flags, bool clearRequirements = true);
					CPrecomputationTestingStep* submitRequirementsUpdate(bool clearRequirements = true);

					TESTINGSTEPTYPE getPrecomputationTestingType();

				// protected methods
				protected:

				// protected variables
				protected:
					TESTINGSTEPTYPE mTestingStepType;
					COntologyPrecomputationItem* mOntologyPrecompItem;
					COntologyProcessingStepData* mProcessingStepData;
					QList<COntologyProcessingRequirement*> mRequirementStepDataList;
					bool mStepFinished;
					cint64 mCombineLastFlags;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CPRECOMPUTATIONTESTINGSTEP_H
