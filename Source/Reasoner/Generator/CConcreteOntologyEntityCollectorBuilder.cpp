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

#include "CConcreteOntologyEntityCollectorBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyEntityCollectorBuilder::CConcreteOntologyEntityCollectorBuilder() {
			}





			QSet<QString>* CConcreteOntologyEntityCollectorBuilder::getClassNameSet() {
				return &mClassNameSet;
			}

			QSet<QString>* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyNameSet() {
				return &mObjectPropertyNameSet;
			}

			QSet<QString>* CConcreteOntologyEntityCollectorBuilder::getDataPropertyNameSet() {
				return &mDataPropertyNameSet;
			}

			QSet<QString>* CConcreteOntologyEntityCollectorBuilder::getIndividualNameSet() {
				return &mIndividualNameSet;
			}

			QSet<QString>* CConcreteOntologyEntityCollectorBuilder::getVariableNameSet() {
				return &mVariableNameSet;
			}






			cint64 CConcreteOntologyEntityCollectorBuilder::getAxiomNumber() {
				return 0;
			}

			bool CConcreteOntologyEntityCollectorBuilder::addNameAbbreviation(const QString &name, const QString &abbreviatedName) {
				mAbbreviationsResolveHash.insert(abbreviatedName,name);
				return true;
			}

			QString CConcreteOntologyEntityCollectorBuilder::resolveAbbreviationName(const QString& name) {
				QStringList nameList = name.split(":");
				QString resolvedName(name);
				if (nameList.count() >= 2) {
					if (mAbbreviationsResolveHash.contains(nameList.first())) {
						resolvedName = mAbbreviationsResolveHash.value(nameList.takeFirst());
						QString remName = nameList.first();
						if (nameList.count() > 1) {
							remName = nameList.join(":");
						}
						resolvedName += remName;
					}
				}
				return resolvedName;
			}

			QString CConcreteOntologyEntityCollectorBuilder::resolveAbbreviationName(const QStringRef& name) {
				return resolveAbbreviationName(name.toString());
			}

			QString CConcreteOntologyEntityCollectorBuilder::getOntologyName() {
				return QString();
			}






			bool CConcreteOntologyEntityCollectorBuilder::addOntologyImport(const QStringRef& name) {
				return false;
			}

			bool CConcreteOntologyEntityCollectorBuilder::addOntologyImport(const QString& name) {
				return false;
			}




			CDeclarationAxiomExpression* CConcreteOntologyEntityCollectorBuilder::getDeclaration(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDeclarationAxiomExpression* CConcreteOntologyEntityCollectorBuilder::getDeclaration(CBuildExpression* buildExpression) {
				return nullptr;
			}

			CDeclarationAxiomExpression* CConcreteOntologyEntityCollectorBuilder::getDeclaration(CExpressionEntity* entity) {
				return nullptr;
			}



			CClassExpression* CConcreteOntologyEntityCollectorBuilder::getClass(const QString& className) {
				mClassNameSet.insert(className);
				return nullptr;
			}

			CClassExpression* CConcreteOntologyEntityCollectorBuilder::getClass(const QStringRef& className) {
				return getClass(className.toString());
			}

			CClassTermExpression* CConcreteOntologyEntityCollectorBuilder::getTopClass() {
				return nullptr;
			}

			CClassTermExpression* CConcreteOntologyEntityCollectorBuilder::getBottomClass() {
				return nullptr;
			}


			CObjectIndividualVariableExpression* CConcreteOntologyEntityCollectorBuilder::getIndividualVariable(const QString &individualVariableName, cint64 axiomNumber) {
				mVariableNameSet.insert(individualVariableName);
				return nullptr;
			}

			CObjectIndividualVariableExpression* CConcreteOntologyEntityCollectorBuilder::getIndividualVariable(const QStringRef &individualVariableName, cint64 axiomNumber) {
				return getIndividualVariable(individualVariableName.toString(),axiomNumber);
			}




			CDataLiteralExpression* CConcreteOntologyEntityCollectorBuilder::getDataLiteral(CDataLexicalValueExpression* dataLexicalValue, CDatatypeExpression* datatype) {
				return nullptr;
			}


			CDatatypeExpression* CConcreteOntologyEntityCollectorBuilder::getDatatype(const QString& datatypeName) {
				return nullptr;
			}

			CDatatypeExpression* CConcreteOntologyEntityCollectorBuilder::getDatatype(const QStringRef& datatypeName) {
				return nullptr;
			}

			CDataLexicalValueExpression* CConcreteOntologyEntityCollectorBuilder::getDataLexicalValue(const QString& dataLexicalValue) {
				return nullptr;
			}

			CDataLexicalValueExpression* CConcreteOntologyEntityCollectorBuilder::getDataLexicalValue(const QStringRef& dataLexicalValue) {
				return nullptr;
			}

			CDataHasValueExpression* CConcreteOntologyEntityCollectorBuilder::getDataHasValue(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDataHasValueExpression* CConcreteOntologyEntityCollectorBuilder::getDataHasValue(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDataSomeValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getDataSomeValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDataSomeValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getDataSomeValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDataAllValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getDataAllValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDataAllValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getDataAllValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDataIntersectionOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataIntersectionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDataIntersectionOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataIntersectionOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDataOneOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataOneOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDataOneOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataOneOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDataUnionOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataUnionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDataUnionOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataUnionOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDataComplementOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataComplementOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDataComplementOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataComplementOf(CBuildExpression* expression) {
				return nullptr;
			}

			CDatatypeRestrictionExpression* CConcreteOntologyEntityCollectorBuilder::getDatatypeRestriction(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDatatypeRestrictionExpression* CConcreteOntologyEntityCollectorBuilder::getDatatypeRestriction(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDataFacetExpression* CConcreteOntologyEntityCollectorBuilder::getDataFacet(const QString& dataFacetIRI) {
				return nullptr;
			}
			
			CDataFacetExpression* CConcreteOntologyEntityCollectorBuilder::getDataFacet(const QStringRef& dataFacetIRI) {
				return nullptr;
			}


			CDataFacetRestrictionExpression* CConcreteOntologyEntityCollectorBuilder::getDataFacetRestriction(CDataLiteralExpression* dataLiteralExpression, CDataFacetExpression* dataFacet) {
				return nullptr;
			}

			CDataFacetRestrictionExpression* CConcreteOntologyEntityCollectorBuilder::getDataFacetRestriction(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDataFacetRestrictionExpression* CConcreteOntologyEntityCollectorBuilder::getDataFacetRestriction(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}



			CDataMaxCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getDataMaxCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				return nullptr;
			}

			CDataMaxCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getDataMaxCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				return nullptr;
			}


			CDataMinCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getDataMinCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				return nullptr;
			}

			CDataMinCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getDataMinCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				return nullptr;
			}


			CDataExactCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getDataExactCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				return nullptr;
			}

			CDataExactCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getDataExactCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				return nullptr;
			}



			CDataHasValueExpression* CConcreteOntologyEntityCollectorBuilder::getDataHasValue(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2) {
				return nullptr;
			}

			CDataSomeValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getDataSomeValuesFrom(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2) {
				return nullptr;
			}

			CDataAllValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getDataAllValuesFrom(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2) {
				return nullptr;
			}


			CDataIntersectionOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataIntersectionOf(const CEXPRESSIONLIST<CDataRangeTermExpression*>& expressions) {
				return nullptr;
			}
			
			CDataOneOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataOneOf(const CEXPRESSIONLIST<CDataRangeTermExpression*>& expressions) {
				return nullptr;
			}

			CDataUnionOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataUnionOf(const CEXPRESSIONLIST<CDataRangeTermExpression*>& expressions) {
				return nullptr;
			}

			CDataComplementOfExpression* CConcreteOntologyEntityCollectorBuilder::getDataComplementOf(CDataRangeTermExpression* expressions) {
				return nullptr;
			}

			CDatatypeRestrictionExpression* CConcreteOntologyEntityCollectorBuilder::getDatatypeRestriction(CDatatypeExpression* datatypeExpression, const CEXPRESSIONLIST<CDataFacetRestrictionExpression*>& expressions) {
				return nullptr;
			}


			CDataMaxCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getDataMaxCardinality(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2, int cardinality) {
				return nullptr;
			}

			CDataMinCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getDataMinCardinality(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2, int cardinality) {
				return nullptr;
			}

			CDataExactCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getDataExactCardinality(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2, int cardinality) {
				return nullptr;
			}

			CDataRangeTermExpression* CConcreteOntologyEntityCollectorBuilder::getTopDataRange() {
				return nullptr;
			}
			
			CDataRangeTermExpression* CConcreteOntologyEntityCollectorBuilder::getBottomDataRange() {
				return nullptr;
			}


			CDataPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getDataPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDataPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getDataPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				return nullptr;
			}

			CNegativeDataPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getNegativeDataPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CNegativeDataPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getNegativeDataPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				return nullptr;
			}



			CDataPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getDataPropertyAssertion(CIndividualTermExpression* expression1, CDataLiteralExpression* expression2, CDataPropertyTermExpression* expression3) {
				return nullptr;
			}

			CNegativeDataPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getNegativeDataPropertyAssertion(CIndividualTermExpression* expression1, CDataLiteralExpression* expression2, CDataPropertyTermExpression* expression3) {
				return nullptr;
			}




			CEquivalentClassesExpression* CConcreteOntologyEntityCollectorBuilder::getEquivalentClasses(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CEquivalentClassesExpression* CConcreteOntologyEntityCollectorBuilder::getEquivalentClasses(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CSubClassOfExpression* CConcreteOntologyEntityCollectorBuilder::getSubClassOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CSubClassOfExpression* CConcreteOntologyEntityCollectorBuilder::getSubClassOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDisjointClassesExpression* CConcreteOntologyEntityCollectorBuilder::getDisjointClasses(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDisjointUnionExpression *CConcreteOntologyEntityCollectorBuilder::getDisjointUnion(const CEXPRESSIONLIST<CBuildExpression *> &expressions) {
				return nullptr;
			}

			CObjectComplementOfExpression* CConcreteOntologyEntityCollectorBuilder::getObjectComplementOf(CBuildExpression* expression) {
				return nullptr;
			}

			CObjectComplementOfExpression* CConcreteOntologyEntityCollectorBuilder::getObjectComplementOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CObjectIntersectionOfExpression* CConcreteOntologyEntityCollectorBuilder::getObjectIntersectionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CObjectUnionOfExpression* CConcreteOntologyEntityCollectorBuilder::getObjectUnionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CObjectMaxCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getObjectMaxCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				return nullptr;
			}

			CObjectMaxCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getObjectMaxCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				return nullptr;
			}

			CObjectMinCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getObjectMinCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				return nullptr;
			}

			CObjectMinCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getObjectMinCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				return nullptr;
			}

			CObjectExactlyCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getObjectExactlyCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality) {
				return nullptr;
			}

			CObjectExactlyCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getObjectExactlyCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality) {
				return nullptr;
			}

			CObjectAllValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getObjectAllValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CObjectAllValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getObjectAllValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CObjectSomeValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getObjectSomeValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CObjectSomeValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getObjectSomeValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CObjectOneOfExpression *CConcreteOntologyEntityCollectorBuilder::getObjectOneOf(const CEXPRESSIONLIST<CBuildExpression *> &expressions) {
				return nullptr;
			}

			CObjectOneOfExpression *CConcreteOntologyEntityCollectorBuilder::getObjectOneOf(CBuildExpression *expression) {
				return nullptr;
			}

			CObjectHasValueExpression *CConcreteOntologyEntityCollectorBuilder::getObjectHasValue(const CEXPRESSIONLIST<CBuildExpression *> &expressions) {
				return nullptr;
			}

			CObjectHasValueExpression *CConcreteOntologyEntityCollectorBuilder::getObjectHasValue(CBuildExpression *expression1, CBuildExpression *expression2) {
				return nullptr;
			}

			CObjectHasSelfExpression *CConcreteOntologyEntityCollectorBuilder::getObjectHasSelf(const CEXPRESSIONLIST<CBuildExpression *> &expressions) {
				return nullptr;
			}

			CObjectHasSelfExpression *CConcreteOntologyEntityCollectorBuilder::getObjectHasSelf(CBuildExpression *expression) {
				return nullptr;
			}



			CEquivalentClassesExpression* CConcreteOntologyEntityCollectorBuilder::getEquivalentClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				return nullptr;
			}

			CSubClassOfExpression* CConcreteOntologyEntityCollectorBuilder::getSubClassOf(CClassTermExpression* expression1, CClassTermExpression* expression2) {
				return nullptr;
			}

			CDisjointClassesExpression* CConcreteOntologyEntityCollectorBuilder::getDisjointClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				return nullptr;
			}

			CDisjointUnionExpression *CConcreteOntologyEntityCollectorBuilder::getDisjointUnion(CClassExpression* classExpression, const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				return nullptr;
			}

			CObjectComplementOfExpression* CConcreteOntologyEntityCollectorBuilder::getObjectComplementOf(CClassTermExpression* expression) {
				return nullptr;
			}

			CObjectIntersectionOfExpression* CConcreteOntologyEntityCollectorBuilder::getObjectIntersectionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				return nullptr;
			}

			CObjectUnionOfExpression* CConcreteOntologyEntityCollectorBuilder::getObjectUnionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) {
				return nullptr;
			}

			CObjectMaxCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getObjectMaxCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) {
				return nullptr;
			}

			CObjectMinCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getObjectMinCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) {
				return nullptr;
			}

			CObjectExactlyCardinalityExpression* CConcreteOntologyEntityCollectorBuilder::getObjectExactlyCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) {
				return nullptr;
			}

			CObjectAllValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getObjectAllValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				return nullptr;
			}

			CObjectSomeValuesFromExpression* CConcreteOntologyEntityCollectorBuilder::getObjectSomeValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				return nullptr;
			}

			CObjectOneOfExpression *CConcreteOntologyEntityCollectorBuilder::getObjectOneOf(const CEXPRESSIONLIST<CIndividualTermExpression*> &expressions) {
				return nullptr;
			}

			CObjectOneOfExpression *CConcreteOntologyEntityCollectorBuilder::getObjectOneOf(CIndividualTermExpression* expression) {
				return nullptr;
			}

			CObjectHasValueExpression *CConcreteOntologyEntityCollectorBuilder::getObjectHasValue(CObjectPropertyTermExpression *expression1, CIndividualTermExpression *expression2) {
				return nullptr;
			}

			CObjectHasSelfExpression *CConcreteOntologyEntityCollectorBuilder::getObjectHasSelf(CObjectPropertyTermExpression *expression) {
				return nullptr;
			}




			CDataPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getDataProberty(const QString& dataPropertyName) {
				mDataPropertyNameSet.insert(dataPropertyName);
				return nullptr;
			}


			CDataPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getDataProberty(const QStringRef& dataPropertyName) {
				return getDataProberty(dataPropertyName.toString());
			}



			CObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getObjectProberty(const QString& probertyName) {
				mObjectPropertyNameSet.insert(probertyName);
				return nullptr;
			}

			CObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getObjectProberty(const QStringRef& probertyName) {
				return getObjectProberty(probertyName.toString());
			}

			CObjectPropertyTermExpression* CConcreteOntologyEntityCollectorBuilder::getTopObjectProberty() {
				return nullptr;
			}


			CSubObjectPropertyOfExpression* CConcreteOntologyEntityCollectorBuilder::getSubObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CSubObjectPropertyOfExpression* CConcreteOntologyEntityCollectorBuilder::getSubObjectPropertyOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CObjectPropertyDomainExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyDomainExpression(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CObjectPropertyDomainExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyDomainExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CObjectPropertyRangeExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyRangeExpression(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CObjectPropertyRangeExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyRangeExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}




			CSubDataPropertyOfExpression* CConcreteOntologyEntityCollectorBuilder::getSubDataPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CSubDataPropertyOfExpression* CConcreteOntologyEntityCollectorBuilder::getSubDataPropertyOf(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}


			CEquivalentDataPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getEquivalentDataProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CEquivalentDataPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getEquivalentDataProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDisjointDataPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getDisjointDataProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDisjointDataPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getDisjointDataProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}


			CDataPropertyDomainExpression* CConcreteOntologyEntityCollectorBuilder::getDataPropertyDomainExpression(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDataPropertyDomainExpression* CConcreteOntologyEntityCollectorBuilder::getDataPropertyDomainExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDataPropertyRangeExpression* CConcreteOntologyEntityCollectorBuilder::getDataPropertyRangeExpression(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDataPropertyRangeExpression* CConcreteOntologyEntityCollectorBuilder::getDataPropertyRangeExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}


			CFunctionalDataPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getFunctionalDataProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CFunctionalDataPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getFunctionalDataProperty(CBuildExpression* expression) {
				return nullptr;
			}




			CSubDataPropertyOfExpression* CConcreteOntologyEntityCollectorBuilder::getSubDataPropertyOf(CDataPropertyTermExpression* expression1, CDataPropertyTermExpression* expression2) {
				return nullptr;
			}


			CEquivalentDataPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getEquivalentDataProperties(const CEXPRESSIONLIST<CDataPropertyTermExpression*>& expressions) {
				return nullptr;
			}

			CDisjointDataPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getDisjointDataProperties(const CEXPRESSIONLIST<CDataPropertyTermExpression*>& expressions) {
				return nullptr;
			}



			CDataPropertyDomainExpression* CConcreteOntologyEntityCollectorBuilder::getDataPropertyDomainExpression(CDataPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				return nullptr;
			}

			CDataPropertyRangeExpression* CConcreteOntologyEntityCollectorBuilder::getDataPropertyRangeExpression(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2) {
				return nullptr;
			}

			CFunctionalDataPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getFunctionalDataProperty(CDataPropertyTermExpression* expression) {
				return nullptr;
			}






			CInverseObjectPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getInverseObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CInverseObjectPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getInverseObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CTransetiveObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getTransetiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CTransetiveObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getTransetiveObjectProperty(CBuildExpression* expression) {
				return nullptr;
			}

			CFunctionalObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CFunctionalObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getFunctionalObjectProperty(CBuildExpression* expression) {
				return nullptr;
			}

			CInverseFunctionalObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getInverseFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CInverseFunctionalObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getInverseFunctionalObjectProperty(CBuildExpression* expression) {
				return nullptr;
			}

			CInverseObjectPropertyOfExpression* CConcreteOntologyEntityCollectorBuilder::getInverseObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CInverseObjectPropertyOfExpression* CConcreteOntologyEntityCollectorBuilder::getInverseObjectPropertyOf(CBuildExpression* expression) {
				return nullptr;
			}

			CObjectPropertyChainExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyChain(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CObjectPropertyChainExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyChain(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CEquivalentObjectPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getEquivalentObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CEquivalentObjectPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getEquivalentObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CDisjointObjectPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getDisjointObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDisjointObjectPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getDisjointObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CSymmetricObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getSymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CSymmetricObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getSymmetricObjectProperty(CBuildExpression* expression) {
				return nullptr;
			}

			CAsymmetricObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getAsymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CAsymmetricObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getAsymmetricObjectProperty(CBuildExpression* expression) {
				return nullptr;
			}

			CReflexiveObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getReflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CReflexiveObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getReflexiveObjectProperty(CBuildExpression* expression) {
				return nullptr;
			}

			CIrreflexiveObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getIrreflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CIrreflexiveObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getIrreflexiveObjectProperty(CBuildExpression* expression) {
				return nullptr;
			}




			CSubObjectPropertyOfExpression* CConcreteOntologyEntityCollectorBuilder::getSubObjectPropertyOf(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions, CObjectPropertyTermExpression* expression) {
				return nullptr;
			}

			CObjectPropertyDomainExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyDomainExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				return nullptr;
			}

			CObjectPropertyRangeExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyRangeExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) {
				return nullptr;
			}

			CInverseObjectPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getInverseObjectProperties(CObjectPropertyTermExpression* expression1, CObjectPropertyTermExpression* expression2) {
				return nullptr;
			}

			CTransetiveObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getTransetiveObjectProperty(CObjectPropertyTermExpression* expression) {
				return nullptr;
			}

			CFunctionalObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getFunctionalObjectProperty(CObjectPropertyTermExpression* expression) {
				return nullptr;
			}

			CInverseFunctionalObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getInverseFunctionalObjectProperty(CObjectPropertyTermExpression* expression) {
				return nullptr;
			}

			CInverseObjectPropertyOfExpression* CConcreteOntologyEntityCollectorBuilder::getInverseObjectPropertyOf(CObjectPropertyTermExpression* expression) {
				return nullptr;
			}

			CSubObjectPropertyOfExpression* CConcreteOntologyEntityCollectorBuilder::getSubObjectPropertyOf(CObjectPropertyChainExpression* expressions, CObjectPropertyTermExpression* expression) {
				return nullptr;
			}

			CObjectPropertyChainExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyChain(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) {
				return nullptr;
			}

			CEquivalentObjectPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getEquivalentObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) {
				return nullptr;
			}

			CDisjointObjectPropertiesExpression* CConcreteOntologyEntityCollectorBuilder::getDisjointObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) {
				return nullptr;
			}

			CSymmetricObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getSymmetricObjectProperty(CObjectPropertyTermExpression* expression) {
				return nullptr;
			}

			CAsymmetricObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getAsymmetricObjectProperty(CObjectPropertyTermExpression* expression) {
				return nullptr;
			}

			CReflexiveObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getReflexiveObjectProperty(CObjectPropertyTermExpression* expression) {
				return nullptr;
			}

			CIrreflexiveObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getIrreflexiveObjectProperty(CObjectPropertyTermExpression* expression) {
				return nullptr;
			}


			CObjectPropertyTermExpression* CConcreteOntologyEntityCollectorBuilder::getCorrectedInverseObjectPropertyOf(CObjectPropertyTermExpression* expression) {
				return nullptr;
			}



			CNamedIndividualExpression* CConcreteOntologyEntityCollectorBuilder::getNamedIndividual(const QString& individualName) {
				mIndividualNameSet.insert(individualName);
				return nullptr;
			}

			CNamedIndividualExpression* CConcreteOntologyEntityCollectorBuilder::getNamedIndividual(const QStringRef& individualName) {
				return getNamedIndividual(individualName.toString());
			}

			CAnonymousIndividualExpression* CConcreteOntologyEntityCollectorBuilder::getAnonymousIndividual(const QString& ontologyName, const QString& individualName) {
				mAnonymousIndividualNameSet.insert(individualName);
				return nullptr;
			}

			CAnonymousIndividualExpression* CConcreteOntologyEntityCollectorBuilder::getAnonymousIndividual(const QStringRef& ontologyName, const QStringRef& individualName) {
				return getAnonymousIndividual(ontologyName.toString(),individualName.toString());
			}


			CClassAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getClassAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CClassAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getClassAssertion(CBuildExpression* expression1, CBuildExpression* expression2) {
				return nullptr;
			}

			CObjectPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CObjectPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				return nullptr;
			}

			CNegativeObjectPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getNegativeObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CNegativeObjectPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getNegativeObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				return nullptr;
			}

			CSameIndividualExpression* CConcreteOntologyEntityCollectorBuilder::getSameIndividual(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}

			CDifferentIndividualsExpression* CConcreteOntologyEntityCollectorBuilder::getDifferentIndividuals(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				return nullptr;
			}



			CClassAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getClassAssertion(CIndividualTermExpression* expression1, CClassTermExpression* expression2) {
				return nullptr;
			}

			CObjectPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3) {
				return nullptr;
			}

			CNegativeObjectPropertyAssertionExpression* CConcreteOntologyEntityCollectorBuilder::getNegativeObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3) {
				return nullptr;
			}

			CSameIndividualExpression* CConcreteOntologyEntityCollectorBuilder::getSameIndividual(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions) {
				return nullptr;
			}

			CDifferentIndividualsExpression* CConcreteOntologyEntityCollectorBuilder::getDifferentIndividuals(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions) {
				return nullptr;
			}




			bool CConcreteOntologyEntityCollectorBuilder::tellOntologyAxiom(CAxiomExpression* axiom) {
				return false;
			}

			bool CConcreteOntologyEntityCollectorBuilder::retractOntologyAxiom(CAxiomExpression* axiom) {
				return false;
			}





		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude