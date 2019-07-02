/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "COntologyClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COntologyClassificationItem::COntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics) {
				statistics = new CClassifierStatistics(nextClassificationStatistics);
				config = new CCalculationConfigurationExtension(configuration,0);

				classCalcSupport = 0;
				possSubclassSubSumTaxonomy = 0;
				mClassConClassification = new CClassConceptClassification();
				mClassificationStatCollStrings = new CClassificationStatisticsCollectionStrings();

				mConfCollectProcessStatistics = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Classification.CollectProcessStatistics",true);;
				
				mClassifyProcessingStepData = nullptr;
				mTaxonomyConstructed = false;
				mTaxonomyConstructionFailed = false;

				mPrecSatSubsumerExtractor = nullptr;
			}


			COntologyClassificationItem::~COntologyClassificationItem() {
				delete mPrecSatSubsumerExtractor;
				delete statistics;
				delete config;
				if (possSubclassSubSumTaxonomy) {
					delete possSubclassSubSumTaxonomy;
				}
				qDeleteAll(possPredIdentifierOccurSetContainer);
			}



			CPrecomputedSaturationSubsumerExtractor* COntologyClassificationItem::getPrecomputedSaturationSubsumerExtractor(bool create) {
				if (!mPrecSatSubsumerExtractor && create) {
					mPrecSatSubsumerExtractor = new CPrecomputedSaturationSubsumerExtractor(onto);
				}
				return mPrecSatSubsumerExtractor;
			}


			COntologyClassificationItem *COntologyClassificationItem::initTaxonomyConcepts(CConcreteOntology *ontology, CTaxonomy *taxonomy) {
				onto = ontology;
				tax = taxonomy;
				CABox *aBox = onto->getABox();
				CTBox *tBox = onto->getTBox();

				currentCalculatingCount = 0;

				mClassifyProcessingStepData = onto->getProcessingSteps()->getOntologyProcessingStepDataVector()->getProcessingStepData(COntologyProcessingStep::OPSCLASSCLASSIFY);
				mTaxonomyConstructionFailed = !mClassifyProcessingStepData->getProcessingStep()->areAllRequirementsSatisfied(ontology);

				remainingTests = true;
				mOnlySubClassCompleteDefinitions = false;

				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();

				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					for (CBOXSET<CConcept*>::const_iterator it = conceptHash->constBegin(), itEnd = conceptHash->constEnd(); it != itEnd; ++it) {
						CConcept *concept = (*it);
						if (concept && concept != topConcept && concept != bottomConcept) {
							// concept which has to be inserted in the taxonomy
							notSatTestedConList.append(concept);
						}
					}
				}

				statistics->incTotalSatisfiableTestCount(notSatTestedConList.count());

				return this;
			}


			bool COntologyClassificationItem::hasNotSatisfiableTestedConcepts() {
				return !notSatTestedConList.isEmpty();
			}

			CConcept *COntologyClassificationItem::takeNextNotSatisfiableTestedConcept() {
				CConcept *con = 0;
				if (!notSatTestedConList.isEmpty()) {
					con = notSatTestedConList.takeFirst();
				}
				return con;
			}


			QList<CConcept *> *COntologyClassificationItem::getNotSatisfiableTestedConceptList() {
				return &notSatTestedConList;
			}


			COntologyClassificationItem *COntologyClassificationItem::addNotSatisfiableTestedConcept(CConcept *concept) {
				notSatTestedConList.append(concept);
				return this;
			}

			CConcreteOntology *COntologyClassificationItem::getOntology() {
				return onto;
			}

			CTaxonomy *COntologyClassificationItem::getTaxonomy() {
				return tax;
			}

			CTBox *COntologyClassificationItem::getTBox() {
				return onto->getTBox();
			}
			
			CCalculationConfigurationExtension *COntologyClassificationItem::getCalculationConfiguration() {
				return config;
			}

			QList<CHierarchyNode *> *COntologyClassificationItem::getAllNodeList() {
				return &allNodeList;
			}

			QList<CHierarchyNode *> *COntologyClassificationItem::getProcessingNodeList() {
				return &processingNodeList;
			}

			QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *> * COntologyClassificationItem::getWorkItemHash()
			{
				return &workItemHash;
			}


			COntologyClassificationItem *COntologyClassificationItem::incCurrentCalculatingCount(qint64 incCount) {
				currentCalculatingCount += incCount;
				return this;
			}

			COntologyClassificationItem *COntologyClassificationItem::decCurrentCalculatingCount(qint64 decCount) {
				currentCalculatingCount -= decCount;
				return this;
			}

			COntologyClassificationItem *COntologyClassificationItem::setGoneOutRemainingTests(bool moreTests) {
				remainingTests = moreTests;
				return this;
			}
			
			bool COntologyClassificationItem::hasRemainingTests() {
				return remainingTests || currentCalculatingCount > 0;
			}

			qint64 COntologyClassificationItem::getCurrentCalculatingCount() {
				return currentCalculatingCount;
			}


			COntologyClassificationItem *COntologyClassificationItem::doClassifiedCallback() {
				classifiedCallbackList.doCallback();
				return this;
			}

			COntologyClassificationItem *COntologyClassificationItem::addClassifiedCallback(CCallbackData *callback) {
				callback->setCallbackDataContext(new CClassifiedCallbackDataContext(tax,onto));
				classifiedCallbackList.addCallbackData(callback);
				if (!hasRemainingTests()) {
					doClassifiedCallback();
				}
				return this;
			}

			qint64 COntologyClassificationItem::getCalcedSatisfiableCount() {
				return statistics->getTestedSatisfiableCount();
			}

			qint64 COntologyClassificationItem::getCalcedSubsumptionCount() {
				return statistics->getTestedSubsumptionCount();
			}

			CClassifierStatistics *COntologyClassificationItem::getClassifierStatistics() {
				return statistics;
			}

			QHash<CConcept *,CConcept *> *COntologyClassificationItem::getDefferedNotSubsumptionRelationHash() {
				return &defferedNotSubsumptionHash;
			}

			QHash<CConcept *,CConcept *> *COntologyClassificationItem::getDefferedSubsumptionRelationHash() {
				return &defferedSubsumptionHash;
			}

			QHash<CConcept *,CConcept *> *COntologyClassificationItem::getDefferedDisjointRelationHash() {
				return &defferedDisjointHash;
			}

			CClassificationCalculationSupport *COntologyClassificationItem::getClassificationCalculationSupport() {
				return classCalcSupport;
			}

			COntologyClassificationItem *COntologyClassificationItem::setClassificationCalculationSupport(CClassificationCalculationSupport *calculationSupport) {
				classCalcSupport = calculationSupport;
				return this;
			}

			QSet<QPair<CConcept *,CConcept *> > *COntologyClassificationItem::getPossibleSubclassSubsumptionConceptSet() {
				return &possSubclassSubSumConceptSet;
			}


			QSet<CConcept *> *COntologyClassificationItem::getSubclassConceptSet() {
				return &subclassConceptSet;
			}


			CTaxonomy *COntologyClassificationItem::getPossibleSubclassSubsumptionTaxonomy() {
				return possSubclassSubSumTaxonomy;
			}

			COntologyClassificationItem *COntologyClassificationItem::setPossibleSubclassSubsumptionTaxonomy(CTaxonomy *tax) {
				possSubclassSubSumTaxonomy = tax;
				return this;
			}

			QSet<QPair<CConcept *,CConceptNegationPair> > *COntologyClassificationItem::getConceptNecessaryOccurSubclassIdentifiersSet() {
				return &conceptNecessaryOccurSubclassIdentifiersSet;
			}

			QHash<CConcept *,CConceptNegationPair> *COntologyClassificationItem::getConceptNecessaryOccurSubclassIdentifiersHash() {
				return &conceptNecessaryOccurSubclassIdentifiersHash;
			}

			QHash<CConceptNegationPair,QSet<CConceptNegationPair> *> *COntologyClassificationItem::getConceptPredecessorPossibleOccurSubclassIdentifierSetHash() {
				return &possPredIdentifierOccurSetHash;
			}

			QList<QSet<CConceptNegationPair> *> *COntologyClassificationItem::getConceptPredecessorPossibleOccurSubclassIdentifierSetContainer() {
				return &possPredIdentifierOccurSetContainer;
			}

			QHash<QPair<CHierarchyNode *,CHierarchyNode *>,double> *COntologyClassificationItem::getSubsumptionProbabilisticHash() {
				return &subsumProbabilisticHash;
			}


			CClassConceptClassification* COntologyClassificationItem::getClassConceptClassification() {
				return mClassConClassification;
			}

			CClassificationStatisticsCollectionStrings* COntologyClassificationItem::getClassificationStatisticsCollectionStrings() {
				return mClassificationStatCollStrings;
			}


			CClassificationCalculationStatisticsCollection* COntologyClassificationItem::getCalculationStatisticsCollection() {
				CClassificationCalculationStatisticsCollection* statColl = nullptr;
				if (!mReuseStatCollList.isEmpty()) {
					statColl = mReuseStatCollList.takeFirst();
				}
				if (!statColl && mClassificationStatCollStrings) {
					statColl = mClassificationStatCollStrings->createCalculationStatisticsCollection();
				}
				return statColl;
			}

			COntologyClassificationItem* COntologyClassificationItem::reuseCalculationStatisticsCollection(CClassificationCalculationStatisticsCollection* statColl) {
				mReuseStatCollList.append(statColl);
				return this;
			}


			bool COntologyClassificationItem::hasOnlySubClassCompleteDefinitions() {
				return mOnlySubClassCompleteDefinitions;
			}

			COntologyClassificationItem* COntologyClassificationItem::setOnlySubClassCompleteDefinitions(bool onlyCompleteDefinitions) {
				mOnlySubClassCompleteDefinitions = onlyCompleteDefinitions;
				return this;
			}

			QHash<CConcept*,CConcept*>* COntologyClassificationItem::getPossibleConceptSubsumptionOccurrenceHash() {
				return &mPossConSubsumOccurrHash;
			}

			QSet< QPair<CConcept*,CConcept*> >* COntologyClassificationItem::getPossibleConceptSubsumptionOccurrenceSet() {
				return &mPossConSubsumOccurrSet;
			}

			bool COntologyClassificationItem::isCollectProcessStatisticsActivated() {
				return mConfCollectProcessStatistics;
			}

			QList<COntologyProcessingRequirement*> COntologyClassificationItem::getOntologyProcessingRequirementList() {
				return mRequirementList;
			}

			COntologyClassificationItem* COntologyClassificationItem::addOntologyProcessingRequirements(const QList<COntologyProcessingRequirement*>& reqList) {
				mRequirementList.append(reqList);
				return this;
			}

			bool COntologyClassificationItem::isTaxonomyConstructed() {
				return mTaxonomyConstructed;
			}

			COntologyClassificationItem* COntologyClassificationItem::setTaxonomyConstructionFailed() {
				mTaxonomyConstructionFailed = true;
				return this;
			}

			bool COntologyClassificationItem::isTaxonomyConstructionFailed() {
				return mTaxonomyConstructionFailed;
			}

			COntologyClassificationItem* COntologyClassificationItem::submitTaxonomyConstructed() {
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
