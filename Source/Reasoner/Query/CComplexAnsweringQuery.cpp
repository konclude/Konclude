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

#include "CComplexAnsweringQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CComplexAnsweringQuery::CComplexAnsweringQuery(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, CConfigurationBase* configuration, const QString& queryName) {
				mOntology = baseOntology;
				mConfig = configuration;
				mQueryStats = nullptr;
				mExpressionsOntology = expressionOntology;
				mQueryResult = nullptr;
				mQueryName = queryName;
				mResultOrdering = nullptr;
				mResultFiltering = nullptr;
			}

			CComplexAnsweringQuery::~CComplexAnsweringQuery() {
				delete mQueryStats;
			}

			CComplexAnsweringQuery* CComplexAnsweringQuery::setQueryResult(CQueryResult* queryResult) {
				if (mQueryResult) {
					delete mQueryResult;
				}
				while (mResultOrdering) {
					CVariableBindingOrdering* tmpOrdering = mResultOrdering;
					mResultOrdering = mResultOrdering->getNext();
					delete tmpOrdering;
				}
				while (mResultFiltering) {
					CVariableBindingFiltering* tmpOrdering = mResultFiltering;
					mResultFiltering = mResultFiltering->getNext();
					delete tmpOrdering;
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

			
			CConcreteOntology* CComplexAnsweringQuery::getExrpessionOntology() {
				return mExpressionsOntology;
			}


			CVariableBindingOrdering* CComplexAnsweringQuery::getResultOrderingLinker() {
				return mResultOrdering;
			}

			CComplexAnsweringQuery* CComplexAnsweringQuery::setResultOrderingLinker(CVariableBindingOrdering* odering) {
				while (mResultOrdering) {
					CVariableBindingOrdering* tmpOrdering = mResultOrdering;
					mResultOrdering = mResultOrdering->getNext();
					delete tmpOrdering;
				}
				mResultOrdering = odering;
				return this;
			}

			CComplexAnsweringQuery* CComplexAnsweringQuery::addResultOrdering(CVariableBindingOrdering* odering) {
				if (mResultOrdering) {
					mResultOrdering->append(odering);
				} else {
					mResultOrdering = odering;
				}
				return this;
			}


			CVariableBindingFiltering* CComplexAnsweringQuery::getResultFilteringLinker() {
				return mResultFiltering;
			}

			CComplexAnsweringQuery* CComplexAnsweringQuery::setResultFilteringLinker(CVariableBindingFiltering* filtering) {
				while (mResultFiltering) {
					CVariableBindingFiltering* tmpOrdering = mResultFiltering;
					mResultFiltering = mResultFiltering->getNext();
					delete tmpOrdering;
				}
				mResultFiltering = filtering;
				return this;
			}

			CComplexAnsweringQuery* CComplexAnsweringQuery::addResultFiltering(CVariableBindingFiltering* filtering) {
				if (mResultFiltering) {
					mResultFiltering->append(filtering);
				} else {
					mResultFiltering = filtering;
				}
				return this;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
