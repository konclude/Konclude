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

#include "CSynchronizedCallbackListExecuter.h"



namespace Konclude {

	namespace Concurrent {

		namespace Callback {



			CSynchronizedCallbackListExecuter::CSynchronizedCallbackListExecuter() {
			}


			CSynchronizedCallbackListExecuter::~CSynchronizedCallbackListExecuter() {
			}


			void CSynchronizedCallbackListExecuter::doCallback() {
				QLinkedList<CCallbackData *> callBackLocalCopyList;
				syncMutex.lock();
				// make local copy
				foreach (CCallbackData *callbackData, callbackList) {
					callBackLocalCopyList.append(callbackData);
				}
				callbackList.clear();
				syncMutex.unlock();

				foreach (CCallbackData *callbackData, callBackLocalCopyList) {
					callbackData->doCallback();
				}
			}


			CCallbackListExecuter *CSynchronizedCallbackListExecuter::addCallbackData(CCallbackData *callbackData) {
				syncMutex.lock();
				CCallbackListExecuter::addCallbackData(callbackData);
				syncMutex.unlock();
				return this;
			}

			CCallbackListExecuter *CSynchronizedCallbackListExecuter::removeCallbackData(CCallbackData *callbackData) {
				syncMutex.lock();
				CCallbackListExecuter::removeCallbackData(callbackData);
				syncMutex.unlock();
				return this;
			}

		}; // end namespace Callback

	}; // end namespace Concurrent

}; // end namespace Konclude