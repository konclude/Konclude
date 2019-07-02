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

#include "CConceptRoleBranchingTrigger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CConceptRoleBranchingTrigger::CConceptRoleBranchingTrigger() : CLinkerBase<CConceptRoleBranchingTrigger*,CConceptRoleBranchingTrigger>(this) {
				mTriggerNegation = false;
				mTriggerConcept = nullptr;
				mUsedConceptTrigger = true;
				mTriggerRole = nullptr;
			}

			CConceptRoleBranchingTrigger* CConceptRoleBranchingTrigger::initConceptBranchingTrigger(CConcept* triggerConcept, bool triggerNegation) {
				mTriggerConcept = triggerConcept;
				mTriggerNegation = triggerNegation;
				mUsedConceptTrigger = true;
				return this;
			}

			CConceptRoleBranchingTrigger* CConceptRoleBranchingTrigger::initRoleBranchingTrigger(CRole *triggerRole) {
				mUsedConceptTrigger = false;
				mTriggerRole = triggerRole;
				return this;
			}


			CConcept* CConceptRoleBranchingTrigger::getTriggerConcept() {
				return mTriggerConcept;
			}


			CRole* CConceptRoleBranchingTrigger::getTriggerRole() {
				return mTriggerRole;
			}

			bool CConceptRoleBranchingTrigger::getTriggerNegation() {
				return mTriggerNegation;
			}


			bool CConceptRoleBranchingTrigger::isConceptTrigger() {
				return mUsedConceptTrigger;
			}

			bool CConceptRoleBranchingTrigger::isRoleTrigger() {
				return !mUsedConceptTrigger;
			}


			CConceptRoleBranchingTrigger::BRANCHINGTRIGGERTYPE CConceptRoleBranchingTrigger::getBranchingTriggerType() {
				if (mUsedConceptTrigger) {
					return BTTCONCEPT;
				} else {
					return BTTROLE;
				}
			}


			CConceptRoleBranchingTrigger* CConceptRoleBranchingTrigger::getNextBranchingTrigger() {
				return getNext();
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
