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

#include "CDatatypeValueSpaceTriggeringMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDatatypeValueSpaceTriggeringMap::CDatatypeValueSpaceTriggeringMap(CBoxContext* boxContext) : CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>(boxContext),mBoxContext(boxContext) {
			}


			CDatatypeValueSpaceTriggeringMap* CDatatypeValueSpaceTriggeringMap::initValueSpaceTriggeringMap(CDatatypeValueSpaceTriggeringMap* map) {
				if (map) {
					*this = *map;
				} else {
					CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::clear();
				}
				return this;
			}




			CDatatypeValueSpaceTriggeringData* CDatatypeValueSpaceTriggeringMap::getDatatypeValueTriggeringData(CDataLiteralCompareValue* value, CDatatypeValueSpaceTriggeringMapData& valueMapData, bool createOrLocalize) {
				CDatatypeValueSpaceTriggeringData* valueData = nullptr;
				if (!valueMapData.mLocValue) {

					valueData = CObjectAllocator< CDatatypeValueSpaceTriggeringData >::allocateAndConstruct(mBoxContext->getMemoryAllocationManager());
					valueData->initValueTriggeringData(valueMapData.mUseValue,value);
					valueMapData.mUseValue = valueMapData.mLocValue = valueData;

				} else {
					valueData = valueMapData.mUseValue;
				}
				return valueData;
			}



			CDatatypeValueSpaceTriggeringData* CDatatypeValueSpaceTriggeringMap::getDatatypeValueTriggeringData(CDataLiteralCompareValue* value, bool createOrLocalize) {
				CDatatypeValueSpaceTriggeringData* valueData = nullptr;
				CDatatypeValueSpaceTriggeringMapArranger mapValueArranger(value);
				if (createOrLocalize) {
					CDatatypeValueSpaceTriggeringMapData& data = CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::operator[](mapValueArranger);
					valueData = getDatatypeValueTriggeringData(value,data,createOrLocalize);
				} else {
					CDatatypeValueSpaceTriggeringMapData data = CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::value(mapValueArranger);
					valueData = data.mUseValue;
				}
				return valueData;
			}




			CDatatypeValueSpaceTriggeringIterator CDatatypeValueSpaceTriggeringMap::getLeftTriggeringIterator(CDataLiteralCompareValue* value, bool inclusive) {
				CDatatypeValueSpaceTriggeringMapArranger mapValueArranger(value);
				CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator itLB = CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::lowerBound(mapValueArranger), itEnd = CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::constEnd();
				if (itLB != itEnd) {
					const CDatatypeValueSpaceTriggeringMapData& mapData = itLB.value();
					CDatatypeValueSpaceTriggeringData* triggerData = mapData.mUseValue;
					if (triggerData->getValue()->isEqualTo(value) && !inclusive) {
						--itLB;
					} else if (triggerData->getValue()->isGreaterThan(value)) {
						--itLB;
					}
					return CDatatypeValueSpaceTriggeringIterator(itLB,itEnd,false);
				} else {
					CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator itBegin = CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::constBegin();
					return CDatatypeValueSpaceTriggeringIterator(itBegin,itEnd,true);
				}
			}



			CDatatypeValueSpaceTriggeringIterator CDatatypeValueSpaceTriggeringMap::getRightTriggeringIterator(CDataLiteralCompareValue* value, bool inclusive) {
				CDatatypeValueSpaceTriggeringMapArranger mapValueArranger(value);
				CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator itLB = CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::lowerBound(mapValueArranger), itEnd = CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::constEnd();
				if (itLB != itEnd) {
					const CDatatypeValueSpaceTriggeringMapData& mapData = itLB.value();
					CDatatypeValueSpaceTriggeringData* triggerData = mapData.mUseValue;
					if (triggerData->getValue()->isEqualTo(value) && !inclusive) {
						++itLB;
					} else if (triggerData->getValue()->isLessThan(value)) {
						++itLB;
					}
				}
				return CDatatypeValueSpaceTriggeringIterator(itLB,itEnd,true);
			}


			CDatatypeValueSpaceTriggeringIterator CDatatypeValueSpaceTriggeringMap::getTriggeringIterator(CDataLiteralCompareValue* value, bool left, bool inclusive) {
				if (left) {
					return getLeftTriggeringIterator(value,inclusive);
				} else {
					return getRightTriggeringIterator(value,inclusive);
				}
			}



			CDatatypeValueSpaceTriggeringIterator CDatatypeValueSpaceTriggeringMap::getIntervalTriggeringIterator(CDataLiteralCompareValue* minValue, bool minInclusive, CDataLiteralCompareValue* maxValue, bool maxInclusive, bool fromMinToMax) {
				CDatatypeValueSpaceTriggeringMapArranger mapValueArrangerMin(minValue);
				CDatatypeValueSpaceTriggeringMapArranger mapValueArrangerMax(maxValue);
				CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator itEnd = CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::constEnd(),
					minItLB = CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::lowerBound(mapValueArrangerMin), 
					maxItLB = CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::lowerBound(mapValueArrangerMax);
					
				if (maxItLB != itEnd) {
					const CDatatypeValueSpaceTriggeringMapData& mapData = maxItLB.value();
					CDatatypeValueSpaceTriggeringData* triggerData = mapData.mUseValue;
					bool moveToPrev = false;
					if (triggerData->getValue()->isEqualTo(maxValue) && !maxInclusive) {
						moveToPrev = true;
					} else if (triggerData->getValue()->isGreaterThan(maxValue)) {
						moveToPrev = true;
					}
					if (moveToPrev) {
						if (maxItLB != minItLB) {
							--maxItLB;
						} else {
							maxItLB = itEnd;
							minItLB = itEnd;
						}
					}
				}
				if (minItLB != itEnd && maxItLB != minItLB) {
					const CDatatypeValueSpaceTriggeringMapData& mapData = minItLB.value();
					CDatatypeValueSpaceTriggeringData* triggerData = mapData.mUseValue;
					bool moveToNext = false;
					if (triggerData->getValue()->isEqualTo(minValue) && !minInclusive) {
						moveToNext = true;
					} else if (triggerData->getValue()->isLessThan(minValue)) {
						moveToNext = true;
					}
					if (moveToNext) {
						if (maxItLB != minItLB) {
							++minItLB;
						} else {
							maxItLB = itEnd;
							minItLB = itEnd;
						}
					}
				}

				if (fromMinToMax) {
					return CDatatypeValueSpaceTriggeringIterator(minItLB,maxItLB,itEnd,true);
				} else {
					return CDatatypeValueSpaceTriggeringIterator(maxItLB,minItLB,itEnd,false);
				}
			}



			bool CDatatypeValueSpaceTriggeringMap::countIntervalMinMaxValueTriggers(CDataLiteralCompareValue* minValue, bool minInclusive, CDataLiteralCompareValue* maxValue, bool maxInclusive, cint64& minTriggerCount, cint64& maxTriggerCount, cint64& valueTriggerCount) {
				CDatatypeValueSpaceTriggeringIterator iterator = getIntervalTriggeringIterator(minValue,minInclusive,maxValue,maxInclusive,true);
				return countIntervalMinMaxValueTriggers(iterator,minTriggerCount,maxTriggerCount,valueTriggerCount);
			}


			bool CDatatypeValueSpaceTriggeringMap::countIntervalValueTriggers(CDataLiteralCompareValue* minValue, bool minInclusive, CDataLiteralCompareValue* maxValue, bool maxInclusive, cint64& valueTriggerCount) {
				cint64 minTriggerCount = 0;
				cint64 maxTriggerCount = 0;
				CDatatypeValueSpaceTriggeringIterator iterator = getIntervalTriggeringIterator(minValue,minInclusive,maxValue,maxInclusive,true);
				return countIntervalMinMaxValueTriggers(iterator,minTriggerCount,maxTriggerCount,valueTriggerCount);
			}



			bool CDatatypeValueSpaceTriggeringMap::countIntervalMinMaxValueTriggers(CDatatypeValueSpaceTriggeringIterator iterator, cint64& minTriggerCount, cint64& maxTriggerCount, cint64& valueTriggerCount) {
				minTriggerCount = 0;
				maxTriggerCount = 0;
				valueTriggerCount = 0;
				while (iterator.hasNext()) {
					CDatatypeValueSpaceTriggeringData* triggerData = iterator.next();
					if (triggerData->getDirectValueTriggeringData()->hasPartialConceptTriggers()) {
						++valueTriggerCount;
					}
					if (triggerData->getMaxExclusiveTriggeringData()->hasPartialConceptTriggers()) {
						++maxTriggerCount;
					}
					if (triggerData->getMaxInclusiveTriggeringData()->hasPartialConceptTriggers()) {
						++maxTriggerCount;
					}
					if (triggerData->getMinExclusiveTriggeringData()->hasPartialConceptTriggers()) {
						++minTriggerCount;
					}
					if (triggerData->getMinInclusiveTriggeringData()->hasPartialConceptTriggers()) {
						++minTriggerCount;
					}
				}
				return true;
			}



			bool CDatatypeValueSpaceTriggeringMap::getIntervalMinMaxTriggerFreeInterval(CDataLiteralCompareValue* minValue, bool minInclusive, CDataLiteralCompareValue* maxValue, bool maxInclusive,
					CDataLiteralCompareValue*& freeLeftValue, bool& freeLeftInclusive, CDataLiteralCompareValue*& freeRightValue, bool& freeRightInclusive) {

				freeRightValue = nullptr;
				freeLeftValue = nullptr;
				freeRightInclusive = false;
				freeLeftInclusive = false;

				CDatatypeValueSpaceTriggeringIterator iterator = getIntervalTriggeringIterator(minValue,minInclusive,maxValue,maxInclusive,true);
				while (iterator.hasNext()) {
					CDatatypeValueSpaceTriggeringData* triggerData = iterator.next();
					if (triggerData->getMaxExclusiveTriggeringData()->hasPartialConceptTriggers()) {
						freeLeftInclusive = true;
						freeLeftValue = triggerData->getValue();
					}
					if (triggerData->getMaxInclusiveTriggeringData()->hasPartialConceptTriggers()) {
						freeLeftInclusive = false;
						freeLeftValue = triggerData->getValue();
					}
					if (!freeRightValue) {
						if (triggerData->getMinExclusiveTriggeringData()->hasPartialConceptTriggers()) {
							freeRightInclusive = true;
							freeRightValue = triggerData->getValue();
						}
						if (triggerData->getMinInclusiveTriggeringData()->hasPartialConceptTriggers()) {
							freeRightInclusive = false;
							freeRightValue = triggerData->getValue();
						}
					}
				}

				if (freeRightValue == nullptr) {
					freeRightValue = maxValue;
					freeRightInclusive = maxInclusive;
				}
				if (freeLeftValue == nullptr) {
					freeLeftValue = minValue;
					freeLeftInclusive = minInclusive;
				}

				if (freeLeftValue->isGreaterThan(freeRightValue)) {
					return false;
				} else if (freeLeftValue->isEqualTo(freeRightValue)) {
					if (!freeLeftInclusive || !freeRightInclusive) {
						return false;
					}
				}
				return true;
			}




		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
