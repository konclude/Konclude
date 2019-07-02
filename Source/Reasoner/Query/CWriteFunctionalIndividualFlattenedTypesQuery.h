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

#ifndef KONCLUDE_REASONER_QUERY_CWRITEFUNCTIONALINDIVIDUALFLATTENEDTYPESQUERY_H
#define KONCLUDE_REASONER_QUERY_CWRITEFUNCTIONALINDIVIDUALFLATTENEDTYPESQUERY_H

// Libraries includes
#include <QString>
#include <QDir>
#include <QStringList>


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
			 *		\class		CWriteFunctionalIndividualFlattenedTypesQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CWriteFunctionalIndividualFlattenedTypesQuery : public CRealizationPremisingQuery, public CConceptRealizationInstantiatedToConceptVisitor {
				// public methods
				public:
					//! Constructor
					CWriteFunctionalIndividualFlattenedTypesQuery(CConcreteOntology* ontology, CConfigurationBase *configuration, const QString& outputFileString, const QString& individualNameString = QString(""), const QString &subClassHierarchyQueryName = QString("UnnamedWriteIndividualTypesQuery"));

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
					
					void writeIndividualDeclaration(const QString& individualName, QFile* outputFile);
					void writeClassDeclaration(const QString& className, QFile* outputFile);
					
					void writeIndividualType(const QString& individualName, const QString& className, QFile* outputFile);
					void writeSubClassRelation(const QString& subClassName, const QString& superClassName, QFile* outputFile);

					void writeOntologyStart(QFile* outputFile);
					void writeOntologyEnd(QFile* outputFile);



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
					QFile* mCurrentOutputFile;


					bool mUseAbbreviatedIRIs;
					bool mWriteDeclarations;
					bool mWriteOnlyDirectTypes;
					bool mWriteSubClassOfInconsistency;

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

#endif // KONCLUDE_REASONER_QUERY_CWRITEFUNCTIONALINDIVIDUALFLATTENEDTYPESQUERY_H
