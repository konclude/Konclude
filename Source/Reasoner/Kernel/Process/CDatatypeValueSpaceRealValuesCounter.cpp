/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CDatatypeValueSpaceRealValuesCounter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeValueSpaceRealValuesCounter::CDatatypeValueSpaceRealValuesCounter() {
					initRealValueCounter();
				}

				CDatatypeValueSpaceRealValuesCounter* CDatatypeValueSpaceRealValuesCounter::initRealValueCounter(cint64 valueCount) {
					initValueCounter(valueCount);
					mIntegerTypeCounter.initValueCounter(valueCount);
					mDecimalTypeCounter.initValueCounter(valueCount);
					mRationalTypeCounter.initValueCounter(valueCount);
					mNonIntegerTypeCounter.initValueCounter(valueCount);
					mNonDecimalTypeCounter.initValueCounter(valueCount);
					mNonRationalTypeCounter.initValueCounter(valueCount);
					return this;
				}


				bool CDatatypeValueSpaceRealValuesCounter::incValueCount(cint64 type, cint64 incCount) {
					bool counted = false;
					if ((type & RVT_INTEGER_ONLY_COUNT) != 0) {
						counted |= mIntegerTypeCounter.incValueCount(incCount);
					}
					if ((type & RVT_DECIMAL_ONLY_COUNT) != 0) {
						counted |= mDecimalTypeCounter.incValueCount(incCount);
					}
					if ((type & RVT_RATIONAL_ONLY_COUNT) != 0) {
						counted |= mRationalTypeCounter.incValueCount(incCount);
					}
					if ((type & RVT_NON_INTEGER_ONLY_COUNT) != 0) {
						counted |= mNonIntegerTypeCounter.incValueCount(incCount);
					}
					if ((type & RVT_NON_DECIMAL_ONLY_COUNT) != 0) {
						counted |= mNonDecimalTypeCounter.incValueCount(incCount);
					}
					if ((type & RVT_NON_RATIONAL_ONLY_COUNT) != 0) {
						counted |= mNonRationalTypeCounter.incValueCount(incCount);
					}
					return counted;
				}

				bool CDatatypeValueSpaceRealValuesCounter::incInfinite(cint64 type) {
					bool counted = false;
					if ((type & RVT_INTEGER_ONLY_COUNT) != 0) {
						counted |= mIntegerTypeCounter.incInfinite();
					}
					if ((type & RVT_DECIMAL_ONLY_COUNT) != 0) {
						counted |= mDecimalTypeCounter.incInfinite();
					}
					if ((type & RVT_RATIONAL_ONLY_COUNT) != 0) {
						counted |= mRationalTypeCounter.incInfinite();
					}
					if ((type & RVT_NON_INTEGER_ONLY_COUNT) != 0) {
						counted |= mNonIntegerTypeCounter.incInfinite();
					}
					if ((type & RVT_NON_DECIMAL_ONLY_COUNT) != 0) {
						counted |= mNonDecimalTypeCounter.incInfinite();
					}
					if ((type & RVT_NON_RATIONAL_ONLY_COUNT) != 0) {
						counted |= mNonRationalTypeCounter.incInfinite();
					}
					return counted;
				}

				bool CDatatypeValueSpaceRealValuesCounter::isInfinite(cint64 type) {
					bool infinite = true;
					if ((type & RVT_INTEGER_ONLY_COUNT) != 0) {
						infinite &= mIntegerTypeCounter.isInfinite();
					}
					if ((type & RVT_DECIMAL_ONLY_COUNT) != 0) {
						infinite &= mDecimalTypeCounter.isInfinite();
					}
					if ((type & RVT_RATIONAL_ONLY_COUNT) != 0) {
						infinite &= mRationalTypeCounter.isInfinite();
					}
					if ((type & RVT_NON_INTEGER_ONLY_COUNT) != 0) {
						infinite &= mNonIntegerTypeCounter.isInfinite();
					}
					if ((type & RVT_NON_DECIMAL_ONLY_COUNT) != 0) {
						infinite &= mNonDecimalTypeCounter.isInfinite();
					}
					if ((type & RVT_NON_RATIONAL_ONLY_COUNT) != 0) {
						infinite &= mNonRationalTypeCounter.isInfinite();
					}
					return infinite;
				}

				cint64 CDatatypeValueSpaceRealValuesCounter::getValueCount(cint64 type) {
					cint64 valueCount = true;
					if ((type & RVT_INTEGER_ONLY_COUNT) != 0) {
						valueCount += mIntegerTypeCounter.getValueCount();
					}
					if ((type & RVT_DECIMAL_ONLY_COUNT) != 0) {
						valueCount += mDecimalTypeCounter.getValueCount();
					}
					if ((type & RVT_RATIONAL_ONLY_COUNT) != 0) {
						valueCount += mRationalTypeCounter.getValueCount();
					}
					if ((type & RVT_NON_INTEGER_ONLY_COUNT) != 0) {
						valueCount += mNonIntegerTypeCounter.getValueCount();
					}
					if ((type & RVT_NON_DECIMAL_ONLY_COUNT) != 0) {
						valueCount += mNonDecimalTypeCounter.getValueCount();
					}
					if ((type & RVT_NON_RATIONAL_ONLY_COUNT) != 0) {
						valueCount += mNonRationalTypeCounter.getValueCount();
					}
					return valueCount;
				}


				bool CDatatypeValueSpaceRealValuesCounter::hasValueAchieved(cint64 value) {
					return CDatatypeValueSpaceValuesCounter::hasValueAchieved(value);
				}

				cint64 CDatatypeValueSpaceRealValuesCounter::getValueCount() {
					return CDatatypeValueSpaceValuesCounter::getValueCount();
				}

				bool CDatatypeValueSpaceRealValuesCounter::isInfinite() {
					return CDatatypeValueSpaceValuesCounter::isInfinite();
				}

				bool CDatatypeValueSpaceRealValuesCounter::hasValueAchieved(cint64 type, cint64 value) {
					bool achieved = true;
					if ((type & RVT_INTEGER_ONLY_COUNT) != 0) {
						achieved &= mIntegerTypeCounter.hasValueAchieved(value);
					}
					if ((type & RVT_DECIMAL_ONLY_COUNT) != 0) {
						achieved &= mDecimalTypeCounter.hasValueAchieved(value);
					}
					if ((type & RVT_RATIONAL_ONLY_COUNT) != 0) {
						achieved &= mRationalTypeCounter.hasValueAchieved(value);
					}
					if ((type & RVT_NON_INTEGER_ONLY_COUNT) != 0) {
						achieved &= mNonIntegerTypeCounter.hasValueAchieved(value);
					}
					if ((type & RVT_NON_DECIMAL_ONLY_COUNT) != 0) {
						achieved &= mNonDecimalTypeCounter.hasValueAchieved(value);
					}
					if ((type & RVT_NON_RATIONAL_ONLY_COUNT) != 0) {
						achieved &= mNonRationalTypeCounter.hasValueAchieved(value);
					}
					return achieved;
				}


				CDatatypeValueSpaceValuesCounter::VALUES_COUNTER_TYPE CDatatypeValueSpaceRealValuesCounter::getCounterType() {
					return VCT_REAL_VALUE_COUNTER;
				}


				bool CDatatypeValueSpaceRealValuesCounter::combineWithValueCounter(CDatatypeValueSpaceValuesCounter* counter, cint64 additionalDiff) {
					bool modified = false;
					modified |= CDatatypeValueSpaceValuesCounter::combineWithValueCounter(counter,additionalDiff);
					CDatatypeValueSpaceRealValuesCounter* realCounter = dynamic_cast<CDatatypeValueSpaceRealValuesCounter*>(counter);
					if (realCounter) {
						modified |= mIntegerTypeCounter.combineWithValueCounter(&realCounter->mIntegerTypeCounter,additionalDiff);
						modified |= mDecimalTypeCounter.combineWithValueCounter(&realCounter->mDecimalTypeCounter,additionalDiff);
						modified |= mRationalTypeCounter.combineWithValueCounter(&realCounter->mRationalTypeCounter,additionalDiff);
						modified |= mNonIntegerTypeCounter.combineWithValueCounter(&realCounter->mNonIntegerTypeCounter,additionalDiff);
						modified |= mNonDecimalTypeCounter.combineWithValueCounter(&realCounter->mNonDecimalTypeCounter,additionalDiff);
						modified |= mNonRationalTypeCounter.combineWithValueCounter(&realCounter->mNonRationalTypeCounter,additionalDiff);
					}
					return modified;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
