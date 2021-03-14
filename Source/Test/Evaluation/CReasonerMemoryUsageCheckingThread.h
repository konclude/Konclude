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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONERMEMORYUSAGECHECKINGTHREAD_H
#define KONCLUDE_TEST_EVALUATION_CREASONERMEMORYUSAGECHECKINGTHREAD_H

// Libraries includes
#include <QString>
#include <QProcess>


// Namespace includes
#include "EvaluationSettings.h"


// Other includes
#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"

#include "Concurrent/CIntervalThread.h"

#include "Test/Evaluation/Events/CReasonerMemoryUsageFinishedEvent.h"



// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Config;
	using namespace Logger;
	using namespace Concurrent;

	namespace Test {

		namespace Evaluation {

			using namespace Evaluation::Events;


			/*! 
			 *
			 *		\class		CReasonerMemoryUsageCheckingThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerMemoryUsageCheckingThread : public CIntervalThread, public CLogIdentifier {

				// public methods
				public:
					//! Constructor
					CReasonerMemoryUsageCheckingThread(CConfiguration *config, QString processId);

					//! Destructor
					virtual ~CReasonerMemoryUsageCheckingThread();


					virtual cint64 getMemoryUsage(bool stopChecking = true);


				// protected methods
				protected:
					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent* event);

					virtual void threadStarted();
					virtual void threadStopped();

					virtual bool processTimer(qint64 timerID);



					void checkMemory();

				// protected variables
				protected:
					CConfiguration* mConfig;

					QString mProcessId;
					QString mMemoryCheckingProgramm;

					cint64 mMaxMemoryUsage;
					bool mChecking;
					bool mContinue;

					cint64 mMemoryCheckingIntervall;

					const cint64 MEMORY_CHECKING_TIMER = 1;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONERMEMORYUSAGECHECKINGTHREAD_H
