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

#ifndef KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONPROGRAMLOADER_H
#define KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONPROGRAMLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CLoaderFactory.h"

// Other includes
#include "Test/Evaluation/CReasonerEvaluationExecutorThread.h"
#include "Test/Evaluation/CReasonerEvaluationTestsuite.h"

#include "Control/Command/CCommanderManagerThread.h"
#include "Control/Command/CCommanderManagerConfigType.h"

#include "Control/Command/Instructions/CInitializeConfigurationCommand.h"

#include "Config/CConfigDataReader.h"

#include "Control/Interface/OWLlink/COWLlinkProcessor.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;
	using namespace Config;
	using namespace Test::Evaluation;

	namespace Control {

		using namespace Command;
		using namespace Instructions;
		using namespace Interface::OWLlink;

		namespace Loader {

			/*! 
			 *
			 *		\class		CReasonerEvaluationProgramLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationProgramLoader : public CLoader, public COWLlinkProcessor {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationProgramLoader();

					//! Destructor
					virtual ~CReasonerEvaluationProgramLoader();


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
					QString mProgramFileString;
					CConfiguration* mLoaderConfig;
					CCommanderManagerThread* mReasonerCommander;
					CCommanderManagerThread* mDeleteReasonerCommander;

					QSemaphore mBlockingSem;
					bool mCloseAfterOutput;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONPROGRAMLOADER_H
