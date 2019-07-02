/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
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


#define nullptr 0

		typedef qint64 cint64;
		typedef int cint;


#define CINT64_MAX std::numeric_limits<cint64>::max();
#define CINT64_MIN std::numeric_limits<cint64>::min();

#define DOUBLE_MAX std::numeric_limits<double>::max();
#define DOUBLE_MIN std::numeric_limits<double>::min();

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
