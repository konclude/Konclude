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

#ifndef KONCLUDE_Control_LOADER_COWLLINKBATCHFILELOADER_H
#define KONCLUDE_Control_LOADER_COWLLINKBATCHFILELOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>
#include <QTime>
//#include <QTest>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CLoaderFactory.h"

// Other includes
#include "Control/Interface/OWLlink/COWLlinkProcessor.h"
#include "Control/Interface/OWLlink/COWLLinkRecordInterpreter.h"

#include "Control/Command/CConfigManagerReader.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;

	namespace Control {

		using namespace Interface::OWLlink;
		using namespace Command;

		namespace Loader {

			/*! 
			 *
			 *		\class		COWLlinkBatchFileLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COWLlinkBatchFileLoader : public CLoader, public COWLlinkProcessor {
				// public methods
				public:
					//! Constructor
					COWLlinkBatchFileLoader();

					//! Destructor
					virtual ~COWLlinkBatchFileLoader();


					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();



				// protected methods
				protected:
					virtual COWLlinkProcessor *initializeOWLlinkContent();
					virtual COWLlinkProcessor *concludeOWLlinkContent();

					virtual CConfiguration *getConfiguration();

				// protected variables
				protected:
					QString reqFileString;
					QString resFileString;

					bool directConsoleOutput;
					bool directErrorOutput;
					bool closeAfterOutput;
					bool blockUntilProcessed;
					QTime mResponseTime;
					QSemaphore blockingSem;
					COWLLinkRecordInterpreter *owllinkInterpreter;

					CConfiguration *loaderConfig;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_LOADER_COWLLINKBATCHFILELOADER_H
