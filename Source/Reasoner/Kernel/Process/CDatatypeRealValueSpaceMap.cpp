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

#include "CDatatypeRealValueSpaceMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeRealValueSpaceMap::CDatatypeRealValueSpaceMap(CProcessContext* processContext) : CPROCESSMAP<CDatatypeRealValueSpaceMapArranger,CDatatypeRealValueSpaceMapData>(processContext),mProcessContext(processContext) {
				}


				CDatatypeRealValueSpaceMap* CDatatypeRealValueSpaceMap::initDatatypeRealValueSpaceMap(CDatatypeRealValueSpaceMap* map) {
					if (map) {
						*this = *map;
						mAbsoluteMinimumExclusionValue = map->mAbsoluteMinimumExclusionValue;
						mAbsoluteMaximumExclusionValue = map->mAbsoluteMaximumExclusionValue;
					}
					return this;
				}



				CDatatypeRealValueSpaceMap* CDatatypeRealValueSpaceMap::initDatatypeRealValueSpaceMap(CDatatypeValueSpaceRealType* valueSpaceType) {
					CPROCESSMAP<CDatatypeRealValueSpaceMapArranger,CDatatypeRealValueSpaceMapData>::clear();
					mAbsoluteMinimumExclusionValue = nullptr;
					mAbsoluteMaximumExclusionValue = nullptr;

					excludeMinimum(valueSpaceType->getMinimumDataLiteralRealValue(),false,nullptr);
					excludeMaximum(valueSpaceType->getMaximumDataLiteralRealValue(),false,nullptr);
					return this;
				}



				bool CDatatypeRealValueSpaceMap::getAbsoluteMinimumValue(CDataLiteralRealValue*& minValue, bool& minValueInclusive, CDependencyTrackPoint** depTrackPoint) {
					if (mAbsoluteMinimumExclusionValue) {
						minValue = mAbsoluteMinimumExclusionValue->getValue();
						minValueInclusive = true;
						if (depTrackPoint) {
							*depTrackPoint = mAbsoluteMinimumExclusionValue->getLeftExclusionData()->getExcludedDependencyTrackPoint();
						}
						if (mAbsoluteMinimumExclusionValue->getValueExclusionData()->isExcluded()) {
							if (mAbsoluteMinimumExclusionValue->getValueExclusionData()->getExcludedDependencyTrackPoint() == mAbsoluteMinimumExclusionValue->getLeftExclusionData()->getExcludedDependencyTrackPoint()) {
								minValueInclusive = false;
							}
						}
						return true;
					}
					return false;
				}


				bool CDatatypeRealValueSpaceMap::getAbsoluteMaximumValue(CDataLiteralRealValue*& maxValue, bool& maxValueInclusive, CDependencyTrackPoint** depTrackPoint) {
					if (mAbsoluteMaximumExclusionValue) {
						maxValue = mAbsoluteMaximumExclusionValue->getValue();
						maxValueInclusive = true;
						if (depTrackPoint) {
							*depTrackPoint = mAbsoluteMaximumExclusionValue->getRightExclusionData()->getExcludedDependencyTrackPoint();
						}
						if (mAbsoluteMaximumExclusionValue->getValueExclusionData()->isExcluded()) {
							if (mAbsoluteMaximumExclusionValue->getValueExclusionData()->getExcludedDependencyTrackPoint() == mAbsoluteMaximumExclusionValue->getRightExclusionData()->getExcludedDependencyTrackPoint()) {
								maxValueInclusive = false;
							}
						}
						return true;
					}
					return false;
				}



				bool CDatatypeRealValueSpaceMap::excludeMinimum(CDataLiteralRealValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint) {

					if (mAbsoluteMinimumExclusionValue) {
						if (mAbsoluteMinimumExclusionValue->getValue()->isGreaterThan(value)) {
							return nullptr;
						}
					}
					bool newValuesPotentiallyExcluded = false;

					CDatatypeRealValueData* valueData = getDatatypeRealValueData(value,true);
					newValuesPotentiallyExcluded |= excludeLeftDataInterval(valueData,depTrackPoint,nullptr,true);
					if (valueInclusively && !isDataValueExcluded(valueData,nullptr)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,nullptr,false);
					}
					mAbsoluteMinimumExclusionValue = valueData;

					// remove all lower values from map
					CDatatypeRealValueSpaceMap::iterator it = CDatatypeRealValueSpaceMap::begin(), itEnd = CDatatypeRealValueSpaceMap::end();
					while (it != itEnd) {
						const CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;
						if (itValueData == valueData) {
							break;
						}
						newValuesPotentiallyExcluded = true;
						it = CDatatypeRealValueSpaceMap::erase(it);
					}

					return newValuesPotentiallyExcluded;
				}



				bool CDatatypeRealValueSpaceMap::excludeMaximum(CDataLiteralRealValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint) {

					if (mAbsoluteMaximumExclusionValue) {
						if (mAbsoluteMaximumExclusionValue->getValue()->isLessThan(value)) {
							return false;
						}
					}
					bool newValuesPotentiallyExcluded = false;

					CDatatypeRealValueData* valueData = getDatatypeRealValueData(value,true);
					newValuesPotentiallyExcluded |= excludeRightDataInterval(valueData,depTrackPoint,nullptr,true);
					if (valueInclusively && !isDataValueExcluded(valueData,nullptr)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,nullptr,false);
					}
					mAbsoluteMaximumExclusionValue = valueData;

					// remove all higher values from map
					CDatatypeRealValueSpaceMapArranger mapValueArranger(value);
					CDatatypeRealValueSpaceMap::iterator itBegin = CDatatypeRealValueSpaceMap::begin(), itUB = CDatatypeRealValueSpaceMap::upperBound(mapValueArranger), itEnd = CDatatypeRealValueSpaceMap::end();
					CDatatypeRealValueSpaceMap::iterator it = itUB;
					while (it != itEnd) {
						const CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;
						it = CDatatypeRealValueSpaceMap::erase(it);
						newValuesPotentiallyExcluded = true;
					}



					return newValuesPotentiallyExcluded;
				}










				bool CDatatypeRealValueSpaceMap::excludeMinimum(CDataLiteralRealValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType) {
					bool newValuesPotentiallyExcluded = false;

					CDatatypeRealValueData* valueData = getDatatypeRealValueData(value,true);
					if (valueInclusively && !isDataValueExcluded(valueData,exclusionType)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,exclusionType,false);
					}

					CDatatypeRealValueSpaceMap::iterator itBegin = CDatatypeRealValueSpaceMap::begin(), itEnd = CDatatypeRealValueSpaceMap::end();
					CDatatypeRealValueSpaceMap::iterator it = itBegin;
					while (it != itEnd) {
						CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;

						if (it == itBegin) {
							if (!isLeftDataIntervalExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(itValueData,depTrackPoint,exclusionType,false);
							}
						}

						if (itValueData != valueData) {
							if (!isDataValueExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(itValueData,depTrackPoint,exclusionType,false);
							}

							++it;
							if (it != itEnd) {
								CDatatypeRealValueSpaceMapData& nextMapData = it.value();
								CDatatypeRealValueData* nextValueData = nextMapData.mUseValue;

								if (!isDataIntervalExcluded(itValueData,nextValueData,exclusionType)) {
									itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
									newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,exclusionType,false);
									nextValueData = getDatatypeRealValueData(nextValueData->getValue(),nextMapData,true,nullptr);
									newValuesPotentiallyExcluded |= excludeLeftDataInterval(nextValueData,depTrackPoint,exclusionType,false);
								}
							}
						} else {
							break;
						}
					}


					return newValuesPotentiallyExcluded;
				}



				bool CDatatypeRealValueSpaceMap::excludeMaximum(CDataLiteralRealValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType) {
					bool newValuesPotentiallyExcluded = false;

					CDatatypeRealValueData* valueData = getDatatypeRealValueData(value,true);
					if (valueInclusively && !isDataValueExcluded(valueData,nullptr)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,nullptr,false);						
					}

					CDatatypeRealValueSpaceMapArranger mapValueArranger(value);
					CDatatypeRealValueSpaceMap::iterator itBegin = CDatatypeRealValueSpaceMap::begin(), itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeRealValueSpaceMap::end();
					CDatatypeRealValueSpaceMap::iterator it = itLB;
					while (it != itEnd) {
						CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;

						if (itValueData != valueData) {
							if (!isDataValueExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(itValueData,depTrackPoint,exclusionType,false);
							}
						}

						++it;
						if (it != itEnd) {
							CDatatypeRealValueSpaceMapData& nextMapData = it.value();
							CDatatypeRealValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData,exclusionType)) {
								itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,exclusionType,false);
								nextValueData = getDatatypeRealValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(nextValueData,depTrackPoint,exclusionType,false);
							}
						} else {

							if (!isRightDataIntervalExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(itValueData,depTrackPoint,exclusionType,false);
							}
						}
					}

					return newValuesPotentiallyExcluded;
				}



				bool CDatatypeRealValueSpaceMap::excludeAll(CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType) {
					bool newValuesPotentiallyExcluded = false;

					CDatatypeRealValueSpaceMap::iterator itBegin = CDatatypeRealValueSpaceMap::begin(), itEnd = CDatatypeRealValueSpaceMap::end();
					CDatatypeRealValueSpaceMap::iterator it = itBegin;

					while (it != itEnd) {
						CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;

						if (it == itBegin) {
							if (!isLeftDataIntervalExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(itValueData,depTrackPoint,exclusionType,false);
							}
							if (!isDataValueExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(itValueData,depTrackPoint,exclusionType,false);
							}
						}

						++it;
						if (it != itEnd) {
							CDatatypeRealValueSpaceMapData& nextMapData = it.value();
							CDatatypeRealValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData,exclusionType)) {
								itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,exclusionType,false);
								nextValueData = getDatatypeRealValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(nextValueData,depTrackPoint,exclusionType,false);
							}
							if (!isDataValueExcluded(nextValueData,exclusionType)) {
								nextValueData = getDatatypeRealValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(nextValueData,depTrackPoint,exclusionType,false);
							}


						} else {
							if (!isRightDataIntervalExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,exclusionType,false);
							}
						}

					}

					return newValuesPotentiallyExcluded;
				}






				bool CDatatypeRealValueSpaceMap::excludeInterval(CDataLiteralRealValue* leftValue, bool leftValueInclusive, CDataLiteralRealValue* rightValue, bool rightValueInclusive, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType) {
					bool newValuesPotentiallyExcluded = false;

					CDatatypeRealValueData* leftValueData = getDatatypeRealValueData(leftValue,true);
					CDatatypeRealValueData* rightValueData = getDatatypeRealValueData(rightValue,true);

					if (leftValueInclusive && !isDataValueExcluded(leftValueData,exclusionType)) {
						newValuesPotentiallyExcluded |= excludeDataValue(leftValueData,depTrackPoint,exclusionType,false);
					}

					if (leftValueInclusive && !isDataValueExcluded(rightValueData,exclusionType)) {
						newValuesPotentiallyExcluded |= excludeDataValue(rightValueData,depTrackPoint,exclusionType,false);
					}


					CDatatypeRealValueSpaceMapArranger mapValueArranger(leftValue);
					CDatatypeRealValueSpaceMap::iterator itBegin = CDatatypeRealValueSpaceMap::begin(), itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeRealValueSpaceMap::end();
					CDatatypeRealValueSpaceMap::iterator it = itLB;
					while (it != itEnd) {
						CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;

						++it;
						if (it != itEnd) {
							CDatatypeRealValueSpaceMapData& nextMapData = it.value();
							CDatatypeRealValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData,exclusionType)) {
								itValueData = getDatatypeRealValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,exclusionType,false);
								nextValueData = getDatatypeRealValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(nextValueData,depTrackPoint,exclusionType,false);
							}

							if (nextValueData == rightValueData) {
								break;
							}

							if (!isDataValueExcluded(nextValueData,exclusionType)) {
								nextValueData = getDatatypeRealValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(nextValueData,depTrackPoint,exclusionType,false);
							}
						}
					}

					return newValuesPotentiallyExcluded;
				}







				bool CDatatypeRealValueSpaceMap::countDataIntervalValues(CDatatypeRealValueData* leftValueExcluded, CDatatypeRealValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataIntervalValues(leftValueExcluded->getValue(),rightValueExcluded->getValue(),leftValueExcluded->getRightExclusionData(),counter);
				}

				bool CDatatypeRealValueSpaceMap::countDataIntervalValues(CDataLiteralRealValue* leftValueExcluded, CDatatypeRealValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataIntervalValues(leftValueExcluded,rightValueExcluded->getValue(),rightValueExcluded->getLeftExclusionData(),counter);
				}


				bool CDatatypeRealValueSpaceMap::countDataIntervalValues(CDatatypeRealValueData* leftValueExcluded, CDataLiteralRealValue* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataIntervalValues(leftValueExcluded->getValue(),rightValueExcluded,leftValueExcluded->getRightExclusionData(),counter);
				}


				bool CDatatypeRealValueSpaceMap::countDataIntervalValues(CDataLiteralRealValue* leftValueExcluded, CDataLiteralRealValue* rightValueExcluded, CDatatypeRealValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter) {
					bool counted = false;
					if (!exlcusionData->isEverythingExcluded()) {

						if (leftValueExcluded->isInfiniteBigNumber() || rightValueExcluded->isInfiniteBigNumber()) {
							counted |= counter->incInfinite();
						} else {
							if (exlcusionData->areOnlyIntegerValuesAllowed()) {
								if (rightValueExcluded->hasFlag(CDataLiteralRealValue::DLRV_INTEGER_FLAG)) {
									cint64 incCount = rightValueExcluded->getPosNegInteger() - leftValueExcluded->getPosNegInteger() - 1;
									counted |= counter->incValueCount(incCount);
								} else {
									cint64 incCount = rightValueExcluded->getPosNegInteger() - leftValueExcluded->getPosNegInteger();
									counted |= counter->incValueCount(incCount);
								}
							} else {
								counted |= counter->incInfinite();
							}
						}

						CDatatypeValueSpaceRealValuesCounter* realValuesCounter = dynamic_cast<CDatatypeValueSpaceRealValuesCounter*>(counter);
						if (realValuesCounter) {
							if (!exlcusionData->areIntegerValuesExcluded()) {
								if (leftValueExcluded->isInfiniteBigNumber() || rightValueExcluded->isInfiniteBigNumber()) {
									counted |= realValuesCounter->incInfinite(CDatatypeValueSpaceRealValuesCounter::RVT_INTEGER_ALL_COUNT);
								}
								if (rightValueExcluded->hasFlag(CDataLiteralRealValue::DLRV_INTEGER_FLAG)) {
									cint64 incCount = rightValueExcluded->getPosNegInteger() - leftValueExcluded->getPosNegInteger() - 1;
									counted |= realValuesCounter->incValueCount(CDatatypeValueSpaceRealValuesCounter::RVT_INTEGER_ALL_COUNT,incCount);
								} else {
									cint64 incCount = rightValueExcluded->getPosNegInteger() - leftValueExcluded->getPosNegInteger();
									counted |= realValuesCounter->incValueCount(CDatatypeValueSpaceRealValuesCounter::RVT_INTEGER_ALL_COUNT,incCount);
								}
							}

							if (!exlcusionData->areDecimalValuesExcluded()) {
								counted |= realValuesCounter->incInfinite(CDatatypeValueSpaceRealValuesCounter::RVT_DECIMAL_ALL_COUNT);
							} else if (!exlcusionData->areRationalValuesExcluded()) {
								counted |= realValuesCounter->incInfinite(CDatatypeValueSpaceRealValuesCounter::RVT_RATIONAL_ALL_COUNT);
							}

							if (!exlcusionData->areRealValuesExcluded()) {
								counted |= realValuesCounter->incInfinite(CDatatypeValueSpaceRealValuesCounter::RVT_NON_RATIONAL_ALL_COUNT);
							} else if (!exlcusionData->areRationalValuesExcluded()) {
								counted |= realValuesCounter->incInfinite(CDatatypeValueSpaceRealValuesCounter::RVT_NON_DECIMAL_ALL_COUNT);
							} else if (!exlcusionData->areDecimalValuesExcluded()) {
								counted |= realValuesCounter->incInfinite(CDatatypeValueSpaceRealValuesCounter::RVT_NON_INTEGER_ALL_COUNT);
							}

						}
					}
					return counted;
				}




				bool CDatatypeRealValueSpaceMap::countDataValue(CDatatypeRealValueData* dataValue, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataValue(dataValue->getValue(),dataValue->getValueExclusionData(),counter);
				}


				bool CDatatypeRealValueSpaceMap::countDataValue(CDataLiteralRealValue* value, CDatatypeRealValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter) {
					bool counted = false;
					if (!exlcusionData->isEverythingExcluded()) {

						if (value->isInfinite()) {
							counted |= counter->incInfinite();
						} else {
							counted |= counter->incValueCount(1);
						}

						CDatatypeValueSpaceRealValuesCounter* realValuesCounter = dynamic_cast<CDatatypeValueSpaceRealValuesCounter*>(counter);
						if (realValuesCounter) {

							if (value->isInfinite()) {
								counted |= realValuesCounter->incInfinite(CDatatypeValueSpaceRealValuesCounter::RVT_INTEGER_ALL_COUNT);
								if (!exlcusionData->areDecimalValuesExcluded()) {
									counted |= realValuesCounter->incInfinite(CDatatypeValueSpaceRealValuesCounter::RVT_NON_INTEGER_ONLY_COUNT);
								}
								if (!exlcusionData->areRationalValuesExcluded()) {
									counted |= realValuesCounter->incInfinite(CDatatypeValueSpaceRealValuesCounter::RVT_NON_INTEGER_ONLY_COUNT);
									counted |= realValuesCounter->incInfinite(CDatatypeValueSpaceRealValuesCounter::RVT_NON_DECIMAL_ONLY_COUNT);
								}
							} else {
								if (value->hasFlag(CDataLiteralRealValue::DLRV_INTEGER_FLAG)) {
									counted |= realValuesCounter->incValueCount(CDatatypeValueSpaceRealValuesCounter::RVT_INTEGER_ONLY_COUNT,1);
								} else {
									counted |= realValuesCounter->incValueCount(CDatatypeValueSpaceRealValuesCounter::RVT_NON_INTEGER_ONLY_COUNT,1);
								}
								if (value->hasFlag(CDataLiteralRealValue::DLRV_DECIMAL_FLAG)) {
									counted |= realValuesCounter->incValueCount(CDatatypeValueSpaceRealValuesCounter::RVT_DECIMAL_ONLY_COUNT,1);
								} else {
									counted |= realValuesCounter->incValueCount(CDatatypeValueSpaceRealValuesCounter::RVT_NON_DECIMAL_ONLY_COUNT,1);
								}
								counted |= realValuesCounter->incValueCount(CDatatypeValueSpaceRealValuesCounter::RVT_RATIONAL_ONLY_COUNT,1);
							}


						}

					}
					return counted;
				}




				bool CDatatypeRealValueSpaceMap::countValueValues(CDataLiteralRealValue* value, CDatatypeValueSpaceValuesCounter* counter) {
					bool counted = false;
					if (mAbsoluteMinimumExclusionValue) {
						if (value->isLessThan(mAbsoluteMinimumExclusionValue->getValue())) {
							return false;
						}
					}
					if (mAbsoluteMaximumExclusionValue) {
						if (value->isGreaterThan(mAbsoluteMaximumExclusionValue->getValue())) {
							return false;
						}
					}

					CDatatypeRealValueSpaceMapArranger mapValueArranger(value);
					CDatatypeRealValueSpaceMap::iterator itBegin = CDatatypeRealValueSpaceMap::begin(), itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeRealValueSpaceMap::end();
					CDatatypeRealValueSpaceMap::iterator it = itLB;


					if (it != itEnd) {
						CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;

						if (value->isEqualTo(itValueData->getValue())) {
							if (!isDataValueExcluded(itValueData,nullptr)) {
								counted |= countDataValue(itValueData,counter);
							}
						} else {
							if (!isDataValueExcluded(value,nullptr)) {
								counted |= countDataValue(value,itValueData->getLeftExclusionData(),counter);
							}
						}
					}

					return counted;
				}







				bool CDatatypeRealValueSpaceMap::addIntervalExclusionDependencies(CDataLiteralRealValue* leftValue, bool leftValueInclusive, CDataLiteralRealValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceDependencyCollector* depCollector) {
					if (mAbsoluteMinimumExclusionValue) {
						if (leftValue->isLessThan(mAbsoluteMinimumExclusionValue->getValue())) {
							leftValue = mAbsoluteMinimumExclusionValue->getValue();
						}
					}
					if (mAbsoluteMaximumExclusionValue) {
						if (rightValue->isGreaterThan(mAbsoluteMaximumExclusionValue->getValue())) {
							rightValue = mAbsoluteMaximumExclusionValue->getValue();
						}
					}

					if (leftValue->isGreaterThan(rightValue)) {
						return false;
					} else if (leftValue->isEqualTo(rightValue)) {
						if (!leftValueInclusive && !rightValueInclusive) {
							return false;
						}
					}


					bool depAdded = false;

					CDatatypeRealValueSpaceMapArranger mapValueArranger(leftValue);
					CDatatypeRealValueSpaceMap::iterator itBegin = CDatatypeRealValueSpaceMap::begin(), itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeRealValueSpaceMap::end();
					CDatatypeRealValueSpaceMap::iterator it = itLB;


					while (it != itEnd) {
						CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;

						if (it == itLB) {
							if (leftValue->isEqualTo(itValueData->getValue())) {
								if (leftValueInclusive) {
									depAdded |= addDataValueExclusionDependencies(itValueData,depCollector);
								}
							} else {
								depAdded |= addExclusionDependencies(itValueData->getLeftExclusionData(),depCollector);
							}
						}

						++it;
						if (it != itEnd) {
							CDatatypeRealValueSpaceMapData& nextMapData = it.value();
							CDatatypeRealValueData* nextValueData = nextMapData.mUseValue;

							depAdded |= addDataIntervalExclusionDependencies(itValueData,nextValueData,depCollector);

							if (nextValueData->getValue()->isEqualTo(rightValue)) {
								if (rightValueInclusive) {
									depAdded |= addDataValueExclusionDependencies(nextValueData,depCollector);
								}
								break;
							} else if (nextValueData->getValue()->isGreaterThan(rightValue)) {
								depAdded |= addExclusionDependencies(itValueData->getRightExclusionData(),depCollector);
								break;
							}

							depAdded |= addDataValueExclusionDependencies(nextValueData,depCollector);
						}
					}

					return depAdded;

				}





				bool CDatatypeRealValueSpaceMap::addValueExclusionDependencies(CDataLiteralRealValue* value, CDatatypeValueSpaceDependencyCollector* depCollector) {
					if (mAbsoluteMinimumExclusionValue) {
						if (value->isLessThan(mAbsoluteMinimumExclusionValue->getValue())) {
							return false;
						}
					}
					if (mAbsoluteMaximumExclusionValue) {
						if (value->isGreaterThan(mAbsoluteMaximumExclusionValue->getValue())) {
							return false;
						}
					}


					bool depAdded = false;

					CDatatypeRealValueSpaceMapArranger mapValueArranger(value);
					CDatatypeRealValueSpaceMap::iterator itBegin = CDatatypeRealValueSpaceMap::begin(), itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeRealValueSpaceMap::end();
					CDatatypeRealValueSpaceMap::iterator it = itLB;


					if (it != itEnd) {
						CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;

						if (value->isEqualTo(itValueData->getValue())) {
							depAdded |= addDataValueExclusionDependencies(itValueData,depCollector);
						} else if (value->isGreaterThan(itValueData->getValue())) {
							depAdded |= addExclusionDependencies(itValueData->getRightExclusionData(),depCollector);
						} else {
							depAdded |= addExclusionDependencies(itValueData->getLeftExclusionData(),depCollector);
						}
					}

					return depAdded;
				}





				bool CDatatypeRealValueSpaceMap::countAllValues(CDatatypeValueSpaceValuesCounter* counter) {
					if (mAbsoluteMaximumExclusionValue && mAbsoluteMinimumExclusionValue) {
						return countIntervalValues(mAbsoluteMinimumExclusionValue->getValue(),true,mAbsoluteMaximumExclusionValue->getValue(),true,counter);
					}
					return false;
				}



				bool CDatatypeRealValueSpaceMap::countIntervalValues(CDataLiteralRealValue* leftValue, bool leftValueInclusive, CDataLiteralRealValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceValuesCounter* counter) {
					bool counted = false;

					if (mAbsoluteMinimumExclusionValue) {
						if (leftValue->isLessThan(mAbsoluteMinimumExclusionValue->getValue())) {
							leftValue = mAbsoluteMinimumExclusionValue->getValue();
						}
					}
					if (mAbsoluteMaximumExclusionValue) {
						if (rightValue->isGreaterThan(mAbsoluteMaximumExclusionValue->getValue())) {
							rightValue = mAbsoluteMaximumExclusionValue->getValue();
						}
					}

					if (leftValue->isGreaterThan(rightValue)) {
						return false;
					} else if (leftValue->isEqualTo(rightValue)) {
						if (!leftValueInclusive && !rightValueInclusive) {
							return false;
						}
					}



					CDatatypeRealValueSpaceMapArranger mapValueArranger(leftValue);
					CDatatypeRealValueSpaceMap::iterator itBegin = CDatatypeRealValueSpaceMap::begin(), itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeRealValueSpaceMap::end();
					CDatatypeRealValueSpaceMap::iterator it = itLB;


					while (it != itEnd) {
						CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;

						if (it == itLB) {
							if (leftValue->isEqualTo(itValueData->getValue())) {
								if (leftValueInclusive) {
									if (!isDataValueExcluded(itValueData,nullptr)) {
										counted |= countDataValue(itValueData,counter);
									}
								}
							} else {
								if (leftValueInclusive) {
									if (!isDataValueExcluded(leftValue,nullptr)) {
										counted |= countDataValue(leftValue,itValueData->getLeftExclusionData(),counter);
									}
								}
								if (!isDataIntervalExcluded(leftValue,itValueData,nullptr)) {
									counted |= countDataIntervalValues(leftValue,itValueData,counter);
								}
							}
						}

						++it;
						if (it != itEnd) {
							CDatatypeRealValueSpaceMapData& nextMapData = it.value();
							CDatatypeRealValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData,nullptr)) {
								counted |= countDataIntervalValues(itValueData,nextValueData,counter);
							}

							if (nextValueData->getValue()->isEqualTo(rightValue)) {
								if (rightValueInclusive) {
									if (!isDataValueExcluded(nextValueData,nullptr)) {
										counted |= countDataValue(nextValueData,counter);
									}
								}
								break;
							} else if (nextValueData->getValue()->isGreaterThan(rightValue)) {
								if (rightValueInclusive) {
									if (!isDataValueExcluded(rightValue,nullptr)) {
										counted |= countDataValue(rightValue,nextValueData->getLeftExclusionData(),counter);
									}
								}
								if (!isDataIntervalExcluded(nextValueData,rightValue,nullptr)) {
									counted |= countDataIntervalValues(nextValueData,rightValue,counter);
								}
								break;
							}

							if (!isDataValueExcluded(nextValueData,nullptr)) {
								counted |= countDataValue(nextValueData,counter);
							}
						}
					}

					return counted;
				}











				bool CDatatypeRealValueSpaceMap::excludeData(CDatatypeRealValueDataExclusion* exlcusionData, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType, bool force) {
					if (!exclusionType) {
						if (force || !exlcusionData->isExcluded()) {
							exlcusionData->setExcluded(true);
							exlcusionData->setExcludedDependencyTrackPoint(depTrackPoint);
							return true;
						}
					} else {
						if (force || !exlcusionData->isExcluded(exclusionType)) {
							exlcusionData->setExclusion(exclusionType,depTrackPoint);
							return true;
						}
					}
					return false;
				}


				bool CDatatypeRealValueSpaceMap::excludeDataValue(CDatatypeRealValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType, bool force) {
					return excludeData(dataValue->getValueExclusionData(),depTrackPoint,exclusionType,force);
				}

				bool CDatatypeRealValueSpaceMap::excludeLeftDataInterval(CDatatypeRealValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType, bool force) {
					return excludeData(dataValue->getLeftExclusionData(),depTrackPoint,exclusionType,force);
				}

				bool CDatatypeRealValueSpaceMap::excludeRightDataInterval(CDatatypeRealValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType, bool force) {
					return excludeData(dataValue->getRightExclusionData(),depTrackPoint,exclusionType,force);
				}



				bool CDatatypeRealValueSpaceMap::restrictToIntegerValues(CDependencyTrackPoint* depTrackPoint) {
					CDatatypeRealValueExclusionType exclusionType(CDatatypeRealValueExclusionType::RVET_INTEGER,true);
					return excludeAll(depTrackPoint,&exclusionType);
				}


				bool CDatatypeRealValueSpaceMap::restrictToNonIntegerValues(CDependencyTrackPoint* depTrackPoint) {
					CDatatypeRealValueExclusionType exclusionType(CDatatypeRealValueExclusionType::RVET_INTEGER,false);
					return excludeAll(depTrackPoint,&exclusionType);
				}


				bool CDatatypeRealValueSpaceMap::restrictToDecimalValues(CDependencyTrackPoint* depTrackPoint) {
					CDatatypeRealValueExclusionType exclusionType(CDatatypeRealValueExclusionType::RVET_DECIMAL,true);
					return excludeAll(depTrackPoint,&exclusionType);
				}


				bool CDatatypeRealValueSpaceMap::restrictToNonDecimalValues(CDependencyTrackPoint* depTrackPoint) {
					CDatatypeRealValueExclusionType exclusionType(CDatatypeRealValueExclusionType::RVET_DECIMAL,false);
					return excludeAll(depTrackPoint,&exclusionType);
				}


				bool CDatatypeRealValueSpaceMap::restrictToRationalValues(CDependencyTrackPoint* depTrackPoint) {
					CDatatypeRealValueExclusionType exclusionType(CDatatypeRealValueExclusionType::RVET_RATIONAL,true);
					return excludeAll(depTrackPoint,&exclusionType);
				}


				bool CDatatypeRealValueSpaceMap::restrictToNonRationalValues(CDependencyTrackPoint* depTrackPoint) {
					CDatatypeRealValueExclusionType exclusionType(CDatatypeRealValueExclusionType::RVET_RATIONAL,false);
					return excludeAll(depTrackPoint,&exclusionType);
				}




				bool CDatatypeRealValueSpaceMap::testValueSpaceReturnClashed() {

					CDatatypeRealValueSpaceMap::const_iterator itBegin = CDatatypeRealValueSpaceMap::constBegin(), itEnd = CDatatypeRealValueSpaceMap::constEnd();
					CDatatypeRealValueSpaceMap::const_iterator it = itBegin;

					// test whether value space is empty
					CDatatypeRealValueData* firstValueData = nullptr;
					
					while (it != itEnd) {

						const CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;
						if (!firstValueData) {
							firstValueData = itValueData;
							if (!isLeftDataIntervalExcluded(firstValueData,nullptr)) {
								return false;
							}
						}
						if (!isDataValueExcluded(itValueData,nullptr)) {
							return false;
						}
						++it;

						if (it != itEnd) {
							const CDatatypeRealValueSpaceMapData& nextMapData = it.value();
							CDatatypeRealValueData* nextValueData = nextMapData.mUseValue;
							if (!isDataIntervalExcluded(itValueData,nextValueData,nullptr)) {
								return false;
							}
						} else {
							if (!isRightDataIntervalExcluded(itValueData,nullptr)) {
								return false;
							}
						}
					}
					return true;
				}




				bool CDatatypeRealValueSpaceMap::addValueSpaceDependencies(CDatatypeValueSpaceDependencyCollector* depCollector) {

					CDatatypeRealValueSpaceMap::const_iterator itBegin = CDatatypeRealValueSpaceMap::constBegin(), itEnd = CDatatypeRealValueSpaceMap::constEnd();
					CDatatypeRealValueSpaceMap::const_iterator it = itBegin;

					it = itBegin;
					while (it != itEnd) {
						const CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;
						addDataValueExclusionDependencies(itValueData,depCollector);

						if (it == itBegin) {
							addLeftIntervalExclusionDependencies(itValueData,depCollector);
						}

						++it;
						if (it == itEnd) {
							addRightIntervalExclusionDependencies(itValueData,depCollector);
						} else {
							const CDatatypeRealValueSpaceMapData& nextMapData = it.value();
							CDatatypeRealValueData* nextValueData = nextMapData.mUseValue;

							addDataIntervalExclusionDependencies(itValueData,nextValueData,depCollector);

						}
					}
					return true;

				}







				bool CDatatypeRealValueSpaceMap::restrictToValue(CDataLiteralRealValue* value, CDependencyTrackPoint* depTrackPoint) {
					bool newValuesPotentiallyExcluded = false;
					newValuesPotentiallyExcluded |= excludeMinimum(value,false,depTrackPoint);
					newValuesPotentiallyExcluded |= excludeMaximum(value,false,depTrackPoint);
					return newValuesPotentiallyExcluded;
				}



				bool CDatatypeRealValueSpaceMap::excludeValue(CDataLiteralRealValue* value, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType) {
					bool newValuesPotentiallyExcluded = false;
					if (mAbsoluteMinimumExclusionValue) {
						if (mAbsoluteMinimumExclusionValue->getValue()->isGreaterThan(value)) {
							return false;
						}
						if (mAbsoluteMinimumExclusionValue->getValue()->isEqualTo(value)) {
							if (isDataValueExcluded(mAbsoluteMinimumExclusionValue,exclusionType)) {
								return false;
							}
						}
					}
					if (mAbsoluteMaximumExclusionValue) {
						if (mAbsoluteMaximumExclusionValue->getValue()->isGreaterThan(value)) {
							return false;
						}
						if (mAbsoluteMaximumExclusionValue->getValue()->isEqualTo(value)) {
							if (isDataValueExcluded(mAbsoluteMaximumExclusionValue,exclusionType)) {
								return false;
							}
						}
					}


					CDatatypeRealValueData* valueData = getDatatypeRealValueData(value,true);
					if (!isDataValueExcluded(valueData,exclusionType)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,exclusionType,false);
					}


					return newValuesPotentiallyExcluded;
				}







				bool CDatatypeRealValueSpaceMap::addDataValueExclusionDependencies(CDatatypeRealValueData* dataValue, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeRealValueDataExclusion* exclusionData = dataValue->getValueExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}

				bool CDatatypeRealValueSpaceMap::addDataIntervalExclusionDependencies(CDatatypeRealValueData* leftValueExcluded, CDatatypeRealValueData* rightValueExcluded, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeRealValueDataExclusion* exclusionData = leftValueExcluded->getRightExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}

				bool CDatatypeRealValueSpaceMap::addLeftIntervalExclusionDependencies(CDatatypeRealValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeRealValueDataExclusion* exclusionData = valueData->getLeftExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}

				bool CDatatypeRealValueSpaceMap::addRightIntervalExclusionDependencies(CDatatypeRealValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeRealValueDataExclusion* exclusionData = valueData->getRightExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}




				bool CDatatypeRealValueSpaceMap::addExclusionDependencies(CDatatypeRealValueDataExclusion* exclusionData, CDatatypeValueSpaceDependencyCollector* depCollector) {
					bool dependenciesAdded = false;
					CDependencyTrackPoint* depTrackPoint = nullptr;
					if (depCollector) {
						if (exclusionData->isExcluded()) {
							depTrackPoint = exclusionData->getExcludedDependencyTrackPoint();
							if (depTrackPoint) {
								dependenciesAdded |= depCollector->addDependency(depTrackPoint);
							}
						} else {
							depTrackPoint = exclusionData->getPositiveExclusionDependencyTrackPoint();
							if (depTrackPoint) {
								dependenciesAdded |= depCollector->addDependency(depTrackPoint);
							}
							depTrackPoint = exclusionData->getNegativeExclusionDependencyTrackPoint();
							if (depTrackPoint) {
								dependenciesAdded |= depCollector->addDependency(depTrackPoint);
							}
						}
					}
					return dependenciesAdded;
				}












				bool CDatatypeRealValueSpaceMap::isLeftDataIntervalExcluded(CDatatypeRealValueData* dataValue, CDatatypeRealValueExclusionType* exclusionType) {
					CDatatypeRealValueDataExclusion* exclusionData = dataValue->getLeftExclusionData();
					if (exclusionData->isEverythingExcluded()) {
						return true;
					}
					if (exclusionType) {
						if (exclusionData->isExcluded(exclusionType)) {
							return true;
						}
					}
					return false;
				}


				bool CDatatypeRealValueSpaceMap::isRightDataIntervalExcluded(CDatatypeRealValueData* dataValue, CDatatypeRealValueExclusionType* exclusionType) {
					CDatatypeRealValueDataExclusion* exclusionData = dataValue->getRightExclusionData();
					if (exclusionData->isEverythingExcluded()) {
						return true;
					}
					if (exclusionType) {
						if (exclusionData->isExcluded(exclusionType)) {
							return true;
						}
					}
					return false;
				}


				bool CDatatypeRealValueSpaceMap::isDataIntervalExcluded(CDataLiteralRealValue* leftValueExcluded, CDatatypeRealValueData* rightValueExcluded, CDatatypeRealValueExclusionType* exclusionType) {
					CDatatypeRealValueDataExclusion* exclusionData = rightValueExcluded->getLeftExclusionData();
					if (exclusionData->isEverythingExcluded()) {
						return true;
					}
					if (exclusionData->areOnlyIntegerValuesAllowed()) {
						if (!containsIntervalIntegers(leftValueExcluded,rightValueExcluded->getValue())) {
							return true;
						}
					}
					if (exclusionType) {
						if (exclusionData->isExcluded(exclusionType)) {
							return true;
						}
					}
					return false;
				}


				bool CDatatypeRealValueSpaceMap::isDataIntervalExcluded(CDatatypeRealValueData* leftValueExcluded, CDataLiteralRealValue* rightValueExcluded, CDatatypeRealValueExclusionType* exclusionType) {
					CDatatypeRealValueDataExclusion* exclusionData = leftValueExcluded->getRightExclusionData();
					if (exclusionData->isEverythingExcluded()) {
						return true;
					}
					if (exclusionData->areOnlyIntegerValuesAllowed()) {
						if (!containsIntervalIntegers(leftValueExcluded->getValue(),rightValueExcluded)) {
							return true;
						}
					}
					if (exclusionType) {
						if (exclusionData->isExcluded(exclusionType)) {
							return true;
						}
					}
					return false;
				}


				bool CDatatypeRealValueSpaceMap::isDataIntervalExcluded(CDatatypeRealValueData* leftValueExcluded, CDatatypeRealValueData* rightValueExcluded, CDatatypeRealValueExclusionType* exclusionType) {
					return isDataIntervalExcluded(leftValueExcluded,rightValueExcluded->getValue(),exclusionType);
				}



				bool CDatatypeRealValueSpaceMap::containsIntervalIntegers(CDataLiteralRealValue* leftValueExcluded, CDataLiteralRealValue* rightValueExcluded, cint64 requiredIntegerCount) {
					if (leftValueExcluded->isInfiniteBigNumber() || rightValueExcluded->isInfiniteBigNumber()) {
						return true;
					} else if (rightValueExcluded->hasFlag(CDataLiteralRealValue::DLRV_INTEGER_FLAG)) {
						return leftValueExcluded->getPosNegInteger()+requiredIntegerCount < rightValueExcluded->getPosNegInteger();
					} else {
						return leftValueExcluded->getPosNegInteger()+requiredIntegerCount <= rightValueExcluded->getPosNegInteger();
					}
				}



				bool CDatatypeRealValueSpaceMap::isDataValueExcluded(CDatatypeRealValueData* valueData, CDatatypeRealValueExclusionType* exclusionType) {
					CDatatypeRealValueDataExclusion* exclusionData = valueData->getValueExclusionData();
					if (exclusionData->isEverythingExcluded()) {
						return true;
					}
					CDataLiteralRealValue* value = valueData->getValue();
					if (value->hasFlag(CDataLiteralRealValue::DLRV_INTEGER_FLAG)) {
						if (exclusionData->areIntegerValuesExcluded()) {
							return true;
						}
					} else if (value->hasFlag(CDataLiteralRealValue::DLRV_DECIMAL_FLAG)) {
						if (exclusionData->areDecimalValuesExcluded()) {
							return true;
						}
					} else if (value->hasFlag(CDataLiteralRealValue::DLRV_RATIONAL_FLAG)) {
						if (exclusionData->areRationalValuesExcluded()) {
							return true;
						}
					}
					if (exclusionType) {
						if (exclusionData->isExcluded(exclusionType)) {
							return true;
						}
					}
					return false;
				}


				bool CDatatypeRealValueSpaceMap::isDataValueExcluded(CDataLiteralRealValue* value, CDatatypeRealValueExclusionType* exclusionType) {
					CDatatypeRealValueSpaceMapArranger mapValueArranger(value);
					CDatatypeRealValueSpaceMap::const_iterator itBegin = CDatatypeRealValueSpaceMap::constBegin(), itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeRealValueSpaceMap::constEnd();

					if (itBegin == itEnd) {
						return false;
					}
					if (itLB == itEnd) {
						return true;
					}
					const CDatatypeRealValueSpaceMapData& mapData = itLB.value();
					CDatatypeRealValueData* valueData = mapData.mUseValue;
					CDatatypeRealValueDataExclusion* exclusionData = nullptr;
					if (valueData->getValue()->isEqualTo(value)) {
						exclusionData = valueData->getValueExclusionData();
					} else if (valueData->getValue()->isLessThan(value)) {			
						exclusionData = valueData->getLeftExclusionData();
					} else {			
						exclusionData = valueData->getRightExclusionData();
					}

					if (exclusionData) {
						CDatatypeRealValueDataExclusion* exclusionData = valueData->getValueExclusionData();
						if (exclusionData->isEverythingExcluded()) {
							return true;
						}
						CDataLiteralRealValue* value = valueData->getValue();
						if (value->hasFlag(CDataLiteralRealValue::DLRV_INTEGER_FLAG)) {
							if (exclusionData->areIntegerValuesExcluded()) {
								return true;
							}
						} else if (value->hasFlag(CDataLiteralRealValue::DLRV_DECIMAL_FLAG)) {
							if (exclusionData->areDecimalValuesExcluded()) {
								return true;
							}
						} else if (value->hasFlag(CDataLiteralRealValue::DLRV_RATIONAL_FLAG)) {
							if (exclusionData->areRationalValuesExcluded()) {
								return true;
							}
						}
					}
					if (exclusionType) {
						if (exclusionData->isExcluded(exclusionType)) {
							return true;
						}
					}
					return false;
				}



				bool CDatatypeRealValueSpaceMap::adaptNewRealValueDataToNeighbours(CDatatypeRealValueData* valueData) {
					CDatatypeRealValueSpaceMapArranger mapValueArranger(valueData->getValue());
					CDatatypeRealValueSpaceMap::const_iterator itBegin = CDatatypeRealValueSpaceMap::constBegin(), itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeRealValueSpaceMap::constEnd();
					CDatatypeRealValueSpaceMap::const_iterator itAdapt = itLB;
					bool rightNeighbour = true;
					++itAdapt;
					if (itAdapt == itEnd) {
						itAdapt = itLB;
						--itAdapt;
						rightNeighbour = false;
					}
					if (itAdapt != itEnd) {
						const CDatatypeRealValueSpaceMapData& adaptMapData = itAdapt.value();
						CDatatypeRealValueData* adaptValueData = adaptMapData.mUseValue;
						CDatatypeRealValueDataExclusion* exclusionData = nullptr;
						if (rightNeighbour) {
							exclusionData = adaptValueData->getLeftExclusionData();
						} else {
							exclusionData = adaptValueData->getRightExclusionData();
						}
						valueData->getValueExclusionData()->initDataExclusion(exclusionData);
						valueData->getLeftExclusionData()->initDataExclusion(exclusionData);
						valueData->getRightExclusionData()->initDataExclusion(exclusionData);
						return true;
					}
					return false;
				}


				CDatatypeRealValueData* CDatatypeRealValueSpaceMap::getDatatypeRealValueData(CDataLiteralRealValue* value, CDatatypeRealValueSpaceMapData& valueMapData, bool createOrLocalize, bool* newValueInsertion) {
					if (newValueInsertion) {
						*newValueInsertion = false;
					}
					CDatatypeRealValueData* realValueData = nullptr;
					if (!valueMapData.mLocValue) {
						bool newlyCreated = false;
						CMemoryAllocationManager* taskMemMan = mProcessContext->getUsedMemoryAllocationManager();
						realValueData = CObjectParameterizingAllocator< CDatatypeRealValueData,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,mProcessContext);
						realValueData->initRealValueData(valueMapData.mUseValue,value);
						if (!valueMapData.mUseValue) {
							newlyCreated = true;
						}
						valueMapData.mUseValue = valueMapData.mLocValue = realValueData;
						if (newlyCreated) {
							adaptNewRealValueDataToNeighbours(realValueData);
						}
						if (newlyCreated && newValueInsertion) {
							*newValueInsertion = true;
						}
					} else {
						realValueData = valueMapData.mUseValue;
					}
					return realValueData;
				}



				CDatatypeRealValueData* CDatatypeRealValueSpaceMap::getDatatypeRealValueData(CDataLiteralRealValue* value, bool createOrLocalize, bool* newValueInsertion) {
					if (newValueInsertion) {
						*newValueInsertion = false;
					}
					CDatatypeRealValueData* realValueData = nullptr;
					CDatatypeRealValueSpaceMapArranger mapValueArranger(value);
					if (createOrLocalize) {
						CDatatypeRealValueSpaceMapData& data = CPROCESSMAP<CDatatypeRealValueSpaceMapArranger,CDatatypeRealValueSpaceMapData>::operator[](mapValueArranger);
						realValueData = getDatatypeRealValueData(value,data,createOrLocalize,newValueInsertion);
					} else {
						CDatatypeRealValueSpaceMapData data = CPROCESSMAP<CDatatypeRealValueSpaceMapArranger,CDatatypeRealValueSpaceMapData>::value(mapValueArranger);
						realValueData = data.mUseValue;
					}
					return realValueData;
				}


				bool CDatatypeRealValueSpaceMap::hasExcludedMinimum(CDataLiteralRealValue* value, bool valueInclusively, CDatatypeRealValueExclusionType* exclusionType) {

					if (mAbsoluteMinimumExclusionValue) {
						if (mAbsoluteMinimumExclusionValue->getValue()->isGreaterThan(value)) {
							return true;
						}
						if (mAbsoluteMinimumExclusionValue->getValue()->isEqualTo(value)) {
							if (isDataValueExcluded(mAbsoluteMinimumExclusionValue,exclusionType)) {
								return true;
							}
							if (!valueInclusively) {
								return true;
							}
						}
					}

					CDatatypeRealValueSpaceMapArranger mapValueArranger(value);
					CDatatypeRealValueSpaceMap::const_iterator itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), it = CDatatypeRealValueSpaceMap::constBegin(), itEnd = CDatatypeRealValueSpaceMap::constEnd();
					if (it == itEnd) {
						// no restrictions in value map
						return false;
					}
					if (it == itLB) {
						const CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* realValueData = mapData.mUseValue;
						if (!valueInclusively) {
							// value is before first value --> excluded
							return isLeftDataIntervalExcluded(realValueData,exclusionType);
						} else {
							if (!isDataValueExcluded(realValueData,exclusionType)) {
								return false;
							}
							return true;
						}
					}
					if (itLB == itEnd) {
						// value is after last value --> not excluded assuming value space is not clashed
						return false;
					}
					while (it != itEnd && it != itLB) {
						// try to find valid values before tested minimum value limit
						const CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* realValueData = mapData.mUseValue;
						if (!isDataValueExcluded(realValueData,exclusionType)) {
							return false;
						}
						++it;

						if (it != itEnd) {
							const CDatatypeRealValueSpaceMapData& nextMapData = it.value();
							CDatatypeRealValueData* nextRealValueData = nextMapData.mUseValue;
							if (it != itLB) {
								if (!isDataIntervalExcluded(realValueData,nextRealValueData,exclusionType)) {
									return false;
								}
							} else {
								if (nextRealValueData->getValue()->isEqualTo(value)) {
									// value is in the value space map
									if (!isDataIntervalExcluded(realValueData,nextRealValueData,exclusionType)) {
										return false;
									}
									if (valueInclusively && !isDataValueExcluded(nextRealValueData,exclusionType)) {
										return false;
									}
								} else {
									// test only to the tested minimum value limit
									if (!isDataIntervalExcluded(realValueData,value,exclusionType)) {
										return false;
									}
									if (valueInclusively && !isDataValueExcluded(value,exclusionType)) {
										return false;
									}
								}
							}
						}
					}
					return true;
				}



				bool CDatatypeRealValueSpaceMap::hasExcludedMaximum(CDataLiteralRealValue* value, bool valueInclusively, CDatatypeRealValueExclusionType* exclusionType) {

					if (mAbsoluteMaximumExclusionValue) {
						if (mAbsoluteMaximumExclusionValue->getValue()->isLessThan(value)) {
							return true;
						}
						if (mAbsoluteMaximumExclusionValue->getValue()->isEqualTo(value)) {
							if (isDataValueExcluded(mAbsoluteMaximumExclusionValue,exclusionType)) {
								return true;
							}
							if (!valueInclusively) {
								return true;
							}
						}
					}

					CDatatypeRealValueSpaceMapArranger mapValueArranger(value);
					CDatatypeRealValueSpaceMap::const_iterator itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), it = CDatatypeRealValueSpaceMap::constBegin(), itEnd = CDatatypeRealValueSpaceMap::constEnd();
					if (it == itEnd) {
						// no restrictions in value map
						return false;
					}
					if (itLB == itEnd) {
						// value is after last value --> excluded
						return true;
					}
					it = itLB;
					while (it != itEnd) {
						const CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* realValueData = mapData.mUseValue;
						if (it == itLB) {
							if (realValueData->getValue()->isEqualTo(value)) {
								if (valueInclusively && !isDataValueExcluded(realValueData,exclusionType)) {
									return false;
								}
							} else {
								if (!isDataIntervalExcluded(value,realValueData,exclusionType)) {
									return false;
								}
								if (!isDataValueExcluded(realValueData,exclusionType)) {
									return false;
								}
								if (valueInclusively && !isDataValueExcluded(value,exclusionType)) {
									return false;
								}
							}
						}
						++it;
						if (it != itEnd) {
							const CDatatypeRealValueSpaceMapData& nextMapData = it.value();
							CDatatypeRealValueData* nextRealValueData = nextMapData.mUseValue;

							if (!isDataValueExcluded(nextRealValueData,exclusionType)) {
								return false;
							}
							if (!isDataIntervalExcluded(realValueData,nextRealValueData,exclusionType)) {
								return false;
							}
						} else {
							if (!isRightDataIntervalExcluded(realValueData,exclusionType)) {
								return false;
							}
						}
					}
					return true;
				}


				bool CDatatypeRealValueSpaceMap::isValueExcluded(CDataLiteralRealValue* value, CDatatypeRealValueExclusionType* exclusionType) {
					if (isDataValueExcluded(value,exclusionType)) {
						return true;
					}
					return false;
				}



				bool CDatatypeRealValueSpaceMap::getNextPossibleDataValue(CDataLiteralRealValue* nextValue, CDataLiteralRealValue* lastValue) {

					if (!lastValue) {
						if (!mAbsoluteMinimumExclusionValue) {
							return false;
						}
						lastValue = mAbsoluteMinimumExclusionValue->getValue();
						if (!isDataValueExcluded(mAbsoluteMinimumExclusionValue,nullptr)) {
							if (lastValue->hasFlag(CDataLiteralRealValue::DLRV_INTEGER_FLAG)) {
								if (lastValue->isInfinite()) {
									return false;
								} else {
									nextValue->initValue(lastValue);
									return true;
								}
							}
						}
					}


					CDatatypeRealValueSpaceMapArranger mapValueArranger(lastValue);
					CDatatypeRealValueSpaceMap::iterator itBegin = CDatatypeRealValueSpaceMap::begin(), itLB = CDatatypeRealValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeRealValueSpaceMap::end();
					CDatatypeRealValueSpaceMap::iterator it = itLB;
					while (it != itEnd) {

						if (it == itLB) {
							CDatatypeRealValueData* itValueData = it.value().mUseValue;
							while (lastValue->isLessThan(itValueData->getValue())) {
								CDatatypeRealValueSpaceMap::iterator itLast = it;
								--itLast;
								if (itLast != itEnd) {
									--it;
								} else {
									if (!isLeftDataIntervalExcluded(itValueData,nullptr)) {
										if (!itValueData->getLeftExclusionData()->areOnlyIntegerValuesAllowed()) {
											return false;
										}
										nextValue->initRationalValueFromNextIntegerValue(lastValue);
										if (nextValue->isLessThan(itValueData->getValue())) {
											return true;
										} else if (nextValue->isEqualTo(itValueData->getValue())) {
											if (!isDataValueExcluded(itValueData,nullptr)) {
												return true;
											}
										}
									}
									break;
								}
							}
						}
						CDatatypeRealValueSpaceMapData& mapData = it.value();
						CDatatypeRealValueData* itValueData = mapData.mUseValue;

						if (itValueData->getValue()->isGreaterThan(lastValue)) {
							lastValue = itValueData->getValue();
						}

						++it;
						if (it != itEnd) {
							CDatatypeRealValueSpaceMapData& nextMapData = it.value();
							CDatatypeRealValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData,nullptr)) {
								if (!itValueData->getRightExclusionData()->areOnlyIntegerValuesAllowed()) {
									return false;
								}
								CDataLiteralRealValue* value = itValueData->getValue();
								nextValue->initRationalValueFromNextIntegerValue(lastValue);
								if (nextValue->isLessThan(nextValueData->getValue())) {
									return true;
								} else if (nextValue->isEqualTo(nextValueData->getValue())) {
									if (!isDataValueExcluded(nextValueData,nullptr)) {
										return true;
									}
								}
							} else if (!isDataValueExcluded(nextValueData,nullptr)) {
								nextValue->initRationalValueFromNextIntegerValue(lastValue);
								if (nextValue->isEqualTo(nextValueData->getValue())) {
									return true;
								}
							}
						} else {
							if (!isRightDataIntervalExcluded(itValueData,nullptr) || !itValueData->getRightExclusionData()->areOnlyIntegerValuesAllowed()) {
								return false;
							} else {
								nextValue->initRationalValueFromNextIntegerValue(lastValue);
								return true;
							}
						}

					}
					return false;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
