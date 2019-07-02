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

#include "CSubSuperPropertiesResultVisitCallbackQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSubSuperPropertiesResultVisitCallbackQuery::CSubSuperPropertiesResultVisitCallbackQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CRole* role, const QString& propertyName, bool subProperties, bool superProperties, bool direct, bool dataProperty, const QString& queryName)
					: CObjectDataPropertyClassificationPremisingQuery(ontology,configuration,dataProperty) {
				mPropertyName = propertyName;
				mRole = role;
				mQueryName = queryName;

				mSubProperties = subProperties;
				mSuperProperties = superProperties;

				QString subSuperString("Sub/Super");
				if (mSubProperties && !mSuperProperties) {
					subSuperString = QString("Sub");
				} else if (!mSubProperties && mSuperProperties) {
					subSuperString = QString("Super");
				}
				if (mPropertyName.isEmpty()) {
					mQueryString = QString("Get %1 Properties").arg(subSuperString);
				} else {
					mQueryString = QString("Get %1 Properties for '%2'").arg(subSuperString).arg(mPropertyName);
				}
				mResult = nullptr;

				mClassificationCalcError = false;
				mQueryConstructError = false;
				mDirect = direct;
				mNode = nullptr;

				mCalcConfig = configuration;
			}

			CSubSuperPropertiesResultVisitCallbackQuery::~CSubSuperPropertiesResultVisitCallbackQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CSubSuperPropertiesResultVisitCallbackQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CSubSuperPropertiesResultVisitCallbackQuery::constructResult(CClassification* classification) {
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


			bool CSubSuperPropertiesResultVisitCallbackQuery::callbackVisitingProperties(CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
				if (mNode) {
					QSet<CRolePropertiesHierarchyNode*>* visitedHierNodeSet = nullptr;
					if (!mDirect) {
						visitedHierNodeSet = new QSet<CRolePropertiesHierarchyNode*>();
					}
					QList<CRolePropertiesHierarchyNode*> visitHierNodetList;
					if (mSubProperties) {
						QSet<CRolePropertiesHierarchyNode*>* childNodeSet = mNode->getChildNodeSet();
						for (QSet<CRolePropertiesHierarchyNode*>::const_iterator it = childNodeSet->constBegin(), itEnd = childNodeSet->constEnd(); it != itEnd; ++it) {
							CRolePropertiesHierarchyNode* childNode(*it);
							visitHierNodetList.append(childNode);
						}
					}
					if (mSuperProperties) {
						QSet<CRolePropertiesHierarchyNode*>* parentNodeSet = mNode->getParentNodeSet();
						for (QSet<CRolePropertiesHierarchyNode*>::const_iterator it = parentNodeSet->constBegin(), itEnd = parentNodeSet->constEnd(); it != itEnd; ++it) {
							CRolePropertiesHierarchyNode* parentNode(*it);
							visitHierNodetList.append(parentNode);
						}
					}

					while (!visitHierNodetList.isEmpty()) {
						CRolePropertiesHierarchyNode* nextNode = visitHierNodetList.takeFirst();

						visitingCallback->startEntityExpressionSet(mOntology);
						QList<CRole*>* eqRoleList = nextNode->getEquivalentRoleList();
						for (QList<CRole*>::const_iterator it = eqRoleList->constBegin(), itEnd = eqRoleList->constEnd(); it != itEnd; ++it) {
							CRole* role(*it);
							visitingCallback->visitRoleAssociatedEntityExpression(role,mOntology);
						}
						visitingCallback->endEntityExpressionSet(mOntology);

						if (!mDirect) {
							if (mSubProperties) {
								QSet<CRolePropertiesHierarchyNode*>* tmpChildNodeSet = nextNode->getChildNodeSet();
								for (QSet<CRolePropertiesHierarchyNode*>::const_iterator it = tmpChildNodeSet->constBegin(), itEnd = tmpChildNodeSet->constEnd(); it != itEnd; ++it) {
									CRolePropertiesHierarchyNode* nextChildNode(*it);
									if (!visitedHierNodeSet->contains(nextChildNode)) {
										visitedHierNodeSet->insert(nextChildNode);
										visitHierNodetList.append(nextChildNode);
									}
								}
							}
							if (mSuperProperties) {
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
					return true;
				}
				return false;
			}


			QString CSubSuperPropertiesResultVisitCallbackQuery::getQueryName() {
				return mQueryName;
			}

			QString CSubSuperPropertiesResultVisitCallbackQuery::getQueryString() {
				return mQueryString;
			}

			bool CSubSuperPropertiesResultVisitCallbackQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CSubSuperPropertiesResultVisitCallbackQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Properties");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CSubSuperPropertiesResultVisitCallbackQuery::hasError() {
				return mClassificationCalcError || mQueryConstructError || CQuery::hasError();
			}

			bool CSubSuperPropertiesResultVisitCallbackQuery::isDirect() {
				return mDirect;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
