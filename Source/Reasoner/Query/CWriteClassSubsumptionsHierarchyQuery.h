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

#ifndef KONCLUDE_REASONER_QUERY_CWRITECLASSSUBSUMPTIONSHIERARCHYQUERY_H
#define KONCLUDE_REASONER_QUERY_CWRITECLASSSUBSUMPTIONSHIERARCHYQUERY_H

// Libraries includes
#include <QString>
#include <QStringList>


// Namespace includes
#include "CQuery.h"
#include "CTaxonomyPremisingQuery.h"
#include "CQueryUnspecifiedStringError.h"
#include "CQueryInconsitentOntologyError.h"
#include "CWriteQuery.h"

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
			 *		\class		CWriteClassSubsumptionsHierarchyQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CWriteClassSubsumptionsHierarchyQuery : public CTaxonomyPremisingQuery, public CWriteQuery {
				// public methods
				public:
					//! Constructor
					CWriteClassSubsumptionsHierarchyQuery(CConcreteOntology *premisingTaxonomyOntology, CConfigurationBase *configuration, const QString& outputName, const QString &superClassName = QString(""), const QString &subClassHierarchyQueryName = QString("UnnamedWriteClassHierarchyQuery"));

					virtual CQueryResult *constructResult(CTaxonomy *taxonomy);

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
					bool writeInconsistentHierarchyResult();
					bool writeSubClassHierarchyResult(CTaxonomy *taxonomy);
					void writeDeclarations(CHierarchyNode* node);
					void writeEquivalences(CHierarchyNode* node);
					void writeBottomEquivalences(CHierarchyNode* node);
					void writeTopEquivalences(CHierarchyNode* node);

					virtual void writeSubClassRelations(CHierarchyNode* superNode, CHierarchyNode* subNode);
					virtual void writeEquivalenceRelations(CHierarchyNode* node);


					virtual void writeDeclaration(const QString& className) = 0;
					virtual void writeSubClassRelation(const QString& subClassName, const QString& superClassName) = 0;					
					virtual void writeEquivalenceRelations(const QStringList& classNameList) = 0;

					virtual void writeOntologyStart() = 0;
					virtual void writeOntologyEnd() = 0;
					virtual void writeOntologyPrefix(const QString& prefixName, const QString& prefixIRI) = 0;


					virtual bool startWritingOutput() = 0;
					virtual bool endWritingOutput() = 0;


				// protected variables
				protected:
					QString mClassName;
					QString mQueryName;
					QString mQueryString;

					QString mBottomClassNameString;
					QString mTopClassNameString;

					QString mOutputFileNameString;


					bool mUseAbbreviatedIRIs;
					bool mWriteDeclarations;

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

#endif // KONCLUDE_REASONER_QUERY_CWRITECLASSSUBSUMPTIONSHIERARCHYQUERY_H
