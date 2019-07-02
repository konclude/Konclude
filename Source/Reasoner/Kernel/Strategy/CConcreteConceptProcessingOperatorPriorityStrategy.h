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

#ifndef KONCLUDE_REASONER_KERNEL_STRATEGY_CCONCRETECONCEPTPROCESSINGOPERATORPRIORITYSTRATEGY_H
#define KONCLUDE_REASONER_KERNEL_STRATEGY_CCONCRETECONCEPTPROCESSINGOPERATORPRIORITYSTRATEGY_H

// Libraries includes
#include <qmath.h>

// Namespace includes
#include "CConceptProcessingPriorityStrategy.h"


// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Strategy {

				using namespace Task;

				/*! 
				 *
				 *		\class		CConcreteConceptProcessingOperatorPriorityStrategy
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConcreteConceptProcessingOperatorPriorityStrategy : public CConceptProcessingPriorityStrategy {
					// public methods
					public:
						//! Constructor
						CConcreteConceptProcessingOperatorPriorityStrategy();

						//! Destructor
						virtual ~CConcreteConceptProcessingOperatorPriorityStrategy();

						virtual void readCalculationConfig(CSatisfiableCalculationTask* satCalcTask);

						virtual CConceptProcessPriority getPriorityForConcept(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode *individual);

						virtual double getPriorityOffsetForDisjunctionDelayedConsidering(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode *individual);
						virtual double getPriorityOffsetForDisjunctionDelayedProcessing(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode *individual);

					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:
						int priCount;
						double *priorities;
						double *symAccessPri;

						double mDisjDelConsidPriOffset;
						double mDisjDelProcessPriOffset;

						bool mVariableBindingsPreparationDelaying;

				};

			}; // end namespace Strategy

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_STRATEGY_CCONCRETECONCEPTPROCESSINGOPERATORPRIORITYSTRATEGY_H
