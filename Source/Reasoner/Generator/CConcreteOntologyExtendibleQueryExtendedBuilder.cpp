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

#include "CConcreteOntologyExtendibleQueryExtendedBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			CConcreteOntologyExtendibleQueryExtendedBuilder::CConcreteOntologyExtendibleQueryExtendedBuilder(CConcreteOntology* baseOntology, CConcreteOntology* ontology, CConfigurationBase *configuration, COntologyBuilder* ontologyBuilder)
					: CConcreteOntologyQueryExtendedBuilder(baseOntology, ontology, configuration, ontologyBuilder) {

			}



			CConcreteOntologyExtendibleQueryExtendedBuilder::~CConcreteOntologyExtendibleQueryExtendedBuilder() {
			}


			QList<CQuery*> CConcreteOntologyExtendibleQueryExtendedBuilder::generateQuerys() {
				QList<CQuery*> queryList;


				bool confBuildQueryStats = CConfigDataReader::readConfigBoolean(config,"Konclude.Query.Statistics.CollectStatistics",false);


				foreach(CQuerySPARQLSelectBasicGraphPatternExpression* sparqlBgpExp, mSparqlSelectExpList) {
					QString queryName = sparqlBgpExp->getName();
					const QList<CAxiomExpression*>& axiomExps = *sparqlBgpExp->getAxiomExpressionList();
					const QList<CExpressionVariable*>& disVarExps = *sparqlBgpExp->getOrderedVariableList();
					const QList<CExpressionVariable*>& ignoreCardVarExps = *sparqlBgpExp->getIgnoreCardinalityVariableList();
					const QList<COrderingTermExpression*>& orderingExps = *sparqlBgpExp->getOrderedOrderingTermExpressionList();
					const QList<CFilteringTermExpression*>& filteringExps = *sparqlBgpExp->getFilteringExpressionList();

					CComplexAssertionsIndividualVariablesAnsweringQuery *query = new CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery(mBaseOntology, mOntology, axiomExps, disVarExps, config, queryName);
					for (COrderingTermExpression* orderingExp : orderingExps) {
						COrderingVariableExpression* orderingVarExp = dynamic_cast<COrderingVariableExpression*>(orderingExp);
						if (orderingVarExp) {
							CVariableBindingOrdering::ORDERING_TYPE type = CVariableBindingOrdering::ASC;
							if (orderingVarExp->getOrderingType() == CVariableBindingOrdering::DESC) {
								type = CVariableBindingOrdering::DESC;
							}
							CVariableBindingOrdering* ordering = new CVariableBindingOrdering(orderingVarExp->getOrderingVariable(), type);
							query->addResultOrdering(ordering);
						}
					}
					for (CFilteringTermExpression* filteringExp : filteringExps) {
						CVariableBindingFiltering* filtering = createFilterFromExpression(filteringExp);
						query->addResultFiltering(filtering);
					}

					query->setIgnoreCardinalityVariableExpressions(ignoreCardVarExps);
					query->setResultLimit(sparqlBgpExp->getLimit());
					query->setResultOffset(sparqlBgpExp->getOffset());
					query->setDistinctRequired(sparqlBgpExp->isDistinct());
					CQueryStatisticsCollectionStrings* queryStats = nullptr;
					if (confBuildQueryStats) {
						queryStats = new CQueryStatisticsCollectionStrings();
					}
					query->setQueryStatistics(queryStats);
					queryList.append(query);
					LOG(NOTICE, "::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder", logTr("Generated Complex-Assertions-Individual-Variables-Answering-Query '%1' from SPARQL SELECT query.").arg(query->getQueryName()), this);
				}
				mSparqlSelectExpList.clear();



				foreach(CQuerySPARQLAskBasicGraphPatternExpression* sparqlBgpExp, mSparqlAskExpList) {
					QString queryName = sparqlBgpExp->getName();
					const QList<CAxiomExpression*>& axiomExps = *sparqlBgpExp->getAxiomExpressionList();
					const QList<CExpressionVariable*> disVarExps;
					const QList<COrderingTermExpression*> orderingExps;
					const QList<CFilteringTermExpression*>& filteringExps = *sparqlBgpExp->getFilteringExpressionList();

					CComplexAssertionsIndividualVariablesAnsweringQuery *query = new CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery(mBaseOntology, mOntology, axiomExps, disVarExps, config, queryName);
					for (CFilteringTermExpression* filteringExp : filteringExps) {
						CVariableBindingFiltering* filtering = createFilterFromExpression(filteringExp);
						query->addResultFiltering(filtering);
					}

					query->setBooleanEntailmentResultRequired(true);
					CQueryStatisticsCollectionStrings* queryStats = nullptr;
					if (confBuildQueryStats) {
						queryStats = new CQueryStatisticsCollectionStrings();
					}
					query->setQueryStatistics(queryStats);
					queryList.append(query);
					LOG(NOTICE, "::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder", logTr("Generated Complex-Assertions-Individual-Variables-Answering-Query '%1' from SPARQL ASK query.").arg(query->getQueryName()), this);
				}
				mSparqlAskExpList.clear();


				queryList.append(CConcreteOntologyQueryExtendedBuilder::generateQuerys());
				return queryList;
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
