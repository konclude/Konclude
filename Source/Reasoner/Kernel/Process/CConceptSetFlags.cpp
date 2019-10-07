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

#include "CConceptSetFlags.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CConceptSetFlags::CConceptSetFlags() {
					reset();
				}


				CConceptSetFlags* CConceptSetFlags::reset() {
					mPositiveContainedConceptOperatorFlags = 0;
					mNegativeContainedConceptOperatorFlags = 0;
					return this;
				}


				CConceptSetFlags* CConceptSetFlags::addConceptFlags(CConcept* concept, bool negation) {
					cint64 operatorFlag = concept->getConceptOperator()->getOperatorFlag();
					if (!negation) {
						mPositiveContainedConceptOperatorFlags |= operatorFlag;
					} else {
						mNegativeContainedConceptOperatorFlags |= operatorFlag;
					}
					return this;
				}

				CConceptSetFlags* CConceptSetFlags::addConceptFlags(CConceptDescriptor* conceptDescriptor) {
					CConcept* concept = conceptDescriptor->getConcept();
					bool negation = conceptDescriptor->isNegated();
					addConceptFlags(concept,negation);
					return this;
				}

				bool CConceptSetFlags::containsATMOSTCardinalityFlags() {
					if (CConceptOperator::getConceptOperator(CCATMOST)->getOperatorFlag() & mPositiveContainedConceptOperatorFlags) {
						return true;
					}
					if (CConceptOperator::getConceptOperator(CCATLEAST)->getOperatorFlag() & mNegativeContainedConceptOperatorFlags) {
						return true;
					}
					return false;
				}


				bool CConceptSetFlags::containsConceptFlags(CConcept* concept, bool negation) {
					cint64 operatorFlag = concept->getConceptOperator()->getOperatorFlag();
					if (negation) {
						return mNegativeContainedConceptOperatorFlags & operatorFlag;
					} else {
						return mPositiveContainedConceptOperatorFlags & operatorFlag;
					}
				}


				bool CConceptSetFlags::containsConceptFlags(CConcept* concept) {
					cint64 operatorFlag = concept->getConceptOperator()->getOperatorFlag();
					return (mNegativeContainedConceptOperatorFlags | mPositiveContainedConceptOperatorFlags) & operatorFlag;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
