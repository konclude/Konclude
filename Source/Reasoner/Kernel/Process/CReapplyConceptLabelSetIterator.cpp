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

#include "CReapplyConceptLabelSetIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CReapplyConceptLabelSetIterator::CReapplyConceptLabelSetIterator(cint64 conceptCount, CConceptDescriptor* conceptDesLinker, const CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator& beginIt, const CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator& endIt, const CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator& addBeginIt, const CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator& addEndIt, bool skipEmptyConceptDescriptors) 
						: mConceptCount(conceptCount),mConceptDesLinkerIt(conceptDesLinker),mConDesDepBeginIt(beginIt),mConDesDepEndIt(endIt),mSkipEmptyConceptDescriptors(skipEmptyConceptDescriptors),mAdditionalConDesDepBeginIt(addBeginIt),mAdditionalConDesDepEndIt(addEndIt) {

					if (mSkipEmptyConceptDescriptors) {
						while (mConDesDepBeginIt != mConDesDepEndIt && mConDesDepBeginIt.value().mConceptDescriptor == nullptr) {
							++mConDesDepBeginIt;
						}
						while (mAdditionalConDesDepBeginIt != mAdditionalConDesDepEndIt && mAdditionalConDesDepBeginIt.value().mConceptDescriptor == nullptr) {							
							++mAdditionalConDesDepBeginIt;
						}
					}
				}



				cint64 CReapplyConceptLabelSetIterator::getRemainingConceptCount() {
					return mConceptCount;
				}


				CReapplyConceptLabelSetIterator& CReapplyConceptLabelSetIterator::operator++() {
					return *moveNext();
				}

				CConceptDescriptor* CReapplyConceptLabelSetIterator::operator*() {
					return getConceptDescriptor();
				}


				CReapplyConceptLabelSetIterator* CReapplyConceptLabelSetIterator::moveNext() {
					if (mConceptDesLinkerIt) {
						mConceptDesLinkerIt = mConceptDesLinkerIt->getNext();
						--mConceptCount;
					} else {
						if (mConDesDepBeginIt != mConDesDepEndIt || mAdditionalConDesDepBeginIt != mAdditionalConDesDepEndIt) {
							if (mConDesDepBeginIt != mConDesDepEndIt && mAdditionalConDesDepBeginIt != mAdditionalConDesDepEndIt) {
								if (mAdditionalConDesDepBeginIt.key() == mConDesDepBeginIt.key()) {
									++mAdditionalConDesDepBeginIt;
									++mConDesDepBeginIt;
								} else if (mAdditionalConDesDepBeginIt.key() < mConDesDepBeginIt.key()) { 
									++mAdditionalConDesDepBeginIt;
								} else {
									++mConDesDepBeginIt;
								}
							} else if (mConDesDepBeginIt == mConDesDepEndIt) {
								++mAdditionalConDesDepBeginIt;
							} else {
								++mConDesDepBeginIt;
							}
							if (mSkipEmptyConceptDescriptors) {
								while (mConDesDepBeginIt != mConDesDepEndIt && mConDesDepBeginIt.value().mConceptDescriptor == nullptr) {
									++mConDesDepBeginIt;
								}
								while (mAdditionalConDesDepBeginIt != mAdditionalConDesDepEndIt && mAdditionalConDesDepBeginIt.value().mConceptDescriptor == nullptr) {							
									++mAdditionalConDesDepBeginIt;
								}
							}
							--mConceptCount;
						}
					}
					return this;
				}

				cint64 CReapplyConceptLabelSetIterator::getDataTag() {
					if (mConceptDesLinkerIt) {
						return mConceptDesLinkerIt->getConceptTag();
					} else {
						if (mConDesDepBeginIt != mConDesDepEndIt && mAdditionalConDesDepBeginIt != mAdditionalConDesDepEndIt) {
							if (mAdditionalConDesDepBeginIt.key() < mConDesDepBeginIt.key()) {
								return mAdditionalConDesDepBeginIt.key();
							} else {
								return mConDesDepBeginIt.key();
							}
						} else if (mConDesDepBeginIt == mConDesDepEndIt) {
							return mAdditionalConDesDepBeginIt.key();
						} else {
							return mConDesDepBeginIt.key();
						}
					}
				}


				CConceptDescriptor* CReapplyConceptLabelSetIterator::getConceptDescriptor() {
					if (mConceptDesLinkerIt) {
						return mConceptDesLinkerIt;
					} else {
						if (mConDesDepBeginIt != mConDesDepEndIt && mAdditionalConDesDepBeginIt != mAdditionalConDesDepEndIt) {
							if (mAdditionalConDesDepBeginIt.key() < mConDesDepBeginIt.key()) {
								return mAdditionalConDesDepBeginIt.value().mConceptDescriptor;
							} else {
								return mConDesDepBeginIt.value().mConceptDescriptor;
							}
						} else if (mConDesDepBeginIt == mConDesDepEndIt) {
							return mAdditionalConDesDepBeginIt.value().mConceptDescriptor;
						} else {
							return mConDesDepBeginIt.value().mConceptDescriptor;
						}
					}
				}

				CDependencyTrackPoint* CReapplyConceptLabelSetIterator::getDependencyTrackPoint() {
					if (mConceptDesLinkerIt) {
						return mConceptDesLinkerIt->getDependencyTrackPoint();
					} else {
						CConceptDescriptor* conDes = getConceptDescriptor();
						if (conDes) {
							return conDes->getDependencyTrackPoint();
						} else {
							return nullptr;
						}
					}
				}

				const CCondensedReapplyQueue *CReapplyConceptLabelSetIterator::getPosNegReapplyQueue() {
					if (mConceptDesLinkerIt) {
						return nullptr;
					} else {
						if (mConDesDepBeginIt != mConDesDepEndIt && mAdditionalConDesDepBeginIt != mAdditionalConDesDepEndIt) {
							if (mAdditionalConDesDepBeginIt.key() < mConDesDepBeginIt.key()) {
								return &mAdditionalConDesDepBeginIt.value().mPosNegReapplyQueue;
							} else {
								return &mConDesDepBeginIt.value().mPosNegReapplyQueue;
							}
						} else if (mConDesDepBeginIt == mConDesDepEndIt) {
							return &mAdditionalConDesDepBeginIt.value().mPosNegReapplyQueue;
						} else {
							return &mConDesDepBeginIt.value().mPosNegReapplyQueue;
						}
					}
				}


				CConceptDescriptor* CReapplyConceptLabelSetIterator::next(bool moveToNext) {
					CConceptDescriptor* conDes = getConceptDescriptor();
					if (moveToNext) {
						moveNext();
					}
					return conDes;
				}

				bool CReapplyConceptLabelSetIterator::hasValue() {
					return mConceptDesLinkerIt || mConDesDepBeginIt != mConDesDepEndIt || mAdditionalConDesDepBeginIt != mAdditionalConDesDepEndIt;
				}

				bool CReapplyConceptLabelSetIterator::hasNext() {
					return mConceptDesLinkerIt || mConDesDepBeginIt != mConDesDepEndIt || mAdditionalConDesDepBeginIt != mAdditionalConDesDepEndIt;
				}

				bool CReapplyConceptLabelSetIterator::operator!=(CReapplyConceptLabelSetIterator& iterator) {
					return mConceptDesLinkerIt != iterator.mConceptDesLinkerIt || mConDesDepBeginIt != iterator.mConDesDepBeginIt || mAdditionalConDesDepBeginIt != iterator.mAdditionalConDesDepEndIt;
				}

				bool CReapplyConceptLabelSetIterator::operator==(CReapplyConceptLabelSetIterator& iterator) {
					return mConceptDesLinkerIt == iterator.mConceptDesLinkerIt && mConDesDepBeginIt == iterator.mConDesDepBeginIt && mAdditionalConDesDepBeginIt != iterator.mAdditionalConDesDepEndIt;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
