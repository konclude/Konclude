/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CTypesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CTypesQuery::CTypesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CIndividual* indi, bool direct, const QString& individualName, const QString& flatternedTypeQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mIndividualName = individualName;
				mIndividual = indi;
				queryName = flatternedTypeQueryName;
				if (mIndividualName.isEmpty()) {
					queryString = QString("Get Types");
				} else {
					queryString = QString("Get Types for individual '%1'").arg(mIndividualName);
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mRealizationCalcError = false;
				mQueryConstructError = false;
				mDirect = direct;

				mRequiresConceptRealisation = true;
				mRequiresSameIndividualRealisation = true;

				mCalcConfig = configuration;
			}

			CTypesQuery::~CTypesQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CTypesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CTypesQuery::constructResult(CRealization* realization) {
				return constructClassSynsetsResult(realization);
			}

			CClassSynsetsResult *CTypesQuery::constructClassSynsetsResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (realization) {
					CConceptRealization* conRealization = realization->getConceptRealization();
					if (conRealization) {
						mResult = new CClassSynsetsResult();
						CClassSynsetsResultVisitorGenerator resultGenerator(mResult,mUseAbbreviatedIRIs);
						conRealization->visitTypes(mIndividual,mDirect,&resultGenerator);
					}
				}
				return mResult;
			}


			QString CTypesQuery::getQueryName() {
				return queryName;
			}

			QString CTypesQuery::getQueryString() {
				return queryString;
			}

			bool CTypesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CTypesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Types");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CTypesQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			bool CTypesQuery::isDirect() {
				return mDirect;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
