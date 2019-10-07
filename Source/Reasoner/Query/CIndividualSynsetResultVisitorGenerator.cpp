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

#include "CIndividualSynsetResultVisitorGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIndividualSynsetResultVisitorGenerator::CIndividualSynsetResultVisitorGenerator(CIndividualSynsetResult* indiSynsetResult, bool abbreviatedIRIs, bool integrateAnonymousIndividuals, CIndividualNameResolver* indiNameResolver) {
				mIndividualSynsetResult = indiSynsetResult;
				mAbbreviatedIRIs = abbreviatedIRIs;
				mIndiNameResolver = indiNameResolver;
				mIntegrateAnonymousIndividuals = integrateAnonymousIndividuals;
			}



			bool CIndividualSynsetResultVisitorGenerator::visitIndividual(const CIndividualReference& indiRef, CConceptRealization* conRealization) {
				if (mIntegrateAnonymousIndividuals || !mIndiNameResolver->isAnonymous(indiRef)) {
					QString individualString = mIndiNameResolver->getIndividualName(indiRef, mAbbreviatedIRIs);
					if (!individualString.isEmpty()) {
						mIndividualSynsetResult->addEquivalentIndividualName(individualString);
					}
				}
				return true;
			}



			bool CIndividualSynsetResultVisitorGenerator::visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
				if (mIntegrateAnonymousIndividuals || !mIndiNameResolver->isAnonymous(indiRef)) {
					QString individualString = mIndiNameResolver->getIndividualName(indiRef, mAbbreviatedIRIs);
					if (!individualString.isEmpty()) {
						mIndividualSynsetResult->addEquivalentIndividualName(individualString);
					}
				}
				return true;
			}



		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
