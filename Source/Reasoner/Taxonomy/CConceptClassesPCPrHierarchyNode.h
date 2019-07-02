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

#ifndef KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPCPRHIERARCHYNODE_H
#define KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPCPRHIERARCHYNODE_H

// Libraries includes
#include <QString>
#include <QStringList>

// Namespace includes
#include "TaxonomySettings.h"
#include "CPCPrHierarchyNode.h"

// Other includes
#include "KoncludeSettings.h"

#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CAbbreviatedIRIName.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CConceptClassesPCPrHierarchyNode
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CConceptClassesPCPrHierarchyNode : public CPCPrHierarchyNode<T,CConcept*> {
				// public methods
				public:
					//! Constructor
					CConceptClassesPCPrHierarchyNode(CConcept* representiveConcept);

					T* addEquivalentConcept(CConcept* concept);

					QList<CConcept*>* getEquivalentConceptList();

					CConcept* getOneEquivalentConcept();
					QString getEquivalentConceptsNodeString();
					QString getHierarchyConceptsNodeString(qint64 depth);

					QStringList getEquivalentConceptStringList(bool preferAbbreviatedNames);
					qint64 getEquivalentConceptCount();


				// protected methods
				protected:
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
					virtual void generateNodeRelationStrings();
					virtual void generateNodeElementStrings();
#endif

				// protected variables
				protected:


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CConceptClassesPCPrHierarchyNode.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPCPRHIERARCHYNODE_H
