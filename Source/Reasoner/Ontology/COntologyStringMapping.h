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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYSTRINGMAPPING_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYSTRINGMAPPING_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyContext.h"


// Other includes
#include "Reasoner/Generator/CStringRefStringHasher.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Generator;

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyStringMapping
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyStringMapping {
				// public methods
				public:
					//! Constructor
					COntologyStringMapping(COntologyContext* ontologyContext = nullptr);

					//! Destructor
					virtual ~COntologyStringMapping();

					COntologyStringMapping* referenceStringMapping(COntologyStringMapping* stringMapping);

					CMAPPINGHASH<CStringRefStringHasher,CConcept*>* getClassNameConceptMappingHash(bool create = true);
					CMAPPINGHASH<CStringRefStringHasher,CRole*>* getPropertyNameRoleMappingHash(bool create = true);
					CMAPPINGHASH<CStringRefStringHasher,CIndividual*>* getIndividualNameIndividualMappingHash(bool create = true);

					CMAPPINGHASH<QString,CNamePrefix*>* getAbbreviatedNamePrefixHash(bool create = true);
					CMAPPINGHASH<QString,CNamePrefix*>* getNamePrefixHash(bool create = true);

					CMAPPINGLIST<CNamePrefix*>* getPrefixList(bool create = true);


					CConcept* getConceptFromName(const QString& fullClassName);
					CRole* getRoleFromName(const QString& fullPropertyName);
					CIndividual* getIndividualFromName(const QString& fullIndividualName);


				// protected methods
				protected:

				// protected variables
				protected:
					COntologyContext* mOntoContext;
					CMAPPINGHASH<CStringRefStringHasher,CConcept*>* mClassNameConceptMapHash;
					CMAPPINGHASH<CStringRefStringHasher,CRole*>* mPropertyNameRoleMapHash;
					CMAPPINGHASH<CStringRefStringHasher,CIndividual*>* mIndividualNameIndividualMapHash;

					CMAPPINGHASH<QString,CNamePrefix*>* mAbbreviatedNamePrefixMapHash;
					CMAPPINGHASH<QString,CNamePrefix*>* mNamePrefixMapHash;

					CMAPPINGLIST<CNamePrefix*>* mPrefixList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYSTRINGMAPPING_H
