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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTINSTANZIATEDINDIVIDUALITEMHASHER_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTINSTANZIATEDINDIVIDUALITEMHASHER_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexConceptInstanziatedIndividualItemLinker.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableCompositionItemVariableExpressionBinding
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexConceptInstanziatedIndividualItemHasher {
				// public methods
				public:
					//! Constructor
					COptimizedComplexConceptInstanziatedIndividualItemHasher(COptimizedComplexConceptInstanziatedIndividualItemLinker* itemLinker);

					bool operator==(const COptimizedComplexConceptInstanziatedIndividualItemHasher& hasher) const;

					COptimizedComplexConceptInstanziatedIndividualItemLinker* getLinker() const;
					CRealizationIndividualInstanceItemReference getRealizationIndividualInstanceItemReference() const;

				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexConceptInstanziatedIndividualItemLinker* mItemLinker;

				// private methods
				private:

				// private variables
				private:

			};


			inline uint qHash(const COptimizedComplexConceptInstanziatedIndividualItemHasher& hasher) {
				return qHash(hasher.getRealizationIndividualInstanceItemReference());
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTINSTANZIATEDINDIVIDUALITEMHASHER_H
