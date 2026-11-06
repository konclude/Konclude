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

#include "CConceptTriggerLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CConceptTriggerLinker::CConceptTriggerLinker() : CSortedLinkerBase<CConceptTriggerLinker*,CConceptTriggerLinker>(this) {
				mTriggerFrequency = UNKNOWN_TRIGGER_FREQUENCY;
			}

			CConceptTriggerLinker* CConceptTriggerLinker::initConceptTriggerLinker(CConcept* triggerConcept, cint64 triggerComplexity) {
				mTriggerConcept = triggerConcept;
				mTriggerComplexity = triggerComplexity;
				return this;
			}

			CConcept* CConceptTriggerLinker::getTriggerConcept() {
				return mTriggerConcept;
			}

			cint64 CConceptTriggerLinker::getTriggerComplexity() {
				return mTriggerComplexity;
			}

			CConceptTriggerLinker* CConceptTriggerLinker::setTriggerComplexity(cint64 complexity) {
				mTriggerComplexity = complexity;
				return this;
			}

			const cint64 CConceptTriggerLinker::UNKNOWN_TRIGGER_FREQUENCY;
			bool CConceptTriggerLinker::sFrequencyOrdered = true;

			cint64 CConceptTriggerLinker::getTriggerFrequency() {
				return mTriggerFrequency;
			}

			CConceptTriggerLinker* CConceptTriggerLinker::setTriggerFrequency(cint64 frequency) {
				mTriggerFrequency = frequency;
				return this;
			}

			bool CConceptTriggerLinker::operator<=(const CConceptTriggerLinker& conceptTriggerLinker) {
				if (sFrequencyOrdered) {
					if (mTriggerFrequency < conceptTriggerLinker.mTriggerFrequency) {
						return true;
					} else if (mTriggerFrequency > conceptTriggerLinker.mTriggerFrequency) {
						return false;
					}
				}
				if (mTriggerComplexity > conceptTriggerLinker.mTriggerComplexity) {
					return true;
				} else if (mTriggerComplexity < conceptTriggerLinker.mTriggerComplexity) {
					return false;
				} 
				// triggers of the same complexity are ordered by their concept's tag, not by the concept's address:
				// the absorber pairs the triggers in this order and reuses an implication whenever a pair already has
				// one, and with the addresses the pairing followed the memory layout, which the allocator chooses
				// differently from one build to the next; on SNOMED CT one build in two to four then reused some 8 000
				// implications more, its saturation labels grew by 60 % and the precomputation took half as long
				// again with half as much memory again; the later concepts first is the pairing of the good builds
				// KONCLUDE_TRIGGER_TIEBREAK=asc is a testing switch: the ascending order pairs the triggers the way the slow
				// memory layouts did before #79, which is the pairing the reproducers of issue #34 need
				static bool ascending = getenv("KONCLUDE_TRIGGER_TIEBREAK") && QByteArray(getenv("KONCLUDE_TRIGGER_TIEBREAK")) == "asc";
				if (ascending) return mTriggerConcept->getConceptTag() <= conceptTriggerLinker.mTriggerConcept->getConceptTag();
				return mTriggerConcept->getConceptTag() >= conceptTriggerLinker.mTriggerConcept->getConceptTag();
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
