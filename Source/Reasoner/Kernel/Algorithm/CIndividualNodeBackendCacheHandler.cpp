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

#include "CIndividualNodeBackendCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CIndividualNodeBackendCacheHandler::CIndividualNodeBackendCacheHandler(CBackendRepresentativeMemoryCacheReader* backAssCacheReader, CBackendRepresentativeMemoryCacheWriter* backAssCacheWriter) : CBackendAssociationCacheHandler(backAssCacheReader, backAssCacheWriter) {
				}














				bool CIndividualNodeBackendCacheHandler::determineAssociationBackendCardinality(CIndividualProcessNode* indiNode, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {

					function<void(CPROCESSHASH<cint64, CRoleCardinalityCountData>* superRoleTagUsedCardCountHash)> initFunc = [&](CPROCESSHASH<cint64, CRoleCardinalityCountData>* superRoleTagUsedCardCountHash)->void {
						CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
						if (conSet) {
							for (CConceptDescriptor* conSatDesIt = conSet->getAddingSortedConceptDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
								CConcept* concept = conSatDesIt->getConcept();
								bool negation = conSatDesIt->isNegated();
								cint64 conOpCode = concept->getOperatorCode();
								if (!negation && conOpCode == CCATMOST || negation && conOpCode == CCATLEAST) {
									CRole* role = concept->getRole();
									cint64 cardinality = concept->getParameter();
									if (negation && conOpCode == CCATLEAST) {
										cardinality = cardinality - 1;
									}
									CRoleCardinalityCountData& cardCountData = (*superRoleTagUsedCardCountHash)[role->getRoleTag()];
									if (!cardCountData.mHasRestrictingCardinality) {
										cardCountData.mHasRestrictingCardinality = true;
										cardCountData.mRestrictingCardinality = cardinality;
									} else {
										cardCountData.mRestrictingCardinality = qMin(cardCountData.mRestrictingCardinality, cardinality);
									}
								}
							}
						}
					};
					return CBackendAssociationCacheHandler::determineCardinalityAssociationBackendItem(initFunc, roleUsedCardHash, false, tmpAssWriteDataLinker, calcAlgContext);
				}










				bool CIndividualNodeBackendCacheHandler::determineFullConceptSetLabelAssociationBackendItemWithSeparatedDeterministicNode(CIndividualProcessNode* extractionIndiNode, CIndividualProcessNode* deterministicConSetNode, CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = extractionIndiNode->getReapplyConceptLabelSet(false);
					if (!conSet) {
						return false;
					}

					CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = getIndividualAssociationDataFromIndividualNode(indiNode, calcAlgContext);

					cint64 maxDetBranchTag = calcAlgContext->getProcessingDataBox()->getMaximumDeterministicBranchTag();

					bool deterministic = false;
					bool excludePositiveNominalConcepts = true;

					bool nondeterministicElements = false;


					CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* signatureTmpConSetRefLabelHash = mSignatureTmpDetConSetRefLabelHash;
					if (!deterministic) {
						signatureTmpConSetRefLabelHash = mSignatureTmpNonDetConSetRefLabelHash;
					}

					function<bool(CConcept* concept, bool negation)> exclusionDetermineFunction = [&](CConcept* concept, bool negation)->bool {
						if (excludePositiveNominalConcepts && concept->getOperatorCode() == CCNOMINAL && !negation) {
							return false;
						}
						return true;
					};

					function<bool(CConcept* concept, bool negation, CDependencyTrackPoint* depTrackPoint)> deterministicDetermineFunction = [&](CConcept* concept, bool negation, CDependencyTrackPoint* depTrackPoint)->bool {
						bool deterministic = true;
						if (deterministicConSetNode != extractionIndiNode) {
							CConceptDescriptor* conDes = nullptr;
							CDependencyTrackPoint* detIndiDepTrackPoint = nullptr;
							if (deterministicConSetNode->getReapplyConceptLabelSet(false)->getConceptDescriptor(concept, conDes, detIndiDepTrackPoint)) {
								deterministic = true;
								if (!detIndiDepTrackPoint || detIndiDepTrackPoint->getBranchingTag() > maxDetBranchTag) {
									deterministic = false;
								}
							} else {
								deterministic = false;
							}
						} else {
							if (!depTrackPoint || depTrackPoint->getBranchingTag() > maxDetBranchTag) {
								deterministic = false;
							}
						}
						if (!deterministic) {
							nondeterministicElements = true;
						}
						return deterministic;
					};

					cint64 conCount = conSet->getConceptCount();
					cint64 conSetSigValue = CBackendRepresentativeMemoryCacheUtilities::getConceptDescriptorSignature(conSet->getAddingSortedConceptDescriptionLinker(), conCount, exclusionDetermineFunction, deterministicDetermineFunction);


					if (conCount > 0) {

						CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getFullConceptSetLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL, conSetSigValue, conCount, conSet->getAddingSortedConceptDescriptionLinker(), exclusionDetermineFunction, deterministicDetermineFunction);

						if (cachedLabelItem) {
							++mStatReusedCachedLabels;
							tmpAssWriteDataLinker->setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL, cachedLabelItem);
							return true;
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*& prevTempRefLabels = (*signatureTmpConSetRefLabelHash)[conSetSigValue];
						if (prevTempRefLabels) {
							for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* prevTempRefLabelIt = prevTempRefLabels; prevTempRefLabelIt; prevTempRefLabelIt = prevTempRefLabelIt->getNext()) {
								if (prevTempRefLabelIt->getCacheValueCount() == conCount) {
									bool compatibleConcepts = true;
									for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = prevTempRefLabelIt->getCacheValueLinker(); labelValueLinkerIt && compatibleConcepts; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
										CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
										CConcept* concept = (CConcept*)cacheValue.getIdentification();
										bool negation = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT || cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_NEGATED_CONCEPT;
										bool deterministic = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT || cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDCONCEPT;

										if (excludePositiveNominalConcepts && concept->getOperatorCode() == CCNOMINAL && !negation) {
											compatibleConcepts = false;
										}

										CConceptDescriptor* conDes = nullptr;
										CDependencyTrackPoint* depTrackPoint = nullptr;
										if (!conSet->getConceptDescriptor(concept, conDes, depTrackPoint)) {
											compatibleConcepts = false;
										} else if (negation != conDes->isNegated()) {
											compatibleConcepts = false;
										} else if (deterministicDetermineFunction(concept, negation, depTrackPoint) != deterministic) {
											compatibleConcepts = false;
										}
									}
									if (compatibleConcepts) {
										++mStatReusedHashedSignatureLabels;
										++mStatReusedTempLabels;
										tmpAssWriteDataLinker->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL, prevTempRefLabelIt);
										return true;
									}
								}
							}
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
						newTempRefLabel->initLabelWriteData(conSetSigValue, CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
						newTempRefLabel->setCompletelySaturated(false);
						newTempRefLabel->setCompletelyHandled(true);
						for (CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
							CConcept* concept = conDesIt->getConcept();
							bool negation = conDesIt->isNegated();

							bool considerConcept = exclusionDetermineFunction(concept, negation);
							if (considerConcept) {
								bool deterministic = deterministicDetermineFunction(concept, negation, conDesIt->getDependencyTrackPoint());
								CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
								labelValueLinker->initLabelValueLinker(getCacheValue(concept, negation, deterministic));
								newTempRefLabel->appendCacheValueLinker(labelValueLinker);
							}
						}
						newTempRefLabel->setNondeterministicElements(nondeterministicElements);
						mNewTmpLabelList->append(newTempRefLabel);
						++mStatCreatedTempLabels;

						tmpAssWriteDataLinker->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL, newTempRefLabel);						
						prevTempRefLabels = newTempRefLabel->append(prevTempRefLabels);
						return true;
					} else {
						return false;
					}
				}






















				bool CIndividualNodeBackendCacheHandler::determineNondeterministicConceptSetLabelAssociationBackendItemWithExcludedConceptsFromDeterministicLabel(CIndividualProcessNode* extractionIndiNode, CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = extractionIndiNode->getReapplyConceptLabelSet(false);
					if (!conSet) {
						return false;
					}

					CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = getIndividualAssociationDataFromIndividualNode(indiNode, calcAlgContext);
					

					bool deterministic = false;
					bool excludePositiveNominalConcepts = true;

					CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* signatureTmpConSetRefLabelHash = mSignatureTmpDetConSetRefLabelHash;
					if (!deterministic) {
						signatureTmpConSetRefLabelHash = mSignatureTmpNonDetConSetRefLabelHash;
					}

					function<bool(CConcept* concept, bool negation)> exclusionDetermineFunction = [&](CConcept* concept, bool negation)->bool {
						if (excludePositiveNominalConcepts && concept->getOperatorCode() == CCNOMINAL && !negation) {
							return false;
						}
						if (hasConceptInAssociatedDeterministicConceptSetLabel(indiAssData, concept, negation, calcAlgContext)) {
							return false;
						}
						return true;
					};

					cint64 conCount = conSet->getConceptCount();
					cint64 conSetSigValue = CBackendRepresentativeMemoryCacheUtilities::getConceptDescriptorSignature(conSet->getAddingSortedConceptDescriptionLinker(), conCount, exclusionDetermineFunction);


					if (conCount > 0) {

						CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getConceptSetLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL, conSetSigValue, conCount, conSet->getAddingSortedConceptDescriptionLinker(), exclusionDetermineFunction);

						if (cachedLabelItem) {
							++mStatReusedCachedLabels;
							tmpAssWriteDataLinker->setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL, cachedLabelItem);
							return true;
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*& prevTempRefLabels = (*signatureTmpConSetRefLabelHash)[conSetSigValue];
						if (prevTempRefLabels) {
							for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* prevTempRefLabelIt = prevTempRefLabels; prevTempRefLabelIt; prevTempRefLabelIt = prevTempRefLabelIt->getNext()) {
								if (prevTempRefLabelIt->getCacheValueCount() == conCount) {
									bool compatibleConcepts = true;
									for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = prevTempRefLabelIt->getCacheValueLinker(); labelValueLinkerIt && compatibleConcepts; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
										CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
										CConcept* concept = (CConcept*)cacheValue.getIdentification();
										bool negation = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;

										if (excludePositiveNominalConcepts && concept->getOperatorCode() == CCNOMINAL && !negation) {
											compatibleConcepts = false;
										}
										if (hasConceptInAssociatedDeterministicConceptSetLabel(indiAssData, concept, negation, calcAlgContext)) {
											compatibleConcepts = false;
										}
									}
									if (compatibleConcepts) {
										++mStatReusedHashedSignatureLabels;
										++mStatReusedTempLabels;
										tmpAssWriteDataLinker->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL, prevTempRefLabelIt);
										return true;
									}
								}
							}
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
						newTempRefLabel->initLabelWriteData(conSetSigValue, CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL);
						newTempRefLabel->setCompletelySaturated(false);
						newTempRefLabel->setCompletelyHandled(true);
						for (CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
							CConcept* concept = conDesIt->getConcept();
							bool negation = conDesIt->isNegated();

							bool considerConcept = exclusionDetermineFunction(concept, negation);
							if (considerConcept) {
								CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
								labelValueLinker->initLabelValueLinker(getCacheValue(concept, negation));
								newTempRefLabel->appendCacheValueLinker(labelValueLinker);
							}
						}
						mNewTmpLabelList->append(newTempRefLabel);
						++mStatCreatedTempLabels;

						tmpAssWriteDataLinker->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL, newTempRefLabel);
						prevTempRefLabels = newTempRefLabel->append(prevTempRefLabels);
						return true;
					} else {
						return false;
					}
				}



				bool CIndividualNodeBackendCacheHandler::determineConceptSetLabelAssociationBackendItem(CIndividualProcessNode* indiNode, cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
					if (!conSet) {
						return false;
					}

					bool deterministic = labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_CONCEPT_SET_LABEL;
					bool excludePositiveNominalConcepts = true;

					CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* signatureTmpConSetRefLabelHash = mSignatureTmpDetConSetRefLabelHash;
					if (!deterministic) {
						signatureTmpConSetRefLabelHash = mSignatureTmpNonDetConSetRefLabelHash;
					}

					cint64 conCount = conSet->getConceptCount();
					cint64 maxDetBranchTag = calcAlgContext->getProcessingDataBox()->getMaximumDeterministicBranchTag();
					cint64 conSetSigValue = CBackendRepresentativeMemoryCacheUtilities::getConceptDescriptorSignature(conSet->getAddingSortedConceptDescriptionLinker(), conCount, deterministic, maxDetBranchTag, excludePositiveNominalConcepts);

					if (conCount > 0) {

						CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getConceptSetLabelCacheEntry(conSetSigValue, conCount, conSet->getAddingSortedConceptDescriptionLinker(), deterministic, maxDetBranchTag, excludePositiveNominalConcepts);
						if (cachedLabelItem) {
							++mStatReusedCachedLabels;
							tmpAssWriteDataLinker->setReferredLabelData(labelType, cachedLabelItem);
							return true;
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*& prevTempRefLabels = (*signatureTmpConSetRefLabelHash)[conSetSigValue];
						if (prevTempRefLabels) {
							for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* prevTempRefLabelIt = prevTempRefLabels; prevTempRefLabelIt; prevTempRefLabelIt = prevTempRefLabelIt->getNext()) {
								if (prevTempRefLabelIt->getCacheValueCount() == conCount) {
									bool compatibleConcepts = true;
									for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = prevTempRefLabelIt->getCacheValueLinker(); labelValueLinkerIt && compatibleConcepts; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
										CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
										CConcept* concept = (CConcept*)cacheValue.getIdentification();
										bool negation = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
										if (excludePositiveNominalConcepts && concept->getOperatorCode() == CCNOMINAL && !negation) {
											compatibleConcepts = false;
										}


										CConceptDescriptor* conDes = nullptr;
										CDependencyTrackPoint* depTrackPoint = nullptr;
										if (!conSet->getConceptDescriptor(concept, conDes, depTrackPoint)) {
											compatibleConcepts = false;
										} else {
											if (conDes->isNegated() != negation) {
												compatibleConcepts = false;
											}

											bool deterministicallyDerived = true;
											if (!depTrackPoint || depTrackPoint->getBranchingTag() > maxDetBranchTag) {
												deterministicallyDerived = false;
											}

											if (deterministicallyDerived != deterministic) {
												compatibleConcepts = false;
											}
										}
									}
									if (compatibleConcepts) {
										++mStatReusedHashedSignatureLabels;
										++mStatReusedTempLabels;
										tmpAssWriteDataLinker->setReferredTemporaryLabelData(labelType, prevTempRefLabelIt);
										return true;
									}
								}
							}
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
						newTempRefLabel->initLabelWriteData(conSetSigValue, labelType);
						newTempRefLabel->setCompletelySaturated(false);
						newTempRefLabel->setCompletelyHandled(true);
						for (CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
							CConcept* concept = conDesIt->getConcept();
							bool negation = conDesIt->isNegated();

							bool deterministicallyDerived = true;
							if (!conDesIt->getDependencyTrackPoint() || conDesIt->getDependencyTrackPoint()->getBranchingTag() > maxDetBranchTag) {
								deterministicallyDerived = false;
							}
							bool considerConcept = deterministicallyDerived == deterministic;
							if (excludePositiveNominalConcepts && concept->getOperatorCode() == CCNOMINAL && !negation) {
								considerConcept = false;
							}

							if (considerConcept) {
								CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
								labelValueLinker->initLabelValueLinker(getCacheValue(concept, negation));
								newTempRefLabel->appendCacheValueLinker(labelValueLinker);
							}
						}
						mNewTmpLabelList->append(newTempRefLabel);
						++mStatCreatedTempLabels;

						tmpAssWriteDataLinker->setReferredTemporaryLabelData(labelType, newTempRefLabel);
						prevTempRefLabels = newTempRefLabel->append(prevTempRefLabels);
						return true;
					} else {
						return false;
					}
				}




















				bool CIndividualNodeBackendCacheHandler::determineSameIndividualSetLabelAssociationBackendItem(CIndividualProcessNode* indiNode, cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, bool* integratedAllMergingsFlag, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualMergingHash* indiMergingHash = indiNode->getIndividualMergingHash(false);
					if (!indiMergingHash) {
						if (integratedAllMergingsFlag) {
							*integratedAllMergingsFlag = true;
						}
						return false;
					}
					cint64 indiId = indiNode->getNominalIndividual()->getIndividualID();

					bool deterministic = labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL;

					CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* signatureTmpIndiSetRefLabelHash = mSignatureTmpDetSameIndiSetRefLabelHash;
					if (!deterministic) {
						signatureTmpIndiSetRefLabelHash = mSignatureTmpNonDetSameIndiSetRefLabelHash;
					}

					cint64 indiCount = indiMergingHash->getMergedIndividualCount() + 1;
					cint64 maxDetBranchTag = calcAlgContext->getProcessingDataBox()->getMaximumDeterministicBranchTag();

					cint64 indiSetSigValue = CBackendRepresentativeMemoryCacheUtilities::getIndividualSetSignature(indiId, indiMergingHash, indiCount, deterministic, maxDetBranchTag);

					if (integratedAllMergingsFlag) {
						*integratedAllMergingsFlag = indiCount == indiMergingHash->getMergedIndividualCount() + 1;
					}

					if (indiCount > 0) {

						CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getIndividualSetLabelCacheEntry(labelType, indiSetSigValue, indiId, indiMergingHash, indiCount, deterministic, maxDetBranchTag);
						if (cachedLabelItem) {
							++mStatReusedCachedLabels;
							tmpAssWriteDataLinker->setReferredLabelData(labelType, cachedLabelItem);
							return true;
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*& prevTempRefLabels = (*signatureTmpIndiSetRefLabelHash)[indiSetSigValue];
						if (prevTempRefLabels) {
							for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* prevTempRefLabelIt = prevTempRefLabels; prevTempRefLabelIt; prevTempRefLabelIt = prevTempRefLabelIt->getNext()) {
								if (prevTempRefLabelIt->getCacheValueCount() == indiCount) {
									bool compatibleLabels = true;
									for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = prevTempRefLabelIt->getCacheValueLinker(); labelValueLinkerIt && compatibleLabels; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
										CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
										cint64 mergedIndiId = cacheValue.getTag();

										if (indiId != mergedIndiId) {
											if (indiMergingHash->hasMergedIndividual(mergedIndiId)) {
												if (!deterministic) {
													CIndividualMergingHashData data = indiMergingHash->value(mergedIndiId);
													if (!data.getDependencyTrackPoint() || data.getDependencyTrackPoint()->getBranchingTag() > maxDetBranchTag) {
														compatibleLabels = false;
													}
												}
											} else {
												compatibleLabels = false;
											}
										}

									}
									if (compatibleLabels) {
										++mStatReusedHashedSignatureLabels;
										++mStatReusedTempLabels;
										tmpAssWriteDataLinker->setReferredTemporaryLabelData(labelType, prevTempRefLabelIt);
										return true;
									}
								}
							}
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
						newTempRefLabel->initLabelWriteData(indiSetSigValue, labelType);
						newTempRefLabel->setCompletelySaturated(false);
						newTempRefLabel->setCompletelyHandled(true);
						for (CIndividualMergingHash::const_iterator it = indiMergingHash->constBegin(), itEnd = indiMergingHash->constEnd(); it != itEnd; ++it) {
							if (it.value().isMergedWithIndividual()) {
								cint64 mergedIndiId = it.key();
								if (mergedIndiId != indiId) {
									CIndividualMergingHashData mergedData = it.value();
									bool deterministicallyDerived = true;
									if (!mergedData.getDependencyTrackPoint() || mergedData.getDependencyTrackPoint()->getBranchingTag() > maxDetBranchTag) {
										deterministicallyDerived = false;
									}
									bool considerIndividual = deterministicallyDerived || !deterministic;
									if (considerIndividual) {
										CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
										labelValueLinker->initLabelValueLinker(mAssBackCacheReader->getCacheValue(mergedIndiId, false));
										newTempRefLabel->appendCacheValueLinker(labelValueLinker);
									}
								}
							}
						}
						if (indiId >= 0) {
							CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
							labelValueLinker->initLabelValueLinker(mAssBackCacheReader->getCacheValue(indiId, false));
							newTempRefLabel->appendCacheValueLinker(labelValueLinker);
						}
						mNewTmpLabelList->append(newTempRefLabel);
						++mStatCreatedTempLabels;

						tmpAssWriteDataLinker->setReferredTemporaryLabelData(labelType, newTempRefLabel);
						prevTempRefLabels = newTempRefLabel->append(prevTempRefLabels);
						return true;
					} else {
						return false;
					}
				}
















				bool CIndividualNodeBackendCacheHandler::determineDifferentIndividualSetLabelAssociationBackendItem(CIndividualProcessNode* indiNode, cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, bool* integratedAllDifferentFlag, CCalculationAlgorithmContext* calcAlgContext) {
					CDistinctHash* indiDistinctHash = indiNode->getDistinctHash(false);
					if (!indiDistinctHash) {
						if (integratedAllDifferentFlag) {
							*integratedAllDifferentFlag = true;
						}
						return false;
					}
					cint64 indiId = indiNode->getNominalIndividual()->getIndividualID();

					bool deterministic = labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL;

					CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >* signatureTmpIndiSetRefLabelHash = mSignatureTmpDetDiffIndiSetRefLabelHash;
					if (!deterministic) {
						signatureTmpIndiSetRefLabelHash = mSignatureTmpNonDetDiffIndiSetRefLabelHash;
					}

					cint64 indiCount = indiDistinctHash->size() + 1;
					cint64 maxDetBranchTag = calcAlgContext->getProcessingDataBox()->getMaximumDeterministicBranchTag();

					cint64 indiSetSigValue = CBackendRepresentativeMemoryCacheUtilities::getIndividualSetSignature(indiId, indiDistinctHash, indiCount, deterministic, maxDetBranchTag);

					if (integratedAllDifferentFlag) {
						*integratedAllDifferentFlag = indiCount == indiDistinctHash->size() + 1;
					}

					if (indiCount > 0) {

						CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getIndividualSetLabelCacheEntry(labelType, indiSetSigValue, indiId, indiDistinctHash, indiCount, deterministic, maxDetBranchTag);
						if (cachedLabelItem) {
							++mStatReusedCachedLabels;
							tmpAssWriteDataLinker->setReferredLabelData(labelType, cachedLabelItem);
							return true;
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*& prevTempRefLabels = (*signatureTmpIndiSetRefLabelHash)[indiSetSigValue];
						if (prevTempRefLabels) {
							for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* prevTempRefLabelIt = prevTempRefLabels; prevTempRefLabelIt; prevTempRefLabelIt = prevTempRefLabelIt->getNext()) {
								if (prevTempRefLabelIt->getCacheValueCount() == indiCount) {
									bool compatibleLabels = true;
									for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = prevTempRefLabelIt->getCacheValueLinker(); labelValueLinkerIt && compatibleLabels; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
										CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
										cint64 distinctIndiId = cacheValue.getTag();

										if (distinctIndiId >= 0 && indiId != distinctIndiId) {
											if (indiDistinctHash->contains(-distinctIndiId)) {
												if (!deterministic) {
													CDistinctEdge* distinctEdge = indiDistinctHash->value(-distinctIndiId);
													if (!distinctEdge->getDependencyTrackPoint() || distinctEdge->getDependencyTrackPoint()->getBranchingTag() > maxDetBranchTag) {
														compatibleLabels = false;
													}
												}
											}
											else {
												compatibleLabels = false;
											}
										}

									}
									if (compatibleLabels) {
										++mStatReusedHashedSignatureLabels;
										++mStatReusedTempLabels;
										tmpAssWriteDataLinker->setReferredTemporaryLabelData(labelType, prevTempRefLabelIt);
										return true;
									}
								}
							}
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
						newTempRefLabel->initLabelWriteData(indiSetSigValue, labelType);
						newTempRefLabel->setCompletelySaturated(false);
						newTempRefLabel->setCompletelyHandled(true);
						for (CDistinctHash::const_iterator it = indiDistinctHash->constBegin(), itEnd = indiDistinctHash->constEnd(); it != itEnd; ++it) {
							cint64 distinctIndiId = -it.key();
							if (distinctIndiId >= 0 && distinctIndiId != indiId) {
								CDistinctEdge* distinctEdge = it.value();
								bool deterministicallyDerived = true;
								if (!distinctEdge->getDependencyTrackPoint() || distinctEdge->getDependencyTrackPoint()->getBranchingTag() > maxDetBranchTag) {
									deterministicallyDerived = false;
								}
								bool considerIndividual = deterministicallyDerived || !deterministic;
								if (considerIndividual) {
									CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
									labelValueLinker->initLabelValueLinker(mAssBackCacheReader->getCacheValue(distinctIndiId, false));
									newTempRefLabel->appendCacheValueLinker(labelValueLinker);
								}
							}
						}
						if (indiId >= 0) {
							CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
							labelValueLinker->initLabelValueLinker(mAssBackCacheReader->getCacheValue(indiId, false));
							newTempRefLabel->appendCacheValueLinker(labelValueLinker);
						}
						mNewTmpLabelList->append(newTempRefLabel);
						++mStatCreatedTempLabels;

						tmpAssWriteDataLinker->setReferredTemporaryLabelData(labelType, newTempRefLabel);
						prevTempRefLabels = newTempRefLabel->append(prevTempRefLabels);
						return true;
					}
					else {
						return false;
					}
				}













































				CIndividualProcessNode* CIndividualNodeBackendCacheHandler::getSuccessorIndividual(CIndividualProcessNode*& indi, CIndividualLinkEdge* link, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualProcessNode* succIndi = nullptr;

					if (link->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag())) {
						succIndi = link->getOppositeIndividual(indi);
					} else {
						succIndi = link->getOppositeIndividual(indi);
						if (!succIndi->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag()) && succIndi->isRelocalized()) {
							cint64 succIndiId = link->getOppositeIndividualID(indi);
							CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
							succIndi = indiProcNodeVec->getData(succIndiId);
						}
					}
					return succIndi;
				}



				void CIndividualNodeBackendCacheHandler::collectRoleSuccessorData(CIndividualLinkEdge* link, bool inversed, cint64 maxDetBranchTag, bool isNominalNeighbour, bool isDataNeighbour, CPROCESSSET<TRoleInversionPair>* combinedNeighbourDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedNeighbourDetRoleInstantiatedLinker,
						CPROCESSSET<TRoleInversionPair>* combinedNeighbourNonDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedNeighbourNonDetRoleInstantiatedLinker, CPROCESSSET<TRoleInversionPair>* combinedExistentialDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedExistentialDetRoleInstantiatedLinker, 
					CPROCESSSET<TRoleInversionPair>* combinedExistentialNonDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedExistentialNonDetRoleInstantiatedLinker, CPROCESSSET<TRoleInversionPair>* combinedDataDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedDataDetRoleInstantiatedLinker,
					CPROCESSSET<TRoleInversionPair>* combinedDataNonDetRoleInstantiatedSet, CSortedNegLinker<CRole *>*& combinedDataNonDetRoleInstantiatedLinker, CRole* role, CCalculationAlgorithmContext* calcAlgContext, bool &collected, CIndividualProcessNode* indiNode, CIndividualProcessNode* connIndiNode,
						cint64 connIndiMergedNominalId, bool connIndiNominalDeterministicallyMerged, CBackendRepresentativeMemoryCacheRoleAssertionLinker* &roleAssertionLinker, CPROCESSSET<cint64>** existentialIndirectlyConnectedNominalIndividualSet, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash) {

					bool deterministicallyDerived = connIndiNominalDeterministicallyMerged;
					CDependencyTrackPoint* lindDepTrackPoint = link->getDependencyTrackPoint();
					if (!lindDepTrackPoint || lindDepTrackPoint->getBranchingTag() > maxDetBranchTag) {
						deterministicallyDerived = false;
					}

					CPROCESSSET<TRoleInversionPair>** useRoleInstantiatedSet = nullptr;
					CSortedNegLinker<CRole*>** useRoleInstantiatedLinker = nullptr;
					if (deterministicallyDerived && isNominalNeighbour) {
						useRoleInstantiatedSet = &combinedNeighbourDetRoleInstantiatedSet;
						useRoleInstantiatedLinker = &combinedNeighbourDetRoleInstantiatedLinker;
					} else if (!deterministicallyDerived && isNominalNeighbour) {
						useRoleInstantiatedSet = &combinedNeighbourNonDetRoleInstantiatedSet;
						useRoleInstantiatedLinker = &combinedNeighbourNonDetRoleInstantiatedLinker;
					} else if (deterministicallyDerived && !isNominalNeighbour && !isDataNeighbour) {
						useRoleInstantiatedSet = &combinedExistentialDetRoleInstantiatedSet;
						useRoleInstantiatedLinker = &combinedExistentialDetRoleInstantiatedLinker;
					} else if (!deterministicallyDerived && !isNominalNeighbour && !isDataNeighbour) {
						useRoleInstantiatedSet = &combinedExistentialNonDetRoleInstantiatedSet;
						useRoleInstantiatedLinker = &combinedExistentialNonDetRoleInstantiatedLinker;
					} else if (deterministicallyDerived && isDataNeighbour) {
						useRoleInstantiatedSet = &combinedDataDetRoleInstantiatedSet;
						useRoleInstantiatedLinker = &combinedDataDetRoleInstantiatedLinker;
					} else if (!deterministicallyDerived && isDataNeighbour) {
						useRoleInstantiatedSet = &combinedDataNonDetRoleInstantiatedSet;
						useRoleInstantiatedLinker = &combinedExistentialNonDetRoleInstantiatedLinker;
					}

					if (useRoleInstantiatedSet && !(*useRoleInstantiatedSet)->contains(TRoleInversionPair(role, inversed))) {
						(*useRoleInstantiatedSet)->insert(TRoleInversionPair(role, inversed));
						CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
						*useRoleInstantiatedLinker = newRoleLinker->init(role, inversed, *useRoleInstantiatedLinker);
						collected = true;
					}


					if (isNominalNeighbour) {
						bool isAssertionBase = false;
						if (lindDepTrackPoint && lindDepTrackPoint->getDependencyNode()->getDependencyType() == CDependencyNode::DNTROLEASSERTIONDEPENDENCY) {
							isAssertionBase = true;

							CROLEASSERTIONDependencyNode* assertionDepNode = (CROLEASSERTIONDependencyNode*)lindDepTrackPoint->getDependencyNode();
							CRole* baseAssertionRole = assertionDepNode->getBaseAssertionRole();
							CIndividual* baseAssertionIndi = assertionDepNode->getBaseAssertionIndividual();
							bool baseAssertionRoleInversed = true;
							if (baseAssertionIndi->getIndividualID() == indiNode->getNominalIndividual()->getIndividualID()) {
								baseAssertionRoleInversed = false;
							} else {
								CIndividualMergingHash* mergingHash = indiNode->getIndividualMergingHash(false);
								if (mergingHash && mergingHash->hasMergedIndividual(baseAssertionIndi->getIndividualID())) {
									baseAssertionRoleInversed = false;
								}
							}

							CBackendRepresentativeMemoryCacheRoleAssertionLinker* linker = CObjectAllocator< CBackendRepresentativeMemoryCacheRoleAssertionLinker >::allocateAndConstruct(calcAlgContext->getMemoryAllocationManager());
							linker->initRoleAssertionLinker(role, inversed, baseAssertionRole == role && baseAssertionRoleInversed == inversed, false, !deterministicallyDerived);
							roleAssertionLinker = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)linker->append(roleAssertionLinker);

						} else if (lindDepTrackPoint && lindDepTrackPoint->getDependencyNode()->getDependencyType() == CDependencyNode::DNTVALUEDEPENDENCY) {
							CVALUEDependencyNode* valuDepNode = (CVALUEDependencyNode*)lindDepTrackPoint->getDependencyNode();
							CIndividualProcessNode* valueIndiNode = valuDepNode->getAppropriateIndividualNode();
							CConceptDescriptor* valueConDes = valuDepNode->getConceptDescriptor();
							if (valueConDes && valueConDes->getConcept()->getRole() && valueIndiNode && valueIndiNode->getNominalIndividual()) {

								CRole* baseValueRole = valueConDes->getConcept()->getRole();
								CIndividual* baseValueIndi = valueIndiNode->getNominalIndividual();
								bool baseValueRoleInversed = true;
								if (baseValueIndi->getIndividualID() == indiNode->getNominalIndividual()->getIndividualID()) {
									baseValueRoleInversed = false;
								} else {
									CIndividualMergingHash* mergingHash = indiNode->getIndividualMergingHash(false);
									if (mergingHash && mergingHash->hasMergedIndividual(baseValueIndi->getIndividualID())) {
										baseValueRoleInversed = false;
									}
								}

								CBackendRepresentativeMemoryCacheRoleAssertionLinker* linker = CObjectAllocator< CBackendRepresentativeMemoryCacheRoleAssertionLinker >::allocateAndConstruct(calcAlgContext->getMemoryAllocationManager());
								linker->initRoleAssertionLinker(role, inversed, false, baseValueRole == role && baseValueRoleInversed == inversed, !deterministicallyDerived);
								roleAssertionLinker = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)linker->append(roleAssertionLinker);

							} else {
								CBackendRepresentativeMemoryCacheRoleAssertionLinker* linker = CObjectAllocator< CBackendRepresentativeMemoryCacheRoleAssertionLinker >::allocateAndConstruct(calcAlgContext->getMemoryAllocationManager());
								linker->initRoleAssertionLinker(role, inversed, false, true, !deterministicallyDerived);
								roleAssertionLinker = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)linker->append(roleAssertionLinker);
							}

						} else {
							CBackendRepresentativeMemoryCacheRoleAssertionLinker* linker = CObjectAllocator< CBackendRepresentativeMemoryCacheRoleAssertionLinker >::allocateAndConstruct(calcAlgContext->getMemoryAllocationManager());
							linker->initRoleAssertionLinker(role, inversed, false, true, !deterministicallyDerived);
							roleAssertionLinker = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)linker->append(roleAssertionLinker);
						}

					} else if (!isDataNeighbour) {
						if (roleUsedCardHash && !inversed) {
							(*roleUsedCardHash)[role] += 1;
						}
						if (connIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
							CSuccessorConnectedNominalSet* succConnNomSet = connIndiNode->getSuccessorNominalConnectionSet(false);
							if (succConnNomSet && existentialIndirectlyConnectedNominalIndividualSet) {
								if (!*existentialIndirectlyConnectedNominalIndividualSet) {
									*existentialIndirectlyConnectedNominalIndividualSet = CObjectParameterizingAllocator< CPROCESSSET<cint64>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
								}
								for (CSuccessorConnectedNominalSet::const_iterator it = succConnNomSet->constBegin(), itEnd = succConnNomSet->constEnd(); it != itEnd; ++it) {
									cint64 succConnNominalIndi = *it;
									if (succConnNominalIndi <= 0) {
										(*existentialIndirectlyConnectedNominalIndividualSet)->insert(-succConnNominalIndi);
									}
								}
							}
						}
					}
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CIndividualNodeBackendCacheHandler::getIndividualAssociationDataFromIndividualNode(CIndividualProcessNode* indiNode, CCalculationAlgorithmContext* calcAlgContext) {
					CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = nullptr;
					CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* backendSyncData = (CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData*)indiNode->getIndividualBackendCacheSynchronisationData(false);
					if (backendSyncData) {
						indiAssData = backendSyncData->getAssocitaionData();
					}		
					if (!indiAssData) {
						indiAssData = getIndividualAssociationData(indiNode->getNominalIndividual(), calcAlgContext);
					}
					return indiAssData;
				}





				bool CIndividualNodeBackendCacheHandler::visitMergedIndividualNodesAssociationData(CIndividualProcessNode* baseIndiNode, CBackendRepresentativeMemoryCacheIndividualAssociationData* excludeIndiAssData, CBackendRepresentativeMemoryLabelCacheItem* excludeIndiSetLabel, bool visitBaseIndiAssoData, cint64 maxDetBranchTag, function<bool(cint64 mergedIndiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* mergedIndiAssData, bool deterministicallyMerged)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					bool visited = false;
					bool continueVisiting = true;

					CBackendRepresentativeMemoryCacheIndividualAssociationData* baseIndiAssData = getIndividualAssociationDataFromIndividualNode(baseIndiNode, calcAlgContext);
					if (visitBaseIndiAssoData && baseIndiAssData) {
						visited = true;
						continueVisiting = visitFunc(baseIndiNode->getNominalIndividual()->getIndividualID(), baseIndiAssData, true);
					}
					CIndividualMergingHash* mergingHash = baseIndiNode->getIndividualMergingHash(false);
					if (mergingHash) {
						for (CIndividualMergingHash::const_iterator it = mergingHash->constBegin(), itEnd = mergingHash->constEnd(); it != itEnd && continueVisiting; ++it) {
							if (it.value().isMergedWithIndividual()) {
								cint64 mergedIndiId = it.key();
								if (!excludeIndiSetLabel || !hasIndividualIdsInAssociatedIndividualSetLabel(excludeIndiAssData, excludeIndiSetLabel, mergedIndiId)) {
									CBackendRepresentativeMemoryCacheIndividualAssociationData* mergedIndiAssData = getIndividualAssociationData(mergedIndiId, calcAlgContext);
									if (mergedIndiAssData) {
										visited = true;
										bool deterministicallyMerged = true;
										CIndividualMergingHashData data = it.value();
										if (!data.getDependencyTrackPoint() || data.getDependencyTrackPoint()->getBranchingTag() > maxDetBranchTag) {
											deterministicallyMerged = false;
										}
										continueVisiting = visitFunc(mergedIndiId, mergedIndiAssData, deterministicallyMerged);
									}
								}
							}
						}
					}
					return visited;
				}





				bool CIndividualNodeBackendCacheHandler::determineRoleInstantiatedSetLabelAssociationBackendItems(CIndividualProcessNode* extractionIndiNode, CIndividualProcessNode* indiNode, bool onlyNondeterministic, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CPROCESSSET<cint64>** existentialIndirectlyConnectedNominalIndividualSet, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, CCalculationAlgorithmContext* calcAlgContext) {

					bool updated = false;

					CPROCESSSET<TRoleInversionPair>* combinedExistentialDetRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CSortedNegLinker<CRole*>* combinedExistentialDetRoleInstantiatedLinker = nullptr;

					CPROCESSSET<TRoleInversionPair>* combinedExistentialNonDetRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CSortedNegLinker<CRole*>* combinedExistentialNonDetRoleInstantiatedLinker = nullptr;



					CPROCESSSET<TRoleInversionPair>* combinedDataDetRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CSortedNegLinker<CRole*>* combinedDataDetRoleInstantiatedLinker = nullptr;

					CPROCESSSET<TRoleInversionPair>* combinedDataNonDetRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CSortedNegLinker<CRole*>* combinedDataNonDetRoleInstantiatedLinker = nullptr;



					CPROCESSSET<TRoleInversionPair>* combinedNeighbourDetRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CSortedNegLinker<CRole*>* combinedNeighbourDetRoleInstantiatedLinker = nullptr;

					CPROCESSSET<TRoleInversionPair>* combinedNeighbourNonDetRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CSortedNegLinker<CRole*>* combinedNeighbourNonDetRoleInstantiatedLinker = nullptr;


					CPROCESSSET<CBackendRepresentativeMemoryCacheTemporaryLabelReference>* neigbourRoleInstantiatedSetCompination = CObjectParameterizingAllocator< CPROCESSSET<CBackendRepresentativeMemoryCacheTemporaryLabelReference>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());


					CPROCESSSET<cint64>* connIndiSet = CObjectParameterizingAllocator< CPROCESSSET<cint64>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					CSuccessorIterator succIt(extractionIndiNode->getSuccessorIterator());
					cint64 lastIndiId = CINT64_MAX;
					while (succIt.hasNext()) {
						CIndividualLinkEdge* link = succIt.nextLink(true);
						cint64 succIndiId = link->getOppositeIndividualID(extractionIndiNode);
						if (succIndiId != lastIndiId) {
							connIndiSet->insert(succIndiId);
							lastIndiId = succIndiId;
						}
					}
					CConnectionSuccessorSetIterator connSuccSetIt = extractionIndiNode->getConnectionSuccessorIterator();
					while (connSuccSetIt.hasNext()) {
						cint64 succConnId = connSuccSetIt.nextSuccessorConnectionID(true);
						connIndiSet->insert(succConnId);
					}


					bool collected = false;
					cint64 maxDetBranchTag = calcAlgContext->getProcessingDataBox()->getMaximumDeterministicBranchTag();
					if (onlyNondeterministic) {
						maxDetBranchTag = -1;
					}
					CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();


					// TODO: integrate/add association data of all merged indis
					CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = getIndividualAssociationDataFromIndividualNode(indiNode, calcAlgContext);

					CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateLinker = nullptr;

					CBackendRepresentativeMemoryLabelCacheItem* maxSameIndiSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
					if (!maxSameIndiSetLabel) {
						maxSameIndiSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
					}

					if (indiAssData && (indiAssData->isIndirectlyConnectedNominalIndividual() || extractionIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSYNCHRONIZEDBACKENDSUCCESSOREXPANSIONBLOCKED | CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED))) {

						visitMergedIndividualNodesAssociationData(extractionIndiNode, indiAssData, maxSameIndiSetLabel, true, maxDetBranchTag, [&](cint64 mergedIndiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* mergedIndiAssData, bool deterministicallyMerged)->bool {

							// reuse roles of existing existential data role instantiation sets
							visitRolesOfAssociatedCompinationRoleSetLabel(mergedIndiAssData, mergedIndiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL), [&](CRole* role, bool inversed)->bool {
								if (deterministicallyMerged && !onlyNondeterministic) {
									if (!combinedDataDetRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
										combinedDataDetRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
										CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
										combinedDataDetRoleInstantiatedLinker = newRoleLinker->init(role, inversed, combinedDataDetRoleInstantiatedLinker);
									}
								} else {
									if (!combinedDataNonDetRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
										combinedDataNonDetRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
										CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
										combinedDataNonDetRoleInstantiatedLinker = newRoleLinker->init(role, inversed, combinedDataNonDetRoleInstantiatedLinker);
									}
								}
								return true;
							}, calcAlgContext);


							// reuse roles of existing existential role instantiation sets
							visitRolesOfAssociatedCompinationRoleSetLabel(mergedIndiAssData, mergedIndiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL), [&](CRole* role, bool inversed)->bool {
								if (deterministicallyMerged && !onlyNondeterministic) {
									if (!combinedExistentialDetRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
										combinedExistentialDetRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
										CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
										combinedExistentialDetRoleInstantiatedLinker = newRoleLinker->init(role, inversed, combinedExistentialDetRoleInstantiatedLinker);
									}
								} else {
									if (!combinedExistentialNonDetRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
										combinedExistentialNonDetRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
										CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
										combinedExistentialNonDetRoleInstantiatedLinker = newRoleLinker->init(role, inversed, combinedExistentialNonDetRoleInstantiatedLinker);
									}
								}
								return true;
							}, calcAlgContext);



							visitRolesOfAssociatedCompinationRoleSetLabel(mergedIndiAssData, mergedIndiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL), [&](CRole* role, bool inversed)->bool {
								if (!combinedExistentialNonDetRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
									combinedExistentialNonDetRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
									CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
									combinedExistentialNonDetRoleInstantiatedLinker = newRoleLinker->init(role, inversed, combinedExistentialNonDetRoleInstantiatedLinker);
								}
								return true;
							}, calcAlgContext);


							visitRolesOfAssociatedCompinationRoleSetLabel(mergedIndiAssData, mergedIndiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL), [&](CRole* role, bool inversed)->bool {
								if (!combinedDataNonDetRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
									combinedDataNonDetRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
									CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
									combinedDataNonDetRoleInstantiatedLinker = newRoleLinker->init(role, inversed, combinedDataNonDetRoleInstantiatedLinker);
								}
								return true;
							}, calcAlgContext);

							return true;
						}, calcAlgContext);
					}



					//TODO: detect removed non-deterministic neighbour connections (e.g., due to different merging), i.e., go through existing associated neighbour array and check for neighbours with only non-deterministic roles which are not instantiated in the current completion graph (although both individuals are present and fully handled)
					bool requiresAllLinksCollecting = false;
					if (indiAssData && indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL)) {
						tmpAssWriteDataLinker->setLinksUpdateType(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REPLACEMENT);
						requiresAllLinksCollecting = true;
					}


					for (CPROCESSSET<cint64>::const_iterator it = connIndiSet->constBegin(), itEnd = connIndiSet->constEnd(); it != itEnd; ++it) {
						cint64 connIndiId = *it;
						CIndividualProcessNode* connIndiNode = indiProcNodeVec->getData(connIndiId);

						if (!connIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED)) {
							bool isNominalNeighbour = connIndiNode->getNominalIndividual() != nullptr;

							bool isDataNeighbour = connIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCONCRETEDATAINDINODE);


							CIndividualMergingHash* connIndiMergingHash = connIndiNode->getIndividualMergingHash(false);
							CIndividualMergingHash::const_iterator connIndiMergedHashIt, connIndiMergedHashItEnd;
							if (connIndiMergingHash) {
								connIndiMergedHashIt = connIndiMergingHash->constBegin();
								connIndiMergedHashItEnd = connIndiMergingHash->constEnd();
							}
							bool allConnIndiHandled = false;
							cint64 connIndiMergedNominalId = -connIndiId;
							bool connIndiDetMerged = true;

							while (!allConnIndiHandled) {


								CBackendRepresentativeMemoryCacheRoleAssertionLinker* roleAssertionLinker = nullptr;

								CSuccessorRoleIterator roleIt(extractionIndiNode->getSuccessorRoleIterator(connIndiNode));
								while (roleIt.hasNext()) {
									CIndividualLinkEdge* link = roleIt.next(true);
									CRole* role = link->getLinkRole();

									collectRoleSuccessorData(link, false, maxDetBranchTag, isNominalNeighbour, isDataNeighbour, combinedNeighbourDetRoleInstantiatedSet, combinedNeighbourDetRoleInstantiatedLinker, combinedNeighbourNonDetRoleInstantiatedSet, combinedNeighbourNonDetRoleInstantiatedLinker, combinedExistentialDetRoleInstantiatedSet,
										combinedExistentialDetRoleInstantiatedLinker, combinedExistentialNonDetRoleInstantiatedSet, combinedExistentialNonDetRoleInstantiatedLinker, combinedDataDetRoleInstantiatedSet, combinedDataDetRoleInstantiatedLinker, combinedDataNonDetRoleInstantiatedSet, combinedDataNonDetRoleInstantiatedLinker, role, calcAlgContext, collected, extractionIndiNode, connIndiNode, connIndiMergedNominalId, connIndiDetMerged, roleAssertionLinker, existentialIndirectlyConnectedNominalIndividualSet, roleUsedCardHash);
								}

								CSuccessorRoleIterator commRoleIt(connIndiNode->getSuccessorRoleIterator(extractionIndiNode));
								while (commRoleIt.hasNext()) {
									CIndividualLinkEdge* link = commRoleIt.next(true);
									CRole* role = link->getLinkRole();

									collectRoleSuccessorData(link, true, maxDetBranchTag, isNominalNeighbour, isDataNeighbour, combinedNeighbourDetRoleInstantiatedSet, combinedNeighbourDetRoleInstantiatedLinker, combinedNeighbourNonDetRoleInstantiatedSet, combinedNeighbourNonDetRoleInstantiatedLinker, combinedExistentialDetRoleInstantiatedSet,
										combinedExistentialDetRoleInstantiatedLinker, combinedExistentialNonDetRoleInstantiatedSet, combinedExistentialNonDetRoleInstantiatedLinker, combinedDataDetRoleInstantiatedSet, combinedDataDetRoleInstantiatedLinker, combinedDataNonDetRoleInstantiatedSet, combinedDataNonDetRoleInstantiatedLinker, role, calcAlgContext, collected, extractionIndiNode, connIndiNode, connIndiMergedNominalId, connIndiDetMerged, roleAssertionLinker, existentialIndirectlyConnectedNominalIndividualSet, roleUsedCardHash);
								}


								if (roleAssertionLinker) {
									CBackendRepresentativeMemoryCacheTemporaryLabelReference neighbourRoleSetLabelRef = getNeighbourRoleInstantiatedSetLabelAssociationBackendItem(nullptr, roleAssertionLinker, nullptr, calcAlgContext);
									neigbourRoleInstantiatedSetCompination->insert(neighbourRoleSetLabelRef);


									bool alreadyPresent = false;
									if (!requiresAllLinksCollecting && indiAssData && neighbourRoleSetLabelRef.getReferredLabelData()) {
										CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetHash = indiAssData->getNeighbourRoleSetHash();
										if (neighbourRoleSetHash) {
											if (neighbourRoleSetLabelRef.getReferredLabelData() == neighbourRoleSetHash->getNeighbourRoleSetLabel(connIndiMergedNominalId)) {
												alreadyPresent = true;
											}
										}
									}

									if (!alreadyPresent) {
										CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* newRoleSetNeighbourUpdateLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker >::allocateAndConstruct(mMemAllocMan);
										newRoleSetNeighbourUpdateLinker->initRoleSetNeighbourUpdateDataLinker(neighbourRoleSetLabelRef, connIndiMergedNominalId);
										roleSetNeighbourUpdateLinker = newRoleSetNeighbourUpdateLinker->append(roleSetNeighbourUpdateLinker);
									}
								}

								allConnIndiHandled = true;
								if (connIndiMergedHashIt != connIndiMergedHashItEnd) {
									while (!connIndiMergedHashIt.value().isMergedWithIndividual() && connIndiMergedHashIt != connIndiMergedHashItEnd) {
										++connIndiMergedHashIt;
									}
									connIndiMergedNominalId = connIndiMergedHashIt.key();
									CIndividualMergingHashData mergingData = connIndiMergedHashIt.value();
									allConnIndiHandled = false;
									connIndiDetMerged = true;
									if (!mergingData.getDependencyTrackPoint() || mergingData.getDependencyTrackPoint()->getBranchingTag() > maxDetBranchTag) {
										connIndiDetMerged = false;
									}
									++connIndiMergedHashIt;
								}
							}

						}
					}



					// not necessary at the moment since all neighbours are expanded if new nominal individuals are merged
					//// add links from the association data of merged individuals, i.e., add all links of other association data that are not in the completion graph and also not as a neighbour in the current association data
					//visitMergedIndividualNodesAssociationData(extractionIndiNode, indiAssData, maxSameIndiSetLabel, true, maxDetBranchTag, [&](cint64 mergedIndiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* mergedIndiAssData, bool deterministicallyMerged)->bool {

					//	CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetHash = indiAssData->getNeighbourRoleSetHash();

					//	if (mergedIndiAssData != indiAssData) {
					//		CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* mergedNeighbourRoleSetArray = mergedIndiAssData->getRoleSetNeighbourArray();
					//		if (mergedNeighbourRoleSetArray) {
					//			CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArrayIndexData* neighbourRoleSetArrayIndexData = mergedNeighbourRoleSetArray->getIndexData();
					//			for (cint64 i = 0; i < neighbourRoleSetArrayIndexData->getArraySize(); ++i) {
					//				CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetlabel = neighbourRoleSetArrayIndexData->getNeighbourRoleSetLabel(i);

					//				mergedNeighbourRoleSetArray->at(i).visitNeighbourIndividualIds([&](cint64 neighbourIndiId)->bool {

					//					if (!connIndiSet->contains(-neighbourIndiId) && (!neighbourRoleSetHash || !neighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndiId))) {
					//						if (!neigbourRoleInstantiatedSetCompination->contains(neighbourRoleSetlabel)) {
					//							neigbourRoleInstantiatedSetCompination->insert(neighbourRoleSetlabel);

					//							visitRolesOfAssociatedNeigbourRoleSetLabel(mergedIndiAssData, neighbourRoleSetlabel, [&](CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)->bool {
					//								if (nondeterministic || !deterministicallyMerged) {
					//									// should never happen, since such non-deterministic neighbour links are considered in the completion graph
					//									//if (!combinedNeighbourNonDetRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
					//									//	combinedNeighbourNonDetRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
					//									//	CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
					//									//	combinedNeighbourNonDetRoleInstantiatedLinker = newRoleLinker->init(role, inversed, combinedNeighbourNonDetRoleInstantiatedLinker);
					//									//	neigbourRoleInstantiatedSetCompination->insert(CBackendRepresentativeMemoryCacheTemporaryLabelReference(neighbourRoleSetlabel));
					//									//}
					//								} else {
					//									neigbourRoleInstantiatedSetCompination->insert(CBackendRepresentativeMemoryCacheTemporaryLabelReference(neighbourRoleSetlabel));
					//									if (!combinedNeighbourDetRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
					//										combinedNeighbourDetRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
					//										CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
					//										combinedNeighbourDetRoleInstantiatedLinker = newRoleLinker->init(role, inversed, combinedNeighbourDetRoleInstantiatedLinker);
					//									}
					//								}
					//								return true;
					//							});

					//						}

					//						CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* newRoleSetNeighbourUpdateLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker >::allocateAndConstruct(mMemAllocMan);
					//						newRoleSetNeighbourUpdateLinker->initRoleSetNeighbourUpdateDataLinker(CBackendRepresentativeMemoryCacheTemporaryLabelReference(neighbourRoleSetlabel), neighbourIndiId);
					//						roleSetNeighbourUpdateLinker = newRoleSetNeighbourUpdateLinker->append(roleSetNeighbourUpdateLinker);
					//					}

					//					return true;
					//				});
					//			}
					//		}
					//	}

					//	return true;
					//}, calcAlgContext);




					if (indiAssData && !requiresAllLinksCollecting) {
						// add link data for not expanded neighbours from the previous data
						if (combinedNeighbourDetRoleInstantiatedLinker || combinedNeighbourNonDetRoleInstantiatedLinker) {

							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* neighbourRoleSetArray = indiAssData->getRoleSetNeighbourArray();
							if (neighbourRoleSetArray) {
								CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* neighbourRoleSetArrayIndexData = neighbourRoleSetArray->getIndexData();
								for (cint64 i = 0; i < neighbourRoleSetArrayIndexData->getArraySize(); ++i) {
									CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetlabel = neighbourRoleSetArrayIndexData->getNeighbourRoleSetLabel(i);

									neighbourRoleSetArray->at(i).visitNeighbourIndividualIds([&](cint64 neighbourIndiId)->bool {
										if (!connIndiSet->contains(-neighbourIndiId)) {
											// neighbour node is not present/connected in completion graph, i.e., make sure previous neighbour connections are correctly considered
											visitRolesOfAssociatedNeigbourRoleSetLabel(indiAssData, neighbourRoleSetlabel, [&](CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)->bool {
												if (nondeterministic) {
													// should never happen, since such non-deterministic neighbour links are considered in the completion graph
													//CIndividualProcessNode* neighbourIndiNode = indiProcNodeVec->getData(-neighbourIndiId);
													//if (!neighbourIndiNode && !combinedNeighbourNonDetRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
													//	combinedNeighbourNonDetRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
													//	CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
													//	combinedNeighbourNonDetRoleInstantiatedLinker = newRoleLinker->init(role, inversed, combinedNeighbourNonDetRoleInstantiatedLinker);
													//	neigbourRoleInstantiatedSetCompination->insert(CBackendRepresentativeMemoryCacheTemporaryLabelReference(neighbourRoleSetlabel));
													//}
												} else {
													neigbourRoleInstantiatedSetCompination->insert(CBackendRepresentativeMemoryCacheTemporaryLabelReference(neighbourRoleSetlabel));
													if (!combinedNeighbourDetRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
														combinedNeighbourDetRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
														CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
														combinedNeighbourDetRoleInstantiatedLinker = newRoleLinker->init(role, inversed, combinedNeighbourDetRoleInstantiatedLinker);
													}
												}
												return true;
											});

											return false;
										}
										return true;
									});
								}
							}
							
						}
					}







					if (combinedExistentialDetRoleInstantiatedLinker) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, combinedExistentialDetRoleInstantiatedLinker, combinedExistentialDetRoleInstantiatedSet, false, mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, labelRef);
						updated = true;
					} else if (!requiresAllLinksCollecting && indiAssData) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, labelRef);
					}

					if (combinedExistentialNonDetRoleInstantiatedLinker) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, combinedExistentialNonDetRoleInstantiatedLinker, combinedExistentialNonDetRoleInstantiatedSet, false, mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, labelRef);
						updated = true;
					} else if (!requiresAllLinksCollecting && indiAssData) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, labelRef);
					}






					if (combinedDataDetRoleInstantiatedLinker) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL, combinedDataDetRoleInstantiatedLinker, combinedDataDetRoleInstantiatedSet, false, mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL, labelRef);
						updated = true;
					} else if (!requiresAllLinksCollecting && indiAssData) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL, labelRef);
					}

					if (combinedDataNonDetRoleInstantiatedLinker) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL, combinedDataNonDetRoleInstantiatedLinker, combinedDataNonDetRoleInstantiatedSet, false, mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL, labelRef);
						updated = true;
					} else if (!requiresAllLinksCollecting && indiAssData) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL, labelRef);
					}






					if (combinedNeighbourDetRoleInstantiatedLinker) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, combinedNeighbourDetRoleInstantiatedLinker, combinedNeighbourDetRoleInstantiatedSet, false, mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, labelRef);
						updated = true;
					} else if (!requiresAllLinksCollecting && indiAssData) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, labelRef);
					}

					if (combinedNeighbourNonDetRoleInstantiatedLinker) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, combinedNeighbourNonDetRoleInstantiatedLinker, combinedNeighbourNonDetRoleInstantiatedSet, false, mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, labelRef);
						updated = true;
					} else if (!requiresAllLinksCollecting && indiAssData) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, labelRef);
					}
					
					if (!combinedNeighbourDetRoleInstantiatedLinker && !combinedNeighbourNonDetRoleInstantiatedLinker) {
						if (indiAssData) {
							CBackendRepresentativeMemoryCacheTemporaryLabelReference tmpLabelRef(indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL));
							tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, tmpLabelRef);
						}
					}

					tmpAssWriteDataLinker->setRoleSetNeighbourUpdateDataLinker(roleSetNeighbourUpdateLinker);

					if (neigbourRoleInstantiatedSetCompination && !neigbourRoleInstantiatedSetCompination->isEmpty()) {
						updated |= determineNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(neigbourRoleInstantiatedSetCompination, tmpAssWriteDataLinker, calcAlgContext);
					} else {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference tmpLabelRef(indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL));
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, tmpLabelRef);
					}

					return updated;

				}









				CIndividualProcessNode* CIndividualNodeBackendCacheHandler::getMergedIntoIndividualNode(CIndividualProcessNode* indi, bool onlyDeterministic, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualProcessNode* mergedIntoIndiNode = indi;
					CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
					cint64 maxDetBranchTag = calcAlgContext->getProcessingDataBox()->getMaximumDeterministicBranchTag();
					while (mergedIntoIndiNode->getMergedIntoIndividualNodeID() != mergedIntoIndiNode->getIndividualNodeID() && (!onlyDeterministic || mergedIntoIndiNode->getMergedDependencyTrackPoint() && mergedIntoIndiNode->getMergedDependencyTrackPoint()->getBranchingTag() <= maxDetBranchTag)) {
						mergedIntoIndiNode = indiProcNodeVec->getData(mergedIntoIndiNode->getMergedIntoIndividualNodeID());
					}
					return mergedIntoIndiNode;
				}




				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher CIndividualNodeBackendCacheHandler::getIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher(CIndividualProcessNode* indiNode, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* backendSyncData = (CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData*)indiNode->getIndividualBackendCacheSynchronisationData(false);
					CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = backendSyncData->getAssocitaionData();
					CBackendRepresentativeMemoryLabelCacheItem* conceptSetLabelItem = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
					CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCombinationLabelItem = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
					// TODO: also consider outgoing data property set labels to hasher
					CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher hasher(conceptSetLabelItem, neighbourRoleSetCombinationLabelItem);
					return hasher;
				}


				bool CIndividualNodeBackendCacheHandler::tryAssociateNodesWithBackendCache(CXLinker<CIndividualProcessNode*>* indiNodeLinker, CSatisfiableTaskRepresentativeBackendUpdatingAdapter* repbackUpdAdapter, CCalculationAlgorithmContext* calcAlgContext) {

					mSignatureTmpDetConSetRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());
					mSignatureTmpNonDetConSetRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					
					mSignatureTmpDetSameIndiSetRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					mSignatureTmpNonDetSameIndiSetRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					
					mSignatureTmpDetDiffIndiSetRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					mSignatureTmpNonDetDiffIndiSetRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					mTmpConceptLabelTmpCardDataHash = CObjectParameterizingAllocator< CPROCESSHASH< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*, CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					mSignatureTmpIndiConnIndiSetRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					mSignatureTmpRefNeigbourRoleInstantiatedSetCombinationLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					mRoleInversionTmpRefNeighbourInstantiatedRoleSetLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< TRoleInversionPair, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					mRoleInversionSetTmpRefNeighbourInstantiatedRoleSetLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					mSuperRoleSignatureCacheValueTmpRefNeighbourInstantiatedRoleSetLabelPairHash = CObjectParameterizingAllocator< CPROCESSHASH< cint64, CCacheValueTmpLabelReferenceData* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					mRoleInversionSetTmpRefCombinedNeighbourRoleInstantiatedSetLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					mNewTmpLabelList = CObjectParameterizingAllocator< CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					mNewTmpCardList = CObjectParameterizingAllocator< CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					prepareCacheMessages(calcAlgContext);

					CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker = nullptr;


					CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* >* nominalIndirectConnectionUpdateDataLinkerHash = CObjectParameterizingAllocator< CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());


					CPROCESSHASH< CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker*>* conceptSetLabelHasherWriteDataLinkerHasher = CObjectParameterizingAllocator< CPROCESSHASH< CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* >* mTmpDelayedInitResolvingLabelList = CObjectParameterizingAllocator< CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());


					CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tmpAssUseDataLinker = nullptr;
					CIndividualRepresentativeBackendCacheLoadedAssociationHash* indiLoadedAssocHash = calcAlgContext->getUsedProcessingDataBox()->getBackendCacheLoadedAssociationHash(false);


					CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();

					for (CXLinker<CIndividualProcessNode*>* indiNodeLinkerIt = indiNodeLinker; indiNodeLinkerIt; indiNodeLinkerIt = indiNodeLinkerIt->getNext()) {
						CIndividualProcessNode* indiNode = indiNodeLinkerIt->getData();

						CIndividual* nominalIndividual = indiNode->getNominalIndividual();
						if (nominalIndividual) {
							indiNode = indiProcNodeVec->getData(indiNode->getIndividualNodeID());

							

							//if (CIRIName::getRecentIRIName(nominalIndividual->getIndividualNameLinker()) == "http://www.w3.org/TR/2003/CR-owl-guide-20030818/wine#Moderate") {
							//	bool debug = true;
							//}


							CIndividualRepresentativeBackendCacheLoadedAssociationData& assoLoadingData = (*indiLoadedAssocHash)[nominalIndividual->getIndividualID()];


							CIndividualProcessNode* detMergedIntoIndiNode = getMergedIntoIndividualNode(indiNode, true, calcAlgContext);
							CIndividualProcessNode* nonDetMergedIntoIndiNode = getMergedIntoIndividualNode(detMergedIntoIndiNode, false, calcAlgContext);


							CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* newAssWriteDataLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
							if (!nominalIndividual->isTemporaryIndividual()) {
								newAssWriteDataLinker->initAccociationWriteData(nominalIndividual, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REPLACEMENT, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION);
							} else {
								newAssWriteDataLinker->initAccociationWriteData(nominalIndividual->getIndividualID(), CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REPLACEMENT, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION);
							}
							if (assoLoadingData.getLoadedIndividualAssociationData()) {
								newAssWriteDataLinker->setUsedAssociationUpdateId(assoLoadingData.getLoadedIndividualAssociationData()->getAssociationDataUpdateId());
							}

							if (indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSYNCHRONIZEDBACKENPROCESSINGDELAYING)) {
								assoLoadingData.setIndividualIntegrated(true);

								for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
									newAssWriteDataLinker->setReferredLabelData(i, assoLoadingData.getLoadedIndividualAssociationData()->getLabelCacheEntry(i));
								}


								mTmpDelayedInitResolvingLabelList->append(newAssWriteDataLinker);
							} else {

								newAssWriteDataLinker->setRepresentativeSameIndividualId(detMergedIntoIndiNode->getNominalIndividual()->getIndividualID());

								bool integratedAllMergingsFlag = false;
								determineSameIndividualSetLabelAssociationBackendItem(detMergedIntoIndiNode, CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL, newAssWriteDataLinker, &integratedAllMergingsFlag, calcAlgContext);
								if (!integratedAllMergingsFlag || detMergedIntoIndiNode != nonDetMergedIntoIndiNode) {
									determineSameIndividualSetLabelAssociationBackendItem(nonDetMergedIntoIndiNode, CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL, newAssWriteDataLinker, &integratedAllMergingsFlag, calcAlgContext);
								}

								bool integratedAllDifferentFlag = false;
								determineDifferentIndividualSetLabelAssociationBackendItem(detMergedIntoIndiNode, CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL, newAssWriteDataLinker, &integratedAllDifferentFlag, calcAlgContext);
								if (!integratedAllDifferentFlag || detMergedIntoIndiNode != nonDetMergedIntoIndiNode) {
									determineDifferentIndividualSetLabelAssociationBackendItem(nonDetMergedIntoIndiNode, CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL, newAssWriteDataLinker, &integratedAllDifferentFlag, calcAlgContext);
								}

								//determineConceptSetLabelAssociationBackendItem(detMergedIntoIndiNode, CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_CONCEPT_SET_LABEL, newAssWriteDataLinker, calcAlgContext);
								//if (detMergedIntoIndiNode == nonDetMergedIntoIndiNode) {
								//	determineConceptSetLabelAssociationBackendItem(nonDetMergedIntoIndiNode, CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL, newAssWriteDataLinker, calcAlgContext);
								//} else {
								//	determineNondeterministicConceptSetLabelAssociationBackendItemWithExcludedConceptsFromDeterministicLabel(nonDetMergedIntoIndiNode, indiNode, newAssWriteDataLinker, calcAlgContext);
								//}
								determineFullConceptSetLabelAssociationBackendItemWithSeparatedDeterministicNode(nonDetMergedIntoIndiNode, detMergedIntoIndiNode, indiNode, newAssWriteDataLinker, calcAlgContext);

								CPROCESSHASH<CRole*, cint64>* roleUsedCardHash = nullptr;
								if (nonDetMergedIntoIndiNode->getReapplyConceptLabelSet(false)->getConceptFlags()->containsATMOSTCardinalityFlags()) {
									roleUsedCardHash = CObjectParameterizingAllocator< CPROCESSHASH<CRole*, cint64>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
								}

								CPROCESSSET<cint64>* existentialIndirectlyConnectedNominalIndividualSet = nullptr;
								determineRoleInstantiatedSetLabelAssociationBackendItems(nonDetMergedIntoIndiNode, indiNode, nonDetMergedIntoIndiNode != detMergedIntoIndiNode, newAssWriteDataLinker, &existentialIndirectlyConnectedNominalIndividualSet, roleUsedCardHash, calcAlgContext);


								if (roleUsedCardHash) {
									determineAssociationBackendCardinality(nonDetMergedIntoIndiNode, roleUsedCardHash, newAssWriteDataLinker, calcAlgContext);
								}


								CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = getIndividualAssociationDataFromIndividualNode(indiNode, calcAlgContext);
								CBackendRepresentativeMemoryLabelCacheItem* indiConNomLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL);
								if (!existentialIndirectlyConnectedNominalIndividualSet && indiConNomLabel && indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSYNCHRONIZEDBACKENDSUCCESSOREXPANSIONBLOCKED)) {
									newAssWriteDataLinker->setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL, indiConNomLabel);
								}


								if (existentialIndirectlyConnectedNominalIndividualSet) {
									existentialIndirectlyConnectedNominalIndividualSet->remove(indiNode->getNominalIndividual()->getIndividualID());
									if (!existentialIndirectlyConnectedNominalIndividualSet->isEmpty()) {
										determineIndirectlyConnectedNominalIndividualSetLabelAssociationBackendItem(existentialIndirectlyConnectedNominalIndividualSet, newAssWriteDataLinker, calcAlgContext);

										for (CPROCESSSET<cint64>::const_iterator it = existentialIndirectlyConnectedNominalIndividualSet->constBegin(), itEnd = existentialIndirectlyConnectedNominalIndividualSet->constEnd(); it != itEnd; ++it) {
											cint64 nomIndiId = *it;

											CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker*& nomIndiConnDataLinker = (*nominalIndirectConnectionUpdateDataLinkerHash)[nomIndiId];
											if (!nomIndiConnDataLinker) {
												nomIndiConnDataLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker >::allocateAndConstruct(mMemAllocMan);
												nomIndiConnDataLinker->initNominalIndirectConnectionData(nomIndiId);
												if (assoLoadingData.getLoadedIndividualAssociationData()) {
													nomIndiConnDataLinker->setLastChangeIntegrationId(assoLoadingData.getLoadedIndividualAssociationData()->getLastIntegratedIndirectlyConnectedIndividualsChangeId());
												}
											}

											// only add linker if the individual was not already connected to the nominal (based on the previous indirectly connected nominal individual set label)
											if (!indiConNomLabel || !mAssBackCacheReader->hasIndividualIdsInAssociatedIndividualSetLabel(indiAssData, indiConNomLabel, nominalIndividual->getIndividualID())) {
												CXLinker<cint64>* indiIdLinker = CObjectAllocator< CXLinker<cint64> >::allocateAndConstruct(mMemAllocMan);
												indiIdLinker->initLinker(nominalIndividual->getIndividualID());
												nomIndiConnDataLinker->addIndirectlyConnectedIndividualIdLinker(indiIdLinker);
											}
										}
									}
								}

							
								assoLoadingData.setIndividualIntegrated(true);
								newAssWriteDataLinker->setCompletelyHandled(true);

								if (assoLoadingData.hasIndirectlyConnectedIndividualsIntegration()) {
									newAssWriteDataLinker->setIndirectlyConnectedIndividualIntegration(true);
									if (assoLoadingData.getLoadedNominalIndirectConnectionData()) {
										newAssWriteDataLinker->setIntegratedIndirectlyConnectedIndividualsChangeId(assoLoadingData.getLoadedNominalIndirectConnectionData()->getLastChangeId());
									}
								}


								// TODO: check whether there is indeed some update
								tmpAssWriteDataLinker = newAssWriteDataLinker->append(tmpAssWriteDataLinker);


								CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* backSyncData = (CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData*)indiNode->getIndividualBackendCacheSynchronisationData(false);
								if (backSyncData && !backSyncData->hasNonConceptSetBackendLabelRelatedProcessing()) {
									CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher hasher = getIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher(indiNode, calcAlgContext);
									conceptSetLabelHasherWriteDataLinkerHasher->insert(hasher, newAssWriteDataLinker);
								}



							}
						}
					}



					if (mTmpDelayedInitResolvingLabelList && !mTmpDelayedInitResolvingLabelList->isEmpty()) {


						//CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* >* mIndiWriteDataLinkerHash = CObjectParameterizingAllocator< CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

						//for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinkerIt = tmpAssWriteDataLinker; tmpAssWriteDataLinkerIt; tmpAssWriteDataLinkerIt = tmpAssWriteDataLinkerIt->getNext()) {
						//	cint64 indiId = tmpAssWriteDataLinkerIt->getData()->getIndividualID();
						//	mIndiWriteDataLinkerHash->insert(indiId, tmpAssWriteDataLinkerIt);
						//}

						//CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHash* useBackendConceptSetLabelProcessingHash = calcAlgContext->getProcessingDataBox()->getBackendCacheConceptSetLabelProcessingHash(false);
						//CIndividualDelayedBackendInitializationProcessingQueue* delayedBackendInitProcessingQueue = calcAlgContext->getProcessingDataBox()->getDelayedBackendConceptSetLabelProcessingInitializationQueue(false);

						for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpDelayedInitResolvingLabel : *mTmpDelayedInitResolvingLabelList) {

							CIndividualProcessNode* indiNode = indiProcNodeVec->getData(-tmpDelayedInitResolvingLabel->getIndividualID());
							CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher hasher = getIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher(indiNode, calcAlgContext);


							CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* writeDataLinker = conceptSetLabelHasherWriteDataLinkerHasher->value(hasher);
							tmpDelayedInitResolvingLabel->setCompletelyHandled(true);

							//CIndividualProcessNode* queuIndiNode = nullptr;
							//if (!writeDataLinker) {
							//	CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData data = useBackendConceptSetLabelProcessingHash->value(hasher);
							//	queuIndiNode = indiProcNodeVec->getData(data.mLastInitScheduledIndiNodeId);
							//	CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* queuedBackSyncData = (CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData*)queuIndiNode->getIndividualBackendCacheSynchronisationData(false);
							//}


							tmpDelayedInitResolvingLabel->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, writeDataLinker->getReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL));
							tmpDelayedInitResolvingLabel->setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, writeDataLinker->getReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL));

							tmpDelayedInitResolvingLabel->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, writeDataLinker->getReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL));
							tmpDelayedInitResolvingLabel->setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, writeDataLinker->getReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL));

							tmpDelayedInitResolvingLabel->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL, writeDataLinker->getReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL));
							tmpDelayedInitResolvingLabel->setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL, writeDataLinker->getReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL));



							tmpAssWriteDataLinker = tmpDelayedInitResolvingLabel->append(tmpAssWriteDataLinker);
						}
					}



					CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinker = nullptr;
					CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* lastTempNomIndirectConnDataLinker = nullptr;

					for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinkerIt = tmpAssWriteDataLinker; tmpAssWriteDataLinkerIt; tmpAssWriteDataLinkerIt = tmpAssWriteDataLinkerIt->getNext()) {
						cint64 indiId = tmpAssWriteDataLinkerIt->getData()->getIndividualID();
						CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* nomIndiConnDataLinker = nominalIndirectConnectionUpdateDataLinkerHash->value(indiId);
						if (nomIndiConnDataLinker) {
							tmpAssWriteDataLinkerIt->setIndirectlyConnectedNominalIndividual(true);
							if (tempNomIndirectConnDataLinker) {
								lastTempNomIndirectConnDataLinker->setNext(nomIndiConnDataLinker);
								lastTempNomIndirectConnDataLinker = nomIndiConnDataLinker;
							} else {
								tempNomIndirectConnDataLinker = nomIndiConnDataLinker;
								lastTempNomIndirectConnDataLinker = nomIndiConnDataLinker;
							}
							nominalIndirectConnectionUpdateDataLinkerHash->remove(indiId);
						}
					}
					for (CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* >::const_iterator it = nominalIndirectConnectionUpdateDataLinkerHash->constBegin(), itEnd = nominalIndirectConnectionUpdateDataLinkerHash->constEnd(); it != itEnd; ++it) {
						cint64 nomIndiId = it.key();
						CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* nomIndiConnDataLinker = it.value();
						if (nomIndiConnDataLinker) {
							if (tempNomIndirectConnDataLinker) {
								lastTempNomIndirectConnDataLinker->setNext(nomIndiConnDataLinker);
								lastTempNomIndirectConnDataLinker = nomIndiConnDataLinker;
							} else {
								tempNomIndirectConnDataLinker = nomIndiConnDataLinker;
								lastTempNomIndirectConnDataLinker = nomIndiConnDataLinker;
							}
						}
					}



					if (indiLoadedAssocHash) {
						for (CIndividualRepresentativeBackendCacheLoadedAssociationHash::const_iterator it = indiLoadedAssocHash->constBegin(), itEnd = indiLoadedAssocHash->constEnd(); it != itEnd; ++it) {
							cint64 indiAssoId = it.key();
							CIndividualRepresentativeBackendCacheLoadedAssociationData indiAssoLoadingData = it.value();
							if (indiAssoLoadingData.getLoadedIndividualAssociationData() && !indiAssoLoadingData.isIndividualIntegrated()) {

								CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* newTmpAssUseDataLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker >::allocateAndConstruct(mMemAllocMan);
								newTmpAssUseDataLinker->initAccociationUseData(indiAssoId);
								newTmpAssUseDataLinker->setUsedAssociationUpdateId(indiAssoLoadingData.getLoadedIndividualAssociationData()->getAssociationDataUpdateId());

								tmpAssUseDataLinker = newTmpAssUseDataLinker->append(tmpAssUseDataLinker);
							}
						}
					}

					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker = nullptr;
					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* lastTempLabelWriteDataLinker = nullptr;
					for (CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >::const_iterator it = mNewTmpLabelList->constBegin(), itEnd = mNewTmpLabelList->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTmpLabelLinker = (*it);
						newTmpLabelLinker->clearNext();
						if (!lastTempLabelWriteDataLinker) {
							tempLabelWriteDataLinker = newTmpLabelLinker;
							lastTempLabelWriteDataLinker = tempLabelWriteDataLinker;
						} else {
							lastTempLabelWriteDataLinker->setNext(newTmpLabelLinker);
							lastTempLabelWriteDataLinker = newTmpLabelLinker;
						}
					}

					CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker = nullptr;
					for (CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >::const_iterator it = mNewTmpCardList->constBegin(), itEnd = mNewTmpCardList->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* newTmpCardLinker = (*it);
						newTmpCardLinker->clearNext();
						tempCardWriteDataLinker = newTmpCardLinker->append(tempCardWriteDataLinker);
					}

					cint64 ontologyIdentifier = calcAlgContext->getProcessingDataBox()->getOntology()->getOntologyID();
					CBackendRepresentativeMemoryCacheLabelAssociationWriteData* writeData = CObjectAllocator< CBackendRepresentativeMemoryCacheLabelAssociationWriteData >::allocateAndConstruct(mMemAllocMan);
					writeData->initWriteData(ontologyIdentifier, tmpAssWriteDataLinker, tempNomIndirectConnDataLinker, tmpAssUseDataLinker, tempLabelWriteDataLinker,tempCardWriteDataLinker);
					addCacheMessages(writeData,calcAlgContext);
					commitCacheMessages(calcAlgContext);

					return false;
				}










			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
