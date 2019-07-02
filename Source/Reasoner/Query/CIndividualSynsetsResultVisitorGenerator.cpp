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

#include "CIndividualSynsetsResultVisitorGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIndividualSynsetsResultVisitorGenerator::CIndividualSynsetsResultVisitorGenerator(CIndividualSynsetsResult* classSynsetsResult, bool abbreviatedIRIs, CIndividualNameResolver* indiNameResolver) {
				mIndividualSynsetsResult = classSynsetsResult;
				mAbbreviatedIRIs = abbreviatedIRIs;
				mIndiNameResolver = indiNameResolver;
			}

			bool CIndividualSynsetsResultVisitorGenerator::visitRoleInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealization* roleRealization) {
				mTmpIndividualSynsetResult = new CIndividualSynsetResult();
				roleRealization->visitIndividuals(indiRealItemRef,this);
				mIndividualSynsetsResult->addIndividualSynset(mTmpIndividualSynsetResult);
				return true;
			}


			bool CIndividualSynsetsResultVisitorGenerator::visitInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealization* conRealization) {
				mTmpIndividualSynsetResult = new CIndividualSynsetResult();
				conRealization->visitIndividuals(indiRealItemRef,this);
				mIndividualSynsetsResult->addIndividualSynset(mTmpIndividualSynsetResult);
				return true;
			}

			bool CIndividualSynsetsResultVisitorGenerator::visitIndividual(const CIndividualReference& indiRef, CConceptRealization* conRealization) {
				QString individualString = mIndiNameResolver->getIndividualName(indiRef, mAbbreviatedIRIs);
				if (!individualString.isEmpty()) {
					mTmpIndividualSynsetResult->addEquivalentIndividualName(individualString);
				}
				return true;
			}


			bool CIndividualSynsetsResultVisitorGenerator::visitIndividual(const CIndividualReference& indiRef, CRoleRealization* roleRealization) {
				QString individualString = mIndiNameResolver->getIndividualName(indiRef, mAbbreviatedIRIs);
				if (!individualString.isEmpty()) {
					mTmpIndividualSynsetResult->addEquivalentIndividualName(individualString);
				}
				return true;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
