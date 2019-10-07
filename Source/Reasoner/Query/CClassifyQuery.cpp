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

#include "CClassifyQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CClassifyQuery::CClassifyQuery(CConcreteOntology *premisingTaxonomyOntology, CConfigurationBase *configuration, const QString &subClassHierarchyQueryName)
					: CTaxonomyPremisingQuery(premisingTaxonomyOntology,configuration) {
				queryName = subClassHierarchyQueryName;
				queryString = QString("Classify Knowledgebase");

				mTaxCalcError = false;
				mQueryConstructError = false;

				mResult = nullptr;

			}

			CClassifyQuery::~CClassifyQuery() {
				delete mResult;
			}


			CQueryResult *CClassifyQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CClassifyQuery::constructResult(CTaxonomy *taxonomy) {
				if (taxonomy) {
					mResult = new CSucceedQueryResult();
				}
				return mResult;
			}


			QString CClassifyQuery::getQueryName() {
				return queryName;
			}

			QString CClassifyQuery::getQueryString() {
				return queryString;
			}

			bool CClassifyQuery::hasAnswer() {
				return mResult != nullptr;
			}

			QString CClassifyQuery::getAnswerString() {
				if (mResult) {
					return QString("Knowledgebase classified");
				} else {
					return QString("Knowledgebase not classified");
				}
			}

			bool CClassifyQuery::hasError() {
				return mTaxCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
