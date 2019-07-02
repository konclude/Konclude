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

#include "CSubSuperClassesResultVisitCallbackQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSubSuperClassesResultVisitCallbackQuery::CSubSuperClassesResultVisitCallbackQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CConcept* concept, const QString& className, bool subClasses, bool superClasses, bool direct, const QString& queryName)
					: CTaxonomyPremisingQuery(ontology,configuration) {
				mClassName = className;
				mConcept = concept;
				mQueryName = queryName;

				mSubClasses = subClasses;
				mSuperClasses = superClasses;

				QString subSuperString("Sub/Super");
				if (mSubClasses && !mSuperClasses) {
					subSuperString = QString("Sub");
				} else if (!mSubClasses && mSuperClasses) {
					subSuperString = QString("Super");
				}
				if (mClassName.isEmpty()) {
					mQueryString = QString("Get %1 Classes").arg(subSuperString);
				} else {
					mQueryString = QString("Get %1 Classes for '%2'").arg(subSuperString).arg(mClassName);
				}
				mResult = nullptr;

				mTaxonomyCalcError = false;
				mQueryConstructError = false;
				mDirect = direct;
				mNode = nullptr;

				mCalcConfig = configuration;
			}

			CSubSuperClassesResultVisitCallbackQuery::~CSubSuperClassesResultVisitCallbackQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CSubSuperClassesResultVisitCallbackQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CSubSuperClassesResultVisitCallbackQuery::constructResult(CTaxonomy *taxonomy) {
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


			bool CSubSuperClassesResultVisitCallbackQuery::callbackVisitingClasses(CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
				if (mNode) {
					QSet<CHierarchyNode*>* visitedHierNodeSet = nullptr;
					if (!mDirect) {
						visitedHierNodeSet = new QSet<CHierarchyNode*>();
					}
					QList<CHierarchyNode*> visitHierNodetList;
					if (mSubClasses) {
						QSet<CHierarchyNode*>* childNodeSet = mNode->getChildNodeSet();
						for (QSet<CHierarchyNode*>::const_iterator it = childNodeSet->constBegin(), itEnd = childNodeSet->constEnd(); it != itEnd; ++it) {
							CHierarchyNode* childNode(*it);
							visitHierNodetList.append(childNode);
						}
					}
					if (mSuperClasses) {
						QSet<CHierarchyNode*>* parentNodeSet = mNode->getParentNodeSet();
						for (QSet<CHierarchyNode*>::const_iterator it = parentNodeSet->constBegin(), itEnd = parentNodeSet->constEnd(); it != itEnd; ++it) {
							CHierarchyNode* parentNode(*it);
							visitHierNodetList.append(parentNode);
						}
					}

					while (!visitHierNodetList.isEmpty()) {
						CHierarchyNode* nextNode = visitHierNodetList.takeFirst();

						visitingCallback->startEntityExpressionSet(ontology);
						QList<CConcept*>* eqConList = nextNode->getEquivalentConceptList();
						for (QList<CConcept*>::const_iterator it = eqConList->constBegin(), itEnd = eqConList->constEnd(); it != itEnd; ++it) {
							CConcept* concept(*it);
							visitingCallback->visitConceptAssociatedEntityExpression(concept,ontology);
						}
						visitingCallback->endEntityExpressionSet(ontology);

						if (!mDirect) {
							if (mSubClasses) {
								QSet<CHierarchyNode*>* tmpChildNodeSet = nextNode->getChildNodeSet();
								for (QSet<CHierarchyNode*>::const_iterator it = tmpChildNodeSet->constBegin(), itEnd = tmpChildNodeSet->constEnd(); it != itEnd; ++it) {
									CHierarchyNode* nextChildNode(*it);
									if (!visitedHierNodeSet->contains(nextChildNode)) {
										visitedHierNodeSet->insert(nextChildNode);
										visitHierNodetList.append(nextChildNode);
									}
								}
							}
							if (mSuperClasses) {
								QSet<CHierarchyNode*>* tmpParentNodeSet = nextNode->getParentNodeSet();
								for (QSet<CHierarchyNode*>::const_iterator it = tmpParentNodeSet->constBegin(), itEnd = tmpParentNodeSet->constEnd(); it != itEnd; ++it) {
									CHierarchyNode* nextParentNode(*it);
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


			QString CSubSuperClassesResultVisitCallbackQuery::getQueryName() {
				return mQueryName;
			}

			QString CSubSuperClassesResultVisitCallbackQuery::getQueryString() {
				return mQueryString;
			}

			bool CSubSuperClassesResultVisitCallbackQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CSubSuperClassesResultVisitCallbackQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Classes");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CSubSuperClassesResultVisitCallbackQuery::hasError() {
				return mTaxonomyCalcError || mQueryConstructError || CQuery::hasError();
			}

			bool CSubSuperClassesResultVisitCallbackQuery::isDirect() {
				return mDirect;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
