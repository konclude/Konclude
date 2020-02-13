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


#ifndef KONCLUDE_PARSER_CSPARQLREDLANDRASQALREASONINGBGPQUERYPARSER_H
#define KONCLUDE_PARSER_CSPARQLREDLANDRASQALREASONINGBGPQUERYPARSER_H

// Libraries includes
#include <Redland.h>
#include <QRegularExpression>


// Namespace includes
#include "CQueryParser.h"
#include "COntologyParser.h"
#include "CSPARQLQueryBuilder.h"
#include "COntologyBuilder.h"
#include "CSPARQLSimpleBuildingParser.h"
#include "CSPARQLQueryParser.h"


// Other includes
#include "Parser/Expressions/CQuerySPARQLBasicGraphPatternExpression.h"
#include "Parser/Expressions/COrderingVariableExpression.h"

#include "Config/CConfigDataReader.h"

#include "Parser/Expressions/CFilteringTermExpression.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Config;

	namespace Parser {

		using namespace Expression;


		/*! 
		 *
		 *		\class		CSPARQLRedlandRasqalReasoningBGPQueryParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSPARQLRedlandRasqalReasoningBGPQueryParser : public CSPARQLSimpleBuildingParser {
			// public methods
			public:
				//! Constructor
				CSPARQLRedlandRasqalReasoningBGPQueryParser(CSPARQLQueryBuilder *queryBuilder, COntologyBuilder* ontoBuilder, CConcreteOntology* ontology, CConfigurationBase* config);

				//! Destructor
				virtual ~CSPARQLRedlandRasqalReasoningBGPQueryParser();


				class CRedlandRasqalBGPSubQueryData {
				public:
					rasqal_graph_pattern* bgp = nullptr;
					int tripleStartPos = 0;
					int tripleEndPos = 0;
					bool completelyHandledResultsTransfer = false;
					cint64 subQueryId = 0;
					QList<cint64> dependentSubQueryIds;
				};

				QHash<QString, CRedlandRasqalBGPSubQueryData*> parseBGPReasoningSubQueries(librdf_world* world, librdf_query* redlandQuery, rasqal_query* rasqalQuery, const QString& queryText, const QString& knowledgeBase, bool defaultKnowledgeBase, cint64& nextSubQueryId);

			// protected variables
			protected:

				class CBGPSubQueryGenerationData {
				public:
					CRedlandRasqalBGPSubQueryData* bgpData = nullptr;
					cint64 subQueryId = 0;

					QList<CRDFStringTriple*> connectedTripleStringList;
					QList<CRDFStringTriple> convertingStringTriplesList;
					QSet<cint64> usedtriplePosSet;
					QList<CRDFStringTriple> additionalIntegratingStringTriplesList;


					QSet<QString> connectedVariableSet;
					QStringList subQueryVarStringList;
					QSet<QString> distinguishedVariableSet;
					QStringList distinguishedVariableStringList;

					QSet<QString> additionalIntegratedVariableSet;

				};


			// protected methods
			protected:



				bool identifySubBGPsDistinctComputable(rasqal_graph_pattern * basicGrapPattern, QSet<rasqal_graph_pattern*>& distinctSubBGPsSet);

				bool identifyHandlingKnowledgeBaseRelevantBGPs(rasqal_graph_pattern* grapPattern, bool rootPattern, QSet<rasqal_graph_pattern*>& kbRelevantBGPsSet, const QString& kbName, bool defaultKB);

				bool checkSingleBGPResultsDirectUsable(bool singleBGPResultsDirectlyUsable, rasqal_graph_pattern * basicGrapPattern, rasqal_query* rasqalQuery);

				bool identifyBGPBasedSubQueryTriples(rasqal_graph_pattern* grapPattern);
				bool identifyBGPBasedSubQueries(rasqal_graph_pattern* grapPattern);

				CBGPSubQueryGenerationData* createSubQueryGenerationData(rasqal_query *query, CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData* bgpData);

				bool generateBGPBasedSubQueries();

				bool integrateFollowingSubQueryRestrictions(CBGPSubQueryGenerationData* subQueryBGPData, CBGPSubQueryGenerationData* followingSubQueryBGPData);

			// protected variables
			protected:
				CSPARQLQueryBuilder* mQueryBuilder;
				QString mQueryText;
				librdf_query* mRedlandQuery;
				rasqal_query* mRasqalQuery;
				rasqal_world* mRasqalWorld;
				librdf_world* mRedlandWorld;
				raptor_world* mRaptorWorld;
				cint64 mNextSubQueryId = 0;

				QSet<rasqal_graph_pattern*> mKnowledgeBaseRelevantBGPsSet;
				bool mQueryContainsSelectAll = false;
				QSet<rasqal_graph_pattern*> mDistinctSubBGPsSet;
				bool mSingleBGPResultsDirectlyUsable = false;

				QList<CQuerySPARQLBasicGraphPatternExpression*> mSPARQLBGPQueryList;

				CConfigurationBase* mConfig;
				bool mConfWriteQueryStructure = false;
				bool mConfForceDistinctSubQueries = false;
				bool mConfReduceBGPSubQueryAnswerVariables = false;
				bool mConfPropagateBGPSubQueryDistinct = false;
				bool mConfMinimizeBGPTriples = false;
				bool mConfAllowResultStreamingFromSingleSubQuery = false;
				bool mConfSubQueriesDependencyIdentification = false;
				bool mConfSubQueriesFollowingRestrictionsIntegration = false;
				bool mConfWriteSubQueryTriples = false;

				QHash<cint64, CBGPSubQueryGenerationData*> mIdSubQueryIdDataHash;
				QHash<rasqal_graph_pattern*, CBGPSubQueryGenerationData*> mBGPSubQueryIdDataHash;
				QHash<QString, CRedlandRasqalBGPSubQueryData*> mBGPSubQueryNameHash;

			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLREDLANDRASQALREASONINGBGPQUERYPARSER_H

#endif // KONCLUDE_REDLAND_INTEGRATION
