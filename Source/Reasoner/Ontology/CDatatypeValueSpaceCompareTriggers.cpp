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

#include "CDatatypeValueSpaceCompareTriggers.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDatatypeValueSpaceCompareTriggers::CDatatypeValueSpaceCompareTriggers(CContext* boxContext) : CDatatypeValueSpaceTriggers(boxContext),mValueTriggerMap(boxContext) {
			}



			CDatatypeValueSpaceCompareTriggers* CDatatypeValueSpaceCompareTriggers::initValueSpaceCompareTriggers(CDatatypeValueSpaceCompareTriggers* data) {
				initValueSpaceTriggers(data);
				if (data) {
					mValueTriggerMap.initValueSpaceTriggeringMap(&data->mValueTriggerMap);
				} else {
					mValueTriggerMap.initValueSpaceTriggeringMap(nullptr);
				}
				return this;
			}



			CDatatypeValueSpaceCompareTriggers* CDatatypeValueSpaceCompareTriggers::addValueConceptTrigger(CDataLiteralCompareValue* value, CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger) {
				incConceptTriggerCount(conceptTrigger->getCount());
				CDatatypeValueSpaceConceptTriggeringData* valueConceptTriggerData = getValueConceptTriggeringData(value);
				valueConceptTriggerData->appendPartialConceptTriggers(conceptTrigger);
				return this;
			}


			CDatatypeValueSpaceCompareTriggers* CDatatypeValueSpaceCompareTriggers::addMinValueConceptTrigger(CDataLiteralCompareValue* value, bool inclusive, CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger) {
				incConceptTriggerCount(conceptTrigger->getCount());
				CDatatypeValueSpaceConceptTriggeringData* minValueConceptTriggerData = getMinValueConceptTriggeringData(value,inclusive);
				minValueConceptTriggerData->appendPartialConceptTriggers(conceptTrigger);
				return this;
			}

			CDatatypeValueSpaceCompareTriggers* CDatatypeValueSpaceCompareTriggers::addMaxValueConceptTrigger(CDataLiteralCompareValue* value, bool inclusive, CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger) {
				incConceptTriggerCount(conceptTrigger->getCount());
				CDatatypeValueSpaceConceptTriggeringData* maxValueConceptTriggerData = getMaxValueConceptTriggeringData(value,inclusive);
				maxValueConceptTriggerData->appendPartialConceptTriggers(conceptTrigger);
				return this;
			}



			CDatatypeValueSpaceCompareTriggers* CDatatypeValueSpaceCompareTriggers::addMinMaxValueConceptTrigger(CDataLiteralCompareValue* value, bool minRestriction, bool inclusive, CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger) {
				if (minRestriction) {
					return addMinValueConceptTrigger(value,inclusive,conceptTrigger);
				} else {
					return addMaxValueConceptTrigger(value,inclusive,conceptTrigger);
				}
			}



			CDatatypeValueSpaceTriggeringMap* CDatatypeValueSpaceCompareTriggers::getValueSpaceTriggeringMap() {
				return &mValueTriggerMap;
			}


			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceCompareTriggers::getValueConceptTriggeringData(CDataLiteralCompareValue* value, bool createOrLocalize) {
				CDatatypeValueSpaceConceptTriggeringData* conTriggerData = nullptr;
				CDatatypeValueSpaceTriggeringData* valueSpaceTriggerData = mValueTriggerMap.getDatatypeValueTriggeringData(value,createOrLocalize);
				if (valueSpaceTriggerData) {
					conTriggerData = valueSpaceTriggerData->getDirectValueTriggeringData();
				}
				return conTriggerData;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceCompareTriggers::getMinValueConceptTriggeringData(CDataLiteralCompareValue* value, bool inclusive, bool createOrLocalize) {
				CDatatypeValueSpaceConceptTriggeringData* conTriggerData = nullptr;
				CDatatypeValueSpaceTriggeringData* valueSpaceTriggerData = mValueTriggerMap.getDatatypeValueTriggeringData(value,createOrLocalize);
				if (valueSpaceTriggerData) {
					conTriggerData = valueSpaceTriggerData->getTriggeringData(true,inclusive);
				}
				return conTriggerData;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceCompareTriggers::getMaxValueConceptTriggeringData(CDataLiteralCompareValue* value, bool inclusive, bool createOrLocalize) {
				CDatatypeValueSpaceConceptTriggeringData* conTriggerData = nullptr;
				CDatatypeValueSpaceTriggeringData* valueSpaceTriggerData = mValueTriggerMap.getDatatypeValueTriggeringData(value,createOrLocalize);
				if (valueSpaceTriggerData) {
					conTriggerData = valueSpaceTriggerData->getTriggeringData(false,inclusive);
				}
				return conTriggerData;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
