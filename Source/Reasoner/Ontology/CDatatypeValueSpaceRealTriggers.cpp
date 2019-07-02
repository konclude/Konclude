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

#include "CDatatypeValueSpaceRealTriggers.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDatatypeValueSpaceRealTriggers::CDatatypeValueSpaceRealTriggers(CBoxContext* boxContext) : CDatatypeValueSpaceCompareTriggers(boxContext) {
			}



			CDatatypeValueSpaceRealTriggers* CDatatypeValueSpaceRealTriggers::initValueSpaceRealTriggers(CDatatypeValueSpaceRealTriggers* data) {
				initValueSpaceCompareTriggers(data);
				if (data) {
					mValueSpateRationalTriggerData.initConceptTriggeringData(&data->mValueSpateRationalTriggerData);
					mValueSpateDecimalTriggerData.initConceptTriggeringData(&data->mValueSpateDecimalTriggerData);
					mValueSpateIntegerTriggerData.initConceptTriggeringData(&data->mValueSpateIntegerTriggerData);
				} else {
					mValueSpateRationalTriggerData.initConceptTriggeringData(nullptr);
					mValueSpateDecimalTriggerData.initConceptTriggeringData(nullptr);
					mValueSpateIntegerTriggerData.initConceptTriggeringData(nullptr);
				}
				return this;
			}



			CDatatypeValueSpaceRealTriggers* CDatatypeValueSpaceRealTriggers::addRealConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger) {
				incConceptTriggerCount(conceptTrigger->getCount());
				mValueSpateTriggerData.appendPartialConceptTriggers(conceptTrigger);
				return this;
			}



			CDatatypeValueSpaceRealTriggers* CDatatypeValueSpaceRealTriggers::addRationalConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger) {
				incConceptTriggerCount(conceptTrigger->getCount());
				mValueSpateRationalTriggerData.appendPartialConceptTriggers(conceptTrigger);
				return this;
			}


			CDatatypeValueSpaceRealTriggers* CDatatypeValueSpaceRealTriggers::addDecimalConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger) {
				incConceptTriggerCount(conceptTrigger->getCount());
				mValueSpateDecimalTriggerData.appendPartialConceptTriggers(conceptTrigger);
				return this;
			}


			CDatatypeValueSpaceRealTriggers* CDatatypeValueSpaceRealTriggers::addIntegerConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger) {
				incConceptTriggerCount(conceptTrigger->getCount());
				mValueSpateIntegerTriggerData.appendPartialConceptTriggers(conceptTrigger);
				return this;
			}



			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceRealTriggers::getRealConceptTriggeringData() {
				return getValueSpaceConceptTriggeringData();
			}


			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceRealTriggers::getRationalConceptTriggeringData() {
				return &mValueSpateRationalTriggerData;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceRealTriggers::getDecimalConceptTriggeringData() {
				return &mValueSpateDecimalTriggerData;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceRealTriggers::getIntegerConceptTriggeringData() {
				return &mValueSpateIntegerTriggerData;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
