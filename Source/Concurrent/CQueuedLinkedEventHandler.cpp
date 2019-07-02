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

#include "CQueuedLinkedEventHandler.h"


namespace Konclude {

	namespace Concurrent {


		CQueuedLinkedEventHandler::CQueuedLinkedEventHandler(CEventProcessor* eventProcessor, CThreadActivator* threadActivator) : CEventLinkerQueue(threadActivator) {
			mUnprocessedEvents = nullptr;
			mEventProcessor = eventProcessor;
		}

		CQueuedLinkedEventHandler::~CQueuedLinkedEventHandler() {
		}


		cint64 CQueuedLinkedEventHandler::handleEvents(CContext* handlerContext, double minEventPriority, cint64 maxProcessingCount) {
			cint64 handledEvents = 0;
			CEventLinker* tmpEventLinker = nullptr;
			for (cint64 processedCount = 0; processedCount < maxProcessingCount || maxProcessingCount == -1; ++processedCount) {
				if (!mUnprocessedEvents || mUnprocessedEvents->getData()->getEventChannelPriority() < minEventPriority) {
					mUnprocessedEvents = takeEvents(mUnprocessedEvents,minEventPriority);
				}
				if (!mUnprocessedEvents || mUnprocessedEvents->getData()->getEventChannelPriority() < minEventPriority) {
					break;
				}
				tmpEventLinker = mUnprocessedEvents;
				mUnprocessedEvents = mUnprocessedEvents->getNextEventLinker();
				CEvent* eventX = tmpEventLinker->getData();
				mEventProcessor->processEvent(eventX,handlerContext);
				++handledEvents;
			}
			return handledEvents;
		}

		bool CQueuedLinkedEventHandler::needEventProcessing(double minEventPriority) {
			return (mUnprocessedEvents && mUnprocessedEvents->getData()->getEventChannelPriority() > minEventPriority) || hasEvents(minEventPriority);
		}



	}; // end namespace Concurrent

}; // end namespace Konclude

