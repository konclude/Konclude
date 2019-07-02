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

#ifndef KONCLUDE_REASONER_QUERY_CJOBSATISFIABLECALLBACKCONTEXTDATA_H
#define KONCLUDE_REASONER_QUERY_CJOBSATISFIABLECALLBACKCONTEXTDATA_H

// Libraries includes


// Namespace includes
#include "CSatisfiableCalculationJob.h"
#include "CJobSatisfiableResult.h"

// Other includes
#include "Concurrent/Callback/CCallbackDataContext.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CJobSatisfiableCallbackContextData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CJobSatisfiableCallbackContextData : public CJobSatisfiableResult, public CCallbackDataContext {
				// public methods
				public:
					//! Constructor
					CJobSatisfiableCallbackContextData(CSatisfiableCalculationJob *satCalcJob);

					//! Destructor
					virtual ~CJobSatisfiableCallbackContextData();

					virtual CSatisfiableCalculationJob *getSatisfiableCalculationJob();

					virtual bool isSatisfiable();
					virtual CJobSatisfiableCallbackContextData *setSatisfiable(bool satis);

					virtual bool hasCalculationError();
					virtual CJobSatisfiableCallbackContextData* setCalculationError(bool error, cint64 errorCode);
					virtual cint64 getCalculationErrorCode();


				// protected methods
				protected:

				// protected variables
				protected:
					CSatisfiableCalculationJob* mSatCalcJob;
					bool mSatisfiable;
					bool mCalcError;
					cint64 mErrorCode;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CJOBSATISFIABLECALLBACKCONTEXTDATA_H
