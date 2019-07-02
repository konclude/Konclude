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

#include "CReasonerEvaluationProfileSelectorFactory.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationProfileSelectorFactory::CReasonerEvaluationProfileSelectorFactory(const QString& baseEvalName, const QString& baseOutputDir, const QString& expressivityDir) {
				mBaseEvalName = baseEvalName;
				mBaseOutputDir = baseOutputDir;
				mExpressivityDir = expressivityDir;
			}


			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationProfileSelectorFactory::getSelectors(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				selectors = getSelectorsForDLProfile(selectors);
				selectors = getSelectorsForRLProfile(selectors);
				selectors = getSelectorsForQLProfile(selectors);
				selectors = getSelectorsForELProfile(selectors);
				return selectors;
			}


			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationProfileSelectorFactory::getSelectorsForDLProfile(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationProfileRequestSelector(CReasonerEvaluationProfileRequestSelector::REPRS_PROFILE_DL,mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName+" :: OWL-DL");
				tmpSelector->setOutputString(mBaseOutputDir+"OWL-DL/");
				selectors = tmpSelector->append(selectors);
				return selectors;
			}


			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationProfileSelectorFactory::getSelectorsForRLProfile(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationProfileRequestSelector(CReasonerEvaluationProfileRequestSelector::REPRS_PROFILE_RL,mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName+" :: OWL-RL");
				tmpSelector->setOutputString(mBaseOutputDir+"OWL-RL/");
				selectors = tmpSelector->append(selectors);
				return selectors;
			}


			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationProfileSelectorFactory::getSelectorsForELProfile(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationProfileRequestSelector(CReasonerEvaluationProfileRequestSelector::REPRS_PROFILE_EL,mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName+" :: OWL-EL");
				tmpSelector->setOutputString(mBaseOutputDir+"OWL-EL/");
				selectors = tmpSelector->append(selectors);
				return selectors;
			}


			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationProfileSelectorFactory::getSelectorsForQLProfile(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;
				CReasonerEvaluationGroupRequestSelector* tmpSelector = nullptr;
				tmpSelector = new CReasonerEvaluationProfileRequestSelector(CReasonerEvaluationProfileRequestSelector::REPRS_PROFILE_QL,mExpressivityDir);
				tmpSelector->setNameString(mBaseEvalName+" :: OWL-QL");
				tmpSelector->setOutputString(mBaseOutputDir+"OWL-QL/");
				selectors = tmpSelector->append(selectors);
				return selectors;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
