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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDATAVALUE_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDATAVALUE_H

// Libraries includes


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
			 *		\class		CReasonerEvaluationDataValue
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationDataValue : public CLinkerBase<CReasonerEvaluationDataValue*,CReasonerEvaluationDataValue> {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationDataValue();

					//! Destructor
					virtual ~CReasonerEvaluationDataValue();

					virtual CReasonerEvaluationDataValue* getCopy() = 0;
					virtual QString generateString() = 0;

					static const cint64 REDFDATANOTAVAILABLE		= 0x0001;
					static const cint64 REDFDATAERROROCCURED		= 0x0002;

					bool hasDataFlags(cint64 flags, bool testPartial);
					CReasonerEvaluationDataValue* setDataFlags(cint64 flags);
					CReasonerEvaluationDataValue* clearDataFlags(cint64 flags);
					CReasonerEvaluationDataValue* setDataFlags(cint64 flags, bool value);

					bool hasErrorOccuredFlag();
					bool hasDataNotAvailableFlag();

					CReasonerEvaluationDataValue* setErrorOccuredFlag(bool value = true);
					CReasonerEvaluationDataValue* setDataNotAvailableFlag(bool value = true);

					cint64 getDataFlags();

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mDataFlags;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDATAVALUE_H
