/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYREVISION_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYREVISION_H

// Libraries includes


// Namespace includes
#include "RevisionSettings.h"
#include "COntologyConfigurationExtension.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Revision {

			/*! 
			 *
			 *		\class		COntologyRevision
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyRevision {
				// public methods
				public:
					//! Constructor
					COntologyRevision(CConcreteOntology *ontology = 0, COntologyRevision *previousOntologyRevision = 0, COntologyConfigurationExtension *ontRevConfig = 0);

					//! Destructor
					virtual ~COntologyRevision();

					virtual CConcreteOntology *getOntology();
					virtual COntologyRevision *setOntology(CConcreteOntology *ontology);

					virtual qint64 getOntologyRevisionDepth();
					virtual COntologyRevision *setOntologyRevisionDepth(qint64 depth);

					virtual COntologyRevision *getPreviousOntologyRevision();
					virtual COntologyRevision *getRootOntologyRevision();

					virtual qint64 getOntologyRevisionPath();
					virtual COntologyRevision *setOntologyRevisionPath(qint64 ontologyRevisionPath);

					virtual COntologyConfigurationExtension *getOntologyConfiguration();
					virtual COntologyRevision *setOntologyConfiguration(COntologyConfigurationExtension *ontRevConfig);


				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology *onto;
					qint64 revisionDepth;
					qint64 revisionPath;
					COntologyRevision *prevRev;
					COntologyRevision *rootRev;

					COntologyConfigurationExtension *config;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYREVISION_H
