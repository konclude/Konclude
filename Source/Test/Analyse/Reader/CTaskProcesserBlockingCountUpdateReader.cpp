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

#include "CTaskProcesserBlockingCountUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CTaskProcesserBlockingCountUpdateReader::CTaskProcesserBlockingCountUpdateReader(QList<CTaskProcessorThreadBase*>* taskProcessorList) {
					mTaskProcessorList = *taskProcessorList;

					mBlockedProcessorCountValueDescription = nullptr;


					if (!mTaskProcessorList.isEmpty()) {
						mBlockedProcessorCountValueDescription = new CAnalyseValueDescription();
						mBlockedProcessorCountValueDescription->setValueName(QString("::BlockedThreadCount"));
						mBlockedProcessorCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));
					}
				}

				CTaskProcesserBlockingCountUpdateReader::~CTaskProcesserBlockingCountUpdateReader() {
					delete mBlockedProcessorCountValueDescription;
				}



				CAnalyseValueDescription* CTaskProcesserBlockingCountUpdateReader::getBlockedProcessorCountValueDescription() {
					return mBlockedProcessorCountValueDescription;
				}

				CAnalyseValue *CTaskProcesserBlockingCountUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription* valueDes) {
					CAnalyseValue *val = 0;

					if (valueDes == mBlockedProcessorCountValueDescription) {
						cint64 blockCount = 0;
						foreach (CTaskProcessorThreadBase* taskProcessor, mTaskProcessorList) {
							if (taskProcessor->isBlocked()) {
								++blockCount;
							}
						}
						val = new CIntegerAnalyseValue(blockCount);
					}
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
