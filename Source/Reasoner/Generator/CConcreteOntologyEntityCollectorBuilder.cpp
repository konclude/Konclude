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

#include "CConcreteOntologyEntityCollectorBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyEntityCollectorBuilder::CConcreteOntologyEntityCollectorBuilder() {
			}





			QSet<QString>* CConcreteOntologyEntityCollectorBuilder::getClassNameSet() {
				return &mClassNameSet;
			}

			QSet<QString>* CConcreteOntologyEntityCollectorBuilder::getPropertyNameSet() {
				return &mPropertyNameSet;
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






			CObjectPropertyExpression* CConcreteOntologyEntityCollectorBuilder::getObjectProberty(const QString& probertyName) {
				mPropertyNameSet.insert(probertyName);
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
				mIndividualNameSet.insert(individualName);
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