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

#include "CTotallyPrecomputationThread.h"



#ifndef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
	#define KONCLUDE_PRECOMPUTATION_SATURATION_STATISTIC_OUTPUT
#endif


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CTotallyPrecomputationThread::CTotallyPrecomputationThread(CReasonerManager *reasonerManager) : CPrecomputationThread(reasonerManager),CLogIdentifier("::Konclude::Reasoner::Kernel::Precomputator",this) {
				mReuseCompletionGraphCacheWriter = nullptr;
				CCompletionGraphCache* complGraphCache = reasonerManager->getCompletionGraphCache();
				if (complGraphCache) {
					CReuseCompletionGraphCache* reuseComplGraphCache = dynamic_cast<CReuseCompletionGraphCache*>(complGraphCache);
					if (reuseComplGraphCache) {
						mReuseCompletionGraphCacheWriter = reuseComplGraphCache->createCacheWriter();
					}
				}
			}


			CTotallyPrecomputationThread::~CTotallyPrecomputationThread() {
			}


			COntologyPrecomputationItem* CTotallyPrecomputationThread::initializeOntologyPrecomputionItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				CTotallyOntologyPrecomputationItem* item = new CTotallyOntologyPrecomputationItem();
				item->initTotallyPrecomputationItem(ontology,config);
				return item;
			}



			bool CTotallyPrecomputationThread::createNextTest() {


				COntologyPrecomputationItem* loopOntPreCompItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !mProcessingOntItemList.isEmpty()) {
					COntologyPrecomputationItem* ontPreCompItem = mProcessingOntItemList.first();

					if (ontPreCompItem == loopOntPreCompItem) {
						// don't run into infinite loop without doing something
						break;
					}

					CTotallyOntologyPrecomputationItem* totallyPreCompItem = (CTotallyOntologyPrecomputationItem*)ontPreCompItem;

					if (!workTestCreated && totallyPreCompItem->isConsistenceStepRequired()) {
						if (!totallyPreCompItem->isConsistenceStepFinished()) {
							if (totallyPreCompItem->areConsistenceStepProcessingRequirementSatisfied()) {

								if (!totallyPreCompItem->hasConceptSaturationPrecomputationCreated()) {
									LOG(INFO,getLogDomain(),logTr("Preparing concept saturation."),this);
									CConcreteOntology* ontology = totallyPreCompItem->getOntology();
									totallyPreCompItem->setSaturationStepRunning(true);
									createSaturationConstructionJob(totallyPreCompItem);
									totallyPreCompItem->setConceptSaturationPrecomputationCreated(true);
									totallyPreCompItem->setSaturationComputationRunning(true);

									addIdentifiedRemainingConsistencyRequiredConcepts(totallyPreCompItem);

								}


								if (totallyPreCompItem->hasRemainingConsistencyRequiredSaturationConcepts() && !totallyPreCompItem->isSaturationComputationRunning()) {
									if (saturateRemainingConsistencyRequiredItems(totallyPreCompItem)) {
										totallyPreCompItem->setSaturationComputationRunning(true);
									}
								}


								if (!totallyPreCompItem->hasConsistenceCheckCreated() && !totallyPreCompItem->isSaturationComputationRunning()) {
									LOG(INFO,getLogDomain(),logTr("Precompute ontology consistency."),this);
									totallyPreCompItem->setConsistenceCheckCreated(true);
									workTestCreated = createConsistencePrecomputationCheck(totallyPreCompItem);
								}

								if (totallyPreCompItem->hasConsistenceCheched()) {
									totallyPreCompItem->getConsistencePrecomputationStep()->setStepFinished(true);

									if (totallyPreCompItem->getOntology()->getConsistence()->isOntologyConsistent()) {
										totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
									} else {
										totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL | COntologyProcessingStatus::PSINCONSITENT);
									}
								}

							} else {
								totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
								totallyPreCompItem->getConsistencePrecomputationStep()->setStepFinished(true);
							}
						} else {
							totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate();
						}
					}







					if (!workTestCreated && totallyPreCompItem->isSaturationStepRequired() && totallyPreCompItem->isConsistenceStepFinished()) {
						if (!totallyPreCompItem->isSaturationStepFinished()) {
							if (totallyPreCompItem->areSaturationStepProcessingRequirementSatisfied()) {

								if (totallyPreCompItem->hasRemainingRequiredSaturationConcepts() && !totallyPreCompItem->isSaturationComputationRunning()) {
									if (saturateRemainingRequiredItems(totallyPreCompItem)) {
										totallyPreCompItem->setSaturationComputationRunning(true);
									}
								}


								if (totallyPreCompItem->hasConceptSaturationPrecomputationChecked() && !totallyPreCompItem->isSaturationComputationRunning()) {
									LOG(INFO,getLogDomain(),logTr("Concept saturation finished."),this);
									totallyPreCompItem->setSaturationStepRunning(false);
									totallyPreCompItem->getSaturationPrecomputationStep()->setStepFinished(true);

									CPrecomputation* precomputation = totallyPreCompItem->getPrecomputation();
									CSaturationData* saturationData = totallyPreCompItem->getSaturationData();
									if (!saturationData) {
										LOG(ERROR,getLogDomain(),logTr("Unknow error in computation, tableau saturation for ontology '%1' failed.").arg(ontPreCompItem->getOntology()->getTerminologyName()),getLogObject());
										totallyPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
									} else {

#ifdef KONCLUDE_PRECOMPUTATION_SATURATION_STATISTIC_OUTPUT
										logSaturationInfos(totallyPreCompItem);
#endif
										extractCommonDisjunctConceptsFromPrecomputedSaturation(totallyPreCompItem);

										totallyPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
									}
								}

							} else {
								totallyPreCompItem->setSaturationStepRunning(false);
								totallyPreCompItem->getSaturationPrecomputationStep()->setStepFinished(true);
								totallyPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							totallyPreCompItem->getSaturationPrecomputationStep()->submitRequirementsUpdate();
						}
					}







					if (!workTestCreated && totallyPreCompItem->isCycleStepRequired() && totallyPreCompItem->isConsistenceStepFinished() && !totallyPreCompItem->isSaturationStepRunning()) {
						if (!totallyPreCompItem->isCycleStepFinished()) {
							if (totallyPreCompItem->areCycleStepProcessingRequirementSatisfied()) {
								if (!totallyPreCompItem->hasConceptCyclePrecomputationChecked()) {
									if (!totallyPreCompItem->hasConceptCyclePrecomputationInitialized()) {
										CConcreteOntology* ontology = totallyPreCompItem->getOntology();
										COntologyCoreConceptCyclesData* coreConceptCycles = ontology->getCoreConceptCyclesData();
										bool hasConceptCycles = false;
										if (coreConceptCycles) {
											CBOXLIST<CConceptCycleData*>* coreConceptCycleList = coreConceptCycles->getConceptCycleList();
											for (CBOXLIST<CConceptCycleData*>::const_iterator it = coreConceptCycleList->constBegin(), itEnd = coreConceptCycleList->constEnd(); it != itEnd; ++it) {
												CConceptCycleData* conCycleData = *it;
												if (mReuseCompletionGraphCacheWriter) {
													totallyPreCompItem->addRemainingConceptCyclePrecomputation(conCycleData);
													hasConceptCycles = true;
												}
											}
										}

										cint64 conceptCount = totallyPreCompItem->getOntology()->getTBox()->getConceptCount();
										cint64 minSizeCount = qMax<cint64>(conceptCount*0.001,50);
										totallyPreCompItem->setMinConceptCycleTestSize(minSizeCount);
										totallyPreCompItem->setConceptCyclePrecomputationInitialized(true);
										if (!hasConceptCycles) {
											totallyPreCompItem->setConceptCyclePrecomputationChecked(true);
										}
									}

									if (totallyPreCompItem->hasRemainingConceptCyclePrecomputation()) {
										CConcreteOntology* ontology = totallyPreCompItem->getOntology();
										COntologyCoreConceptCyclesData* coreConceptCycles = ontology->getCoreConceptCyclesData();
										QList<CConceptCycleData*>* remConCycPreTestList = totallyPreCompItem->getRemainingConceptCyclePrecomputationList();
										while (!workTestCreated && !remConCycPreTestList->isEmpty()) {
											CConceptCycleData* conceptCycleData = remConCycPreTestList->takeFirst();
											if (conceptCycleData->hasNominalConnection()) {
												workTestCreated = createConceptCyclePrecomputation(conceptCycleData,totallyPreCompItem);
												if (workTestCreated) {
													LOG(INFO,getLogDomain(),logTr("Precompute core concept cycle '%1' of '%2' with '%3' concepts.").arg(conceptCycleData->getCycleID()).arg(coreConceptCycles->getConceptCycleCount()).arg(conceptCycleData->getConceptCount()),this);
												}
											}
										}
									}

									if (!workTestCreated) {
										if (!totallyPreCompItem->hasRemainingConceptCyclePrecomputation() && !totallyPreCompItem->hasCurrentPrecomputationTesting()) {
											totallyPreCompItem->setConceptCyclePrecomputationChecked(true);
										}
									}
								}

								if (!workTestCreated) {
									if (totallyPreCompItem->hasConceptCyclePrecomputationChecked()) {
										totallyPreCompItem->getCyclePrecomputationStep()->setStepFinished(true);
										totallyPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSSUCESSFULL);
									}
								}
							} else {
								totallyPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
								totallyPreCompItem->getCyclePrecomputationStep()->setStepFinished(true);
							}
						} else {
							totallyPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate();
						}
					}


					if (!workTestCreated) {
						if (!totallyPreCompItem->hasRemainingProcessingRequirements()) {
							finishOntologyPrecomputation(totallyPreCompItem);
							mProcessingOntItemList.removeFirst();
						} else {
							mProcessingOntItemList.removeFirst();
							mProcessingOntItemList.append(totallyPreCompItem);
							loopOntPreCompItem = ontPreCompItem;
						}
					}
				}
				return workTestCreated;
			}






			void CTotallyPrecomputationThread::extractCommonDisjunctConceptsFromPrecomputedSaturation(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				LOG(INFO,getLogDomain(),logTr("Extract common disjunct concepts."),this);
				CSaturationCommonDisjunctConceptsExtractor satCommonDisjConExt;
				satCommonDisjConExt.extractCommonDisjunctConcepts(totallyPreCompItem->getOntology(),totallyPreCompItem->getSaturationData(),totallyPreCompItem->getSaturatedDisjunctionSet());
			}



			void CTotallyPrecomputationThread::logSaturationInfos(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CConcreteOntology* ontology = totallyPreCompItem->getOntology();
				CSaturationData* saturationData = totallyPreCompItem->getSaturationData();
				CSaturationTaskData* satTaskData = (CSaturationTaskData*)saturationData;
				if (satTaskData) {
					CSatisfiableCalculationTask* satCalcTask = satTaskData->getSaturationTask();
					if (satCalcTask) {
						CProcessingDataBox* procDataBox = satCalcTask->getProcessingDataBox();
						if (procDataBox) {
							CIndividualSaturationProcessNodeVector* satIndiNodeVec = procDataBox->getIndividualSaturationProcessNodeVector(false);
							if (satIndiNodeVec) {
								cint64 saturatedCount = 0;
								cint64 insufficientCount = 0;
								cint64 criticalCount = 0;
								cint64 clashedCount = 0;
								cint64 indiCount = satIndiNodeVec->getItemCount();
								for (cint64 indiID = 0; indiID < indiCount; ++indiID) {
									CIndividualSaturationProcessNode* indiNode = satIndiNodeVec->getData(indiID);
									if (indiNode && indiNode->isCompleted()) {
										++saturatedCount;
										CIndividualSaturationProcessNodeStatusFlags* flags = indiNode->getIndirectStatusFlags();
										if (flags->hasClashedFlag()) {
											++clashedCount;
										}
										if (flags->hasInsufficientFlag()) {
											++insufficientCount;
										}
										if (flags->hasCriticalFlag()) {
											++criticalCount;
										}
									}
								}
								LOG(INFO,getLogDomain(),logTr("Saturated %1 individual nodes, thereof %2 with clashes, %3 critical and %4 insufficient.").arg(saturatedCount).arg(clashedCount).arg(criticalCount).arg(insufficientCount),this);
							}
						}
					}
				}
			}


			bool CTotallyPrecomputationThread::createConceptCyclePrecomputation(CConceptCycleData* conceptCycleData, CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CConcreteOntology* onto = totallyPreCompItem->getOntology();
				CConcept* repCycleClassConcept = conceptCycleData->getRepresentativeCycleClass();

				if (repCycleClassConcept) {

					if (conceptCycleData->getConceptCount() >= totallyPreCompItem->getMinConceptCycleTestSize()) {

						CConceptCyclePrecomputationTestingItem* conCycPrecTestItem = new CConceptCyclePrecomputationTestingItem(conceptCycleData,totallyPreCompItem);

						CSatisfiableCalculationJobGenerator satCalcJobGen(onto);
						CSatisfiableCalculationJob* satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(repCycleClassConcept);
						CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
						satCalcJob->setCalculationConfiguration(calcConfig);
						satCalcJob->setSatisfiableTaskPreyingAdapter(conCycPrecTestItem->getTaskPreyingAdapter());

						processCalculationJob(satCalcJob,totallyPreCompItem,conCycPrecTestItem);
						return true;
					}
				}
				return false;
			}


			
			bool CTotallyPrecomputationThread::createConsistencePrecomputationCheck(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CConcreteOntology* onto = totallyPreCompItem->getOntology();

				QList<CIndividual*> indiList;
				CIndividualVector* indiVec = onto->getABox()->getIndividualVector(false);
				if (indiVec) {
					cint64 indiCount = indiVec->getItemCount();
					for (cint64 idx = 0; idx < indiCount; ++idx) {
						CIndividual* indi = indiVec->getData(idx);
						if (indi) {
							indiList.append(indi);
						}
					}
				}
				CSatisfiableCalculationJobGenerator satCalcJobGen(onto);
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				bool simpleConsistency = false;
				if (indiList.isEmpty()) {
					CConcept* topConcept = onto->getTBox()->getTopConcept();
					if (topConcept->getOperandCount() > 0) {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topConcept);
					} else {
						simpleConsistency = true;
					}
				} else {
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(indiList);
				}


				CConsistence* consistence = onto->getConsistence();
				if (!consistence) {
					consistence = new CConsistence();
					onto->setConsistence(consistence);
				}
				totallyPreCompItem->setConsistence(consistence);

				if (simpleConsistency) {
					consistence->setOntologyConsistent(simpleConsistency);
					totallyPreCompItem->setConsistenceCheched(true);
					return false;
				} else {
					CConsistencePrecomputationTestingItem* consTestItem = new CConsistencePrecomputationTestingItem(totallyPreCompItem,totallyPreCompItem);
					CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
					satCalcJob->setCalculationConfiguration(calcConfig);
					satCalcJob->setSatisfiableTaskPreyingAdapter(consTestItem->getTaskPreyingAdapter());

					processCalculationJob(satCalcJob,totallyPreCompItem,consTestItem);
					return true;
				}
			}





			void CTotallyPrecomputationThread::propagateExistInitializationFlag(CSaturationConceptDataItem* ontConSatDataItem) {
				if (ontConSatDataItem->isPotentiallyExistInitializationConcept()) {
					CSaturationConceptDataItem* parentOntConceptSatTestItem = ontConSatDataItem->getSpecialItemReference();
					while (parentOntConceptSatTestItem && !parentOntConceptSatTestItem->isPotentiallyExistInitializationConcept()) {
						parentOntConceptSatTestItem->setPotentiallyExistInitializationConcept(true);
						parentOntConceptSatTestItem = parentOntConceptSatTestItem->getSpecialItemReference();
					}
				}
			}


			void CTotallyPrecomputationThread::propagateSubsumerItemFlag(CSaturationConceptDataItem* ontClassSatTestItem) {
				if (ontClassSatTestItem->hasIndirectSuccessorsItems()) {
					CSaturationConceptDataItem* subsumerItem = ontClassSatTestItem->getSpecialItemReference();
					while (subsumerItem && !subsumerItem->hasIndirectSuccessorsItems()) {
						subsumerItem->setIndirectSuccessorsItems(true);
						subsumerItem = subsumerItem->getSpecialItemReference();
					}
				}
			}







			void CTotallyPrecomputationThread::analyseConceptSaturationSubsumerExistItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem, QList<CConceptNegationSaturationItem>& processList) {

				QSet<CConceptNegationSaturationItem> processSet;
				for (QList<CConceptNegationSaturationItem>::const_iterator it = processList.constBegin(), itEnd = processList.constEnd(); it != itEnd; ++it) {
					CConceptNegationSaturationItem conNegSatItem(*it);
					processSet.insert(conNegSatItem);
				}

				while (!processList.isEmpty()) {
					CConceptNegationSaturationItem conNegSatItem(processList.takeLast());
					CConcept* concept = conNegSatItem.getConcept();
					bool negated = conNegSatItem.getNegation();
					CSaturationConceptDataItem* ontConSatDataItem = conNegSatItem.getSaturationConceptDataItem();

					cint64 nOpCode = concept->getOperatorCode();
					cint64 opCount = concept->getOperandCount();
					if (!negated && (nOpCode == CCAND || nOpCode == CCSUB || nOpCode == CCEQ || ((nOpCode == CCOR) && opCount == 1)) || negated && (nOpCode == CCOR || ((nOpCode == CCAND || nOpCode == CCEQ) && opCount == 1))) {

						CSortedNegLinker<CConcept*>* opConIt = concept->getOperandList();
						while (opConIt) {
							CConcept* opConcept = opConIt->getData();
							bool opNegation = opConIt->isNegated()^negated;
							cint64 opConOpCode = opConcept->getOperatorCode();
							if (!opNegation && (opConOpCode == CCEQ || opConOpCode == CCSUB || opConOpCode == CCATOM && opConcept->hasClassName())) {
								CSaturationConceptDataItem* subsumerClassTestItem = totallyPreCompItem->getSaturationConceptDataItem(opConcept,opNegation,false);
								subsumerClassTestItem->setIndirectSuccessorsItems(true);
								if (!ontConSatDataItem->getSpecialItemReference() && !ontConSatDataItem->isInvalidSpecialItemReference()) {
									ontConSatDataItem->setSpecialItemReference(subsumerClassTestItem);
								} else {
									ontConSatDataItem->setMultipleDirectPredecessorsItems(true);
								}
							} else if (!opNegation && opConOpCode == CCAND || opNegation && opConOpCode == CCOR) {
								if (!processSet.contains(CConceptNegationSaturationItem(opConcept,opNegation,ontConSatDataItem))) {
									processSet.insert(CConceptNegationSaturationItem(opConcept,opNegation,ontConSatDataItem));
									processList.append(CConceptNegationSaturationItem(opConcept,opNegation,ontConSatDataItem));
								}

							} else if ((!opNegation && (opConOpCode == CCSOME || opConOpCode == CCAQSOME)) || opNegation && opConOpCode == CCALL) {
								CRole* role = opConcept->getRole();
								CConcept* existConcept = opConcept->getOperandList()->getData();
								bool existConNegation = opConcept->getOperandList()->isNegated()^opNegation;
								CSaturationConceptDataItem* existConceptTestItem =  nullptr;
								if (hasRoleRanges(totallyPreCompItem,role)) {
									existConceptTestItem = totallyPreCompItem->getSaturationRoleSuccessorConceptDataItem(role,existConcept,existConNegation,false);
								} else {
									existConceptTestItem = totallyPreCompItem->getSaturationConceptDataItem(existConcept,existConNegation,false);
								}
								ontConSatDataItem->addExistReferenceConceptItemList(existConceptTestItem);
								ontConSatDataItem->setMultipleDirectPredecessorsItems(true);

							} else if (negated && opConOpCode == CCATMOST || !negated && opConOpCode == CCATLEAST) {
								CRole* role = opConcept->getRole();
								CConcept* existConcept = totallyPreCompItem->getOntology()->getTBox()->getTopConcept();
								bool existConceptNegation = false;
								if (opConcept->getOperandList()) {
									existConcept = opConcept->getOperandList()->getData();
									existConceptNegation = opConcept->getOperandList()->isNegated();
								}
								CSaturationConceptDataItem* existConceptTestItem =  nullptr;
								if (hasRoleRanges(totallyPreCompItem,role)) {
									existConceptTestItem = totallyPreCompItem->getSaturationRoleSuccessorConceptDataItem(role,existConcept,existConceptNegation,false);
								} else {
									existConceptTestItem = totallyPreCompItem->getSaturationConceptDataItem(existConcept,existConceptNegation,false);
								}
								ontConSatDataItem->addExistReferenceConceptItemList(existConceptTestItem);
								ontConSatDataItem->setMultipleDirectPredecessorsItems(true);

							} else if (opConOpCode == CCAQCHOOCE) {
								CSortedNegLinker<CConcept*>* opOpConIt = opConcept->getOperandList();
								while (opOpConIt) {
									if (opNegation == opOpConIt->isNegated()) {
										CConcept* opOpConcept = opOpConIt->getData();
										if (opOpConcept->getOperatorCode() == CCAQSOME) {
											CConcept* existConcept = opOpConcept->getOperandList()->getData();
											bool existConNegation = opOpConcept->getOperandList()->isNegated();
											CRole* role = opOpConcept->getRole();
											CSaturationConceptDataItem* existConceptTestItem =  nullptr;
											if (hasRoleRanges(totallyPreCompItem,role)) {
												existConceptTestItem = totallyPreCompItem->getSaturationRoleSuccessorConceptDataItem(role,existConcept,existConNegation,false);
											} else {
												existConceptTestItem = totallyPreCompItem->getSaturationConceptDataItem(existConcept,existConNegation,false);
											}
											ontConSatDataItem->addExistReferenceConceptItemList(existConceptTestItem);
											ontConSatDataItem->setMultipleDirectPredecessorsItems(true);
										} else if (opOpConcept->getOperatorCode() == CCAQAND) {
											if (!processSet.contains(CConceptNegationSaturationItem(opOpConcept,false,ontConSatDataItem))) {
												processSet.insert(CConceptNegationSaturationItem(opOpConcept,false,ontConSatDataItem));
												processList.append(CConceptNegationSaturationItem(opOpConcept,false,ontConSatDataItem));
											}
										}
									}
									opOpConIt = opOpConIt->getNext();
								}
							} else {
								ontConSatDataItem->setMultipleDirectPredecessorsItems(true);
							}
							opConIt = opConIt->getNext();
						}
					}
				}
			}




			void CTotallyPrecomputationThread::extendDisjunctionsCandidateAlternativesItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CSaturationConceptDataItem* ontConSatDataItem, QList<CSaturationConceptDataItem*>* newDisjunctionCandidateAlternativeList) {
				CConcept* concept = ontConSatDataItem->getSaturationConcept();
				bool negation = ontConSatDataItem->getSaturationNegation();
				QList<TConceptNegPair> conNegPairList;

				bool directExamine = true;
				bool candidateAlternativeExtraction = false;
				if (!negation && concept->hasClassName()) {
					if (concept->getOperatorCode() == CCEQ) {
						CBOXHASH<CConcept*,CConcept*>* eqConHash = totallyPreCompItem->getOntology()->getTBox()->getEquivalentConceptCandidateHash(false);
						if (eqConHash->contains(concept)) {
							candidateAlternativeExtraction = true;
						}
					}
					for (CSortedNegLinker<CConcept*>* opConIt = concept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
						CConcept* opConcept = opConIt->getData();
						bool opNegation = opConIt->isNegated()^negation;
						conNegPairList.append(TConceptNegPair(opConcept,opNegation));
						directExamine = false;
					}
				}
				if (directExamine) {
					conNegPairList.append(TConceptNegPair(concept,negation));
				}

				while (!conNegPairList.isEmpty()) {
					TConceptNegPair nextConNegPair(conNegPairList.takeFirst());
					CConcept* nextConcept = nextConNegPair.first;
					bool nextNegation = nextConNegPair.second;
					cint64 nOpCode = nextConcept->getOperatorCode();
					cint64 opCount = nextConcept->getOperandCount();
					if (!nextNegation && (nOpCode == CCAND || (nOpCode == CCOR && opCount == 1)) || nextNegation && (nOpCode == CCOR || ((nOpCode == CCAND) && opCount == 1))) {
						for (CSortedNegLinker<CConcept*>* opConIt = nextConcept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
							CConcept* opConcept = opConIt->getData();
							bool opNegation = opConIt->isNegated()^nextNegation;
							conNegPairList.append(TConceptNegPair(opConcept,opNegation));
						}

					} else if (nextNegation && ((nOpCode == CCAND || nOpCode == CCEQ) && opCount > 1) || !nextNegation && nOpCode == CCOR) {
						// saturate all disjuncts to extract common concepts from all disjunctions

						totallyPreCompItem->addSaturatedDisjunction(nextConcept);

						for (CSortedNegLinker<CConcept*>* opConIt = nextConcept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
							CConcept* opConcept = opConIt->getData();
							bool opNegation = opConIt->isNegated()^nextNegation;

							CConceptProcessData* conProcData = (CConceptProcessData*)opConcept->getConceptData();
							CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
							if (!conRefSatLinking) {
								conRefSatLinking = new CConceptSaturationReferenceLinkingData();
								conProcData->setConceptReferenceLinking(conRefSatLinking);
							}
							if (conRefSatLinking->getConceptSaturationReferenceLinkingData(opNegation) == nullptr) {
								CSaturationConceptDataItem* conItem = totallyPreCompItem->getSaturationConceptDataItem(opConcept,opNegation,true);
								conRefSatLinking->setSaturationReferenceLinkingData(conItem,opNegation);
								newDisjunctionCandidateAlternativeList->append(conItem);
								if (!opConcept->hasClassName() || opNegation) {
									conItem->setInvalidSpecialItemReference(true);
								}
							}
						}


						CConceptProcessData* conProcData = (CConceptProcessData*)nextConcept->getConceptData();
						CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
						if (!conRefSatLinking) {
							conRefSatLinking = new CConceptSaturationReferenceLinkingData();
							conProcData->setConceptReferenceLinking(conRefSatLinking);
						}
						if (conRefSatLinking->getConceptSaturationReferenceLinkingData(nextNegation) == nullptr) {
							CSaturationConceptDataItem* conItem = totallyPreCompItem->getSaturationConceptDataItem(nextConcept,nextNegation,true);
							conRefSatLinking->setSaturationReferenceLinkingData(conItem,nextNegation);
							newDisjunctionCandidateAlternativeList->append(conItem);
							conItem->setInvalidSpecialItemReference(true);
						}



					} else if (!nextNegation && nOpCode == CCALL || nextNegation && nOpCode == CCSOME) {
						if (candidateAlternativeExtraction) {
							// saturate pseudo models to invalidate candidates concepts

							bool saturateNegation = !nextNegation;

							CConceptProcessData* conProcData = (CConceptProcessData*)nextConcept->getConceptData();
							CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
							if (!conRefSatLinking) {
								conRefSatLinking = new CConceptSaturationReferenceLinkingData();
								conProcData->setConceptReferenceLinking(conRefSatLinking);
							}
							if (conRefSatLinking->getConceptSaturationReferenceLinkingData(saturateNegation) == nullptr) {
								CSaturationConceptDataItem* conItem = totallyPreCompItem->getSaturationConceptDataItem(nextConcept,saturateNegation,true);
								conRefSatLinking->setSaturationReferenceLinkingData(conItem,saturateNegation);
								newDisjunctionCandidateAlternativeList->append(conItem);
								conItem->setInvalidSpecialItemReference(true);
							}
						}
					}
				}
			}




			bool CTotallyPrecomputationThread::saturateRemainingRequiredItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				cint64 saturatingConceptCount = 0;
				bool allConSaturReq = totallyPreCompItem->requiresAllConceptsSaturation();
				QList<TConceptNegPair>* reqConSatList = totallyPreCompItem->getRemainingRequiredSaturationConceptList();
				if (allConSaturReq) {
					totallyPreCompItem->setAllConceptsSaturationOrdered(true);
					QList<CSaturationConceptDataItem*>* satOrderdItemList = totallyPreCompItem->getSaturationOrderedItemList();
					for (QList<CSaturationConceptDataItem*>::const_iterator it = satOrderdItemList->constBegin(), itEnd = satOrderdItemList->constEnd(); it != itEnd; ++it) {
						CSaturationConceptDataItem* satConItem(*it);
						if (!satConItem->isItemProcessingMarked()) {
							++saturatingConceptCount;
							satConItem->setItemProcessingMarked(true);
						}
					}
				} else {
					for (QList<TConceptNegPair>::const_iterator it = reqConSatList->constBegin(), itEnd = reqConSatList->constEnd(); it != itEnd; ++it) {
						const TConceptNegPair& conNegPair(*it);
						CConcept* concept(conNegPair.first);
						bool negation(conNegPair.second);
						saturatingConceptCount += markSaturationProcessingItems(totallyPreCompItem,nullptr,concept,negation);
					}
				}
				reqConSatList->clear();
				if (saturatingConceptCount > 0 || totallyPreCompItem->requiresNominalDelayedConceptsSaturationUpdate()) {
					totallyPreCompItem->setNominalDelayedConceptsSaturationUpdateRequired(false);
					createSaturationProcessingJob(totallyPreCompItem,true);
					return true;
				}
				return false;
			}



			bool CTotallyPrecomputationThread::addIdentifiedRemainingConsistencyRequiredConcepts(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				CConcept* topConcept = totallyPreCompItem->getOntology()->getTBox()->getTopConcept();
				bool constConReqSatAdded = false;
				if (topConcept->getOperandList() != nullptr) {
					totallyPreCompItem->addConsistencyRequiredSaturationConcept(topConcept,false);
					constConReqSatAdded = true;
				}
				CIndividualVector* indiVec = totallyPreCompItem->getOntology()->getABox()->getIndividualVector(false);
				if (indiVec) {
					QSet<TConceptNegPair> assConNegPairSet;
					cint64 indiCount = indiVec->getItemCount();
					for (cint64 indiId = 0; indiId < indiCount; ++indiId) {
						CIndividual* indi = indiVec->getData(indiId);
						if (indi) {
							for (CConceptAssertionLinker* conAssLinkIt = indi->getAssertionConceptLinker(); conAssLinkIt; conAssLinkIt = conAssLinkIt->getNext()) {
								CConcept* assCon = conAssLinkIt->getData();
								bool assConNeg = conAssLinkIt->isNegated();
								if (assCon->getOperatorCode() != CCNOMINAL) {
									assConNegPairSet.insert(TConceptNegPair(assCon,assConNeg));
								}
							}
						}
					}
					for (QSet<TConceptNegPair>::const_iterator it = assConNegPairSet.constBegin(), itEnd = assConNegPairSet.constEnd(); it != itEnd; ++it) {
						constConReqSatAdded = true;
						TConceptNegPair assConNegPair(*it);
						totallyPreCompItem->addConsistencyRequiredSaturationConcept(assConNegPair.first,assConNegPair.second);
					}
				}
				
				return constConReqSatAdded;
			}



			bool CTotallyPrecomputationThread::saturateRemainingConsistencyRequiredItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				cint64 saturatingConceptCount = 0;
				QList<TConceptNegPair>* reqConSatList = totallyPreCompItem->getRemainingConsistencyRequiredSaturationConceptList();
				for (QList<TConceptNegPair>::const_iterator it = reqConSatList->constBegin(), itEnd = reqConSatList->constEnd(); it != itEnd; ++it) {
					const TConceptNegPair& conNegPair(*it);
					CConcept* concept(conNegPair.first);
					bool negation(conNegPair.second);
					saturatingConceptCount += markSaturationProcessingItems(totallyPreCompItem,nullptr,concept,negation);
				}
				reqConSatList->clear();
				if (saturatingConceptCount > 0) {
					createSaturationProcessingJob(totallyPreCompItem,false);
					return true;
				}
				return false;
			}


			cint64 CTotallyPrecomputationThread::markSaturationProcessingItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CSaturationConceptDataItem* startMarkingItem, CConcept* startMarkingConcept, bool startMarkingConceptNegation) {

				cint64 markedConcepts = 0;
				QList<CSaturationConceptDataItem*> itemList;
				QList<TConceptNegPair> conNegPairList;

				if (startMarkingItem && !startMarkingItem->isItemProcessingMarked()) {
					++markedConcepts;
					startMarkingItem->setItemProcessingMarked(true);
					itemList.append(startMarkingItem);
				}

				if (startMarkingConcept) {
					CSaturationConceptDataItem* opMarkingItem = totallyPreCompItem->getSaturationConceptDataItem(startMarkingConcept,startMarkingConceptNegation,false);
					if (opMarkingItem) {
						if (!opMarkingItem->isItemProcessingMarked()) {
							++markedConcepts;
							opMarkingItem->setItemProcessingMarked(true);
							itemList.append(opMarkingItem);
						}		
					} else {
						conNegPairList.append(TConceptNegPair(startMarkingConcept,startMarkingConceptNegation));
					}
				}


				while (!itemList.isEmpty() || !conNegPairList.isEmpty()) {

					while (!itemList.isEmpty()) {
						CSaturationConceptDataItem* item = itemList.takeFirst();
						CConcept* nextConcept = item->getSaturationConcept();
						bool nextNegation = item->getSaturationNegation();
						conNegPairList.append(TConceptNegPair(nextConcept,nextNegation));

						QList<CSaturationConceptDataItem*>* existRefItemList = item->getExistReferenceConceptItemList();
						for (QList<CSaturationConceptDataItem*>::const_iterator it2 = existRefItemList->constBegin(), it2End = existRefItemList->constEnd(); it2 != it2End; ++it2) {
							CSaturationConceptDataItem* existRefItem = *it2;
							if (!existRefItem->isItemProcessingMarked()) {
								++markedConcepts;
								existRefItem->setItemProcessingMarked(true);
								itemList.append(existRefItem);
							}
						}
					}


					while (!conNegPairList.isEmpty()) {
						TConceptNegPair nextConNegPair(conNegPairList.takeFirst());
						CConcept* nextConcept = nextConNegPair.first;
						bool nextNegation = nextConNegPair.second;
						cint64 nOpCode = nextConcept->getOperatorCode();
						cint64 opCount = nextConcept->getOperandCount();
						if (!nextNegation && (nOpCode == CCAND || nOpCode == CCSUB || nOpCode == CCIMPLTRIG || nOpCode == CCEQ || (nOpCode == CCOR && opCount == 1) || nOpCode == CCAQSOME || nOpCode == CCSOME || nOpCode == CCATLEAST) || nextNegation && (nOpCode == CCOR || ((nOpCode == CCAND || nOpCode == CCEQ) && opCount == 1) || nOpCode == CCALL || nOpCode == CCATMOST)) {
							for (CSortedNegLinker<CConcept*>* opConIt = nextConcept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
								CConcept* opConcept = opConIt->getData();
								bool influenceNegation = true;
								if (nOpCode == CCAQSOME || nOpCode == CCATLEAST || nOpCode == CCATMOST) {
									influenceNegation = false;
								}
								bool opNegation = opConIt->isNegated();
								if (influenceNegation && nextNegation) {
									opNegation = !opNegation;
								}
								bool opConceptMarked = false;
								if (nOpCode != CCAQCHOOCE || !opNegation) {
									CSaturationConceptDataItem* opMarkingItem = totallyPreCompItem->getSaturationConceptDataItem(opConcept,opNegation,false);
									if (opMarkingItem) {
										if (!opMarkingItem->isItemProcessingMarked()) {
											++markedConcepts;
											opMarkingItem->setItemProcessingMarked(true);
											itemList.append(opMarkingItem);
										}		
									} else {
										conNegPairList.append(TConceptNegPair(opConcept,opNegation));
									}
								}
							}
						}
					}
				}
				return markedConcepts;
			}




			void CTotallyPrecomputationThread::orderItemsSaturationTesting(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CSaturationConceptDataItem* ontConSatDataItem, QList<CSaturationConceptDataItem*>& orderedItemList, QStack<CSaturationSaturationListItem>& saturationItemStack) {


				if (!ontConSatDataItem->isOrderingQueued()) {

					saturationItemStack.push(CSaturationSaturationListItem(ontConSatDataItem));


					while (!saturationItemStack.isEmpty()) {
						CSaturationSaturationListItem& topSatSatListItem = saturationItemStack.top();
						CSaturationConceptDataItem* topOntConSatDataItem = topSatSatListItem.mSaturationItem;
						QList<CSaturationConceptDataItem*>& markOderingSubsumerList = topSatSatListItem.mSaturationItemList;

						if (!topOntConSatDataItem->isOrderingQueued()) {
							if (!topSatSatListItem.mSubsumerOrdered) {
								topSatSatListItem.mSubsumerOrdered = true;

								// first mark all subsumer items
								CSaturationConceptDataItem* subsumerItem = topOntConSatDataItem;
								while (subsumerItem && !subsumerItem->isOrderingQueued()) {
									subsumerItem->setOrderingQueued(true);
									markOderingSubsumerList.prepend(subsumerItem);
									subsumerItem = subsumerItem->getSpecialItemReference();
								}
							}
						}

						if (topSatSatListItem.mLastSubsumerSaturationItem) {
							orderedItemList.append(topSatSatListItem.mLastSubsumerSaturationItem);
							topSatSatListItem.mLastSubsumerSaturationItem = nullptr;
						}

						if (!markOderingSubsumerList.isEmpty()) {
							CSaturationConceptDataItem* subsumerItem = markOderingSubsumerList.takeFirst();
							topSatSatListItem.mLastSubsumerSaturationItem = subsumerItem;

							QList<CSaturationConceptDataItem*>* existRefItemList = subsumerItem->getExistReferenceConceptItemList();
							for (QList<CSaturationConceptDataItem*>::const_iterator it2 = existRefItemList->constBegin(), it2End = existRefItemList->constEnd(); it2 != it2End; ++it2) {
								CSaturationConceptDataItem* existRefItem = *it2;
								saturationItemStack.push(CSaturationSaturationListItem(existRefItem));
							}
							//QListIterator<CSaturationConceptDataItem*> it2(*existRefItemList);
							//it2.toBack();
							//while (it2.hasPrevious()) {
							//	CSaturationConceptDataItem* existRefItem = it2.previous();
							//	saturationItemStack.push(CSaturationSaturationListItem(existRefItem));
							//}


						} else {
							saturationItemStack.pop();
						}
					}	
				}

				//if (!ontConSatDataItem->isOrderingQueued()) {


				//	// first mark all subsumer items
				//	QList<CSaturationConceptDataItem*> markOderingSubsumerList;
				//	CSaturationConceptDataItem* subsumerItem = ontConSatDataItem;
				//	while (subsumerItem && !subsumerItem->isOrderingQueued()) {
				//		subsumerItem->setOrderingQueued(true);
				//		markOderingSubsumerList.prepend(subsumerItem);
				//		subsumerItem = subsumerItem->getSpecialItemReference();
				//	}



				//	for (QList<CSaturationConceptDataItem*>::const_iterator it1 = markOderingSubsumerList.constBegin(), it1End = markOderingSubsumerList.constEnd(); it1 != it1End; ++it1) {
				//		CSaturationConceptDataItem* subsumerItem = *it1;

				//		QList<CSaturationConceptDataItem*>* existRefItemList = subsumerItem->getExistReferenceConceptItemList();
				//		for (QList<CSaturationConceptDataItem*>::const_iterator it2 = existRefItemList->constBegin(), it2End = existRefItemList->constEnd(); it2 != it2End; ++it2) {
				//			CSaturationConceptDataItem* existRefItem = *it2;
				//			orderItemsSaturationTesting(totallyPreCompItem,existRefItem,orderedItemList);
				//		}

				//		orderedItemList.append(subsumerItem);
				//	}

				//}
			}


			bool CTotallyPrecomputationThread::hasRoleRanges(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CRole* role) {
				for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
					CRole* superRole = superRoleIt->getData();
					bool superRoleNegation = superRoleIt->isNegated();
					CSortedNegLinker<CConcept*>* rangeConceptLinker = superRole->getDomainRangeConceptList(!superRoleNegation);
					if (rangeConceptLinker) {
						return true;
					}
				}
				return false;
			}




			void CTotallyPrecomputationThread::createSaturationProcessingJob(CTotallyOntologyPrecomputationItem* totallyPreCompItem, bool allowAllSaturation) {
				CApproximatedSaturationCalculationJob* satCalculationJob = nullptr;
				CConcreteOntology *onto = totallyPreCompItem->getOntology();
				CApproximatedSaturationCalculationJobGenerator satCalculationJobGenerator(onto);
				satCalculationJob = satCalculationJobGenerator.getApproximatedSaturationCalculationJob(0,totallyPreCompItem->getSaturationData());


				bool imiadelyProcessAllConcepts = totallyPreCompItem->requiresAllConceptsSaturation();
				if (!allowAllSaturation) {
					imiadelyProcessAllConcepts = false;
				}

				cint64 saturationConceptCount = 0;
				QList<CSaturationConceptDataItem*>* orderedItemList = totallyPreCompItem->getSaturationOrderedItemList();
				for (QList<CSaturationConceptDataItem*>::const_iterator it = orderedItemList->constBegin(), itEnd = orderedItemList->constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conItem = *it;
					CConcept* concept = conItem->getSaturationConcept();
					bool negation = conItem->getSaturationNegation();
					CRole* role = conItem->getSaturationRoleRanges();
					if (conItem->isItemProcessingMarked() && !conItem->isItemProcessingQueued() || imiadelyProcessAllConcepts) {
						conItem->setItemProcessingQueued(true);
						satCalculationJob = satCalculationJobGenerator.extendApproximatedSaturationCalculationJobProcessing(conItem,satCalculationJob);
						saturationConceptCount++;
					}
				}

				LOG(INFO,getLogDomain(),logTr("Ordered %1 concepts for saturation for ontology '%2'.").arg(saturationConceptCount).arg(totallyPreCompItem->getOntology()->getTerminologyName()),this);

				CSaturationPrecomputationTestingItem* satTestingItem = new CSaturationPrecomputationTestingItem(totallyPreCompItem);
				CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
				satCalculationJob->setCalculationConfiguration(calcConfig);
				satCalculationJob->setSaturationTaskPreyingAdapter(new CTaskPreyingAdapter((CSaturationObserver*)totallyPreCompItem));

				totallyPreCompItem->setSaturationCalculationJob(satCalculationJob);
				processCalculationJob(satCalculationJob,totallyPreCompItem,satTestingItem);
			}





			void CTotallyPrecomputationThread::createSaturationConstructionJob(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {

				CTBox *tBox = totallyPreCompItem->getOntology()->getDataBoxes()->getTBox();

				CConcreteOntology *onto = totallyPreCompItem->getOntology();
				CConcept *topConcept = onto->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = onto->getDataBoxes()->getBottomConcept();

				CConceptVector* conVec = tBox->getConceptVector();

				LOG(INFO,getLogDomain(),logTr("Determine concepts for saturation for ontology '%1'.").arg(totallyPreCompItem->getOntology()->getTerminologyName()),this);


				CApproximatedSaturationCalculationJob* satCalculationJob = nullptr;
				CApproximatedSaturationCalculationJobGenerator satCalculationJobGenerator(onto);




				cint64 statExistsConceptCount = 0;
				cint64 statClassCount = 0;

				QList<CSaturationConceptDataItem*> existConList;
				QList<CSaturationConceptDataItem*> analyseExistSubsumerList;
				QList<CSaturationConceptDataItem*> allConItemList;
				QList<CSaturationConceptDataItem*> disjunctionCandidateAlternativeList;

				cint64 conCount = conVec->getItemCount();
				for (cint64 conIdx = 1; conIdx < conCount; ++conIdx) {
					CConcept* concept = conVec->getData(conIdx);

					//if (CIRIName::getRecentIRIName(concept->getClassNameLinker()) == "file:F:/Projects/OIL/DAMLOilEd/ontologies/ka.daml#Publication") {
					//	bool bug = true;
					//}

					if (concept) {
						cint64 opCode = concept->getOperatorCode();
						if (opCode == CCSOME || opCode == CCALL || opCode == CCAQSOME || opCode == CCATLEAST || opCode == CCATMOST) {
							++statExistsConceptCount;
							bool negation = opCode == CCALL;
							CRole* role = concept->getRole();
							CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
							CConcept* opConcept = nullptr;
							bool opNegation = false;
							if (opLinker) {
								opConcept = opLinker->getData();
								if (opCode == CCATLEAST || opCode == CCATMOST) {
									opNegation = opLinker->isNegated();
								} else {
									opNegation = opLinker->isNegated()^negation;
								}
							} else {
								opConcept = topConcept;
								if (opCode == CCATLEAST || opCode == CCATMOST) {
									opNegation = false;
								} else {
									opNegation = negation;
								}
							}


							if (hasRoleRanges(totallyPreCompItem,role)) {
								CSaturationConceptDataItem* conItem = nullptr;
								conItem = totallyPreCompItem->getSaturationRoleSuccessorConceptDataItem(role,opConcept,opNegation,true);
								conItem->setPotentiallyExistInitializationConcept(true);
								existConList.append(conItem);

								CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
								CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
								if (!conRefSatLinking) {
									conRefSatLinking = new CConceptSaturationReferenceLinkingData();
									conProcData->setConceptReferenceLinking(conRefSatLinking);
								}
								if (conRefSatLinking->getExistentialSuccessorConceptSaturationReferenceLinkingData() == nullptr) {
									conRefSatLinking->setExistentialSuccessorConceptSaturationReferenceLinkingData(conItem);
									allConItemList.append(conItem);
								}

							} else {
								CSaturationConceptDataItem* conItem = nullptr;
								conItem = totallyPreCompItem->getSaturationConceptDataItem(opConcept,opNegation,true);
								conItem->setPotentiallyExistInitializationConcept(true);
								existConList.append(conItem);

								CConceptProcessData* opConProcData = (CConceptProcessData*)opConcept->getConceptData();
								CConceptSaturationReferenceLinkingData* opConRefSatLinking = (CConceptSaturationReferenceLinkingData*)opConProcData->getConceptReferenceLinking();
								if (!opConRefSatLinking) {
									opConRefSatLinking = new CConceptSaturationReferenceLinkingData();
									opConProcData->setConceptReferenceLinking(opConRefSatLinking);
								}
								if (opConRefSatLinking->getConceptSaturationReferenceLinkingData(opNegation) == nullptr) {
									opConRefSatLinking->setSaturationReferenceLinkingData(conItem,opNegation);
									allConItemList.append(conItem);
								}
							}

						} else if (concept->hasClassName()) {

							++statClassCount;
							CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
							CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
							if (!conRefSatLinking) {
								conRefSatLinking = new CConceptSaturationReferenceLinkingData();
								conProcData->setConceptReferenceLinking(conRefSatLinking);
							}
							if (conRefSatLinking->getConceptSaturationReferenceLinkingData(false) == nullptr) {
								CSaturationConceptDataItem* conItem = totallyPreCompItem->getSaturationConceptDataItem(concept,false,true);
								conRefSatLinking->setSaturationReferenceLinkingData(conItem,false);
								allConItemList.append(conItem);
							}
						}
					}
				}

				disjunctionCandidateAlternativeList = allConItemList;
				while (!disjunctionCandidateAlternativeList.isEmpty()) {
					CSaturationConceptDataItem* conTestItem = disjunctionCandidateAlternativeList.takeFirst();
					QList<CSaturationConceptDataItem*> tempNewItems;
					extendDisjunctionsCandidateAlternativesItems(totallyPreCompItem,conTestItem,&tempNewItems);
					if (!tempNewItems.isEmpty()) {
						disjunctionCandidateAlternativeList.append(tempNewItems);
						allConItemList.append(tempNewItems);
					}
				}


				analyseExistSubsumerList = allConItemList;

				QList<CConceptNegationSaturationItem> processList;
				// identify all items, which aren't subsumers of other items
				for (QList<CSaturationConceptDataItem*>::const_iterator it = analyseExistSubsumerList.constBegin(), itEnd = analyseExistSubsumerList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					CConcept* concept = conTestItem->getSaturationConcept();
					bool conceptNegation = conTestItem->getSaturationNegation();
					processList.append(CConceptNegationSaturationItem(concept,conceptNegation,conTestItem));
				}
				analyseConceptSaturationSubsumerExistItems(totallyPreCompItem,processList);

				// find all potentially subsumer items
				for (QList<CSaturationConceptDataItem*>::const_iterator it = analyseExistSubsumerList.constBegin(), itEnd = analyseExistSubsumerList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					propagateSubsumerItemFlag(conTestItem);
				}

				// find all potentially role successors, for which back propagation is necessary
				for (QList<CSaturationConceptDataItem*>::const_iterator it = existConList.constBegin(), itEnd = existConList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					propagateExistInitializationFlag(conTestItem);
				}

				QList<CSaturationConceptDataItem*> leafConList;
				QList<CSaturationConceptDataItem*> leafExistConList;
				// collect all leaf nodes
				for (QList<CSaturationConceptDataItem*>::const_iterator it = allConItemList.constBegin(), itEnd = allConItemList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					if (!conTestItem->hasIndirectSuccessorsItems()) {
						if (!conTestItem->isPotentiallyExistInitializationConcept()) {
							leafConList.append(conTestItem);
						} else {
							leafExistConList.append(conTestItem);
						}
					}
				}



				QList<CSaturationConceptDataItem*>* orderedItemList = totallyPreCompItem->getSaturationOrderedItemList();
				QStack<CSaturationSaturationListItem> saturationItemStack;
				// order all concepts associated to the items for efficient Saturation testing, start with the real leaf items
				for (QList<CSaturationConceptDataItem*>::const_iterator it = leafConList.constBegin(), itEnd = leafConList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					if (!conTestItem->isOrderingQueued()) {
						orderItemsSaturationTesting(totallyPreCompItem,conTestItem,*orderedItemList,saturationItemStack);
					}
				}
				for (QList<CSaturationConceptDataItem*>::const_iterator it = leafExistConList.constBegin(), itEnd = leafExistConList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					if (!conTestItem->isOrderingQueued()) {
						orderItemsSaturationTesting(totallyPreCompItem,conTestItem,*orderedItemList,saturationItemStack);
					}
				}
				for (QList<CSaturationConceptDataItem*>::const_iterator it = allConItemList.constBegin(), itEnd = allConItemList.constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conTestItem = *it;
					if (!conTestItem->isOrderingQueued()) {
						orderItemsSaturationTesting(totallyPreCompItem,conTestItem,*orderedItemList,saturationItemStack);
					}
				}


				// set reference mode for saturation: substitute or copy 
				QListIterator<CSaturationConceptDataItem*> reverseOrderedIt(*orderedItemList);
				reverseOrderedIt.toBack();
				CBOXHASH<CConcept*,CConcept*>* triggerImpHash = totallyPreCompItem->getOntology()->getTBox()->getTriggerImplicationHash(false);
				while (reverseOrderedIt.hasPrevious()) {
					CSaturationConceptDataItem* conTestItem = reverseOrderedIt.previous();
					CSaturationConceptDataItem* referencedTestItem = conTestItem->getSpecialItemReference();
					if (referencedTestItem) {
						if (!conTestItem->isPotentiallyExistInitializationConcept() && !conTestItem->hasMultipleDirectPredecessorsItems() && (!triggerImpHash || !triggerImpHash->contains(conTestItem->getSaturationConcept()))) {
							conTestItem->setSpecialItemReferenceMode(CSaturationConceptDataItem::SATURATIONSUBSTITUTEMODE);
						} else {
							conTestItem->setSpecialItemReferenceMode(CSaturationConceptDataItem::SATURATIONCOPYMODE);
						}
					}
				}

				satCalculationJob = satCalculationJobGenerator.getApproximatedSaturationCalculationJob(0);


				cint64 mSaturationConceptCount = 0;
				for (QList<CSaturationConceptDataItem*>::const_iterator it = orderedItemList->constBegin(), itEnd = orderedItemList->constEnd(); it != itEnd; ++it) {
					CSaturationConceptDataItem* conItem = *it;
					CConcept* concept = conItem->getSaturationConcept();
					bool negation = conItem->getSaturationNegation();
					CRole* role = conItem->getSaturationRoleRanges();
					conItem->setItemProcessingQueued(false);
					satCalculationJob = satCalculationJobGenerator.extendApproximatedSaturationCalculationJobConstruction(role,concept,negation,false,conItem,satCalculationJob);
					mSaturationConceptCount++;
				}

				LOG(INFO,getLogDomain(),logTr("Preparing saturation of %1 concepts for ontology '%2'.").arg(mSaturationConceptCount).arg(totallyPreCompItem->getOntology()->getTerminologyName()),this);

				CSaturationPrecomputationTestingItem* satTestingItem = new CSaturationPrecomputationTestingItem(totallyPreCompItem);
				CCalculationConfigurationExtension* calcConfig = totallyPreCompItem->getCalculationConfiguration();
				satCalculationJob->setCalculationConfiguration(calcConfig);
				satCalculationJob->setSaturationTaskPreyingAdapter(new CTaskPreyingAdapter((CSaturationObserver*)totallyPreCompItem));

				totallyPreCompItem->setSaturationCalculationJob(satCalculationJob);
				processCalculationJob(satCalculationJob,totallyPreCompItem,satTestingItem);
			}
















			bool CTotallyPrecomputationThread::precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CSaturationPrecomputationCalculatedCallbackEvent* pcce) {
				CTotallyOntologyPrecomputationItem* totallyPreCompItem = (CTotallyOntologyPrecomputationItem*)ontPreCompItem;
				if (preTestItem->getPrecomputationTestingType() == CPrecomputationTestingItem::CONCEPTSATURATIONPRECOMPUTATIONTYPE) {
					CSaturationPrecomputationTestingItem* satPreTestItem = (CSaturationPrecomputationTestingItem*)preTestItem;
					totallyPreCompItem->setConceptSaturationPrecomputationChecked(true);
					CPrecomputation* precomputation = totallyPreCompItem->getPrecomputation();
					CSaturationData* saturationData = totallyPreCompItem->getSaturationData();
					precomputation->setSaturationModelData(saturationData);
					totallyPreCompItem->setSaturationComputationRunning(false);

					CSaturationTaskData* satTaskData = (CSaturationTaskData*)saturationData;
					if (satTaskData) {
						CSatisfiableCalculationTask* satCalcTask = satTaskData->getSaturationTask();
						if (satCalcTask) {
							CProcessingDataBox* procDataBox = satCalcTask->getProcessingDataBox();
							if (procDataBox) {
								totallyPreCompItem->setNominalDelayedConceptsSaturationUpdateRequired(procDataBox->isDelayedNominalProcessingOccured());
							}
						}
					}

					return true;
				}
				return false;
			}






			bool CTotallyPrecomputationThread::precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CPrecomputationCalculatedCallbackEvent* pcce) {
				CTotallyOntologyPrecomputationItem* totallyPreCompItem = (CTotallyOntologyPrecomputationItem*)ontPreCompItem;
				if (preTestItem->getPrecomputationTestingType() == CPrecomputationTestingItem::CONSISTENCEPRECOMPUTATIONTYPE) {
					CConsistencePrecomputationTestingItem* consPreTestItem = (CConsistencePrecomputationTestingItem*)preTestItem;

					if (pcce->hasCalculationError()) {
						LOG(ERROR,getLogDomain(),logTr("Error in computation, consistence testing for ontology '%1' failed.").arg(ontPreCompItem->getOntology()->getTerminologyName()),getLogObject());
						totallyPreCompItem->setConsistenceCheched(true);
						totallyPreCompItem->getConsistencePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
					} else {
						totallyPreCompItem->setConsistenceCheched(true);
						CConsistence* consistence = totallyPreCompItem->getConsistence();
						consistence->setConsistenceModelData(totallyPreCompItem->getConsistenceData());
						consistence->setOntologyConsistent(pcce->getTestResultSatisfiable());
					}
					return true;
				} else if (preTestItem->getPrecomputationTestingType() == CPrecomputationTestingItem::CONCEPTCYCLEPRECOMPUTATIONTYPE) {
					CConceptCyclePrecomputationTestingItem* conCyclePreTestItem = (CConceptCyclePrecomputationTestingItem*)preTestItem;
					CConceptCycleData* conCycleData = conCyclePreTestItem->getConceptCycleData();
					CBOXSET<CConcept*>* classCycleSet = conCycleData->getCycleClassSet();

					if (pcce->hasCalculationError()) {
						LOG(ERROR,getLogDomain(),logTr("Error in computation, cyclic concept graph caching for ontology '%1' failed.").arg(ontPreCompItem->getOntology()->getTerminologyName()),getLogObject());
						totallyPreCompItem->setConsistenceCheched(true);
						totallyPreCompItem->getCyclePrecomputationStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
					} else {
						if (pcce->getTestResultSatisfiable()) {
							// insert into cache
							CTempMemoryPoolContainerAllocationManager tmpMemMan(mContext.getMemoryPoolProvider());
							CContextBase* tmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(&tmpMemMan,&tmpMemMan);

							CCACHINGLIST<CCacheValue>* cacheValueList = CObjectParameterizingAllocator< CCACHINGLIST<CCacheValue>,CContext* >::allocateAndConstructAndParameterize(&tmpMemMan,tmpContext);
							for (CBOXSET<CConcept*>::const_iterator it = classCycleSet->constBegin(), itEnd = classCycleSet->constEnd(); it != itEnd; ++it) {
								CConcept* classConcept(*it);
								CCacheValue cacheValue(classConcept->getConceptTag(),(cint64)classConcept,CCacheValue::CACHEVALTAGANDCONCEPT);
								cacheValueList->append(cacheValue);
							}

							CReuseCompletionGraphCacheEntryExpandWriteData* writeData = CObjectAllocator<CReuseCompletionGraphCacheEntryExpandWriteData>::allocateAndConstruct(&tmpMemMan);
							writeData->initExpandWriteData(cacheValueList,cacheValueList,conCyclePreTestItem->getJobInstantiation());
							mReuseCompletionGraphCacheWriter->writeExpandCache(writeData,tmpMemMan.takeMemoryPools());

						}
					}

					if (!totallyPreCompItem->hasRemainingConceptCyclePrecomputation() && !totallyPreCompItem->hasCurrentPrecomputationTesting()) {
						totallyPreCompItem->setConceptCyclePrecomputationChecked(true);
					}

				}
				return false;
			}


			bool CTotallyPrecomputationThread::finishOntologyPrecomputation(CTotallyOntologyPrecomputationItem* totallyPreCompItem) {
				if (totallyPreCompItem->areAllStepFinished()) {
					totallyPreCompItem->setPrecomputationFinished(true);
					CConcreteOntology* ontology = totallyPreCompItem->getOntology();

					CPrecomputation* precomputation = ontology->getPrecomputation();
					precomputation->setPrecomputed(true);
				}

				totallyPreCompItem->doPrecomputationFinishedCallback(true);

				mActiveOntItemSet.remove(totallyPreCompItem);
				mInactiveOntItemSet.insert(totallyPreCompItem);
				return true;
			}


		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
