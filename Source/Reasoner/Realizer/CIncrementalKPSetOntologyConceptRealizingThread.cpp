/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CIncrementalKPSetOntologyConceptRealizingThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CIncrementalKPSetOntologyConceptRealizingThread::CIncrementalKPSetOntologyConceptRealizingThread(CReasonerManager *reasonerManager) : COptimizedRepresentativeKPSetOntologyRealizingThread(reasonerManager) {
			}

			CIncrementalKPSetOntologyConceptRealizingThread::~CIncrementalKPSetOntologyConceptRealizingThread() {
			}


			COntologyRealizingItem* CIncrementalKPSetOntologyConceptRealizingThread::initializeOntologyRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				return COptimizedRepresentativeKPSetOntologyRealizingThread::initializeOntologyRealizingItem(ontology,config);
			}

			void CIncrementalKPSetOntologyConceptRealizingThread::readCalculationConfig(CConfigurationBase *config) {
				COptimizedRepresentativeKPSetOntologyRealizingThread::readCalculationConfig(config);
			}





			bool CIncrementalKPSetOntologyConceptRealizingThread::isIndividualAffectedByIncrementalChanges(CIndividual* individual, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				CConcreteOntology* ontology = reqConfPreCompItem->getOntology();

				cint64 indiID = individual->getIndividualID();

				CConcreteOntology* prevRealizedOntology = ontology->getIncrementalRevisionData()->getPreviousClassTypesRealizedOntology();
				CConcreteOntology* prevConsOntology = ontology;
				do {
					CConsistence* prevCons = prevConsOntology->getConsistence();
					CIncrementalConsistenceTaskData* incConsTaskData = dynamic_cast<CIncrementalConsistenceTaskData*>(prevCons->getConsistenceModelData());
					if (incConsTaskData) {
						if (incConsTaskData->getIndirectlyChangedNodeSet()->contains(indiID)) {
							return true;
						}
					} else {
						return true;
					}
					if (prevConsOntology != prevRealizedOntology) {
						prevConsOntology = prevConsOntology->getIncrementalRevisionData()->getPreviousConsistentOntology();
					}
				} while (prevConsOntology && prevConsOntology != prevRealizedOntology);
				return false;
			}





			bool CIncrementalKPSetOntologyConceptRealizingThread::isIndividualItemAffectedByIncrementalChanges(COptimizedKPSetIndividualItem* indiItem, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				CIndividualDependenceTrackingCollector* indDepTrackingCollector = indiItem->getIndividualDependenceTrackingCollector();
				if (indDepTrackingCollector) {
					CIndividualDependenceTracking* indiDepTracking = indDepTrackingCollector->getExtendingIndividualDependenceTracking();

					CConcreteOntology* ontology = reqConfPreCompItem->getOntology();
					CConcreteOntology* prevRealizedOntology = ontology->getIncrementalRevisionData()->getPreviousClassTypesRealizedOntology();
					CConcreteOntology* prevConsOntology = ontology;
					do {
						CConsistence* prevCons = prevConsOntology->getConsistence();
						CIncrementalConsistenceTaskData* incConsTaskData = dynamic_cast<CIncrementalConsistenceTaskData*>(prevCons->getConsistenceModelData());
						if (incConsTaskData) {
							if (indiDepTracking && indiDepTracking->areIndividualsAffected(incConsTaskData)) {
								return true;
							}
						} else {
							return true;
						}
						if (prevConsOntology != prevRealizedOntology) {
							prevConsOntology = prevConsOntology->getIncrementalRevisionData()->getPreviousConsistentOntology();
						}
					} while (prevConsOntology && prevConsOntology != prevRealizedOntology);

				}
				return false;
			}




			bool CIncrementalKPSetOntologyConceptRealizingThread::initializeIndividualProcessingKPSetsFromConsistencyData(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {


				/*CConcreteOntology* ontology = reqConfPreCompItem->getOntology();
				CConcreteOntology* prevRealizedOntology = ontology->getIncrementalRevisionData()->getPreviousClassTypesRealizedOntology();
				CRealization* prevRealization = prevRealizedOntology->getRealization();

				COptimizedRepresentativeKPSetOntologyRealizingItem* prevConRealItem = (COptimizedRepresentativeKPSetOntologyRealizingItem*)prevRealization->getConceptRealization();
				


				CConcreteOntology* prevConsOntology = ontology->getIncrementalRevisionData()->getPreviousConsistentOntology();


				cint64 previousReusedIndiCalculationsCount = 0;
				cint64 newInitializedIndiCalculationsCount = 0;

				CIndividualVector* indiVec = ontology->getABox()->getIndividualVector(false);
				if (indiVec) {
					CIndividualProcessNodeVector* indiProcVector = nullptr;
					CConsistence* consistence = reqConfPreCompItem->getOntology()->getConsistence();
					if (consistence) {
						CConsistenceData* consData = consistence->getConsistenceModelData();
						if (consData) {
							CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
							CSatisfiableCalculationTask* satConsTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
							if (!satConsTask) {
								satConsTask = consTaskData->getDeterministicSatisfiableTask();
							}
							if (satConsTask) {
								CProcessingDataBox* procDataBox = satConsTask->getProcessingDataBox();
								if (procDataBox) {
									indiProcVector = procDataBox->getIndividualProcessNodeVector();
								}
							}
						}
					}
					CIndividualVector* prevIndiVec = prevRealizedOntology->getABox()->getIndividualVector(false);


					QList<COptimizedKPSetConceptInstancesItem*> equivClassList;
					initializeEquivalentClassList(&equivClassList,reqConfPreCompItem);

					QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* conInstItemHash = reqConfPreCompItem->getConceptInstancesItemHash();

					QHash<CIndividual*,QList<CIndividual*> > individualPossibleSameIndividualListHash;

					CBOXSET<CIndividual*>* activeIndividualSet = reqConfPreCompItem->getOntology()->getABox()->getActiveIndividualSet(false);

					cint64 indiCount = indiVec->getItemCount();
					for (cint64 indiID = 0; indiID < indiCount; ++indiID) {

						CIndividual* individual = indiVec->getData(indiID);
						if (individual && !individual->isAnonymousIndividual() && !reqConfPreCompItem->hasIndividualInstantiatedItem(individual) && activeIndividualSet->contains(individual)) {


							QList<COptimizedKPSetConceptInstancesItem*> knownInstancesList;
							QList<COptimizedKPSetConceptInstancesItem*> possibleInstancesList;

							QList<CIndividual*> knownSameIndividualList;
							QList<CIndividual*> possibleSameIndividualList;

							COptimizedKPSetIndividualItem* prevIndiItem = prevConRealItem->getIndividualInstantiatedItem(individual,false);
							if (!prevIndiItem) {
								CIndividual* prevIndi = prevIndiVec->getData(indiID);
								prevIndiItem = prevConRealItem->getIndividualInstantiatedItem(prevIndi,false);
							}

							bool newRealisationRequired = false;
							if (!prevIndiItem) {
								newRealisationRequired = true;
							} else {
								if (isIndividualAffectedByIncrementalChanges(individual,reqConfPreCompItem)) {
									newRealisationRequired = true;
								} else {
									if (isIndividualItemAffectedByIncrementalChanges(prevIndiItem,reqConfPreCompItem)) {
										newRealisationRequired = true;
									}
								}
							}

							if (newRealisationRequired) {
								++newInitializedIndiCalculationsCount;
								extractKnownPossibleIndividualDataFromConsistencyData(individual,&knownInstancesList,&possibleInstancesList,&knownSameIndividualList,&possibleSameIndividualList,indiProcVector,equivClassList,reqConfPreCompItem);
							} else {
								++previousReusedIndiCalculationsCount;
								QSet<CIndividual*>* prevKnownSameIndiSet = prevIndiItem->getKnownSameIndividualSet();								
								for (QSet<CIndividual*>::const_iterator it = prevKnownSameIndiSet->constBegin(), itEnd = prevKnownSameIndiSet->constEnd(); it != itEnd; ++it) {
									CIndividual* prevKnownSameIndi = *it;
									knownSameIndividualList.append(prevKnownSameIndi);
								}
								COptimizedKPSetConceptInstancesHash* prevKnownPossConInstHash = prevIndiItem->getKnownPossibleInstancesHash();								
								for (COptimizedKPSetConceptInstancesHash::const_iterator it = prevKnownPossConInstHash->constBegin(), itEnd = prevKnownPossConInstHash->constEnd(); it != itEnd; ++it) {
									COptimizedKPSetConceptInstancesItem* instItem = it.key();
									const COptimizedKPSetConceptInstancesHashData& instItemData = it.value();
									if (instItemData.mInstanceItemData->mKnownInstance) {
										COptimizedKPSetConceptInstancesItem* instantiatedItem = conInstItemHash->value(instItem->getHierarchyNode()->getOneEquivalentConcept());
										knownInstancesList.append(instantiatedItem);
									}
								}
							}

							initializeKPSetsForIndividual(reqConfPreCompItem,individual,knownInstancesList,possibleInstancesList,knownSameIndividualList);

							if (!possibleSameIndividualList.isEmpty()) {
								individualPossibleSameIndividualListHash.insert(individual,possibleSameIndividualList);
							}
						}
					}

					if (!individualPossibleSameIndividualListHash.isEmpty()) {
						QSet<COptimizedKPSetIndividualItem*> singleIndividualSet;
						for (QHash<CIndividual*,QList<CIndividual*> >::const_iterator it = individualPossibleSameIndividualListHash.constBegin(), itEnd = individualPossibleSameIndividualListHash.constEnd(); it != itEnd; ++it) {
							CIndividual* individual = it.key();
							COptimizedKPSetIndividualItem* individualItem = reqConfPreCompItem->getIndividualInstantiatedItem(individual,false);
							if (!singleIndividualSet.contains(individualItem)) {
								singleIndividualSet.insert(individualItem);
								const QList<CIndividual*> possibleSameIndividualList = it.value();
								initializeSamePossibleIndividuals(reqConfPreCompItem,individualItem,possibleSameIndividualList);
							}
						}
					}

				}


				COntologyProcessingStatistics* ontProcStats = reqConfPreCompItem->getRealizeSameIndividualsProcessingStep()->getProcessingStepData()->getProcessingStatistics(true);
				ontProcStats->setProcessingCountStatisticValue("incremental-realization-previously-reused-individual-calculations-count",previousReusedIndiCalculationsCount);
				ontProcStats->setProcessingCountStatisticValue("incremental-realization-newly-initialized-individual-calculations-count",newInitializedIndiCalculationsCount);
*/

				return true;
			}



			COptimizedRepresentativeKPSetOntologyRealizingThread* CIncrementalKPSetOntologyConceptRealizingThread::addRealizationStatistics(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CRealizingTestingStep* ontProcStep) {
				COntologyProcessingStatistics* ontProcStats = ontProcStep->getProcessingStepData()->getProcessingStatistics(true);
				ontProcStats->setProcessingCountStatisticValue("incremental-realization-time",reqConfPreCompItem->getInitializationTime()->elapsed());
				if (reqConfPreCompItem->getRealizeConceptProcessingStep() == ontProcStep) {
					ontProcStats->setProcessingCountStatisticValue("incremental-realization-concept-instances-possible-testing-count",reqConfPreCompItem->getTestedPossibleConceptInstancesCount());
					ontProcStats->setProcessingCountStatisticValue("incremental-realization-concept-instances-time",reqConfPreCompItem->getInitializationTime()->elapsed());
					addIndividualDependencyTrackingStatistics(reqConfPreCompItem,ontProcStep,"incremental-realization-concept-instances");
				} else if (reqConfPreCompItem->getRealizeSameIndividualsProcessingStep() == ontProcStep) {
					ontProcStats->setProcessingCountStatisticValue("incremental-realization-same-individuals-possible-testing-count",reqConfPreCompItem->getTestedPossibleSameIndividualsCount());
					addIndividualDependencyTrackingStatistics(reqConfPreCompItem,ontProcStep,"incremental-realization-same-individuals");
				}
				return this;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
