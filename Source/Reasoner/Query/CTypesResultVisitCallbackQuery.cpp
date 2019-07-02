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

#include "CTypesResultVisitCallbackQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CTypesResultVisitCallbackQuery::CTypesResultVisitCallbackQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CIndividual* indi, bool direct, const QString& individualName, const QString& typeQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mIndividualName = individualName;
				mIndividual = indi;
				queryName = typeQueryName;
				if (mIndividualName.isEmpty()) {
					queryString = QString("Get Types");
				} else {
					queryString = QString("Get Types for individual '%1'").arg(mIndividualName);
				}
				mResult = nullptr;

				mRealizationCalcError = false;
				mQueryConstructError = false;
				mDirect = direct;

				mRequiresConceptRealisation = true;
				mRequiresSameIndividualRealisation = true;

				mCalcConfig = configuration;
			}

			CTypesResultVisitCallbackQuery::~CTypesResultVisitCallbackQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CTypesResultVisitCallbackQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CTypesResultVisitCallbackQuery::constructResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
				}
				mConRealization = nullptr;
				if (realization) {
					mConRealization = realization->getConceptRealization();
					if (mConRealization) {
						mResult = new CSucceedQueryResult();
					}
				}
				return mResult;
			}

			bool CTypesResultVisitCallbackQuery::callbackVisitingTypes(CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
				if (mConRealization) {
					CSetOfEntityExpressionSetResultVisitingCallbackGenerator resultGenerator(visitingCallback,mOntology);
					mConRealization->visitTypes(mIndividual,mDirect,&resultGenerator);
					return true;
				}
				return false;
			}


			QString CTypesResultVisitCallbackQuery::getQueryName() {
				return queryName;
			}

			QString CTypesResultVisitCallbackQuery::getQueryString() {
				return queryString;
			}

			bool CTypesResultVisitCallbackQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CTypesResultVisitCallbackQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Types");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CTypesResultVisitCallbackQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			bool CTypesResultVisitCallbackQuery::isDirect() {
				return mDirect;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
