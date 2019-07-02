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

#include "CSaturationDisjunctCommonConceptCountHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationDisjunctCommonConceptCountHash::CSaturationDisjunctCommonConceptCountHash(CProcessContext* processContext) : mCommonConceptCountHash(processContext) {
				}

				CSaturationDisjunctCommonConceptCountHash* CSaturationDisjunctCommonConceptCountHash::initCommonConceptCountHash() {
					mCommonConceptCountHash.clear();
					mDisjunctCount = 0;
					return this;
				}

				CPROCESSHASH<cint64,CSaturationDisjunctCommonConceptCountHashData>* CSaturationDisjunctCommonConceptCountHash::getCommonConceptCountHash() {
					return &mCommonConceptCountHash;
				}

				CSaturationDisjunctCommonConceptCountHash* CSaturationDisjunctCommonConceptCountHash::setDisjunctCount(cint64 disjunctCount) {
					mDisjunctCount = disjunctCount;
					return this;
				}

				CSaturationDisjunctCommonConceptCountHashData& CSaturationDisjunctCommonConceptCountHash::getCommonConceptCountData(cint64 conceptTag) {
					return mCommonConceptCountHash[conceptTag];
				}

				CSaturationDisjunctCommonConceptCountHashData& CSaturationDisjunctCommonConceptCountHash::getCommonConceptCountData(CConcept* concept) {
					return mCommonConceptCountHash[concept->getConceptTag()];
				}

				CSaturationDisjunctCommonConceptCountHashData& CSaturationDisjunctCommonConceptCountHash::getCommonConceptCountData(CConceptSaturationDescriptor* conSatDes) {
					return mCommonConceptCountHash[conSatDes->getConceptTag()];
				}


				CSaturationDisjunctCommonConceptCountHash* CSaturationDisjunctCommonConceptCountHash::removeCommonConceptData(CConceptSaturationDescriptor* conSatDes) {
					mCommonConceptCountHash.remove(conSatDes->getConceptTag());
					return this;
				}

				bool CSaturationDisjunctCommonConceptCountHash::incCommonConceptCountReturnMaxReached(CConceptSaturationDescriptor* conSatDes) {
					bool maxReached = false;
					CSaturationDisjunctCommonConceptCountHashData& hashData = mCommonConceptCountHash[conSatDes->getConceptTag()];
					if (!hashData.mConcept) {
						hashData.mConcept = conSatDes->getConcept();
						hashData.mNegation = conSatDes->isNegated();
					}
					if (hashData.mNegation != conSatDes->isNegated()) {
						return false;
					}
					hashData.mConceptCount++;
					if (hashData.mConceptCount >= mDisjunctCount) {
						return true;
					}
					return false;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
