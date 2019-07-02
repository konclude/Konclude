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

#include "CSaturationCalculationJob.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSaturationCalculationJob::CSaturationCalculationJob() {
				mQueryData = nullptr;
				mSatCalcConstructLinker = nullptr;
				mCalculationConfiguration = nullptr;
				mCalclulationStatisticsCollector = nullptr;
				mSatSubIdAdapter = nullptr;
			}


			CSaturationCalculationJob::~CSaturationCalculationJob() {
				delete mQueryData;
				CSaturationCalculationConstruct* satCalcConstructLinkerIt = mSatCalcConstructLinker;
				while (satCalcConstructLinkerIt) {
					CSaturationCalculationConstruct* tmpLinker = satCalcConstructLinkerIt;
					satCalcConstructLinkerIt = satCalcConstructLinkerIt->getNext();
					delete tmpLinker;
				}
			}

			CCalculationJobQueryData* CSaturationCalculationJob::getQueryData() {
				return mQueryData;
			}

			CSaturationCalculationJob* CSaturationCalculationJob::setQueryData(CCalculationJobQueryData* takeQueryData) {
				delete mQueryData;
				mQueryData = takeQueryData;
				return this;
			}

			CSaturationCalculationJob* CSaturationCalculationJob::addSatisfiableCalculationConstruct(CSaturationCalculationConstruct* takeSatCalcConstruct) {
				if (takeSatCalcConstruct) {
					mSatCalcConstructLinker = takeSatCalcConstruct->append(mSatCalcConstructLinker);
				}
				return this;
			}

			CSaturationCalculationConstruct* CSaturationCalculationJob::getSatisfiableCalculationConstructs() {
				return mSatCalcConstructLinker;
			}

			CSaturationCalculationJob* CSaturationCalculationJob::setCalculationConfiguration(CCalculationConfigurationExtension *calculationConfiguration) {
				mCalculationConfiguration = calculationConfiguration;
				return this;
			}

			CCalculationConfigurationExtension* CSaturationCalculationJob::getCalculationConfiguration() {
				return mCalculationConfiguration;
			}

			CSaturationCalculationJob* CSaturationCalculationJob::setCalclulationStatisticsCollector(CCalculationStatisticsCollector* calclulationStatisticsCollector) {
				mCalclulationStatisticsCollector = calclulationStatisticsCollector;
				return this;
			}

			CCalculationStatisticsCollector* CSaturationCalculationJob::getCalclulationStatisticsCollector() {
				return mCalclulationStatisticsCollector;
			}

			CSaturationCalculationJob* CSaturationCalculationJob::setSatisfiableSubsumptionIdentifierAdapter(CSatisfiableSubsumptionIdentifierAdapter* satSubIdObserver) {
				mSatSubIdAdapter = satSubIdObserver;
				return this;
			}

			CSatisfiableSubsumptionIdentifierAdapter* CSaturationCalculationJob::getSatisfiableSubsumptionIdentifierAdapter() {
				return mSatSubIdAdapter;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
