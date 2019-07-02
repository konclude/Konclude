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

#include "CBackendRepresentativeMemoryCacheReader.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheReader::CBackendRepresentativeMemoryCacheReader() : CLinkerBase<CBackendRepresentativeMemoryCacheReader*,CBackendRepresentativeMemoryCacheReader>(this) {
					mCurrentSlot = nullptr;
					mFixedOntologyData = nullptr;
					mOntologyData = nullptr;
				}


				CBackendRepresentativeMemoryCacheReader* CBackendRepresentativeMemoryCacheReader::updateSlot(CBackendRepresentativeMemoryCacheSlotItem* updatedSlot) {
					CBackendRepresentativeMemoryCacheSlotItem* prevSlot = mUpdatedSlot.fetchAndStoreOrdered(updatedSlot);
					if (prevSlot != nullptr) {
						prevSlot->decReader();
					}
					return this;
				}


				CBackendRepresentativeMemoryCacheReader* CBackendRepresentativeMemoryCacheReader::fixOntologyData(CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					mFixedOntologyData = ontologyData;
					mOntologyData = ontologyData;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheReader::hasUpdatedSlotItem() {
                    return mUpdatedSlot.fetchAndAddRelaxed(0) != nullptr;
				}


				bool CBackendRepresentativeMemoryCacheReader::switchToUpdatedSlotItem() {
					CBackendRepresentativeMemoryCacheSlotItem* updatedSlot = mUpdatedSlot.fetchAndStoreOrdered(nullptr);
					if (updatedSlot) {
						CBackendRepresentativeMemoryCacheSlotItem* prevSlot = mCurrentSlot;
						mCurrentSlot = updatedSlot;
						if (prevSlot != nullptr) {
							prevSlot->decReader();
						}
						mOntologyData = nullptr;
						return true;
					}
					return false;
				}



				CBackendRepresentativeMemoryCacheReader* CBackendRepresentativeMemoryCacheReader::setWorkingOntology(cint64 ontologyIdentifier) {
					mOntologyIdentifier = ontologyIdentifier;
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					mOntologyData = nullptr;
					if (mFixedOntologyData) {
						mOntologyData = mFixedOntologyData;
					}
					return this;
				}

				CBackendRepresentativeMemoryCacheReader* CBackendRepresentativeMemoryCacheReader::setWorkingOntology(CConcreteOntology* ontology) {
					return setWorkingOntology(ontology->getOntologyID());
				}



				bool CBackendRepresentativeMemoryCacheReader::hasSameIndividualsMergings() {
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						return mCurrentSlot->hasSameIndividualsMergings();
					}
					return false;
				}




				bool CBackendRepresentativeMemoryCacheReader::hasCacheEntry(cint64 labelType, cint64 signature) {
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigItemHash = mCurrentSlot->getSignatureLabelItemHash(labelType);
						if (sigItemHash) {
							return sigItemHash->contains(signature);
						}
					}
					return false;
				}



				bool CBackendRepresentativeMemoryCacheReader::visitLabelCacheEntries(cint64 labelType, function<bool(CBackendRepresentativeMemoryLabelCacheItem* labelItem)> visitFunc) {
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					bool continueVisiting = true;
					bool visited = false;
					if (mCurrentSlot) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigItemHash = mCurrentSlot->getSignatureLabelItemHash(labelType);
						if (sigItemHash) {
							for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>::const_iterator sigHashIt = sigItemHash->constBegin(), sigHashItEnd = sigItemHash->constEnd(); sigHashIt != sigHashItEnd && continueVisiting; ++sigHashIt) {
								CBackendRepresentativeMemoryLabelSignatureResolveCacheItem sigResolveCacheItem = sigHashIt.value();
								for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResolveCacheItem.getLabelItems(); itemIt && continueVisiting; itemIt = itemIt->getNext()) {
									continueVisiting = visitFunc(itemIt);
									visited = true;
								}
							}
						}
					}
					return visited;
				}





				CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* CBackendRepresentativeMemoryCacheReader::getLabelCacheEntry(cint64 labelType, cint64 signature) {
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* entry = &mEmptySigResCacheItem;
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigItemHash = mCurrentSlot->getSignatureLabelItemHash(labelType);
						if (sigItemHash && sigItemHash->contains(signature)) {
							CBackendRepresentativeMemoryLabelSignatureResolveCacheItem& item = (*sigItemHash)[signature];
							entry = &item;
						}
					}
					return entry;
				}



				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheReader::getIndividualAssociationData(CIndividual* individual) {
					CBackendRepresentativeMemoryCacheIndividualAssociationData* assData = nullptr;
					if (!assData) {
						assData = getIndividualAssociationData(individual->getIndividualID());
					}
					return assData;
				}


				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheReader::getIndividualAssociationData(cint64 indiId) {

					CBackendRepresentativeMemoryCacheIndividualAssociationData* assData = nullptr;
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						cint64 indiIdAssoDataVectorSize = mCurrentSlot->getIndividualIdAssoiationDataVectorSize();
						CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector = mCurrentSlot->getIndividualIdAssoiationDataVector();
						if (indiIdAssoDataVector && indiId < indiIdAssoDataVectorSize) {
							assData = indiIdAssoDataVector[indiId];
						}
						//CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* indiIdAssoDataHash = mCurrentSlot->getIndividualIdAssociationDataHash();
						//if (indiIdAssoDataHash) {
						//	assData = indiIdAssoDataHash->value(indiId, nullptr);
						//}

						//TODO: create copy of data
					}

					return assData;
				}



				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getIndividualAssociatedCacheLabelItem(cint64 indiId, cint64 labelType) {
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CBackendRepresentativeMemoryCacheIndividualAssociationData* assData = nullptr;
						cint64 indiIdAssoDataVectorSize = mCurrentSlot->getIndividualIdAssoiationDataVectorSize();
						CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector = mCurrentSlot->getIndividualIdAssoiationDataVector();
						if (indiIdAssoDataVector && indiId < indiIdAssoDataVectorSize) {
							assData = indiIdAssoDataVector[indiId];
						}
						if (assData) {
							return assData->getLabelCacheEntry(labelType);
						}
					}

					return nullptr;
				}



				CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* CBackendRepresentativeMemoryCacheReader::getNominalIndirectConnectionData(cint64 indiId) {

					CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* nomConnData = nullptr;
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* nominaIIndividualdIndirectConnectionDataHash = mCurrentSlot->getNominaIIndividualdIndirectConnectionDataHash();
						if (nominaIIndividualdIndirectConnectionDataHash) {
							nomConnData = nominaIIndividualdIndirectConnectionDataHash->value(indiId);
						}

						//TODO: create copy of data
					}

					return nomConnData;
				}


				const CCacheValue CBackendRepresentativeMemoryCacheReader::getCacheValue(cint64 indiId, bool negation) {
					CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHE_VALUE_INDIVIDUAL_ID;
					if (negation) {
						indiId = -indiId;
						cacheValueIdentifier = CCacheValue::CACHE_VALUE_NEGATED_INDIVIDUAL_ID;
					}
					CCacheValue cacheValue;
					cacheValue.initCacheValue(indiId, 0, cacheValueIdentifier);
					return cacheValue;
				}


				const CCacheValue CBackendRepresentativeMemoryCacheReader::getCacheValue(CConcept* concept, bool negation, bool deterministic) {
					cint64 conTag = concept->getConceptTag();
					CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDCONCEPT;
					if (negation) {
						conTag = -conTag;
						if (deterministic) {
							cacheValueIdentifier = CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
						} else {
							cacheValueIdentifier = CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_NEGATED_CONCEPT;
						}
					} else if (!deterministic) {
						cacheValueIdentifier = CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_CONCEPT;
					}
					CCacheValue cacheValue;
					cacheValue.initCacheValue(conTag, (cint64)concept, cacheValueIdentifier);
					return cacheValue;
				}

				const CCacheValue CBackendRepresentativeMemoryCacheReader::getCacheValue(CRole* role) {
					cint64 roleTag = role->getRoleTag();
					CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDROLE;
					CCacheValue cacheValue;
					cacheValue.initCacheValue(roleTag,(cint64)role,cacheValueIdentifier);
					return cacheValue;
				}

				const CCacheValue CBackendRepresentativeMemoryCacheReader::getCacheValue(CRole* role, bool inversed, bool assertionLinkBase, bool nominalConnected, bool nondeterministc) {
					cint64 roleTag = role->getRoleTag();
					CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDROLE;
					if (inversed) {
						roleTag = -roleTag;
						if (assertionLinkBase) {
							if (!nondeterministc) {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGANDINVERSEDASSERTEDROLE;
							} else {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDASSERTEDROLE;
							}
						} else if (nominalConnected) {
							if (!nondeterministc) {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGANDINVERSED_NOMINAL_CONNECTED_ROLE;
							} else {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSED_NOMINAL_CONNECTED_ROLE;
							}
						} else {
							if (!nondeterministc) {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGANDINVERSEDROLE;
							} else {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDROLE;
							}
						}
					} else {
						if (assertionLinkBase) {
							if (!nondeterministc) {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGANDASSERTEDROLE;
							} else {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ASSERTEDROLE;
							}
						} else if (nominalConnected) {
							if (!nondeterministc) {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGAND_NOMINAL_CONNECTED_ROLE;
							} else {
								cacheValueIdentifier = CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_NOMINAL_CONNECTED_ROLE;
							}
						} else if (nondeterministc) {
							cacheValueIdentifier = CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ROLE;
						}
					}
					CCacheValue cacheValue;
					cacheValue.initCacheValue(roleTag, (cint64)role, cacheValueIdentifier);
					return cacheValue;
				}






				bool CBackendRepresentativeMemoryCacheReader::visitLabelsOfAssociatedNeigbourRoleSetCombinationLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCompinationLabel, function<bool(CBackendRepresentativeMemoryLabelCacheItem* label)> visitFunc) {
					if (neighbourRoleSetCompinationLabel) {
						CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinker = neighbourRoleSetCompinationLabel->getCacheValueLinker();
						if (lablelValueLinker) {
							bool continueVisiting = true;
							for (CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinkerIt = lablelValueLinker; lablelValueLinkerIt && continueVisiting; lablelValueLinkerIt = lablelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = lablelValueLinkerIt->getCacheValue();

								CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSet = (CBackendRepresentativeMemoryLabelCacheItem*)cacheValue.getIdentification();
								continueVisiting = visitFunc(neighbourRoleSet);
							}
							return true;
						}
					}
					return false;
				}



				bool CBackendRepresentativeMemoryCacheReader::visitRolesOfAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, function<bool(CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)> visitFunc) {
					if (neighbourRoleSetLabel) {
						CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinker = neighbourRoleSetLabel->getCacheValueLinker();

						if (lablelValueLinker) {
							bool continueVisiting = true;
							for (CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinkerIt = lablelValueLinker; lablelValueLinkerIt && continueVisiting; lablelValueLinkerIt = lablelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = lablelValueLinkerIt->getCacheValue();

								CRole* role = (CRole*)cacheValue.getIdentification();

								CCacheValue::CACHEVALUEIDENTIFIER identifier = cacheValue.getCacheValueIdentifier();
								bool roleInversed = identifier == CCacheValue::CACHEVALTAGANDINVERSEDROLE || identifier == CCacheValue::CACHEVALTAGANDINVERSEDASSERTEDROLE || identifier == CCacheValue::CACHEVALTAGANDINVERSED_NOMINAL_CONNECTED_ROLE ||
									identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDASSERTEDROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSED_NOMINAL_CONNECTED_ROLE;

								bool assertionLinkBase = identifier == CCacheValue::CACHEVALTAGANDASSERTEDROLE || identifier == CCacheValue::CACHEVALTAGANDINVERSEDASSERTEDROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ASSERTEDROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDASSERTEDROLE;

								bool nominalConnected = identifier == CCacheValue::CACHEVALTAGAND_NOMINAL_CONNECTED_ROLE || identifier == CCacheValue::CACHEVALTAGANDINVERSED_NOMINAL_CONNECTED_ROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_NOMINAL_CONNECTED_ROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSED_NOMINAL_CONNECTED_ROLE;

								bool nondeterministc = identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ASSERTEDROLE ||
									identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDASSERTEDROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_NOMINAL_CONNECTED_ROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSED_NOMINAL_CONNECTED_ROLE;

								continueVisiting = visitFunc(role, roleInversed, assertionLinkBase, nominalConnected, nondeterministc);
							}
							return true;
						}
					}
					return false;
				}




				cint64 CBackendRepresentativeMemoryCacheReader::getNeighbourCountForRole(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CRole* role) {
					cint64 count = 0;
					CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* neighbourArray = assData->getRoleSetNeighbourArray();
					CBackendRepresentativeMemoryLabelCacheItem* neighbourCombLabelItem = assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
					if (neighbourCombLabelItem && neighbourArray) {
						CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData*)neighbourCombLabelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData::TAG_RESOLVING_HASH);
						if (extensionData) {
							for (CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingDataLinker* linkerIt = extensionData->getTagLabelResolvingDataLinker(role->getRoleTag()); linkerIt; linkerIt = linkerIt->getNext()) {
								cint64 index = linkerIt->getIndex();
								count += neighbourArray->at(index).getIndividualCount();
							}
						}
					}
					return count;
				}



				bool CBackendRepresentativeMemoryCacheReader::visitNeighbourIndividualIdsForRole(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CRole* role, function<bool(cint64 neighbourIndividualId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, bool nondeterministic)> visitFunc, bool visitOnlyDeterministicNeighbours) {
					CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* neighbourArray = assData->getRoleSetNeighbourArray();
					CBackendRepresentativeMemoryLabelCacheItem* neighbourCombLabelItem = assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
					if (neighbourCombLabelItem && neighbourArray) {
						CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData*)neighbourCombLabelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData::TAG_RESOLVING_HASH);
						if (extensionData) {
							bool continueVisiting = true;
							for (CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingDataLinker* linkerIt = extensionData->getTagLabelResolvingDataLinker(role->getRoleTag()); linkerIt && continueVisiting; linkerIt = linkerIt->getNext()) {
								cint64 index = linkerIt->getIndex();
								if (!visitOnlyDeterministicNeighbours || linkerIt->isDeterministic()) {
									neighbourArray->at(index).visitNeighbourIndividualIds([&](cint64 neighbourIndiId)->bool {
										continueVisiting = visitFunc(neighbourIndiId, linkerIt->getLabelCacheItem(), !linkerIt->isDeterministic());
										return continueVisiting;
									});
								}
							}
							return true;
						}
					}
					return false;
				}







				bool CBackendRepresentativeMemoryCacheReader::hasRoleInAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic) {
					if (neighbourRoleSetLabel) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = neighbourRoleSetLabel->getTagCacheValueHash(false);
						if (labelValueHash) {
							cint64 tag = role->getRoleTag();
							if (inversed) {
								tag = -tag;
							}
							CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(tag);
							if (labelLinker && getCacheValue(role, inversed, assertionLinkBase, nominalLinkBase, nondeterministic) == labelLinker->getCacheValue()) {
								return true;
							}
						}
					}
					return false;
				}




				bool CBackendRepresentativeMemoryCacheReader::hasRoleInAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed) {
					if (neighbourRoleSetLabel) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = neighbourRoleSetLabel->getTagCacheValueHash(false);
						if (labelValueHash) {
							cint64 tag = role->getRoleTag();
							if (inversed) {
								tag = -tag;
							}
							CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(tag);
							if (labelLinker) {
								return true;
							}
						}
					}
					return false;
				}




				bool CBackendRepresentativeMemoryCacheReader::hasIndividualIdsInAssociatedIndividualSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* indiSetLabel, cint64 indiId) {
					if (indiSetLabel) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = indiSetLabel->getTagCacheValueHash(false);
						if (labelValueHash) {
							CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(indiId);
							if (labelLinker) {
								return true;
							}
						}
					}
					return false;
				}


				bool CBackendRepresentativeMemoryCacheReader::hasRoleInAssociatedCompinationRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* compinationRoleSetLabel, CRole* role, bool inversed) {
					if (compinationRoleSetLabel) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = compinationRoleSetLabel->getTagCacheValueHash(false);
						if (labelValueHash) {
							cint64 tag = role->getRoleTag();
							if (inversed) {
								tag = -tag;
							}
							CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(tag);
							if (labelLinker) {
								return true;
							}
						}
					}
					return false;
				}


				bool CBackendRepresentativeMemoryCacheReader::hasRoleInAssociatedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed, bool nondeterministic) {
					if (neighbourRoleSetLabel) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = neighbourRoleSetLabel->getTagCacheValueHash(false);
						if (labelValueHash) {
							cint64 tag = role->getRoleTag();
							if (inversed) {
								tag = -tag;
							}
							CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(tag);
							if (labelLinker) {
								CCacheValue::CACHEVALUEIDENTIFIER identifier = labelLinker->getCacheValue().getCacheValueIdentifier();
								bool nondeterministcCached = identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDROLE || CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ASSERTEDROLE ||
									identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDASSERTEDROLE || CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_NOMINAL_CONNECTED_ROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSED_NOMINAL_CONNECTED_ROLE;

								return nondeterministcCached == nondeterministic;
							}
						}
					}
					return false;
				}




				bool CBackendRepresentativeMemoryCacheReaderhasRoleInAssociatedCombinedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed) {
					if (neighbourRoleSetLabel) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = neighbourRoleSetLabel->getTagCacheValueHash(false);
						if (labelValueHash) {
							cint64 tag = role->getRoleTag();
							if (inversed) {
								tag = -tag;
							}
							CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(tag);
							if (labelLinker) {
								return true;
							}
						}
					}
					return false;
				}




				bool CBackendRepresentativeMemoryCacheReader::visitRolesOfAssociatedCompinationRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* combinationRoleSetLabel, function<bool(CRole* role, bool inversed)> visitFunc) {
					if (combinationRoleSetLabel) {
						CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinker = combinationRoleSetLabel->getCacheValueLinker();

						if (lablelValueLinker) {
							bool continueVisiting = true;
							for (CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinkerIt = lablelValueLinker; lablelValueLinkerIt && continueVisiting; lablelValueLinkerIt = lablelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = lablelValueLinkerIt->getCacheValue();

								CRole* role = (CRole*)cacheValue.getIdentification();
								bool roleInversed = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDINVERSEDROLE;
								continueVisiting = visitFunc(role, roleInversed);
							}
							return true;
						}
					}
					return false;
				}


				bool CBackendRepresentativeMemoryCacheReader::visitRolesOfAssociatedCombinedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, function<bool(CRole* role, bool inversed)> visitFunc) {
					if (neighbourRoleSetLabel) {
						CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinker = neighbourRoleSetLabel->getCacheValueLinker();

						if (lablelValueLinker) {
							bool continueVisiting = true;
							for (CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinkerIt = lablelValueLinker; lablelValueLinkerIt && continueVisiting; lablelValueLinkerIt = lablelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = lablelValueLinkerIt->getCacheValue();

								CRole* role = (CRole*)cacheValue.getIdentification();
								bool roleInversed = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDINVERSEDROLE || cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDINVERSEDASSERTEDROLE || cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDINVERSED_NOMINAL_CONNECTED_ROLE;
								continueVisiting = visitFunc(role, roleInversed);
							}
							return true;
						}
					}
					return false;
				}




				bool CBackendRepresentativeMemoryCacheReader::hasRoleInAssociatedCombinedNeigbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, CRole* role, bool inversed) {
					if (neighbourRoleSetLabel) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = neighbourRoleSetLabel->getTagCacheValueHash(false);

						if (labelValueHash) {
							cint64 tag = role->getRoleTag();
							if (inversed) {
								tag = -tag;
							}
							CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(tag);
							if (labelLinker && getCacheValue(role, inversed) == labelLinker->getCacheValue()) {
								return true;
							}
						}
					}
					return false;
				}



				bool CBackendRepresentativeMemoryCacheReader::hasRoleToNeigbourInAssociatedNeighbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 neighbourIndiId, CRole* role, bool inversed) {
					CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetHash = assData->getNeighbourRoleSetHash();
					if (neighbourRoleSetHash) {
						CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel = neighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndiId);
						if (neighbourRoleSetLabel) {
							return hasRoleInAssociatedNeigbourRoleSetLabel(assData, neighbourRoleSetLabel, role, inversed);
						}
					}
					return false;
				}



				bool CBackendRepresentativeMemoryCacheReader::visitRolesToNeigbourInAssociatedNeighbourRoleSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 neighbourIndiId, function<bool(CRole* role, bool inversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)> visitFunc) {
					if (assData) {
						CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetHash = assData->getNeighbourRoleSetHash();
						if (neighbourRoleSetHash) {
							CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel = neighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndiId);
							if (neighbourRoleSetLabel) {
								return visitRolesOfAssociatedNeigbourRoleSetLabel(assData, neighbourRoleSetLabel, visitFunc);
							}
						}
					}
					return false;
				}



				bool CBackendRepresentativeMemoryCacheReader::visitNominalIndirectlyConnectedIndividualIds(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* nomConnData, function<bool(cint64 indiId)> visitFunc) {
					if (nomConnData) {
						CXLinker<cint64>* linker = nomConnData->getIndirectlyConnectedIndividualIdLinker();
						if (linker) {
							bool continueVisiting = true;
							for (CXLinker<cint64>* connIndiIdIt = linker; connIndiIdIt && continueVisiting; connIndiIdIt = connIndiIdIt->getNext()) {
								cint64 indirectlyConnectedIndiId = connIndiIdIt->getData();
								// TODO: these individuals are only candidates, check whether they are still valid
								continueVisiting = visitFunc(indirectlyConnectedIndiId);
							}
							return true;
						}
					}
					return false;
				}



				bool CBackendRepresentativeMemoryCacheReader::visitIndividualIdsOfAssociatedIndividualSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* indiSetLabel, function<bool(cint64 indiId)> visitFunc) {
					if (indiSetLabel) {
						CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinker = indiSetLabel->getCacheValueLinker();

						if (lablelValueLinker) {
							bool continueVisiting = true;
							for (CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinkerIt = lablelValueLinker; lablelValueLinkerIt && continueVisiting; lablelValueLinkerIt = lablelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = lablelValueLinkerIt->getCacheValue();

								cint64 indiId = cacheValue.getTag();
								continueVisiting = visitFunc(indiId);
							}
							return true;
						}
					}
					return false;
				}



				bool CBackendRepresentativeMemoryCacheReader::visitConceptsOfAssociatedDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, function<bool(CConcept*, bool)> visitFunc) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = assData->getDeterministicConceptSetLabelCacheEntry();
					return visitConceptsOfAssociatedConceptSetLabel(assData, labelItem, visitFunc);
				}



				bool CBackendRepresentativeMemoryCacheReader::hasConceptInAssociatedDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CConcept* concept, bool negation) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = assData->getDeterministicConceptSetLabelCacheEntry();
					return hasConceptInAssociatedConceptSetLabel(assData, labelItem, concept, negation);
				}



				bool CBackendRepresentativeMemoryCacheReader::visitConceptsOfAssociatedNonDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, function<bool(CConcept*, bool)> visitFunc) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL);
					return visitConceptsOfAssociatedConceptSetLabel(assData, labelItem, visitFunc);
				}


				bool CBackendRepresentativeMemoryCacheReader::hasConceptInAssociatedNonDeterministicConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CConcept* concept, bool negation) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL);
					return hasConceptInAssociatedConceptSetLabel(assData, labelItem, concept, negation);
				}

				bool CBackendRepresentativeMemoryCacheReader::visitConceptsOfAssociatedConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, function<bool(CConcept*, bool)> visitFunc) {
					CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinker = labelItem->getCacheValueLinker();

					if (lablelValueLinker) {
						bool continueVisiting = true;
						for (CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinkerIt = lablelValueLinker; lablelValueLinkerIt && continueVisiting; lablelValueLinkerIt = lablelValueLinkerIt->getNext()) {
							const CCacheValue& cacheValue = lablelValueLinkerIt->getCacheValue();

							CConcept* concept = (CConcept*)cacheValue.getIdentification();
							bool conceptNegation = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
							continueVisiting = visitFunc(concept, conceptNegation);
						}
						return true;
					}
					return false;
				}


				bool CBackendRepresentativeMemoryCacheReader::hasConceptInAssociatedConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool negation) {
					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItem->getTagCacheValueHash(false);

					if (labelValueHash) {
						cint64 tag = concept->getConceptTag();
						if (negation) {
							tag = -tag;
						}

						CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(tag);
						if (labelLinker && getCacheValue(concept, negation) == labelLinker->getCacheValue()) {
							return true;
						}
					}
					return false;
				}








				bool CBackendRepresentativeMemoryCacheReader::visitLabelItemIndividualIdAssociations(CBackendRepresentativeMemoryLabelCacheItem* labelItem, function<bool(cint64 indiId, bool sameIndividualMerged)> visitFunc, bool ascending, bool visitBaseIndividual, bool visitSameMergedIndividuals) {
					CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssMapExtensionData = (CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData*)labelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::INDIVIDUAL_ASSOCIATION_MAP);
					if (indiAssMapExtensionData) {
						return indiAssMapExtensionData->visitIndividualIdAssociations(visitFunc, ascending, visitBaseIndividual, visitSameMergedIndividuals);
					}
					return false;
				}




				bool CBackendRepresentativeMemoryCacheReader::visitConceptsOfAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, function<bool(CConcept* concept, bool negation, bool deterministic)> visitFunc, bool visitDeterministicConcepts, bool visitNonDeterministicConcepts) {
					return visitConceptsOfFullConceptSetLabel(labelItem, visitFunc, visitDeterministicConcepts, visitNonDeterministicConcepts);
				}




				bool CBackendRepresentativeMemoryCacheReader::visitConceptsOfFullConceptSetLabel(CBackendRepresentativeMemoryLabelCacheItem* labelItem, function<bool(CConcept* concept, bool negation, bool deterministic)> visitFunc, bool visitDeterministicConcepts, bool visitNonDeterministicConcepts) {
					if (labelItem) {
						CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinker = labelItem->getCacheValueLinker();

						if (lablelValueLinker) {
							bool continueVisiting = true;
							for (CBackendRepresentativeMemoryLabelValueLinker* lablelValueLinkerIt = lablelValueLinker; lablelValueLinkerIt && continueVisiting; lablelValueLinkerIt = lablelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = lablelValueLinkerIt->getCacheValue();

								CConcept* concept = (CConcept*)cacheValue.getIdentification();
								bool conceptNegation = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT || cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_NEGATED_CONCEPT;
								bool deterministic = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT || cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDCONCEPT;
								if (deterministic && visitDeterministicConcepts || !deterministic && visitNonDeterministicConcepts) {
									continueVisiting = visitFunc(concept, conceptNegation, deterministic);
								}
							}
							return true;
						}
					}
					return false;
				}




				bool CBackendRepresentativeMemoryCacheReader::getConceptOccurrenceInAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool& negationFlag, bool& deterministicFlag) {
					if (labelItem) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItem->getTagCacheValueHash(false);
						cint64 tag = concept->getConceptTag();
						if (labelValueHash) {

							CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(tag);
							if (!labelLinker) {
								labelLinker = labelValueHash->value(-tag);
								if (labelLinker) {
									negationFlag = true;
								}
							} else {
								negationFlag = false;
							}

							if (labelLinker) {

								const CCacheValue& cacheValue = labelLinker->getCacheValue();
								CConcept* concept = (CConcept*)cacheValue.getIdentification();
								bool deterministicFlag = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT || cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDCONCEPT;

								return true;
							}
						}
					}
					return false;
				}


				bool CBackendRepresentativeMemoryCacheReader::hasConceptInAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool negation) {
					if (labelItem) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItem->getTagCacheValueHash(false);
						if (labelValueHash) {
							cint64 tag = concept->getConceptTag();
							if (negation) {
								tag = -tag;
							}
							CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(tag);
							if (labelLinker) {
								return true;
							}
						}
					}
					return false;
				}




				bool CBackendRepresentativeMemoryCacheReader::hasConceptInAssociatedFullConceptSetLabel(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CBackendRepresentativeMemoryLabelCacheItem* labelItem, CConcept* concept, bool negation, bool deterministic) {
					if (labelItem) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItem->getTagCacheValueHash(false);
						if (labelValueHash) {
							cint64 tag = concept->getConceptTag();
							if (negation) {
								tag = -tag;
							}
							CBackendRepresentativeMemoryLabelValueLinker* labelLinker = labelValueHash->value(tag);
							if (labelLinker && getCacheValue(concept, negation, deterministic) == labelLinker->getCacheValue()) {
								return true;
							}
						}
					}
					return false;
				}
















				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getLabelCacheEntryViaProvidedCacheValues(cint64 labelType, cint64 signature, cint64 count, function<bool(bool, cint64&, CCacheValue&)> provFunc) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = nullptr;
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* sigResEntry = getLabelCacheEntry(labelType, signature);
					if (sigResEntry) {
						for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResEntry->getLabelItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getCacheValueCount() == count) {
								bool compatible = true;
								CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* cacheValueHash = itemIt->getTagCacheValueHash(false);
								CCacheValue cacheValue;
								cint64 tag;
								bool resetProviding = true;
								while (compatible) {
									if (!provFunc(resetProviding, tag, cacheValue)) {
										break;
									}
									resetProviding = false;
									CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(tag);
									if (!labelValueLinker || labelValueLinker->getCacheValue() != cacheValue) {
										compatible = false;
									}
								}
								if (compatible) {
									labelItem = itemIt;
								}
							}
						}
					}
					return labelItem;
				}





				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getLabelCacheEntryViaRoleLinker(cint64 labelType, cint64 signature, cint64 count, CSortedNegLinker<CRole*>* roleLinker, bool inversed, CRole* assertedRole) {
					CSortedNegLinker<CRole*>* roleLinkerIt = roleLinker;
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = getLabelCacheEntryViaProvidedCacheValues(labelType, signature, count, [&](bool resetProviding, cint64& tag, CCacheValue& cacheValue)->bool {
						if (resetProviding) {
							roleLinkerIt = roleLinker;
						}
						if (!roleLinkerIt) {
							return false;
						}
						CRole* role = roleLinkerIt->getData();
						bool roleInversion = roleLinkerIt->isNegated() ^ inversed;
						bool assertionLinkBase = role == assertedRole && roleInversion == inversed;
						cacheValue = getCacheValue(role, roleInversion, assertionLinkBase, false);
						tag = cacheValue.getTag();
						roleLinkerIt = roleLinkerIt->getNext();
						return true;
					});
					return labelItem;
				}



				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getLabelCacheEntryViaRoleAssertionLinker(cint64 labelType, cint64 signature, cint64 count, CBackendRepresentativeMemoryCacheRoleAssertionLinker* roleAssertionLinker) {
					CBackendRepresentativeMemoryCacheRoleAssertionLinker* roleAssertionLinkerIt = roleAssertionLinker;
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = getLabelCacheEntryViaProvidedCacheValues(labelType, signature, count, [&](bool resetProviding, cint64& tag, CCacheValue& cacheValue)->bool {
						if (resetProviding) {
							roleAssertionLinkerIt = roleAssertionLinker;
						}
						if (!roleAssertionLinkerIt) {
							return false;
						}
						CRole* role = roleAssertionLinkerIt->getData();
						bool roleInversion = roleAssertionLinkerIt->isNegated();
						bool assertionLinkBase = roleAssertionLinkerIt->isABoxAsserted();
						bool nominalConnected = roleAssertionLinkerIt->isNominalConnected();
						bool nondeterministic = roleAssertionLinkerIt->isNondeterministic();
						cacheValue = getCacheValue(role, roleInversion, assertionLinkBase, nominalConnected, nondeterministic);
						tag = cacheValue.getTag();
						roleAssertionLinkerIt = (CBackendRepresentativeMemoryCacheRoleAssertionLinker*)roleAssertionLinkerIt->getNext();
						return true;
					});
					return labelItem;
				}






				const CCacheValue CBackendRepresentativeMemoryCacheReader::getCacheValue(CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neigbourRoleInstantiatedSetTmpLabelLinker) {
					CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem = neigbourRoleInstantiatedSetTmpLabelLinker->getReferredLabelData();
					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpLabelWriteData = neigbourRoleInstantiatedSetTmpLabelLinker->getReferredTemporaryLabelData();

					cint64 tag = 0;
					CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHE_VALUE_TAG_AND_ENTRY;
					cint64 value = (cint64)labelCacheItem;
					if (tmpLabelWriteData) {
						cacheValueIdentifier = CCacheValue::CACHE_VALUE_TAG_AND_TEMPORARY_ENTRY;
						value = (cint64)tmpLabelWriteData;
					} else {
						tag = labelCacheItem->getCacheEntryID();
					}
					CCacheValue cacheValue;
					cacheValue.initCacheValue(tag, (cint64)value, cacheValueIdentifier);

					return cacheValue;
				}



				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getNeighbourRoleInstantiatedSetCompinationLabelCacheEntry(cint64 signature, cint64 count, CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neigbourRoleInstantiatedSetTmpLabelLinker) {
					CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neigbourRoleInstantiatedSetTmpLabelLinkerIt = neigbourRoleInstantiatedSetTmpLabelLinker;
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = getLabelCacheEntryViaProvidedCacheValues(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, signature, count, [&](bool resetProviding, cint64& tag, CCacheValue& cacheValue)->bool {
						if (resetProviding) {
							neigbourRoleInstantiatedSetTmpLabelLinkerIt = neigbourRoleInstantiatedSetTmpLabelLinker;
						}
						if (!neigbourRoleInstantiatedSetTmpLabelLinkerIt) {
							return false;
						}
						cacheValue = getCacheValue(neigbourRoleInstantiatedSetTmpLabelLinkerIt);
						tag = (cint64)cacheValue.getTag();

						neigbourRoleInstantiatedSetTmpLabelLinkerIt = (CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker*)neigbourRoleInstantiatedSetTmpLabelLinkerIt->getNext();
						return true;
					});
					return labelItem;
				}





				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getConceptSetLabelCacheEntry(cint64 labelType, cint64 signature, cint64 conCount, CConceptSaturationDescriptor* conDesLinker) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = nullptr;
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* sigResEntry = getLabelCacheEntry(labelType, signature);
					if (sigResEntry) {
						for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResEntry->getLabelItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getCacheValueCount() == conCount) {
								bool compatible = true;
								CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* cacheValueHash = itemIt->getTagCacheValueHash(false);
								for (CConceptSaturationDescriptor* conDesIt = conDesLinker; conDesIt && compatible; conDesIt = conDesIt->getNext()) {
									cint64 conTag = conDesIt->getConceptTag();
									CConcept* concept = conDesIt->getConcept();
									bool negation = conDesIt->isNegated();
									if (negation) {
										conTag = -conTag;
									}
									CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(conTag);
									if (!labelValueLinker || labelValueLinker->getCacheValue() != getCacheValue(concept,negation)) {
										compatible = false;
									}									
								}
								if (compatible) {
									labelItem = itemIt;
								}
							}
						}
					}
					return labelItem;
				}



				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getDeterministicConceptSetLabelCacheEntry(cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, cint64 maxDeterministicBranchTag, bool excludePositiveNominalConcepts) {
					return getConceptSetLabelCacheEntry(signature, conCount, conDesLinker, true, maxDeterministicBranchTag, excludePositiveNominalConcepts);
				}


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getNondeterministicConceptSetLabelCacheEntry(cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, cint64 maxDeterministicBranchTag, bool excludePositiveNominalConcepts) {
					return getConceptSetLabelCacheEntry(signature, conCount, conDesLinker, false, maxDeterministicBranchTag, excludePositiveNominalConcepts);
				}


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getConceptSetLabelCacheEntry(cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, bool deterministic, cint64 maxDeterministicBranchTag, bool excludePositiveNominalConcepts) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = nullptr;
					cint64 labelType = CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_CONCEPT_SET_LABEL;
					if (!deterministic) {
						labelType = CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL;
					}
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* sigResEntry = getLabelCacheEntry(labelType, signature);
					if (sigResEntry) {
						for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResEntry->getLabelItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getCacheValueCount() == conCount) {
								bool compatible = true;
								CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* cacheValueHash = itemIt->getTagCacheValueHash(false);
								for (CConceptDescriptor* conDesIt = conDesLinker; conDesIt && compatible; conDesIt = conDesIt->getNext()) {
									cint64 conTag = conDesIt->getConceptTag();
									CConcept* concept = conDesIt->getConcept();
									bool negation = conDesIt->isNegated();
									if (negation) {
										conTag = -conTag;
									}
									bool deterministicallyDerived = true;
									if (!conDesIt->getDependencyTrackPoint() || conDesIt->getDependencyTrackPoint()->getBranchingTag() > maxDeterministicBranchTag) {
										deterministicallyDerived = false;
									}
									bool considerConcept = deterministicallyDerived == deterministic;
									if (excludePositiveNominalConcepts && concept->getOperatorCode() == CCNOMINAL && !negation) {
										considerConcept = false;
									}
									if (considerConcept) {
										CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(conTag);
										if (!labelValueLinker || labelValueLinker->getCacheValue() != getCacheValue(concept, negation)) {
											compatible = false;
										}
									}
								}
								if (compatible) {
									labelItem = itemIt;
								}
							}
						}
					}
					return labelItem;
				}


















				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getFullConceptSetLabelCacheEntry(cint64 labelType, cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, function<bool(CConcept* concept, bool negation)> exclusionDetermineFunction, function<bool(CConcept* concept, bool negation, CDependencyTrackPoint* depTrackPoint)> nondeterministicDetermineFunction) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = nullptr;
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* sigResEntry = getLabelCacheEntry(labelType, signature);
					if (sigResEntry) {
						for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResEntry->getLabelItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getCacheValueCount() == conCount) {
								bool compatible = true;
								CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* cacheValueHash = itemIt->getTagCacheValueHash(false);
								for (CConceptDescriptor* conDesIt = conDesLinker; conDesIt && compatible; conDesIt = conDesIt->getNext()) {
									cint64 conTag = conDesIt->getConceptTag();
									CConcept* concept = conDesIt->getConcept();
									bool negation = conDesIt->isNegated();
									if (negation) {
										conTag = -conTag;
									}
									bool considerConcept = exclusionDetermineFunction(concept, negation);
									if (considerConcept) {
										bool deterministicConcept = nondeterministicDetermineFunction(concept, negation, conDesIt->getDependencyTrackPoint());
										CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(conTag);
										if (!labelValueLinker || labelValueLinker->getCacheValue() != getCacheValue(concept, negation, deterministicConcept)) {
											compatible = false;
										}
									}
								}
								if (compatible) {
									labelItem = itemIt;
								}
							}
						}
					}
					return labelItem;
				}






				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getConceptSetLabelCacheEntry(cint64 labelType, cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, function<bool(CConcept* concept, bool negation)> exclusionDetermineFunction) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = nullptr;
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* sigResEntry = getLabelCacheEntry(labelType, signature);
					if (sigResEntry) {
						for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResEntry->getLabelItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getCacheValueCount() == conCount) {
								bool compatible = true;
								CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* cacheValueHash = itemIt->getTagCacheValueHash(false);
								for (CConceptDescriptor* conDesIt = conDesLinker; conDesIt && compatible; conDesIt = conDesIt->getNext()) {
									cint64 conTag = conDesIt->getConceptTag();
									CConcept* concept = conDesIt->getConcept();
									bool negation = conDesIt->isNegated();
									if (negation) {
										conTag = -conTag;
									}
									bool considerConcept = exclusionDetermineFunction(concept, negation);
									if (considerConcept) {
										CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(conTag);
										if (!labelValueLinker || labelValueLinker->getCacheValue() != getCacheValue(concept, negation)) {
											compatible = false;
										}
									}
								}
								if (compatible) {
									labelItem = itemIt;
								}
							}
						}
					}
					return labelItem;
				}








				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getIndividualSetLabelCacheEntry(cint64 labelType, cint64 signature, cint64 indiId, CDistinctHash* indiDistinctHash, cint64& count, bool onlyDeterministic, cint64 maxDeterministicBranchTag) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = nullptr;
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* sigResEntry = getLabelCacheEntry(labelType, signature);
					if (sigResEntry) {
						for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResEntry->getLabelItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getCacheValueCount() == count) {
								bool compatible = true;
								CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* cacheValueHash = itemIt->getTagCacheValueHash(false);
								for (CDistinctHash::const_iterator it = indiDistinctHash->constBegin(), itEnd = indiDistinctHash->constEnd(); it != itEnd; ++it) {
									cint64 distinctIndiId = -it.key();
									if (distinctIndiId >= 0 && distinctIndiId != indiId) {
										CDistinctEdge* distinctEdge = it.value();
										bool deterministicallyDerived = true;
										if (!distinctEdge->getDependencyTrackPoint() || distinctEdge->getDependencyTrackPoint()->getBranchingTag() > maxDeterministicBranchTag) {
											deterministicallyDerived = false;
										}
										bool considerIndividual = deterministicallyDerived || !onlyDeterministic;
										if (considerIndividual) {
											CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(distinctIndiId);
											if (!labelValueLinker || labelValueLinker->getCacheValue() != getCacheValue(distinctIndiId, false)) {
												compatible = false;
											}
										}
									}
								}
								if (compatible && indiId >= 0) {
									CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(indiId);
									if (!labelValueLinker || labelValueLinker->getCacheValue() != getCacheValue(indiId, false)) {
										compatible = false;
									}
								}
								if (compatible) {
									labelItem = itemIt;
								}
							}
						}
					}
					return labelItem;
				}






				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getIndividualSetLabelCacheEntry(cint64 labelType, cint64 signature, cint64 indiId, CIndividualMergingHash* indiMerginghash, cint64& count, bool onlyDeterministic, cint64 maxDeterministicBranchTag) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = nullptr;
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* sigResEntry = getLabelCacheEntry(labelType, signature);
					if (sigResEntry) {
						for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResEntry->getLabelItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getCacheValueCount() == count) {
								bool compatible = true;
								CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* cacheValueHash = itemIt->getTagCacheValueHash(false);
								for (CIndividualMergingHash::const_iterator it = indiMerginghash->constBegin(), itEnd = indiMerginghash->constEnd(); it != itEnd; ++it) {
									if (it.value().isMergedWithIndividual()) {
										cint64 mergedIndiId = it.key();
										if (mergedIndiId != indiId) {
											CIndividualMergingHashData mergedData = it.value();
											bool deterministicallyDerived = true;
											if (!mergedData.getDependencyTrackPoint() || mergedData.getDependencyTrackPoint()->getBranchingTag() > maxDeterministicBranchTag) {
												deterministicallyDerived = false;
											}
											bool considerIndividual = deterministicallyDerived || !onlyDeterministic;
											if (considerIndividual) {
												CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(mergedIndiId);
												if (!labelValueLinker || labelValueLinker->getCacheValue() != getCacheValue(mergedIndiId, false)) {
													compatible = false;
												}
											}
										}
									}
								}
								if (compatible && indiId >= 0) {
									CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(indiId);
									if (!labelValueLinker || labelValueLinker->getCacheValue() != getCacheValue(indiId, false)) {
										compatible = false;
									}
								}
								if (compatible) {
									labelItem = itemIt;
								}
							}
						}
					}
					return labelItem;
				}





				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getIndividualSetLabelCacheEntry(cint64 labelType, cint64 signature, CPROCESSSET<cint64>* individualSet, cint64& count) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = nullptr;
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* sigResEntry = getLabelCacheEntry(labelType, signature);
					if (sigResEntry) {
						for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResEntry->getLabelItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getCacheValueCount() == count) {
								bool compatible = true;
								CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* cacheValueHash = itemIt->getTagCacheValueHash(false);
								for (CPROCESSSET<cint64>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
									cint64 indiId = *it;
									CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(indiId);
									if (!labelValueLinker || labelValueLinker->getCacheValue() != getCacheValue(indiId, false)) {
										compatible = false;
									}
								}
								if (compatible) {
									labelItem = itemIt;
								}
							}
						}
					}
					return labelItem;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
