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

#ifndef KONCLUDE_REASONER_REALIZER_CPOSSIBLEINSTANCESINDIVIDUALSMERGINGDATA_H
#define KONCLUDE_REASONER_REALIZER_CPOSSIBLEINSTANCESINDIVIDUALSMERGINGDATA_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "CPossibleInstancesIndividualsMergingLinker.h"


// Other includes
#include "Reasoner/Ontology/CIndividual.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetConceptInstantiatedItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPossibleInstancesIndividualsMergingData {
				// public methods
				public:
					//! Constructor
					CPossibleInstancesIndividualsMergingData(cint64 maxMergingAttemptCount = 3);

					CPossibleInstancesIndividualsMergingLinker* getPossibleInstancesIndividualsMergingLinker();
					CPossibleInstancesIndividualsMergingData* addPossibleInstancesIndividualsMergingLinker(CPossibleInstancesIndividualsMergingLinker* linker);

					cint64 getMaxMergingAttemptCount();

					cint64 getPossibleMergingIndividualCount();
					cint64 getMergingAttemptCount();
					cint64 getMergingSuccessCount();
					cint64 getMergingStreakFailCount();

					CPossibleInstancesIndividualsMergingData* incMergingAttempt();
					CPossibleInstancesIndividualsMergingData* incMergingSuccess();
					CPossibleInstancesIndividualsMergingData* incMergingStreakFails();

					bool areAllPossibleInstancesIndividualsNonMerging();
					CPossibleInstancesIndividualsMergingData* setAllPossibleInstancesIndividualsNonMerging(bool allNonMergeable);

				// protected methods
				protected:

				// protected variables
				protected:
					CPossibleInstancesIndividualsMergingLinker* mPossibleInstancesIndividualsMergingLinker;


					cint64 mMaxMergingAttemptCount;
					cint64 mPossibleMergingIndiCount;
					QAtomicInt mMergingAttemptCount;
					QAtomicInt mMergingSuccessCount;
					QAtomicInt mMergingStreakFailCount;

					bool mAllPossibleInstancesIndividualsNonMerging;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CPOSSIBLEINSTANCESINDIVIDUALSMERGINGDATA_H
