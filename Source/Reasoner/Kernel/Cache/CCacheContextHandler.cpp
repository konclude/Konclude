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

#include "CCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CCacheHandler::CCacheHandler(CSatisfiableCacheReader *satCacheReader, CUnsatisfiableCacheReader *unsatCacheReader, CSatisfiableCacheWriter *satCacheWriter, CUnsatisfiableCacheWriter *unsatCacheWriter) {
					satReader = satCacheReader;
					unsatReader = unsatCacheReader;
					satWriter = satCacheWriter;
					unsatWriter = unsatCacheWriter;

					incUnsatReader = 0;
					incSatReader = 0;
					lastMergingPrimClash = false;
					if (unsatReader) {
						incUnsatReader = dynamic_cast<CIncrementalUnsatisfiableCacheReader *>(unsatReader);
					}
					if (satReader) {
						incSatReader = dynamic_cast<CIncrementalSatisfiableCacheReader *>(satReader);
					}
				}


				CCacheHandler::~CCacheHandler() {
				}

				bool CCacheHandler::isSatisfiable(QVector<CCacheValue> &itemVec, qint64 count) {
					return satReader->isSatisfiable(itemVec,count);
				}


				bool CCacheHandler::getSatisfiableOutcome(QVector<CCacheValue> &itemVec, qint64 count, QList<CCacheValue> *outcomeList) {
					return satReader->getSatisfiableOutcome(itemVec,count,outcomeList);
				}



				bool CCacheHandler::isUnsatisfiable(QVector<CCacheValue> &itemVec, qint64 count) {
					return unsatReader->isUnsatisfiable(itemVec,count);
				}


				QList<CCacheValue> CCacheHandler::getUnsatisfiableItems(QVector<CCacheValue> &itemVec, qint64 count) {
					return unsatReader->getUnsatisfiableItems(itemVec,count);
				}



				void CCacheHandler::setSatisfiable(QVector<CCacheValue> &itemVec, qint64 itemCount, QVector<CCacheValue> &outcomeVec, qint64 outCount) {
					if (satWriter) {
						satWriter->setSatisfiable(itemVec,itemCount,outcomeVec,outCount);
					}
				}


				void CCacheHandler::setUnsatisfiable(QVector<CCacheValue> &itemVec, qint64 count, QVector<CCacheValue> &clashVec, qint64 clashCount) {
					return unsatWriter->setUnsatisfiable(itemVec,count,clashVec,clashCount);
				}


				bool CCacheHandler::isSatisfiable(CConceptSet *conceptSet) {
					return CSatisfiableCacheReader::isSatisfiable(conceptSet);
				}

				bool CCacheHandler::getSatisfiableOutcome(CConceptSet *conceptSet, QList<CCacheValue> *outcomeList) {
					return CSatisfiableCacheReader::getSatisfiableOutcome(conceptSet,outcomeList);
				}

				bool CCacheHandler::isUnsatisfiable(CConceptSet *conceptSet) {
					return CUnsatisfiableCacheReader::isUnsatisfiable(conceptSet);
				}


				bool CCacheHandler::isUnsatisfiable(QList<CConceptDescriptorLinker *> &conDesNegLinkerList) {
					return CUnsatisfiableCacheReader::isUnsatisfiable(conDesNegLinkerList);
				}






				bool CCacheHandler::isUnsatisfiableIndividualPreferIncrementalTesting(CIndividualProcessBase *indi, QList<QPair<CConcept *,bool> > *sortedAddConceptsList, qint64 *maximalIndividualAncestorTestCount) {
					if (incUnsatReader) {
						incUnsatReader->incrementUnsatisfiableTestReset();
						CConceptSetIterator *conSetIt = indi->getConceptSetBase()->getConceptSetIterator();
						bool continueTesting = true;
						bool unsatisfiable = false;

						QList<QPair<CConcept *,bool> >::const_iterator addConIt = sortedAddConceptsList->constBegin(), addConItEnd = sortedAddConceptsList->constEnd();
						CConcept *addConcept = 0;
						CConcept *lastAddConcept = 0;
						bool addConceptNegation = false;
						bool lastAddConceptNegation = false;
						if (addConIt != addConItEnd) {
							addConcept = addConIt->first;
							addConceptNegation = addConIt->second;
							++addConIt;
						}

						while (conSetIt->hasValue() && continueTesting && !unsatisfiable) {
							CConceptDescriptorLinker *conDesLinker = **conSetIt;
							if (addConcept && conDesLinker->getConceptTag() >= addConcept->getConceptTag()) {

								bool addConUnsatisfiable = false;
								if (addConcept->getConceptTag() == conDesLinker->getConceptTag()) {
									if (addConceptNegation != conDesLinker->isNegated()) {
										lastMergingPrimClash = true;
										addConUnsatisfiable = true;
									}
								}

								if (!addConUnsatisfiable) {
									CConceptDescriptorLinker addConDesLinker;
									addConDesLinker.init(addConcept,addConceptNegation);
									addConUnsatisfiable = incUnsatReader->incrementUnsatisfiableTest(&addConDesLinker,&continueTesting);
								}
								if (addConUnsatisfiable) {
									unsatisfiable = true;
								}
								addConcept = 0;
								if (addConIt != addConItEnd) {
									lastAddConcept = addConcept;
									lastAddConceptNegation = addConceptNegation;
									addConcept = addConIt->first;
									addConceptNegation = addConIt->second;

									if (addConcept && lastAddConcept && lastAddConcept->getConceptTag() == addConcept->getConceptTag()) {
										if (addConceptNegation != lastAddConceptNegation) {
											lastMergingPrimClash = true;
											unsatisfiable = true;
										}
									}

									++addConIt;
								}
							} else {
								unsatisfiable &= incUnsatReader->incrementUnsatisfiableTest(conDesLinker,&continueTesting);
								conSetIt->next();
							}
						}
						delete conSetIt;
						while (addConcept && addConIt != addConItEnd && continueTesting && !unsatisfiable) {
							CConceptDescriptorLinker addConDesLinker;
							addConDesLinker.init(addConcept,addConceptNegation);
							bool addConUnsatisfiable = incUnsatReader->incrementUnsatisfiableTest(&addConDesLinker,&continueTesting);
							if (addConUnsatisfiable) {
								unsatisfiable = true;
							}
							addConcept = 0;
							if (addConIt != addConItEnd) {
								lastAddConcept = addConcept;
								lastAddConceptNegation = addConceptNegation;
								addConcept = addConIt->first;
								addConceptNegation = addConIt->second;

								if (addConcept && lastAddConcept && lastAddConcept->getConceptTag() == addConcept->getConceptTag()) {
									if (addConceptNegation != lastAddConceptNegation) {
										lastMergingPrimClash = true;
										unsatisfiable = true;
									}
								}

								++addConIt;
							}			
						}
						if (maximalIndividualAncestorTestCount) {
							*maximalIndividualAncestorTestCount = 1;
						}
						return unsatisfiable;
					} else if (unsatReader) {
						unsatReader->isUnsatisfiable(indi->getConceptSetBase());
					}
					return false;
				}



				QList<CConceptDescriptorLinker *> CCacheHandler::getIndividualUnsatisfiableConceptDescriptionLinkers(CIndividualProcessBase *indi, QList<CConceptDescriptorLinker *> *sortedAddConceptsList, qint64 maximalIndividualAncestorTestCount, bool restartTesting) {
					QList<CConceptDescriptorLinker *> conDesLinkerList;
					QList<CCacheValue> unsatCacheItemsList;
					if (restartTesting || lastMergingPrimClash) {
						bool incUnsatCacheTesting = incUnsatReader != 0 && !lastMergingPrimClash;
						bool unsatisfiable = false;
						lastMergingPrimClash = false;
						conDesLinkerList.clear();

						if (incUnsatReader) {
							incUnsatReader->incrementUnsatisfiableTestReset();
						}
						CConceptSetIterator *conSetIt = indi->getConceptSetBase()->getConceptSetIterator();
						bool continueTesting = true;

						QList<CConceptDescriptorLinker *>::const_iterator addConIt = sortedAddConceptsList->constBegin(), addConItEnd = sortedAddConceptsList->constEnd();
						CConceptDescriptorLinker *addConceptDesLinker = 0;
						CConceptDescriptorLinker *lastAddConceptDesLinker = 0;

						if (addConIt != addConItEnd) {
							addConceptDesLinker = *addConIt;
							++addConIt;
						}

						while (conSetIt->hasValue() && continueTesting && !unsatisfiable) {
							CConceptDescriptorLinker *conDesLinker = **conSetIt;
							if (addConceptDesLinker && conDesLinker->getConceptTag() >= addConceptDesLinker->getConceptTag()) {
								bool addConUnsatisfiable = false;
								if (conDesLinker->getConceptTag() == addConceptDesLinker->getConceptTag()) {
									if (conDesLinker->isNegated() != addConceptDesLinker->isNegated()) {
										addConUnsatisfiable = true;
										conDesLinkerList.append(conDesLinker);
										conDesLinkerList.append(addConceptDesLinker);
										lastMergingPrimClash = true;
										unsatisfiable = true;
									}
								}
								if (!addConUnsatisfiable && incUnsatCacheTesting) {
									addConUnsatisfiable = incUnsatReader->incrementUnsatisfiableTest(addConceptDesLinker,&continueTesting);
									if (addConUnsatisfiable) {
										unsatCacheItemsList = incUnsatReader->getIncrementalTestedUnsatisfiableItems();
										unsatisfiable = true;
									}
								}
								addConceptDesLinker = 0;
								if (addConIt != addConItEnd) {
									lastAddConceptDesLinker = addConceptDesLinker;
									addConceptDesLinker = *addConIt;
									if (addConceptDesLinker && lastAddConceptDesLinker && lastAddConceptDesLinker->getConceptTag() == addConceptDesLinker->getConceptTag()) {
										if (addConceptDesLinker->isNegated() != lastAddConceptDesLinker->isNegated()) {
											lastMergingPrimClashList.append(lastAddConceptDesLinker);
											lastMergingPrimClashList.append(addConceptDesLinker);
											lastMergingPrimClash = true;
											unsatisfiable = true;
										}
									}
									++addConIt;
								}
							} else {
								if (incUnsatCacheTesting) {
									unsatisfiable = incUnsatReader->incrementUnsatisfiableTest(conDesLinker,&continueTesting);
									if (unsatisfiable) {
										unsatCacheItemsList = incUnsatReader->getIncrementalTestedUnsatisfiableItems();
										unsatisfiable = true;
									}
								}
								conSetIt->next();
							}
						}
						delete conSetIt;
						while (addConceptDesLinker && addConIt != addConItEnd && continueTesting && !unsatisfiable) {
							bool addConUnsatisfiable = incUnsatReader->incrementUnsatisfiableTest(addConceptDesLinker,&continueTesting);
							if (addConUnsatisfiable) {
								unsatCacheItemsList = incUnsatReader->getIncrementalTestedUnsatisfiableItems();
								unsatisfiable = true;
							}
							addConceptDesLinker = 0;
							if (addConIt != addConItEnd) {
								lastAddConceptDesLinker = addConceptDesLinker;
								addConceptDesLinker = *addConIt;
								if (addConceptDesLinker && lastAddConceptDesLinker && lastAddConceptDesLinker->getConceptTag() == addConceptDesLinker->getConceptTag()) {
									if (addConceptDesLinker->isNegated() != lastAddConceptDesLinker->isNegated()) {
										lastMergingPrimClashList.append(lastAddConceptDesLinker);
										lastMergingPrimClashList.append(addConceptDesLinker);
										lastMergingPrimClash = true;
										unsatisfiable = true;
									}
								}
								++addConIt;
							}
						}
					} else if (incUnsatReader) {
						unsatCacheItemsList = incUnsatReader->getLastTestedUnsatisfiableItems();
					}

					if (!lastMergingPrimClash) {
						conDesLinkerList = CUnsatisfiableCacheReader::cacheTrans.backTranslate(indi,sortedAddConceptsList,unsatCacheItemsList);
					} else {
						conDesLinkerList = lastMergingPrimClashList;
					}
					return conDesLinkerList;
				}








				bool CCacheHandler::isUnsatisfiableIndividualPreferIncrementalTesting(CIndividualProcessBase *indi, CConcept *additionalConcept, bool additionalConceptNegation, qint64 *maximalIndividualAncestorTestCount ) {
					if (incUnsatReader) {
						incUnsatReader->incrementUnsatisfiableTestReset();
						CConceptSetIterator *conSetIt = indi->getConceptSetBase()->getConceptSetIterator();
						bool continueTesting = true;
						bool unsatisfiable = false;
						bool addConTested = false;
						while (conSetIt->hasValue() && continueTesting && !unsatisfiable) {
							CConceptDescriptorLinker *conDesLinker = **conSetIt;
							if (conDesLinker->getConceptTag() > additionalConcept->getConceptTag()) {
								CConceptDescriptorLinker addConDesLinker;
								addConDesLinker.init(additionalConcept,additionalConceptNegation);
								bool addConUnsatisfiable = incUnsatReader->incrementUnsatisfiableTest(&addConDesLinker,&continueTesting);
								if (addConUnsatisfiable) {
									unsatisfiable = true;
								}
								addConTested = true;
							}
							if (continueTesting) {
								unsatisfiable &= incUnsatReader->incrementUnsatisfiableTest(conDesLinker,&continueTesting);
							}
							conSetIt->next();
						}
						delete conSetIt;
						if (!addConTested && !unsatisfiable) {
							CConceptDescriptorLinker addConDesLinker;
							addConDesLinker.init(additionalConcept,additionalConceptNegation);
							bool addConUnsatisfiable = incUnsatReader->incrementUnsatisfiableTest(&addConDesLinker,&continueTesting);
							if (addConUnsatisfiable) {
								return true;
							}
						}
						if (maximalIndividualAncestorTestCount) {
							*maximalIndividualAncestorTestCount = 1;
						}
						return unsatisfiable;
					} else if (unsatReader) {
						unsatReader->isUnsatisfiable(indi->getConceptSetBase());
					}
					return false;
				}


				bool CCacheHandler::isSatisfiableIndividualPreferIncrementalTesting(CIndividualProcessBase *indi) {
					bool satisfiable = false;
					if (incSatReader) {
						incSatReader->incrementSatisfiableTestReset();
						CConceptSetIterator *conSetIt = indi->getConceptSetBase()->getConceptSetIterator();
						bool continueTesting = true;
						while (conSetIt->hasValue() && continueTesting && !satisfiable) {
							CConceptDescriptorLinker *conDesLinker = **conSetIt;
							satisfiable = incSatReader->incrementSatisfiableTest(conDesLinker,&continueTesting);
							conSetIt->next();
						}
						delete conSetIt;
					} else if (satReader) {
						satisfiable = satReader->isSatisfiable(indi->getConceptSetBase());
					}
					return satisfiable;
				}


				bool CCacheHandler::isSatisfiableIndividual(CIndividualProcessBase *indi) {					
					bool satisfiable = false;
					if (satReader) {
						satisfiable = satReader->isSatisfiable(indi->getConceptSetBase());
						if (satisfiable) {
							QList<CCacheValue> outList;
							satisfiable = satReader->getSatisfiableOutcome(indi->getConceptSetBase(),&outList);
							if (satisfiable && !outList.isEmpty()) {
								bool foundPossInverseNotSat = false;
								CLink *ancLink = indi->getAncestorLink();
								if (ancLink) {
									CSortedNegLinker<CRole *> *ancRoleIt = ancLink->getRoleLinker();
									for (QList<CCacheValue>::const_iterator it = outList.constBegin(), itEnd = outList.constEnd(); it != itEnd && !foundPossInverseNotSat; ++it) {
										CCacheValue cacheValue(*it);
										if (cacheValue.isCachedRole()) {
											qint64 roleTag = cacheValue.getTag();
											qint64 ontTag = cacheValue.getOntology();
											CSortedNegLinker<CRole *> *testRoleIt = ancRoleIt;
											while (testRoleIt && !foundPossInverseNotSat) {
												CRole *role = testRoleIt->getData();
												if (role->getRoleTag() == roleTag && role->getTerminologyTag() == ontTag) {
													foundPossInverseNotSat = true;
													break;
												}
												testRoleIt = testRoleIt->getNext();
											}
										}
									}
								}
								if (foundPossInverseNotSat) {
									satisfiable = false;
								}
							}
						}
					}
					return satisfiable;
				}

				bool CCacheHandler::isUnsatisfiableIndividualPreferIncrementalTesting(CIndividualProcessBase *indi, qint64 *maximalIndividualAncestorTestCount) {
					bool unsatisfiable = false;
					if (incUnsatReader) {
						incUnsatReader->incrementUnsatisfiableTestReset();
						CConceptSetIterator *conSetIt = indi->getConceptSetBase()->getConceptSetIterator();
						bool continueTesting = true;
						while (conSetIt->hasValue() && continueTesting && !unsatisfiable) {
							CConceptDescriptorLinker *conDesLinker = **conSetIt;
							unsatisfiable = incUnsatReader->incrementUnsatisfiableTest(conDesLinker,&continueTesting);
							conSetIt->next();
						}
						delete conSetIt;
						if (maximalIndividualAncestorTestCount) {
							*maximalIndividualAncestorTestCount = 1;
						}
					} else if (unsatReader) {
						unsatisfiable = unsatReader->isUnsatisfiable(indi->getConceptSetBase());
					}
					return unsatisfiable;
				}


				bool CCacheHandler::isIndividualMergingUnsatisfiable(CIndividualProcessBase *indi1, CIndividualProcessBase *indi2) {
					CConceptSetBase *conSet1 = indi1->getConceptSetBase();
					CConceptSetBase *conSet2 = indi2->getConceptSetBase();
					lastMergingPrimClashList.clear();
					lastMergingPrimClash = false;
					if (incUnsatReader) {
						incUnsatReader->incrementUnsatisfiableTestReset();

						bool mergeable = true;
						CConceptSetIterator *conIter1 = conSet1->getConceptSetIterator();			
						CConceptSetIterator *conIter2 = conSet2->getConceptSetIterator();	

						CConceptDescriptorLinker *conDesLinker1 = 0;
						CConceptDescriptorLinker *conDesLinker2 = 0;

						if (conIter1->hasValue()) {
							conDesLinker1 = **conIter1;
							conIter1->next();
						}
						if (conIter2->hasValue()) {
							conDesLinker2 = **conIter2;
							conIter2->next();
						}
						bool continueTesting = true;
						while (conDesLinker1 && mergeable) {
							CConcept *con1 = conDesLinker1->getData();
							qint64 conTag1 = con1->getConceptTag();
							bool hasThisTag = false;
							while (conDesLinker2 && mergeable) {
								CConcept *con2 = conDesLinker2->getData();
								qint64 conTag2 = con2->getConceptTag();
								if (conTag2 == conTag1) {
									if (conDesLinker1->isNegated() == conDesLinker2->isNegated()) {
										hasThisTag = true;
										break;
									} else {
										mergeable = false;
										hasThisTag = true;
										lastMergingPrimClashList.append(conDesLinker1);
										lastMergingPrimClashList.append(conDesLinker2);
										lastMergingPrimClash = true;
										break;
									}
								} else if (conTag2 > conTag1) {
									hasThisTag = false;
									break;
								} else if (conTag2 < conTag1) {
									if (continueTesting) {
										mergeable &= !incUnsatReader->incrementUnsatisfiableTest(conDesLinker2,&continueTesting);
									}
									if (conIter2->hasValue()) {
										conDesLinker2 = **conIter2;
										conIter2->next();
									} else {
										conDesLinker2 = 0;
									}
								} 
							}

							if (!hasThisTag && continueTesting) {
								mergeable &= !incUnsatReader->incrementUnsatisfiableTest(conDesLinker1,&continueTesting);
							}

							if (conIter1->hasValue()) {
								conDesLinker1 = **conIter1;
								conIter1->next();
							} else {
								conDesLinker1 = 0;
							}
						}

						while (incUnsatReader && continueTesting && mergeable && conDesLinker2) {
							mergeable &= !incUnsatReader->incrementUnsatisfiableTest(conDesLinker2,&continueTesting);
							if (conIter2->hasValue()) {
								conDesLinker2 = **conIter2;
								conIter2->next();
							} else {
								conDesLinker2 = 0;
							}
						}
						delete conIter1;
						delete conIter2;

						return !mergeable;
					} else {						
						return !CConceptSet::isMergeableWithoutClash(conSet1,conSet2);
					}
					return false;
				}






				QList<CConceptDescriptorLinker *> CCacheHandler::getIndividualMergingUnsatisfiableItems(CIndividualProcessBase *indi1, CIndividualProcessBase *indi2, bool restartTesting) {
					QList<CConceptDescriptorLinker *> conDesLinkerList;

					bool clashCached = false;
					QList<CCacheValue> cacheItems;
					if (!incUnsatReader || restartTesting) {
						if (incUnsatReader) {
							incUnsatReader->incrementUnsatisfiableTestReset();
						}
						lastMergingPrimClash = false;
						lastMergingPrimClashList.clear();

						CConceptSetBase *conSet1 = indi1->getConceptSetBase();
						CConceptSetBase *conSet2 = indi2->getConceptSetBase();

						bool mergeable = true;
						CConceptSetIterator *conIter1 = conSet1->getConceptSetIterator();			
						CConceptSetIterator *conIter2 = conSet2->getConceptSetIterator();	

						CConceptDescriptorLinker *conDesLinker1 = 0;
						CConceptDescriptorLinker *conDesLinker2 = 0;

						if (conIter1->hasValue()) {
							conDesLinker1 = **conIter1;
							conIter1->next();
						}
						if (conIter2->hasValue()) {
							conDesLinker2 = **conIter2;
							conIter2->next();
						}
						bool continueTesting = true;
						while (conDesLinker1 && mergeable) {
							CConcept *con1 = conDesLinker1->getData();
							qint64 conTag1 = con1->getConceptTag();
							bool hasThisTag = false;
							while (conDesLinker2 && mergeable) {
								CConcept *con2 = conDesLinker2->getData();
								qint64 conTag2 = con2->getConceptTag();
								if (conTag2 == conTag1) {
									if (conDesLinker1->isNegated() == conDesLinker2->isNegated()) {
										hasThisTag = true;
										break;
									} else {
										mergeable = false;
										lastMergingPrimClashList.append(conDesLinker1);
										lastMergingPrimClashList.append(conDesLinker2);
										lastMergingPrimClash = true;
										hasThisTag = true;
										break;
									}
								} else if (conTag2 > conTag1) {
									hasThisTag = false;
									break;
								} else if (conTag2 < conTag1) {
									if (continueTesting) {
										clashCached |= incUnsatReader->incrementUnsatisfiableTest(conDesLinker2,&continueTesting);
										mergeable &= !clashCached;
									}
									if (conIter2->hasValue()) {
										conDesLinker2 = **conIter2;
										conIter2->next();
									} else {
										conDesLinker2 = 0;
									}
								} 
							}

							if (!hasThisTag && continueTesting && incUnsatReader) {
								clashCached |= incUnsatReader->incrementUnsatisfiableTest(conDesLinker1,&continueTesting);
								mergeable &= !clashCached;
							}

							if (conIter1->hasValue()) {
								conDesLinker1 = **conIter1;
								conIter1->next();
							} else {
								conDesLinker1 = 0;
							}
						}
						while (incUnsatReader && continueTesting && mergeable && conDesLinker2) {
							clashCached |= incUnsatReader->incrementUnsatisfiableTest(conDesLinker2,&continueTesting);
							mergeable &= !clashCached;
							if (conIter2->hasValue()) {
								conDesLinker2 = **conIter2;
								conIter2->next();
							} else {
								conDesLinker2 = 0;
							}
						}
						delete conIter1;
						delete conIter2;

						if (clashCached && incUnsatReader) {
							cacheItems = incUnsatReader->getIncrementalTestedUnsatisfiableItems();
							conDesLinkerList = CUnsatisfiableCacheReader::cacheTrans.backTranslate(indi1,indi2,cacheItems);
						} else if (lastMergingPrimClash) {
							conDesLinkerList = lastMergingPrimClashList;
						}
					} else if (!restartTesting && incUnsatReader) {
						if (lastMergingPrimClash) {
							conDesLinkerList = lastMergingPrimClashList;
						} else {
							cacheItems = incUnsatReader->getLastTestedUnsatisfiableItems();
							conDesLinkerList = CUnsatisfiableCacheReader::cacheTrans.backTranslate(indi1,indi2,cacheItems);
						}
					}					

					return conDesLinkerList;
				}




				QList<CConceptDescriptorLinker *> CCacheHandler::getIndividualUnsatisfiableConceptDescriptionLinkers(CIndividualProcessBase *indi, qint64 maximalIndividualAncestorTestCount, bool restartTesting) {
					QList<CCacheValue> cacheItems;
					if (restartTesting || !incUnsatReader) {
						cacheItems = unsatReader->getUnsatisfiableItems(indi->getConceptSetBase());
					} else {
						cacheItems = incUnsatReader->getLastTestedUnsatisfiableItems();
					}
					QList<CConceptDescriptorLinker *> conDesLinkerList = CUnsatisfiableCacheReader::cacheTrans.backTranslate(indi,cacheItems);
					return conDesLinkerList;
				}



				QList<CConceptDescriptorLinker *> CCacheHandler::getIndividualUnsatisfiableConceptDescriptionLinkers(CIndividualProcessBase *indi, CConceptDescriptorLinker *addConDesLinker, qint64 maximalIndividualAncestorTestCount, bool restartTesting) {
					QList<CConceptDescriptorLinker *> conDesLinkerList;
					QList<CCacheValue> unsatCacheItemsList;
					if (incUnsatReader) {
						if (restartTesting) {
							bool unsatisfiable = false;
							incUnsatReader->incrementUnsatisfiableTestReset();
							CConceptSetIterator *conSetIt = indi->getConceptSetBase()->getConceptSetIterator();
							bool continueTesting = true;
							bool addConTested = false;
							while (conSetIt->hasValue() && continueTesting && !unsatisfiable) {
								CConceptDescriptorLinker *conDesLinker = **conSetIt;
								if (conDesLinker->getConceptTag() > addConDesLinker->getConceptTag()) {
									bool addConUnsatisfiable = incUnsatReader->incrementUnsatisfiableTest(addConDesLinker,&continueTesting);
									if (addConUnsatisfiable) {
										unsatCacheItemsList = incUnsatReader->getIncrementalTestedUnsatisfiableItems();
										unsatisfiable = true;
									}
									addConTested = true;
								}
								if (continueTesting) {
									unsatisfiable = incUnsatReader->incrementUnsatisfiableTest(conDesLinker,&continueTesting);
									if (unsatisfiable) {
										unsatCacheItemsList = incUnsatReader->getIncrementalTestedUnsatisfiableItems();
										unsatisfiable = true;
									}
								}
								conSetIt->next();
							}
							delete conSetIt;
							if (!addConTested) {
								bool addConUnsatisfiable = incUnsatReader->incrementUnsatisfiableTest(addConDesLinker,&continueTesting);
								if (addConUnsatisfiable) {
									unsatCacheItemsList = incUnsatReader->getIncrementalTestedUnsatisfiableItems();
									unsatisfiable = true;
								}
							}
						} else {
							unsatCacheItemsList = incUnsatReader->getLastTestedUnsatisfiableItems();
						}

					} else if (unsatReader) {
						unsatCacheItemsList = unsatReader->getUnsatisfiableItems(indi->getConceptSetBase());
					}
					conDesLinkerList = CUnsatisfiableCacheReader::cacheTrans.backTranslate(indi,addConDesLinker,unsatCacheItemsList);
					return conDesLinkerList;				
				}


				QList<CCacheValue> CCacheHandler::getUnsatisfiableItems(CConceptSet *conceptSet) {
					return CUnsatisfiableCacheReader::getUnsatisfiableItems(conceptSet);
				}

				void CCacheHandler::setSatisfiable(CConceptSet *conceptSet) {
					CSatisfiableCacheWriter::setSatisfiable(conceptSet);
				}

				void CCacheHandler::setUnsatisfiable(CConceptSet *conceptSet, CLinker<CConceptDescriptorLinker *> *clashLinker) {
					CUnsatisfiableCacheWriter::setUnsatisfiable(conceptSet,clashLinker);
				}

				void CCacheHandler::setUnsatisfiable(CConceptSet *conceptSet, QList<CConceptDescriptorLinker *> &clashList) {
					CUnsatisfiableCacheWriter::setUnsatisfiable(conceptSet,clashList);
				}

				void CCacheHandler::setUnsatisfiable(CConceptDescriptorLinker *conDesNegLinker, CLinker<CConceptDescriptorLinker *> *clashLinker) {
					CUnsatisfiableCacheWriter::setUnsatisfiable(conDesNegLinker,clashLinker);
				}

				void CCacheHandler::setUnsatisfiable(QList<CConceptDescriptorLinker *> &conDesNegLinkerList, CLinker<CConceptDescriptorLinker *> *clashLinker) {
					CUnsatisfiableCacheWriter::setUnsatisfiable(conDesNegLinkerList,clashLinker);
				}

				void CCacheHandler::setUnsatisfiable(QList<CConceptDescriptorLinker *> *conDesNegLinkerList, QList<CConceptDescriptorLinker *> *clashConceptsList) {
					CUnsatisfiableCacheWriter::setUnsatisfiable(conDesNegLinkerList,clashConceptsList);
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
