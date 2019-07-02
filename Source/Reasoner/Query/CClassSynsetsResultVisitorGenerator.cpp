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
