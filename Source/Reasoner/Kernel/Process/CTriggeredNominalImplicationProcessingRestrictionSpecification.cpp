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

#include "CTriggeredNominalImplicationProcessingRestrictionSpecification.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CTriggeredNominalImplicationProcessingRestrictionSpecification::CTriggeredNominalImplicationProcessingRestrictionSpecification() {
					mNextImpTrigger = nullptr;
					mLastDependency = nullptr;
				}


				CTriggeredNominalImplicationProcessingRestrictionSpecification* CTriggeredNominalImplicationProcessingRestrictionSpecification::initImplicationTriggeringProcessingRestriction(CTriggeredNominalImplicationProcessingRestrictionSpecification* prevRest) {
					CProcessingRestrictionSpecification::initProcessingRestriction(prevRest);
					if (prevRest) {
						mNextImpTrigger = prevRest->mNextImpTrigger;
						mLastDependency = prevRest->mLastDependency;
					}
					return this;
				}

				CTriggeredNominalImplicationProcessingRestrictionSpecification* CTriggeredNominalImplicationProcessingRestrictionSpecification::setConceptImplicationTrigger(CSortedNegLinker<CConcept*>* conRoleBranchTrigger) {
					mNextImpTrigger = conRoleBranchTrigger;
					return this;
				}


				bool CTriggeredNominalImplicationProcessingRestrictionSpecification::hasConceptImplicationTrigger() {
					return mNextImpTrigger != nullptr;
				}


				CSortedNegLinker<CConcept*>* CTriggeredNominalImplicationProcessingRestrictionSpecification::getConceptImplicationTrigger() {
					return mNextImpTrigger;
				}


				CDependency* CTriggeredNominalImplicationProcessingRestrictionSpecification::getImplicationDependency() {
					return mLastDependency;
				}

				CTriggeredNominalImplicationProcessingRestrictionSpecification* CTriggeredNominalImplicationProcessingRestrictionSpecification::setImplicationDependency(CDependency* dependency) {
					mLastDependency = dependency;
					return this;
				}




			}; // end namespace Kernel

		}; // end namespace Process

	}; // end namespace Reasoner

}; // end namespace Konclude
