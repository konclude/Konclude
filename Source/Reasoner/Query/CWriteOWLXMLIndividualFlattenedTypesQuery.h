/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_QUERY_CWRITEOWLXMLINDIVIDUALFLATTENEDTYPESQUERY_H
#define KONCLUDE_REASONER_QUERY_CWRITEOWLXMLINDIVIDUALFLATTENEDTYPESQUERY_H

// Libraries includes
#include <QString>
#include <QDir>
#include <QStringList>
#include <QXmlStreamWriter>


// Namespace includes
#include "CQuery.h"
#include "CRealizationPremisingQuery.h"
#include "CQueryUnspecifiedStringError.h"
#include "CQueryInconsitentOntologyError.h"

#include "Reasoner/Realization/CConceptRealizationInstantiatedToConceptVisitor.h"


#include "Config/CConfigDataReader.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Realization;

		namespace Query {

			/*! 
			 *
			 *		\class		CWriteOWLXMLIndividualFlattenedTypesQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CWriteOWLXMLIndividualFlattenedTypesQuery : public CRealizationPremisingQuery, public CConceptRealizationInstantiatedToConceptVisitor {
				// public methods
				public:
					//! Constructor
					CWriteOWLXMLIndividualFlattenedTypesQuery(CConcreteOntology* ontology, CConfigurationBase *configuration, const QString& outputFileString, const QString& individualNameString = QString(""), const QString &subClassHierarchyQueryName = QString("UnnamedWriteIndividualTypesQuery"));

					virtual CQueryResult* constructResult(CRealization* realization);

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult *getQueryResult();

					virtual bool hasError();

					virtual CQuery* addQueryError(CQueryError* queryError);

				// protected methods
				protected:
					void forcedPathCreated(const QString& filePath);

					bool writeInconsistentIndividualTypes();
					bool writeIndividualTypesResult(CRealization* realization);
					
					void writeIndividualDeclaration(const QString& individualName, QXmlStreamWriter* outputStreamWriter);
					void writeClassDeclaration(const QString& className, QXmlStreamWriter* outputStreamWriter);
					
					void writeIndividualType(const QString& individualName, const QString& className, QXmlStreamWriter* outputStreamWriter);
					
					void writeOntologyStart(QXmlStreamWriter* outputStreamWriter);
					void writeOntologyEnd(QXmlStreamWriter* outputStreamWriter);
					void writeOntologyPrefix(QXmlStreamWriter* outputStreamWriter, const QString& prefixName, const QString& prefixIRI);



					virtual bool visitConcept(CConcept* concept, CConceptRealization* conRealization);

				// protected variables
				protected:
					QString mClassName;
					QString mQueryName;
					QString mQueryString;

					QString mIndividualNameString;


					QString mOutputFileNameString;
					QString mBottomClassNameString;
					QString mTopClassNameString;

					QSet<CConcept*> mDeclaratedConceptSet;
					QString mCurrentIndividualName;
					QXmlStreamWriter* mCurrentOutputStreamWriter;


					bool mUseAbbreviatedIRIs;
					bool mWriteDeclarations;

					bool mQueryAnswered;
					bool mQueryConstructError;
					bool mRealizationCalcError;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CWRITEOWLXMLINDIVIDUALFLATTENEDTYPESQUERY_H
