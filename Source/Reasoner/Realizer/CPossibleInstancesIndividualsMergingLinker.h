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

#ifndef KONCLUDE_REASONER_REALIZER_CPOSSIBLEINSTANCESINDIVIDUALSMERGINGLINKER_H
#define KONCLUDE_REASONER_REALIZER_CPOSSIBLEINSTANCESINDIVIDUALSMERGINGLINKER_H

// Libraries includes
#include <QAtomicInt>


// Namespace includes
#include "RealizerSettings.h"


// Other includes
#include "Reasoner/Ontology/CIndividual.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		CPossibleInstancesIndividualsMergingLinker
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPossibleInstancesIndividualsMergingLinker : public CXLinker<cint64> {
				// public methods
				public:
					//! Constructor
					CPossibleInstancesIndividualsMergingLinker();

					CPossibleInstancesIndividualsMergingLinker* initPossibleInstancesIndividualsMergingLinker(cint64 mergingIndiId, CPossibleInstancesIndividualsMergingData* mergingData);


					CPossibleInstancesIndividualsMergingLinker* getNextPossibleInstancesIndividualsMergingLinker();

					cint64 getMergingIndividualId();
					bool isSatisfiableMerged();
					CPossibleInstancesIndividualsMergingLinker* incMergingAttempt();
					cint64 getMergingAttemptCount();

					bool isFullTestingScheduled();
					CPossibleInstancesIndividualsMergingLinker* setFullTestingScheduled();
					CPossibleInstancesIndividualsMergingLinker* setSatisfiableMerged();

					CPossibleInstancesIndividualsMergingData* getMergingData();

				// protected methods
				protected:

				// protected variables
				protected:
					bool mSatisfiableMerged;
					bool mFullTestingScheduled;
					QAtomicInt mMergingAttemptCount;

					CPossibleInstancesIndividualsMergingData* mMergingData;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CPOSSIBLEINSTANCESINDIVIDUALSMERGINGLINKER_H
