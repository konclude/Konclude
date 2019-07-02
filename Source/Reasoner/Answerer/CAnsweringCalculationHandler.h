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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGCALCULATIONHANDLER_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGCALCULATIONHANDLER_H

// Libraries includes



// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringMessageDataObserver.h"


// Other includes
#include "Reasoner/Kernel/Cache/COccurrenceStatisticsCacheReader.h"

#include "Reasoner/Query/CSatisfiableCalculationJob.h"

#include "Concurrent/Callback/CCallbackDataContext.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Query;
		using namespace Kernel::Cache;

		namespace Answerer {



			/*! 
			 *
			 *		\class		CAnsweringCalculationHandler
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringCalculationHandler : public CAnsweringMessageDataObserver {
				// public methods
				public:
					//! Constructor
					CAnsweringCalculationHandler();

					//! Destructor
					virtual ~CAnsweringCalculationHandler();


					virtual CAnsweringCalculationHandler* postMessage(CAnsweringHandler* handler, CAnsweringMessageData* message) = 0;

					virtual CAnsweringCalculationHandler* postCalculation(CAnsweringHandler* handler, CSatisfiableCalculationJob* satCalcJob, CAnsweringMessageData* finishedMessage, CCallbackDataContext* callbackContext) = 0;

					virtual CAnsweringCalculationHandler* ensureRequirements(CAnsweringHandler* handler, CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& requirementList, CAnsweringMessageData* finishedMessage) = 0;

					virtual COccurrenceStatisticsCacheReader* getOccurrenceStatisticsCacheReader() = 0;

				// protected methods
				protected:

				// protected variables
				protected:



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGCALCULATIONHANDLER_H
