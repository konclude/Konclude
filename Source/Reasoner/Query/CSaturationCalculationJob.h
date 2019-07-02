/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_QUERY_CSATURATIONCALCULATIONJOB_H
#define KONCLUDE_REASONER_QUERY_CSATURATIONCALCULATIONJOB_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"
#include "CCalculationJob.h"
#include "CSaturationCalculationConstruct.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"
#include "Reasoner/Kernel/Task/CCalculationStatisticsCollector.h"
#include "Reasoner/Kernel/Task/CSatisfiableSubsumptionIdentifierAdapter.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Query {

			/*! 
			 *
			 *		\class		CSaturationCalculationJob
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSaturationCalculationJob : public CCalculationJob {
				// public methods
				public:
					//! Constructor
					CSaturationCalculationJob();
 
					//! Destructor
					virtual ~CSaturationCalculationJob();

					virtual CCalculationJobQueryData* getQueryData();

					CSaturationCalculationJob* setQueryData(CCalculationJobQueryData* takeQueryData);

					CSaturationCalculationJob* addSatisfiableCalculationConstruct(CSaturationCalculationConstruct* takeSatCalcConstruct);
					CSaturationCalculationConstruct* getSatisfiableCalculationConstructs();

					CSaturationCalculationJob* setCalculationConfiguration(CCalculationConfigurationExtension* calculationConfiguration);
					CCalculationConfigurationExtension* getCalculationConfiguration();

					CSaturationCalculationJob* setCalclulationStatisticsCollector(CCalculationStatisticsCollector* calclulationStatisticsCollector);
					CCalculationStatisticsCollector* getCalclulationStatisticsCollector();

					CSaturationCalculationJob* setSatisfiableSubsumptionIdentifierAdapter(CSatisfiableSubsumptionIdentifierAdapter* satSubIdObserver);
					CSatisfiableSubsumptionIdentifierAdapter* getSatisfiableSubsumptionIdentifierAdapter();

				// protected methods
				protected:

				// protected variables
				protected:
					CCalculationJobQueryData* mQueryData;
					CSaturationCalculationConstruct* mSatCalcConstructLinker;
					CCalculationConfigurationExtension* mCalculationConfiguration;
					CCalculationStatisticsCollector* mCalclulationStatisticsCollector;
					CSatisfiableSubsumptionIdentifierAdapter* mSatSubIdAdapter;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSATURATIONCALCULATIONJOB_H
