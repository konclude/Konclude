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

#include "CSubClassHierarchyQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSubClassHierarchyQuery::CSubClassHierarchyQuery(CConcreteOntology *premisingTaxonomyOntology, CTaxonomyPremisingQuerySupport *taxonomyQuerySupport, CConfigurationBase *configuration, const QString &superClassName, const QString &subClassHierarchyQueryName)
					: CTaxonomyPremisingQuery(premisingTaxonomyOntology,configuration) {
				className = superClassName;
				queryName = subClassHierarchyQueryName;
				if (className.isEmpty()) {
					queryString = QString("Get Subclass Hierarchy");
				} else {
					queryString = QString("Get Subclass Hierarchy of class '%1'").arg(className);
				}
				result = 0;
				taxQuerySupport = taxonomyQuerySupport;

				useAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mTaxCalcError = false;
				mQueryConstructError = false;

			}

			CSubClassHierarchyQuery::~CSubClassHierarchyQuery() {
				if (result) {
					delete result;
				}
				if (taxQuerySupport) {
					delete taxQuerySupport;
				}
			}


			CQueryResult *CSubClassHierarchyQuery::getQueryResult() {
				return result;
			}


			CQueryResult *CSubClassHierarchyQuery::constructResult(CTaxonomy *taxonomy) {
				return constructSubClassHierarchyResult(taxonomy);
			}

			CClassHierarchyResult *CSubClassHierarchyQuery::constructSubClassHierarchyResult(CTaxonomy *taxonomy) {
				if (result) {
					delete result;
					result = 0;
				}
				CHierarchyNode *topNode = taxonomy->getTopHierarchyNode();
				if (!className.isEmpty()) {
					topNode = taxonomy->getHierarchyNode(ontology->getConcept(className));
				}
				CHierarchyNode *bottomNode = taxonomy->getBottomHierarchyNode();
				if (topNode) {
					result = new CClassHierarchyResult(bottomNode->getEquivalentConceptStringList(useAbbreviatedIRIs),topNode->getEquivalentConceptStringList(useAbbreviatedIRIs));
					QList<CHierarchyNode *> processNodeList;
					QSet<CHierarchyNode *> processNodeSet;
					processNodeList.append(topNode);
					while (!processNodeList.isEmpty()) {
						CHierarchyNode *procNode = processNodeList.takeFirst();
						QSet<CHierarchyNode *>* childNodeSet(procNode->getChildNodeSet());
						foreach (CHierarchyNode *childNode, *childNodeSet) {
							if (childNode != bottomNode) {
								result->addSubClassRelation(result->getClassSynset(procNode->getEquivalentConceptStringList(useAbbreviatedIRIs)),result->getClassSynset(childNode->getEquivalentConceptStringList(useAbbreviatedIRIs)));
								if (!processNodeSet.contains(childNode)) {
									processNodeSet.insert(childNode);
									processNodeList.append(childNode);
								}
							}
						}
					}
				}
				return result;
			}


			QString CSubClassHierarchyQuery::getQueryName() {
				return queryName;
			}

			QString CSubClassHierarchyQuery::getQueryString() {
				return queryString;
			}

			bool CSubClassHierarchyQuery::hasAnswer() {
				return result != 0;
			}

			QString CSubClassHierarchyQuery::getAnswerString() {
				if (!result) {
					return QString("Unknown Subclass Hierarchy");
				} else {
					return result->getQueryResultString();
				}
			}

			CResultInterceptionData *CSubClassHierarchyQuery::getPossibleResultInterception() {
				return 0;
			}

			CTaxonomyPremisingQuerySupport *CSubClassHierarchyQuery::getTaxonomyQuerySupport() {
				return taxQuerySupport;
			}

			bool CSubClassHierarchyQuery::hasError() {
				return mTaxCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
