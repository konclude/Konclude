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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCOMPLETED_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCOMPLETED_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringMessageData.h"
#include "CAnsweringCalculationStatisticsCollection.h"


// Other includes
#include "Reasoner/Query/CSatisfiableCalculationJob.h"
#include "Reasoner/Query/CJobSatisfiableCallbackContextData.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {


			/*! 
			 *
			 *		\class		CAnsweringMessageDataCalculationCompleted
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringMessageDataCalculationCompleted : public CAnsweringMessageData, public CJobSatisfiableCallbackContextData {
				// public methods
				public:
					//! Constructor
					enum CALCULATIONTYPE {
						SATISFIABLECALCULATION, SUPERCLASSSUBSUMPTIONCALCULATION, SUBCLASSSUBSUMPTIONCALCULATION, INDIVIDUALINSTANCECALCULATION, ITEMINSTANCECALCULATION, INSTANCEENTAILMENTCALCULATION, ITEMINSTANCECANDIDATEPROPAGATIONCALCULATION,
						VARIABLEBINDINGPPROPAGATIONCALCULATION, VARIABLEBINDINGCONFIRMATIONCALCULATION, VARIABLEBINDINGENTAILMENTCALCULATION, QUERYMATERIALIZATIONCALCULATION
					};


					CAnsweringMessageDataCalculationCompleted(CSatisfiableCalculationJob* calcJob, CALCULATIONTYPE calculationType);

					
					CALCULATIONTYPE getCalculationType();


					CSatisfiableCalculationJob* getCalculationJob();
					CJobSatisfiableCallbackContextData* getCalculationCallbackContextData();

					CAnsweringCalculationStatisticsCollection* getStatisticsCollection();
					CAnsweringMessageDataCalculationCompleted* setStatisticsCollection(CAnsweringCalculationStatisticsCollection* statColl);

				// protected methods
				protected:

				// protected variables
				protected:
					CALCULATIONTYPE mCalculationType;
					CSatisfiableCalculationJob* mCalculationJob;
					CJobSatisfiableCallbackContextData* mCalculationCallbackContextData;


					CAnsweringCalculationStatisticsCollection* mStatColl;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCOMPLETED_H
