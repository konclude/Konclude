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

#ifndef KONCLUDE_REASONER_QUERY_CWRITEPROPERTYSUBSUMPTIONSHIERARCHYQUERY_H
#define KONCLUDE_REASONER_QUERY_CWRITEPROPERTYSUBSUMPTIONSHIERARCHYQUERY_H

// Libraries includes
#include <QString>
#include <QDir>
#include <QStringList>
#include <QXmlStreamWriter>


// Namespace includes
#include "CQuery.h"
#include "CQueryUnspecifiedStringError.h"
#include "CQueryInconsitentOntologyError.h"
#include "CClassificationPremisingQuery.h"
#include "CWriteQuery.h"



// Other includes
#include "Reasoner/Taxonomy/CRolePropertiesHierarchy.h"

#include "Config/CConfigDataReader.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Taxonomy;

		namespace Query {

			/*! 
			 *
			 *		\class		CWritePropertySubsumptionsHierarchyQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CWritePropertySubsumptionsHierarchyQuery : public CClassificationPremisingQuery, public CWriteQuery {
				// public methods
				public:
					//! Constructor
					CWritePropertySubsumptionsHierarchyQuery(CConcreteOntology *premisingClassificationOntology, CConfigurationBase *configuration, const QString& outputFileString, bool dataProperties, const QString &superPropertyName = QString(""), const QString &subPropertyHierarchyQueryName = QString("UnnamedWritePropertyHierarchyQuery"));

					virtual CQueryResult *constructResult(CClassification* classification);

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult *getQueryResult();

					virtual bool hasError();

					virtual CQuery* addQueryError(CQueryError* queryError);

					virtual WRITEQUERYTYPE getWriteQueryType();

				// protected methods
				protected:
					bool writeInconsistentHierarchyResult();
					bool writeSubPropertyHierarchyResult(CRolePropertiesHierarchy *hierarchy);
					void writeDeclarations(CRolePropertiesHierarchyNode* node);
					void writeEquivalences(CRolePropertiesHierarchyNode* node);
					void writeBottomEquivalences(CRolePropertiesHierarchyNode* node);
					void writeTopEquivalences(CRolePropertiesHierarchyNode* node);

					virtual void writeSubPropertyRelations(CRolePropertiesHierarchyNode* superNode, CRolePropertiesHierarchyNode* subNode);
					virtual void writeEquivalenceRelations(CRolePropertiesHierarchyNode* node);


					virtual void writeDeclaration(const QString& className) = 0;
					virtual void writeSubPropertyRelation(const QString& subPropertyName, const QString& superPropertyName) = 0;					
					virtual void writeEquivalenceRelations(const QStringList& propertyNameList) = 0;

					virtual void writeOntologyStart() = 0;
					virtual void writeOntologyEnd() = 0;
					virtual void writeOntologyPrefix(const QString& prefixName, const QString& prefixIRI) = 0;


					virtual bool startWritingOutput() = 0;
					virtual bool endWritingOutput() = 0;


				// protected variables
				protected:
					QString mPropertyName;
					QString mQueryName;
					QString mQueryString;

					QString mOutputFileNameString;
					QString mBottomPropertyNameString;
					QString mTopPropertyNameString;


					bool mUseAbbreviatedIRIs;
					bool mWriteDeclarations;

					bool mQueryAnswered;
					bool mQueryConstructError;
					bool mTaxCalcError;

					bool mDataProperties;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CWRITEPROPERTYSUBSUMPTIONSHIERARCHYQUERY_H
