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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNICOMMANDPROCESSOR_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNICOMMANDPROCESSOR_H

// Libraries includes
#include <QTcpSocket>
#include <QHostAddress>

// Namespace includes
#include "JNISettings.h"

// Other includes
#include "Control/Interface/OWLlink/COWLlinkProcessor.h"

#include "Control/Command/CConfigManagerReader.h"

#include "Concurrent/Callback/CBlockingCallbackData.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Control {

		using namespace Command;

		namespace Interface {

			using namespace OWLlink;

			namespace JNI {

				/*! 
				 *
				 *		\class		CJNICommandProcessor
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CJNICommandProcessor : public COWLlinkProcessor {

					// public methods
					public:
						//! Constructor
						CJNICommandProcessor(CConfiguration* loaderConfig);

						//! Destructor
						virtual ~CJNICommandProcessor();


						CJNICommandProcessor* processCommand(CCommand* command, bool blockUntilProcessed = true);


						COntologyRevision* getNewlyCreatedAnonymousOntologyRevision();
						COntologyRevision* getNewOntologyRevision(COntologyRevision* prevOntRev);

						COntologyRevision* installNewOntologyRevision(COntologyRevision* ontRev);


						bool prepareOntologyQuerying(COntologyRevision* ontRev);


						CJNICommandProcessor* calculateOntologyQuery(CQuery* query);

						virtual CConfiguration *getConfiguration();


					// protected methods
					protected:
						virtual void threadStarted();
						virtual void threadStopped();

						virtual COWLlinkProcessor* initializeOWLlinkContent();
						virtual COWLlinkProcessor* concludeOWLlinkContent();



					// protected variables
					protected:


					// private methods
					private:

						CCommand* mProcessingCommand;

						CConfiguration* mLoaderConfig;



					// private variables
					private:

				};

			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNICOMMANDPROCESSOR_H
