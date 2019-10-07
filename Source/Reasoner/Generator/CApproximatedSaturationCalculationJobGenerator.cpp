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

#include "CApproximatedSaturationCalculationJobGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			CApproximatedSaturationCalculationJobGenerator::CApproximatedSaturationCalculationJobGenerator(CConcreteOntology *ontology) {
				mOntology = ontology;
			}


			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJobGenerator::getApproximatedSaturationCalculationJob(cint64 approximationSaturationMode, CSaturationData* saturationData) {
				CApproximatedSaturationCalculationJob* satCalcJob = new CApproximatedSaturationCalculationJob();
				satCalcJob->setContinueSaturationData(saturationData);
				satCalcJob->setOntology(mOntology);
				return satCalcJob;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJobGenerator::extendApproximatedSaturationCalculationJobConstruction(CRole* role, CConcept* concept, bool conceptNegation, bool processing, CSaturationConceptReferenceLinking* satConRefLinking, CApproximatedSaturationCalculationJob* satCalcJob) {
				CApproximatedSaturationCalculationConstructionConstruct* satCalcConstruct = new CApproximatedSaturationCalculationConstructionConstruct();
				satCalcConstruct->setConstructRole(role);
				satCalcConstruct->setConstructConcept(concept);
				satCalcConstruct->setConstructConceptNegation(conceptNegation);
				satCalcConstruct->setSaturationConceptReferenceLinking(satConRefLinking);
				satCalcConstruct->setQueueProcessing(processing);

				satCalcJob->addSatisfiableCalculationConstructionConstruct(satCalcConstruct);
				return satCalcJob;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJobGenerator::extendApproximatedSaturationCalculationJobProcessing(CSaturationConceptReferenceLinking* satConRefLinking, CApproximatedSaturationCalculationJob* satCalcJob) {
				CApproximatedSaturationCalculationProcessingConstruct* satCalcProcConstruct = new CApproximatedSaturationCalculationProcessingConstruct();
				satCalcProcConstruct->setSaturationConceptReferenceLinking(satConRefLinking);
				satCalcJob->addSatisfiableCalculationProcessingConstruct(satCalcProcConstruct);
				return satCalcJob;
			}


			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJobGenerator::extendApproximatedSaturationCalculationJobProcessing(CIndividual* individual, CSaturationIndividualReferenceLinking* satIndiRefLinking, CApproximatedSaturationCalculationJob* satCalcJob) {
				CApproximatedSaturationCalculationConstructionConstruct* satCalcConstruct = new CApproximatedSaturationCalculationConstructionConstruct();
				satCalcConstruct->setIndividual(individual);
				satCalcConstruct->setQueueProcessing(true);
				satCalcConstruct->setSaturationIndividualReferenceLinking(satIndiRefLinking);
				satCalcJob->addSatisfiableCalculationConstructionConstruct(satCalcConstruct);
				return satCalcJob;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJobGenerator::extendApproximatedSaturationCalculationJobProcessing(cint64 indiId, CSaturationIndividualReferenceLinking* satIndiRefLinking, CApproximatedSaturationCalculationJob* satCalcJob) {
				CApproximatedSaturationCalculationConstructionConstruct* satCalcConstruct = new CApproximatedSaturationCalculationConstructionConstruct();
				satCalcConstruct->setIndividualID(indiId);
				satCalcConstruct->setQueueProcessing(true);
				satCalcConstruct->setSaturationIndividualReferenceLinking(satIndiRefLinking);
				satCalcJob->addSatisfiableCalculationConstructionConstruct(satCalcConstruct);
				return satCalcJob;
			}

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
