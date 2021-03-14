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

#include "CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker::CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker(cint64 ontologyDataUpdateId) {
					mOntologyDataUpdateId = ontologyDataUpdateId;
					mMinUsedRecomputationId = CINT64_MAX;
					mMaxUsedRecomputationId = CINT64_MIN;
					setData(this);
					mOntologyDataActive = true;
					mNextOntologyDataAllInactive = false;
				}

				CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker::updateUsedRecomputationId(cint64 recomputationId) {
					cint64 currentMinRecomputationId = mMinUsedRecomputationId;
					do {
						cint64 newMinRecomputationId = qMin(currentMinRecomputationId, recomputationId);
						mMinUsedRecomputationId.testAndSetOrdered(currentMinRecomputationId, newMinRecomputationId);
						currentMinRecomputationId = mMinUsedRecomputationId;
					} while (currentMinRecomputationId != qMin(currentMinRecomputationId, recomputationId));

					cint64 currentMaxRecomputationId = mMaxUsedRecomputationId;
					do {
						cint64 newMaxRecomputationId = qMax(currentMaxRecomputationId, recomputationId);
						mMaxUsedRecomputationId.testAndSetOrdered(currentMaxRecomputationId, newMaxRecomputationId);
						currentMaxRecomputationId = mMaxUsedRecomputationId;
					} while (currentMaxRecomputationId != qMax(currentMaxRecomputationId, recomputationId));

					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker::getMinUsedRecomputationId() {
					return mMinUsedRecomputationId;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker::getMaxUsedRecomputationId() {
					return mMaxUsedRecomputationId;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker::getOntologyDataUpdateId() {
					return mOntologyDataUpdateId;
				}


				bool CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker::isOntologyDataActive() {
					return mOntologyDataActive;
				}

				CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker::setOntologyDataInactive() {
					mOntologyDataActive = false;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker::isNextOntologyDataAllInactive() {
					return mNextOntologyDataAllInactive;
				}

				CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker::setNextOntologyDataAllInactive() {
					mNextOntologyDataAllInactive = true;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
