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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDEXINGBASERESOLVINGLINKER_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDEXINGBASERESOLVINGLINKER_H

// Libraries includes
#include <Redland.h>


// Namespace includes
#include "TriplesSettings.h"


// Other includes
#include "Reasoner/Ontology/CIndividual.h"


#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Triples {



			/*! 
				*
				*		\class		CRedlandStoredTriplesIndexingBaseResolvingLinker
				*		\author		Andreas Steigmiller
				*		\version	0.1
				*		\brief		TODO
				*
				*/
			class CRedlandStoredTriplesIndexingBaseResolvingLinker : public CLinkerBase<cint64, CRedlandStoredTriplesIndexingBaseResolvingLinker> {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesIndexingBaseResolvingLinker();

					enum ASSERTION_TYPE {
						ASSERTION_NONE = 0,
						ASSERTION_INDIVIDUAL = 1,
						ASSERTION_CONCEPT = 2,
						ASSERTION_ROLE = 3,
						ASSERTION_ROLE_REVERSE = 4,
						ASSERTION_DATA = 5
					};

					librdf_statement* mStatement;

					CIndividual* mIndi1;
					cint64 mIndi1HashValue;

					CConcept* mConcept;
					CRole* mRole;
					CDataLiteral* mDataLiteral;

					CIndividual* mIndi2;
					cint64 mIndi2HashValue;

					CRoleAssertionLinker* mRoleAssLinker;


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDEXINGBASERESOLVINGLINKER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
