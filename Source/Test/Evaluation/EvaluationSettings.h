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

#ifndef KONCLUDE_TEST_EVALUATION_TESTSETTINGS
#define KONCLUDE_TEST_EVALUATION_TESTSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes


// Logger includes


namespace Konclude {

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\file		TestSettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */


			// forward declarations

			class CReasonerEvaluationGNUPlotPlotter;
			class CReasonerEvaluationHighchartPlotter;
			class CReasonerEvaluationAnalyseContext;





			// Custom Events >= 2000
			const QEvent::Type EVENTREASONEREVALUATIONTESTSUITE		= (QEvent::Type)3002;
			const QEvent::Type EVENTREASONEREVALUATIONSTART			= (QEvent::Type)3003;
			const QEvent::Type EVENTREASONEREVALUATIONNEXT			= (QEvent::Type)3004;
			const QEvent::Type EVENTREASONEREVALUATIONPROGRAM		= (QEvent::Type)3005;
			const QEvent::Type EVENTREASONEREVALUATIONCHANGEDFILE	= (QEvent::Type)3006;
			const QEvent::Type EVENTREASONEREVALUATIONUPDATE		= (QEvent::Type)3007;


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude


#endif // end KONCLUDE_TEST_EVALUATION_TESTSETTINGS
