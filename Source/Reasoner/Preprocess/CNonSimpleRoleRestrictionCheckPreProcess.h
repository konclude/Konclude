/*
 *		Copyright (C) 2013-2015, 2019, 2026 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_PREPROCESS_CNONSIMPLEROLERESTRICTIONCHECKPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CNONSIMPLEROLERESTRICTIONCHECKPREPROCESS_H

// Libraries includes
#include <QSet>
#include <QStringList>

// Namespace includes
#include "CConcreteOntologyPreProcess.h"


// Other includes
#include "Reasoner/Ontology/CIRIName.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CNonSimpleRoleRestrictionCheckPreProcess
			 *		\version	0.1
			 *		\brief		Warns about the restrictions that OWL 2 DL allows on simple roles only
			 *
			 *		A role is non-simple if it is transitive, the super role of a property chain, or a super role
			 *		of a non-simple role, and so is its inverse. OWL 2 DL does not allow cardinality and self
			 *		restrictions on such a role, nor functional, inverse functional, irreflexive, asymmetric and
			 *		disjoint property axioms; reflexive ones are allowed. Konclude accepted them silently, and the classification of an
			 *		ontology with cardinality restrictions on non-simple roles varied from run to run (issue #32),
			 *		so the step reports them once, after the role hierarchy is known. Other reasoners reject such
			 *		an ontology; Konclude still reasons with it, without a guarantee.
			 *
			 */
			class CNonSimpleRoleRestrictionCheckPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CNonSimpleRoleRestrictionCheckPreProcess();

					//! Destructor
					virtual ~CNonSimpleRoleRestrictionCheckPreProcess();

					virtual CConcreteOntology* preprocess(CConcreteOntology* ontology, CPreProcessContext* context);

				// protected methods
				protected:
					void markNonSimpleRole(CRole* role, QSet<CRole*>& nonSimpleRoleSet);
					QString getRoleName(CRole* role);
					QString getRoleNameListString(const QSet<QString>& roleNameSet);

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CNONSIMPLEROLERESTRICTIONCHECKPREPROCESS_H
