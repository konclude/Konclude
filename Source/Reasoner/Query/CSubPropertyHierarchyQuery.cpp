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

#include "CSubPropertyHierarchyQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSubPropertyHierarchyQuery::CSubPropertyHierarchyQuery(CConcreteOntology *ontology, CConfigurationBase *configuration, CRole* role, bool dataProperties, const QString &superPropertyName, const QString &subPropertyHierarchyQueryName)
					: CObjectDataPropertyClassificationPremisingQuery(ontology,configuration,dataProperties) {
				mPropertyName = superPropertyName;
				mQueryName = subPropertyHierarchyQueryName;
				mRole = role;
				if (mPropertyName.isEmpty()) {
					mQueryString = QString("Get Subproperty Hierarchy");
				} else {
					mQueryString = QString("Get Subproperty Hierarchy of property '%1'").arg(mPropertyName);
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mClassificationCalcError = false;
				mQueryConstructError = false;

			}

			CSubPropertyHierarchyQuery::~CSubPropertyHierarchyQuery() {
				if (mResult) {
					delete mResult;
				}
			}




			CQueryResult *CSubPropertyHierarchyQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CSubPropertyHierarchyQuery::constructResult(CClassification* classification) {
				CPropertyRoleClassification* roleClassification = nullptr;
				if (isDataPropertyQuery()) {
					roleClassification = classification->getDataPropertyRoleClassification();
				} else {
					roleClassification = classification->getObjectPropertyRoleClassification();
				}
				if (roleClassification) {
					return constructSubPropertyHierarchyResult(roleClassification->getRolePropertiesHierarchy());
				}
				return nullptr;
			}


			CPropertyHierarchyResult *CSubPropertyHierarchyQuery::constructSubPropertyHierarchyResult(CRolePropertiesHierarchy *hierarchy) {
				if (mResult) {
					delete mResult;
					mResult = nullptr;
				}
				CRolePropertiesHierarchyNode *topNode = hierarchy->getTopHierarchyNode();
				if (!mPropertyName.isEmpty()) {
					topNode = hierarchy->getHierarchyNode(mRole);
				}
				CRolePropertiesHierarchyNode *bottomNode = hierarchy->getBottomHierarchyNode();
				if (topNode) {
					mResult = new CPropertyHierarchyResult(bottomNode->getEquivalentRoleStringList(mUseAbbreviatedIRIs),topNode->getEquivalentRoleStringList(mUseAbbreviatedIRIs), mDataPropertiesRolesQuery);
					QList<CRolePropertiesHierarchyNode *> processNodeList;
					QSet<CRolePropertiesHierarchyNode *> processNodeSet;
					processNodeList.append(topNode);
					while (!processNodeList.isEmpty()) {
						CRolePropertiesHierarchyNode *procNode = processNodeList.takeFirst();
						QSet<CRolePropertiesHierarchyNode *>* childNodeSet(procNode->getChildNodeSet());
						foreach (CRolePropertiesHierarchyNode *childNode, *childNodeSet) {
							if (childNode != bottomNode) {
								mResult->addSubPropertyRelation(mResult->getPropertySynset(procNode->getEquivalentRoleStringList(mUseAbbreviatedIRIs)),mResult->getPropertySynset(childNode->getEquivalentRoleStringList(mUseAbbreviatedIRIs)));
								if (!processNodeSet.contains(childNode)) {
									processNodeSet.insert(childNode);
									processNodeList.append(childNode);
								}
							}
						}
					}
				}
				return mResult;
			}


			QString CSubPropertyHierarchyQuery::getQueryName() {
				return mQueryName;
			}

			QString CSubPropertyHierarchyQuery::getQueryString() {
				return mQueryString;
			}

			bool CSubPropertyHierarchyQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CSubPropertyHierarchyQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Subproperty Hierarchy");
				} else {
					return mResult->getQueryResultString();
				}
			}


			bool CSubPropertyHierarchyQuery::hasError() {
				return mClassificationCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
