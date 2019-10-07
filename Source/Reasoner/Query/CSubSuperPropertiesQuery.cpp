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

#include "CSubSuperPropertiesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSubSuperPropertiesQuery::CSubSuperPropertiesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, bool direct, CRole* role, const QString& propertyName, bool dataProperty, bool subQuery, bool superQuery, const QString& queryName)
					: CObjectDataPropertyClassificationPremisingQuery(ontology,configuration,dataProperty) {
				
				mCalcConfig = nullptr;
				initSubSuperPropertiesQuery(ontology,configuration,direct,role,propertyName,subQuery,superQuery,queryName);
			}


			CSubSuperPropertiesQuery* CSubSuperPropertiesQuery::initSubSuperPropertiesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, bool direct, CRole* role, const QString& propertyName, bool subQuery, bool superQuery, const QString& queryName = QString("UnnamedQuery")) {
				mPropertyName = propertyName;
				mRole = role;
				mQueryName = queryName;
				if (mQueryName.isEmpty()) {
					QString subSuperString = QString("Sub/Super");
					if (superQuery && !subQuery) {
						subSuperString = QString("Super");
					} else if (!superQuery && subQuery) {
						subSuperString = QString("Sub");
					}
					if (mPropertyName.isEmpty()) {
						mQueryString = QString("Get %1 Properties").arg(subSuperString);
					} else {
						mQueryString = QString("Get %1 Properties for role '%2'").arg(subSuperString).arg(mPropertyName);
					}
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mClassificationCalcError = false;
				mQueryConstructError = false;

				mDirectQuery = direct;
				mSuperPropertiesQuery = superQuery;
				mSubPropertiesQuery = subQuery;

				return this;
			}


			CSubSuperPropertiesQuery::~CSubSuperPropertiesQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			bool CSubSuperPropertiesQuery::isSubPropertyQuery() {
				return mSubPropertiesQuery;
			}

			bool CSubSuperPropertiesQuery::isSuperPropertyQuery() {
				return mSuperPropertiesQuery;
			}





			CQueryResult* CSubSuperPropertiesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CSubSuperPropertiesQuery::constructResult(CClassification* classification) {
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

			CPropertySynsetsResult *CSubSuperPropertiesQuery::constructPropertySynsetsResult(CRolePropertiesHierarchy *hierarchy) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (hierarchy) {
					CRolePropertiesHierarchyNode* node = hierarchy->getHierarchyNode(mRole);
					if (node) {
						mResult = new CPropertySynsetsResult(mDataPropertiesRolesQuery);
						QSet<CRolePropertiesHierarchyNode*>* visitedHierNodeSet = nullptr;
						if (!mDirectQuery) {
							visitedHierNodeSet = new QSet<CRolePropertiesHierarchyNode*>();
						}
						QList<CRolePropertiesHierarchyNode*> visitHierNodetList;
						if (mSubPropertiesQuery) {
							QSet<CRolePropertiesHierarchyNode*>* childNodeSet = node->getChildNodeSet();
							for (QSet<CRolePropertiesHierarchyNode*>::const_iterator it = childNodeSet->constBegin(), itEnd = childNodeSet->constEnd(); it != itEnd; ++it) {
								CRolePropertiesHierarchyNode* childNode(*it);
								visitHierNodetList.append(childNode);
							}
						}
						if (mSuperPropertiesQuery) {
							QSet<CRolePropertiesHierarchyNode*>* parentNodeSet = node->getParentNodeSet();
							for (QSet<CRolePropertiesHierarchyNode*>::const_iterator it = parentNodeSet->constBegin(), itEnd = parentNodeSet->constEnd(); it != itEnd; ++it) {
								CRolePropertiesHierarchyNode* parentNode(*it);
								visitHierNodetList.append(parentNode);
							}
						}

						while (!visitHierNodetList.isEmpty()) {
							CRolePropertiesHierarchyNode* nextNode = visitHierNodetList.takeFirst();
							CPropertySynsetResult* nextSubPropertySynset = new CPropertySynsetResult(nextNode->getEquivalentRoleStringList(mUseAbbreviatedIRIs));
							mResult->addPropertySynset(nextSubPropertySynset);
							if (!mDirectQuery) {
								if (mSubPropertiesQuery) {
									QSet<CRolePropertiesHierarchyNode*>* tmpChildNodeSet = nextNode->getChildNodeSet();
									for (QSet<CRolePropertiesHierarchyNode*>::const_iterator it = tmpChildNodeSet->constBegin(), itEnd = tmpChildNodeSet->constEnd(); it != itEnd; ++it) {
										CRolePropertiesHierarchyNode* nextChildNode(*it);
										if (!visitedHierNodeSet->contains(nextChildNode)) {
											visitedHierNodeSet->insert(nextChildNode);
											visitHierNodetList.append(nextChildNode);
										}
									}
								}
								if (mSuperPropertiesQuery) {
									QSet<CRolePropertiesHierarchyNode*>* tmpParentNodeSet = nextNode->getParentNodeSet();
									for (QSet<CRolePropertiesHierarchyNode*>::const_iterator it = tmpParentNodeSet->constBegin(), itEnd = tmpParentNodeSet->constEnd(); it != itEnd; ++it) {
										CRolePropertiesHierarchyNode* nextParentNode(*it);
										if (!visitedHierNodeSet->contains(nextParentNode)) {
											visitedHierNodeSet->insert(nextParentNode);
											visitHierNodetList.append(nextParentNode);
										}
									}
								}
							}
						}
						delete visitedHierNodeSet;

					}
				}
				return mResult;
			}


			QString CSubSuperPropertiesQuery::getQueryName() {
				return mQueryName;
			}

			QString CSubSuperPropertiesQuery::getQueryString() {
				return mQueryString;
			}

			bool CSubSuperPropertiesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CSubSuperPropertiesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Properties");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CSubSuperPropertiesQuery::hasError() {
				return mClassificationCalcError || mQueryConstructError || CQuery::hasError();
			}

			bool CSubSuperPropertiesQuery::isDirect() {
				return mDirectQuery;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
