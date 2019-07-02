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

#include "COntologyRequirementPreparingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {


				COntologyRequirementPreparingData::COntologyRequirementPreparingData(CConcreteOntology* ontology) {
					mOntology = ontology;
					mRequirementsProcessed = false;
				}


				COntologyRequirementPreparingData::~COntologyRequirementPreparingData() {
					foreach (COntologyProcessingRequirement* requirement, mAllReqList) {
						if (requirement->isDynamicRequirement()) {
							delete requirement;
						}
					}
				}



				COntologyRequirementPreparingData* COntologyRequirementPreparingData::addOntologyRequirement(COntologyProcessingRequirement* requirement) {
					mAllReqList.append(requirement);
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
							mClassClassifierReqList.append(requirement);
							addedReq = true;
						} else if (proType == COntologyProcessingStep::OPDATAPROPERTYCLASSIFIER) {
							mDataPropertyClassifierReqList.append(requirement);
							addedReq = true;
						} else if (proType == COntologyProcessingStep::OPOBJECTPROPERTYCLASSIFIER) {
							mObjectPropertyClassifierReqList.append(requirement);
							addedReq = true;
						} else if (proType == COntologyProcessingStep::OPREALIZER) {
							mRealizerReqList.append(requirement);
							addedReq = true;
						} else if (proType == COntologyProcessingStep::OPANSWERER) {
							mAnswererReqList.append(requirement);
							addedReq = true;
						}
					}
					COntologyProcessingConceptSaturationRequirement* conSatRequirement = dynamic_cast<COntologyProcessingConceptSaturationRequirement*>(requirement);
					if (conSatRequirement) {
						mPrecomputorReqList.append(requirement);
						addedReq = true;
					}
					COntologyProcessingDynamicRealizationRequirement* dynRealRequirement = dynamic_cast<COntologyProcessingDynamicRealizationRequirement*>(requirement);
					if (dynRealRequirement) {
						mRealizerReqList.append(requirement);
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
