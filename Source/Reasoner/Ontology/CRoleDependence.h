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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CROLEDEPENDENCE_H
#define KONCLUDE_REASONER_ONTOLOGY_CROLEDEPENDENCE_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CRole.h"

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
			 *		\class		CRoleDependence
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRoleDependence : public CAllocationObject {
				// public methods
				public:
					//! Constructor
					CRoleDependence();

					//! Destructor
					virtual ~CRoleDependence();

					CRoleDependence *init(CRole *role);
					CRoleDependence *init(qint64 roleTag, CRoleDependence *role, CMemoryManager<CSortedNegLinker<qint64> > *linkerMemMan);

					CRoleDependence *setRole(CRole *role);

					CRoleDependence *setRoleTag(CRole *role);
					qint64 getRoleTag();

					CRoleDependence *setIndirectSubRoleList(CSortedNegLinker<qint64> *indirectDepSubRoleList);
					CRoleDependence *addIndirectSubRoleLinker(CSortedNegLinker<qint64> *indirectDepSubRoleLinker);
					CSortedNegLinker<qint64> *getIndirectSubRoleList();

					CRoleDependence *setSubRoleList(CSortedNegLinker<qint64> *depSubRoleList);
					CRoleDependence *addSubRoleLinker(CSortedNegLinker<qint64> *depSubRoleLinker);
					CSortedNegLinker<qint64> *getSubRoleList();

					CRoleDependence *setConceptDependenceList(CSortedNegLinker<qint64> *conceptDependenceList);
					CRoleDependence *addConceptDependenceLinker(CSortedNegLinker<qint64> *conceptDependenceLinker);
					CSortedNegLinker<qint64> *getConceptDependenceList();

					CRoleDependence *setIndividualDependenceList(CSortedNegLinker<qint64> *indiList);
					CRoleDependence *addIndividualDependenceLinker(CSortedNegLinker<qint64> *indiLinker);
					CSortedNegLinker<qint64> *getIndividualDependenceList();

					CRoleDependence *setIndirectChainedSubRoleList(CSortedNegLinker<qint64> *indirectChainDepSubRoleList);
					CRoleDependence *addIndirectChainedSubRoleLinker(CSortedNegLinker<qint64> *indirectChainDepSubRoleList);
					CSortedNegLinker<qint64> *getIndirectChainedSubRoleList();


				// protected methods
				protected:

				// private methods
				private:

				// private variables
				private:
					qint64 rolDepTag;

					CSortedNegLinker<qint64> *subRoleLinker;

					CSortedNegLinker<qint64> *indirectSubRoleLinker;

					CSortedNegLinker<qint64> *indirectChainedSubRoleLinker;

					CSortedNegLinker<qint64> *conceptDepLinker;

					CSortedNegLinker<qint64> *indiDepsLinker;

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CROLEDEPENDENCE_H
