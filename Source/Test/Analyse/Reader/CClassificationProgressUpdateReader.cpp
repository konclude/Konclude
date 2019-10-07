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

#include "CClassificationProgressUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CClassificationProgressUpdateReader::CClassificationProgressUpdateReader(CClassificationManager *classificationManager) {
					classiMan = classificationManager;

					totalSubsumValueDescription = 0;
					totalSatisValueDescription = 0;
					testedSubsumValueDescription = 0;
					testedSatisValueDescription = 0;
					activeClassifierCountValueDescription = 0;

					statistics = new CClassifierStatistics();

					if (classiMan) {
						totalSubsumValueDescription = new CAnalyseValueDescription();
						totalSubsumValueDescription->setValueName(QString("ClassificationManager::TotalSubsumptionTestCount"));
						totalSubsumValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						totalSatisValueDescription = new CAnalyseValueDescription();
						totalSatisValueDescription->setValueName(QString("ClassificationManager::TotalSatisfiableTestCount"));
						totalSatisValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						testedSubsumValueDescription = new CAnalyseValueDescription();
						testedSubsumValueDescription->setValueName(QString("ClassificationManager::TestedSubsumptionCount"));
						testedSubsumValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						testedSatisValueDescription = new CAnalyseValueDescription();
						testedSatisValueDescription->setValueName(QString("ClassificationManager::TestedSatisfiableCount"));
						testedSatisValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						activeClassifierCountValueDescription = new CAnalyseValueDescription();
						activeClassifierCountValueDescription->setValueName(QString("ClassificationManager::ActiveClassifierCount"));
						activeClassifierCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));
					}
				}

				CClassificationProgressUpdateReader::~CClassificationProgressUpdateReader() {
					if (totalSubsumValueDescription) {
						delete totalSubsumValueDescription;
					}
					if (totalSatisValueDescription) {
						delete totalSatisValueDescription;
					}
					if (testedSubsumValueDescription) {
						delete testedSubsumValueDescription;
					}
					if (testedSatisValueDescription) {
						delete testedSatisValueDescription;
					}
					if (activeClassifierCountValueDescription) {
						delete activeClassifierCountValueDescription;
					}
					if (statistics) {
						delete statistics;
					}
				}



				CAnalyseValueDescription *CClassificationProgressUpdateReader::getTotalSubsumptionTestCountValueDescription() {
					return totalSubsumValueDescription;
				}


				CAnalyseValueDescription *CClassificationProgressUpdateReader::getTestedSubsumptionCountValueDescription() {
					return testedSubsumValueDescription;
				}

				CAnalyseValueDescription *CClassificationProgressUpdateReader::getTotalSatisfiableTestCountValueDescription() {
					return totalSatisValueDescription;
				}

				CAnalyseValueDescription *CClassificationProgressUpdateReader::getTestedSatisfiableCountValueDescription() {
					return testedSatisValueDescription;
				}

				CAnalyseValueDescription *CClassificationProgressUpdateReader::getActiveClassifierCountValueDescription() {
					return activeClassifierCountValueDescription;
				}



				CAnalyseValue *CClassificationProgressUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription *valueDes) {
					CAnalyseValue *val = 0;
					statistics->resetValues();

					if (valueDes == totalSubsumValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getTotalSubsumptionTestCount());
					} else if (valueDes == totalSatisValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getTotalSatisfiableCount());
					} else if (valueDes == testedSubsumValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getTestedSubsumptionCount());
					} else if (valueDes == testedSatisValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getTestedSatisfiableCount());
					} else if (valueDes == activeClassifierCountValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->getActiveClassifierCount());
					}
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
