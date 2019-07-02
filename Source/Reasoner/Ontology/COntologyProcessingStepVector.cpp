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
				COntologyProcessingStep* stepTriplesMapping = new COntologyProcessingStep(COntologyProcessingStep::OPSTRIPLESMAPPING, mPrStCount, COntologyProcessingStep::OPPREPROCESSOR);
				addProcessingSteps(stepTriplesMapping);

				COntologyProcessingStep* activeBuild = new COntologyProcessingStep(COntologyProcessingStep::OPSACTIVECOUNT,mPrStCount,COntologyProcessingStep::OPPREPROCESSOR);
				activeBuild->addProcessingRequirement(new COntologyProcessingStepRequirement(stepTriplesMapping, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(activeBuild);

				COntologyProcessingStep* stepBuild = new COntologyProcessingStep(COntologyProcessingStep::OPSBUILD,mPrStCount,COntologyProcessingStep::OPPREPROCESSOR);
				stepBuild->addProcessingRequirement(new COntologyProcessingStepRequirement(stepTriplesMapping, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepBuild);

				COntologyProcessingStep* stepPreprocess = new COntologyProcessingStep(COntologyProcessingStep::OPSPREPROCESS,mPrStCount,COntologyProcessingStep::OPPREPROCESSOR);
				stepPreprocess->addProcessingRequirement(new COntologyProcessingStepRequirement(stepBuild, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepPreprocess->addProcessingRequirement(new COntologyProcessingStepRequirement(activeBuild, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepPreprocess);


				COntologyProcessingStep* stepIndexing = new COntologyProcessingStep(COntologyProcessingStep::OPSTRIPLESINDEXING, mPrStCount, COntologyProcessingStep::OPPREPROCESSOR);
				stepIndexing->addProcessingRequirement(new COntologyProcessingStepRequirement(stepPreprocess, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepIndexing);

				COntologyProcessingStep* stepConsistency = new COntologyProcessingStep(COntologyProcessingStep::OPSCONSISTENCY,mPrStCount,COntologyProcessingStep::OPPRECOMPUTER);
				stepConsistency->addProcessingRequirement(new COntologyProcessingStepRequirement(stepIndexing, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
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


				COntologyProcessingStep* stepOccStats = new COntologyProcessingStep(COntologyProcessingStep::OPSPRECOMPUTEOCCURRENCESTATISTICS, mPrStCount, COntologyProcessingStep::OPPRECOMPUTER);
				stepOccStats->addProcessingRequirement(new COntologyProcessingStepRequirement(stepConsistency, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, COntologyProcessingStatus::PSINCONSITENT));
				stepOccStats->addProcessingRequirement(new COntologyProcessingStepRequirement(stepIndividualComp, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepOccStats);



				COntologyProcessingStep* stepClassify = new COntologyProcessingStep(COntologyProcessingStep::OPSCLASSCLASSIFY,mPrStCount,COntologyProcessingStep::OPCLASSCLASSIFIER);
				stepClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepConsistency, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, COntologyProcessingStatus::PSINCONSITENT));
				stepClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepSaturation, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepCycles, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepIndividualComp, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepClassify);



				COntologyProcessingStep* stepObjPropClassify = new COntologyProcessingStep(COntologyProcessingStep::OPSOBJECTROPERTYCLASSIFY,mPrStCount,COntologyProcessingStep::OPOBJECTPROPERTYCLASSIFIER);
				stepObjPropClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepConsistency, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, COntologyProcessingStatus::PSINCONSITENT));
				stepObjPropClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepSaturation, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepObjPropClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepCycles, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepObjPropClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepIndividualComp, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepObjPropClassify);



				COntologyProcessingStep* stepDataPropClassify = new COntologyProcessingStep(COntologyProcessingStep::OPSDATAROPERTYCLASSIFY,mPrStCount,COntologyProcessingStep::OPDATAPROPERTYCLASSIFIER);
				stepDataPropClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepConsistency, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, COntologyProcessingStatus::PSINCONSITENT));
				stepDataPropClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepSaturation, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepDataPropClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepCycles, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				stepDataPropClassify->addProcessingRequirement(new COntologyProcessingStepRequirement(stepIndividualComp, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepDataPropClassify);



				COntologyProcessingStep* initRealize = new COntologyProcessingStep(COntologyProcessingStep::OPSINITREALIZE, mPrStCount, COntologyProcessingStep::OPREALIZER);
				initRealize->addProcessingRequirement(new COntologyProcessingStepRequirement(stepClassify, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				initRealize->addProcessingRequirement(new COntologyProcessingStepRequirement(stepObjPropClassify, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(initRealize);

				COntologyProcessingStep* stepRealizeConcepts = new COntologyProcessingStep(COntologyProcessingStep::OPSCONCEPTREALIZE,mPrStCount,COntologyProcessingStep::OPREALIZER);
				stepRealizeConcepts->addProcessingRequirement(new COntologyProcessingStepRequirement(initRealize, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepRealizeConcepts);

				COntologyProcessingStep* stepRealizeRoles = new COntologyProcessingStep(COntologyProcessingStep::OPSROLEREALIZE,mPrStCount,COntologyProcessingStep::OPREALIZER);
				stepRealizeRoles->addProcessingRequirement(new COntologyProcessingStepRequirement(initRealize, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepRealizeRoles);

				COntologyProcessingStep* stepRealizeSameIndividuals = new COntologyProcessingStep(COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE,mPrStCount,COntologyProcessingStep::OPREALIZER);
				stepRealizeSameIndividuals->addProcessingRequirement(new COntologyProcessingStepRequirement(initRealize, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
				addProcessingSteps(stepRealizeSameIndividuals);

				COntologyProcessingStep* stepAnswerComplexQueries = new COntologyProcessingStep(COntologyProcessingStep::OPSANSWERCOMPLEXQUERY, mPrStCount, COntologyProcessingStep::OPANSWERER);
				stepAnswerComplexQueries->addProcessingRequirement(new COntologyProcessingStepRequirement(stepOccStats, COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, COntologyProcessingStatus::PSINCONSITENT));
				addProcessingSteps(stepAnswerComplexQueries);

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
