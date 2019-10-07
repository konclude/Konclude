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

#include "CWriteQueryFileOWL2FunctionalSerializer.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWriteQueryFileOWL2FunctionalSerializer::CWriteQueryFileOWL2FunctionalSerializer(const QString& fileString) : CWriteQueryFileSerializer(fileString) {
			}



			void CWriteQueryFileOWL2FunctionalSerializer::writeClassEquivalenceRelations(const QStringList& classNameList) {
				QString outputClassNameList;
				foreach (const QString& className, classNameList) {
					if (!outputClassNameList.isEmpty()) {
						outputClassNameList += QString(" ");
					}
					outputClassNameList += QString("<%1>").arg(className);
				}
				QString writeString = QString("EquivalentClasses(%1)\n").arg(outputClassNameList);
				mCurrentOutputFile->write(writeString.toUtf8());
			}


			void CWriteQueryFileOWL2FunctionalSerializer::writeObjectPropertyEquivalenceRelations(const QStringList& propertyNameList) {
				QString outputPropertyNameList;
				foreach (const QString& propertyName, propertyNameList) {
					if (!outputPropertyNameList.isEmpty()) {
						outputPropertyNameList += QString(" ");
					}
					outputPropertyNameList += QString("<%1>").arg(propertyName);
				}
				QString writeString = QString("EquivalentObjectProperties(%1)\n").arg(outputPropertyNameList);
				mCurrentOutputFile->write(writeString.toUtf8());
			}


			void CWriteQueryFileOWL2FunctionalSerializer::writeDataPropertyEquivalenceRelations(const QStringList& propertyNameList) {
				QString outputPropertyNameList;
				foreach (const QString& propertyName, propertyNameList) {
					if (!outputPropertyNameList.isEmpty()) {
						outputPropertyNameList += QString(" ");
					}
					outputPropertyNameList += QString("<%1>").arg(propertyName);
				}
				QString writeString = QString("EquivalentDataProperties(%1)\n").arg(outputPropertyNameList);
				mCurrentOutputFile->write(writeString.toUtf8());
			}





			void CWriteQueryFileOWL2FunctionalSerializer::writeClassDeclaration(const QString& className) {
				QString writeString = QString("Declaration(Class(<%1>))\n").arg(className);
				mCurrentOutputFile->write(writeString.toUtf8());
			}


			void CWriteQueryFileOWL2FunctionalSerializer::writeObjectPropertyDeclaration(const QString& propertyName) {
				QString writeString = QString("Declaration(ObjectProperty(<%1>))\n").arg(propertyName);
				mCurrentOutputFile->write(writeString.toUtf8());
			}

			void CWriteQueryFileOWL2FunctionalSerializer::writeDataPropertyDeclaration(const QString& propertyName) {
				QString writeString = QString("Declaration(DataProperty(<%1>))\n").arg(propertyName);
				mCurrentOutputFile->write(writeString.toUtf8());
			}


			void CWriteQueryFileOWL2FunctionalSerializer::writeOntologyStart() {
				writeOntologyPrefix("","http://www.w3.org/2002/07/owl#");
				writeOntologyPrefix("owl","http://www.w3.org/2002/07/owl#");
				writeOntologyPrefix("rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
				writeOntologyPrefix("xml","http://www.w3.org/XML/1998/namespace");
				writeOntologyPrefix("xsd","http://www.w3.org/2001/XMLSchema#");
				writeOntologyPrefix("rdfs","http://www.w3.org/2000/01/rdf-schema#");

				mCurrentOutputFile->write(QString("\nOntology(\n").toUtf8());
			}



			void CWriteQueryFileOWL2FunctionalSerializer::writeOntologyPrefix(const QString& prefixName, const QString& prefixIRI) {
				mCurrentOutputFile->write(QString("Prefix(%1:=<%2>)\n").arg(prefixName).arg(prefixIRI).toUtf8());
			}


			void CWriteQueryFileOWL2FunctionalSerializer::writeOntologyEnd() {
				mCurrentOutputFile->write(QString(")").toUtf8());
			}




			void CWriteQueryFileOWL2FunctionalSerializer::writeSubClassRelation(const QString& subClassName, const QString& superClassName) {
				QString writeString = QString("SubClassOf(<%1> <%2>)\n").arg(subClassName,superClassName);
				mCurrentOutputFile->write(writeString.toUtf8());
			}

			void CWriteQueryFileOWL2FunctionalSerializer::writeSubObjectPropertyRelation(const QString& subPropertyName, const QString& superPropertyName) {
				QString writeString = QString("SubObjectPropertyOf(<%1> <%2>)\n").arg(subPropertyName,superPropertyName);
				mCurrentOutputFile->write(writeString.toUtf8());
			}	

			void CWriteQueryFileOWL2FunctionalSerializer::writeSubDataPropertyRelation(const QString& subPropertyName, const QString& superPropertyName) {
				QString writeString = QString("SubDataPropertyOf(<%1> <%2>)\n").arg(subPropertyName,superPropertyName);
				mCurrentOutputFile->write(writeString.toUtf8());
			}	


			void CWriteQueryFileOWL2FunctionalSerializer::writeNamedIndividualType(const QString& individualName, const QString& className) {
				QString writeString = QString("ClassAssertion(<%1> <%2>)\n").arg(className,individualName);
				mCurrentOutputFile->write(writeString.toUtf8());
			}


			void CWriteQueryFileOWL2FunctionalSerializer::writeAnonymousIndividualType(const QString& individualName, const QString& className) {
				QString writeString = QString("ClassAssertion(<%1> %2)\n").arg(className, individualName);
				mCurrentOutputFile->write(writeString.toUtf8());
			}



			void CWriteQueryFileOWL2FunctionalSerializer::writeNamedIndividualDeclaration(const QString& className) {
				QString writeString = QString("Declaration(NamedIndividual(<%1>))\n").arg(className);
				mCurrentOutputFile->write(writeString.toUtf8());
			}


			void CWriteQueryFileOWL2FunctionalSerializer::writeAnonymousIndividualDeclaration(const QString& className) {
				QString writeString = QString("Declaration(AnonymousIndividual(%1))\n").arg(className);
				mCurrentOutputFile->write(writeString.toUtf8());
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
