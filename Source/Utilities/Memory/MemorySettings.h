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

#ifndef KONCLUDE_UTILITIES_MEMORY_MEMORYSETTINGS
#define KONCLUDE_UTILITIES_MEMORY_MEMORYSETTINGS

// Library includes


// Namespace includes


// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\file		MemorySettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */


			// forward declarations

			// Konclude code configs
			const static cint64 DEFAULTMEMORYPOOLSIZE = 50000;



#ifdef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
	#define KONCLUDE_FORCE_MEMORY_STATISTIC_DEACTIVATED
#endif



		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude


#endif // end KONCLUDE_UTILITIES_MEMORY_MEMORYSETTINGS