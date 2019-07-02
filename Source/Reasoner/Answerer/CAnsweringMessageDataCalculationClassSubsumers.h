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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCLASSSUBSUMERS_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCLASSSUBSUMERS_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringMessageDataCalculationAdapter.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Answerer {


			/*! 
			 *
			 *		\class		CAnsweringMessageDataCalculationClassSubsumers
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringMessageDataCalculationClassSubsumers : public CAnsweringMessageDataCalculationAdapter {
				// public methods
				public:
					//! Constructor
					CAnsweringMessageDataCalculationClassSubsumers();

					CAnsweringMessageDataCalculationClassSubsumers* initAnsweringSubsumptionMessageData(CConcept* subsumedConcept, bool subsumedConceptNegation, CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* subsumerList);

					CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* getClassSubsumerList();
					CConcept* getSubsumedConcept();
					bool getSubsumedConceptNegation();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcept* mSubsumedConcept;
					bool mSubsumedConceptNegation;
					CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* mSubsumerList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCLASSSUBSUMERS_H
