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

#include "CIsPropertySatisfiableQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIsPropertySatisfiableQuery::CIsPropertySatisfiableQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CRole* role, const QString& propertyName, bool dataProperty, const QString& queryName)
					: CObjectDataPropertyClassificationPremisingQuery(ontology,configuration,dataProperty) {
				
				mPropertyName = propertyName;
				mRole = role;
				mQueryName = queryName;
				if (mQueryName.isEmpty()) {
					mQueryString = QString("Is property %1 satisfiable").arg(mPropertyName);
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mClassificationCalcError = false;
				mQueryConstructError = false;
			}


			CIsPropertySatisfiableQuery::~CIsPropertySatisfiableQuery() {
				delete mResult;
			}


			CQueryResult* CIsPropertySatisfiableQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CIsPropertySatisfiableQuery::constructResult(CClassification* classification) {
				CPropertyRoleClassification* roleClassification = nullptr;
				if (isDataPropertyQuery()) {
					roleClassification = classification->getDataPropertyRoleClassification();
				} else {
					roleClassification = classification->getObjectPropertyRoleClassification();
				}
				if (roleClassification) {
					return constructPropertySynsetsResult(roleClassification->getRolePropertiesHierarchy());
				}
				return nullptr;
			}

			CBooleanQueryResult *CIsPropertySatisfiableQuery::constructPropertySynsetsResult(CRolePropertiesHierarchy *hierarchy) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (hierarchy) {
					CRolePropertiesHierarchyNode* bottomNode = hierarchy->getBottomHierarchyNode();
					CRolePropertiesHierarchyNode* node = hierarchy->getHierarchyNode(mRole);
					mResult = new CBooleanQueryResult(bottomNode != node);
				}
				return mResult;
			}


			QString CIsPropertySatisfiableQuery::getQueryName() {
				return mQueryName;
			}

			QString CIsPropertySatisfiableQuery::getQueryString() {
				return mQueryString;
			}

			bool CIsPropertySatisfiableQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CIsPropertySatisfiableQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CIsPropertySatisfiableQuery::hasError() {
				return mClassificationCalcError || mQueryConstructError || CQuery::hasError();
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
