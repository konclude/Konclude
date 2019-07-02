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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTSELECTOR_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTSELECTOR_H

// Libraries includes
#include <QString>


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
			 *		\class		CReasonerEvaluationGroupRequestSelector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationGroupRequestSelector : public CLinkerBase<CReasonerEvaluationGroupRequestSelector*,CReasonerEvaluationGroupRequestSelector> {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationGroupRequestSelector();


					virtual bool isGroupSelected(const QString& groupString) = 0;
					virtual bool isRequestSelected(const QString& groupString, const QString& requestString) = 0;

					QString getNameString();
					QString getOutputString();

					CReasonerEvaluationGroupRequestSelector* setNameString(const QString& name);
					CReasonerEvaluationGroupRequestSelector* setOutputString(const QString& output);

				// protected methods
				protected:

				// protected variables
				protected:
					QString mNameString;
					QString mOutputString;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTSELECTOR_H
