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

#include "CWriteSerializerPropertySubsumptionsHierarchyQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWriteSerializerPropertySubsumptionsHierarchyQuery::CWriteSerializerPropertySubsumptionsHierarchyQuery(CConcreteOntology *premisingClassificationOntology, CConfigurationBase *configuration, CWriteQuerySerializer* serializer, bool dataProperties, const QString &superPropertyName, const QString &subPropertyHierarchyQueryName)
					: CWritePropertySubsumptionsHierarchyQuery(premisingClassificationOntology,configuration,serializer->getOutputName(),dataProperties,superPropertyName,subPropertyHierarchyQueryName) {

				mSerializer = serializer;
			}



			bool CWriteSerializerPropertySubsumptionsHierarchyQuery::startWritingOutput() {
				return mSerializer->startWritingOutput();
			}



			bool CWriteSerializerPropertySubsumptionsHierarchyQuery::endWritingOutput() {
				return mSerializer->endWritingOutput();
			}


			void CWriteSerializerPropertySubsumptionsHierarchyQuery::writeEquivalenceRelations(const QStringList& propertyNameList) {	
				if (mDataProperties) {
					mSerializer->writeDataPropertyEquivalenceRelations(propertyNameList);
				} else {
					mSerializer->writeObjectPropertyEquivalenceRelations(propertyNameList);
				}
			}




			void CWriteSerializerPropertySubsumptionsHierarchyQuery::writeDeclaration(const QString& propertyName) {
				if (mDataProperties) {
					mSerializer->writeDataPropertyDeclaration(propertyName);
				} else {
					mSerializer->writeObjectPropertyDeclaration(propertyName);
				}
			}


			void CWriteSerializerPropertySubsumptionsHierarchyQuery::writeOntologyPrefix(const QString& prefixName, const QString& prefixIRI) {
				mSerializer->writeOntologyPrefix(prefixName,prefixIRI);
			}


			void CWriteSerializerPropertySubsumptionsHierarchyQuery::writeOntologyStart() {
				mSerializer->writeOntologyStart();
			}


			void CWriteSerializerPropertySubsumptionsHierarchyQuery::writeOntologyEnd() {
				mSerializer->writeOntologyEnd();
			}



			void CWriteSerializerPropertySubsumptionsHierarchyQuery::writeSubPropertyRelation(const QString& subPropertyName, const QString& superPropertyName) {
				if (mDataProperties) {
					mSerializer->writeSubDataPropertyRelation(subPropertyName,superPropertyName);
				} else {
					mSerializer->writeSubObjectPropertyRelation(subPropertyName,superPropertyName);
				}
			}




		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
