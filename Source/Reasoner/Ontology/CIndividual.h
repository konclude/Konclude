/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
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
#include "CIndividualIdentifier.h"
#include "CConceptAssertionLinker.h"
#include "CRoleAssertionLinker.h"
#include "CIndividualData.h"
#include "CReverseRoleAssertionLinker.h"
#include "CDataAssertionLinker.h"

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
			class CIndividual :  public CIndividualIdentifier, public CNamedItem, public CAllocationObject {
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
					CIndividual* addAssertionRoleLinker(CRoleAssertionLinker* assRoleLinker);
					CIndividual* setAssertionRoleLinker(CRoleAssertionLinker* assRoleLinker);

					CDataAssertionLinker* getAssertionDataLinker();
					CIndividual* addAssertionDataLinker(CDataAssertionLinker* assDataLinker);
					CIndividual* setAssertionDataLinker(CDataAssertionLinker* assDataLinker);


					CReverseRoleAssertionLinker* getReverseAssertionRoleLinker();
					CIndividual* addReverseAssertionRoleLinker(CReverseRoleAssertionLinker* revAssRoleLinker);
					CIndividual* setReverseAssertionRoleLinker(CReverseRoleAssertionLinker* revAssRoleLinker);

					CIndividual *initIndividual(qint64 indiID = 0);
					CIndividual *initIndividualCopy(CIndividual* individual, CMemoryAllocationManager* memMan);

					bool hasAssertedConcept(CConcept* concept);

					CIndividual* setAnonymousIndividual(bool anonymous);
					bool isAnonymousIndividual();

					CIndividual* setTemporaryFakeIndividual(bool temporaryFake);
					CIndividual* setTemporaryIndividual(bool temporary);
					bool isTemporaryIndividual();

					CIndividual* setFakeIndividual(bool fake);
					bool isFakeIndividual();


					bool hasIndividualData();
					CIndividualData* getIndividualData();
					CIndividual* setIndividualData(CIndividualData* individualData);

				// protected methods
				protected:

				// protected variables
				protected:
					CConceptAssertionLinker* mAssertionConceptLinker;
					CRoleAssertionLinker* mAssertionRoleLinker;
					CDataAssertionLinker* mAssertionDataLinker;
					CReverseRoleAssertionLinker* mReverseAssertionRoleLinker;
					CConcept* mNominalConcept;
					bool mAnonymousIndividual;
					bool mTemporaryIndividual;
					bool mFakeIndividual;

					CIndividualData* mIndividualData;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CINDIVIDUAL_H
