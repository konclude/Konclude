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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYENTITYCOLLECTORBUILDER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYENTITYCOLLECTORBUILDER_H


// Libraries includes
#include <QListIterator>
#include <QHash>
#include <QString>
#include <QObject>

// Namespace includes
#include "CConcreteOntologyBuildDataUpdater.h"
#include "CExpressionSplitter.h"
#include "CExpressionHasher.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CTBox.h"
#include "Reasoner/Ontology/CABox.h"
#include "Reasoner/Ontology/CRBox.h"
#include "Reasoner/Ontology/CAbbreviatedIRIName.h"
#include "Reasoner/Ontology/CIRIName.h"


#include "Utilities/CTrible.hpp"
#include "Utilities/CNegLinker.hpp"
#include "Utilities/CHashableIntConvertedNTuble.hpp"
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CDynamicExpandingMemoryManager.hpp"
#include "Utilities/Memory/CObjectAllocator.h"

#include "Parser/COntologyBuilder.h"

// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Parser;

	namespace Reasoner {

		using namespace Ontology;	

		namespace Generator {

			/*! 
			 *
			 *		\class		CConcreteOntologyEntityCollectorBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyEntityCollectorBuilder : public COntologyBuilder {
				// public methods
				public:
					CConcreteOntologyEntityCollectorBuilder();

					virtual cint64 getAxiomNumber();

					virtual bool addNameAbbreviation(const QString &name, const QString &abbreviatedName);

					virtual QString resolveAbbreviationName(const QString& name);
					virtual QString resolveAbbreviationName(const QStringRef& name);
					virtual QString getOntologyName();

					virtual CDeclarationAxiomExpression* getDeclaration(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDeclarationAxiomExpression* getDeclaration(CBuildExpression* buildExpression);
					virtual CDeclarationAxiomExpression* getDeclaration(CExpressionEntity* entity);

					virtual bool addOntologyImport(const QStringRef& name);
					virtual bool addOntologyImport(const QString& name);

					// virtual methods from CTBoxAxiomBuilder
					virtual CClassExpression* getClass(const QString& className);
					virtual CClassExpression* getClass(const QStringRef& className);
					virtual CClassTermExpression* getTopClass();
					virtual CClassTermExpression* getBottomClass();

					virtual CDataLiteralExpression* getDataLiteral(CDataLexicalValueExpression* dataLexicalValue, CDatatypeExpression* datatype);

					virtual CDatatypeExpression* getDatatype(const QString& datatypeName);
					virtual CDatatypeExpression* getDatatype(const QStringRef& datatypeName);
					virtual CDataLexicalValueExpression* getDataLexicalValue(const QString& dataLexicalValue);
					virtual CDataLexicalValueExpression* getDataLexicalValue(const QStringRef& dataLexicalValue);

					virtual CDataHasValueExpression* getDataHasValue(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDataHasValueExpression* getDataHasValue(CBuildExpression* expression1, CBuildExpression* expression2);

					virtual CDataSomeValuesFromExpression* getDataSomeValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDataSomeValuesFromExpression* getDataSomeValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDataAllValuesFromExpression* getDataAllValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDataAllValuesFromExpression* getDataAllValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2);

					virtual CDataIntersectionOfExpression* getDataIntersectionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDataIntersectionOfExpression* getDataIntersectionOf(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDataOneOfExpression* getDataOneOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDataOneOfExpression* getDataOneOf(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDataUnionOfExpression* getDataUnionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDataUnionOfExpression* getDataUnionOf(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDataComplementOfExpression* getDataComplementOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDataComplementOfExpression* getDataComplementOf(CBuildExpression* expression);
					virtual CDatatypeRestrictionExpression* getDatatypeRestriction(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDatatypeRestrictionExpression* getDatatypeRestriction(CBuildExpression* expression1, CBuildExpression* expression2);


					virtual CDataMaxCardinalityExpression* getDataMaxCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality);
					virtual CDataMaxCardinalityExpression* getDataMaxCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality);

					virtual CDataMinCardinalityExpression* getDataMinCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality);
					virtual CDataMinCardinalityExpression* getDataMinCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality);

					virtual CDataExactCardinalityExpression* getDataExactCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality);
					virtual CDataExactCardinalityExpression* getDataExactCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality);

					virtual CDataHasValueExpression* getDataHasValue(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2);

					virtual CDataSomeValuesFromExpression* getDataSomeValuesFrom(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2);
					virtual CDataAllValuesFromExpression* getDataAllValuesFrom(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2);

					virtual CDataIntersectionOfExpression* getDataIntersectionOf(const CEXPRESSIONLIST<CDataRangeTermExpression*>& expressions);
					virtual CDataOneOfExpression* getDataOneOf(const CEXPRESSIONLIST<CDataRangeTermExpression*>& expressions);
					virtual CDataUnionOfExpression* getDataUnionOf(const CEXPRESSIONLIST<CDataRangeTermExpression*>& expressions);
					virtual CDataComplementOfExpression* getDataComplementOf(CDataRangeTermExpression* expressions);
					virtual CDatatypeRestrictionExpression* getDatatypeRestriction(CDatatypeExpression* datatypeExpression, const CEXPRESSIONLIST<CDataFacetRestrictionExpression*>& expressions);

					virtual CDataMaxCardinalityExpression* getDataMaxCardinality(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2, int cardinality);
					virtual CDataMinCardinalityExpression* getDataMinCardinality(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2, int cardinality);
					virtual CDataExactCardinalityExpression* getDataExactCardinality(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2, int cardinality);


					virtual CDataRangeTermExpression* getTopDataRange();
					virtual CDataRangeTermExpression* getBottomDataRange();


					virtual CDataFacetExpression* getDataFacet(const QString& dataFacetIRI);
					virtual CDataFacetExpression* getDataFacet(const QStringRef& dataFacetIRI);

					virtual CDataFacetRestrictionExpression* getDataFacetRestriction(CDataLiteralExpression* dataLiteralExpression, CDataFacetExpression* dataFacet);

					virtual CDataFacetRestrictionExpression* getDataFacetRestriction(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDataFacetRestrictionExpression* getDataFacetRestriction(CBuildExpression* expression1, CBuildExpression* expression2);

					virtual CDataPropertyAssertionExpression* getDataPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDataPropertyAssertionExpression* getDataPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3);
					virtual CNegativeDataPropertyAssertionExpression* getNegativeDataPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CNegativeDataPropertyAssertionExpression* getNegativeDataPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3);

					virtual CDataPropertyAssertionExpression* getDataPropertyAssertion(CIndividualTermExpression* expression1, CDataLiteralTermExpression* expression2, CDataPropertyTermExpression* expression3);
					virtual CNegativeDataPropertyAssertionExpression* getNegativeDataPropertyAssertion(CIndividualTermExpression* expression1, CDataLiteralTermExpression* expression2, CDataPropertyTermExpression* expression3);

					virtual CDataPropertyExpression* getDataProberty(const QString& dataPropertyName);
					virtual CDataPropertyExpression* getDataProberty(const QStringRef& dataPropertyName);

					virtual CObjectIndividualVariableExpression* getNominalIndividualVariable(const QString &individualVariableName, cint64 axiomNumber);
					virtual CObjectIndividualVariableExpression* getNominalIndividualVariable(const QStringRef &individualVariableName, cint64 axiomNumber);


					virtual CEquivalentClassesExpression* getEquivalentClasses(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CEquivalentClassesExpression* getEquivalentClasses(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CSubClassOfExpression* getSubClassOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CSubClassOfExpression* getSubClassOf(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDisjointClassesExpression* getDisjointClasses(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDisjointUnionExpression *getDisjointUnion(const CEXPRESSIONLIST<CBuildExpression *> &expressions);
					virtual CObjectComplementOfExpression* getObjectComplementOf(CBuildExpression* expression);
					virtual CObjectComplementOfExpression* getObjectComplementOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectIntersectionOfExpression* getObjectIntersectionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectUnionOfExpression* getObjectUnionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectMaxCardinalityExpression* getObjectMaxCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality);
					virtual CObjectMaxCardinalityExpression* getObjectMaxCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality);
					virtual CObjectMinCardinalityExpression* getObjectMinCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality);
					virtual CObjectMinCardinalityExpression* getObjectMinCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality);
					virtual CObjectExactlyCardinalityExpression* getObjectExactlyCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality);
					virtual CObjectExactlyCardinalityExpression* getObjectExactlyCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality);
					virtual CObjectAllValuesFromExpression* getObjectAllValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectAllValuesFromExpression* getObjectAllValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectSomeValuesFromExpression* getObjectSomeValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectSomeValuesFromExpression* getObjectSomeValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectOneOfExpression *getObjectOneOf(const CEXPRESSIONLIST<CBuildExpression *> &expressions);
					virtual CObjectOneOfExpression *getObjectOneOf(CBuildExpression *expression);
					virtual CObjectHasValueExpression *getObjectHasValue(const CEXPRESSIONLIST<CBuildExpression *> &expressions);
					virtual CObjectHasValueExpression *getObjectHasValue(CBuildExpression *expression1, CBuildExpression *expression2);
					virtual CObjectHasSelfExpression *getObjectHasSelf(const CEXPRESSIONLIST<CBuildExpression *> &expressions);
					virtual CObjectHasSelfExpression *getObjectHasSelf(CBuildExpression *expression);


					virtual CEquivalentClassesExpression* getEquivalentClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions);
					virtual CSubClassOfExpression* getSubClassOf(CClassTermExpression* expression1, CClassTermExpression* expression2);
					virtual CDisjointClassesExpression* getDisjointClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions);
					virtual CDisjointUnionExpression *getDisjointUnion(CClassExpression* classExpression, const CEXPRESSIONLIST<CClassTermExpression*>& expressions);
					virtual CObjectComplementOfExpression* getObjectComplementOf(CClassTermExpression* expression);
					virtual CObjectIntersectionOfExpression* getObjectIntersectionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions);
					virtual CObjectUnionOfExpression* getObjectUnionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions);
					virtual CObjectMaxCardinalityExpression* getObjectMaxCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality);
					virtual CObjectMinCardinalityExpression* getObjectMinCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality);
					virtual CObjectExactlyCardinalityExpression* getObjectExactlyCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality);
					virtual CObjectAllValuesFromExpression* getObjectAllValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2);
					virtual CObjectSomeValuesFromExpression* getObjectSomeValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2);
					virtual CObjectOneOfExpression *getObjectOneOf(const CEXPRESSIONLIST<CIndividualTermExpression*> &expressions);
					virtual CObjectOneOfExpression *getObjectOneOf(CIndividualTermExpression* expression);
					virtual CObjectHasValueExpression *getObjectHasValue(CObjectPropertyTermExpression *expression1, CIndividualTermExpression *expression2);
					virtual CObjectHasSelfExpression *getObjectHasSelf(CObjectPropertyTermExpression *expression);





					// virtual methods from CRBoxAxiomBuilder
					virtual CObjectPropertyExpression* getObjectProberty(const QString& probertyName);
					virtual CObjectPropertyExpression* getObjectProberty(const QStringRef& probertyName);
					virtual CObjectPropertyTermExpression* getTopObjectProberty();
					virtual CObjectPropertyTermExpression* getBottomObjectProberty();

					virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectPropertyDomainExpression* getObjectPropertyDomainExpression(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectPropertyDomainExpression* getObjectPropertyDomainExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectPropertyRangeExpression* getObjectPropertyRangeExpression(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectPropertyRangeExpression* getObjectPropertyRangeExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CInverseObjectPropertiesExpression* getInverseObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CInverseObjectPropertiesExpression* getInverseObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CTransetiveObjectPropertyExpression* getTransetiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CTransetiveObjectPropertyExpression* getTransetiveObjectProperty(CBuildExpression* expression);
					virtual CFunctionalObjectPropertyExpression* getFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CFunctionalObjectPropertyExpression* getFunctionalObjectProperty(CBuildExpression* expression);
					virtual CInverseFunctionalObjectPropertyExpression* getInverseFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CInverseFunctionalObjectPropertyExpression* getInverseFunctionalObjectProperty(CBuildExpression* expression);
					virtual CInverseObjectPropertyOfExpression* getInverseObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CInverseObjectPropertyOfExpression* getInverseObjectPropertyOf(CBuildExpression* expression);
					virtual CObjectPropertyChainExpression* getObjectPropertyChain(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectPropertyChainExpression* getObjectPropertyChain(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CEquivalentObjectPropertiesExpression* getEquivalentObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CEquivalentObjectPropertiesExpression* getEquivalentObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDisjointObjectPropertiesExpression* getDisjointObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDisjointObjectPropertiesExpression* getDisjointObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CSymmetricObjectPropertyExpression* getSymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CSymmetricObjectPropertyExpression* getSymmetricObjectProperty(CBuildExpression* expression);
					virtual CAsymmetricObjectPropertyExpression* getAsymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CAsymmetricObjectPropertyExpression* getAsymmetricObjectProperty(CBuildExpression* expression);
					virtual CReflexiveObjectPropertyExpression* getReflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CReflexiveObjectPropertyExpression* getReflexiveObjectProperty(CBuildExpression* expression);
					virtual CIrreflexiveObjectPropertyExpression* getIrreflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CIrreflexiveObjectPropertyExpression* getIrreflexiveObjectProperty(CBuildExpression* expression);



					virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions, CObjectPropertyTermExpression* expression);
					virtual CObjectPropertyDomainExpression* getObjectPropertyDomainExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2);
					virtual CObjectPropertyRangeExpression* getObjectPropertyRangeExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2);
					virtual CInverseObjectPropertiesExpression* getInverseObjectProperties(CObjectPropertyTermExpression* expression1, CObjectPropertyTermExpression* expression2);
					virtual CTransetiveObjectPropertyExpression* getTransetiveObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CFunctionalObjectPropertyExpression* getFunctionalObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CInverseFunctionalObjectPropertyExpression* getInverseFunctionalObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CInverseObjectPropertyOfExpression* getInverseObjectPropertyOf(CObjectPropertyTermExpression* expression);
					virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(CObjectPropertyChainExpression* expressions, CObjectPropertyTermExpression* expression);
					virtual CObjectPropertyChainExpression* getObjectPropertyChain(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions);
					virtual CEquivalentObjectPropertiesExpression* getEquivalentObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions);
					virtual CDisjointObjectPropertiesExpression* getDisjointObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions);
					virtual CSymmetricObjectPropertyExpression* getSymmetricObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CAsymmetricObjectPropertyExpression* getAsymmetricObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CReflexiveObjectPropertyExpression* getReflexiveObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CIrreflexiveObjectPropertyExpression* getIrreflexiveObjectProperty(CObjectPropertyTermExpression* expression);

					CObjectPropertyTermExpression* getCorrectedInverseObjectPropertyOf(CObjectPropertyTermExpression* expression);




					virtual CSubDataPropertyOfExpression* getSubDataPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CSubDataPropertyOfExpression* getSubDataPropertyOf(CBuildExpression* expression1, CBuildExpression* expression2);

					virtual CEquivalentDataPropertiesExpression* getEquivalentDataProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CEquivalentDataPropertiesExpression* getEquivalentDataProperties(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDisjointDataPropertiesExpression* getDisjointDataProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDisjointDataPropertiesExpression* getDisjointDataProperties(CBuildExpression* expression1, CBuildExpression* expression2);

					virtual CDataPropertyDomainExpression* getDataPropertyDomainExpression(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDataPropertyDomainExpression* getDataPropertyDomainExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDataPropertyRangeExpression* getDataPropertyRangeExpression(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDataPropertyRangeExpression* getDataPropertyRangeExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions);

					virtual CFunctionalDataPropertyExpression* getFunctionalDataProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CFunctionalDataPropertyExpression* getFunctionalDataProperty(CBuildExpression* expression);



					virtual CSubDataPropertyOfExpression* getSubDataPropertyOf(CDataPropertyTermExpression* expression1, CDataPropertyTermExpression* expression2);

					virtual CEquivalentDataPropertiesExpression* getEquivalentDataProperties(const CEXPRESSIONLIST<CDataPropertyTermExpression*>& expressions);
					virtual CDisjointDataPropertiesExpression* getDisjointDataProperties(const CEXPRESSIONLIST<CDataPropertyTermExpression*>& expressions);


					virtual CDataPropertyDomainExpression* getDataPropertyDomainExpression(CDataPropertyTermExpression* expression1, CClassTermExpression* expression2);
					virtual CDataPropertyRangeExpression* getDataPropertyRangeExpression(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2);
					virtual CFunctionalDataPropertyExpression* getFunctionalDataProperty(CDataPropertyTermExpression* expression);


					// virtual methods from CABoxAxiomBuilder
					virtual CNamedIndividualExpression* getNamedIndividual(const QString& individualName);
					virtual CNamedIndividualExpression* getNamedIndividual(const QStringRef& individualName);
					virtual CAnonymousIndividualExpression* getAnonymousIndividual(const QString& ontologyName, const QString& individualName);
					virtual CAnonymousIndividualExpression* getAnonymousIndividual(const QStringRef& ontologyName, const QStringRef& individualName);


					virtual CIndividualVariableExpression* getIndividualVariable(const QStringRef &individualVariableName, bool anonymousVariable);
					virtual CIndividualVariableExpression* getIndividualVariable(const QString &individualVariableName, bool anonymousVariable);

					virtual CDataValueVariableExpression* getDataValueVariable(const QStringRef &dataValueVariableName);
					virtual CDataValueVariableExpression* getDataValueVariable(const QString &dataValueVariableName);

					virtual CDataLiteralVariableExpression* getDataLiteralVariable(const QStringRef &dataLiteralVariableName);
					virtual CDataLiteralVariableExpression* getDataLiteralVariable(const QString &dataLiteralVariableName);


					virtual CClassAssertionExpression* getClassAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CClassAssertionExpression* getClassAssertion(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectPropertyAssertionExpression* getObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectPropertyAssertionExpression* getObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3);
					virtual CNegativeObjectPropertyAssertionExpression* getNegativeObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CNegativeObjectPropertyAssertionExpression* getNegativeObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3);
					virtual CSameIndividualExpression* getSameIndividual(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDifferentIndividualsExpression* getDifferentIndividuals(const CEXPRESSIONLIST<CBuildExpression*>& expressions);


					virtual CClassAssertionExpression* getClassAssertion(CIndividualTermExpression* expression1, CClassTermExpression* expression2);
					virtual CObjectPropertyAssertionExpression* getObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3);
					virtual CNegativeObjectPropertyAssertionExpression* getNegativeObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3);
					virtual CSameIndividualExpression* getSameIndividual(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions);
					virtual CDifferentIndividualsExpression* getDifferentIndividuals(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions);


					virtual bool tellOntologyAxiom(CBuildExpression* axiom);
					virtual bool retractOntologyAxiom(CBuildExpression* axiom);

					virtual bool tellOntologyAxiom(CAxiomExpression* axiom);
					virtual bool retractOntologyAxiom(CAxiomExpression* axiom);


					virtual bool addTriplesData(CTriplesData* tripleData);
					virtual CTriplesData* getLatestTriplesData(bool onlyLocal = true, bool* localFlag = nullptr);


					QSet<QString>* getClassNameSet();
					QSet<QString>* getObjectPropertyNameSet();
					QSet<QString>* getDataPropertyNameSet();
					QSet<QString>* getIndividualNameSet();
					QSet<QString>* getNominalVariableNameSet();
					QSet<QString>* getNamedIndividualVariableNameSet();
					QSet<QString>* getAnonymousIndividualVariableNameSet();
					QSet<QString>* getDataValueVariableNameSet();
					QSet<QString>* getDataLiteralVariableNameSet();


				// protected functions
				protected:

				// protected variables
				protected:

					QSet<QString> mClassNameSet;
					QSet<QString> mObjectPropertyNameSet;
					QSet<QString> mDataPropertyNameSet;
					QSet<QString> mIndividualNameSet;
					QSet<QString> mAnonymousIndividualNameSet;
					QSet<QString> mNominalVariableNameSet;
					QSet<QString> mNamedIndividualVariableNameSet;
					QSet<QString> mAnonymousIndividualVariableNameSet;
					QSet<QString> mDataValueVariableNameSet;
					QSet<QString> mDataLiteralVariableNameSet;
					QHash<QString, QString> mAbbreviationsResolveHash;


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYENTITYCOLLECTORBUILDER_H
