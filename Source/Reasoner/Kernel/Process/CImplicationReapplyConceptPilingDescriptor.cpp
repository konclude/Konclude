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

#include "CImplicationReapplyConceptPilingDescriptor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CImplicationReapplyConceptPilingDescriptor::CImplicationReapplyConceptPilingDescriptor() : CLinkerBase<CImplicationReapplyConceptPilingDescriptor*,CImplicationReapplyConceptPilingDescriptor>(this) {
					mConceptPilDes = nullptr;
					mNextTriggerConcept = nullptr;
					mImplicationConPilDes = nullptr;
					mNextReapplyConPilDes = nullptr;
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					mPilingDepth = 0;
					mPrevMaxPilingDepth = 0;
#endif
					mCopyReapplyConPilDes = nullptr;
				}

				CImplicationReapplyConceptPilingDescriptor *CImplicationReapplyConceptPilingDescriptor::initImplicationReapllyConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes, CSortedNegLinker<CConcept*>* nextTriggerConcept, CConceptPilingDescriptor* implicationConPilDes, cint64 pilingDepth, cint64 maxPrevPilingDepth, CImplicationReapplyConceptPilingDescriptor* nextReapplyConPilDes) {
					mConceptPilDes = conPilDes;
					mNextTriggerConcept = nextTriggerConcept;
					mImplicationConPilDes = implicationConPilDes;
					mNextReapplyConPilDes = nextReapplyConPilDes;
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					mPilingDepth = pilingDepth;
					mPrevMaxPilingDepth = maxPrevPilingDepth;
#endif
					mCopyReapplyConPilDes = nullptr;
					return this;
				}


				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::copyImplicationReapllyConceptPilingDescriptor(CImplicationReapplyConceptPilingDescriptor* impReapplyConPilDes) {
					mConceptPilDes = impReapplyConPilDes->mConceptPilDes;
					mNextTriggerConcept = impReapplyConPilDes->mNextTriggerConcept;
					mImplicationConPilDes = impReapplyConPilDes->mImplicationConPilDes;
					mNextReapplyConPilDes = nullptr;
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					mPilingDepth = 0;
					mPrevMaxPilingDepth = 0;
#endif
					mCopyReapplyConPilDes = nullptr;
					return this;
				}


				cint64 CImplicationReapplyConceptPilingDescriptor::getPilingDepth() {
#ifdef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					return 0;
#else
					return mPilingDepth;
#endif
				}

				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::setUpdatedPilingDepth(cint64 pilingDepth) {
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					mPilingDepth = pilingDepth;
#endif
					return this;
				}

				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::setPilingDepth(cint64 pilingDepth) {
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					mPilingDepth = pilingDepth;
#endif
					return this;
				}

				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::setUpdatedPreviousMaxPilingDepth(cint64 prevMaxPilingDepth) {
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					mPrevMaxPilingDepth = prevMaxPilingDepth;
#endif
					return this;
				}

				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::setPreviousMaxPilingDepth(cint64 prevMaxPilingDepth) {
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					mPrevMaxPilingDepth = prevMaxPilingDepth;
#endif
					return this;
				}

				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::setConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes) {
					mConceptPilDes = conPilDes;
					return this;
				}

				cint64 CImplicationReapplyConceptPilingDescriptor::getPreviousMaxPilingDepth() {
#ifdef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					return 0;
#else
					return mPrevMaxPilingDepth;
#endif
				}

				CConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::getConceptPilingDescriptor() {
					return mConceptPilDes;
				}

				CConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::getImplicationConceptPilingDescriptor() {
					return mImplicationConPilDes;
				}

				CSortedNegLinker<CConcept*>* CImplicationReapplyConceptPilingDescriptor::getNextTriggerConcept() {
					return mNextTriggerConcept;
				}

				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::getNextReapplyConceptPilingDescriptor() {
					return mNextReapplyConPilDes;
				}

				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::getCopyReapplyConceptPilingDescriptor() {
					return mCopyReapplyConPilDes;
				}

				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::setCopyReapplyConceptPilingDescriptor(CImplicationReapplyConceptPilingDescriptor* reapplyConPilDes) {
					mCopyReapplyConPilDes = reapplyConPilDes;
					return this;
				}

				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::setNextReapplyConceptPilingDescriptor(CImplicationReapplyConceptPilingDescriptor* reapplyConPilDes) {
					mNextReapplyConPilDes = reapplyConPilDes;
					return this;
				}

				CImplicationReapplyConceptPilingDescriptor* CImplicationReapplyConceptPilingDescriptor::clearCopyReapplyConceptPilingDescriptor() {
					mCopyReapplyConPilDes = nullptr;
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
