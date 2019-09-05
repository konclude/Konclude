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

#include "COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator::COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList,
					QHash<cint64, COptimizedKPSetIndividualItem*>* individualInstantiatedItemHash, CIndividualVector* individualVector, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor)
				
					: COptimizedRepresentativeKPSetCacheLabelItemIterator(labelCacheItemKnownInstancesList, labelCacheItemPossibleInstancesList, individualInstantiatedItemHash, individualVector, sorting, indiInstItemRefCursor, moveOverCursor) {


				TRoleInstancesItemInversedFlagPair roleInstItemInvFlagPair(roleInstancesItem, inversed);
				QSet<TRoleInstancesItemInversedFlagPair> roleInstItemInvFlagPairSet;
				roleInstItemInvFlagPairSet.insert(roleInstItemInvFlagPair);
				for (CBackendRepresentativeMemoryLabelCacheItem* labelItem : labelCacheItemPossibleInstancesList) {
					mPossibleLabelRoleInstancesItemInversionPairSetHash.insert(labelItem, roleInstItemInvFlagPairSet);
				}
				mRoleInstancesItemInversionPairSet.insert(roleInstItemInvFlagPair);

				init();

				mPreventSameIndisMultipleOccurences = true;
			}

			COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator::COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList, 
					QHash<cint64, COptimizedKPSetIndividualItem*>* individualInstantiatedItemHash, CIndividualVector* individualVector, const QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TRoleInstancesItemInversedFlagPair>>& pssibleLabelRoleInstancesItemInversionPairSetHash, const QSet<TRoleInstancesItemInversedFlagPair>& roleInstancesItemInversionPairSet, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor)
				
					: mPossibleLabelRoleInstancesItemInversionPairSetHash(pssibleLabelRoleInstancesItemInversionPairSetHash), mRoleInstancesItemInversionPairSet(roleInstancesItemInversionPairSet), COptimizedRepresentativeKPSetCacheLabelItemIterator(labelCacheItemKnownInstancesList, labelCacheItemPossibleInstancesList, individualInstantiatedItemHash, individualVector, sorting, indiInstItemRefCursor, moveOverCursor) {

				init();


				mPreventSameIndisMultipleOccurences = true;
			}



			COptimizedRepresentativeKPSetCacheLabelItemIterator::CAssociationMapIteratorData* COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator::createAssociationMapIteratorData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, const CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator& iterator, bool possibleInstances) {
				CAssociationMapIteratorDataWithRoleInstancesItems* iteratorData = new CAssociationMapIteratorDataWithRoleInstancesItems(labelItem, iterator, possibleInstances);
				if (possibleInstances) {
					iteratorData->mRoleInstancesItemInversionPairSet = mPossibleLabelRoleInstancesItemInversionPairSetHash.value(labelItem);
				}
				return iteratorData;
			}


			QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TRoleInstancesItemInversedFlagPair>>* COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator::getPossibleLabelRoleInstancesItemInversionPairSetHash() {
				return &mPossibleLabelRoleInstancesItemInversionPairSetHash;
			}


			QSet<TRoleInstancesItemInversedFlagPair>* COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator::getRoleInstancesItemInversionPairSet() {
				return &mRoleInstancesItemInversionPairSet;
			}



			QSet<TRoleInstancesItemInversedFlagPair>* COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator::currentRoleInstancesItemInversionPairSet() {
				if (mCurrentIteratorData) {
					CAssociationMapIteratorDataWithRoleInstancesItems* roleInstanceItemsIteratorData = (CAssociationMapIteratorDataWithRoleInstancesItems*)mCurrentIteratorData;
					return &roleInstanceItemsIteratorData->mRoleInstancesItemInversionPairSet;
				} else {
					return nullptr;
				}
			}


			bool COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator::isValidIteratorPosition(CAssociationMapIteratorData* iteratorData, cint64 indiId) {
				++mAssociatedIndiPossibleVisistedCount;

				bool isValidInstanceFlag = true;
				bool isValidKnownInstanceFlag = true;
				bool isValidPossibleInstanceFlag = true;

				CAssociationMapIteratorDataWithRoleInstancesItems* roleInstanceItemsIteratorData = (CAssociationMapIteratorDataWithRoleInstancesItems*)iteratorData;
				for (TRoleInstancesItemInversedFlagPair roleInstancesItemInversedFlagPair : roleInstanceItemsIteratorData->mRoleInstancesItemInversionPairSet) {
					COptimizedKPSetRoleInstancesItem* roleInstancesItem = roleInstancesItemInversedFlagPair.first;
					bool inversed = roleInstancesItemInversedFlagPair.second;

					checkValidIteratorPosition(iteratorData, indiId, roleInstancesItem, inversed, isValidInstanceFlag, isValidKnownInstanceFlag, isValidPossibleInstanceFlag);
					if (!isValidInstanceFlag) {
						return false;
					}
				}

				if (isValidInstanceFlag) {
					if (!isValidKnownInstanceFlag && isValidPossibleInstanceFlag) {
						iteratorData->mCurrentInstanceOnlyPossible = true;
					}
					++mAssociatedIndiPossibleConfirmedCount;
					return true;
				}
				return false;

			}






			void COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator::checkValidIteratorPosition(CAssociationMapIteratorData* iteratorData, cint64 indiId, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, bool& isValidInstanceFlag, bool& isValidKnownInstanceFlag, bool& isValidPossibleInstanceFlag) {
				if (iteratorData->mCurrentIndiRealizationItem) {
					if (mConsiderSameMergedIndis || (!iteratorData->mCurrentIndiRealizationItem->isItemSameIndividualMerged() && indiId == iteratorData->mCurrentIndiRealizationItem->getIndividualId())) {


						COptimizedKPSetRoleNeighbourInstancesHash* roleNeighbourHash = iteratorData->mCurrentIndiRealizationItem->getKnownPossibleRoleNeighboursInstancesHash();
						const COptimizedKPSetRoleNeighbourInstancesHashData& roleNeighbourHashData = roleNeighbourHash->value(roleInstancesItem);
						COptimizedKPSetRoleInstancesHash* neighbourHash = nullptr;
						if (inversed) {
							neighbourHash = roleNeighbourHashData.mRolePredecessorInstancesHash;
						} else {
							neighbourHash = roleNeighbourHashData.mRoleSuccessorInstancesHash;
						}
						if (neighbourHash) {

							cint64 maxRemainingNeighbourInvestigationCount = 50;
							cint64 remainingNeighbourInvestigationCount = -1;
							bool somePossibleInstanceFlag = false;
							bool someInitializationRequired = false;

							for (COptimizedKPSetRoleInstancesHash::const_iterator it = neighbourHash->constBegin(), itEnd = neighbourHash->constEnd(); it != itEnd && remainingNeighbourInvestigationCount != 0; ++it) {
								// TODO: optimize by using pointer/counter/flag to/for next possible instance data that is a known instance
								if (remainingNeighbourInvestigationCount > 0) {
									remainingNeighbourInvestigationCount--;
								}
								const COptimizedKPSetRoleInstancesHashData& neighbourData = it.value();
								COptimizedKPSetRoleInstancesData* roleInstData = neighbourData.mInstanceItemData;
								if (roleInstData && roleInstData->mKnownInstance) {
									return;
								} else if (mAllowPossibleInstances) {
									somePossibleInstanceFlag = true;
									if (remainingNeighbourInvestigationCount == -1) {
										remainingNeighbourInvestigationCount = maxRemainingNeighbourInvestigationCount;
									}
								} else if (roleInstData->mPossibleInstance && !roleInstData->mTestedInstance && !roleInstData->mDerived) {
									someInitializationRequired = true;
									if (remainingNeighbourInvestigationCount == -1) {
										remainingNeighbourInvestigationCount = maxRemainingNeighbourInvestigationCount;
									}
								}
							}

							if (somePossibleInstanceFlag) {
								isValidKnownInstanceFlag = false;
								return;
							}
							if (someInitializationRequired) {
								iteratorData->mInitializationRequired = true;
								isValidInstanceFlag = false;
								return;
							}

						}
					}
				}
				if (roleInstancesItem->hasComplexRoleData()) {


					QHash<cint64, COptimizedKPSetIndividualComplexRoleData*>* complexRoleDataHash = roleInstancesItem->getIndividualIdComplexRoleDataHash();
					if (!complexRoleDataHash->contains(indiId)) {
						isValidInstanceFlag = false;
						return;
					} else {
						COptimizedKPSetIndividualComplexRoleData* complexRoleData = complexRoleDataHash->value(indiId);
						if (!complexRoleData || !complexRoleData->isInitialized(inversed)) {
							iteratorData->mInitializationRequired = true;
							isValidInstanceFlag = false;
							return;
						} else {

							COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)complexRoleData;
							COptimizedKPSetRoleInstancesHash* possNeighbourInstanceHash = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstancesHash(inversed, false);
							if (possNeighbourInstanceHash) {

								cint64 maxRemainingNeighbourInvestigationCount = 50;
								cint64 remainingNeighbourInvestigationCount = -1;
								bool somePossibleInstanceFlag = false;
								bool someInitializationRequired = false;

								for (COptimizedKPSetRoleInstancesHash::const_iterator itNeighInst = possNeighbourInstanceHash->constBegin(), itNeighInstEnd = possNeighbourInstanceHash->constEnd(); itNeighInst != itNeighInstEnd && remainingNeighbourInvestigationCount != 0; ++itNeighInst) {
									// TODO: optimize by using pointer/counter/flag to/for next possible instance data that is a known instance
									if (remainingNeighbourInvestigationCount > 0) {
										remainingNeighbourInvestigationCount--;
									}
									const COptimizedKPSetRoleInstancesHashData& destIndiInstanceDat = itNeighInst.value();
									cint64 destIndiId = itNeighInst.key();
									COptimizedKPSetRoleInstancesData* instanceData = destIndiInstanceDat.mInstanceItemData;
									if (instanceData && instanceData->mKnownInstance) {
										return;
									} else if (mAllowPossibleInstances) {
										somePossibleInstanceFlag = true;
										if (remainingNeighbourInvestigationCount == -1) {
											remainingNeighbourInvestigationCount = maxRemainingNeighbourInvestigationCount;
										}
									} else if (instanceData->mPossibleInstance && !instanceData->mTestedInstance && !instanceData->mDerived) {
										someInitializationRequired = true;
										if (remainingNeighbourInvestigationCount == -1) {
											remainingNeighbourInvestigationCount = maxRemainingNeighbourInvestigationCount;
										}
									}
								}


								if (somePossibleInstanceFlag) {
									isValidKnownInstanceFlag = false;
									return;
								}
								if (someInitializationRequired) {
									iteratorData->mInitializationRequired = true;
									isValidInstanceFlag = false;
									return;
								}

							}
						}
					}

				}

				isValidInstanceFlag = false;
			}




			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator::getCopy() {
				COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator* iteratorCopy = new COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator(QList<CBackendRepresentativeMemoryLabelCacheItem*>(), QList<CBackendRepresentativeMemoryLabelCacheItem*>(), mIndividualInstantiatedItemHash, mIndividualVector, mPossibleLabelRoleInstancesItemInversionPairSetHash, mRoleInstancesItemInversionPairSet, mSorting, mIndiInstItemRefCursor, mMoveOverCursor);
				this->initCopy(iteratorCopy);
				return iteratorCopy;
			}

			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator::initCopy(CRealizationIndividualInstanceItemReferenceIterator* iterator) {
				COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator* cacheLabelItemIterator = (COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator*)iterator;
				COptimizedRepresentativeKPSetCacheLabelItemIterator::initCopy(cacheLabelItemIterator);
				return cacheLabelItemIterator;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
