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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_COCCURRENCESTATISTICSCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_COCCURRENCESTATISTICSCACHEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Reasoner/Ontology/CConceptProcessData.h"

#include "Reasoner/Kernel/Cache/COccurrenceStatisticsCacheWriter.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			using namespace Cache;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		COccurrenceStatisticsCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceStatisticsCacheHandler {
					// public methods
					public:
						//! Constructor
						COccurrenceStatisticsCacheHandler(COccurrenceStatisticsCacheWriter* occStatsCacheWriter);

						COccurrenceStatisticsCacheHandler* configureOntology(CConcreteOntology* ontology);

						COccurrenceStatisticsCacheHandler* incConceptInstanceOccurrencceStatistics(CConcept* concept, bool nondeterministic, bool existential);
						COccurrenceStatisticsCacheHandler* incRoleInstanceOccurrencceStatistics(CRole* role, bool nondeterministic, bool existential, bool newOutgoing);

						COccurrenceStatisticsCacheHandler* incConceptInstanceOccurrencceStatistics(CConcept* concept, cint64 deterministicCount, cint64 nondeterministicCount, cint64 individualCount, cint64 existentialCount);
						COccurrenceStatisticsCacheHandler* incRoleInstanceOccurrencceStatistics(CRole* role, cint64 deterministicCount, cint64 nondeterministicCount, cint64 individualCount, cint64 existentialCount, cint64 outgoingCount, cint64 incomingCount);


					// protected classes
					protected:

					// protected methods
					protected:

					// protected variables
					protected:
						COccurrenceStatisticsCacheWriter* mOccStatsCacheWriter;
						cint64 mOntologyId;
						CConcreteOntology* mOntology;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_COCCURRENCESTATISTICSCACHEHANDLER_H
