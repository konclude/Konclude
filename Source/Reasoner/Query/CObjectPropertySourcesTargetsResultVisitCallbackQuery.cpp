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

#include "CObjectPropertySourcesTargetsResultVisitCallbackQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CObjectPropertySourcesTargetsResultVisitCallbackQuery::CObjectPropertySourcesTargetsResultVisitCallbackQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CIndividual* individual, CRole* role, bool sources, bool targets, const QString& objectPropertyTargetQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mIndividual = individual;
				mRole = role;

				mIndividualName = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
				mRoleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());

				queryName = objectPropertyTargetQueryName;
				if (queryName.isEmpty()) {
					QString sourcesTargetsString = QString("Sources/Targets");
					if (mSources && !mTargets) {
						sourcesTargetsString = QString("Sources");
					} else if (!mSources && mTargets) {
						sourcesTargetsString = QString("Targets");
					}
					if (!mIndividualName.isEmpty() && !mRoleName.isEmpty()) {
						queryString = QString("Get Object Property %1").arg(sourcesTargetsString);
					} else {
						queryString = QString("Get Object Property %1 for Individual '%2'").arg(sourcesTargetsString).arg(mIndividualName);
					}
				}
				mResult = nullptr;

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mSources = sources;
				mTargets = targets;

				mRequiresRoleRealisation = true;
				mRequiresSameIndividualRealisation = true;

				mCalcConfig = configuration;
			}

			CObjectPropertySourcesTargetsResultVisitCallbackQuery::~CObjectPropertySourcesTargetsResultVisitCallbackQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CObjectPropertySourcesTargetsResultVisitCallbackQuery::getQueryResult() {
				return mResult;
			}



			CQueryResult *CObjectPropertySourcesTargetsResultVisitCallbackQuery::constructResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
				}
				mRoleRealization = nullptr;
				if (realization) {
					mRoleRealization = realization->getRoleRealization();
					if (mRoleRealization) {
						mResult = new CSucceedQueryResult();
					}
				}
				return mResult;
			}

			bool CObjectPropertySourcesTargetsResultVisitCallbackQuery::callbackVisitingIndividuals(CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
				if (mRoleRealization) {
					CSetOfEntityExpressionSetResultVisitingCallbackGenerator resultGenerator(visitingCallback,mOntology);
					if (mSources) {
						mRoleRealization->visitSourceIndividuals(mIndividual,mRole,&resultGenerator);
					}
					if (mTargets) {
						mRoleRealization->visitTargetIndividuals(mIndividual,mRole,&resultGenerator);
					}
					return true;
				}
				return false;
			}



			QString CObjectPropertySourcesTargetsResultVisitCallbackQuery::getQueryName() {
				return queryName;
			}

			QString CObjectPropertySourcesTargetsResultVisitCallbackQuery::getQueryString() {
				return queryString;
			}

			bool CObjectPropertySourcesTargetsResultVisitCallbackQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CObjectPropertySourcesTargetsResultVisitCallbackQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Individuals");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CObjectPropertySourcesTargetsResultVisitCallbackQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
