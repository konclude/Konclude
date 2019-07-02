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

#include "CDatatypeStringValueSpaceMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeStringValueSpaceMap::CDatatypeStringValueSpaceMap(CProcessContext* processContext) : CPROCESSMAP<CDatatypeStringValueSpaceMapArranger,CDatatypeStringValueSpaceMapData>(processContext),mProcessContext(processContext) {
				}


				CDatatypeStringValueSpaceMap* CDatatypeStringValueSpaceMap::initDatatypeStringValueSpaceMap(CDatatypeStringValueSpaceMap* map) {
					if (map) {
						*this = *map;
						mAbsoluteMinimumExclusionValue = map->mAbsoluteMinimumExclusionValue;
						mAbsoluteMaximumExclusionValue = map->mAbsoluteMaximumExclusionValue;
					}
					return this;
				}



				CDatatypeStringValueSpaceMap* CDatatypeStringValueSpaceMap::initDatatypeStringValueSpaceMap(CDatatypeValueSpaceStringType* valueSpaceType) {
					CPROCESSMAP<CDatatypeStringValueSpaceMapArranger,CDatatypeStringValueSpaceMapData>::clear();
					mAbsoluteMinimumExclusionValue = nullptr;
					mAbsoluteMaximumExclusionValue = nullptr;

					excludeMinimum(valueSpaceType->getMinimumDataLiteralStringValue(),false,nullptr);
					excludeMaximum(valueSpaceType->getMaximumDataLiteralStringValue(),false,nullptr);
					return this;
				}



				bool CDatatypeStringValueSpaceMap::getAbsoluteMinimumValue(CDataLiteralStringValue*& minValue, bool& minValueInclusive, CDependencyTrackPoint** depTrackPoint) {
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


				bool CDatatypeStringValueSpaceMap::getAbsoluteMaximumValue(CDataLiteralStringValue*& maxValue, bool& maxValueInclusive, CDependencyTrackPoint** depTrackPoint) {
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



				bool CDatatypeStringValueSpaceMap::excludeMinimum(CDataLiteralStringValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint) {

					if (mAbsoluteMinimumExclusionValue) {
						if (mAbsoluteMinimumExclusionValue->getValue()->isGreaterThan(value)) {
							return nullptr;
						}
					}
					bool newValuesPotentiallyExcluded = false;

					CDatatypeStringValueData* valueData = getDatatypeStringValueData(value,true);
					newValuesPotentiallyExcluded |= excludeLeftDataInterval(valueData,depTrackPoint,nullptr,true);
					if (valueInclusively && !isDataValueExcluded(valueData,nullptr)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,nullptr,false);
					}
					mAbsoluteMinimumExclusionValue = valueData;

					// remove all lower values from map
					CDatatypeStringValueSpaceMap::iterator it = CDatatypeStringValueSpaceMap::begin(), itEnd = CDatatypeStringValueSpaceMap::end();
					while (it != itEnd) {
						const CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;
						if (itValueData == valueData) {
							break;
						}
						newValuesPotentiallyExcluded = true;
						it = CDatatypeStringValueSpaceMap::erase(it);
					}

					return newValuesPotentiallyExcluded;
				}



				bool CDatatypeStringValueSpaceMap::excludeMaximum(CDataLiteralStringValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint) {

					if (mAbsoluteMaximumExclusionValue) {
						if (mAbsoluteMaximumExclusionValue->getValue()->isLessThan(value)) {
							return false;
						}
					}
					bool newValuesPotentiallyExcluded = false;

					CDatatypeStringValueData* valueData = getDatatypeStringValueData(value,true);
					newValuesPotentiallyExcluded |= excludeRightDataInterval(valueData,depTrackPoint,nullptr,true);
					if (valueInclusively && !isDataValueExcluded(valueData,nullptr)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,nullptr,false);
					}
					mAbsoluteMaximumExclusionValue = valueData;

					// remove all higher values from map
					CDatatypeStringValueSpaceMapArranger mapValueArranger(value);
					CDatatypeStringValueSpaceMap::iterator itBegin = CDatatypeStringValueSpaceMap::begin(), itUB = CDatatypeStringValueSpaceMap::upperBound(mapValueArranger), itEnd = CDatatypeStringValueSpaceMap::end();
					CDatatypeStringValueSpaceMap::iterator it = itUB;
					while (it != itEnd) {
						const CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;
						it = CDatatypeStringValueSpaceMap::erase(it);
						newValuesPotentiallyExcluded = true;
					}



					return newValuesPotentiallyExcluded;
				}










				bool CDatatypeStringValueSpaceMap::excludeMinimum(CDataLiteralStringValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType) {
					bool newValuesPotentiallyExcluded = false;

					CDatatypeStringValueData* valueData = getDatatypeStringValueData(value,true);
					if (valueInclusively && !isDataValueExcluded(valueData,exclusionType)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,exclusionType,false);
					}

					CDatatypeStringValueSpaceMap::iterator itBegin = CDatatypeStringValueSpaceMap::begin(), itEnd = CDatatypeStringValueSpaceMap::end();
					CDatatypeStringValueSpaceMap::iterator it = itBegin;
					while (it != itEnd) {
						CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;

						if (it == itBegin) {
							if (!isLeftDataIntervalExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(itValueData,depTrackPoint,exclusionType,false);
							}
						}

						if (itValueData != valueData) {
							if (!isDataValueExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(itValueData,depTrackPoint,exclusionType,false);
							}

							++it;
							if (it != itEnd) {
								CDatatypeStringValueSpaceMapData& nextMapData = it.value();
								CDatatypeStringValueData* nextValueData = nextMapData.mUseValue;

								if (!isDataIntervalExcluded(itValueData,nextValueData,exclusionType)) {
									itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
									newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,exclusionType,false);
									nextValueData = getDatatypeStringValueData(nextValueData->getValue(),nextMapData,true,nullptr);
									newValuesPotentiallyExcluded |= excludeLeftDataInterval(nextValueData,depTrackPoint,exclusionType,false);
								}
							}
						} else {
							break;
						}
					}


					return newValuesPotentiallyExcluded;
				}



				bool CDatatypeStringValueSpaceMap::excludeMaximum(CDataLiteralStringValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType) {
					bool newValuesPotentiallyExcluded = false;

					CDatatypeStringValueData* valueData = getDatatypeStringValueData(value,true);
					if (valueInclusively && !isDataValueExcluded(valueData,nullptr)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,nullptr,false);						
					}

					CDatatypeStringValueSpaceMapArranger mapValueArranger(value);
					CDatatypeStringValueSpaceMap::iterator itBegin = CDatatypeStringValueSpaceMap::begin(), itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeStringValueSpaceMap::end();
					CDatatypeStringValueSpaceMap::iterator it = itLB;
					while (it != itEnd) {
						CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;

						if (itValueData != valueData) {
							if (!isDataValueExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(itValueData,depTrackPoint,exclusionType,false);
							}
						}

						++it;
						if (it != itEnd) {
							CDatatypeStringValueSpaceMapData& nextMapData = it.value();
							CDatatypeStringValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData,exclusionType)) {
								itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,exclusionType,false);
								nextValueData = getDatatypeStringValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(nextValueData,depTrackPoint,exclusionType,false);
							}
						} else {

							if (!isRightDataIntervalExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(itValueData,depTrackPoint,exclusionType,false);
							}
						}
					}

					return newValuesPotentiallyExcluded;
				}



				bool CDatatypeStringValueSpaceMap::excludeAll(CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType) {
					bool newValuesPotentiallyExcluded = false;

					CDatatypeStringValueSpaceMap::iterator itBegin = CDatatypeStringValueSpaceMap::begin(), itEnd = CDatatypeStringValueSpaceMap::end();
					CDatatypeStringValueSpaceMap::iterator it = itBegin;

					while (it != itEnd) {
						CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;

						if (it == itBegin) {
							if (!isLeftDataIntervalExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(itValueData,depTrackPoint,exclusionType,false);
							}
							if (!isDataValueExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(itValueData,depTrackPoint,exclusionType,false);
							}
						}

						++it;
						if (it != itEnd) {
							CDatatypeStringValueSpaceMapData& nextMapData = it.value();
							CDatatypeStringValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData,exclusionType)) {
								itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,exclusionType,false);
								nextValueData = getDatatypeStringValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(nextValueData,depTrackPoint,exclusionType,false);
							}
							if (!isDataValueExcluded(nextValueData,exclusionType)) {
								nextValueData = getDatatypeStringValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(nextValueData,depTrackPoint,exclusionType,false);
							}


						} else {
							if (!isRightDataIntervalExcluded(itValueData,exclusionType)) {
								itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,exclusionType,false);
							}
						}

					}

					return newValuesPotentiallyExcluded;
				}






				bool CDatatypeStringValueSpaceMap::excludeInterval(CDataLiteralStringValue* leftValue, bool leftValueInclusive, CDataLiteralStringValue* rightValue, bool rightValueInclusive, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType) {
					bool newValuesPotentiallyExcluded = false;

					CDatatypeStringValueData* leftValueData = getDatatypeStringValueData(leftValue,true);
					CDatatypeStringValueData* rightValueData = getDatatypeStringValueData(rightValue,true);

					if (leftValueInclusive && !isDataValueExcluded(leftValueData,exclusionType)) {
						newValuesPotentiallyExcluded |= excludeDataValue(leftValueData,depTrackPoint,exclusionType,false);
					}

					if (leftValueInclusive && !isDataValueExcluded(rightValueData,exclusionType)) {
						newValuesPotentiallyExcluded |= excludeDataValue(rightValueData,depTrackPoint,exclusionType,false);
					}


					CDatatypeStringValueSpaceMapArranger mapValueArranger(leftValue);
					CDatatypeStringValueSpaceMap::iterator itBegin = CDatatypeStringValueSpaceMap::begin(), itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeStringValueSpaceMap::end();
					CDatatypeStringValueSpaceMap::iterator it = itLB;
					while (it != itEnd) {
						CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;

						++it;
						if (it != itEnd) {
							CDatatypeStringValueSpaceMapData& nextMapData = it.value();
							CDatatypeStringValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData,exclusionType)) {
								itValueData = getDatatypeStringValueData(itValueData->getValue(),mapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeRightDataInterval(itValueData,depTrackPoint,exclusionType,false);
								nextValueData = getDatatypeStringValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeLeftDataInterval(nextValueData,depTrackPoint,exclusionType,false);
							}

							if (nextValueData == rightValueData) {
								break;
							}

							if (!isDataValueExcluded(nextValueData,exclusionType)) {
								nextValueData = getDatatypeStringValueData(nextValueData->getValue(),nextMapData,true,nullptr);
								newValuesPotentiallyExcluded |= excludeDataValue(nextValueData,depTrackPoint,exclusionType,false);
							}
						}
					}

					return newValuesPotentiallyExcluded;
				}







				bool CDatatypeStringValueSpaceMap::countDataIntervalValues(CDatatypeStringValueData* leftValueExcluded, CDatatypeStringValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataIntervalValues(leftValueExcluded->getValue(),rightValueExcluded->getValue(),leftValueExcluded->getRightExclusionData(),counter);
				}

				bool CDatatypeStringValueSpaceMap::countDataIntervalValues(CDataLiteralStringValue* leftValueExcluded, CDatatypeStringValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataIntervalValues(leftValueExcluded,rightValueExcluded->getValue(),rightValueExcluded->getLeftExclusionData(),counter);
				}


				bool CDatatypeStringValueSpaceMap::countDataIntervalValues(CDatatypeStringValueData* leftValueExcluded, CDataLiteralStringValue* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataIntervalValues(leftValueExcluded->getValue(),rightValueExcluded,leftValueExcluded->getRightExclusionData(),counter);
				}


				bool CDatatypeStringValueSpaceMap::countDataIntervalValues(CDataLiteralStringValue* leftValueExcluded, CDataLiteralStringValue* rightValueExcluded, CDatatypeStringValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter) {
					bool counted = false;
					if (!exlcusionData->isEverythingExcluded()) {

						counted |= counter->incInfinite();

						CDatatypeValueSpaceStringValuesCounter* stringValuesCounter = dynamic_cast<CDatatypeValueSpaceStringValuesCounter*>(counter);
						if (stringValuesCounter) {
							if (!exlcusionData->areLanguageTagsExcluded()) {
								counted |= stringValuesCounter->incInfinite(CDatatypeValueSpaceStringValuesCounter::SVT_WITH_LANGUAG_TAG_COUNT);								
							}
							if (!exlcusionData->areOnlyLanguageTagsAllowed()) {
								counted |= stringValuesCounter->incInfinite(CDatatypeValueSpaceStringValuesCounter::SVT_WITHOUT_LANGUAG_TAG_COUNT);								
							}
						}
					}
					return counted;
				}




				bool CDatatypeStringValueSpaceMap::countDataValue(CDatatypeStringValueData* dataValue, CDatatypeValueSpaceValuesCounter* counter) {
					return countDataValue(dataValue->getValue(),dataValue->getValueExclusionData(),counter);
				}


				bool CDatatypeStringValueSpaceMap::countDataValue(CDataLiteralStringValue* value, CDatatypeStringValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter) {
					bool counted = false;
					if (!exlcusionData->isEverythingExcluded()) {

						if (value->isInfiniteLength()) {
							counted |= counter->incInfinite();
						} else {
							counted |= counter->incValueCount(1);
						}

						CDatatypeValueSpaceStringValuesCounter* stringValuesCounter = dynamic_cast<CDatatypeValueSpaceStringValuesCounter*>(counter);
						if (stringValuesCounter) {

							if (value->isInfiniteLength()) {
								if (!exlcusionData->areLanguageTagsExcluded()) {
									counted |= stringValuesCounter->incInfinite(CDatatypeValueSpaceStringValuesCounter::SVT_WITH_LANGUAG_TAG_COUNT);
								}
								if (!exlcusionData->areOnlyLanguageTagsAllowed()) {
									counted |= stringValuesCounter->incInfinite(CDatatypeValueSpaceStringValuesCounter::SVT_WITHOUT_LANGUAG_TAG_COUNT);								
								}
							} else {
								if (value->hasLanguageTag()) {
									counted |= stringValuesCounter->incValueCount(CDatatypeValueSpaceStringValuesCounter::SVT_WITH_LANGUAG_TAG_COUNT,1);
								} else {
									counted |= stringValuesCounter->incValueCount(CDatatypeValueSpaceStringValuesCounter::SVT_WITHOUT_LANGUAG_TAG_COUNT,1);
								}
							}


						}

					}
					return counted;
				}




				bool CDatatypeStringValueSpaceMap::countValueValues(CDataLiteralStringValue* value, CDatatypeValueSpaceValuesCounter* counter) {
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

					CDatatypeStringValueSpaceMapArranger mapValueArranger(value);
					CDatatypeStringValueSpaceMap::iterator itBegin = CDatatypeStringValueSpaceMap::begin(), itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeStringValueSpaceMap::end();
					CDatatypeStringValueSpaceMap::iterator it = itLB;


					if (it != itEnd) {
						CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;

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







				bool CDatatypeStringValueSpaceMap::addIntervalExclusionDependencies(CDataLiteralStringValue* leftValue, bool leftValueInclusive, CDataLiteralStringValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceDependencyCollector* depCollector) {
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

					CDatatypeStringValueSpaceMapArranger mapValueArranger(leftValue);
					CDatatypeStringValueSpaceMap::iterator itBegin = CDatatypeStringValueSpaceMap::begin(), itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeStringValueSpaceMap::end();
					CDatatypeStringValueSpaceMap::iterator it = itLB;


					while (it != itEnd) {
						CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;

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
							CDatatypeStringValueSpaceMapData& nextMapData = it.value();
							CDatatypeStringValueData* nextValueData = nextMapData.mUseValue;

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





				bool CDatatypeStringValueSpaceMap::addValueExclusionDependencies(CDataLiteralStringValue* value, CDatatypeValueSpaceDependencyCollector* depCollector) {
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

					CDatatypeStringValueSpaceMapArranger mapValueArranger(value);
					CDatatypeStringValueSpaceMap::iterator itBegin = CDatatypeStringValueSpaceMap::begin(), itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeStringValueSpaceMap::end();
					CDatatypeStringValueSpaceMap::iterator it = itLB;


					if (it != itEnd) {
						CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;

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





				bool CDatatypeStringValueSpaceMap::countAllValues(CDatatypeValueSpaceValuesCounter* counter) {
					if (mAbsoluteMaximumExclusionValue && mAbsoluteMinimumExclusionValue) {
						return countIntervalValues(mAbsoluteMinimumExclusionValue->getValue(),true,mAbsoluteMaximumExclusionValue->getValue(),true,counter);
					}
					return false;
				}



				bool CDatatypeStringValueSpaceMap::countIntervalValues(CDataLiteralStringValue* leftValue, bool leftValueInclusive, CDataLiteralStringValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceValuesCounter* counter) {
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



					CDatatypeStringValueSpaceMapArranger mapValueArranger(leftValue);
					CDatatypeStringValueSpaceMap::iterator itBegin = CDatatypeStringValueSpaceMap::begin(), itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeStringValueSpaceMap::end();
					CDatatypeStringValueSpaceMap::iterator it = itLB;


					while (it != itEnd) {
						CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;

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
							CDatatypeStringValueSpaceMapData& nextMapData = it.value();
							CDatatypeStringValueData* nextValueData = nextMapData.mUseValue;

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











				bool CDatatypeStringValueSpaceMap::excludeData(CDatatypeStringValueDataExclusion* exlcusionData, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType, bool force) {
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


				bool CDatatypeStringValueSpaceMap::excludeDataValue(CDatatypeStringValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType, bool force) {
					return excludeData(dataValue->getValueExclusionData(),depTrackPoint,exclusionType,force);
				}

				bool CDatatypeStringValueSpaceMap::excludeLeftDataInterval(CDatatypeStringValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType, bool force) {
					return excludeData(dataValue->getLeftExclusionData(),depTrackPoint,exclusionType,force);
				}

				bool CDatatypeStringValueSpaceMap::excludeRightDataInterval(CDatatypeStringValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType, bool force) {
					return excludeData(dataValue->getRightExclusionData(),depTrackPoint,exclusionType,force);
				}



				bool CDatatypeStringValueSpaceMap::restrictToWithLanguageTagsValues(CDependencyTrackPoint* depTrackPoint) {
					CDatatypeStringValueExclusionType exclusionType(CDatatypeStringValueExclusionType::SVET_LANGUAGE_TAGS_COMPLETELY,true);
					return excludeAll(depTrackPoint,&exclusionType);
				}


				bool CDatatypeStringValueSpaceMap::restrictToWithoutLanguageTagsValues(CDependencyTrackPoint* depTrackPoint) {
					CDatatypeStringValueExclusionType exclusionType(CDatatypeStringValueExclusionType::SVET_LANGUAGE_TAGS_COMPLETELY,false);
					return excludeAll(depTrackPoint,&exclusionType);
				}






				bool CDatatypeStringValueSpaceMap::testValueSpaceReturnClashed() {

					CDatatypeStringValueSpaceMap::const_iterator itBegin = CDatatypeStringValueSpaceMap::constBegin(), itEnd = CDatatypeStringValueSpaceMap::constEnd();
					CDatatypeStringValueSpaceMap::const_iterator it = itBegin;

					// test whether value space is empty
					CDatatypeStringValueData* firstValueData = nullptr;
					
					while (it != itEnd) {

						const CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;
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
							const CDatatypeStringValueSpaceMapData& nextMapData = it.value();
							CDatatypeStringValueData* nextValueData = nextMapData.mUseValue;
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




				bool CDatatypeStringValueSpaceMap::addValueSpaceDependencies(CDatatypeValueSpaceDependencyCollector* depCollector) {

					CDatatypeStringValueSpaceMap::const_iterator itBegin = CDatatypeStringValueSpaceMap::constBegin(), itEnd = CDatatypeStringValueSpaceMap::constEnd();
					CDatatypeStringValueSpaceMap::const_iterator it = itBegin;

					it = itBegin;
					while (it != itEnd) {
						const CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;
						addDataValueExclusionDependencies(itValueData,depCollector);

						if (it == itBegin) {
							addLeftIntervalExclusionDependencies(itValueData,depCollector);
						}

						++it;
						if (it == itEnd) {
							addRightIntervalExclusionDependencies(itValueData,depCollector);
						} else {
							const CDatatypeStringValueSpaceMapData& nextMapData = it.value();
							CDatatypeStringValueData* nextValueData = nextMapData.mUseValue;

							addDataIntervalExclusionDependencies(itValueData,nextValueData,depCollector);

						}
					}
					return true;

				}







				bool CDatatypeStringValueSpaceMap::restrictToValue(CDataLiteralStringValue* value, CDependencyTrackPoint* depTrackPoint) {
					bool newValuesPotentiallyExcluded = false;
					newValuesPotentiallyExcluded |= excludeMinimum(value,false,depTrackPoint);
					newValuesPotentiallyExcluded |= excludeMaximum(value,false,depTrackPoint);
					return newValuesPotentiallyExcluded;
				}



				bool CDatatypeStringValueSpaceMap::excludeValue(CDataLiteralStringValue* value, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType) {
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


					CDatatypeStringValueData* valueData = getDatatypeStringValueData(value,true);
					if (!isDataValueExcluded(valueData,exclusionType)) {
						newValuesPotentiallyExcluded |= excludeDataValue(valueData,depTrackPoint,exclusionType,false);
					}


					return newValuesPotentiallyExcluded;
				}







				bool CDatatypeStringValueSpaceMap::addDataValueExclusionDependencies(CDatatypeStringValueData* dataValue, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeStringValueDataExclusion* exclusionData = dataValue->getValueExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}

				bool CDatatypeStringValueSpaceMap::addDataIntervalExclusionDependencies(CDatatypeStringValueData* leftValueExcluded, CDatatypeStringValueData* rightValueExcluded, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeStringValueDataExclusion* exclusionData = leftValueExcluded->getRightExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}

				bool CDatatypeStringValueSpaceMap::addLeftIntervalExclusionDependencies(CDatatypeStringValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeStringValueDataExclusion* exclusionData = valueData->getLeftExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}

				bool CDatatypeStringValueSpaceMap::addRightIntervalExclusionDependencies(CDatatypeStringValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector) {
					CDatatypeStringValueDataExclusion* exclusionData = valueData->getRightExclusionData();
					return addExclusionDependencies(exclusionData,depCollector);
				}




				bool CDatatypeStringValueSpaceMap::addExclusionDependencies(CDatatypeStringValueDataExclusion* exclusionData, CDatatypeValueSpaceDependencyCollector* depCollector) {
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












				bool CDatatypeStringValueSpaceMap::isLeftDataIntervalExcluded(CDatatypeStringValueData* dataValue, CDatatypeStringValueExclusionType* exclusionType) {
					CDatatypeStringValueDataExclusion* exclusionData = dataValue->getLeftExclusionData();
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


				bool CDatatypeStringValueSpaceMap::isRightDataIntervalExcluded(CDatatypeStringValueData* dataValue, CDatatypeStringValueExclusionType* exclusionType) {
					CDatatypeStringValueDataExclusion* exclusionData = dataValue->getRightExclusionData();
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


				bool CDatatypeStringValueSpaceMap::isDataIntervalExcluded(CDataLiteralStringValue* leftValueExcluded, CDatatypeStringValueData* rightValueExcluded, CDatatypeStringValueExclusionType* exclusionType) {
					CDatatypeStringValueDataExclusion* exclusionData = rightValueExcluded->getLeftExclusionData();
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


				bool CDatatypeStringValueSpaceMap::isDataIntervalExcluded(CDatatypeStringValueData* leftValueExcluded, CDataLiteralStringValue* rightValueExcluded, CDatatypeStringValueExclusionType* exclusionType) {
					CDatatypeStringValueDataExclusion* exclusionData = leftValueExcluded->getRightExclusionData();
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


				bool CDatatypeStringValueSpaceMap::isDataIntervalExcluded(CDatatypeStringValueData* leftValueExcluded, CDatatypeStringValueData* rightValueExcluded, CDatatypeStringValueExclusionType* exclusionType) {
					return isDataIntervalExcluded(leftValueExcluded,rightValueExcluded->getValue(),exclusionType);
				}





				bool CDatatypeStringValueSpaceMap::isDataValueExcluded(CDatatypeStringValueData* valueData, CDatatypeStringValueExclusionType* exclusionType) {
					CDatatypeStringValueDataExclusion* exclusionData = valueData->getValueExclusionData();
					if (exclusionData->isEverythingExcluded()) {
						return true;
					}
					CDataLiteralStringValue* value = valueData->getValue();
					if (value->hasLanguageTag()) {
						if (exclusionData->areLanguageTagsExcluded()) {
							return true;
						}
					} else {
						if (exclusionData->areOnlyLanguageTagsAllowed()) {
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


				bool CDatatypeStringValueSpaceMap::isDataValueExcluded(CDataLiteralStringValue* value, CDatatypeStringValueExclusionType* exclusionType) {
					CDatatypeStringValueSpaceMapArranger mapValueArranger(value);
					CDatatypeStringValueSpaceMap::const_iterator itBegin = CDatatypeStringValueSpaceMap::constBegin(), itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeStringValueSpaceMap::constEnd();

					if (itBegin == itEnd) {
						return false;
					}
					if (itLB == itEnd) {
						return true;
					}
					const CDatatypeStringValueSpaceMapData& mapData = itLB.value();
					CDatatypeStringValueData* valueData = mapData.mUseValue;
					CDatatypeStringValueDataExclusion* exclusionData = nullptr;
					if (valueData->getValue()->isEqualTo(value)) {
						exclusionData = valueData->getValueExclusionData();
					} else if (valueData->getValue()->isLessThan(value)) {			
						exclusionData = valueData->getLeftExclusionData();
					} else {			
						exclusionData = valueData->getRightExclusionData();
					}

					if (exclusionData) {
						CDatatypeStringValueDataExclusion* exclusionData = valueData->getValueExclusionData();
						if (exclusionData->isEverythingExcluded()) {
							return true;
						}
						CDataLiteralStringValue* value = valueData->getValue();
						if (value->hasLanguageTag()) {
							if (exclusionData->areLanguageTagsExcluded()) {
								return true;
							}
						} else {
							if (exclusionData->areOnlyLanguageTagsAllowed()) {
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



				bool CDatatypeStringValueSpaceMap::adaptNewStringValueDataToNeighbours(CDatatypeStringValueData* valueData) {
					CDatatypeStringValueSpaceMapArranger mapValueArranger(valueData->getValue());
					CDatatypeStringValueSpaceMap::const_iterator itBegin = CDatatypeStringValueSpaceMap::constBegin(), itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeStringValueSpaceMap::constEnd();
					CDatatypeStringValueSpaceMap::const_iterator itAdapt = itLB;
					bool rightNeighbour = true;
					++itAdapt;
					if (itAdapt == itEnd) {
						itAdapt = itLB;
						--itAdapt;
						rightNeighbour = false;
					}
					if (itAdapt != itEnd) {
						const CDatatypeStringValueSpaceMapData& adaptMapData = itAdapt.value();
						CDatatypeStringValueData* adaptValueData = adaptMapData.mUseValue;
						CDatatypeStringValueDataExclusion* exclusionData = nullptr;
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


				CDatatypeStringValueData* CDatatypeStringValueSpaceMap::getDatatypeStringValueData(CDataLiteralStringValue* value, CDatatypeStringValueSpaceMapData& valueMapData, bool createOrLocalize, bool* newValueInsertion) {
					if (newValueInsertion) {
						*newValueInsertion = false;
					}
					CDatatypeStringValueData* stringValueData = nullptr;
					if (!valueMapData.mLocValue) {
						bool newlyCreated = false;
						CMemoryAllocationManager* taskMemMan = mProcessContext->getUsedMemoryAllocationManager();
						stringValueData = CObjectParameterizingAllocator< CDatatypeStringValueData,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,mProcessContext);
						stringValueData->initStringValueData(valueMapData.mUseValue,value);
						if (!valueMapData.mUseValue) {
							newlyCreated = true;
						}
						valueMapData.mUseValue = valueMapData.mLocValue = stringValueData;
						if (newlyCreated) {
							adaptNewStringValueDataToNeighbours(stringValueData);
						}
						if (newlyCreated && newValueInsertion) {
							*newValueInsertion = true;
						}
					} else {
						stringValueData = valueMapData.mUseValue;
					}
					return stringValueData;
				}



				CDatatypeStringValueData* CDatatypeStringValueSpaceMap::getDatatypeStringValueData(CDataLiteralStringValue* value, bool createOrLocalize, bool* newValueInsertion) {
					if (newValueInsertion) {
						*newValueInsertion = false;
					}
					CDatatypeStringValueData* stringValueData = nullptr;
					CDatatypeStringValueSpaceMapArranger mapValueArranger(value);
					if (createOrLocalize) {
						CDatatypeStringValueSpaceMapData& data = CPROCESSMAP<CDatatypeStringValueSpaceMapArranger,CDatatypeStringValueSpaceMapData>::operator[](mapValueArranger);
						stringValueData = getDatatypeStringValueData(value,data,createOrLocalize,newValueInsertion);
					} else {
						CDatatypeStringValueSpaceMapData data = CPROCESSMAP<CDatatypeStringValueSpaceMapArranger,CDatatypeStringValueSpaceMapData>::value(mapValueArranger);
						stringValueData = data.mUseValue;
					}
					return stringValueData;
				}


				bool CDatatypeStringValueSpaceMap::hasExcludedMinimum(CDataLiteralStringValue* value, bool valueInclusively, CDatatypeStringValueExclusionType* exclusionType) {

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

					CDatatypeStringValueSpaceMapArranger mapValueArranger(value);
					CDatatypeStringValueSpaceMap::const_iterator itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), it = CDatatypeStringValueSpaceMap::constBegin(), itEnd = CDatatypeStringValueSpaceMap::constEnd();
					if (it == itEnd) {
						// no restrictions in value map
						return false;
					}
					if (it == itLB) {
						const CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* stringValueData = mapData.mUseValue;
						if (!valueInclusively) {
							// value is before first value --> excluded
							return isLeftDataIntervalExcluded(stringValueData,exclusionType);
						} else {
							if (!isDataValueExcluded(stringValueData,exclusionType)) {
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
						const CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* stringValueData = mapData.mUseValue;
						if (!isDataValueExcluded(stringValueData,exclusionType)) {
							return false;
						}
						++it;

						if (it != itEnd) {
							const CDatatypeStringValueSpaceMapData& nextMapData = it.value();
							CDatatypeStringValueData* nextStringValueData = nextMapData.mUseValue;
							if (it != itLB) {
								if (!isDataIntervalExcluded(stringValueData,nextStringValueData,exclusionType)) {
									return false;
								}
							} else {
								if (nextStringValueData->getValue()->isEqualTo(value)) {
									// value is in the value space map
									if (!isDataIntervalExcluded(stringValueData,nextStringValueData,exclusionType)) {
										return false;
									}
									if (valueInclusively && !isDataValueExcluded(nextStringValueData,exclusionType)) {
										return false;
									}
								} else {
									// test only to the tested minimum value limit
									if (!isDataIntervalExcluded(stringValueData,value,exclusionType)) {
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



				bool CDatatypeStringValueSpaceMap::hasExcludedMaximum(CDataLiteralStringValue* value, bool valueInclusively, CDatatypeStringValueExclusionType* exclusionType) {

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

					CDatatypeStringValueSpaceMapArranger mapValueArranger(value);
					CDatatypeStringValueSpaceMap::const_iterator itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), it = CDatatypeStringValueSpaceMap::constBegin(), itEnd = CDatatypeStringValueSpaceMap::constEnd();
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
						const CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* stringValueData = mapData.mUseValue;
						if (it == itLB) {
							if (stringValueData->getValue()->isEqualTo(value)) {
								if (valueInclusively && !isDataValueExcluded(stringValueData,exclusionType)) {
									return false;
								}
							} else {
								if (!isDataIntervalExcluded(value,stringValueData,exclusionType)) {
									return false;
								}
								if (!isDataValueExcluded(stringValueData,exclusionType)) {
									return false;
								}
								if (valueInclusively && !isDataValueExcluded(value,exclusionType)) {
									return false;
								}
							}
						}
						++it;
						if (it != itEnd) {
							const CDatatypeStringValueSpaceMapData& nextMapData = it.value();
							CDatatypeStringValueData* nextStringValueData = nextMapData.mUseValue;

							if (!isDataValueExcluded(nextStringValueData,exclusionType)) {
								return false;
							}
							if (!isDataIntervalExcluded(stringValueData,nextStringValueData,exclusionType)) {
								return false;
							}
						} else {
							if (!isRightDataIntervalExcluded(stringValueData,exclusionType)) {
								return false;
							}
						}
					}
					return true;
				}


				bool CDatatypeStringValueSpaceMap::isValueExcluded(CDataLiteralStringValue* value, CDatatypeStringValueExclusionType* exclusionType) {
					if (isDataValueExcluded(value,exclusionType)) {
						return true;
					}
					return false;
				}



				bool CDatatypeStringValueSpaceMap::getNextPossibleDataValue(CDataLiteralStringValue* nextValue, CDataLiteralStringValue* lastValue) {

					if (!lastValue) {
						if (!mAbsoluteMinimumExclusionValue) {
							return false;
						}
						lastValue = mAbsoluteMinimumExclusionValue->getValue();
						if (!isDataValueExcluded(mAbsoluteMinimumExclusionValue,nullptr)) {
							nextValue->initValue(lastValue);
							return true;
						}
					}


					CDatatypeStringValueSpaceMapArranger mapValueArranger(lastValue);
					CDatatypeStringValueSpaceMap::iterator itBegin = CDatatypeStringValueSpaceMap::begin(), itLB = CDatatypeStringValueSpaceMap::lowerBound(mapValueArranger), itEnd = CDatatypeStringValueSpaceMap::end();
					CDatatypeStringValueSpaceMap::iterator it = itLB;
					while (it != itEnd) {

						if (it == itLB) {
							CDatatypeStringValueData* itValueData = it.value().mUseValue;
							while (lastValue->isLessThan(itValueData->getValue())) {
								CDatatypeStringValueSpaceMap::iterator itLast = it;
								--itLast;
								if (itLast != itEnd) {
									--it;
								} else {
									if (!isLeftDataIntervalExcluded(itValueData,nullptr)) {
										return false;
									}
									break;
								}
							}
						}
						CDatatypeStringValueSpaceMapData& mapData = it.value();
						CDatatypeStringValueData* itValueData = mapData.mUseValue;

						if (itValueData->getValue()->isGreaterThan(lastValue)) {
							lastValue = itValueData->getValue();
						}

						++it;
						if (it != itEnd) {
							CDatatypeStringValueSpaceMapData& nextMapData = it.value();
							CDatatypeStringValueData* nextValueData = nextMapData.mUseValue;

							if (!isDataIntervalExcluded(itValueData,nextValueData,nullptr)) {
								return false;

							} else if (!isDataValueExcluded(nextValueData,nullptr)) {
								nextValue->initValue(nextValueData->getValue());
								return true;
							}
						} else {
							return false;
						}

					}
					return false;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
