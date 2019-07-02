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

#include "CWriteOWLXMLClassSubsumptionsHierarchyQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWriteOWLXMLClassSubsumptionsHierarchyQuery::CWriteOWLXMLClassSubsumptionsHierarchyQuery(CConcreteOntology *premisingTaxonomyOntology, CConfigurationBase *configuration, const QString& outputFileString, const QString &superClassName, const QString &subClassHierarchyQueryName)
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

				mBottomClassNameString = QString("http://www.w3.org/2002/07/owl#Nothing");
				mTopClassNameString = QString("http://www.w3.org/2002/07/owl#Thing");
				mOutputFileNameString = outputFileString;
				mQueryAnswered = false;
			}

			CQueryResult *CWriteOWLXMLClassSubsumptionsHierarchyQuery::getQueryResult() {
				return nullptr;
			}

			CQuery* CWriteOWLXMLClassSubsumptionsHierarchyQuery::addQueryError(CQueryError* queryError) {
				if (CQueryInconsitentOntologyError::hasInconsistentOntologyError(queryError)) {
					// write inconsistency
					if (!writeInconsistentHierarchyResult()) {
						addQueryError(new CQueryUnspecifiedStringError(QString("Could not write class hierarchy to file '%1'.").arg(mOutputFileNameString)));
					}
				}
				CQuery::addQueryError(queryError);
				return this;
			}

			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::forcedPathCreated(const QString& filePath) {
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

			bool CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeInconsistentHierarchyResult() {
				forcedPathCreated(mOutputFileNameString);
				QFile outputFile(mOutputFileNameString);
				if (outputFile.open(QIODevice::WriteOnly)) {

					QXmlStreamWriter outputStreamWriter(&outputFile);
					outputStreamWriter.setAutoFormatting(true);
					outputStreamWriter.writeStartDocument();

					writeOntologyStart(&outputStreamWriter);

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
							writeDeclaration(className,&outputStreamWriter);
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
					writeEquivalenceRelations(incClassNameList,&outputStreamWriter);

					writeOntologyEnd(&outputStreamWriter);

					outputStreamWriter.writeEndDocument();
					outputFile.close();
					return true;
				}
				return false;
			}


			CQueryResult *CWriteOWLXMLClassSubsumptionsHierarchyQuery::constructResult(CTaxonomy *taxonomy) {
				mQueryAnswered = true;
				if (!writeSubClassHierarchyResult(taxonomy)) {
					addQueryError(new CQueryUnspecifiedStringError(QString("Could not write class hierarchy to file '%1'.").arg(mOutputFileNameString)));
				}
				return nullptr;
			}

			bool CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeSubClassHierarchyResult(CTaxonomy *taxonomy) {
				forcedPathCreated(mOutputFileNameString);
				QFile outputFile(mOutputFileNameString);
				if (outputFile.open(QIODevice::WriteOnly)) {

					QXmlStreamWriter outputStreamWriter(&outputFile);
					outputStreamWriter.setAutoFormatting(true);
					outputStreamWriter.writeStartDocument();


					writeOntologyStart(&outputStreamWriter);

					CHierarchyNode* topNode = taxonomy->getTopHierarchyNode();
					CHierarchyNode* bottomNode = taxonomy->getBottomHierarchyNode();
					CHierarchyNode* basicNode = topNode;
					if (!mClassName.isEmpty()) {
						basicNode = taxonomy->getHierarchyNode(ontology->getConcept(mClassName));
					}
					writeDeclarations(bottomNode,&outputStreamWriter);
					writeBottomEquivalences(bottomNode,&outputStreamWriter);
					QSet<CHierarchyNode *> processNodeSet;
					processNodeSet.insert(bottomNode);
					if (basicNode) {
						QList<CHierarchyNode *> processNodeList;
						writeDeclarations(basicNode,&outputStreamWriter);
						writeTopEquivalences(basicNode,&outputStreamWriter);
						processNodeSet.insert(basicNode);
						processNodeList.append(basicNode);
						while (!processNodeList.isEmpty()) {
							CHierarchyNode *procNode = processNodeList.takeFirst();
							QSet<CHierarchyNode *>* childNodeSet(procNode->getChildNodeSet());
							foreach (CHierarchyNode *childNode, *childNodeSet) {

								if (!processNodeSet.contains(childNode)) {
									writeEquivalences(childNode,&outputStreamWriter);
									writeDeclarations(childNode,&outputStreamWriter);
									processNodeSet.insert(childNode);
									processNodeList.append(childNode);
								}

								if (childNode != bottomNode) {
									writeSubClassRelations(procNode,childNode,&outputStreamWriter);
								}
							}
						}
					}
					writeOntologyEnd(&outputStreamWriter);
					outputStreamWriter.writeEndDocument();
					outputFile.close();
					return true;
				}
				return false;
			}

			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeBottomEquivalences(CHierarchyNode* node, QXmlStreamWriter* outputStreamWriter) {
				if (node->getEquivalentConceptCount() > 1) {
					writeEquivalenceRelations(node,outputStreamWriter);
				}
			}

			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeTopEquivalences(CHierarchyNode* node, QXmlStreamWriter* outputStreamWriter) {
				if (node->getEquivalentConceptCount() > 1) {
					writeEquivalenceRelations(node,outputStreamWriter);
				}
			}


			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeEquivalenceRelations(const QStringList& classNameList, QXmlStreamWriter* outputStreamWriter) {
				outputStreamWriter->writeStartElement("EquivalentClasses");
				foreach (const QString& className, classNameList) {
					outputStreamWriter->writeStartElement("Class");
					outputStreamWriter->writeAttribute("IRI",className);
					outputStreamWriter->writeEndElement();
				}
				outputStreamWriter->writeEndElement();
			}


			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeEquivalenceRelations(CHierarchyNode* node, QXmlStreamWriter* outputStreamWriter) {
				const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
				writeEquivalenceRelations(classNameList,outputStreamWriter);
			}



			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeEquivalences(CHierarchyNode* node, QXmlStreamWriter* outputStreamWriter) {
				if (node->getEquivalentConceptCount() > 1) {
					writeEquivalenceRelations(node,outputStreamWriter);
				}
			}


			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeDeclarations(CHierarchyNode* node, QXmlStreamWriter* outputStreamWriter) {
				if (mWriteDeclarations) {
					const QStringList& classNameList(node->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
					foreach (const QString& className, classNameList) {
						writeDeclaration(className,outputStreamWriter);
					}
				}
			}


			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeDeclaration(const QString& className, QXmlStreamWriter* outputStreamWriter) {
				outputStreamWriter->writeStartElement("Declaration");
				outputStreamWriter->writeStartElement("Class");
				outputStreamWriter->writeAttribute("IRI",className);
				outputStreamWriter->writeEndElement();
				outputStreamWriter->writeEndElement();
			}


			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeOntologyPrefix(QXmlStreamWriter* outputStreamWriter, const QString& prefixName, const QString& prefixIRI) {
				outputStreamWriter->writeStartElement("Prefix");
				outputStreamWriter->writeAttribute("name",prefixName);
				outputStreamWriter->writeAttribute("IRI",prefixIRI);
				outputStreamWriter->writeEndElement();
			}


			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeOntologyStart(QXmlStreamWriter* outputStreamWriter) {
				outputStreamWriter->writeStartElement("Ontology");
				outputStreamWriter->writeAttribute("xmlns:rdfs","http://www.w3.org/2000/01/rdf-schema#");
				outputStreamWriter->writeAttribute("xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
				outputStreamWriter->writeAttribute("xmlns","http://www.w3.org/2002/07/owl#");
				outputStreamWriter->writeAttribute("xmlns:xml","http://www.w3.org/XML/1998/namespace");
				outputStreamWriter->writeAttribute("xmlns:xsd","http://www.w3.org/2001/XMLSchema#");

				writeOntologyPrefix(outputStreamWriter,"","http://www.w3.org/2002/07/owl#");
				writeOntologyPrefix(outputStreamWriter,"owl","http://www.w3.org/2002/07/owl#");
				writeOntologyPrefix(outputStreamWriter,"rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
				writeOntologyPrefix(outputStreamWriter,"xml","http://www.w3.org/XML/1998/namespace");
				writeOntologyPrefix(outputStreamWriter,"xsd","http://www.w3.org/2001/XMLSchema#");
				writeOntologyPrefix(outputStreamWriter,"rdfs","http://www.w3.org/2000/01/rdf-schema#");
			}


			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeOntologyEnd(QXmlStreamWriter* outputStreamWriter) {
				outputStreamWriter->writeEndElement();
			}


			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeSubClassRelations(CHierarchyNode* superNode, CHierarchyNode* subNode, QXmlStreamWriter* outputStreamWriter) {
				const QStringList& superClassNameList(superNode->getEquivalentConceptStringList(mUseAbbreviatedIRIs));
				const QStringList& subClassNameList(subNode->getEquivalentConceptStringList(mUseAbbreviatedIRIs));

				writeSubClassRelation(subClassNameList.first(),superClassNameList.first(),outputStreamWriter);
			}


			void CWriteOWLXMLClassSubsumptionsHierarchyQuery::writeSubClassRelation(const QString& subClassName, const QString& superClassName, QXmlStreamWriter* outputStreamWriter) {
				outputStreamWriter->writeStartElement("SubClassOf");
				outputStreamWriter->writeStartElement("Class");
				outputStreamWriter->writeAttribute("IRI",subClassName);
				outputStreamWriter->writeEndElement();
				outputStreamWriter->writeStartElement("Class");
				outputStreamWriter->writeAttribute("IRI",superClassName);
				outputStreamWriter->writeEndElement();
				outputStreamWriter->writeEndElement();
			}



			QString CWriteOWLXMLClassSubsumptionsHierarchyQuery::getQueryName() {
				return mQueryName;
			}

			QString CWriteOWLXMLClassSubsumptionsHierarchyQuery::getQueryString() {
				return mQueryString;
			}

			bool CWriteOWLXMLClassSubsumptionsHierarchyQuery::hasAnswer() {
				return mQueryAnswered;
			}

			QString CWriteOWLXMLClassSubsumptionsHierarchyQuery::getAnswerString() {
				return QString("Subclass Hierarchy written to file '%1'").arg(mOutputFileNameString);
			}


			bool CWriteOWLXMLClassSubsumptionsHierarchyQuery::hasError() {
				return mTaxCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
