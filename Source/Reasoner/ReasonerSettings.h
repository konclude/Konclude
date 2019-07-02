/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_REASONERSETTINGS
#define KONCLUDE_REASONER_REASONERSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes
#include "KoncludeSettings.h"

// Logger includes


namespace Konclude {

	namespace Reasoner {


		/*! 
		 *
		 *		\file		ReasonerSettings
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */



		// forward declarations


		// Custom Events >= 2000
		const QEvent::Type EVENTCALCBOXSATISFIABLE				= (QEvent::Type)2000;
		const QEvent::Type EVENTCALCEDBOXSATISFIABLECALLBACK	= (QEvent::Type)2001;
		const QEvent::Type EVENTCALCQUERY						= (QEvent::Type)2002;
		const QEvent::Type EVENTCALCEDQUERY						= (QEvent::Type)2003;


	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_REASONERSETTINGS