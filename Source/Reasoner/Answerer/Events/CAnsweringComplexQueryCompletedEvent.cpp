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

#include "CAnsweringComplexQueryCompletedEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Events {


				CAnsweringComplexQueryCompletedEvent::CAnsweringComplexQueryCompletedEvent(CThread* thread, CAnsweringHandler *answerHandler, CAnsweringManagerQueryData* queryData)
						: CCustomEvent(EVENTTYPE) {
					mAnswerHandler = answerHandler;
					mQueryData = queryData;
					mThread = thread;
				}


				void CAnsweringComplexQueryCompletedEvent::doCallback() {
					if (mThread) {
						mThread->postEvent(this);
					}
				}

				CAnsweringHandler* CAnsweringComplexQueryCompletedEvent::getAnsweringHandler() {
					return mAnswerHandler;
				}


				CAnsweringManagerQueryData* CAnsweringComplexQueryCompletedEvent::getQueryData() {
					return mQueryData;
				}




			}; // end namespace Events

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
