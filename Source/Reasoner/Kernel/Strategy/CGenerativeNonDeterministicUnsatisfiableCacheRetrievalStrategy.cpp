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

#include "CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Strategy {


				CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy::CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy() {
				}


				CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy::~CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy() {
				}

				bool CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy::testUnsatisfiableCacheForProcessing(CConceptProcessDescriptor* conProDes, CIndividualProcessNode *indi) {
					return false;
				}


				bool CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy::testUnsatisfiableCacheForDisjunctionBranching(CConceptProcessDescriptor *conProDes, CIndividualProcessNode *indi, CPROCESSINGLIST< CSortedNegLinker<CConcept*>* >* disjunctList) {
					return true;
				}

				bool CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy::testUnsatisfiableCacheForMergingInitialization(CConceptProcessDescriptor* conProDes, CIndividualProcessNode *indi) {
					return true;
				}


				bool CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy::testUnsatisfiableCacheForSuccessorGeneration(CConceptProcessDescriptor* conProDes, CIndividualProcessNode *indi) {
					return true;
				}


				bool CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy::testUnsatisfiableCacheForBranchedDisjuncts(CConceptProcessDescriptor *conProDes, CIndividualProcessNode *indi, CSortedNegLinker<CConcept*>* orDisjunctConcept) {
					return true;
				}

				bool CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy::testUnsatisfiableCacheForMergedIndividualNodes(CConceptProcessDescriptor *conProDes, CIndividualProcessNode *indi, CIndividualProcessNode *mergedIndi) {
					return true;
				}

				bool CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy::testUnsatisfiableCacheForQualifiedIndividualNodes(CConceptProcessDescriptor *conProDes, CIndividualProcessNode *indi, CIndividualProcessNode *mergedIndi) {
					return true;
				}



			}; // end namespace Strategy

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
