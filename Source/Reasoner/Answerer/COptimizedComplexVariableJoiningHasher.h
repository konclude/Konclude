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

#ifndef KONCLUDE_REASONER_ANSWERER_COptimizedComplexVariableJoiningHasher_H
#define KONCLUDE_REASONER_ANSWERER_COptimizedComplexVariableJoiningHasher_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableCompositionItem.h"


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
			class COptimizedComplexVariableJoiningHasher {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableJoiningHasher(TIndividualInstanceItemDataBinding* binding, CXLinker<cint64>* keyBindingLinker);


					cint64 getHashValue() const;

					bool operator==(const COptimizedComplexVariableJoiningHasher& hasher) const;

					CXLinker<cint64>* getKeyBindingLinker() const;
					TIndividualInstanceItemDataBinding* getBinding() const;


				// protected methods
				protected:
					COptimizedComplexVariableJoiningHasher* calculateHashValue();

				// protected variables
				protected:
					CXLinker<cint64>* mKeyBindingLinker;
					TIndividualInstanceItemDataBinding* mBinding;
					cint64 mHashValue;


				// private methods
				private:

				// private variables
				private:

			};


			inline uint qHash(const COptimizedComplexVariableJoiningHasher& hasher) {
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

#endif // KONCLUDE_REASONER_ANSWERER_COptimizedComplexVariableJoiningHasher_H
