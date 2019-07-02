/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKPILINGSATURATIONSUBSUMPTIONIDENTIFIERANALYSER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKPILINGSATURATIONSUBSUMPTIONIDENTIFIERANALYSER_H

// Libraries includes


// Namespace includes
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Classifier/CConceptSubsumptionRelationObserver.h"
#include "Reasoner/Classifier/COptimizedClassSaturationSatisfiableTestingItem.h"

#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Classifier;
		using namespace Ontology;

		namespace Kernel {

			using namespace Task;
			using namespace Process;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CSatisfiableTaskPilingSaturationSubsumptionIdentifierAnalyser
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableTaskPilingSaturationSubsumptionIdentifierAnalyser {
					// public methods
					public:
						//! Constructor
						CSatisfiableTaskPilingSaturationSubsumptionIdentifierAnalyser();

						bool readCalculationConfig(CCalculationConfigurationExtension *config);
						bool analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:

					// protected variables
					protected:
						bool mConfAnalyseSubSumptions;
						bool mConfAnalyseAllSubSumptions;
						bool mConfAnalyseIdentifierOccurence;
						bool mConfAnalysePossibleSubSumptions;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKPILINGSATURATIONSUBSUMPTIONIDENTIFIERANALYSER_H
