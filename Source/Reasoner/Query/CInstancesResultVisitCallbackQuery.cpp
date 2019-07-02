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

#include "CInstancesResultVisitCallbackQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CInstancesResultVisitCallbackQuery::CInstancesResultVisitCallbackQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CConcept* concept, bool direct, const QString& conceptName, const QString& flatternedTypeQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mConceptName = conceptName;
				mConcept = concept;
				queryName = flatternedTypeQueryName;
				if (mConceptName.isEmpty()) {
					queryString = QString("Get Instances");
				} else {
					queryString = QString("Get Instances for Type '%1'").arg(mConceptName);
				}
				mResult = nullptr;

				mRealizationCalcError = false;
				mQueryConstructError = false;
				mDirect = direct;

				mCalcConfig = configuration;

				mRequiresConceptRealisation = true;
				mRequiresSameIndividualRealisation = true;

			}

			CInstancesResultVisitCallbackQuery::~CInstancesResultVisitCallbackQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CInstancesResultVisitCallbackQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CInstancesResultVisitCallbackQuery::constructResult(CRealization* realization) {
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

			bool CInstancesResultVisitCallbackQuery::callbackVisitingInstances(CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
				if (mConRealization) {
					CSetOfEntityExpressionSetResultVisitingCallbackGenerator resultGenerator(visitingCallback,mOntology);
					mConRealization->visitInstances(mConcept,mDirect,&resultGenerator);
					return true;
				}
				return false;
			}



			QString CInstancesResultVisitCallbackQuery::getQueryName() {
				return queryName;
			}

			QString CInstancesResultVisitCallbackQuery::getQueryString() {
				return queryString;
			}

			bool CInstancesResultVisitCallbackQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CInstancesResultVisitCallbackQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Individuals");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CInstancesResultVisitCallbackQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			bool CInstancesResultVisitCallbackQuery::isDirect() {
				return mDirect;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
