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

#include "CClassSynsetsResultVisitorGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CClassSynsetsResultVisitorGenerator::CClassSynsetsResultVisitorGenerator(CClassSynsetsResult* classSynsetsResult, bool abbreviatedIRIs) {
				mClassSynsetsResult = classSynsetsResult;
				mAbbreviatedIRIs = abbreviatedIRIs;
			}

			bool CClassSynsetsResultVisitorGenerator::visitType(CConceptInstantiatedItem* item, CConceptRealization* conRealization) {
				mTmpClassSynsetResult = new CClassSynsetResult();
				conRealization->visitConcepts(item,this);
				mClassSynsetsResult->addClassSynset(mTmpClassSynsetResult);
				return true;
			}

			bool CClassSynsetsResultVisitorGenerator::visitConcept(CConcept* concept, CConceptRealization* conRealization) {
				QString conceptString;
				if (mAbbreviatedIRIs) {
					conceptString = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(concept->getClassNameLinker());
				} 
				if (conceptString.isEmpty()) {
					conceptString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
				}
				if (!conceptString.isEmpty()) {
					mTmpClassSynsetResult->addEquivalentClassName(conceptString);
				}
				return true;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
