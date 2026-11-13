/*
 *		Copyright (C) 2013-2015, 2019, 2026 by the Konclude Developer Team.
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

#include "CJNILogMessageQueue.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CJNILogMessageQueue::CJNILogMessageQueue() {
					mDroppedCount = 0;
				}


				CJNILogMessageQueue* CJNILogMessageQueue::getInstance() {
					static QMutex creationMutex;
					static CJNILogMessageQueue* instance = nullptr;
					QMutexLocker locker(&creationMutex);
					if (!instance) {
						instance = new CJNILogMessageQueue();
						// the notice level below 30 is left out, as for the console
						CLogger::getInstance()->addLogObserver(instance, 30);
					}
					return instance;
				}


				void CJNILogMessageQueue::postLogMessage(CLogMessage* message) {
					CQueuedLogMessage queuedMessage;
					queuedMessage.mLevel = message->getLogLevel();
					queuedMessage.mDomain = message->getDomain();
					queuedMessage.mMessage = message->getMessage().trimmed();
					QMutexLocker locker(&mMutex);
					if (mMessages.size() >= MAXIMUM_QUEUED_MESSAGES) {
						mMessages.removeFirst();
						++mDroppedCount;
					}
					mMessages.append(queuedMessage);
				}


				QList<QString> CJNILogMessageQueue::takeMessages(cint64* droppedCount) {
					QList<CQueuedLogMessage> messages;
					{
						QMutexLocker locker(&mMutex);
						messages.swap(mMessages);
						if (droppedCount) {
							*droppedCount = mDroppedCount;
						}
						mDroppedCount = 0;
					}
					QList<QString> entries;
					for (const CQueuedLogMessage& message : messages) {
						entries.append(QString::number(message.mLevel));
						entries.append(message.mDomain);
						entries.append(message.mMessage);
					}
					return entries;
				}


			}; // end namespace JNI

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
