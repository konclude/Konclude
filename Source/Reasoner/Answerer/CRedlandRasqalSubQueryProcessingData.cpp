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

#include "CRedlandRasqalSubQueryProcessingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CRedlandRasqalSubQueryProcessingData::CRedlandRasqalSubQueryProcessingData(cint64 subQueryId, CComplexAnsweringExpressionQuery* subQuery, rasqal_graph_pattern* subqueryGraphPattern, int gpTripleStartPos, int gpTriplesEndPos, librdf_world* world) {
				mSubQuery = subQuery;
				mProcessed = false;
				mProcessing = false;
				mSubQueryId = subQueryId;
				mSubqueryGraphPattern = subqueryGraphPattern;
				mAnswersIt = nullptr;
				mVarBindIt = nullptr;
				mRedlandWorld = world;
				mDefaultReturningNode = nullptr;
				mInitialized = false;
				mNextReturningPosition = 0;
				mCurrentVariableBinding = nullptr;
				mNextBindingsVariablePosition = 0;
				mBindingsCount = 0;
				mBoundVariables = false;
				mBaseResolvingData = nullptr;
				mLastResolvingData = nullptr;
				mBaseAnswerResult = nullptr;
				mBooleanSubQuery = false;
				mBooleanQueryResult = false;
				mBooleanSubQueryChecked = false;
				mGPTripleStartPos = gpTripleStartPos;
				mGPTriplesEndPos = gpTriplesEndPos;
				mFirstTripleReturningBinding = 0;
			}


			CRedlandRasqalSubQueryProcessingData::~CRedlandRasqalSubQueryProcessingData() {
			}

			int CRedlandRasqalSubQueryProcessingData::getGPTripleStartPos() {
				return mGPTripleStartPos;
			}
			int CRedlandRasqalSubQueryProcessingData::getGPTriplesEndPos() {
				return mGPTriplesEndPos;
			}

			cint64 CRedlandRasqalSubQueryProcessingData::getVisitedResultCountStatistics() {
				return mVisitedCount;
			}

			cint64 CRedlandRasqalSubQueryProcessingData::getTotalResultCountStatistics() {
				return mTotalCount;
			}

			cint64 CRedlandRasqalSubQueryProcessingData::getResultOrderingCountStatistics() {
				return mOrderingCount;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::incVisitedResultCountStatistics(cint64 incCount) {
				mVisitedCount += incCount;
				return this;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::incResultOrderingCountStatistics(cint64 incCount) {
				mOrderingCount += incCount;
				return this;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setTotalResultCountStatistics(cint64 count) {
				mTotalCount = count;
				return this;
			}


			CComplexAnsweringExpressionQuery* CRedlandRasqalSubQueryProcessingData::getSubQuery() {
				return mSubQuery;
			}


			bool CRedlandRasqalSubQueryProcessingData::isBooleanSubQuery() {
				if (!mBooleanSubQueryChecked) {
					CQueryResult* queryResult = mSubQuery->getQueryResult();
					if (dynamic_cast<CBooleanQueryResult*>(queryResult)) {
						mBooleanSubQueryChecked = true;
						mBooleanSubQuery = true;
						mBooleanQueryResult = dynamic_cast<CBooleanQueryResult*>(queryResult)->getResult();
					}
				}
				return mBooleanSubQuery;
			}

			bool CRedlandRasqalSubQueryProcessingData::getBooleanQueryResult() {
				return mBooleanQueryResult;
			}


			bool CRedlandRasqalSubQueryProcessingData::isProcessed() {
				return mProcessed;
			}

			bool CRedlandRasqalSubQueryProcessingData::isProcessing() {
				return mProcessing;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setProcessed(bool processed) {
				mProcessed = processed;
				return this;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setProcessing(bool processing) {
				mProcessing = processing;
				return this;
			}

			cint64 CRedlandRasqalSubQueryProcessingData::getSubQueryId() {
				return mSubQueryId;
			}


			CVariableBindingsAnswersResultIterator* CRedlandRasqalSubQueryProcessingData::getAnswersIterator() {
				return mAnswersIt;
			}

			CVariableBindingsResultIterator* CRedlandRasqalSubQueryProcessingData::getVariableBindingsIterator() {
				return mVarBindIt;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setAnswersIterator(CVariableBindingsAnswersResultIterator* answersIterator) {
				mAnswersIt = answersIterator;
				return this;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setVariableBindingsIterator(CVariableBindingsResultIterator* variableBindingsIterator) {
				mVarBindIt = variableBindingsIterator;
				return this;
			}

			CVariableBindingResult* CRedlandRasqalSubQueryProcessingData::getCurrentVariableBinding() {
				return mCurrentVariableBinding;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setCurrentVariableBinding(CVariableBindingResult* varBin) {
				mCurrentVariableBinding = varBin;
				return this;
			}


			librdf_world* CRedlandRasqalSubQueryProcessingData::getRedlandWorld() {
				return mRedlandWorld;
			}


			librdf_node* CRedlandRasqalSubQueryProcessingData::getDefaultReturningNode() {
				return mDefaultReturningNode;
			}


			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setDefaultReturningNode(librdf_node* node) {
				mDefaultReturningNode = node;
				return this;
			}

			bool CRedlandRasqalSubQueryProcessingData::isInitialized() {
				return mInitialized;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setInitialized(bool initialized) {
				mInitialized = initialized;
				return this;
			}

			cint64 CRedlandRasqalSubQueryProcessingData::getNextReturningPosition() {
				return mNextReturningPosition;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setNextReturningPosition(cint64 pos) {
				mNextReturningPosition = pos;
				return this;
			}

			QVector<CRedlandRasqalSubQueryTripleResolvingData*>* CRedlandRasqalSubQueryProcessingData::getTriplesResolvingDataVector() {
				return &mTriplesResolvingDataVec;
			}

			QVector<CVariableBindingResult*>* CRedlandRasqalSubQueryProcessingData::getCurrentBindingsVector() {
				return &mCurrentBindingsVec;
			}

			QVector<CVariableBindingResult*>* CRedlandRasqalSubQueryProcessingData::getNextBindingsVector() {
				return &mNextBindingsVec;
			}

			QStringList* CRedlandRasqalSubQueryProcessingData::getTriplesRemainingBindingVariableList() {
				return &mTriplesRemainingBindingVarList;
			}

			cint64 CRedlandRasqalSubQueryProcessingData::getNextBindingsVariablePosition() {
				return mNextBindingsVariablePosition;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::incNextBindingsVariablePosition(cint64 incCount) {
				mNextBindingsVariablePosition += incCount;
				return this;
			}

			rasqal_graph_pattern* CRedlandRasqalSubQueryProcessingData::getSubqueryGraphPattern() {
				return mSubqueryGraphPattern;
			}


			cint64 CRedlandRasqalSubQueryProcessingData::getBindingsCount() {
				return mBindingsCount;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setBindingsCount(cint64 count) {
				mBindingsCount = count;
				return this;
			}


			QVector<rasqal_literal*>* CRedlandRasqalSubQueryProcessingData::getBindingPositionVariableBoundRedlandNodeVector() {
				return &mBindingsPositionVariableBoundVector;
			}

			bool CRedlandRasqalSubQueryProcessingData::hasBoundVariables() {
				return mBoundVariables;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setBoundVariables(bool boundVariables) {
				mBoundVariables = boundVariables;
				return this;
			}

			CRedlandRasqalSubQueryBoundNodeResolvingHashData* CRedlandRasqalSubQueryProcessingData::getBoundResolvingDataBase() {
				if (!mBaseResolvingData) {
					mBaseResolvingData = new CRedlandRasqalSubQueryBoundNodeResolvingHashData(dynamic_cast<CVariableBindingsAnswersResult*>(mSubQuery->getQueryResult()), false);
				}
				return mBaseResolvingData;
			}


			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::deleteBoundResolvingData() {
				delete mBaseResolvingData;
				mBaseResolvingData = nullptr;
				return this;
			}


			CRedlandRasqalSubQueryBoundNodeResolvingHashData* CRedlandRasqalSubQueryProcessingData::getLastBoundResolvingData() {
				return mLastResolvingData;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setLastBoundResolvingData(CRedlandRasqalSubQueryBoundNodeResolvingHashData* resolvedData) {
				mLastResolvingData = resolvedData;
				return this;
			}


			CVariableBindingsAnswersResult* CRedlandRasqalSubQueryProcessingData::getBaseAnswerResult() {
				return mBaseAnswerResult;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setBaseAnswerResult(CVariableBindingsAnswersResult* baseAnswerResult) {
				mBaseAnswerResult = baseAnswerResult;
				return this;
			}


			cint64 CRedlandRasqalSubQueryProcessingData::getFirstTripleReturningBinding() {
				return mFirstTripleReturningBinding;
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryProcessingData::setFirstTripleReturningBinding(cint64 triplePos) {
				mFirstTripleReturningBinding = triplePos;
				return this;
			}


			QSet<cint64>* CRedlandRasqalSubQueryProcessingData::getDependentSubQueryIdSet() {
				return &mDependentSubQueryIdSet;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REDLAND_INTEGRATION
