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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYINVERSEROLEBUILDER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYINVERSEROLEBUILDER_H

// Libraries includes
#include <QSet>

// Namespace includes


// Other includes
#include "Reasoner/Ontology/CBoxContext.h"
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CRoleDependence.h"
#include "Reasoner/Ontology/CMBox.h"
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Preprocess/CConceptRoleIndividualLocator.h"

#include "Utilities/CDynamicExpandingMemoryManager.hpp"
#include "Utilities/CSortedNegLinker.hpp"

#include "Parser/Expressions/CInverseObjectPropertyOfExpression.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Preprocess;

		namespace Generator {

			/*! 
			 *
			 *		\class		CConcreteOntologyInverseRoleBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyInverseRoleBuilder {
				// public methods
				public:
					//! Constructor
					CConcreteOntologyInverseRoleBuilder();

					//! Destructor
					virtual ~CConcreteOntologyInverseRoleBuilder();

					static CRole* createInverseRoleDelayed(CRole* role, QHash<CRole*,CRole*>* roleInverseRoleCreateHash, CConcreteOntology* onto);

					static bool createDelayedInverseRoles(QHash<CRole*,CRole*>* roleInverseRoleCreateHash, CConcreteOntology* onto);
					static CRole* createInverseRole(CRole* role, CConcreteOntology* onto);
					static CRole* getLocalizedRole(CRole* role, CConcreteOntology* onto);
					static CRole* getLocalizedRole(cint64 roleTag, CConcreteOntology* onto);
					static bool hasInverseRole(CRole* role, bool searchInverseEquivalent = true);
					static CRole* getInverseRole(CRole* role, bool searchInverseEquivalent = true);

				// protected methods
				protected:

				// private methods
				private:


				// private variables
				private:


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYINVERSEROLEBUILDER_H
