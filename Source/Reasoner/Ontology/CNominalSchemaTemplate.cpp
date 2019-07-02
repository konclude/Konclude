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

#include "CNominalSchemaTemplate.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CNominalSchemaTemplate::CNominalSchemaTemplate() {
			}


			CNominalSchemaTemplate* CNominalSchemaTemplate::initNominalSchemaTemplate(CBOXSET<CConcept*>* nomSchemaConSet, CBOXHASH<CConcept*,CConcept*>* conceptNomSchemaConceptsHash, CBOXHASH<CConcept*,CConcept*>* absorbableConceptNomSchemaConceptsHash) {
				mConceptNomSchemaConceptsHash = conceptNomSchemaConceptsHash;
				mAbsorbableConceptNomSchemaConceptsHash = absorbableConceptNomSchemaConceptsHash;
				mNomSchemaConSet = nomSchemaConSet;
				mTerm = nullptr;
				mTemplConcept = nullptr;
				mRefConcept = nullptr;
				mTag = 0;
				return this;
			}

			CNominalSchemaTemplate* CNominalSchemaTemplate::setNominalSchemaTemplateTag(qint64 nomSchemTempTag) {
				mTag = nomSchemTempTag;
				return this;
			}

			qint64 CNominalSchemaTemplate::getNominalSchemaTemplateTag() {
				return mTag;
			}


			CNominalSchemaTemplate* CNominalSchemaTemplate::setTerminology(CTerminology* ontology) {
				mTerm = ontology;
				return this;
			}

			CTerminology* CNominalSchemaTemplate::getTerminology() {
				return mTerm;
			}

			qint64 CNominalSchemaTemplate::getTerminologyTag() const {
				cint64 termTag = 0;
				if (mTerm) {
					termTag = mTerm->getTerminologyID();
				}
				return termTag;
			}


			CNominalSchemaTemplate* CNominalSchemaTemplate::setTemplateConcept(CConcept* templateConcept) {
				mTemplConcept = templateConcept;
				return this;
			}

			CConcept* CNominalSchemaTemplate::getTemplateConcept() {
				return mTemplConcept;
			}

			CNominalSchemaTemplate* CNominalSchemaTemplate::setReferenceConcept(CConcept* referenceConcept) {
				mRefConcept = referenceConcept;
				return this;
			}

			CConcept* CNominalSchemaTemplate::getReferenceConcept() {
				return mRefConcept;
			}

			CBOXSET<CConcept*>* CNominalSchemaTemplate::getNominalSchemaConceptSet() {
				return mNomSchemaConSet;
			}

			CNominalSchemaTemplate* CNominalSchemaTemplate::setNominalSchemaConceptSet(CBOXSET<CConcept*>* nomSchemaConSet) {
				mNomSchemaConSet = nomSchemaConSet;
				return this;
			}

			CBOXHASH<CConcept*,CConcept*>* CNominalSchemaTemplate::getTemplateConceptNominalSchemaConceptHash() {
				return mConceptNomSchemaConceptsHash;
			}

			CNominalSchemaTemplate* CNominalSchemaTemplate::setTemplateConceptNominalSchemaConceptHash(CBOXHASH<CConcept*,CConcept*>* conceptNomSchemaConceptsHash) {
				mConceptNomSchemaConceptsHash = conceptNomSchemaConceptsHash;
				return this;
			}


			CBOXHASH<CConcept*,CConcept*>* CNominalSchemaTemplate::getTemplateAbsorbableConceptNominalSchemaConceptHash() {
				return mAbsorbableConceptNomSchemaConceptsHash;
			}

			CNominalSchemaTemplate* CNominalSchemaTemplate::setTemplateAbsorbableConceptNominalSchemaConceptHash(CBOXHASH<CConcept*,CConcept*>* conceptNomSchemaConceptsHash) {
				mAbsorbableConceptNomSchemaConceptsHash = conceptNomSchemaConceptsHash;
				return this;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
