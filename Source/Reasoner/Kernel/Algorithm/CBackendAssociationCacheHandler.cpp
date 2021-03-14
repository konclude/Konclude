/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CBackendAssociationCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CBackendAssociationCacheHandler::CBackendAssociationCacheHandler(CBackendRepresentativeMemoryCacheReader* backAssCacheReader, CBackendRepresentativeMemoryCacheWriter* backAssCacheWriter) {
					mAssBackCacheReader = backAssCacheReader;
					mAssBackCacheWriter = backAssCacheWriter;

					mStatCreatedTempLabels = 0;
					mStatReusedTempLabels = 0;
					mStatReusedCachedLabels = 0;
					mStatReusedHashedIndiLabels = 0;
					mStatReusedHashedSignatureLabels = 0;

					mTmpContext = nullptr;
					mMemAllocMan = nullptr;
					mWriteData = nullptr;
				}


				CBackendRepresentativeMemoryCache* CBackendAssociationCacheHandler::getCache() {
					return mAssBackCacheWriter->getCache();
				}



				CBackendAssociationCacheHandler* CBackendAssociationCacheHandler::setWorkingOntology(CConcreteOntology* ontology) {
					mAssBackCacheReader->setWorkingOntology(ontology);
					return this;
				}


				CBackendAssociationCacheHandler* CBackendAssociationCacheHandler::checkRecomputationIdUsage(cint64 recomputationId) {
					mAssBackCacheReader->checkRecomputationIdUsage(recomputationId);
					return this;
				}


				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendAssociationCacheHandler::getIndividualRepresentativeIdResolvedAssociationData(cint64 indiId, CCalculationAlgorithmContext* calcAlgContext) {
					CBackendRepresentativeMemoryCacheIndividualAssociationData* assocData = getIndividualAssociationData(indiId, calcAlgContext);
					if (assocData->hasRepresentativeSameIndividualMerging()) {
						assocData = getIndividualAssociationData(assocData->getRepresentativeSameIndividualId(), calcAlgContext);
					}
					return assocData;
				}


				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendAssociationCacheHandler::getIndividualAssociationData(cint64 indiId, CCalculationAlgorithmContext* calcAlgContext) {
					return getIndividualAssociationData(indiId, true, calcAlgContext);
				}



				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendAssociationCacheHandler::getIndividualAssociationData(cint64 indiId, bool recordAccess, CCalculationAlgorithmContext* calcAlgContext) {

					CBackendRepresentativeMemoryCacheIndividualAssociationData* assData = nullptr;

					CIndividualRepresentativeBackendCacheLoadedAssociationHash* indiLoadedAssocHash = calcAlgContext->getUsedProcessingDataBox()->getBackendCacheLoadedAssociationHash(false);
					if (indiLoadedAssocHash) {
						CIndividualRepresentativeBackendCacheLoadedAssociationData loadedAssoData = indiLoadedAssocHash->value(indiId);
						assData = loadedAssoData.getLoadedIndividualAssociationData();
					}

					if (!assData) {
						assData = mAssBackCacheReader->getIndividualAssociationData(indiId);
						if (recordAccess) {
							CIndividualRepresentativeBackendCacheLoadedAssociationHash* locIndiLoadedAssocHash = calcAlgContext->getUsedProcessingDataBox()->getBackendCacheLoadedAssociationHash(true);
							CIndividualRepresentativeBackendCacheLoadedAssociationData& loadedAssoData = (*locIndiLoadedAssocHash)[indiId];
							loadedAssoData.setLoadedIndividualAssociationData(assData);
						}
					}
					return assData;
				}


				CIndividualRepresentativeBackendCacheLoadedAssociationData* CBackendAssociationCacheHandler::getIndividualAssociationLoadingData(cint64 indiId, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualRepresentativeBackendCacheLoadedAssociationHash* locIndiLoadedAssocHash = calcAlgContext->getUsedProcessingDataBox()->getBackendCacheLoadedAssociationHash(true);
					CIndividualRepresentativeBackendCacheLoadedAssociationData& loadedAssoData = (*locIndiLoadedAssocHash)[indiId];

					if (!loadedAssoData.getLoadedIndividualAssociationData()) {
						loadedAssoData.setLoadedIndividualAssociationData(mAssBackCacheReader->getIndividualAssociationData(indiId));
					}

					if (loadedAssoData.getLoadedIndividualAssociationData()) {
						return &loadedAssoData;
					} else {
						return nullptr;
					}
				}


				CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* CBackendAssociationCacheHandler::getLoadedNominalIndividualIndirectConnectionData(cint64 indiId, CIndividualRepresentativeBackendCacheLoadedAssociationData* indiLoadedAssoData, CCalculationAlgorithmContext* calcAlgContext) {
					if (!indiLoadedAssoData) {
						indiLoadedAssoData = getIndividualAssociationLoadingData(indiId, calcAlgContext);
					}
					if (!indiLoadedAssoData->getLoadedNominalIndirectConnectionData()) {
						indiLoadedAssoData->setLoadedNominalIndirectConnectionData(mAssBackCacheReader->getNominalIndirectConnectionData(indiId));
						indiLoadedAssoData->setIndirectlyConnectedIndividualsIntegration(true);
					}
					return indiLoadedAssoData->getLoadedNominalIndirectConnectionData();
				}


				CIndividualRepresentativeBackendCacheLoadedAssociationData* CBackendAssociationCacheHandler::getIndividualAssociationLoadingData(CIndividual* individual, CCalculationAlgorithmContext* calcAlgContext) {
					return getIndividualAssociationLoadingData(individual->getIndividualID(), calcAlgContext);
				}


				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendAssociationCacheHandler::getIndividualAssociationData(CIndividual* individual, CCalculationAlgorithmContext* calcAlgContext) {
					return getIndividualAssociationData(individual->getIndividualID(), calcAlgContext);
				}



				bool CBackendAssociationCacheHandler::prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (!mMemAllocMan) {
						CTaskMemoryPoolAllocationManager satCacheMemManCreaterMemMan(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&satCacheMemManCreaterMemMan);
						mMemAllocMan->initTaskMemoryPoolAllocationManager(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mMemAllocMan,mMemAllocMan);
						return true;
					}
					return false;
				}

				bool CBackendAssociationCacheHandler::commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						CBackendRepresentativeMemoryCacheLabelAssociationWriteData* commitWriteData = nullptr;
						CBackendRepresentativeMemoryCacheLabelAssociationWriteData* writeDataIt = mWriteData;
						while (writeDataIt) {
							CBackendRepresentativeMemoryCacheLabelAssociationWriteData* tmpWriteData = writeDataIt;
							writeDataIt = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)writeDataIt->getNext();
							tmpWriteData->clearNext();
							if (commitWriteData) {
								commitWriteData = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)tmpWriteData->append(commitWriteData);
							} else {
								commitWriteData = tmpWriteData;
							}
						}
						mAssBackCacheWriter->writeCachedData(commitWriteData,mMemPoolCon.takeMemoryPools());
						mWriteData = nullptr;
						mMemAllocMan = nullptr;
						mTmpContext = nullptr;
						return true;
					}
					return false;
				}



				bool CBackendAssociationCacheHandler::addCacheMessages(CBackendRepresentativeMemoryCacheLabelAssociationWriteData* backAssWriteData, CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						mWriteData = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)backAssWriteData->append(mWriteData);
					} else {
						mWriteData = backAssWriteData;
					}
					return true;
				}






				bool CBackendAssociationCacheHandler::determineCardinalityAssociationBackendItem(function<void(CPROCESSHASH<cint64, CBackendAssociationCacheHandler::CRoleCardinalityCountData>* superRoleTagUsedCardCountHash)> initFunc, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, bool superRolePropagation, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {

					CPROCESSHASH<cint64, CRoleCardinalityCountData>* superRoleTagUsedCardCountHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CRoleCardinalityCountData>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					initFunc(superRoleTagUsedCardCountHash);

					for (CPROCESSHASH<CRole*, cint64>::const_iterator it = roleUsedCardHash->constBegin(), itEnd = roleUsedCardHash->constEnd(); it != itEnd; ++it) {
						CRole* role = it.key();
						cint64 count = it.value();
						if (superRolePropagation) {
							for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
								if (!superRoleIt->isNegated()) {
									CRole* superRole = superRoleIt->getData();
									CRoleCardinalityCountData& roleCardCountData = (*superRoleTagUsedCardCountHash)[superRole->getRoleTag()];
									if (roleCardCountData.mHasRestrictingCardinality) {
										roleCardCountData.mExistentialUsedCardinality += count;
									}
								}
							}
						} else {
							CRoleCardinalityCountData& roleCardCountData = (*superRoleTagUsedCardCountHash)[role->getRoleTag()];
							if (roleCardCountData.mHasRestrictingCardinality) {
								roleCardCountData.mExistentialUsedCardinality += count;
							}
						}
					}


					CBackendRepresentativeMemoryLabelCacheItem* conSetLabelItem = tmpAssWriteDataLinker->getReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
					if (conSetLabelItem) {
						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* cardExtData = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)conSetLabelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemExtensionData::CARDINALITY_HASH);
						KONCLUDE_ASSERT_X(cardExtData, "cardinality association updater", "cardinality extension data missing");
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>* roleCardinalityDataHash = cardExtData->getRoleCardinalityDataHash();
						for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>::const_iterator it = roleCardinalityDataHash->constBegin(), itEnd = roleCardinalityDataHash->constEnd(); it != itEnd; ++it) {
							cint64 roleTag = it.key();
							CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardData = it.value();
							CRoleCardinalityCountData& roleCardCountData = (*superRoleTagUsedCardCountHash)[roleTag];
							cardData->updateExistentialMaxUsedCardinality(roleCardCountData.mExistentialUsedCardinality);
							cardData->updateMinimumRestrictingCardinality(roleCardCountData.mRestrictingCardinality);
						}
					} else {
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* conSetTmpLabelItem = tmpAssWriteDataLinker->getReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* extCardWriteData = nullptr;
						if (mTmpConceptLabelTmpCardDataHash) {
							extCardWriteData = mTmpConceptLabelTmpCardDataHash->value(conSetTmpLabelItem);
						}
						if (extCardWriteData) {
							for (CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinkerIt = extCardWriteData->getCardinalityCacheValueLinker(); cardValueLinkerIt; cardValueLinkerIt = cardValueLinkerIt->getNext()) {
								CRoleCardinalityCountData& roleCardCountData = (*superRoleTagUsedCardCountHash)[cardValueLinkerIt->getRoleTag()];
								cardValueLinkerIt->updateExistentialMaxUsedCardinality(roleCardCountData.mExistentialUsedCardinality);
								cardValueLinkerIt->updateMinimalRestrictingCardinality(roleCardCountData.mRestrictingCardinality);
							}
						} else {

							CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* newTempRefCard = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
							newTempRefCard->initLabelWriteData(conSetTmpLabelItem);
							for (CPROCESSHASH<cint64, CRoleCardinalityCountData>::const_iterator it = superRoleTagUsedCardCountHash->constBegin(), itEnd = superRoleTagUsedCardCountHash->constEnd(); it != itEnd; ++it) {
								cint64 roleTag(it.key());
								CRoleCardinalityCountData cadCountData = it.value();
								if (cadCountData.mHasRestrictingCardinality) {
									CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinker = CObjectAllocator< CBackendRepresentativeMemoryCardinalityValueLinker >::allocateAndConstruct(mMemAllocMan);
									cardValueLinker->initCardinalityValueLinker(roleTag, cadCountData.mExistentialUsedCardinality, cadCountData.mRestrictingCardinality);
									newTempRefCard->appendCardinalityCacheValueLinker(cardValueLinker);
								}
							}
							mNewTmpCardList->append(newTempRefCard);
							mTmpConceptLabelTmpCardDataHash->insert(conSetTmpLabelItem, newTempRefCard);
							++mStatCreatedTempLabels;

						}
					}


					return true;
				}




				CBackendRepresentativeMemoryCacheTemporaryLabelReference CBackendAssociationCacheHandler::getRoleInstantiatedSetLabelAssociationBackendItem(cint64 labelType, CSortedNegLinker<CRole*>* roleLinker, CPROCESSSET<TRoleInversionPair>* roleInversionSet, bool extendBySuperRoles, CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >* roleInstantiatedSetLabelHash, CCalculationAlgorithmContext* calcAlgContext) {

					CRole* topRole = calcAlgContext->getProcessingDataBox()->getOntology()->getRBox()->getTopObjectRole();
					if (roleLinker && !roleInversionSet) {
						roleInversionSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
						for (CSortedNegLinker<CRole*>* roleLinkerIt = roleLinker; roleLinkerIt; roleLinkerIt = roleLinkerIt->getNext()) {
							CRole* role = roleLinkerIt->getData();
							bool inversed = roleLinkerIt->isNegated();
							roleInversionSet->insert(TRoleInversionPair(role, inversed));
						}
					}



					CBackendRepresentativeMemoryCacheTemporaryLabelReference*& tmpLabelRefData = (*roleInstantiatedSetLabelHash)[CProcessSetHasher<TRoleInversionPair>(roleInversionSet)];
					if (tmpLabelRefData) {
						return *tmpLabelRefData;
					} else {
						tmpLabelRefData = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelReference >::allocateAndConstruct(mMemAllocMan);
					}


					if (!roleLinker && roleInversionSet) {
						for (CPROCESSSET<TRoleInversionPair>::const_iterator it = roleInversionSet->constBegin(), itEnd = roleInversionSet->constEnd(); it != itEnd; ++it) {
							TRoleInversionPair roleInvPair(*it);
							CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getMemoryAllocationManager());
							newRoleLinker->init(roleInvPair.first, roleInvPair.second);
							roleLinker = (CSortedNegLinker<CRole*>*)newRoleLinker->append(roleLinker);
						}
					}

					CSortedNegLinker<CRole*>* superRoleLinker = nullptr;
					CPROCESSSET<TRoleInversionPair>* superRoleInversionSet = nullptr;
					if (extendBySuperRoles) {
						superRoleInversionSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
						for (CSortedNegLinker<CRole*>* roleLinkerIt = roleLinker; roleLinkerIt; roleLinkerIt = roleLinkerIt->getNext()) {
							CRole* role = roleLinkerIt->getData();
							bool inversed = roleLinkerIt->isNegated();
							if (!superRoleInversionSet->contains(TRoleInversionPair(role, inversed))) {
								for (CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getIndirectSuperRoleList(); superRoleLinkerIt; superRoleLinkerIt = superRoleLinkerIt->getNext()) {
									CRole* superRole = superRoleLinkerIt->getData();
									bool superRoleNegation = superRoleLinkerIt->isNegated() ^ inversed;
									if (superRole != topRole && !superRoleInversionSet->contains(TRoleInversionPair(superRole, superRoleNegation))) {
										superRoleInversionSet->insert(TRoleInversionPair(superRole, superRoleNegation));
										CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getMemoryAllocationManager());
										newRoleLinker->init(superRole, superRoleNegation);
										superRoleLinker = (CSortedNegLinker<CRole*>*)newRoleLinker->append(superRoleLinker);
									}
								}
							}
						}
					} else {
						superRoleLinker = roleLinker;
						superRoleInversionSet = roleInversionSet;
					}


					cint64 superRoleCount = superRoleLinker->getCount();
					cint64 signature = CBackendRepresentativeMemoryCacheUtilities::getRoleInversedLinkerSignature(superRoleLinker, false, superRoleCount);

					CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getLabelCacheEntryViaRoleLinker(labelType, signature, superRoleCount, superRoleLinker, false);
					if (cachedLabelItem) {
						++mStatReusedCachedLabels;
						tmpLabelRefData->setReferredLabelData(cachedLabelItem);
					} else {
						// create new temporary label item
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
						newTempRefLabel->initLabelWriteData(signature, labelType);

						for (CSortedNegLinker<CRole*>* superRoleLinkerIt = superRoleLinker; superRoleLinkerIt; superRoleLinkerIt = superRoleLinkerIt->getNext()) {
							CRole* superRole = superRoleLinkerIt->getData();
							bool roleInversed = superRoleLinkerIt->isNegated();
							CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
							labelValueLinker->initLabelValueLinker(mAssBackCacheReader->getCacheValue(superRole, roleInversed));
							newTempRefLabel->appendCacheValueLinker(labelValueLinker);
						}
						mNewTmpLabelList->append(newTempRefLabel);
						++mStatCreatedTempLabels;
						tmpLabelRefData->setReferredTemporaryLabelData(newTempRefLabel);
					}

					return *tmpLabelRefData;

				}






				bool CBackendAssociationCacheHandler::visitRolesOfAssociatedCompinationRoleSetLabel(CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef, function<bool(CRole* role, bool inversed)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = labelRef.getReferredLabelData();
					if (labelItem) {
						return visitRolesOfAssociatedCompinationRoleSetLabel(nullptr, labelItem, visitFunc, calcAlgContext);
					}
					bool visited = false;
					bool continueVisiting = true;
					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* labelWriteDataLinker = labelRef.getReferredTemporaryLabelData();
					for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* labelWriteDataLinkerIt = labelWriteDataLinker; labelWriteDataLinkerIt && continueVisiting; labelWriteDataLinkerIt = labelWriteDataLinkerIt->getNext()) {
						for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelWriteDataLinkerIt->getCacheValueLinker(); labelValueLinkerIt && continueVisiting; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
							const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();

							CRole* role = (CRole*)cacheValue.getIdentification();
							bool roleInversed = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDINVERSEDROLE;
							continueVisiting = visitFunc(role, roleInversed);
							visited = true;
						}
					}
					return visited;
				}













				bool CBackendAssociationCacheHandler::visitNeighbourArrayIdsForRole(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CRole* role, function<bool(cint64 neighbourArrayId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, bool nondeterministic)> visitFunc, bool visitOnlyDeterministicNeighbours, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitNeighbourArrayIdsForRole(assData, role, visitFunc, visitOnlyDeterministicNeighbours);
				}



				bool CBackendAssociationCacheHandler::visitNeighbourIndividualIdsForRole(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CRole* role, function<bool(cint64 neighbourIndividualId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, bool nondeterministic)> visitFunc, bool visitOnlyDeterministicNeighbours, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitNeighbourIndividualIdsForRole(assData, role, visitFunc, visitOnlyDeterministicNeighbours);
				}



				bool CBackendAssociationCacheHandler::visitNeighbourIndividualIdsForNeighbourArrayIdFromCursor(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 id, function<bool(cint64 neighbourIndividualId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, bool nondeterministic, cint64 cursor)> visitFunc, bool visitOnlyDeterministicNeighbours, cint64 cursor, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitNeighbourIndividualIdsForNeighbourArrayIdFromCursor(assData, id, visitFunc, visitOnlyDeterministicNeighbours, cursor);
				}



				cint64 CBackendAssociationCacheHandler::getNeighbourCountForRole(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CRole* role, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->getNeighbourCountForRole(assData, role);
				}





				cint64 CBackendAssociationCacheHandler::getNeighbourCountForArrayPos(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 pos, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->getNeighbourCountForArrayPos(assData, pos);
				}















				bool CBackendAssociationCacheHandler::determineIndirectlyConnectedNominalIndividualSetLabelAssociationBackendItem(CPROCESSSET<cint64>* existentialIndirectlyConnectedNominalIndividualSet, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {


					cint64 indiCount = existentialIndirectlyConnectedNominalIndividualSet->size();
					cint64 indiSetSigValue = CBackendRepresentativeMemoryCacheUtilities::getIndividualSetSignature(existentialIndirectlyConnectedNominalIndividualSet, indiCount);

					if (indiCount > 0) {

						CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getIndividualSetLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL, indiSetSigValue, existentialIndirectlyConnectedNominalIndividualSet, indiCount);
						if (cachedLabelItem) {
							++mStatReusedCachedLabels;
							tmpAssWriteDataLinker->setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL, cachedLabelItem);
							return true;
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*& prevTempRefLabels = (*mSignatureTmpIndiConnIndiSetRefLabelHash)[indiSetSigValue];
						if (prevTempRefLabels) {
							for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* prevTempRefLabelIt = prevTempRefLabels; prevTempRefLabelIt; prevTempRefLabelIt = prevTempRefLabelIt->getNext()) {
								if (prevTempRefLabelIt->getCacheValueCount() == indiCount) {
									bool compatibleIndividuals = true;
									for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = prevTempRefLabelIt->getCacheValueLinker(); labelValueLinkerIt && compatibleIndividuals; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
										CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
										cint64 indiId = cacheValue.getIdentification();

										if (!existentialIndirectlyConnectedNominalIndividualSet->contains(indiId)) {
											compatibleIndividuals = false;
										}
									}
									if (compatibleIndividuals) {
										++mStatReusedHashedSignatureLabels;
										++mStatReusedTempLabels;
										tmpAssWriteDataLinker->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL, prevTempRefLabelIt);
										return true;
									}
								}
							}
						}


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
						newTempRefLabel->initLabelWriteData(indiSetSigValue, CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL);
						newTempRefLabel->setCompletelySaturated(false);
						newTempRefLabel->setCompletelyHandled(true);
						for (CPROCESSSET<cint64>::const_iterator it = existentialIndirectlyConnectedNominalIndividualSet->constBegin(), itEnd = existentialIndirectlyConnectedNominalIndividualSet->constEnd(); it != itEnd; ++it) {
							cint64 indiId = *it;
							CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
							labelValueLinker->initLabelValueLinker(mAssBackCacheReader->getCacheValue(indiId, false));
							newTempRefLabel->appendCacheValueLinker(labelValueLinker);
						}
						mNewTmpLabelList->append(newTempRefLabel);
						++mStatCreatedTempLabels;

						tmpAssWriteDataLinker->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL, newTempRefLabel);
						prevTempRefLabels = newTempRefLabel->append(prevTempRefLabels);
						return true;
					} else {
						return false;
					}
				}










				CBackendRepresentativeMemoryCacheTemporaryLabelReference CBackendAssociationCacheHandler::getNeighbourRoleInstantiatedSetLabelAssociationBackendItem(CRole* role, bool inversed, CCalculationAlgorithmContext* calcAlgContext) {

					CBackendRepresentativeMemoryCacheTemporaryLabelReference*& tmpLabelRefData = (*mRoleInversionTmpRefNeighbourInstantiatedRoleSetLabelHash)[TRoleInversionPair(role, inversed)];

					if (tmpLabelRefData) {
						return *tmpLabelRefData;
					} else {
						tmpLabelRefData = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelReference >::allocateAndConstruct(mMemAllocMan);
					}

					CSortedNegLinker<CRole*>* superRoleLinker = role->getIndirectSuperRoleList();
					cint64 superRoleCount = superRoleLinker->getCount();
					cint64 signature = CBackendRepresentativeMemoryCacheUtilities::getRoleInversedLinkerSignature(superRoleLinker, inversed, superRoleCount);

					CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getLabelCacheEntryViaRoleLinker(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, signature, superRoleCount, superRoleLinker, inversed, role);
					if (cachedLabelItem) {
						++mStatReusedCachedLabels;
						tmpLabelRefData->setReferredLabelData(cachedLabelItem);
					} else {
						// create new temporary label item
						CLinkedNeighbourRoleAssertionLinker linkedRoleAssertedLinker;
						linkedRoleAssertedLinker.initLinkedNeighbourRoleAssertionLinker(role, inversed);
						CBackendRepresentativeMemoryCacheTemporaryLabelReference newTempLabelRef = getNeighbourRoleInstantiatedSetLabelAssociationBackendItem(&linkedRoleAssertedLinker, nullptr, nullptr, calcAlgContext);
						*tmpLabelRefData = newTempLabelRef;
					}

					return *tmpLabelRefData;

				}







				bool CBackendAssociationCacheHandler::determineNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(CPROCESSSET<CBackendRepresentativeMemoryCacheTemporaryLabelReference>* neigbourRoleInstantiatedSetCompination, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {

					bool hasTemporaryLabelReferences = false;

					cint64 neighbourRoleInstantiatedSetCount = 0;
					CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neigbourRoleInstantiatedSetTmpLabelLinker = nullptr;
					for (CPROCESSSET<CBackendRepresentativeMemoryCacheTemporaryLabelReference>::const_iterator it = neigbourRoleInstantiatedSetCompination->constBegin(), itEnd = neigbourRoleInstantiatedSetCompination->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef(*it);
						if (labelRef.getReferredTemporaryLabelData()) {
							hasTemporaryLabelReferences = true;
						}
						CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* newNeigbourRoleInstantiatedSetTmpLabelLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
						newNeigbourRoleInstantiatedSetTmpLabelLinker->initLabelReferenceDataLinker(labelRef);
						neigbourRoleInstantiatedSetTmpLabelLinker = newNeigbourRoleInstantiatedSetTmpLabelLinker->append(neigbourRoleInstantiatedSetTmpLabelLinker);
						++neighbourRoleInstantiatedSetCount;
					}



					cint64 signature = CBackendRepresentativeMemoryCacheUtilities::getNeighbourRoleInstantiatedSetLinkerSignature(neigbourRoleInstantiatedSetTmpLabelLinker, neighbourRoleInstantiatedSetCount);


					CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getNeighbourRoleInstantiatedSetCompinationLabelCacheEntry(signature, neighbourRoleInstantiatedSetCount, neigbourRoleInstantiatedSetTmpLabelLinker);
					if (cachedLabelItem) {
						++mStatReusedCachedLabels;
						tmpAssWriteDataLinker->setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, cachedLabelItem);
						return true;
					}


					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*& prevTempRefLabels = (*mSignatureTmpRefNeigbourRoleInstantiatedSetCombinationLabelHash)[signature];
					if (prevTempRefLabels) {
						for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* prevTempRefLabelIt = prevTempRefLabels; prevTempRefLabelIt; prevTempRefLabelIt = prevTempRefLabelIt->getNext()) {
							if (prevTempRefLabelIt->getCacheValueCount() == neighbourRoleInstantiatedSetCount) {

								bool compatibleLabels = true;
								for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = prevTempRefLabelIt->getCacheValueLinker(); labelValueLinkerIt && compatibleLabels; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
									CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();

									CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef;
									if (cacheValue.getCacheValueIdentifier() == CCacheValue::CACHE_VALUE_TAG_AND_ENTRY) {
										CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)cacheValue.getIdentification();
										labelRef.setReferredLabelData(labelCacheItem);
									}
									else {
										CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpLabelWriteData = (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*)cacheValue.getIdentification();
										labelRef.setReferredTemporaryLabelData(tmpLabelWriteData);
									}

									if (!neigbourRoleInstantiatedSetCompination->contains(labelRef)) {
										compatibleLabels = false;
									}
								}

								if (compatibleLabels) {
									++mStatReusedHashedSignatureLabels;
									++mStatReusedTempLabels;
									tmpAssWriteDataLinker->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, prevTempRefLabelIt);
									return true;
								}
							}
						}
					}


					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
					newTempRefLabel->initLabelWriteData(signature, CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);


					bool hasNondeterministicElementsLabel = false;
					for (CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neigbourRoleInstantiatedSetTmpLabelLinkerIt = neigbourRoleInstantiatedSetTmpLabelLinker; neigbourRoleInstantiatedSetTmpLabelLinkerIt; neigbourRoleInstantiatedSetTmpLabelLinkerIt = neigbourRoleInstantiatedSetTmpLabelLinkerIt->getNext()) {
						CCacheValue cacheValue = mAssBackCacheReader->getCacheValue(neigbourRoleInstantiatedSetTmpLabelLinkerIt);
						if (neigbourRoleInstantiatedSetTmpLabelLinkerIt->getReferredLabelData() && neigbourRoleInstantiatedSetTmpLabelLinkerIt->getReferredLabelData()->hasStatusFlagsPartially(CBackendRepresentativeMemoryCachingFlags::FLAG_NONDETERMINISTIC_ELEMENTS)) {
							hasNondeterministicElementsLabel = true;
						}
						if (neigbourRoleInstantiatedSetTmpLabelLinkerIt->getReferredTemporaryLabelData() && neigbourRoleInstantiatedSetTmpLabelLinkerIt->getReferredTemporaryLabelData()->hasStatusFlagsPartially(CBackendRepresentativeMemoryCachingFlags::FLAG_NONDETERMINISTIC_ELEMENTS)) {
							hasNondeterministicElementsLabel = true;
						}
						CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
						labelValueLinker->initLabelValueLinker(cacheValue);
						newTempRefLabel->appendCacheValueLinker(labelValueLinker);
					}
					if (hasNondeterministicElementsLabel) {
						newTempRefLabel->addStatusFlags(CBackendRepresentativeMemoryCachingFlags::FLAG_NONDETERMINISTIC_ELEMENTS);
					}

					mNewTmpLabelList->append(newTempRefLabel);
					++mStatCreatedTempLabels;

					tmpAssWriteDataLinker->setReferredTemporaryLabelData(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, newTempRefLabel);
					prevTempRefLabels = newTempRefLabel->append(prevTempRefLabels);
					return true;
				}






				CBackendRepresentativeMemoryCacheTemporaryLabelReference CBackendAssociationCacheHandler::getNeighbourRoleInstantiatedSetLabelAssociationBackendItem(CLinkedNeighbourRoleAssertionLinker* linkedRoleAssertedLinker, CBackendRepresentativeMemoryCacheRoleAssertionLinker* linkedRoleAssertionConnectionLinker, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnectionData, CCalculationAlgorithmContext* calcAlgContext) {

					CBackendRepresentativeMemoryCacheTemporaryLabelReference** tmpCachingLabelRefData = nullptr;
					if (linkedRoleAssertedLinker && !linkedRoleAssertionConnectionLinker && !nominalConnectionData) {
						CPROCESSSET<TRoleInversionPair>* roleInversionSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
						for (CLinkedNeighbourRoleAssertionLinker* linkedRoleAssertedLinkerIt = linkedRoleAssertedLinker; linkedRoleAssertedLinkerIt; linkedRoleAssertedLinkerIt = linkedRoleAssertedLinkerIt->getNext()) {
							CRole* role = linkedRoleAssertedLinkerIt->getAssertedRole();
							bool inversed = linkedRoleAssertedLinkerIt->isAssertedRoleInversed();
							roleInversionSet->insert(TRoleInversionPair(role, inversed));
						}

						CBackendRepresentativeMemoryCacheTemporaryLabelReference*& tmpLabelRefData = (*(mRoleInversionSetTmpRefRoleInstantiatedSetLabelHashTypeArray[CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL]))[CProcessSetHasher<TRoleInversionPair>(roleInversionSet)];
						if (tmpLabelRefData) {
							return *tmpLabelRefData;
						} else {
							tmpLabelRefData = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelReference >::allocateAndConstruct(mMemAllocMan);
						}
						tmpCachingLabelRefData = &tmpLabelRefData;
					}



					CBackendRepresentativeMemoryCacheRoleAssertionLinker* superRoleAssertionLinker = nullptr;
					CPROCESSHASH<TRoleInversionPair, CBackendRepresentativeMemoryCacheRoleAssertionLinker*>* superRoleInversionLinkerHash = CObjectParameterizingAllocator< CPROCESSHASH<TRoleInversionPair, CBackendRepresentativeMemoryCacheRoleAssertionLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());




					CRole* topRole = calcAlgContext->getProcessingDataBox()->getOntology()->getRBox()->getTopObjectRole();
					for (CLinkedNeighbourRoleAssertionLinker* linkedRoleAssertedLinkerIt = linkedRoleAssertedLinker; linkedRoleAssertedLinkerIt; linkedRoleAssertedLinkerIt = linkedRoleAssertedLinkerIt->getNext()) {
						CRole* role = linkedRoleAssertedLinkerIt->getAssertedRole();
						bool inversed = linkedRoleAssertedLinkerIt->isAssertedRoleInversed();
						CBackendRepresentativeMemoryCacheRoleAssertionLinker*& assertionLinker = (*superRoleInversionLinkerHash)[TRoleInversionPair(role, inversed)];
						if (!assertionLinker) {
							for (CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getIndirectSuperRoleList(); superRoleLinkerIt; superRoleLinkerIt = superRoleLinkerIt->getNext()) {
								CRole* superRole = superRoleLinkerIt->getData();
								bool superRoleNegation = superRoleLinkerIt->isNegated() ^ inversed;
								if (superRole != topRole && superRoleInversionLinkerHash->value(TRoleInversionPair(superRole, superRoleNegation)) == nullptr) {
									CBackendRepresentativeMemoryCacheRoleAssertionLinker* newRoleLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheRoleAssertionLinker >::allocateAndConstruct(calcAlgContext->getMemoryAllocationManager());
									newRoleLinker->initRoleAssertionLinker(superRole, superRoleNegation, superRole == role && superRoleNegation == inversed, false, false);
									superRoleInversionLinkerHash->insert(TRoleInversionPair(superRole, superRoleNegation), newRoleLinker);
									superRoleAssertionLinker = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)newRoleLinker->append(superRoleAssertionLinker);
								}
							}
						} else {
							assertionLinker->setABoxAsserted(true);
						}
					}

					for (CBackendRepresentativeMemoryCacheRoleAssertionLinker* linkedRoleAssertionConnectionLinkerIt = linkedRoleAssertionConnectionLinker; linkedRoleAssertionConnectionLinkerIt; linkedRoleAssertionConnectionLinkerIt = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)linkedRoleAssertionConnectionLinkerIt->getNext()) {
						CRole* role = linkedRoleAssertionConnectionLinkerIt->getData();
						bool inversed = linkedRoleAssertionConnectionLinkerIt->isNegated();
						bool aboxAsserted = linkedRoleAssertionConnectionLinkerIt->isABoxAsserted();
						bool nominalConnected = linkedRoleAssertionConnectionLinkerIt->isNominalConnected();
						bool nondeterministic = linkedRoleAssertionConnectionLinkerIt->isNondeterministic();
						CBackendRepresentativeMemoryCacheRoleAssertionLinker*& assertionLinker = (*superRoleInversionLinkerHash)[TRoleInversionPair(role, inversed)];
						if (!assertionLinker) {
							CBackendRepresentativeMemoryCacheRoleAssertionLinker* newRoleLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheRoleAssertionLinker >::allocateAndConstruct(calcAlgContext->getMemoryAllocationManager());
							newRoleLinker->initRoleAssertionLinker(role, inversed, aboxAsserted, nominalConnected, nondeterministic);
							assertionLinker = newRoleLinker;
							superRoleAssertionLinker = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)newRoleLinker->append(superRoleAssertionLinker);
						}
					}


					for (CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnectionDataIt = nominalConnectionData; nominalConnectionDataIt; nominalConnectionDataIt = nominalConnectionDataIt->getNext()) {
						CIndividualReference nomConnIndi = nominalConnectionDataIt->getConnectedIndividual();
						CRole* role = nominalConnectionDataIt->getConnectionRole();
						bool inversed = nominalConnectionDataIt->isInversedConnection();
						CBackendRepresentativeMemoryCacheRoleAssertionLinker*& assertionLinker = (*superRoleInversionLinkerHash)[TRoleInversionPair(role, inversed)];
						if (!assertionLinker) {
							for (CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getIndirectSuperRoleList(); superRoleLinkerIt; superRoleLinkerIt = superRoleLinkerIt->getNext()) {
								CRole* superRole = superRoleLinkerIt->getData();
								bool superRoleNegation = superRoleLinkerIt->isNegated() ^ inversed;
								if (superRole != topRole && superRoleInversionLinkerHash->value(TRoleInversionPair(superRole, superRoleNegation)) == nullptr) {
									CBackendRepresentativeMemoryCacheRoleAssertionLinker* newRoleLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheRoleAssertionLinker >::allocateAndConstruct(calcAlgContext->getMemoryAllocationManager());
									newRoleLinker->initRoleAssertionLinker(superRole, superRoleNegation, false, true, false);
									superRoleInversionLinkerHash->insert(TRoleInversionPair(superRole, superRoleNegation), newRoleLinker);
									superRoleAssertionLinker = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)newRoleLinker->append(superRoleAssertionLinker);
								}
							}
						}
					}


					cint64 superRoleCount = superRoleAssertionLinker->getCount();
					cint64 signature = CBackendRepresentativeMemoryCacheUtilities::getRoleInversedLinkerSignature(superRoleAssertionLinker, false, superRoleCount);

					CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getLabelCacheEntryViaRoleAssertionLinker(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, signature, superRoleCount, superRoleAssertionLinker);
					if (cachedLabelItem) {
						++mStatReusedCachedLabels;
						if (tmpCachingLabelRefData) {
							(*tmpCachingLabelRefData)->setReferredLabelData(cachedLabelItem);
						}
						return CBackendRepresentativeMemoryCacheTemporaryLabelReference(cachedLabelItem);
					} else {
						// create new temporary label item
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = getNeighbourRoleInstantiatedSetLabelFromAssertionLinker(signature, superRoleCount, superRoleAssertionLinker, calcAlgContext);
						if (tmpCachingLabelRefData) {
							(*tmpCachingLabelRefData)->setReferredTemporaryLabelData(newTempRefLabel);
						}
						return CBackendRepresentativeMemoryCacheTemporaryLabelReference(newTempRefLabel);
					}

				}



				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendAssociationCacheHandler::getNeighbourRoleInstantiatedSetLabelFromAssertionLinker(cint64 signature, cint64 superRoleCount, CBackendRepresentativeMemoryCacheRoleAssertionLinker* superRoleAssertionLinker, CCalculationAlgorithmContext* calcAlgContext) {

					CCacheValueTmpLabelReferenceData*& cacheValuesTmpLabelRefData = (*mSuperRoleSignatureCacheValueTmpRefNeighbourInstantiatedRoleSetLabelPairHash)[signature];

					for (CCacheValueTmpLabelReferenceData* cacheValuesTmpLabelRefDataIt = cacheValuesTmpLabelRefData; cacheValuesTmpLabelRefDataIt; cacheValuesTmpLabelRefDataIt = cacheValuesTmpLabelRefDataIt->mNext) {
						
						if (superRoleCount == cacheValuesTmpLabelRefDataIt->mCacheValues->size()) {
							bool matching = true;
							for (CBackendRepresentativeMemoryCacheRoleAssertionLinker* superRoleLinkerIt = superRoleAssertionLinker; superRoleLinkerIt && matching; superRoleLinkerIt = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)superRoleLinkerIt->getNext()) {
								CRole* superRole = superRoleLinkerIt->getData();
								bool roleInversed = superRoleLinkerIt->isNegated();
								bool asserted = superRoleLinkerIt->isABoxAsserted();
								bool nomConnected = superRoleLinkerIt->isNominalConnected();
								bool nondeterministic = superRoleLinkerIt->isNondeterministic();
								CCacheValue cacheValue = mAssBackCacheReader->getCacheValue(superRole, roleInversed, asserted, nomConnected, nondeterministic);

								if (!cacheValuesTmpLabelRefDataIt->mCacheValues->contains(cacheValue)) {
									matching = false;
								}
							}
							if (matching) {
								return cacheValuesTmpLabelRefDataIt->mTmpLabelRef;
							}
						}
					}


					CPROCESSSET<CCacheValue>* superRoleCacheValueSet = CObjectParameterizingAllocator< CPROCESSSET<CCacheValue>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CCacheValueTmpLabelReferenceData* newCacheValuesTmpLabelRefData = CObjectAllocator< CCacheValueTmpLabelReferenceData >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
					newCacheValuesTmpLabelRefData->mCacheValues = superRoleCacheValueSet;
					newCacheValuesTmpLabelRefData->mNext = cacheValuesTmpLabelRefData;
					cacheValuesTmpLabelRefData = newCacheValuesTmpLabelRefData;


					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
					newTempRefLabel->initLabelWriteData(signature, CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL);

					bool hasNondeterministicRoles = false;
					for (CBackendRepresentativeMemoryCacheRoleAssertionLinker* superRoleLinkerIt = superRoleAssertionLinker; superRoleLinkerIt; superRoleLinkerIt = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)superRoleLinkerIt->getNext()) {
						CRole* superRole = superRoleLinkerIt->getData();
						bool roleInversed = superRoleLinkerIt->isNegated();
						bool asserted = superRoleLinkerIt->isABoxAsserted();
						bool nomConnected = superRoleLinkerIt->isNominalConnected();
						bool nondeterministic = superRoleLinkerIt->isNondeterministic();
						hasNondeterministicRoles |= nondeterministic;
						CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
						CCacheValue cacheValue = mAssBackCacheReader->getCacheValue(superRole, roleInversed, asserted, nomConnected, nondeterministic);

						newCacheValuesTmpLabelRefData->mCacheValues->insert(cacheValue);

						labelValueLinker->initLabelValueLinker(cacheValue);
						newTempRefLabel->appendCacheValueLinker(labelValueLinker);
					}
					if (hasNondeterministicRoles) {
						newTempRefLabel->addStatusFlags(CBackendRepresentativeMemoryCachingFlags::FLAG_NONDETERMINISTIC_ELEMENTS);
					}
					mNewTmpLabelList->append(newTempRefLabel);

					newCacheValuesTmpLabelRefData->mTmpLabelRef = newTempRefLabel;

					++mStatCreatedTempLabels;
					return newTempRefLabel;
				}









				const CCacheValue CBackendAssociationCacheHandler::getCacheValue(CConcept* concept, bool negation, bool deterministic) {
					return mAssBackCacheReader->getCacheValue(concept,negation, deterministic);
				}



				const CCacheValue CBackendAssociationCacheHandler::getCacheValue(CRole* role) {
					return mAssBackCacheReader->getCacheValue(role);
				}



				bool CBackendAssociationCacheHandler::visitConceptsOfAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, function<bool(CConcept* concept, bool negation, bool deterministic)> visitFunc, bool visitDeterministicConcepts, bool visitNonDeterministicConcepts, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitConceptsOfAssociatedFullConceptSetLabel(assData, labelItem, visitFunc, visitDeterministicConcepts, visitNonDeterministicConcepts);
				}

				bool CBackendAssociationCacheHandler::hasConceptInAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->hasConceptInAssociatedFullConceptSetLabel(assData, labelItem, concept, negation);
				}

				bool CBackendAssociationCacheHandler::hasConceptInAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool negation, bool deterministic, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->hasConceptInAssociatedFullConceptSetLabel(assData, labelItem, concept, negation, deterministic);
				}




				bool CBackendAssociationCacheHandler::visitIndividualIdsOfAssociatedIndividualSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* indiSetLabel, function<bool(cint64 indiId)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitIndividualIdsOfAssociatedIndividualSetLabel(assData, indiSetLabel, visitFunc);
				}

				bool CBackendAssociationCacheHandler::visitNominalIndirectlyConnectedIndividualIds(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 indiId, function<bool(cint64 indiId)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* nomConnData = getLoadedNominalIndividualIndirectConnectionData(indiId, nullptr, calcAlgContext);
					return mAssBackCacheReader->visitNominalIndirectlyConnectedIndividualIds(assData, nomConnData, visitFunc);
				}

				bool CBackendAssociationCacheHandler::visitConceptsOfAssociatedDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, function<bool(CConcept*, bool)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitConceptsOfAssociatedDeterministicConceptSetLabel(assData, visitFunc);
				}

				bool CBackendAssociationCacheHandler::visitConceptsOfAssociatedConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* conSetLabel, function<bool(CConcept*, bool)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitConceptsOfAssociatedConceptSetLabel(assData, conSetLabel, visitFunc);
				}

				bool CBackendAssociationCacheHandler::hasConceptInAssociatedDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->hasConceptInAssociatedDeterministicConceptSetLabel(assData, concept, negation);
				}


				bool CBackendAssociationCacheHandler::visitConceptsOfAssociatedNonDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, function<bool(CConcept*, bool)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitConceptsOfAssociatedNonDeterministicConceptSetLabel(assData, visitFunc);
				}

				bool CBackendAssociationCacheHandler::hasConceptInAssociatedNonDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->hasConceptInAssociatedNonDeterministicConceptSetLabel(assData, concept, negation);
				}



				bool CBackendAssociationCacheHandler::visitRolesOfAssociatedCombinedNeighbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, function<bool(CRole* role, bool inversed)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitRolesOfAssociatedCombinedNeigbourRoleSetLabel(assData, neighbourRoleSetLabel, visitFunc);
				}


				bool CBackendAssociationCacheHandler::visitLabelsOfAssociatedNeigbourRoleSetCombinationLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCompinationLabel, function<bool(CBackendRepresentativeMemoryLabelCacheItem* label)> visitFunc) {
					return mAssBackCacheReader->visitLabelsOfAssociatedNeigbourRoleSetCombinationLabel(assData, neighbourRoleSetCompinationLabel, visitFunc);
				}



				bool CBackendAssociationCacheHandler::visitRolesOfAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, function<bool(CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)> visitFunc) {
					return mAssBackCacheReader->visitRolesOfAssociatedNeigbourRoleSetLabel(assData, neighbourRoleSetLabel, visitFunc);
				}

				bool CBackendAssociationCacheHandler::hasRoleInAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic) {
					return mAssBackCacheReader->hasRoleInAssociatedNeigbourRoleSetLabel(assData, neighbourRoleSetLabel, role, inversed, assertionLinkBase, nominalLinkBase, nondeterministic);
				}

				bool CBackendAssociationCacheHandler::hasRoleInAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed, bool nondeterministic) {
					return mAssBackCacheReader->hasRoleInAssociatedNeigbourRoleSetLabel(assData, neighbourRoleSetLabel, role, inversed, nondeterministic);
				}



				bool CBackendAssociationCacheHandler::hasRoleInAssociatedCombinedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed) {
					return mAssBackCacheReader->hasRoleInAssociatedCombinedNeigbourRoleSetLabel(assData, neighbourRoleSetLabel, role, inversed);
				}



				bool CBackendAssociationCacheHandler::hasRoleInAssociatedCompinationRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed) {
					return mAssBackCacheReader->hasRoleInAssociatedCompinationRoleSetLabel(assData, neighbourRoleSetLabel, role, inversed);
				}


				bool CBackendAssociationCacheHandler::hasIndividualIdsInAssociatedIndividualSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* indiSetLabel, cint64 indiId) {
					return mAssBackCacheReader->hasIndividualIdsInAssociatedIndividualSetLabel(assData, indiSetLabel, indiId);
				}

				bool CBackendAssociationCacheHandler::hasRoleToNeigbourInAssociatedNeighbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 neighbourIndiId, CRole* role, bool inversed) {
					return mAssBackCacheReader->hasRoleToNeigbourInAssociatedNeighbourRoleSetLabel(assData, neighbourIndiId, role, inversed);
				}

				bool CBackendAssociationCacheHandler::visitRolesToNeigbourInAssociatedNeighbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 neighbourIndiId, function<bool(CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitRolesToNeigbourInAssociatedNeighbourRoleSetLabel(assData, neighbourIndiId, visitFunc);
				}





				bool CBackendAssociationCacheHandler::visitRolesOfAssociatedCompinationRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* combinationRoleSetLabel, function<bool(CRole* role, bool inversed)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->visitRolesOfAssociatedCompinationRoleSetLabel(assData, combinationRoleSetLabel, visitFunc);
				}





				bool CBackendAssociationCacheHandler::checkLabelContainsCacheValue(CBackendRepresentativeMemoryLabelCacheItem* cacheItemLabel, const CCacheValue& cacheValue, CCalculationAlgorithmContext* calcAlgContext) {
					if (cacheItemLabel) {
						CBackendRepresentativeMemoryLabelValueLinker* containedValueLinker = cacheItemLabel->getTagCacheValueHash(false)->value(cacheValue.getTag());
						if (containedValueLinker && containedValueLinker->getCacheValue() == cacheValue) {
							return true;
						}
					}
					return false;
				}

				bool CBackendAssociationCacheHandler::checkLabelContainsAllLinkedCacheValues(CBackendRepresentativeMemoryLabelCacheItem* cacheItemLabel, CBackendRepresentativeMemoryLabelValueLinker* cacheValueLinker, CCalculationAlgorithmContext* calcAlgContext) {
					for (CBackendRepresentativeMemoryLabelValueLinker* cacheValueLinkerIt = cacheValueLinker; cacheValueLinkerIt; cacheValueLinkerIt = cacheValueLinkerIt->getNext()) {
						if (!checkLabelContainsCacheValue(cacheItemLabel, cacheValueLinkerIt->getCacheValue(), calcAlgContext)) {
							return false;
						}
					}
					return true;
				}


				bool CBackendAssociationCacheHandler::checkNeighbourRoleInstantiatedSetCompinationLabelContainsNeighbourRoleInstantiatedSet(CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleInstantiatedSetCompinationLabel, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleInstantiatedSetLabel, CCalculationAlgorithmContext* calcAlgContext) {
					if (neighbourRoleInstantiatedSetCompinationLabel) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker tempLabelRefDataLinker;
						CBackendRepresentativeMemoryCacheTemporaryLabelReference tempLabelRef;
						tempLabelRef.setReferredLabelData(neighbourRoleInstantiatedSetLabel);
						tempLabelRefDataLinker.initLabelReferenceDataLinker(tempLabelRef);
						return checkLabelContainsCacheValue(neighbourRoleInstantiatedSetCompinationLabel, mAssBackCacheReader->getCacheValue(&tempLabelRefDataLinker), calcAlgContext);
					}
					return false;
				}

				bool CBackendAssociationCacheHandler::checkNeighbourRoleInstantiatedSetCompinationLabelContainsNeighbourRoleInstantiatedSet(CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleInstantiatedSetCompinationLabel, CBackendRepresentativeMemoryCacheTemporaryLabelReference& tempLabelRef, CCalculationAlgorithmContext* calcAlgContext) {
					if (neighbourRoleInstantiatedSetCompinationLabel) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker tempLabelRefDataLinker;
						tempLabelRefDataLinker.initLabelReferenceDataLinker(tempLabelRef);
						return checkLabelContainsCacheValue(neighbourRoleInstantiatedSetCompinationLabel, mAssBackCacheReader->getCacheValue(&tempLabelRefDataLinker), calcAlgContext);
					}
					return false;
				}


				bool CBackendAssociationCacheHandler::checkNeighbourRoleInstantiatedSetCompinationLabelContainsNeighbourRoleInstantiatedSet(CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleInstantiatedSetCompinationLabel, CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neighbourRoleInstantiatedSetLabelDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
					return checkLabelContainsCacheValue(neighbourRoleInstantiatedSetCompinationLabel, mAssBackCacheReader->getCacheValue(neighbourRoleInstantiatedSetLabelDataLinker), calcAlgContext);
				}
















			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
