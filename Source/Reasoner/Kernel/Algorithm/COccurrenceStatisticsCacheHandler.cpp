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

#include "COccurrenceStatisticsCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				COccurrenceStatisticsCacheHandler::COccurrenceStatisticsCacheHandler(COccurrenceStatisticsCacheWriter* occStatsCacheWriter) {
					mOccStatsCacheWriter = occStatsCacheWriter;
					mOntologyId = -1;
					mOntology = nullptr;
				}

				COccurrenceStatisticsCacheHandler* COccurrenceStatisticsCacheHandler::configureOntology(CConcreteOntology* ontology) {
					mOntologyId = ontology->getOntologyID();
					mOntology = ontology;
					return this;
				}

				COccurrenceStatisticsCacheHandler* COccurrenceStatisticsCacheHandler::incConceptInstanceOccurrencceStatistics(CConcept* concept, bool nondeterministic, bool existential) {
					cint64 deterministicCount = 0, nondeterministicCount = 0, individualCount = 0, existentialCount = 0;
					if (nondeterministic) {
						nondeterministicCount = 1;
					} else {
						deterministicCount = 1;
					}
					if (existential) {
						existentialCount = 1;
					} else {
						individualCount = 1;
					}
					incConceptInstanceOccurrencceStatistics(concept, deterministicCount, nondeterministicCount, individualCount, existentialCount);
					return this;
				}

				COccurrenceStatisticsCacheHandler* COccurrenceStatisticsCacheHandler::incRoleInstanceOccurrencceStatistics(CRole* role, bool nondeterministic, bool existential, bool newOutgoing) {
					cint64 deterministicCount = 0, nondeterministicCount = 0, individualCount = 0, existentialCount = 0, outgoingCount = 0, incomingCount = 0;
					if (nondeterministic) {
						nondeterministicCount = 1;
					} else {
						deterministicCount = 1;
					}
					if (existential) {
						existentialCount = 1;
					} else {
						individualCount = 1;
					}
					if (newOutgoing) {
						outgoingCount = 1;
					}
					incRoleInstanceOccurrencceStatistics(role, deterministicCount, nondeterministicCount, individualCount, existentialCount, outgoingCount, incomingCount);
					return this;
				}


				COccurrenceStatisticsCacheHandler* COccurrenceStatisticsCacheHandler::incConceptInstanceOccurrencceStatistics(CConcept* concept, cint64 deterministicCount, cint64 nondeterministicCount, cint64 individualCount, cint64 existentialCount) {
					mOccStatsCacheWriter->incConceptInstanceOccurrencceStatistics(mOntology, concept->getConceptTag(), deterministicCount, nondeterministicCount, individualCount, existentialCount);
					return this;
				}

				COccurrenceStatisticsCacheHandler* COccurrenceStatisticsCacheHandler::incRoleInstanceOccurrencceStatistics(CRole* role, cint64 deterministicCount, cint64 nondeterministicCount, cint64 individualCount, cint64 existentialCount, cint64 outgoingCount, cint64 incomingCount) {
					mOccStatsCacheWriter->incRoleInstanceOccurrencceStatistics(mOntology, role->getRoleTag(), deterministicCount, nondeterministicCount, individualCount, existentialCount, outgoingCount, incomingCount);
					return this;
				}

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
