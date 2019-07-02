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

#include "CDatatypeValueSpaceTriggers.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDatatypeValueSpaceTriggers::CDatatypeValueSpaceTriggers(CBoxContext* boxContext) : mBoxContext(boxContext) {
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
