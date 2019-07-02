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

#include "CTaskProcessingUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CTaskProcessingUpdateReader::CTaskProcessingUpdateReader(CTaskProcessorThreadBase* taskProcessor, const QString& threadName) {
					mTaskProcessor = taskProcessor;
					mThreadName = threadName;

					mReserveScheduleTaskCountValueDescription = nullptr;


					if (mTaskProcessor) {
						mReserveScheduleTaskCountValueDescription = new CAnalyseValueDescription();
						mReserveScheduleTaskCountValueDescription->setValueName(QString("%1::ReserveScheduleTasks").arg(mThreadName));
						mReserveScheduleTaskCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));
					}
				}

				CTaskProcessingUpdateReader::~CTaskProcessingUpdateReader() {
					delete mReserveScheduleTaskCountValueDescription;
				}



				CAnalyseValueDescription* CTaskProcessingUpdateReader::getReserveScheduleTaskCountValueDescription() {
					return mReserveScheduleTaskCountValueDescription;
				}

				CAnalyseValue *CTaskProcessingUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription* valueDes) {
					CAnalyseValue *val = 0;

					if (valueDes == mReserveScheduleTaskCountValueDescription) {
						val = new CIntegerAnalyseValue(mTaskProcessor->countScheduleableProcessingTasks());
					}
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
