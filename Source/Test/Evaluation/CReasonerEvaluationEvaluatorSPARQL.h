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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEVALUATORSPARQL_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEVALUATORSPARQL_H

// Libraries includes
#include <QTextStream>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationTerminationResult.h"
#include "CReasonerEvaluationRequestResult.h"
#include "CReasonerEvaluationEvaluator.h"
#include "CReasonerEvaluationRequestResponseSPARQL.h"

// Other includes
#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationEvaluatorSPARQL
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationEvaluatorSPARQL : public CReasonerEvaluationEvaluator {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationEvaluatorSPARQL();

					//! Destructor
					virtual ~CReasonerEvaluationEvaluatorSPARQL();


					virtual bool evaluateResults(CReasonerEvaluationRequestResult* requestResult, CReasonerEvaluationTerminationResult* terminationResult, const QString& inputFileString, const QString& responseFileString, CConfiguration* config) override;


				// protected methods
				protected:
					virtual bool evaluateResults(CReasonerEvaluationRequestResult* requestResult, CReasonerEvaluationTerminationResult* terminationResult, const QString& inputFileString, const QString& responseFileString, CConfiguration* config, bool includeResponses, bool includeOutputs);



				// protected variables
				protected:
					QDomDocument createResponseDocument();

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEVALUATORSPARQL_H
