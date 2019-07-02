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

#include "CDatatypeValueSpaceStringValuesCounter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeValueSpaceStringValuesCounter::CDatatypeValueSpaceStringValuesCounter() {
					initStringValueCounter();
				}

				CDatatypeValueSpaceStringValuesCounter* CDatatypeValueSpaceStringValuesCounter::initStringValueCounter(cint64 valueCount) {
					initValueCounter(valueCount);
					mWithLanguageTagTypeCounter.initValueCounter(valueCount);
					mWithoutLanguageTagTypeCounter.initValueCounter(valueCount);
					return this;
				}


				bool CDatatypeValueSpaceStringValuesCounter::incValueCount(cint64 type, cint64 incCount) {
					bool counted = false;
					if ((type & SVT_WITH_LANGUAG_TAG_COUNT) != 0) {
						counted |= mWithLanguageTagTypeCounter.incValueCount(incCount);
					}
					if ((type & SVT_WITHOUT_LANGUAG_TAG_COUNT) != 0) {
						counted |= mWithoutLanguageTagTypeCounter.incValueCount(incCount);
					}
					return counted;
				}

				bool CDatatypeValueSpaceStringValuesCounter::incInfinite(cint64 type) {
					bool counted = false;
					if ((type & SVT_WITH_LANGUAG_TAG_COUNT) != 0) {
						counted |= mWithLanguageTagTypeCounter.incInfinite();
					}
					if ((type & SVT_WITHOUT_LANGUAG_TAG_COUNT) != 0) {
						counted |= mWithoutLanguageTagTypeCounter.incInfinite();
					}
					return counted;
				}

				bool CDatatypeValueSpaceStringValuesCounter::isInfinite(cint64 type) {
					bool infinite = true;
					if ((type & SVT_WITH_LANGUAG_TAG_COUNT) != 0) {
						infinite &= mWithLanguageTagTypeCounter.isInfinite();
					}
					if ((type & SVT_WITHOUT_LANGUAG_TAG_COUNT) != 0) {
						infinite &= mWithoutLanguageTagTypeCounter.isInfinite();
					}
					return infinite;
				}

				cint64 CDatatypeValueSpaceStringValuesCounter::getValueCount(cint64 type) {
					cint64 valueCount = true;
					if ((type & SVT_WITH_LANGUAG_TAG_COUNT) != 0) {
						valueCount += mWithLanguageTagTypeCounter.getValueCount();
					}
					if ((type & SVT_WITHOUT_LANGUAG_TAG_COUNT) != 0) {
						valueCount += mWithoutLanguageTagTypeCounter.getValueCount();
					}
					return valueCount;
				}


				bool CDatatypeValueSpaceStringValuesCounter::hasValueAchieved(cint64 value) {
					return CDatatypeValueSpaceValuesCounter::hasValueAchieved(value);
				}

				cint64 CDatatypeValueSpaceStringValuesCounter::getValueCount() {
					return CDatatypeValueSpaceValuesCounter::getValueCount();
				}

				bool CDatatypeValueSpaceStringValuesCounter::isInfinite() {
					return CDatatypeValueSpaceValuesCounter::isInfinite();
				}

				bool CDatatypeValueSpaceStringValuesCounter::hasValueAchieved(cint64 type, cint64 value) {
					bool achieved = true;
					if ((type & SVT_WITH_LANGUAG_TAG_COUNT) != 0) {
						achieved &= mWithLanguageTagTypeCounter.hasValueAchieved(value);
					}
					if ((type & SVT_WITHOUT_LANGUAG_TAG_COUNT) != 0) {
						achieved &= mWithoutLanguageTagTypeCounter.hasValueAchieved(value);
					}
					return achieved;
				}


				CDatatypeValueSpaceValuesCounter::VALUES_COUNTER_TYPE CDatatypeValueSpaceStringValuesCounter::getCounterType() {
					return VCT_STRING_VALUE_COUNTER;
				}


				bool CDatatypeValueSpaceStringValuesCounter::combineWithValueCounter(CDatatypeValueSpaceValuesCounter* counter, cint64 additionalDiff) {
					bool modified = false;
					modified |= CDatatypeValueSpaceValuesCounter::combineWithValueCounter(counter,additionalDiff);
					CDatatypeValueSpaceStringValuesCounter* realCounter = dynamic_cast<CDatatypeValueSpaceStringValuesCounter*>(counter);
					if (realCounter) {
						modified |= mWithLanguageTagTypeCounter.combineWithValueCounter(&realCounter->mWithLanguageTagTypeCounter,additionalDiff);
						modified |= mWithoutLanguageTagTypeCounter.combineWithValueCounter(&realCounter->mWithoutLanguageTagTypeCounter,additionalDiff);
					}
					return modified;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
