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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINSPECTOR_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINSPECTOR_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"

#include <QVector>
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

					/*! The set of (concept, negation) pairs already visited by analyseConceptStructureFlags, as two bits
					 *  per concept tag. It replaced a QSet of pairs, whose millions of insertions and lookups were a
					 *  fifth of the preprocessing of SNOMED CT AU (issue #92); the concept tags are dense. */
					class CVisitedConceptNegationSet {
						public:
							inline bool contains(CConcept* concept, bool negated) const {
								cint64 tag = concept->getConceptTag();
								return tag >= 0 && tag < mBits.size() && (mBits[tag] & (negated ? 2 : 1)) != 0;
							}
							inline void insert(CConcept* concept, bool negated) {
								cint64 tag = concept->getConceptTag();
								if (tag < 0) {
									return;
								}
								if (tag >= mBits.size()) {
									mBits.resize(qMax<cint64>(tag + 1, mBits.size() * 2 + 1024)); // new elements are value-initialised to zero
								}
								mBits[tag] |= (negated ? 2 : 1);
							}
						private:
							QVector<quint8> mBits;
					};

					bool analyseConceptStructureFlags(CConcept* concept, bool negated, CVisitedConceptNegationSet& singleConNegSet, QHash<CRole*,bool>* existRoleHash, CStructureFlags& flags);

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
