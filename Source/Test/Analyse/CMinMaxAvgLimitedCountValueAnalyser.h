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

#ifndef KONCLUDE_TEST_ANALYSE_CMINMAXAVGLIMITEDCOUNTVALUEANALYSER_H
#define KONCLUDE_TEST_ANALYSE_CMINMAXAVGLIMITEDCOUNTVALUEANALYSER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "AnalyseSettings.h"
#include "CAnalyseValue.h"
#include "CValueAnalyser.h"
#include "CConvertDoubleAnalyseValue.h"
#include "CIntegerAnalyseValue.h"
#include "CDoubleAnalyseValue.h"
#include "CMinMaxAvgCountValueAnalyser.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;

	namespace Test {

		namespace Analyse {


			/*! 
			 *
			 *		\class		CMinMaxAvgLimitedCountValueAnalyser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CMinMaxAvgLimitedCountValueAnalyser : public CMinMaxAvgCountValueAnalyser {
				// public methods
				public:
					//! Constructor
					CMinMaxAvgLimitedCountValueAnalyser(qint64 valueLimit);

					//! Destructor
					virtual ~CMinMaxAvgLimitedCountValueAnalyser();


					virtual CValueAnalyser *analyseValue(double value);

					virtual CMinMaxAvgLimitedCountValueAnalyser *setValueLimit(qint64 valueLimit);
					virtual qint64 getValueLimit();

				// protected methods
				protected:

				// protected variables
				protected:


					QLinkedList<double> values;

					qint64 valLimit;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_CMINMAXAVGLIMITEDCOUNTVALUEANALYSER_H
