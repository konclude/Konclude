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

#include "CDatatypeValueSpaceTriggers.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDatatypeValueSpaceTriggers::CDatatypeValueSpaceTriggers(CContext* boxContext) : mBoxContext(boxContext) {
			}



			CDatatypeValueSpaceTriggers* CDatatypeValueSpaceTriggers::initValueSpaceTriggers(CDatatypeValueSpaceTriggers* data) {
				if (data) {
					mValueSpateTriggerData.initConceptTriggeringData(&data->mValueSpateTriggerData);
					mConceptTriggerCount = data->mConceptTriggerCount;
				} else {
					mValueSpateTriggerData.initConceptTriggeringData(nullptr);
					mConceptTriggerCount = 0;
				}
				return this;
			}



			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceTriggers::getValueSpaceConceptTriggeringData() {
				return &mValueSpateTriggerData;
			}

			cint64 CDatatypeValueSpaceTriggers::getConceptTriggerCount() {
				return mConceptTriggerCount;
			}

			CDatatypeValueSpaceTriggers* CDatatypeValueSpaceTriggers::incConceptTriggerCount(cint64 incCount) {
				mConceptTriggerCount += incCount;
				return this;
			}




			CDatatypeValueSpaceTriggers* CDatatypeValueSpaceTriggers::addValueSpaceConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger) {
				incConceptTriggerCount(conceptTrigger->getCount());
				mValueSpateTriggerData.appendPartialConceptTriggers(conceptTrigger);
				return this;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
