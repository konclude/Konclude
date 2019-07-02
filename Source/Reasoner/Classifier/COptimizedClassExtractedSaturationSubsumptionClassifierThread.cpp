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

#include "COptimizedClassExtractedSaturationSubsumptionClassifierThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedClassExtractedSaturationSubsumptionClassifierThread::COptimizedClassExtractedSaturationSubsumptionClassifierThread(CReasonerManager *reasonerManager) 
					: CSubsumptionClassifierThread(reasonerManager), CLogIdentifier("::Konclude::Reasoner::Kernel::Classifier::OptimizedClassExtractedSaturationSubsumptionClassifierThread",this) {
				mClassificationCount = 0;
				satTestedCount = 0;
				totalToTestCount = 0;
			}


			COptimizedClassExtractedSaturationSubsumptionClassifierThread::~COptimizedClassExtractedSaturationSubsumptionClassifierThread() {
			}


			CTaxonomy *COptimizedClassExtractedSaturationSubsumptionClassifierThread::createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config) {
				CTBox *tBox = ontology->getTBox();
				CConceptVector *conVec = tBox->getConceptVector();
				CConcept *topConcept = conVec->getData(1);
				CConcept *bottomConcept = conVec->getData(0);
				CPartialPruningTaxonomy *tax = new CPartialPruningTaxonomy(topConcept,bottomConcept);
				return tax->readConfig(config);
			}


			void COptimizedClassExtractedSaturationSubsumptionClassifierThread::readCalculationConfig(CCalculationConfigurationExtension *config) {
				mTransitiveReductionAddAllPredecessors = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Classification.TransitiveReduction.AddAllPredecessors",true);
			}


			
			CSubsumptionClassifierThread *COptimizedClassExtractedSaturationSubsumptionClassifierThread::scheduleOntologyClassification(CConcreteOntology *ontology, CTaxonomy *taxonomy, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config) {

				COptimizedClassExtractedSaturationOntologyClassificationItem *ontClassItem = new COptimizedClassExtractedSaturationOntologyClassificationItem(config,statistics);
				ontClassItem->initTaxonomyConcepts(ontology,taxonomy);
				ontItemList.append(ontClassItem);
				processingOntItemList.append(ontClassItem);
				ontItemHash.insert(ontology,ontClassItem);

				CCalculationConfigurationExtension* calcConfig = ontClassItem->getCalculationConfiguration();

				readCalculationConfig(calcConfig);

				if (CConfigDataReader::readConfigBoolean(ontClassItem->getCalculationConfiguration(),"Konclude.Calculation.Classification.IndividualDependenceTracking",true)) {
					ontClassItem->setIndividualDependenceTrackingCollector(new CIndividualDependenceTrackingCollector());
				}

				CPartialPruningTaxonomy *parTax = dynamic_cast<CPartialPruningTaxonomy *>(taxonomy);
				if (parTax) {
					COntologyClassificationItem *ontClassItem = ontItemHash.value(ontology);
					parTax->createStatistics(ontClassItem->getClassifierStatistics());
				}

				readSubsumptionFromPrecomputedSaturation(ontClassItem);

				if (satTestedCount == totalToTestCount) {
					satTestedCount = 0;
					totalToTestCount = 1;
					classStartTime.start();
				} else {
					totalToTestCount += 1;
				}
				++mClassificationCount;
				return this;
			}




			void COptimizedClassExtractedSaturationSubsumptionClassifierThread::readSubsumptionFromPrecomputedSaturation(COptimizedClassExtractedSaturationOntologyClassificationItem* ontClassItem) {
				CTBox *tBox = ontClassItem->getOntology()->getDataBoxes()->getTBox();

				CConcreteOntology *onto = ontClassItem->getOntology();
				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();

				CConceptVector* conVec = tBox->getConceptVector();

				LOG(INFO,getLogDomain(),logTr("Read subsumers from precomputed saturation for ontology '%1'.").arg(ontClassItem->getOntology()->getTerminologyName()),getLogObject());


				QHash<CConcept*,COptimizedClassExtractedSaturationSatisfiableTestingItem*>* conceptSatItemHash = ontClassItem->getClassSatisfiableTestItemHash();
				QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* satTestItemContainer = ontClassItem->getClassSatisfiableTestItemList();

				COptimizedClassExtractedSaturationSatisfiableTestingItem* topItem = ontClassItem->getClassSatisfiableTestItem(topConcept,true);
				COptimizedClassExtractedSaturationSatisfiableTestingItem* bottomItem = ontClassItem->getClassSatisfiableTestItem(bottomConcept,true);

				ontClassItem->initTopBottomSatisfiableTestingItems(topItem,bottomItem);
				CTaxonomy *tax = ontClassItem->getTaxonomy();
				CHierarchyNode* bottomHierNode = tax->getBottomHierarchyNode();

				CPrecomputedSaturationSubsumerExtractor precSatSubsumerExtractor(onto);

				QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*> extClassItemList;

				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					cint64 conCount = conVec->getItemCount();
					for (cint64 conIdx = 1; conIdx < conCount; ++conIdx) {
						CConcept* concept = conVec->getData(conIdx);

						if (concept) {
							if (concept->hasClassName() && conceptHash->contains(concept)) {
								bool unsatFlag = false;
								bool insuffFlag = false;
								bool incProcFlag = false;
								precSatSubsumerExtractor.getConceptFlags(concept,&unsatFlag,&insuffFlag,&incProcFlag);

								if (unsatFlag) {
									bottomHierNode->addEquivalentConcept(concept);
								} else {
									CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
									CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
									if (!conRefSatLinking) {
										conRefSatLinking = new CConceptSaturationReferenceLinkingData();
										conProcData->setConceptReferenceLinking(conRefSatLinking);
									}
									COptimizedClassExtractedSaturationSatisfiableTestingItem* classItem = ontClassItem->getClassSatisfiableTestItem(concept,true);
									conRefSatLinking->setClassifierReferenceLinkingData(classItem);

									extClassItemList.append(classItem);
								}
							}
						}
					}
				}


				for (QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>::const_iterator it = extClassItemList.constBegin(), itEnd = extClassItemList.constEnd(); it != itEnd; ++it) {
					COptimizedClassExtractedSaturationSatisfiableTestingItem* classExtTestItem(*it);
					CConcept* concept = classExtTestItem->getTestingConcept();
					cint64 subsumerCount = precSatSubsumerExtractor.getSubsumerCount(concept);
					classExtTestItem->setExtractionSubsumerCount(subsumerCount);
				}

				ontClassItem->decCurrentCalculatingCount();
				++satTestedCount;
			}


			COptimizedClassExtractedSaturationSatisfiableTestingItem* getItemForConcept(CConcept* concept) {
				CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
				if (conProcData) {
					CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
					if (conRefSatLinking) {
						COptimizedClassExtractedSaturationSatisfiableTestingItem* classItem = (COptimizedClassExtractedSaturationSatisfiableTestingItem*)conRefSatLinking->getClassifierReferenceLinkingData();
						return classItem;
					}
				}
				return nullptr;
			}

			bool isDirectSubsumer(CConcept* concept, CConcept* subsumerConcept) {
				COptimizedClassExtractedSaturationSatisfiableTestingItem* classItem = getItemForConcept(concept);;
				if (classItem) {
					cint64 conSubsumerCount = classItem->getExtractionSubsumerCount();					
					COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumerClassItem = getItemForConcept(subsumerConcept);
					if (subsumerClassItem) {
						cint64 subsumerConSubsumerCount = subsumerClassItem->getExtractionSubsumerCount();
						if (subsumerConSubsumerCount == conSubsumerCount-1) {
							return true;
						}
					}
				}
				return false;
			}


			QString COptimizedClassExtractedSaturationSubsumptionClassifierThread::getStatusString() {
				statUpdateMutex.lock();
				QString string = statusString;
				string.detach();
				statUpdateMutex.unlock();
				return string;
			}



			CClassificationProgress* COptimizedClassExtractedSaturationSubsumptionClassifierThread::getClassificationProgress() {
				if (satTestedCount != totalToTestCount) {					
					cint64 testedClasses = satTestedCount;
					cint64 totalClasses = totalToTestCount;
					cint64 elapsedTime = classStartTime.elapsed();
					double remainingMilliSeconds = 0.;
					double progessPercent = 0.;
					if (elapsedTime <= 0 && testedClasses > 0) {
						remainingMilliSeconds = 0.;
					} else {
						remainingMilliSeconds = ((double)elapsedTime / (double)satTestedCount) * (totalToTestCount-testedClasses);
					}
					if (totalClasses <= 0) {
						progessPercent = 0.;
					} else {
						progessPercent = ((double)testedClasses / (double)totalClasses) * 100.;
					}
					cint64 classificCount = mClassificationCount;
					mClassifyProgess.setClassificationCount(classificCount);
					mClassifyProgess.setProgessPercent(progessPercent);
					mClassifyProgess.setRemainingMilliSeconds(remainingMilliSeconds);
					mClassifyProgess.setTestedClasses(testedClasses);
					mClassifyProgess.setTotalClasses(totalClasses);
				} else {
					mClassifyProgess.setProgessPercent(0.);
					mClassifyProgess.setRemainingMilliSeconds(0.);
					mClassifyProgess.setTestedClasses(satTestedCount);
					mClassifyProgess.setTotalClasses(totalToTestCount);
				}
				return &mClassifyProgess;
			}



			bool COptimizedClassExtractedSaturationSubsumptionClassifierThread::createNextSubsumtionTest() {


				COntologyClassificationItem *loopOntClassItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !processingOntItemList.isEmpty()) {
					COntologyClassificationItem *ontClassItem = processingOntItemList.first();

					if (ontClassItem == loopOntClassItem) {
						// don't run into infinite loop without doing something
						break;
					}

					COptimizedClassExtractedSaturationOntologyClassificationItem *optSubClassItem = (COptimizedClassExtractedSaturationOntologyClassificationItem *)ontClassItem;


					if (!workTestCreated) {
						if (optSubClassItem || ontClassItem->isTaxonomyConstructionFailed()) {
							finishOntologyClassification(ontClassItem);
							processingOntItemList.removeFirst();
						} else {
							processingOntItemList.removeFirst();
							processingOntItemList.append(ontClassItem);
							loopOntClassItem = ontClassItem;
						}
					}
				}
				return workTestCreated;
			}




			bool COptimizedClassExtractedSaturationSubsumptionClassifierThread::interpreteTestResults(CTestCalculatedCallbackEvent *testResult) {
				return false;
			}



			bool itemSortLessThan(const COptimizedClassExtractedSaturationSatisfiableTestingItem* item1, const COptimizedClassExtractedSaturationSatisfiableTestingItem* item2) {
				return item1->getExtractionSubsumerCount() < item2->getExtractionSubsumerCount();
			}



			bool COptimizedClassExtractedSaturationSubsumptionClassifierThread::finishOntologyClassification(COntologyClassificationItem *ontClassItem) {

				if (!ontClassItem->isTaxonomyConstructed() && !ontClassItem->isTaxonomyConstructionFailed()) {
					CTaxonomy *taxonomy = ontClassItem->getTaxonomy();
					CConcreteOntology *ontology = ontClassItem->getOntology();

					LOG(INFO,getLogDomain(),logTr("Building class taxonomy for ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

#ifdef OPTIMIZEDCLASSEXTRACTEDSATURATIONCLASSIFIERDEBUGLOGOUTPUT
					LOG(INFO,getLogDomain(),QString("Building Class Taxonomy"),this);
#endif


					// build taxonomy

					COptimizedClassExtractedSaturationOntologyClassificationItem *optSubClassItem = (COptimizedClassExtractedSaturationOntologyClassificationItem *)ontClassItem;
					QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* satItemList = optSubClassItem->getClassSatisfiableTestItemList();

					COptimizedClassExtractedSaturationSatisfiableTestingItem* bottomItem = optSubClassItem->getBottomConceptSatisfiableTestItem();

					CTaxonomy *tax = optSubClassItem->getTaxonomy();

					QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*> itemList;

					COptimizedClassExtractedSaturationSatisfiableTestingItem* topItem = optSubClassItem->getTopConceptSatisfiableTestItem();
					CHierarchyNode* topHierNode = tax->getTopHierarchyNode();
					CHierarchyNode* bottomHierNode = tax->getBottomHierarchyNode();

					CPrecomputedSaturationSubsumerExtractor precSatSubsumerExtractor(ontology);



					cint64 topSubsumingItemCount = topItem->getExtractionSubsumerCount();
					topItem->setSatisfiableConceptHierarchyNode(topHierNode);
					if (topSubsumingItemCount > 0) {
						precSatSubsumerExtractor.extractSubsumers(topItem->getTestingConcept(),topItem);
						QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* subsumingConceptItemList = topItem->sortSubsumingConceptItemList();
						for (QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
							COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
							// mark as equivalent
							subsumedConceptItem->setEquivalentItem(true);
							topHierNode->addEquivalentConcept(subsumedConceptItem->getTestingConcept());
							tax->updateNodeEquivalences(topHierNode);
						}
					}


					qSort(satItemList->begin(),satItemList->end(),itemSortLessThan);


					for (QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>::const_iterator it = satItemList->constBegin(), itEnd = satItemList->constEnd(); it != itEnd; ++it) {
						COptimizedClassExtractedSaturationSatisfiableTestingItem* item = *it;
						if (!item->isEquivalentItem() && item != bottomItem && item != topItem) {
							CHierarchyNode* itemNode = tax->getHierarchyNode(item->getTestingConcept(),true);
							item->setSatisfiableConceptHierarchyNode(itemNode);
							cint64 itemSubsumingCount = item->getExtractionSubsumerCount();
							itemList.append(item);
							if (itemSubsumingCount > topSubsumingItemCount) {

								CConcept* directSubsumerConcept = precSatSubsumerExtractor.getDirectSubsumerCount(item->getTestingConcept(),&isDirectSubsumer);
								bool requiresDetailedSubsumerSearch = true;
								if (directSubsumerConcept) {
									COptimizedClassExtractedSaturationSatisfiableTestingItem* directSubsumerItem = getItemForConcept(directSubsumerConcept);
									if (!directSubsumerItem->isEquivalentItem()) {
										item->setDirectSubsumer(directSubsumerItem);
										requiresDetailedSubsumerSearch = false;
									}
								} 
								
								if (requiresDetailedSubsumerSearch) {
									precSatSubsumerExtractor.extractSubsumers(item->getTestingConcept(),item);
									QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* subsumingConceptItemList = item->sortSubsumingConceptItemList();
									for (QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
										COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
										cint64 subsumedItemSubsumingCount = subsumedConceptItem->getExtractionSubsumerCount();
										if (subsumedItemSubsumingCount == itemSubsumingCount) {
											// mark as equivalent
											subsumedConceptItem->setEquivalentItem(true);
											itemNode->addEquivalentConcept(subsumedConceptItem->getTestingConcept());
											tax->updateNodeEquivalences(itemNode);
										} else {
											break;
										}
									}
								}
							}
						}
					}


					for (QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedClassExtractedSaturationSatisfiableTestingItem* item = *it;
						if (!item->isEquivalentItem() && item != topItem) {
							cint64 itemSubsumingCount = item->getExtractionSubsumerCount();
							CHierarchyNode* itemHierNode = item->getSatisfiableConceptHierarchyNode();

							if (item->hasDirectSubsumer()) {
								if (mTransitiveReductionAddAllPredecessors) {
									itemHierNode->addPredecessorNode(topHierNode);
								}
								COptimizedClassExtractedSaturationSatisfiableTestingItem* directSubsumerItem = item->getDirectSubsumer();
								directSubsumerItem->setPredecessorItem(true);
								CHierarchyNode* subsumedHierNode = directSubsumerItem->getSatisfiableConceptHierarchyNode();
								itemHierNode->makeParent(subsumedHierNode);
								QSet<CHierarchyNode*>* predSet = subsumedHierNode->getPredecessorNodeSet();
								if (mTransitiveReductionAddAllPredecessors) {
									for (QSet<CHierarchyNode*>::const_iterator itPred = predSet->constBegin(), itPredEnd = predSet->constEnd(); itPred != itPredEnd; ++itPred) {
										CHierarchyNode* predNode(*itPred);
										itemHierNode->addPredecessorNode(predNode);
									}
								}

							} else {
								QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* subsumingConceptItemList = item->getSubsumingConceptItemList();
								QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd();

								cint64 eqCount = 0;
								cint64 remainingSubsumingCount = itemSubsumingCount;
								// remove direct equivalences
								while (subsumedIt != subsumedItEnd) {
									COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
									cint64 subsumedItemSubsumingCount = subsumedConceptItem->getExtractionSubsumerCount();
									if (subsumedItemSubsumingCount != itemSubsumingCount) {
										break;
									} else {
										++eqCount;
										++subsumedIt;
									}
								}

								while (subsumedIt != subsumedItEnd && (*subsumedIt)->isEquivalentItem()) {
									++subsumedIt;
								}

								// find first parent
								if (subsumedIt == subsumedItEnd) {
									// no parent, use top as parent
									itemHierNode->makeParent(topHierNode);
								} else {
									while (subsumedIt != subsumedItEnd) {
										COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
										if (!subsumedConceptItem->isEquivalentItem()) {
											remainingSubsumingCount -= makeParentAddPredeccessors(item,subsumedConceptItem,remainingSubsumingCount,true);
											++subsumedIt;
											break;
										} else {
											++subsumedIt;
										}
									}


									while (subsumedIt != subsumedItEnd && (remainingSubsumingCount > 0)) {
										COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
										if (!subsumedConceptItem->isEquivalentItem()) {

											remainingSubsumingCount -= makeParentAddPredeccessors(item,subsumedConceptItem,remainingSubsumingCount,false);
										}
										++subsumedIt;
									}


									//bool searchForMoreParents = remainingSubsumingCount > 0;
									//while (subsumedIt != subsumedItEnd) {
									//	COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
									//	if (!subsumedConceptItem->isEquivalentItem()) {
									//		bool predOfOnePrevParent = false;
									//		if (searchForMoreParents) {
									//			for (QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>::const_iterator parentIt = parentItemList.constBegin(), parentItEnd = parentItemList.constEnd(); parentIt != parentItEnd && !predOfOnePrevParent; ++parentIt) {
									//				COptimizedClassExtractedSaturationSatisfiableTestingItem* parentItem = *parentIt;
									//				if (precSatSubsumerExtractor.hasSubsumerConcept(parentItem->getTestingConcept(),subsumedConceptItem->getTestingConcept())) {
									//					predOfOnePrevParent = true;
									//				}
									//			}
									//		}
									//		subsumedConceptItem->setPredecessorItem(true);
									//		CHierarchyNode* subsumedHierNode = subsumedConceptItem->getSatisfiableConceptHierarchyNode();
									//		if (!predOfOnePrevParent && searchForMoreParents) {
									//			itemHierNode->makeParent(subsumedHierNode);
									//			parentItemList.append(subsumedConceptItem);
									//		} else {
									//			if (mTransitiveReductionAddAllPredecessors) {
									//				itemHierNode->addPredecessorNode(subsumedHierNode);
									//			}
									//		}
									//	}
									//	++subsumedIt;
									//}
								}
							}
						}
					}


					for (QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedClassExtractedSaturationSatisfiableTestingItem* item = *it;
						if (!item->isPredecessorItem() && !item->isEquivalentItem()) {
							CHierarchyNode* itemHierNode = item->getSatisfiableConceptHierarchyNode();
							bottomHierNode->makeParent(itemHierNode);
						}
					}


#ifdef OPTIMIZEDCLASSEXTRACTEDSATURATIONCLASSIFIERDEBUGSTATUSDESCRIPTION
					statUpdateMutex.lock();
					statusString = QString("Finished ontology classification");
					statUpdateMutex.unlock();
#endif


					CClassification* classification = ontology->getClassification();
					if (classification) {
						CClassConceptClassification* classConClassification = ontClassItem->getClassConceptClassification();
						CClassificationStatisticsCollectionStrings* classifStatCollStrings = ontClassItem->getClassificationStatisticsCollectionStrings();
						CClassifierStatistics* classifierStats = ontClassItem->getClassifierStatistics();
						classifStatCollStrings->addProcessingStatistics("class-classification-total-satisfiable-test-count",classifierStats->getTotalSatisfiableCount());
						classConClassification->setClassConceptTaxonomy(taxonomy);
						classConClassification->setClassificationStatistics(classifStatCollStrings);
						if (ontClassItem->getIndividualDependenceTrackingCollector()) {
							classConClassification->setDependentIndividualsTracking(ontClassItem->getIndividualDependenceTrackingCollector()->getExtendingIndividualDependenceTracking());
						}
						classification->setClassConceptClassification(classConClassification);
					}
					ontology->setConceptTaxonomy(taxonomy);

					ontClassItem->setGoneOutRemainingTests(false);

					taxonomy->setTaxonomyComplete(true);
					--mClassificationCount;
					LOG(INFO,getLogDomain(),logTr("Ontology '%1' classified.").arg(ontology->getTerminologyName()),getLogObject());
				}
				ontClassItem->submitTaxonomyConstructed();
				ontClassItem->doClassifiedCallback();
				return true;
			}





			bool COptimizedClassExtractedSaturationSubsumptionClassifierThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (CSubsumptionClassifierThread::processCustomsEvents(type,event)) {
					return true;
				}
				return false;
			}



			cint64 COptimizedClassExtractedSaturationSubsumptionClassifierThread::makeParentAddPredeccessors(COptimizedClassExtractedSaturationSatisfiableTestingItem* conceptItem, COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumedConceptItem, cint64 remainingSubsumingResolvingCount, bool firstResolve) {				
				CHierarchyNode* node = conceptItem->getSatisfiableConceptHierarchyNode();
				CHierarchyNode* newParentNode = subsumedConceptItem->getSatisfiableConceptHierarchyNode();
				cint64 resolvedExtSubsumerCount = 0;
				if (!node->hasPredecessorNode(newParentNode)) {
					if (firstResolve) {
						resolvedExtSubsumerCount = subsumedConceptItem->getExtractionSubsumerCount();
					} else {
						resolvedExtSubsumerCount = newParentNode->getEquivalentConceptCount();
					}
					node->addParentNode(newParentNode);
					newParentNode->addChildNode(node);
					if (mTransitiveReductionAddAllPredecessors || resolvedExtSubsumerCount != remainingSubsumingResolvingCount) {
						resolvedExtSubsumerCount = 0;
						QList<CHierarchyNode*> predAddList;
						predAddList.append(newParentNode);
						while (!predAddList.isEmpty()) {
							CHierarchyNode* nextPredNode = predAddList.takeFirst();
							if (!node->hasPredecessorNode(nextPredNode)) {
								node->addPredecessorNode(nextPredNode);
								//nextPredNode->addSuccessorNode(node);
								resolvedExtSubsumerCount += nextPredNode->getEquivalentConceptCount();
								QSet<CHierarchyNode*>* nextParentPredSet = nextPredNode->getParentNodeSet();
								for (QSet<CHierarchyNode*>::const_iterator it = nextParentPredSet->constBegin(), itEnd = nextParentPredSet->constEnd(); it != itEnd; ++it) {
									CHierarchyNode* nextPredPredNode(*it);
									predAddList.append(nextPredPredNode);
								}
							}
						}
					} else {
						node->makePredecessor(newParentNode);
					}
				}
				return resolvedExtSubsumerCount;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
