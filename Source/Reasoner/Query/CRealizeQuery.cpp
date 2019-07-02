/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CRealizeQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CRealizeQuery::CRealizeQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const QString& queryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mQueryName = queryName;
				queryString = QString("Realize Knowledgebase");

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mResult = nullptr;
			}

			CRealizeQuery::~CRealizeQuery() {
				delete mResult;
			}


			CQueryResult* CRealizeQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CRealizeQuery::constructResult(CRealization* realization) {
				if (realization) {
					mResult = new CSucceedQueryResult();
				} else {
					mQueryConstructError = true;
				}
				return mResult;
			}

			QString CRealizeQuery::getQueryName() {
				return mQueryName;
			}

			QString CRealizeQuery::getQueryString() {
				return queryString;
			}

			bool CRealizeQuery::hasAnswer() {
				return mResult != nullptr;
			}

			QString CRealizeQuery::getAnswerString() {
				if (mResult) {
					return QString("Knowledgebase realized");
				} else {
					return QString("Knowledgebase not realized");
				}
			}

			bool CRealizeQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
