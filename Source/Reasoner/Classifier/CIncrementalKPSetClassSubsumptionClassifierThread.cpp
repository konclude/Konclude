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

#include "CIncrementalKPSetClassSubsumptionClassifierThread.h"

namespace Konclude {

	namespace Reasoner {

		namespace Classifier {



			CIncrementalKPSetClassSubsumptionClassifierThread::CIncrementalKPSetClassSubsumptionClassifierThread(CReasonerManager *reasonerManager) 
					: COptimizedKPSetClassSubsumptionClassifierThread(reasonerManager) {

			}
			
			
			CIncrementalKPSetClassSubsumptionClassifierThread::~CIncrementalKPSetClassSubsumptionClassifierThread() {
			}




			void CIncrementalKPSetClassSubsumptionClassifierThread::readCalculationConfig(CCalculationConfigurationExtension *config) {
				COptimizedKPSetClassSubsumptionClassifierThread::readCalculationConfig(config);

			}


			COptimizedKPSetClassOntologyClassificationItem* CIncrementalKPSetClassSubsumptionClassifierThread::createOntologyClassificationItem(CConcreteOntology *ontology, CConfigurationBase *config) {
				return new CIncrementalKPSetClassOntologyClassificationItem(config,statistics);
			}


			CSubsumptionClassifierThread *CIncrementalKPSetClassSubsumptionClassifierThread::scheduleOntologyClassification(CConcreteOntology *ontology, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config) {


				CTaxonomy *taxonomy = createEmptyTaxonomyForOntology(ontology,config);
				CIncrementalKPSetClassOntologyClassificationItem* ontClassItem = (CIncrementalKPSetClassOntologyClassificationItem*)createOntologyClassificationItem(ontology,config);
				ontClassItem->initTaxonomyConcepts(ontology,taxonomy);
				ontItemList.append(ontClassItem);
				processingOntItemList.append(ontClassItem);
				ontItemHash.insert(ontology,ontClassItem);

				readCalculationConfig(ontClassItem->getCalculationConfiguration());


				// get previous classification result
				COntologyIncrementalRevisionData* incOntData = ontology->getIncrementalRevisionData();
				CConcreteOntology* prevClassifiedOnt = incOntData->getPreviousClassesClassifiedOntology();
				bool reclassificationRequired = false;
				CClassConceptClassification* prevClassification = nullptr;
				if (prevClassifiedOnt) {
					prevClassification = prevClassifiedOnt->getClassification()->getClassConceptClassification();
					if (prevClassification) {
						CIndividualDependenceTracking* prevDepIndiTracking = prevClassification->getDependentIndividualsTracking();
						if (prevDepIndiTracking) {
							CConcreteOntology* prevConsOnt = ontology;
							do {
								CConsistence* prevConsData = prevConsOnt->getConsistence();
								if (prevConsData) {
									if (prevDepIndiTracking->areIndividualsAffected(prevConsData->getConsistenceModelData())) {
										reclassificationRequired = true;
									}
								}
								if (prevConsOnt != prevClassifiedOnt) {
									prevConsOnt = prevConsOnt->getIncrementalRevisionData()->getPreviousConsistentOntology();
								}
							} while (!reclassificationRequired && prevConsOnt && prevConsOnt != prevClassifiedOnt);
						} else if (!prevClassification->isIndividualDependentTrackingSet()) {
							reclassificationRequired = true;
						}
					} else {
						reclassificationRequired = true;
					}
				} else {
					reclassificationRequired = true;
				}



				if (!reclassificationRequired) {
					ontClassItem->setPreviousHierarchyReused(true);
					CIndividualDependenceTracking* prevDepIndiTracking = prevClassification->getDependentIndividualsTracking();
					CTaxonomy* prevTaxonomy = prevClassification->getClassConceptTaxonomy();
					if (prevTaxonomy) {

						QHash<CConcept*,CHierarchyNode*>* prevConHierNodeHash = prevTaxonomy->getConceptHierarchyNodeHash();

						for (QHash<CConcept*,CHierarchyNode*>::const_iterator it = prevConHierNodeHash->constBegin(), itEnd = prevConHierNodeHash->constEnd(); it != itEnd; ++it) {
							CConcept* concept = it.key();
							CHierarchyNode* prevHierNode = it.value();
							ontClassItem->incPreviousReusedClassCalculationCount();
							if (prevHierNode->isActive()) {
								CHierarchyNode* hierNode = taxonomy->getHierarchyNode(concept,true);
								QList<CConcept*>* eqConList = prevHierNode->getEquivalentConceptList();
								if (eqConList->count() > 1 && hierNode->getEquivalentConceptList()->count() <= 1) {
									for (QList<CConcept*>::const_iterator eqConIt = eqConList->constBegin(), eqConItEnd = eqConList->constEnd(); eqConIt != eqConItEnd; ++eqConIt) {
										CConcept* eqConcept = *eqConIt;
										if (eqConcept != concept) {
											hierNode->addEquivalentConcept(eqConcept);
										}
									}
									taxonomy->updateNodeEquivalences(hierNode);
								}
							}
						}


						for (QHash<CConcept*,CHierarchyNode*>::const_iterator it = prevConHierNodeHash->constBegin(), itEnd = prevConHierNodeHash->constEnd(); it != itEnd; ++it) {
							CConcept* concept = it.key();
							CHierarchyNode* prevHierNode = it.value();
							if (prevHierNode->isActive()) {
								CHierarchyNode* hierNode = taxonomy->getHierarchyNode(concept,true);
								QSet<CHierarchyNode*>* prevPredSet = prevHierNode->getPredecessorNodeSet();
								for (QSet<CHierarchyNode*>::const_iterator prevPredIt = prevPredSet->constBegin(), prevPredItEnd = prevPredSet->constEnd(); prevPredIt != prevPredItEnd; ++prevPredIt) {
									CHierarchyNode* prevPredNode(*prevPredIt);
									CHierarchyNode* predHierNode = taxonomy->getHierarchyNode(prevPredNode->getOneEquivalentConcept(),true);
									hierNode->addPredecessorNode(predHierNode);
								}
								QSet<CHierarchyNode*>* prevParentSet = prevHierNode->getParentNodeSet();
								for (QSet<CHierarchyNode*>::const_iterator prevParentIt = prevParentSet->constBegin(), prevParentItEnd = prevParentSet->constEnd(); prevParentIt != prevParentItEnd; ++prevParentIt) {
									CHierarchyNode* prevParentNode(*prevParentIt);
									CHierarchyNode* parentHierNode = taxonomy->getHierarchyNode(prevParentNode->getOneEquivalentConcept(),true);
									hierNode->makeParent(parentHierNode);
								}
							}
						}
						

						CClassification* classification = ontology->getClassification();
						if (classification) {
							CClassConceptClassification* classConClassification = ontClassItem->getClassConceptClassification();
							addClassificationStatistics(ontClassItem,classConClassification);
							classConClassification->setClassConceptTaxonomy(taxonomy);
							classification->setClassConceptClassification(classConClassification);
							CClassificationStatisticsCollectionStrings* classifStatCollStrings = ontClassItem->getClassificationStatisticsCollectionStrings();
							classConClassification->setClassificationStatistics(classifStatCollStrings);
							if (prevDepIndiTracking) {
								classConClassification->setDependentIndividualsTracking(prevDepIndiTracking->getCopiedIndividualDependencyTracking());
							}
						}


						ontology->setConceptTaxonomy(taxonomy);
						ontClassItem->setHasRemainingTests(false);
						taxonomy->setTaxonomyComplete(true);

						ontClassItem->submitTaxonomyConstructed();
						ontClassItem->doClassifiedCallback();
						LOG(INFO,getLogDomain(),logTr("Obtained classification of ontology '%1' by reusing previous class hierarchy.").arg(ontology->getTerminologyName()),getLogObject());
					} else {
						reclassificationRequired = true;
					}
				}



				if (reclassificationRequired) {
					if (CConfigDataReader::readConfigBoolean(ontClassItem->getCalculationConfiguration(),"Konclude.Calculation.Classification.IndividualDependenceTracking",true)) {
						ontClassItem->setIndividualDependenceTrackingCollector(new CIndividualDependenceTrackingCollector());
					}


					CPartialPruningTaxonomy *parTax = dynamic_cast<CPartialPruningTaxonomy *>(taxonomy);
					if (parTax) {
						COntologyClassClassificationItem *ontClassItem = (COntologyClassClassificationItem*)ontItemHash.value(ontology);
						parTax->createStatistics(ontClassItem->getClassifierStatistics());
					}

					tryReusePreviousClassesCalculations(ontClassItem,prevClassification);
					createObviousSubsumptionSatisfiableTestingOrder(ontClassItem);

					if (satTestedCount == totalToSatTestClassCount) {
						satTestedCount = 0;
						totalToSatTestClassCount = ontClassItem->getRemainingSatisfiableTestsCount();
						classStartTime.start();
					} else {
						totalToSatTestClassCount += ontClassItem->getRemainingSatisfiableTestsCount();
					}
					++mClassificationCount;
				}
				return this;
			}


