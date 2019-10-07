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

#ifndef KONCLUDE_TEST_ANALYSE_READER_CTHREADTIMESUPDATEREADER_H
#define KONCLUDE_TEST_ANALYSE_READER_CTHREADTIMESUPDATEREADER_H

// Libraries includes
#include <QString>


// Namespace includes


// Other includes
#include "Test/Analyse/AnalyseSettings.h"
#include "Test/Analyse/CAnalyseValue.h"
#include "Test/Analyse/CAnalyseValueUpdateReader.h"
#include "Test/Analyse/CAnalyseValueDescription.h"
#include "Test/Analyse/CIntegerAnalyseValue.h"
#include "Test/Analyse/CConvertStringAnalyseValue.h"

#include "Concurrent/CThread.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Concurrent;

	namespace Test {

		namespace Analyse {

			namespace Reader {

				/*! 
				 *
				 *		\class		CThreadTimesUpdateReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CThreadTimesUpdateReader : public CAnalyseValueUpdateReader {
					// public methods
					public:
						//! Constructor
						CThreadTimesUpdateReader(CThread *thread);

						//! Destructor
						virtual ~CThreadTimesUpdateReader();

						virtual CAnalyseValue *getAnalyseValueUpdate(CAnalyseValueDescription *valueDes);

						virtual CAnalyseValueDescription *getWaitTimeValueDescription();
						virtual CAnalyseValueDescription *getBlockTimeValueDescription();
						virtual CAnalyseValueDescription *getRunTimeValueDescription();

					// protected methods
					protected:

					// protected variables
					protected:

						CThread *observedThread;

						CAnalyseValueDescription *waitTimeValueDescription;
						CAnalyseValueDescription *blockTimeValueDescription;
						CAnalyseValueDescription *runTimeValueDescription;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_READER_CTHREADTIMESUPDATEREADER_H
