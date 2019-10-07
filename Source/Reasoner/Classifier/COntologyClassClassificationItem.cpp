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

#include "COntologyClassClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COntologyClassClassificationItem::COntologyClassClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics) : COntologyClassificationItem(configuration,nextClassificationStatistics) {

				mTaxonomyConstructed = false;
				mTaxonomyConstructionFailed = false;

				mPrecSatSubsumerExtractor = nullptr;

			}


			COntologyClassClassificationItem::~COntologyClassClassificationItem() {
				delete mPrecSatSubsumerExtractor;
			}


			CPrecomputedSaturationSubsumerExtractor* COntologyClassClassificationItem::getPrecomputedSaturationSubsumerExtractor(bool create) {
				if (!mPrecSatSubsumerExtractor && create) {
					mPrecSatSubsumerExtractor = new CPrecomputedSaturationSubsumerExtractor(onto);
				}
				return mPrecSatSubsumerExtractor;
			}


			COntologyClassClassificationItem *COntologyClassClassificationItem::initTaxonomyConcepts(CConcreteOntology *ontology, CTaxonomy *taxonomy) {
				onto = ontology;
				tax = taxonomy;
				CABox *aBox = onto->getABox();
				CTBox *tBox = onto->getTBox();

				currentCalculatingCount = 0;

				mClassifyProcessingStepData = onto->getProcessingSteps()->getOntologyProcessingStepDataVector()->getProcessingStepData(COntologyProcessingStep::OPSCLASSCLASSIFY);
				mTaxonomyConstructionFailed = !mClassifyProcessingStepData->getProcessingStep()->areAllRequirementsSatisfied(ontology);

				remainingTests = true;

				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();

				cint64 satConCount = 0;
				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					for (CBOXSET<CConcept*>::const_iterator it = conceptHash->constBegin(), itEnd = conceptHash->constEnd(); it != itEnd; ++it) {
						CConcept *concept = (*it);
						if (concept && concept != topConcept && concept != bottomConcept) {
							// concept which has to be inserted in the taxonomy
							++satConCount;
						}
					}
				}

				statistics->incTotalSatisfiableTestCount(satConCount);

				return this;
			}


			CTaxonomy *COntologyClassClassificationItem::getTaxonomy() {
				return tax;
			}

			CTBox *COntologyClassClassificationItem::getTBox() {
				return onto->getTBox();
			}
			

			QHash<CSatisfiableCalculationJob *, CClassClassificationComputationItem *> * COntologyClassClassificationItem::getWorkItemHash()
			{
				return &workItemHash;
			}


			CClassConceptClassification* COntologyClassClassificationItem::getClassConceptClassification() {
				return this;
			}



			bool COntologyClassClassificationItem::isTaxonomyConstructed() {
				return mTaxonomyConstructed;
			}

			COntologyClassClassificationItem* COntologyClassClassificationItem::setTaxonomyConstructionFailed() {
				mTaxonomyConstructionFailed = true;
				return this;
			}

			bool COntologyClassClassificationItem::isTaxonomyConstructionFailed() {
				return mTaxonomyConstructionFailed;
			}

			COntologyClassClassificationItem* COntologyClassClassificationItem::submitTaxonomyConstructed() {
				mTaxonomyConstructed = true;
				mClassifyProcessingStepData->getProcessingStatus()->setProcessingFlags(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED);
				cint64 errorFlags = COntologyProcessingStatus::PSSUCESSFULL;
				if (mTaxonomyConstructionFailed) {
					errorFlags = COntologyProcessingStatus::PSFAILED;
				}
				mClassifyProcessingStepData->getProcessingStatus()->setErrorFlags(errorFlags);
				for (QList<COntologyProcessingRequirement*>::const_iterator it = mRequirementList.constBegin(), itEnd = mRequirementList.constEnd(); it != itEnd; ++it) {
					COntologyProcessingRequirement* ontReq(*it);
					ontReq->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED,errorFlags);
				}
				return this;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
