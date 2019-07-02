/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CEventThread.h"


namespace Konclude {

	namespace Concurrent {


		CEventThread::CEventThread() : CThread("EventThread") {
			mHandleEventsCountPerStep = 10;
			mHandleEventStopped = false;
			mEventSignalized = false;
			mLastProcessingStartedTag = 0;
			mLastProcessingStartRequestTag = 0;
			mEventProcessingStopped = true;
			threadContext = nullptr;
		}

		CEventThread::~CEventThread() {
		}


		CEventHandler* CEventThread::createInstalledEventHandler(CEventProcessor* eventProcessor) {
			CQueuedLinkedEventHandler* eventHandler = new CQueuedLinkedEventHandler(eventProcessor,this);
			mAddEventHandlerSyncMutex.lock();
			CXLinker<CEventHandler*>* eventHandlerLinker = new CXLinker<CEventHandler*>(eventHandler,mEventHandlerLinker);
			mEventHandlerLinker = eventHandlerLinker;
			mAddEventHandlerSyncMutex.unlock();
			return eventHandler;
		}

		CThreadActivator* CEventThread::signalizeEvent() {
			mEventSignalized = true;
			if (mEventProcessingStopped) {
				// post processing event
				if (mLastProcessingStartedTag == mLastProcessingStartRequestTag) {
					++mLastProcessingStartRequestTag;
					postEvent(new CHandleEventsEvent());
				}
			}
			return this;
		}


		bool CEventThread::processControlEvents(QEvent::Type type, CControlEvent *event) {
			if (CThread::processControlEvents(type,event)) {
				return true;
			} else {
				if (type == CHandleEventsEvent::EVENTTYPE) {
					mLastProcessingStartedTag = mLastProcessingStartRequestTag;
					handleEvents();
					return true;
				}
			}
			return false;
		}

		bool CEventThread::handleEvents(double minEventPriority, cint64 maxHandleEventCount) {
			bool handleNextRound = true;
			mEventProcessingStopped = false;
			bool roundEventProcessed = false;
			while (handleNextRound && !mHandleEventStopped) {
				roundEventProcessed = false;
				CXLinker<CEventHandler*>* eventHandlerLinkerIt = mEventHandlerLinker;
				if (eventHandlerLinkerIt) {
					while (maxHandleEventCount != 0 && !mHandleEventStopped) {

						CEventHandler* eventHandler = eventHandlerLinkerIt->getData();
						if (eventHandler->needEventProcessing(minEventPriority)) {
							cint64 maxHandleEventCountStep = mHandleEventsCountPerStep;
							if (maxHandleEventCount > 0) {
								if (maxHandleEventCountStep > maxHandleEventCount) {
									maxHandleEventCountStep = maxHandleEventCount;
								}
							}
							cint64 handledEventCount = eventHandler->handleEvents(threadContext,minEventPriority,maxHandleEventCountStep);
							if (maxHandleEventCount > 0) {
								maxHandleEventCount -= handledEventCount;
							}
							roundEventProcessed = handledEventCount > 0;
						}
						if (roundEventProcessed) {
							mEventProcessingStopped = false;
						}
						// round robin
						eventHandlerLinkerIt = eventHandlerLinkerIt->getNext();
						if (!eventHandlerLinkerIt) {
							if (mEventSignalized || roundEventProcessed) {
								handleNextRound = true;
								mEventSignalized = false;
							} else {
								if (!mEventProcessingStopped) {
									handleNextRound = true;
									mEventProcessingStopped = true;
									mEventSignalized = false;
								} else {
									handleNextRound = false;
								}
							}
							eventHandlerLinkerIt = mEventHandlerLinker;
						}
					}
				} else {
					handleNextRound = false;
				}
			}
			return true;
		}


	}; // end namespace Concurrent

}; // end namespace Konclude

