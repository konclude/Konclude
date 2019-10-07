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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CINDIVIDUALDEPENDENCE_H
#define KONCLUDE_REASONER_ONTOLOGY_CINDIVIDUALDEPENDENCE_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "OntologySettings.h"
#include "CIndividual.h"

// Other includes
#include "Utilities/CSortedNegLinker.hpp"
#include "Utilities/CAllocationObject.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CIndividualDependence
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualDependence : public CAllocationObject {
				// public methods
				public:
					//! Constructor
					CIndividualDependence();

					//! Destructor
					virtual ~CIndividualDependence();

					CIndividualDependence *init(CIndividual *indi);
					CIndividualDependence *init(qint64 indiTag, CIndividualDependence *indiDep, CMemoryManager<CSortedNegLinker<qint64> > *linkerMemMan);

					qint64 getIndividualDependenceTag();
					CIndividualDependence *setIndividualDependenceTag(qint64 tag);

					CIndividualDependence *setConceptDependenceList(CSortedNegLinker<qint64> *conceptList);
					CIndividualDependence *addConceptDependenceLinker(CSortedNegLinker<qint64> *conceptLinker);
					CSortedNegLinker<qint64> *getConceptDependenceList();

				// protected methods
				protected:

				// private methods
				private:

				// private variables
				private:
					qint64 indiDepTag;
					CSortedNegLinker<qint64> *conceptDepsLinker;

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CINDIVIDUALDEPENDENCE_H
