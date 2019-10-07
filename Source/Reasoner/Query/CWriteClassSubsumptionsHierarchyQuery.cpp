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

#include "CWriteClassSubsumptionsHierarchyQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWriteClassSubsumptionsHierarchyQuery::CWriteClassSubsumptionsHierarchyQuery(CConcreteOntology *premisingTaxonomyOntology, CConfigurationBase *configuration, const QString& outputName, const QString &superClassName, const QString &subClassHierarchyQueryName)
					: CTaxonomyPremisingQuery(premisingTaxonomyOntology,configuration) {
				mClassName = superClassName;
				mQueryName = subClassHierarchyQueryName;
				if (mClassName.isEmpty()) {
					mQueryString = QString("Get Subclass Hierarchy");
				} else {
					mQueryString = QString("Get Subclass Hierarchy of class '%1'").arg(mClassName);
				}

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.CLI.Output.AbbreviatedIRIs",false);
				mWriteDeclarations = CConfigDataReader::readConfigBoolean(configuration,"Konclude.CLI.Output.WriteDeclarations",false);

				mTaxCalcError = false;
				mQueryConstructError = false;

				mOutputFileNameString = outputName;

				mBottomClassNameString = QString("http://www.w3.org/2002/07/owl#Nothing");
				mTopClassNameString = QString("http://www.w3.org/2002/07/owl#Thing");
				mQueryAnswered = false;
			}

			CQueryResult *CWriteClassSubsumptionsHierarchyQuery::getQueryResult() {
				return nullptr;
			}

			CQuery* CWriteClassSubsumptionsHierarchyQuery::addQueryError(CQueryError* queryError) {
				if (CQueryInconsitentOntologyError::hasInconsistentOntologyError(queryError)) {
					// write inconsistency
					if (!writeInconsistentHierarchyResult()) {
						addQueryError(new CQueryUnspecifiedStringError(QString("Could not write class hierarchy to file '%1'.").arg(mOutputFileNameString)));
					}
				}
				CQuery::addQueryError(queryError);
				return this;
			}


			bool CWriteClassSubsumptionsHierarchyQuery::writeInconsistentHierarchyResult() {
				if (startWritingOutput()) {

					writeOntologyStart();

					CConcept* mTopConcept = ontology->getTBox()->getTopConcept();
					CConcept* mBottomConcept = ontology->getTBox()->getTopConcept();

					CBOXSET<CConcept*>* activeClassConceptSet = ontology->getTBox()->getActiveClassConceptSet();


					if (mWriteDeclarations) {
						for (CBOXSET<CConcept*>::const_iterator it = activeClassConceptSet->constBegin(), itEnd = activeClassConceptSet->constEnd(); it != itEnd; ++it) {
							CConcept* concept(*it);
							QString className = CIRIName::getRecentIRIName(concept->getClassNameLinker());
							if (mUseAbbreviatedIRIs) {
								className = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(concept->getClassNameLinker());
							}
							writeDeclaration(className);
						}
					}

					QStringList incClassNameList;
					for (CBOXSET<CConcept*>::const_iterator it = activeClassConceptSet->constBegin(), itEnd = activeClassConceptSet->constEnd(); it != itEnd; ++it) {
						CConcept* concept(*it);
						QString className = CIRIName::getRecentIRIName(concept->getClassNameLinker());
						if (mUseAbbreviatedIRIs) {
							className = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(concept->getClassNameLinker());
						}
						incClassNameList.append(className);
					}
					writeEquivalenceRelations(incClassNameList);

					writeOntologyEnd();

					return endWritingOutput();
				}
				return false;
			}


			CQueryResult *CWriteClassSubsumptionsHierarchyQuery::constructResult(CTaxonomy *taxonomy) {
				mQueryAnswered = true;
				if (!writeSubClassHierarchyResult(taxonomy)) {
					addQueryError(new CQueryUnspecifiedStringError(QString("Could not write class hierarchy to file '%1'.").arg(mOutputFileNameString)));
				}
				return nullptr;
			}

			bool CWriteClassSubsumptionsHierarchyQuery::writeSubClassHierarchyResult(CTaxonomy *taxonomy) {
				if (startWritingOutput()) {
					writeOntologyStart();

					CHierarchyNode* topNode = taxonomy->getTopHierarchyNode();
					CHierarchyNode* bottomNode = taxonomy->getBottomHierarchyNode();
					CHierarchyNode* basicNode = topNode;
					if (!mClassName.isEmpty()) {
						basicNode = taxonomy->getHierarchyNode(ontology->getConcept(mClassName));
					}
					writeDeclarations(bottomNode);
					writeBottomEquivalences(bottomNode);
					QSet<CHierarchyNode *> processNodeSet;
					processNodeSet.insert(bottomNode);
					if (basicNode) {
						QList<CHierarchyNode *> processNodeList;
						writeDeclarations(basicNode);
						writeTopEquivalences(basicNode);
						processNodeSet.insert(basicNode);
						processNodeList.append(basicNode);
						while (!processNodeList.isEmpty()) {
							CHierarchyNode *procNode = processNodeList.takeFirst();
							QSet<CHierarchyNode *>* childNodeSet(procNode->getChildNodeSet());
							foreach (CHierarchyNode *childNode, *childNodeSet) {

								if (!processNodeSet.contains(childNode)) {
									writeDeclarations(childNode);
									writeEquivalences(childNode);
									processNodeSet.insert(childNode);
									processNodeList.append(childNode);
								}

								if (childNode != bottomNode) {
									writeSubClassRelations(procNode,childNode);
								}
							}
						}
					}
					writeOntologyEnd();
					return endWritingOutput();
				}
				return false;
			}

			void CWriteClassSubsumptionsHierarchyQuery::writeBottomEquivalences(CHierarchyNode* node) {
				if (node->getEquivalentConceptCount() > 1) {
					writeEquivalenceRelations(node);
				}
			}

			void CWriteClassSubsumptionsHierarchyQuery::writeTopEquivalences(CHierarchyNode* node) {
				if (node->getEquivalentConceptCount() > 1) {
					writeEquivalenceRelations(node);
				}
			}




			void CWriteClassSubsumptionsHierarchyQuery::writeEquivalenceRelations(CHierarchyNode* node) {
				const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
				writeEquivalenceRelations(classNameList);
			}



			void CWriteClassSubsumptionsHierarchyQuery::writeEquivalences(CHierarchyNode* node) {
				if (node->getEquivalentConceptCount() > 1) {
					writeEquivalenceRelations(node);
				}
			}


			void CWriteClassSubsumptionsHierarchyQuery::writeDeclarations(CHierarchyNode* node) {
				if (mWriteDeclarations) {
					const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
					foreach (const QString& className, classNameList) {
						writeDeclaration(className);
					}
				}
			}




			void CWriteClassSubsumptionsHierarchyQuery::writeSubClassRelations(CHierarchyNode* superNode, CHierarchyNode* subNode) {
				const QStringList& superClassNameList(superNode->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
				const QStringList& subClassNameList(subNode->getEquivalentConceptStringList(mUseAbbreviatedIRIs));

				writeSubClassRelation(subClassNameList.first(),superClassNameList.first());
			}



			CWriteQuery::WRITEQUERYTYPE CWriteClassSubsumptionsHierarchyQuery::getWriteQueryType() {
				return CWriteQuery::WRITESUBCLASSHIERARCHY;
			}



			QString CWriteClassSubsumptionsHierarchyQuery::getQueryName() {
				return mQueryName;
			}

			QString CWriteClassSubsumptionsHierarchyQuery::getQueryString() {
				return mQueryString;
			}

			bool CWriteClassSubsumptionsHierarchyQuery::hasAnswer() {
				return mQueryAnswered;
			}

			QString CWriteClassSubsumptionsHierarchyQuery::getAnswerString() {
				return QString("Subclass Hierarchy written to file '%1'").arg(mOutputFileNameString);
			}


			bool CWriteClassSubsumptionsHierarchyQuery::hasError() {
				return mTaxCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
