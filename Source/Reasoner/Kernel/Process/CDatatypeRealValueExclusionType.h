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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEREALVALUEEXCLUSIONTYPE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEREALVALUEEXCLUSIONTYPE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDatatypeRealValueExclusionType
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeRealValueExclusionType {
					// public methods
					public:
						enum REAL_VALUE_EXCLUSION_TYPE { RVET_NOTHING = 0, RVET_INTEGER = 1, RVET_DECIMAL = 2, RVET_RATIONAL = 3, RVET_REAL = 4, RVET_ALL = 5 };

						//! Constructor
						CDatatypeRealValueExclusionType(REAL_VALUE_EXCLUSION_TYPE exclusionType = RVET_NOTHING, bool negated = false);


						CDatatypeRealValueExclusionType* setExclusionType(REAL_VALUE_EXCLUSION_TYPE exclusionType, bool negated);
						REAL_VALUE_EXCLUSION_TYPE getExclusionType();
						bool getExclusionNegation();

					// protected methods
					protected:

					// protected variables
					protected:
						REAL_VALUE_EXCLUSION_TYPE mExclusionType;
						bool mExclusionNegation;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEREALVALUEEXCLUSIONTYPE_H
