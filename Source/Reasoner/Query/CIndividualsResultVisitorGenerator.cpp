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

#include "CIndividualsResultVisitorGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIndividualsResultVisitorGenerator::CIndividualsResultVisitorGenerator(CIndividualsResult* individualsResult, bool abbreviatedIRIs, bool integrateAnonymousIndividuals, CIndividualNameResolver* indiNameResolver) {
				mIndividualsResult = individualsResult;
				mAbbreviatedIRIs = abbreviatedIRIs;
				mIndiNameResolver = indiNameResolver;
				mIntegrateAnonymousIndividuals = integrateAnonymousIndividuals;
			}

			bool CIndividualsResultVisitorGenerator::visitIndividual(const CIndividualReference& indiRef, CConceptRealization* conRealization) {
				if (mIntegrateAnonymousIndividuals || !mIndiNameResolver->isAnonymous(indiRef)) {
					QString individualString = mIndiNameResolver->getIndividualName(indiRef, mAbbreviatedIRIs);
					if (!individualString.isEmpty()) {
						mIndividualsResult->addIndividual(individualString);
					}
				}
				return true;
			}


			bool CIndividualsResultVisitorGenerator::visitIndividual(const CIndividualReference& indiRef, CRoleRealization* roleRealization) {
				if (mIntegrateAnonymousIndividuals || !mIndiNameResolver->isAnonymous(indiRef)) {
					QString individualString = mIndiNameResolver->getIndividualName(indiRef, mAbbreviatedIRIs);
					if (!individualString.isEmpty()) {
						mIndividualsResult->addIndividual(individualString);
					}
				}
				return true;
			}


			bool CIndividualsResultVisitorGenerator::visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
				if (mIntegrateAnonymousIndividuals || !mIndiNameResolver->isAnonymous(indiRef)) {
					QString individualString = mIndiNameResolver->getIndividualName(indiRef, mAbbreviatedIRIs);
					if (!individualString.isEmpty()) {
						mIndividualsResult->addIndividual(individualString);
					}
				}
				return true;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
