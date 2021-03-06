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

#ifndef KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYBUILDER_H
#define KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYBUILDER_H

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
			 *		\class		CAbstractHierarchyBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class N, class E>
			class CAbstractHierarchyBuilder {
				// public methods
				public:
					//! Constructor
					CAbstractHierarchyBuilder();


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

#include "CAbstractHierarchyBuilder.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYBUILDER_H
