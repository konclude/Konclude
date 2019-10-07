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

#include "CComplexConceptItemComputationProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CComplexConceptItemComputationProcess::CComplexConceptItemComputationProcess() {
				for (cint64 i = 0; i < COMPLEXCONCEPTSTEPCOMPUTATIONPROCESSTYPECOUNT; ++i) {
					mComputationProcessArray[i] = nullptr;
				}
			}


			CComplexConceptItemComputationProcess::~CComplexConceptItemComputationProcess() {
			}


			CComplexConceptStepComputationProcess* CComplexConceptItemComputationProcess::getSatisfiableClassNodesComputationProcess(bool create) {
				return getComputationProcess(COMPUTATIONTYPESATISFIABILITY);
			}


			CComplexConceptStepComputationProcess* CComplexConceptItemComputationProcess::getSuperClassNodesComputationProcess(bool create) {
				return getComputationProcess(COMPUTATIONTYPESUPERCLASSNODES);
			}

			CComplexConceptStepComputationProcess* CComplexConceptItemComputationProcess::getSubClassNodesComputationProcess(bool create) {
				return getComputationProcess(COMPUTATIONTYPESUBCLASSNODES);
			}

			CComplexConceptStepComputationProcess* CComplexConceptItemComputationProcess::getSubClassRealizationProcess(bool create) {
				return getComputationProcess(REALIZATIONTYPESUBCLASS);
			}


			CComplexConceptStepComputationProcess* CComplexConceptItemComputationProcess::getEquivalentClassNodesComputationProcess(bool create) {
				return getComputationProcess(COMPUTATIONTYPEEQUIVALENTCLASSNODES);
			}


			CComplexConceptStepInstanceComputationProcess* CComplexConceptItemComputationProcess::getInstancesComputationProcess(bool create) {
				return (CComplexConceptStepInstanceComputationProcess*)getComputationProcess(COMPUTATIONTYPEINSTANCES);
			}


			CComplexConceptStepComputationProcess* CComplexConceptItemComputationProcess::getComputationProcess(ComplexConceptStepComputationProcessType computationType, bool create) {
				if (computationType < 0 || computationType >= COMPLEXCONCEPTSTEPCOMPUTATIONPROCESSTYPECOUNT) {
					return nullptr;
				}
				if (create && !mComputationProcessArray[computationType]) {
					if (computationType == COMPUTATIONTYPEINSTANCES) {
						mComputationProcessArray[computationType] = new CComplexConceptStepInstanceComputationProcess();
					} else {
						mComputationProcessArray[computationType] = new CComplexConceptStepComputationProcess(computationType);
					}
				}
				return mComputationProcessArray[computationType];
			}


			bool CComplexConceptItemComputationProcess::isComputationRequired(ComplexConceptStepComputationProcessType computationType) {
				if (computationType < 0 || computationType >= COMPLEXCONCEPTSTEPCOMPUTATIONPROCESSTYPECOUNT) {
					return false;
				}
				if (!mComputationProcessArray[computationType]) {
					return false;
				}
				if (mComputationProcessArray[computationType]->isComputationProcessRequired()) {
					return true;
				}
				if (computationType == COMPUTATIONTYPEINSTANCES) {
					return getInstancesComputationProcess(true)->requiresAdditionalInstanceComputation();
				}
				return false;
			}


			bool CComplexConceptItemComputationProcess::isComputationFinished(ComplexConceptStepComputationProcessType computationType) {
				if (computationType < 0 || computationType >= COMPLEXCONCEPTSTEPCOMPUTATIONPROCESSTYPECOUNT) {
					return false;
				}
				if (!mComputationProcessArray[computationType]) {
					return false;
				}
				return mComputationProcessArray[computationType]->isComputationProcessFinished();
			}


			bool CComplexConceptItemComputationProcess::isComputationStarted(ComplexConceptStepComputationProcessType computationType) {
				if (computationType < 0 || computationType >= COMPLEXCONCEPTSTEPCOMPUTATIONPROCESSTYPECOUNT) {
					return false;
				}
				if (!mComputationProcessArray[computationType]) {
					return false;
				}
				return mComputationProcessArray[computationType]->isComputationProcessStarted();
			}




			CComplexConceptStepComputationProcess* CComplexConceptItemComputationProcess::getNextConceptStepComputationProcess(ComplexConceptStepComputationProcessType type, bool onlyRequired) {
				while ((cint64)type + 1 < COMPLEXCONCEPTSTEPCOMPUTATIONPROCESSTYPECOUNT) {
					type = (ComplexConceptStepComputationProcessType)(type + 1);
					if (!onlyRequired || isComputationRequired(type)) {
						return mComputationProcessArray[type];
					}
				}
				return nullptr;
			}


			CComplexConceptStepComputationProcess* CComplexConceptItemComputationProcess::getNextConceptStepComputationProcess(CComplexConceptStepComputationProcess* step, bool onlyRequired) {
				return getNextConceptStepComputationProcess(step->getComputationType(), onlyRequired);
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
