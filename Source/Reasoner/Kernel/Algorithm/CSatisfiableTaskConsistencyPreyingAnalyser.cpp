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

#include "CSatisfiableTaskConsistencyPreyingAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CSatisfiableTaskConsistencyPreyingAnalyser::CSatisfiableTaskConsistencyPreyingAnalyser() {
				}


				CSatisfiableTaskConsistencyPreyingAnalyser::~CSatisfiableTaskConsistencyPreyingAnalyser() {
				}


				bool CSatisfiableTaskConsistencyPreyingAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					CTaskPreyingAdapter* consPreyAdatper = statCalcTask->getConsistenceAdapter();
					if (consPreyAdatper) {
						CTaskPreyingListner* taskPreyingListner = consPreyAdatper->getTaskPreyingListner();
						if (taskPreyingListner) {
							taskPreyingListner->tellPreyedSatisfialbeTask(statCalcTask);
							return true;
						}
						CConsistenceObserver* consObs = consPreyAdatper->getConsitenceObserver();
						if (consObs) {
							CSatisfiableCalculationTask* rootStatCalcTask = (CSatisfiableCalculationTask*)statCalcTask->getRootTask();
							CConsistenceTaskData* consTaskData = new CConsistenceTaskData(rootStatCalcTask,statCalcTask);
							consObs->tellConsistenceData(consTaskData);
							return true;
						}
					}
					return false;
				}



			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
