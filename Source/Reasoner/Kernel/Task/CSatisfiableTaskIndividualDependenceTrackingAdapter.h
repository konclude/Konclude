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

#ifndef KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKINDIVIDUALDEPENDENCETRACKINGADAPTER_H
#define KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKINDIVIDUALDEPENDENCETRACKINGADAPTER_H

// Libraries includes


// Namespace includes
#include "TaskSettings.h"


// Other includes
#include "Reasoner/Consistiser/CIndividualDependenceTrackingObserver.h"
#include "Reasoner/Consistiser/CIndividualDependenceTrackingMarker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Consistiser;

		namespace Kernel {

			namespace Task {


				/*! 
				 *
				 *		\class		CSatisfiableTaskIndividualDependenceTrackingAdapter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableTaskIndividualDependenceTrackingAdapter {
					// public methods
					public:
						//! Constructor
						CSatisfiableTaskIndividualDependenceTrackingAdapter(CIndividualDependenceTrackingObserver* indiDepTrackObserver, CIndividualDependenceTrackingMarker* indiDepTrackMarker = nullptr);

						CSatisfiableTaskIndividualDependenceTrackingAdapter* setIndividualDependenceTrackingObserver(CIndividualDependenceTrackingObserver* indiDepTrackObserver);
						CIndividualDependenceTrackingObserver* getIndividualDependenceTrackingObserver();

						CSatisfiableTaskIndividualDependenceTrackingAdapter* setIndividualDependenceTrackingMarker(CIndividualDependenceTrackingMarker* indiDepTrackMarker);
						CIndividualDependenceTrackingMarker* getIndividualDependenceTrackingMarker();


					// protected methods
					protected:

					// protected variables
					protected:
						CIndividualDependenceTrackingObserver* mIndiDepTrackObserver;
						CIndividualDependenceTrackingMarker* mIndiDepTrackMarker;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKINDIVIDUALDEPENDENCETRACKINGADAPTER_H
