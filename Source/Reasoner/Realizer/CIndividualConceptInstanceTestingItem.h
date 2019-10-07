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

#ifndef KONCLUDE_REASONER_REALIZER_CINDIVIDUALCONCEPTINSTANCETESTINGITEM_H
#define KONCLUDE_REASONER_REALIZER_CINDIVIDUALCONCEPTINSTANCETESTINGITEM_H

// Libraries includes
#include <QTime>

// Namespace includes
#include "RealizerSettings.h"
#include "CRealizingTestingItem.h"

// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableTaskRealizationPossibleInstancesMergingAdapter.h"

// Logger includes
#include "Logger/CLogger.h"

#ifdef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

#define KONCLUCE_DEBUG_PRINTING_REALIZATION_CONCEPT_INSTANCE_TESTING_TIME(a) a

#else

#define KONCLUCE_DEBUG_PRINTING_REALIZATION_CONCEPT_INSTANCE_TESTING_TIME(a) a

#endif


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {

			using namespace Kernel::Task;


			/*! 
			 *
			 *		\class		CIndividualInstanceTestingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualConceptInstanceTestingItem : public CRealizingTestingItem {
				// public methods
				public:
					//! Constructor
					CIndividualConceptInstanceTestingItem(COntologyRealizingItem* preproItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem, COntologyRealizingDynamicRequirmentProcessingData* procData = nullptr, CSatisfiableTaskRealizationPossibleInstancesMergingAdapter* possInstanceMergingDataAdapter = nullptr);
					~CIndividualConceptInstanceTestingItem();

					virtual REALIZINGTESTINGTYPE getRealizingTestingType();

					COptimizedKPSetConceptInstancesItem* getInstancesItem();
					COptimizedKPSetIndividualItem* getInstantiatedItem();

					CSatisfiableTaskRealizationPossibleInstancesMergingAdapter* getPossibleInstanceMergingDataAdapter();

					KONCLUCE_DEBUG_PRINTING_REALIZATION_CONCEPT_INSTANCE_TESTING_TIME(QTime startTime;)

				// protected methods
				protected:

				// protected variables
				protected:
					COntologyRealizingItem* mOntologyPreproItem;
					COptimizedKPSetConceptInstancesItem* mInstancesItem;
					COptimizedKPSetIndividualItem* mInstantiatedItem;
					CSatisfiableTaskRealizationPossibleInstancesMergingAdapter* mPossInstanceMergingDataAdapter;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CINDIVIDUALCONCEPTINSTANCETESTINGITEM_H
