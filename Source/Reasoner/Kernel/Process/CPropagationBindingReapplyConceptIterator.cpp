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

#include "CPropagationBindingReapplyConceptIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CPropagationBindingReapplyConceptIterator::CPropagationBindingReapplyConceptIterator(const CPROCESSHASH<TIndividualConceptPair,CPropagationBindingReapplyConceptHashData>::iterator& itBegin, const CPROCESSHASH<TIndividualConceptPair,CPropagationBindingReapplyConceptHashData>::iterator& itEnd) {
					mItBegin = itBegin;
					mItEnd = itEnd;
				}

				CPropagationBindingReapplyConceptDescriptor* CPropagationBindingReapplyConceptIterator::nextReapplyDescriptor(bool moveNext) {
					CPropagationBindingReapplyConceptDescriptor* reapplyConDes = nullptr;
					if (mItBegin != mItEnd) {
						CPropagationBindingReapplyConceptHashData& data = mItBegin.value();
						reapplyConDes = data.getPropagationBindingReapplyConceptDescriptor();
						if (moveNext) {
							if (mItBegin != mItEnd) {
								++mItBegin;
							}
						}
					}
					return reapplyConDes;
				}

				CPropagationBindingReapplyConceptIterator* CPropagationBindingReapplyConceptIterator::clearReapplyDescriptor() {
					if (mItBegin != mItEnd) {
						CPropagationBindingReapplyConceptHashData& data = mItBegin.value();
						data.clearPropagationBindingReapplyConceptDescriptor();
					}
					return this;
				}

				bool CPropagationBindingReapplyConceptIterator::moveNext() {
					if (mItBegin != mItEnd) {
						++mItBegin;
						return true;
					}
					return false;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
