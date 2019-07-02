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

#include "COntologyTriplesAssertionsAccessor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyTriplesAssertionsAccessor::COntologyTriplesAssertionsAccessor() {
			}

			bool COntologyTriplesAssertionsAccessor::visitIndividualAssertions(CIndividual* individual, COntologyTriplesIndividualAssertionsVisitor* visitor, bool visitConceptAssertions, bool visitRoleAssertions, bool visitReverseRoleAssertions, bool visitDataAssertions) {
				return visitIndividualAssertions(individual->getIndividualID(), visitor, visitConceptAssertions, visitRoleAssertions, visitReverseRoleAssertions, visitDataAssertions);
			}


			cint64 COntologyTriplesAssertionsAccessor::getIndividualId(const QString& indiName) {
				cint64 indiId = -1;
				visitIndividualId(indiName, [&](cint64 visitedIndId)->bool {
					indiId = visitedIndId;
					return true;
				});
				return indiId;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
