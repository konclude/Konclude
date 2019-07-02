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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONSATISFIABLECALCULATIONDATA_H
#define KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONSATISFIABLECALCULATIONDATA_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"

// Other includes
#include "Reasoner/Taxonomy/CHierarchyNodeData.h"

#include "Reasoner/Kernel/Task/CTaskPreyingListner.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationJobInstantiation.h"

#include "Utilities/UtilitiesSettings.h"
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Taxonomy;
		using namespace Kernel::Task;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CClassificationSatisfiableCalculationData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationSatisfiableCalculationData : public CHierarchyNodeData, public CTaskPreyingListner {
				// public methods
				public:
					//! Constructor
					CClassificationSatisfiableCalculationData();

					virtual CTaskPreyingListner* tellPreyedSatisfialbeTask(CSatisfiableCalculationTask* task);

					bool hasSatisfiableCalculationTask();
					CSatisfiableCalculationTask* getSatisfiableCalculationTask();
					CClassificationSatisfiableCalculationData* setSatisfiableCalculationTask(CSatisfiableCalculationTask* task);

					bool hasSatisfiableCalculationJobInstantiation();
					CSatisfiableCalculationJobInstantiation* getSatisfiableCalculationJobInstantiation();
					bool clearSatisfiableCalculationJobInstantiation();

					CClassificationSatisfiableCalculationData* getPreviousCalculationData();
					CClassificationSatisfiableCalculationData* setPreviousCalculationData(CClassificationSatisfiableCalculationData* data);
					
					cint64 getDependendCalculationCount();
					CClassificationSatisfiableCalculationData* incDependendCalculationCount(cint64 incCount = 1);
					bool decDependendCalculationCount(cint64 decCount = 1);

				// protected methods
				protected:

				// protected variables
				protected:
					CSatisfiableCalculationTask* mSatCalcedTask;
					CSatisfiableCalculationJobInstantiation* mSatCalcedJob;
					CClassificationSatisfiableCalculationData* mPrevData;
					cint64 mDependendCalculationCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONSATISFIABLECALCULATIONDATA_H
