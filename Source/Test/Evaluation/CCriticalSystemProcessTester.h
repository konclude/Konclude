/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_TEST_EVALUATION_CCRITICALSYSTEMPROCESSTESTER_H
#define KONCLUDE_TEST_EVALUATION_CCRITICALSYSTEMPROCESSTESTER_H

// Libraries includes
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QObject>

// Namespace includes
#include "EvaluationSettings.h"


// Other includes
#include "Config/CConfigDataReader.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CCriticalSystemProcessTester
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCriticalSystemProcessTester {

				// public methods
				public:
					//! Constructor
					CCriticalSystemProcessTester(CConfiguration *config);


					bool hasCriticalSystemProcesses();


				// protected methods
				protected:

				// protected variables
				protected:
					CConfiguration* mConfig;

					QString mTestScriptString;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CCRITICALSYSTEMPROCESSTESTER_H
