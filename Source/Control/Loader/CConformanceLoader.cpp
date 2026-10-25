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

#include "CConformanceLoader.h"

#include <QCoreApplication>


namespace Konclude {

	namespace Control {

		namespace Loader {


			//! Class-level exit code shared with main() for CI return semantics.
			int CConformanceLoader::sLastTestExitCode = 0;


			CConformanceLoader::CConformanceLoader() {
				mLoaderConfig = nullptr;
				sLastTestExitCode = 0;
			}


			CConformanceLoader::~CConformanceLoader() {
			}


			CLoader *CConformanceLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				mLoaderConfig = config;
				return this;
			}


			CLoader *CConformanceLoader::load() {
				QString inputDirectory = CConfigDataReader::readConfigString(mLoaderConfig, "Konclude.Conformance.InputDirectory", "Tests/cases");
				QString outputFile = CConfigDataReader::readConfigString(mLoaderConfig, "Konclude.Conformance.OutputFile", "");
				QString binaryFile = CConfigDataReader::readConfigString(mLoaderConfig, "Konclude.Conformance.BinaryFile", "");
				QString knownIssuesFile = CConfigDataReader::readConfigString(mLoaderConfig, "Konclude.Conformance.KnownIssuesFile", "");
				// Default reasoner configuration file for all test cases (e.g. one that switches
				// Konclude.Calculation.Optimization.AllowBackendNeighbourExpansionBlocking off).
				// A test-specific kt:usesTestSystemConfig assertion in the control ontology takes precedence.
				QString configFile = CConfigDataReader::readConfigString(mLoaderConfig, "Konclude.Conformance.ConfigFile", "");
				cint64 jobCount = CConfigDataReader::readConfigInteger(mLoaderConfig, "Konclude.Conformance.Jobs", 1);
				cint64 repeatCount = CConfigDataReader::readConfigInteger(mLoaderConfig, "Konclude.Conformance.Repeat", 1);
				cint64 timeoutSeconds = CConfigDataReader::readConfigInteger(mLoaderConfig, "Konclude.Conformance.TimeoutSeconds", 60);
				cint64 progressIntervalSeconds = CConfigDataReader::readConfigInteger(mLoaderConfig, "Konclude.Conformance.ProgressInterval", 3);

				CConformanceRunner runner;
				runner.setInputDirectory(inputDirectory);
				runner.setOutputFile(outputFile);
				runner.setBinaryFile(binaryFile);
				runner.setKnownIssuesFile(knownIssuesFile);
				runner.setConfigFile(configFile);
				runner.setJobCount(jobCount);
				runner.setRepeatCount(repeatCount);
				runner.setTimeoutSeconds(timeoutSeconds);
				runner.setProgressIntervalSeconds(progressIntervalSeconds);
				int failures = runner.run();

				// Flush the logger synchronously so the console report is complete, then
				// stop the Qt event loop so control returns to main(), which performs a
				// normal shutdown (draining the logger) and returns the recorded exit code.
				// A non-zero code is propagated through CConformanceLoader::getLastTestExitCode()
				// so a failing/unstable run can make CI turn red.
				CLogger::getInstance()->waitSynchronization();
				setLastTestExitCode(failures > 0 ? 1 : 0);
				QCoreApplication::exit();

				return this;
			}


			CLoader *CConformanceLoader::exit() {
				return this;
			}


		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
