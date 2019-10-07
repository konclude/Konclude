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

#ifndef KONCLUDE_REASONER_CONSISTISER_CIndividualPrecomputationTestingItem_H
#define KONCLUDE_REASONER_CONSISTISER_CIndividualPrecomputationTestingItem_H

// Libraries includes


// Namespace includes
#include "ConsistiserSettings.h"
#include "CPrecomputationTestingItem.h"

// Other includes
#include "Reasoner/Kernel/Task/CTaskPreyingAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskRepresentativeBackendUpdatingAdapter.h"

#include "Reasoner/Ontology/CIndividualReference.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Kernel::Task;

		namespace Consistiser {


			/*! 
			 *
			 *		\class		CIndividualPrecomputationTestingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualPrecomputationTestingItem : public CPrecomputationTestingItem {
				// public methods
				public:
					//! Constructor
					CIndividualPrecomputationTestingItem(CConsistenceObserver* observer, CIndividualPrecomputationCoordinationHash* indiComCoordHash, COntologyPrecomputationItem* preCompItem, bool* unstatFlagPointer = nullptr);

					virtual PRECOMPUTATIONTYPE getPrecomputationTestingType();

					CTaskPreyingAdapter* getTaskPreyingAdapter();
					CSatisfiableTaskRepresentativeBackendUpdatingAdapter* getRepresentativeCacheUpdatingAdapter();

					QList<CIndividualReference>* getIndividualComputationList();
					CIndividualPrecomputationTestingItem* addIndividualComputations(QList<CIndividualReference>& indiRefList);

					CIndividualPrecomputationCoordinationHash* getIndividualComputationCoordinationHash();
					CIndividualPrecomputationTestingItem* setIndividualComputationCoordinationHash(CIndividualPrecomputationCoordinationHash* indiComCoordHash);


				// protected methods
				protected:

				// protected variables
				protected:
					CTaskPreyingAdapter mTaskPreyingAdapter;
					CSatisfiableTaskRepresentativeBackendUpdatingAdapter mRepresentativeCacheUpdatingAdapter;
					CConsistenceObserver* mObserver;
					QList<CIndividualReference> mIndiComputationList;
					CIndividualPrecomputationCoordinationHash* mIndiCompCoordHash;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CIndividualPrecomputationTestingItem_H
