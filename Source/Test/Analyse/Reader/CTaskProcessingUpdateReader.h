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

#ifndef KONCLUDE_TEST_ANALYSE_READER_CTASKPROCESSINGUPDATEREADER_H
#define KONCLUDE_TEST_ANALYSE_READER_CTASKPROCESSINGUPDATEREADER_H

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

#include "Scheduler/CTaskProcessorThreadBase.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Scheduler;

	namespace Test {

		namespace Analyse {

			namespace Reader {

				/*! 
				 *
				 *		\class		CTaskProcessingUpdateReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CTaskProcessingUpdateReader : public CAnalyseValueUpdateReader {
					// public methods
					public:
						//! Constructor
						CTaskProcessingUpdateReader(CTaskProcessorThreadBase* taskProcessor, const QString& threadName);

						//! Destructor
						virtual ~CTaskProcessingUpdateReader();

						virtual CAnalyseValue* getAnalyseValueUpdate(CAnalyseValueDescription* valueDes);

						virtual CAnalyseValueDescription* getReserveScheduleTaskCountValueDescription();

					// protected methods
					protected:

					// protected variables
					protected:
						CAnalyseValueDescription* mReserveScheduleTaskCountValueDescription;
						CTaskProcessorThreadBase* mTaskProcessor;

						QString mThreadName;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_READER_CTASKPROCESSINGUPDATEREADER_H
