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

#ifndef KONCLUDE_CONTROL_LOADER_CCONFORMANCELOADER_H
#define KONCLUDE_CONTROL_LOADER_CCONFORMANCELOADER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CLoaderFactory.h"

// Other includes
#include "Test/Conformance/CConformanceRunner.h"

#include "Config/CConfigDataReader.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


namespace Konclude {

	using namespace Logger;
	using namespace Config;
	using namespace Test::Conformance;

	namespace Control {

		namespace Loader {

			/*!
			 *		\class		CConformanceLoader
			 *		\brief		Loader for the 'conformance' subcommand: starts the
			 *					conformance test runner that discovers all *.kt.owl.xml
			 *					under the input directory, runs them and reports results.
			 */
			class CConformanceLoader : public CLoader {
				// public methods
				public:
					CConformanceLoader();
					virtual ~CConformanceLoader();

					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();
					//! Records the exit code (0 = green, non-zero = failing/unstable test run)
					//! so that main() can return it after a normal, logger-flushing shutdown.
					static void setLastTestExitCode(int code) { sLastTestExitCode = code; }
					static int getLastTestExitCode() { return sLastTestExitCode; }

				// protected variables
				protected:
					CConfiguration* mLoaderConfig;
				// private variables
				private:
					static int sLastTestExitCode;

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_CCONFORMANCELOADER_H
