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

#include "CEquivalentClassesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CEquivalentClassesQuery::CEquivalentClassesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CConcept* concept, const QString& className, const QString& queryName)
					: CTaxonomyPremisingQuery(ontology,configuration) {
				mClassName = className;
				mConcept = concept;
				mQueryName = queryName;
				if (mClassName.isEmpty()) {
					mQueryString = QString("Get Equivalent Classes");
				} else {
					mQueryString = QString("Get Equivalent Classes for concept '%1'").arg(mClassName);
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mTaxonomyCalcError = false;
				mQueryConstructError = false;

				mCalcConfig = configuration;
			}

			CEquivalentClassesQuery::~CEquivalentClassesQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CEquivalentClassesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CEquivalentClassesQuery::constructResult(CTaxonomy *taxonomy) {
				return constructClassesResult(taxonomy);
			}

			CClassSetResult *CEquivalentClassesQuery::constructClassesResult(CTaxonomy *taxonomy) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (taxonomy) {
					CHierarchyNode* node = taxonomy->getHierarchyNode(mConcept);
					if (node) {
						mResult = new CClassSetResult();
						foreach (const QString& className, node->getEquivalentConceptStringList(mUseAbbreviatedIRIs)) {
							mResult->addClass(className);
						}
					}
				}
				return mResult;
			}


			QString CEquivalentClassesQuery::getQueryName() {
				return mQueryName;
			}

			QString CEquivalentClassesQuery::getQueryString() {
				return mQueryString;
			}

			bool CEquivalentClassesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CEquivalentClassesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Classes");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CEquivalentClassesQuery::hasError() {
				return mTaxonomyCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
