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
					CDataPropertyExpression* dataPropExp = dynamic_cast<CDataPropertyExpression*>(buildExpression);
					if (dataPropExp) {
						return getDeclaration((CExpressionEntity*)dataPropExp);
					}
					CDatatypeExpression* datatypeExp = dynamic_cast<CDatatypeExpression*>(buildExpression);
					if (datatypeExp) {
						return getDeclaration((CExpressionEntity*)datatypeExp);
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
					CDataPropertyExpression* dataPropExp = dynamic_cast<CDataPropertyExpression*>(buildExpression);
					if (dataPropExp) {
						return getDeclaration((CExpressionEntity*)dataPropExp);
					}
					CDatatypeExpression* datatypeExp = dynamic_cast<CDatatypeExpression*>(buildExpression);
					if (datatypeExp) {
						return getDeclaration((CExpressionEntity*)datatypeExp);
					}
				}
				LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'Declaration'-Expression."),this);
				return nullptr;
			}


			CEquivalentClassesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentClasses(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CEquivalentClassesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(2)) {
					expression = getEquivalentClasses(*expSplitter.getClassTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'EquivalentClasses'-Expression."),this);
				}
				return expression;
			}

			CEquivalentClassesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentClasses(CBuildExpression* expression1, CBuildExpression* expression2) {
				CEquivalentClassesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(2)) {
					expression = getEquivalentClasses(*expSplitter.getClassTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'EquivalentClasses'-Expression."),this);
				}
				return expression;
			}

			CSubClassOfExpression* CConcreteOntologyBuildDataUpdater::getSubClassOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CSubClassOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(2)) {
					expression = getSubClassOf(expSplitter.getFirstClassTermExpression(),expSplitter.getSecondClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SubClassOf'-Expression."),this);
				}
				return expression;
			}

			CSubClassOfExpression* CConcreteOntologyBuildDataUpdater::getSubClassOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				CSubClassOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(2)) {
					expression = getSubClassOf(expSplitter.getFirstClassTermExpression(),expSplitter.getSecondClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SubClassOf'-Expression."),this);
				}
				return expression;
			}


			CDisjointClassesExpression* CConcreteOntologyBuildDataUpdater::getDisjointClasses(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDisjointClassesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(2)) {
					expression = getDisjointClasses(*expSplitter.getClassTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DisjointClasses'-Expression."),this);
				}
				return expression;
			}

			CDisjointUnionExpression* CConcreteOntologyBuildDataUpdater::getDisjointUnion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDisjointUnionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(2)) {
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
				if (expSplitter.testForExpressionComposition(1)) {
					expression = getObjectComplementOf(expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectComplementOf'-Expression."),this);
				}
				return expression;
			}

			CObjectComplementOfExpression* CConcreteOntologyBuildDataUpdater::getObjectComplementOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectComplementOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(1)) {
					expression = getObjectComplementOf(expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectComplementOf'-Expression."),this);
				}
				return expression;
			}

			CObjectIntersectionOfExpression* CConcreteOntologyBuildDataUpdater::getObjectIntersectionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectIntersectionOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0)) {
					expression = getObjectIntersectionOf(*expSplitter.getClassTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectIntersectionOf'-Expression."),this);
				}
				return expression;
			}

			CObjectUnionOfExpression* CConcreteOntologyBuildDataUpdater::getObjectUnionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectUnionOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0)) {
					expression = getObjectUnionOf(*expSplitter.getClassTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectUnionOf'-Expression."),this);
				}
				return expression;
			}

			CObjectMaxCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMaxCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				CObjectMaxCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectMaxCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectMaxCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectMaxCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMaxCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				CObjectMaxCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectMaxCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectMaxCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectMinCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMinCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				CObjectMinCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectMinCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectMinCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectMinCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMinCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				CObjectMinCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectMinCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectMinCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectExactlyCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectExactlyCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				CObjectExactlyCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectExactlyCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectExactCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectExactlyCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectExactlyCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				CObjectExactlyCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectExactlyCardinality(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectExactCardinality'-Expression."),this);
				}
				return expression;
			}

			CObjectAllValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectAllValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectAllValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectAllValuesFrom(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectAllValuesFrom'-Expression."),this);
				}
				return expression;
			}

			CObjectAllValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectAllValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectAllValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectAllValuesFrom(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectAllValuesFrom'-Expression."),this);
				}
				return expression;
			}

			CObjectSomeValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectSomeValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectSomeValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectSomeValuesFrom(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectSomeValuesFrom'-Expression."),this);
				}
				return expression;
			}

			CObjectSomeValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectSomeValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectSomeValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectSomeValuesFrom(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstOptionalClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectSomeValuesFrom'-Expression."),this);
				}
				return expression;
			}


			CDataFacetRestrictionExpression* CConcreteOntologyBuildDataUpdater::getDataFacetRestriction(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataFacetRestrictionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,0,1,0,0,1)) {
					CDataLiteralExpression* dataLitExp = dynamic_cast<CDataLiteralExpression*>(expSplitter.getFirstDataLiteralTermExpression());
					if (dataLitExp) {
						expression = getDataFacetRestriction(dataLitExp, expSplitter.getFirstDataFacetExpression());
					}
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'FacetRestriction'-Expression."),this);
				}
				return expression;
			}

			CDataFacetRestrictionExpression* CConcreteOntologyBuildDataUpdater::getDataFacetRestriction(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDataFacetRestrictionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,0,0,1,0,0,1)) {
					CDataLiteralExpression* dataLitExp = dynamic_cast<CDataLiteralExpression*>(expSplitter.getFirstDataLiteralTermExpression());
					if (dataLitExp) {
						expression = getDataFacetRestriction(dataLitExp, expSplitter.getFirstDataFacetExpression());
					}
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'FacetRestriction'-Expression."),this);
				}
				return expression;
			}

			CDataSomeValuesFromExpression* CConcreteOntologyBuildDataUpdater::getDataSomeValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataSomeValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getDataSomeValuesFrom(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataSomeValuesFrom'-Expression."),this);
				}
				return expression;
			}

			CDataSomeValuesFromExpression* CConcreteOntologyBuildDataUpdater::getDataSomeValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDataSomeValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getDataSomeValuesFrom(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataSomeValuesFrom'-Expression."),this);
				}
				return expression;
			}


			CDataLiteralExpression* CConcreteOntologyBuildDataUpdater::getDataLiteral(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataLiteralExpression* expression = nullptr;
				if (expressions.size() >= 2) {
					expression = getDataLiteral(expressions.first(), expressions.last());
				} else {
					LOG(ERROR, "::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder", logTr("Couldn't match parameters for 'DataLiteral'-Expression."), this);
				}
				return expression;
			}


			CDataLiteralExpression* CConcreteOntologyBuildDataUpdater::getDataLiteral(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDataLiteralExpression* expression = nullptr;

				CDataLexicalValueExpression* dataLexicalValue = dynamic_cast<CDataLexicalValueExpression*>(expression1);
				CDatatypeExpression* datatype = dynamic_cast<CDatatypeExpression*>(expression2);
				if (!dataLexicalValue) {
					dataLexicalValue = dynamic_cast<CDataLexicalValueExpression*>(expression2);
					datatype = dynamic_cast<CDatatypeExpression*>(expression1);
				}

				if (dataLexicalValue && datatype) {
					expression = getDataLiteral(dataLexicalValue, datatype);
				} else {
					LOG(ERROR, "::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder", logTr("Couldn't match parameters for 'DataLiteral'-Expression."), this);
				}
				return expression;
			}




			CDataHasValueExpression* CConcreteOntologyBuildDataUpdater::getDataHasValue(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataHasValueExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getDataHasValue(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataHasValue'-Expression."),this);
				}
				return expression;
			}

			CDataHasValueExpression* CConcreteOntologyBuildDataUpdater::getDataHasValue(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDataHasValueExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getDataHasValue(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataHasValue'-Expression."),this);
				}
				return expression;
			}




			CDataAllValuesFromExpression* CConcreteOntologyBuildDataUpdater::getDataAllValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataAllValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,1,1)) {
					expression = getDataAllValuesFrom(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataAllValuesFrom'-Expression."),this);
				}
				return expression;
			}

			CDataAllValuesFromExpression* CConcreteOntologyBuildDataUpdater::getDataAllValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDataAllValuesFromExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,1,1)) {
					expression = getDataAllValuesFrom(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataAllValuesFrom'-Expression."),this);
				}
				return expression;
			}





			CDataIntersectionOfExpression* CConcreteOntologyBuildDataUpdater::getDataIntersectionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataIntersectionOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,1)) {
					expression = getDataIntersectionOf(*expSplitter.getDataRangeTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataIntersectionOf'-Expression."),this);
				}
				return expression;
			}

			CDataIntersectionOfExpression* CConcreteOntologyBuildDataUpdater::getDataIntersectionOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDataIntersectionOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,1)) {
					expression = getDataIntersectionOf(*expSplitter.getDataRangeTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataIntersectionOf'-Expression."),this);
				}
				return expression;
			}





			CDataOneOfExpression* CConcreteOntologyBuildDataUpdater::getDataOneOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataOneOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,1)) {
					expression = getDataOneOf(*expSplitter.getDataRangeTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataOneOf'-Expression."),this);
				}
				return expression;
			}

			CDataOneOfExpression* CConcreteOntologyBuildDataUpdater::getDataOneOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDataOneOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,1)) {
					expression = getDataOneOf(*expSplitter.getDataRangeTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataOneOf'-Expression."),this);
				}
				return expression;
			}





			CDataUnionOfExpression* CConcreteOntologyBuildDataUpdater::getDataUnionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataUnionOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,1)) {
					expression = getDataUnionOf(*expSplitter.getDataRangeTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataUnionOf'-Expression."),this);
				}
				return expression;
			}

			CDataUnionOfExpression* CConcreteOntologyBuildDataUpdater::getDataUnionOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDataUnionOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,1)) {
					expression = getDataUnionOf(*expSplitter.getDataRangeTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataUnionOf'-Expression."),this);
				}
				return expression;
			}





			CDataComplementOfExpression* CConcreteOntologyBuildDataUpdater::getDataComplementOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataComplementOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,1)) {
					expression = getDataComplementOf(expSplitter.getFirstDataRangeTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataComplementOf'-Expression."),this);
				}
				return expression;
			}

			CDataComplementOfExpression* CConcreteOntologyBuildDataUpdater::getDataComplementOf(CBuildExpression* expression1) {
				CDataComplementOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1);
				if (expSplitter.testForExpressionComposition(0,0,0,1)) {
					expression = getDataComplementOf(expSplitter.getFirstDataRangeTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataComplementOf'-Expression."),this);
				}
				return expression;
			}




			CDatatypeRestrictionExpression* CConcreteOntologyBuildDataUpdater::getDatatypeRestriction(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDatatypeRestrictionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,0,0,1,1)) {
					expression = getDatatypeRestriction(expSplitter.getFirstDatatypeExpression(),*expSplitter.getDataFacetRestrictionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DatatypeRestriction'-Expression."),this);
				}
				return expression;
			}

			CDatatypeRestrictionExpression* CConcreteOntologyBuildDataUpdater::getDatatypeRestriction(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDatatypeRestrictionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,0,0,0,1,1)) {
					expression = getDatatypeRestriction(expSplitter.getFirstDatatypeExpression(),*expSplitter.getDataFacetRestrictionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DatatypeRestriction'-Expression."),this);
				}
				return expression;
			}




			CDataMaxCardinalityExpression* CConcreteOntologyBuildDataUpdater::getDataMaxCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				CDataMaxCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getDataMaxCardinality(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataMaxCardinality'-Expression."),this);
				}
				return expression;
			}

			CDataMaxCardinalityExpression* CConcreteOntologyBuildDataUpdater::getDataMaxCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				CDataMaxCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getDataMaxCardinality(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataMaxCardinality'-Expression."),this);
				}
				return expression;
			}





			CDataMinCardinalityExpression* CConcreteOntologyBuildDataUpdater::getDataMinCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				CDataMinCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getDataMinCardinality(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataMinCardinality'-Expression."),this);
				}
				return expression;
			}

			CDataMinCardinalityExpression* CConcreteOntologyBuildDataUpdater::getDataMinCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				CDataMinCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getDataMinCardinality(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataMinCardinality'-Expression."),this);
				}
				return expression;
			}




			CDataExactCardinalityExpression* CConcreteOntologyBuildDataUpdater::getDataExactCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				CDataExactCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getDataExactCardinality(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataExactCardinality'-Expression."),this);
				}
				return expression;
			}

			CDataExactCardinalityExpression* CConcreteOntologyBuildDataUpdater::getDataExactCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				CDataExactCardinalityExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getDataExactCardinality(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstOptionalDataRangeTermExpression(),cardinality);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataExactCardinality'-Expression."),this);
				}
				return expression;
			}






			CObjectOneOfExpression* CConcreteOntologyBuildDataUpdater::getObjectOneOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectOneOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0)) {
					expression = getObjectOneOf(*expSplitter.getIndividualTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectOneOf'-Expression."),this);
				}
				return expression;
			}


			CObjectOneOfExpression* CConcreteOntologyBuildDataUpdater::getObjectOneOf(CBuildExpression* indiExpression) {
				CObjectOneOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(indiExpression);
				if (expSplitter.testForExpressionComposition(0,0,0)) {
					expression = getObjectOneOf(*expSplitter.getIndividualTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectOneOf'-Expression."),this);
				}
				return expression;
			}



			CObjectHasValueExpression* CConcreteOntologyBuildDataUpdater::getObjectHasValue(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectHasValueExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1,1)) {
					expression = getObjectHasValue(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstIndividualTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectHasValue'-Expression."),this);
				}
				return expression;
			}

			CObjectHasValueExpression* CConcreteOntologyBuildDataUpdater::getObjectHasValue(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectHasValueExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,1,1)) {
					expression = getObjectHasValue(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstIndividualTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectHasValue'-Expression."),this);
				}
				return expression;
			}



			CObjectHasSelfExpression* CConcreteOntologyBuildDataUpdater::getObjectHasSelf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectHasSelfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectHasSelf(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectHasSelf'-Expression."),this);
				}
				return expression;
			}

			CObjectHasSelfExpression* CConcreteOntologyBuildDataUpdater::getObjectHasSelf(CBuildExpression* selfExpression) {
				CObjectHasSelfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(selfExpression);
				if (expSplitter.testForExpressionComposition(0,1)) {
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
					if (expSplitter.testForExpressionComposition(0,2)) {
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
				if (!expression) {
					CExpressionSplitter expSplitter(expression1,expression2);
					if (expSplitter.testForExpressionComposition(0,2)) {
						CEXPRESSIONLIST<CObjectPropertyTermExpression*>* objectPropertyExpressionList = expSplitter.getObjectPropertyTermExpressionList();
						CObjectPropertyTermExpression* superObjectPropertyExpression = objectPropertyExpressionList->takeLast();
						expression = getSubObjectPropertyOf(*objectPropertyExpressionList,superObjectPropertyExpression);
					} else {
						LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SubObjectPropertyOf'-Expression."),this);
					}
				}
				return expression;
			}

			CObjectPropertyChainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyChain(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectPropertyChainExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectPropertyChain(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyChain'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyChainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyChain(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectPropertyChainExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getObjectPropertyChain(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyChain'-Expression."),this);
				}
				return expression;
			}

			CEquivalentObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CEquivalentObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,2)) {
					expression = getEquivalentObjectProperties(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'EquivalentObjectProperties'-Expression."),this);
				}
				return expression;
			}

			CEquivalentObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				CEquivalentObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,2)) {
					expression = getEquivalentObjectProperties(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'EquivalentObjectProperties'-Expression."),this);
				}
				return expression;
			}

			CDisjointObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getDisjointObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDisjointObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,2)) {
					expression = getDisjointObjectProperties(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DisjointObjectProperties'-Expression."),this);
				}
				return expression;
			}

			CDisjointObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getDisjointObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDisjointObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,2)) {
					expression = getDisjointObjectProperties(*expSplitter.getObjectPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DisjointObjectProperties'-Expression."),this);
				}
				return expression;
			}


			CObjectPropertyDomainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyDomainExpression(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectPropertyDomainExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(1,1)) {
					expression = getObjectPropertyDomainExpression(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyDomain'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyDomainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyDomainExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectPropertyDomainExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(1,1)) {
					expression = getObjectPropertyDomainExpression(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyDomain'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyRangeExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyRangeExpression(CBuildExpression* expression1, CBuildExpression* expression2) {
				CObjectPropertyRangeExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(1,1)) {
					expression = getObjectPropertyRangeExpression(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyRange'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyRangeExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyRangeExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectPropertyRangeExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(1,1)) {
					expression = getObjectPropertyRangeExpression(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyRange'-Expression."),this);
				}
				return expression;
			}










			CSubDataPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getSubDataPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CSubDataPropertyOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,2)) {
					expression = getSubDataPropertyOf(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getSecondDataPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SubDataPropertyOf'-Expression."),this);
				}
				return expression;
			}

			CSubDataPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getSubDataPropertyOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				CSubDataPropertyOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,0,2)) {
					expression = getSubDataPropertyOf(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getSecondDataPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SubDataPropertyOf'-Expression."),this);
				}
				return expression;
			}


			CEquivalentDataPropertiesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentDataProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CEquivalentDataPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,2)) {
					expression = getEquivalentDataProperties(*expSplitter.getDataPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'EquivalentDataProperties'-Expression."),this);
				}
				return expression;
			}

			CEquivalentDataPropertiesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentDataProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				CEquivalentDataPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,0,2)) {
					expression = getEquivalentDataProperties(*expSplitter.getDataPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'EquivalentDataProperties'-Expression."),this);
				}
				return expression;
			}

			CDisjointDataPropertiesExpression* CConcreteOntologyBuildDataUpdater::getDisjointDataProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDisjointDataPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,2)) {
					expression = getDisjointDataProperties(*expSplitter.getDataPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DisjointDataProperties'-Expression."),this);
				}
				return expression;
			}

			CDisjointDataPropertiesExpression* CConcreteOntologyBuildDataUpdater::getDisjointDataProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDisjointDataPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,0,2)) {
					expression = getDisjointDataProperties(*expSplitter.getDataPropertyTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DisjointDataProperties'-Expression."),this);
				}
				return expression;
			}


			CDataPropertyDomainExpression* CConcreteOntologyBuildDataUpdater::getDataPropertyDomainExpression(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDataPropertyDomainExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(1,0,0,0,1)) {
					expression = getDataPropertyDomainExpression(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataPropertyDomain'-Expression."),this);
				}
				return expression;
			}

			CDataPropertyDomainExpression* CConcreteOntologyBuildDataUpdater::getDataPropertyDomainExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataPropertyDomainExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(1,0,0,0,1)) {
					expression = getDataPropertyDomainExpression(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataPropertyDomain'-Expression."),this);
				}
				return expression;
			}

			CDataPropertyRangeExpression* CConcreteOntologyBuildDataUpdater::getDataPropertyRangeExpression(CBuildExpression* expression1, CBuildExpression* expression2) {
				CDataPropertyRangeExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,0,0,1,1)) {
					expression = getDataPropertyRangeExpression(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstDataRangeTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataPropertyRange'-Expression."),this);
				}
				return expression;
			}

			CDataPropertyRangeExpression* CConcreteOntologyBuildDataUpdater::getDataPropertyRangeExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataPropertyRangeExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,1,1)) {
					expression = getDataPropertyRangeExpression(expSplitter.getFirstDataPropertyTermExpression(),expSplitter.getFirstDataRangeTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataPropertyRange'-Expression."),this);
				}
				return expression;
			}


			CFunctionalDataPropertyExpression* CConcreteOntologyBuildDataUpdater::getFunctionalDataProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CFunctionalDataPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getFunctionalDataProperty(expSplitter.getFirstDataPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'FunctionalDataProperty'-Expression."),this);
				}
				return expression;
			}

			CFunctionalDataPropertyExpression* CConcreteOntologyBuildDataUpdater::getFunctionalDataProperty(CBuildExpression* funcExpression) {
				CFunctionalDataPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.testForExpressionComposition(0,0,0,0,1)) {
					expression = getFunctionalDataProperty(expSplitter.getFirstDataPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'FunctionalDataProperty'-Expression."),this);
				}
				return expression;
			}








			CInverseObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CInverseObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,2)) {
					expression = getInverseObjectProperties(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getSecondObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseObjectProperties'-Expression."),this);
				}
				return expression;
			}

			CInverseObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				CInverseObjectPropertiesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(0,2)) {
					expression = getInverseObjectProperties(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getSecondObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseObjectProperties'-Expression."),this);
				}
				return expression;
			}


			CTransetiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getTransetiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CTransetiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getTransetiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'TransetiveObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CTransetiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getTransetiveObjectProperty(CBuildExpression* transExpression) {
				CTransetiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(transExpression);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getTransetiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'TransetiveObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CFunctionalObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getFunctionalObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'FunctionalObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getFunctionalObjectProperty(CBuildExpression* funcExpression) {
				CFunctionalObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getFunctionalObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'FunctionalObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CInverseFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getInverseFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CInverseFunctionalObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getInverseFunctionalObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseFunctionalObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CInverseFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getInverseFunctionalObjectProperty(CBuildExpression* funcExpression) {
				CInverseFunctionalObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getInverseFunctionalObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseFunctionalObjectProperty'-Expression."),this);
				}
				return expression;
			}
			CSymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getSymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CSymmetricObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getSymmetricObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SymmetricObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CSymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getSymmetricObjectProperty(CBuildExpression* funcExpression) {
				CSymmetricObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getSymmetricObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SymmetricObjectProperty'-Expression."),this);
				}
				return expression;
			}


			CAsymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getAsymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CAsymmetricObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getAsymmetricObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'AsymmetricObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CAsymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getAsymmetricObjectProperty(CBuildExpression* funcExpression) {
				CAsymmetricObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getAsymmetricObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'AsymmetricObjectProperty'-Expression."),this);
				}
				return expression;
			}


			CReflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getReflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CReflexiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getReflexiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ReflexiveObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CReflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getReflexiveObjectProperty(CBuildExpression* funcExpression) {
				CReflexiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getReflexiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ReflexiveObjectProperty'-Expression."),this);
				}
				return expression;
			}


			CIrreflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getIrreflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CIrreflexiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getIrreflexiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'IrreflexiveObjectProperty'-Expression."),this);
				}
				return expression;
			}

			CIrreflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getIrreflexiveObjectProperty(CBuildExpression* funcExpression) {
				CIrreflexiveObjectPropertyExpression* expression = nullptr;
				CExpressionSplitter expSplitter(funcExpression);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getIrreflexiveObjectProperty(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'IrreflexiveObjectProperty'-Expression."),this);
				}
				return expression;
			}


			CInverseObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CInverseObjectPropertyOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getInverseObjectPropertyOf(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseObjectPropertyOf'-Expression."),this);
				}
				return expression;
			}

			CInverseObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectPropertyOf(CBuildExpression* invExpression) {
				CInverseObjectPropertyOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(invExpression);
				if (expSplitter.testForExpressionComposition(0,1)) {
					expression = getInverseObjectPropertyOf(expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'InverseObjectPropertyOf'-Expression."),this);
				}
				return expression;
			}



			CClassAssertionExpression* CConcreteOntologyBuildDataUpdater::getClassAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CClassAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(1,0,1)) {
					expression = getClassAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ClassAssertion'-Expression."),this);
				}
				return expression;
			}

			CClassAssertionExpression* CConcreteOntologyBuildDataUpdater::getClassAssertion(CBuildExpression* expression1, CBuildExpression* expression2) {
				CClassAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2);
				if (expSplitter.testForExpressionComposition(1,0,1)) {
					expression = getClassAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getFirstClassTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ClassAssertion'-Expression."),this);
				}
				return expression;
			}





			CObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CObjectPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1,2)) {
					expression = getObjectPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getSecondIndividualTermExpression(),expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyAssertion'-Expression."),this);
				}
				return expression;
			}

			CObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				CObjectPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2,expression3);
				if (expSplitter.testForExpressionComposition(0,1,2)) {
					expression = getObjectPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getSecondIndividualTermExpression(),expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'ObjectPropertyAssertion'-Expression."),this);
				}
				return expression;
			}


			CNegativeObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getNegativeObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CNegativeObjectPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,1,2)) {
					expression = getNegativeObjectPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getSecondIndividualTermExpression(),expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'NegativeObjectPropertyAssertion'-Expression."),this);
				}
				return expression;
			}

			CNegativeObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getNegativeObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				CNegativeObjectPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2,expression3);
				if (expSplitter.testForExpressionComposition(0,1,2)) {
					expression = getNegativeObjectPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getSecondIndividualTermExpression(),expSplitter.getFirstObjectPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'NegativeObjectPropertyAssertion'-Expression."),this);
				}
				return expression;
			}




			CDataPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getDataPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDataPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,1,0,1,1)) {
					expression = getDataPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getFirstDataLiteralTermExpression(),expSplitter.getFirstDataPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataPropertyAssertion'-Expression."),this);
				}
				return expression;
			}

			CDataPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getDataPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				CDataPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2,expression3);
				if (expSplitter.testForExpressionComposition(0,0,1,0,1,1)) {
					expression = getDataPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getFirstDataLiteralTermExpression(),expSplitter.getFirstDataPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DataPropertyAssertion'-Expression."),this);
				}
				return expression;
			}

			CNegativeDataPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getNegativeDataPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CNegativeDataPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,1,0,1,1)) {
					expression = getNegativeDataPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getFirstDataLiteralTermExpression(),expSplitter.getFirstDataPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'NegativeDataPropertyAssertion'-Expression."),this);
				}
				return expression;
			}

			CNegativeDataPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getNegativeDataPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				CNegativeDataPropertyAssertionExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expression1,expression2,expression3);
				if (expSplitter.testForExpressionComposition(0,0,1,0,1,1)) {
					expression = getNegativeDataPropertyAssertion(expSplitter.getFirstIndividualTermExpression(),expSplitter.getFirstDataLiteralTermExpression(),expSplitter.getFirstDataPropertyTermExpression());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'NegativeDataPropertyAssertion'-Expression."),this);
				}
				return expression;
			}








			CSameIndividualExpression* CConcreteOntologyBuildDataUpdater::getSameIndividual(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CSameIndividualExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,2)) {
					expression = getSameIndividual(*expSplitter.getIndividualTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'SameIndividual'-Expression."),this);
				}
				return expression;
			}


			CDifferentIndividualsExpression* CConcreteOntologyBuildDataUpdater::getDifferentIndividuals(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				CDifferentIndividualsExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				if (expSplitter.testForExpressionComposition(0,0,2)) {
					expression = getDifferentIndividuals(*expSplitter.getIndividualTermExpressionList());
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("Couldn't match parameters for 'DifferentIndividual'-Expression."),this);
				}
				return expression;
			}



			CConcreteOntologyBuildDataUpdater* CConcreteOntologyBuildDataUpdater::addHashedExpression(CBuildExpression* expression) {
				mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
				mExpressionBuildContainerList->append(expression);
				return this;
			}


			CBuildExpression* CConcreteOntologyBuildDataUpdater::getHashedExpression(CBuildExpression* expression) {
				CBuildExpression* hashExpression = nullptr;
				hashExpression = mExpressionBuildHash->value(CExpressionHasher(expression),nullptr);
				return hashExpression;
			}

			CDeclarationAxiomExpression* CConcreteOntologyBuildDataUpdater::getDeclaration(CExpressionEntity* entity) {
				CDeclarationAxiomExpression* expression = nullptr;
				CDeclarationAxiomExpression tmpExpression(entity);
				expression = (CDeclarationAxiomExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDeclarationAxiomExpression(entity);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDECLARATION);
				}
				return expression;
			}



			CEquivalentClassesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				CEquivalentClassesExpression* expression = nullptr;
				CEquivalentClassesExpression tmpExpression(expressions);
				expression = (CEquivalentClassesExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CEquivalentClassesExpression(expressions);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETEQUIVALENTCLASSES);
				}
				return expression;
			}

			CSubClassOfExpression* CConcreteOntologyBuildDataUpdater::getSubClassOf(CClassTermExpression* expression1, CClassTermExpression* expression2) {
				CSubClassOfExpression* expression = nullptr;
				CSubClassOfExpression tmpExpression(expression1,expression2);
				expression = (CSubClassOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CSubClassOfExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETESUBCLASSOF);
				}
				return expression;
			}

			CDisjointClassesExpression* CConcreteOntologyBuildDataUpdater::getDisjointClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				CDisjointClassesExpression* expression = nullptr;
				CDisjointClassesExpression tmpExpression(expressions);
				expression = (CDisjointClassesExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDisjointClassesExpression(expressions);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDISJOINTCLASSES);
				}
				return expression;
			}


			CDisjointUnionExpression* CConcreteOntologyBuildDataUpdater::getDisjointUnion(CClassExpression* classExpression, const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				CDisjointUnionExpression* expression = nullptr;
				CDisjointUnionExpression tmpExpression(classExpression,expressions);
				expression = (CDisjointUnionExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDisjointUnionExpression(classExpression,expressions);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDISJOINTUNION);
				}
				return expression;
			}


			CObjectComplementOfExpression* CConcreteOntologyBuildDataUpdater::getObjectComplementOf(CClassTermExpression* compExpression) {
				CObjectComplementOfExpression* expression = nullptr;
				CObjectComplementOfExpression tmpExpression(compExpression);
				expression = (CObjectComplementOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectComplementOfExpression(compExpression);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTCOMPLEMENTOF);
				}
				return expression;
			}

			CObjectIntersectionOfExpression* CConcreteOntologyBuildDataUpdater::getObjectIntersectionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				CObjectIntersectionOfExpression* expression = nullptr;

				CObjectIntersectionOfExpression tmpExpression(expressions);
				expression = (CObjectIntersectionOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectIntersectionOfExpression(expressions);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTINTERSECTION);
				}
				return expression;
			}

			CObjectUnionOfExpression* CConcreteOntologyBuildDataUpdater::getObjectUnionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				CObjectUnionOfExpression* expression = nullptr;

				CObjectUnionOfExpression tmpExpression(expressions);
				expression = (CObjectUnionOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectUnionOfExpression(expressions);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTUNION);
				}
				return expression;
			}


			CObjectMaxCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMaxCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) {
				CObjectMaxCardinalityExpression* expression = nullptr;
				if (expression2 == nullptr) {
					expression2 = mTopClassExpression;
				}
				CObjectMaxCardinalityExpression tmpExpression(expression1,expression2,cardinality);
				expression = (CObjectMaxCardinalityExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectMaxCardinalityExpression(expression1,expression2,cardinality);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTMAXCARDINALITY);
					mBuildExpCounter->setMaxObjectCardinalityMinimumMaximumCandidate(cardinality);
				}
				return expression;
			}

			CObjectMinCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectMinCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) {
				CObjectMinCardinalityExpression* expression = nullptr;
				if (expression2 == nullptr) {
					expression2 = mTopClassExpression;
				}
				CObjectMinCardinalityExpression tmpExpression(expression1,expression2,cardinality);
				expression = (CObjectMinCardinalityExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectMinCardinalityExpression(expression1,expression2,cardinality);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTMINCARDINALITY);
					mBuildExpCounter->setMinObjectCardinalityMinimumMaximumCandidate(cardinality);
				}
				return expression;
			}

			CObjectExactlyCardinalityExpression* CConcreteOntologyBuildDataUpdater::getObjectExactlyCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) {
				CObjectExactlyCardinalityExpression* expression = nullptr;
				if (expression2 == nullptr) {
					expression2 = mTopClassExpression;
				}
				CObjectExactlyCardinalityExpression tmpExpression(expression1,expression2,cardinality);
				expression = (CObjectExactlyCardinalityExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectExactlyCardinalityExpression(expression1,expression2,cardinality);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTEXACTLYCARDINALITY);
					mBuildExpCounter->setMaxObjectCardinalityMinimumMaximumCandidate(cardinality);
					mBuildExpCounter->setMinObjectCardinalityMinimumMaximumCandidate(cardinality);
				}
				return expression;
			}

			CObjectAllValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectAllValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				CObjectAllValuesFromExpression* expression = nullptr;
				CObjectAllValuesFromExpression tmpExpression(expression1,expression2);
				expression = (CObjectAllValuesFromExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectAllValuesFromExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTALLVALUEFROM);
				}
				return expression;
			}

			CObjectOneOfExpression* CConcreteOntologyBuildDataUpdater::getObjectOneOf(const CEXPRESSIONLIST<CIndividualTermExpression*> &expressions) {
				CObjectOneOfExpression* expression = nullptr;
				CObjectOneOfExpression tmpExpression(expressions);
				expression = (CObjectOneOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectOneOfExpression(expressions);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTONEOF);
				}
				return expression;
			}


			CObjectOneOfExpression* CConcreteOntologyBuildDataUpdater::getObjectOneOf(CIndividualTermExpression* indiExpression) {
				if (!mTmpObjectOneOfExpression) {
					mTmpObjectOneOfExpression = new CObjectOneOfExpression(indiExpression);
				} else {
					mTmpObjectOneOfExpression->initObjectOneOfExpression(indiExpression);
				}
				CBuildExpression* expression = getHashedExpression(mTmpObjectOneOfExpression);
				if (!expression) {
					addHashedExpression(mTmpObjectOneOfExpression);
					mBuildConceptSet->insert(mTmpObjectOneOfExpression);
					mBuildConceptList->append(mTmpObjectOneOfExpression);
					expression = mTmpObjectOneOfExpression;
					mTmpObjectOneOfExpression = nullptr;
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTONEOF);
				}
				return (CObjectOneOfExpression*)expression;
			}





			CDataLiteralExpression* CConcreteOntologyBuildDataUpdater::getDataLiteral(CDataLexicalValueExpression* dataLexicalValue, CDatatypeExpression* datatype) {
				CDataLiteralExpression* expression = nullptr;
				CDataLiteralExpression tmpExpression(dataLexicalValue,datatype);
				expression = (CDataLiteralExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataLiteralExpression(dataLexicalValue,datatype);
					addHashedExpression(expression);
					mBuildDataRangeSet->insert(expression);
					mBuildDataRangeList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATALITERAL);
				}
				return expression;
			}





			CDataSomeValuesFromExpression* CConcreteOntologyBuildDataUpdater::getDataSomeValuesFrom(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2) {
				CDataSomeValuesFromExpression* expression = nullptr;
				CDataSomeValuesFromExpression tmpExpression(expression1,expression2);
				expression = (CDataSomeValuesFromExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataSomeValuesFromExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATASOMEVALUEFROM);
				}
				return expression;
			}



			CDataHasValueExpression* CConcreteOntologyBuildDataUpdater::getDataHasValue(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2) {
				CDataHasValueExpression* expression = nullptr;
				CDataHasValueExpression tmpExpression(expression1,expression2);
				expression = (CDataHasValueExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataHasValueExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAHASVALUE);
				}
				return expression;
			}


			CDataAllValuesFromExpression* CConcreteOntologyBuildDataUpdater::getDataAllValuesFrom(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2) {
				CDataAllValuesFromExpression* expression = nullptr;
				CDataAllValuesFromExpression tmpExpression(expression1,expression2);
				expression = (CDataAllValuesFromExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataAllValuesFromExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAALLVALUEFROM);
				}
				return expression;
			}



			CDataMaxCardinalityExpression* CConcreteOntologyBuildDataUpdater::getDataMaxCardinality(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2, int cardinality) {
				CDataMaxCardinalityExpression* expression = nullptr;
				if (expression2 == nullptr) {
					expression2 = mTopDataRangeExpression;
				}
				CDataMaxCardinalityExpression tmpExpression(expression1,expression2,cardinality);
				expression = (CDataMaxCardinalityExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataMaxCardinalityExpression(expression1,expression2,cardinality);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAMAXCARDINALITY);
					mBuildExpCounter->setMaxDataCardinalityMinimumMaximumCandidate(cardinality);
				}
				return expression;
			}


			CDataMinCardinalityExpression* CConcreteOntologyBuildDataUpdater::getDataMinCardinality(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2, int cardinality) {
				CDataMinCardinalityExpression* expression = nullptr;
				if (expression2 == nullptr) {
					expression2 = mTopDataRangeExpression;
				}
				CDataMinCardinalityExpression tmpExpression(expression1,expression2,cardinality);
				expression = (CDataMinCardinalityExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataMinCardinalityExpression(expression1,expression2,cardinality);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAMINCARDINALITY);
					mBuildExpCounter->setMinDataCardinalityMinimumMaximumCandidate(cardinality);
				}
				return expression;
			}




			CDataExactCardinalityExpression* CConcreteOntologyBuildDataUpdater::getDataExactCardinality(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2, int cardinality) {
				CDataExactCardinalityExpression* expression = nullptr;
				if (expression2 == nullptr) {
					expression2 = mTopDataRangeExpression;
				}
				CDataExactCardinalityExpression tmpExpression(expression1,expression2,cardinality);
				expression = (CDataExactCardinalityExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataExactCardinalityExpression(expression1,expression2,cardinality);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAEXACTCARDINALITY);
					mBuildExpCounter->setMaxDataCardinalityMinimumMaximumCandidate(cardinality);
					mBuildExpCounter->setMinDataCardinalityMinimumMaximumCandidate(cardinality);
				}
				return expression;
			}



			CDataFacetRestrictionExpression* CConcreteOntologyBuildDataUpdater::getDataFacetRestriction(CDataLiteralExpression* dataLiteralExpression, CDataFacetExpression* dataFacet) {
				CDataFacetRestrictionExpression* expression = nullptr;
				CDataFacetRestrictionExpression tmpExpression(dataLiteralExpression,dataFacet);
				expression = (CDataFacetRestrictionExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataFacetRestrictionExpression(dataLiteralExpression,dataFacet);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATATYPERESTRICTION);
				}
				return expression;
			}






			CDataIntersectionOfExpression* CConcreteOntologyBuildDataUpdater::getDataIntersectionOf(const CEXPRESSIONLIST<CDataRangeTermExpression*>& expressions) {
				CDataIntersectionOfExpression* expression = nullptr;
				CDataIntersectionOfExpression tmpExpression(expressions);
				expression = (CDataIntersectionOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataIntersectionOfExpression(expressions);
					addHashedExpression(expression);
					mBuildDataRangeSet->insert(expression);
					mBuildDataRangeList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAINTERSECTIONOF);
				}
				return expression;
			}

			CDataOneOfExpression* CConcreteOntologyBuildDataUpdater::getDataOneOf(const CEXPRESSIONLIST<CDataRangeTermExpression*>& expressions) {
				CDataOneOfExpression* expression = nullptr;
				CDataOneOfExpression tmpExpression(expressions);
				expression = (CDataOneOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataOneOfExpression(expressions);
					addHashedExpression(expression);
					mBuildDataRangeSet->insert(expression);
					mBuildDataRangeList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAONEOF);
				}
				return expression;
			}

			CDataUnionOfExpression* CConcreteOntologyBuildDataUpdater::getDataUnionOf(const CEXPRESSIONLIST<CDataRangeTermExpression*>& expressions) {
				CDataUnionOfExpression* expression = nullptr;
				CDataUnionOfExpression tmpExpression(expressions);
				expression = (CDataUnionOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataUnionOfExpression(expressions);
					addHashedExpression(expression);
					mBuildDataRangeSet->insert(expression);
					mBuildDataRangeList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAUNIONOF);
				}
				return expression;
			}

			CDataComplementOfExpression* CConcreteOntologyBuildDataUpdater::getDataComplementOf(CDataRangeTermExpression* expression1) {
				CDataComplementOfExpression* expression = nullptr;
				CDataComplementOfExpression tmpExpression(expression1);
				expression = (CDataComplementOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataComplementOfExpression(expression1);
					addHashedExpression(expression);
					mBuildDataRangeSet->insert(expression);
					mBuildDataRangeList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATACOMPLEMENTOF);
				}
				return expression;
			}

			CDatatypeRestrictionExpression* CConcreteOntologyBuildDataUpdater::getDatatypeRestriction(CDatatypeExpression* datatypeExpression, const CEXPRESSIONLIST<CDataFacetRestrictionExpression*>& expressions) {
				CDatatypeRestrictionExpression* expression = nullptr;
				CDatatypeRestrictionExpression tmpExpression(datatypeExpression,expressions);
				expression = (CDatatypeRestrictionExpression*)getHashedExpression(&tmpExpression);;
				if (!expression) {
					expression = new CDatatypeRestrictionExpression(datatypeExpression,expressions);
					addHashedExpression(expression);
					mBuildDataRangeSet->insert(expression);
					mBuildDataRangeList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAFACETRESTRICION);
				}
				return expression;
			}



















			CObjectSomeValuesFromExpression* CConcreteOntologyBuildDataUpdater::getObjectSomeValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				CObjectSomeValuesFromExpression* expression = nullptr;
				CObjectSomeValuesFromExpression tmpExpression(expression1,expression2);
				expression = (CObjectSomeValuesFromExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectSomeValuesFromExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTSOMEVALUEFROM);
				}
				return expression;
			}

			CObjectHasValueExpression* CConcreteOntologyBuildDataUpdater::getObjectHasValue(CObjectPropertyTermExpression* expression1, CIndividualTermExpression* expression2) {
				CObjectHasValueExpression* expression = nullptr;
				CObjectHasValueExpression tmpExpression(expression1,expression2);
				expression = (CObjectHasValueExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectHasValueExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTHASVALUE);
				}
				return expression;
			}

			CObjectHasSelfExpression* CConcreteOntologyBuildDataUpdater::getObjectHasSelf(CObjectPropertyTermExpression* obPrExpression) {
				CObjectHasSelfExpression* expression = nullptr;
				CObjectHasSelfExpression tmpExpression(obPrExpression);
				expression = (CObjectHasSelfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectHasSelfExpression(obPrExpression);
					addHashedExpression(expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTHASSELF);
				}
				return expression;
			}


			CSubObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getSubObjectPropertyOf(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& subExpressions, CObjectPropertyTermExpression* superExpression) {
				CSubObjectPropertyOfExpression* expression = nullptr;
				CSubObjectPropertyOfExpression tmpExpression(subExpressions,superExpression);
				expression = (CSubObjectPropertyOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CSubObjectPropertyOfExpression(subExpressions,superExpression);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETSUBOBJECTPROPERTYOF);
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
				expression = (CObjectPropertyChainExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectPropertyChainExpression(expressions);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTPROPERTYCHAIN);
				}
				return expression;
			}

			CEquivalentObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) {
				CEquivalentObjectPropertiesExpression* expression = nullptr;
				CEquivalentObjectPropertiesExpression tmpExpression(expressions);
				expression = (CEquivalentObjectPropertiesExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CEquivalentObjectPropertiesExpression(expressions);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETEQUIVALENTOBJECTPROPERTIES);
				}
				return expression;
			}

			CDisjointObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getDisjointObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) {
				CDisjointObjectPropertiesExpression* expression = nullptr;
				CDisjointObjectPropertiesExpression tmpExpression(expressions);
				expression = (CDisjointObjectPropertiesExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDisjointObjectPropertiesExpression(expressions);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDISJOINTOBJECTPROPERTIES);
				}
				return expression;
			}


			CObjectPropertyDomainExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyDomainExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				CObjectPropertyDomainExpression* expression = nullptr;
				CObjectPropertyDomainExpression tmpExpression(expression1,expression2);
				expression = (CObjectPropertyDomainExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectPropertyDomainExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTPROPERTYDOMAIN);
				}
				return expression;
			}

			CObjectPropertyRangeExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyRangeExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				CObjectPropertyRangeExpression* expression = nullptr;
				CObjectPropertyRangeExpression tmpExpression(expression1,expression2);
				expression = (CObjectPropertyRangeExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectPropertyRangeExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTPROPERTYRANGE);
				}
				return expression;
			}








			CSubDataPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getSubDataPropertyOf(CDataPropertyTermExpression* subExpression, CDataPropertyTermExpression* superExpression) {
				CSubDataPropertyOfExpression* expression = nullptr;
				CSubDataPropertyOfExpression tmpExpression(subExpression,superExpression);
				expression = (CSubDataPropertyOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CSubDataPropertyOfExpression(subExpression,superExpression);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETSUBDATAPROPERTYOF);
				}
				return expression;
			}

			CEquivalentDataPropertiesExpression* CConcreteOntologyBuildDataUpdater::getEquivalentDataProperties(const CEXPRESSIONLIST<CDataPropertyTermExpression*>& expressions) {
				CEquivalentDataPropertiesExpression* expression = nullptr;
				CEquivalentDataPropertiesExpression tmpExpression(expressions);
				expression = (CEquivalentDataPropertiesExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CEquivalentDataPropertiesExpression(expressions);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETEQUIVALENTDATAPROPERTIES);
				}
				return expression;
			}

			CDisjointDataPropertiesExpression* CConcreteOntologyBuildDataUpdater::getDisjointDataProperties(const CEXPRESSIONLIST<CDataPropertyTermExpression*>& expressions) {
				CDisjointDataPropertiesExpression* expression = nullptr;
				CDisjointDataPropertiesExpression tmpExpression(expressions);
				expression = (CDisjointDataPropertiesExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDisjointDataPropertiesExpression(expressions);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDISJOINTDATAPROPERTIES);
				}
				return expression;
			}


			CDataPropertyDomainExpression* CConcreteOntologyBuildDataUpdater::getDataPropertyDomainExpression(CDataPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				CDataPropertyDomainExpression* expression = nullptr;
				CDataPropertyDomainExpression tmpExpression(expression1,expression2);
				expression = (CDataPropertyDomainExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataPropertyDomainExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAPROPERTYDOMAIN);
				}
				return expression;
			}

			CDataPropertyRangeExpression* CConcreteOntologyBuildDataUpdater::getDataPropertyRangeExpression(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2) {
				CDataPropertyRangeExpression* expression = nullptr;
				CDataPropertyRangeExpression tmpExpression(expression1,expression2);
				expression = (CDataPropertyRangeExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataPropertyRangeExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAPROPERTYRANGE);
				}
				return expression;
			}

			CFunctionalDataPropertyExpression* CConcreteOntologyBuildDataUpdater::getFunctionalDataProperty(CDataPropertyTermExpression* buildExpression) {
				CFunctionalDataPropertyExpression* expression = nullptr;
				CFunctionalDataPropertyExpression tmpExpression(buildExpression);
				expression = (CFunctionalDataPropertyExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CFunctionalDataPropertyExpression(buildExpression);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETFUNCTIONALPROPERTYOF);
				}
				return expression;
			}








			CInverseObjectPropertiesExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectProperties(CObjectPropertyTermExpression* expression1, CObjectPropertyTermExpression* expression2) {
				CInverseObjectPropertiesExpression* expression = nullptr;
				CInverseObjectPropertiesExpression tmpExpression(expression1,expression2);
				expression = (CInverseObjectPropertiesExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CInverseObjectPropertiesExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETINVERSEOBJECTPROPERTIES);
				}
				return expression;
			}

			CTransetiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getTransetiveObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CTransetiveObjectPropertyExpression* expression = nullptr;
				CTransetiveObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CTransetiveObjectPropertyExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CTransetiveObjectPropertyExpression(buildExpression);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETTRANSETIVEPROPERTYOF);
				}
				return expression;
			}

			CFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getFunctionalObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CFunctionalObjectPropertyExpression* expression = nullptr;
				CFunctionalObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CFunctionalObjectPropertyExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CFunctionalObjectPropertyExpression(buildExpression);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETFUNCTIONALOBJECTPROPERTY);
				}
				return expression;
			}


			CInverseFunctionalObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getInverseFunctionalObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CInverseFunctionalObjectPropertyExpression* expression = nullptr;
				CInverseFunctionalObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CInverseFunctionalObjectPropertyExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CInverseFunctionalObjectPropertyExpression(buildExpression);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETINVERSEFUNCTIONALPROPERTY);
				}
				return expression;
			}

			CSymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getSymmetricObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CSymmetricObjectPropertyExpression* expression = nullptr;
				CSymmetricObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CSymmetricObjectPropertyExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CSymmetricObjectPropertyExpression(buildExpression);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETSYMMETRICPROPERTY);
				}
				return expression;
			}

			CAsymmetricObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getAsymmetricObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CAsymmetricObjectPropertyExpression* expression = nullptr;
				CAsymmetricObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CAsymmetricObjectPropertyExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CAsymmetricObjectPropertyExpression(buildExpression);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETASYMMETRICPROPERTY);
				}
				return expression;
			}

			CReflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getReflexiveObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CReflexiveObjectPropertyExpression* expression = nullptr;
				CReflexiveObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CReflexiveObjectPropertyExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CReflexiveObjectPropertyExpression(buildExpression);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETREFLEXIVEPROPERTY);
				}
				return expression;
			}


			CIrreflexiveObjectPropertyExpression* CConcreteOntologyBuildDataUpdater::getIrreflexiveObjectProperty(CObjectPropertyTermExpression* buildExpression) {
				CIrreflexiveObjectPropertyExpression* expression = nullptr;
				CIrreflexiveObjectPropertyExpression tmpExpression(buildExpression);
				expression = (CIrreflexiveObjectPropertyExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CIrreflexiveObjectPropertyExpression(buildExpression);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETIRREFLEXIVEPROPERTY);
				}
				return expression;
			}

			CInverseObjectPropertyOfExpression* CConcreteOntologyBuildDataUpdater::getInverseObjectPropertyOf(CObjectPropertyTermExpression* buildExpression) {
				CInverseObjectPropertyOfExpression* expression = nullptr;
				CInverseObjectPropertyOfExpression tmpExpression(buildExpression);
				expression = (CInverseObjectPropertyOfExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CInverseObjectPropertyOfExpression(buildExpression);
					addHashedExpression(expression);
					mBuildObjectRoleSet->insert(expression);
					mBuildObjectRoleList->append(expression);
					mInverseObjectPropertyHash->insert(buildExpression,expression);
					mInverseObjectPropertyHash->insert(expression,buildExpression);
					mInverseObjectPropertyList->append( QPair<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>(expression,buildExpression) );
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETINVERSEOBJECTPROPERTYOF);
				}
				return expression;
			}


			CObjectPropertyTermExpression* CConcreteOntologyBuildDataUpdater::getCorrectedInverseObjectPropertyOf(CObjectPropertyTermExpression* invExpression) {
				CInverseObjectPropertyOfExpression* invCastExpression = dynamic_cast<CInverseObjectPropertyOfExpression*>(invExpression);
				if (invCastExpression) {
					return invCastExpression->getInverseOfExpression();
				}
				CObjectPropertyTermExpression* inverseObjPropTermExp = mInverseObjectPropertyHash->value(invExpression, nullptr);
				if (inverseObjPropTermExp) {
					return inverseObjPropTermExp;
				}
				CInverseObjectPropertyOfExpression* expression = nullptr;
				expression = getInverseObjectPropertyOf(invExpression);
				return expression;
			}



			CClassAssertionExpression* CConcreteOntologyBuildDataUpdater::getClassAssertion(CIndividualTermExpression* expression1, CClassTermExpression* expression2) {
				CClassAssertionExpression* expression = nullptr;
				CClassAssertionExpression tmpExpression(expression1,expression2);
				expression = (CClassAssertionExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CClassAssertionExpression(expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETCLASSASSERTION);
				}
				return expression;
			}

			CObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3) {
				CObjectPropertyAssertionExpression* expression = nullptr;
				CObjectPropertyAssertionExpression tmpExpression(expression3,expression1,expression2);
				expression = (CObjectPropertyAssertionExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CObjectPropertyAssertionExpression(expression3,expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTPROPERTYASSERTION);
				}
				return expression;
			}

			CNegativeObjectPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getNegativeObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3) {
				CNegativeObjectPropertyAssertionExpression* expression = nullptr;
				CNegativeObjectPropertyAssertionExpression tmpExpression(expression3,expression1,expression2);
				expression = (CNegativeObjectPropertyAssertionExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CNegativeObjectPropertyAssertionExpression(expression3,expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETNEGATIVEOBJECTPROPERTYASSERTION);
				}
				return expression;
			}

			CSameIndividualExpression* CConcreteOntologyBuildDataUpdater::getSameIndividual(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions) {
				CSameIndividualExpression* expression = nullptr;
				CSameIndividualExpression tmpExpression(expressions);
				expression = (CSameIndividualExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CSameIndividualExpression(expressions);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETSAMEINDIVIDUAL);
				}
				return expression;
			}

			CDifferentIndividualsExpression* CConcreteOntologyBuildDataUpdater::getDifferentIndividuals(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions) {
				CDifferentIndividualsExpression* expression = nullptr;
				CDifferentIndividualsExpression tmpExpression(expressions);
				expression = (CDifferentIndividualsExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDifferentIndividualsExpression(expressions);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDIFFERENTINDIVIDUAL);
				}
				return expression;
			}



			CDataPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getDataPropertyAssertion(CIndividualTermExpression* expression1, CDataLiteralTermExpression* expression2, CDataPropertyTermExpression* expression3) {
				CDataPropertyAssertionExpression* expression = nullptr;
				CDataPropertyAssertionExpression tmpExpression(expression3,expression1,expression2);
				expression = (CDataPropertyAssertionExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CDataPropertyAssertionExpression(expression3,expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAPROPERTYASSERTION);
				}
				return expression;
			}

			CNegativeDataPropertyAssertionExpression* CConcreteOntologyBuildDataUpdater::getNegativeDataPropertyAssertion(CIndividualTermExpression* expression1, CDataLiteralTermExpression* expression2, CDataPropertyTermExpression* expression3) {
				CNegativeDataPropertyAssertionExpression* expression = nullptr;
				CNegativeDataPropertyAssertionExpression tmpExpression(expression3,expression1,expression2);
				expression = (CNegativeDataPropertyAssertionExpression*)getHashedExpression(&tmpExpression);
				if (!expression) {
					expression = new CNegativeDataPropertyAssertionExpression(expression3,expression1,expression2);
					addHashedExpression(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETNEGATIVEDATAPROPERTYASSERTION);
				}
				return expression;
			}





			bool CConcreteOntologyBuildDataUpdater::addTriplesData(CTriplesData* tripleData) {
				mOntologyTriplesData->addTriplesData(tripleData);
				return true;
			}

			CTriplesData* CConcreteOntologyBuildDataUpdater::getLatestTriplesData(bool onlyLocal, bool* localFlag) {
				CTriplesData* tripleData = mOntologyTriplesData->getLatestTriplesData(onlyLocal);
				if (localFlag && tripleData) {
					if (mOntologyTriplesData->getLatestTriplesData(true)) {
						*localFlag = true;
					} else {
						*localFlag = false;
					}
				}
				return tripleData;
			}





			bool CConcreteOntologyBuildDataUpdater::initializeBuilding() {

				mTmpObjectOneOfExpression = nullptr;


				mMemManager = CContext::getMemoryAllocationManager(mOnto->getOntologyContext());

				mOntoStrings = mOnto->getStringMapping();
				mOntoBuild = mOnto->getBuildData();

				mBuildExpCounter = mOntoBuild->getBuildExpressionCounter();

				mDeclarationAxiomSet = mOntoBuild->getDeclarationAxiomSet();

				mTopClassExpression = mOntoBuild->getTopClassExpression();
				mBottomClassExpression = mOntoBuild->getBottomClassExpression();
				mTopObjPropExpression = mOntoBuild->getTopObjectPropertyExpression();
				mBottomObjPropExpression = mOntoBuild->getBottomObjectPropertyExpression();

				mTopDataRangeExpression = mOntoBuild->getTopDataRangeExpression();
				mBottomDataRangeExpression = mOntoBuild->getBottomDataRangeExpression();

				mTopDataPropExpression = mOntoBuild->getTopDataPropertyExpression();
				mBottomDataPropExpression = mOntoBuild->getBottomDataPropertyExpression();

				mBuildConceptSet = mOntoBuild->getBuildConceptSet();
				mBuildObjectRoleSet = mOntoBuild->getBuildObjectRoleSet();
				mBuildIndividualSet = mOntoBuild->getBuildIndividualSet();
				mBuildDataRoleSet = mOntoBuild->getBuildDataRoleSet();
				mBuildDatatypeSet = mOntoBuild->getBuildDatatypeSet();
				mBuildDataRangeSet = mOntoBuild->getBuildDataRangeSet();

				mBuildConceptList = mOntoBuild->getBuildConceptList();
				mBuildObjectRoleList = mOntoBuild->getBuildObjectRoleList();
				mBuildIndividualList = mOntoBuild->getBuildIndividualList();
				mBuildDataRoleList = mOntoBuild->getBuildDataRoleList();
				mBuildDataRangeList = mOntoBuild->getBuildDataRangeList();
				mBuildDatatypeList = mOntoBuild->getBuildDatatypeList();

				mExpressionBuildContainerList = mOntoBuild->getExpressionBuildListContainer();
				mInverseObjectPropertyHash = mOntoBuild->getInverseObjectPropertyHash();
				mInverseObjectPropertyList = mOntoBuild->getInverseObjectPropertyList();

				mExpressionBuildHash = mOntoBuild->getStructuralExpressionBuildHash();

				mClassBuildHash = mOntoBuild->getClassEntityBuildHash();
				mObjectPropertyBuildHash = mOntoBuild->getObjectPropertyEntityBuildHash();
				mIndividualBuildHash = mOntoBuild->getIndividualEntityBuildHash();
				mAnoIndividualBuildHash = mOntoBuild->getAnonymousIndividualBuildHash();
				mNominalIndividualVariableBuildHash = mOntoBuild->getNominalIndividualVariableBuildHash();

				mAnonymousIndividualVariableBuildHash = mOntoBuild->getAnonymousIndividualVariableBuildHash();
				mNamedIndividualVariableBuildHash = mOntoBuild->getNamedIndividualVariableBuildHash();

				mDataValueVariableBuildHash = mOntoBuild->getDataValueVariableBuildHash();
				mDataLiteralVariableBuildHash = mOntoBuild->getDataLiteralVariableBuildHash();

				mDataPropertyBuildHash = mOntoBuild->getDataPropertyEntityBuildHash();
				mDataLexicalValueBuildHash = mOntoBuild->getDataLexicalValueBuildHash();

				mNextAxiomNumber = mOntoBuild->getNextAxiomNumber();
				mNextEntityNumber = mOntoBuild->getNextEntityNumber();
				mNextMaxAxiomNumberOffset = 0;


				mAbbreviatedNamePrefixMapHash = mOntoStrings->getAbbreviatedNamePrefixHash();
				mNamePrefixMapHash = mOntoStrings->getNamePrefixHash();
				mPrefixList = mOntoStrings->getPrefixList();

				mIndividualVariableIDHash = mOntoBuild->getIndividualVariableIDHash();

				mDatatypeBuildHash = mOntoBuild->getDatatypeIRIBuildHash();
				mDataFacetBuildHash = mOntoBuild->getFacetIRIBuildHash();

				mImportDataHash = mOntoBuild->getImportDataHash();

				mOntologyTriplesData = mOnto->getOntologyTriplesData();
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

			CDataRangeTermExpression* CConcreteOntologyBuildDataUpdater::getTopDataRange() {
				return mTopDataRangeExpression;
			}

			CDataRangeTermExpression* CConcreteOntologyBuildDataUpdater::getBottomDataRange() {
				return mBottomDataRangeExpression;
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




			bool CConcreteOntologyBuildDataUpdater::addOntologyImport(const QStringRef& name) {
				if (!mImportDataHash->contains(CStringRefStringHasher(name))) {
					QString importIRIString = name.toString();
					COntologyImportData*& importData = (*mImportDataHash)[CStringRefStringHasher(importIRIString)];
					importData = new COntologyImportData();
					importData->initOntologyImportData(importIRIString);
					return true;
				}
				return false;
			}

			bool CConcreteOntologyBuildDataUpdater::addOntologyImport(const QString& name) {
				COntologyImportData*& importData = (*mImportDataHash)[CStringRefStringHasher(name)];
				if (!importData) {
					importData = new COntologyImportData();
					importData->initOntologyImportData(name);
					return true;
				}
				return false;
			}





			bool CConcreteOntologyBuildDataUpdater::addNameAbbreviation(const QString &name, const QString &abbreviatedName) {
				CNamePrefix* previousNamePrefix = mAbbreviatedNamePrefixMapHash->value(abbreviatedName);
				if (!previousNamePrefix || previousNamePrefix->getPrefixString() != name) {
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
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETCLASS);
				} else {
					if (expression == mBottomClassExpression) {
						mBuildExpCounter->incRefferedBottomClassCount();
					}
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
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETCLASS);
				} else {
					if (expression == mBottomClassExpression) {
						mBuildExpCounter->incRefferedBottomClassCount();
					}
				}
				return expression;
			}



			CDatatypeExpression* CConcreteOntologyBuildDataUpdater::getDatatype(const QString& datatypeName) {
				CDatatypeExpression* expression = nullptr;
				expression = mDatatypeBuildHash->value(datatypeName);
				if (!expression) {
					expression = new CDatatypeExpression(datatypeName);
					expression->setEntityID(mNextEntityNumber++);
					mDatatypeBuildHash->insert(datatypeName,expression);
					mBuildDatatypeSet->insert(expression);
					mBuildDatatypeList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATATYPE);
				}
				return expression;
			}

			CDatatypeExpression* CConcreteOntologyBuildDataUpdater::getDatatype(const QStringRef& datatypeName) {
				CDatatypeExpression* expression = nullptr;
				expression = mDatatypeBuildHash->value(datatypeName);
				if (!expression) {
					QString datatypeNameString(datatypeName.toString());
					expression = new CDatatypeExpression(datatypeNameString);
					expression->setEntityID(mNextEntityNumber++);
					mDatatypeBuildHash->insert(datatypeNameString,expression);
					mBuildDatatypeSet->insert(expression);
					mBuildDatatypeList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATATYPE);
				}
				return expression;
			}







			CDataFacetExpression* CConcreteOntologyBuildDataUpdater::getDataFacet(const QString& dataFacetIRI) {
				CDataFacetExpression* expression = nullptr;
				expression = mDataFacetBuildHash->value(dataFacetIRI);
				if (!expression) {
					expression = new CDataFacetExpression(dataFacetIRI);
					expression->setEntityID(mNextEntityNumber++);
					mDataFacetBuildHash->insert(dataFacetIRI,expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAFACETIRI);
				}
				return expression;
			}

			CDataFacetExpression* CConcreteOntologyBuildDataUpdater::getDataFacet(const QStringRef& dataFacetIRI) {
				CDataFacetExpression* expression = nullptr;
				expression = mDataFacetBuildHash->value(dataFacetIRI);
				if (!expression) {
					QString datatypeNameString(dataFacetIRI.toString());
					expression = new CDataFacetExpression(datatypeNameString);
					expression->setEntityID(mNextEntityNumber++);
					mDataFacetBuildHash->insert(dataFacetIRI,expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAFACETIRI);
				}
				return expression;
			}







			CDataLexicalValueExpression* CConcreteOntologyBuildDataUpdater::getDataLexicalValue(const QString& dataLexicalValue) {
				CDataLexicalValueExpression* expression = nullptr;
				expression = mDataLexicalValueBuildHash->value(dataLexicalValue);
				if (!expression) {
					expression = new CDataLexicalValueExpression(dataLexicalValue);
					expression->setEntityID(mNextEntityNumber++);
					mDataLexicalValueBuildHash->insert(dataLexicalValue,expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATALEXICALVALUE);
				}
				return expression;
			}

			CDataLexicalValueExpression* CConcreteOntologyBuildDataUpdater::getDataLexicalValue(const QStringRef& dataLexicalValue) {
				CDataLexicalValueExpression* expression = nullptr;
				expression = mDataLexicalValueBuildHash->value(dataLexicalValue);
				if (!expression) {
					QString dataLexicalValueString(dataLexicalValue.toString());
					expression = new CDataLexicalValueExpression(dataLexicalValueString);
					expression->setEntityID(mNextEntityNumber++);
					mDataLexicalValueBuildHash->insert(dataLexicalValueString,expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATALEXICALVALUE);
				}
				return expression;
			}







			CDataPropertyExpression* CConcreteOntologyBuildDataUpdater::getDataProberty(const QString& dataPropertyName) {
				CDataPropertyExpression* expression = nullptr;
				expression = mDataPropertyBuildHash->value(dataPropertyName);
				if (!expression) {
					expression = new CDataPropertyExpression(dataPropertyName);
					expression->setEntityID(mNextEntityNumber++);
					mDataPropertyBuildHash->insert(dataPropertyName,expression);
					mBuildDataRoleSet->insert(expression);
					mBuildDataRoleList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAPROPERTY);
				} else {
					if (expression == mBottomDataPropExpression) {
						mBuildExpCounter->incRefferedBottomDataPropertyCount();
					}
				}
				return expression;
			}

			CDataPropertyExpression* CConcreteOntologyBuildDataUpdater::getDataProberty(const QStringRef& dataPropertyName) {
				CDataPropertyExpression* expression = nullptr;
				expression = mDataPropertyBuildHash->value(dataPropertyName);
				if (!expression) {
					QString dataPropertyNameString(dataPropertyName.toString());
					expression = new CDataPropertyExpression(dataPropertyNameString);
					expression->setEntityID(mNextEntityNumber++);
					mDataPropertyBuildHash->insert(dataPropertyNameString,expression);
					mBuildDataRoleSet->insert(expression);
					mBuildDataRoleList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAPROPERTY);
				} else {
					if (expression == mBottomDataPropExpression) {
						mBuildExpCounter->incRefferedBottomDataPropertyCount();
					}
				}
				return expression;
			}







			CObjectIndividualVariableExpression* CConcreteOntologyBuildDataUpdater::getNominalIndividualVariable(const QString &individualVariableName, cint64 axiomNumber) {
				CObjectIndividualVariableExpression* expression = nullptr;
				expression = mNominalIndividualVariableBuildHash->value(QPair<CStringRefStringHasher,cint64>(individualVariableName,axiomNumber));
				if (!expression) {
					expression = new CObjectIndividualVariableExpression(individualVariableName,axiomNumber);
					expression->setEntityID(mNextEntityNumber++);
					mNextMaxAxiomNumberOffset = qMax(axiomNumber,mNextMaxAxiomNumberOffset);
					mNominalIndividualVariableBuildHash->insert(QPair<CStringRefStringHasher,cint64>(individualVariableName,axiomNumber),expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETNOMINALINDIVIDUALVARIABLE);
				}
				return expression;
			}

			CObjectIndividualVariableExpression* CConcreteOntologyBuildDataUpdater::getNominalIndividualVariable(const QStringRef &individualVariableName, cint64 axiomNumber) {
				CObjectIndividualVariableExpression* expression = nullptr;
				expression = mNominalIndividualVariableBuildHash->value(QPair<CStringRefStringHasher,cint64>(individualVariableName,axiomNumber));
				if (!expression) {
					QString individualVariableNameString(individualVariableName.toString());
					mNextMaxAxiomNumberOffset = qMax(axiomNumber,mNextMaxAxiomNumberOffset);
					expression = new CObjectIndividualVariableExpression(individualVariableNameString,axiomNumber);
					expression->setEntityID(mNextEntityNumber++);
					mNominalIndividualVariableBuildHash->insert(QPair<CStringRefStringHasher,cint64>(individualVariableNameString,axiomNumber),expression);
					mBuildConceptSet->insert(expression);
					mBuildConceptList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETNOMINALINDIVIDUALVARIABLE);
				}
				return expression;
			}





			CIndividualVariableExpression* CConcreteOntologyBuildDataUpdater::getIndividualVariable(const QStringRef &individualVariableName, bool anonymousVariable) {
				CIndividualVariableExpression* expression = nullptr;
				CBUILDHASH<CStringRefStringHasher, CIndividualVariableExpression*>* indiVarNameExpHash = mNamedIndividualVariableBuildHash;
				if (anonymousVariable) {
					indiVarNameExpHash = mAnonymousIndividualVariableBuildHash;
				}
				expression = indiVarNameExpHash->value(individualVariableName);
				if (!expression) {
					if (!anonymousVariable) {
						expression = new CNamedIndividualVariableExpression(individualVariableName.toString());
					} else {
						expression = new CAnonymousIndividualVariableExpression(individualVariableName.toString());
					}
					expression->setEntityID(mNextEntityNumber++);
					indiVarNameExpHash->insert(individualVariableName, expression);
					if (!anonymousVariable) {
						mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETNAMEDINDIVIDUALVARIABLE);
					} else {
						mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETANONYMOUSINDIVIDUALVARIABLE);
					}
				}
				return expression;
			}


			CIndividualVariableExpression* CConcreteOntologyBuildDataUpdater::getIndividualVariable(const QString &individualVariableName, bool anonymousVariable) {
				CIndividualVariableExpression* expression = nullptr;
				CBUILDHASH<CStringRefStringHasher, CIndividualVariableExpression*>* indiVarNameExpHash = mNamedIndividualVariableBuildHash;
				if (anonymousVariable) {
					indiVarNameExpHash = mAnonymousIndividualVariableBuildHash;
				}
				expression = indiVarNameExpHash->value(individualVariableName);
				if (!expression) {
					if (!anonymousVariable) {
						expression = new CNamedIndividualVariableExpression(individualVariableName);
					} else {
						expression = new CAnonymousIndividualVariableExpression(individualVariableName);
					}
					expression->setEntityID(mNextEntityNumber++);
					indiVarNameExpHash->insert(individualVariableName, expression);
					if (!anonymousVariable) {
						mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETNAMEDINDIVIDUALVARIABLE);
					} else {
						mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETANONYMOUSINDIVIDUALVARIABLE);
					}
				}
				return expression;
			}





			CDataValueVariableExpression* CConcreteOntologyBuildDataUpdater::getDataValueVariable(const QStringRef &dataValueVariableName) {
				CDataValueVariableExpression* expression = nullptr;
				expression = mDataValueVariableBuildHash->value(dataValueVariableName);
				if (!expression) {
					expression = new CDataValueVariableExpression(dataValueVariableName.toString());
					expression->setEntityID(mNextEntityNumber++);
					mDataValueVariableBuildHash->insert(dataValueVariableName, expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAVALUEVARIABLE);
				}
				return expression;
			}


			CDataValueVariableExpression* CConcreteOntologyBuildDataUpdater::getDataValueVariable(const QString &dataValueVariableName) {
				CDataValueVariableExpression* expression = nullptr;
				expression = mDataValueVariableBuildHash->value(dataValueVariableName);
				if (!expression) {
					expression = new CDataValueVariableExpression(dataValueVariableName);
					expression->setEntityID(mNextEntityNumber++);
					mDataValueVariableBuildHash->insert(dataValueVariableName, expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATAVALUEVARIABLE);
				}
				return expression;
			}




			CDataLiteralVariableExpression* CConcreteOntologyBuildDataUpdater::getDataLiteralVariable(const QStringRef &dataLiteralVariableName) {
				CDataLiteralVariableExpression* expression = nullptr;
				expression = mDataLiteralVariableBuildHash->value(dataLiteralVariableName);
				if (!expression) {
					expression = new CDataLiteralVariableExpression(dataLiteralVariableName.toString());
					expression->setEntityID(mNextEntityNumber++);
					mDataLiteralVariableBuildHash->insert(dataLiteralVariableName, expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATALITERALVARIABLE);
				}
				return expression;
			}


			CDataLiteralVariableExpression* CConcreteOntologyBuildDataUpdater::getDataLiteralVariable(const QString &dataLiteralVariableName) {
				CDataLiteralVariableExpression* expression = nullptr;
				expression = mDataLiteralVariableBuildHash->value(dataLiteralVariableName);
				if (!expression) {
					expression = new CDataLiteralVariableExpression(dataLiteralVariableName);
					expression->setEntityID(mNextEntityNumber++);
					mDataLiteralVariableBuildHash->insert(dataLiteralVariableName, expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETDATALITERALVARIABLE);
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
					mBuildObjectRoleSet->insert(expression);
					mBuildObjectRoleList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTPROPERTY);
				} else {
					if (expression == mBottomObjPropExpression) {
						mBuildExpCounter->incRefferedBottomObjectPropertyCount();
					}
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
					mBuildObjectRoleSet->insert(expression);
					mBuildObjectRoleList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETOBJECTPROPERTY);
				} else {
					if (expression == mBottomObjPropExpression) {
						mBuildExpCounter->incRefferedBottomObjectPropertyCount();
					}
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
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETNAMEDINDIVIDUAL);
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
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETNAMEDINDIVIDUAL);
				}
				return expression;
			}



			CAnonymousIndividualExpression* CConcreteOntologyBuildDataUpdater::getAnonymousIndividual(const QString& ontologyName, const QString& individualName) {
				CAnonymousIndividualExpression* expression = nullptr;
				QPair<CStringRefStringHasher,CStringRefStringHasher> namePair(ontologyName,individualName);
				expression = mAnoIndividualBuildHash->value(namePair);
				if (!expression) {
					expression = new CAnonymousIndividualExpression(ontologyName,individualName);
					expression->setEntityID(mNextEntityNumber++);
					mAnoIndividualBuildHash->insert(namePair,expression);
					mBuildIndividualSet->insert(expression);
					mBuildIndividualList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETANONYMOUSINDIVIDUAL);
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
					expression->setEntityID(mNextEntityNumber++);
					mAnoIndividualBuildHash->insert(QPair<CStringRefStringHasher,CStringRefStringHasher>(ontologyNameString,individualNameString),expression);
					mBuildIndividualSet->insert(expression);
					mBuildIndividualList->append(expression);
					mBuildExpCounter->incBuildExpressionCount(CBuildExpression::BETANONYMOUSINDIVIDUAL);
				}
				return expression;
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude