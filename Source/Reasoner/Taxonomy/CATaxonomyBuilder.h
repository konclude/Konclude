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

#ifndef KONCLUDE_REASONER_TAXONOMY_CATAXONOMYBUILDER_H
#define KONCLUDE_REASONER_TAXONOMY_CATAXONOMYBUILDER_H

// Libraries includes


// Namespace includes
#include "TaxonomySettings.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CATaxonomyBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class N, class E>
			class CATaxonomyBuilder {
				// public methods
				public:
					//! Constructor
					CATaxonomyBuilder();


					virtual N* getTopHierarchyNode() = 0;
					virtual N* getBottomHierarchyNode() = 0;

					virtual N* getHierarchyNode(const E& element, bool create) = 0;

					virtual bool hasHierarchyNode(const E& element) = 0;

					T* setElementSatisfiable(const E& element, bool satisfiable);
					T* setElementSatisfiable(const E& element);
					T* setElementUnsatisfiable(const E& element);
					T* setElementSubsumption(const E& subsumerElement, const E& subsumedElement);
					T* setElementEquivalence(const E& element1, const E& element2);


					T* setNodeSatisfiable(N* node, bool satisfiable);
					virtual T* setNodeSatisfiable(N* node) = 0;
					virtual T* setNodeUnsatisfiable(N* node) = 0;
					virtual T* setNodeSubsumption(N* subsumerNode, N* subsumedNode) = 0;
					virtual T* setNodeEquivalence(N* eqNode1, N* eqNode2) = 0;


					virtual T* setTaxonomyComplete(bool completed = true) = 0;

					virtual T* setActiveNode(N* node, bool nodeActive = true) = 0;
					virtual T* setInactiveNode(N* node, bool nodeInactive = true) = 0;



				// protected methods
				protected:

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

#include "CATaxonomyBuilder.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CATAXONOMYBUILDER_H
