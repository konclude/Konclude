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

#include "COntologyProcessingRequirementExpander.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyProcessingRequirementExpander::COntologyProcessingRequirementExpander() {
				COntologyProcessingStepVector* ontProStepVec = COntologyProcessingStepVector::getProcessingStepVectorInstance();
				mActiveCountOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSACTIVECOUNT), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
				mBuildOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSBUILD), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
				mPreprocessOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSPREPROCESS), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
				mConsistencyOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSCONSISTENCY), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
				mIndividualOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSPRECOMPUTEINDIVIDUAL), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
				mSaturationOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSPRECOMPUTESATURATION), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
				mPrecomputeOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSPRECOMPUTECYCLES), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
				mClassClassifyOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSCLASSCLASSIFY), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
				mRequiredConsistencyOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSCONSISTENCY), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, COntologyProcessingStatus::PSINCONSITENT);
				mConceptRealizeOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSCONCEPTREALIZE), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
				mSameIndividualsRealizeOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
				mRoleRealizeOntProStepReq = new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSROLEREALIZE), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0);
			}

			COntologyProcessingRequirementExpander::~COntologyProcessingRequirementExpander() {
				delete mActiveCountOntProStepReq;
				delete mBuildOntProStepReq;
				delete mPreprocessOntProStepReq;
				delete mConsistencyOntProStepReq;
				delete mIndividualOntProStepReq;
				delete mSaturationOntProStepReq;
				delete mPrecomputeOntProStepReq;
				delete mClassClassifyOntProStepReq;
				delete mRequiredConsistencyOntProStepReq;
				delete mConceptRealizeOntProStepReq;
				delete mRoleRealizeOntProStepReq;
				delete mSameIndividualsRealizeOntProStepReq;
			}


			const QList<COntologyProcessingRequirement*> COntologyProcessingRequirementExpander::getUnsatisfiedRequirementsExpanded(const QList<COntologyProcessingRequirement*>& requirements, CConcreteOntology* ontology) {
				QList<COntologyProcessingRequirement*> mActualReqList;
				QList<COntologyProcessingRequirement*> mCheckReqList;
				QSet<COntologyProcessingRequirement*> mCheckReqSet;

				for (QList<COntologyProcessingRequirement*>::const_iterator it = requirements.constBegin(), itEnd = requirements.constEnd(); it != itEnd; ++it) {
					COntologyProcessingRequirement* requirement = *it;
					if (!mCheckReqSet.contains(requirement)) {
						mCheckReqSet.insert(requirement);
						mCheckReqList.append(requirement);
					}
				}

				while (!mCheckReqList.isEmpty()) {
					COntologyProcessingRequirement* requirement = mCheckReqList.takeFirst();
					if (!requirement->isRequirementSatisfied(ontology)) {
						COntologyProcessingStepRequirement* reqProStep = dynamic_cast<COntologyProcessingStepRequirement*>(requirement);
						if (reqProStep) {
							COntologyProcessingStep* proStep = reqProStep->getRequiredProcessingStep();
							if (proStep) {
								QList<COntologyProcessingRequirement*>* additionalRequirements = proStep->getProcessingRequirementList();
								for (QList<COntologyProcessingRequirement*>::const_iterator it = additionalRequirements->constBegin(), itEnd = additionalRequirements->constEnd(); it != itEnd; ++it) {
									COntologyProcessingRequirement* additionalRequirement = *it;
									if (!mCheckReqSet.contains(additionalRequirement)) {
										mCheckReqSet.insert(additionalRequirement);
										mCheckReqList.append(additionalRequirement);
									}
								}
							}
						}
						mActualReqList.append(requirement);
					}
				}
				return mActualReqList;
			}


			COntologyProcessingStepRequirement* COntologyProcessingRequirementExpander::getRequiredConsistencyOntologyProcessingStepRequirement() {
				return mRequiredConsistencyOntProStepReq;
			}


			COntologyProcessingStepRequirement* COntologyProcessingRequirementExpander::getRequiredIndividualPrecomputationOntologyProcessingStepRequirement() {
				return mIndividualOntProStepReq;
			}

			COntologyProcessingStepRequirement* COntologyProcessingRequirementExpander::getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::PROCESSINGSTEPTYPE processingStep) {
				COntologyProcessingStepRequirement* ontProStepReq = nullptr;
				if (processingStep == COntologyProcessingStep::OPSACTIVECOUNT) {
					ontProStepReq = mActiveCountOntProStepReq;
				} else if (processingStep == COntologyProcessingStep::OPSBUILD) {
					ontProStepReq = mBuildOntProStepReq;
				} else if (processingStep == COntologyProcessingStep::OPSPREPROCESS) {
					ontProStepReq = mPreprocessOntProStepReq;
				} else if (processingStep == COntologyProcessingStep::OPSCONSISTENCY) {
					ontProStepReq = mConsistencyOntProStepReq;
				} else if (processingStep == COntologyProcessingStep::OPSPRECOMPUTEINDIVIDUAL) {
					ontProStepReq = mIndividualOntProStepReq;
				} else if (processingStep == COntologyProcessingStep::OPSPRECOMPUTESATURATION) {
					ontProStepReq = mSaturationOntProStepReq;
				} else if (processingStep == COntologyProcessingStep::OPSPRECOMPUTECYCLES) {
					ontProStepReq = mPrecomputeOntProStepReq;
				} else if (processingStep == COntologyProcessingStep::OPSCLASSCLASSIFY) {
					ontProStepReq = mClassClassifyOntProStepReq;
				} else if (processingStep == COntologyProcessingStep::OPSCONCEPTREALIZE) {
					ontProStepReq = mConceptRealizeOntProStepReq;
				} else if (processingStep == COntologyProcessingStep::OPSROLEREALIZE) {
					ontProStepReq = mRoleRealizeOntProStepReq;
				} else if (processingStep == COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE) {
					ontProStepReq = mSameIndividualsRealizeOntProStepReq;
				}
				return ontProStepReq;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
