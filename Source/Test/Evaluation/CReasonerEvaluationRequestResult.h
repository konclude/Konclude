/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTRESULT_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTRESULT_H

// Libraries includes


// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationRequestResponse.h"


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
			 *		\class		CReasonerEvaluationRequestResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationRequestResult {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationRequestResult();

					~CReasonerEvaluationRequestResult();

					CReasonerEvaluationRequestResult* addResponse(CReasonerEvaluationRequestResponse* response, bool initRequestResponse);
					QList<CReasonerEvaluationRequestResponse*>* getResponseList();

					cint64 getResponseTime();
					bool getResponsedInTime();

					CReasonerEvaluationRequestResult* setResponseTime(cint64 responseTime);
					CReasonerEvaluationRequestResult* setResponsedInTime(bool responsedInTime);

					bool hasSucessfullReasonerCommunication();
					CReasonerEvaluationRequestResult* setSucessfullReasonerCommunication(bool sucessfull);

				// protected methods
				protected:

				// protected variables
				protected:
					QList<CReasonerEvaluationRequestResponse*> mInitResponseList;
					QList<CReasonerEvaluationRequestResponse*> mResponseList;
					cint64 mResponseTime;
					bool mResponsedInTime;
					bool mResponseError;
					bool mSucessfullCommunication;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTRESULT_H
