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

#include "CFlattenedTypesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CFlattenedTypesQuery::CFlattenedTypesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CIndividual* indi, bool direct, const QString& individualName, const QString& flatternedTypeQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mIndividualName = individualName;
				mIndividual = indi;
				queryName = flatternedTypeQueryName;
				if (mIndividualName.isEmpty()) {
					queryString = QString("Get Flattened Types");
				} else {
					queryString = QString("Get Flattened Types for individual '%1'").arg(mIndividualName);
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mRealizationCalcError = false;
				mQueryConstructError = false;
				mDirect = direct;

			}

			CFlattenedTypesQuery::~CFlattenedTypesQuery() {
				if (mResult) {
					delete mResult;
				}
			}


			CQueryResult* CFlattenedTypesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CFlattenedTypesQuery::constructResult(CRealization* realization) {
				return constructClassesResult(realization);
			}

			CClassesResult *CFlattenedTypesQuery::constructClassesResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (realization) {
					CConceptRealization* conRealization = realization->getConceptRealization();
					if (conRealization) {
						mResult = new CClassesResult();
						CClassesResultVisitorGenerator resultGenerator(mResult,mUseAbbreviatedIRIs);
						conRealization->visitTypes(mIndividual,mDirect,&resultGenerator);
					}
				}
				return mResult;
			}


			QString CFlattenedTypesQuery::getQueryName() {
				return queryName;
			}

			QString CFlattenedTypesQuery::getQueryString() {
				return queryString;
			}

			bool CFlattenedTypesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CFlattenedTypesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Classes");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CFlattenedTypesQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			bool CFlattenedTypesQuery::isDirect() {
				return mDirect;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
