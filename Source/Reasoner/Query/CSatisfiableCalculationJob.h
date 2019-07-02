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

#ifndef KONCLUDE_REASONER_QUERY_CSATISFIABLECALCULATIONJOB_H
#define KONCLUDE_REASONER_QUERY_CSATISFIABLECALCULATIONJOB_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"
#include "CCalculationJob.h"
#include "CSatisfiableCalculationConstruct.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"
#include "Reasoner/Kernel/Task/CCalculationStatisticsCollector.h"
#include "Reasoner/Kernel/Task/CSatisfiableSubsumptionIdentifierAdapter.h"
#include "Reasoner/Kernel/Task/CTaskPreyingAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationJobInstantiation.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskClassificationMessageAdapter.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Query {

			/*! 
			 *
			 *		\class		CSatisfiableCalculationJob
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSatisfiableCalculationJob : public CCalculationJob {
				// public methods
				public:
					//! Constructor
					CSatisfiableCalculationJob();
 
					//! Destructor
					virtual ~CSatisfiableCalculationJob();

					virtual CCalculationJobQueryData* getQueryData();

					CSatisfiableCalculationJob* setQueryData(CCalculationJobQueryData* takeQueryData);

					CSatisfiableCalculationJob* addSatisfiableCalculationConstruct(CSatisfiableCalculationConstruct* takeSatCalcConstruct);
					CSatisfiableCalculationConstruct* getSatisfiableCalculationConstructs();

					virtual CConfigurationBase* getConfiguration();

					CSatisfiableCalculationJob* setCalculationConfiguration(CCalculationConfigurationExtension* calculationConfiguration);
					CCalculationConfigurationExtension* getCalculationConfiguration();

					CSatisfiableCalculationJob* setCalclulationStatisticsCollector(CCalculationStatisticsCollector* calclulationStatisticsCollector);
					CCalculationStatisticsCollector* getCalclulationStatisticsCollector();

					CSatisfiableCalculationJob* setSatisfiableTaskPreyingAdapter(CTaskPreyingAdapter* preyingAdapter);
					CTaskPreyingAdapter* getSatisfiableTaskPreyingAdapter();

					CSatisfiableCalculationJob* setSatisfiableSubsumptionIdentifierAdapter(CSatisfiableSubsumptionIdentifierAdapter* satSubIdObserver);
					CSatisfiableSubsumptionIdentifierAdapter* getSatisfiableSubsumptionIdentifierAdapter();

					CSatisfiableCalculationJob* setSatisfiableClassificationMessageAdapter(CSatisfiableTaskClassificationMessageAdapter* classMessObserver);
					CSatisfiableTaskClassificationMessageAdapter* getSatisfiableClassificationMessageAdapter();

					CSatisfiableCalculationJobInstantiation* getConsecutivelyCalculationJobInstantiation();
					CSatisfiableCalculationJob* setConsecutivelyCalculationJobInstantiation(CSatisfiableCalculationJobInstantiation* jobInstantiation);


					cint64 getNextRelativeNodeID(bool moveNext = true);

				// protected methods
				protected:

				// protected variables
				protected:
					CCalculationJobQueryData* mQueryData;
					cint64 mNextRelativeNodeID;
					CSatisfiableCalculationConstruct* mSatCalcConstructLinker;
					CCalculationConfigurationExtension* mCalculationConfiguration;
					CCalculationStatisticsCollector* mCalclulationStatisticsCollector;
					CTaskPreyingAdapter* mPreyingAdapter;
					CSatisfiableSubsumptionIdentifierAdapter* mSatSubIdAdapter;
					CSatisfiableCalculationJobInstantiation* mJobInstantiation;
					CSatisfiableTaskClassificationMessageAdapter* mSatClassMessAdapter;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSATISFIABLECALCULATIONJOB_H
