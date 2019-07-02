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

#ifndef KONCLUDE_REASONER_PREPROCESS_CCONCEPTROLEINDIVIDUALLOCATOR_H
#define KONCLUDE_REASONER_PREPROCESS_CCONCEPTROLEINDIVIDUALLOCATOR_H

// Libraries includes


// Namespace includes


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CConceptRoleIndividualLocator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptRoleIndividualLocator {
				// public methods
				public:
					//! Constructor
					CConceptRoleIndividualLocator();

					//! Destructor
					virtual ~CConceptRoleIndividualLocator();

					static CConcept* getLocatedConcept(CConcept* concept, CConcreteOntology* ontology);

					static CRole* getLocatedRole(CRole* role, CConcreteOntology* ontology);

					static CIndividual* getLocatedIndividual(CIndividual* individual, CConcreteOntology* ontology);

				// protected methods
				protected:

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CCONCEPTROLEINDIVIDUALLOCATOR_H
