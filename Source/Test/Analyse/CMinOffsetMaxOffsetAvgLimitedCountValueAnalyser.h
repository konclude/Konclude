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

#ifndef KONCLUDE_TEST_ANALYSE_CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser_H
#define KONCLUDE_TEST_ANALYSE_CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CMinMaxAvgLimitedCountValueAnalyser.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;

	namespace Test {

		namespace Analyse {


			/*! 
			 *
			 *		\class		CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser : public CMinMaxAvgLimitedCountValueAnalyser {
				// public methods
				public:
					//! Constructor
					CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser(qint64 valueLimit, double minOffset = 0., double maxOffset = 0.1);

					//! Destructor
					virtual ~CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser();


					virtual double getMaxValue();
					virtual double getMinValue();

				// protected methods
				protected:

				// protected variables
				protected:
					double minOff;
					double maxOff;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser_H
