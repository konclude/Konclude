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

#ifndef KONCLUDE_REASONER_CONSISTISER_CCONCEPTCYCLEPRECOMPUTATIONTESTINGITEM_H
#define KONCLUDE_REASONER_CONSISTISER_CCONCEPTCYCLEPRECOMPUTATIONTESTINGITEM_H

// Libraries includes


// Namespace includes
#include "ConsistiserSettings.h"
#include "CPrecomputationTestingItem.h"

// Other includes
#include "Reasoner/Kernel/Task/CTaskPreyingAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Consistiser {


			/*! 
			 *
			 *		\class		CConceptCyclePrecomputationTestingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptCyclePrecomputationTestingItem : public CPrecomputationTestingItem, public CTaskPreyingListner {
				// public methods
				public:
					//! Constructor
					CConceptCyclePrecomputationTestingItem(CConceptCycleData* conceptCycleData, COntologyPrecomputationItem* preCompItem);

					virtual PRECOMPUTATIONTYPE getPrecomputationTestingType();

					CTaskPreyingAdapter* getTaskPreyingAdapter();

					virtual CTaskPreyingListner* tellPreyedSatisfialbeTask(CSatisfiableCalculationTask* task);

					CConceptCycleData* getConceptCycleData();

					CSatisfiableCalculationJobInstantiation* getJobInstantiation();


				// protected methods
				protected:

				// protected variables
				protected:
					CTaskPreyingAdapter mTaskPreyingAdapter;
					QAtomicPointer<CSatisfiableCalculationTask> mSatCalcTaskPointer;

					CConceptCycleData* mConceptCycleData;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CCONCEPTCYCLEPRECOMPUTATIONTESTINGITEM_H
