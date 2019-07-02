/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CQtHttpConnectionHandlerPooledFactory.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {

			CQtHttpConnectionHandlerPooledFactory::CQtHttpConnectionHandlerPooledFactory(CQtHttpConnectionHandlerFactory* factory) {
				mMaxHandlerCount = -1;
			}


			CQtHttpConnectionHandler* CQtHttpConnectionHandlerPooledFactory::getConnectionHandler() {
				CQtHttpPooledConnectionHandler* handler = nullptr;
				if (!mFreeHandlerList.isEmpty()) {
					handler = mFreeHandlerList.takeFirst();
					mBusyHandlerList.append(handler);
				} else {
					cint64 busyHandlerCount = mBusyHandlerList.count();
					for (cint64 i = 0; i < busyHandlerCount; ++i) {
						CQtHttpPooledConnectionHandler* tmpHandler = mBusyHandlerList.takeFirst();
						if (tmpHandler->isHandlerBusy()) {
							mBusyHandlerList.append(tmpHandler);
						} else {
							if (!handler) {
								mBusyHandlerList.append(tmpHandler);
								handler = tmpHandler;
							} else {
								mFreeHandlerList.append(tmpHandler);
							}
						}
					}
				}
				if (handler) {
					handler->setHandlerBusy();
				}
				return handler;
			}


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

