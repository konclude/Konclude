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

#ifndef KONCLUDE_REASONER_QUERY_CWRITEINDIVIDUALFLATTENEDTYPESQUERY_H
#define KONCLUDE_REASONER_QUERY_CWRITEINDIVIDUALFLATTENEDTYPESQUERY_H

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
#include "CWriteQuery.h"

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
			 *		\class		CWriteIndividualFlattenedTypesQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CWriteIndividualFlattenedTypesQuery : public CRealizationPremisingQuery, public CConceptRealizationInstantiatedToConceptVisitor, public CWriteQuery {
				// public methods
				public:
					//! Constructor
					CWriteIndividualFlattenedTypesQuery(CConcreteOntology* ontology, CConfigurationBase *configuration, const QString& outputFileString, const QString& individualNameString = QString(""), const QString &subClassHierarchyQueryName = QString("UnnamedWriteIndividualTypesQuery"));

					virtual CQueryResult* constructResult(CRealization* realization);

					virtual WRITEQUERYTYPE getWriteQueryType();

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult *getQueryResult();

					virtual bool hasError();

					virtual CQuery* addQueryError(CQueryError* queryError);

				// protected methods
				protected:
					bool writeInconsistentIndividualTypes();
					bool writeIndividualTypesResult(CRealization* realization);
					
					virtual void writeIndividualDeclaration(const QString& individualName, bool anonymous) = 0;
					virtual void writeNamedIndividualDeclaration(const QString& individualName) = 0;
					virtual void writeAnonymousIndividualDeclaration(const QString& className) = 0;
					virtual void writeClassDeclaration(const QString& className) = 0;
					
					virtual void writeIndividualType(const QString& individualName, bool anonymous, const QString& className) = 0;
					virtual void writeNamedIndividualType(const QString& individualName, const QString& className) = 0;
					virtual void writeAnonymousIndividualType(const QString& individualName, const QString& className) = 0;
					virtual void writeSubClassRelation(const QString& subClassName, const QString& superClassName) = 0;

					virtual void writeOntologyStart() = 0;
					virtual void writeOntologyEnd() = 0;
					virtual void writeOntologyPrefix(const QString& prefixName, const QString& prefixIRI) = 0;



					virtual bool visitConcept(CConcept* concept, CConceptRealization* conRealization);


					virtual bool startWritingOutput() = 0;
					virtual bool endWritingOutput() = 0;


					bool visitIndividuals(function<bool(const CIndividualReference& indiRef)> visitFunc);


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
					bool mCurrentIndividualAnonymous;
					
					bool mUseAbbreviatedIRIs;
					bool mWriteDeclarations;
					bool mWriteOnlyDirectTypes;
					bool mWriteSubClassOfInconsistency;
					bool mWriteAnonymousIndividuals;

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

#endif // KONCLUDE_REASONER_QUERY_CWRITEINDIVIDUALFLATTENEDTYPESQUERY_H
