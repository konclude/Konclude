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

#include "CDatatypeDoubleValueSpaceMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeDoubleValueSpaceMap::CDatatypeDoubleValueSpaceMap(CProcessContext* processContext) : CDatatypeCompareValueSpaceMap(processContext) {
				}

				bool CDatatypeDoubleValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					return false;
				}

				bool CDatatypeDoubleValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* value) {
					return false;
				}

				bool CDatatypeDoubleValueSpaceMap::getValueCopy(CDataLiteralCompareValue* valueCopyInto, CDataLiteralCompareValue* valueCopyFrom) {
					CDataLiteralDoubleValue* doubleValueCopyInto = dynamic_cast<CDataLiteralDoubleValue*>(valueCopyInto); 
					CDataLiteralDoubleValue* doubleValueCopyFrom = dynamic_cast<CDataLiteralDoubleValue*>(valueCopyFrom);
					doubleValueCopyInto->initValue(doubleValueCopyFrom);
					return true;
				}


				bool CDatatypeDoubleValueSpaceMap::isNaN(cint64 bits) {
					return ((bits & Q_UINT64_C(0x7ff0000000000000)) == Q_UINT64_C(0x7ff0000000000000)) && ((bits & Q_UINT64_C(0x000fffffffffffff)) != 0);
				}

				bool CDatatypeDoubleValueSpaceMap::getValueNext(CDataLiteralCompareValue* valueNext, CDataLiteralCompareValue* valueLast) {
					CDataLiteralDoubleValue* doubleValueNext = dynamic_cast<CDataLiteralDoubleValue*>(valueNext);
					CDataLiteralDoubleValue* doubleValueLast = dynamic_cast<CDataLiteralDoubleValue*>(valueLast);


					double nextDoubleValue = 0.;
					double lastDoubleValue = doubleValueLast->getDouble();
					cint64 bits = *reinterpret_cast<cint64*>(&lastDoubleValue);
					cint64 magnitude = (bits & Q_UINT64_C(0x7fffffffffffffff));
					bool positive = ((bits & Q_UINT64_C(0x8000000000000000))==0);
					// The successors of NaN and +INF are these numbers themselves.
					if (isNaN(bits) || (magnitude == Q_UINT64_C(0x7ff0000000000000) && positive)) {
						doubleValueNext->initValueFromDouble(lastDoubleValue);
					} else {
						bool newPositive;
						cint64 newMagnitude;
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
						cint64 newBits = newMagnitude | (newPositive ? 0 : Q_UINT64_C(0x8000000000000000));
						nextDoubleValue = *reinterpret_cast<double*>(&newBits);
						doubleValueNext->initValueFromDouble(nextDoubleValue);
						return true;
					}
					return false;
				}


				cuint64 CDatatypeDoubleValueSpaceMap::getIntervalValueCount(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					CDataLiteralDoubleValue* doubleValueLeft = dynamic_cast<CDataLiteralDoubleValue*>(leftValueExcluded);
					CDataLiteralDoubleValue* doubleValueRight = dynamic_cast<CDataLiteralDoubleValue*>(rightValueExcluded);


					double leftDoubleValue = doubleValueLeft->getDouble();
					double rightDoubleValue = doubleValueRight->getDouble();

					cint64 bitsLowerBoundExclusive = *reinterpret_cast<cint64*>(&leftDoubleValue);
					cint64 bitsUpperBoundExclusive = *reinterpret_cast<cint64*>(&rightDoubleValue);
					if (isNaN(bitsLowerBoundExclusive) || isNaN(bitsUpperBoundExclusive))
						return 0;

					cuint64 valueCount = 0;

					bool positiveLowerBoundExclusive = ((bitsLowerBoundExclusive & Q_UINT64_C(0x8000000000000000))==0);
					bool positiveUpperBoundExclusive = ((bitsUpperBoundExclusive & Q_UINT64_C(0x8000000000000000))==0);

					cint64 magnitudeLowerBoundExclusive = (bitsLowerBoundExclusive & Q_UINT64_C(0x7fffffffffffffff));
					cint64 magnitudeUpperBoundExclusive = (bitsUpperBoundExclusive & Q_UINT64_C(0x7fffffffffffffff));

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
						cint64 plusZeroToEnd = magnitudeUpperBoundExclusive;
						valueCount = startToMinusZero+plusZeroToEnd  -1; // extra -1 to count +0/-0 only once for now
					}

					return valueCount;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
