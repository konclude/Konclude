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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYCONTEXT_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYCONTEXT_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"

// Other includes
#include "Context/CContext.h"
#include "Utilities/CAllocationObject.h"

// Logger includes
#include "Logger/CLogger.h"

#include "Utilities/Memory/CMemoryPool.h"

namespace Konclude {

	using namespace Context;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyContext
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptReferenceLinking;

			class COntologyContext : public CContext {
				// public methods
				public:
					//! Constructor
					COntologyContext();

					//! Destructor
					virtual ~COntologyContext();

					virtual COntologyContext* addUsedMemoryPools(CMemoryPool* memoryPools) = 0;
					/*!
					 *	Registers an object allocated in the context's memory pools that owns memory outside
					 *	them, such as the string of a name; its destructor is run before the pools are
					 *	released, which the release alone does not do (issue #45).
					 */
					virtual COntologyContext* addDestructiblePoolObject(CAllocationObject* object);
					/*!
					 *	Takes ownership of a linking data allocated on the heap and attached to a concept of the
					 *	ontology from a processing thread, which cannot allocate from the context's pools; it is
					 *	deleted with the context (issue #45).
					 */
					virtual COntologyContext* addOwnedConceptReferenceLinking(CConceptReferenceLinking* linking);

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYCONTEXT_H
