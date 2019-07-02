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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYTEXTFORMATER_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYTEXTFORMATER_H

// Libraries includes
#include <QPair>
#include <QDir>

// Namespace includes
#include "OntologySettings.h"
#include "CRole.h"
#include "CConcept.h"
#include "CConcreteOntology.h"
#include "CIRIName.h"
#include "CConceptTextFormater.h"
#include "CConceptProcessData.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyTextFormater
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyTextFormater {
				// public methods
				public:
					//! Constructor
					COntologyTextFormater();

					//! Destructor
					virtual ~COntologyTextFormater();

					static QString getConceptString(CConcept* concept, CConcreteOntology* ontology);
					static QString getTBoxConceptsString(CConcreteOntology* ontology);

					static QString getRoleString(CRole* role, CConcreteOntology* ontology);
					static QString getRBoxRolesString(CConcreteOntology* ontology);

					static QString getIndividualString(CIndividual* indi, CConcreteOntology* ontology);
					static QString getABoxIndividualsString(CConcreteOntology* ontology);
				
					static QString getOntologyString(CConcreteOntology* ontology);

					static bool writeOntologyToFile(CConcreteOntology* ontology, const QString& filename);

				// protected methods
				protected:

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYTEXTFORMATER_H
