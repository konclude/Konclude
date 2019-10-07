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

#ifndef KONCLUDE_UTILITIES_UTILITIESSETTINGS
#define KONCLUDE_UTILITIES_UTILITIESSETTINGS

// Libraries includes
#include <QtGlobal>

// Namespace includes


// Other includes
#include <limits>

// Logger includes

using namespace std;

namespace Konclude {

	namespace Utilities {


		typedef qint64 cint64;
		typedef quint64 cuint64;
		typedef int cint;

#define CINT_MAX std::numeric_limits<cint>::max()
#define CINT_MIN std::numeric_limits<cint>::min()

#define CINT64_MAX std::numeric_limits<cint64>::max()
#define CINT64_MIN std::numeric_limits<cint64>::min()

#define DOUBLE_MAX std::numeric_limits<double>::max()
#define DOUBLE_MIN std::numeric_limits<double>::min()

		// forward declarations
		class CAllocationObject;
		class CMemoryReleaser;



		/*! 
		 *
		 *		\file		UtilitiesSettings
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */





	}; // end namespace Utilities

}; // end namespace Konclude


#endif // end KONCLUDE_UTILITIES_UTILITIESSETTINGS
