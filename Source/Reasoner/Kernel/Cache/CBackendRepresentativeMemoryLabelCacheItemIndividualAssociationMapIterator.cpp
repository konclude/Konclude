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

#include "CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator::CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator() {
					mHasCurrentIndiId = false;
				}

				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator::CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator(QMap<cint64, DummyValue>::const_iterator baseIndiAssoMapItBegin, QMap<cint64, DummyValue>::const_iterator baseIndiAssoMapItEnd,
							QMap<cint64, DummyValue>::const_iterator sameIndiMergedAssoMapItBegin, QMap<cint64, DummyValue>::const_iterator sameIndiMergedAssoMapItEnd, bool iterateAscending) {

					mBaseIndiAssoMapItBegin = baseIndiAssoMapItBegin;
					mBaseIndiAssoMapItEnd = baseIndiAssoMapItEnd;

					mSameIndiMergedAssoMapItBegin = sameIndiMergedAssoMapItBegin;
					mSameIndiMergedAssoMapItEnd = sameIndiMergedAssoMapItEnd;

					mIterateAscending = iterateAscending;

					if (mIterateAscending) {
						mBaseIndiAssoMapIt = baseIndiAssoMapItBegin;
						mSameIndiMergedAssoMapIt = mSameIndiMergedAssoMapItBegin;
					} else {
						mBaseIndiAssoMapIt = mBaseIndiAssoMapItEnd;
						if (mBaseIndiAssoMapItBegin != mBaseIndiAssoMapItEnd) {
							--mBaseIndiAssoMapIt;
						}
						mSameIndiMergedAssoMapIt = mSameIndiMergedAssoMapItEnd;
						if (mSameIndiMergedAssoMapItBegin != mBaseIndiAssoMapItEnd) {
							--mSameIndiMergedAssoMapIt;
						}
					}


					mHasCurrentIndiId = false;
					mCurrentIndiSameMerged = false;
					moveNext();
				}

				cint64 CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator::currentAssociatedIndividualId() {
					return mCurrentIndiId;
				}

				bool CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator::currentAssociatedIndividualSameMerged() {
					return mCurrentIndiSameMerged;
				}


				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator* CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator::moveNext() {
					mCurrentIndiId = -1;
					mHasCurrentIndiId = false;
					mCurrentIndiSameMerged = false;

					if (mIterateAscending) {
						if (mBaseIndiAssoMapIt != mBaseIndiAssoMapItEnd && mSameIndiMergedAssoMapIt != mSameIndiMergedAssoMapItEnd) {
							if (mBaseIndiAssoMapIt.key() < mSameIndiMergedAssoMapIt.key()) {
								mCurrentIndiId = mBaseIndiAssoMapIt.key();
								++mBaseIndiAssoMapIt;
								mHasCurrentIndiId = true;
							} else {
								mCurrentIndiId = mSameIndiMergedAssoMapIt.key();
								++mSameIndiMergedAssoMapIt;
								mHasCurrentIndiId = true;
								mCurrentIndiSameMerged = true;
							}
							
						} else if (mBaseIndiAssoMapIt != mBaseIndiAssoMapItEnd) {
							mCurrentIndiId = mBaseIndiAssoMapIt.key();
							++mBaseIndiAssoMapIt;
							mHasCurrentIndiId = true;
						} else if (mSameIndiMergedAssoMapIt != mSameIndiMergedAssoMapItEnd) {
							mCurrentIndiId = mSameIndiMergedAssoMapIt.key();
							++mSameIndiMergedAssoMapIt;
							mHasCurrentIndiId = true;
							mCurrentIndiSameMerged = true;
						}
					} else {
						if (mBaseIndiAssoMapIt != mBaseIndiAssoMapItEnd && mSameIndiMergedAssoMapIt != mSameIndiMergedAssoMapItEnd) {
							if (mBaseIndiAssoMapIt.key() > mSameIndiMergedAssoMapIt.key()) {
								mCurrentIndiId = mBaseIndiAssoMapIt.key();
								if (mBaseIndiAssoMapIt == mBaseIndiAssoMapItBegin) {
									mBaseIndiAssoMapIt = mBaseIndiAssoMapItEnd;
								} else {
									--mBaseIndiAssoMapIt;
								}
								mHasCurrentIndiId = true;
							} else {
								mCurrentIndiId = mSameIndiMergedAssoMapIt.key();
								if (mSameIndiMergedAssoMapIt == mSameIndiMergedAssoMapItBegin) {
									mSameIndiMergedAssoMapIt = mSameIndiMergedAssoMapItEnd;
								} else {
									--mSameIndiMergedAssoMapIt;
								}
								mHasCurrentIndiId = true;
								mCurrentIndiSameMerged = true;
							}
						} else if (mBaseIndiAssoMapIt != mBaseIndiAssoMapItEnd) {
							mCurrentIndiId = mBaseIndiAssoMapIt.key();
							if (mBaseIndiAssoMapIt == mBaseIndiAssoMapItBegin) {
								mBaseIndiAssoMapIt = mBaseIndiAssoMapItEnd;
							} else {
								--mBaseIndiAssoMapIt;
							}
							mHasCurrentIndiId = true;
						} else if (mSameIndiMergedAssoMapIt != mSameIndiMergedAssoMapItEnd) {
							mCurrentIndiId = mSameIndiMergedAssoMapIt.key();
							if (mSameIndiMergedAssoMapIt == mSameIndiMergedAssoMapItBegin) {
								mSameIndiMergedAssoMapIt = mSameIndiMergedAssoMapItEnd;
							} else {
								--mSameIndiMergedAssoMapIt;
							}
							mHasCurrentIndiId = true;
							mCurrentIndiSameMerged = true;
						}
					}
					return this;
				}

				bool CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator::atEnd() {
					return !mHasCurrentIndiId;
				}




			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
