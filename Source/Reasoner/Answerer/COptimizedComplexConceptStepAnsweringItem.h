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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTSTEPANSWERINGITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTSTEPANSWERINGITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"



// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Answerer {

			

			/*! 
			 *
			 *		\class		COptimizedComplexConceptStepAnsweringItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexConceptStepAnsweringItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexConceptStepAnsweringItem();

					//! Destructor
					virtual ~COptimizedComplexConceptStepAnsweringItem();


					COptimizedComplexConceptStepAnsweringItem* addQueuedConceptItem(COptimizedComplexConceptItem* item);
					COptimizedComplexConceptItem* takeNextQueuedConceptItem();
					COptimizedComplexConceptItem* getNextQueuedConceptItem();

					QList<COptimizedComplexConceptItem*>* getConceptItemQueuedList();
					QSet<COptimizedComplexConceptItem*>* getConceptItemQueuedSet();

					QSet<COptimizedComplexConceptItem*>* getConceptItemProcessingSet();
					COptimizedComplexConceptStepAnsweringItem* addProcessingConceptItem(COptimizedComplexConceptItem* item);
					COptimizedComplexConceptStepAnsweringItem* removeProcessingConceptItem(COptimizedComplexConceptItem* item);

					bool hasConceptItemsQueued();
					bool hasConceptItemsProcessing();


				// protected methods
				protected:

				// protected variables
				protected:
					QList<COptimizedComplexConceptItem*> mConceptItemQueuedList;
					QSet<COptimizedComplexConceptItem*> mConceptItemQueuedSet;
					QSet<COptimizedComplexConceptItem*> mConceptItemProcessingSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTSTEPANSWERINGITEM_H
