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

#ifndef KONCLUDE_REASONER_REALIZER_REALIZERSETTINGS
#define KONCLUDE_REASONER_REALIZER_REALIZERSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes


// Logger includes


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {

			/*! 
			 *
			 *		\file		RealizerSettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */



			// forward declarations
			class CRealizingTestingItem;
			class COntologyRealizingItem;
			class COptimizedKPSetConceptInstancesItem;
			class COptimizedKPSetConceptInstantiatedItem;
			class COptimizedKPSetConceptInstancesData;


			// Custom Events >= 2000
			const QEvent::Type EVENTPREALIZEONTOLOGY					= (QEvent::Type)2000;
			const QEvent::Type EVENTCALLBACKREALIZEDONTOLOGY			= (QEvent::Type)2001;
			const QEvent::Type EVENTREALIZINGCALCULATEDCALLBACK			= (QEvent::Type)2002;



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_REALIZER_REALIZERSETTINGS