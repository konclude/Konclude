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

#include "CVariableBindingsAnswersStreamingResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CVariableBindingsAnswersStreamingResult::CVariableBindingsAnswersStreamingResult(CVariableBindingsAnswersStreamingHandler* handler) {
				mHandler = handler;
				mResultCount = 0;
			}


			CVariableBindingsAnswersStreamingResult* CVariableBindingsAnswersStreamingResult::initResult(const QStringList& varNames) {
				mVariableNames = varNames;
				mHandler->initResultStreaming(varNames);
				return this;
			}


			CVariableBindingsAnswersStreamingResult::~CVariableBindingsAnswersStreamingResult() {
			}


			CVariableBindingsAnswersStreamingResult* CVariableBindingsAnswersStreamingResult::addResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality) {
				addReusedResultVariableBindings(varBindings, cardinality);
				delete varBindings;
				return this;
			}

			
			CVariableBindingsAnswersResult* CVariableBindingsAnswersStreamingResult::addReusedResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality) {
				mResultCount += cardinality;
				mHandler->streamResultVariableBindings(varBindings, cardinality);
				return this;
			}


			QString CVariableBindingsAnswersStreamingResult::getQueryResultString() {
				return QString();
			}


			bool CVariableBindingsAnswersStreamingResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (otherQueryResult) {
					CVariableBindingsAnswersStreamingResult* otherBoolQueryResult = dynamic_cast<CVariableBindingsAnswersStreamingResult*>(otherQueryResult);
					if (otherBoolQueryResult) {
						return true;
					} 
				}
				return false;
			}
			

			const QStringList& CVariableBindingsAnswersStreamingResult::getVariableNames() {
				return mVariableNames;
			}



			CVariableBindingsAnswersResultIterator* CVariableBindingsAnswersStreamingResult::getVariableBindingsAnswersIterator(CVariableBindingsAnswersResultIterator* reuseIt) {
				return nullptr;
			}


			cint64 CVariableBindingsAnswersStreamingResult::getResultCount() {
				return mResultCount;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
