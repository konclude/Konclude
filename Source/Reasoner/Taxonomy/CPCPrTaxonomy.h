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

#ifndef KONCLUDE_REASONER_TAXONOMY_CPCPRTAXONOMY_H
#define KONCLUDE_REASONER_TAXONOMY_CPCPRTAXONOMY_H

// Libraries includes


// Namespace includes
#include "TaxonomySettings.h"
#include "CATaxonomy.h"
#include "CATaxonomyBuilder.h"
#include "CPCPrHierarchyNode.h"
#include "CHierarchyDepthTraversalIterator.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CPCPrTaxonomy
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class N, class E>
			class CPCPrTaxonomy : public CATaxonomy<T,N,E>, public CATaxonomyBuilder<T,N,E> {
				// public methods
				public:
					//! Constructor
					CPCPrTaxonomy();
					CPCPrTaxonomy(N* topHierarchyNode, N* bottomHierarchyNode);
					CPCPrTaxonomy(const E& topElement, const E& bottomElement);

					//! Destructor
					virtual ~CPCPrTaxonomy();

					T* initTaxonomy(const E& topElement, const E& bottomElement);



					virtual N* getTopHierarchyNode();
					virtual N* getBottomHierarchyNode();

					virtual N* getHierarchyNode(const E& element);

					virtual N* getHierarchyNode(const E& element, bool create);

					virtual bool hasHierarchyNode(const E& element);


					virtual bool isComplete();


					virtual bool isNodeSubsumption(N* subsumerNode, N* subsumedNode);
					virtual bool isNodeEquivalence(N* node1, N* node2);
					virtual bool isNodeSatisfiable(N* node);
					virtual bool isNodeNotSatisfiable(N* node);

					virtual bool isTopHierarchyNode(N* node);
					virtual bool isBottomHierarchyNode(N* node);


					virtual T* setTaxonomyComplete(bool completed = true);

					virtual bool isActiveNode(N* node);
					virtual T* setActiveNode(N* node, bool nodeActive = true);
					virtual T* setInactiveNode(N* node, bool nodeInactive = true);

					virtual N* getActiveNode(N* node);


					virtual T* setNodeSatisfiable(N* node);
					virtual T* setNodeUnsatisfiable(N* node);
					virtual T* setNodeSubsumption(N* subsumerNode, N* subsumedNode);
					virtual T* setNodeEquivalence(N* eqNode1, N* eqNode2);


					virtual CHierarchyDepthTraversalIterator<N> getDepthTraversalIterator(bool forceUnique = true);


				// protected methods
				protected:
					N* createHierarchyNode(const E& element);

					bool repairedParentNodeInsertion(N* node, N* insertParentNode);
					bool repairInsertion(N* node, N* insertParentNode);


				// protected variables
				protected:
					QHash<E,N*> elmNodeHash;
					N* topNode;
					N* bottomNode;

					bool taxonomyComplete;

					QList<N*> nodeContainer;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CPCPrTaxonomy.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CPCPRTAXONOMY_H
