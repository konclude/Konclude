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

#ifndef KONCLUDE_TEST_ANALYSE_CANALYSEDATAUPDATER_H
#define KONCLUDE_TEST_ANALYSE_CANALYSEDATAUPDATER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "AnalyseSettings.h"
#include "CAnalyseDataSeries.h"
#include "CAnalyseDataUpdateItem.h"

// Other includes
#include "Concurrent/CIntervalThread.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Concurrent;

	namespace Test {

		namespace Analyse {


			/*! 
			 *
			 *		\class		CAnalyseDataUpdater
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnalyseDataUpdater : public CIntervalThread {
				// public methods
				public:
					//! Constructor
					CAnalyseDataUpdater();

					//! Destructor
					virtual ~CAnalyseDataUpdater();

					virtual CAnalyseDataUpdater *installAnalyseDataUpdate(CAnalyseDataSeries *dataSeries, qint64 updateInterval);
					virtual CAnalyseDataUpdater *installAnalyseDataUpdateObserver(CAnalyseDataSeries *dataSeries, CAnalyseDataObserver *observer);

				// protected methods
				protected:
					virtual bool processTimer(qint64 timerID);

				// protected variables
				protected:
					QHash<CAnalyseDataSeries *, CAnalyseDataUpdateItem *> analyseUpdateHash;
					QHash<qint64, CAnalyseDataUpdateItem *> timerIDUpdateHash;
					qint64 nextTimerID;
					QMutex timerMutex;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_CANALYSEDATAUPDATER_H
