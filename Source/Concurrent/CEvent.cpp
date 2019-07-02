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

#include "CEvent.h"


namespace Konclude {

	namespace Concurrent {

		CEvent::CEvent(cint64 eventTypeID, CMemoryPool* memoryPool) : CMemoryPoolContainer(memoryPool) {
			mEventPriority = 0.;
			mEventTypeID = eventTypeID;
			initEventLinker(this,nullptr);
		}

		CEvent::~CEvent() {
		}

		double CEvent::getEventChannelPriority() {
			return mEventPriority;
		}

		CEvent* CEvent::setEventChannelPriority(double eventPriority) {
			mEventPriority = eventPriority;
			return this;
		}

		cint64 CEvent::getEventTypeID() {
			return mEventTypeID;
		}

		CEvent* CEvent::setEventTypeID(cint64 eventTypeID) {
			mEventTypeID = eventTypeID;
			return this;
		}


	}; // end namespace Concurrent

}; // end namespace Konclude

