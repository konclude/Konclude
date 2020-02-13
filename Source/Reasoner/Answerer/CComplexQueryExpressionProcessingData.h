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

#ifndef KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYEXPRESSIONPROCESSINGDATA_H
#define KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYEXPRESSIONPROCESSINGDATA_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CComplexConceptStepComputationProcess.h"
#include "CAnsweringHandlingStatistics.h"
#include "CComplexQueryProcessingData.h"



// Other includes
#include "Reasoner/Query/CComplexAnsweringExpressionQuery.h"
#include "Reasoner/Query/CComplexQueryExtendibleData.h"

#include "Concurrent/Callback/CCallbackData.h"

#include "Reasoner/Answerer/Conclusion/CAbstractComplexQueryFinishingHandler.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {

			using namespace Conclusion;

			/*! 
			 *
			 *		\class		CComplexQueryExpressionProcessingData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CComplexQueryExpressionProcessingData : public CComplexQueryProcessingData {
				// public methods
				public:
					//! Constructor
					CComplexQueryExpressionProcessingData(CComplexAnsweringExpressionQuery* query = nullptr, CCallbackData* callback = nullptr);

					~CComplexQueryExpressionProcessingData();

					
					CComplexAnsweringExpressionQuery* getQuery();


					bool isWaitingComputationStep();
					CComplexQueryExpressionProcessingData* setComputationStepFinished(CComplexConceptStepComputationProcess* computationStep);
					CComplexQueryExpressionProcessingData* setComputationStepWaiting(CComplexConceptStepComputationProcess* computationStep);

					COptimizedComplexConceptItem* getConceptItem();
					CComplexQueryExpressionProcessingData* setConceptItem(COptimizedComplexConceptItem* conceptItem);




					bool isBuildingVariableItem();
					CComplexQueryExpressionProcessingData* incBuildingVariableItem(cint64 count = 1);
					CComplexQueryExpressionProcessingData* decBuildingVariableItem(cint64 count = 1);

					COptimizedComplexBuildingVariableCompositionsItem* getVariableBuildingItem();
					CComplexQueryExpressionProcessingData* setVariableBuildingItem(COptimizedComplexBuildingVariableCompositionsItem* variableBuildingItem);


					bool isComputationError();
					CComplexQueryExpressionProcessingData* setComputationError(bool error);



					bool hasComputation();



					bool hasEntailmentComputation();
					CComplexQueryExpressionProcessingData* incEntailmentComputation(cint64 count = 1);
					CComplexQueryExpressionProcessingData* decEntailmentComputation(cint64 count = 1);



					CAbstractComplexQueryFinishingHandler* getFinishingHandler();
					CComplexQueryExpressionProcessingData* setFinishingHandler(CAbstractComplexQueryFinishingHandler* finishingHandler);



					cint64 getOffsetSkippedMappingCount();
					CComplexQueryExpressionProcessingData* setOffsetSkippedMappingCount(cint64 skippedMappingCount);
					CComplexQueryExpressionProcessingData* incOffsetSkippedMappingCount(cint64 incSkippedMappingCount);

					CComplexQueryExtendibleData* getQueryExtendibleData();
					CComplexQueryExpressionProcessingData* setQueryExtendibleData(CComplexQueryExtendibleData* extData);


				// protected methods
				protected:

				// protected variables
				protected:
					CComplexAnsweringExpressionQuery* mQuery;

					cint64 mWaitingComputationStepsCount;
					cint64 mWaitingEntailmentComputationsCount;


					cint64 mBuildingVariableItemCount;
					COptimizedComplexBuildingVariableCompositionsItem* mVariableBuildingItem;

					COptimizedComplexConceptItem* mConceptItem;
					bool mComputationError;

					
					CAbstractComplexQueryFinishingHandler* mFinishingHandler;


					cint64 mOffsetSkippedMappingCount;

					CComplexQueryExtendibleData* mExtendibleData;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYEXPRESSIONPROCESSINGDATA_H
