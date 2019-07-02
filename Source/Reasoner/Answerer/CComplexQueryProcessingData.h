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

#ifndef KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYPROCESSINGDATA_H
#define KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYPROCESSINGDATA_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CComplexConceptStepComputationProcess.h"



// Other includes
#include "Reasoner/Query/CComplexAnsweringQuery.h"

#include "Concurrent/Callback/CCallbackData.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {


			/*! 
			 *
			 *		\class		CComplexQueryProcessingData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CComplexQueryProcessingData {
				// public methods
				public:
					//! Constructor
					CComplexQueryProcessingData(CComplexAnsweringQuery* query = nullptr, CCallbackData* callback = nullptr);

					~CComplexQueryProcessingData();

					
					CComplexAnsweringQuery* getQuery();
					CCallbackData* getCallback();


					bool isWaitingComputationStep();
					CComplexQueryProcessingData* setComputationStepFinished(CComplexConceptStepComputationProcess* computationStep);
					CComplexQueryProcessingData* setComputationStepWaiting(CComplexConceptStepComputationProcess* computationStep);

					COptimizedComplexConceptItem* getConceptItem();
					CComplexQueryProcessingData* setConceptItem(COptimizedComplexConceptItem* conceptItem);




					bool isBuildingVariableItem();
					CComplexQueryProcessingData* incBuildingVariableItem(cint64 count = 1);
					CComplexQueryProcessingData* decBuildingVariableItem(cint64 count = 1);

					COptimizedComplexBuildingVariableCompositionsItem* getVariableBuildingItem();
					CComplexQueryProcessingData* setVariableBuildingItem(COptimizedComplexBuildingVariableCompositionsItem* variableBuildingItem);




					bool isUnsatisfiable();
					CComplexQueryProcessingData* setUnsatisfiable(bool unsat);


					bool hasComputation();



					bool hasEntailmentComputation();
					CComplexQueryProcessingData* incEntailmentComputation(cint64 count = 1);
					CComplexQueryProcessingData* decEntailmentComputation(cint64 count = 1);

					bool isProcessingStarted();
					CComplexQueryProcessingData* setProcessingStarted(bool started = true);

					bool isProcessingFinished();
					CComplexQueryProcessingData* setProcessingFinished(bool finished = true);

					qint64 getProcessingTime();


				// protected methods
				protected:

				// protected variables
				protected:
					CComplexAnsweringQuery* mQuery;
					CCallbackData* mCallback;

					cint64 mWaitingComputationStepsCount;
					cint64 mWaitingEntailmentComputationsCount;


					cint64 mBuildingVariableItemCount;
					COptimizedComplexBuildingVariableCompositionsItem* mVariableBuildingItem;

					COptimizedComplexConceptItem* mConceptItem;
					bool mUnsatisfiable;

					bool mProcessingStarted = false;
					bool mProcessingFinished = false;
					qint64 mProcessingTime = 0;
					QTime mProcessingTimer;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYPROCESSINGDATA_H
