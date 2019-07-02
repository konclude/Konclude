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

#ifndef KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHY_H
#define KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHY_H

// Libraries includes


// Namespace includes
#include "TaxonomySettings.h"
#include "CAbstractHierarchy.h"
#include "CAbstractHierarchyBuilder.h"
#include "CParentChildPredecessorHierarchyNode.h"
#include "CHierarchyDepthTraversalIterator.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CParentChildPredecessorHierarchy
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class N, class E>
			class CParentChildPredecessorHierarchy : public CAbstractHierarchy<T,N,E>, public CAbstractHierarchyBuilder<T,N,E> {
				// public methods
				public:
					//! Constructor
					CParentChildPredecessorHierarchy();
					CParentChildPredecessorHierarchy(N* topHierarchyNode, N* bottomHierarchyNode);
					CParentChildPredecessorHierarchy(const E& topElement, const E& bottomElement);

					//! Destructor
					virtual ~CParentChildPredecessorHierarchy();

					T* initHierarchy(const E& topElement, const E& bottomElement);



					virtual N* getTopHierarchyNode();
					virtual N* getBottomHierarchyNode();

					virtual N* getHierarchyNode(const E& element);

					virtual N* getHierarchyNode(const E& element, bool create);

					virtual bool hasHierarchyNode(const E& element);



					virtual bool isNodeSubsumption(N* subsumerNode, N* subsumedNode);
					virtual bool isNodeEquivalence(N* node1, N* node2);
					virtual bool isNodeSatisfiable(N* node);
					virtual bool isNodeNotSatisfiable(N* node);

					virtual bool isTopHierarchyNode(N* node);
					virtual bool isBottomHierarchyNode(N* node);


					T* updateNodeEquivalentElements(N* node);


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


					QList<N*> nodeContainer;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CParentChildPredecessorHierarchy.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHY_H
