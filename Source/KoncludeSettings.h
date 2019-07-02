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

#ifndef KONCLUDE_KONCLUDESETTINGS
#define KONCLUDE_KONCLUDESETTINGS

// Libraries includes
#include <QtGlobal>

// Namespace includes


// Other includes


// Logger includes


namespace Konclude {


	/*! 
	 *
	 *		\file		KoncludeSettings
	 *		\author		Andreas Steigmiller
	 *		\version	0.1
	 *		\brief		TODO
	 *
	 */


	#define KONCLUDE_VERSION_MAJOR				0
	#define KONCLUDE_VERSION_MINOR				5
	#define KONCLUDE_VERSION_BUILD				0
	#define KONCLUDE_VERSION_REVISION			275

	#define KONCLUDE_VERSION_STRING				"0.5.0-275"

	#define KONCLUDE_NAME						"Konclude"
	#define KONCLUDE_NAME_EXTENSION				"Uni Ulm Parallel Reasoner"
	#define KONCLUDE_DESCRIPTION				"Reasoner for the SROIQV Description Logic"

	#define KONCLUDE_DEFAULTREASONERMANAGER		"ReasonerManager"

	#define KONCLUDE_DEFAULTCALCULATIONMANAGER	"Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager"


	#define KONCLUDE_OWLLINK_VERSION_MAJOR				1
	#define KONCLUDE_OWLLINK_VERSION_MINOR				0




	// Konclude code configs

//#define KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
//#define KONCLUDE_FORCE_STATISTIC_DEACTIVATED


#define KONCLUDE_ASSERT(expression) Q_ASSERT(expression)
#define KONCLUDE_ASSERT_X(expression,source,reason) Q_ASSERT_X(expression,source,reason)


}; // end namespace Konclude


#endif // end KONCLUDE_KONCLUDESETTINGS
