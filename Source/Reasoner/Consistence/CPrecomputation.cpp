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

#include "CPrecomputation.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistence {


			CPrecomputation::CPrecomputation() {
				mPrecomputed = false;
				mSaturData = nullptr;
				mStatsColl = nullptr;
				mInitializedSaturationNodeCount = 0;
				mSaturationNodeCount = 0;
			}

			CPrecomputation::~CPrecomputation() {
				delete mSaturData;
			}


			CPrecomputation* CPrecomputation::incInitializedSaturationNodeCount(cint64 count) {
				mInitializedSaturationNodeCount.fetch_add(count,std::memory_order_relaxed);
				return this;
			}

			CPrecomputation* CPrecomputation::updateSaturationNodeCount(cint64 nodeCount) {
				cint64 current = mSaturationNodeCount.load(std::memory_order_relaxed);
				while (nodeCount > current && !mSaturationNodeCount.compare_exchange_weak(current,nodeCount,std::memory_order_relaxed)) {
				}
				return this;
			}

			cint64 CPrecomputation::getInitializedSaturationNodeCount() {
				return mInitializedSaturationNodeCount.load(std::memory_order_relaxed);
			}

			cint64 CPrecomputation::getSaturationNodeCount() {
				return mSaturationNodeCount.load(std::memory_order_relaxed);
			}

			bool CPrecomputation::isPrecomputed() {
				return mPrecomputed;
			}


			CPrecomputation* CPrecomputation::setPrecomputed(bool precomputed) {
				mPrecomputed = precomputed;
				return this;
			}

			CSaturationData* CPrecomputation::getSaturationModelData() {
				return mSaturData;
			}

			bool CPrecomputation::hasSaturationModelData() {
				return mSaturData != nullptr;
			}

			CPrecomputation* CPrecomputation::setSaturationModelData(CSaturationData* saturationData) {
				mSaturData = saturationData;
				return this;
			}


			CPrecomputationStatistics* CPrecomputation::getPrecomputationStatistics() {
				return mStatsColl;
			}

			CPrecomputation* CPrecomputation::setPrecomputationStatistics(CPrecomputationStatistics* statColl) {
				mStatsColl = statColl;
				return this;
			}

		}; // end namespace Consistence

	}; // end namespace Reasoner

}; // end namespace Konclude
