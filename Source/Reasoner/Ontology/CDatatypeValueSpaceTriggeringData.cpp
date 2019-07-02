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

#include "CDatatypeValueSpaceTriggeringData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDatatypeValueSpaceTriggeringData::CDatatypeValueSpaceTriggeringData() {
				mValue = nullptr;

			}



			CDatatypeValueSpaceTriggeringData* CDatatypeValueSpaceTriggeringData::initValueTriggeringData(CDatatypeValueSpaceTriggeringData* valueData, CDataLiteralCompareValue* value) {
				mValue = value;
				if (valueData) {
					mValueTriggerData.initConceptTriggeringData(&valueData->mValueTriggerData);
					mMinInclusiveTriggerData.initConceptTriggeringData(&valueData->mMinInclusiveTriggerData);
					mMinExclusiveTriggerData.initConceptTriggeringData(&valueData->mMinExclusiveTriggerData);
					mMaxInclusiveTriggerData.initConceptTriggeringData(&valueData->mMaxInclusiveTriggerData);
					mMaxExclusiveTriggerData.initConceptTriggeringData(&valueData->mMaxExclusiveTriggerData);
				} else {
					mValueTriggerData.initConceptTriggeringData(nullptr);
					mMinInclusiveTriggerData.initConceptTriggeringData(nullptr);
					mMinExclusiveTriggerData.initConceptTriggeringData(nullptr);
					mMaxInclusiveTriggerData.initConceptTriggeringData(nullptr);
					mMaxExclusiveTriggerData.initConceptTriggeringData(nullptr);
				}
				return this;
			}




			CDataLiteralCompareValue* CDatatypeValueSpaceTriggeringData::getValue() {
				return mValue;
			}


			CDatatypeValueSpaceTriggeringData* CDatatypeValueSpaceTriggeringData::setValue(CDataLiteralCompareValue* value) {
				mValue = value;
				return this;
			}


			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceTriggeringData::getDirectValueTriggeringData() {
				return &mValueTriggerData;
			}


			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceTriggeringData::getMinInclusiveTriggeringData() {
				return &mMinInclusiveTriggerData;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceTriggeringData::getMinExclusiveTriggeringData() {
				return &mMinExclusiveTriggerData;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceTriggeringData::getMaxInclusiveTriggeringData() {
				return &mMaxInclusiveTriggerData;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceTriggeringData::getMaxExclusiveTriggeringData() {
				return &mMaxExclusiveTriggerData;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceTriggeringData::getTriggeringData(bool min, bool inclusive) {
				if (min && inclusive) {
					return getMinInclusiveTriggeringData();
				} else if (min && !inclusive) {
					return getMinExclusiveTriggeringData();
				} else if (!min && inclusive) {
					return getMaxInclusiveTriggeringData();
				} else {
					return getMaxExclusiveTriggeringData();
				}
			}


			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceTriggeringData::getTriggeringData() {
				return getDirectValueTriggeringData();
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
