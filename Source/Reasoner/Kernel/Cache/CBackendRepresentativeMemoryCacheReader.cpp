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

#include "CBackendRepresentativeMemoryCacheReader.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheReader::CBackendRepresentativeMemoryCacheReader() : CLinkerBase<CBackendRepresentativeMemoryCacheReader*,CBackendRepresentativeMemoryCacheReader>(this) {
					mCurrentSlot = nullptr;
				}


				CBackendRepresentativeMemoryCacheReader* CBackendRepresentativeMemoryCacheReader::updateSlot(CBackendRepresentativeMemoryCacheSlotItem* updatedSlot) {
					CBackendRepresentativeMemoryCacheSlotItem* prevSlot = mUpdatedSlot.fetchAndStoreOrdered(updatedSlot);
					if (prevSlot != nullptr) {
						prevSlot->decReader();
					}
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
						return true;
					}
					return false;
				}


				bool CBackendRepresentativeMemoryCacheReader::hasCacheEntry(cint64 signature) {
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>* sigItemHash = mCurrentSlot->getSignatureLabelItemHash();
						if (sigItemHash) {
							return sigItemHash->contains(signature);
						}
					}
					return false;
				}



				CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* CBackendRepresentativeMemoryCacheReader::getLabelCacheEntry(cint64 signature) {
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* entry = nullptr;
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>* sigItemHash = mCurrentSlot->getSignatureLabelItemHash();
						if (sigItemHash) {
							entry = sigItemHash->value(signature,nullptr);
						}
					}
					return entry;
				}


				CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem* CBackendRepresentativeMemoryCacheReader::getCardinalityCacheEntry(cint64 signature) {
					CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem* entry = nullptr;
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*>* sigItemHash = mCurrentSlot->getSignatureCardinalityItemHash();
						if (sigItemHash) {
							entry = sigItemHash->value(signature,nullptr);
						}
					}
					return entry;
				}


				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheReader::getIndividualAssociation(CIndividual* individual) {
					CBackendRepresentativeMemoryCacheIndividualAssociationData* assData = nullptr;
					CIndividualProcessData* indiProData = (CIndividualProcessData*)individual->getIndividualData();
					if (indiProData) {
						assData = (CBackendRepresentativeMemoryCacheIndividualAssociationData*)indiProData->getBackendCachingData();
					}
					return assData;
				}



				const CCacheValue CBackendRepresentativeMemoryCacheReader::getCacheValue(CConcept* concept, bool negation) {
					cint64 conTag = concept->getConceptTag();
					CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDCONCEPT;
					if (negation) {
						cacheValueIdentifier = CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
					}
					CCacheValue cacheValue;
					cacheValue.initCacheValue(conTag,(cint64)concept,cacheValueIdentifier);
					return cacheValue;
				}



				const CCacheValue CBackendRepresentativeMemoryCacheReader::getCacheValue(CRole* role) {
					cint64 roleTag = role->getRoleTag();
					CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDROLE;
					CCacheValue cacheValue;
					cacheValue.initCacheValue(roleTag,(cint64)role,cacheValueIdentifier);
					return cacheValue;
				}



				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getLabelCacheEntry(cint64 signature, cint64 conCount, CConceptSaturationDescriptor* conDesLinker) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = nullptr;
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* sigResEntry = getLabelCacheEntry(signature);
					if (sigResEntry) {
						for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResEntry->getLabelItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getDeterministicCacheValueCount() == conCount) {
								bool compatible = true;
								CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* cacheValueHash = itemIt->getDeterministicTagCacheValueHash(false);
								for (CConceptSaturationDescriptor* conDesIt = conDesLinker; conDesIt && compatible; conDesIt = conDesIt->getNext()) {
									cint64 conTag = conDesIt->getConceptTag();
									CConcept* concept = conDesIt->getConcept();
									bool negation = conDesIt->isNegated();
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



				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheReader::getLabelCacheEntry(cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, CConcept* exceptionConcept) {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = nullptr;
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* sigResEntry = getLabelCacheEntry(signature);
					if (sigResEntry) {
						for (CBackendRepresentativeMemoryLabelCacheItem* itemIt = sigResEntry->getLabelItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getDeterministicCacheValueCount() == conCount) {
								bool compatible = true;
								CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* cacheValueHash = itemIt->getDeterministicTagCacheValueHash(false);
								for (CConceptDescriptor* conDesIt = conDesLinker; conDesIt && compatible; conDesIt = conDesIt->getNext()) {
									cint64 conTag = conDesIt->getConceptTag();
									CConcept* concept = conDesIt->getConcept();
									bool negation = conDesIt->isNegated();
									if (concept != exceptionConcept) {
										CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = cacheValueHash->value(conTag);
										if (!labelValueLinker || labelValueLinker->getCacheValue() != getCacheValue(concept,negation)) {
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




				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCacheReader::getCardinalityCacheEntry(cint64 signature, CPROCESSHASH<CRole*,cint64>* roleFreeCardHash) {
					CBackendRepresentativeMemoryCardinalityCacheItem* labelItem = nullptr;
					CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem* sigResEntry = getCardinalityCacheEntry(signature);
					if (sigResEntry) {
						cint64 roleCardCount = roleFreeCardHash->size();
						for (CBackendRepresentativeMemoryCardinalityCacheItem* itemIt = sigResEntry->getCardinalityCacheItems(); itemIt && !labelItem; itemIt = itemIt->getNext()) {
							if (itemIt->getCardinalityCacheValueCount() == roleCardCount) {
								bool compatible = true;
								CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalityValueLinker*>* cacheValueHash = itemIt->getTagCardinalityCacheValueHash(false);

								for (CPROCESSHASH<CRole*,cint64>::const_iterator it = roleFreeCardHash->constBegin(), itEnd = roleFreeCardHash->constEnd(); it != itEnd; ++it) {
									CRole* role(it.key());
									cint64 freeCard(it.value());
									cint64 roleTag = role->getRoleTag();
									CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinker = cacheValueHash->value(roleTag);
									if (!cardValueLinker || cardValueLinker->getCacheValue() != getCacheValue(role) || cardValueLinker->getFreeCardinality() != freeCard) {
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