			bool CIncrementalKPSetClassSubsumptionClassifierThread::tryReusePreviousClassesCalculations(CIncrementalKPSetClassOntologyClassificationItem *ontClassItem, CClassConceptClassification* prevClassConClassification) {
				CIncrementalKPSetClassOntologyClassificationItem* prevIncKPClassItem = dynamic_cast<CIncrementalKPSetClassOntologyClassificationItem*>(prevClassConClassification);
				while (prevIncKPClassItem && prevIncKPClassItem->getPreviousReusedClassClassification()) {
					prevClassConClassification = prevIncKPClassItem->getPreviousReusedClassClassification();
					prevIncKPClassItem = dynamic_cast<CIncrementalKPSetClassOntologyClassificationItem*>(prevClassConClassification);
				}
				COptimizedKPSetClassOntologyClassificationItem* prevKPClassOntItem = dynamic_cast<COptimizedKPSetClassOntologyClassificationItem*>(prevClassConClassification);
				if (prevKPClassOntItem) {
					QList<COptimizedKPSetClassTestingItem*>* prevConTestItemCont = prevKPClassOntItem->getConceptSatisfiableTestItemContainer();
					for (QList<COptimizedKPSetClassTestingItem*>::const_iterator prevConItemIt = prevConTestItemCont->constBegin(), prevConItemItEnd = prevConTestItemCont->constEnd(); prevConItemIt != prevConItemItEnd; ++prevConItemIt) {
						COptimizedKPSetClassTestingItem* prevConItem = (*prevConItemIt);
						if (!prevConItem->hasIndividualDependenceTracked()) {
							ontClassItem->incPreviousReusedClassCalculationCount();
							COptimizedKPSetClassTestingItem* conItem = ontClassItem->getConceptSatisfiableTestItem(prevConItem->getTestingConcept(),true);
							conItem->setSatisfiableTestedResult(prevConItem->getSatisfiableTestedResult());
							conItem->setSatisfiableTested(true);
							QSet<COptimizedKPSetClassTestingItem*>* prevSubsumerSet = prevConItem->getSubsumingConceptItemSet();
							for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = prevSubsumerSet->constBegin(), itEnd = prevSubsumerSet->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetClassTestingItem* prevSubsumerItem = (*it);
								COptimizedKPSetClassTestingItem* subsumerConItem = ontClassItem->getConceptSatisfiableTestItem(prevSubsumerItem->getTestingConcept(),true);
								conItem->addSubsumingConceptItem(subsumerConItem);
							}
						} else {
							ontClassItem->incNewInitializedClassCalculationCount();
						}
					}
					return true;
				}
				COptimizedSubClassOntologyClassificationItem* prevSubClassOntItem = dynamic_cast<COptimizedSubClassOntologyClassificationItem*>(prevClassConClassification);
				if (prevSubClassOntItem) {
					QList<COptimizedSubClassSatisfiableTestingItem*>* prevConTestItemCont = prevSubClassOntItem->getConceptSatisfiableTestItemContainer();
					for (QList<COptimizedSubClassSatisfiableTestingItem*>::const_iterator prevConItemIt = prevConTestItemCont->constBegin(), prevConItemItEnd = prevConTestItemCont->constEnd(); prevConItemIt != prevConItemItEnd; ++prevConItemIt) {
						COptimizedSubClassSatisfiableTestingItem* prevConItem = (*prevConItemIt);
						if (!prevConItem->hasIndividualDependenceTracked()) {
							ontClassItem->incPreviousReusedClassCalculationCount();
							COptimizedKPSetClassTestingItem* conItem = ontClassItem->getConceptSatisfiableTestItem(prevConItem->getSatisfiableConcept(),true);
							conItem->setSatisfiableTestedResult(prevConItem->getSatisfiableTestedResult());
							conItem->setSatisfiableTested(true);
							QSet<COptimizedSubClassSatisfiableTestingItem*>* prevSubsumerSet = prevConItem->getSubsumingConceptItemSet();
							for (QSet<COptimizedSubClassSatisfiableTestingItem*>::const_iterator it = prevSubsumerSet->constBegin(), itEnd = prevSubsumerSet->constEnd(); it != itEnd; ++it) {
								COptimizedSubClassSatisfiableTestingItem* prevSubsumerItem = (*it);
								COptimizedKPSetClassTestingItem* subsumerConItem = ontClassItem->getConceptSatisfiableTestItem(prevSubsumerItem->getSatisfiableConcept(),true);
								conItem->addSubsumingConceptItem(subsumerConItem);
							}
						} else {
							ontClassItem->incNewInitializedClassCalculationCount();
						}
					}
					return true;
				}
				return false;
			}


