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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINMAPPEDKEYSBINDINGSCARDINALITYLINKER_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINMAPPEDKEYSBINDINGSCARDINALITYLINKER_H

// Libraries includes


// Namespace includes
#include "CompositionSettings.h"


// Other includes
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualBindingsCardinalityLinker.h"
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualUpdateCardinalityLinker.h"
#include "Reasoner/Answerer/COptimizedComplexVariableJoiningHasher.h"


#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {


				/*! 
				 *
				 *		\class		CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker : public CLinkerBase<COptimizedComplexVariableIndividualBindings*, CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker> {
					// public methods
					public:
						//! Constructor
						CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker();

						CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker* initMappedKeysBindingsCardinalityData(const COptimizedComplexVariableJoiningHasher& bindingHasher, cint64 mappedKeyIndex, COptimizedComplexVariableIndividualBindings* varMapping, COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites, bool leftSide, bool inserting);


						cint64 getMappedKeyIndex();

						bool isInserting();
						bool isLeftSide();

						CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker* setInserting(bool inserting);
						CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker* setLeftSide(bool leftSide);


						COptimizedComplexVariableIndividualBindings* getVariableMapping();
						COptimizedComplexVariableIndividualBindingsCardinality* getNewCardinalites();
						COptimizedComplexVariableIndividualBindingsCardinality* getPreviousCardinalites();

						COptimizedComplexVariableJoiningHasher getBindingHasher();

					// protected methods
					protected:

					// protected variables
					protected:
						COptimizedComplexVariableJoiningHasher mBindingHasher;
						cint64 mKeyIndex;

						COptimizedComplexVariableIndividualBindingsCardinality* mNewCardinalites;
						COptimizedComplexVariableIndividualBindingsCardinality* mPrevCardinalites;

						bool mInserting;
						bool mLeftSide;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINMAPPEDKEYSBINDINGSCARDINALITYLINKER_H
