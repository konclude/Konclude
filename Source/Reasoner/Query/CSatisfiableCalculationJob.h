/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
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
#include "Reasoner/Kernel/Task/CTaskPreyingAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationJobInstantiation.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskClassificationMessageAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskIncrementalConsistencyTestingAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskIndividualDependenceTrackingAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskRealizationPossibleAssertionCollectingAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskClassificationRoleMarkedMessageAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskAnswererSubsumptionMessageAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskAnswererBindingPropagationAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskRealizationPossibleInstancesMergingAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskAnswererInstancePropagationMessageAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskRepresentativeBackendUpdatingAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskAnswererQueryingMaterializationAdapter.h"

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

					CSatisfiableCalculationJob* setSatisfiableClassificationMessageAdapter(CSatisfiableTaskClassificationMessageAdapter* classMessObserver);
					CSatisfiableTaskClassificationMessageAdapter* getSatisfiableClassificationMessageAdapter();

					CSatisfiableCalculationJobInstantiation* getConsecutivelyCalculationJobInstantiation();
					CSatisfiableCalculationJob* setConsecutivelyCalculationJobInstantiation(CSatisfiableCalculationJobInstantiation* jobInstantiation);


					CSatisfiableCalculationJob* setSatisfiableTaskIncrementalConsistencyTestingAdapter(CSatisfiableTaskIncrementalConsistencyTestingAdapter* incConsTestAdaptor);
					CSatisfiableTaskIncrementalConsistencyTestingAdapter* getSatisfiableTaskIncrementalConsistencyTestingAdapter();


					CSatisfiableCalculationJob* setRealizationMarkedCandidatesMessageAdapter(CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter* realMessObserver);
					CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter* getRealizationMarkedCandidatesMessageAdapter();

					CSatisfiableCalculationJob* setSatisfiableTaskIndividualDependenceTrackingAdapter(CSatisfiableTaskIndividualDependenceTrackingAdapter* indDepTrackAdaptor);
					CSatisfiableTaskIndividualDependenceTrackingAdapter* getSatisfiableTaskIndividualDependenceTrackingAdapter();


					CSatisfiableCalculationJob* setPossibleAssertionCollectionAdapter(CSatisfiableTaskRealizationPossibleAssertionCollectingAdapter* possAssCollAdapter);
					CSatisfiableTaskRealizationPossibleAssertionCollectingAdapter* getPossibleAssertionCollectionAdapter();


					CSatisfiableCalculationJob* setSatisfiableClassificationRoleMarkedMessageAdapter(CSatisfiableTaskClassificationRoleMarkedMessageAdapter* classRoleMarkedMessageAdapter);
					CSatisfiableTaskClassificationRoleMarkedMessageAdapter* getSatisfiableClassificationRoleMarkedMessageAdapter();

					CSatisfiableCalculationJob* setSatisfiableAnswererSubsumptionMessageAdapter(CSatisfiableTaskAnswererSubsumptionMessageAdapter* answererMessageAdapter);
					CSatisfiableTaskAnswererSubsumptionMessageAdapter* getSatisfiableAnswererSubsumptionMessageAdapter();


					CSatisfiableCalculationJob* setSatisfiableAnswererBindingPropagationAdapter(CSatisfiableTaskAnswererBindingPropagationAdapter* answererMessageAdapter);
					CSatisfiableTaskAnswererBindingPropagationAdapter* getSatisfiableAnswererBindingPropagationAdapter();

					CSatisfiableCalculationJob* setSatisfiablePossibleInstancesMergingAdapter(CSatisfiableTaskRealizationPossibleInstancesMergingAdapter* possInstMergingAdapter);
					CSatisfiableTaskRealizationPossibleInstancesMergingAdapter* getSatisfiablePossibleInstancesMergingAdapter();


					CSatisfiableCalculationJob* setSatisfiableAnswererInstancePropagationMessageAdapter(CSatisfiableTaskAnswererInstancePropagationMessageAdapter* answererMessageAdapter);
					CSatisfiableTaskAnswererInstancePropagationMessageAdapter* getSatisfiableAnswererInstancePropagationMessageAdapter();


					CSatisfiableCalculationJob* setSatisfiableRepresentativeBackendCacheUpdatingAdapter(CSatisfiableTaskRepresentativeBackendUpdatingAdapter* answererMessageAdapter);
					CSatisfiableTaskRepresentativeBackendUpdatingAdapter* getSatisfiableRepresentativeBackendCacheUpdatingAdapter();


					CSatisfiableCalculationJob* setSatisfiableAnswererMaterializationAdapter(CSatisfiableTaskAnswererQueryingMaterializationAdapter* collAdapter);
					CSatisfiableTaskAnswererQueryingMaterializationAdapter* getSatisfiableAnswererMaterializationAdapter();


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
					CSatisfiableCalculationJobInstantiation* mJobInstantiation;
					CSatisfiableTaskClassificationMessageAdapter* mSatClassMessAdapter;
					CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter* mRealMarkCandMessAdapter;
					CSatisfiableTaskIncrementalConsistencyTestingAdapter* mSatIncConsTestingAdapter;
					CSatisfiableTaskIndividualDependenceTrackingAdapter* mSatIndDepTrackAdapter;
					CSatisfiableTaskRealizationPossibleAssertionCollectingAdapter* mPossAssCollAdapter;
					CSatisfiableTaskClassificationRoleMarkedMessageAdapter* mClassRoleMarkedMessageAdapter;
					CSatisfiableTaskAnswererSubsumptionMessageAdapter* mAnswererSubsumptionMessageAdapter;
					CSatisfiableTaskAnswererBindingPropagationAdapter* mAnswererBindingPropagationAdapter;
					CSatisfiableTaskRealizationPossibleInstancesMergingAdapter* mSatisfiablePossibleInstancesMergingAdapter;
					CSatisfiableTaskAnswererInstancePropagationMessageAdapter* mAnswererInstancePropagationMessageAdapter;
					CSatisfiableTaskRepresentativeBackendUpdatingAdapter* mRepresentativeBackendUpdatingAdapter;
					CSatisfiableTaskAnswererQueryingMaterializationAdapter* mAnswererMaterializationAdapter;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSATISFIABLECALCULATIONJOB_H
