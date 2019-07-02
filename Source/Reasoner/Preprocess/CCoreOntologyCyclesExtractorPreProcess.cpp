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

#include "CCoreOntologyCyclesExtractorPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CCoreOntologyCyclesExtractorPreProcess::CCoreOntologyCyclesExtractorPreProcess() {
			}


			CCoreOntologyCyclesExtractorPreProcess::~CCoreOntologyCyclesExtractorPreProcess() {
			}


			bool CCoreOntologyCyclesExtractorPreProcess::addConceptsToList(CSortedNegLinker<CConcept*>* opLinker, bool negated, QList<TConceptNegPair>& nextTestingConceptList) {
				for (CSortedNegLinker<CConcept*>* opLinkerIt = opLinker; opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
					CConcept* opConcept = opLinkerIt->getData();
					bool opConNeg = opLinkerIt->isNegated()^negated;
					nextTestingConceptList.append(TConceptNegPair(opConcept,opConNeg));
				}
				return true;
			}




			bool CCoreOntologyCyclesExtractorPreProcess::getConceptCycle(QSet<TConceptNegPair>* visitedConceptSet, QSet<TConceptNegPair>* cylceConceptSet, bool* cycleNominalConnection) {

				bool cycleClosed = false;
				bool nominalConn = false;
				QHash<TConceptNegPair,CTestingItem*> testedConceptHash;

				QList<CTestingItem*> itemContainer;
				QList<CTestingItem*> testingConceptList;
				QList<TConceptNegPair> nextTestingConceptList;

				for (QSet<TConceptNegPair>::const_iterator it = cylceConceptSet->constBegin(), itEnd = cylceConceptSet->constEnd(); it != itEnd; ++it) {
					TConceptNegPair testConNegPair(*it);
					CConcept* testingConcept = testConNegPair.first;
					bool testConNegation = testConNegPair.second;

					CSortedNegLinker<CConcept*>* opConLinker = testingConcept->getOperandList();

					cint64 testOpCode = testingConcept->getOperatorCode();
					if (!testConNegation && (testOpCode == CCEQ || testOpCode == CCSUB || testOpCode == CCAND || testOpCode == CCSOME || testOpCode == CCATLEAST || testOpCode == CCAQSOME) 
						|| testConNegation && (testOpCode == CCOR || testOpCode == CCALL)) {

							addConceptsToList(opConLinker,testConNegation,nextTestingConceptList);

							CRole* role = testingConcept->getRole();
							if (role) {
								addConceptsToList(role->getDomainConceptList(),false,nextTestingConceptList);
								addConceptsToList(role->getRangeConceptList(),false,nextTestingConceptList);
							}

					} else if (testOpCode == CCAQCHOOCE) {
						for (CSortedNegLinker<CConcept*>* opLinkerIt = opConLinker; opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
							CConcept* opConcept = opLinkerIt->getData();
							bool opConNeg = opLinkerIt->isNegated();
							if (opConNeg == testConNegation) {
								nextTestingConceptList.append(TConceptNegPair(opConcept,false));
							}
						}
					}

					while (!nextTestingConceptList.isEmpty()) {
						TConceptNegPair nextConNegPair(nextTestingConceptList.takeFirst());
						if (!cylceConceptSet->contains(nextConNegPair)) {
							CTestingItem* nextTestingItem = new CTestingItem(nextConNegPair);
							itemContainer.append(nextTestingItem);
							// depth first search
							testingConceptList.prepend(nextTestingItem);
						}
					}
				}


				while (!testingConceptList.isEmpty()) {
					CTestingItem* testingItem = testingConceptList.takeFirst();
					TConceptNegPair testConNegPair(testingItem->mConNegPair);
					CConcept* testingConcept = testConNegPair.first;
					bool testConNegation = testConNegPair.second;

					if (cylceConceptSet->contains(testConNegPair)) {

						QList<CTestingItem*> backtrackingItemList;
						backtrackingItemList.append(testingItem->mPrevItem);

						while (!backtrackingItemList.isEmpty()) {
							CTestingItem* testedItem = backtrackingItemList.takeFirst();

							if (testedItem && !cylceConceptSet->contains(testedItem->mConNegPair)) {
								cylceConceptSet->insert(testedItem->mConNegPair);
								QList<CTestingItem*> depList(testedConceptHash.values(testedItem->mConNegPair));
								foreach (CTestingItem* depTestedItem, depList) {
									if (depTestedItem) {
										backtrackingItemList.append(depTestedItem);
									}
								}
							}
						}




					}

					if (!testedConceptHash.contains(testConNegPair)) {


						CSortedNegLinker<CConcept*>* opConLinker = testingConcept->getOperandList();

						cint64 testOpCode = testingConcept->getOperatorCode();
						if (!testConNegation && (testOpCode == CCEQ || testOpCode == CCSUB || testOpCode == CCAND || testOpCode == CCSOME || testOpCode == CCATLEAST || testOpCode == CCAQSOME) 
							|| testConNegation && (testOpCode == CCOR || testOpCode == CCALL)) {

								addConceptsToList(opConLinker,testConNegation,nextTestingConceptList);

								CRole* role = testingConcept->getRole();
								if (role) {
									addConceptsToList(role->getDomainConceptList(),false,nextTestingConceptList);
									addConceptsToList(role->getRangeConceptList(),false,nextTestingConceptList);
								}

						} else if (testOpCode == CCAQCHOOCE) {
							for (CSortedNegLinker<CConcept*>* opLinkerIt = opConLinker; opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
								CConcept* opConcept = opLinkerIt->getData();
								bool opConNeg = opLinkerIt->isNegated();
								if (opConNeg == testConNegation) {
									nextTestingConceptList.append(TConceptNegPair(opConcept,false));
								}
							}
						} else if (testOpCode == CCNOMINAL || testOpCode == CCVALUE) {
							nominalConn = true;
						}

						while (!nextTestingConceptList.isEmpty()) {
							TConceptNegPair nextConNegPair(nextTestingConceptList.takeFirst());
							CTestingItem* nextTestingItem = new CTestingItem(nextConNegPair,testingItem->mSearchingDepth+1,testingItem);
							itemContainer.append(nextTestingItem);
							// depth first search
							testingConceptList.prepend(nextTestingItem);
						}

					}
					testedConceptHash.insertMulti(testConNegPair,testingItem->mPrevItem);
				}

				cDeleteAll(itemContainer);


				for (QSet<TConceptNegPair>::const_iterator it = cylceConceptSet->constBegin(), itEnd = cylceConceptSet->constEnd(); it != itEnd; ++it) {
					TConceptNegPair testConNegPair(*it);
					visitedConceptSet->insert(testConNegPair);
				}

				if (cycleNominalConnection) {
					*cycleNominalConnection = nominalConn;
				}

				return true;
			}



			bool CCoreOntologyCyclesExtractorPreProcess::hasConceptCycle(CConcept* concept, bool negated, QSet<TConceptNegPair>* visitedConceptSet, QSet<TConceptNegPair>* cylceConceptSet) {

				TConceptNegPair conNegPair(concept,negated);

				if (!visitedConceptSet->contains(conNegPair)) {

					bool cycleClosed = false;
					QHash<TConceptNegPair,CTestingItem*> testedConceptDepHash;

					QList<CTestingItem*> itemContainer;
					QList<CTestingItem*> testingConceptList;
					CTestingItem* firstTestingItem = new CTestingItem(conNegPair);
					itemContainer.append(firstTestingItem);
					testingConceptList.append(firstTestingItem);

					QList<TConceptNegPair> nextTestingConceptList;

					while (!testingConceptList.isEmpty() && !cycleClosed) {
						CTestingItem* testingItem = testingConceptList.takeFirst();
						TConceptNegPair testConNegPair(testingItem->mConNegPair);
						CConcept* testingConcept = testConNegPair.first;
						bool testConNegation = testConNegPair.second;

						CTestingItem* alreadyTestedItem = testedConceptDepHash.value(testConNegPair,nullptr);
						if (alreadyTestedItem) {


							if (alreadyTestedItem->mSearchingDepth < testingItem->mSearchingDepth) {
								// test for real cycle
								bool foundCycle = false;
								CTestingItem* prevTestedItemIt = testingItem->mPrevItem;
								while (prevTestedItemIt && !foundCycle) {
									if (testConNegPair == prevTestedItemIt->mConNegPair) {
										foundCycle = true;
									}
									prevTestedItemIt = prevTestedItemIt->mPrevItem;
								}
								if (foundCycle) {
									CTestingItem* prevTestedItemIt = testingItem->mPrevItem;
									while (prevTestedItemIt && !cycleClosed) {
										cylceConceptSet->insert(prevTestedItemIt->mConNegPair);

										if (testConNegPair == prevTestedItemIt->mConNegPair) {
											cycleClosed = true;
										}
										prevTestedItemIt = prevTestedItemIt->mPrevItem;
									}
								}
							}
							// concept is already examined, no cycle for this concept


						} else {
							if (!visitedConceptSet->contains(testConNegPair)) {
								visitedConceptSet->insert(testConNegPair);
								// examine concept and sub concepts
								testedConceptDepHash.insert(testConNegPair,testingItem);

								CSortedNegLinker<CConcept*>* opConLinker = testingConcept->getOperandList();

								cint64 testOpCode = testingConcept->getOperatorCode();
								if (!testConNegation && (testOpCode == CCEQ || testOpCode == CCSUB || testOpCode == CCAND || testOpCode == CCSOME || testOpCode == CCATLEAST || testOpCode == CCAQSOME) 
									|| testConNegation && (testOpCode == CCOR || testOpCode == CCALL)) {

										addConceptsToList(opConLinker,testConNegation,nextTestingConceptList);

										CRole* role = testingConcept->getRole();
										if (role) {
											addConceptsToList(role->getDomainConceptList(),false,nextTestingConceptList);
											addConceptsToList(role->getRangeConceptList(),false,nextTestingConceptList);
										}

								} else if (testOpCode == CCAQCHOOCE) {
									for (CSortedNegLinker<CConcept*>* opLinkerIt = opConLinker; opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
										CConcept* opConcept = opLinkerIt->getData();
										bool opConNeg = opLinkerIt->isNegated();
										if (opConNeg == testConNegation) {
											nextTestingConceptList.append(TConceptNegPair(opConcept,false));
										}
									}
								}

								while (!nextTestingConceptList.isEmpty()) {
									TConceptNegPair nextConNegPair(nextTestingConceptList.takeFirst());
									CTestingItem* nextTestingItem = new CTestingItem(nextConNegPair,testingItem->mSearchingDepth+1,testingItem);
									itemContainer.append(nextTestingItem);
									// depth first search
									testingConceptList.prepend(nextTestingItem);
								}
							}

						}

					}

					cDeleteAll(itemContainer);
					return cycleClosed;

				}
				return false;
			}



			CConcreteOntology* CCoreOntologyCyclesExtractorPreProcess::preprocess(CConcreteOntology* ontology, CPreProcessContext* context) {

				bool skipForELOntologies = CConfigDataReader::readConfigBoolean(context->getConfiguration(),"Konclude.Calculation.Preprocessing.CoreConceptCyclesExtraction.SkipForELFragment",true);
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();

				bool nominalsUsed = ontology->getStructureSummary()->hasNominalOccurrence();

				if (nominalsUsed && (nonELConstructsUsed || !skipForELOntologies)) {

					CTBox* tbox = ontology->getDataBoxes()->getTBox();
					CABox* abox = ontology->getDataBoxes()->getABox();
					CRBox* rbox = ontology->getDataBoxes()->getRBox();

					CConceptVector* conVec = tbox->getConceptVector();
					qint64 itemConCounts = conVec->getItemCount();

					QSet<TConceptNegPair> visitedConceptSet;
					COntologyContext* ontoContext = ontology->getOntologyContext();
					CMemoryAllocationManager* mMemMan = ontoContext->getMemoryAllocationManager();
					COntologyCoreConceptCyclesData* coreConCyclesData = ontology->getCoreConceptCyclesData();

					mStatExtractedCyclesCount = 0;
					mStatCyclicConceptsCount = 0;

					cint64 nextCycleID = 0;

					for (qint64 i = 0; i < itemConCounts; ++i) {
						CConcept* concept = conVec->getData(i);
						if (concept) {
							cint64 opCode = concept->getOperatorCode();
							if (opCode == CCSOME || opCode == CCALL || opCode == CCATLEAST || opCode == CCAQSOME || opCode == CCSOME || opCode == CCSUB || opCode == CCEQ) {
								bool negated = opCode == CCALL || opCode == CCATMOST;
								bool opNegated = opCode == CCALL;

								CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
								while (opLinkerIt) {

									CConcept* opConcept = concept;
									bool opNegate = opNegated^opLinkerIt->isNegated();

									QSet<TConceptNegPair> cylceConceptSet;

									if (hasConceptCycle(opConcept,opNegate,&visitedConceptSet,&cylceConceptSet)) {
										bool cycleNominalConnection = false;
										if (getConceptCycle(&visitedConceptSet,&cylceConceptSet,&cycleNominalConnection)) {

											++mStatExtractedCyclesCount;
											cint64 cycleConceptCount = cylceConceptSet.count();
											mStatCyclicConceptsCount += cycleConceptCount;

											CConceptCycleData* conCycleData = CObjectParameterizingAllocator< CConceptCycleData,CContext* >::allocateAndConstructAndParameterize(mMemMan,ontoContext);
											conCycleData->initConceptCycleData();
											conCycleData->addCycleConcepts(cylceConceptSet);
											conCycleData->setNominalConnection(cycleNominalConnection);
											conCycleData->setCycleID(nextCycleID++);

											coreConCyclesData->installCycle(conCycleData);

										}
									}

									opLinkerIt = opLinkerIt->getNext();
								}
							}
						}
					}

					LOG(INFO,"::Konclude::Reasoner::Preprocess::CoreConceptCycleExtractor",logTr("Found %1 concept cycles with %2 cyclic concepts.").arg(mStatExtractedCyclesCount).arg(mStatCyclicConceptsCount),this);
				}

				return ontology;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
