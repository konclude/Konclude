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

#include "CReasonerEvaluationUpdaterThread.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationUpdaterThread::CReasonerEvaluationUpdaterThread() : CThread("ReasonerEvaluationUpdaterThread") {
				startThread();
			}


			CReasonerEvaluationUpdaterThread::~CReasonerEvaluationUpdaterThread() {
			}


			void CReasonerEvaluationUpdaterThread::addProgramFile(const QString& changedFile) {
				postEvent(new CReasonerEvaluationUpdateEvent(changedFile));
			}

			void CReasonerEvaluationUpdaterThread::threadStarted() {
				CThread::threadStarted();
				mSystemWatcher = new QFileSystemWatcher(this);
				connect(mSystemWatcher,SIGNAL(fileChanged(const QString&)),this,SLOT(watchedProgramFileChanged(const QString&)));
			}

			void CReasonerEvaluationUpdaterThread::threadStopped() {
				CThread::threadStopped();
				disconnect(mSystemWatcher,SIGNAL(fileChanged(const QString&)),this,SLOT(watchedProgramFileChanged(const QString&)));
			}

			bool CReasonerEvaluationUpdaterThread::processCustomsEvents(QEvent::Type type, CCustomEvent* event) {
				if (CThread::processCustomsEvents(type,event)) {
					return true;
				} else {
					if (type == CReasonerEvaluationUpdateEvent::EVENTTYPE) {
						CReasonerEvaluationUpdateEvent* repe = (CReasonerEvaluationUpdateEvent*)event;
						QString programString = repe->getProgramFile();
						mSystemWatcher->addPath(programString);
						return true;
					} else if (type == CReasonerEvaluationProgramFileChangedEvent::EVENTTYPE) {
						CReasonerEvaluationProgramFileChangedEvent* repfce = (CReasonerEvaluationProgramFileChangedEvent*)event;
						QString changedFile = repfce->getChangedFile();
						return true;
					}
				}
				return false;
			}

			void CReasonerEvaluationUpdaterThread::watchedProgramFileChanged(const QString& changedFile) {
				postEvent(new CReasonerEvaluationProgramFileChangedEvent(changedFile));
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
