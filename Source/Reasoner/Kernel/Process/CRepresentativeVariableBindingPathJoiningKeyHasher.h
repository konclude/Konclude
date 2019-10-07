/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREPRESENTATIVEVARIABLEBINDINGPATHJOININGKEYHASHER_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREPRESENTATIVEVARIABLEBINDINGPATHJOININGKEYHASHER_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CRepresentativeVariableBindingPathJoiningKeyData.h"


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CRepresentativeVariableBindingPathJoiningKeyHasher
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CRepresentativeVariableBindingPathJoiningKeyHasher {
					// public methods
					public:
						//! Constructor
						CRepresentativeVariableBindingPathJoiningKeyHasher(CRepresentativeVariableBindingPathJoiningKeyData* data);
						CRepresentativeVariableBindingPathJoiningKeyHasher(CVariableBindingPath* varBindPath, CSortedLinker<CVariable*>* keyVars);

						cint64 getHashValue() const;

						bool operator==(const CRepresentativeVariableBindingPathJoiningKeyHasher& hasher) const;

					// protected methods
					protected:
						cint64 calculateHashValue(CVariableBindingPath* varBindPath, CSortedLinker<CVariable*>* keyVars);


					// protected variables
					protected:
						CRepresentativeVariableBindingPathJoiningKeyData* mJoiningData;
						CVariableBindingPath* mVarBindPath;
						CSortedLinker<CVariable*>* mKeyVars;
						cint64 mHashValue;

					// private methods
					private:

					// private variables
					private:

				};

				inline uint qHash(const CRepresentativeVariableBindingPathJoiningKeyHasher& hasher) {
					qint64 key = (qint64)hasher.getHashValue();
					if (sizeof(quint64) > sizeof(uint)) {
						return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
					} else {
						return uint(key);
					}
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREPRESENTATIVEVARIABLEBINDINGPATHJOININGKEYHASHER_H
