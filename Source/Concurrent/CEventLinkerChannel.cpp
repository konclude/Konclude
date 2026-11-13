/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CEventLinkerChannel.h"


namespace Konclude {

	namespace Concurrent {


		CEventLinkerChannel::CEventLinkerChannel(CThreadActivator* threadActivator, double channelPriority) {
			mThreadActivator = threadActivator;
			mPostedEventLinker.storeRelease(nullptr);
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
			// the posted list is taken whole, so no event address is compared with that of an
			// earlier event whose memory may have been reused since
			return true;
		}


		CEventChannel* CEventLinkerChannel::postEvent(CEvent* processingEvent) {
			processingEvent->setEventChannelPriority(mChannelPriority);
			CEventLinker* headEventLinker = nullptr;
			do {
				headEventLinker = mPostedEventLinker.loadAcquire();
				processingEvent->initEventLinker(processingEvent,headEventLinker);
			} while (!mPostedEventLinker.testAndSetRelease(headEventLinker,processingEvent));
			++postedLinkerCount;
			mThreadActivator->signalizeEvent();
			return this;
		}


		CEventLinker* CEventLinkerChannel::takeEvents(CEventLinker* addEventLinker) {
			// the whole posted list is taken, which reverses its order back to the one of posting; an
			// empty channel is left alone, the receiver polls its channels often and a write on every
			// poll would bounce the cache line between it and the posters
			if (!mPostedEventLinker.loadAcquire()) {
				return addEventLinker;
			}
			CEventLinker* takenEventLinker = mPostedEventLinker.fetchAndStoreAcquire(nullptr);
			while (takenEventLinker) {
				CEventLinker* nextEventLinker = takenEventLinker->getNextEventLinker();
				addEventLinker = takenEventLinker->setNextEventLinker(addEventLinker);
				++fetchedLinkerCount;
				takenEventLinker = nextEventLinker;
			}
			return addEventLinker;
		}


		bool CEventLinkerChannel::hasEvents() {
			return mPostedEventLinker.loadRelaxed() != nullptr;
		}


		double CEventLinkerChannel::getChannelPriority() {
			return mChannelPriority;
		}


	}; // end namespace Concurrent

}; // end namespace Konclude

