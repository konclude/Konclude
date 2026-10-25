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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERUNCONFIG_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERUNCONFIG_H

// Libraries includes
#include <QString>


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceRunConfig
			 *		\brief		Execution settings of the test harness for one run of a
			 *					test: timeout and repetition count.
			 *
			 *					A run configuration is *not* the reasoner configuration that
			 *					is handed to the executed system with '-c'; that one is a
			 *					CConformanceSystemConfig / kt:usesTestSystemConfig assignment. Run
			 *					configurations are declared as kt:RunConfig individuals in a
			 *					control ontology and assigned to tests or suites with
			 *					kt:usesTestRunConfig. Without an assignment the built-in default
			 *					configuration (id "default") is used, whose timeout and
			 *					repetition count come from Konclude.Conformance.TimeoutSeconds
			 *					and Konclude.Conformance.Repeat.
			 */
			class CConformanceRunConfig {
				// public methods
				public:
					CConformanceRunConfig();

					//! True for the built-in configuration that is used when no
					//! kt:usesTestRunConfig assignment exists.
					bool isBuiltinDefault() const;

					//! Label used in reports and in per-test output file names, always
					//! prefixed with "run-" (e.g. "run-default", "run-repeat2"). Falls
					//! back to the local part of mId when no kt:runConfigName was given.
					QString getDisplayName() const;

				// public variables
				public:
					QString mId;				//!< individual IRI of the configuration
					QString mName;				//!< optional human-readable name (kt:runConfigName)
					qint64 mTimeoutSeconds;		//!< 0 => runner default
					qint64 mRepeatCount;		//!< 0 => runner default
			};


			inline CConformanceRunConfig::CConformanceRunConfig()
					: mTimeoutSeconds(0), mRepeatCount(0) {
			}


			inline bool CConformanceRunConfig::isBuiltinDefault() const {
				return mId == QString("default");
			}


			inline QString CConformanceRunConfig::getDisplayName() const {
				QString name;
				if (mId.isEmpty() || isBuiltinDefault()) {
					name = QString("default");
				} else if (!mName.isEmpty()) {
					name = mName;
				} else {
					int colonIndex = mId.lastIndexOf(':');
					name = colonIndex >= 0 ? mId.mid(colonIndex + 1) : mId;
				}
				name.replace(' ', '-');
				if (!name.startsWith("run-")) {
					name.prepend("run-");
				}
				return name;
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERUNCONFIG_H
