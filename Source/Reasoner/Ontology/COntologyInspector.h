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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINSPECTOR_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINSPECTOR_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyStructureSummary.h"
#include "CConcreteOntology.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyInspector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyInspector {
				// public methods
				public:
					//! Constructor
					COntologyInspector();

					//! Destructor
					virtual ~COntologyInspector();

					virtual COntologyStructureSummary *inspectConcept(CConcreteOntology *ontology, CConcept* concept, bool negation, COntologyStructureSummary *ontStructSum);
					virtual COntologyStructureSummary *inspectOntology(CConcreteOntology *ontology);
					virtual CBOXSET<CConcept*> *createConceptContainsSet(CConcept *concept, CTBox *tBox, CBOXSET<CConcept*> *containsSet = nullptr);

					virtual COntologyInspector *createGCIConceptSet(CTBox *tBox);
					virtual COntologyStructureSummary *createStructureSummary(CConcreteOntology *ontology);

					bool testOntologyForNonDeterministicConcepts(CConcreteOntology *ontology, COntologyStructureSummary *ontStructSum = nullptr);
					bool testOntologyForNonELConcepts(CConcreteOntology *ontology, COntologyStructureSummary *ontStructSum = nullptr);

					bool analyzeOntologyConceptsStructureFlags(CConcreteOntology *ontology, COntologyStructureSummary *ontStructSum = nullptr);

				// protected methods
				protected:

					class CStructureFlags {
						public:

							CStructureFlags() {
								mValidDeterministic = true;
								mNominalOccurence = false;
								mUniversalRoleOccurence = false;
								mValidEL = true;
							}

							bool mValidEL;
							bool mValidDeterministic;
							bool mNominalOccurence;
							bool mUniversalRoleOccurence;
					};

					bool analyseConceptStructureFlags(CConcept* concept, bool negated, QSet< QPair<CConcept*,bool> >& singleConNegSet, QHash<CRole*,bool>* existRoleHash, CStructureFlags& flags);

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINSPECTOR_H
