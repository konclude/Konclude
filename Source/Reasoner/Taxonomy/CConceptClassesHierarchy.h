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

#ifndef KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESHIERARCHY_H
#define KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESHIERARCHY_H

// Libraries includes


// Namespace includes
#include "TaxonomySettings.h"
#include "CConceptClassesParentChildPredecessorHierarchy.h"
#include "CConceptClassesHierarchyNode.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CLightConceptClassesTaxonomy
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptClassesHierarchy : public CConceptClassesParentChildPredecessorHierarchy<CConceptClassesHierarchy,CConceptClassesHierarchyNode> {
				// public methods
				public:
					//! Constructor
					CConceptClassesHierarchy(CConceptClassesHierarchyNode* topHierarchyNode, CConceptClassesHierarchyNode* bottomHierarchyNode);
					CConceptClassesHierarchy(CConcept* topConcept, CConcept* bottomConcept);




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


#endif // KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESHIERARCHY_H
