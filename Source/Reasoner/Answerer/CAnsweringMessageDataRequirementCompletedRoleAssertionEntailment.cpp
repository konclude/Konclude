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

#include "CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment::CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment(CEntailmentQueryProcessingData* entailmentProcData, CRole* role, CIndividual* individualSource, CIndividual* individualDestination) : CAnsweringMessageDataRequirementCompleted(CAnsweringMessageDataRequirementCompleted::ROLEASSERTIONENTAILMENTREQUIREMENT) {
				mEntailmentProcData = entailmentProcData;
				mRole = role;
				mIndividualSource = individualSource;
				mIndividualDestination = individualDestination;
			}


			CEntailmentQueryProcessingData* CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment::getEntailmentProcessingData() {
				return mEntailmentProcData;
			}

			CRole* CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment::getRole() {
				return mRole;
			}

			CIndividual* CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment::getIndividualSource() {
				return mIndividualSource;
			}

			CIndividual* CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment::getIndividualDestination() {
				return mIndividualDestination;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
