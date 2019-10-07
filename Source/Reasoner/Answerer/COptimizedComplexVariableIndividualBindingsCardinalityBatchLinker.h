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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALBINDINGSCARDINALITYBATCHLINKER_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALBINDINGSCARDINALITYBATCHLINKER_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableIndividualBindingsCardinalityLinker.h"



// Other includes
#include "Utilities/UtilitiesSettings.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Answerer {




			/*! 
			 *
			 *		\class		COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker : public CLinkerBase<cint64, COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker> {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker();
					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* startBindingsCardLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker* endBindingsCardLinker, cint64 linkerCount);

					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* initBindingsCardinalityBatchLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* startBindingsCardLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker* endBindingsCardLinker, cint64 linkerCount);

					cint64 getLinkerCount();

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* getStartBindingsCardinalityLinker() const ;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* getEndBindingsCardinalityLinker() const ;


					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* addStartBindingsCardLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* startBindingsCardLinker, cint64 addedCount);


				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mStartBindingsCardLinker;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mEndBindingsCardLinker;




				// private methods
				private:

				// private variables
				private:

			};



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALBINDINGSCARDINALITYBATCHLINKER_H
