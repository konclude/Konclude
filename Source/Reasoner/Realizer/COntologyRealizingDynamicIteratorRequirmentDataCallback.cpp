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

#include "COntologyRealizingDynamicIteratorRequirmentDataCallback.h"
#include "COptimizedRepresentativeKPSetOntologyRealizingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {



			COntologyRealizingDynamicIteratorRequirmentDataCallback::COntologyRealizingDynamicIteratorRequirmentDataCallback(COntologyRealizingDynamicRequirmentProcessingData* procData, COptimizedRepresentativeKPSetOntologyRealizingItem* realizationItem, COntologyProcessingIteratorRealizationRequirement* iteratorRequirement) {
				mProcData = procData;
				mRealizationItem = realizationItem;
				mIteratorRequirement = iteratorRequirement;
				mFoundInstancesCount = 0;
				mRealizedAllInstances = false;

				mRoleInitializationCount = 0;
				mRoleRealizationCount = 0;
				mConceptRealizationCount = 0;
			}


			void COntologyRealizingDynamicIteratorRequirmentDataCallback::doCallback() {
				mRealizationItem->addProcessingRequirement(this, mProcData);
			}


			COntologyRealizingDynamicRequirmentProcessingData* COntologyRealizingDynamicIteratorRequirmentDataCallback::getRequirementProcessingData() {
				return mProcData;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COntologyRealizingDynamicIteratorRequirmentDataCallback::getRealizationItem() {
				return mRealizationItem;
			}

			COntologyProcessingIteratorRealizationRequirement* COntologyRealizingDynamicIteratorRequirmentDataCallback::getRealizationIteratorRequirement() {
				return mIteratorRequirement;
			}

			cint64 COntologyRealizingDynamicIteratorRequirmentDataCallback::getFoundInstancesCount() {
				return mFoundInstancesCount;
			}

			COntologyRealizingDynamicIteratorRequirmentDataCallback* COntologyRealizingDynamicIteratorRequirmentDataCallback::incFoundInstancesCount(cint64 count) {
				mFoundInstancesCount += count;
				return this;
			}


			bool COntologyRealizingDynamicIteratorRequirmentDataCallback::hasRealizedAllInstances() {
				return mRealizedAllInstances;
			}

			COntologyRealizingDynamicIteratorRequirmentDataCallback* COntologyRealizingDynamicIteratorRequirmentDataCallback::setRealizedAllInstances(bool realizedAll) {
				mRealizedAllInstances = realizedAll;
				return this;
			}


			COntologyProcessingDynamicRealizationRequirement* COntologyRealizingDynamicIteratorRequirmentDataCallback::submitRequirementUpdate(cint64 processingFlags, cint64 errorFlags) {
				mIteratorRequirement->submitRequirementUpdate(processingFlags, errorFlags);
				return this;
			}



			cint64 COntologyRealizingDynamicIteratorRequirmentDataCallback::getRoleInitializationCount() {
				return mRoleInitializationCount;
			}

			cint64 COntologyRealizingDynamicIteratorRequirmentDataCallback::getRoleRealizationCount() {
				return mRoleRealizationCount;
			}

			cint64 COntologyRealizingDynamicIteratorRequirmentDataCallback::getConceptRealizationCount() {
				return mConceptRealizationCount;
			}

			COntologyRealizingDynamicIteratorRequirmentDataCallback* COntologyRealizingDynamicIteratorRequirmentDataCallback::incRoleInitializationCount() {
				mRoleInitializationCount++;
				return this;
			}

			COntologyRealizingDynamicIteratorRequirmentDataCallback* COntologyRealizingDynamicIteratorRequirmentDataCallback::incRoleRealizationCount() {
				mRoleRealizationCount++;
				return this;
			}

			COntologyRealizingDynamicIteratorRequirmentDataCallback* COntologyRealizingDynamicIteratorRequirmentDataCallback::incConceptRealizationCount() {
				mConceptRealizationCount++;
				return this;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
