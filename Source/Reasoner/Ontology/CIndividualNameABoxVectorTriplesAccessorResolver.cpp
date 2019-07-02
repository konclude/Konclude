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

#include "CIndividualNameABoxVectorTriplesAccessorResolver.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CIndividualNameABoxVectorTriplesAccessorResolver::CIndividualNameABoxVectorTriplesAccessorResolver(CConcreteOntology* ontology) {
				mOntology = ontology;
			}

			QString CIndividualNameABoxVectorTriplesAccessorResolver::getIndividualName(const CIndividualReference& indiRef, bool abbreviated) const {
				CIndividual* indi = indiRef.getIndividual();
				QString individualString;
				if (!indi) {
					indi = mOntology->getABox()->getIndividualVector(false)->getData(indiRef.getIndividualID());
				}
				if (indi) {
					if (abbreviated) {
						individualString = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(indi->getIndividualNameLinker());
					}
					if (individualString.isEmpty()) {
						individualString = CIRIName::getRecentIRIName(indi->getIndividualNameLinker());
					}
				} else {
					COntologyTriplesAssertionsAccessor* accessor = mOntology->getOntologyTriplesData()->getTripleAssertionAccessor();
					if (accessor) {
						accessor->visitIndividualName(indiRef.getIndividualID(), [&](const QString& indiName)->bool {
							individualString = indiName;
							return true;
						});
					}
				}
				return individualString;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
