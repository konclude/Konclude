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

#include "CDatatypeValueSpaceValuesCounter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeValueSpaceValuesCounter::CDatatypeValueSpaceValuesCounter() {
					initValueCounter((cint64)0);
				}

				CDatatypeValueSpaceValuesCounter* CDatatypeValueSpaceValuesCounter::initValueCounter(cint64 valueCount) {
					mValueCount = valueCount;
					mInfinite = false;
					return this;
				}


				CDatatypeValueSpaceValuesCounter* CDatatypeValueSpaceValuesCounter::resetValueCounter() {
					initValueCounter((cint64)0);
					return this;
				}


				CDatatypeValueSpaceValuesCounter* CDatatypeValueSpaceValuesCounter::initValueCounter(CDatatypeValueSpaceValuesCounter* valueCounter) {
					mValueCount = valueCounter->mValueCount;
					mInfinite = valueCounter->mInfinite;
					return this;
				}


				bool CDatatypeValueSpaceValuesCounter::incValueCount(cint64 incCount) {
					if (!mInfinite) {
						mValueCount += incCount;
						return true;
					}
					return false;
				}

				bool CDatatypeValueSpaceValuesCounter::incInfinite() {
					if (!mInfinite) {
						mInfinite = true;
						return true;
					}
					return false;
				}

				bool CDatatypeValueSpaceValuesCounter::isInfinite() {
					return mInfinite;
				}

				cint64 CDatatypeValueSpaceValuesCounter::getValueCount() {
					return mValueCount;
				}

				CDatatypeValueSpaceValuesCounter::VALUES_COUNTER_TYPE CDatatypeValueSpaceValuesCounter::getCounterType() {
					return VCT_VALUE_COUNTER;
				}


				bool CDatatypeValueSpaceValuesCounter::hasValueAchieved(cint64 value) {
					if (mInfinite) {
						return true;
					}
					return mValueCount >= value;
				}


				bool CDatatypeValueSpaceValuesCounter::combineWithValueCounter(CDatatypeValueSpaceValuesCounter* counter, cint64 additionalDiff) {
					if (counter->isInfinite()) {
						return incInfinite();
					} else {
						cint64 otherValueCount = counter->getValueCount() - additionalDiff;
						if (otherValueCount > 0) {
							return incValueCount(otherValueCount);
						}
						return false;
					}
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
