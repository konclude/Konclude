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

#include "CSatisfiableCalculationJob.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSatisfiableCalculationJob::CSatisfiableCalculationJob() {
				mQueryData = nullptr;
				mSatCalcConstructLinker = nullptr;
				mCalculationConfiguration = nullptr;
				mCalclulationStatisticsCollector = nullptr;
				mPreyingAdapter = nullptr;
				mSatSubIdAdapter = nullptr;
				mSatClassMessAdapter = nullptr;
				mJobInstantiation = nullptr;
				mNextRelativeNodeID = 0;
			}


			CSatisfiableCalculationJob::~CSatisfiableCalculationJob() {
				delete mQueryData;
				CSatisfiableCalculationConstruct* satCalcConstructLinkerIt = mSatCalcConstructLinker;
				while (satCalcConstructLinkerIt) {
					CSatisfiableCalculationConstruct* tmpLinker = satCalcConstructLinkerIt;
					satCalcConstructLinkerIt = satCalcConstructLinkerIt->getNext();
					delete tmpLinker;
				}
			}

			CCalculationJobQueryData* CSatisfiableCalculationJob::getQueryData() {
				return mQueryData;
			}

			CSatisfiableCalculationJob* CSatisfiableCalculationJob::setQueryData(CCalculationJobQueryData* takeQueryData) {
				delete mQueryData;
				mQueryData = takeQueryData;
				return this;
			}

			CSatisfiableCalculationJob* CSatisfiableCalculationJob::addSatisfiableCalculationConstruct(CSatisfiableCalculationConstruct* takeSatCalcConstruct) {
				if (takeSatCalcConstruct) {
					mSatCalcConstructLinker = takeSatCalcConstruct->append(mSatCalcConstructLinker);
				}
				return this;
			}

			CSatisfiableCalculationConstruct* CSatisfiableCalculationJob::getSatisfiableCalculationConstructs() {
				return mSatCalcConstructLinker;
			}

			CSatisfiableCalculationJob* CSatisfiableCalculationJob::setCalculationConfiguration(CCalculationConfigurationExtension *calculationConfiguration) {
				mCalculationConfiguration = calculationConfiguration;
				return this;
			}

			CCalculationConfigurationExtension* CSatisfiableCalculationJob::getCalculationConfiguration() {
				return mCalculationConfiguration;
			}

			CConfigurationBase* CSatisfiableCalculationJob::getConfiguration() {
				return mCalculationConfiguration;
			}

			CSatisfiableCalculationJob* CSatisfiableCalculationJob::setCalclulationStatisticsCollector(CCalculationStatisticsCollector* calclulationStatisticsCollector) {
				mCalclulationStatisticsCollector = calclulationStatisticsCollector;
				return this;
			}

			CCalculationStatisticsCollector* CSatisfiableCalculationJob::getCalclulationStatisticsCollector() {
				return mCalclulationStatisticsCollector;
			}

			CSatisfiableCalculationJob* CSatisfiableCalculationJob::setSatisfiableTaskPreyingAdapter(CTaskPreyingAdapter* preyingAdapter) {
				mPreyingAdapter = preyingAdapter;
				return this;
			}

			CTaskPreyingAdapter* CSatisfiableCalculationJob::getSatisfiableTaskPreyingAdapter() {
				return mPreyingAdapter;
			}

			CSatisfiableCalculationJob* CSatisfiableCalculationJob::setSatisfiableSubsumptionIdentifierAdapter(CSatisfiableSubsumptionIdentifierAdapter* satSubIdObserver) {
				mSatSubIdAdapter = satSubIdObserver;
				return this;
			}


			CSatisfiableCalculationJob* CSatisfiableCalculationJob::setSatisfiableClassificationMessageAdapter(CSatisfiableTaskClassificationMessageAdapter* classMessObserver) {
				mSatClassMessAdapter = classMessObserver;
				return this;
			}

			CSatisfiableTaskClassificationMessageAdapter* CSatisfiableCalculationJob::getSatisfiableClassificationMessageAdapter() {
				return mSatClassMessAdapter;
			}


			CSatisfiableSubsumptionIdentifierAdapter* CSatisfiableCalculationJob::getSatisfiableSubsumptionIdentifierAdapter() {
				return mSatSubIdAdapter;
			}

			CSatisfiableCalculationJobInstantiation* CSatisfiableCalculationJob::getConsecutivelyCalculationJobInstantiation() {
				return mJobInstantiation;
			}

			CSatisfiableCalculationJob* CSatisfiableCalculationJob::setConsecutivelyCalculationJobInstantiation(CSatisfiableCalculationJobInstantiation* jobInstantiation) {
				mJobInstantiation = jobInstantiation;
				return this;
			}


			cint64 CSatisfiableCalculationJob::getNextRelativeNodeID(bool moveNext) {
				return mNextRelativeNodeID;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
