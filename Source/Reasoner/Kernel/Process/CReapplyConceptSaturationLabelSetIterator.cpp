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

#include "CReapplyConceptSaturationLabelSetIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CReapplyConceptSaturationLabelSetIterator::CReapplyConceptSaturationLabelSetIterator(const CPROCESSHASH<cint64,CConceptSaturationDescriptorReapplyData>::const_iterator& itBegin, const CPROCESSHASH<cint64,CConceptSaturationDescriptorReapplyData>::const_iterator& itEnd, const CPROCESSHASH<cint64,CConceptSaturationDescriptorReapplyData>::const_iterator& itAdditionalBegin, const CPROCESSHASH<cint64,CConceptSaturationDescriptorReapplyData>::const_iterator& itAdditionalEnd, bool iterateConSatDes, bool iterateReapplies) 
						: mItBegin(itBegin), mItEnd(itEnd), mItAdditionalBegin(itAdditionalBegin), mItAdditionalEnd(itAdditionalEnd), mIterateConSatDes(iterateConSatDes), mIterateReapplies(iterateReapplies) {
					
					while (!isIteratorValid()) {
						++mItBegin;
					}
				}

				bool CReapplyConceptSaturationLabelSetIterator::isIteratorValid() {
					return isIteratorValid(mIterateConSatDes,mIterateReapplies);
				}

				bool CReapplyConceptSaturationLabelSetIterator::isIteratorValid(bool allowConSatDes, bool allowReapplies) {
					if (mItBegin == mItEnd) {
						if (mItAdditionalBegin == mItAdditionalEnd) {
							return true;
						}
					}
					if (mItBegin != mItEnd) {
						const CConceptSaturationDescriptorReapplyData& data = mItBegin.value();
						if (data.mConSatDes && allowConSatDes) {
							return true;
						}
						if (data.mImpReapplyConSatDes && allowReapplies) {
							return true;
						}
					} else {
						const CConceptSaturationDescriptorReapplyData& data = mItAdditionalBegin.value();
						if (data.mConSatDes && allowConSatDes) {
							return true;
						}
						if (data.mImpReapplyConSatDes && allowReapplies) {
							return true;
						}
					}
					return false;
				}



				cint64 CReapplyConceptSaturationLabelSetIterator::getDataTag() {
					if (mItBegin != mItEnd) {
						cint64 dataTag = mItBegin.key();
						return dataTag;
					} else {
						cint64 dataTag = mItAdditionalBegin.key();
						return dataTag;
					}
					//CConceptSaturationDescriptor* conDes = mItBegin.value().mConSatDes;
					//if (conDes) {
					//	return conDes->getConceptTag();
					//} else {
					//	return 0;
					//}
				}

				CConceptSaturationDescriptor* CReapplyConceptSaturationLabelSetIterator::getConceptSaturationDescriptor() {
					if (mItBegin != mItEnd) {
						return mItBegin.value().mConSatDes;
					} else {
						return mItAdditionalBegin.value().mConSatDes;
					}
				}

				CImplicationReapplyConceptSaturationDescriptor* CReapplyConceptSaturationLabelSetIterator::getImplicationReapplyConceptSaturationDescriptor() {
					if (mItBegin != mItEnd) {
						return mItBegin.value().mImpReapplyConSatDes;
					} else {
						return mItAdditionalBegin.value().mImpReapplyConSatDes;
					}
				}


				bool CReapplyConceptSaturationLabelSetIterator::hasNext() {
					if (mItBegin != mItEnd) {
						return true;
					} else {
						return mItAdditionalBegin != mItAdditionalEnd;
					}
				}

				cint64 CReapplyConceptSaturationLabelSetIterator::next(bool moveToNext) {
					cint64 tag = getDataTag();
					if (moveToNext) {
						if (mItBegin != mItEnd) {
							++mItBegin;
						} else {
							++mItAdditionalBegin;
						}
						while (!isIteratorValid()) {
							if (mItBegin != mItEnd) {
								++mItBegin;
							} else {
								++mItAdditionalBegin;
							}
						}
					}
					return tag;
				}

				CReapplyConceptSaturationLabelSetIterator* CReapplyConceptSaturationLabelSetIterator::moveNext() {
					if (mItBegin != mItEnd) {
						++mItBegin;
					} else {
						++mItAdditionalBegin;
					}
					while (!isIteratorValid()) {
						if (mItBegin != mItEnd) {
							++mItBegin;
						} else {
							++mItAdditionalBegin;
						}
					}
					return this;
				}

				bool CReapplyConceptSaturationLabelSetIterator::operator!=(CReapplyConceptSaturationLabelSetIterator& const_iterator) {
					return mItBegin != const_iterator.mItBegin && mItAdditionalBegin != const_iterator.mItAdditionalBegin;
				}

				bool CReapplyConceptSaturationLabelSetIterator::operator==(CReapplyConceptSaturationLabelSetIterator& const_iterator) {
					return mItBegin == const_iterator.mItBegin && mItAdditionalBegin == const_iterator.mItAdditionalBegin;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
