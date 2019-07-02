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


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
