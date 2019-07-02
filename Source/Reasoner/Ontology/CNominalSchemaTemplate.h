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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CNOMINALSCHEMATEMPLATE_H
#define KONCLUDE_REASONER_ONTOLOGY_CNOMINALSCHEMATEMPLATE_H

// Libraries includes

// Namespace includes
#include "OntologySettings.h"
#include "CTagItem.h"
#include "CConcept.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CNominalSchemaTemplate
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CNominalSchemaTemplate : public CTagItem {
				// public methods
				public:
					//! Constructor
					CNominalSchemaTemplate();

					CNominalSchemaTemplate* initNominalSchemaTemplate(CBOXSET<CConcept*>* nomSchemaConSet, CBOXHASH<CConcept*,CConcept*>* conceptNomSchemaConceptsHash, CBOXHASH<CConcept*,CConcept*>* absorbableConceptNomSchemaConceptsHash);

					CNominalSchemaTemplate* setNominalSchemaTemplateTag(qint64 nomSchemTempTag);
					qint64 getNominalSchemaTemplateTag();

					CNominalSchemaTemplate* setTerminology(CTerminology* ontology);
					CTerminology* getTerminology();
					qint64 getTerminologyTag() const;

					CNominalSchemaTemplate* setTemplateConcept(CConcept* templateConcept);
					CConcept* getTemplateConcept();

					CNominalSchemaTemplate* setReferenceConcept(CConcept* referenceConcept);
					CConcept* getReferenceConcept();

					CBOXSET<CConcept*>* getNominalSchemaConceptSet();
					CNominalSchemaTemplate* setNominalSchemaConceptSet(CBOXSET<CConcept*>* nomSchemaConSet);

					CBOXHASH<CConcept*,CConcept*>* getTemplateConceptNominalSchemaConceptHash();
					CNominalSchemaTemplate* setTemplateConceptNominalSchemaConceptHash(CBOXHASH<CConcept*,CConcept*>* conceptNomSchemaConceptsHash);

					CBOXHASH<CConcept*,CConcept*>* getTemplateAbsorbableConceptNominalSchemaConceptHash();
					CNominalSchemaTemplate* setTemplateAbsorbableConceptNominalSchemaConceptHash(CBOXHASH<CConcept*,CConcept*>* conceptNomSchemaConceptsHash);

				// protected methods
				protected:

				// protected variables
				protected:
					CTerminology* mTerm;
					CConcept* mTemplConcept;
					CConcept* mRefConcept;

					CBOXSET<CConcept*>* mNomSchemaConSet;
					CBOXHASH<CConcept*,CConcept*>* mConceptNomSchemaConceptsHash;
					CBOXHASH<CConcept*,CConcept*>* mAbsorbableConceptNomSchemaConceptsHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CNOMINALSCHEMATEMPLATE_H
