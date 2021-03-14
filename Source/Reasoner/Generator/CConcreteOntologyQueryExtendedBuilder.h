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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYQUERYEXTENDEDBUILDER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYQUERYEXTENDEDBUILDER_H

// Libraries includes


// Namespace includes
#include "CQueryBuilderGenerator.h"
#include "CConcreteOntologyQueryBasicBuilder.h"
#include "CSatisfiableCalculationJobGenerator.h"
#include "CExpressionSplitter.h"


// Other includes
#include "Reasoner/Query/CIsClassSatisfiableQuery.h"
#include "Reasoner/Query/CAreClassesEquivalentQuery.h"
#include "Reasoner/Query/CIsClassSubsumedByQuery.h"
#include "Reasoner/Query/CAreClassesDisjointQuery.h"
#include "Reasoner/Query/CIsClassSubsumedByQuery.h"
#include "Reasoner/Query/CIsInstanceOfQuery.h"
#include "Reasoner/Query/CQueryStatisticsCollectionStrings.h"
#include "Reasoner/Query/CFlattenedTypesQuery.h"
#include "Reasoner/Query/CFlattenedInstancesQuery.h"
#include "Reasoner/Query/CAreAxiomsEntailedQuery.h"

#include "Reasoner/Query/CComplexSatisfiabilityAnsweringQuery.h"
#include "Reasoner/Query/CComplexSuperClassesAnsweringQuery.h"
#include "Reasoner/Query/CComplexSubClassesAnsweringQuery.h"
#include "Reasoner/Query/CComplexEquivalentClassesAnsweringQuery.h"
#include "Reasoner/Query/CComplexIndividualInstancesAnsweringQuery.h"
#include "Reasoner/Query/CComplexAssertionsIndividualVariablesAnsweringQuery.h"
#include "Reasoner/Query/CComplexAxiomsIndividualMixedVariablesAnsweringQuery.h"
#include "Reasoner/Query/CComplexAxiomsClassVariablesAnsweringQuery.h"
#include "Reasoner/Query/CVariableBindingFilteringLiteralComparison.h"
#include "Reasoner/Query/CComplexAxiomsObjectPropertyVariablesAnsweringQuery.h"
#include "Reasoner/Query/CComplexAxiomsDataPropertyVariablesAnsweringQuery.h"

#include "Reasoner/Ontology/CConceptTextFormater.h"
#include "Reasoner/Ontology/CIRIName.h"

#include "Parser/COntologyBuilder.h"
#include "Parser/CSPARQLQueryBuilder.h"

#include "Config/CConfigurationBase.h"
#include "Config/CConfigDataReader.h"

