/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */
#ifndef LOGGER_EVENTS_CREQUESTLOGMESSAGESEVENT_H
#define LOGGER_EVENTS_CREQUESTLOGMESSAGESEVENT_H

#include <QEvent>
#include <QLinkedList>
#include <QStringList>

#include "Concurrent/Events/CCustomCallbackEvent.h"

#include "../LoggerSettings.h"
#include "../CLogMessage.h"


namespace Konclude {

	using namespace Concurrent;
	using namespace Events;

	namespace Logger {

		namespace Events {


			/*! 
			 *	\class		CRequestLogMessagesEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CRequestLogMessagesEvent : public CCustomCallbackEvent {
				// public methods
				public:
					//! Constructor
					CRequestLogMessagesEvent(CCallbackData *callback, qint64 beginID, qint64 endID, double beginLevel, double endLevel, QStringList domainList, QLinkedList<void *> objectList);

					//! Destructor
					virtual ~CRequestLogMessagesEvent();

					
					CRequestLogMessagesEvent *setMessageBeginID(qint64 beginID);
					qint64 getMessageBeginID();

					CRequestLogMessagesEvent *setMessageEndID(qint64 endID);
					qint64 getMessageEndID();

					CRequestLogMessagesEvent *setLevelBegin(double beginLevel);
					double getBeginLevel();

					CRequestLogMessagesEvent *setLevelEnd(double endLevel);
					double getEndLevel();

					CRequestLogMessagesEvent *setDomains(QStringList domainList);
					QStringList getDomains();
	
					CRequestLogMessagesEvent *setObjects(QLinkedList<void *> objectList);
					QLinkedList<void *> getObjects();


				// protected methods
				protected:


				// private members
				private:
					qint64 messageIDBegin;
					qint64 messageIDEnd;
					double levelBegin;
					double levelEnd; 
					QStringList domains;
					QLinkedList<void *> objects;
					


			};

		}; // end namespace Events

	}; // end namespace Logger

}; // end namespace Konclude

#endif // LOGGER_EVENTS_CREQUESTLOGMESSAGESEVENT_H
