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

#ifndef KONCLUDE_REASONER_QUERY_CWRITEQUERYFILEOWL2FUNCTIONALSERIALIZER_H
#define KONCLUDE_REASONER_QUERY_CWRITEQUERYFILEOWL2FUNCTIONALSERIALIZER_H

// Libraries includes
#include <QString>
#include <QDir>
#include <QFile>


// Namespace includes
#include "QuerySettings.h"
#include "CWriteQueryFileSerializer.h"


// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CWriteQueryFileOWL2FunctionalSerializer
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CWriteQueryFileOWL2FunctionalSerializer : public CWriteQueryFileSerializer {
				// public methods
				public:
					//! Constructor
					CWriteQueryFileOWL2FunctionalSerializer(const QString& fileString);

					virtual void writeObjectPropertyDeclaration(const QString& propertyName);
					virtual void writeDataPropertyDeclaration(const QString& propertyName);
					virtual void writeSubObjectPropertyRelation(const QString& subPropertyName, const QString& superPropertyName);	
					virtual void writeSubDataPropertyRelation(const QString& subPropertyName, const QString& superPropertyName);
					virtual void writeObjectPropertyEquivalenceRelations(const QStringList& propertyNameList);
					virtual void writeDataPropertyEquivalenceRelations(const QStringList& propertyNameList);


					virtual void writeClassDeclaration(const QString& className);
					virtual void writeSubClassRelation(const QString& subClassName, const QString& superClassName);	
					virtual void writeClassEquivalenceRelations(const QStringList& classNameList);


					virtual void writeNamedIndividualDeclaration(const QString& individualName);
					virtual void writeAnonymousIndividualDeclaration(const QString& className);
					virtual void writeNamedIndividualType(const QString& individualName, const QString& className);
					virtual void writeAnonymousIndividualType(const QString& individualName, const QString& className);

					virtual void writeOntologyStart();
					virtual void writeOntologyEnd();
					virtual void writeOntologyPrefix(const QString& prefixName, const QString& prefixIRI);


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CWRITEQUERYFILEOWL2FUNCTIONALSERIALIZER_H
