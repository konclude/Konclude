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

#include "CConcreteOntologyBuildDataUpdater.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyBuildDataUpdater::CConcreteOntologyBuildDataUpdater(CConcreteOntology* updateConcreteOntology) {
				mOnto = updateConcreteOntology;
			}



			CDeclarationAxiomExpression* CConcreteOntologyBuildDataUpdater::getDeclaration(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				FOREACHIT (CBuildExpression* buildExpression, expressions) {
					CClassExpression* classExpr = dynamic_cast<CClassExpression*>(buildExpression);
					if (classExpr) {
						return getDeclaration((CExpressionEntity*)classExpr);
					}
					CObjectPropertyExpression* propertyExp = dynamic_cast<CObjectPropertyExpression*>(buildExpression);
					if (propertyExp) {
						return getDeclaration((CExpressionEntity*)propertyExp);
					}
					CNamedIndividualExpression* indiExp = dynamic_cast<CNamedIndividualExpression*>(buildExpression);
					if (indiExp) {
						return getDeclaration((CExpressionEntity*)indiExp);
					}
				}
				LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'Declaration'-Expression."),this);
				return nullptr;
			}


			CDeclarationAxiomExpression* CConcreteOntologyBuildDataUpdater::getDeclaration(CBuildExpression* buildExpression) {
				if (buildExpression) {
					CClassExpression* classExpr = dynamic_cast<CClassExpression*>(buildExpression);
					if (classExpr) {
						return getDeclaration((CExpressionEntity*)classExpr);
					}
					CObjectPropertyExpression* propertyExp = dynamic_cast<CObjectPropertyExpression*>(buildExpression);
					if (propertyExp) {
						return getDeclaration((CExpressionEntity*)propertyExp);
					}
					CNamedIndividualExpression* indiExp = dynamic_cast<CNamedIndividualExpression*>(buildExpression);
					if (indiExp) {
						return getDeclaration((CExpressionEntity*)indiExp);
					}
				}
				LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'Declaration'-Expression."),this);
				return nullptr;
			}


			CEquivalentClassesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentClasses(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CEquivalentClassesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(2)) {
					expression = getEquivalentClasses(*expSplitter.getClassTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'EquivalentClasses'-Expression."),this);
				}
				return expression;
			}

			CEquivalentClassesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentClasses(CBuildExpression* expression1, CBuildExpression* expression2) {
				CEquivalentClassesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(2)) {
					expression = getEquivalentClasses(*expSplitter.getClassTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'EquivalentClasses'-Expression."),this);
				}
				return expression;
			}

			CSubClassOfExpression* CConcreteOntologyBuildDataUpdater::getSubClassOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CSubClassOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(2)) {
					expression = getSubClassOf(expSplitter.getFirstClassTermExpression(),expSplitter.getSecondClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SubClassOf'-Expression."),this);
				}
				return expression;
			}

			CSubClassOfExpression* CConcreteOntologyBuildDataUpdater::getSubClassOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				CSubClassOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(2)) {
					expression = getSubClassOf(expSplitter.getFirstClassTermExpression(),expSplitter.getSecondClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SubClassOf'-Expression."),this);
				}
				return expression;
			}


			CDisjointClassesExpression* CConcreteOntologyBuildDataUpdater::getDisjointClasses(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDisjointClassesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(2)) {
					expression = getDisjointClasses(*expSplitter.getClassTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DisjointClasses'-Expression."),this);
				}
				return expression;
			}

			CDisjointUnionExpression* CConcreteOntologyBuildDataUpdater::getDisjointUnion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDisjointUnionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(2)) {
					CEXPRESSIONLIST<CClassTermExpression*>* classTermExpressionList = expSplitter.getClassTermExpressionList();
					CClassExpression* classExpression = nullptr;
					cint64 pos = -1;
					FOREACHIT (CClassTermExpression* classTermExp, *classTermExpressionList) {
						++pos;
						if (dynamic_cast<CClassExpression*>(classTermExp)) {
							classExpression = (CClassExpression*)classTermExp;
							break;
						}
					}
					if (classExpression) {
						classTermExpressionList->removeAt(pos);
					} else {
						LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't resolve ClassExpression for 'DisjointUnion'-Expression."),this);
					}
					expression = getDisjointUnion(classExpression,*classTermExpressionList);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DisjointUnion'-Expression."),this);
				}
				return expression;
			}

			CObjectComplementOfExpression* CConcreteOntologyBuildDataUpdater::getObjectComplementOf(CBuildExpression* compExpression) {
				CObjectComplementOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(compExpression);
				if (expSplitter.proofExpressionComposition(1)) {
					expression = getObjectComplementOf(expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectComplementOf'-Expression."),this);
				}
				return expression;
			}

			CObjectComplementOfExpression* CConcreteOntologyBuildDataUpdater::getObjectComplementOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectComplementOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(1)) {
					expression = getObjectComplementOf(expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectComplementOf'-Expression."),this);
				}
				return expression;
			}

			CObjectIntersectionOfExpression* CConcreteOntologyBuildDataUpdater::getObjectIntersectionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectIntersectionOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0)) {
					expression = getObjectIntersectionOf(*expSplitter.getClassTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectIntersectionOf'-Expression."),this);
				}
				return expression;
			}

			CObjectUnionOfExpression* CConcreteOntologyBuildDataUpdater::getObjectUnionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectUnionOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0)) {
					expression = getObjectUnionOf(*expSplitter.getClassTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectUnionOf'-Expression."),this);
				}
				return expression;
			}

			CObjectMaxCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMaxCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				CObjectMaxCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectMaxCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectMaxCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectMaxCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMaxCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				CObjectMaxCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectMaxCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectMaxCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectMinCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMinCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				CObjectMinCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectMinCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectMinCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectMinCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMinCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				CObjectMinCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectMinCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectMinCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectExactlyCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectExactlyCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				CObjectExactlyCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectExactlyCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectExactCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectExactlyCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectExactlyCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				CObjectExactlyCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectExactlyCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectExactCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectAllValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectAllValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectAllValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectAllValuesFrom(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectAllValuesFrom'-Expression."),this);
				}
				return expression;
			}

			CObjectAllValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectAllValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectAllValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectAllValuesFrom(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectAllValuesFrom'-Expression."),this);
				}
				return expression;
			}

			CObjectSomeValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectSomeValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectSomeValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectSomeValuesFrom(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectSomeValuesFrom'-Expression."),this);
				}
				return expression;
			}

			CObjectSomeValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectSomeValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectSomeValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectSomeValuesFrom(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectSomeValuesFrom'-Expression."),this);
				}
				return expression;
			}


			CObjectOneOfExpression* CConcreteOntologyBuildDataUpdater::getObjectOneOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectOneOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,0,0)) {
					expression = getObjectOneOf(*expSplitter.getIndividualTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectOneOf'-Expression."),this);
				}
				return expression;
			}


			CObjectOneOfExpression* CConcreteOntologyBuildDataUpdater::getObjectOneOf(CBuildExpression* indiExpression) {
				CObjectOneOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(indiExpression);
				if (expSplitter.proofExpressionComposition(0,0,0)) {
					expression = getObjectOneOf(*expSplitter.getIndividualTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectOneOf'-Expression."),this);
				}
				return expression;
			}



			CObjectHasValueExpression* CConcreteOntologyBuildDataUpdater::getObjectHasValue(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectHasValueExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1,1)) {
					expression = getObjectHasValue(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstIndividualTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectHasValue'-Expression."),this);
				}
				return expression;
			}

			CObjectHasValueExpression* CConcreteOntologyBuildDataUpdater::getObjectHasValue(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectHasValueExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,1,1)) {
					expression = getObjectHasValue(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstIndividualTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectHasValue'-Expression."),this);
				}
				return expression;
			}



			CObjectHasSelfExpression* CConcreteOntologyBuildDataUpdater::getObjectHasSelf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectHasSelfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectHasSelf(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectHasSelf'-Expression."),this);
				}
				return expression;
			}

			CObjectHasSelfExpression* CConcreteOntologyBuildDataUpdater::getObjectHasSelf(CBuildExpression* selfExpression) {
				CObjectHasSelfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(selfExpression);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectHasSelf(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectHasSelf'-Expression."),this);
				}
				return expression;
			}



			CSubObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getSubObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CSubObjectPropertyOfExpression* expression = nullptr;
				if (expressions.count() == 2) {
					CObjectPropertyChainExpression* propertyChain = dynamic_cast<CObjectPropertyChainExpression*>(expressions.first());
					CObjectPropertyTermExpression* superObjectPropTerm = dynamic_cast<CObjectPropertyTermExpression*>(expressions.last());
					if (propertyChain && superObjectPropTerm) {
						expression = getSubObjectPropertyOf(propertyChain,superObjectPropTerm);
					}
				}
				if (!expression) {
					CExpressionSplitter expSplitter(expressions);
					if (expSplitter.proofExpressionComposition(0,2)) {
						CEXPRESSIONLIST<CObjectPropertyTermExpression*>* objectPropertyExpressionList = expSplitter.getObjectPropertyTermExpressionList();
						CObjectPropertyTermExpression* superObjectPropertyExpression = objectPropertyExpressionList->takeLast();
						expression = getSubObjectPropertyOf(*objectPropertyExpressionList,superObjectPropertyExpression);
					} else {
						LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SubObjectPropertyOf'-Expression."),this);
					}
				}
				return expression;
			}

			CSubObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getSubObjectPropertyOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				CSubObjectPropertyOfExpression* expression = nullptr;
				CObjectPropertyChainExpression* propertyChain = dynamic_cast<CObjectPropertyChainExpression*>(expression1);
				CObjectPropertyTermExpression* superObjectPropTerm = dynamic_cast<CObjectPropertyTermExpression*>(expression2);
				if (propertyChain && superObjectPropTerm) {
					expression = getSubObjectPropertyOf(propertyChain,superObjectPropTerm);
				}
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,2)) {
					CEXPRESSIONLIST<CObjectPropertyTermExpression*>* objectPropertyExpressionList = expSplitter.getObjectPropertyTermExpressionList();
					CObjectPropertyTermExpression* superObjectPropertyExpression = objectPropertyExpressionList->takeLast();
					expression = getSubObjectPropertyOf(*objectPropertyExpressionList,superObjectPropertyExpression);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SubObjectPropertyOf'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyChainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyChain(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectPropertyChainExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectPropertyChain(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyChain'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyChainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyChain(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectPropertyChainExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getObjectPropertyChain(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyChain'-Expression."),this);
				}
				return expression;
			}

			CEquivalentObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CEquivalentObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,2)) {
					expression = getEquivalentObjectProperties(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'EquivalentObjectProperties'-Expression."),this);
				}
				return expression;
			}

			CEquivalentObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				CEquivalentObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,2)) {
					expression = getEquivalentObjectProperties(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'EquivalentObjectProperties'-Expression."),this);
				}
				return expression;
			}

			CDisjointObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getDisjointObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDisjointObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,2)) {
					expression = getDisjointObjectProperties(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DisjointObjectProperties'-Expression."),this);
				}
				return expression;
			}

			CDisjointObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getDisjointObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDisjointObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,2)) {
					expression = getDisjointObjectProperties(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DisjointObjectProperties'-Expression."),this);
				}
				return expression;
			}


			CObjectPropertyDomainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyDomainExpression(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectPropertyDomainExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(1,1)) {
					expression = getObjectPropertyDomainExpression(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyDomain'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyDomainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyDomainExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectPropertyDomainExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(1,1)) {
					expression = getObjectPropertyDomainExpression(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyDomain'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyRangeExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyRangeExpression(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectPropertyRangeExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(1,1)) {
					expression = getObjectPropertyRangeExpression(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyRange'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyRangeExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyRangeExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectPropertyRangeExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(1,1)) {
					expression = getObjectPropertyRangeExpression(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyRange'-Expression."),this);
				}
				return expression;
			}


			CInverseObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CInverseObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,2)) {
					expression = getInverseObjectProperties(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getSecondObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseObjectProperties'-Expression."),this);
				}
				return expression;
			}

			CInverseObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				CInverseObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(0,2)) {
					expression = getInverseObjectProperties(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getSecondObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseObjectProperties'-Expression."),this);
				}
				return expression;
			}


			CTransetiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getTransetiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CTransetiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getTransetiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'TransetiveObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CTransetiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getTransetiveObjectProperty(CBuildExpression* transExpression) {
				CTransetiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(transExpression);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getTransetiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'TransetiveObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CFunctionalObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getFunctionalObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'FunctionalObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getFunctionalObjectProperty(CBuildExpression* funcExpression) {
				CFunctionalObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getFunctionalObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'FunctionalObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CInverseFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getInverseFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CInverseFunctionalObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getInverseFunctionalObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseFunctionalObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CInverseFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getInverseFunctionalObjectProperty(CBuildExpression* funcExpression) {
				CInverseFunctionalObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getInverseFunctionalObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseFunctionalObjectProperty'-Expression."),this);
				}
				return expression;
			}
			CSymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getSymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CSymmetricObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getSymmetricObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SymmetricObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CSymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getSymmetricObjectProperty(CBuildExpression* funcExpression) {
				CSymmetricObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getSymmetricObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SymmetricObjectProperty'-Expression."),this);
				}
				return expression;
			}


			CAsymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getAsymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CAsymmetricObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getAsymmetricObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'AsymmetricObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CAsymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getAsymmetricObjectProperty(CBuildExpression* funcExpression) {
				CAsymmetricObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getAsymmetricObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'AsymmetricObjectProperty'-Expression."),this);
				}
				return expression;
			}


			CReflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getReflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CReflexiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getReflexiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ReflexiveObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CReflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getReflexiveObjectProperty(CBuildExpression* funcExpression) {
				CReflexiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getReflexiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ReflexiveObjectProperty'-Expression."),this);
				}
				return expression;
			}


			CIrreflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getIrreflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CIrreflexiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getIrreflexiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'IrreflexiveObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CIrreflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getIrreflexiveObjectProperty(CBuildExpression* funcExpression) {
				CIrreflexiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getIrreflexiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'IrreflexiveObjectProperty'-Expression."),this);
				}
				return expression;
			}


			CInverseObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CInverseObjectPropertyOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getInverseObjectPropertyOf(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseObjectPropertyOf'-Expression."),this);
				}
				return expression;
			}

			CInverseObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectPropertyOf(CBuildExpression* invExpression) {
				CInverseObjectPropertyOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(invExpression);
				if (expSplitter.proofExpressionComposition(0,1)) {
					expression = getInverseObjectPropertyOf(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseObjectPropertyOf'-Expression."),this);
				}
				return expression;
			}



			CClassAssertionExpression* CConcreteOntologyBuildDataUpdater::getClassAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CClassAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(1,0,1)) {
					expression = getClassAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ClassAssertion'-Expression."),this);
				}
				return expression;
			}

			CClassAssertionExpression* CConcreteOntologyBuildDataUpdater::getClassAssertion(CBuildExpression* expression1, CBuildExpression* expression2) {
				CClassAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.proofExpressionComposition(1,0,1)) {
					expression = getClassAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ClassAssertion'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1,2)) {
					expression = getObjectPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getSecondIndividualTermExpression(),expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyAssertion'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				CObjectPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2,expression3);
				if (expSplitter.proofExpressionComposition(0,1,2)) {
					expression = getObjectPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getSecondIndividualTermExpression(),expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyAssertion'-Expression."),this);
				}
				return expression;
			}


			CNegativeObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getNegativeObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CNegativeObjectPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,1,2)) {
					expression = getNegativeObjectPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getSecondIndividualTermExpression(),expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'NegativeObjectPropertyAssertion'-Expression."),this);
				}
				return expression;
			}

			CNegativeObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getNegativeObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				CNegativeObjectPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2,expression3);
				if (expSplitter.proofExpressionComposition(0,1,2)) {
					expression = getNegativeObjectPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getSecondIndividualTermExpression(),expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'NegativeObjectPropertyAssertion'-Expression."),this);
				}
				return expression;
			}


			CSameIndividualExpression* CConcreteOntologyBuildDataUpdater::getSameIndividual(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CSameIndividualExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,0,2)) {
					expression = getSameIndividual(*expSplitter.getIndividualTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SameIndividual'-Expression."),this);
				}
				return expression;
			}


			CDifferentIndividualsExpression* CConcreteOntologyBuildDataUpdater::getDifferentIndividuals(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDifferentIndividualsExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.proofExpressionComposition(0,0,2)) {
					expression = getDifferentIndividuals(*expSplitter.getIndividualTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DifferentIndividual'-Expression."),this);
				}
				return expression;
			}

			CDeclarationAxiomExpression* CConcreteOntologyBuildDataUpdater::getDeclaration(CExpressionEntity* entity) {
				CDeclarationAxiomExpression* expression = nullptr;
				CDeclarationAxiomExpression tmpExpression(entity);
				expression = (CDeclarationAxiomExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CDeclarationAxiomExpression(entity);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}



			CEquivalentClassesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				CEquivalentClassesExpression* expression = nullptr;
				CEquivalentClassesExpression tmpExpression(expressions);
				expression = (CEquivalentClassesExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CEquivalentClassesExpression(expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CSubClassOfExpression* CConcreteOntologyBuildDataUpdater::getSubClassOf(CClassTermExpression* expression1, CClassTermExpression* expression2) {
				CSubClassOfExpression* expression = nullptr;
				CSubClassOfExpression tmpExpression(expression1,expression2);
				expression = (CSubClassOfExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CSubClassOfExpression(expression1,expression2);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CDisjointClassesExpression* CConcreteOntologyBuildDataUpdater::getDisjointClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				CDisjointClassesExpression* expression = nullptr;
				CDisjointClassesExpression tmpExpression(expressions);
				expression = (CDisjointClassesExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CDisjointClassesExpression(expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}


			CDisjointUnionExpression* CConcreteOntologyBuildDataUpdater::getDisjointUnion(CClassExpression* classExpression, const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				CDisjointUnionExpression* expression = nullptr;
				CDisjointUnionExpression tmpExpression(classExpression,expressions);
				expression = (CDisjointUnionExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CDisjointUnionExpression(classExpression,expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}


			CObjectComplementOfExpression* CConcreteOntologyBuildDataUpdater::getObjectComplementOf(CClassTermExpression* compExpression) {
				CObjectComplementOfExpression* expression = nullptr;
				CObjectComplementOfExpression tmpExpression(compExpression);
				expression = (CObjectComplementOfExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectComplementOfExpression(compExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectIntersectionOfExpression* CConcreteOntologyBuildDataUpdater::getObjectIntersectionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				CObjectIntersectionOfExpression* expression = nullptr;
				CObjectIntersectionOfExpression tmpExpression(expressions);
				expression = (CObjectIntersectionOfExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectIntersectionOfExpression(expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectUnionOfExpression* CConcreteOntologyBuildDataUpdater::getObjectUnionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				CObjectUnionOfExpression* expression = nullptr;
				CObjectUnionOfExpression tmpExpression(expressions);
				expression = (CObjectUnionOfExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectUnionOfExpression(expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}


			CObjectMaxCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMaxCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) {
				CObjectMaxCardinalityExpression* expression = nullptr;
				if (expression2 == nullptr) {
					expression2 = mTopClassExpression;
				}
				CObjectMaxCardinalityExpression tmpExpression(expression1,expression2,cardinality);
				expression = (CObjectMaxCardinalityExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectMaxCardinalityExpression(expression1,expression2,cardinality);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectMinCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMinCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) {
				CObjectMinCardinalityExpression* expression = nullptr;
				if (expression2 == nullptr) {
					expression2 = mTopClassExpression;
				}
				CObjectMinCardinalityExpression tmpExpression(expression1,expression2,cardinality);
				expression = (CObjectMinCardinalityExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectMinCardinalityExpression(expression1,expression2,cardinality);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectExactlyCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectExactlyCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) {
				CObjectExactlyCardinalityExpression* expression = nullptr;
				if (expression2 == nullptr) {
					expression2 = mTopClassExpression;
				}
				CObjectExactlyCardinalityExpression tmpExpression(expression1,expression2,cardinality);
				expression = (CObjectExactlyCardinalityExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectExactlyCardinalityExpression(expression1,expression2,cardinality);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectAllValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectAllValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				CObjectAllValuesFromExpression* expression = nullptr;
				CObjectAllValuesFromExpression tmpExpression(expression1,expression2);
				expression = (CObjectAllValuesFromExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectAllValuesFromExpression(expression1,expression2);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectOneOfExpression* CConcreteOntologyBuildDataUpdater::getObjectOneOf(const CEXPRESSIONLIST<CIndividualTermExpression*> &expressions) {
				CObjectOneOfExpression* expression = nullptr;
				CObjectOneOfExpression tmpExpression(expressions);
				expression = (CObjectOneOfExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectOneOfExpression(expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}


			CObjectOneOfExpression* CConcreteOntologyBuildDataUpdater::getObjectOneOf(CIndividualTermExpression* indiExpression) {
				if (!mTmpObjectOneOfExpression) {
					mTmpObjectOneOfExpression = new CObjectOneOfExpression(indiExpression);
				} else {
					mTmpObjectOneOfExpression->initObjectOneOfExpression(indiExpression);
				}
				CBuildExpression*& expression = (*mExpressionBuildHash)[CExpressionHasher(mTmpObjectOneOfExpression)];
				if (!expression) {
					mExpressionBuildContainerList->append(mTmpObjectOneOfExpression);
					mBuildConceptSet->insert(mTmpObjectOneOfExpression);
					mBuildConceptList->append(mTmpObjectOneOfExpression);
					expression = mTmpObjectOneOfExpression;
					mTmpObjectOneOfExpression = nullptr;
				}
				return (CObjectOneOfExpression*)expression;
			}


			CObjectSomeValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectSomeValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				CObjectSomeValuesFromExpression* expression = nullptr;
				CObjectSomeValuesFromExpression tmpExpression(expression1,expression2);
				expression = (CObjectSomeValuesFromExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectSomeValuesFromExpression(expression1,expression2);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectHasValueExpression* CConcreteOntologyBuildDataUpdater::getObjectHasValue(CObjectPropertyTermExpression* expression1, CIndividualTermExpression* expression2) {
				CObjectHasValueExpression* expression = nullptr;
				CObjectHasValueExpression tmpExpression(expression1,expression2);
				expression = (CObjectHasValueExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectHasValueExpression(expression1,expression2);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectHasSelfExpression* CConcreteOntologyBuildDataUpdater::getObjectHasSelf(CObjectPropertyTermExpression* obPrExpression) {
				CObjectHasSelfExpression* expression = nullptr;
				CObjectHasSelfExpression tmpExpression(obPrExpression);
				expression = (CObjectHasSelfExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectHasSelfExpression(obPrExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}


			CSubObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getSubObjectPropertyOf(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& subExpressions, CObjectPropertyTermExpression* superExpression) {
				CSubObjectPropertyOfExpression* expression = nullptr;
				CSubObjectPropertyOfExpression tmpExpression(subExpressions,superExpression);
				expression = (CSubObjectPropertyOfExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CSubObjectPropertyOfExpression(subExpressions,superExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CSubObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getSubObjectPropertyOf(CObjectPropertyChainExpression* expressions, CObjectPropertyTermExpression* superExpression) {
				CSubObjectPropertyOfExpression* expression = nullptr;
				expression = getSubObjectPropertyOf(*expressions->getChainObjectPropertyTermExpressionList(),superExpression);
				return expression;
			}


			CObjectPropertyChainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyChain(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) {
				CObjectPropertyChainExpression* expression = nullptr;
				CObjectPropertyChainExpression tmpExpression(expressions);
				expression = (CObjectPropertyChainExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectPropertyChainExpression(expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CEquivalentObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) {
				CEquivalentObjectPropertiesExpression* expression = nullptr;
				CEquivalentObjectPropertiesExpression tmpExpression(expressions);
				expression = (CEquivalentObjectPropertiesExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CEquivalentObjectPropertiesExpression(expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CDisjointObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getDisjointObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) {
				CDisjointObjectPropertiesExpression* expression = nullptr;
				CDisjointObjectPropertiesExpression tmpExpression(expressions);
				expression = (CDisjointObjectPropertiesExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CDisjointObjectPropertiesExpression(expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}


			CObjectPropertyDomainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyDomainExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				CObjectPropertyDomainExpression* expression = nullptr;
				CObjectPropertyDomainExpression tmpExpression(expression1,expression2);
				expression = (CObjectPropertyDomainExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectPropertyDomainExpression(expression1,expression2);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CObjectPropertyRangeExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyRangeExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				CObjectPropertyRangeExpression* expression = nullptr;
				CObjectPropertyRangeExpression tmpExpression(expression1,expression2);
				expression = (CObjectPropertyRangeExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectPropertyRangeExpression(expression1,expression2);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CInverseObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectProperties(CObjectPropertyTermExpression* expression1, CObjectPropertyTermExpression* expression2) {
				CInverseObjectPropertiesExpression* expression = nullptr;
				CInverseObjectPropertiesExpression tmpExpression(expression1,expression2);
				expression = (CInverseObjectPropertiesExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CInverseObjectPropertiesExpression(expression1,expression2);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CTransetiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getTransetiveObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CTransetiveObjectPropertyExpression* expression = nullptr;
				CTransetiveObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CTransetiveObjectPropertyExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CTransetiveObjectPropertyExpression(buildExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getFunctionalObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CFunctionalObjectPropertyExpression* expression = nullptr;
				CFunctionalObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CFunctionalObjectPropertyExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CFunctionalObjectPropertyExpression(buildExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}


			CInverseFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getInverseFunctionalObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CInverseFunctionalObjectPropertyExpression* expression = nullptr;
				CInverseFunctionalObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CInverseFunctionalObjectPropertyExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CInverseFunctionalObjectPropertyExpression(buildExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CSymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getSymmetricObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CSymmetricObjectPropertyExpression* expression = nullptr;
				CSymmetricObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CSymmetricObjectPropertyExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CSymmetricObjectPropertyExpression(buildExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CAsymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getAsymmetricObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CAsymmetricObjectPropertyExpression* expression = nullptr;
				CAsymmetricObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CAsymmetricObjectPropertyExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CAsymmetricObjectPropertyExpression(buildExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CReflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getReflexiveObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CReflexiveObjectPropertyExpression* expression = nullptr;
				CReflexiveObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CReflexiveObjectPropertyExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CReflexiveObjectPropertyExpression(buildExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}


			CIrreflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getIrreflexiveObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CIrreflexiveObjectPropertyExpression* expression = nullptr;
				CIrreflexiveObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CIrreflexiveObjectPropertyExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CIrreflexiveObjectPropertyExpression(buildExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CInverseObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectPropertyOf(CObjectPropertyTermExpression* buildExpression) {
				CInverseObjectPropertyOfExpression* expression = nullptr;
				CInverseObjectPropertyOfExpression tmpExpression(buildExpression);
				expression = (CInverseObjectPropertyOfExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CInverseObjectPropertyOfExpression(buildExpression);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mBuildRoleSet->insert(expression);
					mBuildRoleList->append(expression);
					mInverseObjectPropertyHash->insert(buildExpression,expression);
					mInverseObjectPropertyHash->insert(expression,buildExpression);
					mInverseObjectPropertyList->append( QPair<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>(expression,buildExpression) );
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}


			CObjectPropertyTermExpression* CConcreteOntologyBuildDataUpdater::getCorrectedInverseObjectPropertyOf(CObjectPropertyTermExpression* invExpression) {
				CInverseObjectPropertyOfExpression* invCastExpression = dynamic_cast<CInverseObjectPropertyOfExpression*>(invExpression);
				if (invCastExpression) {
					return invCastExpression->getInverseOfExpression();
				}
				CInverseObjectPropertyOfExpression* expression = nullptr;
				expression = getInverseObjectPropertyOf(invExpression);
				return expression;
			}



			CClassAssertionExpression* CConcreteOntologyBuildDataUpdater::getClassAssertion(CIndividualTermExpression* expression1, CClassTermExpression* expression2) {
				CClassAssertionExpression* expression = nullptr;
				CClassAssertionExpression tmpExpression(expression1,expression2);
				expression = (CClassAssertionExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CClassAssertionExpression(expression1,expression2);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3) {
				CObjectPropertyAssertionExpression* expression = nullptr;
				CObjectPropertyAssertionExpression tmpExpression(expression3,expression1,expression2);
				expression = (CObjectPropertyAssertionExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CObjectPropertyAssertionExpression(expression3,expression1,expression2);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CNegativeObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getNegativeObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3) {
				CNegativeObjectPropertyAssertionExpression* expression = nullptr;
				CNegativeObjectPropertyAssertionExpression tmpExpression(expression3,expression1,expression2);
				expression = (CNegativeObjectPropertyAssertionExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CNegativeObjectPropertyAssertionExpression(expression3,expression1,expression2);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CSameIndividualExpression* CConcreteOntologyBuildDataUpdater::getSameIndividual(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions) {
				CSameIndividualExpression* expression = nullptr;
				CSameIndividualExpression tmpExpression(expressions);
				expression = (CSameIndividualExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CSameIndividualExpression(expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}

			CDifferentIndividualsExpression* CConcreteOntologyBuildDataUpdater::getDifferentIndividuals(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions) {
				CDifferentIndividualsExpression* expression = nullptr;
				CDifferentIndividualsExpression tmpExpression(expressions);
				expression = (CDifferentIndividualsExpression*)mExpressionBuildHash->value(CExpressionHasher(&tmpExpression),nullptr);
				if (!expression) {
					expression = new CDifferentIndividualsExpression(expressions);
					mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
					mExpressionBuildContainerList->append(expression);
				}
				return expression;
			}





			bool CConcreteOntologyBuildDataUpdater::initializeBuilding() {

				mTmpObjectOneOfExpression = nullptr;


				mMemManager = CContext::getMemoryAllocationManager(mOnto->getOntologyContext());

				mOntoStrings = mOnto->getStringMapping();
				mOntoBuild = mOnto->getBuildData();

				mDeclarationAxiomSet = mOntoBuild->getDeclarationAxiomSet();

				mTopClassExpression = mOntoBuild->getTopClassExpression();
				mBottomClassExpression = mOntoBuild->getBottomClassExpression();
				mTopObjPropExpression = mOntoBuild->getTopObjectPropertyExpression();
				mBottomObjPropExpression = mOntoBuild->getBottomObjectPropertyExpression();

				mBuildConceptSet = mOntoBuild->getBuildConceptSet();
				mBuildRoleSet = mOntoBuild->getBuildRoleSet();
				mBuildIndividualSet = mOntoBuild->getBuildIndividualSet();

				mBuildConceptList = mOntoBuild->getBuildConceptList();
				mBuildRoleList = mOntoBuild->getBuildRoleList();
				mBuildIndividualList = mOntoBuild->getBuildIndividualList();

				mExpressionBuildContainerList = mOntoBuild->getExpressionBuildListContainer();
				mInverseObjectPropertyHash = mOntoBuild->getInverseObjectPropertyHash();
				mInverseObjectPropertyList = mOntoBuild->getInverseObjectPropertyList();

				mExpressionBuildHash = mOntoBuild->getStructuralExpressionBuildHash();

				mClassBuildHash = mOntoBuild->getClassEntityBuildHash();
				mObjectPropertyBuildHash = mOntoBuild->getObjectPropertyEntityBuildHash();
				mIndividualBuildHash = mOntoBuild->getIndividualEntityBuildHash();
				mAnoIndividualBuildHash = mOntoBuild->getAnonymousIndividualBuildHash();
				mIndividualVariableBuildHash = mOntoBuild->getIndividualVariableBuildHash();

				mNextAxiomNumber = mOntoBuild->getNextAxiomNumber();
				mNextEntityNumber = mOntoBuild->getNextEntityNumber();
				mNextMaxAxiomNumberOffset = 0;


				mAbbreviatedNamePrefixMapHash = mOntoStrings->getAbbreviatedNamePrefixHash();
				mNamePrefixMapHash = mOntoStrings->getNamePrefixHash();
				mPrefixList = mOntoStrings->getPrefixList();

				mIndividualVariableIDHash = mOntoBuild->getIndividualVariableIDHash();
				return true;
			}



			bool CConcreteOntologyBuildDataUpdater::completeBuilding() {

				delete mTmpObjectOneOfExpression;
				mTmpObjectOneOfExpression = nullptr;

				return true;
			}


			cint64 CConcreteOntologyBuildDataUpdater::getAxiomNumber() {
				return mNextAxiomNumber;
			}



			CObjectPropertyTermExpression* CConcreteOntologyBuildDataUpdater::getTopObjectProberty() {
				return mTopObjPropExpression;
			}

			CClassTermExpression* CConcreteOntologyBuildDataUpdater::getTopClass() {
				CClassTermExpression* expression = mTopClassExpression;
				return expression;
			}

			CClassTermExpression* CConcreteOntologyBuildDataUpdater::getBottomClass() {
				CClassTermExpression* expression = mBottomClassExpression;
				return expression;
			}


			bool CConcreteOntologyBuildDataUpdater::addNameAbbreviation(const QString &name, const QString &abbreviatedName) {
				if (!mAbbreviatedNamePrefixMapHash->contains(abbreviatedName)) {
					CNamePrefix* namePrefix = new CNamePrefix(abbreviatedName,name);
					mAbbreviatedNamePrefixMapHash->insert(abbreviatedName,namePrefix);
					mNamePrefixMapHash->insert(name,namePrefix);
					mPrefixList->append(namePrefix);
					return true;
				} else {
					return false;
				}
			}

			QString CConcreteOntologyBuildDataUpdater::getOntologyName() {
				return mOnto->getOntologyName();
			}

			QString CConcreteOntologyBuildDataUpdater::resolveAbbreviationName(const QString& name) {
				QString resolvedString(name);
				cint64 colonPos = name.indexOf(":");
				if (colonPos >= 0 && colonPos < name.length()) {
					QString abbName = name.mid(0,colonPos);
					QString idName = name.mid(colonPos+1);
					CNamePrefix* namePrefix = mAbbreviatedNamePrefixMapHash->value(abbName,nullptr);
					if (namePrefix) {
						resolvedString = namePrefix->getPrefixString()+idName;
					}
				}
				return resolvedString;
			}

			QString CConcreteOntologyBuildDataUpdater::resolveAbbreviationName(const QStringRef& name) {
				QString resolvedString(name.toString());
				cint64 colonPos = resolvedString.indexOf(":");
				if (colonPos >= 0 && colonPos < resolvedString.length()) {
					QString abbName = resolvedString.mid(0,colonPos);
					QString idName = resolvedString.mid(colonPos+1);
					CNamePrefix* namePrefix = mAbbreviatedNamePrefixMapHash->value(abbName,nullptr);
					if (namePrefix) {
						resolvedString = namePrefix->getPrefixString()+idName;
					}
				}
				return resolvedString;
			}

			CClassExpression* CConcreteOntologyBuildDataUpdater::getClass(const QString& className) {
				CClassExpression* expression = nullptr;
				expression = mClassBuildHash->value(className);
				if (!expression) {
					expression = new CClassExpression(className);
					expression->setEntityID(mNextEntityNumber++);
					mClassBuildHash->insert(className,expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CClassExpression* CConcreteOntologyBuildDataUpdater::getClass(const QStringRef& className) {
				CClassExpression* expression = nullptr;
				expression = mClassBuildHash->value(className);
				if (!expression) {
					QString classNameString(className.toString());
					expression = new CClassExpression(classNameString);
					expression->setEntityID(mNextEntityNumber++);
					mClassBuildHash->insert(classNameString,expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectIndividualVariableExpression* CConcreteOntologyBuildDataUpdater::getIndividualVariable(const QString &individualVariableName, cint64 axiomNumber) {
				CObjectIndividualVariableExpression* expression = nullptr;
				expression = mIndividualVariableBuildHash->value(QPair<CStringRefStringHasher,cint64>(individualVariableName,axiomNumber));
				if (!expression) {
					expression = new CObjectIndividualVariableExpression(individualVariableName,axiomNumber);
					expression->setEntityID(mNextEntityNumber++);
					mNextMaxAxiomNumberOffset = qMax(axiomNumber,mNextMaxAxiomNumberOffset);
					mIndividualVariableBuildHash->insert(QPair<CStringRefStringHasher,cint64>(individualVariableName,axiomNumber),expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectIndividualVariableExpression* CConcreteOntologyBuildDataUpdater::getIndividualVariable(const QStringRef &individualVariableName, cint64 axiomNumber) {
				CObjectIndividualVariableExpression* expression = nullptr;
				expression = mIndividualVariableBuildHash->value(QPair<CStringRefStringHasher,cint64>(individualVariableName,axiomNumber));
				if (!expression) {
					QString individualVariableNameString(individualVariableName.toString());
					mNextMaxAxiomNumberOffset = qMax(axiomNumber,mNextMaxAxiomNumberOffset);
					expression = new CObjectIndividualVariableExpression(individualVariableNameString,axiomNumber);
					expression->setEntityID(mNextEntityNumber++);
					mIndividualVariableBuildHash->insert(QPair<CStringRefStringHasher,cint64>(individualVariableNameString,axiomNumber),expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
				}
				return expression;
			}

			CObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getObjectProberty(const QString& probertyName) {
				CObjectPropertyExpression* expression = nullptr;
				expression = mObjectPropertyBuildHash->value(probertyName);
				if (!expression) {
					expression = new CObjectPropertyExpression(probertyName);
					expression->setEntityID(mNextEntityNumber++);
					mObjectPropertyBuildHash->insert(probertyName,expression);
					mBuildRoleSet->insert(expression);
					mBuildRoleList->append(expression);
				}
				return expression;
			}

			CObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getObjectProberty(const QStringRef& probertyName) {
				CObjectPropertyExpression* expression = nullptr;
				expression = mObjectPropertyBuildHash->value(probertyName);
				if (!expression) {
					QString propertyNameString(probertyName.toString());
					expression = new CObjectPropertyExpression(propertyNameString);
					expression->setEntityID(mNextEntityNumber++);
					mObjectPropertyBuildHash->insert(propertyNameString,expression);
					mBuildRoleSet->insert(expression);
					mBuildRoleList->append(expression);
				}
				return expression;
			}

			CNamedIndividualExpression* CConcreteOntologyBuildDataUpdater::getNamedIndividual(const QString& individualName) {
				CNamedIndividualExpression* expression = nullptr;
				expression = mIndividualBuildHash->value(individualName);
				if (!expression) {
					expression = new CNamedIndividualExpression(individualName);
					expression->setEntityID(mNextEntityNumber++);
					mIndividualBuildHash->insert(individualName,expression);
					mBuildIndividualSet->insert(expression);
					mBuildIndividualList->append(expression);
				}
				return expression;
			}

			CNamedIndividualExpression* CConcreteOntologyBuildDataUpdater::getNamedIndividual(const QStringRef& individualName) {
				CNamedIndividualExpression* expression = nullptr;
				expression = mIndividualBuildHash->value(individualName);
				if (!expression) {
					QString individualNameString(individualName.toString());
					expression = new CNamedIndividualExpression(individualNameString);
					expression->setEntityID(mNextEntityNumber++);
					mIndividualBuildHash->insert(individualNameString,expression);
					mBuildIndividualSet->insert(expression);
					mBuildIndividualList->append(expression);
				}
				return expression;
			}



			CAnonymousIndividualExpression* CConcreteOntologyBuildDataUpdater::getAnonymousIndividual(const QString& ontologyName, const QString& individualName) {
				CAnonymousIndividualExpression* expression = nullptr;
				QPair<CStringRefStringHasher,CStringRefStringHasher> namePair(ontologyName,individualName);
				expression = mAnoIndividualBuildHash->value(namePair);
				if (!expression) {
					expression = new CAnonymousIndividualExpression(ontologyName,individualName);
					mAnoIndividualBuildHash->insert(namePair,expression);
					mBuildIndividualSet->insert(expression);
					mBuildIndividualList->append(expression);
				}
				return expression;
			}


			CAnonymousIndividualExpression* CConcreteOntologyBuildDataUpdater::getAnonymousIndividual(const QStringRef& ontologyName, const QStringRef& individualName) {
				CAnonymousIndividualExpression* expression = nullptr;
				QPair<CStringRefStringHasher,CStringRefStringHasher> namePair(ontologyName,individualName);
				expression = mAnoIndividualBuildHash->value(namePair);
				if (!expression) {
					QString ontologyNameString(ontologyName.toString());
					QString individualNameString(individualName.toString());
					expression = new CAnonymousIndividualExpression(ontologyNameString,individualNameString);
					mAnoIndividualBuildHash->insert(QPair<CStringRefStringHasher,CStringRefStringHasher>(ontologyNameString,individualNameString),expression);
					mBuildIndividualSet->insert(expression);
					mBuildIndividualList->append(expression);
				}
				return expression;
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude