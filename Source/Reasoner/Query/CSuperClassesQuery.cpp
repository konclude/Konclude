/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CSuperClassesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSuperClassesQuery::CSuperClassesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CConcept* concept, bool direct, const QString& className, const QString& queryName)
					: CTaxonomyPremisingQuery(ontology,configuration) {
				mClassName = className;
				mConcept = concept;
				mQueryName = queryName;
				if (mClassName.isEmpty()) {
					mQueryString = QString("Get Super Classes");
				} else {
					mQueryString = QString("Get Super Classes for concept '%1'").arg(mClassName);
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mTaxonomyCalcError = false;
				mQueryConstructError = false;
				mDirect = direct;

				mCalcConfig = configuration;
			}

			CSuperClassesQuery::~CSuperClassesQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CSuperClassesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CSuperClassesQuery::constructResult(CTaxonomy *taxonomy) {
				return constructClassSynsetsResult(taxonomy);
			}

			CClassSynsetsResult *CSuperClassesQuery::constructClassSynsetsResult(CTaxonomy *taxonomy) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (taxonomy) {
					CHierarchyNode* node = taxonomy->getHierarchyNode(mConcept);
					if (node) {
						mResult = new CClassSynsetsResult();
						QSet<CHierarchyNode*>* visitedHierNodeSet = nullptr;
						if (!mDirect) {
							visitedHierNodeSet = new QSet<CHierarchyNode*>();
						}
						QList<CHierarchyNode*> visitHierNodetList;
						QSet<CHierarchyNode*>* parentNodeSet = node->getParentNodeSet();
						for (QSet<CHierarchyNode*>::const_iterator it = parentNodeSet->constBegin(), itEnd = parentNodeSet->constEnd(); it != itEnd; ++it) {
							CHierarchyNode* parentNode(*it);
							visitHierNodetList.append(parentNode);
						}

						while (!visitHierNodetList.isEmpty()) {
							CHierarchyNode* nextNode = visitHierNodetList.takeFirst();
							CClassSynsetResult* nextSubClassSynset = new CClassSynsetResult(nextNode->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
							mResult->addClassSynset(nextSubClassSynset);
							if (!mDirect) {
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
						delete visitedHierNodeSet;

					}
				}
				return mResult;
			}


			QString CSuperClassesQuery::getQueryName() {
				return mQueryName;
			}

			QString CSuperClassesQuery::getQueryString() {
				return mQueryString;
			}

			bool CSuperClassesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CSuperClassesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Classes");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CSuperClassesQuery::hasError() {
				return mTaxonomyCalcError || mQueryConstructError || CQuery::hasError();
			}

			bool CSuperClassesQuery::isDirect() {
				return mDirect;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
