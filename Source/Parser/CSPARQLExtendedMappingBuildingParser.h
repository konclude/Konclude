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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_PARSER_CSPARQLEXTENDEDMAPPINGBUILDINGPARSER_H
#define KONCLUDE_PARSER_CSPARQLEXTENDEDMAPPINGBUILDINGPARSER_H

// Libraries includes


// Namespace includes
#include "CSPARQLSimpleBuildingParser.h"
#include "COntologyBuilder.h"
#include "CSPARQLBuildingParserTripleData.h"
#include "CSPARQLBuildingParserTermData.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Generator/CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Ontology;
	using namespace Reasoner::Generator;

	namespace Parser {

		/*! 
		 *
		 *		\class		CSPARQLExtendedMappingBuildingParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSPARQLExtendedMappingBuildingParser : public CSPARQLSimpleBuildingParser {
			// public methods
			public:
				//! Constructor
				CSPARQLExtendedMappingBuildingParser(COntologyBuilder* ontoBuilder, CConcreteOntology* ontology);

				//! Destructor
				virtual ~CSPARQLExtendedMappingBuildingParser();


			// protected methods
			protected:

				virtual QList<CAxiomExpression*> getTripleOWLAxioms(QHash<QString, CRDFHahsedSubjectData>* subjectTripleHash, bool anonymousIndividualsAsVariables = false);

				virtual QList<CAxiomExpression*> getTripleOWLAxioms(QList<CRDFStringTriple>* tripleStringList, bool anonymousIndividualsAsVariables = false, QSet<CExpressionVariable*>* varExpSet = nullptr);

				QList<CAxiomExpression *> handlingMappingRequiredTermTripleData(QList<CSPARQLBuildingParserTripleData *>* tripleDataList, bool anonymousIndividualsAsVariables, QSet<CExpressionVariable *>* varExpSet);



				QList<CSPARQLBuildingParserTripleData*>* createTermTripleData(QList<CRDFStringTriple>* tripleStringList);
				QList<CSPARQLBuildingParserTripleData*>* createTermTripleData(QHash<QString, CRDFHahsedSubjectData>* subjectTripleHash);



				CSPARQLBuildingParserTermData* getTermData(const QString& termString, QHash<QString, CSPARQLBuildingParserTermData*>* termStringDataHash);

				bool analyseTermTripleDataRequiresExtendedMapping(QList<CSPARQLBuildingParserTripleData*>* tripleList, bool anonymousIndividualsAsVariables);
				bool makeIndividualTerm(CSPARQLBuildingParserTermData* termData, bool anonymousIndividualsAsVariables);
				bool makeClassTerm(CSPARQLBuildingParserTermData* termData);
				bool makePropertyTerm(CSPARQLBuildingParserTermData* termData);
				bool makeObjectPropertyTerm(CSPARQLBuildingParserTermData* termData);
				bool makeDataPropertyTerm(CSPARQLBuildingParserTermData* termData);
				bool makeDataLiteralTerm(CSPARQLBuildingParserTermData* termData);

				bool makeTermTypedBasedOnTerm(CSPARQLBuildingParserTermData* termData, CSPARQLBuildingParserTermData* baseData, bool anonymousIndividualsAsVariables);


				CRedlandStoredTriplesData* getUpdatingTripleData();
				librdf_node* getRedlandRDFNode(CSPARQLBuildingParserTermData* termData, CRedlandStoredTriplesData* triplesData, QHash<CSPARQLBuildingParserTermData*, librdf_node*>* reuseExpHash, QHash<CBuildExpression*, QString>* termVariableExpressionNewEntityHash, cint64& newEntityNumber, QSet<CExpressionVariable *>* varExpSet);

			// protected variables
			protected:

				
			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLEXTENDEDMAPPINGBUILDINGPARSER_H

#endif // KONCLUDE_REDLAND_INTEGRATION
