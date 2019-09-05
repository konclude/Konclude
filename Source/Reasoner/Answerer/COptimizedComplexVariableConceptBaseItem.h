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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECONCEPTBASEITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECONCEPTBASEITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableCompositionItem.h"
#include "COptimizedComplexConceptItem.h"
#include "COptimizedComplexConceptInstanziatedIndividualItemLinker.h"



// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {


	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableConceptBaseItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableConceptBaseItem : public COptimizedComplexVariableCompositionItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableConceptBaseItem(COptimizedComplexConceptItem* conceptItem);

					virtual ~COptimizedComplexVariableConceptBaseItem();

					virtual COMPOSITION_TYPE getCompositionType();

					COptimizedComplexConceptItem* getConceptItem();


					COptimizedComplexConceptInstanziatedIndividualItemLinker* getInstanceItemProcessingLinker();
					COptimizedComplexConceptInstanziatedIndividualItemLinker* getInstanceItemProcessedLinker();
					COptimizedComplexConceptInstanziatedIndividualItemLinker* getInstanceItemRetrievedLinker();

					COptimizedComplexVariableConceptBaseItem* setInstanceItemProcessingLinker(COptimizedComplexConceptInstanziatedIndividualItemLinker* linker);
					COptimizedComplexVariableConceptBaseItem* setInstanceItemProcessedLinker(COptimizedComplexConceptInstanziatedIndividualItemLinker* linker);
					COptimizedComplexVariableConceptBaseItem* setInstanceItemRetrievedLinker(COptimizedComplexConceptInstanziatedIndividualItemLinker* linker);

				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexConceptItem* mConceptItem;


					COptimizedComplexConceptInstanziatedIndividualItemLinker* mInstanceItemProcessingLinker;
					COptimizedComplexConceptInstanziatedIndividualItemLinker* mInstanceItemProcessedLinker;
					COptimizedComplexConceptInstanziatedIndividualItemLinker* mInstanceItemRetrievedLinker;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECONCEPTBASEITEM_H
