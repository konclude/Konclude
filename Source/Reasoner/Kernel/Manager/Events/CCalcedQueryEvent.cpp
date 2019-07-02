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

#include "CCalcedQueryEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {

				namespace Events {


					CCalcedQueryEvent::CCalcedQueryEvent(CThread *receiverThread, CQuery *query) 
							: CCustomEvent(EVENTTYPE) {

						this->query = query;
						recThread = receiverThread;

						queryString = query->getQueryString();
						queryName = query->getQueryName();
					}



					CCalcedQueryEvent::~CCalcedQueryEvent() {
					}

					void CCalcedQueryEvent::doCallback() {
						answerString = query->getAnswerString();
						if (recThread) {
							recThread->postEvent(this);
						}
					}

					void CCalcedQueryEvent::sendEventWithAnswer(const QString& newAnswerString) {
						answerString = newAnswerString;
						if (recThread) {
							recThread->postEvent(this);
						}
					}

					QString CCalcedQueryEvent::getAnswerString() {
						return answerString;
					}

					QString CCalcedQueryEvent::getQueryString() {
						return queryString;
					}

					QString CCalcedQueryEvent::getQueryName() {
						return queryName;
					}

					CQuery *CCalcedQueryEvent::getQuery() {
						return query;
					}

				}; // end namespace Events

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
