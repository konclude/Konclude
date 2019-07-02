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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CCONCEPTDEPENDENCE_H
#define KONCLUDE_REASONER_ONTOLOGY_CCONCEPTDEPENDENCE_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "OntologySettings.h"
#include "CConcept.h"

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
			 *		\class		CConceptDependence
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptDependence : public CAllocationObject {
				// public methods
				public:
					//! Constructor
					CConceptDependence();

					//! Destructor
					virtual ~CConceptDependence();

					CConceptDependence *init(CConcept *concept);
					CConceptDependence *init(qint64 conTag, CConceptDependence *conceptDep, CMemoryManager<CSortedNegLinker<qint64> > *linkerMemMan);

					qint64 getConceptDependenceTag();
					CConceptDependence *getConceptDependenceTag(qint64 tag);

					CConceptDependence *setConceptDependenceList(CSortedNegLinker<qint64> *conceptList);
					CConceptDependence *addConceptDependenceLinker(CSortedNegLinker<qint64> *conceptLinker);
					CSortedNegLinker<qint64> *getConceptDependenceList();


					CConceptDependence *setRoleDependenceList(CSortedNegLinker<qint64> *roleList);
					CConceptDependence *addRoleDependenceLinker(CSortedNegLinker<qint64> *roleLinker);
					CSortedNegLinker<qint64> *getRoleDependenceList();

					CConceptDependence *setIndividualDependenceList(CSortedNegLinker<qint64> *indiList);
					CConceptDependence *addIndividualDependenceLinker(CSortedNegLinker<qint64> *indiLinker);
					CSortedNegLinker<qint64> *getIndividualDependenceList();


				// protected methods
				protected:

				// private methods
				private:

				// private variables
				private:
					qint64 conceptDepTag;
					CSortedNegLinker<qint64> *conceptDepsLinker;
					CSortedNegLinker<qint64> *roleDepsLinker;
					CSortedNegLinker<qint64> *indiDepsLinker;

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CCONCEPTDEPENDENCE_H
