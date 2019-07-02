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

#ifndef KONCLUDE_REASONER_ANSWERER_CCOMPLEXCONCEPTSTEPCOMPUTATIONPROCESS_H
#define KONCLUDE_REASONER_ANSWERER_CCOMPLEXCONCEPTSTEPCOMPUTATIONPROCESS_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CRequirementWaitingDependencyData.h"



// Other includes
#include "Utilities/UtilitiesSettings.h"



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Answerer {


			enum ComplexConceptStepComputationProcessType {
				COMPUTATIONTYPESATISFIABILITY = 0, COMPUTATIONTYPESUPERCLASSNODES = 1, COMPUTATIONTYPESUBCLASSNODES = 2, COMPUTATIONTYPEEQUIVALENTCLASSNODES = 3, COMPUTATIONTYPEINSTANCES = 4, REALIZATIONTYPESUBCLASS = 5
			};

			static const cint64 COMPLEXCONCEPTSTEPCOMPUTATIONPROCESSTYPECOUNT = 6;



			/*! 
			 *
			 *		\class		CComplexConceptStepComputationProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CComplexConceptStepComputationProcess : public CRequirementWaitingDependencyData {
				// public methods
				public:
					

					//! Constructor
					CComplexConceptStepComputationProcess(ComplexConceptStepComputationProcessType computationType);

					~CComplexConceptStepComputationProcess();

					ComplexConceptStepComputationProcessType getComputationType();

					
					bool isComputationProcessFinished();
					bool isComputationProcessStarted();
					bool isComputationProcessQueued();
					bool isComputationProcessRequired();
					bool isComputationProcessProcessing();

					CComplexConceptStepComputationProcess* setComputationProcessStarted(bool started);
					CComplexConceptStepComputationProcess* setComputationProcessFinished(bool finished);
					CComplexConceptStepComputationProcess* setComputationProcessQueued(bool queued);
					CComplexConceptStepComputationProcess* setComputationProcessRequired(bool required);
					CComplexConceptStepComputationProcess* setComputationProcessProcessing(bool processing);



					CComplexConceptStepComputationProcess* incCurrentlyRunningComputationCount(cint64 incCount = 1);
					CComplexConceptStepComputationProcess* decCurrentlyRunningComputationCount(cint64 decCount = 1);
					CComplexConceptStepComputationProcess* incFinishedComputationCount(cint64 incCount = 1);
					CComplexConceptStepComputationProcess* incDerivedComputationCount(cint64 incCount = 1);

					cint64 getCurrentlyRunningComputationCount();
					cint64 getFinishedComputationCount();
					cint64 getDerivedComputationCount();


				// protected methods
				protected:

				// protected variables
				protected:
					ComplexConceptStepComputationProcessType mComputationType;
					bool mFinished;
					bool mStarted;
					bool mRequired;
					bool mQueued;
					bool mProcessing;

					cint64 mCurrentlyRunningComputationCount;
					cint64 mFinishedComputationCount;
					cint64 mDerivedComputationCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CCOMPLEXCONCEPTSTEPCOMPUTATIONPROCESS_H
