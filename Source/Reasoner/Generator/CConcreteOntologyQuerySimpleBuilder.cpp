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

#include "CConcreteOntologyQuerySimpleBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			CConcreteOntologyQuerySimpleBuilder::CConcreteOntologyQuerySimpleBuilder(CConcreteOntology* ontology, CConfigurationBase *configuration)
					: CConcreteOntologyQueryBasicBuilder(ontology,configuration) {

			}



			CConcreteOntologyQuerySimpleBuilder::~CConcreteOntologyQuerySimpleBuilder() {
			}





			CQueryAreClassesEquivalentExpression *CConcreteOntologyQuerySimpleBuilder::getAreClassesEquivalenceQuery(const CEXPRESSIONLIST<CClassTermExpression*> &testClassExpressions, const QString& queryName) {
				CQueryAreClassesEquivalentExpression *classEqExp = new CQueryAreClassesEquivalentExpression(queryName,testClassExpressions);
				container.append(classEqExp);
				classEqExpList.append(classEqExp);
				return classEqExp;
			}


			CQueryAreClassesDisjointExpression *CConcreteOntologyQuerySimpleBuilder::getAreClassesDisjointQuery(const CEXPRESSIONLIST<CClassTermExpression*> &testClassExpressions, const QString& queryName) {
				CQueryAreClassesDisjointExpression *classEqExp = new CQueryAreClassesDisjointExpression(queryName,testClassExpressions);
				container.append(classEqExp);
				classDisjointExpList.append(classEqExp);
				return classEqExp;
			}

			CQueryIsClassSubsumedByExpression *CConcreteOntologyQuerySimpleBuilder::getIsClassSubsumedByQuery(CClassTermExpression* subsumerClassExpression, CClassTermExpression* subsumedClassExpression, const QString& queryName)  {
				CQueryIsClassSubsumedByExpression* classEqExp = new CQueryIsClassSubsumedByExpression(queryName,subsumerClassExpression,subsumedClassExpression);
				container.append(classEqExp);
				classSubsumedExpList.append(classEqExp);
				return classEqExp;
			}

			
			CQueryIsEntailedExpression* CConcreteOntologyQuerySimpleBuilder::getIsEntailedQuery(const CEXPRESSIONLIST<CAxiomExpression*>& axiomExpressions, const QString& queryName) {
				return nullptr;
			}




			QList<CQuery*> CConcreteOntologyQuerySimpleBuilder::generateQuerys() {
				QList<CQuery*> queryList;

				CSatisfiableCalculationJobGenerator* satCalcJobGenerator = new CSatisfiableCalculationJobGenerator(mOntology);

				bool confBuildQueryStats = true;



				foreach (CQueryAreClassesEquivalentExpression *classEqExp, classEqExpList) {
					QString queryName = classEqExp->getName();
					QStringList conceptNamesStrings;
					foreach (CClassTermExpression *buildExp, *classEqExp->getClassTermExpressionList()) {
						CConcept *concept = getConceptFromBuildExpression(buildExp);
						if (concept) {
							QString className = getStringFromConcept(concept);
							conceptNamesStrings.append(className);
						}
					}
					CAreClassesEquivalentQuery *query = new CAreClassesEquivalentQuery(conceptNamesStrings,queryName);
					CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
					CQueryStatisticsCollectionStrings* queryStats = nullptr;
					if (confBuildQueryStats) {
						queryStats = new CQueryStatisticsCollectionStrings();
					}
					CEXPRESSIONLIST<CClassTermExpression*>* testClassExpressions = classEqExp->getClassTermExpressionList();
					for (CEXPRESSIONLIST<CClassTermExpression*>::const_iterator it0 = testClassExpressions->constBegin(); it0 != testClassExpressions->constEnd(); ++it0) {
						CEXPRESSIONLIST<CClassTermExpression*>::const_iterator it1 = it0;
						if (++it1 == testClassExpressions->constEnd()) {
							it1 = testClassExpressions->constBegin();
						}
						CClassTermExpression *buildExp1 = *it0;
						CClassTermExpression *buildExp2 = *it1;
						CConcept *concept1 = getConceptFromBuildExpression(buildExp1);
						CConcept *concept2 = getConceptFromBuildExpression(buildExp2);

						CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(concept1,false,concept2,true);
						if (confBuildQueryStats) {
							satCalcJob->setCalclulationStatisticsCollector(queryStats->createCalculationStatisticsCollection());
						}
						satCalcJob->setCalculationConfiguration(calcConfig);
						query->addTestSatisfiableCalculationJob(satCalcJob,false);
					}

					query->setQueryStatistics(queryStats);

					queryList.append(query);
					LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated AreClassesEquivalent-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					
					delete classEqExp;
				}



				foreach (CQueryAreClassesDisjointExpression *classDisjExp, classDisjointExpList) {
					QString queryName = classDisjExp->getName();
					QStringList conceptNamesStrings;
					foreach (CClassTermExpression *buildExp, *classDisjExp->getClassTermExpressionList()) {
						CConcept *concept = getConceptFromBuildExpression(buildExp);
						if (concept) {
							QString className = getStringFromConcept(concept);
							conceptNamesStrings.append(className);
						}
					}
					CAreClassesDisjointQuery *query = new CAreClassesDisjointQuery(conceptNamesStrings,queryName);
					CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
					CQueryStatisticsCollectionStrings* queryStats = nullptr;
					if (confBuildQueryStats) {
						queryStats = new CQueryStatisticsCollectionStrings();
					}

					CEXPRESSIONLIST<CClassTermExpression*>* testClassExpressions = classDisjExp->getClassTermExpressionList();
					for (CEXPRESSIONLIST<CClassTermExpression*>::const_iterator it0 = testClassExpressions->constBegin(); it0 != testClassExpressions->constEnd(); ++it0) {
						for (CEXPRESSIONLIST<CClassTermExpression*>::const_iterator it1 = it0; it1 != testClassExpressions->constEnd(); ++it1) {
							CClassTermExpression *buildExp1 = *it0;
							CClassTermExpression *buildExp2 = *it1;
							CConcept *concept1 = getConceptFromBuildExpression(buildExp1);
							CConcept *concept2 = getConceptFromBuildExpression(buildExp2);

							CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(concept1,false,concept2,false);
							if (confBuildQueryStats) {
								satCalcJob->setCalclulationStatisticsCollector(queryStats->createCalculationStatisticsCollection());
							}
							satCalcJob->setCalculationConfiguration(calcConfig);
							query->addTestSatisfiableCalculationJob(satCalcJob,false);
						}
					}

					query->setQueryStatistics(queryStats);
					queryList.append(query);
					LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated AreClassesDisjoint-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
				
					delete classDisjExp;
				}



				foreach (CQueryIsClassSubsumedByExpression *classSubsumedExp, classSubsumedExpList) {
					QString queryName = classSubsumedExp->getName();
					QStringList conceptNamesStrings;

					QString subsumedClassName;
					QString subsummerClassName;
					qint64 loopStep = 0;

					CConcept *subsumedConcept = 0;
					CConcept *subsumerConcept = 0;

					subsumedConcept = getConceptFromBuildExpression(classSubsumedExp->getSubsumedClassExpression());
					subsumerConcept = getConceptFromBuildExpression(classSubsumedExp->getSubsumerClassExpression());

					subsumedClassName = getStringFromConcept(subsumedConcept);
					subsummerClassName = getStringFromConcept(subsumerConcept);

					CIsClassSubsumedByQuery *query = new CIsClassSubsumedByQuery(subsumedClassName,subsummerClassName,queryName);
					CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);

					CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(subsumerConcept,false,subsumedConcept,true);

					CQueryStatisticsCollectionStrings* queryStats = nullptr;
					if (confBuildQueryStats) {
						queryStats = new CQueryStatisticsCollectionStrings();
						satCalcJob->setCalclulationStatisticsCollector(queryStats->createCalculationStatisticsCollection());
					}
					query->setQueryStatistics(queryStats);
					satCalcJob->setCalculationConfiguration(calcConfig);
					query->addTestSatisfiableCalculationJob(satCalcJob,false);


					queryList.append(query);
					LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated IsClassSubsemedBy-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);

					delete classSubsumedExp;
				}


				delete satCalcJobGenerator;


				queryList.append(CConcreteOntologyQueryBasicBuilder::generateQuerys());
				return queryList;
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
