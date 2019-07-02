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

#include "CIndividualMergingHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualMergingHash::CIndividualMergingHash(CProcessContext* processContext) : CPROCESSHASH<cint64, CIndividualMergingHashData>(processContext) {
				}



				CIndividualMergingHash* CIndividualMergingHash::initIndividualMergingHash(CIndividualMergingHash* indiMergingHash) {
					if (indiMergingHash) {
						*this = *indiMergingHash;
						mMergedIndividualLinker = indiMergingHash->mMergedIndividualLinker;
						mMergedIndividualCount = indiMergingHash->mMergedIndividualCount;
					} else {
						this->clear();
						mMergedIndividualLinker = nullptr;
						mMergedIndividualCount = 0;
					}
					return this;
				}


				CXLinker<cint64>* CIndividualMergingHash::getMergedIndividualLinker() {
					return mMergedIndividualLinker;
				}

				CIndividualMergingHash* CIndividualMergingHash::addMergedIndividualLinker(CXLinker<cint64>* linker) {
					mMergedIndividualCount += linker->getCount();
					mMergedIndividualLinker = linker->append(mMergedIndividualLinker);
					return this;
				}


				cint64 CIndividualMergingHash::getMergedIndividualCount() {
					return mMergedIndividualCount;
				}

				bool CIndividualMergingHash::hasMergedIndividual(cint64 individualId) {
					return value(individualId).isMergedWithIndividual();
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
