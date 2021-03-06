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

#ifndef KONCLUDE_REASONER_QUERY_CWRITEQUERYSERIALIZER_H
#define KONCLUDE_REASONER_QUERY_CWRITEQUERYSERIALIZER_H

// Libraries includes



// Namespace includes
#include "QuerySettings.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CWriteQuerySerializer
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CWriteQuerySerializer {
				// public methods
				public:
					//! Constructor
					CWriteQuerySerializer();

					virtual const QString getOutputName() = 0;

					virtual void writeObjectPropertyDeclaration(const QString& propertyName) = 0;
					virtual void writeDataPropertyDeclaration(const QString& propertyName) = 0;
					virtual void writeSubObjectPropertyRelation(const QString& subPropertyName, const QString& superPropertyName) = 0;	
					virtual void writeSubDataPropertyRelation(const QString& subPropertyName, const QString& superPropertyName) = 0;	
					virtual void writeObjectPropertyEquivalenceRelations(const QStringList& propertyNameList) = 0;
					virtual void writeDataPropertyEquivalenceRelations(const QStringList& propertyNameList) = 0;


					virtual void writeClassDeclaration(const QString& className) = 0;
					virtual void writeSubClassRelation(const QString& subClassName, const QString& superClassName) = 0;	
					virtual void writeClassEquivalenceRelations(const QStringList& classNameList) = 0;


					virtual void writeNamedIndividualDeclaration(const QString& individualName) = 0;
					virtual void writeAnonymousIndividualDeclaration(const QString& individualName) = 0;
					virtual void writeNamedIndividualType(const QString& individualName, const QString& className) = 0;
					virtual void writeAnonymousIndividualType(const QString& individualName, const QString& className) = 0;

					virtual void writeOntologyPrefix(const QString& prefixName, const QString& prefixIRI) = 0;
					virtual void writeOntologyStart() = 0;
					virtual void writeOntologyEnd() = 0;


					virtual bool startWritingOutput() = 0;
					virtual bool endWritingOutput() = 0;


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

#endif // KONCLUDE_REASONER_QUERY_CWRITEQUERYSERIALIZER_H
