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

#include "COntologyRealizingDynamicRequirmentProcessingStatistics.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COntologyRealizingDynamicRequirmentProcessingStatistics::COntologyRealizingDynamicRequirmentProcessingStatistics() {
				mIteratorRoleInstancesInitializationCount = 0;
				mIteratorRoleInstancesRealizationCount = 0;
				mIteratorConceptInstancesRealizationCount = 0;
				mIteratorPreparationStartingCount = 0;
				mIteratorPreparationFinsihingCount = 0;
				mIteratorScheduledIndividualRealizationCount = 0;
			}


			COntologyRealizingDynamicRequirmentProcessingStatistics* COntologyRealizingDynamicRequirmentProcessingStatistics::incIteratorRoleInstancesInitializationCount(cint64 incCount) {
				mIteratorRoleInstancesInitializationCount += incCount;
				return this;
			}

			cint64 COntologyRealizingDynamicRequirmentProcessingStatistics::getIteratorRoleInstancesInitializationCount() {
				return mIteratorRoleInstancesInitializationCount;
			}

			COntologyRealizingDynamicRequirmentProcessingStatistics* COntologyRealizingDynamicRequirmentProcessingStatistics::incIteratorRoleInstancesRealizationCount(cint64 incCount) {
				mIteratorRoleInstancesRealizationCount += incCount;
				return this;
			}

			cint64 COntologyRealizingDynamicRequirmentProcessingStatistics::getIteratorRoleInstancesRealizationCount() {
				return mIteratorRoleInstancesRealizationCount;
			}

			COntologyRealizingDynamicRequirmentProcessingStatistics* COntologyRealizingDynamicRequirmentProcessingStatistics::incIteratorConceptInstancesRealizationCount(cint64 incCount) {
				mIteratorConceptInstancesRealizationCount += incCount;
				return this;
			}

			cint64 COntologyRealizingDynamicRequirmentProcessingStatistics::getIteratorConceptInstancesRealizationCount() {
				return mIteratorConceptInstancesRealizationCount;
			}

			COntologyRealizingDynamicRequirmentProcessingStatistics* COntologyRealizingDynamicRequirmentProcessingStatistics::incIteratorPreparationStartingCount(cint64 incCount) {
				mIteratorPreparationStartingCount += incCount;
				return this;
			}

			cint64 COntologyRealizingDynamicRequirmentProcessingStatistics::getIteratorPreparationStartingCount() {
				return mIteratorPreparationStartingCount;
			}

			COntologyRealizingDynamicRequirmentProcessingStatistics* COntologyRealizingDynamicRequirmentProcessingStatistics::incIteratorPreparationFinsihingCount(cint64 incCount) {
				mIteratorPreparationFinsihingCount += incCount;
				return this;
			}

			cint64 COntologyRealizingDynamicRequirmentProcessingStatistics::getIteratorPreparationFinsihingCount() {
				return mIteratorPreparationFinsihingCount;
			}

			COntologyRealizingDynamicRequirmentProcessingStatistics* COntologyRealizingDynamicRequirmentProcessingStatistics::incmIteratorScheduledIndividualRealizationCount(cint64 incCount) {
				mIteratorScheduledIndividualRealizationCount += incCount;
				return this;
			}

			cint64 COntologyRealizingDynamicRequirmentProcessingStatistics::getmIteratorScheduledIndividualRealizationCount() {
				return mIteratorScheduledIndividualRealizationCount;
			}




		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
