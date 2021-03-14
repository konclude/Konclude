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

#include "CCacheAnswersWeightedUsageCostItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CCacheAnswersWeightedUsageCostItem::CCacheAnswersWeightedUsageCostItem() {
				mComputationCost = 0;
				mLastCacheInstalledCostWeightedUsage = -1;
				mTimeCostMeasurementTimer = nullptr;
				mCacheUpdateQueued = false;
				mQueryUsageCount = 0;
				mCacheWeightedUsageFactor = 0;
				mPrevComputationCost = 0;
				mTotalQueryUsageCount = 0;
			}


			CCacheAnswersWeightedUsageCostItem::~CCacheAnswersWeightedUsageCostItem() {
			}



			cint64 CCacheAnswersWeightedUsageCostItem::getQueryUsageCount() {
				return mQueryUsageCount;
			}

			CCacheAnswersWeightedUsageCostItem* CCacheAnswersWeightedUsageCostItem::incQueryUsageCount(cint64 count) {
				mTotalQueryUsageCount++;
				mQueryUsageCount += count;
				return this;
			}

			CCacheAnswersWeightedUsageCostItem* CCacheAnswersWeightedUsageCostItem::decQueryUsageCount(cint64 count) {
				mQueryUsageCount -= count;
				return this;
			}

			CCacheAnswersWeightedUsageCostItem* CCacheAnswersWeightedUsageCostItem::setQueryUsageCount(cint64 count) {
				mQueryUsageCount = count;
				return this;
			}


			double CCacheAnswersWeightedUsageCostItem::getCacheWeightedUsageFactor() {
				return mCacheWeightedUsageFactor;
			}


			CCacheAnswersWeightedUsageCostItem* CCacheAnswersWeightedUsageCostItem::setCacheWeightedUsageFactor(double factor) {
				mCacheWeightedUsageFactor = factor;
				return this;
			}


			CCacheAnswersWeightedUsageCostItem* CCacheAnswersWeightedUsageCostItem::incCacheWeightedUsageFactor(double factor) {
				mCacheWeightedUsageFactor += factor;
				return this;
			}

			double CCacheAnswersWeightedUsageCostItem::getLastCacheInstalledCostWeightedUsage() {
				return mLastCacheInstalledCostWeightedUsage;
			}

			CCacheAnswersWeightedUsageCostItem* CCacheAnswersWeightedUsageCostItem::setLastCacheInstalledCostWeightedUsage(double usageWeightedCost) {
				mLastCacheInstalledCostWeightedUsage = usageWeightedCost;
				return this;
			}




			bool CCacheAnswersWeightedUsageCostItem::isCacheUpdateQueued() {
				return mCacheUpdateQueued;
			}

			CCacheAnswersWeightedUsageCostItem* CCacheAnswersWeightedUsageCostItem::setCacheUpdateQueued(bool queued) {
				mCacheUpdateQueued = queued;
				return this;
			}




			bool CCacheAnswersWeightedUsageCostItem::clearComputation() {
				mPrevComputationCost = mComputationCost;
				mComputationCost = 0;
				return true;
			}

			bool CCacheAnswersWeightedUsageCostItem::startComputationCostMearsuring() {
				if (!mTimeCostMeasurementTimer) {
					mTimeCostMeasurementTimer = new QTime();
					mTimeCostMeasurementTimer->start();
					return true;
				} else {
					return false;
				}
			}

			bool CCacheAnswersWeightedUsageCostItem::stopComputationCostMearsuring() {
				if (mTimeCostMeasurementTimer) {
					mComputationCost += mTimeCostMeasurementTimer->elapsed();
					mComputationCost = qMax(0.1, mComputationCost);
					delete mTimeCostMeasurementTimer;
					mTimeCostMeasurementTimer = nullptr;
					return true;
				} else {
					return false;
				}
			}

			bool CCacheAnswersWeightedUsageCostItem::isComputationCostMeasuring() {
				return mTimeCostMeasurementTimer != nullptr;
			}


			double CCacheAnswersWeightedUsageCostItem::getComputationCost() {
				return mComputationCost;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
