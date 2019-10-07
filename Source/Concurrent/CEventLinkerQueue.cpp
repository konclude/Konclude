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

#include "CEventLinkerQueue.h"


namespace Konclude {

	namespace Concurrent {


		CEventLinkerQueue::CEventLinkerQueue(CThreadActivator* threadActivator) {
			mThreadActivator = threadActivator;
			mDefaultEventChannel = new CSyncedEventChannel(this);
			mReverseSortedChannelLinker = new CEventChannelLinker(mDefaultEventChannel);

			mActivatorThreshold = 1;
			mInexactEventSingnals = 0;
			mThreadActivated = false;
		}

		CEventLinkerQueue::~CEventLinkerQueue() {
			while (mReverseSortedChannelLinker) {
				CEventChannelLinker* tmpDeleteChannel = mReverseSortedChannelLinker;
				mReverseSortedChannelLinker = mReverseSortedChannelLinker->getNext();
				delete tmpDeleteChannel->getData();
				delete tmpDeleteChannel;
			}
		}


		CSyncedEventChannel* CEventLinkerQueue::getDefaultSyncedEventChannel(double channelPriority) {
			return mDefaultEventChannel;
		}

		CSyncedFreeEventChannel* CEventLinkerQueue::createSyncedFreeEventChannel(double channelPriority) {
			mCreateChannelSyncMutex.lock();
			CSyncedFreeEventChannel* tmpChannel = new CSyncedFreeEventChannel(this);
			CEventChannelLinker* tmpChannelLinker = new CEventChannelLinker(tmpChannel);
			mReverseSortedChannelLinker = mReverseSortedChannelLinker->insertSortedNextSorted(tmpChannelLinker);
			mCreateChannelSyncMutex.unlock();
			return tmpChannel;
		}

		CThreadActivator* CEventLinkerQueue::signalizeEvent() {
			mInexactEventSingnals = mInexactEventSingnals+1;
			if (mInexactEventSingnals >= mActivatorThreshold) {
				if (true || !mThreadActivated) {
					// may called multiple times
					mThreadActivated = true;
					if (mThreadActivator) {
						mThreadActivator->signalizeEvent();
					}
				}
			}
			return this;
		}

		CEventLinker* CEventLinkerQueue::takeEvents(CEventLinker* addEventLinker, double minEventPriority) {
			mThreadActivated = false;
			mInexactEventSingnals = 0;
			CEventChannelLinker* channelLinkerIt = mReverseSortedChannelLinker;
			while (channelLinkerIt) {
				CEventChannel *channel = channelLinkerIt->getData();
				if (channel->getChannelPriority() >= minEventPriority) {
					CEventLinkerChannel* linkerChannel = dynamic_cast<CEventLinkerChannel*>(channel);
					if (linkerChannel) {
						addEventLinker = linkerChannel->takeEvents(addEventLinker);
					} else {
						// take events step for step, currently not supported
					}
				}
				channelLinkerIt = channelLinkerIt->getNext();
			}
			return addEventLinker;
		}

		bool CEventLinkerQueue::hasEvents(double minEventPriority) {
			CEventChannelLinker* channelLinkerIt = mReverseSortedChannelLinker;
			while (channelLinkerIt) {
				CEventChannel *channel = channelLinkerIt->getData();
				if (channel->getChannelPriority() >= minEventPriority && channel->hasEvents()) {
					return true;
				}
				channelLinkerIt = channelLinkerIt->getNext();
			}
			return false;
		}





	}; // end namespace Concurrent

}; // end namespace Konclude

