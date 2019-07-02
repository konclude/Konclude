/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPESTRINGVALUEEXCLUSIONTYPE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPESTRINGVALUEEXCLUSIONTYPE_H

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
				 *		\class		CDatatypeStringValueExclusionType
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeStringValueExclusionType {
					// public methods
					public:
						enum STRING_VALUE_EXCLUSION_TYPE { SVET_NOTHING = 0, SVET_LANGUAGE_TAGS_COMPLETELY = 1, SVET_ALL = 2 };

						//! Constructor
						CDatatypeStringValueExclusionType(STRING_VALUE_EXCLUSION_TYPE exclusionType = SVET_NOTHING, bool negated = false);


						CDatatypeStringValueExclusionType* setExclusionType(STRING_VALUE_EXCLUSION_TYPE exclusionType, bool negated);
						STRING_VALUE_EXCLUSION_TYPE getExclusionType();
						bool getExclusionNegation();

					// protected methods
					protected:

					// protected variables
					protected:
						STRING_VALUE_EXCLUSION_TYPE mExclusionType;
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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDatatypeStringValueExclusionType_H
