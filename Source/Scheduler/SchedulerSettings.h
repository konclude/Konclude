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

#ifndef KONCLUDE_SCHEDULER_SCHEDULERSETTINGS
#define KONCLUDE_SCHEDULER_SCHEDULERSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes


namespace Konclude {

	using namespace Utilities;

	namespace Scheduler {


		/*! 
		 *
		 *		\file		SchedulerSettings
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */



		// forward declarations

		class CTaskReserveArrayQueue;
		class CTaskReserveArrayQueueConsumer;
		class CTaskReserveMultiArrayQueue;
		class CTaskReserveMultiArrayQueueConsumer;
		class CTaskSchedulerCommunicator;
		class CTaskEventHandlerBasedProcessor;
		class CTaskEventHandlerBasedScheduler;
		class CTaskEventHandlerBasedDistributionCompletor;

		template<class T> class CProcessingQueueExtensionIterator;
		template<class T> class CProcessingQueueIterator;
		template<class T> class CProcessingQueue;
		template<class T> class CProcessingQueueExtension;



		// Custom Events >= 2000




		static const cint64 CREQUESTPROCESSTASKEVENTTYPEID			= 5001;
		static const cint64 CREQUESTSCHEDULETASKEVENTTYPEID			= 5002;
		static const cint64 CRESPONSESCHEDULETASKEVENTTYPEID		= 5003;
		static const cint64 CSENDTASKCOMPLETEEVENTTYPEID			= 5004;
		static const cint64 CSENDTASKPROCESSEVENTTYPEID				= 5005;
		static const cint64 CSENDTASKSCHEDULEEVENTTYPEID			= 5006;
		static const cint64 CTASKADDITIONALALLOCATIONEVENTTYPEID	= 5007;
		static const cint64 CDISTRIBUTEMEMORYPOOLEVENTTYPEID		= 5008;
		static const cint64 CTASKDISPENSENOTIFICATIONEVENTTYPEID	= 5009;
		static const cint64 CUPDATETASKSTATUSEVENTTYPEID			= 5010;
		static const cint64 CTASKSTATUSUPDATEDEVENTTYPEID			= 5011;
		static const cint64 CALIGNEDUPDATETASKSTATUSEVENTTYPEID		= 5012;
		static const cint64 CCRITICALSCHEDULETASKEVENTTYPEID		= 5013;

#ifndef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
	#define KONCLUDE_SCHEDULER_TASK_THREADS_TIME_STATISTICS
	#define KONCLUDE_SCHEDULER_TASK_PROCESSING_STATISTICS
#endif


	}; // end namespace Scheduler

}; // end namespace Konclude


#endif // end KONCLUDE_SCHEDULER_SCHEDULERSETTINGS
