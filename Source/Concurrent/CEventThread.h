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

#ifndef KONCLUDE_CONCURRENT_CEVENTTHREAD_H
#define KONCLUDE_CONCURRENT_CEVENTTHREAD_H

// Library includes


// Namespace includes
#include "ConcurrentSettings.h"
#include "CThread.h"
#include "CEventHandler.h"
#include "CQueuedLinkedEventHandler.h"
#include "CThreadActivator.h"
#include "CEventProcessor.h"

// Other includes
#include "Concurrent/Events/CHandleEventsEvent.h"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Concurrent {

		using namespace Events;


		/*! 
		 *
		 *		\class		CEventThread
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CEventThread : public CThread, public CThreadActivator {
			// public methods
			public:
				//! Constructor
				CEventThread();

				//! Destructor
				virtual ~CEventThread();

				virtual CEventHandler* createInstalledEventHandler(CEventProcessor* eventProcessor);

				virtual CThreadActivator* signalizeEvent();

			// protected methods
			protected:
				virtual bool processControlEvents(QEvent::Type type, CControlEvent *event);

				virtual bool handleEvents(double minEventPriority = 0., cint64 maxHandleEventCount = -1);

			// protected variables
			protected:
				QMutex mAddEventHandlerSyncMutex;
				CXLinker<CEventHandler*>* mEventHandlerLinker;
				CContext* threadContext;

				bool mHandleEventStopped;
				cint64 mHandleEventsCountPerStep;

				bool mEventSignalized;
				bool mEventProcessingStopped;
				cint64 mLastProcessingStartRequestTag;
				cint64 mLastProcessingStartedTag;

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CEVENTTHREAD_H
