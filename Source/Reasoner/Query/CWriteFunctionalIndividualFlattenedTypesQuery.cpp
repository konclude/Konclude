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

#include "CWriteFunctionalIndividualFlattenedTypesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWriteFunctionalIndividualFlattenedTypesQuery::CWriteFunctionalIndividualFlattenedTypesQuery(CConcreteOntology* ontology, CConfigurationBase *configuration, const QString& outputFileString, const QString& individualNameString, const QString &subClassHierarchyQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mQueryName = subClassHierarchyQueryName;
				mIndividualNameString = individualNameString;
				if (mIndividualNameString.isEmpty()) {
					mQueryString = QString("Write Individual Types");
				} else {
					mQueryString = QString("Write Individual Types for '%1'").arg(mIndividualNameString);
				}

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.ORE.RealizationResult.AbbreviatedIRIs",false);
				mWriteDeclarations = CConfigDataReader::readConfigBoolean(configuration,"Konclude.ORE.RealizationResult.WriteDeclarations",false);
				mWriteOnlyDirectTypes = CConfigDataReader::readConfigBoolean(configuration,"Konclude.ORE.RealizationResult.WriteOnlyDirectTypes",false);
				mWriteSubClassOfInconsistency = CConfigDataReader::readConfigBoolean(configuration,"Konclude.ORE.RealizationResult.WriteReducedInconsistency",false);

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mBottomClassNameString = QString("http://www.w3.org/2002/07/owl#Nothing");
				mTopClassNameString = QString("http://www.w3.org/2002/07/owl#Thing");
				mOutputFileNameString = outputFileString;
				mQueryAnswered = false;
			}


			CQueryResult *CWriteFunctionalIndividualFlattenedTypesQuery::getQueryResult() {
				return nullptr;
			}


			CQuery* CWriteFunctionalIndividualFlattenedTypesQuery::addQueryError(CQueryError* queryError) {
				if (CQueryInconsitentOntologyError::hasInconsistentOntologyError(queryError)) {
					// write inconsistency
					if (!writeInconsistentIndividualTypes()) {
						addQueryError(new CQueryUnspecifiedStringError(QString("Could not write individual types to file '%1'.").arg(mOutputFileNameString)));
					}
				}
				CQuery::addQueryError(queryError);
				return this;
			}


			void CWriteFunctionalIndividualFlattenedTypesQuery::forcedPathCreated(const QString& filePath) {
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


			bool CWriteFunctionalIndividualFlattenedTypesQuery::writeInconsistentIndividualTypes() {
				forcedPathCreated(mOutputFileNameString);
				QFile outputFile(mOutputFileNameString);
				if (outputFile.open(QIODevice::WriteOnly)) {
					writeOntologyStart(&outputFile);

					CConcept* bottomConcept = mOntology->getTBox()->getBottomConcept();

					CBOXSET<CIndividual*>* activeIndividualSet = mOntology->getABox()->getActiveIndividualSet();

					QString bottomClassName = CIRIName::getRecentIRIName(bottomConcept->getClassNameLinker());
					if (mUseAbbreviatedIRIs) {
						bottomClassName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(bottomConcept->getClassNameLinker());
					}

					if (mWriteDeclarations) {
						writeClassDeclaration(bottomClassName,&outputFile);
					}

					if (mWriteSubClassOfInconsistency) {

						CConcept* topConcept = mOntology->getTBox()->getTopConcept();
						QString topClassName = CIRIName::getRecentIRIName(topConcept->getClassNameLinker());
						if (mUseAbbreviatedIRIs) {
							topClassName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(topConcept->getClassNameLinker());
						}
						if (mWriteDeclarations) {
							writeClassDeclaration(topClassName,&outputFile);
						}
						writeSubClassRelation(topClassName,bottomClassName,&outputFile);

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
									writeIndividualDeclaration(individualName,&outputFile);
								}
								writeIndividualType(individualName,bottomClassName,&outputFile);
							}
						} else {
							if (mWriteDeclarations) {
								writeIndividualDeclaration(mIndividualNameString,&outputFile);
							}
							writeIndividualType(mIndividualNameString,bottomClassName,&outputFile);
						}
					}

					writeOntologyEnd(&outputFile);

					outputFile.close();
					return true;
				}
				return false;
			}




			bool CWriteFunctionalIndividualFlattenedTypesQuery::visitConcept(CConcept* concept, CConceptRealization* conRealization) {
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
						writeClassDeclaration(conceptString,mCurrentOutputFile);
					}
					writeIndividualType(mCurrentIndividualName,conceptString,mCurrentOutputFile);
				}
				return true;
			}





			CQueryResult *CWriteFunctionalIndividualFlattenedTypesQuery::constructResult(CRealization* realization) {
				mQueryAnswered = true;
				if (!writeIndividualTypesResult(realization)) {
					addQueryError(new CQueryUnspecifiedStringError(QString("Could not write individual types to file '%1'.").arg(mOutputFileNameString)));
				}
				return nullptr;
			}








			bool CWriteFunctionalIndividualFlattenedTypesQuery::writeIndividualTypesResult(CRealization* realization) {
				forcedPathCreated(mOutputFileNameString);
				QFile outputFile(mOutputFileNameString);
				if (outputFile.open(QIODevice::WriteOnly)) {
					mCurrentOutputFile = &outputFile;

					CConceptRealization* conRealization = realization->getConceptRealization();
					writeOntologyStart(&outputFile);

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
								writeIndividualDeclaration(mCurrentIndividualName,&outputFile);
							}
							conRealization->visitTypes(individual,mWriteOnlyDirectTypes,this);
						}
					}


					writeOntologyEnd(&outputFile);
					outputFile.close();
					return true;
				}
				return false;
			}





			void CWriteFunctionalIndividualFlattenedTypesQuery::writeIndividualDeclaration(const QString& className, QFile* outputFile) {
				QString writeString = QString("Declaration(NamedIndividual(<%1>))\n").arg(className);
				outputFile->write(writeString.toUtf8());
			}


			void CWriteFunctionalIndividualFlattenedTypesQuery::writeClassDeclaration(const QString& className, QFile* outputFile) {
				QString writeString = QString("Declaration(Class(<%1>))\n").arg(className);
				outputFile->write(writeString.toUtf8());
			}


			void CWriteFunctionalIndividualFlattenedTypesQuery::writeOntologyStart(QFile* outputFile) {
				outputFile->write(QString("Prefix(:=<http://www.w3.org/2002/07/owl#>)\n").toUtf8());
				outputFile->write(QString("Prefix(owl:=<http://www.w3.org/2002/07/owl#>)\n").toUtf8());
				outputFile->write(QString("Prefix(rdf:=<http://www.w3.org/1999/02/22-rdf-syntax-ns#>)\n").toUtf8());
				outputFile->write(QString("Prefix(xml:=<http://www.w3.org/XML/1998/namespace>)\n").toUtf8());
				outputFile->write(QString("Prefix(xsd:=<http://www.w3.org/2001/XMLSchema#>)\n").toUtf8());
				outputFile->write(QString("Prefix(rdfs:=<http://www.w3.org/2000/01/rdf-schema#>)\n").toUtf8());

				outputFile->write(QString("\nOntology(\n").toUtf8());
			}


			void CWriteFunctionalIndividualFlattenedTypesQuery::writeOntologyEnd(QFile* outputFile) {
				outputFile->write(QString(")").toUtf8());
			}


			void CWriteFunctionalIndividualFlattenedTypesQuery::writeSubClassRelation(const QString& subClassName, const QString& superClassName, QFile* outputFile) {
				QString writeString = QString("SubClassOf(<%1> <%2>)\n").arg(subClassName,superClassName);
				outputFile->write(writeString.toUtf8());
			}


			void CWriteFunctionalIndividualFlattenedTypesQuery::writeIndividualType(const QString& individualName, const QString& className, QFile* outputFile) {
				QString writeString = QString("ClassAssertion(<%1> <%2>)\n").arg(className,individualName);
				outputFile->write(writeString.toUtf8());
			}




			QString CWriteFunctionalIndividualFlattenedTypesQuery::getQueryName() {
				return mQueryName;
			}

			QString CWriteFunctionalIndividualFlattenedTypesQuery::getQueryString() {
				return mQueryString;
			}

			bool CWriteFunctionalIndividualFlattenedTypesQuery::hasAnswer() {
				return mQueryAnswered;
			}

			QString CWriteFunctionalIndividualFlattenedTypesQuery::getAnswerString() {
				if (mIndividualNameString.isEmpty()) {
					return QString("Individual Types written to file '%1'").arg(mOutputFileNameString);
					mQueryString = QString("Write Individual Types");
				} else {
					return QString("Individual Types for '%1' written to file '%2'").arg(mIndividualNameString).arg(mOutputFileNameString);
				}
			}


			bool CWriteFunctionalIndividualFlattenedTypesQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
