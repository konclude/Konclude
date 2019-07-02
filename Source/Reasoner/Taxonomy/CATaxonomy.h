/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_TAXONOMY_CATAXONOMY_H
#define KONCLUDE_REASONER_TAXONOMY_CATAXONOMY_H

// Libraries includes


// Namespace includes
#include "TaxonomySettings.h"
#include "CHierarchy.h"
#include "CTaxonomy.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CATaxonomy
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class N, class E>
			class CATaxonomy : public CHierarchy {
				// public methods
				public:
					//! Constructor
					CATaxonomy();


					virtual N* getTopHierarchyNode() = 0;
					virtual N* getBottomHierarchyNode() = 0;

					virtual N* getHierarchyNode(const E& element) = 0;
					virtual bool hasHierarchyNode(const E& element) = 0;


					bool isElementSubsumption(const E& subsumerElement, const E& subsumedElement);
					bool isElementEquivalence(const E& element1, const E& element2);
					bool isElementSatisfiable(const E& element);
					bool isElementNotSatisfiable(const E& element);


					bool isTopHierarchyElement(const E& element);
					bool isBottomHierarchyElement(const E& element);



					virtual bool isNodeSubsumption(N* subsumerNode, N* subsumedNode) = 0;
					virtual bool isNodeEquivalence(N* node1, N* node2) = 0;
					virtual bool isNodeSatisfiable(N* node) = 0;
					virtual bool isNodeNotSatisfiable(N* node) = 0;

					virtual bool isTopHierarchyNode(N* element) = 0;
					virtual bool isBottomHierarchyNode(N* element) = 0;



					virtual bool isComplete() = 0;

					virtual bool isActiveNode(N* node) = 0;

					virtual N* getActiveNode(N* node) = 0;


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

#include "CATaxonomy.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CATAXONOMY_H
