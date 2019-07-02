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

#include "CSPARQLHttpConnectionHandlerProcessorFactory.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace SPARQL {


				CSPARQLHttpConnectionHandlerProcessorFactory::CSPARQLHttpConnectionHandlerProcessorFactory(CConfiguration* loaderConfig) {
					mLoaderConfig = loaderConfig;
					mHandlerReleased = false;
				}


				CSPARQLHttpConnectionHandlerProcessorFactory::~CSPARQLHttpConnectionHandlerProcessorFactory() {
					qDeleteAll(mContainerList);
				}

				CQtHttpConnectionHandler* CSPARQLHttpConnectionHandlerProcessorFactory::getConnectionHandler() {
					CSPARQLHttpConnectionHandlerProcessor* connHandler = nullptr;
					
					if (mHandlerReleased && mFreeHandlerList.isEmpty()) {
						for (QList<CSPARQLHttpConnectionHandlerProcessor*>::const_iterator it = mContainerList.constBegin(); it != mContainerList.constEnd(); ++it) {
							CSPARQLHttpConnectionHandlerProcessor* handler(*it);
							if (!handler->isHandlerBusy()) {
								if (!handler->isHandlerQueued()) {
									handler->setHandlerQueued(true);
									mFreeHandlerList.append(handler);
								}
							}
						}
						mHandlerReleased = false;
					}

					if (!connHandler && !mFreeHandlerList.isEmpty()) {
						connHandler = mFreeHandlerList.takeFirst();
						connHandler->setHandlerQueued(false);
					}


					if (!connHandler) {
						connHandler = new CSPARQLHttpConnectionHandlerProcessor(mLoaderConfig,this);
						connHandler->waitSynchronization();
						connHandler->setHandlerQueued(false);
						mContainerList.append(connHandler);
					}
					return connHandler;
				}


				CQtHttpConnectionHandlerReleaser* CSPARQLHttpConnectionHandlerProcessorFactory::releaseConnectionHandler(CQtHttpConnectionHandler* connectionHandler) {
					mHandlerReleased = true;
					return this;
				}


			}; // end namespace SPARQL 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
