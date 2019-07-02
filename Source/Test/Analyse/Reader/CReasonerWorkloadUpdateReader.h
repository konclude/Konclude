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

#ifndef KONCLUDE_TEST_ANALYSE_READER_CREASONERWORKLOADUPDATEREADER_H
#define KONCLUDE_TEST_ANALYSE_READER_CREASONERWORKLOADUPDATEREADER_H

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
#include "Test/Analyse/CDoubleAnalyseValue.h"


#include "Reasoner/Kernel/Manager/CAnalyseReasonerManager.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Scheduler;

	using namespace Reasoner::Kernel;
	using namespace Reasoner::Kernel::Manager;

	namespace Test {

		namespace Analyse {

			namespace Reader {

				/*! 
				 *
				 *		\class		CReasonerWorkloadUpdateReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReasonerWorkloadUpdateReader : public CAnalyseValueUpdateReader {
					// public methods
					public:
						//! Constructor
						CReasonerWorkloadUpdateReader(CAnalyseReasonerManager  *reasonerManager);

						//! Destructor
						virtual ~CReasonerWorkloadUpdateReader();

						virtual CAnalyseValue *getAnalyseValueUpdate(CAnalyseValueDescription *valueDes);

						virtual CAnalyseValueDescription *getQueryTotalWorkCountValueDescription();
						virtual CAnalyseValueDescription *getQueryCalcedWorkCountValueDescription();
						virtual CAnalyseValueDescription *getQueryProgressValueDescription();

					// protected methods
					protected:

					// protected variables
					protected:
						CAnalyseReasonerManager *reasoner;

						CAnalyseValueDescription *queryProgressDescription;
						CAnalyseValueDescription *queryTotalWorkDescription;
						CAnalyseValueDescription *queryClosedWorkDescription;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_READER_CREASONERWORKLOADUPDATEREADER_H
