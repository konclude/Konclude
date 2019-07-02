/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CWriteFunctionalClassSubsumptionsHierarchyQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWriteFunctionalClassSubsumptionsHierarchyQuery::CWriteFunctionalClassSubsumptionsHierarchyQuery(CConcreteOntology *premisingTaxonomyOntology, CConfigurationBase *configuration, const QString& outputFileString, const QString &superClassName, const QString &subClassHierarchyQueryName)
					: CTaxonomyPremisingQuery(premisingTaxonomyOntology,configuration) {
				mClassName = superClassName;
				mQueryName = subClassHierarchyQueryName;
				if (mClassName.isEmpty()) {
					mQueryString = QString("Get Subclass Hierarchy");
				} else {
					mQueryString = QString("Get Subclass Hierarchy of class '%1'").arg(mClassName);
				}

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.ORE.ClassHierarchyResult.AbbreviatedIRIs",false);
				mWriteDeclarations = CConfigDataReader::readConfigBoolean(configuration,"Konclude.ORE.ClassHierarchyResult.WriteDeclarations",false);
				mWriteDirectEquivalences = CConfigDataReader::readConfigBoolean(configuration,"Konclude.ORE.ClassHierarchyResult.WriteDirectEquivalences",false);

				mWriteSimpleTopImplications = CConfigDataReader::readConfigBoolean(configuration,"Konclude.ORE.ClassHierarchyResult.WriteReducedTopSubclasses",false);
				mWriteSimpleBottomImplications = CConfigDataReader::readConfigBoolean(configuration,"Konclude.ORE.ClassHierarchyResult.WriteReducedBottomSubclasses",true);
				mWriteSimpleInconsistency = CConfigDataReader::readConfigBoolean(configuration,"Konclude.ORE.ClassHierarchyResult.WriteReducedInconsistency",true);

				mTaxCalcError = false;
				mQueryConstructError = false;

				mBottomClassNameString = QString("http://www.w3.org/2002/07/owl#Nothing");
				mTopClassNameString = QString("http://www.w3.org/2002/07/owl#Thing");
				mOutputFileNameString = outputFileString;
				mQueryAnswered = false;
			}

			CQueryResult *CWriteFunctionalClassSubsumptionsHierarchyQuery::getQueryResult() {
				return nullptr;
			}

			CQuery* CWriteFunctionalClassSubsumptionsHierarchyQuery::addQueryError(CQueryError* queryError) {
				if (CQueryInconsitentOntologyError::hasInconsistentOntologyError(queryError)) {
					// write inconsistency
					if (!writeInconsistentHierarchyResult()) {
						addQueryError(new CQueryUnspecifiedStringError(QString("Could not write class hierarchy to file '%1'.").arg(mOutputFileNameString)));
					}
				}
				CQuery::addQueryError(queryError);
				return this;
			}

			void CWriteFunctionalClassSubsumptionsHierarchyQuery::forcedPathCreated(const QString& filePath) {
				QString path = filePath;
				if (path.contains("/") || path.contains("\\")) {
					int lastSlash = path.lastIndexOf("/");
					int lastBackSlash = path.lastIndexOf("\\");
					int lastSeparator = qMax(lastBackSlash,lastSlash);
					path = path.mid(0,lastSeparator);
					if (!path.isEmpty()) {
						QDir dir;
						dir.mkpath(path);
					}
				}
			}

			bool CWriteFunctionalClassSubsumptionsHierarchyQuery::writeInconsistentHierarchyResult() {
				forcedPathCreated(mOutputFileNameString);
				QFile outputFile(mOutputFileNameString);
				if (outputFile.open(QIODevice::WriteOnly)) {
					writeOntologyStart(&outputFile);

					CConcept* mTopConcept = ontology->getTBox()->getTopConcept();
					CConcept* mBottomConcept = ontology->getTBox()->getTopConcept();

					CBOXSET<CConcept*>* activeClassConceptSet = ontology->getTBox()->getActiveClassConceptSet();

					if (mWriteSimpleInconsistency) {
						writeSubClassRelation(mTopClassNameString,mBottomClassNameString,&outputFile);
					} else {
						if (mWriteDeclarations) {
							for (CBOXSET<CConcept*>::const_iterator it = activeClassConceptSet->constBegin(), itEnd = activeClassConceptSet->constEnd(); it != itEnd; ++it) {
								CConcept* concept(*it);
								QString className = CIRIName::getRecentIRIName(concept->getClassNameLinker());
								if (mUseAbbreviatedIRIs) {
									className = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(concept->getClassNameLinker());
								}
								writeDeclaration(className,&outputFile);
							}
						}

						if (mWriteDirectEquivalences) {
							QStringList incClassNameList;
							for (CBOXSET<CConcept*>::const_iterator it = activeClassConceptSet->constBegin(), itEnd = activeClassConceptSet->constEnd(); it != itEnd; ++it) {
								CConcept* concept(*it);
								QString className = CIRIName::getRecentIRIName(concept->getClassNameLinker());
								if (mUseAbbreviatedIRIs) {
									className = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(concept->getClassNameLinker());
								}
								incClassNameList.append(className);
							}
							writeEquivalenceRelations(incClassNameList,&outputFile);
						} else {
							for (CBOXSET<CConcept*>::const_iterator it1 = activeClassConceptSet->constBegin(), it1End = activeClassConceptSet->constEnd(); it1 != it1End; ++it1) {
								CConcept* concept1(*it1);
								if (concept1 != mBottomConcept) {
									for (CBOXSET<CConcept*>::const_iterator it2 = activeClassConceptSet->constBegin(), it2End = activeClassConceptSet->constEnd(); it2 != it1End; ++it2) {
										CConcept* concept2(*it2);
										if (concept1 != concept2 && concept2 != mTopConcept) {
											QString className1 = CIRIName::getRecentIRIName(concept1->getClassNameLinker());
											QString className2 = CIRIName::getRecentIRIName(concept2->getClassNameLinker());
											if (mUseAbbreviatedIRIs) {
												className1 = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(concept1->getClassNameLinker());
												className2 = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(concept2->getClassNameLinker());
											}
											writeSubClassRelation(className1,className2,&outputFile);
										}
									}
								}
							}
						}
					}

					writeOntologyEnd(&outputFile);
					outputFile.close();
					return true;
				}
				return false;
			}


			CQueryResult *CWriteFunctionalClassSubsumptionsHierarchyQuery::constructResult(CTaxonomy *taxonomy) {
				mQueryAnswered = true;
				if (!writeSubClassHierarchyResult(taxonomy)) {
					addQueryError(new CQueryUnspecifiedStringError(QString("Could not write class hierarchy to file '%1'.").arg(mOutputFileNameString)));
				}
				return nullptr;
			}

			bool CWriteFunctionalClassSubsumptionsHierarchyQuery::writeSubClassHierarchyResult(CTaxonomy *taxonomy) {
				forcedPathCreated(mOutputFileNameString);
				QFile outputFile(mOutputFileNameString);
				if (outputFile.open(QIODevice::WriteOnly)) {

					writeOntologyStart(&outputFile);

					CHierarchyNode* topNode = taxonomy->getTopHierarchyNode();
					CHierarchyNode* bottomNode = taxonomy->getBottomHierarchyNode();
					CHierarchyNode* basicNode = topNode;
					if (!mClassName.isEmpty()) {
						basicNode = taxonomy->getHierarchyNode(ontology->getConcept(mClassName));
					}
					writeDeclarations(bottomNode,&outputFile);
					writeBottomEquivalences(bottomNode,&outputFile);
					QSet<CHierarchyNode *> processNodeSet;
					processNodeSet.insert(bottomNode);
					if (basicNode) {
						QList<CHierarchyNode *> processNodeList;
						writeDeclarations(basicNode,&outputFile);
						writeTopEquivalences(basicNode,&outputFile);
						processNodeSet.insert(basicNode);
						processNodeList.append(basicNode);
						while (!processNodeList.isEmpty()) {
							CHierarchyNode *procNode = processNodeList.takeFirst();
							QSet<CHierarchyNode *>* childNodeSet(procNode->getChildNodeSet());
							foreach (CHierarchyNode *childNode, *childNodeSet) {

								if (childNode != bottomNode) {
									if (!processNodeSet.contains(childNode)) {
										writeEquivalences(childNode,&outputFile);
										writeDeclarations(childNode,&outputFile);
										processNodeSet.insert(childNode);
										processNodeList.append(childNode);
									}

									writeSubClassRelations(procNode,childNode,&outputFile);
								}
							}
						}
					}
					writeOntologyEnd(&outputFile);
					outputFile.close();
					return true;
				}
				return false;
			}

			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeBottomEquivalences(CHierarchyNode* node, QFile* outputFile) {
				if (node->getEquivalentConceptCount() > 1) {
					if (mWriteDirectEquivalences) {
						writeEquivalenceRelations(node,outputFile);
					} else {
						if (mWriteSimpleBottomImplications) {
							const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
							foreach (const QString& className1, classNameList) {
								if (className1 != mBottomClassNameString) {
									writeSubClassRelation(className1,mBottomClassNameString,outputFile);
								}
							}
						} else {
							const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
							foreach (const QString& className1, classNameList) {
								foreach (const QString& className2, classNameList) {
									if (className1 != className2 && className1 != mBottomClassNameString) {
										writeSubClassRelation(className1,className2,outputFile);
									}
								}
							}
						}
					}
				}
			}

			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeTopEquivalences(CHierarchyNode* node, QFile* outputFile) {
				if (node->getEquivalentConceptCount() > 1) {
					if (mWriteDirectEquivalences) {
						writeEquivalenceRelations(node,outputFile);
					} else {
						if (mWriteSimpleTopImplications) {
							const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
							foreach (const QString& className1, classNameList) {
								if (className1 != mTopClassNameString) {
									writeSubClassRelation(mTopClassNameString,className1,outputFile);
								}
							}
						} else {
							const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
							foreach (const QString& className1, classNameList) {
								foreach (const QString& className2, classNameList) {
									if (className1 != className2 && className2 != mTopClassNameString) {
										writeSubClassRelation(className1,className2,outputFile);
									}
								}
							}
						}
					}
				}
			}


			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeEquivalenceRelations(const QStringList& classNameList, QFile* outputFile) {
				QString outputClassNameList;
				foreach (const QString& className, classNameList) {
					if (!outputClassNameList.isEmpty()) {
						outputClassNameList += QString(" ");
					}
					outputClassNameList += QString("<%1>").arg(className);
				}
				QString writeString = QString("EquivalentClasses(%1)\n").arg(outputClassNameList);
				outputFile->write(writeString.toUtf8());
			}


			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeEquivalenceRelations(CHierarchyNode* node, QFile* outputFile) {
				const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
				writeEquivalenceRelations(classNameList,outputFile);
			}



			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeEquivalences(CHierarchyNode* node, QFile* outputFile) {
				if (node->getEquivalentConceptCount() > 1) {
					if (mWriteDirectEquivalences) {
						writeEquivalenceRelations(node,outputFile);
					} else {
						const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
						foreach (const QString& className1, classNameList) {
							foreach (const QString& className2, classNameList) {
								if (className1 != className2) {
									writeSubClassRelation(className1,className2,outputFile);
								}
							}
						}
					}
				}
			}


			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeDeclarations(CHierarchyNode* node, QFile* outputFile) {
				if (mWriteDeclarations) {
					const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
					foreach (const QString& className, classNameList) {
						writeDeclaration(className,outputFile);
					}
				}
			}


			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeDeclaration(const QString& className, QFile* outputFile) {
				QString writeString = QString("Declaration(Class(<%1>))\n").arg(className);
				outputFile->write(writeString.toUtf8());
			}


			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeOntologyStart(QFile* outputFile) {
				outputFile->write(QString("Prefix(:=<http://www.w3.org/2002/07/owl#>)\n").toUtf8());
				outputFile->write(QString("Prefix(owl:=<http://www.w3.org/2002/07/owl#>)\n").toUtf8());
				outputFile->write(QString("Prefix(rdf:=<http://www.w3.org/1999/02/22-rdf-syntax-ns#>)\n").toUtf8());
				outputFile->write(QString("Prefix(xml:=<http://www.w3.org/XML/1998/namespace>)\n").toUtf8());
				outputFile->write(QString("Prefix(xsd:=<http://www.w3.org/2001/XMLSchema#>)\n").toUtf8());
				outputFile->write(QString("Prefix(rdfs:=<http://www.w3.org/2000/01/rdf-schema#>)\n").toUtf8());

				outputFile->write(QString("\nOntology(\n").toUtf8());
			}


			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeOntologyEnd(QFile* outputFile) {
				outputFile->write(QString(")").toUtf8());
			}


			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeSubClassRelations(CHierarchyNode* superNode, CHierarchyNode* subNode, QFile* outputFile) {
				const QStringList& superClassNameList(superNode->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
				const QStringList& subClassNameList(subNode->getEquivalentConceptStringList(mUseAbbreviatedIRIs));

				foreach (const QString& subClassName, subClassNameList) {
					foreach (const QString& superClassName, superClassNameList) {
						writeSubClassRelation(subClassName,superClassName,outputFile);
					}
				}

			}


			void CWriteFunctionalClassSubsumptionsHierarchyQuery::writeSubClassRelation(const QString& subClassName, const QString& superClassName, QFile* outputFile) {
				QString writeString = QString("SubClassOf(<%1> <%2>)\n").arg(subClassName,superClassName);
				outputFile->write(writeString.toUtf8());
			}



			QString CWriteFunctionalClassSubsumptionsHierarchyQuery::getQueryName() {
				return mQueryName;
			}

			QString CWriteFunctionalClassSubsumptionsHierarchyQuery::getQueryString() {
				return mQueryString;
			}

			bool CWriteFunctionalClassSubsumptionsHierarchyQuery::hasAnswer() {
				return mQueryAnswered;
			}

			QString CWriteFunctionalClassSubsumptionsHierarchyQuery::getAnswerString() {
				return QString("Subclass Hierarchy written to file '%1'").arg(mOutputFileNameString);
			}


			bool CWriteFunctionalClassSubsumptionsHierarchyQuery::hasError() {
				return mTaxCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
