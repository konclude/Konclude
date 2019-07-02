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

#include "CDatatypeCompareValueSpaceMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeCompareValueSpaceMap::CDatatypeCompareValueSpaceMap(CProcessContext* processContext) : CPROCESSMAP<CDatatypeCompareValueSpaceMapArranger,CDatatypeCompareValueSpaceMapData>(processContext),mProcessContext(processContext) {
				}


				CDatatypeCompareValueSpaceMap* CDatatypeCompareValueSpaceMap::initDatatypeCompareValueSpaceMap(CDatatypeCompareValueSpaceMap* map) {
					if (map) {
						*this = *map;
						mAbsoluteMinimumExclusionValue = map->mAbsoluteMinimumExclusionValue;
						mAbsoluteMaximumExclusionValue = map->mAbsoluteMaximumExclusionValue;
					}
					return this;
				}



				CDatatypeCompareValueSpaceMap* CDatatypeCompareValueSpaceMap::initDatatypeCompareValueSpaceMap(CDatatypeValueSpaceCompareType* valueSpaceType) {
					CPROCESSMAP<CDatatypeCompareValueSpaceMapArranger,CDatatypeCompareValueSpaceMapData>::clear();
					mAbsoluteMinimumExclusionValue = nullptr;
					mAbsoluteMaximumExclusionValue = nullptr;

					excludeMinimum(valueSpaceType->getMinimumDataLiteralCompareValue(),false,nullptr);
					excludeMaximum(valueSpaceType->getMaximumDataLiteralCompareValue(),false,nullptr);
					return this;
				}



				bool CDatatypeCompareValueSpaceMap::getAbsoluteMinimumValue(CDataLiteralCompareValue*& minValue, bool& minValueInclusive, CDependencyTrackPoint** depTrackPoint) {
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


				bool CDatatypeCompareValueSpaceMap::getAbsoluteMaximumValue(CDataLiteralCompareValue*& maxValue, bool& maxValueInclusive, CDependencyTrackPoint** depTrackPoint) {
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



				bool CDatatypeCompareValueSpaceMap::excludeMinimum(CDataLiteralCompareValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint) {

					if (mAbsoluteMinimumExclusionValue) {
						if (mAbsoluteMinimumExclusionValue->getValue()->isGreaterThan(value)) {
							return false;
						}
					}
					bool newValuesPotentiallyExcluded = false;

					CDatatypeCompareValueData* valueData = getDatatypeCompareValueData(value,true);
					newValuesPotentiallyExcluded |= excludeLeftDataInterval(valueData,depTrackPoint,true);
					if (valueInclusively && !isDataValueExcluded(valueData)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,false);
					}
					mAbsoluteMinimumExclusionValue = valueData;

					// remove all lower values from map
					CDatatypeCompareValueSpaceMap::iterator it = CDatatypeCompareValueSpaceMap::begin(), itEnd = CDatatypeCompareValueSpaceMap::end();
					while (it != itEnd) {
						const CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;
						if (itValueData == valueData) {
							break;
						}
						newValuesPotentiallyExcluded = true;
						it = CDatatypeCompareValueSpaceMap::erase(it);
					}

					return newValuesPotentiallyExcluded;
				}



				bool CDatatypeCompareValueSpaceMap::excludeMaximum(CDataLiteralCompareValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint) {

					if (mAbsoluteMaximumExclusionValue) {
						if (mAbsoluteMaximumExclusionValue->getValue()->isLessThan(value)) {
							return false;
						}
					}
					bool newValuesPotentiallyExcluded = false;

					CDatatypeCompareValueData* valueData = getDatatypeCompareValueData(value,true);
					newValuesPotentiallyExcluded |= excludeRightDataInterval(valueData,depTrackPoint,true);
					if (valueInclusively && !isDataValueExcluded(valueData)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,false);
					}
					mAbsoluteMaximumExclusionValue = valueData;

					// remove all higher values from map
					CDatatypeCompareValueSpaceMapArranger mapValueArranger(value);
					CDatatypeCompareValueSpaceMap::iterator itBegin = CDatatypeCompareValueSpaceMap::begin(), itUB = CDatatypeCompareValueSpaceMap::upperBound(mapValueArranger), itEnd = CDatatypeCompareValueSpaceMap::end();
					CDatatypeCompareValueSpaceMap::iterator it = itUB;
					while (it != itEnd) {
						const CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;
						newValuesPotentiallyExcluded = true;
						it = CDatatypeCompareValueSpaceMap::erase(it);
					}



					return newValuesPotentiallyExcluded;
				}






				bool CDatatypeCompareValueSpaceMap::excludeAll(CDependencyTrackPoint* depTrackPoint) {
					bool newValuesPotentiallyExcluded = false;

					CDatatypeCompareValueSpaceMap::iterator itBegin = CDatatypeCompareValueSpaceMap::begin(), itEnd = CDatatypeCompareValueSpaceMap::end();
					CDatatypeCompareValueSpaceMap::iterator it = itBegin;

					while (it != itEnd) {
						CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;

						if (it == itBegin) {
							if (!isLeftDataIntervalExcluded(itValueData)) {
								itValueData = getDatatypeCompareValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(itValueData,depTrackPoint,false);
							}
							if (!isDataValueExcluded(itValueData)) {
								itValueData = getDatatypeCompareValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(itValueData,depTrackPoint,false);
							}
						}

						++it;
						if (it != itEnd) {
							CDatatypeCompareValueSpaceMapData& nextMapData = it.value();
							CDatatypeCompareValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData)) {
								itValueData = getDatatypeCompareValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,false);
								nextValueData = getDatatypeCompareValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(nextValueData,depTrackPoint,false);
							}
							if (!isDataValueExcluded(nextValueData)) {
								nextValueData = getDatatypeCompareValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(nextValueData,depTrackPoint,false);
							}


						} else {
							if (!isRightDataIntervalExcluded(itValueData)) {
								itValueData = getDatatypeCompareValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,false);
							}
						}

					}

					return newValuesPotentiallyExcluded;
				}






				bool CDatatypeCompareValueSpaceMap::excludeInterval(CDataLiteralCompareValue* leftValue, bool leftValueInclusive, CDataLiteralCompareValue* rightValue, bool rightValueInclusive, CDependencyTrackPoint* depTrackPoint) {
					bool newValuesPotentiallyExcluded = false;

					CDatatypeCompareValueData* leftValueData = getDatatypeCompareValueData(leftValue,true);
					CDatatypeCompareValueData* rightValueData = getDatatypeCompareValueData(rightValue,true);

					if (leftValueInclusive && !isDataValueExcluded(leftValueData)) {
						newValuesPotentiallyExcluded |= excludeDataValue(leftValueData,depTrackPoint,false);
					}

					if (leftValueInclusive && !isDataValueExcluded(rightValueData)) {
						newValuesPotentiallyExcluded |= excludeDataValue(rightValueData,depTrackPoint,false);
					}


					CDatatypeCompareValueSpaceMapArranger mapValueArranger(leftValue);
					CDatatypeCompareValueSpaceMap::iterator itBegin = CDatatypeCompareValueSpaceMap::begin(), itLB = CDatatypeCompareValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeCompareValueSpaceMap::end();
					CDatatypeCompareValueSpaceMap::iterator it = itLB;
					while (it != itEnd) {
						CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;

						++it;
						if (it != itEnd) {
							CDatatypeCompareValueSpaceMapData& nextMapData = it.value();
							CDatatypeCompareValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData)) {
								itValueData = getDatatypeCompareValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,false);
								nextValueData = getDatatypeCompareValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(nextValueData,depTrackPoint,false);
							}

							if (nextValueData == rightValueData) {
								break;
							}

							if (!isDataValueExcluded(nextValueData)) {
								nextValueData = getDatatypeCompareValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(nextValueData,depTrackPoint,false);
							}
						}
					}

					return newValuesPotentiallyExcluded;
				}







				bool CDatatypeCompareValueSpaceMap::countDataIntervalValues(CDatatypeCompareValueData* leftValueExcluded, CDatatypeCompareValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataIntervalValues(leftValueExcluded->getValue(),rightValueExcluded->getValue(),leftValueExcluded->getRightExclusionData(),counter);
				}

				bool CDatatypeCompareValueSpaceMap::countDataIntervalValues(CDataLiteralCompareValue* leftValueExcluded, CDatatypeCompareValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataIntervalValues(leftValueExcluded,rightValueExcluded->getValue(),rightValueExcluded->getLeftExclusionData(),counter);
				}


				bool CDatatypeCompareValueSpaceMap::countDataIntervalValues(CDatatypeCompareValueData* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataIntervalValues(leftValueExcluded->getValue(),rightValueExcluded,leftValueExcluded->getRightExclusionData(),counter);
				}


				bool CDatatypeCompareValueSpaceMap::countDataIntervalValues(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded, CDatatypeCompareValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter) {
					bool counted = false;
					if (!exlcusionData->isExcluded()) {

						if (representsInfinitelyManyValues(leftValueExcluded) || representsInfinitelyManyValues(rightValueExcluded) || representsInfinitelyManyValues(leftValueExcluded,rightValueExcluded)) {
							counted |= counter->incInfinite();
						} else {
							cuint64 valueCount = getIntervalValueCount(leftValueExcluded,rightValueExcluded);
							counted |= counter->incValueCount(valueCount);
						}

					}
					return counted;
				}




				bool CDatatypeCompareValueSpaceMap::countDataValue(CDatatypeCompareValueData* dataValue, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataValue(dataValue->getValue(),dataValue->getValueExclusionData(),counter);
				}


				bool CDatatypeCompareValueSpaceMap::countDataValue(CDataLiteralCompareValue* value, CDatatypeCompareValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter) {
					bool counted = false;
					if (!exlcusionData->isExcluded()) {

						if (representsInfinitelyManyValues(value)) {
							counted |= counter->incInfinite();
						} else {
							counted |= counter->incValueCount(1);
						}
					}
					return counted;
				}




				bool CDatatypeCompareValueSpaceMap::countValueValues(CDataLiteralCompareValue* value, CDatatypeValueSpaceValuesCounter* counter) {
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

					CDatatypeCompareValueSpaceMapArranger mapValueArranger(value);
					CDatatypeCompareValueSpaceMap::iterator itBegin = CDatatypeCompareValueSpaceMap::begin(), itLB = CDatatypeCompareValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeCompareValueSpaceMap::end();
					CDatatypeCompareValueSpaceMap::iterator it = itLB;


					if (it != itEnd) {
						CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;

						if (value->isEqualTo(itValueData->getValue())) {
							if (!isDataValueExcluded(itValueData)) {
								counted |= countDataValue(itValueData,counter);
							}
						} else {
							if (!isDataValueExcluded(value)) {
								counted |= countDataValue(value,itValueData->getLeftExclusionData(),counter);
							}
						}
					}

					return counted;
				}







				bool CDatatypeCompareValueSpaceMap::addIntervalExclusionDependencies(CDataLiteralCompareValue* leftValue, bool leftValueInclusive, CDataLiteralCompareValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceDependencyCollector* depCollector) {
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

					CDatatypeCompareValueSpaceMapArranger mapValueArranger(leftValue);
					CDatatypeCompareValueSpaceMap::iterator itBegin = CDatatypeCompareValueSpaceMap::begin(), itLB = CDatatypeCompareValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeCompareValueSpaceMap::end();
					CDatatypeCompareValueSpaceMap::iterator it = itLB;


					while (it != itEnd) {
						CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;

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
							CDatatypeCompareValueSpaceMapData& nextMapData = it.value();
							CDatatypeCompareValueData* nextValueData = nextMapData.mUseValue;

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





				bool CDatatypeCompareValueSpaceMap::addValueExclusionDependencies(CDataLiteralCompareValue* value, CDatatypeValueSpaceDependencyCollector* depCollector) {
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

					CDatatypeCompareValueSpaceMapArranger mapValueArranger(value);
					CDatatypeCompareValueSpaceMap::iterator itBegin = CDatatypeCompareValueSpaceMap::begin(), itLB = CDatatypeCompareValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeCompareValueSpaceMap::end();
					CDatatypeCompareValueSpaceMap::iterator it = itLB;


					if (it != itEnd) {
						CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;

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





				bool CDatatypeCompareValueSpaceMap::countAllValues(CDatatypeValueSpaceValuesCounter* counter) {
					if (mAbsoluteMaximumExclusionValue && mAbsoluteMinimumExclusionValue) {
						return countIntervalValues(mAbsoluteMinimumExclusionValue->getValue(),true,mAbsoluteMaximumExclusionValue->getValue(),true,counter);
					}
					return false;
				}



				bool CDatatypeCompareValueSpaceMap::countIntervalValues(CDataLiteralCompareValue* leftValue, bool leftValueInclusive, CDataLiteralCompareValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceValuesCounter* counter) {
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



					CDatatypeCompareValueSpaceMapArranger mapValueArranger(leftValue);
					CDatatypeCompareValueSpaceMap::iterator itBegin = CDatatypeCompareValueSpaceMap::begin(), itLB = CDatatypeCompareValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeCompareValueSpaceMap::end();
					CDatatypeCompareValueSpaceMap::iterator it = itLB;


					while (it != itEnd && !counter->isInfinite()) {
						CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;

						if (it == itLB) {
							if (leftValue->isEqualTo(itValueData->getValue())) {
								if (leftValueInclusive) {
									if (!isDataValueExcluded(itValueData)) {
										counted |= countDataValue(itValueData,counter);
									}
								}
							} else {
								if (leftValueInclusive) {
									if (!isDataValueExcluded(leftValue)) {
										counted |= countDataValue(leftValue,itValueData->getLeftExclusionData(),counter);
									}
								}
								if (!isDataIntervalExcluded(leftValue,itValueData)) {
									counted |= countDataIntervalValues(leftValue,itValueData,counter);
								}
							}
						}

						++it;
						if (it != itEnd) {
							CDatatypeCompareValueSpaceMapData& nextMapData = it.value();
							CDatatypeCompareValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData)) {
								counted |= countDataIntervalValues(itValueData,nextValueData,counter);
							}

							if (nextValueData->getValue()->isEqualTo(rightValue)) {
								if (rightValueInclusive) {
									if (!isDataValueExcluded(nextValueData)) {
										counted |= countDataValue(nextValueData,counter);
									}
								}
								break;
							} else if (nextValueData->getValue()->isGreaterThan(rightValue)) {
								if (rightValueInclusive) {
									if (!isDataValueExcluded(rightValue)) {
										counted |= countDataValue(rightValue,nextValueData->getLeftExclusionData(),counter);
									}
								}
								if (!isDataIntervalExcluded(nextValueData,rightValue)) {
									counted |= countDataIntervalValues(nextValueData,rightValue,counter);
								}
								break;
							}

							if (!isDataValueExcluded(nextValueData)) {
								counted |= countDataValue(nextValueData,counter);
							}
						}
					}

					return counted;
				}











				bool CDatatypeCompareValueSpaceMap::excludeData(CDatatypeCompareValueDataExclusion* exlcusionData, CDependencyTrackPoint* depTrackPoint, bool force) {
					if (force || !exlcusionData->isExcluded()) {
						exlcusionData->setExcluded(true);
						exlcusionData->setExcludedDependencyTrackPoint(depTrackPoint);
						return true;
					}
					return false;
				}


				bool CDatatypeCompareValueSpaceMap::excludeDataValue(CDatatypeCompareValueData* dataValue, CDependencyTrackPoint* depTrackPoint, bool force) {
					return excludeData(dataValue->getValueExclusionData(),depTrackPoint,force);
				}

				bool CDatatypeCompareValueSpaceMap::excludeLeftDataInterval(CDatatypeCompareValueData* dataValue, CDependencyTrackPoint* depTrackPoint, bool force) {
					return excludeData(dataValue->getLeftExclusionData(),depTrackPoint,force);
				}

				bool CDatatypeCompareValueSpaceMap::excludeRightDataInterval(CDatatypeCompareValueData* dataValue, CDependencyTrackPoint* depTrackPoint, bool force) {
					return excludeData(dataValue->getRightExclusionData(),depTrackPoint,force);
				}





				bool CDatatypeCompareValueSpaceMap::testValueSpaceReturnClashed() {

					CDatatypeCompareValueSpaceMap::const_iterator itBegin = CDatatypeCompareValueSpaceMap::constBegin(), itEnd = CDatatypeCompareValueSpaceMap::constEnd();
					CDatatypeCompareValueSpaceMap::const_iterator it = itBegin;

					// test whether value space is empty
					CDatatypeCompareValueData* firstValueData = nullptr;
					
					while (it != itEnd) {

						const CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;
						if (!firstValueData) {
							firstValueData = itValueData;
							if (!isLeftDataIntervalExcluded(firstValueData)) {
								return false;
							}
						}
						if (!isDataValueExcluded(itValueData)) {
							return false;
						}
						++it;

						if (it != itEnd) {
							const CDatatypeCompareValueSpaceMapData& nextMapData = it.value();
							CDatatypeCompareValueData* nextValueData = nextMapData.mUseValue;
							if (!isDataIntervalExcluded(itValueData,nextValueData)) {
								return false;
							}
						} else {
							if (!isRightDataIntervalExcluded(itValueData)) {
								return false;
							}
						}
					}
					return true;
				}




				bool CDatatypeCompareValueSpaceMap::addValueSpaceDependencies(CDatatypeValueSpaceDependencyCollector* depCollector) {

					CDatatypeCompareValueSpaceMap::const_iterator itBegin = CDatatypeCompareValueSpaceMap::constBegin(), itEnd = CDatatypeCompareValueSpaceMap::constEnd();
					CDatatypeCompareValueSpaceMap::const_iterator it = itBegin;

					it = itBegin;
					while (it != itEnd) {
						const CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;
						addDataValueExclusionDependencies(itValueData,depCollector);

						if (it == itBegin) {
							addLeftIntervalExclusionDependencies(itValueData,depCollector);
						}

						++it;
						if (it == itEnd) {
							addRightIntervalExclusionDependencies(itValueData,depCollector);
						} else {
							const CDatatypeCompareValueSpaceMapData& nextMapData = it.value();
							CDatatypeCompareValueData* nextValueData = nextMapData.mUseValue;

							addDataIntervalExclusionDependencies(itValueData,nextValueData,depCollector);

						}
					}
					return true;

				}







				bool CDatatypeCompareValueSpaceMap::restrictToValue(CDataLiteralCompareValue* value, CDependencyTrackPoint* depTrackPoint) {
					bool newValuesPotentiallyExcluded = false;
					newValuesPotentiallyExcluded |= excludeMinimum(value,false,depTrackPoint);
					newValuesPotentiallyExcluded |= excludeMaximum(value,false,depTrackPoint);
					return newValuesPotentiallyExcluded;
				}



				bool CDatatypeCompareValueSpaceMap::excludeValue(CDataLiteralCompareValue* value, CDependencyTrackPoint* depTrackPoint) {
					bool newValuesPotentiallyExcluded = false;
					if (mAbsoluteMinimumExclusionValue) {
						if (mAbsoluteMinimumExclusionValue->getValue()->isGreaterThan(value)) {
							return false;
						}
						if (mAbsoluteMinimumExclusionValue->getValue()->isEqualTo(value)) {
							if (isDataValueExcluded(mAbsoluteMinimumExclusionValue)) {
								return false;
							}
						}
					}
					if (mAbsoluteMaximumExclusionValue) {
						if (mAbsoluteMaximumExclusionValue->getValue()->isGreaterThan(value)) {
							return false;
						}
						if (mAbsoluteMaximumExclusionValue->getValue()->isEqualTo(value)) {
							if (isDataValueExcluded(mAbsoluteMaximumExclusionValue)) {
								return false;
							}
						}
					}


					CDatatypeCompareValueData* valueData = getDatatypeCompareValueData(value,true);
					if (!isDataValueExcluded(valueData)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,false);
					}


					return newValuesPotentiallyExcluded;
				}







				bool CDatatypeCompareValueSpaceMap::addDataValueExclusionDependencies(CDatatypeCompareValueData* dataValue, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeCompareValueDataExclusion* exclusionData = dataValue->getValueExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}

				bool CDatatypeCompareValueSpaceMap::addDataIntervalExclusionDependencies(CDatatypeCompareValueData* leftValueExcluded, CDatatypeCompareValueData* rightValueExcluded, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeCompareValueDataExclusion* exclusionData = leftValueExcluded->getRightExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}

				bool CDatatypeCompareValueSpaceMap::addLeftIntervalExclusionDependencies(CDatatypeCompareValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeCompareValueDataExclusion* exclusionData = valueData->getLeftExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}

				bool CDatatypeCompareValueSpaceMap::addRightIntervalExclusionDependencies(CDatatypeCompareValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeCompareValueDataExclusion* exclusionData = valueData->getRightExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}




				bool CDatatypeCompareValueSpaceMap::addExclusionDependencies(CDatatypeCompareValueDataExclusion* exclusionData, CDatatypeValueSpaceDependencyCollector* depCollector) {
					bool dependenciesAdded = false;
					CDependencyTrackPoint* depTrackPoint = nullptr;
					if (depCollector) {
						if (exclusionData->isExcluded()) {
							depTrackPoint = exclusionData->getExcludedDependencyTrackPoint();
							if (depTrackPoint) {
								dependenciesAdded |= depCollector->addDependency(depTrackPoint);
							}
						}
					}
					return dependenciesAdded;
				}












				bool CDatatypeCompareValueSpaceMap::isLeftDataIntervalExcluded(CDatatypeCompareValueData* dataValue) {
					CDatatypeCompareValueDataExclusion* exclusionData = dataValue->getLeftExclusionData();
					if (exclusionData->isExcluded()) {
						return true;
					}
					return false;
				}


				bool CDatatypeCompareValueSpaceMap::isRightDataIntervalExcluded(CDatatypeCompareValueData* dataValue) {
					CDatatypeCompareValueDataExclusion* exclusionData = dataValue->getRightExclusionData();
					if (exclusionData->isExcluded()) {
						return true;
					}
					return false;
				}


				bool CDatatypeCompareValueSpaceMap::isDataIntervalExcluded(CDataLiteralCompareValue* leftValueExcluded, CDatatypeCompareValueData* rightValueExcluded) {
					CDatatypeCompareValueDataExclusion* exclusionData = rightValueExcluded->getLeftExclusionData();
					if (exclusionData->isExcluded()) {
						return true;
					}
					return false;
				}


				bool CDatatypeCompareValueSpaceMap::isDataIntervalExcluded(CDatatypeCompareValueData* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					CDatatypeCompareValueDataExclusion* exclusionData = leftValueExcluded->getRightExclusionData();
					if (exclusionData->isExcluded()) {
						return true;
					}
					return false;
				}


				bool CDatatypeCompareValueSpaceMap::isDataIntervalExcluded(CDatatypeCompareValueData* leftValueExcluded, CDatatypeCompareValueData* rightValueExcluded) {
					return isDataIntervalExcluded(leftValueExcluded,rightValueExcluded->getValue());
				}






				bool CDatatypeCompareValueSpaceMap::isDataValueExcluded(CDatatypeCompareValueData* valueData) {
					CDatatypeCompareValueDataExclusion* exclusionData = valueData->getValueExclusionData();
					if (exclusionData->isExcluded()) {
						return true;
					}
					return false;
				}


				bool CDatatypeCompareValueSpaceMap::isDataValueExcluded(CDataLiteralCompareValue* value) {
					CDatatypeCompareValueSpaceMapArranger mapValueArranger(value);
					CDatatypeCompareValueSpaceMap::const_iterator itBegin = CDatatypeCompareValueSpaceMap::constBegin(), itLB = CDatatypeCompareValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeCompareValueSpaceMap::constEnd();

					if (itBegin == itEnd) {
						return false;
					}
					if (itLB == itEnd) {
						return true;
					}
					const CDatatypeCompareValueSpaceMapData& mapData = itLB.value();
					CDatatypeCompareValueData* valueData = mapData.mUseValue;
					CDatatypeCompareValueDataExclusion* exclusionData = nullptr;
					if (valueData->getValue()->isEqualTo(value)) {
						exclusionData = valueData->getValueExclusionData();
					} else if (valueData->getValue()->isLessThan(value)) {			
						exclusionData = valueData->getLeftExclusionData();
					} else {			
						exclusionData = valueData->getRightExclusionData();
					}

					if (exclusionData) {
						CDatatypeCompareValueDataExclusion* exclusionData = valueData->getValueExclusionData();
						if (exclusionData->isExcluded()) {
							return true;
						}
					}
					return false;
				}



				bool CDatatypeCompareValueSpaceMap::adaptNewCompareValueDataToNeighbours(CDatatypeCompareValueData* valueData) {
					CDatatypeCompareValueSpaceMapArranger mapValueArranger(valueData->getValue());
					CDatatypeCompareValueSpaceMap::const_iterator itBegin = CDatatypeCompareValueSpaceMap::constBegin(), itLB = CDatatypeCompareValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeCompareValueSpaceMap::constEnd();
					CDatatypeCompareValueSpaceMap::const_iterator itAdapt = itLB;
					bool rightNeighbour = true;
					++itAdapt;
					if (itAdapt == itEnd) {
						itAdapt = itLB;
						--itAdapt;
						rightNeighbour = false;
					}
					if (itAdapt != itEnd) {
						const CDatatypeCompareValueSpaceMapData& adaptMapData = itAdapt.value();
						CDatatypeCompareValueData* adaptValueData = adaptMapData.mUseValue;
						CDatatypeCompareValueDataExclusion* exclusionData = nullptr;
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


				CDatatypeCompareValueData* CDatatypeCompareValueSpaceMap::getDatatypeCompareValueData(CDataLiteralCompareValue* value, CDatatypeCompareValueSpaceMapData& valueMapData, bool createOrLocalize, bool* newValueInsertion) {
					if (newValueInsertion) {
						*newValueInsertion = false;
					}
					CDatatypeCompareValueData* compareValueData = nullptr;
					if (!valueMapData.mLocValue) {
						bool newlyCreated = false;
						CMemoryAllocationManager* taskMemMan = mProcessContext->getUsedMemoryAllocationManager();
						compareValueData = CObjectParameterizingAllocator< CDatatypeCompareValueData,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,mProcessContext);
						compareValueData->initCompareValueData(valueMapData.mUseValue,value);
						if (!valueMapData.mUseValue) {
							newlyCreated = true;
						}
						valueMapData.mUseValue = valueMapData.mLocValue = compareValueData;
						if (newlyCreated) {
							adaptNewCompareValueDataToNeighbours(compareValueData);
						}
						if (newlyCreated && newValueInsertion) {
							*newValueInsertion = true;
						}
					} else {
						compareValueData = valueMapData.mUseValue;
					}
					return compareValueData;
				}



				CDatatypeCompareValueData* CDatatypeCompareValueSpaceMap::getDatatypeCompareValueData(CDataLiteralCompareValue* value, bool createOrLocalize, bool* newValueInsertion) {
					if (newValueInsertion) {
						*newValueInsertion = false;
					}
					CDatatypeCompareValueData* compareValueData = nullptr;
					CDatatypeCompareValueSpaceMapArranger mapValueArranger(value);
					if (createOrLocalize) {
						CDatatypeCompareValueSpaceMapData& data = CPROCESSMAP<CDatatypeCompareValueSpaceMapArranger,CDatatypeCompareValueSpaceMapData>::operator[](mapValueArranger);
						compareValueData = getDatatypeCompareValueData(value,data,createOrLocalize,newValueInsertion);
					} else {
						CDatatypeCompareValueSpaceMapData data = CPROCESSMAP<CDatatypeCompareValueSpaceMapArranger,CDatatypeCompareValueSpaceMapData>::value(mapValueArranger);
						compareValueData = data.mUseValue;
					}
					return compareValueData;
				}


				bool CDatatypeCompareValueSpaceMap::hasExcludedMinimum(CDataLiteralCompareValue* value, bool valueInclusively) {

					if (mAbsoluteMinimumExclusionValue) {
						if (mAbsoluteMinimumExclusionValue->getValue()->isGreaterThan(value)) {
							return true;
						}
						if (mAbsoluteMinimumExclusionValue->getValue()->isEqualTo(value)) {
							if (isDataValueExcluded(mAbsoluteMinimumExclusionValue)) {
								return true;
							}
							if (!valueInclusively) {
								return true;
							}
						}
					}

					CDatatypeCompareValueSpaceMapArranger mapValueArranger(value);
					CDatatypeCompareValueSpaceMap::const_iterator itLB = CDatatypeCompareValueSpaceMap::lowerBound(mapValueArranger), it = CDatatypeCompareValueSpaceMap::constBegin(), itEnd = CDatatypeCompareValueSpaceMap::constEnd();
					if (it == itEnd) {
						// no restrictions in value map
						return false;
					}
					if (it == itLB) {
						const CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* compareValueData = mapData.mUseValue;
						if (!valueInclusively) {
							// value is before first value --> excluded
							return isLeftDataIntervalExcluded(compareValueData);
						} else {
							if (!isDataValueExcluded(compareValueData)) {
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
						const CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* compareValueData = mapData.mUseValue;
						if (!isDataValueExcluded(compareValueData)) {
							return false;
						}
						++it;

						if (it != itEnd) {
							const CDatatypeCompareValueSpaceMapData& nextMapData = it.value();
							CDatatypeCompareValueData* nextCompareValueData = nextMapData.mUseValue;
							if (it != itLB) {
								if (!isDataIntervalExcluded(compareValueData,nextCompareValueData)) {
									return false;
								}
							} else {
								if (nextCompareValueData->getValue()->isEqualTo(value)) {
									// value is in the value space map
									if (!isDataIntervalExcluded(compareValueData,nextCompareValueData)) {
										return false;
									}
									if (valueInclusively && !isDataValueExcluded(nextCompareValueData)) {
										return false;
									}
								} else {
									// test only to the tested minimum value limit
									if (!isDataIntervalExcluded(compareValueData,value)) {
										return false;
									}
									if (valueInclusively && !isDataValueExcluded(value)) {
										return false;
									}
								}
							}
						}
					}
					return true;
				}



				bool CDatatypeCompareValueSpaceMap::hasExcludedMaximum(CDataLiteralCompareValue* value, bool valueInclusively) {

					if (mAbsoluteMaximumExclusionValue) {
						if (mAbsoluteMaximumExclusionValue->getValue()->isLessThan(value)) {
							return true;
						}
						if (mAbsoluteMaximumExclusionValue->getValue()->isEqualTo(value)) {
							if (isDataValueExcluded(mAbsoluteMaximumExclusionValue)) {
								return true;
							}
							if (!valueInclusively) {
								return true;
							}
						}
					}

					CDatatypeCompareValueSpaceMapArranger mapValueArranger(value);
					CDatatypeCompareValueSpaceMap::const_iterator itLB = CDatatypeCompareValueSpaceMap::lowerBound(mapValueArranger), it = CDatatypeCompareValueSpaceMap::constBegin(), itEnd = CDatatypeCompareValueSpaceMap::constEnd();
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
						const CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* compareValueData = mapData.mUseValue;
						if (it == itLB) {
							if (compareValueData->getValue()->isEqualTo(value)) {
								if (valueInclusively && !isDataValueExcluded(compareValueData)) {
									return false;
								}
							} else {
								if (!isDataIntervalExcluded(value,compareValueData)) {
									return false;
								}
								if (!isDataValueExcluded(compareValueData)) {
									return false;
								}
								if (valueInclusively && !isDataValueExcluded(value)) {
									return false;
								}
							}
						}
						++it;
						if (it != itEnd) {
							const CDatatypeCompareValueSpaceMapData& nextMapData = it.value();
							CDatatypeCompareValueData* nextCompareValueData = nextMapData.mUseValue;

							if (!isDataValueExcluded(nextCompareValueData)) {
								return false;
							}
							if (!isDataIntervalExcluded(compareValueData,nextCompareValueData)) {
								return false;
							}
						} else {
							if (!isRightDataIntervalExcluded(compareValueData)) {
								return false;
							}
						}
					}
					return true;
				}


				bool CDatatypeCompareValueSpaceMap::isValueExcluded(CDataLiteralCompareValue* value) {
					if (isDataValueExcluded(value)) {
						return true;
					}
					return false;
				}



				bool CDatatypeCompareValueSpaceMap::getNextPossibleDataValue(CDataLiteralCompareValue* nextValue, CDataLiteralCompareValue* lastValue) {

					if (!lastValue) {
						if (!mAbsoluteMinimumExclusionValue) {
							return false;
						}
						lastValue = mAbsoluteMinimumExclusionValue->getValue();
						if (!isDataValueExcluded(mAbsoluteMinimumExclusionValue)) {
							if (representsInfinitelyManyValues(lastValue)) {
								return false;
							} else {
								getValueCopy(nextValue,lastValue);
								return true;
							}
						}
					}


					CDatatypeCompareValueSpaceMapArranger mapValueArranger(lastValue);
					CDatatypeCompareValueSpaceMap::iterator itBegin = CDatatypeCompareValueSpaceMap::begin(), itLB = CDatatypeCompareValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeCompareValueSpaceMap::end();
					CDatatypeCompareValueSpaceMap::iterator it = itLB;
					while (it != itEnd) {

						if (it == itLB) {
							CDatatypeCompareValueData* itValueData = it.value().mUseValue;
							while (lastValue->isLessThan(itValueData->getValue())) {
								CDatatypeCompareValueSpaceMap::iterator itLast = it;
								--itLast;
								if (itLast != itEnd) {
									--it;
								} else {
									if (!isLeftDataIntervalExcluded(itValueData)) {
										getValueNext(nextValue,lastValue);
										if (nextValue->isLessThan(itValueData->getValue())) {
											return true;
										} else if (nextValue->isEqualTo(itValueData->getValue())) {
											if (!isDataValueExcluded(itValueData)) {
												return true;
											}
										}
									}
									break;
								}
							}
						}
						CDatatypeCompareValueSpaceMapData& mapData = it.value();
						CDatatypeCompareValueData* itValueData = mapData.mUseValue;

						if (itValueData->getValue()->isGreaterThan(lastValue)) {
							lastValue = itValueData->getValue();
						}

						++it;
						if (it != itEnd) {
							CDatatypeCompareValueSpaceMapData& nextMapData = it.value();
							CDatatypeCompareValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData)) {
								getValueNext(nextValue,lastValue);
								if (nextValue->isLessThan(nextValueData->getValue())) {
									return true;
								} else if (nextValue->isEqualTo(nextValueData->getValue())) {
									if (!isDataValueExcluded(nextValueData)) {
										return true;
									}
								}
							} else if (!isDataValueExcluded(nextValueData)) {
								getValueNext(nextValue,lastValue);
								if (nextValue->isEqualTo(nextValueData->getValue())) {
									return true;
								}
							}
						} else {
							if (!isRightDataIntervalExcluded(itValueData)) {
								return false;
							} else {
								getValueNext(nextValue,lastValue);
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
