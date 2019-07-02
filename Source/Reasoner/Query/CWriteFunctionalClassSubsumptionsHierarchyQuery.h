/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_QUERY_CWRITEFUNCTIONALCLASSSUBSUMPTIONSHIERARCHYQUERY_H
#define KONCLUDE_REASONER_QUERY_CWRITEFUNCTIONALCLASSSUBSUMPTIONSHIERARCHYQUERY_H

// Libraries includes
#include <QString>
#include <QDir>
#include <QStringList>


// Namespace includes
#include "CQuery.h"
#include "CTaxonomyPremisingQuery.h"
#include "CQueryUnspecifiedStringError.h"
#include "CQueryInconsitentOntologyError.h"

#include "Config/CConfigDataReader.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CWriteFunctionalClassSubsumptionsHierarchyQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CWriteFunctionalClassSubsumptionsHierarchyQuery : public CTaxonomyPremisingQuery {
				// public methods
				public:
					//! Constructor
					CWriteFunctionalClassSubsumptionsHierarchyQuery(CConcreteOntology *premisingTaxonomyOntology, CConfigurationBase *configuration, const QString& outputFileString, const QString &superClassName = QString(""), const QString &subClassHierarchyQueryName = QString("UnnamedWriteClassHierarchyQuery"));

					virtual CQueryResult *constructResult(CTaxonomy *taxonomy);

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
					bool writeInconsistentHierarchyResult();
					bool writeSubClassHierarchyResult(CTaxonomy *taxonomy);
					void writeDeclarations(CHierarchyNode* node, QFile* outputFile);
					void writeEquivalences(CHierarchyNode* node, QFile* outputFile);
					void writeBottomEquivalences(CHierarchyNode* node, QFile* outputFile);
					void writeTopEquivalences(CHierarchyNode* node, QFile* outputFile);
					void writeDeclaration(const QString& className, QFile* outputFile);
					void writeSubClassRelations(CHierarchyNode* superNode, CHierarchyNode* subNode, QFile* outputFile);
					void writeSubClassRelation(const QString& subClassName, const QString& superClassName, QFile* outputFile);
					
					void writeEquivalenceRelations(CHierarchyNode* node, QFile* outputFile);
					void writeEquivalenceRelations(const QStringList& classNameList, QFile* outputFile);

					void writeOntologyStart(QFile* outputFile);
					void writeOntologyEnd(QFile* outputFile);

				// protected variables
				protected:
					QString mClassName;
					QString mQueryName;
					QString mQueryString;

					QString mOutputFileNameString;
					QString mBottomClassNameString;
					QString mTopClassNameString;


					bool mUseAbbreviatedIRIs;
					bool mWriteDeclarations;
					bool mWriteDirectEquivalences;
					bool mWriteSimpleBottomImplications;
					bool mWriteSimpleTopImplications;
					bool mWriteSimpleInconsistency;

					bool mQueryAnswered;
					bool mQueryConstructError;
					bool mTaxCalcError;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CWRITEFUNCTIONALCLASSSUBSUMPTIONSHIERARCHYQUERY_H
