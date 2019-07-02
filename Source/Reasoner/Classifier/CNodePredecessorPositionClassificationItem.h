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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CNODEPREDECESSORPOSITIONCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CNODEPREDECESSORPOSITIONCLASSIFICATIONITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassifierStatistics.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CNodePredecessorPositionClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CNodePredecessorPositionClassificationItem {
				// public methods
				public:
					//! Constructor
					CNodePredecessorPositionClassificationItem(CHierarchyNode *node);

					//! Destructor
					virtual ~CNodePredecessorPositionClassificationItem();

					virtual CHierarchyNode *getTestingNode();

					virtual CNodePredecessorPositionClassificationItem *appendPredecessorNode(CHierarchyNode *node);
					virtual QList<CHierarchyNode *> *getPredecessorNodeList();

					virtual QList<CHierarchyNode *> *getInitializationPredecessorNodeList();

					virtual bool isTestingConcluded();


				// protected methods
				protected:

				// protected variables
				protected:
					CHierarchyNode *testNode;
					QList<CHierarchyNode *> predNodelist;
					QList<CHierarchyNode *> initPredNodelist;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CNODEPREDECESSORPOSITIONCLASSIFICATIONITEM_H
