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

#ifndef KONCLUDE_TEST_ANALYSE_CANALYSEDATA_H
#define KONCLUDE_TEST_ANALYSE_CANALYSEDATA_H

// Libraries includes
#include <QString>

// Namespace includes
#include "AnalyseSettings.h"
#include "CAnalyseDataDescription.h"
#include "CAnalyseValue.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;

	namespace Test {

		namespace Analyse {


			/*! 
			 *
			 *		\class		CAnalyseData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnalyseData {
				// public methods
				public:
					//! Constructor
					CAnalyseData(qint64 valueCount, CAnalyseDataDescription *dataDes);

					//! Destructor
					virtual ~CAnalyseData();

					virtual CAnalyseDataDescription *getDataDescription();
					virtual CAnalyseData *setDataDescription(CAnalyseDataDescription *dataDes);


					virtual CAnalyseData *setValue(qint64 index, CAnalyseValue *takeValue);
					virtual CAnalyseValue *getValue(qint64 index);


					virtual CAnalyseValue *createInstalledValue(CAnalyseUpdateDescription *valueDes);
					virtual CAnalyseValue *createInstalledValue(qint64 valueDesIndex);


				// protected methods
				protected:

				// protected variables
				protected:
					CAnalyseDataDescription *description;
					CAnalyseValue **values;

					qint64 valCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_CANALYSEDATA_H
