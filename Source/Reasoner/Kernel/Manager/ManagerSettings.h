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

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_MANAGERSETTINGS
#define KONCLUDE_REASONER_KERNEL_MANAGER_MANAGERSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes


// Logger includes


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {

				/*! 
				 *
				 *		\file		KernelSettings
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */


				// forward declaration
				class CPrecomputationManager;
				class CPreprocessingManager;
				class CReasonerManager;
				class CRequirementPreparingData;
				class CReasoningTaskData;


				// Custom Events >= 2000
				const QEvent::Type EVENTJOBCALCULATEDSATISFIABLECALLBACK		= (QEvent::Type)2000;
				const QEvent::Type EVENTREASONINGSATISFIABLECALCULATIONJOB		= (QEvent::Type)2001;
				const QEvent::Type EVENTCALCQUERY								= (QEvent::Type)2002;
				const QEvent::Type EVENTCALCEDQUERY								= (QEvent::Type)2003;
				const QEvent::Type EVENTQUERIEDTAXCOMPUTEDCALLBACK				= (QEvent::Type)2004;
				const QEvent::Type EVENTQUERYJOBCALCULATEDSATISFIABLECALLBACK	= (QEvent::Type)2005;
				const QEvent::Type EVENTREQUIREMENTPROCESSEDCALLBACK			= (QEvent::Type)2006;
				const QEvent::Type EVENTPREPAREONTOLOGY							= (QEvent::Type)2007;

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_KERNEL_MANAGER_MANAGERSETTINGS