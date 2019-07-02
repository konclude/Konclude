/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CConcreteOntologyQueryExtendedBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			CConcreteOntologyQueryExtendedBuilder::CConcreteOntologyQueryExtendedBuilder(CConcreteOntology* ontology, CConfigurationBase *configuration, COntologyBuilder* ontologyBuilder)
					: CConcreteOntologyQueryBasicBuilder(ontology,configuration) {

				mOntologyBuilder = ontologyBuilder;
			}



			CConcreteOntologyQueryExtendedBuilder::~CConcreteOntologyQueryExtendedBuilder() {
			}



			CQueryIsEntailedExpression* CConcreteOntologyQueryExtendedBuilder::getIsEntailedQuery(const CEXPRESSIONLIST<CAxiomExpression*>& testAxiomExpressions, const QString& queryName) {
				CQueryIsEntailedExpression *classEqExp = new CQueryIsEntailedExpression(queryName,testAxiomExpressions);
				for (CEXPRESSIONLIST<CAxiomExpression*>::const_iterator axiomIt = testAxiomExpressions.constBegin(), axiomEndIt = testAxiomExpressions.constEnd(); axiomIt != axiomEndIt; ++axiomIt) {
					CAxiomExpression* axiom = *axiomIt;
					bool queryForAxiomCreated = false;
					
					CSubClassOfExpression* subClassAxiom = dynamic_cast<CSubClassOfExpression*>(axiom);
					if (subClassAxiom) {
						CEntailmentAxiomExpressionAssociation* entAxiomExpAss = new CEntailmentAxiomExpressionAssociation(axiom);
						CClassTermExpression* subClassExp = subClassAxiom->getSubClassTermExpression();
						CClassTermExpression* superClassExp = subClassAxiom->getSuperClassTermExpression();
						CClassTermExpression* negSuperClassExp = mOntologyBuilder->getObjectComplementOf(superClassExp);
						CClassTermExpression* buildExp = mOntologyBuilder->getObjectIntersectionOf(CEXPRESSIONLIST<CClassTermExpression*>()<<subClassExp<<negSuperClassExp);
						entAxiomExpAss->addConstructedTestClass(buildExp);
						classEqExp->addConstructedTestAxiomExpressions(entAxiomExpAss);
						queryForAxiomCreated = true;
					}

					CEquivalentClassesExpression* equivClassAxiom = dynamic_cast<CEquivalentClassesExpression*>(axiom);
					if (equivClassAxiom) {
						CEXPRESSIONLIST<CClassTermExpression*>* equivClassesList = equivClassAxiom->getClassTermExpressionList();
						CEXPRESSIONLIST<CClassTermExpression*>::const_iterator clItFirst = equivClassesList->constBegin();
						CEXPRESSIONLIST<CClassTermExpression*>::const_iterator clItNext = clItFirst;
						CEXPRESSIONLIST<CClassTermExpression*>::const_iterator clItLast = clItNext;
						++clItNext;
						for (CEXPRESSIONLIST<CClassTermExpression*>::const_iterator clItEnd = equivClassesList->constEnd(); clItNext != clItEnd; ++clItNext) {
							clItLast = clItNext;
							CEntailmentAxiomExpressionAssociation* entAxiomExpAss = new CEntailmentAxiomExpressionAssociation(axiom);
							CClassTermExpression* classExp1 = *clItLast;
							CClassTermExpression* classExp2 = *clItNext;
							CClassTermExpression* negSuperClassExp = mOntologyBuilder->getObjectComplementOf(classExp2);
							CClassTermExpression* buildExp = mOntologyBuilder->getObjectIntersectionOf(CEXPRESSIONLIST<CClassTermExpression*>()<<classExp1<<negSuperClassExp);
							entAxiomExpAss->addConstructedTestClass(buildExp);
							classEqExp->addConstructedTestAxiomExpressions(entAxiomExpAss);
						}
						CEntailmentAxiomExpressionAssociation* entAxiomExpAss = new CEntailmentAxiomExpressionAssociation(axiom);
						CClassTermExpression* classExp1 = *clItLast;
						CClassTermExpression* classExp2 = *clItFirst;
						CClassTermExpression* negSuperClassExp = mOntologyBuilder->getObjectComplementOf(classExp2);
						CClassTermExpression* buildExp = mOntologyBuilder->getObjectIntersectionOf(CEXPRESSIONLIST<CClassTermExpression*>()<<classExp1<<negSuperClassExp);
						entAxiomExpAss->addConstructedTestClass(buildExp);
						classEqExp->addConstructedTestAxiomExpressions(entAxiomExpAss);
						queryForAxiomCreated = true;
					}

					CObjectPropertyDomainExpression* objPropDomainAxiom = dynamic_cast<CObjectPropertyDomainExpression*>(axiom);
					if (objPropDomainAxiom) {
						CEntailmentAxiomExpressionAssociation* entAxiomExpAss = new CEntailmentAxiomExpressionAssociation(axiom);
						CClassTermExpression* domainClassExp = objPropDomainAxiom->getClassTermExpression();
						CObjectPropertyTermExpression* objPropExp = objPropDomainAxiom->getObjectPropertyTermExpression();
						CClassTermExpression* negDomainClassExp = mOntologyBuilder->getObjectComplementOf(domainClassExp);
						CClassTermExpression* topClassExp = mOntologyBuilder->getTopClass();
						CClassTermExpression* someObjPropExp = mOntologyBuilder->getObjectSomeValuesFrom(objPropExp,topClassExp);
						CClassTermExpression* buildExp = mOntologyBuilder->getObjectIntersectionOf(CEXPRESSIONLIST<CClassTermExpression*>()<<someObjPropExp<<negDomainClassExp);
						entAxiomExpAss->addConstructedTestClass(buildExp);
						classEqExp->addConstructedTestAxiomExpressions(entAxiomExpAss);
						queryForAxiomCreated = true;
					}

					CObjectPropertyRangeExpression* objPropRangeAxiom = dynamic_cast<CObjectPropertyRangeExpression*>(axiom);
					if (objPropRangeAxiom) {
						CEntailmentAxiomExpressionAssociation* entAxiomExpAss = new CEntailmentAxiomExpressionAssociation(axiom);
						CClassTermExpression* rangeClassExp = objPropRangeAxiom->getClassTermExpression();
						CObjectPropertyTermExpression* objPropExp = objPropRangeAxiom->getObjectPropertyTermExpression();
						CClassTermExpression* negRangeClassExp = mOntologyBuilder->getObjectComplementOf(rangeClassExp);
						CClassTermExpression* buildExp = mOntologyBuilder->getObjectSomeValuesFrom(objPropExp,negRangeClassExp);
						entAxiomExpAss->addConstructedTestClass(buildExp);
						classEqExp->addConstructedTestAxiomExpressions(entAxiomExpAss);
						queryForAxiomCreated = true;
					}

					CClassAssertionExpression* classAssAxiom = dynamic_cast<CClassAssertionExpression*>(axiom);
					if (classAssAxiom) {
						CEntailmentAxiomExpressionAssociation* entAxiomExpAss = new CEntailmentAxiomExpressionAssociation(axiom,CEntailmentAxiomExpressionAssociation::EAECTAXIOMSPESIFIC);
						classEqExp->addConstructedTestAxiomExpressions(entAxiomExpAss);
						queryForAxiomCreated = true;
					}


					if (!queryForAxiomCreated) {
						LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Ignoring not supported axiom type for IsEntailed-Query '%1'.").arg(queryName),this);
					}

				}
				container.append(classEqExp);
				isEntailedExpList.append(classEqExp);
				return classEqExp;
			}



			CQueryAreClassesEquivalentExpression *CConcreteOntologyQueryExtendedBuilder::getAreClassesEquivalenceQuery(const CEXPRESSIONLIST<CClassTermExpression*> &testClassExpressions, const QString& queryName) {
				CQueryAreClassesEquivalentExpression *classEqExp = new CQueryAreClassesEquivalentExpression(queryName,testClassExpressions);
				for (CEXPRESSIONLIST<CClassTermExpression*>::const_iterator it0 = testClassExpressions.constBegin(); it0 != testClassExpressions.constEnd(); ++it0) {
					CEXPRESSIONLIST<CClassTermExpression*>::const_iterator it1 = it0;
					if (++it1 == testClassExpressions.constEnd()) {
						it1 = testClassExpressions.constBegin();
					}
					CClassTermExpression *buildExp1 = *it0;
					CClassTermExpression *buildExp2 = *it1;
					CClassTermExpression *compBuildExp1 = mOntologyBuilder->getObjectComplementOf(buildExp1);
					CEXPRESSIONLIST<CClassTermExpression*> intersectExpressionsList;
					intersectExpressionsList.append(compBuildExp1);
					intersectExpressionsList.append(buildExp2);
					CClassTermExpression *intersectionCompBuildExp1AndBuildExp2 = mOntologyBuilder->getObjectIntersectionOf(intersectExpressionsList);
					classEqExp->addConstructedTestClass(intersectionCompBuildExp1AndBuildExp2);
				}
				container.append(classEqExp);
				classEqExpList.append(classEqExp);
				return classEqExp;
			}


			CQueryAreClassesDisjointExpression *CConcreteOntologyQueryExtendedBuilder::getAreClassesDisjointQuery(const CEXPRESSIONLIST<CClassTermExpression*> &testClassExpressions, const QString& queryName) {
				CQueryAreClassesDisjointExpression *classEqExp = new CQueryAreClassesDisjointExpression(queryName,testClassExpressions);
				for (CEXPRESSIONLIST<CClassTermExpression*>::const_iterator it0 = testClassExpressions.constBegin(); it0 != testClassExpressions.constEnd(); ++it0) {
					for (CEXPRESSIONLIST<CClassTermExpression*>::const_iterator it1 = it0; it1 != testClassExpressions.constEnd(); ++it1) {
						CClassTermExpression *buildExp1 = *it0;
						CClassTermExpression *buildExp2 = *it1;
						CEXPRESSIONLIST<CClassTermExpression*> intersectExpressionsList;
						intersectExpressionsList.append(buildExp1);
						intersectExpressionsList.append(buildExp2);
						CClassTermExpression *intersectionBuildExp1AndBuildExp2 = mOntologyBuilder->getObjectIntersectionOf(intersectExpressionsList);
						classEqExp->addConstructedTestClass(intersectionBuildExp1AndBuildExp2);
					}
				}
				container.append(classEqExp);
				classDisjointExpList.append(classEqExp);
				return classEqExp;
			}

			CQueryIsClassSubsumedByExpression *CConcreteOntologyQueryExtendedBuilder::getIsClassSubsumedByQuery(CClassTermExpression* subsumerClassExpression, CClassTermExpression* subsumedClassExpression, const QString& queryName)  {
				CQueryIsClassSubsumedByExpression* classEqExp = new CQueryIsClassSubsumedByExpression(queryName,subsumerClassExpression,subsumedClassExpression);
				CClassTermExpression *compBuildExp2 = mOntologyBuilder->getObjectComplementOf(subsumedClassExpression);
				CEXPRESSIONLIST<CClassTermExpression*> intersectExpressionsList;
				intersectExpressionsList.append(compBuildExp2);
				intersectExpressionsList.append(subsumerClassExpression);
				CClassTermExpression *intersectionBuildExp1AndCompBuildExp2 = mOntologyBuilder->getObjectIntersectionOf(intersectExpressionsList);
				classEqExp->setConstructedTestClassExpression(intersectionBuildExp1AndCompBuildExp2);
				container.append(classEqExp);
				classSubsumedExpList.append(classEqExp);
				return classEqExp;
			}



			QList<CQuery*> CConcreteOntologyQueryExtendedBuilder::generateQuerys() {
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
					foreach (CClassTermExpression *buildExp, *classEqExp->getConstructedTestClasses()) {
						CConcept *concept = getConceptFromBuildExpression(buildExp);
						if (concept) {

							CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(concept,concept->hasMappingNegation());
							if (confBuildQueryStats) {
								satCalcJob->setCalclulationStatisticsCollector(queryStats->createCalculationStatisticsCollection());
							}
							satCalcJob->setCalculationConfiguration(calcConfig);
							query->addTestSatisfiableCalculationJob(satCalcJob,false);


						}
					}
					query->setQueryStatistics(queryStats);

					queryList.append(query);
					LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated AreClassesEquivalent-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
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
					foreach (CClassTermExpression *buildExp, *classDisjExp->getConstructedTestClasses()) {
						CConcept *concept = getConceptFromBuildExpression(buildExp);
						if (concept) {

							CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(concept,concept->hasMappingNegation());
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
				}



				foreach (CQueryIsClassSubsumedByExpression *classSubsumedExp, classSubsumedExpList) {
					QString queryName = classSubsumedExp->getName();

					QString subsumedClassName;
					QString subsummerClassName;

					CConcept *subsumedConcept = 0;
					CConcept *subsumerConcept = 0;

					subsumedConcept = getConceptFromBuildExpression(classSubsumedExp->getSubsumedClassExpression());
					subsumerConcept = getConceptFromBuildExpression(classSubsumedExp->getSubsumerClassExpression());

					subsumedClassName = getStringFromConcept(subsumedConcept);
					subsummerClassName = getStringFromConcept(subsumerConcept);

					CConcept *concept = getConceptFromBuildExpression(classSubsumedExp->getConstructedTestClassExpression());

					CIsClassSubsumedByQuery *query = new CIsClassSubsumedByQuery(subsumedClassName,subsummerClassName,queryName);
					CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);

					CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(concept,concept->hasMappingNegation());

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
				}











				foreach (CQueryIsEntailedExpression *isEntailedExp, isEntailedExpList) {
					QString queryName = isEntailedExp->getName();


					QStringList axiomTypeNameStrings;
					QSet<QString> axiomTypeNameStringSet;
					CEXPRESSIONLIST<CEntailmentAxiomExpressionAssociation*>* entAxExpAssList = isEntailedExp->getConstructedTestAxiomExpressions();
					foreach (CEntailmentAxiomExpressionAssociation* entAxiExpAss, *entAxExpAssList) {
						CAxiomExpression* axiomExpression = entAxiExpAss->getAxiomExpression();
						CSubClassOfExpression* subClassAxiom = dynamic_cast<CSubClassOfExpression*>(axiomExpression);
						if (subClassAxiom) {
							if (!axiomTypeNameStringSet.contains("SubClassOf")) {
								axiomTypeNameStringSet.insert("SubClassOf");
								axiomTypeNameStrings += QString("SubClassOf");
							}
						}
						CEquivalentClassesExpression* equivClassAxiom = dynamic_cast<CEquivalentClassesExpression*>(axiomExpression);
						if (equivClassAxiom) {
							if (!axiomTypeNameStringSet.contains("EquivalentClasses")) {
								axiomTypeNameStringSet.insert("EquivalentClasses");
								axiomTypeNameStrings += QString("EquivalentClasses");
							}
						}
						CObjectPropertyDomainExpression* objPropDomainAxiom = dynamic_cast<CObjectPropertyDomainExpression*>(axiomExpression);
						if (objPropDomainAxiom) {
							if (!axiomTypeNameStringSet.contains("ObjectPropertyDomain")) {
								axiomTypeNameStringSet.insert("ObjectPropertyDomain");
								axiomTypeNameStrings += QString("ObjectPropertyDomain");
							}
						}
						CObjectPropertyRangeExpression* objPropRangeAxiom = dynamic_cast<CObjectPropertyRangeExpression*>(axiomExpression);
						if (objPropRangeAxiom) {
							if (!axiomTypeNameStringSet.contains("ObjectPropertyRange")) {
								axiomTypeNameStringSet.insert("ObjectPropertyRange");
								axiomTypeNameStrings += QString("ObjectPropertyRange");
							}
						}
						CClassAssertionExpression* classAssAxiom = dynamic_cast<CClassAssertionExpression*>(axiomExpression);
						if (classAssAxiom) {
							if (!axiomTypeNameStringSet.contains("ClassAssertion")) {
								axiomTypeNameStringSet.insert("ClassAssertion");
								axiomTypeNameStrings += QString("ClassAssertion");
							}
						}
					}
					CAreAxiomsEntailedQuery *query = new CAreAxiomsEntailedQuery(axiomTypeNameStrings,queryName);
					CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
					CQueryStatisticsCollectionStrings* queryStats = nullptr;
					if (confBuildQueryStats) {
						queryStats = new CQueryStatisticsCollectionStrings();
					}


					foreach (CEntailmentAxiomExpressionAssociation* entAxiExpAss, *entAxExpAssList) {
						CAxiomExpression* axiomExpression = entAxiExpAss->getAxiomExpression();
						CEntailmentAxiomExpressionAssociation::ENTAILMENTAXIOMEXPRESSIONCONSTRUCTIONTYPE constructionType = entAxiExpAss->getConstructionType();

						if (constructionType == CEntailmentAxiomExpressionAssociation::EAECTCONCEPTSATISFIABILITY) {

							CConcept* concept = getConceptFromBuildExpression(entAxiExpAss->takeNextConstructedTestClass());
							if (concept) {
								CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(concept,concept->hasMappingNegation());
								if (confBuildQueryStats) {
									satCalcJob->setCalclulationStatisticsCollector(queryStats->createCalculationStatisticsCollection());
								}
								satCalcJob->setCalculationConfiguration(calcConfig);
								query->addTestSatisfiableCalculationJob(satCalcJob,false);
							}
						} else if (constructionType == CEntailmentAxiomExpressionAssociation::EAECTAXIOMSPESIFIC) {
							CClassAssertionExpression* classAssAxiom = dynamic_cast<CClassAssertionExpression*>(axiomExpression);
							if (classAssAxiom) {
								CIndividual* individual = getIndividualFromBuildExpression(classAssAxiom->getIndividualTermExpression());
								CConcept* concept = getConceptFromBuildExpression(entAxiExpAss->takeNextConstructedTestClass());
								if (concept && individual) {
									CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(concept,concept->hasMappingNegation(),individual);
									if (confBuildQueryStats) {
										satCalcJob->setCalclulationStatisticsCollector(queryStats->createCalculationStatisticsCollection());
									}
									satCalcJob->setCalculationConfiguration(calcConfig);
									query->addTestSatisfiableCalculationJob(satCalcJob,false);
								}
							}
						}
					}
					query->setQueryStatistics(queryStats);

					queryList.append(query);
					LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated AreEntailed-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
				}






				delete satCalcJobGenerator;


				queryList.append(CConcreteOntologyQueryBasicBuilder::generateQuerys());
				return queryList;
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
