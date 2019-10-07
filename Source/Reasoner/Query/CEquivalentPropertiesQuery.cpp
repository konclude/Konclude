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

#include "CEquivalentPropertiesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CEquivalentPropertiesQuery::CEquivalentPropertiesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CRole* role, const QString& propertyName, bool dataProperties, const QString& queryName)
					: CObjectDataPropertyClassificationPremisingQuery(ontology,configuration,dataProperties) {
				mPropertyName = propertyName;
				mRole = role;
				mQueryName = queryName;
				if (mPropertyName.isEmpty()) {
					mQueryString = QString("Get Equivalent Properties");
				} else {
					mQueryString = QString("Get Equivalent Properties for role '%1'").arg(mPropertyName);
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mClassificationCalcError = false;
				mQueryConstructError = false;

				mCalcConfig = configuration;
			}

			CEquivalentPropertiesQuery::~CEquivalentPropertiesQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CEquivalentPropertiesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CEquivalentPropertiesQuery::constructResult(CClassification *classification) {
				CPropertyRoleClassification* roleClassification = nullptr;
				if (isDataPropertyQuery()) {
					roleClassification = classification->getDataPropertyRoleClassification();
				} else {
					roleClassification = classification->getObjectPropertyRoleClassification();
				}
				if (roleClassification) {
					return constructPropertiesResult(roleClassification->getRolePropertiesHierarchy());
				} 
				return nullptr;
			}

			CPropertySetResult *CEquivalentPropertiesQuery::constructPropertiesResult(CRolePropertiesHierarchy *hierarchy) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (hierarchy) {
					CRolePropertiesHierarchyNode* node = hierarchy->getHierarchyNode(mRole);
					if (node) {
						mResult = new CPropertySetResult(mDataPropertiesRolesQuery);
						foreach (const QString& propertyName, node->getEquivalentRoleStringList(mUseAbbreviatedIRIs)) {
							mResult->addProperty(propertyName);
						}
					}
				}
				return mResult;
			}


			QString CEquivalentPropertiesQuery::getQueryName() {
				return mQueryName;
			}

			QString CEquivalentPropertiesQuery::getQueryString() {
				return mQueryString;
			}

			bool CEquivalentPropertiesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CEquivalentPropertiesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Properties");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CEquivalentPropertiesQuery::hasError() {
				return mClassificationCalcError || mQueryConstructError || CQuery::hasError();
			}



		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
