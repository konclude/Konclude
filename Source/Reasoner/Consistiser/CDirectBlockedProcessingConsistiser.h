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

#ifndef KONCLUDE_REASONER_CONSISTISER_CDIRECTBLOCKEDPROCESSINGCONSISTISER_H
#define KONCLUDE_REASONER_CONSISTISER_CDIRECTBLOCKEDPROCESSINGCONSISTISER_H

// Libraries includes


// Namespace includes
#include "ConsistiserSettings.h"
#include "CConsistiser.h"
#include "CConsistenceObserver.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"

#include "Control/Command/CConfigManagerReader.h"

#include "Reasoner/Kernel/Task/CTaskPreyingAdapter.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Control::Command;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Generator;
		using namespace Kernel::Task;

		namespace Consistiser {


			/*! 
			 *
			 *		\class		CDirectBlockedProcessingConsistiser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDirectBlockedProcessingConsistiser : public CConsistiser, public CConsistenceObserver {
				// public methods
				public:
					//! Constructor
					CDirectBlockedProcessingConsistiser();

					//! Destructor
					virtual ~CDirectBlockedProcessingConsistiser();


					virtual bool determineConsistency(CConcreteOntology* onto, CConfiguration *config);

					virtual CConsistenceObserver* tellConsistenceData(CConsistenceData* consData);


				// protected methods
				protected:

				// protected variables
				protected:
					CConsistence* mConsistence;
					CTaskPreyingAdapter mConsAdapter;
					QMutex mSynMutex;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CDIRECTBLOCKEDPROCESSINGCONSISTISER_H
