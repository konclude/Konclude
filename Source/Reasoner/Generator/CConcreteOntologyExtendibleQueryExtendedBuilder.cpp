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



				foreach(CQuerySPARQLSelectBasicGraphPatternExpression* sparqlBgpExp, mSparqlIndividualSelectExpList) {
					createSelectQueryFromBGPExpression(sparqlBgpExp, "Individual", [&](CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName) {
						return new CComplexAssertionsIndividualVariablesAnsweringQuery(baseOntology, expressionOntology, axiomExpressions, disVariables, configuration, queryName);
					}, confBuildQueryStats, queryList);
				}
				mSparqlIndividualSelectExpList.clear();

				foreach(CQuerySPARQLSelectBasicGraphPatternExpression* sparqlBgpExp, mSparqlClassSelectExpList) {
					createSelectQueryFromBGPExpression(sparqlBgpExp, "Class", [&](CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName) {
						return new CComplexAxiomsClassVariablesAnsweringQuery(baseOntology, expressionOntology, axiomExpressions, disVariables, configuration, queryName);
					}, confBuildQueryStats, queryList);
				}
				mSparqlClassSelectExpList.clear();

				foreach(CQuerySPARQLSelectBasicGraphPatternExpression* sparqlBgpExp, mSparqlObjPropSelectExpList) {
					createSelectQueryFromBGPExpression(sparqlBgpExp, "Object-Property", [&](CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName) {
						return new CComplexAxiomsObjectPropertyVariablesAnsweringQuery(baseOntology, expressionOntology, axiomExpressions, disVariables, configuration, queryName);
					}, confBuildQueryStats, queryList);
				}
				mSparqlObjPropSelectExpList.clear();

				foreach(CQuerySPARQLSelectBasicGraphPatternExpression* sparqlBgpExp, mSparqlDataPropSelectExpList) {
					createSelectQueryFromBGPExpression(sparqlBgpExp, "Data-Property", [&](CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName) {
						return new CComplexAxiomsDataPropertyVariablesAnsweringQuery(baseOntology, expressionOntology, axiomExpressions, disVariables, configuration, queryName);
					}, confBuildQueryStats, queryList);
				}
				mSparqlDataPropSelectExpList.clear();




				foreach(CQuerySPARQLSelectBasicGraphPatternExpression* sparqlBgpExp, mSparqlIndividualMixedSelectExpList) {
					createSelectQueryFromBGPExpression(sparqlBgpExp, "Individual-Mixed", [&](CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName) {
						return new CComplexAxiomsIndividualMixedVariablesAnsweringQuery(baseOntology, expressionOntology, axiomExpressions, disVariables, configuration, queryName);
					}, confBuildQueryStats, queryList);
				}
				mSparqlIndividualMixedSelectExpList.clear();


				foreach(CQuerySPARQLAskBasicGraphPatternExpression* sparqlBgpExp, mSparqlIndividualAskExpList) {
					createAskQueryFromBGPExpression(sparqlBgpExp, "Individual", [&](CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName) {
						return new CComplexAssertionsIndividualVariablesAnsweringQuery(baseOntology, expressionOntology, axiomExpressions, disVariables, configuration, queryName);
					}, confBuildQueryStats, queryList);
				}
				mSparqlIndividualAskExpList.clear();

				foreach(CQuerySPARQLAskBasicGraphPatternExpression* sparqlBgpExp, mSparqlClassAskExpList) {
					createAskQueryFromBGPExpression(sparqlBgpExp, "Class", [&](CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName) {
						return new CComplexAxiomsClassVariablesAnsweringQuery(baseOntology, expressionOntology, axiomExpressions, disVariables, configuration, queryName);
					}, confBuildQueryStats, queryList);
				}
				mSparqlClassAskExpList.clear();

				foreach(CQuerySPARQLAskBasicGraphPatternExpression* sparqlBgpExp, mSparqlObjectPropAskExpList) {
					createAskQueryFromBGPExpression(sparqlBgpExp, "Object-Property", [&](CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName) {
						return new CComplexAxiomsObjectPropertyVariablesAnsweringQuery(baseOntology, expressionOntology, axiomExpressions, disVariables, configuration, queryName);
					}, confBuildQueryStats, queryList);
				}
				mSparqlObjectPropAskExpList.clear();

				foreach(CQuerySPARQLAskBasicGraphPatternExpression* sparqlBgpExp, mSparqlDataPropAskExpList) {
					createAskQueryFromBGPExpression(sparqlBgpExp, "Data-Property", [&](CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName) {
						return new CComplexAxiomsDataPropertyVariablesAnsweringQuery(baseOntology, expressionOntology, axiomExpressions, disVariables, configuration, queryName);
					}, confBuildQueryStats, queryList);
				}
				mSparqlDataPropAskExpList.clear();

				queryList.append(CConcreteOntologyQueryExtendedBuilder::generateQuerys());
				return queryList;
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
