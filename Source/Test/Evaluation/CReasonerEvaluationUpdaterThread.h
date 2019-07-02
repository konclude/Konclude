/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONUPDATERTHREAD_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONUPDATERTHREAD_H

// Libraries includes
#include <QFileSystemWatcher>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationUpdater.h"


// Other includes
#include "Config/CConfiguration.h"

#include "Concurrent/CThread.h"

#include "Test/Evaluation/Events/CReasonerEvaluationUpdateEvent.h"
#include "Test/Evaluation/Events/CReasonerEvaluationProgramFileChangedEvent.h"

#include "Control/Command/CConfigManagerReader.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;
	using namespace Concurrent;
	using namespace Control::Command;

	namespace Test {

		namespace Evaluation {

			using namespace Evaluation::Events;


			/*! 
			 *
			 *		\class		CReasonerEvaluationUpdaterThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationUpdaterThread : public CThread, public CReasonerEvaluationUpdater {
				Q_OBJECT

				// public methods
				public:
					//! Constructor
					CReasonerEvaluationUpdaterThread();

					//! Destructor
					virtual ~CReasonerEvaluationUpdaterThread();


					void addProgramFile(const QString& changedFile);

				// protected methods
				protected:
					virtual void threadStarted();
					virtual void threadStopped();

					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent* event);


				// protected slots
				protected slots:
					void watchedProgramFileChanged(const QString& changedFile);


				// protected variables
				protected:
					QFileSystemWatcher* mSystemWatcher;
					QStringList mNextChangedProgramFileList;
					QSet<QString> mNextChangedProgramFileSet;

					bool mProgramRunning;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONUPDATERTHREAD_H
