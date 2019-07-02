/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEVALUATEDCALLBACKCONTEXTDATA_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEVALUATEDCALLBACKCONTEXTDATA_H

// Libraries includes


// Namespace includes
#include "EvaluationSettings.h"


// Other includes
#include "Concurrent/Callback/CCallbackDataContext.h"

#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Callback;
	using namespace Utilities;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationEvaluatedCallbackContextData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationEvaluatedCallbackContextData : public CCallbackDataContext {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationEvaluatedCallbackContextData(bool failed);
					CReasonerEvaluationEvaluatedCallbackContextData(bool runned, const QString& reasonerNameString, const QString& reasonerOutputDirString, cint64 testTimeout, cint64 testCount);


					bool getEvaluationRunned();
					bool getEvaluationFailed();
					CReasonerEvaluationEvaluatedCallbackContextData* getEvaluationRunned(bool runned);

					QString getReasonerNameString();
					QString getReasonerOutputDirectoryString();

					cint64 getTestTimeout();
					cint64 getTestCount();

				// protected methods
				protected:

				// protected variables
				protected:
					bool mEvaluationRunned;
					bool mEvaluationFailed;
					QString mReasonerNameString;
					QString mReasonerOutputDirectoryString;
					cint64 mTestTimeout;
					cint64 mTestCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEVALUATEDCALLBACKCONTEXTDATA_H
