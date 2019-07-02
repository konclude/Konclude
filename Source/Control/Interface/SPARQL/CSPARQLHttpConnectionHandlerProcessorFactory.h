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

#ifndef KONCLUDE_CONTROL_INTERFACE_SPARQL_CSPARQLHTTPCONNECTIONHANDLERPROCESSORFACTORY_H
#define KONCLUDE_CONTROL_INTERFACE_SPARQL_CSPARQLHTTPCONNECTIONHANDLERPROCESSORFACTORY_H

// Libraries includes
#include <QList>
#include <QListIterator>

// Namespace includes
#include "SPARQLSettings.h"
#include "CSPARQLHttpConnectionHandlerProcessor.h"

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

			namespace SPARQL {

				/*! 
				 *
				 *		\class		CSPARQLHttpConnectionHandlerProcessorFactory
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSPARQLHttpConnectionHandlerProcessorFactory : public CQtHttpConnectionHandlerFactory, public CQtHttpConnectionHandlerReleaser {

					// public methods
					public:
						//! Constructor
						CSPARQLHttpConnectionHandlerProcessorFactory(CConfiguration* loaderConfig);

						//! Destructor
						virtual ~CSPARQLHttpConnectionHandlerProcessorFactory();

						virtual CQtHttpConnectionHandler* getConnectionHandler();
						virtual CQtHttpConnectionHandlerReleaser* releaseConnectionHandler(CQtHttpConnectionHandler* connectionHandler);

					// protected methods
					protected:

					// protected variables
					protected:
						CConfiguration* mLoaderConfig;
						QList<CSPARQLHttpConnectionHandlerProcessor*> mContainerList;
						QList<CSPARQLHttpConnectionHandlerProcessor*> mFreeHandlerList;
						bool mHandlerReleased;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace SPARQL 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_SPARQL_CSPARQLHTTPCONNECTIONHANDLERPROCESSORFACTORY_H
