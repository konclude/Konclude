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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CLASSIFICATIONSETTINGS
#define KONCLUDE_REASONER_CLASSIFIER_CLASSIFICATIONSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes
#include "Utilities/Container/CQtManagedRestrictedModificationList.h"
#include "Utilities/Container/CQtManagedRestrictedModificationMap.h"
#include "Utilities/Container/CQtManagedRestrictedModificationHash.h"

// Logger includes


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Classifier {

			/*! 
			 *
			 *		\file		ClassificationSettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */



			// forward declarations

			class COptimizedKPSetClassTestingItem;


			// Custom Events >= 2000
			const QEvent::Type EVENTCLASSIFYONTOLOGY					= (QEvent::Type)2000;
			const QEvent::Type EVENTTESTCALCULATEDCALLBACK				= (QEvent::Type)2001;
			const QEvent::Type EVENTINTERCEPTONTOLOGYTESTRESULT			= (QEvent::Type)2002;
			const QEvent::Type EVENTONTOLOGYTELLSUBSUMPTION				= (QEvent::Type)2003;
			const QEvent::Type EVENTCALLBACKCLASSIFIEDONTOLOGY			= (QEvent::Type)2004;
			const QEvent::Type EVENTONTOLOGYTELLDISJOINTS				= (QEvent::Type)2005;
			const QEvent::Type EVENTONTOLOGYTELLSATISFIABLE				= (QEvent::Type)2006;
			const QEvent::Type EVENTONTOLOGYTELLCONNECIDENOCCUR			= (QEvent::Type)2007;
			const QEvent::Type EVENTONTOLOGYTELLCONPOSSSUBSUM			= (QEvent::Type)2008;
			const QEvent::Type EVENTSATURATIONCALCULATEDCALLBACK		= (QEvent::Type)2009;
			const QEvent::Type EVENTONTOLOGYTELLCLASSIFICATIONMESSAGE	= (QEvent::Type)2010;



#define CCLASSSUBSUMPTIONMESSAGELIST CQtManagedRestrictedModificationList
#define CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST CQtManagedRestrictedModificationList
#define CCLASSPSEUDOMODELHASH CQtManagedRestrictedModificationHash
#define CCLASSPSEUDOMODELMAP CQtManagedRestrictedModificationMap


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_CLASSIFIER_CLASSIFICATIONSETTINGS