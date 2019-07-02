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

#include "CReusingIndividualNodeConceptExpansionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CReusingIndividualNodeConceptExpansionData::CReusingIndividualNodeConceptExpansionData() {
				}


				CReusingIndividualNodeConceptExpansionData* CReusingIndividualNodeConceptExpansionData::initReusingExpansionData(CReusingIndividualNodeConceptExpansionData* prevData) {
					if (prevData) {
						initBlockingExpansionData(prevData);
						mReusingTriedCount = prevData->mReusingTriedCount;
						mReusingFailedCount = prevData->mReusingFailedCount;
						mReuseConceptsDependencyTrackPoint = prevData->mReuseConceptsDependencyTrackPoint;
						mLastNonDetExpansionLinker = prevData->mLastNonDetExpansionLinker;
					} else {
						initBlockingExpansionData(nullptr);
						mReusingTriedCount = 0;
						mReusingFailedCount = 0;
						mReuseConceptsDependencyTrackPoint = nullptr;
						mLastNonDetExpansionLinker = nullptr;
					}
					return this;
				}

				cint64 CReusingIndividualNodeConceptExpansionData::getReusingTriedCount() {
					return mReusingTriedCount;
				}

				cint64 CReusingIndividualNodeConceptExpansionData::getReusingFailedCount() {
					return mReusingFailedCount;
				}

				CReusingIndividualNodeConceptExpansionData* CReusingIndividualNodeConceptExpansionData::setReusingTriedCount(cint64 triedCount) {
					mReusingTriedCount = triedCount;
					return this;
				}

				CReusingIndividualNodeConceptExpansionData* CReusingIndividualNodeConceptExpansionData::setReusingFailedCount(cint64 failedCount) {
					mReusingFailedCount = failedCount;
					return this;
				}

				CReusingIndividualNodeConceptExpansionData* CReusingIndividualNodeConceptExpansionData::incReusingTriedCount(cint64 incCount) {
					mReusingTriedCount += incCount;
					return this;
				}

				CReusingIndividualNodeConceptExpansionData* CReusingIndividualNodeConceptExpansionData::incReusingFailedCount(cint64 incCount) {
					mReusingFailedCount += incCount;
					return this;
				}

				CReusingIndividualNodeConceptExpansionData* CReusingIndividualNodeConceptExpansionData::addReusingFailedSignatureAndIndividual(cint64 conSetSignature, cint64 individualID) {
					mReusedIndiviudals.insert(individualID);
					mReusedConceptSetSignatures.insert(conSetSignature);
					return this;
				}

				CNonDeterministicDependencyTrackPoint* CReusingIndividualNodeConceptExpansionData::getReuseConceptsDependencyTrackPoint() {
					return mReuseConceptsDependencyTrackPoint;
				}

				CReusingIndividualNodeConceptExpansionData* CReusingIndividualNodeConceptExpansionData::setReuseConceptsDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint) {
					mReuseConceptsDependencyTrackPoint = depTrackPoint;
					return this;
				}

				CXLinker<CConceptDescriptor*>* CReusingIndividualNodeConceptExpansionData::getLastNonDeterministicExpansionLinker() {
					return mLastNonDetExpansionLinker;
				}

				CReusingIndividualNodeConceptExpansionData* CReusingIndividualNodeConceptExpansionData::setLastNonDeterministicExpansionLinker(CXLinker<CConceptDescriptor*>* expLinker) {
					mLastNonDetExpansionLinker = expLinker;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
