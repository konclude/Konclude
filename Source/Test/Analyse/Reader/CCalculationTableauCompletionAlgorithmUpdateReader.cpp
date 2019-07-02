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

#include "CCalculationTableauCompletionAlgorithmUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CCalculationTableauCompletionAlgorithmUpdateReader::CCalculationTableauCompletionAlgorithmUpdateReader(CCalculationTableauCompletionTaskHandleAlgorithm *algorithm) {
					alg = algorithm;

					atmostRuleCountValueDescription = 0;
					atleastRuleCountValueDescription = 0;
					orRuleCountValueDescription = 0;
					andRuleCountValueDescription = 0;
					someRuleCountValueDescription = 0;
					allRuleCountValueDescription = 0;
					rulesCountValueDescription = 0;


					if (alg) {
						atmostRuleCountValueDescription = new CAnalyseValueDescription();
						atmostRuleCountValueDescription->setValueName(QString("Algorithm::AtmostAppliedCount"));
						atmostRuleCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						atleastRuleCountValueDescription = new CAnalyseValueDescription();
						atleastRuleCountValueDescription->setValueName(QString("Algorithm::AtleastAppliedCount"));
						atleastRuleCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						orRuleCountValueDescription = new CAnalyseValueDescription();
						orRuleCountValueDescription->setValueName(QString("Algorithm::OrAppliedCount"));
						orRuleCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						andRuleCountValueDescription = new CAnalyseValueDescription();
						andRuleCountValueDescription->setValueName(QString("Algorithm::AndAppliedCount"));
						andRuleCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						someRuleCountValueDescription = new CAnalyseValueDescription();
						someRuleCountValueDescription->setValueName(QString("Algorithm::SomeAppliedCount"));
						someRuleCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						allRuleCountValueDescription = new CAnalyseValueDescription();
						allRuleCountValueDescription->setValueName(QString("Algorithm::AllAppliedCount"));
						allRuleCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));

						rulesCountValueDescription = new CAnalyseValueDescription();
						rulesCountValueDescription->setValueName(QString("Algorithm::TotalRulesAppliedCount"));
						rulesCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));
					}
				}

				CCalculationTableauCompletionAlgorithmUpdateReader::~CCalculationTableauCompletionAlgorithmUpdateReader() {
					if (alg) {
						delete atmostRuleCountValueDescription;
						delete atleastRuleCountValueDescription;
						delete orRuleCountValueDescription;
						delete andRuleCountValueDescription;
						delete someRuleCountValueDescription;
						delete allRuleCountValueDescription;
						delete rulesCountValueDescription;
					}
				}



				CAnalyseValueDescription *CCalculationTableauCompletionAlgorithmUpdateReader::getAtmostRuleCountValueDescription() {
					return atmostRuleCountValueDescription;
				}

				CAnalyseValueDescription *CCalculationTableauCompletionAlgorithmUpdateReader::getAtleastRuleCountValueDescription() {
					return atleastRuleCountValueDescription;
				}

				CAnalyseValueDescription *CCalculationTableauCompletionAlgorithmUpdateReader::getOrRuleCountValueDescription() {
					return orRuleCountValueDescription;
				}

				CAnalyseValueDescription *CCalculationTableauCompletionAlgorithmUpdateReader::getAndRuleCountValueDescription() {
					return andRuleCountValueDescription;
				}

				CAnalyseValueDescription *CCalculationTableauCompletionAlgorithmUpdateReader::getAllRuleCountValueDescription() {
					return allRuleCountValueDescription;
				}

				CAnalyseValueDescription *CCalculationTableauCompletionAlgorithmUpdateReader::getSomeRuleCountValueDescription() {
					return someRuleCountValueDescription;
				}

				CAnalyseValueDescription *CCalculationTableauCompletionAlgorithmUpdateReader::getRulesCountValueDescription() {
					return rulesCountValueDescription;
				}



				CAnalyseValue *CCalculationTableauCompletionAlgorithmUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription *valueDes) {
					CAnalyseValue *val = 0;

					if (valueDes == atmostRuleCountValueDescription && alg) {
						val = new CIntegerAnalyseValue(alg->getAppliedATMOSTRuleCount());
					} else if (valueDes == atleastRuleCountValueDescription && alg) {
						val = new CIntegerAnalyseValue(alg->getAppliedATLEASTRuleCount());
					} else if (valueDes == orRuleCountValueDescription && alg) {
						val = new CIntegerAnalyseValue(alg->getAppliedORRuleCount());
					} else if (valueDes == andRuleCountValueDescription && alg) {
						val = new CIntegerAnalyseValue(alg->getAppliedANDRuleCount());
					} else if (valueDes == someRuleCountValueDescription && alg) {
						val = new CIntegerAnalyseValue(alg->getAppliedSOMERuleCount());
					} else if (valueDes == allRuleCountValueDescription && alg) {
						val = new CIntegerAnalyseValue(alg->getAppliedALLRuleCount());
					} else if (valueDes == rulesCountValueDescription && alg) {
						val = new CIntegerAnalyseValue(alg->getAppliedTotalRuleCount());
					} 
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
