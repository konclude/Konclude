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

#include "CSubClassesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSubClassesQuery::CSubClassesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CConcept* concept, bool direct, const QString& className, const QString& queryName)
					: CTaxonomyPremisingQuery(ontology,configuration) {
				mClassName = className;
				mConcept = concept;
				mQueryName = queryName;
				if (mClassName.isEmpty()) {
					mQueryString = QString("Get Sub Classes");
				} else {
					mQueryString = QString("Get Sub Classes for concept '%1'").arg(mClassName);
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mTaxonomyCalcError = false;
				mQueryConstructError = false;
				mDirect = direct;

				mCalcConfig = configuration;
			}

			CSubClassesQuery::~CSubClassesQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CSubClassesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CSubClassesQuery::constructResult(CTaxonomy *taxonomy) {
				return constructClassSynsetsResult(taxonomy);
			}

			CClassSynsetsResult *CSubClassesQuery::constructClassSynsetsResult(CTaxonomy *taxonomy) {
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
						QSet<CHierarchyNode*>* childNodeSet = node->getChildNodeSet();
						for (QSet<CHierarchyNode*>::const_iterator it = childNodeSet->constBegin(), itEnd = childNodeSet->constEnd(); it != itEnd; ++it) {
							CHierarchyNode* childNode(*it);
							visitHierNodetList.append(childNode);
						}

						while (!visitHierNodetList.isEmpty()) {
							CHierarchyNode* nextNode = visitHierNodetList.takeFirst();
							CClassSynsetResult* nextSubClassSynset = new CClassSynsetResult(nextNode->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
							mResult->addClassSynset(nextSubClassSynset);
							if (!mDirect) {
								QSet<CHierarchyNode*>* tmpChildNodeSet = nextNode->getChildNodeSet();
								for (QSet<CHierarchyNode*>::const_iterator it = tmpChildNodeSet->constBegin(), itEnd = tmpChildNodeSet->constEnd(); it != itEnd; ++it) {
									CHierarchyNode* nextChildNode(*it);
									if (!visitedHierNodeSet->contains(nextChildNode)) {
										visitedHierNodeSet->insert(nextChildNode);
										visitHierNodetList.append(nextChildNode);
									}
								}
							}
						}
						delete visitedHierNodeSet;

					}
				}
				return mResult;
			}


			QString CSubClassesQuery::getQueryName() {
				return mQueryName;
			}

			QString CSubClassesQuery::getQueryString() {
				return mQueryString;
			}

			bool CSubClassesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CSubClassesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Classes");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CSubClassesQuery::hasError() {
				return mTaxonomyCalcError || mQueryConstructError || CQuery::hasError();
			}

			bool CSubClassesQuery::isDirect() {
				return mDirect;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
