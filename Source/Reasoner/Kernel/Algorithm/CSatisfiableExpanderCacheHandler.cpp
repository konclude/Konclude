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

#include "CSatisfiableExpanderCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CSatisfiableExpanderCacheHandler::CSatisfiableExpanderCacheHandler(CSignatureSatisfiableExpanderCacheReader* satCacheReader, CSignatureSatisfiableExpanderCacheWriter* satCacheWriter) {
					mSatCacheReader = satCacheReader;
					mSatCacheWriter = satCacheWriter;

					mTmpContext = nullptr;
					mMemAllocMan = nullptr;
					mWriteData = nullptr;
				}


				CSatisfiableExpanderCacheHandler::~CSatisfiableExpanderCacheHandler() {
				}




				bool CSatisfiableExpanderCacheHandler::isIndividualNodeExpandCached(CIndividualProcessNode* individualNode, bool* satisfiable, CSignatureSatisfiableExpanderCacheEntry** entry, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);
					if (conSet && !conSet->getConceptStructure()->hasBindingPropagationConcepts() && !conSet->getConceptStructure()->hasDynamicCreatedConcepts()) {
						cint64 conSig = conSet->getConceptSignatureValue();
						CSignatureSatisfiableExpanderCacheEntry* cachedEntry = mSatCacheReader->getCacheEntry(conSig);
						if (cachedEntry) {

							cint64 conSetCount = conSet->getConceptCount();
							cint64 expCount = cachedEntry->getExpanderCacheValueCount();
							if (expCount < conSetCount) {
								return false;
							}

							if (!compareIndividualNodeCompatibility(individualNode,cachedEntry,calcAlgContext)) {
								return false;
							}							

							if (entry) {
								*entry = cachedEntry;
							}
							if (satisfiable) {
								*satisfiable = cachedEntry->isSatisfiable();
							}
							return true;
						}
					}
					return false;
				}



				bool CSatisfiableExpanderCacheHandler::compareIndividualNodeCompatibility(CIndividualProcessNode* individualNode, CSignatureSatisfiableExpanderCacheEntry* cachedEntry, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);
					cint64 conSetCount = conSet->getConceptCount();
					CExpanderCacheValueLinker* expCacheValueLinker = cachedEntry->getExpanderCacheValueLinker();
					cint64 conNr = 0;
					while (expCacheValueLinker && conNr++ < conSetCount) {

						CCacheValue* cacheValue = expCacheValueLinker->getCacheValue();
						CConcept* concept = (CConcept*)cacheValue->getIdentification();

						CConceptDescriptor* conDes = nullptr;
						CDependencyTrackPoint* depTrackPoint = nullptr;

						if (conSet->getConceptDescriptor(concept,conDes,depTrackPoint)) {
							bool conNegated = conDes->isNegated();
							bool cacheNegated = cacheValue->getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
							if (conNegated != cacheNegated || conDes->getConcept() != concept) {
								return false;
							}
						} else {
							return false;
						}

						expCacheValueLinker = expCacheValueLinker->getNext();
					}
					return true;

				}












				bool CSatisfiableExpanderCacheHandler::cacheIndividualNodeSatisfiable(CIndividualProcessNode* individualNode, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(SATEXPCACHEWRITINGREQUESTCOUNT,calcAlgContext);
					STATINC(SATEXPCACHESATISFIABLEWRITINGREQUESTCOUNT,calcAlgContext);


					CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);
					if (conSet) {
						if (conSet->getConceptStructure()->hasBindingPropagationConcepts() || conSet->getConceptStructure()->hasDynamicCreatedConcepts()) {
							return false;
						}
						mTestConSet = conSet;
						cint64 conSetSignature = conSet->getConceptSignatureValue();
						CIndividualNodeSatisfiableExpandingCacheStoringData* satStorData = (CIndividualNodeSatisfiableExpandingCacheStoringData*)individualNode->getIndividualSatisfiableCacheStoringData(false);
						if (satStorData) {
							if (satStorData->hasCachingError()) {
								return false;
							}
							if (satStorData->hasPreviousSatisfiableCached()) {
								if (satStorData->getLastCachedSignature() == conSetSignature) {
									return false;
								}
							}
						}

						bool localizedSatStorData = false;

						CMemoryAllocationManager* memMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						CIndividualNodeSatisfiableExpandingCacheStoringData* locSatStorData = (CIndividualNodeSatisfiableExpandingCacheStoringData*)individualNode->getIndividualSatisfiableCacheStoringData(true);
						if (locSatStorData && !satStorData) {
							// is probably caching by another thread
							return false;
						}
						if (!locSatStorData) {
							locSatStorData = CObjectAllocator<CIndividualNodeSatisfiableExpandingCacheStoringData>::allocateAndConstruct(taskMemMan);
							locSatStorData->initSatisfiableExpandingCacheRetrievalData(satStorData);
							satStorData = locSatStorData;
							individualNode->setIndividualSatisfiableCacheStoringData(locSatStorData);
							localizedSatStorData = true;
						}

						CConceptDescriptor* lastAddedConDes = conSet->getAddingSortedConceptDescriptionLinker();

						CSignatureSatisfiableExpanderCacheEntry* cachedEntry = mSatCacheReader->getCacheEntry(conSetSignature);
						if (cachedEntry) {
							STATINC(SATEXPCACHEWRITINGSIGNATUREALREADYCACHEDCOUNT,calcAlgContext);

							if (!compareIndividualNodeCompatibility(individualNode,cachedEntry,calcAlgContext)) {
								STATINC(SATEXPCACHEWRITINGSIGNATUREALREADYCACHEDINCOMPATIBLECOUNT,calcAlgContext);
								STATINC(SATEXPCACHEWRITINGERRORCOUNT,calcAlgContext);
								satStorData->setCachingError(true);
								return false;
							} else {

								if (!cachedEntry->isSatisfiable()) {
									// write only satisfiability
									STATINC(SATEXPCACHEWRITINGSIGNATUREALREADYCACHEDSATADDEDCOUNT,calcAlgContext);

									prepareCacheMessages(calcAlgContext);

									CTaskMemoryPoolAllocationManager* satCacheMemMan = mMemAllocMan;
									CContextBase* tmpContext = mTmpContext;
									CCACHINGLIST<CCacheValue>* cacheValueList = CObjectParameterizingAllocator< CCACHINGLIST<CCacheValue>,CContext* >::allocateAndConstructAndParameterize(satCacheMemMan,tmpContext);
									CCACHINGLIST<CCacheValue>* branchedList = CObjectParameterizingAllocator< CCACHINGLIST<CCacheValue>,CContext* >::allocateAndConstructAndParameterize(satCacheMemMan,tmpContext);

									CConceptDescriptor* conDesIt = lastAddedConDes;
									while (conDesIt) {
										CConceptDescriptor* conDes = conDesIt;
										cint64 conTag = conDes->getConceptTag();

										CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDCONCEPT;
										if (conDes->getNegation()) {
											cacheValueIdentifier = CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
										}
										CCacheValue cacheValue;
										cacheValue.initCacheValue(conTag,(cint64)conDes->getConcept(),cacheValueIdentifier);
										cacheValueList->prepend(cacheValue);

										if (isCardinalityRestrictionCriticalForSatisfiable(individualNode,conDes,calcAlgContext)) {
											branchedList->append(cacheValue);
										}


										conDesIt = conDesIt->getNextConceptDesciptor();
									}

									CSignatureSatisfiableExpanderCacheEntrySatisfiableBranchWriteData* satBranchWriteData = CObjectAllocator< CSignatureSatisfiableExpanderCacheEntrySatisfiableBranchWriteData >::allocateAndConstruct(satCacheMemMan);
									satBranchWriteData->initSatisfiableBranchWriteData(conSetSignature,cacheValueList,branchedList);
									addCacheMessages(satBranchWriteData,calcAlgContext);

								}

								satStorData->setLastCachedConceptDescriptor(lastAddedConDes);
								satStorData->setPreviousCached(true);
								satStorData->setPreviousSatisfiableCached(true);
								satStorData->setLastCachedSignature(conSetSignature);
								return true;
							}
						}


						CConceptDescriptor* lastCachedConceptDes = satStorData->getLastCachedConceptDescriptor();

						CPROCESSINGLIST<CConceptDescriptor*> reverseConDesList(calcAlgContext->getUsedTaskProcessorContext());

						// check whether to restart caching
						bool directBranched = false;
						bool dependencyToAncestor = false;
						cint64 lastBranchingTag = -1;
						CConceptDescriptor* conDesIt = lastAddedConDes;
						while (conDesIt != lastCachedConceptDes) {
							CConceptDescriptor* conDes = conDesIt;
							CDependencyTrackPoint* depTrackPoint = conDesIt->getDependencyTrackPoint();						
							if (hasDependencyToAncestor(individualNode,depTrackPoint,&directBranched,calcAlgContext)) {
								dependencyToAncestor = true;
							}
							reverseConDesList.prepend(conDes);
							conDesIt = conDesIt->getNextConceptDesciptor();
						}
						if (dependencyToAncestor) {
							satStorData->setPreviousCached(false);
							satStorData->setPreviousSatisfiableCached(false);
							satStorData->setLastCachedSignature(0);
							satStorData->setLastCachedConceptDescriptor(nullptr);
							lastCachedConceptDes = nullptr;
						}
						if (directBranched) {
							satStorData->setIndividualNodeOrSuccessorBranchedConcept(true);
						}

						while (conDesIt) {
							CConceptDescriptor* conDes = conDesIt;
							reverseConDesList.prepend(conDes);
							conDesIt = conDesIt->getNextConceptDesciptor();
						}


						cint64 lastConSetSignature = satStorData->getLastCachedSignature();
						if (!satStorData->hasPreviousCached() && lastCachedConceptDes && lastConSetSignature != 0 && lastConSetSignature != conSetSignature) {
							cacheIndividualNodeExpansion(individualNode,satStorData,lastCachedConceptDes,nullptr,lastConSetSignature,0,calcAlgContext);
						}



						// try satisfiable caching

						prepareCacheMessages(calcAlgContext);
						CTaskMemoryPoolAllocationManager* satCacheMemMan = mMemAllocMan;
						CTaskMemoryPoolAllocationManager* expCacheMemMan = mMemAllocMan;
						CContextBase* tmpSatBranchContext = mTmpContext;
						CContextBase* tmpExpContext = mTmpContext;

						CCACHINGHASH<cint64,cint64>* depHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,cint64>,CContext* >::allocateAndConstructAndParameterize(expCacheMemMan,tmpExpContext);
						CCACHINGLIST<CCacheValue>* expValueList = CObjectParameterizingAllocator< CCACHINGLIST<CCacheValue>,CContext* >::allocateAndConstructAndParameterize(expCacheMemMan,tmpExpContext);

						CCACHINGLIST<CCacheValue>* satValueList = CObjectParameterizingAllocator< CCACHINGLIST<CCacheValue>,CContext* >::allocateAndConstructAndParameterize(satCacheMemMan,tmpSatBranchContext);
						CCACHINGLIST<CCacheValue>* branchedList = CObjectParameterizingAllocator< CCACHINGLIST<CCacheValue>,CContext* >::allocateAndConstructAndParameterize(satCacheMemMan,tmpSatBranchContext);

						if (!satStorData->hasPreviousCached()) {
							lastCachedConceptDes = nullptr;
							lastConSetSignature = 0;
						}

						bool lastCachedConDesReached = false;
						bool cachingError = false;
						CPROCESSINGSET<cint64> notBranchConceptsSet(calcAlgContext->getUsedTaskProcessorContext());

						for (CPROCESSINGLIST<CConceptDescriptor*>::const_iterator it = reverseConDesList.constBegin(), itEnd = reverseConDesList.constEnd(); it != itEnd; ++it) {
							CConceptDescriptor* conDesIt = *it;

							CConceptDescriptor* conDes = conDesIt;
							CDependencyTrackPoint* depTrackPoint = nullptr;
							bool conceptDepInNodeOrSuccesssorBranched = false;
							cint64 conTag = conDes->getConceptTag();


							if (lastCachedConDesReached) {
								CConceptDescriptor* conDes = conDesIt;
								CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();

								if (!simpleDependencyTracking(individualNode,conTag,depTrackPoint,depHash,&notBranchConceptsSet,&conceptDepInNodeOrSuccesssorBranched,calcAlgContext)) {
									CPROCESSINGSET<CDepthDependencyPair> depSet(calcAlgContext->getTaskProcessorContext());
									CPROCESSINGLIST<CDepthDependencyPair> depList(calcAlgContext->getTaskProcessorContext());

									cint64 baseAncDepth = individualNode->getIndividualAncestorDepth();

									depSet.insert(CDepthDependencyPair(baseAncDepth,depTrackPoint));
									depList.append(CDepthDependencyPair(baseAncDepth,depTrackPoint));

									if (!complexDependencyTracking(individualNode,conTag,&depSet,&depList,depHash,&notBranchConceptsSet,&conceptDepInNodeOrSuccesssorBranched,calcAlgContext)) {
										// error
										cachingError = true;
									}
								}
							}

							bool addConceptToSatBranchedList = false;
							if (conceptDepInNodeOrSuccesssorBranched) {
								if (isConceptRelevantForSatisfiableBranch(individualNode,conDes,calcAlgContext)) {
									addConceptToSatBranchedList = true;
								}
							} else {
								notBranchConceptsSet.insert(conDes->getConceptTag());
								if (isCardinalityRestrictionCriticalForSatisfiable(individualNode,conDes,calcAlgContext)) {
									addConceptToSatBranchedList = true;
								}

								CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDCONCEPT;
								if (conDes->getNegation()) {
									cacheValueIdentifier = CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
								}
								expValueList->append(CCacheValue(conTag,(cint64)conDes->getConcept(),cacheValueIdentifier));
								satValueList->append(CCacheValue(conTag,(cint64)conDes->getConcept(),cacheValueIdentifier));
							}


							if (addConceptToSatBranchedList) {
								STATINC(SATEXPCACHEWRITINGSIGSATCRITICALBRANCHCONCEPTADDEDCOUNT,calcAlgContext);
								CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDCONCEPT;
								if (conDes->getNegation()) {
									cacheValueIdentifier = CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
								}
								branchedList->append(CCacheValue(conTag,(cint64)conDes->getConcept(),cacheValueIdentifier));
							}

							directBranched |= conceptDepInNodeOrSuccesssorBranched;



							if (!lastCachedConDesReached) {
								if (conDesIt == lastCachedConceptDes) {
									lastCachedConDesReached = true;
								}
							}
						}



						if (cachingError) {
							STATINC(SATEXPCACHEWRITINGERRORCOUNT,calcAlgContext);
							satStorData->setCachingError(true);
						} else {

							cint64 lastSig = lastConSetSignature;
							if (!satStorData->hasPreviousSatisfiableCached() && lastConSetSignature == conSetSignature) {
								lastSig = 0;
							}
							cint64 newSig = conSetSignature;

							STATINC(SATEXPCACHEWRITINGSIGNATUREEXPANSIONCOUNT,calcAlgContext);
							CSignatureSatisfiableExpanderCacheEntryExpandWriteData* expandWriteData = CObjectAllocator< CSignatureSatisfiableExpanderCacheEntryExpandWriteData >::allocateAndConstruct(expCacheMemMan);
							expandWriteData->initExpandWriteData(lastSig,newSig,expValueList,depHash);


							STATINC(SATEXPCACHEWRITINGSIGNATURESATISFIABLECOUNT,calcAlgContext);
							CSignatureSatisfiableExpanderCacheEntrySatisfiableBranchWriteData* satBranchWriteData = CObjectAllocator< CSignatureSatisfiableExpanderCacheEntrySatisfiableBranchWriteData >::allocateAndConstruct(satCacheMemMan);
							satBranchWriteData->initSatisfiableBranchWriteData(newSig,satValueList,branchedList);
							satBranchWriteData->setNext(expandWriteData);
							addCacheMessages(satBranchWriteData,calcAlgContext);


							satStorData->setPreviousCached(true);
							satStorData->setPreviousSatisfiableCached(true);
							satStorData->setLastCachedSignature(newSig);
							satStorData->setLastCachedConceptDescriptor(lastAddedConDes);
							satStorData->setIndividualNodeOrSuccessorBranchedConcept(directBranched);
							return true;
						}
					}
					return false;
				}









				bool CSatisfiableExpanderCacheHandler::cacheIndividualNodeExpansion(CIndividualProcessNode* individualNode, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(SATEXPCACHEWRITINGREQUESTCOUNT,calcAlgContext);
					STATINC(SATEXPCACHESATISFIABLEWRITINGREQUESTCOUNT,calcAlgContext);
					CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);
					mTestConSet = conSet;
					cint64 conSetSignature = conSet->getConceptSignatureValue();
					CIndividualNodeSatisfiableExpandingCacheStoringData* satStorData = (CIndividualNodeSatisfiableExpandingCacheStoringData*)individualNode->getIndividualSatisfiableCacheStoringData(false);
					if (satStorData) {
						if (satStorData->hasCachingError()) {
							return false;
						}
						if (satStorData->hasIndividualNodeOrSuccessorBranchedConcept()) {
							return false;
						}
						if (satStorData->getLastCachedSignature() == conSetSignature) {
							return false;
						}
					}

					CMemoryAllocationManager* memMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CIndividualNodeSatisfiableExpandingCacheStoringData* locSatStorData = (CIndividualNodeSatisfiableExpandingCacheStoringData*)individualNode->getIndividualSatisfiableCacheStoringData(true);
					if (locSatStorData && !satStorData) {
						// is probably caching by another thread
						return false;
					}
					if (!locSatStorData) {
						locSatStorData = CObjectAllocator<CIndividualNodeSatisfiableExpandingCacheStoringData>::allocateAndConstruct(taskMemMan);
						locSatStorData->initSatisfiableExpandingCacheRetrievalData(satStorData);
						satStorData = locSatStorData;
						individualNode->setIndividualSatisfiableCacheStoringData(locSatStorData);
					}


					CConceptDescriptor* lastAddedConDes = conSet->getAddingSortedConceptDescriptionLinker();
					CConceptDescriptor* lastCachedConceptDes = satStorData->getLastCachedConceptDescriptor();
					if (!satStorData->hasPreviousCached() && !lastCachedConceptDes) {
						satStorData->setLastCachedConceptDescriptor(lastAddedConDes);
						satStorData->setLastCachedSignature(conSetSignature);
						STATINC(SATEXPCACHEWRITINGEXPANSIONDELAYEDCOUNT,calcAlgContext);
						return true;
					}



					CSignatureSatisfiableExpanderCacheEntry* cachedEntry = mSatCacheReader->getCacheEntry(conSetSignature);
					if (cachedEntry) {
						STATINC(SATEXPCACHEWRITINGSIGNATUREALREADYCACHEDCOUNT,calcAlgContext);

						cint64 conSetCount = conSet->getConceptCount();
						cint64 expCount = cachedEntry->getExpanderCacheValueCount();
						if (expCount < conSetCount) {
							return false;
						}

						if (!compareIndividualNodeCompatibility(individualNode,cachedEntry,calcAlgContext)) {
							STATINC(SATEXPCACHEWRITINGSIGNATUREALREADYCACHEDINCOMPATIBLECOUNT,calcAlgContext);
							STATINC(SATEXPCACHEWRITINGERRORCOUNT,calcAlgContext);
							satStorData->setCachingError(true);
							return false;
						} else {
							satStorData->setLastCachedConceptDescriptor(lastAddedConDes);
							satStorData->setPreviousCached(true);
							satStorData->setLastCachedSignature(conSetSignature);
							return true;
						}
					}

					

					//CConceptDescriptor* testConDesIt = lastAddedConDes;
					//while (testConDesIt && testConDesIt->hasNext() && testConDesIt->getNextConceptDesciptor()->hasNext()) {
					//	testConDesIt = testConDesIt->getNextConceptDesciptor();
					//}
					//QString conString;
					//if (testConDesIt) {
					//	conString = CIRIName::getRecentIRIName(testConDesIt->getConcept()->getClassNameLinker());
					//	if (conString == "http://www.owllink.org/testsuite/galen#Diabetes" || conString == "http://www.owllink.org/testsuite/galen#ProperHepaticArtery") {
					//		bool bug = true;
					//	}
					//}


					// check whether to restart caching
					bool directBranched = false;
					bool dependencyToAncestor = false;
					CConceptDescriptor* conDesIt = lastAddedConDes;
					while (conDesIt != lastCachedConceptDes) {
						CConceptDescriptor* conDes = conDesIt;
						CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
						if (hasDependencyToAncestor(individualNode,depTrackPoint,&directBranched,calcAlgContext)) {
							dependencyToAncestor = true;
						}
						conDesIt = conDesIt->getNextConceptDesciptor();
					}

					if (dependencyToAncestor) {
						satStorData->setPreviousCached(false);
						satStorData->setPreviousSatisfiableCached(false);
						satStorData->setLastCachedSignature(conSetSignature);
						satStorData->setLastCachedConceptDescriptor(lastAddedConDes);
						return true;
					}
					if (directBranched) {
						satStorData->setIndividualNodeOrSuccessorBranchedConcept(true);
						return false;
					}

					cint64 lastConSetSignature = satStorData->getLastCachedSignature();
					if (!satStorData->hasPreviousCached() && lastCachedConceptDes && lastConSetSignature != 0) {
						cacheIndividualNodeExpansion(individualNode,satStorData,lastCachedConceptDes,nullptr,lastConSetSignature,0,calcAlgContext);
					}
					if (!satStorData->hasCachingError() && !satStorData->hasIndividualNodeOrSuccessorBranchedConcept()) {
						cacheIndividualNodeExpansion(individualNode,satStorData,lastAddedConDes,lastCachedConceptDes,conSetSignature,lastConSetSignature,calcAlgContext);
					}

					//commitCacheMessages(calcAlgContext);

					return false;
				}







				bool CSatisfiableExpanderCacheHandler::cacheIndividualNodeExpansion(CIndividualProcessNode* individualNode, CIndividualNodeSatisfiableExpandingCacheStoringData* satStorData, CConceptDescriptor* lastAddedConDes, CConceptDescriptor* lastCachedConceptDes, cint64 lastSignature, cint64 lastCachedSignature, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);


					// try caching
					prepareCacheMessages(calcAlgContext);
					CTaskMemoryPoolAllocationManager* satCacheMemMan = mMemAllocMan;
					CContextBase* tmpContext = mTmpContext;
					CCACHINGHASH<cint64,cint64>* depHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,cint64>,CContext* >::allocateAndConstructAndParameterize(satCacheMemMan,tmpContext);
					CCACHINGLIST<CCacheValue>* cacheValueList = CObjectParameterizingAllocator< CCACHINGLIST<CCacheValue>,CContext* >::allocateAndConstructAndParameterize(satCacheMemMan,tmpContext);

					CConceptDescriptor* conDesIt = lastAddedConDes;
					bool directBranched = false;
					bool cachingError = false;
					if (lastCachedConceptDes) {
						while (conDesIt != lastCachedConceptDes && !directBranched && !cachingError) {
							CConceptDescriptor* conDes = conDesIt;
							CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();

							cint64 conTag = conDes->getConceptTag();

							if (!simpleDependencyTracking(individualNode,conTag,depTrackPoint,depHash,nullptr,&directBranched,calcAlgContext)) {
								CPROCESSINGSET<CDepthDependencyPair> depSet(calcAlgContext->getTaskProcessorContext());
								CPROCESSINGLIST<CDepthDependencyPair> depList(calcAlgContext->getTaskProcessorContext());

								cint64 baseAncDepth = individualNode->getIndividualAncestorDepth();

								depSet.insert(CDepthDependencyPair(baseAncDepth,depTrackPoint));
								depList.append(CDepthDependencyPair(baseAncDepth,depTrackPoint));

								if (!complexDependencyTracking(individualNode,conTag,&depSet,&depList,depHash,nullptr,&directBranched,calcAlgContext)) {
									// error
									cachingError = true;
								}
							}

							CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDCONCEPT;
							if (conDes->getNegation()) {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
							}
							cacheValueList->prepend(CCacheValue(conTag,(cint64)conDes->getConcept(),cacheValueIdentifier));

							conDesIt = conDesIt->getNextConceptDesciptor();
						}
					}


					if (directBranched || cachingError) {
						if (directBranched) {
							satStorData->setIndividualNodeOrSuccessorBranchedConcept(true);
						}
						if (cachingError) {
							STATINC(SATEXPCACHEWRITINGERRORCOUNT,calcAlgContext);
							satStorData->setCachingError(true);
						}
					} else {

						while (conDesIt) {
							CConceptDescriptor* conDes = conDesIt;
							cint64 conTag = conDes->getConceptTag();

							CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDCONCEPT;
							if (conDes->getNegation()) {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
							}
							cacheValueList->prepend(CCacheValue(conTag,(cint64)conDes->getConcept(),cacheValueIdentifier));

							conDesIt = conDesIt->getNextConceptDesciptor();
						}

						cint64 lastSig = lastCachedSignature;
						cint64 newSig = lastSignature;

						STATINC(SATEXPCACHEWRITINGSIGNATUREEXPANSIONCOUNT,calcAlgContext);
						CSignatureSatisfiableExpanderCacheEntryExpandWriteData* expandWriteData = CObjectAllocator< CSignatureSatisfiableExpanderCacheEntryExpandWriteData >::allocateAndConstruct(satCacheMemMan);
						expandWriteData->initExpandWriteData(lastSig,newSig,cacheValueList,depHash);
						addCacheMessages(expandWriteData,calcAlgContext);

						satStorData->setPreviousCached(true);
						satStorData->setLastCachedSignature(newSig);
						satStorData->setLastCachedConceptDescriptor(lastAddedConDes);
						return true;
					}
					return false;				
				}








				bool CSatisfiableExpanderCacheHandler::prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (!mMemAllocMan) {
						CTaskMemoryPoolAllocationManager satCacheMemManCreaterMemMan(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&satCacheMemManCreaterMemMan);
						mMemAllocMan->initTaskMemoryPoolAllocationManager(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mMemAllocMan,mMemAllocMan);
						return true;
					}
					return false;
				}

				bool CSatisfiableExpanderCacheHandler::commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						CSignatureSatisfiableExpanderCacheEntryWriteData* commitWriteData = nullptr;
						CSignatureSatisfiableExpanderCacheEntryWriteData* writeDataIt = mWriteData;
						while (writeDataIt) {
							CSignatureSatisfiableExpanderCacheEntryWriteData* tmpWriteData = writeDataIt;
							writeDataIt = (CSignatureSatisfiableExpanderCacheEntryWriteData*)writeDataIt->getNext();
							tmpWriteData->clearNext();
							if (commitWriteData) {
								commitWriteData = (CSignatureSatisfiableExpanderCacheEntryWriteData*)tmpWriteData->append(commitWriteData);
							} else {
								commitWriteData = tmpWriteData;
							}
						}
						mSatCacheWriter->writeCachedData(commitWriteData,mMemPoolCon.takeMemoryPools());
						mWriteData = nullptr;
						mMemAllocMan = nullptr;
						mTmpContext = nullptr;
						return true;
					}
					return false;
				}



				bool CSatisfiableExpanderCacheHandler::addCacheMessages(CSignatureSatisfiableExpanderCacheEntryWriteData* expandWriteData, CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						mWriteData = (CSignatureSatisfiableExpanderCacheEntryWriteData*)expandWriteData->append(mWriteData);
					} else {
						mWriteData = expandWriteData;
					}
					//commitCacheMessages(calcAlgContext);
					return true;
				}





				bool CSatisfiableExpanderCacheHandler::isCardinalityRestrictionCriticalForSatisfiable(CIndividualProcessNode* individualNode, CConceptDescriptor* conDes, CCalculationAlgorithmContext* calcAlgContext) {
					CConcept* concept = conDes->getConcept();
					bool conNeg = conDes->getNegation();

					cint64 conCode = concept->getOperatorCode();
					if (!conNeg && (conCode == CCATMOST) || conNeg && (conCode == CCATLEAST)) {
						cint64 cardinality = concept->getParameter() - 1*conNeg;
						if (cardinality > 1) {
							CRole* role = concept->getRole();
							if (role) {
								cint64 succCount = individualNode->getRoleSuccessorCount(role);
								if (succCount >= cardinality) {
									return hasInverseSubRole(role,calcAlgContext);
								}
							}
						}
					} 
					return false;
				}



				bool CSatisfiableExpanderCacheHandler::hasInverseSubRole(CRole* role, CCalculationAlgorithmContext* calcAlgContext) {
					return true;
				}

				bool CSatisfiableExpanderCacheHandler::hasInverseSuperRole(CRole* role, CCalculationAlgorithmContext* calcAlgContext) {
					return true;
				}


				bool CSatisfiableExpanderCacheHandler::isConceptRelevantForSatisfiableBranch(CIndividualProcessNode* individualNode, CConceptDescriptor* conDes, CCalculationAlgorithmContext* calcAlgContext) {
					CConcept* concept = conDes->getConcept();
					bool conNeg = conDes->getNegation();

					cint64 conCode = concept->getOperatorCode();
					CConceptOperator* conOperator = concept->getConceptOperator();
					if (!conNeg && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE)) || conNeg && (conCode == CCSOME)) {
						CRole* role = concept->getRole();
						if (role) {							
							return hasInverseSubRole(role,calcAlgContext);
						}
					} 
					if (!conNeg && (conCode == CCATMOST) || conNeg && (conCode == CCATLEAST)) {
						CRole* role = concept->getRole();
						if (role) {							
							if (hasInverseSubRole(role,calcAlgContext)) {
								cint64 cardinality = concept->getParameter() - 1*conNeg;
								cint64 succCount = individualNode->getRoleSuccessorCount(role);
								if (succCount >= cardinality) {
									return true;
								}
								return false;
							} else {
								return false;
							}
						}
					} 
					if (!conNeg && (conCode == CCSOME || conCode == CCAQSOME || conCode == CCATLEAST) || conNeg && (conCode == CCALL || conCode == CCATMOST)) {
						CRole* role = concept->getRole();
						if (role) {
							// test has ATMOST restriction for some super role
							CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
							while (superRoleIt) {
								if (!superRoleIt->isNegated()) {
									CRole* superRole = superRoleIt->getData();
									CReapplyQueueIterator reapplyIt = individualNode->getRoleReapplyIterator(superRole,false);
									while (reapplyIt.hasNext()) {
										CReapplyConceptDescriptor* reapplyDes = reapplyIt.next();
										CConceptDescriptor* reapplyConDes = reapplyDes->getConceptDescriptor();
										CConcept* reapplyConcept = reapplyConDes->getConcept();
										CRole* reapplyRole = reapplyConcept->getRole();
										bool reapplyConNeg = reapplyConDes->getNegation();
										cint64 reapplyConOpCode = reapplyConcept->getOperatorCode();
										if (reapplyConNeg && reapplyConOpCode == CCATLEAST || !reapplyConNeg && reapplyConOpCode == CCATMOST) {
											cint64 cardinality = reapplyConcept->getParameter() - 1*reapplyConNeg;
											cint64 succCount = individualNode->getRoleSuccessorCount(role);
											if (succCount >= cardinality) {
												return hasInverseSubRole(reapplyRole,calcAlgContext);
											}
										}
									}
								}
								superRoleIt = superRoleIt->getNext();
							}
							return false;
						}
					}
					if (!conNeg && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE))) {
						return isAutomatConceptRelevantForSatisfiableBranch(individualNode,concept,conNeg,calcAlgContext);
					}
					return false;
				}



				bool CSatisfiableExpanderCacheHandler::isAutomatConceptRelevantForSatisfiableBranch(CIndividualProcessNode* individualNode, CConcept* concept, bool conNegation, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 conCode = concept->getOperatorCode();
					CConceptOperator* conOperator = concept->getConceptOperator();
					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQALL_TYPE)) {
						CRole* role = concept->getRole();
						if (role) {
							CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
							while (superRoleIt) {
								if (superRoleIt->isNegated()) {
									return true;
								}
								superRoleIt = superRoleIt->getNext();
							}
						}
					} else if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE)) {
						CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList();
						while (opConLinkerIt) {
							CConcept* opCon = opConLinkerIt->getData();
							bool opNeg = opConLinkerIt->isNegated();
							if (isAutomatConceptRelevantForSatisfiableBranch(individualNode,opCon,opNeg,calcAlgContext)) {
								return true;
							}
							opConLinkerIt = opConLinkerIt->getNext();
						}
					}
					return false;
				}



				bool CSatisfiableExpanderCacheHandler::simpleDependencyTracking(CIndividualProcessNode* individualNode, cint64 conTag, CDependencyTrackPoint* depTrackPoint, CCACHINGHASH<cint64,cint64>* depHash, CPROCESSINGSET<cint64>* notBranchConceptsSet, bool* branchedFlag, CCalculationAlgorithmContext* calcAlgContext) {
					if (branchedFlag && !depTrackPoint->isPointingToDeterministicDependencyNode()) {
						*branchedFlag = true;
						return true;
					}
					if (depTrackPoint->isPointingToIndependentDependencyNode()) {
						return true;
					}
					CDependencyNode* depNode = depTrackPoint->getDependencyNode();
					if (depNode->hasAdditionalDependencies()) {
						return false;
					}
					if (depNode->hasAppropriateIndividualNode()) {
						CIndividualProcessNode* appIndiNode = depNode->getAppropriateIndividualNode();
						cint64 appIndiDepth = appIndiNode->getIndividualAncestorDepth();
						cint64 baseDepth = individualNode->getIndividualAncestorDepth();
						if (appIndiDepth != baseDepth) {
							return false;
						}
					}
					CConceptDescriptor* conDes = depNode->getConceptDescriptor();
					if (conDes) {
						cint64 depConTag = conDes->getConceptTag();

						if (branchedFlag && notBranchConceptsSet && !notBranchConceptsSet->contains(conDes->getConceptTag())) {
							*branchedFlag = true;
							return true;
						}
						depHash->insertMulti(conTag,depConTag);



						//CConceptDescriptor* cD = nullptr;
						//CDependencyTrackPoint* dTP = nullptr;
						//if (individualNode->getReapplyConceptLabelSet(false)->getConceptDescriptor(conTag,cD,dTP)) {
						//	if (cD->isNegated() && cD->getConcept()->getOperatorCode() == CCSUB) {
						//		if (conDes->isNegated() && conDes->getConcept()->getOperatorCode() == CCSUB) {
						//			bool bug = true;
						//		}
						//	}
						//}
							

						//CConceptDescriptor* cD = nullptr;
						//CDependencyTrackPoint* dTP = nullptr;
						//if (!individualNode->getReapplyConceptLabelSet(false)->getConceptDescriptor(depConTag,cD,dTP)) {
						//	bool bug = true;
						//} else {
						//	CConceptDescriptor* conDesIt = cD;
						//	while (conDesIt) {
						//		if (conDesIt->getConceptTag() == conTag) {
						//			bool bug = true;												
						//		}
						//		conDesIt = conDesIt->getNext();
						//	}

						//	bool foundDepTag = false;
						//	conDesIt = conDes;
						//	while (conDesIt) {
						//		if (conDesIt->getConceptTag() == depConTag) {
						//			foundDepTag = true;												
						//		}
						//		conDesIt = conDesIt->getNext();
						//	}
						//	if (!foundDepTag) {
						//		bool bug = true;
						//	}
						//}

						return true;
					}
					return false;
				}



				bool CSatisfiableExpanderCacheHandler::complexDependencyTracking(CIndividualProcessNode* individualNode, cint64 conTag, CPROCESSINGSET<CDepthDependencyPair>* depSet, CPROCESSINGLIST<CDepthDependencyPair>* depList, CCACHINGHASH<cint64,cint64>* depHash, CPROCESSINGSET<cint64>* notBranchConceptsSet, bool* branchedFlag, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 baseAncDepth = individualNode->getIndividualAncestorDepth();
					while (!depList->isEmpty()) {
						CDepthDependencyPair depthDepPair(depList->takeFirst());
						cint64 ancDepth = depthDepPair.first;
						CDependencyTrackPoint* depTrackPoint = depthDepPair.second;

						if (branchedFlag && !depTrackPoint->isPointingToDeterministicDependencyNode()) {
							*branchedFlag = true;
							return true;
						}

						CDependencyNode* depNode = depTrackPoint->getDependencyNode();
						CIndividualProcessNode* appIndiNode = depNode->getAppropriateIndividualNode();
						cint64 newAncDepth = ancDepth;
						bool continueDepLoading = true;
						if (appIndiNode) {
							newAncDepth = appIndiNode->getIndividualAncestorDepth();
						}
						if (newAncDepth == baseAncDepth) {
							CConceptDescriptor* conDes = depNode->getConceptDescriptor();
							if (conDes) {
								if (branchedFlag && notBranchConceptsSet) {
									if (!notBranchConceptsSet->contains(conDes->getConceptTag())) {
										*branchedFlag = true;
										return true;
									}
								}
								continueDepLoading = false;
								cint64 depConTag = conDes->getConceptTag();
								if (depConTag != conTag) {
									depHash->insertMulti(conTag,depConTag);


									//CConceptDescriptor* cD = nullptr;
									//CDependencyTrackPoint* dTP = nullptr;
									//if (individualNode->getReapplyConceptLabelSet(false)->getConceptDescriptor(conTag,cD,dTP)) {
									//	if (cD->isNegated() && cD->getConcept()->getOperatorCode() == CCSUB) {
									//		if (conDes->isNegated() && conDes->getConcept()->getOperatorCode() == CCSUB) {
									//			bool bug = true;
									//		}
									//	}
									//}


									//CConceptDescriptor* cD = nullptr;
									//CDependencyTrackPoint* dTP = nullptr;
									//if (!individualNode->getReapplyConceptLabelSet(false)->getConceptDescriptor(depConTag,cD,dTP)) {
									//	bool bug = true;
									//} else {
									//	CConceptDescriptor* conDesIt = cD;
									//	while (conDesIt) {
									//		if (conDesIt->getConceptTag() == conTag) {
									//			bool bug = true;												
									//		}
									//		conDesIt = conDesIt->getNext();
									//	}

									//	bool foundDepTag = false;
									//	conDesIt = conDes;
									//	while (conDesIt) {
									//		if (conDesIt->getConceptTag() == depConTag) {
									//			foundDepTag = true;												
									//		}
									//		conDesIt = conDesIt->getNext();
									//	}
									//	if (!foundDepTag) {
									//		bool bug = true;
									//	}

									//}
								} 
							}
						}
						if (newAncDepth < baseAncDepth) {
							return false;
						}
						if (continueDepLoading) {
							CDependencyTrackPoint* prevDepTrackPoint = depNode->getPreviousDependencyTrackPoint();
							cint64 nextAncDepth = newAncDepth;
							CDependencyNode* nextDepNode = prevDepTrackPoint->getDependencyNode();
							if (nextDepNode && nextDepNode->hasAppropriateIndividualNode()) {
								nextAncDepth = nextDepNode->getAppropriateIndividualNode()->getIndividualAncestorDepth();
							}
							if (!depSet->contains(CDepthDependencyPair(nextAncDepth,prevDepTrackPoint))) {
								depSet->insert(CDepthDependencyPair(nextAncDepth,prevDepTrackPoint));
								depList->append(CDepthDependencyPair(nextAncDepth,prevDepTrackPoint));
							}
						}

						CDependencyIterator depIt = depNode->getAdditionalDependencyIterator();
						while (depIt.hasNext()) {
							CDependency* dependency = depIt.nextDependency();
							// load additional dependencies
							CDependencyTrackPoint* prevDepTrackPoint = dependency->getPreviousDependencyTrackPoint();
							cint64 nextAncDepth = ancDepth;
							CDependencyNode* nextDepNode = prevDepTrackPoint->getDependencyNode();
							if (nextDepNode && nextDepNode->hasAppropriateIndividualNode()) {
								nextAncDepth = nextDepNode->getAppropriateIndividualNode()->getIndividualAncestorDepth();
							}
							if (!depSet->contains(CDepthDependencyPair(ancDepth,prevDepTrackPoint))) {
								depSet->insert(CDepthDependencyPair(ancDepth,prevDepTrackPoint));
								depList->append(CDepthDependencyPair(ancDepth,prevDepTrackPoint));
							}
						}
					}
					return true;
				}



				bool CSatisfiableExpanderCacheHandler::hasDependencyToAncestor(CIndividualProcessNode* individualNode, CDependencyTrackPoint* depTrackPoint, bool* branchedFlag, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 ancDepth = individualNode->getIndividualAncestorDepth();
					bool dependencyToAncestor = false;
					if (ancDepth <= 0) {
						dependencyToAncestor = depTrackPoint->isPointingToIndependentDependencyNode();
					} else {
						CDependencyNode* depNode = depTrackPoint->getDependencyNode();
						if (depNode->hasAppropriateIndividualNode()) {
							CIndividualProcessNode* appIndiNode = depNode->getAppropriateIndividualNode();
							cint64 appIndiAncDepth = appIndiNode->getIndividualAncestorDepth();
							dependencyToAncestor = appIndiAncDepth < ancDepth;
						} else {
							CDependencyNode::DEPENDENCNODEYTYPE depType = depNode->getDependencyType();
							if (depType == CDependencyNode::DNTMERGEDCONCEPT) {
								dependencyToAncestor = hasDependencyToAncestor(individualNode,depNode->getPreviousDependencyTrackPoint(),branchedFlag,calcAlgContext);
							}
						}
					}
					if (!dependencyToAncestor && branchedFlag) {
						if (!depTrackPoint->isPointingToDeterministicDependencyNode()) {
							*branchedFlag = true;
						}
					}
					return dependencyToAncestor;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
