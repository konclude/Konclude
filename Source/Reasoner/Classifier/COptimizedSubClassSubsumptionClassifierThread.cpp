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

#include "COptimizedSubClassSubsumptionClassifierThread.h"

namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGCOUTOUTPUT
	#define OPTIMIZEDSUBCLASSCLASSIFIERDEBUGB
#endif 
#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGCOUTOUTPUT
	#define OPTIMIZEDSUBCLASSCLASSIFIERDEBUGB
#endif 
#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGBOXDESCRIPTION
	#define OPTIMIZEDSUBCLASSCLASSIFIERDEBUGB
#endif 
#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
	#define OPTIMIZEDSUBCLASSCLASSIFIERDEBUGB
#endif 

			COptimizedSubClassSubsumptionClassifierThread::COptimizedSubClassSubsumptionClassifierThread(CReasonerManager *reasonerManager) 
					: CSubsumptionClassifierThread(reasonerManager), CLogIdentifier("::Konclude::Reasoner::Kernel::Classifier::OptimizedSubClassSubsumptionClassifierThread",this) {
				satTestedCount = 0;
				totalToTestCount = 0;
				mClassificationCount = 0;
			}


			COptimizedSubClassSubsumptionClassifierThread::~COptimizedSubClassSubsumptionClassifierThread() {
			}


			CTaxonomy *COptimizedSubClassSubsumptionClassifierThread::createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config) {
				CTBox *tBox = ontology->getTBox();
				CConceptVector *conVec = tBox->getConceptVector();
				CConcept *topConcept = conVec->getData(1);
				CConcept *bottomConcept = conVec->getData(0);
				CPartialPruningTaxonomy *tax = new CPartialPruningTaxonomy(topConcept,bottomConcept);
				return tax->readConfig(config);
			}


			void COptimizedSubClassSubsumptionClassifierThread::readCalculationConfig(CCalculationConfigurationExtension *config) {
				if (config) {
					bool configErrorFlag = false;
					cint64 processorCount = 1;
					QString processorCountString = CConfigDataReader::readConfigString(config,"Konclude.Calculation.ProcessorCount",QString(),&configErrorFlag);
					if (!configErrorFlag) {
						if (processorCountString == "AUTO") {
							processorCount = CThread::idealThreadCount();
						} else {
							qint64 convertedWorkerCount = processorCountString.toInt(&configErrorFlag);
							if (configErrorFlag) {
								processorCount = convertedWorkerCount;
							}
						}
					}

					bool mulConfigErrorFlag = false;
					cint64 multiplicator = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedSubClassSubsumptionClassifier.MultipliedUnitsParallelSatisfiableCalculationCount",1,&mulConfigErrorFlag);
					confMaxTestParallelCount = processorCount*multiplicator;
					bool maxConfigErrorFlag = false;
					cint64 maxParallel = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedSubClassSubsumptionClassifier.MaximumParallelSatisfiableCalculationCount",1,&maxConfigErrorFlag);
					if (!maxConfigErrorFlag) {
						if (!mulConfigErrorFlag) {
							confMaxTestParallelCount = qMin(confMaxTestParallelCount,maxParallel);
						} else {
							confMaxTestParallelCount = maxParallel;
						}
					}
				} else {
					confMaxTestParallelCount = 1;
				}
			}


			
			CSubsumptionClassifierThread *COptimizedSubClassSubsumptionClassifierThread::scheduleOntologyClassification(CConcreteOntology *ontology, CTaxonomy *taxonomy, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config) {

				COptimizedSubClassOntologyClassificationItem *ontClassItem = new COptimizedSubClassOntologyClassificationItem(config,statistics);
				ontClassItem->setClassificationCalculationSupport(classificationSupport);
				ontClassItem->initTaxonomyConcepts(ontology,taxonomy);
				ontItemList.append(ontClassItem);
				processingOntItemList.append(ontClassItem);
				ontItemHash.insert(ontology,ontClassItem);

				readCalculationConfig(ontClassItem->getCalculationConfiguration());

				CPartialPruningTaxonomy *parTax = dynamic_cast<CPartialPruningTaxonomy *>(taxonomy);
				if (parTax) {
					COntologyClassificationItem *ontClassItem = ontItemHash.value(ontology);
					parTax->createStatistics(ontClassItem->getClassifierStatistics());
				}

				createObviousSubsumptionSatisfiableTestingOrder(ontClassItem);

				if (satTestedCount == totalToTestCount) {
					satTestedCount = 0;
					totalToTestCount = ontClassItem->getRemainingSatisfiableTestsCount();
					classStartTime.start();
				} else {
					totalToTestCount += ontClassItem->getRemainingSatisfiableTestsCount();
				}
				++mClassificationCount;
				return this;
			}




			void COptimizedSubClassSubsumptionClassifierThread::createObviousSubsumptionSatisfiableTestingOrder(COptimizedSubClassOntologyClassificationItem* ontClassItem) {
				CPrecomputation* precomputation = ontClassItem->getOntology()->getPrecomputation();
				CSaturationData* saturationData = precomputation->getSaturationModelData();
			
				bool subsumerSaturationExtraction = CConfigDataReader::readConfigBoolean(ontClassItem->getCalculationConfiguration(),"Konclude.Calculation.Classification.SaturationSubsumerExtraction",true);

				if (saturationData && subsumerSaturationExtraction) {
					createObviousSubsumptionSatisfiableTestingOrderFromSaturationData(ontClassItem);
				} else {
					createObviousSubsumptionSatisfiableTestingOrderFromBuildData(ontClassItem);
				}
			}




			void COptimizedSubClassSubsumptionClassifierThread::createObviousSubsumptionSatisfiableTestingOrderFromSaturationData(COptimizedSubClassOntologyClassificationItem* ontClassItem) {

				CTBox *tBox = ontClassItem->getOntology()->getDataBoxes()->getTBox();
				CBOXHASH<CConcept*,CConcept*>* candidateEqConHash = tBox->getCandidateEquivalentConceptHash(false);
				CBOXHASH<CConcept*,CConcept*>* eqConCandidateHash = tBox->getEquivalentConceptCandidateHash(false);
				CBOXSET<CConcept*>* eqConNonCandidateSet = tBox->getEquivalentConceptNonCandidateSet(false);

				CConcreteOntology *onto = ontClassItem->getOntology();
				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();


				QHash<CConcept*,COptimizedSubClassSatisfiableTestingItem*>* conceptSatItemHash = ontClassItem->getConceptSatisfiableTestItemHash();
				QList<COptimizedSubClassSatisfiableTestingItem*>* satTestItemContainer = ontClassItem->getConceptSatisfiableTestItemContainer();

				COptimizedSubClassSatisfiableTestingItem* topItem = ontClassItem->getConceptSatisfiableTestItem(topConcept,true);
				COptimizedSubClassSatisfiableTestingItem* bottomItem = ontClassItem->getConceptSatisfiableTestItem(bottomConcept,true);

				ontClassItem->initTopBottomSatisfiableTestingItems(topItem,bottomItem);

				QList<COptimizedSubClassSatisfiableTestingItem*>* nextItemList = ontClassItem->getNextSatisfiableTestingItemList();
				QSet<COptimizedSubClassSatisfiableTestingItem*>* nextCandItemSet = ontClassItem->getNextCandidateSatisfiableTestingItemSet();
				QSet<COptimizedSubClassSatisfiableTestingItem*>* remainingCandItemSet = ontClassItem->getRemainingCandidateSatisfiableTestingItemSet();

				if (topConcept->getOperandList()) {
					// has not absorbed GCIs
					nextItemList->append(topItem);
				}
				bool invalidatedConRefLinking = false;

				QHash<CConcept*,CConceptReferenceLinking*>* conRefLinkDataHash = ontClassItem->getConceptReferenceLinkingDataHash();
				QList<CConcept*> extClassConceptList;
				CPrecomputedSaturationSubsumerExtractor* precSatSubsumerExtractor = ontClassItem->getPrecomputedSaturationSubsumerExtractor(true);

				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					for (CBOXSET<CConcept*>::const_iterator it = conceptHash->constBegin(), itEnd = conceptHash->constEnd(); it != itEnd; ++it) {
						CConcept *concept = (*it);
						if (concept && concept != topConcept && concept != bottomConcept || (concept == topConcept && topConcept->getOperandList())) {
							COptimizedSubClassSatisfiableTestingItem* subsumerItem = ontClassItem->getConceptSatisfiableTestItem(concept,true);

							CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
							CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
							if (!conRefSatLinking) {
								conRefSatLinking = new CConceptSaturationReferenceLinkingData();
								conProcData->setConceptReferenceLinking(conRefSatLinking);
							}

							conRefLinkDataHash->insert(concept,conRefSatLinking);

							if (conRefSatLinking->getClassifierReferenceLinkingData() || invalidatedConRefLinking) {
								conProcData->setInvalidatedReferenceLinking(true);
							} else {
								conRefSatLinking->setClassifierReferenceLinkingData(subsumerItem);
							}
							extClassConceptList.append(concept);
						}
					}
				}


				for (QList<CConcept*>::const_iterator it = extClassConceptList.constBegin(), itEnd = extClassConceptList.constEnd(); it != itEnd; ++it) {
					CConcept* concept(*it);
					CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
					CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
					COptimizedSubClassSatisfiableTestingItem* classItem = (COptimizedSubClassSatisfiableTestingItem*)conRefSatLinking->getClassifierReferenceLinkingData();

					bool unsatisfiableFlag = false;
					bool insufficientFlag = false;

					if (precSatSubsumerExtractor->getConceptFlags(concept,&unsatisfiableFlag,&insufficientFlag)) {
						if (!unsatisfiableFlag) {
							precSatSubsumerExtractor->extractSubsumers(concept,classItem);
							cint64 foundSubsumerCount = classItem->getSubsumingConceptItemCount();
							classItem->setUnprocessedPredecessorItems(foundSubsumerCount);
							if (!insufficientFlag) {
								classItem->setResultSatisfiableDerivated(true);
							}
							ontClassItem->incRemainingSatisfiableTestsCount();
							remainingCandItemSet->insert(classItem);
							if (foundSubsumerCount <= 0) {
								nextItemList->append(classItem);
							}
						} else {
							ontClassItem->incRemainingSatisfiableTestsCount();
							classItem->setResultUnsatisfiableDerivated(true);
							nextItemList->append(classItem);
						}
					}
				}

			}




			void COptimizedSubClassSubsumptionClassifierThread::createObviousSubsumptionSatisfiableTestingOrderFromBuildData(COptimizedSubClassOntologyClassificationItem* ontClassItem) {

				CTBox *tBox = ontClassItem->getOntology()->getDataBoxes()->getTBox();
				CBOXHASH<CConcept*,CConcept*>* candidateEqConHash = tBox->getCandidateEquivalentConceptHash(false);
				CBOXHASH<CConcept*,CConcept*>* eqConCandidateHash = tBox->getEquivalentConceptCandidateHash(false);
				CBOXSET<CConcept*>* eqConNonCandidateSet = tBox->getEquivalentConceptNonCandidateSet(false);

				CConcreteOntology *onto = ontClassItem->getOntology();
				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();


				QHash<CConcept*,COptimizedSubClassSatisfiableTestingItem*>* conceptSatItemHash = ontClassItem->getConceptSatisfiableTestItemHash();
				QList<COptimizedSubClassSatisfiableTestingItem*>* satTestItemContainer = ontClassItem->getConceptSatisfiableTestItemContainer();

				COptimizedSubClassSatisfiableTestingItem* topItem = ontClassItem->getConceptSatisfiableTestItem(topConcept,true);
				COptimizedSubClassSatisfiableTestingItem* bottomItem = ontClassItem->getConceptSatisfiableTestItem(bottomConcept,true);

				ontClassItem->initTopBottomSatisfiableTestingItems(topItem,bottomItem);

				QList<COptimizedSubClassSatisfiableTestingItem*>* nextItemList = ontClassItem->getNextSatisfiableTestingItemList();
				QSet<COptimizedSubClassSatisfiableTestingItem*>* nextCandItemSet = ontClassItem->getNextCandidateSatisfiableTestingItemSet();
				QSet<COptimizedSubClassSatisfiableTestingItem*>* remainingCandItemSet = ontClassItem->getRemainingCandidateSatisfiableTestingItemSet();

				if (topConcept->getOperandList()) {
					// has not absorbed GCIs
					nextItemList->append(topItem);
				}

				bool invalidatedConRefLinking = false;


				QHash<CConcept*,CConceptReferenceLinking*>* conRefLinkDataHash = ontClassItem->getConceptReferenceLinkingDataHash();

				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					for (CBOXSET<CConcept*>::const_iterator it = conceptHash->constBegin(), itEnd = conceptHash->constEnd(); it != itEnd; ++it) {
						CConcept *concept = (*it);
						if (concept && concept != topConcept && concept != bottomConcept || (concept == topConcept && topConcept->getOperandList())) {
							QSet<QPair<CConcept *,bool> > conSet;
							QList<QPair<CConcept *,bool> > conList;
							QSet<CConcept*> conSubsumSet;
							conList.append(QPair<CConcept *,bool>(concept,false));
							conSet.insert(QPair<CConcept *,bool>(concept,false));
							while (!conList.isEmpty()) {
								QPair<CConcept *,bool> conNegPair = conList.takeFirst();
								CConcept *con = conNegPair.first;
								bool neg = conNegPair.second;
								qint64 conOpCode = con->getProcessingOperatorTag();
								cint64 opCount = con->getOperandCount();
								cint64 param = con->getParameter();
								if (conOpCode == CCAND && (!neg || neg && opCount <= 1) || conOpCode == CCOR && (neg || !neg && opCount <= 1)) {
									CSortedNegLinker<CConcept *> *opConIt = con->getOperandList();
									while (opConIt) {
										CConcept *opCon = opConIt->getData();
										bool opNeg = opConIt->isNegated();
										QPair<CConcept *,bool> opConNegPair(opCon,neg ^ opNeg);
										if (opCon->hasClassName()) {
											if (neg ^ opNeg) {				
												if (!conSet.contains(opConNegPair)) {
													conSet.insert(opConNegPair);
													conList.append(opConNegPair);
												}
											} else {
												conSubsumSet.insert(opCon);
											}
										} else {
											if (!conSet.contains(opConNegPair)) {
												conSet.insert(opConNegPair);
												conList.append(opConNegPair);
											}
										}
										opConIt = opConIt->getNext();
									}
								} else if (!neg && (conOpCode == CCSOME || conOpCode == CCATLEAST && param >= 1) || neg && (conOpCode == CCALL || conOpCode == CCATMOST && param >= 2)) {
									CRole* role = con->getRole();
									CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
									while (superRoleIt) {
										CRole* superRole = superRoleIt->getData();
										bool superRoleNeg = superRoleIt->isNegated();

										CSortedNegLinker<CConcept*>* domainConIt = superRole->getDomainRangeConceptList(superRoleNeg);
										while (domainConIt) {
											CConcept *domCon = domainConIt->getData();
											bool opDomNeg = domainConIt->isNegated();
											QPair<CConcept *,bool> domConNegPair(domCon,neg ^ opDomNeg);
											if (domCon->hasClassName()) {
												if (neg ^ opDomNeg) {				
													if (!conSet.contains(domConNegPair)) {
														conSet.insert(domConNegPair);
														conList.append(domConNegPair);
													}
												} else {
													conSubsumSet.insert(domCon);
												}
											} else {
												if (!conSet.contains(domConNegPair)) {
													conSet.insert(domConNegPair);
													conList.append(domConNegPair);
												}
											}

											domainConIt = domainConIt->getNext();
										}

										superRoleIt = superRoleIt->getNext();
									}
								} else if (!neg && (conOpCode == CCNOMINAL)) {
									CIndividual* individual = con->getNominalIndividual();
									CConceptAssertionLinker* assConLinkIt = individual->getAssertionConceptLinker();
									while (assConLinkIt) {
										CConcept* assCon = assConLinkIt->getData();
										bool assConNeg = assConLinkIt->isNegated();

										QPair<CConcept *,bool> assConNegPair(assCon,neg ^ assConNeg);
										if (assCon->hasClassName()) {
											if (neg ^ assConNeg) {				
												if (!conSet.contains(assConNegPair)) {
													conSet.insert(assConNegPair);
													conList.append(assConNegPair);
												}
											} else {
												conSubsumSet.insert(assCon);
											}
										} else {
											if (!conSet.contains(assConNegPair)) {
												conSet.insert(assConNegPair);
												conList.append(assConNegPair);
											}
										}

										assConLinkIt = assConLinkIt->getNext();
									}
								}
							}


							//QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
							//if (iriClassNameString == "http://www.owllink.org/testsuite/galen#Haem") {
							//	bool bug = true;
							//}


							COptimizedSubClassSatisfiableTestingItem* subsumerItem = ontClassItem->getConceptSatisfiableTestItem(concept,true);

							CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
							CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
							if (!conRefSatLinking) {
								conRefSatLinking = new CConceptSaturationReferenceLinkingData();
								conProcData->setConceptReferenceLinking(conRefSatLinking);
							}

							conRefLinkDataHash->insert(concept,conRefSatLinking);

							if (conRefSatLinking->getClassifierReferenceLinkingData() || invalidatedConRefLinking) {
								conProcData->setInvalidatedReferenceLinking(true);
							} else {
								conRefSatLinking->setClassifierReferenceLinkingData(subsumerItem);
							}


							bool foundSubsumingConcepts = false;
							for (QSet<CConcept*>::const_iterator it = conSubsumSet.constBegin(), itEnd = conSubsumSet.constEnd(); it != itEnd; ++it) {
								CConcept* subsumedConcept = *it;
								COptimizedSubClassSatisfiableTestingItem* subsumedItem = ontClassItem->getConceptSatisfiableTestItem(subsumedConcept,true);
								subsumedItem->addSuccessorSatisfiableTestItem(subsumerItem);
								subsumerItem->incUnprocessedPredecessorItems();
								foundSubsumingConcepts = true;
							}

							ontClassItem->incRemainingSatisfiableTestsCount();

							remainingCandItemSet->insert(subsumerItem);
							if (!foundSubsumingConcepts) {
								nextItemList->append(subsumerItem);
							}


						}
					}
				}
			}


			QString COptimizedSubClassSubsumptionClassifierThread::getStatusString() {
				statUpdateMutex.lock();
				QString string = statusString;
				string.detach();
				statUpdateMutex.unlock();
				return string;
			}



			CClassificationProgress* COptimizedSubClassSubsumptionClassifierThread::getClassificationProgress() {
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



			bool COptimizedSubClassSubsumptionClassifierThread::createNextSubsumtionTest() {


				COntologyClassificationItem *loopOntClassItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !processingOntItemList.isEmpty()) {
					COntologyClassificationItem *ontClassItem = processingOntItemList.first();

					if (ontClassItem == loopOntClassItem) {
						// don't run into infinite loop without doing something
						break;
					}

					COptimizedSubClassOntologyClassificationItem *optSubClassItem = (COptimizedSubClassOntologyClassificationItem *)ontClassItem;


					while (!workTestCreated && optSubClassItem->hasRemainingSatisfiableTests() && !ontClassItem->isTaxonomyConstructionFailed()) {
						// get next satisfiable test

						QList<COptimizedSubClassSatisfiableTestingItem*>* nextItemList = optSubClassItem->getNextSatisfiableTestingItemList();
						QSet<COptimizedSubClassSatisfiableTestingItem*>* nextCandItemSet = optSubClassItem->getNextCandidateSatisfiableTestingItemSet();
						QSet<COptimizedSubClassSatisfiableTestingItem*>* remainingCandItemSet = optSubClassItem->getRemainingCandidateSatisfiableTestingItemSet();


						COptimizedSubClassSatisfiableTestingItem* nextSatTestItem = nullptr;
						if (!nextItemList->isEmpty()) {
							nextSatTestItem = nextItemList->takeFirst();
							remainingCandItemSet->remove(nextSatTestItem);
							nextCandItemSet->remove(nextSatTestItem);

						} else if (!nextCandItemSet->isEmpty()) {
							COptimizedSubClassSatisfiableTestingItem* minItem = nullptr;
							cint64 minUnpPredCount = 0;
							for (QSet<COptimizedSubClassSatisfiableTestingItem*>::const_iterator it = nextCandItemSet->constBegin(), itEnd = nextCandItemSet->constEnd(); it != itEnd; ++it) {
								COptimizedSubClassSatisfiableTestingItem* item = (*it);
								cint64 predCount = item->getUnprocessedPredecessorItemCount();
								if (!minItem || minUnpPredCount < predCount) {
									minUnpPredCount = predCount;
									minItem = item;
								}
							}
							nextSatTestItem = minItem;
							remainingCandItemSet->remove(nextSatTestItem);
							nextCandItemSet->remove(nextSatTestItem);

						} else if (!remainingCandItemSet->isEmpty()) {
							COptimizedSubClassSatisfiableTestingItem* minItem = nullptr;
							//cint64 minUnpPredCount = 0;
							//for (QSet<COptimizedSubClassSatisfiableTestingItem*>::const_iterator it = remainingCandItemSet->constBegin(), itEnd = remainingCandItemSet->constEnd(); it != itEnd; ++it) {
							//	COptimizedSubClassSatisfiableTestingItem* item = (*it);
							//	cint64 predCount = item->getUnprocessedPredecessorItemCount();
							//	if (!minItem || minUnpPredCount < predCount) {
							//		minUnpPredCount = predCount;
							//		minItem = item;
							//	}
							//}
							//nextSatTestItem = minItem;
							//remainingCandItemSet->remove(nextSatTestItem);
							QSet<COptimizedSubClassSatisfiableTestingItem*>::const_iterator itNext = remainingCandItemSet->constBegin();
							nextSatTestItem = *itNext;
							remainingCandItemSet->remove(nextSatTestItem);							
						}


						if (nextSatTestItem && !nextSatTestItem->isSatisfiableTestOrdered()) {
							optSubClassItem->decRemainingSatisfiableTestsCount();
							optSubClassItem->incRunningSatisfiableTestsCount();
							if (calculateSatisfiable(optSubClassItem,nextSatTestItem)) {
								workTestCreated = true;
							} else {
								interpreteSatisfiableResult(optSubClassItem,nextSatTestItem->getSatisfiableConcept(),nextSatTestItem->getSatisfiableTestedResult());
							}
						}
					}


					if (!workTestCreated) {
						if (optSubClassItem->hasAllSatisfiableTestsCompleted() || ontClassItem->isTaxonomyConstructionFailed()) {
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



			bool COptimizedSubClassSubsumptionClassifierThread::calculateSatisfiable(COptimizedSubClassOntologyClassificationItem *optSubClassItem, COptimizedSubClassSatisfiableTestingItem* nextSatTestItem) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CClassificationWorkItem *workItem = 0;

				CConcept* concept = nextSatTestItem->getSatisfiableConcept();
				nextSatTestItem->setSatisfiableTestOrdered(true);

				bool satisfiableFlag = false;

#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGB
				QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
#endif
#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
				statUpdateMutex.lock();
				statusString = QString("[%2/%3] Calculating, is '%1' satisfiable").arg(iriClassNameString).arg(optSubClassItem->getClassifierStatistics()->getTestedSatisfiableCount()).arg(optSubClassItem->getClassifierStatistics()->getTotalSatisfiableCount());
				statUpdateMutex.unlock();
#endif
#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGLOGOUTPUT
				LOG(INFO,getLogDomain(),QString("Calculating whether '%1' is satisfiable").arg(iriClassNameString),this);
#endif
#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGCOUTOUTPUT
				cout<<QString("Calculating whether '%1' is satisfiable").arg(iriClassNameString).toLocal8Bit().data()<<endl<<endl;
#endif

				//if (iriClassNameString == "http://www.owllink.org/testsuite/galen#Haem") {
				//	bool bug = true;
				//}


				if (nextSatTestItem->isResultSatisfiableDerivated()) {
					satisfiableFlag = true;
					nextSatTestItem->setSatisfiableTested(true);
					nextSatTestItem->setSatisfiableTestedResult(true);
					return false;
				}
				if (nextSatTestItem->isResultUnsatisfiableDerivated()) {
					satisfiableFlag = false;
					nextSatTestItem->setSatisfiableTested(true);
					nextSatTestItem->setSatisfiableTestedResult(false);
					return false;
				}
				CPrecomputedSaturationSubsumerExtractor* precSatSubsumerExtractor = optSubClassItem->getPrecomputedSaturationSubsumerExtractor(false);
				bool unsatisfiableFlag = false;
				if (precSatSubsumerExtractor && precSatSubsumerExtractor->getConceptFlags(concept,&unsatisfiableFlag,nullptr) && unsatisfiableFlag) {
					satisfiableFlag = false;
					nextSatTestItem->setSatisfiableTested(true);
					nextSatTestItem->setSatisfiableTestedResult(false);
					return false;
				}
				if (fastSatisfiableOnlySubClassPrecheckTest(optSubClassItem,nextSatTestItem,&satisfiableFlag)) {
					nextSatTestItem->setSatisfiableTested(true);
					nextSatTestItem->setSatisfiableTestedResult(satisfiableFlag);
					return false;
				}

				CSatisfiableCalculationJobGenerator satCalcJobGen(optSubClassItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(concept);
				QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *> *workHash = optSubClassItem->getWorkItemHash();

				CClassifierStatistics *ontClassStat = optSubClassItem->getClassifierStatistics();
				if (ontClassStat) {
					ontClassStat->incCalculatedTestedSatisfiableCount(1);
				}

				workItem = new CClassificationWorkItem(satCalcJob,concept);
				workHash->insertMulti(satCalcJob,workItem);
				workOntItemHash.insert(workItem,optSubClassItem);

				QHash<CConcept*,CConceptReferenceLinking*>* conRefLinkDataHash = optSubClassItem->getConceptReferenceLinkingDataHash();


				satCalcJob->setSatisfiableClassificationMessageAdapter(new CSatisfiableTaskClassificationMessageAdapter(concept,optSubClassItem->getOntology(),this,nullptr,
						CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTSUBSUMERSROOTNODE | 
						CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTOTHERNODESSINGLEDEPENDENCY | 
						CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTSUBSUMERSOTHERNODES));

				optSubClassItem->incCurrentCalculatingCount();
				processCalculationJob(satCalcJob,optSubClassItem,workItem);
				return true;
			}







			bool COptimizedSubClassSubsumptionClassifierThread::fastSatisfiableOnlySubClassPrecheckTest(COptimizedSubClassOntologyClassificationItem *optSubClassItem, COptimizedSubClassSatisfiableTestingItem* nextSatTestItem, bool* isSatisfiableFlag) {
				QHash<CConcept*,COptimizedSubClassSatisfiableTestingItem*>* conceptSatItemHash = optSubClassItem->getConceptSatisfiableTestItemHash();
				CBOXHASH<CConcept*,CConcept*>* triggerImpHash = optSubClassItem->getOntology()->getTBox()->getTriggerImplicationHash(false);
				CConcept* concept = nextSatTestItem->getSatisfiableConcept();
				bool satFlag = false;
				bool testComplete = false;
				if (concept->getOperatorCode() == CCSUB) {
					cint64 conCount = concept->getOperandCount();
					if (conCount == 1) {
						CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
						CConcept* firstConcept = opLinkerIt->getData();
						cint64 firstOpCode = firstConcept->getOperatorCode();
						CConcept* subClassConcept = nullptr;
						if (!opLinkerIt->isNegated() && (firstOpCode == CCATOM || firstOpCode == CCSUB || firstOpCode == CCEQ)) {
							subClassConcept = firstConcept;
						}

						if (!triggerImpHash->contains(concept) && subClassConcept) {
							COptimizedSubClassSatisfiableTestingItem* subClassConItem = conceptSatItemHash->value(subClassConcept);
							if (subClassConItem && subClassConItem->isSatisfiableTested()) {
								if (!triggerImpHash || !triggerImpHash->contains(subClassConcept)) {
									QList<COptimizedSubClassSatisfiableTestingItem*>* subsumedSubsumingConList = subClassConItem->getSubsumingConceptItemList();
									for (QList<COptimizedSubClassSatisfiableTestingItem*>::const_iterator it = subsumedSubsumingConList->constBegin(), itEnd = subsumedSubsumingConList->constEnd(); it != itEnd; ++it) {
										COptimizedSubClassSatisfiableTestingItem* subsumedItem = (*it);
										if (subClassConItem != subsumedItem && subsumedItem != nextSatTestItem) {
											nextSatTestItem->addSubsumingConceptItem(subsumedItem);
										}
									}
									nextSatTestItem->addSubsumingConceptItem(subClassConItem);
									satFlag = subClassConItem->getSatisfiableTestedResult();
									testComplete = true;
								}
							}
						}
					}
				} else if (concept->getOperatorCode() == CCATOM) {
					satFlag = true;
					testComplete = true;
				}
				if (satFlag && isSatisfiableFlag) {
					*isSatisfiableFlag = true;
				}
				return testComplete;
			}





			CSubsumptionClassifierThread *COptimizedSubClassSubsumptionClassifierThread::processCalculationJob(CSatisfiableCalculationJob* job, COntologyClassificationItem *ontClassItem, CClassificationWorkItem* workItem) {
				CClassificationCalculationStatisticsCollection* statColl =  nullptr;
				if (ontClassItem->isCollectProcessStatisticsActivated()) {
					statColl = ontClassItem->getCalculationStatisticsCollection();
				}
				job->setCalclulationStatisticsCollector(statColl);
				CTestCalculatedCallbackEvent *testResultCallback = new CTestCalculatedCallbackEvent(this,job,workItem,statColl);
				job->setCalculationConfiguration(ontClassItem->getCalculationConfiguration());
				CSubsumptionClassifierThread::processCalculationJob(job,ontClassItem,testResultCallback);
				return this;
			}



			bool COptimizedSubClassSubsumptionClassifierThread::processToldClassificationMessage(COntologyClassificationItem *ontClassItem, CClassificationMessageData* messageDataLinker, CMemoryPool* memoryPools) {
				COptimizedSubClassOntologyClassificationItem *optSubClassItem = (COptimizedSubClassOntologyClassificationItem *)ontClassItem;
				CClassificationMessageData* messageDataLinkerIt = messageDataLinker;
				while (messageDataLinkerIt) {
					CClassificationMessageData* messageData = messageDataLinkerIt;
					if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLCLASSSUBSUMPTION) {
						CClassificationSubsumptionMessageData* subsumMessageData = (CClassificationSubsumptionMessageData*)messageData;
						CConcept* subsumedConcept = subsumMessageData->getSubsumedConcept();
						CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* subsumerList = subsumMessageData->getClassSubsumerList();

						COptimizedSubClassSatisfiableTestingItem* subsumedItem = nullptr;
						subsumedItem = optSubClassItem->getConceptSatisfiableTestItem(subsumedConcept);


						//QString iriClassNameString = CIRIName::getRecentIRIName(subsumedItem->getSatisfiableConcept()->getClassNameLinker());
						//if (iriClassNameString == "http://www.owllink.org/testsuite/galen#LowerLobeOfLeftLung") {
						//	bool bug = true;
						//}


						if (subsumerList) {
							for (CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>::const_iterator it = subsumerList->constBegin(), itEnd = subsumerList->constEnd(); it != itEnd; ++it) {
								CConcept* subsumerConcept(*it);
								COptimizedSubClassSatisfiableTestingItem* subsumerItem = optSubClassItem->getConceptSatisfiableTestItem(subsumerConcept);
								subsumedItem->addSubsumingConceptItem(subsumerItem);
							}
						}

						subsumedItem->setResultSatisfiableDerivated(true);

					}
					messageDataLinkerIt = messageDataLinkerIt->getNext();
				}
				return true;
			}


			bool COptimizedSubClassSubsumptionClassifierThread::interpreteToldSubsumptionResult(COntologyClassificationItem *ontClassItem, const QList<QPair<CConcept *,CConcept *> > &subSumRelList, bool isSubSum) {
				CConcept *lastConcept = nullptr;
				COptimizedSubClassOntologyClassificationItem *optSubClassItem = (COptimizedSubClassOntologyClassificationItem *)ontClassItem;
				COptimizedSubClassSatisfiableTestingItem* subsumerItem = nullptr;

				for (QList<QPair<CConcept *,CConcept *> >::const_iterator it = subSumRelList.constBegin(), itEnd = subSumRelList.constEnd(); it != itEnd; ++it) {
					CConcept *subsumerConcept = it->first;
					if (lastConcept != subsumerConcept) {
						lastConcept = subsumerConcept;
						subsumerItem = optSubClassItem->getConceptSatisfiableTestItem(subsumerConcept);
					}
					CConcept *subsumedConcept = it->second;
					if (isSubSum) {
						COptimizedSubClassSatisfiableTestingItem* subsumedItem = optSubClassItem->getConceptSatisfiableTestItem(subsumedConcept);
						subsumerItem->addSubsumingConceptItem(subsumedItem);
					}
				}
				return true;
			}


			bool COptimizedSubClassSubsumptionClassifierThread::interpreteSubsumptionResult(COntologyClassificationItem *ontClassItem, CConcept *subsumerConcept, CConcept *subsumedConcept, bool isSubsumption) {
				return false;
			}


			bool COptimizedSubClassSubsumptionClassifierThread::interpreteSatisfiableResult(COntologyClassificationItem *ontClassItem, CConcept *satisfiableConcept, bool isSatis) {
				COptimizedSubClassOntologyClassificationItem *optSubClassItem = (COptimizedSubClassOntologyClassificationItem *)ontClassItem;

				CClassifierStatistics *ontClassStat = optSubClassItem->getClassifierStatistics();
				if (ontClassStat) {
					ontClassStat->incToldSatisfiableCount(1);
				}

				++satTestedCount;

				optSubClassItem->decRunningSatisfiableTestsCount();

				COptimizedSubClassSatisfiableTestingItem* satTestedItem = optSubClassItem->getConceptSatisfiableTestItem(satisfiableConcept);
				satTestedItem->setSatisfiableTested(true);
				satTestedItem->setSatisfiableTestedResult(isSatis);

				COptimizedSubClassSatisfiableTestingItem* topItem = optSubClassItem->getTopConceptSatisfiableTestItem();

				if (isSatis) {
					// update processing lists
#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGB
					QString iriClassNameString = CIRIName::getRecentIRIName(satisfiableConcept->getClassNameLinker());
					satTestedConList.append(iriClassNameString);
#endif
#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGLOGOUTPUT
					LOG(INFO,getLogDomain(),QString("[%1 / %2] Calculated: '%3' is satisfiable").arg(satTestedCount).arg(totalToTestCount).arg(iriClassNameString),this);
#endif
#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
					statUpdateMutex.lock();
					statusString = QString("Interpreting, '%1' is satisfiable").arg(iriClassNameString);
					statUpdateMutex.unlock();
#endif
#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGCOUTOUTPUT
					cout<<QString("Calculated: '%1' is satisfiable").arg(iriClassNameString).toLocal8Bit().data()<<endl<<endl;
#endif

					optSubClassItem->addSatisfiableConceptItem(satTestedItem);

				} else {
					CTaxonomy *tax = optSubClassItem->getTaxonomy();
					if (topItem) {
						tax->setConceptSatisfiable(satisfiableConcept,false);
					} else {
						tax->getBottomHierarchyNode()->addEquivalentConcept(satisfiableConcept);
						tax->updateNodeEquivalences(tax->getBottomHierarchyNode());
					}
				}

				QList<COptimizedSubClassSatisfiableTestingItem*>* nextItemList = optSubClassItem->getNextSatisfiableTestingItemList();
				QSet<COptimizedSubClassSatisfiableTestingItem*>* nextCandItemSet = optSubClassItem->getNextCandidateSatisfiableTestingItemSet();
				QSet<COptimizedSubClassSatisfiableTestingItem*>* remainingCandItemSet = optSubClassItem->getRemainingCandidateSatisfiableTestingItemSet();


				QList<COptimizedSubClassSatisfiableTestingItem*>* succItemList = satTestedItem->getSuccessorItemList();
				for (QList<COptimizedSubClassSatisfiableTestingItem*>::const_iterator it = succItemList->constBegin(), itEnd = succItemList->constEnd(); it != itEnd; ++it) {
					COptimizedSubClassSatisfiableTestingItem* succItem = *it;
					if (!isSatis) {
						succItem->setResultUnsatisfiableDerivated(true);
					}
					succItem->decUnprocessedPredecessorItems();
					if (succItem->hasOnlyProcessedPredecessorItems()) {
						nextItemList->append(succItem);
					} else {
						nextCandItemSet->insert(succItem);
					}
				}

				return false;
			}


			bool COptimizedSubClassSubsumptionClassifierThread::interpreteTestResults(CTestCalculatedCallbackEvent *testResult) {
				CSatisfiableCalculationJob *satCalcJob = testResult->getSatisfiableCalculationJob();
				bool testSat = testResult->getTestResultSatisfiable();
				CClassificationWorkItem *workItem = testResult->getClassificationWorkItem();

				COntologyClassificationItem *ontClassItem = workOntItemHash.value(workItem);
				if (ontClassItem) {

					QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *> *workHash = ontClassItem->getWorkItemHash();

					CTaxonomy *taxonomy = ontClassItem->getTaxonomy();

					if (workItem) {
						ontClassItem->decCurrentCalculatingCount();
						if (workItem->isTestValid()) {
							if (testResult->hasCalculationError()) {
								LOG(ERROR,getLogDomain(),logTr("Error in computation, classification for ontology '%1' failed.").arg(ontClassItem->getOntology()->getTerminologyName()),getLogObject());
								ontClassItem->setTaxonomyConstructionFailed();

							} else {
								if (workItem->isConceptSatisfiableTest()) {
									interpreteSatisfiableResult(ontClassItem,workItem->getSatisfiableTestedConcept(),testSat);
								}
							}
						}
						QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *>::iterator itWorkItem = workHash->find(satCalcJob);
						while (itWorkItem != workHash->end()) {						
							if (itWorkItem.value() == workItem) {
								workHash->erase(itWorkItem);
								break;
							}
							++itWorkItem;
						}
						delete workItem;
					}
					workOntItemHash.remove(workItem);

					CClassificationCalculationStatisticsCollection* statisticCollection = testResult->getUsedStatisticsCollection();
					if (statisticCollection) {
						ontClassItem->reuseCalculationStatisticsCollection(statisticCollection);
					}

#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGTAXONOMYCOUTOUTPUT
					QString taxString = taxonomy->getConceptHierarchyString();
					cout<<taxString.toLocal8Bit().data()<<endl<<endl;
#endif
				} 

				return true;
			}


			bool COptimizedSubClassSubsumptionClassifierThread::finishOntologyClassification(COntologyClassificationItem *ontClassItem) {

				if (!ontClassItem->isTaxonomyConstructed() && !ontClassItem->isTaxonomyConstructionFailed()) {

#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGLOGOUTPUT
					LOG(INFO,getLogDomain(),QString("Building Class Taxonomy"),this);
#endif

					// build taxonomy

					COptimizedSubClassOntologyClassificationItem *optSubClassItem = (COptimizedSubClassOntologyClassificationItem *)ontClassItem;
					QList<COptimizedSubClassSatisfiableTestingItem*>* satItemList = optSubClassItem->getSatisfiableConceptItemList();

					CTaxonomy *tax = optSubClassItem->getTaxonomy();

					QList<COptimizedSubClassSatisfiableTestingItem*> itemList;

					COptimizedSubClassSatisfiableTestingItem* topItem = optSubClassItem->getTopConceptSatisfiableTestItem();
					CHierarchyNode* topHierNode = tax->getTopHierarchyNode();
					CHierarchyNode* bottomHierNode = tax->getBottomHierarchyNode();



					cint64 topSubsumingItemCount = topItem->getSubsumingConceptItemCount();
					topItem->setSatisfiableConceptHierarchyNode(topHierNode);
					if (topSubsumingItemCount) {
						QList<COptimizedSubClassSatisfiableTestingItem*>* subsumingConceptItemList = topItem->sortSubsumingConceptItemList();
						for (QList<COptimizedSubClassSatisfiableTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
							COptimizedSubClassSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
							// mark as equivalent
							subsumedConceptItem->setEquivalentItem(true);
							topHierNode->addEquivalentConcept(subsumedConceptItem->getSatisfiableConcept());
							tax->updateNodeEquivalences(topHierNode);
						}
					}


					for (QList<COptimizedSubClassSatisfiableTestingItem*>::const_iterator it = satItemList->constBegin(), itEnd = satItemList->constEnd(); it != itEnd; ++it) {
						COptimizedSubClassSatisfiableTestingItem* item = *it;
						if (!item->isEquivalentItem() && item != topItem) {

							CHierarchyNode* itemNode = tax->getHierarchyNode(item->getSatisfiableConcept(),true);
							item->setSatisfiableConceptHierarchyNode(itemNode);
							cint64 itemSubsumingCount = item->getSubsumingConceptItemCount();
							itemList.append(item);
							if (itemSubsumingCount > topSubsumingItemCount) {
								QList<COptimizedSubClassSatisfiableTestingItem*>* subsumingConceptItemList = item->sortSubsumingConceptItemList();
								for (QList<COptimizedSubClassSatisfiableTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
									COptimizedSubClassSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
									cint64 subsumedItemSubsumingCount = subsumedConceptItem->getSubsumingConceptItemCount();
									if (subsumedItemSubsumingCount == itemSubsumingCount) {
										// mark as equivalent
										subsumedConceptItem->setEquivalentItem(true);
										itemNode->addEquivalentConcept(subsumedConceptItem->getSatisfiableConcept());
										tax->updateNodeEquivalences(itemNode);
									} else {
										break;
									}
								}
							}
						}
					}


					for (QList<COptimizedSubClassSatisfiableTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedSubClassSatisfiableTestingItem* item = *it;

						if (!item->isEquivalentItem() && item != topItem) {
							cint64 itemSubsumingCount = item->getSubsumingConceptItemCount();
							CHierarchyNode* itemHierNode = item->getSatisfiableConceptHierarchyNode();

							//QString iriClassNameString = CIRIName::getRecentIRIName(item->getSatisfiableConcept()->getClassNameLinker());
							//if (iriClassNameString == "http://www.owllink.org/testsuite/galen#LowerLobeOfLeftLung") {
							//	bool bug = true;
							//}


							QList<COptimizedSubClassSatisfiableTestingItem*>* subsumingConceptItemList = item->getSubsumingConceptItemList();
							QList<COptimizedSubClassSatisfiableTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd();

							cint64 eqCount = 0;
							cint64 remainingSubsumingCount = itemSubsumingCount;
							// remove direct equivalences
							while (subsumedIt != subsumedItEnd) {
								COptimizedSubClassSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
								cint64 subsumedItemSubsumingCount = subsumedConceptItem->getSubsumingConceptItemCount();
								if (subsumedItemSubsumingCount != itemSubsumingCount) {
									break;
								} else {
									++eqCount;
									++subsumedIt;
									--remainingSubsumingCount;
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
								itemHierNode->addPredecessorNode(topHierNode);
								QList<COptimizedSubClassSatisfiableTestingItem*> parentItemList;
								while (subsumedIt != subsumedItEnd) {
									COptimizedSubClassSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
									if (!subsumedConceptItem->isEquivalentItem()) {
										subsumedConceptItem->setPredecessorItem(true);
										CHierarchyNode* subsumedHierNode = subsumedConceptItem->getSatisfiableConceptHierarchyNode();
										itemHierNode->makeParent(subsumedHierNode);
										parentItemList.append(subsumedConceptItem);
										remainingSubsumingCount -= subsumedConceptItem->getSubsumingConceptItemCount()+1;
										++subsumedIt;
										break;
									} else {
										++subsumedIt;
									}
								}

								bool searchForMoreParents = remainingSubsumingCount > 0;
								while (subsumedIt != subsumedItEnd) {
									COptimizedSubClassSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
									if (!subsumedConceptItem->isEquivalentItem()) {
										bool predOfOnePrevParent = false;
										if (searchForMoreParents) {
											for (QList<COptimizedSubClassSatisfiableTestingItem*>::const_iterator parentIt = parentItemList.constBegin(), parentItEnd = parentItemList.constEnd(); parentIt != parentItEnd && !predOfOnePrevParent; ++parentIt) {
												COptimizedSubClassSatisfiableTestingItem* parentItem = *parentIt;
												if (parentItem->getSubsumingConceptItemSet()->contains(subsumedConceptItem)) {
													predOfOnePrevParent = true;
												}
											}
										}
										subsumedConceptItem->setPredecessorItem(true);
										CHierarchyNode* subsumedHierNode = subsumedConceptItem->getSatisfiableConceptHierarchyNode();
										if (!predOfOnePrevParent && searchForMoreParents) {
											itemHierNode->makeParent(subsumedHierNode);
											parentItemList.append(subsumedConceptItem);
										} else {
											itemHierNode->addPredecessorNode(subsumedHierNode);
										}
									}
									++subsumedIt;
								}
							}
						}
					}

					for (QList<COptimizedSubClassSatisfiableTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedSubClassSatisfiableTestingItem* item = *it;
						if (!item->isPredecessorItem() && !item->isEquivalentItem()) {
							CHierarchyNode* itemHierNode = item->getSatisfiableConceptHierarchyNode();
							bottomHierNode->makeParent(itemHierNode);
						}
					}



#ifdef OPTIMIZEDSUBCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
					statUpdateMutex.lock();
					statusString = QString("Finished ontology classification");
					statUpdateMutex.unlock();
#endif
					CTaxonomy *taxonomy = ontClassItem->getTaxonomy();
					CConcreteOntology *ontology = ontClassItem->getOntology();


					CClassification* classification = ontology->getClassification();
					if (classification) {
						CClassConceptClassification* classConClassification = ontClassItem->getClassConceptClassification();
						CClassificationStatisticsCollectionStrings* classifStatCollStrings = ontClassItem->getClassificationStatisticsCollectionStrings();
						CClassifierStatistics* classifierStats = ontClassItem->getClassifierStatistics();
						classifStatCollStrings->addProcessingStatistics("class-classification-total-satisfiable-test-count",classifierStats->getTotalSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-satisfiable-tested-count",classifierStats->getTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-satisfiable-pseudo-model-merged-tested-count",classifierStats->getPseudoModelMergedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-satisfiable-derivate-tested-count",classifierStats->getDerivatedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-satisfiable-calculated-tested-count",classifierStats->getCalculatedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-satisfiable-pruned-tested-count",classifierStats->getPrunedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-satisfiable-told-tested-count",classifierStats->getToldSatisfiableCount()-classifierStats->getCalculatedTestedSatisfiableCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-total-subsumption-test-count",classifierStats->getTotalSatisfiableCount()*(classifierStats->getTotalSatisfiableCount()-1));
						classifStatCollStrings->addProcessingStatistics("class-classification-subsumption-tested-count",classifierStats->getTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-subsumption-pseudo-model-merged-tested-count",classifierStats->getPseudoModelMergedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-subsumption-subclass-tested-count",classifierStats->getSubclassTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-subsumption-subclass-identifier-occur-tested-count",classifierStats->getSubclassIdentifierOccurTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-subsumption-derivate-tested-count",classifierStats->getDerivatedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-subsumption-calculated-tested-count",classifierStats->getCalculatedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-subsumption-pruned-tested-count",classifierStats->getPrunedTestedSubsumptionCount());
						classifStatCollStrings->addProcessingStatistics("class-classification-subsumption-told-tested-count",classifierStats->getToldSubsumptionCount()-classifierStats->getCalculatedTestedSubsumptionCount());
						classConClassification->setClassConceptTaxonomy(taxonomy);
						classConClassification->setClassificationStatistics(classifStatCollStrings);
						classification->setClassConceptClassification(classConClassification);
					}
					ontology->setConceptTaxonomy(taxonomy);

					ontClassItem->setGoneOutRemainingTests(false);

					taxonomy->setTaxonomyComplete(true);
					--mClassificationCount;
					LOG(INFO,getLogDomain(),logTr("Ontology '%1' classified with %2 satisfiable tests.").arg(ontology->getTerminologyName()).arg(ontClassItem->getCalcedSatisfiableCount()),getLogObject());
				}
				ontClassItem->submitTaxonomyConstructed();
				ontClassItem->doClassifiedCallback();
				return true;
			}


			bool COptimizedSubClassSubsumptionClassifierThread::interceptTestResults(CInterceptOntologyTestResultEvent *interceptResult) {
				CConcreteOntology *ontology = interceptResult->getOntology();
				COntologyClassificationItem *ontoClassItem = ontItemHash.value(ontology);
				CCallbackData *callbackData = interceptResult->getCallbackData();
				bool tested = false;
				bool result = false;
				if (ontoClassItem) {
					CTaxonomy *taxonomy = ontoClassItem->getTaxonomy();
					if (interceptResult->isConceptSatisfiableTest()) {
						CConcept *concept = interceptResult->getSatisfiableTestedConcept();
						result = taxonomy->isSatisfiable(concept);
						if (result) {
							tested = true;
						} else {
							result = !taxonomy->isNotSatisfiable(concept);
							if (!result) {
								tested = true;
							} else {
								result = false;
							}
						}
					} else if (interceptResult->isConceptSubsumptionTest()) {
						CConcept *subsumerConcept = interceptResult->getSubsumerTestedConcept();
						CConcept *subsumedConcept = interceptResult->getSubsumedTestedConcept();
						result = taxonomy->isSubsumption(subsumerConcept,subsumedConcept);
						if (result) {
							tested = true;
						} else {
							result = !taxonomy->isNotSubsumption(subsumerConcept,subsumedConcept);
							if (!result) {
								tested = true;
							} else {
								result = false;
							}
						}
					}
				}
				CInterceptResultCallbackDataContext *interceptContext = new CInterceptResultCallbackDataContext(tested,result);
				callbackData->setCallbackDataContext(interceptContext);
				callbackData->doCallback();
				return true;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
