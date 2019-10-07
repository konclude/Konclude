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

#include "CPropertySynsetsResultVisitorGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CPropertySynsetsResultVisitorGenerator::CPropertySynsetsResultVisitorGenerator(CPropertySynsetsResult* propSynsetsResult, bool abbreviatedIRIs) {
				mPropertySynsetsResult = propSynsetsResult;
				mAbbreviatedIRIs = abbreviatedIRIs;
			}

			bool CPropertySynsetsResultVisitorGenerator::visitRoleInstantiated(CRoleInstantiatedItem* item, CRoleRealization* roleRealization) {
				mTmpPropertySynsetResult = new CPropertySynsetResult(mPropertySynsetsResult->hasDataProperties());
				roleRealization->visitRoles(item,this);
				mPropertySynsetsResult->addPropertySynset(mTmpPropertySynsetResult);
				return true;
			}

			bool CPropertySynsetsResultVisitorGenerator::visitRole(CRole* role, CRoleRealization* roleRealization) {
				QString roleString;
				if (mAbbreviatedIRIs) {
					roleString = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(role->getPropertyNameLinker());
				} 
				if (roleString.isEmpty()) {
					roleString = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
				}
				if (!roleString.isEmpty()) {
					mTmpPropertySynsetResult->addEquivalentPropertyName(roleString);
				}
				return true;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
