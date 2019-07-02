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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CPARTIALPRUNINGHIERARCHYNODE_H
#define KONCLUDE_REASONER_CLASSIFIER_CPARTIALPRUNINGHIERARCHYNODE_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassifierStatistics.h"

// Other includes
#include "Reasoner/Taxonomy/CHierarchyNode.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Taxonomy;

		namespace Classifier {



			/*! 
			 *
			 *		\class		CPartialPruningHierarchyNode
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPartialPruningHierarchyNode : public CHierarchyNode {
				// public methods
				public:
					//! Constructor
					CPartialPruningHierarchyNode(CConcept *representiveConcept);

					//! Destructor
					virtual ~CPartialPruningHierarchyNode();

					virtual CHierarchyNode *repairParentNodeInsertion(CHierarchyNode *replacedParentNodeCandidate, CClassifierStatistics *statistics = 0);
					virtual CHierarchyNode *repairChildsParentNodeReplacement(CHierarchyNode *replacedParentNodeCandidate, CClassifierStatistics *statistics = 0);

					virtual CHierarchyNode *mergeToParentNode(CHierarchyNode *mergeParentDirection, CClassifierStatistics *statistics = 0);

					virtual CHierarchyNode *propagateTransitiveRelations(CClassifierStatistics *statistics = 0);
					virtual CHierarchyNode *propagateTransitivePredecessorRelation(CHierarchyNode *transAddPredNode, CClassifierStatistics *statistics = 0);

					virtual CHierarchyNode *pruneTransitiveRelations(CHierarchyNode *incompNode, CClassifierStatistics *statistics = 0);

					CHierarchyNode* repairChildNodes(CClassifierStatistics *statistics);
					CHierarchyNode* repairParentNodes(CClassifierStatistics *statistics);

				// protected methods
				protected:

				// protected variables
				protected:


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CPARTIALPRUNINGHIERARCHYNODE_H
