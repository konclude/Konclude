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

#ifndef KONCLUDE_REASONER_ANSWERER_CCACHEANSWERSWEIGHTEDUSAGECOSTITEM_H
#define KONCLUDE_REASONER_ANSWERER_CCACHEANSWERSWEIGHTEDUSAGECOSTITEM_H

// Libraries includes
#include <QTime>

// Namespace includes
#include "AnswererSettings.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"

#include "Utilities/UtilitiesSettings.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		CCacheAnswersWeightedUsageCostItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCacheAnswersWeightedUsageCostItem {
				// public methods
				public:
					//! Constructor
					CCacheAnswersWeightedUsageCostItem();
					~CCacheAnswersWeightedUsageCostItem();

					virtual bool clearComputation();

					virtual bool startComputationCostMearsuring();
					virtual bool stopComputationCostMearsuring();


					cint64 getQueryUsageCount();
					CCacheAnswersWeightedUsageCostItem* incQueryUsageCount(cint64 count = 1);
					CCacheAnswersWeightedUsageCostItem* decQueryUsageCount(cint64 count = 1);
					CCacheAnswersWeightedUsageCostItem* setQueryUsageCount(cint64 count);


					double getCacheWeightedUsageFactor();
					CCacheAnswersWeightedUsageCostItem* setCacheWeightedUsageFactor(double factor);
					CCacheAnswersWeightedUsageCostItem* incCacheWeightedUsageFactor(double factor);

					double getLastCacheInstalledCostWeightedUsage();
					CCacheAnswersWeightedUsageCostItem* setLastCacheInstalledCostWeightedUsage(double usageWeightedCost);

					bool isCacheUpdateQueued();
					CCacheAnswersWeightedUsageCostItem* setCacheUpdateQueued(bool queued);

					double getComputationCost();

					bool isComputationCostMeasuring();

				// protected methods
				protected:

				// protected variables
				protected:
					double mLastCacheInstalledCostWeightedUsage;

					QTime* mTimeCostMeasurementTimer;

					bool mCacheUpdateQueued;

					cint64 mQueryUsageCount;
					cint64 mTotalQueryUsageCount;
					double mCacheWeightedUsageFactor;

					double mComputationCost;
					double mPrevComputationCost;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CCACHEANSWERSWEIGHTEDUSAGECOSTITEM_H
