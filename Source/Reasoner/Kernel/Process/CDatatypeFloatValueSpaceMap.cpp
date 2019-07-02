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

#include "CDatatypeFloatValueSpaceMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeFloatValueSpaceMap::CDatatypeFloatValueSpaceMap(CProcessContext* processContext) : CDatatypeCompareValueSpaceMap(processContext) {
				}

				bool CDatatypeFloatValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					return false;
				}

				bool CDatatypeFloatValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* value) {
					return false;
				}

				bool CDatatypeFloatValueSpaceMap::getValueCopy(CDataLiteralCompareValue* valueCopyInto, CDataLiteralCompareValue* valueCopyFrom) {
					CDataLiteralFloatValue* floatValueCopyInto = dynamic_cast<CDataLiteralFloatValue*>(valueCopyInto); 
					CDataLiteralFloatValue* floatValueCopyFrom = dynamic_cast<CDataLiteralFloatValue*>(valueCopyFrom);
					floatValueCopyInto->initValue(floatValueCopyFrom);
					return true;
				}


				bool CDatatypeFloatValueSpaceMap::isNaN(int bits) {
					return ((bits & 0x7f800000) == 0x7f800000) && ((bits & 0x003fffff) != 0);
				}

				bool CDatatypeFloatValueSpaceMap::getValueNext(CDataLiteralCompareValue* valueNext, CDataLiteralCompareValue* valueLast) {
					CDataLiteralFloatValue* floatValueNext = dynamic_cast<CDataLiteralFloatValue*>(valueNext);
					CDataLiteralFloatValue* floatValueLast = dynamic_cast<CDataLiteralFloatValue*>(valueLast);


					float nextFloatValue = 0.;
					float lastFloatValue = floatValueLast->getFloat();
					int bits = *reinterpret_cast<int*>(&lastFloatValue);
					int magnitude = (bits & 0x7fffffff);
					bool positive = ((bits & 0x80000000)==0);
					// The successors of NaN and +INF are these numbers themselves.
					if (isNaN(bits) || (magnitude == 0x7f800000 && positive)) {
						floatValueNext->initValueFromFloat(lastFloatValue);
					} else {
						bool newPositive;
						int newMagnitude;
						if (positive) {
							newPositive = true;
							newMagnitude = magnitude+1;
						} else if (!positive && magnitude == 0) {
							// The successor of -0 is +0
							newPositive = true;
							newMagnitude = 1; // skip +0 for now
						} else { // if (!positive && magnitude != 0)
							newPositive = false;
							newMagnitude = magnitude-1;
						}
						int newBits = newMagnitude | (newPositive ? 0 : 0x80000000);
						nextFloatValue = *reinterpret_cast<float*>(&newBits);
						floatValueNext->initValueFromFloat(nextFloatValue);
						return true;
					}
					return false;
				}


				cuint64 CDatatypeFloatValueSpaceMap::getIntervalValueCount(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					CDataLiteralFloatValue* floatValueLeft = dynamic_cast<CDataLiteralFloatValue*>(leftValueExcluded);
					CDataLiteralFloatValue* floatValueRight = dynamic_cast<CDataLiteralFloatValue*>(rightValueExcluded);


					float leftFloatValue = floatValueLeft->getFloat();
					float rightFloatValue = floatValueRight->getFloat();

					int bitsLowerBoundExclusive = *reinterpret_cast<int*>(&leftFloatValue);
					int bitsUpperBoundExclusive = *reinterpret_cast<int*>(&rightFloatValue);
					if (isNaN(bitsLowerBoundExclusive) || isNaN(bitsUpperBoundExclusive))
						return 0;

					cuint64 valueCount = 0;

					bool positiveLowerBoundExclusive = ((bitsLowerBoundExclusive & 0x80000000)==0);
					bool positiveUpperBoundExclusive = ((bitsUpperBoundExclusive & 0x80000000)==0);

					int magnitudeLowerBoundExclusive = (bitsLowerBoundExclusive & 0x7fffffff);
					int magnitudeUpperBoundExclusive = (bitsUpperBoundExclusive & 0x7fffffff);

					// Determine the number of elements. This works even if 'lowerBoundExclusive' or 'upperBoundExclusive' is +INF or -INF
					if (positiveLowerBoundExclusive && positiveUpperBoundExclusive) {
						// it must be that magnitudeLowerBoundExclusive < magnitudeUpperBoundExclusive
						valueCount = magnitudeUpperBoundExclusive-magnitudeLowerBoundExclusive-1;

					} else if (!positiveLowerBoundExclusive && !positiveUpperBoundExclusive) {
						// it must be that magnitudeUpperBoundExclusive < magnitudeLowerBoundExclusive
						valueCount = magnitudeLowerBoundExclusive-magnitudeUpperBoundExclusive-1;

					} else if (!positiveLowerBoundExclusive && positiveUpperBoundExclusive) {
						// the number of values from 'lowerBoundExclusive' to -0
						cuint64 startToMinusZero = magnitudeLowerBoundExclusive;

						// the number of values from +0 to 'upperBoundExclusive'
						int plusZeroToEnd = magnitudeUpperBoundExclusive;
						valueCount = startToMinusZero+plusZeroToEnd  -1; // extra -1 to count +0/-0 only once for now
					}

					return valueCount;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
