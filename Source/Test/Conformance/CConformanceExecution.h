/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEEXECUTION_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEEXECUTION_H

// Libraries includes
#include <QString>
#include <QFileInfo>

// Namespace includes
#include "CConformanceTestCase.h"
#include "CConformanceRunConfig.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceExecution
			 *		\brief		One test case executed with one combination of a run
			 *					configuration and a reasoner (system) configuration.
			 *
			 *					A test that is assigned two run configurations and one system
			 *					configuration therefore produces two executions; the plan built
			 *					by the runner is the Cartesian product of the applicable run
			 *					configurations and system configurations.
			 */
			class CConformanceExecution {
				// public methods
				public:
					CConformanceExecution();

					//! Label of the system configuration used in reports and file names,
					//! always prefixed with "system-" (e.g. "system-default",
					//! "system-no-saturation").
					QString getSystemConfigName() const;

				// public variables
				public:
					CConformanceTestCase mTestCase;
					CConformanceRunConfig mRunConfig;
					QString mSystemConfigPath;		//!< '-c' file, empty when none applies
					QString mSuiteId;
					QString mSuiteName;
			};


			inline CConformanceExecution::CConformanceExecution() {
			}


			inline QString CConformanceExecution::getSystemConfigName() const {
				QString name;
				if (mSystemConfigPath.isEmpty()) {
					name = QString("default");
				} else {
					name = QFileInfo(mSystemConfigPath).completeBaseName();
				}
				name.replace(' ', '-');
				if (!name.startsWith("system-")) {
					name.prepend("system-");
				}
				return name;
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEEXECUTION_H
