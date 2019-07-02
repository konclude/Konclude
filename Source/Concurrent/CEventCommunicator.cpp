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

#include "CEventCommunicator.h"


namespace Konclude {

	namespace Concurrent {


		CEventCommunicator::CEventCommunicator(CEventHandler* eventHandler) {
			mEventHandler = eventHandler;
			mEventChannel = nullptr;
		}

		CEventCommunicator::~CEventCommunicator() {
		}

		CEventCommunicator* CEventCommunicator::postEvent(CEvent* event) {
			if (!mEventChannel) {
				mEventChannel = mEventHandler->createSyncedFreeEventChannel();
			}
			mEventChannel->postEvent(event);
			return this;
		}

		bool CEventCommunicator::checkPostEvent(CEvent* event) {
			if (!mEventChannel) {
				mEventChannel = mEventHandler->createSyncedFreeEventChannel();
			}
			return mEventChannel->checkPostEvent(event);
		}

		bool CEventCommunicator::postEvent(CEventHandler* eventHandler, CEvent* event) {
			CEventChannel* eventChannel = eventHandler->getDefaultSyncedEventChannel();
			eventChannel->postEvent(event);
			return true;
		}

		bool CEventCommunicator::checkPostEvent(CEventHandler* eventHandler, CEvent* event) {
			CEventChannel* eventChannel = eventHandler->getDefaultSyncedEventChannel();
			return eventChannel->checkPostEvent(event);
		}

	}; // end namespace Concurrent

}; // end namespace Konclude

