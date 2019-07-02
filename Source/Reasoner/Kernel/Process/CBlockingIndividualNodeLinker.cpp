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

#include "CBlockingIndividualNodeLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBlockingIndividualNodeLinker::CBlockingIndividualNodeLinker() : CLinkerBase<CIndividualProcessNode*,CBlockingIndividualNodeLinker>(nullptr) {
				}

				CBlockingIndividualNodeLinker* CBlockingIndividualNodeLinker::initBlockingIndividualNodeLinker(CBlockingIndividualNodeLinker* blockIndNodeLinker) {
					if (blockIndNodeLinker) {
						mLastFailedSubsetConDes = blockIndNodeLinker->mLastFailedSubsetConDes;
						setData(blockIndNodeLinker->getData());
					} else {					
						mLastFailedSubsetConDes = nullptr;
						setData(nullptr);
					}
					return this;
				}


				CBlockingIndividualNodeLinker* CBlockingIndividualNodeLinker::initBlockingIndividualNodeLinker(CIndividualProcessNode* indiNode) {
					mLastFailedSubsetConDes = nullptr;
					setData(indiNode);
					return this;
				}


				CConceptDescriptor* CBlockingIndividualNodeLinker::getLastFailedSubsetConceptDescriptor() {
					return mLastFailedSubsetConDes;
				}

				CBlockingIndividualNodeLinker* CBlockingIndividualNodeLinker::setLastFailedSubsetConceptDescriptor(CConceptDescriptor* conDes) {
					mLastFailedSubsetConDes = conDes;
					return this;
				}

				CIndividualProcessNode* CBlockingIndividualNodeLinker::getCandidateIndividualNode() {
					return getData();
				}

				CBlockingIndividualNodeLinker* CBlockingIndividualNodeLinker::setCandidateIndividualNode(CIndividualProcessNode* indiNode) {
					setData(indiNode);
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
