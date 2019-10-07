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

#include "CSameIndividualsResultVisitCallbackQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSameIndividualsResultVisitCallbackQuery::CSameIndividualsResultVisitCallbackQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CIndividual* individual, const QString& indiName, const QString& sameIndisQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mIndividualName = indiName;
				mIndividual = individual;
				queryName = sameIndisQueryName;
				if (mIndividualName.isEmpty()) {
					queryString = QString("Get Same Individuals");
				} else {
					queryString = QString("Get Same Individuals for '%1'").arg(mIndividualName);
				}
				mResult = nullptr;

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mRequiresSameIndividualRealisation = true;

				mCalcConfig = configuration;
			}

			CSameIndividualsResultVisitCallbackQuery::~CSameIndividualsResultVisitCallbackQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CSameIndividualsResultVisitCallbackQuery::getQueryResult() {
				return mResult;
			}

			CQueryResult *CSameIndividualsResultVisitCallbackQuery::constructResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
				}
				mSameRealization = nullptr;
				if (realization) {
					mSameRealization = realization->getConceptRealization();
					if (mSameRealization) {
						mResult = new CSucceedQueryResult();
					}
				}
				return mResult;
			}

			bool CSameIndividualsResultVisitCallbackQuery::callbackVisitingInstances(CEntityExpressionSetResultVisitingCallback* visitingCallback) {
				if (mSameRealization) {
					CEntityExpressionSetResultVisitingCallbackGenerator resultGenerator(visitingCallback,mOntology);
					mSameRealization->visitSameIndividuals(mIndividual,&resultGenerator);
					return true;
				}
				return false;
			}


			QString CSameIndividualsResultVisitCallbackQuery::getQueryName() {
				return queryName;
			}

			QString CSameIndividualsResultVisitCallbackQuery::getQueryString() {
				return queryString;
			}

			bool CSameIndividualsResultVisitCallbackQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CSameIndividualsResultVisitCallbackQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Individuals");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CSameIndividualsResultVisitCallbackQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
