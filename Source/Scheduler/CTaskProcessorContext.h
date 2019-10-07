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

#ifndef KONCLUDE_SCHEDULER_CTASKPROCESSORCONTEXT_H
#define KONCLUDE_SCHEDULER_CTASKPROCESSORCONTEXT_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskHandleContext.h"
#include "CTaskProcessorCommunicator.h"


// Other includes
#include "Context/CContext.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTaskProcessorContext
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskProcessorContext : public CTaskHandleContext {
			// public methods
			public:
				//! Constructor
				CTaskProcessorContext();

				//! Destructor
				virtual ~CTaskProcessorContext();

				virtual CTaskProcessorCommunicator* getTaskProcessorCommunicator() = 0;

			// protected methods
			protected:

			// protected variables
			protected:

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKPROCESSORCONTEXT_H
