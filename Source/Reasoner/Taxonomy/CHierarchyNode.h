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

#ifndef KONCLUDE_REASONER_TAXONOMY_CHIERARCHYNODE_H
#define KONCLUDE_REASONER_TAXONOMY_CHIERARCHYNODE_H

// Libraries includes
#include <QStringList>

// Namespace includes
#include "TaxonomySettings.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CAbbreviatedIRIName.h"

#include "KoncludeSettings.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Taxonomy {

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

	//#define DEBUGHIERARCHYNODECONCEPTSTRINGS

#endif

			/*! 
			 *
			 *		\class		CHierarchyNode
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CHierarchyNode {
				// public methods
				public:
					//! Constructor
					CHierarchyNode(CConcept *representiveConcept);

					//! Destructor
					virtual ~CHierarchyNode();

					virtual CHierarchyNode *addParentNode(CHierarchyNode *parentNode);
					virtual CHierarchyNode *addChildNode(CHierarchyNode *childNode);

					virtual CHierarchyNode *makeParent(CHierarchyNode *parentNode);
					virtual CHierarchyNode *makeChild(CHierarchyNode *childNode);

					virtual CHierarchyNode *addEquivalentConcept(CConcept *concept);

					virtual bool hasChildNode(CHierarchyNode *node);
					virtual bool hasParentNode(CHierarchyNode *node);

					virtual CHierarchyNode *addPredecessorNode(CHierarchyNode *preNode);
					virtual CHierarchyNode *addSuccessorNode(CHierarchyNode *succNode);

					virtual CHierarchyNode *makePredecessor(CHierarchyNode *parentNode);
					virtual CHierarchyNode *makeSuccessor(CHierarchyNode *childNode);

					virtual bool hasSuccessorNode(CHierarchyNode *node);
					virtual bool hasPredecessorNode(CHierarchyNode *node);


					virtual CHierarchyNode *removeParent(CHierarchyNode *parentNode);
					virtual CHierarchyNode *removeChild(CHierarchyNode *childNode);
					virtual CHierarchyNode *removePredecessor(CHierarchyNode *predNode);
					virtual CHierarchyNode *removeSuccessor(CHierarchyNode *succNode);

					cint64 getPredecessorNodeCount() const;


					virtual QSet<CHierarchyNode *> *getParentNodeSet();
					virtual QSet<CHierarchyNode *> *getChildNodeSet();
					virtual QSet<CHierarchyNode *> *getPredecessorNodeSet();
					virtual QSet<CHierarchyNode *> *getSuccessorNodeSet();
					virtual QList<CConcept *> *getEquivalentConceptList();

					virtual QList<CHierarchyNode *> getParentNodeList();
					virtual QList<CHierarchyNode *> getChildNodeList();
					virtual QList<CHierarchyNode *> getPredecessorNodeList();
					virtual QList<CHierarchyNode *> getSuccessorNodeList();


					virtual CHierarchyNode *addIncompatiblePredecessorNode(CHierarchyNode *incompNode);
					virtual CHierarchyNode *makeIncompatiblePredecessor(CHierarchyNode *incompNode);
					virtual bool hasIncompatiblePredecessorNode(CHierarchyNode *node);
					virtual CHierarchyNode *removeIncompatiblePredecessorNode(CHierarchyNode *incompNode);


					virtual CHierarchyNode *addDisjointNode(CHierarchyNode *disjNode);
					virtual CHierarchyNode *makeDisjoint(CHierarchyNode *disjNode);
					virtual bool hasDisjointNode(CHierarchyNode *node);
					virtual CHierarchyNode *removeDisjointNode(CHierarchyNode *disjNode);


					virtual CHierarchyNode *addIncompatibleSuccessorNode(CHierarchyNode *incompNode);
					virtual CHierarchyNode *makeIncompatibleSuccessor(CHierarchyNode *incompNode);
					virtual bool hasIncompatibleSuccessorNode(CHierarchyNode *node);
					virtual CHierarchyNode *removeIncompatibleSuccessorNode(CHierarchyNode *incompNode);


					virtual QSet<CHierarchyNode *> *getIncompatibleSuccessorNodeSet();
					virtual QSet<CHierarchyNode *> *getIncompatiblePredecessorNodeSet();
					virtual QList<CHierarchyNode *> getIncompatibleSuccessorNodeList();
					virtual QList<CHierarchyNode *> getIncompatiblePredecessorNodeList();

					virtual QSet<CHierarchyNode *> *getDisjointNodeSet();
					virtual QList<CHierarchyNode *> getDisjointNodeList();

					virtual CConcept *getOneEquivalentConcept();
					virtual QString getEquivalentConceptsNodeString();
					virtual QString getHierarchyConceptsNodeString(qint64 depth);

					virtual QStringList getEquivalentConceptStringList(bool preferAbbreviatedNames);
					virtual qint64 getEquivalentConceptCount();

					virtual bool isActive();
					virtual CHierarchyNode *setActive(bool activeNode = true);
					virtual CHierarchyNode *setInactive(bool inactiveNode = true);

					virtual bool isNeedExplicitMentioned();
					virtual CHierarchyNode *setNeedExplicitMentioned(bool needsExplicitMentioned = true);

				// protected methods
				protected:
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
					virtual CHierarchyNode *generateNodeStrings();
#endif

				// protected variables
				protected:
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
					QStringList nodeStrings;
					QStringList parentNodeStrings;
					QStringList childNodeStrings;
					QStringList precNodeStrings;
					QStringList succNodeStrings;
					QStringList incPrecNodeStrings;
					QStringList incSuccNodeStrings;
					QStringList disjointNodeStrings;
#endif

					QList<CConcept *> eqConList;
					qint64 eqConCount;

					bool active;
					bool mention;

					QSet<CHierarchyNode *> parentNodeSet;
					QSet<CHierarchyNode *> childNodeSet;

					QSet<CHierarchyNode *> predNodeSet;
					QSet<CHierarchyNode *> succNodeSet;

					QSet<CHierarchyNode *> incompPredNodeSet;
					QSet<CHierarchyNode *> incompSuccNodeSet;

					QSet<CHierarchyNode *> disjointNodeSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CHIERARCHYNODE_H
