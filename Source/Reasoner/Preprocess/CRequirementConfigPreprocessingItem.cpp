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

#include "CRequirementConfigPreprocessingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CRequirementConfigPreprocessingItem::CRequirementConfigPreprocessingItem() {
			}

			CRequirementConfigPreprocessingItem* CRequirementConfigPreprocessingItem::initRequirementConfigPreprocessingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				COntologyPreprocessingItem::initPreprocessingItem(ontology,config);
				mPreprocessing = nullptr;
				mCalculationConfig = new CCalculationConfigurationExtension(config,0);
				COntologyProcessingStepDataVector* ontProStepDataVec = mOntology->getProcessingSteps()->getOntologyProcessingStepDataVector();
				mActiveCountingProcessingStep = new CPreprocessingTestingStep(CPreprocessingTestingStep::ACTIVECOUNTINGPREPROCESSINGSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSACTIVECOUNT),this);
				mBuildProcessingStep = new CPreprocessingTestingStep(CPreprocessingTestingStep::BUILDPREPROCESSINGSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSBUILD),this);
				mPreprocessProcessingStep = new CPreprocessingTestingStep(CPreprocessingTestingStep::PREPROCESSPREPROCESSINGSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSPREPROCESS),this);
				mProcessingSteps.append(mPreprocessProcessingStep);
				mProcessingSteps.append(mBuildProcessingStep);
				mProcessingSteps.append(mActiveCountingProcessingStep);
				return this;
			}


			CRequirementConfigPreprocessingItem::~CRequirementConfigPreprocessingItem() {
				delete mCalculationConfig;
				delete mActiveCountingProcessingStep;
				delete mBuildProcessingStep;
				delete mPreprocessProcessingStep;
			}

			COntologyPreprocessingItem* CRequirementConfigPreprocessingItem::addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement) {	
				bool supportRequirement = false;
				COntologyProcessingStepRequirement* stepProcRequirement = dynamic_cast<COntologyProcessingStepRequirement*>(ontoRequirement);
				if (stepProcRequirement) {
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSBUILD) {
						mBuildProcessingStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
					} 
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSACTIVECOUNT) {
						mActiveCountingProcessingStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
					} 
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSPREPROCESS) {
						mPreprocessProcessingStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
					}
				}
				if (!supportRequirement) {
					stepProcRequirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSUNSUPPORTED);
				}
				return this;
			}


			CCalculationConfigurationExtension* CRequirementConfigPreprocessingItem::getCalculationConfiguration() {
				return mCalculationConfig;
			}

			CRequirementConfigPreprocessingItem* CRequirementConfigPreprocessingItem::setPreprocessing(CPreprocessing* preprocessing) {
				mPreprocessing = preprocessing;
				return this;
			}

			CPreprocessing* CRequirementConfigPreprocessingItem::getPreprocessing() {
				return mPreprocessing;
			}


			CPreprocessingTestingStep* CRequirementConfigPreprocessingItem::getBuildProcessingStep() {
				return mBuildProcessingStep;
			}

			CPreprocessingTestingStep* CRequirementConfigPreprocessingItem::getActiveCountingProcessingStep() {
				return mActiveCountingProcessingStep;
			}

			CPreprocessingTestingStep* CRequirementConfigPreprocessingItem::getPreprocessProcessingStep() {
				return mPreprocessProcessingStep;
			}


			bool CRequirementConfigPreprocessingItem::isBuildStepFinished() {
				return mBuildProcessingStep->isStepFinished();
			}

			bool CRequirementConfigPreprocessingItem::isBuildStepRequired() {
				return mBuildProcessingStep->hasRequirements();
			}


			bool CRequirementConfigPreprocessingItem::isActiveCountingStepFinished() {
				return mActiveCountingProcessingStep->isStepFinished();
			}

			bool CRequirementConfigPreprocessingItem::isActiveCountingStepRequired() {
				return mActiveCountingProcessingStep->hasRequirements();
			}

			bool CRequirementConfigPreprocessingItem::isPreprocessStepFinished() {
				return mPreprocessProcessingStep->isStepFinished();
			}

			bool CRequirementConfigPreprocessingItem::isPreprocessStepRequired() {
				return mPreprocessProcessingStep->hasRequirements();
			}


			bool CRequirementConfigPreprocessingItem::areBuildStepProcessingRequirementSatisfied() {
				return mBuildProcessingStep->areStepProcessingRequirementSatisfied();
			}

			bool CRequirementConfigPreprocessingItem::areActiveCountingStepProcessingRequirementSatisfied() {
				return mActiveCountingProcessingStep->areStepProcessingRequirementSatisfied();
			}


			bool CRequirementConfigPreprocessingItem::arePreprocessStepProcessingRequirementSatisfied() {
				return mPreprocessProcessingStep->areStepProcessingRequirementSatisfied();
			}

			bool CRequirementConfigPreprocessingItem::areAllStepFinished() {
				for (QList<CPreprocessingTestingStep*>::const_iterator it = mProcessingSteps.constBegin(), itEnd = mProcessingSteps.constEnd(); it != itEnd; ++it) {
					if (!(*it)->isStepFinished()) {
						return false;
					}
				}
				return true;
			}

			bool CRequirementConfigPreprocessingItem::hasRemainingProcessingRequirements() {
				for (QList<CPreprocessingTestingStep*>::const_iterator it = mProcessingSteps.constBegin(), itEnd = mProcessingSteps.constEnd(); it != itEnd; ++it) {
					if ((*it)->hasRequirements()) {
						return true;
					}
				}
				return false;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
