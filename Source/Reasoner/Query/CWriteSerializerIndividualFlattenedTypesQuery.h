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

#ifndef KONCLUDE_REASONER_QUERY_CWRITESERIALIZERINDIVIDUALFLATTENEDTYPESQUERY_H
#define KONCLUDE_REASONER_QUERY_CWRITESERIALIZERINDIVIDUALFLATTENEDTYPESQUERY_H

// Libraries includes
#include <QString>
#include <QDir>
#include <QStringList>
#include <QXmlStreamWriter>


// Namespace includes
#include "CQuery.h"
#include "CWriteIndividualFlattenedTypesQuery.h"
#include "CWriteQuerySerializer.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CWriteSerializerIndividualFlattenedTypesQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CWriteSerializerIndividualFlattenedTypesQuery : public CWriteIndividualFlattenedTypesQuery {
				// public methods
				public:
					//! Constructor
					CWriteSerializerIndividualFlattenedTypesQuery(CConcreteOntology* ontology, CConfigurationBase *configuration, CWriteQuerySerializer* serializer, const QString& individualNameString = QString(""), const QString &subClassHierarchyQueryName = QString("UnnamedWriteIndividualTypesQuery"));


				// protected methods
				protected:
					
					virtual void writeIndividualDeclaration(const QString& individualName);
					virtual void writeClassDeclaration(const QString& className);

					virtual void writeIndividualType(const QString& individualName, const QString& className);
					virtual void writeSubClassRelation(const QString& subClassName, const QString& superClassName);

					virtual void writeOntologyStart();
					virtual void writeOntologyEnd();
					virtual void writeOntologyPrefix(const QString& prefixName, const QString& prefixIRI);


					virtual bool startWritingOutput();
					virtual bool endWritingOutput();

				// protected variables
				protected:
					CWriteQuerySerializer* mSerializer;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CWRITESERIALIZERINDIVIDUALFLATTENEDTYPESQUERY_H
