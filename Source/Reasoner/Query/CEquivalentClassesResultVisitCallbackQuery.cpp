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

#include "CEquivalentClassesResultVisitCallbackQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CEquivalentClassesResultVisitCallbackQuery::CEquivalentClassesResultVisitCallbackQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CConcept* concept, const QString& className, const QString& queryName)
					: CTaxonomyPremisingQuery(ontology,configuration) {
				mClassName = className;
				mConcept = concept;
				mQueryName = queryName;

				if (mClassName.isEmpty()) {
					mQueryString = QString("Get Equivalent Classes");
				} else {
					mQueryString = QString("Get Equivalent Classes for '%1'").arg(mClassName);
				}
				mResult = nullptr;

				mTaxonomyCalcError = false;
				mQueryConstructError = false;

				mNode = nullptr;

				mCalcConfig = configuration;
			}

			CEquivalentClassesResultVisitCallbackQuery::~CEquivalentClassesResultVisitCallbackQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CEquivalentClassesResultVisitCallbackQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CEquivalentClassesResultVisitCallbackQuery::constructResult(CTaxonomy *taxonomy) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				mNode = taxonomy->getHierarchyNode(mConcept);
				if (mNode) {
					mResult = new CSucceedQueryResult();
				}
				return mResult;
			}

			bool CEquivalentClassesResultVisitCallbackQuery::callbackVisitingClasses(CEntityExpressionSetResultVisitingCallback* visitingCallback) {
				if (mNode) {
					QList<CConcept*>* eqConList = mNode->getEquivalentConceptList();
					for (QList<CConcept*>::const_iterator it = eqConList->constBegin(), itEnd = eqConList->constEnd(); it != itEnd; ++it) {
						CConcept* concept(*it);
						visitingCallback->visitConceptAssociatedEntityExpression(concept,ontology);
					}
					return true;
				}
				return false;
			}


			QString CEquivalentClassesResultVisitCallbackQuery::getQueryName() {
				return mQueryName;
			}

			QString CEquivalentClassesResultVisitCallbackQuery::getQueryString() {
				return mQueryString;
			}

			bool CEquivalentClassesResultVisitCallbackQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CEquivalentClassesResultVisitCallbackQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Classes");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CEquivalentClassesResultVisitCallbackQuery::hasError() {
				return mTaxonomyCalcError || mQueryConstructError || CQuery::hasError();
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
