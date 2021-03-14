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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESDATAMODELITERATOR_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESDATAMODELITERATOR_H

// Libraries includes


// Namespace includes
#include "TriplesSettings.h"
#include "CTriplesDataIterator.h"


// Other includes
#include "Utilities/Container/CLinker.h"
#include "Utilities/UtilitiesSettings.h"

#include "Reasoner/Ontology/OntologySettings.h"

#include "redland.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Triples {




			/*! 
			 *
			 *		\class		CRedlandStoredTriplesDataModelIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesDataModelIterator : public CTriplesDataIterator {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesDataModelIterator(librdf_stream* stream);
					~CRedlandStoredTriplesDataModelIterator();



					virtual QString getNodeString(cint64 pos);
					virtual TRIPLE_NODE_TYPE getNodeType(cint64 pos);

					TRIPLE_NODE_TYPE getNodeType(librdf_node* node);


					virtual bool hasValue();
					virtual CTriplesDataIterator* moveNext();

				// protected methods
				protected:

				// protected variables
				protected:
					librdf_stream* mStream;
					QString mDefaultDatatypeString;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESDATAMODELITERATOR_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
