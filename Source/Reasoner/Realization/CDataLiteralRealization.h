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
#ifndef KONCLUDE_REASONER_REALIZATION_CDATALITERALREALIZATION_H
#define KONCLUDE_REASONER_REALIZATION_CDATALITERALREALIZATION_H

// Libraries includes


// Namespace includes
#include "RealizationSettings.h"


// Other includes
#include "Reasoner/Ontology/CIndividualReference.h"
#include "Reasoner/Ontology/CDataLiteral.h"
#include "Reasoner/Ontology/CRole.h"


// Logger includes


namespace Konclude {

	namespace Reasoner {

		namespace Realization {

			/*! 
			 *
			 *		\class		CDataLiteralRealizationVisitor
			 *
			 *		\brief		Visitor for realized data roles and data values of an individual.
			 *
			 *		The visitor is used for both directions: the data roles that an individual instantiates
			 *		(with at least one value) and the values of a single data role.
			 *
			 */
			class CDataLiteralRealizationVisitor {
				// public methods
				public:
					virtual ~CDataLiteralRealizationVisitor() {}

					//! called for every data role that the individual instantiates;
					//! deterministic is false when at least one of the values is only possibly entailed
					virtual bool visitDataRole(CRole* dataRole, bool deterministic) = 0;

					//! called for every value of the data role; deterministic is false for values that
					//! were derived through a non-deterministic branch (and verified afterwards)
					virtual bool visitDataLiteral(CRole* dataRole, CDataLiteral* dataLiteral, bool deterministic) = 0;
			};


			/*! 
			 *
			 *		\class		CDataLiteralRealization
			 *
			 *		\brief		Realization of the data values (and data roles) of individuals.
			 *
			 *		The realization answers which data roles an individual instantiates and which data
			 *		literals are entailed for a data role. It is computed lazily, i.e., the data of an
			 *		individual is analysed when it is requested for the first time.
			 *
			 */
			class CDataLiteralRealization {
				// public methods
				public:
					virtual ~CDataLiteralRealization() {}

					//! visits the data roles that the individual instantiates (roles with at least one value)
					virtual bool visitDataRoles(const CIndividualReference& indiRef, CDataLiteralRealizationVisitor* visitor) = 0;

					//! visits the values of the data role for the individual
					virtual bool visitDataLiterals(const CIndividualReference& indiRef, CRole* dataRole, CDataLiteralRealizationVisitor* visitor) = 0;

					//! tests whether the individual has the data value (considering sub-roles of the role)
					virtual bool isDataLiteralInstance(CRole* dataRole, const CIndividualReference& indiRef, CDataLiteral* dataLiteral) = 0;

					//! whether the values of the individual for the role still have to be realized
					virtual bool requiresDataLiteralsRealization(const CIndividualReference& indiRef, CRole* dataRole) = 0;

					//! realizes (analyses) the data values of the individual on demand
					virtual bool realizeDataLiterals(const CIndividualReference& indiRef) = 0;

					//! whether all values of the role are deterministic (no pending verification)
					virtual bool hasOnlyDeterministicDataLiterals(CRole* dataRole) = 0;
			};


		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZATION_CDATALITERALREALIZATION_H
