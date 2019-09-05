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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGDATA_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGDATA_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker.h"


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
			class COptimizedComplexVariableJoiningData {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableJoiningData();

					COptimizedComplexVariableJoiningData* addLeftBindingLinker(COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker);
					COptimizedComplexVariableJoiningData* addRightBindingLinker(COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker);

					COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* getLeftBindingLinker() const;
					COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* getRightBindingLinker() const;


					COptimizedComplexVariableJoiningData* addBindingLinker(bool left, COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker);
					COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* getBindingLinker(bool left) const;
					bool hasBindings(bool left) const;

#ifdef CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
					mutable cint64 mLeftAdded = 0;
					mutable cint64 mRightAdded = 0;

					mutable cint64 mJoinedCount = 0;
					mutable cint64 mInsertionCount = 0;
					mutable cint64 mCheckingCount = 0;
#endif //CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT


				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* mLeftBindingLinker;
					COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* mRightBindingLinker;


				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGDATA_H
