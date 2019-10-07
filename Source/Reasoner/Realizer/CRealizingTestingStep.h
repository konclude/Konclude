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

#ifndef KONCLUDE_REASONER_REALIZER_CREALIZINGTESTINGSTEP_H
#define KONCLUDE_REASONER_REALIZER_CREALIZINGTESTINGSTEP_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"

// Other includes
#include "Reasoner/Ontology/COntologyProcessingRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingStepData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		CRealizingTestingStep
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRealizingTestingStep {
				// public methods
				public:
					enum TESTINGSTEPTYPE { INITREALIZEPROCESSINGSTEP = 0, REALIZECONCEPTPROCESSINGSTEP = 1, REALIZEROLEPROCESSINGSTEP = 2, REALIZESAMEINDIVIDUALSPROCESSINGSTEP = 3 };
					static const cint64 TESTINGSTEPTYPECOUNT = 4;
					//! Constructor
					CRealizingTestingStep(TESTINGSTEPTYPE testingStepType, COntologyProcessingStepData* ontProcStepData, COntologyRealizingItem* preproItem);

					COntologyRealizingItem* getOntologyRealizingItem();
					CRealizingTestingStep* setOntologyRealizingItem(COntologyRealizingItem* preproItem);

					bool hasRequirements();
					bool isStepFinished();
					bool areStepProcessingRequirementSatisfied();
					CRealizingTestingStep* setStepFinished(bool finished);
					QList<COntologyProcessingRequirement*>* getRequirementList();
					COntologyProcessingStepData* getProcessingStepData();

					CRealizingTestingStep* setProcessingStepData(COntologyProcessingStepData* processingData);
					CRealizingTestingStep* addProcessingRequirement(COntologyProcessingRequirement* processingRequirement);
					CRealizingTestingStep* addRequirementProcessingData(COntologyRealizingDynamicRequirmentProcessingData* procData);
					CRealizingTestingStep* removeRequirementProcessingData(COntologyRealizingDynamicRequirmentProcessingData* procData);

					CRealizingTestingStep* submitRequirementsUpdate(cint64 flags = COntologyProcessingStatus::PSSUCESSFULL, bool clearRequirements = true, bool doCallbacks = true);

					TESTINGSTEPTYPE getRealizingTestingType();

				// protected methods
				protected:

				// protected variables
				protected:
					TESTINGSTEPTYPE mTestingStepType;
					COntologyRealizingItem* mOntologyPreproItem;
					COntologyProcessingStepData* mProcessingStepData;
					QList<COntologyProcessingRequirement*> mRequirementStepDataList;
					QSet<COntologyRealizingDynamicRequirmentProcessingData*> mRequirementProcDataSet;
					bool mStepFinished;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CREALIZINGTESTINGSTEP_H
