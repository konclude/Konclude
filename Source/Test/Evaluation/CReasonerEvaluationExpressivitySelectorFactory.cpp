/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CReasonerEvaluationExpressivitySelectorFactory.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationExpressivitySelectorFactory::CReasonerEvaluationExpressivitySelectorFactory(const QString& baseEvalName, const QString& baseOutputDir, const QString& expressivityDir) {
				mBaseEvalName = baseEvalName;
				mBaseOutputDir = baseOutputDir;
				mExpressivityDir = expressivityDir;
			}


			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationExpressivitySelectorFactory::getSelectors(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				selectors = getSelectorsForELToELPlusPlus(selectors);
				selectors = getSelectorsForALToSHIQ(selectors);
				selectors = getSelectorsForALToSROIQ(selectors);
				selectors = getSelectorsForSHIQToSROIQ(selectors);
				selectors = getSelectorsForALEHIFTransivityToSROIQ(selectors);
				selectors = getSelectorsForALToALEHIFTransivity(selectors);
				selectors = getSelectorsForALToALERTransivity(selectors);
				return selectors;
			}


			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationExpressivitySelectorFactory::getSelectorsForELToELPlusPlus(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationExpressivityRequestSelector(QList<COntologyExpressivity>()<<COntologyExpressivity("ALER+"),mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName+" :: [EL-EL++]");
				tmpSelector->setOutputString(mBaseOutputDir+"[EL-EL++]/");
				selectors = tmpSelector->append(selectors);
				return selectors;
			}

			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationExpressivitySelectorFactory::getSelectorsForALToSHIQ(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				QString outputDirString = mBaseOutputDir+"[AL-SHIQ]/";
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationExpressivityRequestSelector(QList<COntologyExpressivity>(),QList<COntologyExpressivity>()<<COntologyExpressivity("SHIQ"),mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName+" :: [AL-SHIQ]");
				tmpSelector->setOutputString(outputDirString);
				selectors = tmpSelector->append(selectors);
				return selectors;
			}

			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationExpressivitySelectorFactory::getSelectorsForALToSROIQ(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				QString outputDirString = mBaseOutputDir+"[AL-SROIQ]/";
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationExpressivityRequestSelector(QList<COntologyExpressivity>(),QList<COntologyExpressivity>()<<COntologyExpressivity("SROIQ"),mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName+" :: [AL-SROIQ]");
				tmpSelector->setOutputString(outputDirString);
				selectors = tmpSelector->append(selectors);
				return selectors;
			}

			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationExpressivitySelectorFactory::getSelectorsForSHIQToSROIQ(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				QString outputDirString = mBaseOutputDir+"(SHIQ-SROIQ]/";
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationExpressivityRequestSelector(QList<COntologyExpressivity>()<<COntologyExpressivity("SHIQ"),QList<COntologyExpressivity>()<<COntologyExpressivity("SROIQ"),mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName+" :: (SHIQ-SROIQ]");
				tmpSelector->setOutputString(outputDirString);
				selectors = tmpSelector->append(selectors);
				return selectors;
			}

			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationExpressivitySelectorFactory::getSelectorsForALEHIFTransivityToSROIQ(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				QString outputDirString = mBaseOutputDir+"(ALEHIF+-SROIQ]/";
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationExpressivityRequestSelector(QList<COntologyExpressivity>()<<COntologyExpressivity("ALEHIF+"),QList<COntologyExpressivity>()<<COntologyExpressivity("SROIQ"),mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName+" :: (ALEHIF+-SROIQ]");
				tmpSelector->setOutputString(outputDirString);
				selectors = tmpSelector->append(selectors);
				return selectors;
			}

			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationExpressivitySelectorFactory::getSelectorsForALToALEHIFTransivity(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				QString outputDirString = mBaseOutputDir+"[AL-ALEHIF+]/";
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationExpressivityRequestSelector(QList<COntologyExpressivity>(),QList<COntologyExpressivity>()<<COntologyExpressivity("ALEHIF+"),mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName+" :: [AL-ALEHIF+]");
				tmpSelector->setOutputString(outputDirString);
				selectors = tmpSelector->append(selectors);
				return selectors;
			}


			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationExpressivitySelectorFactory::getSelectorsForALToALERTransivity(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				QString outputDirString = mBaseOutputDir+"[AL-ALER+]/";
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationExpressivityRequestSelector(QList<COntologyExpressivity>(),QList<COntologyExpressivity>()<<COntologyExpressivity("ALER+"),mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName + " :: [AL-ALER+]");
				tmpSelector->setOutputString(outputDirString);
				selectors = tmpSelector->append(selectors);
				return selectors;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
