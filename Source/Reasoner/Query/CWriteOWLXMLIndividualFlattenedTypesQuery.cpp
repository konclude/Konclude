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

#include "CWriteOWLXMLIndividualFlattenedTypesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWriteOWLXMLIndividualFlattenedTypesQuery::CWriteOWLXMLIndividualFlattenedTypesQuery(CConcreteOntology* ontology, CConfigurationBase *configuration, const QString& outputFileString, const QString& individualNameString, const QString &subClassHierarchyQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mQueryName = subClassHierarchyQueryName;
				mIndividualNameString = individualNameString;
				if (mIndividualNameString.isEmpty()) {
					mQueryString = QString("Write Individual Types");
				} else {
					mQueryString = QString("Write Individual Types for '%1'").arg(mIndividualNameString);
				}

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.CLI.Output.AbbreviatedIRIs",false);
				mWriteDeclarations = CConfigDataReader::readConfigBoolean(configuration,"Konclude.CLI.Output.WriteDeclarations",false);
				mWriteOnlyDirectTypes = CConfigDataReader::readConfigBoolean(configuration,"Konclude.CLI.Output.WriteOnlyDirectTypes",false);
				mWriteSubClassOfInconsistency = CConfigDataReader::readConfigBoolean(configuration,"Konclude.CLI.Output.WriteReducedInconsistency",false);

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mBottomClassNameString = QString("http://www.w3.org/2002/07/owl#Nothing");
				mTopClassNameString = QString("http://www.w3.org/2002/07/owl#Thing");
				mOutputFileNameString = outputFileString;
				mQueryAnswered = false;

				mRequiresSameIndividualRealisation = true;
				mRequiresConceptRealisation = true;
			}


			CQueryResult *CWriteOWLXMLIndividualFlattenedTypesQuery::getQueryResult() {
				return nullptr;
			}


			CQuery* CWriteOWLXMLIndividualFlattenedTypesQuery::addQueryError(CQueryError* queryError) {
				if (CQueryInconsitentOntologyError::hasInconsistentOntologyError(queryError)) {
					// write inconsistency
					if (!writeInconsistentIndividualTypes()) {
						addQueryError(new CQueryUnspecifiedStringError(QString("Could not write individual types to file '%1'.").arg(mOutputFileNameString)));
					}
				}
				CQuery::addQueryError(queryError);
				return this;
			}


			void CWriteOWLXMLIndividualFlattenedTypesQuery::forcedPathCreated(const QString& filePath) {
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


			bool CWriteOWLXMLIndividualFlattenedTypesQuery::writeInconsistentIndividualTypes() {
				forcedPathCreated(mOutputFileNameString);
				QFile outputFile(mOutputFileNameString);
				if (outputFile.open(QIODevice::WriteOnly)) {

					QXmlStreamWriter outputStreamWriter(&outputFile);
					outputStreamWriter.setAutoFormatting(true);
					outputStreamWriter.writeStartDocument();

					writeOntologyStart(&outputStreamWriter);

					CConcept* bottomConcept = mOntology->getTBox()->getBottomConcept();

					CBOXSET<CIndividual*>* activeIndividualSet = mOntology->getABox()->getActiveIndividualSet();

					QString bottomClassName = CIRIName::getRecentIRIName(bottomConcept->getClassNameLinker());
					if (mUseAbbreviatedIRIs) {
						bottomClassName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(bottomConcept->getClassNameLinker());
					}

					if (mWriteDeclarations) {
						writeClassDeclaration(bottomClassName,&outputStreamWriter);
					}

					if (mWriteSubClassOfInconsistency) {

						CConcept* topConcept = mOntology->getTBox()->getTopConcept();
						QString topClassName = CIRIName::getRecentIRIName(topConcept->getClassNameLinker());
						if (mUseAbbreviatedIRIs) {
							topClassName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(topConcept->getClassNameLinker());
						}
						if (mWriteDeclarations) {
							writeClassDeclaration(topClassName,&outputStreamWriter);
						}
						writeSubClassRelation(topClassName,bottomClassName,&outputStreamWriter);

					} else {
						if (mIndividualNameString.isEmpty()) {
							QStringList incClassNameList;
							for (CBOXSET<CIndividual*>::const_iterator it = activeIndividualSet->constBegin(), itEnd = activeIndividualSet->constEnd(); it != itEnd; ++it) {
								CIndividual* individual(*it);
								QString individualName = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
								if (mUseAbbreviatedIRIs) {
									individualName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(individual->getIndividualNameLinker());
								}
								if (mWriteDeclarations) {
									writeIndividualDeclaration(individualName,&outputStreamWriter);
								}
								writeIndividualType(individualName,bottomClassName,&outputStreamWriter);
							}
						} else {
							if (mWriteDeclarations) {
								writeIndividualDeclaration(mIndividualNameString,&outputStreamWriter);
							}
							writeIndividualType(mIndividualNameString,bottomClassName,&outputStreamWriter);
						}
					}

					writeOntologyEnd(&outputStreamWriter);

					outputStreamWriter.writeEndDocument();
					outputFile.close();
					return true;
				}
				return false;
			}




			bool CWriteOWLXMLIndividualFlattenedTypesQuery::visitConcept(CConcept* concept, CConceptRealization* conRealization) {
				QString conceptString;
				if (mUseAbbreviatedIRIs) {
					conceptString = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(concept->getClassNameLinker());
				} 
				if (conceptString.isEmpty()) {
					conceptString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
				}
				if (!conceptString.isEmpty()) {
					if (mWriteDeclarations && !mDeclaratedConceptSet.contains(concept)) {
						mDeclaratedConceptSet.insert(concept);
						writeClassDeclaration(conceptString,mCurrentOutputStreamWriter);
					}
					writeIndividualType(mCurrentIndividualName,conceptString,mCurrentOutputStreamWriter);
				}
				return true;
			}





			CQueryResult *CWriteOWLXMLIndividualFlattenedTypesQuery::constructResult(CRealization* realization) {
				mQueryAnswered = true;
				if (!writeIndividualTypesResult(realization)) {
					addQueryError(new CQueryUnspecifiedStringError(QString("Could not write individual types to file '%1'.").arg(mOutputFileNameString)));
				}
				return nullptr;
			}








			bool CWriteOWLXMLIndividualFlattenedTypesQuery::writeIndividualTypesResult(CRealization* realization) {
				forcedPathCreated(mOutputFileNameString);
				QFile outputFile(mOutputFileNameString);
				if (outputFile.open(QIODevice::WriteOnly)) {

					QXmlStreamWriter outputStreamWriter(&outputFile);
					outputStreamWriter.setAutoFormatting(true);
					outputStreamWriter.writeStartDocument();

					mCurrentOutputStreamWriter = &outputStreamWriter;

					CConceptRealization* conRealization = realization->getConceptRealization();

					writeOntologyStart(&outputStreamWriter);

					CBOXSET<CIndividual*>* activeIndividualSet = mOntology->getABox()->getActiveIndividualSet();

					QStringList incClassNameList;
					for (CBOXSET<CIndividual*>::const_iterator it = activeIndividualSet->constBegin(), itEnd = activeIndividualSet->constEnd(); it != itEnd; ++it) {
						CIndividual* individual(*it);
						QString individualName = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
						QString abbreviatedIndividualName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(individual->getIndividualNameLinker());
						if (mIndividualNameString.isEmpty() || individualName == mIndividualNameString || individualName == abbreviatedIndividualName) {
							if (mUseAbbreviatedIRIs) {
								mCurrentIndividualName = abbreviatedIndividualName;
							} else {
								mCurrentIndividualName = individualName;
							}
							if (mWriteDeclarations) {
								writeIndividualDeclaration(mCurrentIndividualName,&outputStreamWriter);
							}
							conRealization->visitTypes(individual,mWriteOnlyDirectTypes,this);
						}
					}


					writeOntologyEnd(&outputStreamWriter);
					outputStreamWriter.writeEndDocument();
					outputFile.close();
					return true;
				}
				return false;
			}



			void CWriteOWLXMLIndividualFlattenedTypesQuery::writeSubClassRelation(const QString& subClassName, const QString& superClassName, QXmlStreamWriter* outputStreamWriter) {
				outputStreamWriter->writeStartElement("SubClassOf");
				outputStreamWriter->writeStartElement("Class");
				outputStreamWriter->writeAttribute("IRI",subClassName);
				outputStreamWriter->writeEndElement();
				outputStreamWriter->writeStartElement("Class");
				outputStreamWriter->writeAttribute("IRI",superClassName);
				outputStreamWriter->writeEndElement();
				outputStreamWriter->writeEndElement();
			}


			void CWriteOWLXMLIndividualFlattenedTypesQuery::writeIndividualDeclaration(const QString& className, QXmlStreamWriter* outputStreamWriter) {
				outputStreamWriter->writeStartElement("Declaration");
				outputStreamWriter->writeStartElement("NamedIndividual");
				outputStreamWriter->writeAttribute("IRI",className);
				outputStreamWriter->writeEndElement();
				outputStreamWriter->writeEndElement();
			}


			void CWriteOWLXMLIndividualFlattenedTypesQuery::writeClassDeclaration(const QString& className, QXmlStreamWriter* outputStreamWriter) {
				outputStreamWriter->writeStartElement("Declaration");
				outputStreamWriter->writeStartElement("Class");
				outputStreamWriter->writeAttribute("IRI",className);
				outputStreamWriter->writeEndElement();
				outputStreamWriter->writeEndElement();
			}


			void CWriteOWLXMLIndividualFlattenedTypesQuery::writeOntologyPrefix(QXmlStreamWriter* outputStreamWriter, const QString& prefixName, const QString& prefixIRI) {
				outputStreamWriter->writeStartElement("Prefix");
				outputStreamWriter->writeAttribute("name",prefixName);
				outputStreamWriter->writeAttribute("IRI",prefixIRI);
				outputStreamWriter->writeEndElement();
			}


			void CWriteOWLXMLIndividualFlattenedTypesQuery::writeOntologyStart(QXmlStreamWriter* outputStreamWriter) {
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


			void CWriteOWLXMLIndividualFlattenedTypesQuery::writeOntologyEnd(QXmlStreamWriter* outputStreamWriter) {
				outputStreamWriter->writeEndElement();
			}




			void CWriteOWLXMLIndividualFlattenedTypesQuery::writeIndividualType(const QString& individualName, const QString& className, QXmlStreamWriter* outputStreamWriter) {
				outputStreamWriter->writeStartElement("ClassAssertion");
				outputStreamWriter->writeStartElement("Class");
				outputStreamWriter->writeAttribute("IRI",className);
				outputStreamWriter->writeEndElement();
				outputStreamWriter->writeStartElement("NamedIndividual");
				outputStreamWriter->writeAttribute("IRI",individualName);
				outputStreamWriter->writeEndElement();
				outputStreamWriter->writeEndElement();
			}




			QString CWriteOWLXMLIndividualFlattenedTypesQuery::getQueryName() {
				return mQueryName;
			}

			QString CWriteOWLXMLIndividualFlattenedTypesQuery::getQueryString() {
				return mQueryString;
			}

			bool CWriteOWLXMLIndividualFlattenedTypesQuery::hasAnswer() {
				return mQueryAnswered;
			}

			QString CWriteOWLXMLIndividualFlattenedTypesQuery::getAnswerString() {
				if (mIndividualNameString.isEmpty()) {
					return QString("Individual Types written to file '%1'").arg(mOutputFileNameString);
					mQueryString = QString("Write Individual Types");
				} else {
					return QString("Individual Types for '%1' written to file '%2'").arg(mIndividualNameString).arg(mOutputFileNameString);
				}
			}


			bool CWriteOWLXMLIndividualFlattenedTypesQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
