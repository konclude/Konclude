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

#include "CBackwardPilingPropagationReapplyDescriptor.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBackwardPilingPropagationReapplyDescriptor::CBackwardPilingPropagationReapplyDescriptor() : CLinkerBase<CBackwardPilingPropagationReapplyDescriptor*,CBackwardPilingPropagationReapplyDescriptor>(this) {
					mReapplyConDes = nullptr;
				}


				CBackwardPilingPropagationReapplyDescriptor* CBackwardPilingPropagationReapplyDescriptor::initBackwardPropagationReapplyDescriptor(CConceptPilingDescriptor* conDes, cint64 pilingDepth) {
					initPilingDepth(pilingDepth);
					mReapplyConDes = conDes;
					return this;
				}

				CBackwardPilingPropagationReapplyDescriptor* CBackwardPilingPropagationReapplyDescriptor::initBackwardPropagationReapplyDescriptor(CBackwardPilingPropagationReapplyDescriptor* reapplyDes) {
					initPilingDepth(reapplyDes->getPilingDepth());
					mReapplyConDes = reapplyDes->getReapplyConceptPilingDescriptor();
					return this;
				}

				CConceptPilingDescriptor* CBackwardPilingPropagationReapplyDescriptor::getReapplyConceptPilingDescriptor() {
					return mReapplyConDes;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
