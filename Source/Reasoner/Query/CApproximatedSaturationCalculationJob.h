/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_QUERY_CAPPROXIMATEDSATURATIONCALCULATIONJOB_H
#define KONCLUDE_REASONER_QUERY_CAPPROXIMATEDSATURATIONCALCULATIONJOB_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"
#include "CCalculationJob.h"
#include "CApproximatedSaturationCalculationConstructionConstruct.h"
#include "CApproximatedSaturationCalculationProcessingConstruct.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"
#include "Reasoner/Kernel/Task/CCalculationStatisticsCollector.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationJobInstantiation.h"

#include "Reasoner/Kernel/Task/CSaturationIndividualsAnalysingAdapter.h"
#include "Reasoner/Kernel/Task/CTaskPreyingAdapter.h"

#include "Reasoner/Consistence/CSaturationData.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Consistence;

		namespace Query {

			/*! 
			 *
			 *		\class		CApproximatedSaturationCalculationJob
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CApproximatedSaturationCalculationJob : public CCalculationJob {
				// public methods
				public:
					//! Constructor
					CApproximatedSaturationCalculationJob();
 
					//! Destructor
					virtual ~CApproximatedSaturationCalculationJob();

					virtual CCalculationJobQueryData* getQueryData();

					CApproximatedSaturationCalculationJob* setQueryData(CCalculationJobQueryData* takeQueryData);

					CApproximatedSaturationCalculationJob* addSatisfiableCalculationConstructionConstruct(CApproximatedSaturationCalculationConstructionConstruct* takeSatCalcConstruct);
					CApproximatedSaturationCalculationConstructionConstruct* getSatisfiableCalculationConstructionConstructs();

					CApproximatedSaturationCalculationJob* addSatisfiableCalculationProcessingConstruct(CApproximatedSaturationCalculationProcessingConstruct* takeSatCalcConstruct);
					CApproximatedSaturationCalculationProcessingConstruct* getSatisfiableCalculationProcessingConstructs();

					CApproximatedSaturationCalculationJob* setCalculationConfiguration(CCalculationConfigurationExtension* calculationConfiguration);
					CCalculationConfigurationExtension* getCalculationConfiguration();

					CApproximatedSaturationCalculationJob* setCalclulationStatisticsCollector(CCalculationStatisticsCollector* calclulationStatisticsCollector);
					CCalculationStatisticsCollector* getCalclulationStatisticsCollector();

					CApproximatedSaturationCalculationJob* setSaturationTaskPreyingAdapter(CTaskPreyingAdapter* preyingAdapter);
					CTaskPreyingAdapter* getSaturationTaskPreyingAdapter();

					CSaturationData* getContinueSaturationData();
					CApproximatedSaturationCalculationJob* setContinueSaturationData(CSaturationData* saturationData);

					bool isSeparateSaturation();
					CApproximatedSaturationCalculationJob* setSeparateSaturation(bool separateSaturation);

					CApproximatedSaturationCalculationJob* setSaturationIndividualsAnalysationObserver(CSaturationIndividualsAnalysingAdapter* indiAnalAdapter);
					CSaturationIndividualsAnalysingAdapter* getSaturationIndividualsAnalysationObserver();

				// protected methods
				protected:

				// protected variables
				protected:
					CCalculationJobQueryData* mQueryData;
					CApproximatedSaturationCalculationConstructionConstruct* mSatCalcConstructionConstructLinker;
					CApproximatedSaturationCalculationProcessingConstruct* mSatCalcProcessingConstructLinker;
					CCalculationConfigurationExtension* mCalculationConfiguration;
					CCalculationStatisticsCollector* mCalclulationStatisticsCollector;
					CTaskPreyingAdapter* mPreyingAdapter;
					CSaturationData* mContinueSaturationData;
					bool mSeparateSaturation;
					CSaturationIndividualsAnalysingAdapter* mIndiAnalAdapter;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CAPPROXIMATEDSATURATIONCALCULATIONJOB_H
