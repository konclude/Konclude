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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPINGSMULTIHASH_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPINGSMULTIHASH_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableIndividualMappings.h"
#include "COptimizedComplexVariableIndividualMappingsMultiHashPart.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {



			/*! 
			 *
			 *		\class		COptimizedComplexVariableIndividualMappingsMultiHash
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableIndividualMappingsMultiHash : public COptimizedComplexVariableIndividualMappings {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableIndividualMappingsMultiHash(cint64 bindingSize, cint64 multiHashPartsCount);
					virtual ~COptimizedComplexVariableIndividualMappingsMultiHash();

					virtual COptimizedComplexVariableIndividualMappings* incCurrentUpdateId(cint64 incUpdateCount = 1);

					COptimizedComplexVariableIndividualMappingsMultiHashPart* getMultiHashPart(cint64 index);

					bool addBindingsCardinalities(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* addedCardinalites);

					bool addInsertingBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker, bool addCardinalitiesIfAlreadyPresent);

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* takeBindingsCardinalityLinker();


					COptimizedComplexVariableIndividualMappingsMultiHash* updateLinkers(COptimizedComplexVariableIndividualMappingsMultiHashPart* multiPart);
					COptimizedComplexVariableIndividualMappingsMultiHash* updateBindingCount(COptimizedComplexVariableIndividualMappingsMultiHashPart* multiPart);

					COptimizedComplexVariableIndividualMappingsMultiHash* resetBindingCount();

				// protected methods
				protected:

				// protected variables
				protected:

					cint64 mMultiHashPartsCount;
					COptimizedComplexVariableIndividualMappingsMultiHashPart** mMultiHashPartArray;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPINGSMULTIHASH_H
