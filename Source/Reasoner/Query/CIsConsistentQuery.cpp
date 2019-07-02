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

#include "CIsConsistentQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIsConsistentQuery::CIsConsistentQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const QString& consistentQueryName)
					: CConsistencePremisingQuery(ontology,configuration) {

				queryName = consistentQueryName;
				queryString = QString("Is ontology consistent?");
				mResult = nullptr;
			}

			CIsConsistentQuery::~CIsConsistentQuery() {
				if (mResult) {
					delete mResult;
				}
			}


			CQueryResult* CIsConsistentQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CIsConsistentQuery::constructResult(CConsistence* consistence) {
				return constructConsistenceResult(consistence);
			}


			CBooleanQueryResult* CIsConsistentQuery::constructConsistenceResult(CConsistence* consistence) {
				if (consistence) {					
					mResult = new CBooleanQueryResult(consistence->isOntologyConsistent());
				}
				return mResult;
			}


			QString CIsConsistentQuery::getQueryName() {
				return queryName;
			}

			QString CIsConsistentQuery::getQueryString() {
				return queryString;
			}

			bool CIsConsistentQuery::hasAnswer() {
				return mResult != 0;
			}


			CQuery* CIsConsistentQuery::addQueryError(CQueryError* queryError) {
				if (CQueryInconsitentOntologyError::hasInconsistentOntologyError(queryError)) {
					mResult = new CBooleanQueryResult(false);
				}
				return CQuery::addQueryError(queryError);
			}


			QString CIsConsistentQuery::getAnswerString() {
				if (!mResult) {
					return QString("Consistency Unknown");
				} else {
					return mResult->getQueryResultString();
				}
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
