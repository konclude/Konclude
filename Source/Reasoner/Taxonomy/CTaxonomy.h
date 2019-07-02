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

#ifndef KONCLUDE_REASONER_TAXONOMY_CTAXONOMY_H
#define KONCLUDE_REASONER_TAXONOMY_CTAXONOMY_H

// Libraries includes


// Namespace includes
#include "TaxonomySettings.h"
#include "CHierarchyNode.h"
#include "CHierarchy.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CTaxonomy
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTaxonomy : public CHierarchy {
				// public methods
				public:
					//! Constructor
					CTaxonomy(CHierarchyNode *topHierarchyNode, CHierarchyNode *bottomHierarchyNode);
					CTaxonomy(CConcept *topConcept, CConcept *bottomConcept);

					//! Destructor
					virtual ~CTaxonomy();


					virtual CHierarchyNode *getTopHierarchyNode();
					virtual CHierarchyNode *getBottomHierarchyNode();

					virtual CHierarchyNode *getHierarchyNode(CConcept *concept, bool create = true);
					virtual bool hasHierarchyNode(CConcept *concept);

					virtual CTaxonomy *setConceptSatisfiable(CConcept *concept, bool satisfiable) = 0;
					virtual CTaxonomy *setConceptSubsumption(CConcept *subsumerConcept, CConcept *subsumedConcept) = 0;
					virtual CTaxonomy *setConceptNotSubsumption(CConcept *notSubsumerConcept, CConcept *notSubsumedConcept) = 0;

					virtual CTaxonomy *setConceptDisjoints(CConcept *disjointConcept1, CConcept *disjointConcept2) = 0;


					virtual bool isSubsumption(CConcept *subsumerConcept, CConcept *subsumedConcept);
					virtual bool isEquivalence(CConcept *concept1, CConcept *concept2);
					virtual bool isNotSubsumption(CConcept *notSubsumerConcept, CConcept *notSubsumedConcept);
					virtual bool isSatisfiable(CConcept *concept);
					virtual bool isNotSatisfiable(CConcept *concept);

					virtual bool isDisjoint(CConcept *disjointConcept1, CConcept *disjointConcept2);
					virtual bool isNotDisjoint(CConcept *disjointConcept1, CConcept *disjointConcept2);

					virtual bool isTopHierarchyNode(CConcept *concept);
					virtual bool isBottomHierarchyNode(CConcept *concept);

					virtual QString getConceptHierarchyString();

					virtual bool isComplete();

					virtual CTaxonomy *setTaxonomyComplete(bool completed = true);

					virtual bool isActiveNode(CHierarchyNode *node);
					virtual CTaxonomy *setActiveNode(CHierarchyNode *node, bool nodeActive = true);
					virtual CTaxonomy *setInactiveNode(CHierarchyNode *node, bool nodeInactive = true);

					virtual CHierarchyNode *getActiveNode(CHierarchyNode *node);

					virtual CTaxonomy *updateNodeEquivalences(CHierarchyNode *node);

					virtual QHash<CConcept*,CHierarchyNode*>* getConceptHierarchyNodeHash();

				// protected methods
				protected:
					virtual CHierarchyNode *createHierarchyNode(CConcept *concept);

				// protected variables
				protected:
					QHash<CConcept *,CHierarchyNode *> conNodeHash;
					CHierarchyNode *topNode;
					CHierarchyNode *bottomNode;

					bool topIsUnsatisfiable;
					bool taxonomyComplete;

					QList<CHierarchyNode*> nodeContainer;

					QSet<CHierarchyNode*> activeNodeSet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CTAXONOMY_H
