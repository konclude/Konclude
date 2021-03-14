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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDEXINGBASEBATCHLINKER_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDEXINGBASEBATCHLINKER_H

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
				*		\class		CRedlandStoredTriplesIndexingBaseBatchLinker
				*		\author		Andreas Steigmiller
				*		\version	0.1
				*		\brief		TODO
				*
				*/
			class CRedlandStoredTriplesIndexingBaseBatchLinker : public CLinkerBase<cint64, CRedlandStoredTriplesIndexingBaseBatchLinker> {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesIndexingBaseBatchLinker();

					CRedlandStoredTriplesIndexingBaseBatchLinker* initBatchLinker(CXLinker<librdf_statement*>* statementStartLinker, CXLinker<librdf_statement*>* statementEndLinker, cint64 count);
					CXLinker<librdf_statement*>* getNextStatementLinker();

				// protected methods
				protected:

				// protected variables
				protected:
					CXLinker<librdf_statement*>* mStatementStartLinker;
					CXLinker<librdf_statement*>* mStatementEndLinker;


				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDEXINGBASEBATCHLINKER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