#include "Parser/Expressions/COrderingVariableExpression.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Parser;
	using namespace Parser::Expression;
	using namespace Config;

	namespace Reasoner {

		using namespace Query;
		using namespace Ontology;

		namespace Generator {

			/*! 
			 *
			 *		\class		CConcreteOntologyQueryExtendedBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyQueryExtendedBuilder : public CConcreteOntologyQueryBasicBuilder, public CSPARQLQueryBuilder {
				// public methods
				public:
					//! Constructor
					CConcreteOntologyQueryExtendedBuilder(CConcreteOntology* baseOntology, CConcreteOntology* ontology, CConfigurationBase *configuration, COntologyBuilder* ontologyBuilder);

					//! Destructor
					virtual ~CConcreteOntologyQueryExtendedBuilder();

					virtual QList<CQuery*> generateQuerys();





					virtual CQueryIsEntailedExpression* getIsEntailedQuery(const CEXPRESSIONLIST<CAxiomExpression*>& axiomExpressions, const QString& queryName);


					virtual CQueryAreClassesEquivalentExpression* getAreClassesEquivalenceQuery(const CEXPRESSIONLIST<CClassTermExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryAreClassesDisjointExpression* getAreClassesDisjointQuery(const CEXPRESSIONLIST<CClassTermExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryIsClassSubsumedByExpression* getIsClassSubsumedByQuery(CClassTermExpression* subsumerClassExpression, CClassTermExpression* subsumedClassExpression, const QString& queryName);


					virtual bool requiresPreprocessedOntology();


					virtual CQuerySPARQLSelectBasicGraphPatternExpression* getSPARQLBasicGraphPatternClassSelectQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QList<CExpressionVariable*>& disVarList, cint64 limit, cint64 offset, const QString& queryName = "");
					virtual CQuerySPARQLSelectBasicGraphPatternExpression* getSPARQLBasicGraphPatternObjectPropertiesSelectQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QList<CExpressionVariable*>& disVarList, cint64 limit, cint64 offset, const QString& queryName = "");
					virtual CQuerySPARQLSelectBasicGraphPatternExpression* getSPARQLBasicGraphPatternDataPropertiesSelectQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QList<CExpressionVariable*>& disVarList, cint64 limit, cint64 offset, const QString& queryName = "");

					virtual CQuerySPARQLAskBasicGraphPatternExpression* getSPARQLBasicGraphPatternClassAskQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QString& queryName = "");
					virtual CQuerySPARQLAskBasicGraphPatternExpression* getSPARQLBasicGraphPatternObjectPropertiesAskQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QString& queryName = "");
					virtual CQuerySPARQLAskBasicGraphPatternExpression* getSPARQLBasicGraphPatternDataPropertiesAskQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QString& queryName = "");


					virtual CQuerySPARQLSelectBasicGraphPatternExpression* getSPARQLBasicGraphPatternIndividualSelectQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QList<CExpressionVariable*>& disVarList, const QList<CExpressionVariable*>& ignoreCardVarList, bool distinctModifier, cint64 limit, cint64 offset, const QString& queryName = "");
					virtual CQuerySPARQLSelectBasicGraphPatternExpression* getSPARQLBasicGraphPatternIndividualSelectQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QList<CExpressionVariable*>& disVarList, const CEXPRESSIONLIST<COrderingTermExpression*>& orderingList, const CEXPRESSIONLIST<CFilteringTermExpression*>& filteringList, bool distinctModifier, cint64 limit, cint64 offset, const QString& queryName = "");
					virtual CQuerySPARQLSelectBasicGraphPatternExpression* getSPARQLBasicGraphPatternIndividualSelectQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QList<CExpressionVariable*>& disVarList, const CEXPRESSIONLIST<COrderingTermExpression*>& orderingList, const CEXPRESSIONLIST<CFilteringTermExpression*>& filteringList, bool distinctModifier, const QString& queryName = "");
					virtual CQuerySPARQLAskBasicGraphPatternExpression* getSPARQLBasicGraphPatternIndividualAskQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const CEXPRESSIONLIST<CFilteringTermExpression*>& filteringList, const QString& queryName = "");

					virtual CQuerySPARQLSelectBasicGraphPatternExpression* getSPARQLBasicGraphPatternIndividualMixedSelectQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QList<CExpressionVariable*>& disVarList, const QList<CExpressionVariable*>& ignoreCardVarList, bool distinctModifier, cint64 limit, cint64 offset, const QString& queryName = "");


					virtual COrderingVariableExpression* getOrderingVariableExpression(CExpressionVariable* variable, bool ascOrdering);
					virtual CFilteringComparisonDataLiteralExpression* getFilteringComparisonDataLiteralExpression(CDataLiteralVariableExpression* variable, CDataLiteralExpression* dataLiteralExp, CFilteringComparisonDataLiteralExpression::COMPARING_TYPE comparingType);


				// protected methods
				protected:

					CVariableBindingFiltering* createFilterFromExpression(CFilteringTermExpression* filteringTermExpression);

					void createAskQueryFromBGPExpression(CQuerySPARQLAskBasicGraphPatternExpression* sparqlBgpExp, const QString& typeString, function<CComplexVariablesAnsweringQuery*(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName)> queryFunc, bool confBuildQueryStats, QList<CQuery *> &queryList);
					void createSelectQueryFromBGPExpression(CQuerySPARQLSelectBasicGraphPatternExpression* sparqlBgpExp, const QString& typeString, function<CComplexVariablesAnsweringQuery*(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName)> queryFunc, bool confBuildQueryStats, QList<CQuery *> &queryList);

				// protected variables
				protected:		
					CConcreteOntology* mBaseOntology;
					COntologyBuilder* mOntologyBuilder;

					CEXPRESSIONLIST<CBuildExpression*> container;


					CEXPRESSIONLIST<CQueryIsEntailedExpression*> isEntailedExpList;
					CEXPRESSIONLIST<CQuerySPARQLSelectBasicGraphPatternExpression*> mSparqlClassSelectExpList;
					CEXPRESSIONLIST<CQuerySPARQLSelectBasicGraphPatternExpression*> mSparqlObjPropSelectExpList;
					CEXPRESSIONLIST<CQuerySPARQLSelectBasicGraphPatternExpression*> mSparqlDataPropSelectExpList;
					CEXPRESSIONLIST<CQuerySPARQLSelectBasicGraphPatternExpression*> mSparqlIndividualSelectExpList;
					CEXPRESSIONLIST<CQuerySPARQLSelectBasicGraphPatternExpression*> mSparqlIndividualMixedSelectExpList;

					CEXPRESSIONLIST<CQuerySPARQLAskBasicGraphPatternExpression*> mSparqlIndividualAskExpList;
					CEXPRESSIONLIST<CQuerySPARQLAskBasicGraphPatternExpression*> mSparqlClassAskExpList;
					CEXPRESSIONLIST<CQuerySPARQLAskBasicGraphPatternExpression*> mSparqlObjectPropAskExpList;
					CEXPRESSIONLIST<CQuerySPARQLAskBasicGraphPatternExpression*> mSparqlDataPropAskExpList;

					CEXPRESSIONLIST<CBuildExpression*> mExpContainer;



				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYQUERYEXTENDEDBUILDER_H
