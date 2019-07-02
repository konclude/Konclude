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

#include "CReapplyConceptSaturationLabelSetIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CReapplyConceptSaturationLabelSetIterator::CReapplyConceptSaturationLabelSetIterator(const CPROCESSHASH<cint64,CConceptSaturationDescriptorReapplyData>::const_iterator& itBegin, const CPROCESSHASH<cint64,CConceptSaturationDescriptorReapplyData>::const_iterator& itEnd, bool iterateConSatDes, bool iterateReapplies) 
						: mItBegin(itBegin), mItEnd(itEnd), mIterateConSatDes(iterateConSatDes), mIterateReapplies(iterateReapplies) {
					
					while (!isIteratorValid()) {
						++mItBegin;
					}
				}

				bool CReapplyConceptSaturationLabelSetIterator::isIteratorValid() {
					return isIteratorValid(mIterateConSatDes,mIterateReapplies);
				}

				bool CReapplyConceptSaturationLabelSetIterator::isIteratorValid(bool allowConSatDes, bool allowReapplies) {
					if (mItBegin == mItEnd) {
						return true;
					}
					const CConceptSaturationDescriptorReapplyData& data = mItBegin.value();
					if (data.mConSatDes && allowConSatDes) {
						return true;
					}
					if (data.mImpReapplyConSatDes && allowReapplies) {
						return true;
					}
					return false;
				}



				cint64 CReapplyConceptSaturationLabelSetIterator::getDataTag() {
					cint64 dataTag = mItBegin.key();
					return dataTag;
					//CConceptSaturationDescriptor* conDes = mItBegin.value().mConSatDes;
					//if (conDes) {
					//	return conDes->getConceptTag();
					//} else {
					//	return 0;
					//}
				}

				CConceptSaturationDescriptor* CReapplyConceptSaturationLabelSetIterator::getConceptSaturationDescriptor() {
					return mItBegin.value().mConSatDes;
				}

				CImplicationReapplyConceptSaturationDescriptor* CReapplyConceptSaturationLabelSetIterator::getImplicationReapplyConceptSaturationDescriptor() {
					return mItBegin.value().mImpReapplyConSatDes;
				}


				bool CReapplyConceptSaturationLabelSetIterator::hasNext() {
					return mItBegin != mItEnd;
				}

				cint64 CReapplyConceptSaturationLabelSetIterator::next(bool moveToNext) {
					cint64 tag = getDataTag();
					if (moveToNext) {
						++mItBegin;
						while (!isIteratorValid()) {
							++mItBegin;
						}
					}
					return tag;
				}

				CReapplyConceptSaturationLabelSetIterator* CReapplyConceptSaturationLabelSetIterator::moveNext() {
					++mItBegin;
					while (!isIteratorValid()) {
						++mItBegin;
					}
					return this;
				}

				bool CReapplyConceptSaturationLabelSetIterator::operator!=(CReapplyConceptSaturationLabelSetIterator& const_iterator) {
					return mItBegin != const_iterator.mItBegin;
				}

				bool CReapplyConceptSaturationLabelSetIterator::operator==(CReapplyConceptSaturationLabelSetIterator& const_iterator) {
					return mItBegin == const_iterator.mItBegin;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
