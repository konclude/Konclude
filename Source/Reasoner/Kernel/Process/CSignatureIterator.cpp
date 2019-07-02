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

#include "CSignatureIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSignatureIterator::CSignatureIterator(CPROCESSHASH<cint64,CSignatureBlockingCandidateData>* sigHash)
						: mSigHash(sigHash) {

					mItBegin = mSigHash->constBegin();
					mItEnd = mSigHash->constEnd();
				}


				bool CSignatureIterator::hasNext() {
					return mItBegin != mItEnd;
				}

				bool CSignatureIterator::moveNext() {
					if (mItBegin != mItEnd) {
						++mItBegin;
						return true;
					}
					return false;
				}

				cint64 CSignatureIterator::next(bool moveNext) {
					cint64 signature = -1;
					if (mItBegin != mItEnd) {
						signature = mItBegin.key();
						if (moveNext) {
							++mItBegin;
						}
					}
					return signature;
				}

				cint64 CSignatureIterator::getSignature() {
					cint64 signature = -1;
					if (mItBegin != mItEnd) {
						signature = mItBegin.key();
					}
					return signature;
				}

				CXLinker<cint64>* CSignatureIterator::getCandidateLinker() {
					CXLinker<cint64>* candidateLinker = nullptr;
					if (mItBegin != mItEnd) {
						CSignatureBlockingCandidateData data = mItBegin.value();
						candidateLinker = data.getCandidateIndividualLinker();
					}
					return candidateLinker;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
