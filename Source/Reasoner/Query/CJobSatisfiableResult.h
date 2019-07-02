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

#ifndef KONCLUDE_REASONER_QUERY_CJOBSATISFIABLERESULT_H
#define KONCLUDE_REASONER_QUERY_CJOBSATISFIABLERESULT_H

// Libraries includes


// Namespace includes
#include "CSatisfiableCalculationJob.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CJobSatisfiableResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CJobSatisfiableResult {
				// public methods
				public:
					//! Constructor
					CJobSatisfiableResult();

					//! Destructor
					virtual ~CJobSatisfiableResult();

					virtual CSatisfiableCalculationJob *getSatisfiableCalculationJob() = 0;

					virtual bool isSatisfiable() = 0;

					virtual bool hasCalculationError() = 0;
					virtual cint64 getCalculationErrorCode() = 0;


				// protected methods
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CJOBSATISFIABLERESULT_H
