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

#include "CClassificationStatusUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CClassificationStatusUpdateReader::CClassificationStatusUpdateReader(CSubsumptionClassifier *subsumptionClassifier) {
					subsumClassifier = subsumptionClassifier;

					statusValueDescription = 0;

					if (subsumClassifier) {
						statusValueDescription = new CAnalyseValueDescription();
						statusValueDescription->setValueName(QString("SubsumptionClassifier::StatusText"));
						statusValueDescription->setTemplateValue(new CStringAnalyseValue(0));
					}
				}

				CClassificationStatusUpdateReader::~CClassificationStatusUpdateReader() {
					if (statusValueDescription) {
						delete statusValueDescription;
					}
				}



				CAnalyseValueDescription *CClassificationStatusUpdateReader::getSubsumptionClassifierStatusDescription() {
					return statusValueDescription;
				}

				CAnalyseValue *CClassificationStatusUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription *valueDes) {
					CAnalyseValue *val = 0;

					if (valueDes == statusValueDescription && subsumClassifier) {
						val = new CStringAnalyseValue(subsumClassifier->getStatusString());
					}
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
