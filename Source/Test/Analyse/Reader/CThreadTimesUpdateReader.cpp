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

#include "CThreadTimesUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CThreadTimesUpdateReader::CThreadTimesUpdateReader(CThread *thread) {
					observedThread = thread;

					waitTimeValueDescription = 0;
					blockTimeValueDescription = 0;
					runTimeValueDescription = 0;

					if (observedThread) {
						waitTimeValueDescription = new CAnalyseValueDescription();
						waitTimeValueDescription->setValueName(observedThread->getThreadName()+QString("-WaitTime"));
						waitTimeValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						blockTimeValueDescription = new CAnalyseValueDescription();
						blockTimeValueDescription->setValueName(observedThread->getThreadName()+QString("-BlockTime"));
						blockTimeValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						runTimeValueDescription = new CAnalyseValueDescription();
						runTimeValueDescription->setValueName(observedThread->getThreadName()+QString("-RunTime"));
						runTimeValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));
					}
				}

				CThreadTimesUpdateReader::~CThreadTimesUpdateReader() {
					if (waitTimeValueDescription) {
						delete waitTimeValueDescription;
					}
					if (blockTimeValueDescription) {
						delete blockTimeValueDescription;
					}
					if (runTimeValueDescription) {
						delete runTimeValueDescription;
					}
				}



				CAnalyseValueDescription *CThreadTimesUpdateReader::getWaitTimeValueDescription() {
					return waitTimeValueDescription;
				}

				CAnalyseValueDescription *CThreadTimesUpdateReader::getBlockTimeValueDescription() {
					return blockTimeValueDescription;
				}

				CAnalyseValueDescription *CThreadTimesUpdateReader::getRunTimeValueDescription() {
					return runTimeValueDescription;
				}



				CAnalyseValue *CThreadTimesUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription *valueDes) {
					CAnalyseValue *val = 0;

					if (valueDes == waitTimeValueDescription && observedThread) {
						val = new CIntegerAnalyseValue(observedThread->getWaitTimeSecs());
					} else if (valueDes == blockTimeValueDescription && observedThread) {
						val = new CIntegerAnalyseValue(observedThread->getBlockTimeSecs());
					} else if (valueDes == runTimeValueDescription && observedThread) {
						val = new CIntegerAnalyseValue(observedThread->getRunTimeSecs());
					}
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
