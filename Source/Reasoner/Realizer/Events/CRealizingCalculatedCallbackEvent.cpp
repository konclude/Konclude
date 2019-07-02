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

#include "CRealizingCalculatedCallbackEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {

			namespace Events {


				CRealizingCalculatedCallbackEvent::CRealizingCalculatedCallbackEvent(CThread *receiverThread, CSatisfiableCalculationJob *satCalcJob, CRealizingTestingItem* testingItem) 
							: CCustomEvent(EVENTTYPE),CJobSatisfiableCallbackContextData(satCalcJob) {
					mRecThread = receiverThread;
					mSatCalcJob = satCalcJob;
					mTestingItem = testingItem;
					setCallbackDataContext(this);
				}


				CRealizingCalculatedCallbackEvent::~CRealizingCalculatedCallbackEvent() {
					takeCallbackDataContext();
				}

				void CRealizingCalculatedCallbackEvent::doCallback() {
					mRecThread->postEvent(this);
				}

				bool CRealizingCalculatedCallbackEvent::getTestResultSatisfiable() {
					return mSatisfiable;
				}

				CSatisfiableCalculationJob *CRealizingCalculatedCallbackEvent::getSatisfiableCalculationJob() {
					return mSatCalcJob;
				}

				CRealizingTestingItem* CRealizingCalculatedCallbackEvent::getTestingItem() {
					return mTestingItem;
				}


			}; // end namespace Events

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
