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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CABOX_H
#define KONCLUDE_REASONER_ONTOLOGY_CABOX_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CIndividual.h"
#include "CBoxContext.h"
#include "CConcreteOntologyContextBase.h"
#include "CIndividualVector.h"
#include "CDataBox.h"


// Other includes
#include "Utilities/CSortedNegLinker.hpp"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CABox
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CABox : public CDataBox {
				// public methods
				public:
					//! Constructor
					CABox(CBoxContext* boxContext = nullptr);

					//! Destructor
					virtual ~CABox();


					CABox *setIndividualVector(CIndividualVector* roleVec);
					CIndividualVector* getIndividualVector(bool create = true);

					qint64 getIndividualCount();

					virtual CABox *referenceABox(CABox *aBox);
					virtual CABox *getABoxReferenceCopy(CBoxContext *boxContext);

					virtual CBoxContext *getBoxContext(bool create = true);

					cint64 getUniversalConnectionIndividualID();
					CABox* setUniversalConnectionIndividualID(cint64 univConnIndiID);

					bool hasUniversalConnectionIndividual();
					CABox* setHasUniversalConnectionIndividual(bool hasUnivConnIndi);


					CBOXSET<CIndividual*>* getActiveIndividualSet(bool create = true);
					CABox* setActiveIndividualSet(CBOXSET<CIndividual*>* takeActiveIndividualSet);


				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mUnivConnIndiID;
					bool mHasUnivConnIndiID;
					CIndividualVector* mIndiVec;

					CBOXSET<CIndividual*>* mActiveIndividualSet;

					CBoxContext *mBoxContext;
					CBoxContext *mDeleteBoxContext;
					CMemoryAllocationManager* mMemMan;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CABOX_H
