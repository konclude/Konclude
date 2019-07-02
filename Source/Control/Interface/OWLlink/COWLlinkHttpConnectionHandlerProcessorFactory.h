/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_CONTROL_INTERFACE_OWLLINK_COWLLINKHTTPCONNECTIONHANDLERPROCESSORFACTORY_H
#define KONCLUDE_CONTROL_INTERFACE_OWLLINK_COWLLINKHTTPCONNECTIONHANDLERPROCESSORFACTORY_H

// Libraries includes
#include <QList>
#include <QListIterator>

// Namespace includes
#include "OWLlinkSettings.h"
#include "COWLlinkHttpConnectionHandlerProcessor.h"

// Other includes
#include "Network/HTTP/CQtHttpConnectionHandlerFactory.h"
#include "Network/HTTP/CQtHttpConnectionHandlerReleaser.h"

#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


namespace Konclude {

	using namespace Config;

	using namespace Network::HTTP;
	using namespace Network::HTTP::Events;

	namespace Control {

		namespace Interface {

			namespace OWLlink {

				/*! 
				 *
				 *		\class		COWLlinkHttpConnectionHandlerProcessorFactory
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COWLlinkHttpConnectionHandlerProcessorFactory : public CQtHttpConnectionHandlerFactory, public CQtHttpConnectionHandlerReleaser {

					// public methods
					public:
						//! Constructor
						COWLlinkHttpConnectionHandlerProcessorFactory(CConfiguration* loaderConfig);

						//! Destructor
						virtual ~COWLlinkHttpConnectionHandlerProcessorFactory();

						virtual CQtHttpConnectionHandler* getConnectionHandler();
						virtual CQtHttpConnectionHandlerReleaser* releaseConnectionHandler(CQtHttpConnectionHandler* connectionHandler);

					// protected methods
					protected:

					// protected variables
					protected:
						CConfiguration* mLoaderConfig;
						QList<COWLlinkHttpConnectionHandlerProcessor*> mContainerList;
						QList<COWLlinkHttpConnectionHandlerProcessor*> mFreeHandlerList;
						bool mHandlerReleased;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace OWLlink 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_OWLLINK_COWLLINKHTTPCONNECTIONHANDLERPROCESSORFACTORY_H
