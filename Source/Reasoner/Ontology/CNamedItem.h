/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_ONTOLOGY_CNAMEDITEM_H
#define KONCLUDE_REASONER_ONTOLOGY_CNAMEDITEM_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CName.h"

// Other includes

#include "Utilities/CLinker.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CNamedItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CNamedItem {
				// public methods
				public:
					//! Constructor
					CNamedItem();

					//! Destructor
					virtual ~CNamedItem();

					bool hasName();
					CNamedItem *setNameLinker(CLinker<CName *> *nameLinker);
					CNamedItem *addNameLinker(CLinker<CName *> *nameLinker);
					CLinker<CName *> *getNameLinker();


				// protected methods
				protected:

				// protected variables
				protected:
					CLinker<CName*>* mNameLinker;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CNAMEDITEM_H
