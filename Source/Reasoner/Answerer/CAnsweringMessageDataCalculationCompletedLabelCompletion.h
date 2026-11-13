/*
 *		Copyright (C) 2013-2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCOMPLETEDLABELCOMPLETION_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCOMPLETEDLABELCOMPLETION_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringMessageDataCalculationCompleted.h"
#include "COptimizedComplexConceptItem.h"


// Other includes
#include "Reasoner/Taxonomy/CHierarchyNode.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Answerer {

			/*!
			 *
			 *		\class		CAnsweringMessageDataCalculationCompletedLabelCompletion
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		The end of a satisfiability test that was run to complete the label of a
			 *					class for the saturation decider, while the sub class search of the concept
			 *					item waits at the hierarchy node.
			 *
			 */
			class CAnsweringMessageDataCalculationCompletedLabelCompletion : public CAnsweringMessageDataCalculationCompleted {
				// public methods
				public:
					//! Constructor
					CAnsweringMessageDataCalculationCompletedLabelCompletion(CSatisfiableCalculationJob* calcJob, COptimizedComplexConceptItem* conceptItem, CHierarchyNode* testingNode, CConcept* completedConcept);

					COptimizedComplexConceptItem* getConceptItem();
					CHierarchyNode* getTestingNode();
					CConcept* getCompletedConcept();

				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexConceptItem* mConceptItem;
					CHierarchyNode* mTestingNode;
					CConcept* mCompletedConcept;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCOMPLETEDLABELCOMPLETION_H
