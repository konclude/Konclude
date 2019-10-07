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

#ifndef KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPARENTCHILDPREDECESSORHIERARCHYNODE_H
#define KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPARENTCHILDPREDECESSORHIERARCHYNODE_H

// Libraries includes
#include <QString>
#include <QStringList>

// Namespace includes
#include "TaxonomySettings.h"
#include "CParentChildPredecessorHierarchyNode.h"

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
			 *		\class		CConceptClassesParentChildPredecessorHierarchyNode
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CConceptClassesParentChildPredecessorHierarchyNode : public CParentChildPredecessorHierarchyNode<T,CConcept*> {
				// public methods
				public:
					//! Constructor
					CConceptClassesParentChildPredecessorHierarchyNode(CConcept* representiveConcept);

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

#include "CConceptClassesParentChildPredecessorHierarchyNode.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPARENTCHILDPREDECESSORHIERARCHYNODE_H
