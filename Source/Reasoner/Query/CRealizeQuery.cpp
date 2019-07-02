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

				mRequiresConceptRealisation = CConfigDataReader::readConfigBoolean(configuration,"Konclude.Calculation.Realization.RealizePrecomputation.ConceptInstances",true);
				mRequiresSameIndividualRealisation = CConfigDataReader::readConfigBoolean(configuration,"Konclude.Calculation.Realization.RealizePrecomputation.SameIndividuals",true);
				mRequiresRoleRealisation = CConfigDataReader::readConfigBoolean(configuration,"Konclude.Calculation.Realization.RealizePrecomputation.RoleInstances",false);


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
