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

#include "COntologyRealizingDynamicRequirmentCallbackData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COntologyRealizingDynamicRequirmentCallbackData::COntologyRealizingDynamicRequirmentCallbackData(CCallbackData* callback) {
				mCallback = callback;
				mProcessingRequirmentCount = 0;
			}


			COntologyRealizingDynamicRequirmentCallbackData* COntologyRealizingDynamicRequirmentCallbackData::incProcessingRequirmentCount(cint64 incCount) {
				mProcessingRequirmentCount += incCount;
				return this;
			}


			COntologyRealizingDynamicRequirmentCallbackData* COntologyRealizingDynamicRequirmentCallbackData::decProcessingRequirmentCount(cint64 decCount) {
				mProcessingRequirmentCount -= decCount;
				return this;
			}

			COntologyRealizingDynamicRequirmentCallbackData* COntologyRealizingDynamicRequirmentCallbackData::setProcessingFinishedCallback(CCallbackData* callback) {
				mCallback = callback;
				return this;
			}

			cint64 COntologyRealizingDynamicRequirmentCallbackData::getCurrentProcessingRequirmentCount() {
				return mProcessingRequirmentCount;
			}

			bool COntologyRealizingDynamicRequirmentCallbackData::hasCurrentProcessingRequirmentCount() {
				return mProcessingRequirmentCount > 0;
			}
			
			CCallbackData* COntologyRealizingDynamicRequirmentCallbackData::getProcessingFinishedCallback() {
				return mCallback;
			}

			COntologyRealizingDynamicRequirmentProcessingStatistics* COntologyRealizingDynamicRequirmentCallbackData::getStatistics() {
				return &mStatistics;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
