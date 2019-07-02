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
#include "CWatchDogThreadData.h"


namespace Konclude {

	namespace Concurrent {


		CWatchDogThreadData::CWatchDogThreadData() {
			reg = false;
			id = 0;
			waitTime = 0;
			runTime = 0;
			thread = 0;
			active = false;
			updateNumber = 0;
			error = false;
		}

		CWatchDogThreadData::~CWatchDogThreadData() {
		}

		bool CWatchDogThreadData::isRegisterThread() {
			return reg;
		}

		void CWatchDogThreadData::setRegisterThread(bool isRegister) {
			reg = isRegister;
		}


		void CWatchDogThreadData::setThreadName(QString threadName) {
			name = threadName;
		}

		QString CWatchDogThreadData::getThreadName() {
			return name;
		}


		void CWatchDogThreadData::setThreadID(qint64 threadID) {
			id = threadID;
		}

		qint64 CWatchDogThreadData::getThreadID() {
			return id;
		}

		void CWatchDogThreadData::setWaitTime(qint64 threadWaitTime) {
			waitTime = threadWaitTime;
		}

		qint64 CWatchDogThreadData::getWaitTime() {
			return waitTime;
		}

		void CWatchDogThreadData::setRunTime(qint64 threadRunTime) {
			runTime = threadRunTime;
		}

		qint64 CWatchDogThreadData::getRunTime() {
			return runTime;
		}

		CThread *CWatchDogThreadData::getThread() {
			return thread;
		}

		void CWatchDogThreadData::setThread(CThread *threadObject) {
			thread = threadObject;
		}

		void CWatchDogThreadData::setThreadActive(bool threadIsActive) {
			active = threadIsActive;
		}

		bool CWatchDogThreadData::isThreadActive() {
			return active;
		}


		void CWatchDogThreadData::setThreadError(bool threadHasError) {
			error = threadHasError;
		}

		bool CWatchDogThreadData::isThreadError() {
			return error;
		}

		void CWatchDogThreadData::setUpdateNumber(qint64 number, QDateTime updateTime) {
			updateNumber = number;
			upTime = updateTime;
		}

		qint64 CWatchDogThreadData::getUpdateNumber() {
			return updateNumber;
		}


		void CWatchDogThreadData::setTaskDescription(QString description) {
			taskDescription = description;
		}

		QString CWatchDogThreadData::getTaskDescription() {
			return taskDescription;
		}


	}; // end namespace Concurrent

}; // end namespace Konclude
