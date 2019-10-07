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

#include "COntologyPropertyRoleClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COntologyPropertyRoleClassificationItem::COntologyPropertyRoleClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics) : COntologyClassificationItem(configuration,nextClassificationStatistics) {
				mHierarchyConstructed = false;
				mHierarchyConstructionFailed = false;

			}


			COntologyPropertyRoleClassificationItem::~COntologyPropertyRoleClassificationItem() {
			}


			COntologyPropertyRoleClassificationItem *COntologyPropertyRoleClassificationItem::initClassificationItem(CConcreteOntology *ontology, CRolePropertiesHierarchy *hierarchy, bool classifyDataProperties) {
				onto = ontology;
				mHierarchy = hierarchy;

				currentCalculatingCount = 0;
				mClassifyDataRoles = classifyDataProperties;

				if (!mClassifyDataRoles) {
					mClassifyProcessingStepData = onto->getProcessingSteps()->getOntologyProcessingStepDataVector()->getProcessingStepData(COntologyProcessingStep::OPSOBJECTROPERTYCLASSIFY);
				} else {
					mClassifyProcessingStepData = onto->getProcessingSteps()->getOntologyProcessingStepDataVector()->getProcessingStepData(COntologyProcessingStep::OPSDATAROPERTYCLASSIFY);
				}
				mHierarchyConstructionFailed = !mClassifyProcessingStepData->getProcessingStep()->areAllRequirementsSatisfied(ontology);

				remainingTests = true;

				return this;
			}



			CRolePropertiesHierarchy *COntologyPropertyRoleClassificationItem::getHierarchy() {
				return mHierarchy;
			}

			CTBox *COntologyPropertyRoleClassificationItem::getTBox() {
				return onto->getTBox();
			}
			
			QHash<CSatisfiableCalculationJob*,CPropertyClassificationComputationItem*>* COntologyPropertyRoleClassificationItem::getComputationItemHash()
			{
				return &mWorkItemHash;
			}



			CPropertyRoleClassification* COntologyPropertyRoleClassificationItem::getPropertyRoleClassification() {
				return this;
			}


			bool COntologyPropertyRoleClassificationItem::isHierarchyConstructed() {
				return mHierarchyConstructed;
			}

			COntologyPropertyRoleClassificationItem* COntologyPropertyRoleClassificationItem::setHierarchyConstructionFailed() {
				mHierarchyConstructionFailed = true;
				return this;
			}

			bool COntologyPropertyRoleClassificationItem::isHierarchyConstructionFailed() {
				return mHierarchyConstructionFailed;
			}

			COntologyPropertyRoleClassificationItem* COntologyPropertyRoleClassificationItem::submitHierarchyConstructed() {
				mHierarchyConstructed = true;
				mClassifyProcessingStepData->getProcessingStatus()->setProcessingFlags(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED);
				cint64 errorFlags = COntologyProcessingStatus::PSSUCESSFULL;
				if (mHierarchyConstructionFailed) {
					errorFlags = COntologyProcessingStatus::PSFAILED;
				}
				mClassifyProcessingStepData->getProcessingStatus()->setErrorFlags(errorFlags);
				for (QList<COntologyProcessingRequirement*>::const_iterator it = mRequirementList.constBegin(), itEnd = mRequirementList.constEnd(); it != itEnd; ++it) {
					COntologyProcessingRequirement* ontReq(*it);
					ontReq->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED,errorFlags);
				}
				return this;
			}



			bool COntologyPropertyRoleClassificationItem::isDataRolesClassification() {
				return mClassifyDataRoles;
			}

			bool COntologyPropertyRoleClassificationItem::isObjectRolesClassification() {
				return !mClassifyDataRoles;
			}

			COntologyPropertyRoleClassificationItem* COntologyPropertyRoleClassificationItem::setDataRolesClassification(bool classifyDataRoles) {
				mClassifyDataRoles = classifyDataRoles;
				return this;
			}

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
