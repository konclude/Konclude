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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTRESPONSEOWLLINK_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTRESPONSEOWLLINK_H

// Libraries includes
#include <QDomElement>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationRequestResponse.h"


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
			 *		\class		CReasonerEvaluationRequestResponseOWLlink
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationRequestResponseOWLlink : public CReasonerEvaluationRequestResponse {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationRequestResponseOWLlink();

					CReasonerEvaluationRequestResponseOWLlink* setRequestNode(const QDomElement& reqNode);
					CReasonerEvaluationRequestResponseOWLlink* setModifiedNode(const QDomElement& reqNode);
					CReasonerEvaluationRequestResponseOWLlink* setResponseNode(const QDomElement& resNode);

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

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTRESPONSEOWLLINK_H
