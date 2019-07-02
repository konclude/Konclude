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

#include "COptimizedClassSaturationSubsumptionClassifierThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


#ifdef OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGCOUTOUTPUT
	#define OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGB
#endif 
#ifdef OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGCOUTOUTPUT
	#define OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGB
#endif 
#ifdef OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGBOXDESCRIPTION
	#define OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGB
#endif 
#ifdef OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGSTATUSDESCRIPTION
	#define OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGB
#endif 

			COptimizedClassSaturationSubsumptionClassifierThread::COptimizedClassSaturationSubsumptionClassifierThread(CReasonerManager *reasonerManager) 
					: CSubsumptionClassifierThread(reasonerManager), CLogIdentifier("::Konclude::Reasoner::Kernel::Classifier::OptimizedClassSaturationSubsumptionClassifierThread",this) {
				satTestedCount = 0;
				totalToTestCount = 0;
				mClassificationCount = 0;
			}


			COptimizedClassSaturationSubsumptionClassifierThread::~COptimizedClassSaturationSubsumptionClassifierThread() {
			}


			CTaxonomy *COptimizedClassSaturationSubsumptionClassifierThread::createEmptyTaxonomyForOntology(CConcreteOntology *ontology, CConfigurationBase *config) {
				CTBox *tBox = ontology->getTBox();
				CConceptVector *conVec = tBox->getConceptVector();
				CConcept *topConcept = conVec->getData(1);
				CConcept *bottomConcept = conVec->getData(0);
				CPartialPruningTaxonomy *tax = new CPartialPruningTaxonomy(topConcept,bottomConcept);
				return tax->readConfig(config);
			}


			void COptimizedClassSaturationSubsumptionClassifierThread::readCalculationConfig(CCalculationConfigurationExtension *config) {
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


			
			CSubsumptionClassifierThread *COptimizedClassSaturationSubsumptionClassifierThread::scheduleOntologyClassification(CConcreteOntology *ontology, CTaxonomy *taxonomy, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config) {

				COptimizedClassSaturationOntologyClassificationItem *ontClassItem = new COptimizedClassSaturationOntologyClassificationItem(config,statistics);
				ontClassItem->setClassificationCalculationSupport(classificationSupport);
				ontClassItem->initTaxonomyConcepts(ontology,taxonomy);
				ontItemList.append(ontClassItem);
				processingOntItemList.append(ontClassItem);
				ontItemHash.insert(ontology,ontClassItem);

				CCalculationConfigurationExtension* calcConfig = ontClassItem->getCalculationConfiguration();

				readCalculationConfig(calcConfig);

				CPartialPruningTaxonomy *parTax = dynamic_cast<CPartialPruningTaxonomy *>(taxonomy);
				if (parTax) {
					COntologyClassificationItem *ontClassItem = ontItemHash.value(ontology);
					parTax->createStatistics(ontClassItem->getClassifierStatistics());
				}

				bool readPrecomputationSaturationData = false;
				CPrecomputation* precomputation = ontology->getPrecomputation();
				if (precomputation->isPrecomputed()) {
					if (precomputation->hasSaturationModelData()) {
						readPrecomputationSaturationData = true;
					}
				}
				if (readPrecomputationSaturationData) {
					readSubsumptionFromPrecomputedSaturation(ontClassItem);
				} else {
					bool confPilingSaturation = calcConfig->isSaturationPilingActivated();
					if (confPilingSaturation) {
						createObviousSubsumptionPilingSatisfiableTestingOrder(ontClassItem);
					} else {
						createObviousSubsumptionSatisfiableTestingOrder(ontClassItem);
					}
				}

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



			void COptimizedClassSaturationSubsumptionClassifierThread::propagateExistInitializationFlag(COptimizedConceptSaturationSatisfiableTestingItem* ontConSatTestItem) {
				if (ontConSatTestItem->isPotentiallyExistInitializationConcept()) {
					COptimizedConceptSaturationSatisfiableTestingItem* parentOntConceptSatTestItem = ontConSatTestItem->getSpecialItemReference();
					if (parentOntConceptSatTestItem && !parentOntConceptSatTestItem->isPotentiallyExistInitializationConcept()) {
						parentOntConceptSatTestItem->setPotentiallyExistInitializationConcept(true);
						propagateExistInitializationFlag(parentOntConceptSatTestItem);
					}
				}
			}


			void COptimizedClassSaturationSubsumptionClassifierThread::propagateSubsumerItemFlag(COptimizedConceptSaturationSatisfiableTestingItem* ontClassSatTestItem) {
				if (ontClassSatTestItem->hasIndirectSuccessorsItems()) {
					COptimizedConceptSaturationSatisfiableTestingItem* subsumerItem = ontClassSatTestItem->getSpecialItemReference();
					while (subsumerItem && !subsumerItem->hasIndirectSuccessorsItems()) {
						subsumerItem->setIndirectSuccessorsItems(true);
						subsumerItem = subsumerItem->getSpecialItemReference();
					}
				}
			}


			void COptimizedClassSaturationSubsumptionClassifierThread::analyseConceptSatisfiableSubsumerExistItems(COptimizedClassSaturationOntologyClassificationItem* ontClassItem, COptimizedConceptSaturationSatisfiableTestingItem* ontConSatTestItem, CConcept* concept, bool negated) {

				CSortedNegLinker<CConcept*>* opConIt = concept->getOperandList();
				while (opConIt) {
					CConcept* opConcept = opConIt->getData();
					bool opNegation = opConIt->isNegated()^negated;
					cint64 opConOpCode = opConcept->getOperatorCode();
					if (!opNegation && (opConOpCode == CCSUB || opConOpCode == CCATOM && opConcept->hasClassName())) {
						COptimizedClassSaturationSatisfiableTestingItem* subsumerClassTestItem = ontClassItem->getClassSatisfiableTestItem(opConcept,false);
						subsumerClassTestItem->addSuccessorSatisfiableTestItem(ontConSatTestItem);
						subsumerClassTestItem->setIndirectSuccessorsItems(true);
						if (!ontConSatTestItem->getSpecialItemReference()) {
							ontConSatTestItem->setSpecialItemReference(subsumerClassTestItem);
						} else {
							ontConSatTestItem->setMultipleDirectPredecessorsItems(true);
						}
					} else if (!opNegation && opConOpCode == CCAND) {
						analyseConceptSatisfiableSubsumerExistItems(ontClassItem,ontConSatTestItem,opConcept,opNegation);
					} else if ((!opNegation && (opConOpCode == CCSOME || opConOpCode == CCAQSOME)) || opNegation && opConOpCode == CCALL) {
						CConcept* existConcept = opConcept->getOperandList()->getData();
						COptimizedConceptSaturationSatisfiableTestingItem* existConceptTestItem = ontClassItem->getConceptSatisfiableTestItem(existConcept,false);
						ontConSatTestItem->addExistReferenceConceptItemList(existConceptTestItem);
						ontConSatTestItem->setMultipleDirectPredecessorsItems(true);
					} else if (opConOpCode == CCAQCHOOCE) {
						CSortedNegLinker<CConcept*>* opOpConIt = opConcept->getOperandList();
						while (opOpConIt) {
							if (opNegation == opOpConIt->isNegated()) {
								CConcept* opOpConcept = opOpConIt->getData();
								if (opOpConcept->getOperatorCode() == CCAQSOME) {
									CConcept* existConcept = opOpConcept->getOperandList()->getData();
									COptimizedConceptSaturationSatisfiableTestingItem* existConceptTestItem = ontClassItem->getConceptSatisfiableTestItem(existConcept,false);
									ontConSatTestItem->addExistReferenceConceptItemList(existConceptTestItem);
									ontConSatTestItem->setMultipleDirectPredecessorsItems(true);
								} else if (opOpConcept->getOperatorCode() == CCAQAND) {
									analyseConceptSatisfiableSubsumerExistItems(ontClassItem,ontConSatTestItem,opOpConcept,false);
								}
							}
							opOpConIt = opOpConIt->getNext();
						}
					} else {
						ontConSatTestItem->setMultipleDirectPredecessorsItems(true);
					}
					opConIt = opConIt->getNext();
				}
			}


			void COptimizedClassSaturationSubsumptionClassifierThread::orderItemsSaturationTesting(COptimizedClassSaturationOntologyClassificationItem* ontClassItem, COptimizedConceptSaturationSatisfiableTestingItem* ontConSatTestItem, QList<COptimizedConceptSaturationSatisfiableTestingItem*>& orderedItemList) {
				if (!ontConSatTestItem->isOrderingQueued()) {


					// first mark all subsumer items
					QList<COptimizedConceptSaturationSatisfiableTestingItem*> markOderingSubsumerList;
					COptimizedConceptSaturationSatisfiableTestingItem* subsumerItem = ontConSatTestItem;
					while (subsumerItem && !subsumerItem->isOrderingQueued()) {
						subsumerItem->setOrderingQueued(true);
						markOderingSubsumerList.prepend(subsumerItem);
						subsumerItem = subsumerItem->getSpecialItemReference();
					}


					
					for (QList<COptimizedConceptSaturationSatisfiableTestingItem*>::const_iterator it1 = markOderingSubsumerList.constBegin(), it1End = markOderingSubsumerList.constEnd(); it1 != it1End; ++it1) {
						COptimizedConceptSaturationSatisfiableTestingItem* subsumerItem = *it1;

						QList<COptimizedConceptSaturationSatisfiableTestingItem*>* existRefItemList = subsumerItem->getExistReferenceConceptItemList();
						for (QList<COptimizedConceptSaturationSatisfiableTestingItem*>::const_iterator it2 = existRefItemList->constBegin(), it2End = existRefItemList->constEnd(); it2 != it2End; ++it2) {
							COptimizedConceptSaturationSatisfiableTestingItem* existRefItem = *it2;
							orderItemsSaturationTesting(ontClassItem,existRefItem,orderedItemList);
						}

						orderedItemList.append(subsumerItem);
					}

				}
			}



			void COptimizedClassSaturationSubsumptionClassifierThread::createObviousSubsumptionPilingSatisfiableTestingOrder(COptimizedClassSaturationOntologyClassificationItem* ontClassItem) {

				CTBox *tBox = ontClassItem->getOntology()->getDataBoxes()->getTBox();

				CConcreteOntology *onto = ontClassItem->getOntology();
				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();

				CConceptVector* conVec = tBox->getConceptVector();

				LOG(INFO,getLogDomain(),logTr("Determine saturation order to classify ontology '%1'.").arg(ontClassItem->getOntology()->getTerminologyName()),getLogObject());


				QHash<CConcept*,COptimizedClassSaturationSatisfiableTestingItem*>* conceptSatItemHash = ontClassItem->getClassSatisfiableTestItemHash();
				QList<COptimizedClassSaturationSatisfiableTestingItem*>* satTestItemContainer = ontClassItem->getClassSatisfiableTestItemList();

				COptimizedClassSaturationSatisfiableTestingItem* topItem = ontClassItem->getClassSatisfiableTestItem(topConcept,true);
				COptimizedClassSaturationSatisfiableTestingItem* bottomItem = ontClassItem->getClassSatisfiableTestItem(bottomConcept,true);

				ontClassItem->initTopBottomSatisfiableTestingItems(topItem,bottomItem);

				CSaturationCalculationJob* satCalculationJob = nullptr;
				CSaturationCalculationJobGenerator satCalculationJobGenerator(onto);


				QList<COptimizedClassSaturationSatisfiableTestingItem*> currentConItemList;
				QSet<COptimizedClassSaturationSatisfiableTestingItem*> remainConItemSet;

				QList<CConcept*> testConList;

				cint64 statExistClassCount = 0;
				cint64 statExistsConceptCount = 0;
				cint64 statClassCount = 0;
				cint64 statPrimitiveClassCount = 0;

				QList<COptimizedConceptSaturationSatisfiableTestingItem*> existConList;
				QList<COptimizedConceptSaturationSatisfiableTestingItem*> analyseExistSubsumerList;
				QList<COptimizedConceptSaturationSatisfiableTestingItem*> allConItemList;

				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					cint64 conCount = conVec->getItemCount();
					for (cint64 conIdx = 1; conIdx < conCount; ++conIdx) {
						CConcept* concept = conVec->getData(conIdx);

						//if (CIRIName::getRecentIRIName(concept->getClassNameLinker()) == "file:F:/Projects/OIL/DAMLOilEd/ontologies/ka.daml#Publication") {
						//	bool bug = true;
						//}

						if (concept) {
							cint64 opCode = concept->getOperatorCode();
							if (opCode == CCSOME || opCode == CCALL || opCode == CCAQSOME) {
								++statExistsConceptCount;
								bool negation = opCode == CCALL;
								CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
								CConcept* opConcept = opLinker->getData();
								bool opNegation = opLinker->isNegated()^negation;

								COptimizedConceptSaturationSatisfiableTestingItem* conItem = nullptr;
								COptimizedClassSaturationSatisfiableTestingItem* classItem = nullptr;
								if (!opNegation) {
									if (opConcept->hasClassName() && conceptHash->contains(opConcept)) {
										classItem = ontClassItem->getClassSatisfiableTestItem(opConcept,true);
										classItem->setPotentiallyExistInitializationConcept(true);
										conItem = classItem;
									}
								}
								if (!conItem) {
									conItem = ontClassItem->getConceptSatisfiableTestItem(opConcept,true);
									conItem->setPotentiallyExistInitializationConcept(true);
								}

								existConList.append(conItem);


								if (!opConcept->hasClassName() || !conceptHash->contains(opConcept)) {
									CConceptProcessData* conProcData = (CConceptProcessData*)opConcept->getConceptData();
									CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
									if (!conRefSatLinking) {
										conRefSatLinking = new CConceptSaturationReferenceLinkingData();
										conProcData->setConceptReferenceLinking(conRefSatLinking);
									}
									if (!conRefSatLinking->getClassifierReferenceLinkingData()) {
										conRefSatLinking->setClassifierReferenceLinkingData(conItem);

										allConItemList.append(conItem);
									}
								}

							} else if (concept->hasClassName() && conceptHash->contains(concept)) {

								++statClassCount;
								CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
								CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
								if (!conRefSatLinking) {
									conRefSatLinking = new CConceptSaturationReferenceLinkingData();
									conProcData->setConceptReferenceLinking(conRefSatLinking);
								}

								if (!conRefSatLinking->getClassifierReferenceLinkingData()) {

									COptimizedClassSaturationSatisfiableTestingItem* classItem = ontClassItem->getClassSatisfiableTestItem(concept,true);
									CConceptSaturationReferenceLinkingData* conRefSatLinking = new CConceptSaturationReferenceLinkingData();
									conProcData->setConceptReferenceLinking(conRefSatLinking);
									conRefSatLinking->setClassifierReferenceLinkingData(classItem);

									allConItemList.append(classItem);
								}
							}
						}
					}

					analyseExistSubsumerList = allConItemList;

					// identify all items, which aren't subsumers of other items
					for (QList<COptimizedConceptSaturationSatisfiableTestingItem*>::const_iterator it = analyseExistSubsumerList.constBegin(), itEnd = analyseExistSubsumerList.constEnd(); it != itEnd; ++it) {
						COptimizedConceptSaturationSatisfiableTestingItem* conTestItem = *it;
						CConcept* concept = conTestItem->getSatisfiableConcept();
						cint64 opCode = concept->getOperatorCode();
						analyseConceptSatisfiableSubsumerExistItems(ontClassItem,conTestItem,concept,false);
					}

					// find all potentially subsumer items
					for (QList<COptimizedConceptSaturationSatisfiableTestingItem*>::const_iterator it = analyseExistSubsumerList.constBegin(), itEnd = analyseExistSubsumerList.constEnd(); it != itEnd; ++it) {
						COptimizedConceptSaturationSatisfiableTestingItem* conTestItem = *it;
						propagateSubsumerItemFlag(conTestItem);
					}

					// find all potentially role successors, for which back propagation is necessary
					for (QList<COptimizedConceptSaturationSatisfiableTestingItem*>::const_iterator it = existConList.constBegin(), itEnd = existConList.constEnd(); it != itEnd; ++it) {
						COptimizedConceptSaturationSatisfiableTestingItem* conTestItem = *it;
						propagateExistInitializationFlag(conTestItem);
					}

					QList<COptimizedConceptSaturationSatisfiableTestingItem*> leafConList;
					QList<COptimizedConceptSaturationSatisfiableTestingItem*> leafExistConList;
					// collect all leaf nodes
					for (QList<COptimizedConceptSaturationSatisfiableTestingItem*>::const_iterator it = allConItemList.constBegin(), itEnd = allConItemList.constEnd(); it != itEnd; ++it) {
						COptimizedConceptSaturationSatisfiableTestingItem* conTestItem = *it;
						if (!conTestItem->hasIndirectSuccessorsItems()) {
							if (!conTestItem->isPotentiallyExistInitializationConcept()) {
								leafConList.append(conTestItem);
							} else {
								leafExistConList.append(conTestItem);
							}
						}
					}



					QList<COptimizedConceptSaturationSatisfiableTestingItem*> orderedItemList;
					// order all concepts associated to the items for efficient satisfiable testing, start with the real leaf items
					for (QList<COptimizedConceptSaturationSatisfiableTestingItem*>::const_iterator it = leafConList.constBegin(), itEnd = leafConList.constEnd(); it != itEnd; ++it) {
						COptimizedConceptSaturationSatisfiableTestingItem* conTestItem = *it;
						if (!conTestItem->isOrderingQueued()) {
							orderItemsSaturationTesting(ontClassItem,conTestItem,orderedItemList);
						}
					}
					for (QList<COptimizedConceptSaturationSatisfiableTestingItem*>::const_iterator it = leafExistConList.constBegin(), itEnd = leafExistConList.constEnd(); it != itEnd; ++it) {
						COptimizedConceptSaturationSatisfiableTestingItem* conTestItem = *it;
						if (!conTestItem->isOrderingQueued()) {
							orderItemsSaturationTesting(ontClassItem,conTestItem,orderedItemList);
						}
					}
					for (QList<COptimizedConceptSaturationSatisfiableTestingItem*>::const_iterator it = allConItemList.constBegin(), itEnd = allConItemList.constEnd(); it != itEnd; ++it) {
						COptimizedConceptSaturationSatisfiableTestingItem* conTestItem = *it;
						if (!conTestItem->isOrderingQueued()) {
							orderItemsSaturationTesting(ontClassItem,conTestItem,orderedItemList);
						}
					}


					// set reference mode for saturation: substitute, copy or piling 
					QListIterator<COptimizedConceptSaturationSatisfiableTestingItem*> reverseOrderedIt(orderedItemList);
					reverseOrderedIt.toBack();
					while (reverseOrderedIt.hasPrevious()) {
						COptimizedConceptSaturationSatisfiableTestingItem* conTestItem = reverseOrderedIt.previous();
						COptimizedConceptSaturationSatisfiableTestingItem* referencedTestItem = conTestItem->getSpecialItemReference();
						if (referencedTestItem) {
							if (!conTestItem->isPotentiallyExistInitializationConcept() && !conTestItem->hasMultipleDirectPredecessorsItems()) {
								conTestItem->setSpecialItemReferenceMode(COptimizedConceptSaturationSatisfiableTestingItem::SATURATIONSUBSTITUTEMODE);
							} else if (!referencedTestItem->hasPilingReferenceItem()) {
								conTestItem->setSpecialItemReferenceMode(COptimizedConceptSaturationSatisfiableTestingItem::SATURATIONPILINGMODE);
								referencedTestItem->setPilingReferenceItem(true);
							} else {
								conTestItem->setSpecialItemReferenceMode(COptimizedConceptSaturationSatisfiableTestingItem::SATURATIONCOPYMODE);
							}
						}
					}

					cint64 mSaturationConceptCount = 0;
					for (QList<COptimizedConceptSaturationSatisfiableTestingItem*>::const_iterator it = orderedItemList.constBegin(), itEnd = orderedItemList.constEnd(); it != itEnd; ++it) {
						COptimizedConceptSaturationSatisfiableTestingItem* conItem = *it;
						CConcept* concept = conItem->getSatisfiableConcept();
						satCalculationJob = satCalculationJobGenerator.getSaturationCalculationJob(concept,false,nullptr,satCalculationJob);
						mSaturationConceptCount++;
					}

					LOG(INFO,getLogDomain(),logTr("Ordered %1 concepts for saturation for ontology '%2'.").arg(mSaturationConceptCount).arg(ontClassItem->getOntology()->getTerminologyName()),getLogObject());

				}



				ontClassItem->setSaturationCalculationJob(satCalculationJob);
			}








			void COptimizedClassSaturationSubsumptionClassifierThread::readSubsumptionFromPrecomputedSaturation(COptimizedClassSaturationOntologyClassificationItem* ontClassItem) {
				CTBox *tBox = ontClassItem->getOntology()->getDataBoxes()->getTBox();

				CConcreteOntology *onto = ontClassItem->getOntology();
				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();

				CConceptVector* conVec = tBox->getConceptVector();

				LOG(INFO,getLogDomain(),logTr("Read subsumers from precomputed saturation for ontology '%1'.").arg(ontClassItem->getOntology()->getTerminologyName()),getLogObject());


				QHash<CConcept*,COptimizedClassSaturationSatisfiableTestingItem*>* conceptSatItemHash = ontClassItem->getClassSatisfiableTestItemHash();
				QList<COptimizedClassSaturationSatisfiableTestingItem*>* satTestItemContainer = ontClassItem->getClassSatisfiableTestItemList();

				COptimizedClassSaturationSatisfiableTestingItem* topItem = ontClassItem->getClassSatisfiableTestItem(topConcept,true);
				COptimizedClassSaturationSatisfiableTestingItem* bottomItem = ontClassItem->getClassSatisfiableTestItem(bottomConcept,true);

				ontClassItem->initTopBottomSatisfiableTestingItems(topItem,bottomItem);
				CTaxonomy *tax = ontClassItem->getTaxonomy();
				CHierarchyNode* bottomHierNode = tax->getBottomHierarchyNode();

				CPrecomputedSaturationSubsumerExtractor precSatSubsumerExtractor(onto);

				QList<CConcept*> extClassConceptList;

				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					cint64 conCount = conVec->getItemCount();
					for (cint64 conIdx = 1; conIdx < conCount; ++conIdx) {
						CConcept* concept = conVec->getData(conIdx);

						if (concept) {
							if (concept->hasClassName() && conceptHash->contains(concept)) {
								bool unsatFlag = false;
								bool insuffFlag = false;
								precSatSubsumerExtractor.getConceptFlags(concept,&unsatFlag,&insuffFlag);

								if (unsatFlag) {
									bottomHierNode->addEquivalentConcept(concept);
								} else {
									CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
									CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
									if (!conRefSatLinking) {
										conRefSatLinking = new CConceptSaturationReferenceLinkingData();
										conProcData->setConceptReferenceLinking(conRefSatLinking);
									}
									COptimizedClassSaturationSatisfiableTestingItem* classItem = ontClassItem->getClassSatisfiableTestItem(concept,true);
									conRefSatLinking->setClassifierReferenceLinkingData(classItem);

									extClassConceptList.append(concept);
								}
							}
						}
					}
				}

				for (QList<CConcept*>::const_iterator it = extClassConceptList.constBegin(), itEnd = extClassConceptList.constEnd(); it != itEnd; ++it) {
					CConcept* concept(*it);
					CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
					CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
					COptimizedClassSaturationSatisfiableTestingItem* classItem = (COptimizedClassSaturationSatisfiableTestingItem*)conRefSatLinking->getClassifierReferenceLinkingData();

					precSatSubsumerExtractor.extractSubsumers(concept,classItem);
				}
				ontClassItem->setRemainingSaturationCalculation(false);
				ontClassItem->setSaturationCalculated(true);
				ontClassItem->decCurrentCalculatingCount();
				++satTestedCount;
			}







			void COptimizedClassSaturationSubsumptionClassifierThread::createObviousSubsumptionSatisfiableTestingOrder(COptimizedClassSaturationOntologyClassificationItem* ontClassItem) {

				CTBox *tBox = ontClassItem->getOntology()->getDataBoxes()->getTBox();

				CConcreteOntology *onto = ontClassItem->getOntology();
				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();

				CConceptVector* conVec = tBox->getConceptVector();


				QHash<CConcept*,COptimizedClassSaturationSatisfiableTestingItem*>* conceptSatItemHash = ontClassItem->getClassSatisfiableTestItemHash();
				QList<COptimizedClassSaturationSatisfiableTestingItem*>* satTestItemContainer = ontClassItem->getClassSatisfiableTestItemList();

				COptimizedClassSaturationSatisfiableTestingItem* topItem = ontClassItem->getClassSatisfiableTestItem(topConcept,true);
				COptimizedClassSaturationSatisfiableTestingItem* bottomItem = ontClassItem->getClassSatisfiableTestItem(bottomConcept,true);

				ontClassItem->initTopBottomSatisfiableTestingItems(topItem,bottomItem);

				CSaturationCalculationJob* satCalculationJob = nullptr;
				CSaturationCalculationJobGenerator satCalculationJobGenerator(onto);


				QList<COptimizedClassSaturationSatisfiableTestingItem*> currentConItemList;
				QSet<COptimizedClassSaturationSatisfiableTestingItem*> remainConItemSet;

				QList<CConcept*> testConList;

				cint64 statExistClassCount = 0;
				cint64 statExistsConceptCount = 0;
				cint64 statClassCount = 0;
				cint64 statPrimitiveClassCount = 0;

				//QSet<CConcept*> primitiveClassSet;
				//QSet<CConcept*> directSubsumerClassSet;

				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					cint64 conCount = conVec->getItemCount();
					for (cint64 conIdx = 1; conIdx < conCount; ++conIdx) {
						CConcept* concept = conVec->getData(conIdx);
						if (concept) {
							cint64 opCode = concept->getOperatorCode();
							if (opCode == CCSOME || opCode == CCALL || opCode == CCAQSOME) {
								++statExistsConceptCount;
								bool negation = opCode == CCALL;
								CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
								CConcept* opConcept = opLinker->getData();
								bool opNegation = opLinker->isNegated()^negation;
								COptimizedClassSaturationSatisfiableTestingItem* classItem = nullptr;
								if (!opNegation) {
									if (opConcept->hasClassName() && conceptHash->contains(opConcept)) {
										++statExistClassCount;
										classItem = ontClassItem->getClassSatisfiableTestItem(opConcept,true);
										classItem->setPotentiallyExistInitializationConcept(true);
									}
								}
								CConceptProcessData* conProcData = (CConceptProcessData*)opConcept->getConceptData();
								CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
								if (!conRefSatLinking) {
									conRefSatLinking = new CConceptSaturationReferenceLinkingData();
									conProcData->setConceptReferenceLinking(conRefSatLinking);
								}
								if (!conRefSatLinking->getClassifierReferenceLinkingData()) {
									conRefSatLinking->setClassifierReferenceLinkingData(classItem);
									testConList.append(opConcept);
								}

							} else if (concept->hasClassName() && conceptHash->contains(concept)) {

								++statClassCount;
								CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
								CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
								if (!conRefSatLinking) {
									conRefSatLinking = new CConceptSaturationReferenceLinkingData();
									conProcData->setConceptReferenceLinking(conRefSatLinking);
								}
								if (!conRefSatLinking->getClassifierReferenceLinkingData()) {

									CConcept* directSubsumer = nullptr;
									if (concept->getOperatorCode() == CCSUB && concept->getOperandCount() == 1) {
										CConcept* opConcept = concept->getOperandList()->getData();
										bool opConNegation = concept->getOperandList()->isNegated();
										if (opConcept->hasClassName() && !opConNegation) {
											directSubsumer = opConcept;
										}
									}

									COptimizedClassSaturationSatisfiableTestingItem* conItem = ontClassItem->getClassSatisfiableTestItem(concept,true);

									if (!directSubsumer) {
										// add first
										++statPrimitiveClassCount;
										currentConItemList.append(conItem);
									} else {
										// add after subsumer has been added
										COptimizedClassSaturationSatisfiableTestingItem* subsumerItem = ontClassItem->getClassSatisfiableTestItem(directSubsumer,true);
										subsumerItem->addSuccessorSatisfiableTestItem(conItem);
										remainConItemSet.insert(conItem);
										//primitiveClassSet.insert(concept);
										//directSubsumerClassSet.insert(directSubsumer);
									}
								}
							}
						}
					}


					//cint64 statLeafClassCount = 0;
					//cint64 statExistLeafClassCount = 0;
					//cint64 statNonPrimitiveLeafClassCount = 0;
					//cint64 statAndSubLeafClassCount = 0;
					//QSet<CConcept*> leafConceptSet;
					//QSet<CConcept*> existLeafConceptSet;
					//QSet<CConcept*> leafNonPrimitiveClassSet;
					//CMBox *mBox = ontClassItem->getOntology()->getDataBoxes()->getMBox();
					//CConceptDependenceVector* conDepVec = mBox->getConceptDependenceVector(false);
					//for (QSet<CConcept*>::const_iterator it = conceptHash->constBegin(), itEnd = conceptHash->constEnd(); it != itEnd; ++it) {
					//	CConcept* concept = *it;
					//	CConceptDependence* conDep = conDepVec->getData(concept->getConceptTag());
					//	bool hasDependencies = false;
					//	bool hasNonExistDependencies = false;
					//	bool hasAndSubDependencies = false;
					//	if (conDep) {
					//		if (conDep->getConceptDependenceList() != nullptr) {
					//			hasDependencies = true;
					//			CSortedNegLinker<cint64>* conceptDepLinkerIt = conDep->getConceptDependenceList();
					//			while (conceptDepLinkerIt) {
					//				cint64 conTag = conceptDepLinkerIt->getData();
					//				CConcept* depConcept = conVec->getData(conTag);
					//				if (depConcept) {
					//					cint64 depConCode = depConcept->getOperatorCode();
					//					if (depConCode != CCSOME && depConCode != CCALL) {
					//						hasNonExistDependencies = true;
					//					}
					//					if (depConCode == CCAND || depConCode == CCSUB) {
					//						hasAndSubDependencies = true;
					//					} else {
					//						bool bug = true;
					//					}
					//				}
					//				conceptDepLinkerIt = conceptDepLinkerIt->getNext();
					//			}
					//		}
					//	}
					//	if (!hasDependencies) {
					//		++statLeafClassCount;
					//		leafConceptSet.insert(concept);
					//		if (!primitiveClassSet.contains(concept)) {
					//			leafNonPrimitiveClassSet.insert(concept);
					//			++statNonPrimitiveLeafClassCount;
					//		}
					//	} else {
					//		if (!hasNonExistDependencies) {
					//			++statExistLeafClassCount;
					//			existLeafConceptSet.insert(concept);
					//		}
					//		if (!hasAndSubDependencies) {
					//			++statAndSubLeafClassCount;
					//		}
					//	}
					//}
					//directSubsumerClassSet -= primitiveClassSet;


					QList<COptimizedClassSaturationSatisfiableTestingItem*> nextConItemList;
					do {
						nextConItemList.clear();
						for (QList<COptimizedClassSaturationSatisfiableTestingItem*>::const_iterator it = currentConItemList.constBegin(), itEnd = currentConItemList.constEnd(); it != itEnd; ++it) {
							COptimizedClassSaturationSatisfiableTestingItem* currConItem = *it;
							CConcept* concept = currConItem->getSatisfiableConcept();
							CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
							CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
							if (!conRefSatLinking) {
								conRefSatLinking = new CConceptSaturationReferenceLinkingData();
								conProcData->setConceptReferenceLinking(conRefSatLinking);
							}
							if (!conRefSatLinking->getClassifierReferenceLinkingData()) {
								conRefSatLinking->setClassifierReferenceLinkingData(currConItem);
								testConList.append(concept);
							}
							QList<COptimizedClassSaturationSatisfiableTestingItem*>* succConItemList = (QList<COptimizedClassSaturationSatisfiableTestingItem*>*)currConItem->getSuccessorItemList();
							for (QList<COptimizedClassSaturationSatisfiableTestingItem*>::const_iterator itSucc = succConItemList->constBegin(), itSuccEnd = succConItemList->constEnd(); itSucc != itSuccEnd; ++itSucc) {
								COptimizedClassSaturationSatisfiableTestingItem* succItem = *itSucc;
								remainConItemSet.remove(succItem);
								nextConItemList.append(succItem);
							}
						}
						if (nextConItemList.isEmpty() && !remainConItemSet.isEmpty()) {
							QSet<COptimizedClassSaturationSatisfiableTestingItem*>::iterator it = remainConItemSet.begin();
							COptimizedClassSaturationSatisfiableTestingItem* nextItem = *it;
							nextConItemList.append(nextItem);
							remainConItemSet.erase(it);
						}
						currentConItemList = nextConItemList;
					} while (!currentConItemList.isEmpty());


					for (QList<CConcept*>::const_iterator it = testConList.constBegin(), itEnd = testConList.constEnd(); it != itEnd; ++it) {
						CConcept* concept = *it;
						satCalculationJob = satCalculationJobGenerator.getSaturationCalculationJob(concept,false,nullptr,satCalculationJob);
					}
				}


				//cint64 subRefCountVec[10];
				//cint64 subRefCountMore = 0;
				//for (cint64 i = 0; i < 10; ++i) {
				//	subRefCountVec[i] = 0;
				//}

				//for (QList<CConcept*>::const_iterator it = testConList.constBegin(), itEnd = testConList.constEnd(); it != itEnd; ++it) {
				//	CConcept* concept = *it;
				//	cint64 subRefCount = 0;
				//	CSortedNegLinker<CConcept*>* opListIt = concept->getOperandList();
				//	while (opListIt) {
				//		CConcept* opConcept = opListIt->getData();
				//		if (opConcept->getOperatorCode() == CCSUB) {
				//			++subRefCount;
				//		}
				//		if (opConcept->getOperatorCode() == CCAND) {
				//			CSortedNegLinker<CConcept*>* opOpListIt = opConcept->getOperandList();
				//			while (opOpListIt) {
				//				CConcept* opOpConcept = opOpListIt->getData();
				//				if (opOpConcept->getOperatorCode() == CCSUB) {
				//					++subRefCount;
				//				}
				//				opOpListIt = opOpListIt->getNext();
				//			}
				//		}
				//		opListIt = opListIt->getNext();
				//	}
				//	if (subRefCount < 10) {
				//		subRefCountVec[subRefCount]++;
				//	} else {
				//		subRefCountMore++;
				//	}
				//}

				//cint64 subRefCount0 = subRefCountVec[0];
				//cint64 subRefCount1 = subRefCountVec[1];
				//cint64 subRefCount2 = subRefCountVec[2];
				//cint64 subRefCount3 = subRefCountVec[3];
				//cint64 subRefCount4 = subRefCountVec[4];
				//cint64 subRefCount5 = subRefCountVec[5];
				//cint64 subRefCount6 = subRefCountVec[6];
				//cint64 subRefCount7 = subRefCountVec[7];
				//cint64 subRefCount8 = subRefCountVec[8];
				//cint64 subRefCount9 = subRefCountVec[9];


				ontClassItem->setSaturationCalculationJob(satCalculationJob);
			}



			QString COptimizedClassSaturationSubsumptionClassifierThread::getStatusString() {
				statUpdateMutex.lock();
				QString string = statusString;
				string.detach();
				statUpdateMutex.unlock();
				return string;
			}



			CClassificationProgress* COptimizedClassSaturationSubsumptionClassifierThread::getClassificationProgress() {
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



			bool COptimizedClassSaturationSubsumptionClassifierThread::createNextSubsumtionTest() {


				COntologyClassificationItem *loopOntClassItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !processingOntItemList.isEmpty()) {
					COntologyClassificationItem *ontClassItem = processingOntItemList.first();

					if (ontClassItem == loopOntClassItem) {
						// don't run into infinite loop without doing something
						break;
					}

					COptimizedClassSaturationOntologyClassificationItem *optSubClassItem = (COptimizedClassSaturationOntologyClassificationItem *)ontClassItem;


					while (!workTestCreated && optSubClassItem->hasRemainingSaturationCalculation() && !ontClassItem->isTaxonomyConstructionFailed()) {
						// get next satisfiable test

						CSaturationCalculationJob* satCalcJob = optSubClassItem->getSaturationCalculationJob();
						if (satCalcJob) {
							workOntItemHash.insert(satCalcJob,optSubClassItem);

							optSubClassItem->incCurrentCalculatingCount();
							processCalculationJob(satCalcJob,optSubClassItem);
							optSubClassItem->setRemainingSaturationCalculation(false);
							workTestCreated = true;
						}
					}


					if (!workTestCreated) {
						if (optSubClassItem->hasSaturationCalculated() || ontClassItem->isTaxonomyConstructionFailed()) {
							finishOntologyClassification(ontClassItem);
							processingOntItemList.removeFirst();
							delete ontClassItem;
						} else {
							processingOntItemList.removeFirst();
							processingOntItemList.append(ontClassItem);
							loopOntClassItem = ontClassItem;
						}
					}
				}
				return workTestCreated;
			}





			CSubsumptionClassifierThread *COptimizedClassSaturationSubsumptionClassifierThread::processCalculationJob(CSaturationCalculationJob* job, COntologyClassificationItem *ontClassItem) {
				CSaturationCalculatedCallbackEvent *testResultCallback = new CSaturationCalculatedCallbackEvent(this,job);
				CClassificationStatisticsCollectionStrings* classifStats = ontClassItem->getClassificationStatisticsCollectionStrings();
				if (classifStats && ontClassItem->isCollectProcessStatisticsActivated()) {
					job->setCalclulationStatisticsCollector(classifStats->createCalculationStatisticsCollection());
				}
				job->setCalculationConfiguration(ontClassItem->getCalculationConfiguration());
				if (!calculationManager) {
					calculationManager = reasoner->getCalculationManager();
				}
				if (calculationManager) {
					calculationManager->calculateJob(job,testResultCallback);
				} else {
					// error, not compatible
				}
				return this;
			}


			bool COptimizedClassSaturationSubsumptionClassifierThread::interpreteToldSubsumptionResult(COntologyClassificationItem *ontClassItem, const QList<QPair<CConcept *,CConcept *> > &subSumRelList, bool isSubSum) {
				return false;
			}


			bool COptimizedClassSaturationSubsumptionClassifierThread::interpreteSubsumptionResult(COntologyClassificationItem *ontClassItem, CConcept *subsumerConcept, CConcept *subsumedConcept, bool isSubsumption) {
				return false;
			}


			bool COptimizedClassSaturationSubsumptionClassifierThread::interpreteSatisfiableResult(COntologyClassificationItem *ontClassItem, CConcept *satisfiableConcept, bool isSatis) {
				return false;
			}

			bool COptimizedClassSaturationSubsumptionClassifierThread::interpreteTestResults(CTestCalculatedCallbackEvent *testResult) {
				return false;
			}


			bool COptimizedClassSaturationSubsumptionClassifierThread::interpreteTestResults(CSaturationCalculatedCallbackEvent *testResult) {
				CSaturationCalculationJob *satCalcJob = testResult->getSatisfiableCalculationJob();

				COntologyClassificationItem *ontClassItem = workOntItemHash.value(satCalcJob);
				if (ontClassItem) {
					CTaxonomy *taxonomy = ontClassItem->getTaxonomy();

					++satTestedCount;
					ontClassItem->decCurrentCalculatingCount();
					COptimizedClassSaturationOntologyClassificationItem *optSubClassItem = (COptimizedClassSaturationOntologyClassificationItem *)ontClassItem;
					optSubClassItem->setSaturationCalculated(true);
					workOntItemHash.remove(satCalcJob);

#ifdef OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGTAXONOMYCOUTOUTPUT
					QString taxString = taxonomy->getConceptHierarchyString();
					cout<<taxString.toLocal8Bit().data()<<endl<<endl;
#endif
				} 

				return true;
			}


			bool COptimizedClassSaturationSubsumptionClassifierThread::finishOntologyClassification(COntologyClassificationItem *ontClassItem) {

				if (!ontClassItem->isTaxonomyConstructed() && !ontClassItem->isTaxonomyConstructionFailed()) {
					CTaxonomy *taxonomy = ontClassItem->getTaxonomy();
					CConcreteOntology *ontology = ontClassItem->getOntology();

					LOG(INFO,getLogDomain(),logTr("Building class taxonomy for ontology '%1'.").arg(ontology->getTerminologyName()),getLogObject());

#ifdef OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGLOGOUTPUT
					LOG(INFO,getLogDomain(),QString("Building Class Taxonomy"),this);
#endif


					// build taxonomy

					COptimizedClassSaturationOntologyClassificationItem *optSubClassItem = (COptimizedClassSaturationOntologyClassificationItem *)ontClassItem;
					QList<COptimizedClassSaturationSatisfiableTestingItem*>* satItemList = optSubClassItem->getClassSatisfiableTestItemList();

					COptimizedClassSaturationSatisfiableTestingItem* bottomItem = optSubClassItem->getBottomConceptSatisfiableTestItem();

					CTaxonomy *tax = optSubClassItem->getTaxonomy();

					QList<COptimizedClassSaturationSatisfiableTestingItem*> itemList;

					COptimizedClassSaturationSatisfiableTestingItem* topItem = optSubClassItem->getTopConceptSatisfiableTestItem();
					CHierarchyNode* topHierNode = tax->getTopHierarchyNode();
					CHierarchyNode* bottomHierNode = tax->getBottomHierarchyNode();



					cint64 topSubsumingItemCount = topItem->getSubsumingConceptItemCount();
					topItem->setSatisfiableConceptHierarchyNode(topHierNode);
					if (topSubsumingItemCount) {
						QList<COptimizedClassSaturationSatisfiableTestingItem*>* subsumingConceptItemList = topItem->sortSubsumingConceptItemList();
						for (QList<COptimizedClassSaturationSatisfiableTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
							COptimizedClassSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
							// mark as equivalent
							subsumedConceptItem->setEquivalentItem(true);
							topHierNode->addEquivalentConcept(subsumedConceptItem->getSatisfiableConcept());
							tax->updateNodeEquivalences(topHierNode);
						}
					}


					for (QList<COptimizedClassSaturationSatisfiableTestingItem*>::const_iterator it = satItemList->constBegin(), itEnd = satItemList->constEnd(); it != itEnd; ++it) {
						COptimizedClassSaturationSatisfiableTestingItem* item = *it;
						if (!item->isEquivalentItem() && item != bottomItem && item != topItem) {
							CHierarchyNode* itemNode = tax->getHierarchyNode(item->getSatisfiableConcept(),true);
							item->setSatisfiableConceptHierarchyNode(itemNode);
							cint64 itemSubsumingCount = item->getSubsumingConceptItemCount();
							itemList.append(item);
							if (itemSubsumingCount > topSubsumingItemCount) {
								QList<COptimizedClassSaturationSatisfiableTestingItem*>* subsumingConceptItemList = item->sortSubsumingConceptItemList();
								for (QList<COptimizedClassSaturationSatisfiableTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd(); subsumedIt != subsumedItEnd; ++subsumedIt) {
									COptimizedClassSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
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


					for (QList<COptimizedClassSaturationSatisfiableTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedClassSaturationSatisfiableTestingItem* item = *it;
						if (!item->isEquivalentItem() && item != topItem) {
							cint64 itemSubsumingCount = item->getSubsumingConceptItemCount();
							CHierarchyNode* itemHierNode = item->getSatisfiableConceptHierarchyNode();

							QList<COptimizedClassSaturationSatisfiableTestingItem*>* subsumingConceptItemList = item->getSubsumingConceptItemList();
							QList<COptimizedClassSaturationSatisfiableTestingItem*>::const_iterator subsumedIt = subsumingConceptItemList->constBegin(), subsumedItEnd = subsumingConceptItemList->constEnd();

							cint64 eqCount = 0;
							cint64 remainingSubsumingCount = itemSubsumingCount;
							// remove direct equivalences
							while (subsumedIt != subsumedItEnd) {
								COptimizedClassSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
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
								QList<COptimizedClassSaturationSatisfiableTestingItem*> parentItemList;
								while (subsumedIt != subsumedItEnd) {
									COptimizedClassSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
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
									COptimizedClassSaturationSatisfiableTestingItem* subsumedConceptItem = *subsumedIt;
									if (!subsumedConceptItem->isEquivalentItem()) {
										bool predOfOnePrevParent = false;
										if (searchForMoreParents) {
											for (QList<COptimizedClassSaturationSatisfiableTestingItem*>::const_iterator parentIt = parentItemList.constBegin(), parentItEnd = parentItemList.constEnd(); parentIt != parentItEnd && !predOfOnePrevParent; ++parentIt) {
												COptimizedClassSaturationSatisfiableTestingItem* parentItem = *parentIt;
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

					for (QList<COptimizedClassSaturationSatisfiableTestingItem*>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						COptimizedClassSaturationSatisfiableTestingItem* item = *it;
						if (!item->isPredecessorItem() && !item->isEquivalentItem()) {
							CHierarchyNode* itemHierNode = item->getSatisfiableConceptHierarchyNode();
							bottomHierNode->makeParent(itemHierNode);
						}
					}


#ifdef OPTIMIZEDCLASSSATURATIONCLASSIFIERDEBUGSTATUSDESCRIPTION
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





			bool COptimizedClassSaturationSubsumptionClassifierThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (CSubsumptionClassifierThread::processCustomsEvents(type,event)) {
					return true;
				} else if (type == CSaturationCalculatedCallbackEvent::EVENTTYPE) {
					CSaturationCalculatedCallbackEvent *tcue = (CSaturationCalculatedCallbackEvent *)event;
					interpreteTestResults(tcue);
					currRunningTestParallelCount--;
					doNextPendingTests();
					return true;
				} 
				return false;
			}


			bool COptimizedClassSaturationSubsumptionClassifierThread::interceptTestResults(CInterceptOntologyTestResultEvent *interceptResult) {
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
