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

#include "COntologyProcessingStepVector.h"
#include "COntologyProcessingStepRequirement.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			QMutex* COntologyProcessingStepVector::mInstanceMutex = new QMutex();
			COntologyProcessingStepVector* COntologyProcessingStepVector::mProcStepVectorInstance = nullptr;


			COntologyProcessingStepVector::COntologyProcessingStepVector() {
				mPrStCount = 0;
			}


			COntologyProcessingStepVector* COntologyProcessingStepVector::initDefaultProcessingSteps() {
				COntologyProcessingStep* activeBuild = new COntologyProcessingStep(COntologyProcessingStep::OPSACTIVECOUNT,mPrStCount,COntologyProcessingStep::OPPREPROCESSOR);
				addProcessingSteps(activeBuild);

				COntologyProcessingStep* stepBuild = new COntologyProcessingStep(COntologyProcessingStep::OPSBUILD,mPrStCount,COntologyProcessingStep::OPPREPROCESSOR);
				addProcessingSteps(stepBuild);

				COntologyProcessingStep* stepPreprocess = new COntologyProcessingStep(COntologyProcessingStep::OPSPREPROCESS,mPrStCount,COntologyProcessingStep::OPPREPROCESSOR);
				stepPreprocess->addProcessingRequirement(new COntologyProcessingStepRequirement(stepBuild, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepPreprocess->addProcessingRequirement(new COntologyProcessingStepRequirement(activeBuild, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepPreprocess);

				COntologyProcessingStep* stepConsistency = new COntologyProcessingStep(COntologyProcessingStep::OPSCONSISTENCY,mPrStCount,COntologyProcessingStep::OPPRECOMPUTER);
				stepConsistency->addProcessingRequirement(new COntologyProcessingStepRequirement(stepBuild, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepConsistency->addProcessingRequirement(new COntologyProcessingStepRequirement(stepPreprocess, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepConsistency);



				COntologyProcessingStep* stepIndividualComp = new COntologyProcessingStep(COntologyProcessingStep::OPSPRECOMPUTEINDIVIDUAL,mPrStCount,COntologyProcessingStep::OPPRECOMPUTER);
				stepIndividualComp->addProcessingRequirement(new COntologyProcessingStepRequirement(stepConsistency, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, COntologyProcessingStatus::PSINCONSITENT));
				addProcessingSteps(stepIndividualComp);



				COntologyProcessingStep* stepSaturation = new COntologyProcessingStep(COntologyProcessingStep::OPSPRECOMPUTESATURATION,mPrStCount,COntologyProcessingStep::OPPRECOMPUTER);
				stepSaturation->addProcessingRequirement(new COntologyProcessingStepRequirement(stepConsistency, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, COntologyProcessingStatus::PSINCONSITENT));
				stepSaturation->addProcessingRequirement(new COntologyProcessingStepRequirement(stepIndividualComp, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepSaturation);

				COntologyProcessingStep* stepCycles = new COntologyProcessingStep(COntologyProcessingStep::OPSPRECOMPUTECYCLES,mPrStCount,COntologyProcessingStep::OPPRECOMPUTER);
				stepCycles->addProcessingRequirement(new COntologyProcessingStepRequirement(stepConsistency, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, COntologyProcessingStatus::PSINCONSITENT));
				stepCycles->addProcessingRequirement(new COntologyProcessingStepRequirement(stepIndividualComp, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepCycles);

				COntologyProcessingStep* stepClassify = new COntologyProcessingStep(COntologyProcessingStep::OPSCLASSCLASSIFY,mPrStCount,COntologyProcessingStep::OPCLASSCLASSIFIER);
				stepClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepConsistency, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, COntologyProcessingStatus::PSINCONSITENT));
				stepClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepSaturation, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepCycles, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepIndividualComp, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepClassify);

				COntologyProcessingStep* stepRealizeConcepts = new COntologyProcessingStep(COntologyProcessingStep::OPSCONCEPTREALIZE,mPrStCount,COntologyProcessingStep::OPREALIZER);
				stepRealizeConcepts->addProcessingRequirement(new COntologyProcessingStepRequirement(stepClassify, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepRealizeConcepts);

				COntologyProcessingStep* stepRealizeRoles = new COntologyProcessingStep(COntologyProcessingStep::OPSROLEREALIZE,mPrStCount,COntologyProcessingStep::OPREALIZER);
				stepRealizeRoles->addProcessingRequirement(new COntologyProcessingStepRequirement(stepClassify, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepRealizeRoles);

				COntologyProcessingStep* stepRealizeSameIndividuals = new COntologyProcessingStep(COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE,mPrStCount,COntologyProcessingStep::OPREALIZER);
				stepRealizeSameIndividuals->addProcessingRequirement(new COntologyProcessingStepRequirement(stepClassify, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepRealizeSameIndividuals);
				return this;
			}


			COntologyProcessingStepVector* COntologyProcessingStepVector::addProcessingSteps(COntologyProcessingStep* proStepData) {
				mPrStepVector.append(proStepData);
				++mPrStCount;
				return this;
			}



			COntologyProcessingStep* COntologyProcessingStepVector::getProcessingStep(cint64 stepID) {
				if (stepID < 0 || stepID >= mPrStCount) {
					return nullptr;
				}
				return mPrStepVector[stepID];
			}

			COntologyProcessingStep* COntologyProcessingStepVector::getProcessingStep(COntologyProcessingStep::PROCESSINGSTEPTYPE stepType) {
				return getProcessingStep((cint64)stepType);
			}


			cint64 COntologyProcessingStepVector::getProcessingStepCount() {
				return mPrStCount;
			}


			COntologyProcessingStepVector* COntologyProcessingStepVector::getProcessingStepVectorInstance() {
				if (mProcStepVectorInstance == nullptr) {
					mInstanceMutex->lock();
					if (mProcStepVectorInstance == nullptr) {
						mProcStepVectorInstance = new COntologyProcessingStepVector();
						mProcStepVectorInstance->initDefaultProcessingSteps();
					}
					mInstanceMutex->unlock();
				}
				return mProcStepVectorInstance;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
