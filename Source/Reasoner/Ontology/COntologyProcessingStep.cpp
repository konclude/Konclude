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

#include "COntologyProcessingStep.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyProcessingStep::COntologyProcessingStep(PROCESSINGSTEPTYPE processingType, cint64 id, PROCESSORTYPE processorType) {
				mProcessingType = processingType;
				mProcessorType = processorType;
				mProcessingStepID = id;
			}

			COntologyProcessingStep::PROCESSINGSTEPTYPE COntologyProcessingStep::getOntologyProcessingType() {
				return mProcessingType;
			}

			COntologyProcessingStep* COntologyProcessingStep::addProcessingRequirement(COntologyProcessingRequirement* requirement) {
				mProcessingRequirementList.append(requirement);
				return this;
			}

			cint64 COntologyProcessingStep::getProcessingStepID() {
				return mProcessingStepID;
			}

			COntologyProcessingStep::PROCESSORTYPE COntologyProcessingStep::getProcessorType() {
				return mProcessorType;
			}

			QList<COntologyProcessingRequirement*>* COntologyProcessingStep::getProcessingRequirementList() {
				return &mProcessingRequirementList;
			}

			bool COntologyProcessingStep::areAllRequirementsSatisfied(COntology* ontology) {
				for (QList<COntologyProcessingRequirement*>::const_iterator it = mProcessingRequirementList.constBegin(), itEnd = mProcessingRequirementList.constEnd(); it != itEnd; ++it) {
					COntologyProcessingRequirement* ontReq(*it);
					if (!ontReq->isRequirementSatisfied(ontology)) {
						return false;
					}
				}
				return true;
			}


			const QString COntologyProcessingStep::getProcessingStepName() {
				QString stepName;
				if (mProcessingType == OPSACTIVECOUNT) {
					stepName = QString("Active-Entity-Counting");
				} else if (mProcessingType == OPSBUILD) {
					stepName = QString("Representation-Building");
				} else if (mProcessingType == OPSPREPROCESS) {
					stepName = QString("Preprocessing");
				} else if (mProcessingType == OPSCONSISTENCY) {
					stepName = QString("Consistency-Checking");
				} else if (mProcessingType == OPSPRECOMPUTESATURATION) {
					stepName = QString("Saturation-Precomputing");
				} else if (mProcessingType == OPSPRECOMPUTEINDIVIDUAL) {
					stepName = QString("Individual-Precomputing");
				} else if (mProcessingType == OPSPRECOMPUTECYCLES) {
					stepName = QString("Cycles-Precomputing");
				} else if (mProcessingType == OPSCLASSCLASSIFY) {
					stepName = QString("Classification");
				} else if (mProcessingType == OPSCONCEPTREALIZE) {
					stepName = QString("Types-Realization");
				} else if (mProcessingType == OPSSAMEINDIVIDUALSREALIZE) {
					stepName = QString("Same-Individual-Realization");
				} else if (mProcessingType == OPSROLEREALIZE) {
					stepName = QString("Role-Realization");
				}
				return stepName;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
