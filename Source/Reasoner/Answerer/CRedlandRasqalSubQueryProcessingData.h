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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYPROCESSINGDATA_H
#define KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYPROCESSINGDATA_H

// Libraries includes
#include <Redland.h>


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringHandlingStatistics.h"
#include "CComplexQueryCompositionProcessingData.h"
#include "CRedlandRasqalSubQueryBoundNodeResolvingHashData.h"



// Other includes
#include "Reasoner/Query/CComplexAnsweringExpressionQuery.h"
#include "Reasoner/Query/CVariableBindingsAnswersResultIterator.h"
#include "Reasoner/Query/CVariableBindingsAnswersResult.h"
#include "Reasoner/Query/CBooleanQueryResult.h"

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
			 *		\class		CRedlandRasqalSubQueryProcessingData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandRasqalSubQueryProcessingData {
				// public methods
				public:
					//! Constructor
					CRedlandRasqalSubQueryProcessingData(cint64 subQueryId, CComplexAnsweringExpressionQuery* subQuery, rasqal_graph_pattern* subqueryGraphPattern, int gpTripleStartPos, int gpTriplesEndPos, librdf_world* world);

					~CRedlandRasqalSubQueryProcessingData();

					
					CComplexAnsweringExpressionQuery* getSubQuery();


					bool isProcessed();
					CRedlandRasqalSubQueryProcessingData* setProcessed(bool processed);

					bool isProcessing();
					CRedlandRasqalSubQueryProcessingData* setProcessing(bool processing);

					cint64 getSubQueryId();


					CVariableBindingsAnswersResultIterator* getAnswersIterator();
					CVariableBindingsResultIterator* getVariableBindingsIterator();

					CRedlandRasqalSubQueryProcessingData* setAnswersIterator(CVariableBindingsAnswersResultIterator* answersIterator);
					CRedlandRasqalSubQueryProcessingData* setVariableBindingsIterator(CVariableBindingsResultIterator* variableBindingsIterator);


					CVariableBindingResult* getCurrentVariableBinding();
					CRedlandRasqalSubQueryProcessingData* setCurrentVariableBinding(CVariableBindingResult* varBin);

					librdf_world* getRedlandWorld();


					librdf_node* getDefaultReturningNode();

					CRedlandRasqalSubQueryProcessingData* setDefaultReturningNode(librdf_node* node);



					bool isInitialized();
					CRedlandRasqalSubQueryProcessingData* setInitialized(bool initialized);

					cint64 getNextReturningPosition();
					CRedlandRasqalSubQueryProcessingData* setNextReturningPosition(cint64 pos);


					QVector<CRedlandRasqalSubQueryTripleResolvingData*>* getTriplesResolvingDataVector();
					QVector<CVariableBindingResult*>* getCurrentBindingsVector();
					QVector<CVariableBindingResult*>* getNextBindingsVector();

					QStringList* getTriplesRemainingBindingVariableList();
					cint64 getNextBindingsVariablePosition();
					CRedlandRasqalSubQueryProcessingData* incNextBindingsVariablePosition(cint64 incCount = 1);



					rasqal_graph_pattern* getSubqueryGraphPattern();
					int getGPTripleStartPos();
					int getGPTriplesEndPos();

					cint64 getBindingsCount();
					CRedlandRasqalSubQueryProcessingData* setBindingsCount(cint64 count);


					QVector<rasqal_literal*>* getBindingPositionVariableBoundRedlandNodeVector();
					bool hasBoundVariables();
					CRedlandRasqalSubQueryProcessingData* setBoundVariables(bool boundVariables);


					CRedlandRasqalSubQueryBoundNodeResolvingHashData* getBoundResolvingDataBase();

					CRedlandRasqalSubQueryBoundNodeResolvingHashData* getLastBoundResolvingData();
					CRedlandRasqalSubQueryProcessingData* setLastBoundResolvingData(CRedlandRasqalSubQueryBoundNodeResolvingHashData* resolvedData);

					CRedlandRasqalSubQueryProcessingData* deleteBoundResolvingData();


					CVariableBindingsAnswersResult* getBaseAnswerResult();
					CRedlandRasqalSubQueryProcessingData* setBaseAnswerResult(CVariableBindingsAnswersResult* baseAnswerResult);

					bool isBooleanSubQuery();
					bool getBooleanQueryResult();

					cint64 getVisitedResultCountStatistics();
					cint64 getResultOrderingCountStatistics();
					cint64 getTotalResultCountStatistics();
					CRedlandRasqalSubQueryProcessingData* incVisitedResultCountStatistics(cint64 incCount = 1);
					CRedlandRasqalSubQueryProcessingData* incResultOrderingCountStatistics(cint64 incCount = 1);
					CRedlandRasqalSubQueryProcessingData* setTotalResultCountStatistics(cint64 count);


					cint64 getFirstTripleReturningBinding();
					CRedlandRasqalSubQueryProcessingData* setFirstTripleReturningBinding(cint64 triplePos);

					QSet<cint64>* getDependentSubQueryIdSet();

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mVisitedCount = 0;
					cint64 mOrderingCount = 0;
					cint64 mTotalCount = 0;

					int mGPTripleStartPos;
					int mGPTriplesEndPos;

					cint64 mFirstTripleReturningBinding;


					bool mProcessed;
					bool mProcessing;
					bool mBooleanSubQuery;
					bool mBooleanQueryResult;
					bool mBooleanSubQueryChecked;
					cint64 mSubQueryId;
					cint64 mBindingsCount;
					CComplexAnsweringExpressionQuery* mSubQuery;
					rasqal_graph_pattern* mSubqueryGraphPattern;
					librdf_world* mRedlandWorld;

					CVariableBindingsAnswersResultIterator* mAnswersIt;
					CVariableBindingsResultIterator* mVarBindIt;

					CVariableBindingResult* mCurrentVariableBinding;

					QVector<CVariableBindingResult*> mCurrentBindingsVec;
					QVector<CVariableBindingResult*> mNextBindingsVec;
					QVector<CRedlandRasqalSubQueryTripleResolvingData*> mTriplesResolvingDataVec;
					QStringList mTriplesRemainingBindingVarList;

					librdf_node* mDefaultReturningNode;

					bool mInitialized;
					cint64 mNextReturningPosition;
					cint64 mNextBindingsVariablePosition;

					QVector<rasqal_literal*> mBindingsPositionVariableBoundVector;
					bool mBoundVariables;

					CRedlandRasqalSubQueryBoundNodeResolvingHashData* mBaseResolvingData;
					CRedlandRasqalSubQueryBoundNodeResolvingHashData* mLastResolvingData;

					CVariableBindingsAnswersResult* mBaseAnswerResult;

					QSet<cint64> mDependentSubQueryIdSet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYPROCESSINGDATA_H

#endif // KONCLUDE_REDLAND_INTEGRATION
