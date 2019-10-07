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

#include "CConceptSetStructure.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CConceptSetStructure::CConceptSetStructure() {
					reset();
				}

				CConceptSetStructure* CConceptSetStructure::reset() {
					mDynamicCreatedConcepts = false;
					mBindingPropagationConcepts = false;
					return this;
				}

				CConceptSetStructure* CConceptSetStructure::setBindingPropagationConcepts(bool bindingPropagationConcepts) {
					mBindingPropagationConcepts = bindingPropagationConcepts;
					return this;
				}

				bool CConceptSetStructure::hasBindingPropagationConcepts() {
					return mBindingPropagationConcepts;
				}


				CConceptSetStructure* CConceptSetStructure::addedConcept(CConceptDescriptor* conceptDescriptor) {
					CConcept* concept = conceptDescriptor->getConcept();
					cint64 conCode = concept->getOperatorCode();
					CConceptOperator* conOperator = concept->getConceptOperator();
					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_PROPAGATION_TYPE)) {
						mBindingPropagationConcepts = true;
					}
					if (concept->getTerminology() == nullptr) {
						mDynamicCreatedConcepts = true;
					}
					return this;
				}


				CConceptSetStructure* CConceptSetStructure::setDynamicCreatedConcepts(bool dynamicCreatedConcepts) {
					mDynamicCreatedConcepts = dynamicCreatedConcepts;
					return this;
				}

				bool CConceptSetStructure::hasDynamicCreatedConcepts() {
					return mDynamicCreatedConcepts;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
