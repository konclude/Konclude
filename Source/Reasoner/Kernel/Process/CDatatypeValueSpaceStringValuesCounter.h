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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEVALUESPACESTRINGVALUESCOUNTER_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEVALUESPACESTRINGVALUESCOUNTER_H

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
				 *		\class		CDatatypeValueSpaceStringValuesCounter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeValueSpaceStringValuesCounter : public CDatatypeValueSpaceValuesCounter {
					// public methods
					public:
						//! Constructor
						CDatatypeValueSpaceStringValuesCounter();


						const static cint64 SVT_WITH_LANGUAG_TAG_COUNT					= 0x0001;
						const static cint64 SVT_WITHOUT_LANGUAG_TAG_COUNT				= 0x0002;


						CDatatypeValueSpaceStringValuesCounter* initStringValueCounter(cint64 valueCount = 0);

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
						CDatatypeValueSpaceValuesCounter mWithLanguageTagTypeCounter;
						CDatatypeValueSpaceValuesCounter mWithoutLanguageTagTypeCounter;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEVALUESPACESTRINGVALUESCOUNTER_H
