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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEVALUESPACEREALVALUESCOUNTER_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEVALUESPACEREALVALUESCOUNTER_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CDatatypeValueSpaceValuesCounter.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDatatypeValueSpaceRealValuesCounter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeValueSpaceRealValuesCounter : public CDatatypeValueSpaceValuesCounter {
					// public methods
					public:
						//! Constructor
						CDatatypeValueSpaceRealValuesCounter();


						const static cint64 RVT_INTEGER_ONLY_COUNT				= 0x0001;
						const static cint64 RVT_DECIMAL_ONLY_COUNT				= 0x0002;
						const static cint64 RVT_RATIONAL_ONLY_COUNT				= 0x0004;


						const static cint64 RVT_INTEGER_ALL_COUNT					= RVT_INTEGER_ONLY_COUNT | RVT_DECIMAL_ONLY_COUNT | RVT_RATIONAL_ONLY_COUNT;
						const static cint64 RVT_DECIMAL_ALL_COUNT					= RVT_DECIMAL_ONLY_COUNT | RVT_RATIONAL_ONLY_COUNT;
						const static cint64 RVT_RATIONAL_ALL_COUNT					= RVT_RATIONAL_ONLY_COUNT;


						const static cint64 RVT_NON_INTEGER_ONLY_COUNT				= 0x0010;
						const static cint64 RVT_NON_DECIMAL_ONLY_COUNT				= 0x0020;
						const static cint64 RVT_NON_RATIONAL_ONLY_COUNT				= 0x0040;

						const static cint64 RVT_NON_RATIONAL_ALL_COUNT					= RVT_NON_RATIONAL_ONLY_COUNT | RVT_NON_DECIMAL_ONLY_COUNT | RVT_NON_INTEGER_ONLY_COUNT;
						const static cint64 RVT_NON_DECIMAL_ALL_COUNT					= RVT_NON_DECIMAL_ONLY_COUNT | RVT_NON_INTEGER_ONLY_COUNT;
						const static cint64 RVT_NON_INTEGER_ALL_COUNT					= RVT_NON_INTEGER_ONLY_COUNT;


						CDatatypeValueSpaceRealValuesCounter* initRealValueCounter(cint64 valueCount = 0);

						virtual bool combineWithValueCounter(CDatatypeValueSpaceValuesCounter* counter, cint64 additionalDiff = 0);

						bool incValueCount(cint64 type, cint64 incCount = 1);
						bool incInfinite(cint64 type);

						bool isInfinite(cint64 type);
						bool isInfinite();
						cint64 getValueCount(cint64 type);
						cint64 getValueCount();

						bool hasValueAchieved(cint64 type, cint64 value);
						bool hasValueAchieved(cint64 value);


						virtual VALUES_COUNTER_TYPE getCounterType();

					// protected methods
					protected:

					// protected variables
					protected:
						CDatatypeValueSpaceValuesCounter mIntegerTypeCounter;
						CDatatypeValueSpaceValuesCounter mDecimalTypeCounter;
						CDatatypeValueSpaceValuesCounter mRationalTypeCounter;
						CDatatypeValueSpaceValuesCounter mNonIntegerTypeCounter;
						CDatatypeValueSpaceValuesCounter mNonDecimalTypeCounter;
						CDatatypeValueSpaceValuesCounter mNonRationalTypeCounter;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEVALUESPACEREALVALUESCOUNTER_H
