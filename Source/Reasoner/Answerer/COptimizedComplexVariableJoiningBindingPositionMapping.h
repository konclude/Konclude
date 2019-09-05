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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGBINDINGPOSITIONMAPPING_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGBINDINGPOSITIONMAPPING_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"


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
			class COptimizedComplexVariableJoiningBindingPositionMapping {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableJoiningBindingPositionMapping(COptimizedComplexVariableCompositionItem* joiningVarItem, cint64 leftSize, cint64 rightSize);
					~COptimizedComplexVariableJoiningBindingPositionMapping();

					COptimizedComplexVariableJoiningBindingPositionMapping(const COptimizedComplexVariableJoiningBindingPositionMapping& mapping);


					COptimizedComplexVariableCompositionItem* getJoiningVariableItem();

					COptimizedComplexVariableJoiningBindingPositionMapping* addBindingPositionMapping(bool left, cint64 varIdx, cint64 mappedVarIdx);
					cint64 getMappedBindingPosition(bool left, cint64 varIdx);

					bool isSelfSameVariableReduction();
					COptimizedComplexVariableJoiningBindingPositionMapping* setSelfSameVariableReduction(bool selfSameVariableReduction);

					bool isVariableReduction();
					COptimizedComplexVariableJoiningBindingPositionMapping* setVariableReduction(bool variableReduction);


					cint64 getHashValue() const;
					bool operator==(const COptimizedComplexVariableJoiningBindingPositionMapping& mapping) const;


					CXLinker<cint64>* getLeftKeyBindingLinker();
					CXLinker<cint64>* getRightKeyBindingLinker();

					COptimizedComplexVariableJoiningBindingPositionMapping* setLeftKeyBindingLinker(CXLinker<cint64>* linker);
					COptimizedComplexVariableJoiningBindingPositionMapping* setRightKeyBindingLinker(CXLinker<cint64>* linker);


					cint64* getRightPosMapArray();
					cint64* getLeftPosMapArray();


				// protected methods
				protected:

				// protected variables
				protected:
					bool mSelfSameVariableReduction;
					bool mVariableReduction;
					mutable bool mHashValueCaluclated;
					mutable cint64 mHashValue;
					COptimizedComplexVariableCompositionItem* mJoiningVarItem;
					CXLinker<cint64>* mLeftKeyBindingLinker;
					CXLinker<cint64>* mRightKeyBindingLinker;
					cint64* mRightPosMapArray;
					cint64* mLeftPosMapArray;
					cint64 mLeftMapSize;
					cint64 mRightMapSize;


				// private methods
				private:

				// private variables
				private:

			};


			inline uint qHash(const COptimizedComplexVariableJoiningBindingPositionMapping& hasher) {
				qint64 key = (qint64)hasher.getHashValue();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				} else {
					return uint(key);
				}
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGBINDINGPOSITIONMAPPING_H
