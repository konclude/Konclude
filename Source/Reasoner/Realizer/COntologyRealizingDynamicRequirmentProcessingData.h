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

#ifndef KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGDYNAMICREQUIRMENTPROCESSINGDATA_H
#define KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGDYNAMICREQUIRMENTPROCESSINGDATA_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COntologyRealizingDynamicRequirmentCallbackData.h"
#include "COntologyRealizingDynamicRequirmentProcessingStatistics.h"
#include "CRealizingTestingStep.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"


#include "Concurrent/Callback/CCallbackListExecuter.h"
#include "Concurrent/Callback/CCallbackData.h"

#include "Config/CConfigurationBase.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COntologyRealizingDynamicRequirmentProcessingData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyRealizingDynamicRequirmentProcessingData {
				// public methods
				public:
					//! Constructor
					COntologyRealizingDynamicRequirmentProcessingData(COntologyProcessingRequirement* procReq = nullptr, COntologyRealizingDynamicRequirmentCallbackData* callback = nullptr);

					COntologyRealizingDynamicRequirmentProcessingData* incProcessingItemCount(CRealizingTestingStep* realizingStep, cint64 incCount = 1);
					COntologyRealizingDynamicRequirmentProcessingData* decProcessingItemCount(CRealizingTestingStep* realizingStep, cint64 decCount = 1);
					
					COntologyRealizingDynamicRequirmentProcessingData* setCallbackData(COntologyRealizingDynamicRequirmentCallbackData* callbackData);
					COntologyRealizingDynamicRequirmentCallbackData* getCallbackData();

					cint64 getCurrentProcessingItemCount();
					bool hasCurrentProcessingItemCount();



					bool hasAssociatedRelizationTestingStepProcessingItemCount(CRealizingTestingStep* realizingStep);
					CRealizingTestingStep* getAssociatedRelizationTestingStep(CRealizingTestingStep::TESTINGSTEPTYPE type);
					COntologyRealizingDynamicRequirmentProcessingData* setAssociatedRelizationTestingStep(CRealizingTestingStep* step);
					COntologyRealizingDynamicRequirmentProcessingData* clearAssociatedRelizationTestingStep(CRealizingTestingStep::TESTINGSTEPTYPE type);

					COntologyProcessingRequirement* getProcessingRequirement();
					COntologyRealizingDynamicRequirmentProcessingData* setProcessingRequirement(COntologyProcessingRequirement* procReq);

					COntologyRealizingDynamicRequirmentProcessingStatistics* getStatistics();


				// protected methods
				protected:

				// protected variables
				protected:
					COntologyRealizingDynamicRequirmentProcessingStatistics* mStatistics;
					cint64 mProcessingItemCount;
					cint64 mProcessingItemInitialCount = 0;
					COntologyRealizingDynamicRequirmentCallbackData* mCallbackData;
					cint64 mAssRealStepProcessingCount[CRealizingTestingStep::TESTINGSTEPTYPECOUNT];
					CRealizingTestingStep* mAssRealStep[CRealizingTestingStep::TESTINGSTEPTYPECOUNT];
					COntologyProcessingRequirement* mProcReq;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGDYNAMICREQUIRMENTPROCESSINGDATA_H
