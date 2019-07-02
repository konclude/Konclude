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

#include "CEventLinkerChannel.h"


namespace Konclude {

	namespace Concurrent {


		CEventLinkerChannel::CEventLinkerChannel(CThreadActivator* threadActivator, double channelPriority) {
			mThreadActivator = threadActivator;
			mPostedEventLinker = nullptr;
			mLastTakedPostEventLinker = nullptr;
			mLastPostedEventLinker = nullptr;
			mLastTakedDuplicatedPostEventLinker = nullptr;

			acquiredLinkerCount = 0;
			fetchedLinkerCount = 0;
			postedLinkerCount = 0;

			mChannelPriority = channelPriority;
		}

		CEventLinkerChannel::~CEventLinkerChannel() {
		}


		bool CEventLinkerChannel::checkPostEvent(CEvent* processingEvent) {
			return processingEvent != mLastTakedPostEventLinker;
		}


		CEventChannel* CEventLinkerChannel::postEvent(CEvent* processingEvent) {
			mLastPostedEventLinker = mPostedEventLinker;
			CEventLinker* newEventPostLinker = processingEvent;
			processingEvent->setEventChannelPriority(mChannelPriority);
			//if (newEventPostLinker == mLastTakedPostEventLinker) {
			//	mLastTakedDuplicatedPostEventLinker = newEventPostLinker;
			//}
			if (mPostedEventLinker != newEventPostLinker) {
				newEventPostLinker->initEventLinker(processingEvent,mPostedEventLinker);
				mPostedEventLinker = newEventPostLinker;
			} else {
				// write access to mLastTakedPostEventLinker is safe only in this case
				newEventPostLinker->initEventLinker(processingEvent,nullptr);
				mPostedEventLinker = newEventPostLinker;
				mLastTakedPostEventLinker = nullptr;
			}
			++postedLinkerCount;
			mThreadActivator->signalizeEvent();
			return this;
		}


		CEventLinker* CEventLinkerChannel::takeEvents(CEventLinker* addEventLinker) {
			// reverse stored event order, take all events
			CEventLinker* tmpEventPostLinker = mPostedEventLinker;
			//bool duplicatedStopp = tmpEventPostLinker == mLastTakedDuplicatedPostEventLinker;
			//mLastTakedDuplicatedPostEventLinker = nullptr;
			CEventLinker* tmpNextLastPostEventLinker = tmpEventPostLinker;
			CEventLinker* tmpAddEventLinker = nullptr;
			while (tmpEventPostLinker && (tmpEventPostLinker != mLastTakedPostEventLinker /*|| duplicatedStopp*/)) {
				//if (tmpEventPostLinker == mLastTakedPostEventLinker) {
				//	duplicatedStopp = false;
				//}
				tmpAddEventLinker = tmpEventPostLinker;
				tmpEventPostLinker = tmpEventPostLinker->getNextEventLinker();
				addEventLinker = tmpAddEventLinker->setNextEventLinker(addEventLinker);
				++fetchedLinkerCount;
			}
			if (tmpNextLastPostEventLinker != mLastTakedPostEventLinker) {
				mLastTakedPostEventLinker = tmpNextLastPostEventLinker;
			}
			return addEventLinker;
		}



		bool CEventLinkerChannel::hasEvents() {
			return mLastTakedPostEventLinker != mPostedEventLinker;
		}



		double CEventLinkerChannel::getChannelPriority() {
			return mChannelPriority;
		}


	}; // end namespace Concurrent

}; // end namespace Konclude

