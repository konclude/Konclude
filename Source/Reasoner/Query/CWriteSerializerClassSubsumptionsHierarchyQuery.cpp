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

#include "CWriteSerializerClassSubsumptionsHierarchyQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWriteSerializerClassSubsumptionsHierarchyQuery::CWriteSerializerClassSubsumptionsHierarchyQuery(CConcreteOntology *premisingTaxonomyOntology, CConfigurationBase *configuration, CWriteQuerySerializer* serializer, const QString &superClassName, const QString &subClassHierarchyQueryName)
					: CWriteClassSubsumptionsHierarchyQuery(premisingTaxonomyOntology,configuration,serializer->getOutputName(),superClassName,subClassHierarchyQueryName) {

				mSerializer = serializer;
			}



			bool CWriteSerializerClassSubsumptionsHierarchyQuery::startWritingOutput() {
				return mSerializer->startWritingOutput();
			}



			bool CWriteSerializerClassSubsumptionsHierarchyQuery::endWritingOutput() {
				return mSerializer->endWritingOutput();
			}


			void CWriteSerializerClassSubsumptionsHierarchyQuery::writeEquivalenceRelations(const QStringList& classNameList) {
				mSerializer->writeClassEquivalenceRelations(classNameList);
			}




			void CWriteSerializerClassSubsumptionsHierarchyQuery::writeDeclaration(const QString& className) {
				mSerializer->writeClassDeclaration(className);
			}


			void CWriteSerializerClassSubsumptionsHierarchyQuery::writeOntologyPrefix(const QString& prefixName, const QString& prefixIRI) {
				mSerializer->writeOntologyPrefix(prefixName,prefixIRI);
			}


			void CWriteSerializerClassSubsumptionsHierarchyQuery::writeOntologyStart() {
				mSerializer->writeOntologyStart();
			}


			void CWriteSerializerClassSubsumptionsHierarchyQuery::writeOntologyEnd() {
				mSerializer->writeOntologyEnd();
			}



			void CWriteSerializerClassSubsumptionsHierarchyQuery::writeSubClassRelation(const QString& subClassName, const QString& superClassName) {
				mSerializer->writeSubClassRelation(subClassName,superClassName);
			}




		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
