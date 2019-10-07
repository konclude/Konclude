/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_TAXONOMY_CPCPRSUIPISDIHIERARCHYNODE_H
#define KONCLUDE_REASONER_TAXONOMY_CPCPRSUIPISDIHIERARCHYNODE_H

// Qt includes
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

	#define DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS

#endif

			/*! 
			 *
			 *		\class		CPCPrSuIpIsDiHierarchyNode
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPCPrSuIpIsDiHierarchyNode {
				// public methods
				public:
					//! Constructor
					CPCPrSuIpIsDiHierarchyNode(CConcept *representiveConcept);

					//! Deconstructor
					virtual ~CPCPrSuIpIsDiHierarchyNode();

					virtual CPCPrSuIpIsDiHierarchyNode *addParentNode(CPCPrSuIpIsDiHierarchyNode *parentNode);
					virtual CPCPrSuIpIsDiHierarchyNode *addChildNode(CPCPrSuIpIsDiHierarchyNode *childNode);

					virtual CPCPrSuIpIsDiHierarchyNode *makeParent(CPCPrSuIpIsDiHierarchyNode *parentNode);
					virtual CPCPrSuIpIsDiHierarchyNode *makeChild(CPCPrSuIpIsDiHierarchyNode *childNode);

					virtual CPCPrSuIpIsDiHierarchyNode *addEquivalentConcept(CConcept *concept);

					virtual bool hasChildNode(CPCPrSuIpIsDiHierarchyNode *node);
					virtual bool hasParentNode(CPCPrSuIpIsDiHierarchyNode *node);

					virtual CPCPrSuIpIsDiHierarchyNode *addPredecessorNode(CPCPrSuIpIsDiHierarchyNode *preNode);
					virtual CPCPrSuIpIsDiHierarchyNode *addSuccessorNode(CPCPrSuIpIsDiHierarchyNode *succNode);

					virtual CPCPrSuIpIsDiHierarchyNode *makePredecessor(CPCPrSuIpIsDiHierarchyNode *parentNode);
					virtual CPCPrSuIpIsDiHierarchyNode *makeSuccessor(CPCPrSuIpIsDiHierarchyNode *childNode);

					virtual bool hasSuccessorNode(CPCPrSuIpIsDiHierarchyNode *node);
					virtual bool hasPredecessorNode(CPCPrSuIpIsDiHierarchyNode *node);


					virtual CPCPrSuIpIsDiHierarchyNode *removeParent(CPCPrSuIpIsDiHierarchyNode *parentNode);
					virtual CPCPrSuIpIsDiHierarchyNode *removeChild(CPCPrSuIpIsDiHierarchyNode *childNode);
					virtual CPCPrSuIpIsDiHierarchyNode *removePredecessor(CPCPrSuIpIsDiHierarchyNode *predNode);
					virtual CPCPrSuIpIsDiHierarchyNode *removeSuccessor(CPCPrSuIpIsDiHierarchyNode *succNode);



					virtual QSet<CPCPrSuIpIsDiHierarchyNode *> *getParentNodeSet();
					virtual QSet<CPCPrSuIpIsDiHierarchyNode *> *getChildNodeSet();
					virtual QSet<CPCPrSuIpIsDiHierarchyNode *> *getPredecessorNodeSet();
					virtual QSet<CPCPrSuIpIsDiHierarchyNode *> *getSuccessorNodeSet();
					virtual QList<CConcept *> *getEquivalentConceptList();

					virtual QList<CPCPrSuIpIsDiHierarchyNode *> getParentNodeList();
					virtual QList<CPCPrSuIpIsDiHierarchyNode *> getChildNodeList();
					virtual QList<CPCPrSuIpIsDiHierarchyNode *> getPredecessorNodeList();
					virtual QList<CPCPrSuIpIsDiHierarchyNode *> getSuccessorNodeList();


					virtual CPCPrSuIpIsDiHierarchyNode *addIncompatiblePredecessorNode(CPCPrSuIpIsDiHierarchyNode *incompNode);
					virtual CPCPrSuIpIsDiHierarchyNode *makeIncompatiblePredecessor(CPCPrSuIpIsDiHierarchyNode *incompNode);
					virtual bool hasIncompatiblePredecessorNode(CPCPrSuIpIsDiHierarchyNode *node);
					virtual CPCPrSuIpIsDiHierarchyNode *removeIncompatiblePredecessorNode(CPCPrSuIpIsDiHierarchyNode *incompNode);


					virtual CPCPrSuIpIsDiHierarchyNode *addDisjointNode(CPCPrSuIpIsDiHierarchyNode *disjNode);
					virtual CPCPrSuIpIsDiHierarchyNode *makeDisjoint(CPCPrSuIpIsDiHierarchyNode *disjNode);
					virtual bool hasDisjointNode(CPCPrSuIpIsDiHierarchyNode *node);
					virtual CPCPrSuIpIsDiHierarchyNode *removeDisjointNode(CPCPrSuIpIsDiHierarchyNode *disjNode);


					virtual CPCPrSuIpIsDiHierarchyNode *addIncompatibleSuccessorNode(CPCPrSuIpIsDiHierarchyNode *incompNode);
					virtual CPCPrSuIpIsDiHierarchyNode *makeIncompatibleSuccessor(CPCPrSuIpIsDiHierarchyNode *incompNode);
					virtual bool hasIncompatibleSuccessorNode(CPCPrSuIpIsDiHierarchyNode *node);
					virtual CPCPrSuIpIsDiHierarchyNode *removeIncompatibleSuccessorNode(CPCPrSuIpIsDiHierarchyNode *incompNode);


					virtual QSet<CPCPrSuIpIsDiHierarchyNode *> *getIncompatibleSuccessorNodeSet();
					virtual QSet<CPCPrSuIpIsDiHierarchyNode *> *getIncompatiblePredecessorNodeSet();
					virtual QList<CPCPrSuIpIsDiHierarchyNode *> getIncompatibleSuccessorNodeList();
					virtual QList<CPCPrSuIpIsDiHierarchyNode *> getIncompatiblePredecessorNodeList();

					virtual QSet<CPCPrSuIpIsDiHierarchyNode *> *getDisjointNodeSet();
					virtual QList<CPCPrSuIpIsDiHierarchyNode *> getDisjointNodeList();

					virtual CConcept *getOneEquivalentConcept();
					virtual QString getEquivalentConceptsNodeString();
					virtual QString getHierarchyConceptsNodeString(qint64 depth);

					virtual QStringList getEquivalentConceptStringList(bool preferAbbreviatedNames);
					virtual qint64 getEquivalentConceptCount();

					virtual bool isActive();
					virtual CPCPrSuIpIsDiHierarchyNode *setActive(bool activeNode = true);
					virtual CPCPrSuIpIsDiHierarchyNode *setInactive(bool inactiveNode = true);

					virtual bool isNeedExplicitMentioned();
					virtual CPCPrSuIpIsDiHierarchyNode *setNeedExplicitMentioned(bool needsExplicitMentioned = true);

				// protected methods
				protected:
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
					virtual CPCPrSuIpIsDiHierarchyNode *generateNodeStrings();
#endif

				// protected variables
				protected:
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
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

					QSet<CPCPrSuIpIsDiHierarchyNode *> parentNodeSet;
					QSet<CPCPrSuIpIsDiHierarchyNode *> childNodeSet;

					QSet<CPCPrSuIpIsDiHierarchyNode *> predNodeSet;
					QSet<CPCPrSuIpIsDiHierarchyNode *> succNodeSet;

					QSet<CPCPrSuIpIsDiHierarchyNode *> incompPredNodeSet;
					QSet<CPCPrSuIpIsDiHierarchyNode *> incompSuccNodeSet;

					QSet<CPCPrSuIpIsDiHierarchyNode *> disjointNodeSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CPCPRSUIPISDIHIERARCHYNODE_H