			bool CIncrementalKPSetClassSubsumptionClassifierThread::addClassificationStatistics(COntologyClassClassificationItem *ontClassItem, CClassConceptClassification* classConClassification) {
				CClassificationStatisticsCollectionStrings* classifStatCollStrings = ontClassItem->getClassificationStatisticsCollectionStrings();
				CClassifierStatistics* classifierStats = ontClassItem->getClassifierStatistics();
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-total-satisfiable-test-count",classifierStats->getTotalSatisfiableCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-satisfiable-tested-count",classifierStats->getTestedSatisfiableCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-satisfiable-pseudo-model-merged-tested-count",classifierStats->getPseudoModelMergedTestedSatisfiableCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-satisfiable-derivation-tested-count",classifierStats->getDerivatedTestedSatisfiableCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-satisfiable-calculated-tested-count",classifierStats->getCalculatedTestedSatisfiableCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-satisfiable-pruned-tested-count",classifierStats->getPrunedTestedSatisfiableCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-satisfiable-told-tested-count",classifierStats->getToldSatisfiableCount()-classifierStats->getCalculatedTestedSatisfiableCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-total-subsumption-test-count",classifierStats->getTotalSatisfiableCount()*(classifierStats->getTotalSatisfiableCount()-1));
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-subsumption-tested-count",classifierStats->getTestedSubsumptionCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-subsumption-pseudo-model-merged-tested-count",classifierStats->getPseudoModelMergedTestedSubsumptionCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-subsumption-subclass-tested-count",classifierStats->getSubclassTestedSubsumptionCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-subsumption-subclass-identifier-occur-tested-count",classifierStats->getSubclassIdentifierOccurTestedSubsumptionCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-subsumption-derivation-tested-count",classifierStats->getDerivatedTestedSubsumptionCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-subsumption-calculated-tested-count",classifierStats->getCalculatedTestedSubsumptionCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-subsumption-pruned-tested-count",classifierStats->getPrunedTestedSubsumptionCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-subsumption-told-tested-count",classifierStats->getToldSubsumptionCount()-classifierStats->getCalculatedTestedSubsumptionCount());
				classifStatCollStrings->addProcessingStatistics("incremental-class-classification-time",ontClassItem->getInitializationTime()->elapsed());

				COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem = dynamic_cast<COptimizedKPSetClassOntologyClassificationItem*>(ontClassItem);
				if (optKPSetClassificationItem) {
					addIndividualDependencyTrackingStatistics(optKPSetClassificationItem,"incremental-class-classification");
				}

				COntologyProcessingStatistics* ontProcStats = ontClassItem->getClassificationProcessingStep()->getProcessingStatistics(true);
				ontProcStats->setProcessingCountStatisticValue("incremental-class-classification-time",ontClassItem->getInitializationTime()->elapsed());

				CIncrementalKPSetClassOntologyClassificationItem* incOntClassItem = dynamic_cast<CIncrementalKPSetClassOntologyClassificationItem*>(ontClassItem);
				if (incOntClassItem) {
					ontProcStats->setProcessingCountStatisticValue("incremental-class-classification-previous-hierarchy-reused-count",incOntClassItem->isPreviousHierarchyReused());
					ontProcStats->setProcessingCountStatisticValue("incremental-class-classification-previously-reused-class-calculation-count",incOntClassItem->getPreviousReusedClassCalculationCount());
					ontProcStats->setProcessingCountStatisticValue("incremental-class-classification-newly-initialized-class-calculation-count",incOntClassItem->getNewInitializedClassCalculationCount());
				}
				return true;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
