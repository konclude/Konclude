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

#include "CSaturationNodeBackendAssociationCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CSaturationNodeBackendAssociationCacheHandler::CSaturationNodeBackendAssociationCacheHandler(CBackendRepresentativeMemoryCacheReader* backAssCacheReader, CBackendRepresentativeMemoryCacheWriter* backAssCacheWriter, COccurrenceStatisticsCacheWriter* occStatsCacheWriter) : CBackendAssociationCacheHandler(backAssCacheReader, backAssCacheWriter) {
					mOccStatsCacheWriter = occStatsCacheWriter;
					mConfDirectCardinalityInsufficient = false;
					mConfOccurrenceStatisticsCounting = true;
				}


				


				void CSaturationNodeBackendAssociationCacheHandler::readCalculationConfig(CCalculationConfigurationExtension* config) {
					if (config) {
						mConfOccurrenceStatisticsCounting = config->isOccurrenceStatisticsCollectionActivated();
					} else {
						mConfOccurrenceStatisticsCounting = true;
					}
				}








				bool CSaturationNodeBackendAssociationCacheHandler::determineCardinalityAssociationBackendItem(CIndividualSaturationProcessNode* satNode, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {

					if (mConfDirectCardinalityInsufficient) {
						tmpAssWriteDataLinker->setCompletelyHandled(false);
						tmpAssWriteDataLinker->setCompletelySaturated(false);
						return false;
					} else {
						function<void(CPROCESSHASH<cint64, CRoleCardinalityCountData>* superRoleTagUsedCardCountHash)> initFunc = [&](CPROCESSHASH<cint64, CRoleCardinalityCountData>* superRoleTagUsedCardCountHash)->void {
							CReapplyConceptSaturationLabelSet* conSet = satNode->getReapplyConceptSaturationLabelSet(false);
							if (conSet) {
								for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
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
						return CBackendAssociationCacheHandler::determineCardinalityAssociationBackendItem(initFunc, roleUsedCardHash, true, tmpAssWriteDataLinker, calcAlgContext);
					}
				}









				bool CSaturationNodeBackendAssociationCacheHandler::collectConceptOccurrenceStatistics(CIndividualSaturationProcessNode* indiNode, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptSaturationLabelSet* conSet = indiNode->getReapplyConceptSaturationLabelSet(false);
					CConcreteOntology* ontology = calcAlgContext->getProcessingDataBox()->getOntology();
					for (CConceptSaturationDescriptor* conSatDesLinkerIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesLinkerIt; conSatDesLinkerIt = conSatDesLinkerIt->getNext()) {
						CConcept* concept = conSatDesLinkerIt->getConcept();
						mOccStatsCacheWriter->incConceptInstanceOccurrencceStatistics(ontology, concept->getConceptTag(), 1, 0, 1, 0);
					}
					return true;
				}







				bool CSaturationNodeBackendAssociationCacheHandler::determineFullConceptSetLabelAssociationBackendItem(CIndividualSaturationProcessNode* satNode, cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptSaturationLabelSet* conSet = satNode->getReapplyConceptSaturationLabelSet(false);
					if (!conSet) {
						return false;
					}
					CIndividualSaturationProcessNode* copiedNode = satNode->getCopyIndividualNode();
					if (copiedNode && copiedNode->getReapplyConceptSaturationLabelSet(false) && copiedNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker() == conSet->getConceptSaturationDescriptionLinker()) {
						return determineFullConceptSetLabelAssociationBackendItem(copiedNode, labelType,tmpAssWriteDataLinker,calcAlgContext);
					}

					QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>* hashedRefPair = nullptr;
					if (satNode->getNominalIndividual() == nullptr) {
						QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>& refPair = (*mIndiNodeRefDetConceptSetLabelHash)[satNode];
						if (refPair.first) {
							tmpAssWriteDataLinker->setReferredLabelData(labelType, refPair.first);
							++mStatReusedHashedIndiLabels;
							return true;
						} else if (refPair.second) {
							tmpAssWriteDataLinker->setReferredTemporaryLabelData(labelType, refPair.second);
							++mStatReusedHashedIndiLabels;
							return true;
						}
						hashedRefPair = &refPair;
					}


					cint64 conCount = conSet->getConceptCount();
					cint64 conSetSigValue = CBackendRepresentativeMemoryCacheUtilities::getConceptDescriptorSignature(conSet->getConceptSaturationDescriptionLinker(), conCount, nullptr);

					
					CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getConceptSetLabelCacheEntry(labelType, conSetSigValue,conSet->getConceptCount(),conSet->getConceptSaturationDescriptionLinker());
					if (cachedLabelItem) {
						++mStatReusedCachedLabels;
						if (hashedRefPair) {
							hashedRefPair->first = cachedLabelItem;
						}
						tmpAssWriteDataLinker->setReferredLabelData(labelType, cachedLabelItem);
						return true;
					}


					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*& prevTempRefLabels = (*mSignatureTmpRefDetConceptSetLabelHash)[conSetSigValue];
					if (prevTempRefLabels) {
						for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* prevTempRefLabelIt = prevTempRefLabels; prevTempRefLabelIt; prevTempRefLabelIt = prevTempRefLabelIt->getNext()) {
							if (prevTempRefLabelIt->getCacheValueCount() == conSet->getConceptCount()) {
								bool compatibleConcepts = true;
								for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = prevTempRefLabelIt->getCacheValueLinker(); labelValueLinkerIt && compatibleConcepts; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
									CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
									CConcept* concept = (CConcept*)cacheValue.getIdentification();
									bool negation = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
									if (!conSet->hasConcept(concept,negation)) {
										compatibleConcepts = false;
									}
								}
								if (compatibleConcepts) {
									++mStatReusedHashedSignatureLabels;
									++mStatReusedTempLabels;
									if (hashedRefPair) {
										hashedRefPair->second = prevTempRefLabelIt;
									}
									tmpAssWriteDataLinker->setReferredTemporaryLabelData(labelType, prevTempRefLabelIt);
									return true;
								}
							}
						}
					}


					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
					newTempRefLabel->initLabelWriteData(conSetSigValue, labelType);
					CIndividualSaturationProcessNodeStatusFlags* indFlags = satNode->getIndirectStatusFlags();
					CIndividualSaturationProcessNodeStatusFlags* dirFlags = satNode->getDirectStatusFlags();
					bool insufficient = indFlags->hasInsufficientFlag() || !indFlags->hasCompletedFlag() || !dirFlags->hasCompletedFlag();
					if (!insufficient) {
						newTempRefLabel->setCompletelySaturated(true);
						newTempRefLabel->setCompletelyHandled(true);
					}
					for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
						CConcept* concept = conSatDesIt->getConcept();
						bool negation = conSatDesIt->isNegated();
						CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
						labelValueLinker->initLabelValueLinker(getCacheValue(concept,negation));
						newTempRefLabel->appendCacheValueLinker(labelValueLinker);
					}
					mNewTmpLabelList->append(newTempRefLabel);
					++mStatCreatedTempLabels;

					if (hashedRefPair) {
						hashedRefPair->second = newTempRefLabel;
					}
					tmpAssWriteDataLinker->setReferredTemporaryLabelData(labelType, newTempRefLabel);
					prevTempRefLabels = newTempRefLabel->append(prevTempRefLabels);
					return true;
				}



































				bool CSaturationNodeBackendAssociationCacheHandler::determineSaturationNodeIndividualCombinedNeighbourRoleInstantiatedSetLabelAssociationBackendItem(CIndividualSaturationProcessNode* satNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {

					CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnDataLinker = mNominalIndiIdRoleConnectionDataHash->value(satNode->getNominalIndividual()->getIndividualID());

					CIndividualSaturationProcessNodeExtensionData* extensionData = satNode->getIndividualExtensionData(false);
					if (!extensionData && !nominalConnDataLinker) {
						return false;
					}

					CLinkedNeighbourRoleAssertionSaturationHash* linkedNeighbourRoleAssHash = extensionData->getLinkedNeighbourRoleAssertionHash(false);
					if (!linkedNeighbourRoleAssHash && !nominalConnDataLinker) {
						return false;
					}

					CPROCESSSET<TRoleInversionPair>* combinedNeigbourRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					CSortedNegLinker<CRole*>* combinedNeigbourRoleInstantiatedLinker = nullptr;


					CPROCESSHASH<cint64, CLinkedNeighbourRoleAssertionSaturationData*>* neighbourRoleAssertionDataHash = nullptr;
					if (linkedNeighbourRoleAssHash) {
						neighbourRoleAssertionDataHash = linkedNeighbourRoleAssHash->getNeighbourRoleAssertionDataHash();
					}
					if (neighbourRoleAssertionDataHash) {
						for (CPROCESSHASH<cint64, CLinkedNeighbourRoleAssertionSaturationData*>::const_iterator it = neighbourRoleAssertionDataHash->constBegin(), itEnd = neighbourRoleAssertionDataHash->constEnd(); it != itEnd; ++it) {
							cint64 neighbourIndiId = it.key();
							CLinkedNeighbourRoleAssertionSaturationData* data = it.value();
							for (CLinkedNeighbourRoleAssertionLinker* linkedRoleAssertedLinkerIt = data->getLinkedRoleAssertedLinker(); linkedRoleAssertedLinkerIt; linkedRoleAssertedLinkerIt = linkedRoleAssertedLinkerIt->getNext()) {
								CRole* role = linkedRoleAssertedLinkerIt->getAssertedRole();
								bool inversed = linkedRoleAssertedLinkerIt->isAssertedRoleInversed();

								if (!combinedNeigbourRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
									CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
									combinedNeigbourRoleInstantiatedLinker = roleLinker->init(role, inversed, combinedNeigbourRoleInstantiatedLinker);
									combinedNeigbourRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
								}
							}
						}
					}

					for (CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnData = nominalConnDataLinker; nominalConnData; nominalConnData = nominalConnData->getNext()) {
						CRole* role = nominalConnData->getConnectionRole();
						bool inversed = nominalConnData->isInversedConnection();

						if (!combinedNeigbourRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
							CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
							combinedNeigbourRoleInstantiatedLinker = roleLinker->init(role, inversed, combinedNeigbourRoleInstantiatedLinker);
							combinedNeigbourRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
						}
					}

					if (combinedNeigbourRoleInstantiatedLinker) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, combinedNeigbourRoleInstantiatedLinker, combinedNeigbourRoleInstantiatedSet, true, mRoleInversionSetTmpRefCombinedNeighbourRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, labelRef);

						if (mConfOccurrenceStatisticsCounting) {
							CConcreteOntology* ontology = calcAlgContext->getProcessingDataBox()->getOntology();
							visitRolesOfAssociatedCompinationRoleSetLabel(labelRef, [&](CRole* role, bool inversed)->bool {
								if (!inversed) {
									mOccStatsCacheWriter->incRoleInstanceOccurrencceStatistics(ontology, role->getRoleTag(), 1, 0, 1, 0, 0, 0);
								}
								return true;
							}, calcAlgContext);
						}

						return true;
					}
					else {
						return false;
					}


				}












				CIndividualSaturationProcessNode* CSaturationNodeBackendAssociationCacheHandler::getSuccessorSaturationNode(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode* satNode, CCalculationAlgorithmContext* calcAlgContext) {
					CConcept* concept = conDes->getConcept();
					bool conNegation = conDes->isNegated();
					CRole* role = concept->getRole();
					CConceptData* conceptData = concept->getConceptData();
					if (conceptData) {
						CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* extSatCalcRefLinkData = confSatRefLinkingData->getExistentialSuccessorConceptSaturationReferenceLinkingData();
							if (extSatCalcRefLinkData) {
								CIndividualSaturationProcessNode* existIndiNode = (CIndividualSaturationProcessNode*)extSatCalcRefLinkData->getIndividualProcessNodeForConcept();
								if (existIndiNode) {
									return existIndiNode;
								}
							}
						}
					}
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					while (conceptOpLinkerIt) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opConNegation = conceptOpLinkerIt->isNegated() ^ conNegation;
						CConceptData* opConceptData = opConcept->getConceptData();
						if (opConceptData) {
							CConceptProcessData* conProcData = (CConceptProcessData*)opConceptData;
							CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
							if (conRefLinking) {
								CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
								CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(opConNegation);
								if (satCalcRefLinkData) {
									CIndividualSaturationProcessNode* existIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
									if (existIndiNode) {
										return existIndiNode;
									}
								}
							}
						}
						conceptOpLinkerIt = conceptOpLinkerIt->getNext();
					}
					CConcept* baseTopConcept = nullptr;
					if (!role->isDataRole()) {
						baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
					} else {
						baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopDataRangeConcept();
					}
					CConcept* opConcept = baseTopConcept;
					bool opConNegation = false;
					CConceptData* opConceptData = opConcept->getConceptData();
					if (opConceptData) {
						CConceptProcessData* conProcData = (CConceptProcessData*)opConceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(opConNegation);
							if (satCalcRefLinkData) {
								CIndividualSaturationProcessNode* existIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
								if (existIndiNode) {
									return existIndiNode;
								}
							}
						}
					}
					return nullptr;
				}





				bool CSaturationNodeBackendAssociationCacheHandler::collectSaturationNodeSuccessorRelatedRoles(cint64 satIndiId, CIndividualSaturationProcessNode* satNode, CPROCESSSET<TRoleInversionPair>** roleSet, CSortedNegLinker<CRole*>** roleLinker,
						CRole* role, CIndividualSaturationProcessNode* succSatNode, CSaturationSuccessorData* succData,
						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >* nominalIndiIdRoleConnectionDataHash, CPROCESSSET<cint64>** existentialIndirectlyConnectedNominalIndividualSet, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, CCalculationAlgorithmContext* calcAlgContext) {

					bool collected = false;
					if (roleSet && !(*roleSet)->contains(TRoleInversionPair(role, false))) {
						(*roleSet)->insert(TRoleInversionPair(role, false));
						collected = true;

						CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
						*roleLinker = newRoleLinker->init(role, false, *roleLinker);

						if (roleUsedCardHash) {
							cint64 cardCount = 1;
							if (succData) {
								cardCount = succData->mSuccCount;
							}
							(*roleUsedCardHash)[role] += cardCount;
						}
					}

					if (mConfOccurrenceStatisticsCounting && succSatNode && !succSatNode->isSeparated()) {
						succSatNode->setOccurrenceStatisticsCollectingRequired(true);
					}

					if (succSatNode && succSatNode->getIndirectStatusFlags()->hasNominalConnectionFlag()) {
						CSuccessorConnectedNominalSet* succConnNomSet = succSatNode->getSuccessorConnectedNominalSet(false);
						if (succConnNomSet && existentialIndirectlyConnectedNominalIndividualSet) {
							for (CSuccessorConnectedNominalSet::const_iterator it = succConnNomSet->constBegin(), itEnd = succConnNomSet->constEnd(); it != itEnd; ++it) {
								cint64 nomIndiId = *it;
								if (!*existentialIndirectlyConnectedNominalIndividualSet) {
									*existentialIndirectlyConnectedNominalIndividualSet = CObjectParameterizingAllocator< CPROCESSSET<cint64>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
								}
								(*existentialIndirectlyConnectedNominalIndividualSet)->insert(nomIndiId);
							}
						}
					}


					if (nominalIndiIdRoleConnectionDataHash && (succSatNode && succSatNode->getIntegratedNominalIndividual() || succData && succData->mVALUENominalConnection)) {
						cint64 nominalId = -1;
						if (succSatNode && succSatNode->getIntegratedNominalIndividual()) {
							nominalId = succSatNode->getIntegratedNominalIndividual()->getIndividualID();
						} else if (succData && succData->mVALUENominalConnection) {
							nominalId = succData->mVALUENominalID;
						}
						collected = true;

						CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData*& prevNominalConnectionData = (*nominalIndiIdRoleConnectionDataHash)[nominalId];
						CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* newNominalConnectionData = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
						newNominalConnectionData->initNominalRoleConnectionData(role, true, CIndividualReference(satIndiId));
						newNominalConnectionData->setNext(prevNominalConnectionData);
						prevNominalConnectionData = newNominalConnectionData;


						CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData*& prevSaturationNodeConnectionData = (*nominalIndiIdRoleConnectionDataHash)[satIndiId];
						CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* newSaturationNodeConnectionData = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
						newSaturationNodeConnectionData->initNominalRoleConnectionData(role, false, CIndividualReference(nominalId));
						newSaturationNodeConnectionData->setNext(prevSaturationNodeConnectionData);
						prevSaturationNodeConnectionData = newSaturationNodeConnectionData;
					}
					return collected;
				}



				bool CSaturationNodeBackendAssociationCacheHandler::collectSaturationNodeSuccessorRelatedRoles(cint64 satIndiId, CIndividualSaturationProcessNode* satNode, CPROCESSSET<TRoleInversionPair>** roleSet, CSortedNegLinker<CRole*>** roleLinker,
						bool collectExistentialConnectionRoles, bool collectNominalConnectionRoles, bool collectDataConnectionRoles, bool analyseConcepts, bool analyseSuccessorLinks, 
						CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >* nominalIndiIdRoleConnectionDataHash, CPROCESSSET<cint64>** existentialIndirectlyConnectedNominalIndividualSet, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, 
						CPROCESSHASH<TRoleInversionPair, CSuccessorRelatedNodesAlternativeCheckingData>* successorRelatedNodesAlternativeCheckingHash, CCalculationAlgorithmContext* calcAlgContext) {

					CReapplyConceptSaturationLabelSet* conSet = satNode->getReapplyConceptSaturationLabelSet(false);
					if (!conSet) {
						return false;
					}

					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = satNode->getLinkedRoleSuccessorHash(false);

					CIndividualSaturationProcessNode* copiedNode = satNode->getCopyIndividualNode();
					if (copiedNode && copiedNode->getReapplyConceptSaturationLabelSet(false) && copiedNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker() == conSet->getConceptSaturationDescriptionLinker() && (!linkedSuccHash || copiedNode->getLinkedRoleSuccessorHash(false))) {
						return collectSaturationNodeSuccessorRelatedRoles(satIndiId, copiedNode, roleSet, roleLinker, collectExistentialConnectionRoles, collectNominalConnectionRoles, collectDataConnectionRoles, analyseConcepts, analyseSuccessorLinks, nominalIndiIdRoleConnectionDataHash, existentialIndirectlyConnectedNominalIndividualSet, roleUsedCardHash, successorRelatedNodesAlternativeCheckingHash, calcAlgContext);
					}

					bool collected = false;

					if (analyseSuccessorLinks && linkedSuccHash) {
						CPROCESSHASH<CRole*, CLinkedRoleSaturationSuccessorData*>* linkSuccDatahash = linkedSuccHash->getLinkedRoleSuccessorHash();
						for (CPROCESSHASH<CRole*, CLinkedRoleSaturationSuccessorData*>::const_iterator it = linkSuccDatahash->constBegin(), itEnd = linkSuccDatahash->constEnd(); it != itEnd; ++it) {
							CRole* role = it.key();
							CLinkedRoleSaturationSuccessorData* linkSuccData = it.value();
							if (linkSuccData->mSuccCount > 0) {
								CPROCESSMAP<cint64, CSaturationSuccessorData*>* succNodeMap = linkSuccData->getSuccessorNodeDataMap(false);
								for (CPROCESSMAP<cint64, CSaturationSuccessorData*>::const_iterator succIt = succNodeMap->constBegin(), succItEnd = succNodeMap->constEnd(); succIt != succItEnd; ++succIt) {
									CSaturationSuccessorData* succData = succIt.value();
									if (succData->mActiveCount > 0) {
										CIndividualSaturationProcessNode* succSatNode = succData->mSuccIndiNode;
										bool collectSuccessorNode = true;
										if (succSatNode->isABoxIndividualRepresentationNode()) {
											collectSuccessorNode = false;
										}
										if (succSatNode && succSatNode->getIntegratedNominalIndividual() || succData->mVALUENominalConnection) {
											if (!collectNominalConnectionRoles) {
												collectSuccessorNode = false;
											}
										} else {
											if (role->isDataRole()) {
												if (!collectDataConnectionRoles) {
													collectSuccessorNode = false;
												}
											} else {
												if (!collectExistentialConnectionRoles) {
													collectSuccessorNode = false;
												}
											}
										}
										if (collectSuccessorNode) {
											collected |= collectSaturationNodeSuccessorRelatedRoles(satIndiId, satNode, roleSet, roleLinker, role, succSatNode, succData, nominalIndiIdRoleConnectionDataHash, existentialIndirectlyConnectedNominalIndividualSet, roleUsedCardHash, calcAlgContext);
										}
									}
								}
							}
						}

					} else if (analyseConcepts) {
						for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
							CConcept* concept = conSatDesIt->getConcept();
							bool conNegation = conSatDesIt->isNegated();
							cint64 conCode = concept->getOperatorCode();
							CRole* role = concept->getRole();
							bool existentialSuccessorCand = false;
							if (!conNegation && conCode == CCVALUE && collectNominalConnectionRoles) {
								CSaturationSuccessorData succData;
								succData.mVALUENominalConnection = true;
								succData.mVALUENominalID = concept->getNominalIndividual()->getIndividualID();
								collected |= collectSaturationNodeSuccessorRelatedRoles(satIndiId, satNode, roleSet, roleLinker, role, nullptr, &succData, nominalIndiIdRoleConnectionDataHash, existentialIndirectlyConnectedNominalIndividualSet, roleUsedCardHash, calcAlgContext);
							}
							if (!conNegation && conCode == CCATLEAST || conNegation && conCode == CCATMOST) {
								CIndividualSaturationProcessNode* succSatNode = getSuccessorSaturationNode(conSatDesIt, satNode, calcAlgContext);
								CSaturationSuccessorData succData;
								succData.mSuccCount = concept->getParameter();
								if (conNegation) {
									succData.mSuccCount--;
								}
								bool collectSuccessorNode = true;
								if (successorRelatedNodesAlternativeCheckingHash) {
									CSuccessorRelatedNodesAlternativeCheckingData alternativeCheckingData = successorRelatedNodesAlternativeCheckingHash->value(TRoleInversionPair(role, false));
									for (CXLinker<CIndividualSaturationProcessNode*>* alternativeNodeLinkerIt = alternativeCheckingData.mCheckingNodeLinker; alternativeNodeLinkerIt && collectSuccessorNode; alternativeNodeLinkerIt = alternativeNodeLinkerIt->getNext()) {
										CIndividualSaturationProcessNode* alternativeCheckingNode = alternativeNodeLinkerIt->getData();
										if (alternativeCheckingNode && succData.mSuccCount == 1) {
											bool containsExistentiallyRestrictedConcept = true;
											if (concept->getOperandList()) {
												for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && containsExistentiallyRestrictedConcept; opConLinkerIt = opConLinkerIt->getNext()) {
													CConcept* opCon = opConLinkerIt->getData();
													bool opConNeg = opConLinkerIt->isNegated();
													if (!alternativeCheckingNode->getReapplyConceptSaturationLabelSet(false) || !alternativeCheckingNode->getReapplyConceptSaturationLabelSet(false)->containsConcept(opCon, opConNeg)) {
														containsExistentiallyRestrictedConcept = false;
													}
												}
											}
											if (containsExistentiallyRestrictedConcept) {
												collectSuccessorNode = false;
											}
										}
									}
								}

								if (collectSuccessorNode) {
									collected |= collectSaturationNodeSuccessorRelatedRoles(satIndiId, satNode, roleSet, roleLinker, role, succSatNode, &succData, nominalIndiIdRoleConnectionDataHash, existentialIndirectlyConnectedNominalIndividualSet, roleUsedCardHash, calcAlgContext);
								}
							}

							if (!conNegation && (conCode == CCSOME || conCode == CCAQSOME) || conNegation && (conCode == CCALL)) {
								existentialSuccessorCand = true;
							}
							if (role && existentialSuccessorCand) {
								CIndividualSaturationProcessNode* succSatNode = getSuccessorSaturationNode(conSatDesIt, satNode, calcAlgContext);
								if (succSatNode) {
									bool collectSuccessorNode = true;
									if (succSatNode->getIntegratedNominalIndividual()) {
										if (!collectNominalConnectionRoles) {
											collectSuccessorNode = false;
										}
									} else {
										if (role->isDataRole()) {
											if (!collectDataConnectionRoles) {
												collectSuccessorNode = false;
											}
										} else {
											if (!collectExistentialConnectionRoles) {
												collectSuccessorNode = false;
											}
										}
									}

									if (collectSuccessorNode && successorRelatedNodesAlternativeCheckingHash) {
										CSuccessorRelatedNodesAlternativeCheckingData alternativeCheckingData = successorRelatedNodesAlternativeCheckingHash->value(TRoleInversionPair(role, false));
										for (CXLinker<CIndividualSaturationProcessNode*>* alternativeNodeLinkerIt = alternativeCheckingData.mCheckingNodeLinker; alternativeNodeLinkerIt && collectSuccessorNode; alternativeNodeLinkerIt = alternativeNodeLinkerIt->getNext()) {
											CIndividualSaturationProcessNode* alternativeCheckingNode = alternativeNodeLinkerIt->getData();
											if (alternativeCheckingNode) {
												bool containsExistentiallyRestrictedConcept = true;
												if (concept->getOperandList()) {
													for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && containsExistentiallyRestrictedConcept; opConLinkerIt = opConLinkerIt->getNext()) {
														CConcept* opCon = opConLinkerIt->getData();
														bool opConNeg = opConLinkerIt->isNegated() ^ conNegation;
														if (!alternativeCheckingNode->getReapplyConceptSaturationLabelSet(false) || !alternativeCheckingNode->getReapplyConceptSaturationLabelSet(false)->containsConcept(opCon, opConNeg)) {
															containsExistentiallyRestrictedConcept = false;
														}
													}
												}
												if (containsExistentiallyRestrictedConcept) {
													collectSuccessorNode = false;
												}
											}
										}
									}

									if (collectSuccessorNode) {
										collected |= collectSaturationNodeSuccessorRelatedRoles(satIndiId, satNode, roleSet, roleLinker, role, succSatNode, nullptr, nominalIndiIdRoleConnectionDataHash, existentialIndirectlyConnectedNominalIndividualSet, roleUsedCardHash, calcAlgContext);
									}
								}
							}

						}
					}

					return collected;
				}




				bool CSaturationNodeBackendAssociationCacheHandler::determineCombinedExistentialRoleInstantiatedSetLabelAssociationBackendItem(CIndividualSaturationProcessNode* satNode, CPROCESSSET<cint64>** existentialIndirectlyConnectedNominalIndividualSet, CPROCESSHASH<CRole*, cint64>* roleUsedCardHash, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {


					CPROCESSHASH<TRoleInversionPair, CSuccessorRelatedNodesAlternativeCheckingData>* successorRelatedNodesAlternativeCheckingHash = nullptr;

					CIndividualSaturationProcessNodeExtensionData* extensionData = satNode->getIndividualExtensionData(false);
					// don't use it at the moment since it causes errors
					// TODO: fix
					if (extensionData) {
						CLinkedNeighbourRoleAssertionSaturationHash* linkedNeighbourRoleAssHash = extensionData->getLinkedNeighbourRoleAssertionHash(false);
						CPROCESSHASH<cint64, CLinkedNeighbourRoleAssertionSaturationData*>* neighbourRoleAssertionDataHash = nullptr;
						if (linkedNeighbourRoleAssHash) {
							neighbourRoleAssertionDataHash = linkedNeighbourRoleAssHash->getNeighbourRoleAssertionDataHash();
						}
						if (neighbourRoleAssertionDataHash) {
							successorRelatedNodesAlternativeCheckingHash = CObjectParameterizingAllocator< CPROCESSHASH<TRoleInversionPair, CSuccessorRelatedNodesAlternativeCheckingData>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
							CRole* lastRole = nullptr;
							bool lastRoleInversion = false;
							cint64 lastRoleUsedCount = 0;
							for (CPROCESSHASH<cint64, CLinkedNeighbourRoleAssertionSaturationData*>::const_iterator it = neighbourRoleAssertionDataHash->constBegin(), itEnd = neighbourRoleAssertionDataHash->constEnd(); it != itEnd; ++it) {
								cint64 neighbourIndiId = it.key();
								CLinkedNeighbourRoleAssertionSaturationData* data = it.value();
								for (CLinkedNeighbourRoleAssertionSaturationNodeLinker* linkedRoleAssertedLinkerIt = data->getLinkedRoleAssertedLinker(); linkedRoleAssertedLinkerIt; linkedRoleAssertedLinkerIt = (CLinkedNeighbourRoleAssertionSaturationNodeLinker*)linkedRoleAssertedLinkerIt->getNext()) {
									CRole* role = linkedRoleAssertedLinkerIt->getAssertedRole();
									bool inversed = linkedRoleAssertedLinkerIt->isAssertedRoleInversed();

									if ((role == lastRole && lastRoleInversion == inversed && lastRoleUsedCount < 3) || role != lastRole || lastRoleInversion != inversed) {
										if (role == lastRole && lastRoleInversion == inversed) {
											++lastRoleUsedCount;
										} else {
											lastRole = role;
											lastRoleInversion = inversed;
											lastRoleUsedCount = 0;
										}

										CIndividualSaturationProcessNode* connectedSatNode = linkedRoleAssertedLinkerIt->getConnectedSaturationNode();

										for (CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getIndirectSuperRoleList(); superRoleLinkerIt; superRoleLinkerIt = superRoleLinkerIt->getNext()) {
											CRole* superRole = superRoleLinkerIt->getData();
											bool superRoleInversion = inversed ^ superRoleLinkerIt->isNegated();

											if (!superRoleInversion) {
												CSuccessorRelatedNodesAlternativeCheckingData& checkingData = (*successorRelatedNodesAlternativeCheckingHash)[TRoleInversionPair(superRole, superRoleInversion)];
												if (!checkingData.mRole) {
													checkingData.mRole = superRole;
													checkingData.mInversed = superRoleInversion;
												}
												if (checkingData.mCheckingNodeCount < 3) {
													CXLinker<CIndividualSaturationProcessNode*>* checkingNodeLinker = CObjectAllocator< CXLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
													checkingNodeLinker->initLinker(connectedSatNode);
													checkingData.mCheckingNodeLinker = checkingNodeLinker->append(checkingData.mCheckingNodeLinker);
												}
											}
										}

									}									
								}
							}
						}
					}




					CPROCESSSET<TRoleInversionPair>* combinedExistentialRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CSortedNegLinker<CRole*>* combinedExistentialRoleInstantiatedLinker = nullptr;

					collectSaturationNodeSuccessorRelatedRoles(satNode->getIndividualID(), satNode, &combinedExistentialRoleInstantiatedSet, &combinedExistentialRoleInstantiatedLinker, true, false, false, true, true, nullptr, existentialIndirectlyConnectedNominalIndividualSet, roleUsedCardHash, successorRelatedNodesAlternativeCheckingHash, calcAlgContext);

					if (combinedExistentialRoleInstantiatedLinker) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, combinedExistentialRoleInstantiatedLinker, combinedExistentialRoleInstantiatedSet, true, mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, labelRef);

						if (mConfOccurrenceStatisticsCounting) {
							CConcreteOntology* ontology = calcAlgContext->getProcessingDataBox()->getOntology();
							visitRolesOfAssociatedCompinationRoleSetLabel(labelRef, [&](CRole* role, bool inversed)->bool {
								if (!inversed) {
									mOccStatsCacheWriter->incRoleInstanceOccurrencceStatistics(ontology, role->getRoleTag(), 1, 0, 1, 0, 0, 0);
								} else if (role->getInverseRole()) {
									mOccStatsCacheWriter->incRoleInstanceOccurrencceStatistics(ontology, role->getInverseRole()->getRoleTag(), 1, 0, 0, 1, 0, 0);
								}
								return true;
							}, calcAlgContext);
						}

						return true;
					} else {
						return false;
					}

				}





				bool CSaturationNodeBackendAssociationCacheHandler::determineCombinedDataRoleInstantiatedSetLabelAssociationBackendItem(CIndividualSaturationProcessNode* satNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {

					CPROCESSSET<TRoleInversionPair>* combinedDataRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CSortedNegLinker<CRole*>* combinedDataRoleInstantiatedLinker = nullptr;

					// TODO: only do this, if there are data value concepts, i.e., set flag if data value concepts are used
					collectSaturationNodeSuccessorRelatedRoles(satNode->getIndividualID(), satNode, &combinedDataRoleInstantiatedSet, &combinedDataRoleInstantiatedLinker, false, false, true, true, true, nullptr, nullptr, nullptr, nullptr, calcAlgContext);

					CLinkedDataValueAssertionSaturationData* dataValueAssertionsData = satNode->getLinkedDataValueAssertionData(false);
					if (dataValueAssertionsData) {
						for (CXLinker<CRole*>* dataRoleIt = dataValueAssertionsData->getDataValueRoleAssertionLinker(); dataRoleIt; dataRoleIt = dataRoleIt->getNext()) {
							CRole* dataRole = dataRoleIt->getData();
							if (!combinedDataRoleInstantiatedSet->contains(TRoleInversionPair(dataRole, false))) {
								combinedDataRoleInstantiatedSet->insert(TRoleInversionPair(dataRole, false));

								CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
								combinedDataRoleInstantiatedLinker = newRoleLinker->init(dataRole, false, combinedDataRoleInstantiatedLinker);
							}
						}
					}


					if (combinedDataRoleInstantiatedLinker) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL, combinedDataRoleInstantiatedLinker, combinedDataRoleInstantiatedSet, true, mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL, labelRef);
						return true;
					} else {
						return false;
					}

				}













































				bool CSaturationNodeBackendAssociationCacheHandler::determineSaturationNodeIndividualNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(CIndividualSaturationProcessNode* satNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
				
					CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnection = mNominalIndiIdRoleConnectionDataHash->value(satNode->getNominalIndividual()->getIndividualID());

					CIndividualSaturationProcessNodeExtensionData* extensionData = satNode->getIndividualExtensionData(false);
					if (!extensionData && !nominalConnection) {
						return false;
					}

					CLinkedNeighbourRoleAssertionSaturationHash* linkedNeighbourRoleAssHash = extensionData->getLinkedNeighbourRoleAssertionHash(false);
					if (!linkedNeighbourRoleAssHash && !nominalConnection) {
						return false;
					}

					CPROCESSSET<CBackendRepresentativeMemoryCacheTemporaryLabelReference>* neigbourRoleInstantiatedSetCompination = CObjectParameterizingAllocator< CPROCESSSET<CBackendRepresentativeMemoryCacheTemporaryLabelReference>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >* mNominalNeighbourIndiIdRoleConnectionDataHash = nullptr;
					if (nominalConnection) {
						mNominalIndiIdRoleConnectionDataHash->remove(satNode->getNominalIndividual()->getIndividualID());
						mNominalNeighbourIndiIdRoleConnectionDataHash = CObjectParameterizingAllocator< CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

						for (CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnectionIt = nominalConnection; nominalConnectionIt; nominalConnectionIt = nominalConnectionIt->getNext()) {
							CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData*& neigbbourNominalConnectionDataLinker = (*mNominalNeighbourIndiIdRoleConnectionDataHash)[nominalConnectionIt->getConnectedIndividual().getIndividualID()];
							CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* newNeigbbourNominalConnectionDataLinker = CObjectAllocator<CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData>::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
							newNeigbbourNominalConnectionDataLinker->initNominalRoleConnectionData(nominalConnectionIt->getConnectionRole(), nominalConnectionIt->isInversedConnection(), nominalConnectionIt->getConnectedIndividual());
							neigbbourNominalConnectionDataLinker = newNeigbbourNominalConnectionDataLinker->append(neigbbourNominalConnectionDataLinker);
						}
					}


					CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateLinker = nullptr;

					CPROCESSHASH<cint64, CLinkedNeighbourRoleAssertionSaturationData*>* neighbourRoleAssertionDataHash = nullptr;
					if (linkedNeighbourRoleAssHash) {
						neighbourRoleAssertionDataHash = linkedNeighbourRoleAssHash->getNeighbourRoleAssertionDataHash();
					}
					if (neighbourRoleAssertionDataHash) {
						for (CPROCESSHASH<cint64, CLinkedNeighbourRoleAssertionSaturationData*>::const_iterator it = neighbourRoleAssertionDataHash->constBegin(), itEnd = neighbourRoleAssertionDataHash->constEnd(); it != itEnd; ++it) {
							cint64 neighbourIndiId = it.key();
							CLinkedNeighbourRoleAssertionSaturationData* data = it.value();

							CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalNeighbourConnection = nullptr;
							if (mNominalNeighbourIndiIdRoleConnectionDataHash) {
								nominalNeighbourConnection = mNominalNeighbourIndiIdRoleConnectionDataHash->value(neighbourIndiId);
								mNominalNeighbourIndiIdRoleConnectionDataHash->remove(neighbourIndiId);
							}

							CBackendRepresentativeMemoryCacheTemporaryLabelReference neighbourRoleSetLabelRef;
							CLinkedNeighbourRoleAssertionLinker* linkedRoleAssertedLinker = data->getLinkedRoleAssertedLinker();
							if (linkedRoleAssertedLinker && !linkedRoleAssertedLinker->hasNext() && !nominalNeighbourConnection) {
								CRole* role = linkedRoleAssertedLinker->getAssertedRole();
								bool inversed = linkedRoleAssertedLinker->isAssertedRoleInversed();
								neighbourRoleSetLabelRef = getNeighbourRoleInstantiatedSetLabelAssociationBackendItem(role, inversed, calcAlgContext);
								neigbourRoleInstantiatedSetCompination->insert(neighbourRoleSetLabelRef);
							} else {
								neighbourRoleSetLabelRef = getNeighbourRoleInstantiatedSetLabelAssociationBackendItem(linkedRoleAssertedLinker, nullptr, nominalNeighbourConnection, calcAlgContext);
								neigbourRoleInstantiatedSetCompination->insert(neighbourRoleSetLabelRef);
							}

							CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* newRoleSetNeighbourUpdateLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker >::allocateAndConstruct(mMemAllocMan);
							newRoleSetNeighbourUpdateLinker->initRoleSetNeighbourUpdateDataLinker(neighbourRoleSetLabelRef, neighbourIndiId);
							roleSetNeighbourUpdateLinker = newRoleSetNeighbourUpdateLinker->append(roleSetNeighbourUpdateLinker);
						}
					} 
					if (mNominalNeighbourIndiIdRoleConnectionDataHash && !mNominalNeighbourIndiIdRoleConnectionDataHash->isEmpty()) {
						for (CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >::const_iterator it = mNominalNeighbourIndiIdRoleConnectionDataHash->constBegin(), itEnd = mNominalNeighbourIndiIdRoleConnectionDataHash->constEnd(); it != itEnd; ++it) {
							cint64 neighbourIndiId = it.key();
							CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalNeighbourConnection = it.value();

							CBackendRepresentativeMemoryCacheTemporaryLabelReference neighbourRoleSetLabelRef = getNeighbourRoleInstantiatedSetLabelAssociationBackendItem(nullptr, nullptr, nominalNeighbourConnection, calcAlgContext);
							neigbourRoleInstantiatedSetCompination->insert(neighbourRoleSetLabelRef);

							CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* newRoleSetNeighbourUpdateLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker >::allocateAndConstruct(mMemAllocMan);
							newRoleSetNeighbourUpdateLinker->initRoleSetNeighbourUpdateDataLinker(neighbourRoleSetLabelRef, neighbourIndiId);
							roleSetNeighbourUpdateLinker = newRoleSetNeighbourUpdateLinker->append(roleSetNeighbourUpdateLinker);
						}
					}

					tmpAssWriteDataLinker->setRoleSetNeighbourUpdateDataLinker(roleSetNeighbourUpdateLinker);
					return determineNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(neigbourRoleInstantiatedSetCompination, tmpAssWriteDataLinker, calcAlgContext);
				}











				bool CSaturationNodeBackendAssociationCacheHandler::determineNominalIndividualIdNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(cint64 nominalId, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnection, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
					CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateLinker = nullptr;

					CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mAssBackCacheReader->getIndividualAssociationData(nominalId);
					CBackendRepresentativeMemoryLabelCacheItem* existingNeighbourRoleInstantiatedSetCompinationLabel = nullptr;
					if (indiAssData) {
						existingNeighbourRoleInstantiatedSetCompinationLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
					}
					CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* existingNeighbourRoleSetHash = nullptr;
					if (indiAssData) {
						existingNeighbourRoleSetHash = indiAssData->getNeighbourRoleSetHash();
					}


					bool updated = false;


					CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >* mNominalNeighbourIndiIdRoleConnectionDataHash = nullptr;
					if (nominalConnection) {
						mNominalNeighbourIndiIdRoleConnectionDataHash = CObjectParameterizingAllocator< CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

						for (CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnectionIt = nominalConnection; nominalConnectionIt; nominalConnectionIt = nominalConnectionIt->getNext()) {
							CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData*& neigbbourNominalConnectionDataLinker = (*mNominalNeighbourIndiIdRoleConnectionDataHash)[nominalConnectionIt->getConnectedIndividual().getIndividualID()];
							CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* newNeigbbourNominalConnectionDataLinker = CObjectAllocator<CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData>::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
							newNeigbbourNominalConnectionDataLinker->initNominalRoleConnectionData(nominalConnectionIt->getConnectionRole(), nominalConnectionIt->isInversedConnection(), nominalConnectionIt->getConnectedIndividual());
							neigbbourNominalConnectionDataLinker = newNeigbbourNominalConnectionDataLinker->append(neigbbourNominalConnectionDataLinker);
						}
					}


					CPROCESSSET<CBackendRepresentativeMemoryCacheTemporaryLabelReference>* neigbourRoleInstantiatedSetCompination = nullptr;
					for (CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >::const_iterator it = mNominalNeighbourIndiIdRoleConnectionDataHash->constBegin(), itEnd = mNominalNeighbourIndiIdRoleConnectionDataHash->constEnd(); it != itEnd; ++it) {
						cint64 neighbourIndiId = it.key();
						CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnectionData = it.value();
						CBackendRepresentativeMemoryLabelCacheItem* prevNeighbourRoleSetLabel = nullptr; 
						if (existingNeighbourRoleSetHash) {
							prevNeighbourRoleSetLabel = existingNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndiId);
						}


						bool updateRequired = false;
						if (!prevNeighbourRoleSetLabel) {
							updateRequired = true;
						} else {
							for (CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnectionIt = nominalConnectionData; !updateRequired && nominalConnectionIt; nominalConnectionIt = nominalConnectionIt->getNext()) {
								if (!mAssBackCacheReader->hasRoleInAssociatedNeigbourRoleSetLabel(indiAssData, prevNeighbourRoleSetLabel, nominalConnectionIt->getConnectionRole(), nominalConnectionIt->isInversedConnection(), false, true, false)) {
									updateRequired = true;
								}
							}
						}

						if (updateRequired) {
							CBackendRepresentativeMemoryCacheRoleAssertionLinker* linkedRoleAssertedLinker = nullptr;
							if (prevNeighbourRoleSetLabel) {
								mAssBackCacheReader->visitRolesOfAssociatedNeigbourRoleSetLabel(indiAssData, prevNeighbourRoleSetLabel, [&](CRole* role, bool inversed, bool aboxAsserted, bool nominalConnection, bool nondeterministic)->bool {
									CBackendRepresentativeMemoryCacheRoleAssertionLinker* newLinkedRoleAssertedLinker = CObjectAllocator<CBackendRepresentativeMemoryCacheRoleAssertionLinker>::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
									newLinkedRoleAssertedLinker->initRoleAssertionLinker(role, inversed, aboxAsserted, nominalConnection, nondeterministic);
									linkedRoleAssertedLinker = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)newLinkedRoleAssertedLinker->append(linkedRoleAssertedLinker);
									return true;
								});
							}

							CBackendRepresentativeMemoryCacheTemporaryLabelReference neighbourRoleSetLabelRef = getNeighbourRoleInstantiatedSetLabelAssociationBackendItem(nullptr, linkedRoleAssertedLinker, nominalConnectionData, calcAlgContext);
							CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* newRoleSetNeighbourUpdateLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker >::allocateAndConstruct(mMemAllocMan);
							newRoleSetNeighbourUpdateLinker->initRoleSetNeighbourUpdateDataLinker(neighbourRoleSetLabelRef, neighbourIndiId);
							roleSetNeighbourUpdateLinker = newRoleSetNeighbourUpdateLinker->append(roleSetNeighbourUpdateLinker);


							if (neighbourRoleSetLabelRef.getReferredTemporaryLabelData() || !checkNeighbourRoleInstantiatedSetCompinationLabelContainsNeighbourRoleInstantiatedSet(existingNeighbourRoleInstantiatedSetCompinationLabel, neighbourRoleSetLabelRef, calcAlgContext)) {
								if (!neigbourRoleInstantiatedSetCompination) {
									neigbourRoleInstantiatedSetCompination = CObjectParameterizingAllocator< CPROCESSSET<CBackendRepresentativeMemoryCacheTemporaryLabelReference>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
								}
								neigbourRoleInstantiatedSetCompination->insert(neighbourRoleSetLabelRef);
							}

							updated = true;
						}
					}
					tmpAssWriteDataLinker->setRoleSetNeighbourUpdateDataLinker(roleSetNeighbourUpdateLinker);

					if (neigbourRoleInstantiatedSetCompination) {
						if (indiAssData) {
							mAssBackCacheReader->visitLabelsOfAssociatedNeigbourRoleSetCombinationLabel(indiAssData, existingNeighbourRoleInstantiatedSetCompinationLabel, [&](CBackendRepresentativeMemoryLabelCacheItem* label)->bool {
								neigbourRoleInstantiatedSetCompination->insert(CBackendRepresentativeMemoryCacheTemporaryLabelReference(label));
								return true;
							});
						}
						updated |= determineNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(neigbourRoleInstantiatedSetCompination, tmpAssWriteDataLinker, calcAlgContext);
					}

					return updated;
				}










				bool CSaturationNodeBackendAssociationCacheHandler::determineNominalIndividualIdCombinedNeighbourRoleInstantiatedSetLabelAssociationBackendItem(cint64 nominalId, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnDataLinker, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {

					bool updateRequired = false;
					bool updated = false;

					CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mAssBackCacheReader->getIndividualAssociationData(nominalId);
					CBackendRepresentativeMemoryLabelCacheItem* existingDeterministicCombinedNeighbourRoleInstantiatedSetLabel = nullptr;
					if (indiAssData) {
						existingDeterministicCombinedNeighbourRoleInstantiatedSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL);
					}

					for (CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnData = nominalConnDataLinker; !updateRequired && nominalConnData; nominalConnData = nominalConnData->getNext()) {
						if (!mAssBackCacheReader->hasRoleInAssociatedNeigbourRoleSetLabel(indiAssData, existingDeterministicCombinedNeighbourRoleInstantiatedSetLabel, nominalConnData->getConnectionRole(), nominalConnData->isInversedConnection(), false, true, false)) {
							updateRequired = true;
						}
					}


					if (updateRequired) {

						CPROCESSSET<TRoleInversionPair>* combinedNeigbourRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
						CSortedNegLinker<CRole*>* combinedNeigbourRoleInstantiatedLinker = nullptr;


						if (existingDeterministicCombinedNeighbourRoleInstantiatedSetLabel) {
							mAssBackCacheReader->visitRolesOfAssociatedNeigbourRoleSetLabel(indiAssData, existingDeterministicCombinedNeighbourRoleInstantiatedSetLabel, [&](CRole* role, bool inversed, bool aboxAsserted, bool nominalConnection, bool nondeterministic)->bool {
								if (!combinedNeigbourRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
									CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
									combinedNeigbourRoleInstantiatedLinker = roleLinker->init(role, inversed, combinedNeigbourRoleInstantiatedLinker);
									combinedNeigbourRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
								}
								return true;
							});
						}

						for (CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnData = nominalConnDataLinker; nominalConnData; nominalConnData = nominalConnData->getNext()) {
							CRole* role = nominalConnData->getConnectionRole();
							bool inversed = nominalConnData->isInversedConnection();

							if (!combinedNeigbourRoleInstantiatedSet->contains(TRoleInversionPair(role, inversed))) {
								CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
								combinedNeigbourRoleInstantiatedLinker = roleLinker->init(role, inversed, combinedNeigbourRoleInstantiatedLinker);
								combinedNeigbourRoleInstantiatedSet->insert(TRoleInversionPair(role, inversed));
							}
						}

						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, combinedNeigbourRoleInstantiatedLinker, combinedNeigbourRoleInstantiatedSet, true, mRoleInversionSetTmpRefCombinedNeighbourRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, labelRef);

						if (mConfOccurrenceStatisticsCounting) {
							CConcreteOntology* ontology = calcAlgContext->getProcessingDataBox()->getOntology();
							visitRolesOfAssociatedCompinationRoleSetLabel(labelRef, [&](CRole* role, bool inversed)->bool {
								if (!inversed) {
									mOccStatsCacheWriter->incRoleInstanceOccurrencceStatistics(ontology, role->getRoleTag(), 1, 0, 1, 0, 0, 0);
								}
								return true;
							}, calcAlgContext);
						}


						updated = true;
					}
					return updated;

				}








































				bool CSaturationNodeBackendAssociationCacheHandler::collectNominalRelatedRoles(cint64 satIndiId, cint64 nominalId, CIndividualSaturationProcessNode* satNode, CPROCESSSET<TRoleInversionPair>** roleSet, CSortedNegLinker<CRole*>** roleLinker,
						CRole* role, CCalculationAlgorithmContext* calcAlgContext) {

					bool collected = false;
					// collect inversed since we are interested in the roles from the perspective of the nominal
					if (roleSet && !(*roleSet)->contains(TRoleInversionPair(role, true))) {
						(*roleSet)->insert(TRoleInversionPair(role, true));
						collected = true;

						CSortedNegLinker<CRole*>* newRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(calcAlgContext->getUsedTemporaryMemoryAllocationManager());
						*roleLinker = newRoleLinker->init(role, true, *roleLinker);
					}

					return collected;
				}



				bool CSaturationNodeBackendAssociationCacheHandler::collectNominalRelatedRoles(cint64 satIndiId, cint64 nominalId, CIndividualSaturationProcessNode* satNode, CPROCESSSET<TRoleInversionPair>** roleSet, CSortedNegLinker<CRole*>** roleLinker, CCalculationAlgorithmContext* calcAlgContext) {

					CReapplyConceptSaturationLabelSet* conSet = satNode->getReapplyConceptSaturationLabelSet(false);
					if (!conSet) {
						return false;
					}

					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = satNode->getLinkedRoleSuccessorHash(false);

					CIndividualSaturationProcessNode* copiedNode = satNode->getCopyIndividualNode();
					if (copiedNode && copiedNode->getReapplyConceptSaturationLabelSet(false) && copiedNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker() == conSet->getConceptSaturationDescriptionLinker() && (!linkedSuccHash || copiedNode->getLinkedRoleSuccessorHash(false))) {
						return collectNominalRelatedRoles(satIndiId, nominalId, copiedNode, roleSet, roleLinker, calcAlgContext);
					}

					bool collected = false;

					if (linkedSuccHash) {
						CPROCESSHASH<CRole*, CLinkedRoleSaturationSuccessorData*>* linkSuccDatahash = linkedSuccHash->getLinkedRoleSuccessorHash();
						for (CPROCESSHASH<CRole*, CLinkedRoleSaturationSuccessorData*>::const_iterator it = linkSuccDatahash->constBegin(), itEnd = linkSuccDatahash->constEnd(); it != itEnd; ++it) {
							CRole* role = it.key();
							CLinkedRoleSaturationSuccessorData* linkSuccData = it.value();
							if (linkSuccData->mSuccCount > 0) {
								CPROCESSMAP<cint64, CSaturationSuccessorData*>* succNodeMap = linkSuccData->getSuccessorNodeDataMap(false);
								for (CPROCESSMAP<cint64, CSaturationSuccessorData*>::const_iterator succIt = succNodeMap->constBegin(), succItEnd = succNodeMap->constEnd(); succIt != succItEnd; ++succIt) {
									CSaturationSuccessorData* succData = succIt.value();
									if (succData->mActiveCount > 0) {
										CIndividualSaturationProcessNode* succSatNode = succData->mSuccIndiNode;
										bool collectSuccessorNode = false;
										if (succData->mVALUENominalConnection && succData->mVALUENominalID == nominalId) {
											collectSuccessorNode = true;
										}

										if (succSatNode && succSatNode->getIntegratedNominalIndividual() && succSatNode->getIntegratedNominalIndividual()->getIndividualID() == nominalId) {
											collectSuccessorNode = true;
										}
										if (collectSuccessorNode) {
											collected |= collectNominalRelatedRoles(satIndiId, nominalId, satNode, roleSet, roleLinker, role, calcAlgContext);
										}
									}
								}
							}
						}
					} else {
						for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
							CConcept* concept = conSatDesIt->getConcept();
							bool conNegation = conSatDesIt->isNegated();
							cint64 conCode = concept->getOperatorCode();
							CRole* role = concept->getRole();
							bool collectSuccessorNode = false;
							bool existentialSuccessorCand = false;
							if (!conNegation && (conCode == CCSOME || conCode == CCAQSOME) || conNegation && (conCode == CCALL)) {
								existentialSuccessorCand = true;
							}
							if (!conNegation && conCode == CCATLEAST || conNegation && conCode == CCATMOST) {
								existentialSuccessorCand = true;
							}
							if (!conNegation && conCode == CCVALUE) {
								collectSuccessorNode = true;
							}
							if (role && existentialSuccessorCand) {
								CIndividualSaturationProcessNode* succSatNode = getSuccessorSaturationNode(conSatDesIt, satNode, calcAlgContext);
								if (succSatNode && succSatNode->getIntegratedNominalIndividual() && succSatNode->getIntegratedNominalIndividual()->getIndividualID() == nominalId) {
									collectSuccessorNode = true;
								}
							}
							if (collectSuccessorNode) {
								collected |= collectNominalRelatedRoles(satIndiId, nominalId, satNode, roleSet, roleLinker, role, calcAlgContext);
							}
						}
					}

					return collected;
				}



				bool CSaturationNodeBackendAssociationCacheHandler::collectNominalRelatedRolesFromPredecessors(cint64 satIndiId, cint64 nominalId, CIndividualSaturationProcessNode* satIndiNode, CPROCESSSET<TRoleInversionPair>** roleSet, CSortedNegLinker<CRole*>** roleLinker, CCalculationAlgorithmContext* calcAlgContext) {

					bool collected = false;
					CPROCESSSET< CIndividualSaturationProcessNode* >* handledSatNodeSet = CObjectParameterizingAllocator< CPROCESSSET< CIndividualSaturationProcessNode* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CRoleBackwardSaturationPropagationHash* backwardPropHash = satIndiNode->getRoleBackwardPropagationHash(false);
					if (backwardPropHash) {
						CPROCESSHASH<CRole*, CRoleBackwardSaturationPropagationHashData>* backPropHash = backwardPropHash->getRoleBackwardPropagationDataHash();
						for (CPROCESSHASH<CRole*, CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropHash->constBegin(), itEnd = backPropHash->constEnd(); it != itEnd; ++it) {
							const CRoleBackwardSaturationPropagationHashData& backwardPropData = it.value();
							for (CBackwardSaturationPropagationLink* linkIt = backwardPropData.mLinkLinker; linkIt; linkIt = linkIt->getNext()) {
								CIndividualSaturationProcessNode* sourceIndividual = linkIt->getSourceIndividual();
								if (!handledSatNodeSet->contains(sourceIndividual)) {
									handledSatNodeSet->insert(sourceIndividual);
									collected |= collectNominalRelatedRoles(satIndiId, nominalId, sourceIndividual, roleSet, roleLinker, calcAlgContext);
								}
							}
						}
					}

					for (CXLinker<CIndividualSaturationProcessNode*>* nonInvConnIndiLinkerIt = satIndiNode->getNonInverseConnectedIndividualNodeLinker(); nonInvConnIndiLinkerIt; nonInvConnIndiLinkerIt = nonInvConnIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* sourceIndividual = nonInvConnIndiLinkerIt->getData();
						if (!handledSatNodeSet->contains(sourceIndividual)) {
							handledSatNodeSet->insert(sourceIndividual);
							collected |= collectNominalRelatedRoles(satIndiId, nominalId, sourceIndividual, roleSet, roleLinker, calcAlgContext);
						}
					}
					return collected;
				}




				bool CSaturationNodeBackendAssociationCacheHandler::determineNominalIndividualIdPossiblyExistentialRoleInstantiatedSetLabelAssociationBackendItem(cint64 nominalId, CSaturationNominalDependentNodeData* nomDepNodeData, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
					bool updated = false;

					CPROCESSSET<TRoleInversionPair>* combinedNeigbourRoleInstantiatedSet = CObjectParameterizingAllocator< CPROCESSSET<TRoleInversionPair>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CSortedNegLinker<CRole*>* combinedNeigbourRoleInstantiatedLinker = nullptr;

					for (CSaturationNominalDependentNodeData* nominalDepData = nomDepNodeData; nominalDepData; nominalDepData = nominalDepData->getNext()) {

						CIndividualSaturationProcessNode* depNode = nominalDepData->getDependentIndividualSaturationNode();
						if (nominalDepData->getNominalConnectionType() == CSaturationNominalDependentNodeData::VALUECONNECTION) {
							collectNominalRelatedRoles(depNode->getIndividualID(), nominalId, depNode, &combinedNeigbourRoleInstantiatedSet, &combinedNeigbourRoleInstantiatedLinker, calcAlgContext);
						} else if (nominalDepData->getNominalConnectionType() == CSaturationNominalDependentNodeData::NOMINALCONNECTION) {
							collectNominalRelatedRolesFromPredecessors(depNode->getIndividualID(), nominalId, depNode, &combinedNeigbourRoleInstantiatedSet, &combinedNeigbourRoleInstantiatedLinker, calcAlgContext);
						}
					}

					if (combinedNeigbourRoleInstantiatedLinker) {
						CBackendRepresentativeMemoryCacheTemporaryLabelReference labelRef = getRoleInstantiatedSetLabelAssociationBackendItem(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, combinedNeigbourRoleInstantiatedLinker, combinedNeigbourRoleInstantiatedSet, true, mRoleInversionSetTmpRefCombinedNeighbourRoleInstantiatedSetLabelHash, calcAlgContext);
						tmpAssWriteDataLinker->setReferredLabel(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, labelRef);
						updated = true;

						if (mConfOccurrenceStatisticsCounting) {
							CConcreteOntology* ontology = calcAlgContext->getProcessingDataBox()->getOntology();
							visitRolesOfAssociatedCompinationRoleSetLabel(labelRef, [&](CRole* role, bool inversed)->bool {
								if (!inversed) {
									mOccStatsCacheWriter->incRoleInstanceOccurrencceStatistics(ontology, role->getRoleTag(), 0, 1, 1, 0, 0, 0);
								} else if (role->getInverseRole()) {
									mOccStatsCacheWriter->incRoleInstanceOccurrencceStatistics(ontology, role->getInverseRole()->getRoleTag(), 0, 1, 0, 1, 0, 0);
								}
								return true;
							}, calcAlgContext);
						}

					}
					return updated;

				}




































				bool CSaturationNodeBackendAssociationCacheHandler::tryAssociateNodesWithBackendCache(CIndividualSaturationProcessNodeLinker* indiSaturationAnalysingNodeLinker, CSatisfiableTaskRepresentativeBackendUpdatingAdapter* repbackUpdAdapter, CCalculationAlgorithmContext* calcAlgContext) {
					calcAlgContext->mUsedTempMemMan = nullptr;

					mIndiNodeRefDetConceptSetLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< CIndividualSaturationProcessNode*,QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*> >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());
					mSignatureTmpRefDetConceptSetLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());

					mTmpConceptLabelTmpCardDataHash = CObjectParameterizingAllocator< CPROCESSHASH< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*, CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());

					mSignatureTmpRefNeigbourRoleInstantiatedSetCombinationLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					mRoleInversionTmpRefNeighbourInstantiatedRoleSetLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< TRoleInversionPair, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					mRoleInversionSetTmpRefNeighbourInstantiatedRoleSetLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					mSuperRoleSignatureCacheValueTmpRefNeighbourInstantiatedRoleSetLabelPairHash = CObjectParameterizingAllocator< CPROCESSHASH< cint64, CCacheValueTmpLabelReferenceData* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					
					mRoleInversionSetTmpRefCombinedExistentialRoleInstantiatedSetLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					mRoleInversionSetTmpRefCombinedNeighbourRoleInstantiatedSetLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< CProcessSetHasher<TRoleInversionPair>, CBackendRepresentativeMemoryCacheTemporaryLabelReference* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					
					mNominalIndiIdRoleConnectionDataHash = CObjectParameterizingAllocator< CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					mNewTmpLabelList = CObjectParameterizingAllocator< CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					mNewTmpCardList = CObjectParameterizingAllocator< CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					
					mSignatureTmpIndiConnIndiSetRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
					CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* >* nominalIndirectConnectionUpdateDataLinkerHash = CObjectParameterizingAllocator< CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());

					prepareCacheMessages(calcAlgContext);

					CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker = nullptr;
					CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* lastTmpAssWriteDataLinker = nullptr;


					CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tmpAssUseDataLinker = nullptr;
					CIndividualRepresentativeBackendCacheLoadedAssociationHash* indiLoadedAssocHash = calcAlgContext->getUsedProcessingDataBox()->getBackendCacheLoadedAssociationHash(false);

					CSaturationNominalDependentNodeHash* nominalDependentNodeHash = calcAlgContext->getUsedProcessingDataBox()->getSaturationNominalDependentNodeHash(false);

					for (CIndividualSaturationProcessNodeLinker* indiSaturationAnalysingNodeLinkerIt = indiSaturationAnalysingNodeLinker; indiSaturationAnalysingNodeLinkerIt; indiSaturationAnalysingNodeLinkerIt = indiSaturationAnalysingNodeLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* satNode = indiSaturationAnalysingNodeLinkerIt->getProcessingIndividual();
						CIndividual* nominalIndividual = satNode->getNominalIndividual();
						 
						if (nominalIndividual) {
							
							CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* newAssWriteDataLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
							if (!nominalIndividual->isTemporaryIndividual()) {
								newAssWriteDataLinker->initAccociationWriteData(nominalIndividual, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION);
							} else {
								newAssWriteDataLinker->initAccociationWriteData(nominalIndividual->getIndividualID(), CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION);
							}

							if (mConfOccurrenceStatisticsCounting) {
								collectConceptOccurrenceStatistics(satNode, calcAlgContext);
							}
							determineFullConceptSetLabelAssociationBackendItem(satNode, CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL, newAssWriteDataLinker,calcAlgContext);

							CPROCESSSET<cint64>* existentialIndirectlyConnectedNominalIndividualSet = nullptr;

							CPROCESSHASH<CRole*, cint64>* roleUsedCardHash = nullptr;
							if (satNode->getReapplyConceptSaturationLabelSet(false)->getConceptFlags()->containsATMOSTCardinalityFlags()) {
								roleUsedCardHash = CObjectParameterizingAllocator< CPROCESSHASH<CRole*, cint64>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
							}

							determineCombinedExistentialRoleInstantiatedSetLabelAssociationBackendItem(satNode, &existentialIndirectlyConnectedNominalIndividualSet, roleUsedCardHash, newAssWriteDataLinker, calcAlgContext);
							determineCombinedDataRoleInstantiatedSetLabelAssociationBackendItem(satNode, newAssWriteDataLinker, calcAlgContext);

							if (roleUsedCardHash) {
								determineCardinalityAssociationBackendItem(satNode, roleUsedCardHash, newAssWriteDataLinker, calcAlgContext);
							}

							if (existentialIndirectlyConnectedNominalIndividualSet) {
								determineIndirectlyConnectedNominalIndividualSetLabelAssociationBackendItem(existentialIndirectlyConnectedNominalIndividualSet, newAssWriteDataLinker, calcAlgContext);

								for (CPROCESSSET<cint64>::const_iterator it = existentialIndirectlyConnectedNominalIndividualSet->constBegin(), itEnd = existentialIndirectlyConnectedNominalIndividualSet->constEnd(); it != itEnd; ++it) {
									cint64 nomIndiId = *it;

									//TODO: only add linker if the individual was not already connected to the nominal (based on the previous indirectly connected nominal individual set label)
									CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker*& nomIndiConnDataLinker = (*nominalIndirectConnectionUpdateDataLinkerHash)[nomIndiId];
									if (!nomIndiConnDataLinker) {
										nomIndiConnDataLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker >::allocateAndConstruct(mMemAllocMan);
										nomIndiConnDataLinker->initNominalIndirectConnectionData(nomIndiId);
										if (indiLoadedAssocHash && indiLoadedAssocHash->contains(satNode->getIndividualID())) {
											CIndividualRepresentativeBackendCacheLoadedAssociationData& assoLoadingData = (*indiLoadedAssocHash)[satNode->getIndividualID()];
											if (assoLoadingData.getLoadedIndividualAssociationData()) {
												nomIndiConnDataLinker->setLastChangeIntegrationId(assoLoadingData.getLoadedIndividualAssociationData()->getLastIntegratedIndirectlyConnectedIndividualsChangeId());
											}
										}
									}
									CXLinker<cint64>* indiIdLinker = CObjectAllocator< CXLinker<cint64> >::allocateAndConstruct(mMemAllocMan);
									indiIdLinker->initLinker(nominalIndividual->getIndividualID());
									nomIndiConnDataLinker->addIndirectlyConnectedIndividualIdLinker(indiIdLinker);
								}
							}

							if (nominalDependentNodeHash && nominalDependentNodeHash->getNominalDependentNodeData(nominalIndividual->getIndividualID())) {
								CSaturationNominalDependentNodeData* nomDepNodeData = nominalDependentNodeHash->getNominalDependentNodeData(nominalIndividual->getIndividualID());
								if (nomDepNodeData) {
									// identify connections and save them as possibly/non-deterministic (existentially) instantiated roles
									newAssWriteDataLinker->setIndirectlyConnectedNominalIndividual(true);
									determineNominalIndividualIdPossiblyExistentialRoleInstantiatedSetLabelAssociationBackendItem(nominalIndividual->getIndividualID(), nomDepNodeData, newAssWriteDataLinker, calcAlgContext);
								}
							}



							collectSaturationNodeSuccessorRelatedRoles(satNode->getIndividualID(), satNode, nullptr, nullptr, false, true, false, true, true, mNominalIndiIdRoleConnectionDataHash, nullptr, nullptr, nullptr, calcAlgContext);

							CIndividualSaturationProcessNodeStatusFlags* indFlags = satNode->getIndirectStatusFlags();
							CIndividualSaturationProcessNodeStatusFlags* dirFlags = satNode->getDirectStatusFlags();
							bool insufficient = indFlags->hasInsufficientFlag() || !indFlags->hasCompletedFlag() || !dirFlags->hasCompletedFlag();
							if (insufficient) {
								newAssWriteDataLinker->setCompletelySaturated(false);
								newAssWriteDataLinker->setCompletelyHandled(false);
							}

							if (indiLoadedAssocHash && indiLoadedAssocHash->contains(satNode->getIndividualID())) {
								CIndividualRepresentativeBackendCacheLoadedAssociationData& assoLoadingData = (*indiLoadedAssocHash)[satNode->getIndividualID()];
								assoLoadingData.setIndividualIntegrated(true);
							}

							if (!lastTmpAssWriteDataLinker) {
								tmpAssWriteDataLinker = newAssWriteDataLinker;
								lastTmpAssWriteDataLinker = tmpAssWriteDataLinker;
							} else {
								lastTmpAssWriteDataLinker->setNext(newAssWriteDataLinker);
								lastTmpAssWriteDataLinker = newAssWriteDataLinker;
							}
						}
					}

					CIndividualSaturationProcessNodeLinker* nextIndiSaturationAnalysingNodeLinkerIt = indiSaturationAnalysingNodeLinker;
					for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* newAssWriteDataLinker = tmpAssWriteDataLinker; newAssWriteDataLinker; newAssWriteDataLinker = newAssWriteDataLinker->getNext()) {

						CIndividual* nominalIndividual = nullptr;
						CIndividualSaturationProcessNode* satNode = nullptr;
						while (!nominalIndividual) {
							satNode = nextIndiSaturationAnalysingNodeLinkerIt->getProcessingIndividual();
							nominalIndividual = satNode->getNominalIndividual();
							nextIndiSaturationAnalysingNodeLinkerIt = nextIndiSaturationAnalysingNodeLinkerIt->getNext();
						}

						determineSaturationNodeIndividualCombinedNeighbourRoleInstantiatedSetLabelAssociationBackendItem(satNode, newAssWriteDataLinker, calcAlgContext);
						determineSaturationNodeIndividualNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(satNode, newAssWriteDataLinker, calcAlgContext);
					}

					if (mNominalIndiIdRoleConnectionDataHash && !mNominalIndiIdRoleConnectionDataHash->isEmpty()) {
						for (CPROCESSHASH< cint64, CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* >::const_iterator it = mNominalIndiIdRoleConnectionDataHash->constBegin(), itEnd = mNominalIndiIdRoleConnectionDataHash->constEnd(); it != itEnd; ++it) {
							cint64 indiId = it.key();
							CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* nominalConnectionData = it.value();

							CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* newAssWriteDataLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
							newAssWriteDataLinker->initAccociationWriteData(indiId, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION);

							determineNominalIndividualIdCombinedNeighbourRoleInstantiatedSetLabelAssociationBackendItem(indiId, nominalConnectionData, newAssWriteDataLinker, calcAlgContext);
							determineNominalIndividualIdNeighbourRoleInstantiatedSetCompinationLabelAssociationBackendItem(indiId, nominalConnectionData, newAssWriteDataLinker, calcAlgContext);


							if (indiLoadedAssocHash && indiLoadedAssocHash->contains(indiId)) {
								CIndividualRepresentativeBackendCacheLoadedAssociationData& assoLoadingData = (*indiLoadedAssocHash)[indiId];
								assoLoadingData.setIndividualIntegrated(true);
							}


							if (!lastTmpAssWriteDataLinker) {
								tmpAssWriteDataLinker = newAssWriteDataLinker;
								lastTmpAssWriteDataLinker = tmpAssWriteDataLinker;
							} else {
								lastTmpAssWriteDataLinker->setNext(newAssWriteDataLinker);
								lastTmpAssWriteDataLinker = newAssWriteDataLinker;
							}
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
							if (!indiAssoLoadingData.isIndividualIntegrated()) {

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
