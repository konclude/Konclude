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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGHANDLINGSTATISTICS_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGHANDLINGSTATISTICS_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		CAnsweringHandlingStatistics
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringHandlingStatistics {
				// public methods
				public:
					//! Constructor
					CAnsweringHandlingStatistics(CAnsweringHandlingStatistics *nextPrecompuationStatistics = 0);

					qint64 getTotalAnsweredQueriesCount();
					CAnsweringHandlingStatistics* incTotalAnsweredQueriesCount(qint64 incCount = 1);


					qint64 getExistentialQueryPartAbsorptionCount();
					CAnsweringHandlingStatistics* incExistentialQueryPartAbsorptionCount(qint64 incCount = 1);

					qint64 getCreatedComplexConceptItemsCount();
					CAnsweringHandlingStatistics* incCreatedComplexConceptItemsCount(qint64 incCount = 1);

					qint64 getComplexConceptItemsPossibleInstanceVerificationCount();
					CAnsweringHandlingStatistics* incComplexConceptItemsPossibleInstanceVerificationCount(qint64 incCount = 1);

					qint64 getComplexConceptItemsSatisfiabilityTestingCount();
					CAnsweringHandlingStatistics* incComplexConceptItemsSatisfiabilityTestingCount(qint64 incCount = 1);

					qint64 getRequestedRealizationRequirementCount();
					CAnsweringHandlingStatistics* incRequestedRealizationRequirementCount(qint64 incCount = 1);



					qint64 getComplexConceptItemsCandidatePropagationCount();
					CAnsweringHandlingStatistics* incComplexConceptItemsCandidatePropagationCount(qint64 incCount = 1);

					qint64 getComplexConceptItemsSubClassSubsumptionTestingCount();
					CAnsweringHandlingStatistics* incComplexConceptItemsSubClassSubsumptionTestingCount(qint64 incCount = 1);

					qint64 getComplexConceptItemsSuperClassSubsumptionTestingCount();
					CAnsweringHandlingStatistics* incComplexConceptItemsSuperClassSubsumptionTestingCount(qint64 incCount = 1);

					qint64 getExistentialQueryPartEntailmentTestingCount();
					CAnsweringHandlingStatistics* incExistentialQueryPartEntailmentTestingCount(qint64 incCount = 1);

					qint64 getExistentialQueryPartVariableBindingPropagationCount();
					CAnsweringHandlingStatistics* incExistentialQueryPartVariableBindingPropagationCount(qint64 incCount = 1);

					qint64 getExistentialQueryPartVariableBindingConfirmationCount();
					CAnsweringHandlingStatistics* incExistentialQueryPartVariableBindingConfirmationCount(qint64 incCount = 1);

					qint64 getComplexConceptAssertionEntailmentTestingCount();
					CAnsweringHandlingStatistics* incComplexConceptAssertionEntailmentTestingCount(qint64 incCount = 1);

					qint64 getComplexConceptSubClassAxiomEntailmentTestingCount();
					CAnsweringHandlingStatistics* incComplexConceptSubClassAxiomEntailmentTestingCount(qint64 incCount = 1);



					CAnsweringHandlingStatistics* resetValues();
					CAnsweringHandlingStatistics* appendStatistics(CAnsweringHandlingStatistics *statistics);

				// protected methods
				protected:

				// protected variables
				protected:
					qint64 mTotalAnsweredQueries;
					qint64 mAbsorbedExistentialQueryPartCount;
					qint64 mCreatedComplexConceptItemsCount;
					qint64 mComplexConceptItemsPossibleInstanceVerificationCount;
					qint64 mComplexConceptItemsSatisfiabilityTestingCount;

					qint64 mComplexConceptItemsCandidatePropagationCount;
					qint64 mComplexConceptItemsSubClassSubsumptionTestingCount;
					qint64 mComplexConceptItemsSuperClassSubsumptionTestingCount;
					qint64 mExistentialQueryPartEntailmentTestingCount;
					qint64 mExistentialQueryPartVariableBindingPropagationCount;
					qint64 mExistentialQueryPartVariableBindingConfirmationCount;
					qint64 mComplexConceptAssertionEntailmentTestingCount;
					qint64 mComplexConceptSubClassAxiomEntailmentTestingCount;
				
					qint64 mRequestedRealizationRequirementCount;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGHANDLINGSTATISTICS_H
