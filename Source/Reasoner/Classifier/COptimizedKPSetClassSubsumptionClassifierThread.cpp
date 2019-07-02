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

#include "COptimizedKPSetClassSubsumptionClassifierThread.h"

namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCOUTOUTPUT
	#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGB
#endif 
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCOUTOUTPUT
	#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGB
#endif 
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGBOXDESCRIPTION
	#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGB
#endif 
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
	#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGB
#endif 

#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGB
	#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(a)
#else
	#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(a)
#endif



//#define OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(a) a


			COptimizedKPSetClassSubsumptionClassifierThread::COptimizedKPSetClassSubsumptionClassifierThread(CReasonerManager *reasonerManager) 
					: CSubsumptionClassifierThread(reasonerManager), CLogIdentifier("::Konclude::Reasoner::Kernel::Classifier::OptimizedKPSetClassSubsumptionClassifierThread",this) {
				satTestedCount = 0;
				totalToSatTestClassCount = 0;
				mClassificationCount = 0;
				subsumTestedCount = 0;
				totalToSubsumTestCount = 0;

				mConfPossSubsumCalcOrderConceptSorted = true;
				mConfPossSubsumCalcOrderFewestSubsumptionSorted = false;

				mConfPossSubsumCalcOrderTopDown = true;
				mConfPossSubsumCalcOrderBottomUp = false;

				mConfPossSubsumPseudoModelPretest = true;
				mConfPossSubsumPseudoModelTest = true;
				mConfWriteDebuggingData = false;

				mStatProcesedSubsumMessCount = 0;
				mStatProcesedPossSubsumInitMessCount = 0;
				mStatProcesedPossSubsumUpdateMessCount = 0;
				mStatProcesedPseudoModelMessCount = 0;

				mInterpretedSubsumptionCalculationCount = 0;
				mOrderedSubsumptionCalculationCount = 0;
				mPseudoModelPretestSubsumptionCalculationCount = 0;
				mCreatedCalculationTaskCount = 0;
				mRecievedCallbackCount = 0;
			}


			COptimizedKPSetClassSubsumptionClassifierThread::~COptimizedKPSetClassSubsumptionClassifierThread() {
			}


			CTaxonomy *COptimizedKPSetClassSubsumptionClassifierThread::createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config) {
				CTBox *tBox = ontology->getTBox();
				CConceptVector *conVec = tBox->getConceptVector();
				CConcept *topConcept = conVec->getData(1);
				CConcept *bottomConcept = conVec->getData(0);
				CPartialPruningTaxonomy *tax = new CPartialPruningTaxonomy(topConcept,bottomConcept);
				return tax->readConfig(config);
			}


			void COptimizedKPSetClassSubsumptionClassifierThread::readCalculationConfig(CCalculationConfigurationExtension *config) {
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
					cint64 multiplicator = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedKPSetClassSubsumptionClassifier.MultipliedUnitsParallelSatisfiableCalculationCount",1,&mulConfigErrorFlag);
					confMaxTestParallelCount = processorCount*multiplicator;
					bool maxConfigErrorFlag = false;
					cint64 maxParallel = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedKPSetClassSubsumptionClassifier.MaximumParallelSatisfiableCalculationCount",1,&maxConfigErrorFlag);
					if (!maxConfigErrorFlag) {
						if (!mulConfigErrorFlag) {
							confMaxTestParallelCount = qMin(confMaxTestParallelCount,maxParallel);
						} else {
							confMaxTestParallelCount = maxParallel;
						}
					}
					double factor = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Classification.OptimizedKPSetClassSubsumptionClassifier.ParallelSatisfiableCalculationCreationFactor",1000)/1000.;
					confMinTestParallelCount = confMaxTestParallelCount*factor;

					mConfWriteDebuggingData = CConfigDataReader::readConfigBoolean(config,"Konclude.Debugging.WriteDebuggingData",false);

				} else {
					confMaxTestParallelCount = 1;
					confMinTestParallelCount = 1;
					mConfWriteDebuggingData = false;
				}

			}


			
			CSubsumptionClassifierThread *COptimizedKPSetClassSubsumptionClassifierThread::scheduleOntologyClassification(CConcreteOntology *ontology, CTaxonomy *taxonomy, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config) {

				COptimizedKPSetClassOntologyClassificationItem *ontClassItem = new COptimizedKPSetClassOntologyClassificationItem(config,statistics);
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

				if (satTestedCount == totalToSatTestClassCount) {
					satTestedCount = 0;
					totalToSatTestClassCount = ontClassItem->getRemainingSatisfiableTestsCount();
					classStartTime.start();
				} else {
					totalToSatTestClassCount += ontClassItem->getRemainingSatisfiableTestsCount();
				}
				++mClassificationCount;
				return this;
			}

			void COptimizedKPSetClassSubsumptionClassifierThread::createObviousSubsumptionSatisfiableTestingOrder(COptimizedKPSetClassOntologyClassificationItem* ontClassItem) {
				CPrecomputation* precomputation = ontClassItem->getOntology()->getPrecomputation();
				CSaturationData* saturationData = precomputation->getSaturationModelData();
				if (saturationData) {
					createObviousSubsumptionSatisfiableTestingOrderFromSaturationData(ontClassItem);
				} else {
					createObviousSubsumptionSatisfiableTestingOrderFromBuildData(ontClassItem);
				}
			}




			void COptimizedKPSetClassSubsumptionClassifierThread::createObviousSubsumptionSatisfiableTestingOrderFromSaturationData(COptimizedKPSetClassOntologyClassificationItem* ontClassItem) {

				CTBox *tBox = ontClassItem->getOntology()->getDataBoxes()->getTBox();
				CBOXHASH<CConcept*,CConcept*>* candidateEqConHash = tBox->getCandidateEquivalentConceptHash(false);
				CBOXHASH<CConcept*,CConcept*>* eqConCandidateHash = tBox->getEquivalentConceptCandidateHash(false);
				CBOXSET<CConcept*>* eqConNonCandidateSet = tBox->getEquivalentConceptNonCandidateSet(false);

				CConcreteOntology *onto = ontClassItem->getOntology();
				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();


				QHash<CConcept*,COptimizedKPSetClassTestingItem*>* conceptSatItemHash = ontClassItem->getConceptSatisfiableTestItemHash();
				QList<COptimizedKPSetClassTestingItem*>* satTestItemContainer = ontClassItem->getConceptSatisfiableTestItemContainer();

				COptimizedKPSetClassTestingItem* topItem = ontClassItem->getConceptSatisfiableTestItem(topConcept,true);
				COptimizedKPSetClassTestingItem* bottomItem = ontClassItem->getConceptSatisfiableTestItem(bottomConcept,true);

				ontClassItem->initTopBottomSatisfiableTestingItems(topItem,bottomItem);

				QList<COptimizedKPSetClassTestingItem*>* nextItemList = ontClassItem->getNextSatisfiableTestingItemList();
				QSet<COptimizedKPSetClassTestingItem*>* nextCandItemSet = ontClassItem->getNextCandidateSatisfiableTestingItemSet();
				QSet<COptimizedKPSetClassTestingItem*>* remainingCandItemSet = ontClassItem->getRemainingCandidateSatisfiableTestingItemSet();

				bool testTopConcept = false;
				if (topConcept->getOperandList() || eqConNonCandidateSet && !eqConNonCandidateSet->isEmpty()) {
					// has not absorbed GCIs
					nextItemList->append(topItem);
					testTopConcept = true;
				} else {
					topItem->setPossibleSubsumptionMapInitialized(true);
				}

				bool invalidatedConRefLinking = false;

				QSet<CConcept*>* equivConNonCandSet = ontClassItem->getEquivaltentConceptNonCandidateSet();
				CBOXHASH<CConcept*,CConcept*>* equivConCandidateHash = ontClassItem->getEquivalentConceptCandidateHash();
				if (eqConCandidateHash) {	
					for (CBOXHASH<CConcept*,CConcept*>::const_iterator it = eqConCandidateHash->constBegin(), itEnd = eqConCandidateHash->constEnd(); it != itEnd; ++it) {
						CConcept* eqCon = it.key();
						CConcept* candCon = it.value();
						equivConCandidateHash->insert(eqCon,candCon);
					}
				}


				QHash<CConcept*,CConceptReferenceLinking*>* conRefLinkDataHash = ontClassItem->getConceptReferenceLinkingDataHash();
				QList<CConcept*> extClassConceptList;
				CPrecomputedSaturationSubsumerExtractor precSatSubsumerExtractor(onto);



				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					for (CBOXSET<CConcept*>::const_iterator it = conceptHash->constBegin(), itEnd = conceptHash->constEnd(); it != itEnd; ++it) {
						CConcept *concept = (*it);
						if (concept && concept != topConcept && concept != bottomConcept || (concept == topConcept && testTopConcept)) {
							if (concept->getOperatorCode() == CCEQ) {
								if (!eqConCandidateHash || !eqConCandidateHash->contains(concept)) {
									equivConNonCandSet->insert(concept);
								}
							}			
							COptimizedKPSetClassTestingItem* subsumerItem = ontClassItem->getConceptSatisfiableTestItem(concept,true);

							CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
							CConceptSatisfiableReferenceLinkingData* conRefSatLinking = (CConceptSatisfiableReferenceLinkingData*)conProcData->getConceptReferenceLinking();
							if (!conRefSatLinking) {
								conRefSatLinking = new CConceptSatisfiableReferenceLinkingData();
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
					COptimizedKPSetClassTestingItem* classItem = (COptimizedKPSetClassTestingItem*)conRefSatLinking->getClassifierReferenceLinkingData();

					bool unsatisfiableFlag = false;
					bool insufficientFlag = false;


					//QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
					//if (iriClassNameString == "http://owl.man.ac.uk/2005/07/sssw/people#sheep") {
					//	bool bug = true;
					//}



					if (precSatSubsumerExtractor.getConceptFlags(concept,&unsatisfiableFlag,&insufficientFlag)) {
						if (!unsatisfiableFlag) {
							bool possibleSubsumerFlag = false;
							precSatSubsumerExtractor.extractSubsumers(concept,classItem,&possibleSubsumerFlag);
							if (equivConNonCandSet && !equivConNonCandSet->isEmpty()) {
								for (CBOXSET<CConcept*>::const_iterator eqConIt = equivConNonCandSet->constBegin(), eqConItEnd = equivConNonCandSet->constEnd(); !possibleSubsumerFlag && eqConIt != eqConItEnd; ++eqConIt) {
									CConcept* eqCon = *eqConIt;
									CConceptProcessData* eqConProcData = (CConceptProcessData*)eqCon->getConceptData();
									CConceptSaturationReferenceLinkingData* eqConRefSatLinking = (CConceptSaturationReferenceLinkingData*)eqConProcData->getConceptReferenceLinking();
									COptimizedKPSetClassTestingItem* eqClassItem = (COptimizedKPSetClassTestingItem*)eqConRefSatLinking->getClassifierReferenceLinkingData();
									if (!classItem->hasSubsumerConceptItem(eqClassItem)) {
										possibleSubsumerFlag = true;
									}
								}
							}
							cint64 foundSubsumerCount = classItem->getSubsumingConceptItemCount();
							classItem->setUnprocessedPredecessorItems(foundSubsumerCount);
							if (!insufficientFlag && !possibleSubsumerFlag) {
								classItem->setResultSatisfiableDerivated(true);
								classItem->setPossibleSubsumptionMapInitialized(true);
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




			void COptimizedKPSetClassSubsumptionClassifierThread::createObviousSubsumptionSatisfiableTestingOrderFromBuildData(COptimizedKPSetClassOntologyClassificationItem* ontClassItem) {

				CTBox *tBox = ontClassItem->getOntology()->getDataBoxes()->getTBox();
				CBOXHASH<CConcept*,CConcept*>* candidateEqConHash = tBox->getCandidateEquivalentConceptHash(false);
				CBOXHASH<CConcept*,CConcept*>* eqConCandidateHash = tBox->getEquivalentConceptCandidateHash(false);
				CBOXSET<CConcept*>* eqConNonCandidateSet = tBox->getEquivalentConceptNonCandidateSet(false);

				CConcreteOntology *onto = ontClassItem->getOntology();
				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();


				QHash<CConcept*,COptimizedKPSetClassTestingItem*>* conceptSatItemHash = ontClassItem->getConceptSatisfiableTestItemHash();
				QList<COptimizedKPSetClassTestingItem*>* satTestItemContainer = ontClassItem->getConceptSatisfiableTestItemContainer();

				COptimizedKPSetClassTestingItem* topItem = ontClassItem->getConceptSatisfiableTestItem(topConcept,true);
				COptimizedKPSetClassTestingItem* bottomItem = ontClassItem->getConceptSatisfiableTestItem(bottomConcept,true);

				ontClassItem->initTopBottomSatisfiableTestingItems(topItem,bottomItem);

				QList<COptimizedKPSetClassTestingItem*>* nextItemList = ontClassItem->getNextSatisfiableTestingItemList();
				QSet<COptimizedKPSetClassTestingItem*>* nextCandItemSet = ontClassItem->getNextCandidateSatisfiableTestingItemSet();
				QSet<COptimizedKPSetClassTestingItem*>* remainingCandItemSet = ontClassItem->getRemainingCandidateSatisfiableTestingItemSet();

				bool testTopConcept = true;
				if (topConcept->getOperandList() || eqConNonCandidateSet && !eqConNonCandidateSet->isEmpty()) {
					// has not absorbed GCIs
					nextItemList->append(topItem);
					testTopConcept = true;
				} else {
					topItem->setPossibleSubsumptionMapInitialized(true);
				}

				bool invalidatedConRefLinking = false;

				QSet<CConcept*>* equivConNonCandSet = ontClassItem->getEquivaltentConceptNonCandidateSet();
				CBOXHASH<CConcept*,CConcept*>* equivConCandidateHash = ontClassItem->getEquivalentConceptCandidateHash();
				if (eqConCandidateHash) {	
					for (CBOXHASH<CConcept*,CConcept*>::const_iterator it = eqConCandidateHash->constBegin(), itEnd = eqConCandidateHash->constEnd(); it != itEnd; ++it) {
						CConcept* eqCon = it.key();
						CConcept* candCon = it.value();
						equivConCandidateHash->insert(eqCon,candCon);
					}
				}


				QHash<CConcept*,CConceptReferenceLinking*>* conRefLinkDataHash = ontClassItem->getConceptReferenceLinkingDataHash();

				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					for (CBOXSET<CConcept*>::const_iterator it = conceptHash->constBegin(), itEnd = conceptHash->constEnd(); it != itEnd; ++it) {
						CConcept *concept = (*it);
						if (concept && concept != topConcept && concept != bottomConcept || (concept == topConcept && testTopConcept)) {
							if (concept->getOperatorCode() == CCEQ) {
								if (!eqConCandidateHash || !eqConCandidateHash->contains(concept)) {
									equivConNonCandSet->insert(concept);
								}
							}
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


							COptimizedKPSetClassTestingItem* subsumerItem = ontClassItem->getConceptSatisfiableTestItem(concept,true);

							CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
							CConceptSatisfiableReferenceLinkingData* conRefSatLinking = (CConceptSatisfiableReferenceLinkingData*)conProcData->getConceptReferenceLinking();
							if (!conRefSatLinking) {
								conRefSatLinking = new CConceptSatisfiableReferenceLinkingData();
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
								COptimizedKPSetClassTestingItem* subsumedItem = ontClassItem->getConceptSatisfiableTestItem(subsumedConcept,true);
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



			QString COptimizedKPSetClassSubsumptionClassifierThread::getStatusString() {
				statUpdateMutex.lock();
				QString string = statusString;
				string.detach();
				statUpdateMutex.unlock();
				return string;
			}



			CClassificationProgress* COptimizedKPSetClassSubsumptionClassifierThread::getClassificationProgress() {
				if (satTestedCount != totalToSatTestClassCount || subsumTestedCount != totalToSubsumTestCount) {					
					cint64 tested = satTestedCount + subsumTestedCount;
					cint64 total = totalToSatTestClassCount + totalToSubsumTestCount;
					cint64 elapsedTime = classStartTime.elapsed();
					double remainingMilliSeconds = 0.;
					double progessPercent = 0.;
					if (elapsedTime <= 0 && total > 0) {
						remainingMilliSeconds = 0.;
					} else {
						remainingMilliSeconds = ((double)elapsedTime / (double)tested) * (total-tested);
					}
					if (total <= 0) {
						progessPercent = 0.;
					} else {
						progessPercent = ((double)tested / (double)total) * 100.;
					}
					cint64 classificCount = mClassificationCount;
					mClassifyProgess.setClassificationCount(classificCount);
					mClassifyProgess.setProgessPercent(progessPercent);
					mClassifyProgess.setRemainingMilliSeconds(remainingMilliSeconds);
					mClassifyProgess.setTestedClasses(satTestedCount);
					mClassifyProgess.setTotalClasses(totalToSatTestClassCount);
					mClassifyProgess.setTestedSubsumptions(subsumTestedCount);
					mClassifyProgess.setTotalSubsumptions(totalToSubsumTestCount);
				} else {
					mClassifyProgess.setProgessPercent(0.);
					mClassifyProgess.setRemainingMilliSeconds(0.);
					mClassifyProgess.setTestedClasses(satTestedCount);
					mClassifyProgess.setTotalClasses(totalToSatTestClassCount);
					mClassifyProgess.setTestedSubsumptions(subsumTestedCount);
					mClassifyProgess.setTotalSubsumptions(totalToSubsumTestCount);
				}
				return &mClassifyProgess;
			}

			bool itemSortLessSubsumptionsThan(const COptimizedKPSetClassTestingItem* item1, const COptimizedKPSetClassTestingItem* item2) {
				return item1->getSubsumingConceptItemCount() < item2->getSubsumingConceptItemCount();
			}

			bool itemSortMoreSubsumptionsThan(const COptimizedKPSetClassTestingItem* item1, const COptimizedKPSetClassTestingItem* item2) {
				return item1->getSubsumingConceptItemCount() > item2->getSubsumingConceptItemCount();
			}



			void COptimizedKPSetClassSubsumptionClassifierThread::testDebugPossibleSubsumerCorrectCounted(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem) {
				QList<COptimizedKPSetClassTestingItem*>* classList = optKPSetClassificationItem->getConceptSatisfiableTestItemContainer();
				cint64 totalCount = 0;
				foreach (COptimizedKPSetClassTestingItem* item, *classList) {
					COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
					if (possSubsumMap) {
						cint64 mapCount = 0;
						for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetClassPossibleSubsumptionData* data = it.value();
							if (data->isSubsumptionUnknown() || data->isUpdateRequired()) {
								mapCount++;
								totalCount++;
							}
						}
						if (mapCount != possSubsumMap->getRemainingPossibleSubsumptionCount()) {
							bool bug = true;
						}
					}
				}
				if (totalCount != optKPSetClassificationItem->getRemainingPossibleSubsumptionTestsCount()) {
					bool bug = true;
				}

			}

			void COptimizedKPSetClassSubsumptionClassifierThread::testDebugPossibleSubsumerCorrectReferenced(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem) {
				QList<COptimizedKPSetClassTestingItem*>* classList = optKPSetClassificationItem->getSatisfiableConceptItemList();
				if (mConfPossSubsumCalcOrderBottomUp) {
					QSet<COptimizedKPSetClassTestingItem*>* remItemSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionClassTestingSet();
					foreach (COptimizedKPSetClassTestingItem* item, *classList) {
						COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
						if (possSubsumMap) {
							for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetClassPossibleSubsumptionData* data = it.value();
								if (data->isSubsumptionUnknown() || data->isUpdateRequired()) {				
									COptimizedKPSetClassTestingItem* possSubsumerItem = data->getClassItem();
									if (!possSubsumerItem->getPossibleSubsumedSet(false)->contains(item) && !optKPSetClassificationItem->getCurrentPossibleSubsumptionTestingItemSet()->contains(possSubsumerItem)) {
										bool bug = true;
									}
									if (!remItemSet->contains(possSubsumerItem) && !optKPSetClassificationItem->getCurrentPossibleSubsumptionTestingItemSet()->contains(possSubsumerItem)) {
										bool bug = true;
									}
								}
							}
						}
					}
				}
			}


			QString COptimizedKPSetClassSubsumptionClassifierThread::createDebugKPSetString(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, const QString& filename) {
				QString debugString;
				QList<COptimizedKPSetClassTestingItem*>* classList = optKPSetClassificationItem->getSatisfiableConceptItemList();
				foreach (COptimizedKPSetClassTestingItem* item, *classList) {
					QString iriClassNameString = CIRIName::getRecentIRIName(item->getTestingConcept()->getClassNameLinker());
					QString classDebugString = QString("Class: %1\r\n").arg(iriClassNameString);
					QString subsumString;
					foreach (COptimizedKPSetClassTestingItem* subsumItem, *item->getSubsumingConceptItemList()) {
						QString subsumIRIClassNameString = CIRIName::getRecentIRIName(subsumItem->getTestingConcept()->getClassNameLinker());
						if (subsumString.isEmpty()) {
							subsumString += subsumIRIClassNameString;
						} else {
							subsumString += QString(", %1").arg(subsumIRIClassNameString);
						}
					}
					classDebugString += QString("Subsumer Classes: %1\r\n").arg(subsumString);

					QString possSubsumString;
					COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
					if (possSubsumMap) {
						for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetClassPossibleSubsumptionData* data = it.value();
							QString subsumIRIClassNameString = CIRIName::getRecentIRIName(data->getClassItem()->getTestingConcept()->getClassNameLinker());
							QString possString;
							if (data->isSubsumptionConfirmed()) {
								possString = "s";
							} else if (data->isSubsumptionInvalided()) {
								possString = "n";
							} else {
								possString = "u";
							}

							if (possSubsumString.isEmpty()) {
								possSubsumString += QString("%1(%2)").arg(subsumIRIClassNameString).arg(possString);
							} else {
								possSubsumString += QString(", %1(%2)").arg(subsumIRIClassNameString).arg(possString);
							}
						}
					}
					classDebugString += QString("Possible Subsumer Classes: %1\r\n\r\n\r\n").arg(possSubsumString);

					debugString += classDebugString;
				}

				QFile file(filename);
				if (file.open(QIODevice::WriteOnly)) {
					file.write(debugString.toLocal8Bit());
					file.close();
				}

				return debugString;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::createNextSubsumtionTest() {


				COntologyClassificationItem *loopOntClassItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !processingOntItemList.isEmpty()) {
					COntologyClassificationItem *ontClassItem = processingOntItemList.first();

					if (ontClassItem == loopOntClassItem) {
						// don't run into infinite loop without doing something
						break;
					}

					COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetClassOntologyClassificationItem *)ontClassItem;

					if (!optKPSetClassificationItem->hasSatisfiableTestingPhaseFinished() && !ontClassItem->isTaxonomyConstructionFailed()) {

						while (!workTestCreated && optKPSetClassificationItem->hasRemainingSatisfiableTests()) {
							// get next satisfiable test

							QList<COptimizedKPSetClassTestingItem*>* nextItemList = optKPSetClassificationItem->getNextSatisfiableTestingItemList();
							QSet<COptimizedKPSetClassTestingItem*>* nextCandItemSet = optKPSetClassificationItem->getNextCandidateSatisfiableTestingItemSet();
							QSet<COptimizedKPSetClassTestingItem*>* remainingCandItemSet = optKPSetClassificationItem->getRemainingCandidateSatisfiableTestingItemSet();


							COptimizedKPSetClassTestingItem* nextSatTestItem = nullptr;
							if (!nextItemList->isEmpty()) {
								nextSatTestItem = nextItemList->takeFirst();
								remainingCandItemSet->remove(nextSatTestItem);
								nextCandItemSet->remove(nextSatTestItem);

							} else if (!nextCandItemSet->isEmpty()) {
								COptimizedKPSetClassTestingItem* minItem = nullptr;
								cint64 minUnpPredCount = 0;
								for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = nextCandItemSet->constBegin(), itEnd = nextCandItemSet->constEnd(); it != itEnd; ++it) {
									COptimizedKPSetClassTestingItem* item = (*it);
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
								COptimizedKPSetClassTestingItem* minItem = nullptr;
								//cint64 minUnpPredCount = 0;
								//for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = remainingCandItemSet->constBegin(), itEnd = remainingCandItemSet->constEnd(); it != itEnd; ++it) {
								//	COptimizedKPSetClassTestingItem* item = (*it);
								//	cint64 predCount = item->getUnprocessedPredecessorItemCount();
								//	if (!minItem || minUnpPredCount < predCount) {
								//		minUnpPredCount = predCount;
								//		minItem = item;
								//	}
								//}
								//nextSatTestItem = minItem;
								QSet<COptimizedKPSetClassTestingItem*>::const_iterator itNext = remainingCandItemSet->constBegin();
								nextSatTestItem = *itNext;
								remainingCandItemSet->remove(nextSatTestItem);
							}


							if (nextSatTestItem && !nextSatTestItem->isSatisfiableTestOrdered()) {
								optKPSetClassificationItem->decRemainingSatisfiableTestsCount();
								optKPSetClassificationItem->incRunningSatisfiableTestsCount();
								if (calculateSatisfiable(optKPSetClassificationItem,nextSatTestItem)) {
									workTestCreated = true;
								} else {
									interpreteSatisfiableResult(optKPSetClassificationItem,nextSatTestItem->getTestingConcept(),nextSatTestItem->getSatisfiableTestedResult());
								}
							}
						}


						if (!workTestCreated) {
							if (optKPSetClassificationItem->hasAllSatisfiableTestsCompleted()) {
								optKPSetClassificationItem->setSatisfiableTestingPhaseFinished(true);
								OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
								// initialize possible subsumption testing phase


								cint64 remainingSubSumTestCount = 0;

								QSet<COptimizedKPSetClassTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionClassTestingSet();

								QList<COptimizedKPSetClassTestingItem*>* nextItemList = optKPSetClassificationItem->getNextPossibleSubsumptionTestingItemList();

								COptimizedKPSetClassTestingItem* topItem = optKPSetClassificationItem->getTopConceptSatisfiableTestItem();
								COptimizedKPSetClassTestingItem* bottomItem = optKPSetClassificationItem->getBottomConceptSatisfiableTestItem();

								topItem->setPropagationConnected(true);

								QList<COptimizedKPSetClassTestingItem*> classList(*optKPSetClassificationItem->getSatisfiableConceptItemList());
								qSort(classList.begin(),classList.end(),itemSortLessSubsumptionsThan);
								QList<COptimizedKPSetClassTestingItem*> resevePossSubsumClassList;
								foreach (COptimizedKPSetClassTestingItem* item, classList) {
									COptimizedKPSetClassPossibleSubsumptionMap* possSubSumMap = item->getPossibleSubsumptionMap(false);
									if (possSubSumMap && possSubSumMap->hasRemainingPossibleSubsumptions()) {	
										if (mConfPossSubsumCalcOrderTopDown) {
											nextItemList->append(item);
											remTestingSet->insert(item);
										}
										remainingSubSumTestCount += possSubSumMap->getRemainingPossibleSubsumptionCount();
										resevePossSubsumClassList.append(item);
									}
									if (!item->isPossibleSubsumptionMapInitialized()) {
										bool bug = true;
									}

									QList<COptimizedKPSetClassTestingItem*>* subsumerList = item->getSubsumingConceptItemList();
									COptimizedKPSetClassTestingItem* maxSubsumerItem = nullptr;
									cint64 maxSubsumerCount = 0;
									for (QList<COptimizedKPSetClassTestingItem*>::const_iterator it = subsumerList->constBegin(), itEnd = subsumerList->constEnd(); it != itEnd; ++it) {
										COptimizedKPSetClassTestingItem* subsumerItem = *it;
										if (!maxSubsumerItem || subsumerItem->getSubsumingConceptItemCount() > maxSubsumerCount) {
											maxSubsumerCount = subsumerItem->getSubsumingConceptItemCount();
											maxSubsumerItem = subsumerItem;
										}
									}
									if (!maxSubsumerItem) {
										maxSubsumerItem = topItem;
									}

									bool upPropConnected = false;
									QSet<COptimizedKPSetClassTestingItem*>* upPropSet = item->getUpPropagationItemSet();
									if (item != topItem) {
										upPropSet->insert(maxSubsumerItem);
										maxSubsumerItem->addDownPropagationItem(item);
										upPropConnected |= maxSubsumerItem->isPropagationConnected();
									}

									for (QList<COptimizedKPSetClassTestingItem*>::const_iterator itSub = subsumerList->constBegin(), itSubEnd = subsumerList->constEnd(); itSub != itSubEnd; ++itSub) {
										COptimizedKPSetClassTestingItem* subsumerItem = *itSub;	
										bool alreadySubsumed = false;
										for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator itProp = upPropSet->constBegin(), itPropEnd = upPropSet->constEnd(); !alreadySubsumed && itProp != itPropEnd; ++itProp) {
											COptimizedKPSetClassTestingItem* propItem = *itProp;
											if (propItem == subsumerItem || propItem->hasSubsumerConceptItem(subsumerItem)) {
												alreadySubsumed = true;
											}
										}
										if (!alreadySubsumed) {
											upPropSet->insert(subsumerItem);
											subsumerItem->addDownPropagationItem(item);
											upPropConnected |= subsumerItem->isPropagationConnected();
										}
									}
									if (!upPropConnected) {
										for (QList<COptimizedKPSetClassTestingItem*>::const_iterator itSub = subsumerList->constBegin(), itSubEnd = subsumerList->constEnd(); itSub != itSubEnd && !upPropConnected; ++itSub) {
											COptimizedKPSetClassTestingItem* subsumerItem = *itSub;	
											if (subsumerItem->isPropagationConnected()) {
												upPropSet->insert(subsumerItem);
												subsumerItem->addDownPropagationItem(item);
												upPropConnected |= subsumerItem->isPropagationConnected();
											}
										}
										if (!upPropConnected) {
											upPropSet->insert(topItem);
											topItem->addDownPropagationItem(item);
											upPropConnected |= topItem->isPropagationConnected();
										}
									}
									item->setPropagationConnected(upPropConnected);



								}
								OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
								OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(createDebugKPSetString(optKPSetClassificationItem,"classkpsets-inital.txt"));


								foreach (COptimizedKPSetClassTestingItem* item, classList) {
									COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
									QSet<COptimizedKPSetClassTestingItem*>* upPropSet = item->getUpPropagationItemSet();
									if (!possSubsumMap && item->isPossibleSubsumptionMapInitialized()) {
										for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator itProp = upPropSet->constBegin(), itPropEnd = upPropSet->constEnd(); itProp != itPropEnd; ++itProp) {
											COptimizedKPSetClassTestingItem* upPropItem = *itProp;

											COptimizedKPSetClassPossibleSubsumptionMap* upPropItemPossSubsumMap = upPropItem->getPossibleSubsumptionMap(false);
											if (upPropItemPossSubsumMap) {
												for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator itUpPoss = upPropItemPossSubsumMap->constBegin(), itUpPossEnd = upPropItemPossSubsumMap->constEnd(); itUpPoss != itUpPossEnd; ++itUpPoss) {
													COptimizedKPSetClassPossibleSubsumptionData* upPossData = itUpPoss.value();
													if (!item->hasSubsumerConceptItem(upPossData->getClassItem()) && item != upPossData->getClassItem()) {
														if (!upPossData->isSubsumptionInvalided()) {
															upPossData->setSubsumptionInvalid(true);
															if (upPossData->isUpdateRequired()) {
																prunePossibleSubsumptions(optKPSetClassificationItem,upPropItem,upPossData);
															}
														}
														OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
													}
												}
											}
										}
									} else {
										for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator itProp = upPropSet->constBegin(), itPropEnd = upPropSet->constEnd(); itProp != itPropEnd; ++itProp) {
											COptimizedKPSetClassTestingItem* upPropItem = *itProp;

											COptimizedKPSetClassPossibleSubsumptionMap* upPropItemPossSubsumMap = upPropItem->getPossibleSubsumptionMap(false);
											if (upPropItemPossSubsumMap) {
												COptimizedKPSetClassPossibleSubsumptionMap::const_iterator itPoss = possSubsumMap->constBegin(), itPossEnd = possSubsumMap->constEnd();
												COptimizedKPSetClassPossibleSubsumptionMap::const_iterator itUpPoss = upPropItemPossSubsumMap->constBegin(), itUpPossEnd = upPropItemPossSubsumMap->constEnd();
												while (itPoss != itPossEnd && itUpPoss != itUpPossEnd) {
													CConcept* possCon = itPoss.key().getConcept();
													CConcept* possUpCon = itUpPoss.key().getConcept();
													if (possCon->getConceptTag() == possUpCon->getConceptTag()) {
														++itPoss;
														++itUpPoss;
													} else if (possCon->getConceptTag() < possUpCon->getConceptTag()) {
														++itPoss;
													} else if (possCon->getConceptTag() > possUpCon->getConceptTag()) {
														COptimizedKPSetClassPossibleSubsumptionData* upPossData = itUpPoss.value();
														if (!item->hasSubsumerConceptItem(upPossData->getClassItem()) && item != upPossData->getClassItem()) {
															if (!upPossData->isSubsumptionInvalided()) {
																upPossData->setSubsumptionInvalid(true);
																if (upPossData->isUpdateRequired()) {
																	prunePossibleSubsumptions(optKPSetClassificationItem,upPropItem,upPossData);
																}
																OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
															}
														}
														++itUpPoss;
													}
												}
												while (itUpPoss != itUpPossEnd) {
													COptimizedKPSetClassPossibleSubsumptionData* upPossData = itUpPoss.value();
													if (!item->hasSubsumerConceptItem(upPossData->getClassItem()) && item != upPossData->getClassItem()) {
														if (!upPossData->isSubsumptionInvalided()) {
															upPossData->setSubsumptionInvalid(true);
															if (upPossData->isUpdateRequired()) {
																prunePossibleSubsumptions(optKPSetClassificationItem,upPropItem,upPossData);
															}
															OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
														}
													}
													++itUpPoss;
												}
											}
										}
									}
								}

								foreach (COptimizedKPSetClassTestingItem* item, resevePossSubsumClassList) {
									COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
									if (possSubsumMap) {
										if (possSubsumMap->hasRemainingPossibleSubsumptions()) {
											for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
												COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = it.value();
												
												if (mConfPossSubsumPseudoModelPretest) {
													if (possSubsumData->isSubsumptionUnknown()) {
														bool isSubsumptionFlag = false;
														if (fastPseudoModelSubsumptionClassPrecheckTest(optKPSetClassificationItem,item,possSubsumData->getClassItem(),&isSubsumptionFlag)) {
															CClassifierStatistics* ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
															if (ontClassStat) {
																ontClassStat->incPseudoModelMergedTestedSubsumptionCount(1);
															}
															if (!isSubsumptionFlag) {
																optKPSetClassificationItem->incRunningPossibleSubsumptionTestsCount();
																possSubsumData->setSubsumptionInvalid(true);
																++mPseudoModelPretestSubsumptionCalculationCount;
																interpreteSubsumptionResult(optKPSetClassificationItem,item->getTestingConcept(),possSubsumData->getClassItem()->getTestingConcept(),possSubsumData->isSubsumptionConfirmed());
															}
														}
													}
												}
												if (mConfPossSubsumCalcOrderBottomUp) {
													if (possSubsumData->isSubsumptionUnknown() || possSubsumData->isUpdateRequired()) {
														COptimizedKPSetClassTestingItem* possSubsumerItem = possSubsumData->getClassItem();
														possSubsumerItem->getPossibleSubsumedSet(true)->insert(item);
														if (!remTestingSet->contains(possSubsumerItem)) {
															remTestingSet->insert(possSubsumerItem);
															nextItemList->append(possSubsumerItem);
														}
													}
												}
											}
										}
									}
								}


								if (mConfPossSubsumCalcOrderBottomUp) {
									for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = remTestingSet->constBegin(), itEnd = remTestingSet->constEnd(); it != itEnd; ++it) {
										COptimizedKPSetClassTestingItem* possSubsumerItem = *it;
										QList<COptimizedKPSetClassTestingItem*>* subsumersSortedList = new QList<COptimizedKPSetClassTestingItem*>(possSubsumerItem->getPossibleSubsumedSet(false)->toList());										
										qSort(subsumersSortedList->begin(),subsumersSortedList->end(),itemSortMoreSubsumptionsThan);
										possSubsumerItem->setPossibleSubsumedList(subsumersSortedList);
									}
									OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectReferenced(optKPSetClassificationItem));
								}

								if (mConfWriteDebuggingData) {
									createDebugKPSetString(optKPSetClassificationItem,"classkpsets-inital-pruned.txt");
								}
								OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(createDebugKPSetString(optKPSetClassificationItem,"classkpsets-inital-pruned.txt"));
								OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));

							}
						}
					}

					if (optKPSetClassificationItem->hasSatisfiableTestingPhaseFinished() && !optKPSetClassificationItem->hasPossibleSubsumptionTestingPhaseFinished() && !ontClassItem->isTaxonomyConstructionFailed()) {

						QList<COptimizedKPSetClassTestingItem*>* nextItemList = optKPSetClassificationItem->getNextPossibleSubsumptionTestingItemList();
						QSet<COptimizedKPSetClassTestingItem*>* currentItemSet = optKPSetClassificationItem->getCurrentPossibleSubsumptionTestingItemSet();

						bool canContinueSearchingForTests = true;

						while (!workTestCreated && optKPSetClassificationItem->hasRemainingPossibleSubsumptionTests() && canContinueSearchingForTests) {
							// get next possible subsumption test
							canContinueSearchingForTests = false;

							COptimizedKPSetClassTestingItem* nextPossSubsumTestItem = nullptr;
							if (!nextItemList->isEmpty()) {
								nextPossSubsumTestItem = nextItemList->takeFirst();
							}

							if (nextPossSubsumTestItem) {
								canContinueSearchingForTests = true;

								if (mConfPossSubsumCalcOrderTopDown) {

									COptimizedKPSetClassPossibleSubsumptionMap* possSubsumerMap = nextPossSubsumTestItem->getPossibleSubsumptionMap(false);
									if (possSubsumerMap && possSubsumerMap->hasRemainingPossibleSubsumptions()) {

										COptimizedKPSetClassPossibleSubsumptionData* possSubsumCalcData = getNextPossibleSubsumptionCalculation(optKPSetClassificationItem,nextPossSubsumTestItem);
										if (possSubsumCalcData) {
											currentItemSet->insert(nextPossSubsumTestItem);
											optKPSetClassificationItem->incRunningPossibleSubsumptionTestsCount();

											if (calculateSubsumption(optKPSetClassificationItem,nextPossSubsumTestItem,possSubsumCalcData->getClassItem(),possSubsumCalcData)) {
												workTestCreated = true;
											} else {
												interpreteSubsumptionResult(optKPSetClassificationItem,nextPossSubsumTestItem->getTestingConcept(),possSubsumCalcData->getClassItem()->getTestingConcept(),possSubsumCalcData->isSubsumptionConfirmed());
											}
										}
									} else {
										QSet<COptimizedKPSetClassTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionClassTestingSet();
										remTestingSet->remove(nextPossSubsumTestItem);
									}

								} else if (mConfPossSubsumCalcOrderBottomUp) {
									COptimizedKPSetClassPossibleSubsumptionData* possSubsumCalcData = nullptr;
									COptimizedKPSetClassTestingItem* possSubsumedItem = nullptr;

									OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
									OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectReferenced(optKPSetClassificationItem));

									CConcept* possSubsumerConcept = nextPossSubsumTestItem->getTestingConcept();
									CConcept* candidateConcept = possSubsumerConcept;
									if (possSubsumerConcept->getOperatorCode() == CCEQ) {
										candidateConcept = optKPSetClassificationItem->getEquivalentConceptCandidateHash()->value(possSubsumerConcept);
										if (!candidateConcept) {
											candidateConcept = possSubsumerConcept;
										}
									}

									QList<COptimizedKPSetClassTestingItem*>* possSubsumedSortedList = nextPossSubsumTestItem->getPossibleSubsumedList();
									if (possSubsumedSortedList && !possSubsumedSortedList->isEmpty()) {										
										while (!possSubsumCalcData && !possSubsumedSortedList->isEmpty()) {
											possSubsumedItem = possSubsumedSortedList->takeFirst();
											if (nextPossSubsumTestItem->getPossibleSubsumedSet(false)->contains(possSubsumedItem)) {
												nextPossSubsumTestItem->getPossibleSubsumedSet(false)->remove(possSubsumedItem);
												COptimizedKPSetClassPossibleSubsumptionMap* possSubsumerMap = possSubsumedItem->getPossibleSubsumptionMap(false);
												if (possSubsumerMap && possSubsumerMap->hasRemainingPossibleSubsumptions()) {
													possSubsumCalcData = possSubsumerMap->value(CConceptTagComparer(candidateConcept));
												}
											}
										}
									}

									if (possSubsumCalcData) {
										//if ((possSubsumCalcData->isUpdateRequired() || possSubsumCalcData->isSubsumptionUnknown())) {
											currentItemSet->insert(nextPossSubsumTestItem);
											optKPSetClassificationItem->incRunningPossibleSubsumptionTestsCount();


											//QString iriClassNameString1 = CIRIName::getRecentIRIName(possSubsumedItem->getTestingConcept()->getClassNameLinker());
											//QString iriClassNameString2 = CIRIName::getRecentIRIName(nextPossSubsumTestItem->getTestingConcept()->getClassNameLinker());
											//if (iriClassNameString2 == "http://www.owllink.org/testsuite/npt8-D#TransitionMetalAtom" && iriClassNameString1 == "http://www.owllink.org/testsuite/npt8-D#TitaniumFamilyAtom") {
											//	bool bug = true;
											//}

											//if (subsumCalcedSet.contains(QPair<CConcept*,CConcept*>(possSubsumedItem->getTestingConcept(),nextPossSubsumTestItem->getTestingConcept()))) {
											//	bool bug = true;
											//}
											//subsumCalcedSet.insert(QPair<CConcept*,CConcept*>(possSubsumedItem->getTestingConcept(),nextPossSubsumTestItem->getTestingConcept()));

											if (calculateSubsumption(optKPSetClassificationItem,possSubsumedItem,nextPossSubsumTestItem,possSubsumCalcData)) {
												workTestCreated = true;
											} else {
												interpreteSubsumptionResult(optKPSetClassificationItem,possSubsumedItem->getTestingConcept(),nextPossSubsumTestItem->getTestingConcept(),possSubsumCalcData->isSubsumptionConfirmed());
											}
											OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectReferenced(optKPSetClassificationItem));
										//}
									} else {
										QSet<COptimizedKPSetClassTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionClassTestingSet();
										remTestingSet->remove(nextPossSubsumTestItem);
										OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectReferenced(optKPSetClassificationItem));
									}

								}
							}
						}

						if (!workTestCreated && currentItemSet->isEmpty()) {
							optKPSetClassificationItem->setPossibleSubsumptionTestingPhaseFinished(true);
						}
					}


					if (!workTestCreated) {
						if (optKPSetClassificationItem->hasPossibleSubsumptionTestingPhaseFinished() || ontClassItem->isTaxonomyConstructionFailed()) {
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


			COptimizedKPSetClassPossibleSubsumptionData* COptimizedKPSetClassSubsumptionClassifierThread::getNextPossibleSubsumptionCalculation(COptimizedKPSetClassOntologyClassificationItem *ontClassItem, COptimizedKPSetClassTestingItem* classItem) {
				COptimizedKPSetClassPossibleSubsumptionData* possSubsumer = nullptr;
				// I don't know which is the best, simply return the first one
				// sort by subsumer count?
				COptimizedKPSetClassPossibleSubsumptionMap* possSubsumerMap = classItem->getPossibleSubsumptionMap(false);
				if (possSubsumerMap) {
					if (mConfPossSubsumCalcOrderConceptSorted) {
						for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator it = possSubsumerMap->constBegin(), itEnd = possSubsumerMap->constEnd(); !possSubsumer && it != itEnd; ++it) {
							COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = it.value();
							if (possSubsumData->isSubsumptionUnknown()) {
								possSubsumer = possSubsumData;
							} else if (possSubsumData->isUpdateRequired()) {
								prunePossibleSubsumptions(ontClassItem,classItem,possSubsumData);							
							}
							OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(ontClassItem));
						}
					}
				}
				return possSubsumer;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::calculateSatisfiable(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* nextSatTestItem) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CClassificationWorkItem *workItem = 0;

				CConcept* concept = nextSatTestItem->getTestingConcept();
				nextSatTestItem->setSatisfiableTestOrdered(true);

				bool satisfiableFlag = false;

#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGB
				QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
#endif
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
				statUpdateMutex.lock();
				statusString = QString("[%2/%3] Calculating, is '%1' satisfiable").arg(iriClassNameString).arg(optKPSetClassificationItem->getClassifierStatistics()->getTestedSatisfiableCount()).arg(optKPSetClassificationItem->getClassifierStatistics()->getTotalSatisfiableCount());
				statUpdateMutex.unlock();
#endif
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGLOGOUTPUT
				LOG(INFO,getLogDomain(),QString("Calculating whether '%1' is satisfiable").arg(iriClassNameString),this);
#endif
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCOUTOUTPUT
				cout<<QString("Calculating whether '%1' is satisfiable").arg(iriClassNameString).toLocal8Bit().data()<<endl<<endl;
#endif

				//if (iriClassNameString == "http://owl.man.ac.uk/2005/07/sssw/people#sheep") {
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
				if (fastSatisfiableOnlySubClassPrecheckTest(optKPSetClassificationItem,nextSatTestItem,&satisfiableFlag)) {
					nextSatTestItem->setSatisfiableTested(true);
					nextSatTestItem->setSatisfiableTestedResult(satisfiableFlag);
					return false;
				}

				CSatisfiableCalculationJobGenerator satCalcJobGen(optKPSetClassificationItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(concept);
				QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *> *workHash = optKPSetClassificationItem->getWorkItemHash();

				CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
				if (ontClassStat) {
					ontClassStat->incCalculatedTestedSatisfiableCount(1);
				}

				workItem = new CClassificationWorkItem(satCalcJob,concept);
				workHash->insertMulti(satCalcJob,workItem);
				workOntItemHash.insert(workItem,optKPSetClassificationItem);

				QHash<CConcept*,CConceptReferenceLinking*>* conRefLinkDataHash = optKPSetClassificationItem->getConceptReferenceLinkingDataHash();
				cint64 extFlags = CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTALL;
				// TODO: set extraction flags


				satCalcJob->setSatisfiableClassificationMessageAdapter(new CSatisfiableTaskClassificationMessageAdapter(concept,optKPSetClassificationItem->getOntology(),this,conRefLinkDataHash,extFlags));

				optKPSetClassificationItem->incCurrentCalculatingCount();
				processCalculationJob(satCalcJob,optKPSetClassificationItem,workItem);
				++mCreatedCalculationTaskCount;
				return true;
			}





			bool COptimizedKPSetClassSubsumptionClassifierThread::calculateSubsumption(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* subsumedItem, COptimizedKPSetClassTestingItem* possSubsumerItem, COptimizedKPSetClassPossibleSubsumptionData* possSubsumData) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CClassificationWorkItem *workItem = 0;
				++mOrderedSubsumptionCalculationCount;

				CConcept* subsumedConcept = subsumedItem->getTestingConcept();
				CConcept* subsumerConcept = possSubsumerItem->getTestingConcept();

				bool isSubsumptionFlag = false;
				bool precheckedSubsum = false;


#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGB
				QString iriClassNameString1 = CIRIName::getRecentIRIName(subsumedConcept->getClassNameLinker());
				QString iriClassNameString2 = CIRIName::getRecentIRIName(subsumerConcept->getClassNameLinker());
#endif
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
				statUpdateMutex.lock();
				statusString = QString("Calculating, is '%1' subsumed by '%2'").arg(iriClassNameString1).arg(iriClassNameString2);
				statUpdateMutex.unlock();
#endif
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGLOGOUTPUT
				LOG(INFO,getLogDomain(),QString("Calculating whether '%1' is subsumed by '%2'").arg(iriClassNameString1).arg(iriClassNameString2),this);
#endif
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCOUTOUTPUT
				cout<<QString("Calculating whether '%1' is subsumed by '%2'").arg(iriClassNameString1).arg(iriClassNameString2).toLocal8Bit().data()<<endl<<endl;
#endif


				//if (iriClassNameString1 == "http://www.bootstrep.eu/ontology/GRO#BindingOfProteinToProteinBindingSiteOfDNA" && iriClassNameString2 == "http://www.bootstrep.eu/ontology/GRO#DNAProteinInteraction" ) {
				//	bool bug = true;
				//}


				CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
				if (possSubsumData && possSubsumData->isSubsumptionKnown()) {
					return false;
				} else if (possSubsumData && subsumedItem->hasSubsumerConceptItem(possSubsumerItem)) {					
					possSubsumData->setSubsumptionConfirmed(true);
					return false;
				} else if (mConfPossSubsumPseudoModelTest && fastPseudoModelSubsumptionClassPrecheckTest(optKPSetClassificationItem,subsumedItem,possSubsumerItem,&isSubsumptionFlag)) {
					if (ontClassStat) {
						ontClassStat->incPseudoModelMergedTestedSubsumptionCount(1);
					}
					return false;
				}

				CSatisfiableCalculationJobGenerator satCalcJobGen(optKPSetClassificationItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(subsumedConcept,false,subsumerConcept,true);
				QHash<CSatisfiableCalculationJob *, CClassificationWorkItem *> *workHash = optKPSetClassificationItem->getWorkItemHash();

				if (ontClassStat) {
					ontClassStat->incCalculatedTestedSubsumptionCount(1);
					ontClassStat->incToldSubsumptionCount(1);
				}

				workItem = new CClassificationWorkItem(satCalcJob,subsumerConcept,subsumedConcept);
				workHash->insertMulti(satCalcJob,workItem);
				workOntItemHash.insert(workItem,optKPSetClassificationItem);

				QHash<CConcept*,CConceptReferenceLinking*>* conRefLinkDataHash = optKPSetClassificationItem->getConceptReferenceLinkingDataHash();
				cint64 extFlags = CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTSUBSUMERSOTHERNODES | 
						CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTPOSSIBLESUBSUMERSROOTNODE | 
						CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTPOSSIBLESUBSUMERSOTHERNODES | 
						CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTOTHERNODESMULTIPLEDEPENDENCY;
				// TODO: set extraction flags

				satCalcJob->setSatisfiableClassificationMessageAdapter(new CSatisfiableTaskClassificationMessageAdapter(subsumedConcept,optKPSetClassificationItem->getOntology(),this,conRefLinkDataHash,extFlags));

				optKPSetClassificationItem->incCurrentCalculatingCount();
				processCalculationJob(satCalcJob,optKPSetClassificationItem,workItem);
				optKPSetClassificationItem->incCalculatedPossibleSubsumerCount();
				++mCreatedCalculationTaskCount;
				return true;
			}






			bool COptimizedKPSetClassSubsumptionClassifierThread::fastSatisfiableOnlySubClassPrecheckTest(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* nextSatTestItem, bool* isSatisfiableFlag) {
				QHash<CConcept*,COptimizedKPSetClassTestingItem*>* conceptSatItemHash = optKPSetClassificationItem->getConceptSatisfiableTestItemHash();
				CBOXHASH<CConcept*,CConcept*>* triggerImpHash = optKPSetClassificationItem->getOntology()->getTBox()->getTriggerImplicationHash(false);
				CConcept* concept = nextSatTestItem->getTestingConcept();
				bool satFlag = false;
				bool testComplete = false;
				if (concept->getOperatorCode() == CCSUB) {
					cint64 conCount = concept->getOperandCount();
					if (conCount == 1) {
						CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
						CConcept* firstConcept = opLinkerIt->getData();
						cint64 firstOpCode = firstConcept->getOperatorCode();
						CConcept* superClassConcept = nullptr;
						if (!opLinkerIt->isNegated() && (firstOpCode == CCATOM || firstOpCode == CCSUB || firstOpCode == CCEQ)) {
							superClassConcept = firstConcept;
						}

						if (superClassConcept) {
							COptimizedKPSetClassTestingItem* superClassConItem = conceptSatItemHash->value(superClassConcept);
							if (superClassConItem && superClassConItem->isSatisfiableTested()) {
								if (!triggerImpHash || !triggerImpHash->contains(superClassConcept)) {
									QList<COptimizedKPSetClassTestingItem*>* subsumedSubsumingConList = superClassConItem->getSubsumingConceptItemList();
									for (QList<COptimizedKPSetClassTestingItem*>::const_iterator it = subsumedSubsumingConList->constBegin(), itEnd = subsumedSubsumingConList->constEnd(); it != itEnd; ++it) {
										COptimizedKPSetClassTestingItem* subsumedItem = (*it);
										if (nextSatTestItem != subsumedItem && superClassConItem != subsumedItem) {
											nextSatTestItem->addSubsumingConceptItem(subsumedItem);
										}
									}
									nextSatTestItem->addSubsumingConceptItem(superClassConItem);
									satFlag = superClassConItem->getSatisfiableTestedResult();
									testComplete = true;

									if (superClassConItem->isClassPseudoModelInitalized()) {
										CClassificationClassPseudoModel* superClassPMModel = superClassConItem->getClassPseudoModel();
										CClassificationClassPseudoModel* classPMModel = nextSatTestItem->getClassPseudoModel();

										CTempMemoryPoolContainerAllocationManager tempMemoryPoolConAllocMan(mContext.getMemoryPoolAllocationManager());
										CContext* tmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(&tempMemoryPoolConAllocMan,&tempMemoryPoolConAllocMan);
										CClassificationClassPseudoModelHash* pmModelHash = CObjectParameterizingAllocator< CClassificationClassPseudoModelHash,CContext* >::allocateAndConstructAndParameterize(&tempMemoryPoolConAllocMan,tmpContext);
										pmModelHash->initPseudoModelHash(superClassPMModel->getPseudoModelHash());
										CClassificationClassPseudoModelData* pmModelData = pmModelHash->getPseudoModelData(0,true);

										CClassificationClassPseudoModelConceptMap* pmModelConMap = pmModelData->getPseudoModelConceptMap(true);
										CClassificationClassPseudoModelConceptData& pmModelConData = (*pmModelConMap)[CConceptTagComparer(concept)];
										pmModelConData.setDeterministic(true);

										optKPSetClassificationItem->addMemoryPools(tempMemoryPoolConAllocMan.takeMemoryPools());
										classPMModel->setPseudoModelHash(pmModelHash);
										nextSatTestItem->setClassPseudoModelInitalized(true);
									}

									if (superClassConItem->isPossibleSubsumptionMapInitialized()) {
										COptimizedKPSetClassPossibleSubsumptionMap* superPosSubsumMap = superClassConItem->getPossibleSubsumptionMap(false);
										if (superPosSubsumMap && !nextSatTestItem->isPossibleSubsumptionMapInitialized()) {
											COptimizedKPSetClassPossibleSubsumptionMap* subPosSubsumMap = nextSatTestItem->getPossibleSubsumptionMap(true);
											for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator it = superPosSubsumMap->constBegin(), itEnd = superPosSubsumMap->constEnd(); it != itEnd; ++it) {
												CConcept* con = it.key().getConcept();
												COptimizedKPSetClassPossibleSubsumptionData* data = it.value();
												if (data->isSubsumptionUnknown() && !nextSatTestItem->hasSubsumerConceptItem(data->getClassItem()) && nextSatTestItem != data->getClassItem()) {
													COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = new COptimizedKPSetClassPossibleSubsumptionData(data->getClassItem());
													subPosSubsumMap->insert(CConceptTagComparer(con),possSubsumData);
													incRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,subPosSubsumMap);
												}
											}
										}
										nextSatTestItem->setPossibleSubsumptionMapInitialized(true);
									}
								}
							}
						}
					}
				} else if (concept->getOperatorCode() == CCATOM) {
					COptimizedKPSetClassTestingItem* topClassConItem = optKPSetClassificationItem->getTopConceptSatisfiableTestItem();
					if (topClassConItem->isPossibleSubsumptionMapInitialized()) {

						satFlag = true;
						testComplete = true;
						CClassificationClassPseudoModel* classPMModel = nextSatTestItem->getClassPseudoModel();

						CTempMemoryPoolContainerAllocationManager tempMemoryPoolConAllocMan(mContext.getMemoryPoolAllocationManager());
						CContext* tmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(&tempMemoryPoolConAllocMan,&tempMemoryPoolConAllocMan);
						CClassificationClassPseudoModelHash* pmModelHash = CObjectParameterizingAllocator< CClassificationClassPseudoModelHash,CContext* >::allocateAndConstructAndParameterize(&tempMemoryPoolConAllocMan,tmpContext);
						CClassificationClassPseudoModelData* pmModelData = pmModelHash->getPseudoModelData(0,true);

						CClassificationClassPseudoModelConceptMap* pmModelConMap = pmModelData->getPseudoModelConceptMap(true);
						CClassificationClassPseudoModelConceptData& pmModelConData = (*pmModelConMap)[CConceptTagComparer(concept)];
						pmModelConData.setDeterministic(true);

						optKPSetClassificationItem->addMemoryPools(tempMemoryPoolConAllocMan.takeMemoryPools());
						classPMModel->setPseudoModelHash(pmModelHash);
						nextSatTestItem->setClassPseudoModelInitalized(true);

					
						COptimizedKPSetClassPossibleSubsumptionMap* superPosSubsumMap = topClassConItem->getPossibleSubsumptionMap(false);
						if (superPosSubsumMap) {
							COptimizedKPSetClassPossibleSubsumptionMap* subPosSubsumMap = nextSatTestItem->getPossibleSubsumptionMap(true);
							for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator it = superPosSubsumMap->constBegin(), itEnd = superPosSubsumMap->constEnd(); it != itEnd; ++it) {
								CConcept* con = it.key().getConcept();
								COptimizedKPSetClassPossibleSubsumptionData* data = it.value();
								if (data->isSubsumptionUnknown() && !nextSatTestItem->hasSubsumerConceptItem(data->getClassItem()) && nextSatTestItem != data->getClassItem()) {
									COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = new COptimizedKPSetClassPossibleSubsumptionData(data->getClassItem());
									subPosSubsumMap->insert(CConceptTagComparer(con),possSubsumData);
									incRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,subPosSubsumMap);
								}
							}
						}
						nextSatTestItem->setPossibleSubsumptionMapInitialized(true);
					}
				}
				if (satFlag && isSatisfiableFlag) {
					*isSatisfiableFlag = true;
				}
				return testComplete;
			}


			void COptimizedKPSetClassSubsumptionClassifierThread::incRemainingPossibleSubsumptionTestingCount(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassPossibleSubsumptionMap* posSubsumMap) {
				optKPSetClassificationItem->incPossibleSubsumerCount();
				optKPSetClassificationItem->incRemainingPossibleSubsumptionTestsCount();
				posSubsumMap->incRemainingPossibleSubsumptionCount();
				++totalToSubsumTestCount;
			}


			void COptimizedKPSetClassSubsumptionClassifierThread::decRemainingPossibleSubsumptionTestingCount(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassPossibleSubsumptionMap* posSubsumMap, bool subsumptionConfirmed) {
				if (subsumptionConfirmed) {
					optKPSetClassificationItem->incTruePossibleSubsumerCount();
				} else {
					optKPSetClassificationItem->incFalsePossibleSubsumerCount();
				}
				optKPSetClassificationItem->decRemainingPossibleSubsumptionTestsCount();
				posSubsumMap->decRemainingPossibleSubsumptionCount();
				++subsumTestedCount;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::fastPseudoModelSubsumptionClassPrecheckTest(COptimizedKPSetClassOntologyClassificationItem *optSubClassItem, COptimizedKPSetClassTestingItem* subsumedItem, COptimizedKPSetClassTestingItem* subsumerItem, bool* isSubsumptionFlag) {
				if (subsumedItem->isClassPseudoModelInitalized() && subsumerItem->isClassPseudoModelInitalized()) {
					CClassificationClassPseudoModel* subsumedPM = subsumedItem->getClassPseudoModel();
					CClassificationClassPseudoModel* subsumerPM = subsumerItem->getClassPseudoModel();


					CClassificationClassPseudoModelHash* subsumedPMHash = subsumedPM->getPseudoModelHash();
					CClassificationClassPseudoModelHash* subsumerPMHash = subsumerPM->getPseudoModelHash();

					CClassificationClassPseudoModelData* subsumedPMData = subsumedPMHash->getPseudoModelData(0,false);
					CClassificationClassPseudoModelData* subsumerPMData = subsumerPMHash->getPseudoModelData(0,false);

					if (subsumedPMData && subsumerPMData) {
						bool isSubSumPossible =  isPseudoModelSubsumerPossible(optSubClassItem,subsumedPMData,subsumedPMHash,subsumerPMData,subsumerPMHash);
						if (!isSubSumPossible) {
							if (isSubsumptionFlag) {
								*isSubsumptionFlag = false;
							}
							return true;
						}
					}
					return false;
				}
				return false;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::isPseudoModelSubsumerPossible(COptimizedKPSetClassOntologyClassificationItem *optSubClassItem, CClassificationClassPseudoModelData* subsumedPMData, CClassificationClassPseudoModelHash* subsumedPMHash, CClassificationClassPseudoModelData* subsumerPMData, CClassificationClassPseudoModelHash* subsumerPMHash) {
				if (subsumedPMData->hasValidConceptMap() && subsumerPMData->hasValidConceptMap()) {
					CClassificationClassPseudoModelConceptMap* subsumedConMap = subsumedPMData->getPseudoModelConceptMap(false);
					CClassificationClassPseudoModelConceptMap* subsumerConMap = subsumerPMData->getPseudoModelConceptMap(false);
					if (subsumedConMap && subsumerConMap) {
						CClassificationClassPseudoModelConceptMap::const_iterator it1 = subsumerConMap->constBegin(), it1End = subsumerConMap->constEnd();
						CClassificationClassPseudoModelConceptMap::const_iterator it2 = subsumedConMap->constBegin(), it2End = subsumedConMap->constEnd();
						while (it1 != it1End && it2 != it2End) {
							CConcept* concept1 = it1.key().getConcept();
							CConcept* concept2 = it2.key().getConcept();
							cint64 con1Tag = concept1->getConceptTag();
							cint64 con2Tag = concept2->getConceptTag();
							if (con2Tag < con1Tag) {
								++it2;
							} else if (con2Tag == con1Tag) {
								++it1;
								++it2;
							} else if (con2Tag > con1Tag) {
								if (it1.value().isDeterministic()) {
									// in the possible subsumed model is missing a concept, subsumption is not possible
									return false;
								}
								++it1;
							}
						}
						while (it1 != it1End) {
							if (it1.value().isDeterministic()) {
								return false;
							}
							++it1;
						}
					}
				}
				if (subsumedPMData->hasValidRoleMap() && subsumerPMData->hasValidRoleMap()) {
					CClassificationClassPseudoModelRoleMap* subsumedRoleMap = subsumedPMData->getPseudoModelRoleMap(false);
					CClassificationClassPseudoModelRoleMap* subsumerRoleMap = subsumerPMData->getPseudoModelRoleMap(false);
					if (subsumedRoleMap && subsumerRoleMap) {
						CClassificationClassPseudoModelRoleMap::const_iterator it1 = subsumerRoleMap->constBegin(), it1End = subsumerRoleMap->constEnd();
						CClassificationClassPseudoModelRoleMap::const_iterator it2 = subsumedRoleMap->constBegin(), it2End = subsumedRoleMap->constEnd();
						while (it1 != it1End && it2 != it2End) {
							CRole* role1 = it1.key().getRole();
							CRole* role2 = it2.key().getRole();
							cint64 role1Tag = role1->getRoleTag();
							cint64 role2Tag = role2->getRoleTag();
							if (role2Tag < role1Tag) {
								++it2;
							} else if (role2Tag == role1Tag) {
								const CClassificationClassPseudoModelRoleData& role1Data = it1.value();
								const CClassificationClassPseudoModelRoleData& role2Data = it2.value();
								if (!role1Data.isPossibleSubsumerOf(role2Data)) {
									// some cardinalities are not satisfy the subsumption condition
									return false;
								}
								cint64 succ1ID = role1Data.getSuccessorModelID();
								cint64 succ2ID = role2Data.getSuccessorModelID();
								CClassificationClassPseudoModelData* succ1ModelData = subsumerPMHash->getPseudoModelData(succ1ID,false);
								CClassificationClassPseudoModelData* succ2ModelData = subsumedPMHash->getPseudoModelData(succ2ID,false);
								if (succ1ModelData && succ2ModelData) {
									if (!isPseudoModelSubsumerPossible(optSubClassItem,succ2ModelData,subsumedPMHash,succ1ModelData,subsumerPMHash)) {
										return false;
									}
								}
								++it1;
								++it2;
							} else if (role2Tag > role1Tag) {
								if (it1.value().isDeterministic()) {
									// in the possible subsumed model is missing a concept, subsumption is not possible
									return false;
								}
								++it1;
							}
						}
						while (it1 != it1End) {
							if (it1.value().isDeterministic()) {
								return false;
							}
							++it1;
						}

					}
				}
				return true;
			}


			CSubsumptionClassifierThread *COptimizedKPSetClassSubsumptionClassifierThread::processCalculationJob(CSatisfiableCalculationJob* job, COntologyClassificationItem *ontClassItem, CClassificationWorkItem* workItem) {
				CClassificationCalculationStatisticsCollection* statColl =  nullptr;
				if (ontClassItem->isCollectProcessStatisticsActivated()) {
					statColl = ontClassItem->getCalculationStatisticsCollection();
				}
				job->setCalclulationStatisticsCollector(statColl);
				CTestCalculatedCallbackEvent *testResultCallback = new CTestCalculatedCallbackEvent(this,job,workItem,statColl);
				job->setCalculationConfiguration(ontClassItem->getCalculationConfiguration());
				CSubsumptionClassifierThread::processCalculationJob(job,ontClassItem,testResultCallback,false);
				return this;
			}



			bool COptimizedKPSetClassSubsumptionClassifierThread::processToldClassificationMessage(COntologyClassificationItem *ontClassItem, CClassificationMessageData* messageDataLinker, CMemoryPool* memoryPools) {
				COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetClassOntologyClassificationItem *)ontClassItem;
				OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
				CClassificationMessageData* messageDataLinkerIt = messageDataLinker;
				while (messageDataLinkerIt) {
					CClassificationMessageData* messageData = messageDataLinkerIt;
					if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLCLASSSUBSUMPTION) {
						++mStatProcesedSubsumMessCount;
						CClassificationSubsumptionMessageData* subsumMessageData = (CClassificationSubsumptionMessageData*)messageData;
						CConcept* subsumedConcept = subsumMessageData->getSubsumedConcept();
						CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* subsumerList = subsumMessageData->getClassSubsumerList();

						COptimizedKPSetClassTestingItem* subsumedItem = nullptr;
						subsumedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumedConcept);


						//QString iriClassNameString = CIRIName::getRecentIRIName(subsumedItem->getTestingConcept()->getClassNameLinker());
						//if (iriClassNameString == "http://www.owllink.org/testsuite/galen#LowerLobeOfLeftLung") {
						//	bool bug = true;
						//}


						COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = subsumedItem->getPossibleSubsumptionMap(false);
						OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
						if (subsumerList) {
							for (CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>::const_iterator it = subsumerList->constBegin(), itEnd = subsumerList->constEnd(); it != itEnd; ++it) {
								CConcept* subsumerConcept(*it);
								COptimizedKPSetClassTestingItem* subsumerItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumerConcept);
								if (subsumedItem != subsumerItem) {
									subsumedItem->addSubsumingConceptItem(subsumerItem);
									if (possSubsumMap) {
										COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = possSubsumMap->value(subsumerConcept,nullptr);
										if (possSubsumData) {
											if (!possSubsumData->isSubsumptionConfirmed()) {
												possSubsumData->setSubsumptionConfirmed(true);

												if (possSubsumData->isUpdateRequired()) {
													prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possSubsumData);
												}
												OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
											}
										}
									}
									propagateDownSubsumption(optKPSetClassificationItem,subsumedItem,subsumerItem);
								}
							}
						}
						//if (!possSubsumMap || !possSubsumMap->hasRemainingPossibleSubsumptions()) {
						subsumedItem->setResultSatisfiableDerivated(true);
						//}

					} else if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLCLASSINITIALIZEPOSSIBLESUBSUM) {
						++mStatProcesedPossSubsumInitMessCount;
						CClassificationInitializePossibleSubsumptionMessageData* possSubsumMessageData = (CClassificationInitializePossibleSubsumptionMessageData*)messageData;
						CConcept* subsumedConcept = possSubsumMessageData->getSubsumedConcept();
						CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>* possSubsumerList = possSubsumMessageData->getClassPossibleSubsumerList();

#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGB
						QString iriClassNameString = CIRIName::getRecentIRIName(subsumedConcept->getClassNameLinker());
						possSubsumInitConList.append(iriClassNameString);
#endif


						COptimizedKPSetClassTestingItem* subsumedItem = nullptr;
						subsumedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumedConcept);

						QSet<CConcept*>* equivConNonCandidateSet = optKPSetClassificationItem->getEquivaltentConceptNonCandidateSet();
						COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = subsumedItem->getPossibleSubsumptionMap(false);
						if (equivConNonCandidateSet->isEmpty() && (!possSubsumerList || possSubsumerList->empty())) {
							if (possSubsumMap) {
								for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
									COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = it.value();
									if (possSubsumData->isSubsumptionUnknown()) {
										possSubsumData->setSubsumptionInvalid(true);
										if (possSubsumData->isUpdateRequired()) {
											prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possSubsumData);
										}
										OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
									}
								}
							}
						} else {
							if (!subsumedItem->isPossibleSubsumptionMapInitialized() || possSubsumMap) {
								possSubsumMap = subsumedItem->getPossibleSubsumptionMap(true);
								CBOXHASH<CConcept*,CConcept*>* equivConCandidateHash = optKPSetClassificationItem->getEquivalentConceptCandidateHash();
								if (possSubsumMap->isEmpty()) {
									// initialize the possible subsumption map
									if (possSubsumerList) {
										for (CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>::const_iterator it = possSubsumerList->constBegin(), itEnd = possSubsumerList->constEnd(); it != itEnd; ++it) {
											CClassificationInitializePossibleSubsumptionData* possSubsumDataIt = *it;
											if (possSubsumDataIt->isPossibleSubsumerValid()) {
												CConcept* possSubsumConcept = possSubsumDataIt->getPossibleSubsumerConcept();
												if (possSubsumConcept->getOperatorCode() != CCEQ || !equivConCandidateHash->contains(possSubsumConcept)) {
													COptimizedKPSetClassTestingItem* possSubsumItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(possSubsumConcept);
													if (!subsumedItem->hasSubsumerConceptItem(possSubsumItem) && subsumedItem != possSubsumItem) {
														COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = new COptimizedKPSetClassPossibleSubsumptionData(possSubsumItem);
														possSubsumMap->insert(CConceptTagComparer(possSubsumConcept),possSubsumData);
														incRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,possSubsumMap);
														OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
													}
													OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
												}
											}
										}
									}
									OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
									for (QSet<CConcept*>::const_iterator it = equivConNonCandidateSet->constBegin(), itEnd = equivConNonCandidateSet->constEnd(); it != itEnd; ++it) {
										CConcept* eqConcept = *it;
										COptimizedKPSetClassTestingItem* possEqSubsumItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(eqConcept);
										if (!subsumedItem->hasSubsumerConceptItem(possEqSubsumItem) && subsumedItem != possEqSubsumItem) {
											if (!possSubsumMap->contains(CConceptTagComparer(eqConcept))) {
												COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = new COptimizedKPSetClassPossibleSubsumptionData(possEqSubsumItem);
												possSubsumMap->insert(CConceptTagComparer(eqConcept),possSubsumData);
												incRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,possSubsumMap);
											}
										}
									}
									OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));

									// prune ancestor items
									QSet<COptimizedKPSetClassTestingItem*>* upPropSet = subsumedItem->getUpPropagationItemSet();
									for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = upPropSet->constBegin(), itEnd = upPropSet->constEnd(); it != itEnd; ++it) {
										COptimizedKPSetClassTestingItem* upPropItem = *it;
										COptimizedKPSetClassPossibleSubsumptionMap* upPropItemPossSubsumMap = upPropItem->getPossibleSubsumptionMap(false);
										if (upPropItemPossSubsumMap) {
											COptimizedKPSetClassPossibleSubsumptionMap::const_iterator itPoss = possSubsumMap->constBegin(), itPossEnd = possSubsumMap->constEnd();
											COptimizedKPSetClassPossibleSubsumptionMap::const_iterator itUpPoss = upPropItemPossSubsumMap->constBegin(), itUpPossEnd = upPropItemPossSubsumMap->constEnd();
											while (itPoss != itPossEnd && itUpPoss != itUpPossEnd) {
												CConcept* possCon = itPoss.key().getConcept();
												CConcept* possUpCon = itUpPoss.key().getConcept();
												if (possCon->getConceptTag() == possUpCon->getConceptTag()) {
													++itPoss;
													++itUpPoss;
												} else if (possCon->getConceptTag() < possUpCon->getConceptTag()) {
													++itPoss;
												} else if (possCon->getConceptTag() > possUpCon->getConceptTag()) {
													COptimizedKPSetClassPossibleSubsumptionData* upPossData = itUpPoss.value();
													if (!subsumedItem->hasSubsumerConceptItem(upPossData->getClassItem()) && subsumedItem != upPossData->getClassItem()) {
														if (!upPossData->isSubsumptionInvalided()) {
															upPossData->setSubsumptionInvalid(true);
															if (upPossData->isUpdateRequired()) {
																prunePossibleSubsumptions(optKPSetClassificationItem,upPropItem,upPossData);
															}
															OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
														}
													}
													++itUpPoss;
												}
											}
											while (itUpPoss != itUpPossEnd) {
												COptimizedKPSetClassPossibleSubsumptionData* upPossData = itUpPoss.value();
												if (!subsumedItem->hasSubsumerConceptItem(upPossData->getClassItem()) && subsumedItem != upPossData->getClassItem()) {
													if (!upPossData->isSubsumptionInvalided()) {
														upPossData->setSubsumptionInvalid(true);
														if (upPossData->isUpdateRequired()) {
															prunePossibleSubsumptions(optKPSetClassificationItem,upPropItem,upPossData);
														}
														OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
													}
												}
												++itUpPoss;
											}
										}
									}

									// prune descendant items
									QSet<COptimizedKPSetClassTestingItem*>* downPropSet = subsumedItem->getDownPropagationItemSet();
									for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = downPropSet->constBegin(), itEnd = downPropSet->constEnd(); it != itEnd; ++it) {
										COptimizedKPSetClassTestingItem* downPropItem = *it;
										COptimizedKPSetClassPossibleSubsumptionMap* downPropItemPossSubsumMap = downPropItem->getPossibleSubsumptionMap(false);
										if (downPropItemPossSubsumMap) {
											COptimizedKPSetClassPossibleSubsumptionMap::const_iterator itPoss = possSubsumMap->constBegin(), itPossEnd = possSubsumMap->constEnd();
											COptimizedKPSetClassPossibleSubsumptionMap::const_iterator itDownPoss = downPropItemPossSubsumMap->constBegin(), itDownPossEnd = downPropItemPossSubsumMap->constEnd();
											while (itPoss != itPossEnd && itDownPoss != itDownPossEnd) {
												CConcept* possCon = itPoss.key().getConcept();
												CConcept* possDownCon = itDownPoss.key().getConcept();
												if (possCon->getConceptTag() == possDownCon->getConceptTag()) {
													++itPoss;
													++itDownPoss;
												} else if (possDownCon->getConceptTag() < possCon->getConceptTag()) {
													++itDownPoss;
												} else if (possDownCon->getConceptTag() > possCon->getConceptTag()) {
													COptimizedKPSetClassPossibleSubsumptionData* possData = itPoss.value();
													if (!downPropItem->hasSubsumerConceptItem(possData->getClassItem()) && downPropItem != possData->getClassItem()) {
														if (!possData->isSubsumptionInvalided()) {
															possData->setSubsumptionInvalid(true);
															if (possData->isUpdateRequired()) {
																prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
															}
															OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
														}
													}
													++itPoss;
												}
											}
											while (itPoss != itPossEnd) {
												COptimizedKPSetClassPossibleSubsumptionData* possData = itDownPoss.value();
												if (!downPropItem->hasSubsumerConceptItem(possData->getClassItem()) && downPropItem != possData->getClassItem()) {
													if (!possData->isSubsumptionInvalided()) {
														possData->setSubsumptionInvalid(true);
														if (possData->isUpdateRequired()) {
															prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
														}
														OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
													}
												}
												++itPoss;
											}
										} else if (downPropItem->isPossibleSubsumptionMapInitialized()) {
											for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator itPoss = possSubsumMap->constBegin(), itPossEnd = possSubsumMap->constEnd(); itPoss != itPossEnd; ++itPoss) {
												COptimizedKPSetClassPossibleSubsumptionData* possData = itPoss.value();
												if (!downPropItem->hasSubsumerConceptItem(possData->getClassItem()) && downPropItem != possData->getClassItem()) {
													if (!possData->isSubsumptionInvalided()) {
														possData->setSubsumptionInvalid(true);
														if (possData->isUpdateRequired()) {
															prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
														}
													}
												}
											}
										}
									}

								} else {
									// prune the possible subsumption map
									COptimizedKPSetClassPossibleSubsumptionMap::const_iterator itPoss = possSubsumMap->constBegin(), itPossEnd = possSubsumMap->constEnd();
									if (possSubsumerList) {
										CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>::const_iterator itNew = possSubsumerList->constBegin(), itNewEnd = possSubsumerList->constEnd();
										while (itPoss != itPossEnd && itNew != itNewEnd) {
											COptimizedKPSetClassPossibleSubsumptionData* possData = itPoss.value();
											CConcept* possCon = itPoss.key().getConcept();
											CClassificationInitializePossibleSubsumptionData* possNewSubsumDataIt = *itNew;
											if (possNewSubsumDataIt->isPossibleSubsumerValid()) {
												CConcept* newCon = possNewSubsumDataIt->getPossibleSubsumerConcept();
												if (possCon->getConceptTag() == newCon->getConceptTag()) {
													++itPoss;
													++itNew;
												} else if (possCon->getConceptTag() < newCon->getConceptTag()) {
													if (possCon->getOperatorCode() != CCEQ) {
														if (!possData->isSubsumptionInvalided()) {
															possData->setSubsumptionInvalid(true);
															if (possData->isUpdateRequired()) {
																prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
															}
															OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
														}
													}
													++itPoss;
												} else if (possCon->getConceptTag() > newCon->getConceptTag()) {
													++itNew;
												}
											} else {
												++itNew;
											}
										}
									}
									while (itPoss != itPossEnd) {
										COptimizedKPSetClassPossibleSubsumptionData* possData = itPoss.value();
										CConcept* possCon = itPoss.key().getConcept();
										if (possCon->getOperatorCode() != CCEQ) {
											if (!possData->isSubsumptionInvalided()) {
												possData->setSubsumptionInvalid(true);
												if (possData->isUpdateRequired()) {
													prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possData);
												}
												OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
											}
										}
										++itPoss;
									}
								}
							}
						}
						subsumedItem->setPossibleSubsumptionMapInitialized(true);


					} else if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLCLASSUPDATEPOSSIBLESUBSUM) {
						++mStatProcesedPossSubsumUpdateMessCount;
						CClassificationUpdatePossibleSubsumptionMessageData* possSubsumMessageData = (CClassificationUpdatePossibleSubsumptionMessageData*)messageData;
						CConcept* subsumedConcept = possSubsumMessageData->getSubsumedConcept();

						COptimizedKPSetClassTestingItem* subsumedItem = nullptr;
						subsumedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumedConcept);

						COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = subsumedItem->getPossibleSubsumptionMap(false);
						if (possSubsumMap && !possSubsumMap->empty()) {
							possSubsumMap = subsumedItem->getPossibleSubsumptionMap(true);
							for (COptimizedKPSetClassPossibleSubsumptionMap::const_iterator it = possSubsumMap->constBegin(), itEnd = possSubsumMap->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = it.value();
								if (possSubsumData->isUpdateRequired()) {
									prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possSubsumData);
									OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
								}
							}
						}
					} else if (messageData->getClassificationMessageDataType() == CClassificationMessageData::TELLCLASSPSEUDOMODELIDENTIFIERS) {
						++mStatProcesedPseudoModelMessCount;
						CClassificationPseudoModelIdentifierMessageData* pmMessageData = (CClassificationPseudoModelIdentifierMessageData*)messageData;
						CConcept* pmConcept = pmMessageData->getPseudoModelConcept();
						CMemoryPool* pmMemoryPools = pmMessageData->getPseudoModelMemoryPools();
						CClassificationClassPseudoModelHash* pmHash = pmMessageData->getPseudoModelHash();

						COptimizedKPSetClassTestingItem* pmItem = nullptr;
						pmItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(pmConcept);
						pmItem->getClassPseudoModel()->setPseudoModelHash(pmHash);
						pmItem->setClassPseudoModelInitalized(true);

						optKPSetClassificationItem->addMemoryPools(pmMemoryPools);

					}
					messageDataLinkerIt = messageDataLinkerIt->getNext();
				}
				OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
				return true;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::interpreteToldSubsumptionResult(COntologyClassificationItem *ontClassItem, const QList<QPair<CConcept *,CConcept *> > &subSumRelList, bool isSubSum) {
				CConcept *lastConcept = nullptr;
				COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetClassOntologyClassificationItem *)ontClassItem;
				COptimizedKPSetClassTestingItem* subsumerItem = nullptr;

				for (QList<QPair<CConcept *,CConcept *> >::const_iterator it = subSumRelList.constBegin(), itEnd = subSumRelList.constEnd(); it != itEnd; ++it) {
					CConcept *subsumerConcept = it->first;
					if (lastConcept != subsumerConcept) {
						lastConcept = subsumerConcept;
						subsumerItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumerConcept);
					}
					CConcept *subsumedConcept = it->second;
					if (isSubSum) {
						COptimizedKPSetClassTestingItem* subsumedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumedConcept);
						subsumerItem->addSubsumingConceptItem(subsumedItem);
					}
				}
				return true;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::interpreteSubsumptionResult(COntologyClassificationItem *ontClassItem, CConcept *subsumedConcept, CConcept *subsumerConcept, bool isSubsumption) {
				COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetClassOntologyClassificationItem *)ontClassItem;

				++mInterpretedSubsumptionCalculationCount;
				CConcept* candidateConcept = subsumerConcept;

				if (subsumerConcept->getOperatorCode() == CCEQ) {
					candidateConcept = optKPSetClassificationItem->getEquivalentConceptCandidateHash()->value(subsumerConcept);
					if (!candidateConcept) {
						candidateConcept = subsumerConcept;
					}
				}

				optKPSetClassificationItem->decRunningPossibleSubsumptionTestsCount();

				COptimizedKPSetClassTestingItem* subsumedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumedConcept);
				COptimizedKPSetClassTestingItem* subsumerItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(subsumerConcept);



				//QString iriClassNameString1 = CIRIName::getRecentIRIName(subsumedConcept->getClassNameLinker());
				//QString iriClassNameString2 = CIRIName::getRecentIRIName(subsumerConcept->getClassNameLinker());
				//if (iriClassNameString1 == "http://www.bootstrep.eu/ontology/GRO#BindingOfProteinToProteinBindingSiteOfDNA" && iriClassNameString2 == "http://www.bootstrep.eu/ontology/GRO#DNAProteinInteraction" && !isSubsumption) {
				//	bool bug = true;
				//	calculateSubsumption(optKPSetClassificationItem,subsumedItem,subsumerItem,nullptr);
				//}

				//if (iriClassNameString1 == "http://www.bootstrep.eu/ontology/GRO#BindingOfProteinToProteinBindingSiteOfDNA" && iriClassNameString2 == "http://www.bootstrep.eu/ontology/GRO#DNAProteinInteraction") {
				//	bool isSubsumptionFlag = false;
				//	if (fastPseudoModelSubsumptionClassPrecheckTest(optKPSetClassificationItem,subsumedItem,subsumerItem,&isSubsumptionFlag)) {
				//		if (isSubsumptionFlag != isSubsumption) {
				//			while (true) {
				//				fastPseudoModelSubsumptionClassPrecheckTest(optKPSetClassificationItem,subsumedItem,subsumerItem,&isSubsumptionFlag);
				//			}				
				//		}
				//	}
				//}


				COptimizedKPSetClassTestingItem* topItem = optKPSetClassificationItem->getTopConceptSatisfiableTestItem();

				COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = subsumedItem->getPossibleSubsumptionMap(false);
				COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = nullptr;
				if (possSubsumMap) {
					possSubsumData = possSubsumMap->value(candidateConcept);
				}

				if (isSubsumption) {
					optKPSetClassificationItem->incCalculatedTruePossibleSubsumerCount();
					if (possSubsumData) {
						possSubsumData->setSubsumptionConfirmed(true);
					}
					subsumedItem->addSubsumingConceptItem(subsumerItem);
					subsumedItem->addUpPropagationItem(subsumerItem);
					subsumerItem->addDownPropagationItem(subsumedItem);
					propagateDownSubsumption(optKPSetClassificationItem,subsumedItem,subsumerItem);
				} else {

					//if (CIRIName::getRecentIRIName(possSubsumData->getClassItem()->getTestingConcept()->getClassNameLinker()) == "http://www.loria.fr/~coulet/ontology/snpontology/version1.6/snpontology_full.owl#haplotype") {
					//	LOG(ERROR,getLogDomain(),logTr("Calcualted non subsumption with haplotype for item %1.").arg(CIRIName::getRecentIRIName(subsumedItem->getTestingConcept()->getClassNameLinker())),getLogObject());
					//	bool bug = true;
					//}


					optKPSetClassificationItem->incCalculatedFalsePossibleSubsumerCount();
					if (possSubsumData) {
						possSubsumData->setSubsumptionInvalid(true);
					}
				}

				if (possSubsumData && possSubsumData->isUpdateRequired()) {
					prunePossibleSubsumptions(optKPSetClassificationItem,subsumedItem,possSubsumData);
					OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
				}


				QList<COptimizedKPSetClassTestingItem*>* nextItemList = optKPSetClassificationItem->getNextPossibleSubsumptionTestingItemList();
				QSet<COptimizedKPSetClassTestingItem*>* currentItemSet = optKPSetClassificationItem->getCurrentPossibleSubsumptionTestingItemSet();
				if (mConfPossSubsumCalcOrderTopDown) {
					if (currentItemSet->contains(subsumedItem)) {
						currentItemSet->remove(subsumedItem);
						if (possSubsumMap) {
							if (possSubsumMap->hasRemainingPossibleSubsumptions()) {
								nextItemList->prepend(subsumedItem);
							} else {
								QSet<COptimizedKPSetClassTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionClassTestingSet();
								remTestingSet->remove(subsumedItem);
							}
						}
					}
				} 
				if (mConfPossSubsumCalcOrderBottomUp) {
					if (currentItemSet->contains(subsumerItem)) {
						currentItemSet->remove(subsumerItem);
						if (subsumerItem->hasRemainingPossibleSubsumedItems()) {
							QSet<COptimizedKPSetClassTestingItem*>* remPossSubsumedItemSet = subsumerItem->getPossibleSubsumedSet(false);
							QList<COptimizedKPSetClassTestingItem*>* remPossSubsumedItemList = subsumerItem->getPossibleSubsumedList();
							nextItemList->prepend(subsumerItem);

							QSet<COptimizedKPSetClassTestingItem*>* upPropSet = subsumedItem->getUpPropagationItemSet();
							for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator itUp = upPropSet->constBegin(), itUpEnd = upPropSet->constEnd(); itUp != itUpEnd; ++itUp) {
								COptimizedKPSetClassTestingItem* upItem = *itUp;
								if (remPossSubsumedItemSet->contains(upItem)) {
									remPossSubsumedItemList->prepend(upItem);
								}
							}
							OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectReferenced(optKPSetClassificationItem));

						} else {
							QSet<COptimizedKPSetClassTestingItem*>* remTestingSet = optKPSetClassificationItem->getRemainingPossibleSubsumptionClassTestingSet();
							remTestingSet->remove(subsumerItem);
							OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectReferenced(optKPSetClassificationItem));
						}
					}
				}
				return true;
			}



			bool COptimizedKPSetClassSubsumptionClassifierThread::prunePossibleSubsumptions(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* item, COptimizedKPSetClassPossibleSubsumptionData* possSubsumData) {

				//if (possSubsumData->isSubsumptionInvalided()) {
				//	//if (CIRIName::getRecentIRIName(item->getTestingConcept()->getClassNameLinker()) == "http://www.w3.org/2002/07/owl#Thing") {
				//		if (CIRIName::getRecentIRIName(possSubsumData->getClassItem()->getTestingConcept()->getClassNameLinker()) == "http://www.loria.fr/~coulet/ontology/snpontology/version1.6/snpontology_full.owl#haplotype") {
				//			LOG(ERROR,getLogDomain(),logTr("Pruned haplotype for item %1.").arg(CIRIName::getRecentIRIName(item->getTestingConcept()->getClassNameLinker())),getLogObject());
				//			bool bug = true;
				//		}
				//	//}
				//}

				if (possSubsumData->isUpdateRequired()) {
					COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
					possSubsumData->setSubsumptionUpdated(true);
					decRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,possSubsumMap,true);
					if (possSubsumData->isSubsumptionConfirmed()) {
						// is subsuming
						COptimizedKPSetClassTestingItem* subsumerItem = possSubsumData->getClassItem();
						CConcept* subsumerConcept = subsumerItem->getTestingConcept();
						// establish the subsumption for all successor nodes
						QSet<COptimizedKPSetClassTestingItem*>* downPropSet = item->getDownPropagationItemSet();
						for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = downPropSet->constBegin(), itEnd = downPropSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetClassTestingItem* downPropItem = *it;
							pruneDownSubsumption(optKPSetClassificationItem,downPropItem,subsumerConcept);
						}
						if (subsumerConcept->getOperatorCode() == CCEQ) {
							CConcept* candidateConcept = optKPSetClassificationItem->getEquivalentConceptCandidateHash()->value(subsumerConcept);
							if (candidateConcept) {
								for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = downPropSet->constBegin(), itEnd = downPropSet->constEnd(); it != itEnd; ++it) {
									COptimizedKPSetClassTestingItem* downPropItem = *it;
									pruneDownSubsumption(optKPSetClassificationItem,downPropItem,candidateConcept);
								}
							}
						}
						OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
						return true;
					} else {
						// is not subsuming
						COptimizedKPSetClassTestingItem* notSubsumerItem = possSubsumData->getClassItem();
						CConcept* notSubsumerConcept = notSubsumerItem->getTestingConcept();
						// establish the subsumption for all successor nodes
						QSet<COptimizedKPSetClassTestingItem*>* upPropSet = item->getUpPropagationItemSet();
						for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = upPropSet->constBegin(), itEnd = upPropSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetClassTestingItem* upPropItem = *it;
							pruneUpNotSubsumption(optKPSetClassificationItem,upPropItem,notSubsumerConcept);
						}
						if (notSubsumerConcept->getOperatorCode() == CCEQ) {
							CConcept* candidateConcept = optKPSetClassificationItem->getEquivalentConceptCandidateHash()->value(notSubsumerConcept);
							if (candidateConcept) {
								for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = upPropSet->constBegin(), itEnd = upPropSet->constEnd(); it != itEnd; ++it) {
									COptimizedKPSetClassTestingItem* upPropItem = *it;
									pruneUpNotSubsumption(optKPSetClassificationItem,upPropItem,candidateConcept);
								}
							}
						}
						OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(testDebugPossibleSubsumerCorrectCounted(optKPSetClassificationItem));
						return true;

					}
				}
				return false;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::propagateDownSubsumption(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* item, COptimizedKPSetClassTestingItem* subsumerItem) {
				bool propagated = false;
				QSet<COptimizedKPSetClassTestingItem*>* downPropSet = item->getDownPropagationItemSet();
				for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = downPropSet->constBegin(), itEnd = downPropSet->constEnd(); it != itEnd; ++it) {
					COptimizedKPSetClassTestingItem* downPropItem = *it;
					if (!downPropItem->hasSubsumerConceptItem(subsumerItem) && downPropItem != subsumerItem) {
						downPropItem->addSubsumingConceptItem(subsumerItem);
						propagateDownSubsumption(optKPSetClassificationItem,downPropItem,subsumerItem);
						propagated = true;
					}
				}
				return propagated;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::pruneDownSubsumption(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* item, CConcept* subsumerConcept) {		
				COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
				if (possSubsumMap) {
					COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = possSubsumMap->value(subsumerConcept);
					if (possSubsumData && !possSubsumData->isSubsumptionConfirmed()) {
						possSubsumData->setSubsumptionConfirmed(true);
						possSubsumData->setSubsumptionUpdated(true);
						decRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,possSubsumMap,true);

						CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
						if (ontClassStat) {
							ontClassStat->incDerivatedTestedSubsumptionCount(1);
						}

						QSet<COptimizedKPSetClassTestingItem*>* downPropSet = item->getDownPropagationItemSet();
						for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = downPropSet->constBegin(), itEnd = downPropSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetClassTestingItem* downPropItem = *it;
							pruneDownSubsumption(optKPSetClassificationItem,downPropItem,subsumerConcept);
						}
						return true;
					}
				}
				return false;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::pruneUpNotSubsumption(COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem, COptimizedKPSetClassTestingItem* item, CConcept* notSubsumerConcept) {		
				COptimizedKPSetClassPossibleSubsumptionMap* possSubsumMap = item->getPossibleSubsumptionMap(false);
				if (possSubsumMap) {
					COptimizedKPSetClassPossibleSubsumptionData* possSubsumData = possSubsumMap->value(notSubsumerConcept);
					if (possSubsumData && !possSubsumData->isSubsumptionInvalided()) {
						possSubsumData->setSubsumptionInvalid(true);
						possSubsumData->setSubsumptionUpdated(true);
						decRemainingPossibleSubsumptionTestingCount(optKPSetClassificationItem,possSubsumMap,false);

						CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
						if (ontClassStat) {
							ontClassStat->incPrunedTestedSubsumptionCount(1);
						}

						QSet<COptimizedKPSetClassTestingItem*>* upPropSet = item->getUpPropagationItemSet();
						for (QSet<COptimizedKPSetClassTestingItem*>::const_iterator it = upPropSet->constBegin(), itEnd = upPropSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetClassTestingItem* upPropItem = *it;
							pruneUpNotSubsumption(optKPSetClassificationItem,upPropItem,notSubsumerConcept);
						}
						return true;
					}
				}
				return false;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::interpreteSatisfiableResult(COntologyClassificationItem *ontClassItem, CConcept *satisfiableConcept, bool isSatis) {
				COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetClassOntologyClassificationItem *)ontClassItem;

				CClassifierStatistics *ontClassStat = optKPSetClassificationItem->getClassifierStatistics();
				if (ontClassStat) {
					ontClassStat->incToldSatisfiableCount(1);
				}

				++satTestedCount;

				optKPSetClassificationItem->decRunningSatisfiableTestsCount();

				COptimizedKPSetClassTestingItem* satTestedItem = optKPSetClassificationItem->getConceptSatisfiableTestItem(satisfiableConcept);
				//if (CIRIName::getRecentIRIName(satisfiableConcept->getClassNameLinker()) == "http://www.gdst.uqam.ca/Documents/Ontologies/HIT/Equipment_SH_Ontology.owl#Folding_chair" && !isSatis) {
				//	bool bug = true;
				//	calculateSatisfiable(optKPSetClassificationItem,satTestedItem);
				//	return false;
				//}


				satTestedItem->setSatisfiableTested(true);
				satTestedItem->setSatisfiableTestedResult(isSatis);

				COptimizedKPSetClassTestingItem* topItem = optKPSetClassificationItem->getTopConceptSatisfiableTestItem();

				if (isSatis) {
					// update processing lists
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGB
					QString iriClassNameString = CIRIName::getRecentIRIName(satisfiableConcept->getClassNameLinker());
					satTestedConList.append(iriClassNameString);
#endif
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGLOGOUTPUT
					LOG(INFO,getLogDomain(),QString("[%1 / %2] Calculated: '%3' is satisfiable").arg(satTestedCount).arg(totalToSatTestClassCount).arg(iriClassNameString),this);
#endif
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
					statUpdateMutex.lock();
					statusString = QString("Interpreting, '%1' is satisfiable").arg(iriClassNameString);
					statUpdateMutex.unlock();
#endif
#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCOUTOUTPUT
					cout<<QString("Calculated: '%1' is satisfiable").arg(iriClassNameString).toLocal8Bit().data()<<endl<<endl;
#endif

					optKPSetClassificationItem->addSatisfiableConceptItem(satTestedItem);

				} else {
					CTaxonomy *tax = optKPSetClassificationItem->getTaxonomy();
					if (topItem) {
						tax->setConceptSatisfiable(satisfiableConcept,false);
					} else {
						tax->getBottomHierarchyNode()->addEquivalentConcept(satisfiableConcept);
						tax->updateNodeEquivalences(tax->getBottomHierarchyNode());
					}
				}

				QList<COptimizedKPSetClassTestingItem*>* nextItemList = optKPSetClassificationItem->getNextSatisfiableTestingItemList();
				QSet<COptimizedKPSetClassTestingItem*>* nextCandItemSet = optKPSetClassificationItem->getNextCandidateSatisfiableTestingItemSet();
				QSet<COptimizedKPSetClassTestingItem*>* remainingCandItemSet = optKPSetClassificationItem->getRemainingCandidateSatisfiableTestingItemSet();


				QList<COptimizedKPSetClassTestingItem*>* subSumItemList = satTestedItem->getSubsumingConceptItemList();
				QList<COptimizedKPSetClassTestingItem*>* succItemList = satTestedItem->getSuccessorItemList();
				for (QList<COptimizedKPSetClassTestingItem*>::const_iterator it = succItemList->constBegin(), itEnd = succItemList->constEnd(); it != itEnd; ++it) {
					COptimizedKPSetClassTestingItem* succItem = *it;
					if (!isSatis) {
						succItem->setResultUnsatisfiableDerivated(true);
					} else {
						for (QList<COptimizedKPSetClassTestingItem*>::const_iterator itSubsum = subSumItemList->constBegin(), itSubsumEnd = subSumItemList->constEnd(); itSubsum != itSubsumEnd; ++itSubsum) {
							COptimizedKPSetClassTestingItem* subusmerItem = *itSubsum;
							if (succItem != subusmerItem) {
								succItem->addSubsumingConceptItem(subusmerItem);
							}
						}
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


			bool COptimizedKPSetClassSubsumptionClassifierThread::interpreteTestResults(CTestCalculatedCallbackEvent *testResult) {
				CSatisfiableCalculationJob *satCalcJob = testResult->getSatisfiableCalculationJob();
				bool testSat = testResult->getTestResultSatisfiable();
				CClassificationWorkItem *workItem = testResult->getClassificationWorkItem();
				++mRecievedCallbackCount;


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
								} else if (workItem->isConceptSubsumptionTest()) {
									interpreteSubsumptionResult(ontClassItem,workItem->getSubsumedTestedConcept(),workItem->getSubsumerTestedConcept(),!testSat);
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
						delete satCalcJob;
					}
					workOntItemHash.remove(workItem);


					CClassificationCalculationStatisticsCollection* statisticCollection = testResult->getUsedStatisticsCollection();
					if (statisticCollection) {
						ontClassItem->reuseCalculationStatisticsCollection(statisticCollection);
					}

#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGTAXONOMYCOUTOUTPUT
					QString taxString = taxonomy->getConceptHierarchyString();
					cout<<taxString.toLocal8Bit().data()<<endl<<endl;
#endif
				} 


				return true;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::finishOntologyClassification(COntologyClassificationItem *ontClassItem) {

				if (!ontClassItem->isTaxonomyConstructed() && !ontClassItem->isTaxonomyConstructionFailed()) {

#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGLOGOUTPUT
					LOG(INFO,getLogDomain(),QString("Building Class Taxonomy"),this);
#endif
					CTaxonomy *taxonomy = ontClassItem->getTaxonomy();
					CConcreteOntology *ontology = ontClassItem->getOntology();
					LOG(INFO,getLogDomain(),logTr("Starting Transitive Reduction for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

					// build taxonomy

					COptimizedKPSetClassOntologyClassificationItem *optKPSetClassificationItem = (COptimizedKPSetClassOntologyClassificationItem *)ontClassItem;
					QList<COptimizedKPSetClassTestingItem*>* satItemList = optKPSetClassificationItem->getSatisfiableConceptItemList();
					OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGCONSISTENCYTEST(createDebugKPSetString(optKPSetClassificationItem,"classkpsets-final.txt"));

					CTaxonomy *tax = optKPSetClassificationItem->getTaxonomy();

					QList<COptimizedKPSetClassTestingItem*> itemList;

					COptimizedKPSetClassTestingItem* topItem = optKPSetClassificationItem->getTopConceptSatisfiableTestItem();
					CHierarchyNode* topHierNode = tax->getTopHierarchyNode();
					CHierarchyNode* bottomHierNode = tax->getBottomHierarchyNode();



					cint64 topSubsumingItemCount = topItem->getSubsumingConceptItemCount();
					topItem->setSatisfiableConceptHierarchyNode(topHierNode);
					if (topSubsumingItemCount) {
						QList<COptimizedKPSetClassTestingItem*>* subsumingConceptItemList = topItem->sortSubsumingConceptItemList();
						for (QList<COptimizedKPSetClassTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
							COptimizedKPSetClassTestingItem* subsumedConceptItem = *subsumedIt;
							// mark as equivalent
							subsumedConceptItem->setEquivalentItem(true);
							topHierNode->addEquivalentConcept(subsumedConceptItem->getTestingConcept());
							tax->updateNodeEquivalences(topHierNode);
						}
					}

					LOG(INFO,getLogDomain(),logTr("Sorting classes for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

					for (QList<COptimizedKPSetClassTestingItem*>::const_iterator it = satItemList->constBegin(), itEnd = satItemList->constEnd(); it != itEnd; ++it) {
						COptimizedKPSetClassTestingItem* item = *it;

						//QString iriClassNameString = CIRIName::getRecentIRIName(item->getTestingConcept()->getClassNameLinker());
						//if (iriClassNameString == "http://ontology.dumontierlab.com/UnitRaisedToPower") {
						//	bool bug = true;
						//}

						if (!item->isEquivalentItem() && item != topItem) {
							CHierarchyNode* itemNode = tax->getHierarchyNode(item->getTestingConcept(),true);
							item->setSatisfiableConceptHierarchyNode(itemNode);
							cint64 itemSubsumingCount = item->getSubsumingConceptItemCount();
							itemList.append(item);
							if (itemSubsumingCount > topSubsumingItemCount) {
								QList<COptimizedKPSetClassTestingItem*>* subsumingConceptItemList = item->sortSubsumingConceptItemList();
								for (QList<COptimizedKPSetClassTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
									COptimizedKPSetClassTestingItem* subsumedConceptItem = *subsumedIt;
									cint64 subsumedItemSubsumingCount = subsumedConceptItem->getSubsumingConceptItemCount();
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

					LOG(INFO,getLogDomain(),logTr("Transitive Reduction for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

					for (QList<COptimizedKPSetClassTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedKPSetClassTestingItem* item = *it;

						//QString iriClassNameString = CIRIName::getRecentIRIName(item->getTestingConcept()->getClassNameLinker());
						//if (iriClassNameString == "http://usefulinc.com/ns/doap#Project") {
						//	bool bug = true;
						//}

						if (!item->isEquivalentItem() && item != topItem) {
							cint64 itemSubsumingCount = item->getSubsumingConceptItemCount();
							CHierarchyNode* itemHierNode = item->getSatisfiableConceptHierarchyNode();

							QList<COptimizedKPSetClassTestingItem*>* subsumingConceptItemList = item->getSubsumingConceptItemList();
							QList<COptimizedKPSetClassTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd();

							cint64 eqCount = 0;
							cint64 remainingSubsumingCount = itemSubsumingCount;
							// remove direct equivalences
							while (subsumedIt != subsumedItEnd) {
								COptimizedKPSetClassTestingItem* subsumedConceptItem = *subsumedIt;
								cint64 subsumedItemSubsumingCount = subsumedConceptItem->getSubsumingConceptItemCount();
								if (itemSubsumingCount != subsumedItemSubsumingCount) {
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
								QList<COptimizedKPSetClassTestingItem*> parentItemList;
								while (subsumedIt != subsumedItEnd) {
									COptimizedKPSetClassTestingItem* subsumedConceptItem = *subsumedIt;
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
									COptimizedKPSetClassTestingItem* subsumedConceptItem = *subsumedIt;
									if (!subsumedConceptItem->isEquivalentItem()) {
										bool predOfOnePrevParent = false;
										if (searchForMoreParents) {
											for (QList<COptimizedKPSetClassTestingItem*>::const_iterator parentIt = parentItemList.constBegin(), parentItEnd = parentItemList.constEnd(); parentIt != parentItEnd && !predOfOnePrevParent; ++parentIt) {
												COptimizedKPSetClassTestingItem* parentItem = *parentIt;
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

					for (QList<COptimizedKPSetClassTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedKPSetClassTestingItem* item = *it;
						if (!item->isPredecessorItem() && !item->isEquivalentItem()) {
							CHierarchyNode* itemHierNode = item->getSatisfiableConceptHierarchyNode();
							bottomHierNode->makeParent(itemHierNode);
						}
					}

					LOG(INFO,getLogDomain(),logTr("Collect statistics for Ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());


#ifdef OPTIMIZEDKPSETCLASSCLASSIFIERDEBUGSTATUSDESCRIPTION
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
					LOG(INFO,getLogDomain(),logTr("Ontology '%1' classified.").arg(ontology->getTerminologyName()),getLogObject());
					LOG(INFO,getLogDomain(),logTr("Used %1 satisfiable tests, %2 pseudo model merged subsumption tests and %3 calculated subsumption tests.").arg(ontClassItem->getCalcedSatisfiableCount()).arg(ontClassItem->getClassifierStatistics()->getPseudoModelMergedTestedSubsumptionCount()).arg(ontClassItem->getClassifierStatistics()->getCalculatedTestedSubsumptionCount()),getLogObject());
					LOG(INFO,getLogDomain(),logTr("Confirmed %1 of %2 possible subsumer concepts.").arg(optKPSetClassificationItem->getTruePossibleSubsumerCount()).arg(optKPSetClassificationItem->getPossibleSubsumerCount()),getLogObject());
					LOG(INFO,getLogDomain(),logTr("Confirmed %1 and invalidated %2 possible subsumer concepts with calculation.").arg(optKPSetClassificationItem->getCalculatedTruePossibleSubsumerCount()).arg(optKPSetClassificationItem->getFalsePossibleSubsumerCount()),getLogObject());
				}
				ontClassItem->submitTaxonomyConstructed();
				ontClassItem->doClassifiedCallback();
				return true;
			}


			bool COptimizedKPSetClassSubsumptionClassifierThread::interceptTestResults(CInterceptOntologyTestResultEvent *interceptResult) {
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
