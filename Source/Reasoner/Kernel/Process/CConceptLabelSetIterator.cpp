/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CConceptLabelSetIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CConceptLabelSetIterator::CConceptLabelSetIterator(cint64 conceptCount, CConceptDescriptor* conceptDesLinker, const CPROCESSMAP<cint64,CConceptDescriptorDependencyPair>::iterator& beginIt, const CPROCESSMAP<cint64,CConceptDescriptorDependencyPair>::iterator& endIt) 
						: mConceptCount(conceptCount),mConceptDesLinkerIt(conceptDesLinker),mConDesDepBeginIt(beginIt),mConDesDepEndIt(endIt) {
				}



				cint64 CConceptLabelSetIterator::getRemainingConceptCount() {
					return mConceptCount;
				}


				CConceptLabelSetIterator& CConceptLabelSetIterator::operator++() {
					return *moveNext();
				}

				CConceptDescriptor* CConceptLabelSetIterator::operator*() {
					return getConceptDescriptor();
				}


				CConceptLabelSetIterator* CConceptLabelSetIterator::moveNext() {
					if (mConceptDesLinkerIt) {
						mConceptDesLinkerIt = mConceptDesLinkerIt->getNext();
						--mConceptCount;
					} else {
						if (mConDesDepBeginIt != mConDesDepEndIt) {
							++mConDesDepBeginIt;
							--mConceptCount;
						}
					}
					return this;
				}

				CConceptDescriptor* CConceptLabelSetIterator::getConceptDescriptor() {
					if (mConceptDesLinkerIt) {
						return mConceptDesLinkerIt;
					} else {
						return mConDesDepBeginIt.value().first;
					}
				}

				CDependencyTrackPoint* CConceptLabelSetIterator::getDependencyTrackPoint() {
					if (mConceptDesLinkerIt) {
						return nullptr;
					} else {
						return mConDesDepBeginIt.value().second;
					}
				}


				CConceptDescriptor* CConceptLabelSetIterator::next(bool moveToNext) {
					CConceptDescriptor* conDes = getConceptDescriptor();
					if (moveToNext) {
						moveNext();
					}
					return conDes;
				}

				bool CConceptLabelSetIterator::hasValue() {
					return mConceptDesLinkerIt || mConDesDepBeginIt != mConDesDepEndIt;
				}

				bool CConceptLabelSetIterator::hasNext() {
					return mConceptDesLinkerIt || mConDesDepBeginIt != mConDesDepEndIt;
				}

				bool CConceptLabelSetIterator::operator!=(CConceptLabelSetIterator& iterator) {
					return mConceptDesLinkerIt != iterator.mConceptDesLinkerIt || mConDesDepBeginIt != iterator.mConDesDepBeginIt;
				}

				bool CConceptLabelSetIterator::operator==(CConceptLabelSetIterator& iterator) {
					return mConceptDesLinkerIt == iterator.mConceptDesLinkerIt && mConDesDepBeginIt == iterator.mConDesDepBeginIt;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
