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

#include "CPrecomputationCalculatedCallbackEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {

			namespace Events {


				CPrecomputationCalculatedCallbackEvent::CPrecomputationCalculatedCallbackEvent(CThread *receiverThread, CSatisfiableCalculationJob *satCalcJob, CPrecomputationTestingItem* testingItem) 
							: CCustomEvent(EVENTTYPE),CJobSatisfiableCallbackContextData(satCalcJob) {
					mRecThread = receiverThread;
					mSatCalcJob = satCalcJob;
					mTestingItem = testingItem;
					setCallbackDataContext(this);
				}


				CPrecomputationCalculatedCallbackEvent::~CPrecomputationCalculatedCallbackEvent() {
					takeCallbackDataContext();
				}

				void CPrecomputationCalculatedCallbackEvent::doCallback() {
					mRecThread->postEvent(this);
				}

				bool CPrecomputationCalculatedCallbackEvent::getTestResultSatisfiable() {
					return mSatisfiable;
				}

				CSatisfiableCalculationJob *CPrecomputationCalculatedCallbackEvent::getSatisfiableCalculationJob() {
					return mSatCalcJob;
				}

				CPrecomputationTestingItem* CPrecomputationCalculatedCallbackEvent::getTestingItem() {
					return mTestingItem;
				}


			}; // end namespace Events

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
