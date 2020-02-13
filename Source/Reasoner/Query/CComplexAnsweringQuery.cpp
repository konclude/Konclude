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

#include "CComplexAnsweringQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CComplexAnsweringQuery::CComplexAnsweringQuery(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, CConfigurationBase* configuration, const QString& queryName) {
				mOntology = baseOntology;
				mExpressionsOntology = expressionOntology;
				mConfig = configuration;
				mQueryStats = nullptr;
				mQueryResult = nullptr;
				mQueryName = queryName;
			}

			CComplexAnsweringQuery::~CComplexAnsweringQuery() {
				delete mQueryStats;
			}

			CComplexAnsweringQuery* CComplexAnsweringQuery::setQueryResult(CQueryResult* queryResult) {
				if (mQueryResult) {
					delete mQueryResult;
				}
				mQueryResult = queryResult;
				return this;
			}

			CQueryResult* CComplexAnsweringQuery::getQueryResult() {
				return mQueryResult;
			}

			QString CComplexAnsweringQuery::getQueryName() {
				return mQueryName;
			}

			QString CComplexAnsweringQuery::getQueryString() {
				return "Complex-Answering-Query";
			}

			QString CComplexAnsweringQuery::getAnswerString() {
				return "Unknown";
			}

			bool CComplexAnsweringQuery::hasAnswer() {
				return mQueryResult != nullptr;
			}
			

			CConcreteOntology* CComplexAnsweringQuery::getBaseOntology() {
				return mOntology;
			}

			CConcreteOntology* CComplexAnsweringQuery::getExrpessionOntology() {
				return mExpressionsOntology;
			}

			CConfigurationBase* CComplexAnsweringQuery::getConfiguration() {
				return mConfig;
			}

			CComplexAnsweringQuery* CComplexAnsweringQuery::setQueryStatistics(CQueryStatistics* stats) {
				mQueryStats = stats;
				return this;
			}

			CQueryStatistics* CComplexAnsweringQuery::getQueryStatistics() {
				return mQueryStats;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
