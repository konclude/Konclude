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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYCONTEXTCOLLECTIONMEMORYMANAGERSINGLETONPROVIDER_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYCONTEXTCOLLECTIONMEMORYMANAGERSINGLETONPROVIDER_H

// Libraries includes


// Namespace includes
#include "COntologyContextMemoryManager.h"
#include "COntologyContextCollectionMemoryManager.h"
#include "COntologyContextMemoryManagerProvider.h"

// Other includes
#include "Utilities/CAllocationObject.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			namespace Memory {

				/*! 
				 *
				 *		\class		COntologyContextCollectionMemoryManagerSingletonProvider
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COntologyContextCollectionMemoryManagerSingletonProvider : public COntologyContextMemoryManagerProvider {
					// public methods
					public:
						//! Constructor
						COntologyContextCollectionMemoryManagerSingletonProvider();

						//! Destructor
						virtual ~COntologyContextCollectionMemoryManagerSingletonProvider();

						virtual COntologyContextMemoryManager *getOntologyMemoryManager();

						static COntologyContextCollectionMemoryManagerSingletonProvider *getInstance();


					// protected methods
					protected:

					// protected variables
					protected:
						static COntologyContextCollectionMemoryManagerSingletonProvider *instance;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Memory

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYCONTEXTCOLLECTIONMEMORYMANAGERSINGLETONPROVIDER_H
