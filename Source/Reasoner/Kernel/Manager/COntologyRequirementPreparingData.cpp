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

#include "COntologyRequirementPreparingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {


				COntologyRequirementPreparingData::COntologyRequirementPreparingData(CConcreteOntology* ontology) {
					mOntology = ontology;
					mRequirementsProcessed = false;
				}


				COntologyRequirementPreparingData* COntologyRequirementPreparingData::addOntologyRequirement(COntologyProcessingRequirement* requirement) {
					bool addedReq = false;
					COntologyProcessingStepRequirement* requirementStep = dynamic_cast<COntologyProcessingStepRequirement*>(requirement);
					if (requirementStep) {
						COntologyProcessingStep::PROCESSORTYPE proType = requirementStep->getRequiredProcessingStep()->getProcessorType();
						if (proType == COntologyProcessingStep::OPPREPROCESSOR) {
							mPreprocessorReqList.append(requirement);
							addedReq = true;
						} else if (proType == COntologyProcessingStep::OPPRECOMPUTER) {
							mPrecomputorReqList.append(requirement);
							addedReq = true;
						} else if (proType == COntologyProcessingStep::OPCLASSCLASSIFIER) {
							mClassifierReqList.append(requirement);
							addedReq = true;
						} else if (proType == COntologyProcessingStep::OPCONCEPTREALIZER) {
							mRealizerReqList.append(requirement);
							addedReq = true;
						}
					}
					COntologyProcessingConceptSaturationRequirement* conSatRequirement = dynamic_cast<COntologyProcessingConceptSaturationRequirement*>(requirement);
					if (conSatRequirement) {
						mPrecomputorReqList.append(requirement);
						addedReq = true;
					}

					if (!addedReq) {
						mFailedReqList.append(requirement);
					}
					return this;
				}


			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
