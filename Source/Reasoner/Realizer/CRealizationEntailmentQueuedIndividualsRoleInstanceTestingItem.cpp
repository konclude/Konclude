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

#include "CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem::CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem(const CRealizationIndividualInstanceItemReference& indSourceiItemRef, const CRealizationIndividualInstanceItemReference& indDestiItemRef, COptimizedKPSetRoleInstancesItem* roleItem, bool roleInversed, COntologyRealizingDynamicRequirmentProcessingData* procData) : CRealizationEntailmentQueuedTestingItem(procData, CRealizationEntailmentQueuedTestingItem::EQTT_INDIVIDUAL_CONCEPT_INSTANCE) {
				mRoleItem = roleItem;
				mIndividualSourceItemRef = indSourceiItemRef;
				mIndividualDestItemRef = indDestiItemRef;
				mRoleInversed = roleInversed;
			}

			CRealizationIndividualInstanceItemReference CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem::getIndividualSourceItemReference() {
				return mIndividualSourceItemRef;
			}

			CRealizationIndividualInstanceItemReference CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem::getIndividualDestinationItemReference() {
				return mIndividualDestItemRef;
			}

			COptimizedKPSetRoleInstancesItem* CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem::getRoleItem() {
				return mRoleItem;
			}

			bool CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem::isRoleInversed() {
				return mRoleInversed;
			}

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
