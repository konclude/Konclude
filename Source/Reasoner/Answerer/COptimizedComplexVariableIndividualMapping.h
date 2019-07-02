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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPING_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPING_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableIndividualBindings.h"
#include "COptimizedComplexVariableIndividualBindingsHasher.h"
#include "COptimizedComplexVariableIndividualBindingsCardinalityLinker.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {



			/*! 
			 *
			 *		\class		COptimizedComplexVariableIndividualMapping
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableIndividualMapping : public QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*> {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableIndividualMapping(cint64 bindingSize);
					~COptimizedComplexVariableIndividualMapping();


					enum VARIABLE_TYPE {
						INDIVIDUAL_VARIABLE, DATA_LITERAL_VARIABLE
					};



					VARIABLE_TYPE getBindingMapping(cint64 idx);
					COptimizedComplexVariableIndividualMapping* setBindingMapping(cint64 idx, VARIABLE_TYPE varType);

					cint64 getBindingSize();

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* getLastAddedBindingsCardinalityLinker();
					COptimizedComplexVariableIndividualMapping* setLastAddedBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker);



					bool addInsertingBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker, bool addCardinalitiesIfAlreadyPresent);

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* takeBindingsCardinalityLinker();


				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mBindingSize;
					VARIABLE_TYPE* mBindingMapping;

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mLastAddedBindingsCardinalityLinker;




				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPING_H
