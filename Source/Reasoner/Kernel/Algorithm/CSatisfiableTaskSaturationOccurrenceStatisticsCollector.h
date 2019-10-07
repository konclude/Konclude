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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKSATURATIONOCCURRENCESTATISTICSCOLLECTOR_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKSATURATIONOCCURRENCESTATISTICSCOLLECTOR_H

// Libraries includes


// Namespace includes
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Reasoner/Kernel/Cache/COccurrenceStatisticsCacheWriter.h"

#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Task/CSaturationTaskData.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CIndividualProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

// Logger includes
#include "Logger/CLogger.h"




namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			using namespace Task;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CSatisfiableTaskSaturationOccurrenceStatisticsCollector
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableTaskSaturationOccurrenceStatisticsCollector {
					// public methods
					public:
						//! Constructor
						CSatisfiableTaskSaturationOccurrenceStatisticsCollector(COccurrenceStatisticsCacheWriter* occStatsCacheWriter);

						//! Destructor
						virtual ~CSatisfiableTaskSaturationOccurrenceStatisticsCollector();


						bool analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext);


					// protected methods
					protected:


						bool collectOccurrenceStatistics(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContext* calcAlgContext);
						CIndividualSaturationProcessNode* getSuccessorNode(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode* satNode, CCalculationAlgorithmContext* calcAlgContext);
						bool visitNodeSuccessors(CIndividualSaturationProcessNode* satNode,
							bool visitedExistentialSuccessessors, bool visitNominalSuccessors, bool visitDataSuccessors, bool analyseConcepts, bool analyseSuccessorLinks,
							function<bool(CIndividualSaturationProcessNode* succNode, cint64 succId)> visitSuccNodeFunc, function<bool(CRole* role, bool inversed)> visitSuccRoleFunc, CCalculationAlgorithmContext* calcAlgContext);

						bool collectConceptOccurrenceStatistics(CIndividualSaturationProcessNode* indiNode, CCalculationAlgorithmContext* calcAlgContext);


					// protected variables
					protected:
						COccurrenceStatisticsCacheWriter* mOccStatsCacheWriter;
						cint64 mOntologyTag;
						CConcreteOntology* mOntology;

						cint64 mCollectedOccStatsNodeCount;
						cint64 mCollectedOccStatsSuccNodeCount;
						cint64 mCollectedOccStatsConceptCount;
						cint64 mCollectedOccStatsRoleCount;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKSATURATIONOCCURRENCESTATISTICSCOLLECTOR_H
