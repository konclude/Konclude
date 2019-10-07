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

#include "CWriteSerializerIndividualFlattenedTypesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWriteSerializerIndividualFlattenedTypesQuery::CWriteSerializerIndividualFlattenedTypesQuery(CConcreteOntology* ontology, CConfigurationBase *configuration, CWriteQuerySerializer* serializer, const QString& individualNameString, const QString &subClassHierarchyQueryName)
					: CWriteIndividualFlattenedTypesQuery(ontology,configuration,serializer->getOutputName(),individualNameString,subClassHierarchyQueryName) {

				mSerializer = serializer;
			}




			bool CWriteSerializerIndividualFlattenedTypesQuery::startWritingOutput() {
				return mSerializer->startWritingOutput();
			}



			bool CWriteSerializerIndividualFlattenedTypesQuery::endWritingOutput() {
				return mSerializer->endWritingOutput();
			}


			void CWriteSerializerIndividualFlattenedTypesQuery::writeSubClassRelation(const QString& subClassName, const QString& superClassName) {
				mSerializer->writeSubClassRelation(subClassName,superClassName);
			}



			void CWriteSerializerIndividualFlattenedTypesQuery::writeIndividualDeclaration(const QString& individualName, bool anonymous) {
				if (anonymous) {
					mSerializer->writeAnonymousIndividualDeclaration(individualName);
				} else {
					mSerializer->writeNamedIndividualDeclaration(individualName);
				}
			}

			void CWriteSerializerIndividualFlattenedTypesQuery::writeNamedIndividualDeclaration(const QString& className) {
				mSerializer->writeNamedIndividualDeclaration(className);
			}

			void CWriteSerializerIndividualFlattenedTypesQuery::writeAnonymousIndividualDeclaration(const QString& className) {
				mSerializer->writeAnonymousIndividualDeclaration(className);
			}

			void CWriteSerializerIndividualFlattenedTypesQuery::writeClassDeclaration(const QString& className) {
				mSerializer->writeClassDeclaration(className);
			}


			void CWriteSerializerIndividualFlattenedTypesQuery::writeOntologyPrefix(const QString& prefixName, const QString& prefixIRI) {
				mSerializer->writeOntologyPrefix(prefixName,prefixIRI);
			}


			void CWriteSerializerIndividualFlattenedTypesQuery::writeOntologyStart() {
				mSerializer->writeOntologyStart();
			}


			void CWriteSerializerIndividualFlattenedTypesQuery::writeOntologyEnd() {
				mSerializer->writeOntologyEnd();
			}


			void CWriteSerializerIndividualFlattenedTypesQuery::writeIndividualType(const QString& individualName, bool anonymous, const QString& className) {
				if (anonymous) {
					mSerializer->writeAnonymousIndividualType(individualName, className);
				} else {
					mSerializer->writeNamedIndividualType(individualName, className);
				}
			}

			void CWriteSerializerIndividualFlattenedTypesQuery::writeNamedIndividualType(const QString& individualName, const QString& className) {
				mSerializer->writeNamedIndividualType(individualName, className);
			}

			void CWriteSerializerIndividualFlattenedTypesQuery::writeAnonymousIndividualType(const QString& individualName, const QString& className) {
				mSerializer->writeAnonymousIndividualType(individualName, className);
			}



		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
