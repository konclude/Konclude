/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CReapplyConceptLabelSetIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CReapplyConceptLabelSetIterator::CReapplyConceptLabelSetIterator(cint64 conceptCount, CConceptDescriptor* conceptDesLinker, const CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator& beginIt, const CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator& endIt, bool skipEmptyConceptDescriptors) 
						: mConceptCount(conceptCount),mConceptDesLinkerIt(conceptDesLinker),mConDesDepBeginIt(beginIt),mConDesDepEndIt(endIt),mSkipEmptyConceptDescriptors(skipEmptyConceptDescriptors) {

					if (mSkipEmptyConceptDescriptors) {
						while (mConDesDepBeginIt != mConDesDepEndIt && mConDesDepBeginIt.value().mConceptDescriptor == nullptr) {
							++mConDesDepBeginIt;
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
						if (mConDesDepBeginIt != mConDesDepEndIt) {
							++mConDesDepBeginIt;
							if (mSkipEmptyConceptDescriptors) {
								while (mConDesDepBeginIt != mConDesDepEndIt && mConDesDepBeginIt.value().mConceptDescriptor == nullptr) {
									++mConDesDepBeginIt;
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
						return mConDesDepBeginIt.key();
					}
				}


				CConceptDescriptor* CReapplyConceptLabelSetIterator::getConceptDescriptor() {
					if (mConceptDesLinkerIt) {
						return mConceptDesLinkerIt;
					} else {
						return mConDesDepBeginIt.value().mConceptDescriptor;
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
						return &mConDesDepBeginIt.value().mPosNegReapplyQueue;
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
					return mConceptDesLinkerIt || mConDesDepBeginIt != mConDesDepEndIt;
				}

				bool CReapplyConceptLabelSetIterator::hasNext() {
					return mConceptDesLinkerIt || mConDesDepBeginIt != mConDesDepEndIt;
				}

				bool CReapplyConceptLabelSetIterator::operator!=(CReapplyConceptLabelSetIterator& iterator) {
					return mConceptDesLinkerIt != iterator.mConceptDesLinkerIt || mConDesDepBeginIt != iterator.mConDesDepBeginIt;
				}

				bool CReapplyConceptLabelSetIterator::operator==(CReapplyConceptLabelSetIterator& iterator) {
					return mConceptDesLinkerIt == iterator.mConceptDesLinkerIt && mConDesDepBeginIt == iterator.mConDesDepBeginIt;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
