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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTRESPONSE_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTRESPONSE_H

// Libraries includes
#include <QDomElement>

// Namespace includes
#include "EvaluationSettings.h"


// Other includes
#include "Config/CConfiguration.h"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;
	using namespace Utilities::Container;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationRequestResponse
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationRequestResponse : public CLinkerBase<CReasonerEvaluationRequestResponse*,CReasonerEvaluationRequestResponse> {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationRequestResponse();

					CReasonerEvaluationRequestResponse* setTimedOut(bool timedOut);
					CReasonerEvaluationRequestResponse* setResponseError(bool responseError);
					CReasonerEvaluationRequestResponse* setResponseTime(cint64 responseTime);
					CReasonerEvaluationRequestResponse* setRequestNode(const QDomElement& reqNode);
					CReasonerEvaluationRequestResponse* setModifiedNode(const QDomElement& reqNode);
					CReasonerEvaluationRequestResponse* setResponseNode(const QDomElement& resNode);

					cint64 getResponseTime();
					bool isTimedOut();
					bool hasResponseError();
					QDomElement getRequestNode();
					QDomElement getResponseNode();
					QDomElement getModifiedNode();

				// protected methods
				protected:

				// protected variables
				protected:
					QDomElement mRequestNode;
					QDomElement mResponseNode;
					QDomElement mModifiedNode;
					cint64 mResponseTime;
					bool mTimedOut;
					bool mResponseError;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTRESPONSE_H
