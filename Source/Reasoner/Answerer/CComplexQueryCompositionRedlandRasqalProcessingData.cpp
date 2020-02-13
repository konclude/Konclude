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

#include "CComplexQueryCompositionRedlandRasqalProcessingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CComplexQueryCompositionRedlandRasqalProcessingData::CComplexQueryCompositionRedlandRasqalProcessingData(CRedlandRasqalBGPsCompositionQuery* query, CCallbackData* callback) : CComplexQueryCompositionProcessingData(query, callback) {
				mQuery = query;
				mProcessingError = false;
				mResolver = nullptr;
				mResultDirectlyFromSubQueryStreamed = false;
				mSubQueriesGenerated = false;
				mQueryProcessed = false;
				mDependentOntologiesLoaded = false;
				mDependentOntologiesIdentified = false;
			}


			CComplexQueryCompositionRedlandRasqalProcessingData::~CComplexQueryCompositionRedlandRasqalProcessingData() {
				delete mResolver;
			}

			CRedlandRasqalBGPsCompositionQuery* CComplexQueryCompositionRedlandRasqalProcessingData::getRedlandRasqalQuery() {
				return mQuery;
			}


			QVector<CRedlandRasqalSubQueryProcessingData*>* CComplexQueryCompositionRedlandRasqalProcessingData::getSubQueryProcessingDataVector() {
				return &mSubQueryProcDataVec;
			}

			CRedlandRasqalSubQueryProcessingData* CComplexQueryCompositionRedlandRasqalProcessingData::getSubQueryProcessingData(cint64 subQueryId) {
				return mSubQueryProcDataVec[subQueryId];
			}

			CRedlandRasqalModelSubQueryResolver* CComplexQueryCompositionRedlandRasqalProcessingData::getResolver() {
				return mResolver;
			}

			CComplexQueryCompositionRedlandRasqalProcessingData* CComplexQueryCompositionRedlandRasqalProcessingData::setResolver(CRedlandRasqalModelSubQueryResolver* resolver) {
				mResolver = resolver;
				return this;
			}


			QStringList* CComplexQueryCompositionRedlandRasqalProcessingData::getRedlandRasqalErrorWarningMessageList() {
				return &mMessageList;
			}


			bool CComplexQueryCompositionRedlandRasqalProcessingData::areResultDirectlyFromSubQueryStreamed() {
				return mResultDirectlyFromSubQueryStreamed;
			}

			CComplexQueryCompositionRedlandRasqalProcessingData* CComplexQueryCompositionRedlandRasqalProcessingData::setResultDirectlyFromSubQueryStreamed(bool directlyStreamed) {
				mResultDirectlyFromSubQueryStreamed = directlyStreamed;
				return this;
			}


			bool CComplexQueryCompositionRedlandRasqalProcessingData::hasSubQueriesGenerated() {
				return mSubQueriesGenerated;
			}

			CComplexQueryCompositionRedlandRasqalProcessingData* CComplexQueryCompositionRedlandRasqalProcessingData::setSubQueriesGenerated(bool generated) {
				mSubQueriesGenerated = generated;
				return this;
			}

			bool CComplexQueryCompositionRedlandRasqalProcessingData::isQueryProcessed() {
				return mQueryProcessed;
			}

			CComplexQueryCompositionRedlandRasqalProcessingData* CComplexQueryCompositionRedlandRasqalProcessingData::setQueryProcessed(bool processed) {
				mQueryProcessed = processed;
				return this;
			}

			bool CComplexQueryCompositionRedlandRasqalProcessingData::isDependentOntologiesLoaded() {
				return mDependentOntologiesLoaded;
			}

			CComplexQueryCompositionRedlandRasqalProcessingData* CComplexQueryCompositionRedlandRasqalProcessingData::setDependentOntologiesLoaded(bool loaded) {
				mDependentOntologiesLoaded = loaded;
				return this;
			}

			bool CComplexQueryCompositionRedlandRasqalProcessingData::isDependentOntologiesIdentified() {
				return mDependentOntologiesIdentified;
			}

			CComplexQueryCompositionRedlandRasqalProcessingData* CComplexQueryCompositionRedlandRasqalProcessingData::setDependentOntologiesIdentified(bool identified) {
				mDependentOntologiesIdentified = identified;
				return this;
			}


			bool CComplexQueryCompositionRedlandRasqalProcessingData::hasRemainingSubQueriesProcessing() {
				return !mRemainingSubQueriesProcessingSet.isEmpty();
			}

			cint64 CComplexQueryCompositionRedlandRasqalProcessingData::getRemainingSubQueriesProcessingCount() {
				return mRemainingSubQueriesProcessingSet.count();
			}

			QSet<CRedlandRasqalSubQueryProcessingData*>* CComplexQueryCompositionRedlandRasqalProcessingData::getRemainingSubQueriesProcessingSet() {
				return &mRemainingSubQueriesProcessingSet;
			}

			QSet<cint64>* CComplexQueryCompositionRedlandRasqalProcessingData::getProcessedSubQueryIdSet() {
				return &mProcessedSubQueryIdSet;
			}

			QHash<CQuery*, CRedlandRasqalSubQueryProcessingData*>* CComplexQueryCompositionRedlandRasqalProcessingData::getSubQueryDataHash() {
				return &mSubQueryDataHash;
			}

			QHash<cint64, CRedlandRasqalSubQueryProcessingData*>* CComplexQueryCompositionRedlandRasqalProcessingData::getIdSubQueryDataHash() {
				return &mIdSubQueryDataHash;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
