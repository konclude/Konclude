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

#include "CSatisfiableTaskMultiSubsumptionIdentifierAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CSatisfiableTaskMultiSubsumptionIdentifierAnalyser::CSatisfiableTaskMultiSubsumptionIdentifierAnalyser() {
					mConfAnalyseSubSumptions = true;
					mConfAnalyseAllSubSumptions = false;
					mConfAnalyseIdentifierOccurence = true;
					mConfAnalysePossibleSubSumptions = true;
				}



				bool CSatisfiableTaskMultiSubsumptionIdentifierAnalyser::readCalculationConfig(CCalculationConfigurationExtension *config) {					
					mConfAnalyseSubSumptions = true;
					mConfAnalyseAllSubSumptions = false;
					mConfAnalyseIdentifierOccurence = true;
					mConfAnalysePossibleSubSumptions = true;
					if (config) {
						return true;
					}
					return false;
				}


				CIndividualProcessNode* CSatisfiableTaskMultiSubsumptionIdentifierAnalyser::getCorrectedIndividualID(CIndividualProcessNode* baseIndiNode, CIndividualProcessNodeVector* indiNodeVec) {
					CIndividualProcessNode* indi = indiNodeVec->getData(baseIndiNode->getIndividualID());
					while (indi->hasMergedIntoIndividualNodeID()) {
						indi = indiNodeVec->getData(indi->getMergedIntoIndividualNodeID());
					}
					return indi;
				}


				bool CSatisfiableTaskMultiSubsumptionIdentifierAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					CSatisfiableSubsumptionIdentifierAdapter* satSubIdAdapter = statCalcTask->getSatisfiableSubsumptionIdentifierAdapter();
					if (satSubIdAdapter) {
						CConceptNecessaryIdentifierOccurrenceObserver *necIdenOccObserver = satSubIdAdapter->getNecessaryIdentifierOccurrenceObserver();
						CConceptPossibleSubsumptionsOccurrenceObserver* possSubsumOccObserver = satSubIdAdapter->getPossibleSubsumptionsOccurrenceObserver();
						CConceptSubsumptionRelationObserver *subsumptionObs = satSubIdAdapter->getConceptSubsumptionObserver();
						CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
						CConcept *subsumerConcept = satSubIdAdapter->getSatisfiableTestingConcept();
						CConcreteOntology *ontology = satSubIdAdapter->getTestingOntology();
						CProcessingDataBox* processingDataBox = statCalcTask->getProcessingDataBox();
						CIndividualProcessNode* constructedIndiNode = processingDataBox->getConstructedIndividualNode();
						CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();
						if (ontology && subsumerConcept && constructedIndiNode) {
							cint64 constructedID = constructedIndiNode->getIndividualID();
							CIndividualProcessNode* baseIndi = getCorrectedIndividualID(constructedIndiNode,indiNodeVec);
							cint64 baseIndiID = baseIndi->getIndividualID();


							CPROCESSINGSET<QPair<CConcept *,CConcept *> > subsumingSet(taskProcessorContext);
							CPROCESSINGSET<QPair<CConcept *,CConcept *> > disjointSet(taskProcessorContext);
							CPROCESSINGSET<CConceptNegationPair> necIdenOccurSet(taskProcessorContext);
							CPROCESSINGSET<CConcept*> possSubsumSet(taskProcessorContext);


							cint64 maxIndiID = indiNodeVec->getItemCount();
							for (cint64 indiID = baseIndiID; indiID < maxIndiID; ++indiID) {
								CIndividualProcessNode* indiNode = indiNodeVec->getData(indiID);
								if (indiNode) {
									CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
									if (conSet) {
										CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(false,true,false);
										while (conSetIt.hasNext()) {
											CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
											CConcept* concept = conDes->getConcept();
											bool negated = conDes->getNegation();

											if (concept->hasClassName() && concept->getConceptTag() != 1) {

												CDependencyTrackPoint* depTrackPoint = conSetIt.getDependencyTrackPoint();
												bool deterministic = false;
												if (depTrackPoint) {
													if (depTrackPoint->getBranchingTag() <= 0) {
														deterministic = true;
													}
												}

												if (deterministic) {
													if (mConfAnalyseIdentifierOccurence && necIdenOccObserver) {
														if (!negated) {
															necIdenOccurSet.insert(CConceptNegationPair(concept,negated));
														}
													}

													if (mConfAnalyseSubSumptions && subsumptionObs) {
														if (subsumerConcept != concept) {
															if (indiID == baseIndiID) {
																if (!negated) {
																	subsumingSet.insert(QPair<CConcept *,CConcept *>(subsumerConcept,concept));
																} else {
																	disjointSet.insert(QPair<CConcept *,CConcept *>(subsumerConcept,concept));
																}
															}
														}
													}
													if (mConfAnalyseAllSubSumptions && subsumptionObs) {
														if (subsumerConcept != concept) {
															CDependencyTrackPoint* prevDepTrackPoint = depTrackPoint;
															CConceptDescriptor* prevConDes = conDes;
															while (prevDepTrackPoint && prevConDes) {
																CDependencyNode* depNode = prevDepTrackPoint->getDependencyNode();
																if (depNode->getDependencyType() == CDependencyNode::DNTANDDEPENDENCY) {
																	CDependencyTrackPoint* nextDepTrackPoint = depNode->getPreviousDependencyTrackPoint();
																	CConceptDescriptor* nextConDes = depNode->getConceptDescriptor();
																	if (nextConDes) {
																		CConcept *prevConcept = nextConDes->getConcept();
																		bool prevNegated = nextConDes->isNegated();
																		// prevConcept has deterministically generated concept	
																		if (!prevNegated && !negated) {
																			// prevConcept is subsuming concept
																			subsumingSet.insert(QPair<CConcept*,CConcept*>(prevConcept,concept));
																		} else if (prevNegated != negated) {
																			// prevConcept is disjoint concept
																			disjointSet.insert(QPair<CConcept*,CConcept*>(prevConcept,concept));
																		}
																	}
																	prevDepTrackPoint = nextDepTrackPoint;
																	prevConDes = nextConDes;
																} else {
																	break;
																}
															}
														}
													}
												} else {
													// non deterministic
													if (mConfAnalysePossibleSubSumptions && possSubsumOccObserver) {
														if (subsumerConcept != concept) {
															if (indiID == baseIndiID) {
																if (!negated) {
																	possSubsumSet.insert(concept);
																}
															}
														}
													}
												}
											}
											conSetIt.moveNext();
										}
									}
								}
								if (!mConfAnalyseAllSubSumptions || !mConfAnalyseIdentifierOccurence) {
									break;
								}
								if (indiID < constructedID) {
									indiID = constructedID;
								}
							}


							if ((mConfAnalyseAllSubSumptions || mConfAnalyseSubSumptions) && subsumptionObs) {
								QList<QPair<CConcept *,CConcept *> > subsumerList;
								QList<QPair<CConcept *,CConcept *> > disjointList;

								for (CPROCESSINGSET<QPair<CConcept *,CConcept *> >::const_iterator it = subsumingSet.constBegin(), itEnd = subsumingSet.constEnd(); it != itEnd; ++it) {
									CConcept *subsumerConcept(it->first);
									CConcept *subsumedConcept(it->second);
									subsumerList.append(QPair<CConcept *,CConcept *>(subsumerConcept,subsumedConcept));
								}

								for (CPROCESSINGSET<QPair<CConcept *,CConcept *> >::const_iterator it = disjointSet.constBegin(), itEnd = disjointSet.constEnd(); it != itEnd; ++it) {
									CConcept *disjointConcept1(it->first);
									CConcept *disjointConcept2(it->second);
									disjointList.append(QPair<CConcept *,CConcept *>(disjointConcept1,disjointConcept2));
								}

								if (!subsumerList.isEmpty()) {
									subsumptionObs->tellConceptSupsumptionRelation(ontology,subsumerList,true);
								}
								if (!disjointList.isEmpty()) {
									subsumptionObs->tellConceptDisjointRelation(ontology,disjointList);
								}
							}

							if (mConfAnalyseIdentifierOccurence && necIdenOccObserver) {
								QList<CConceptNegationPair> necIdenOccurList;

								for (CPROCESSINGSET<CConceptNegationPair>::const_iterator it = necIdenOccurSet.constBegin(), itEnd = necIdenOccurSet.constEnd(); it != itEnd; ++it) {
									necIdenOccurList.append(CConceptNegationPair(*it));
								}

								if (!necIdenOccurList.isEmpty()) {
									necIdenOccObserver->tellConceptNecessaryIdentifierOccurrence(ontology,subsumerConcept,necIdenOccurList);
								}
							}

							if (mConfAnalysePossibleSubSumptions && possSubsumOccObserver) {
								QList<CConcept*> possSubsumList;

								for (CPROCESSINGSET<CConcept*>::const_iterator it = possSubsumSet.constBegin(), itEnd = possSubsumSet.constEnd(); it != itEnd; ++it) {
									possSubsumList.append(*it);
								}

								if (!possSubsumList.isEmpty()) {
									possSubsumOccObserver->tellConceptPossibleSubsumptionsOccurrence(ontology,subsumerConcept,possSubsumList);
								}
							}

							return true;

						}
					}
					return false;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
