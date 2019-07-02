/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_STRATEGY_CUNSATISFIABLECACHERETRIEVALSTRATEGY_H
#define KONCLUDE_REASONER_KERNEL_STRATEGY_CUNSATISFIABLECACHERETRIEVALSTRATEGY_H

// Libraries includes


// Namespace includes


// Other includes
#include "Reasoner/Kernel/Algorithm/AlgorithmSettings.h"

#include "Reasoner/Kernel/Process/CConceptProcessDescriptor.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Algorithm;

			namespace Strategy {

				/*! 
				 *
				 *		\class		CUnsatisfiableCacheRetrievalStrategy
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CUnsatisfiableCacheRetrievalStrategy {
					// public methods
					public:
						//! Constructor
						CUnsatisfiableCacheRetrievalStrategy();

						//! Destructor
						virtual ~CUnsatisfiableCacheRetrievalStrategy();

						virtual bool testUnsatisfiableCacheForProcessing(CConceptProcessDescriptor* conProDes, CIndividualProcessNode *indi) = 0;
						virtual bool testUnsatisfiableCacheForDisjunctionBranching(CConceptProcessDescriptor *conProDes, CIndividualProcessNode *indi, CPROCESSINGLIST< CSortedNegLinker<CConcept*>* >* disjunctList) = 0;
						virtual bool testUnsatisfiableCacheForMergingInitialization(CConceptProcessDescriptor* conProDes, CIndividualProcessNode *indi) = 0;
						virtual bool testUnsatisfiableCacheForSuccessorGeneration(CConceptProcessDescriptor* conProDes, CIndividualProcessNode *indi) = 0;
						virtual bool testUnsatisfiableCacheForBranchedDisjuncts(CConceptProcessDescriptor *conProDes, CIndividualProcessNode *indi, CSortedNegLinker<CConcept*>* orDisjunctConcept) = 0;
						virtual bool testUnsatisfiableCacheForMergedIndividualNodes(CConceptProcessDescriptor *conProDes, CIndividualProcessNode *indi, CIndividualProcessNode *mergedIndi) = 0;
						virtual bool testUnsatisfiableCacheForQualifiedIndividualNodes(CConceptProcessDescriptor *conProDes, CIndividualProcessNode *indi, CIndividualProcessNode *mergedIndi) = 0;


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Strategy

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_STRATEGY_CUNSATISFIABLECACHERETRIEVALSTRATEGY_H
