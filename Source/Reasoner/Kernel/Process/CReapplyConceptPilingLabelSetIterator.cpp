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

#include "CReapplyConceptPilingLabelSetIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CReapplyConceptPilingLabelSetIterator::CReapplyConceptPilingLabelSetIterator(const CPROCESSHASH<cint64,CConceptPilingDescriptorReapplyData>::iterator& itBegin, const CPROCESSHASH<cint64,CConceptPilingDescriptorReapplyData>::iterator& itEnd, bool iterateConPilDes, bool iterateReapplies, cint64 maxPilingDepthIterate) 
						: mItBegin(itBegin), mItEnd(itEnd), mIterateConPilDes(iterateConPilDes), mIterateReapplies(iterateReapplies), mMaxPilingDepthIterate(maxPilingDepthIterate) {
					
					while (!isIteratorValid()) {
						++mItBegin;
					}
				}

				bool CReapplyConceptPilingLabelSetIterator::isIteratorValid() {
					return isIteratorValid(mIterateConPilDes,mIterateReapplies,mMaxPilingDepthIterate);
				}

				bool CReapplyConceptPilingLabelSetIterator::isIteratorValid(bool allowConPilDes, bool allowReapplies, cint64 maxPilDepth) {
					if (mItBegin == mItEnd) {
						return true;
					}
					CConceptPilingDescriptorReapplyData& data = mItBegin.value();
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					if (data.mPilingDepth > maxPilDepth) {
						return false;
					}
#endif
					if (data.mConPilDes && allowConPilDes) {
						return true;
					}
					if (data.mImpReapplyConPilDes && allowReapplies) {
						return true;
					}
					return false;
				}



				cint64 CReapplyConceptPilingLabelSetIterator::getDataTag() {
#ifdef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					return 0;
#else
					return mItBegin.value().mPilingDepth;
#endif
				}

				CConceptPilingDescriptor* CReapplyConceptPilingLabelSetIterator::getConceptPilingDescriptor() {
					return mItBegin.value().mConPilDes;
				}

				CImplicationReapplyConceptPilingDescriptor* CReapplyConceptPilingLabelSetIterator::getImplicationReapplyConceptPilingDescriptor() {
					return mItBegin.value().mImpReapplyConPilDes;
				}

				cint64 CReapplyConceptPilingLabelSetIterator::getDataPilingDepth() {
#ifdef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					return 0;
#else
					return mItBegin.value().mPilingDepth;
#endif
				}


				bool CReapplyConceptPilingLabelSetIterator::hasNext() {
					return mItBegin != mItEnd;
				}

				cint64 CReapplyConceptPilingLabelSetIterator::next(bool moveToNext) {
					cint64 tag = getDataTag();
					if (moveToNext) {
						++mItBegin;
						while (!isIteratorValid()) {
							++mItBegin;
						}
					}
					return tag;
				}

				CReapplyConceptPilingLabelSetIterator* CReapplyConceptPilingLabelSetIterator::moveNext() {
					++mItBegin;
					while (!isIteratorValid()) {
						++mItBegin;
					}
					return this;
				}

				bool CReapplyConceptPilingLabelSetIterator::operator!=(CReapplyConceptPilingLabelSetIterator& iterator) {
					return mItBegin != iterator.mItBegin;
				}

				bool CReapplyConceptPilingLabelSetIterator::operator==(CReapplyConceptPilingLabelSetIterator& iterator) {
					return mItBegin == iterator.mItBegin;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
