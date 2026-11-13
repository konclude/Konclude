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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNILOGMESSAGEQUEUE_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNILOGMESSAGEQUEUE_H

// Libraries includes
#include <QMutex>
#include <QList>
#include <QString>

// Namespace includes


// Other includes
#include "Utilities/UtilitiesSettings.h"
#include "Logger/CAbstractLogObserver.h"
#include "Logger/CLogMessage.h"
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Utilities;

	namespace Control {

		namespace Interface {

			namespace JNI {


				/*! 
				 *
				 *		\class		CJNILogMessageQueue
				 *		\version	0.1
				 *		\brief		Keeps Konclude's log messages for the Java side, which logs them with SLF4J
				 *
				 *		Without an observer the library's log reached nobody inside another program, a warning
				 *		about the ontology included (issue #32). The logger delivers the messages on its own
				 *		thread, so they are only queued here, and the Java side takes them after its calls into
				 *		the library. There is one queue for the process, as there is one logger; a full queue
				 *		drops the oldest messages and counts them.
				 *
				 */
				class CJNILogMessageQueue : public CAbstractLogObserver {
					// public methods
					public:
						//! the queue of the process, created and registered with the logger on the first call
						static CJNILogMessageQueue* getInstance();

						virtual void postLogMessage(CLogMessage* message);

						//! the queued messages as level, domain and text, three entries each, and empties the queue
						QList<QString> takeMessages(cint64* droppedCount);

					// protected methods
					protected:
						CJNILogMessageQueue();

					// protected variables
					protected:
						struct CQueuedLogMessage {
							double mLevel;
							QString mDomain;
							QString mMessage;
						};
						QMutex mMutex;
						QList<CQueuedLogMessage> mMessages;
						cint64 mDroppedCount;

						static const cint64 MAXIMUM_QUEUED_MESSAGES = 10000;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace JNI

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNILOGMESSAGEQUEUE_H
