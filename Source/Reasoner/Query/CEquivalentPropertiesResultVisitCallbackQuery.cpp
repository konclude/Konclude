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

#include "CEquivalentPropertiesResultVisitCallbackQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CEquivalentPropertiesResultVisitCallbackQuery::CEquivalentPropertiesResultVisitCallbackQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CRole* role, bool dataProperty, const QString& propertyName, const QString& queryName)
					: CObjectDataPropertyClassificationPremisingQuery(ontology,configuration,dataProperty) {
				mPropertyName = propertyName;
				mRole = role;
				mQueryName = queryName;

				if (mPropertyName.isEmpty()) {
					mQueryString = QString("Get Equivalent Properties");
				} else {
					mQueryString = QString("Get Equivalent Properties for '%1'").arg(mPropertyName);
				}
				mResult = nullptr;

				mClassificationCalcError = false;
				mQueryConstructError = false;

				mNode = nullptr;

				mCalcConfig = configuration;
			}

			CEquivalentPropertiesResultVisitCallbackQuery::~CEquivalentPropertiesResultVisitCallbackQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CEquivalentPropertiesResultVisitCallbackQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CEquivalentPropertiesResultVisitCallbackQuery::constructResult(CClassification* classification) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				CPropertyRoleClassification* roleClassification = nullptr;
				if (isDataPropertyQuery()) {
					roleClassification = classification->getDataPropertyRoleClassification();
				} else {
					roleClassification = classification->getObjectPropertyRoleClassification();
				}
				if (roleClassification) {
					CRolePropertiesHierarchy* hierarchy = roleClassification->getRolePropertiesHierarchy();
					mNode = hierarchy->getHierarchyNode(mRole);
				}
				if (mNode) {
					mResult = new CSucceedQueryResult();
				}
				return mResult;
			}

			bool CEquivalentPropertiesResultVisitCallbackQuery::callbackVisitingProperties(CEntityExpressionSetResultVisitingCallback* visitingCallback) {
				if (mNode) {
					QList<CRole*>* eqRoleList = mNode->getEquivalentRoleList();
					for (QList<CRole*>::const_iterator it = eqRoleList->constBegin(), itEnd = eqRoleList->constEnd(); it != itEnd; ++it) {
						CRole* role(*it);
						visitingCallback->visitRoleAssociatedEntityExpression(role,mOntology);
					}
					return true;
				}
				return false;
			}


			QString CEquivalentPropertiesResultVisitCallbackQuery::getQueryName() {
				return mQueryName;
			}

			QString CEquivalentPropertiesResultVisitCallbackQuery::getQueryString() {
				return mQueryString;
			}

			bool CEquivalentPropertiesResultVisitCallbackQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CEquivalentPropertiesResultVisitCallbackQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Properties");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CEquivalentPropertiesResultVisitCallbackQuery::hasError() {
				return mClassificationCalcError || mQueryConstructError || CQuery::hasError();
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
