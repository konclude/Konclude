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
#include "CRequestLogMessagesEvent.h"

namespace Konclude {

	namespace Logger {

		namespace Events {

			CRequestLogMessagesEvent::CRequestLogMessagesEvent(CCallbackData *callback, qint64 beginID, qint64 endID, double beginLevel, double endLevel, QStringList domainList, QLinkedList<void *> objectList) 
					: CCustomCallbackEvent(EVENTREQUESTLOGMESSAGES,callback) {
				messageIDBegin = beginID;
				messageIDEnd = endID;
				levelBegin = beginLevel;
				levelEnd = endLevel;
				domains = domainList;
				objects = objectList;
			}

			CRequestLogMessagesEvent::~CRequestLogMessagesEvent() {
			}


			CRequestLogMessagesEvent *CRequestLogMessagesEvent::setMessageBeginID(qint64 beginID) {
				messageIDBegin = beginID;
				return this;
			}

			qint64 CRequestLogMessagesEvent::getMessageBeginID() {
				return messageIDBegin;
			}


			CRequestLogMessagesEvent *CRequestLogMessagesEvent::setMessageEndID(qint64 endID) {
				messageIDEnd = endID;
				return this;
			}

			qint64 CRequestLogMessagesEvent::getMessageEndID() {
				return messageIDEnd;
			}


			CRequestLogMessagesEvent *CRequestLogMessagesEvent::setLevelBegin(double beginLevel) {
				levelBegin = beginLevel;
				return this;
			}

			double CRequestLogMessagesEvent::getBeginLevel() {
				return levelBegin;
			}


			CRequestLogMessagesEvent *CRequestLogMessagesEvent::setLevelEnd(double endLevel) {
				levelEnd = endLevel;
				return this;
			}

			double CRequestLogMessagesEvent::getEndLevel() {
				return levelEnd;
			}


			CRequestLogMessagesEvent *CRequestLogMessagesEvent::setDomains(QStringList domainList) {
				domains = domainList;
				return this;
			}

			QStringList CRequestLogMessagesEvent::getDomains() {
				return domains;
			}


			CRequestLogMessagesEvent *CRequestLogMessagesEvent::setObjects(QLinkedList<void *> objectList) {
				objects = objectList;
				return this;
			}

			QLinkedList<void *> CRequestLogMessagesEvent::getObjects() {
				return objects;
			}



		}; // end namespace Events

	}; // end namespace Logger

}; // end namespace Konclude