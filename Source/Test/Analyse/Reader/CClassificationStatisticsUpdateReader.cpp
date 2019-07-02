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

#include "CClassificationStatisticsUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CClassificationStatisticsUpdateReader::CClassificationStatisticsUpdateReader(CClassificationManager *classificationManager) {
					classiMan = classificationManager;

					pseudoModelMergedSubsumValueDescription = 0;
					derivatedSubsumValueDescription = 0;
					calculatedSubsumValueDescription = 0;
					prunedSubsumValueDescription = 0;
					subclassSubsumValueDescription = 0;
					subclassIdOccSubsumValueDescription = 0;

					statistics = new CClassifierStatistics();

					if (classiMan) {
						pseudoModelMergedSubsumValueDescription = new CAnalyseValueDescription();
						pseudoModelMergedSubsumValueDescription->setValueName(QString("PseudoModelMergedTestedSubsumptionCount"));
						pseudoModelMergedSubsumValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						subclassSubsumValueDescription = new CAnalyseValueDescription();
						subclassSubsumValueDescription->setValueName(QString("SubclassTestedSubsumptionCount"));
						subclassSubsumValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						subclassIdOccSubsumValueDescription = new CAnalyseValueDescription();
						subclassIdOccSubsumValueDescription->setValueName(QString("SubclassIdentifierOccurTestedSubsumptionCount"));
						subclassIdOccSubsumValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						derivatedSubsumValueDescription = new CAnalyseValueDescription();
						derivatedSubsumValueDescription->setValueName(QString("DerivatedTestedSubsumptionCount"));
						derivatedSubsumValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						calculatedSubsumValueDescription = new CAnalyseValueDescription();
						calculatedSubsumValueDescription->setValueName(QString("CalculatedTestedSubsumptionCount"));
						calculatedSubsumValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						prunedSubsumValueDescription = new CAnalyseValueDescription();
						prunedSubsumValueDescription->setValueName(QString("PrunedTestedSubsumptionCount"));
						prunedSubsumValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						toldSubsumValueDescription = new CAnalyseValueDescription();
						toldSubsumValueDescription->setValueName(QString("ToldSubsumptionCount"));
						toldSubsumValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));
					}
				}

				CClassificationStatisticsUpdateReader::~CClassificationStatisticsUpdateReader() {
					if (pseudoModelMergedSubsumValueDescription) {
						delete pseudoModelMergedSubsumValueDescription;
					}
					if (derivatedSubsumValueDescription) {
						delete derivatedSubsumValueDescription;
					}
					if (calculatedSubsumValueDescription) {
						delete calculatedSubsumValueDescription;
					}
					if (prunedSubsumValueDescription) {
						delete prunedSubsumValueDescription;
					}
					if (subclassSubsumValueDescription) {
						delete subclassSubsumValueDescription;
					}
					if (subclassIdOccSubsumValueDescription) {
						delete subclassIdOccSubsumValueDescription;
					}
					if (toldSubsumValueDescription) {
						delete toldSubsumValueDescription;
					}
					if (statistics) {
						delete statistics;
					}
				}



				CAnalyseValueDescription *CClassificationStatisticsUpdateReader::getPseudoModelMergedTestedSubsumptionCountValueDescription() {
					return pseudoModelMergedSubsumValueDescription;
				}


				CAnalyseValueDescription *CClassificationStatisticsUpdateReader::getDerivatedTestedSubsumptionCountValueDescription() {
					return derivatedSubsumValueDescription;
				}

				CAnalyseValueDescription *CClassificationStatisticsUpdateReader::getCalculatedTestedSubsumptionCountValueDescription() {
					return calculatedSubsumValueDescription;
				}

				CAnalyseValueDescription *CClassificationStatisticsUpdateReader::getPrunedTestedSubsumptionCountValueDescription() {
					return prunedSubsumValueDescription;
				}

				CAnalyseValueDescription *CClassificationStatisticsUpdateReader::getSubclassTestedSubsumptionCountValueDescription() {
					return subclassSubsumValueDescription;
				}

				CAnalyseValueDescription *CClassificationStatisticsUpdateReader::getSubclassIdentifierOccurTestedSubsumptionCountValueDescription() {
					return subclassIdOccSubsumValueDescription;
				}

				CAnalyseValueDescription *CClassificationStatisticsUpdateReader::getToldSubsumptionCountValueDescription() {
					return toldSubsumValueDescription;
				}


				CAnalyseValue *CClassificationStatisticsUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription *valueDes) {
					CAnalyseValue *val = 0;
					statistics->resetValues();

					if (valueDes == pseudoModelMergedSubsumValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getPseudoModelMergedTestedSubsumptionCount());
					} else if (valueDes == derivatedSubsumValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getDerivatedTestedSubsumptionCount());
					} else if (valueDes == calculatedSubsumValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getCalculatedTestedSubsumptionCount());
					} else if (valueDes == prunedSubsumValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getPrunedTestedSubsumptionCount());
					} else if (valueDes == subclassSubsumValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getSubclassTestedSubsumptionCount());
					}  else if (valueDes == subclassIdOccSubsumValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getSubclassIdentifierOccurTestedSubsumptionCount());
					} else if (valueDes == toldSubsumValueDescription && classiMan) {
						val = new CIntegerAnalyseValue(classiMan->collectClassificationStatistics(statistics)->getToldSubsumptionCount()-statistics->getCalculatedTestedSubsumptionCount());
					} 
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
