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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CINDIVIDUAL_H
#define KONCLUDE_REASONER_ONTOLOGY_CINDIVIDUAL_H

// Libraries includes
#include <QSet>
#include <QHash>

// Namespace includes
#include "OntologySettings.h"
#include "CNamedItem.h"
#include "CTagItem.h"
#include "CConceptAssertionLinker.h"
#include "CRoleAssertionLinker.h"

// Other includes
#include "Utilities/CAllocationObject.h"
#include "Utilities/CLinker.hpp"
#include "Utilities/Container/CNegLinker.h"
#include "Utilities/Memory/CMemoryAllocationManager.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CIndividual
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividual :  public CTagItem, public CNamedItem, public CAllocationObject {
				// public methods
				public:
					//! Constructor
					CIndividual(qint64 id = 0);

					bool hasIndividualName();
					CIndividual *setIndividualNameLinker(CLinker<CName *> *individualNameLinker);
					CIndividual *addIndividualNameLinker(CLinker<CName *> *individualNameLinker);
					CLinker<CName *> *getIndividualNameLinker();


					CConcept* getIndividualNominalConcept();
					CIndividual* setIndividualNominalConcept(CConcept* nomConcept);

					CConceptAssertionLinker* getAssertionConceptLinker();
					CIndividual* addAssertionConceptLinker(CConceptAssertionLinker* assConLinker);
					CIndividual* setAssertionConceptLinker(CConceptAssertionLinker* assConLinker);

					CRoleAssertionLinker* getAssertionRoleLinker();
					CIndividual* addAssertionRoleLinker(CRoleAssertionLinker* assConLinker);
					CIndividual* setAssertionRoleLinker(CRoleAssertionLinker* assConLinker);

					CIndividual *initIndividual(qint64 indiID = 0);
					CIndividual *initIndividualCopy(CIndividual* individual, CMemoryAllocationManager* memMan);

					CIndividual *setIndividualID(qint64 id);
					qint64 getIndividualID();
					bool hasAssertedConcept(CConcept* concept);


				// protected methods
				protected:

				// protected variables
				protected:
					CConceptAssertionLinker* mAssertionConceptLinker;
					CRoleAssertionLinker* mAssertionRoleLinker;
					CConcept* mNominalConcept;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CINDIVIDUAL_H
