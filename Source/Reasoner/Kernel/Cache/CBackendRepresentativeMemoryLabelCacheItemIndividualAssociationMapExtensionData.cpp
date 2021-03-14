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

#include "CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData(CContext* context) : CBackendRepresentativeMemoryLabelCacheItemExtensionData(context) {
					mCacheItemExtensionType = INDIVIDUAL_ASSOCIATION_MAP;
				}



				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::addIndividualIdAssociation(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssocData) {
					return addIndividualIdAssociation(indiAssocData->getAssociatedIndividualId(), indiAssocData->hasRepresentativeSameIndividualMerging());
				}

				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::removeIndividualIdAssociation(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssocData) {
					return removeIndividualIdAssociation(indiAssocData->getAssociatedIndividualId(), indiAssocData->hasRepresentativeSameIndividualMerging());
				}


				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::addIndividualIdAssociation(cint64 indiId, bool sameIndividualMerged) {
					if (!sameIndividualMerged) {
						mBaseIndiAssoMap.insert(indiId, DummyValue());
					} else {
						mSameIndiMergedAssoMap.insert(indiId, DummyValue());
					}
					return this;
				}

				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::removeIndividualIdAssociation(cint64 indiId, bool sameIndividualMerged) {
					if (!sameIndividualMerged) {
						mBaseIndiAssoMap.remove(indiId);
					} else {
						mSameIndiMergedAssoMap.remove(indiId);
					}
					return this;
				}


				cint64 CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::getIndividualIdAssociationCount() {
					return mBaseIndiAssoMap.size() + mSameIndiMergedAssoMap.size();
				}


				bool CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::visitIndividualIdAssociationsAscending(function<bool(cint64 indiId, bool sameIndividualMerged)> visitFunc, bool visitBaseIndividuals, bool visitSameMergedIndividuals) {
					bool visited = false;
					bool continueVisiting = true;

					QMap<cint64, DummyValue>::const_iterator itBase = mBaseIndiAssoMap.constBegin(), itBaseEnd = mBaseIndiAssoMap.constEnd();
					QMap<cint64, DummyValue>::const_iterator itMerged = mSameIndiMergedAssoMap.constBegin(), itMergedEnd = mSameIndiMergedAssoMap.constEnd();

					if (!visitBaseIndividuals) {
						itBase = itBaseEnd;
					}
					if (visitSameMergedIndividuals) {
						itMerged = itMergedEnd;
					}


					while (itBase != itBaseEnd || itMerged != itMergedEnd) {
						cint64 nextIndiId = -1;
						bool mergedIndi = false;
						if (itBase == itBaseEnd) {
							nextIndiId = itMerged.key();
							++itMerged;
							mergedIndi = true;
						} else if (itMerged == itMergedEnd) {
							nextIndiId = itBase.key();
							++itBase;
						} else {
							if (itBase.key() < itMerged.key()) {
								nextIndiId = itBase.key();
								++itBase;
							} else {
								nextIndiId = itMerged.key();
								++itMerged;
								mergedIndi = true;
							}
						}
						continueVisiting = visitFunc(nextIndiId, mergedIndi);
						visited = true;
					}

					return visited;
				}


				bool CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::visitIndividualIdAssociationsDescending(function<bool(cint64 indiId, bool sameIndividualMerged)> visitFunc, bool visitBaseIndividuals, bool visitSameMergedIndividuals) {
					bool visited = false;
					bool continueVisiting = true;

					QMapIterator<cint64, DummyValue> baseReverseIt(mBaseIndiAssoMap);
					QMapIterator<cint64, DummyValue> sameReverseIt(mSameIndiMergedAssoMap);

					baseReverseIt.toBack();
					sameReverseIt.toBack();

					if (visitBaseIndividuals) {
						baseReverseIt.toBack();
					}
					if (visitSameMergedIndividuals) {
						sameReverseIt.toBack();
					}

					while (baseReverseIt.hasPrevious() || sameReverseIt.hasPrevious()) {
						cint64 nextIndiId = -1;
						bool mergedIndi = false;
						if (!baseReverseIt.hasPrevious()) {
							nextIndiId = sameReverseIt.previous().key();
							mergedIndi = true;
						} else if (!sameReverseIt.hasPrevious()) {
							nextIndiId = baseReverseIt.previous().key();
						} else {
							if (baseReverseIt.peekPrevious().key() < sameReverseIt.peekPrevious().key()) {
								nextIndiId = sameReverseIt.previous().key();
								mergedIndi = true;
							} else {
								nextIndiId = baseReverseIt.previous().key();
							}
						}
						continueVisiting = visitFunc(nextIndiId, mergedIndi);
						visited = true;
					}

					return visited;
				}


				bool CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::visitIndividualIdAssociations(function<bool(cint64 indiId, bool sameIndividualMerged)> visitFunc, bool ascending, bool visitBaseIndividuals, bool visitSameMergedIndividuals) {
					if (ascending) {
						return visitIndividualIdAssociationsAscending(visitFunc, visitBaseIndividuals, visitSameMergedIndividuals);
					} else {
						return visitIndividualIdAssociationsDescending(visitFunc, visitBaseIndividuals, visitSameMergedIndividuals);
					}
				}


				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::getIterator(bool ascending, bool visitBaseIndividuals, bool visitSameMergedIndividuals) {
				
					QMap<cint64, DummyValue>::const_iterator baseIndiAssoMapItBegin = mBaseIndiAssoMap.constEnd();
					QMap<cint64, DummyValue>::const_iterator baseIndiAssoMapItEnd = mBaseIndiAssoMap.constEnd();
					if (visitBaseIndividuals && !mBaseIndiAssoMap.isEmpty()) {
						baseIndiAssoMapItBegin = mBaseIndiAssoMap.constBegin();
					}

					QMap<cint64, DummyValue>::const_iterator sameIndiMergedAssoMapItBegin = mSameIndiMergedAssoMap.constEnd();
					QMap<cint64, DummyValue>::const_iterator sameIndiMergedAssoMapItEnd = mSameIndiMergedAssoMap.constEnd();
					if (visitSameMergedIndividuals && !mSameIndiMergedAssoMap.isEmpty()) {
						sameIndiMergedAssoMapItBegin = mSameIndiMergedAssoMap.constBegin();
					}
					
					return CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator(baseIndiAssoMapItBegin, baseIndiAssoMapItEnd, sameIndiMergedAssoMapItBegin, sameIndiMergedAssoMapItEnd, ascending);
				}


				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::getIterator(cint64 cursorId, bool moveOverCursor, bool ascending, bool visitBaseIndividuals, bool visitSameMergedIndividuals) {

					QMap<cint64, DummyValue>::const_iterator baseIndiAssoMapItBegin = mBaseIndiAssoMap.constEnd();
					QMap<cint64, DummyValue>::const_iterator baseIndiAssoMapItEnd = mBaseIndiAssoMap.constEnd();
					if (visitBaseIndividuals && !mBaseIndiAssoMap.isEmpty()) {
						if (ascending) {
							if (moveOverCursor) {
								baseIndiAssoMapItBegin = mBaseIndiAssoMap.upperBound(cursorId);
							} else {
								baseIndiAssoMapItBegin = mBaseIndiAssoMap.lowerBound(cursorId);
							}
						} else {
							if (moveOverCursor) {
								baseIndiAssoMapItEnd = mBaseIndiAssoMap.lowerBound(cursorId);
							} else {
								baseIndiAssoMapItEnd = mBaseIndiAssoMap.upperBound(cursorId);
							}
						}
					}

					QMap<cint64, DummyValue>::const_iterator sameIndiMergedAssoMapItBegin = mSameIndiMergedAssoMap.constEnd();
					QMap<cint64, DummyValue>::const_iterator sameIndiMergedAssoMapItEnd = mSameIndiMergedAssoMap.constEnd();
					if (visitSameMergedIndividuals && !mSameIndiMergedAssoMap.isEmpty()) {
						if (ascending) {
							if (moveOverCursor) {
								sameIndiMergedAssoMapItBegin = mSameIndiMergedAssoMap.upperBound(cursorId);
							} else {
								sameIndiMergedAssoMapItBegin = mSameIndiMergedAssoMap.lowerBound(cursorId);
							}
						} else {
							if (moveOverCursor) {
								sameIndiMergedAssoMapItEnd = mSameIndiMergedAssoMap.lowerBound(cursorId);
							} else {
								sameIndiMergedAssoMapItEnd = mSameIndiMergedAssoMap.upperBound(cursorId);
							}
						}
					}

					return CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator(baseIndiAssoMapItBegin, baseIndiAssoMapItEnd, sameIndiMergedAssoMapItBegin, sameIndiMergedAssoMapItEnd, ascending);
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
