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

#include "CXMLOWL2Handler.h"


namespace Konclude {

	namespace Parser {



		CXMLOWL2Handler::CXMLOWL2Handler(COntologyBuilder* ontoBuilder)
				: CLogIdentifier("::Konclude::Parser::OWL2QtXMLOntologyParser",this),recordRouter(0,this) {

			mOntoBuilder = ontoBuilder;
			mAxiomNumber = 0;
			LOG(INFO,getLogDomain(),logTr("Created OWL 2.0 Qt XML Parser."),this);

			initializeParseFunctionJumpHash();
		}

		CXMLOWL2Handler::CXMLOWL2Handler(COntologyBuilder* ontoBuilder, CCommandRecordRouter *commandRecordRouter)
			: CLogIdentifier("::Konclude::Parser::OWL2QtXMLOntologyParser",this),recordRouter(*commandRecordRouter,this) {

				mOntoBuilder = ontoBuilder;
				mAxiomNumber = 0;
				LOG(INFO,getLogDomain(),logTr("Created OWL 2.0 Qt XML Parser."),this);

				initializeParseFunctionJumpHash();
		}


		


		bool CXMLOWL2Handler::startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts, CParsingHandlingContext* handlingContext) {
			mStack.push(new CParseStackObject(namespaceURI,localName,qName,atts));
			return true;
		}

		bool CXMLOWL2Handler::endElement(const QString& namespaceURI, const QString& localName, const QString& qName, CParsingHandlingContext* handlingContext) {
			CParseStackObject* parseStackObject = mStack.pop();
			parseBuildNode(parseStackObject);
			delete parseStackObject;
			return true;
		}


		bool CXMLOWL2Handler::startDocument(CParsingHandlingContext* handlingContext) {
			mStack.push(new CParseStackObject("","","",QXmlAttributes()));
			return true;
		}

		bool CXMLOWL2Handler::endDocument(CParsingHandlingContext* handlingContext) {
			if (mStack.count() > 0) {
				CParseStackObject* parseStackObject = mStack.pop();
				parseTellOntologyAxiomNode(parseStackObject);
				delete parseStackObject;
			}
			return true;
		}






		void CXMLOWL2Handler::parseTellOntologyAxiomNode(CParseStackObject* parseStackObj) {
			if (mOntoBuilder && parseStackObj->hasExpressions()) {

				QList<CBuildExpression*>* expList = parseStackObj->getExpressions();
				for (QList<CBuildExpression*>::const_iterator it = expList->constBegin(), itEnd = expList->constEnd(); it != itEnd; ++it) {
					CAxiomExpression* axiomExpression = dynamic_cast<CAxiomExpression*>(*it);
					if (axiomExpression) {
						mOntoBuilder->tellOntologyAxiom(axiomExpression);
					}
				}

			}
		}

		void CXMLOWL2Handler::parseRetractOntologyAxiomNode(CParseStackObject* parseStackObj) {
			if (mOntoBuilder && parseStackObj->hasExpressions()) {

				QList<CBuildExpression*>* expList = parseStackObj->getExpressions();
				for (QList<CBuildExpression*>::const_iterator it = expList->constBegin(), itEnd = expList->constEnd(); it != itEnd; ++it) {
					CAxiomExpression* axiomExpression = dynamic_cast<CAxiomExpression*>(*it);
					if (axiomExpression) {
						mOntoBuilder->retractOntologyAxiom(axiomExpression);
					}
				}

			}
		}


		void CXMLOWL2Handler::parsePrefixNode(CParseStackObject* parseStackObj) {
			QString abbName = parseStackObj->mAtts.value(mNameString);
			QString totalIRIName = parseStackObj->mAtts.value(mIRIString);

			if (abbName.isEmpty()) {
				abbName = parseStackObj->mAtts.value(mOWLPrefixNameString);
			}
			if (totalIRIName.isEmpty()) {
				totalIRIName = parseStackObj->mAtts.value(mOWLPrefixIRIString);
			}

			if (!totalIRIName.isEmpty()) {
				mOntoBuilder->addNameAbbreviation(totalIRIName,abbName);
			}
		}




		QString CXMLOWL2Handler::getEntityName(CParseStackObject* parseStackObj) {
			QString entityName;

			bool abbreviated = false;

			QString attrNameString;
			attrNameString = parseStackObj->mAtts.value(mIRIString);
			if (attrNameString.isEmpty()) {
				attrNameString = parseStackObj->mAtts.value(mOWLPrefixIRIString);
				abbreviated = true;
			}
			if (attrNameString.isEmpty()) {
				attrNameString = parseStackObj->mAtts.value(mAbbreviatedIRIString);
				abbreviated = true;
			}
			if (attrNameString.isEmpty()) {
				attrNameString = parseStackObj->mAtts.value(mOWLPrefixAbbreviatedIRIString);
				abbreviated = true;
			}
			if (attrNameString.isEmpty()) {
				attrNameString = parseStackObj->mAtts.value(mURIString);
				abbreviated = true;
			}
			if (attrNameString.isEmpty()) {
				attrNameString = parseStackObj->mAtts.value(mOWLPrefixURIString);
				abbreviated = true;
			}
			if (abbreviated) {
				entityName = mOntoBuilder->resolveAbbreviationName(attrNameString);
			} else {
				entityName = attrNameString;
			}

			return entityName;
		}

		QString CXMLOWL2Handler::getCardinality(CParseStackObject* parseStackObj) {
			QString cardinalityString;
			cardinalityString = parseStackObj->mAtts.value(mCardinalityString);
			if (cardinalityString.isEmpty()) {
				cardinalityString = parseStackObj->mAtts.value(mOWLPrefixCardinalityString);
			}

			return cardinalityString;
		}


		QString CXMLOWL2Handler::getNodeID(CParseStackObject* parseStackObj) {
			QString nodeIDString;
			nodeIDString = parseStackObj->mAtts.value(mNodeIDString);
			if (nodeIDString.isEmpty()) {
				nodeIDString = parseStackObj->mAtts.value(mOWLPrefixNodeIDString);
			}

			return nodeIDString;
		}


		bool CXMLOWL2Handler::initializeParseFunctionJumpHash() {

			mOWLString = QString("http://www.w3.org/2002/07/owl#");
			mOWLPrefixString = QString("owl");

			mURIString = QString("URI");
			mOWLPrefixURIString = mOWLPrefixString+QString(":")+mURIString;

			mIRIString = QString("IRI");
			mOWLPrefixIRIString = mOWLPrefixString+QString(":")+mIRIString;

			mAbbreviatedIRIString = QString("abbreviatedIRI");
			mOWLPrefixAbbreviatedIRIString = mOWLPrefixString+QString(":")+mAbbreviatedIRIString;

			mCardinalityString = QString("cardinality");
			mOWLPrefixCardinalityString = mOWLPrefixString+QString(":")+mCardinalityString;

			mNodeIDString = QString("nodeID");
			mOWLPrefixNodeIDString = mOWLPrefixString+QString(":")+mNodeIDString;

			mNameString = QString("name");
			mOWLPrefixNameString = mOWLPrefixString+QString(":")+mNameString;




			mParseFunctionJumpHash.insert("Prefix",&CXMLOWL2Handler::jumpFunctionParsePrefixNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":Prefix",&CXMLOWL2Handler::jumpFunctionParsePrefixNode);

			mParseFunctionJumpHash.insert("Class",&CXMLOWL2Handler::jumpFunctionParseClassNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":Class",&CXMLOWL2Handler::jumpFunctionParseClassNode);
			mParseFunctionJumpHash.insert("OWLClass",&CXMLOWL2Handler::jumpFunctionParseClassNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":OWLClass",&CXMLOWL2Handler::jumpFunctionParseClassNode);
			mParseFunctionJumpHash.insert("ObjectProperty",&CXMLOWL2Handler::jumpFunctionParseObjectPropertyNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectProperty",&CXMLOWL2Handler::jumpFunctionParseObjectPropertyNode);
			mParseFunctionJumpHash.insert("IndividualVariable",&CXMLOWL2Handler::jumpFunctionParseIndividualVariable);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":IndividualVariable",&CXMLOWL2Handler::jumpFunctionParseIndividualVariable);
			mParseFunctionJumpHash.insert("Variable",&CXMLOWL2Handler::jumpFunctionParseIndividualVariable);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":Variable",&CXMLOWL2Handler::jumpFunctionParseIndividualVariable);

			mParseFunctionJumpHash.insert("Declaration",&CXMLOWL2Handler::jumpFunctionParseDeclarationNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":Declaration",&CXMLOWL2Handler::jumpFunctionParseDeclarationNode);

			mParseFunctionJumpHash.insert("SubClassOf",&CXMLOWL2Handler::jumpFunctionParseSubClassOfNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":SubClassOf",&CXMLOWL2Handler::jumpFunctionParseSubClassOfNode);
			mParseFunctionJumpHash.insert("EquivalentClasses",&CXMLOWL2Handler::jumpFunctionParseEquivalentClassesNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":EquivalentClasses",&CXMLOWL2Handler::jumpFunctionParseEquivalentClassesNode);
			mParseFunctionJumpHash.insert("DisjointClasses",&CXMLOWL2Handler::jumpFunctionParseDisjontClassesNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":DisjointClasses",&CXMLOWL2Handler::jumpFunctionParseDisjontClassesNode);
			mParseFunctionJumpHash.insert("DisjointUnion",&CXMLOWL2Handler::jumpFunctionParseDisjontUnionNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":DisjointUnion",&CXMLOWL2Handler::jumpFunctionParseDisjontUnionNode);

			mParseFunctionJumpHash.insert("SubObjectPropertyOf",&CXMLOWL2Handler::jumpFunctionParseSubObjectPropertyOfNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":SubObjectPropertyOf",&CXMLOWL2Handler::jumpFunctionParseSubObjectPropertyOfNode);
			mParseFunctionJumpHash.insert("ObjectPropertyChain",&CXMLOWL2Handler::jumpFunctionParseObjectPropertyChainNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectPropertyChain",&CXMLOWL2Handler::jumpFunctionParseObjectPropertyChainNode);
			mParseFunctionJumpHash.insert("EquivalentObjectProperties",&CXMLOWL2Handler::jumpFunctionParseEquivalentObjectPropertiesNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":EquivalentObjectProperties",&CXMLOWL2Handler::jumpFunctionParseEquivalentObjectPropertiesNode);
			mParseFunctionJumpHash.insert("DisjointObjectProperties",&CXMLOWL2Handler::jumpFunctionParseDisjointObjectPropertiesNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":DisjointObjectProperties",&CXMLOWL2Handler::jumpFunctionParseDisjointObjectPropertiesNode);

			mParseFunctionJumpHash.insert("ObjectInverseOf",&CXMLOWL2Handler::jumpFunctionParseInverseObjectPropertyOfNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectInverseOf",&CXMLOWL2Handler::jumpFunctionParseInverseObjectPropertyOfNode);
			mParseFunctionJumpHash.insert("InverseObjectProperties",&CXMLOWL2Handler::jumpFunctionParseInverseObjectPropertiesNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":InverseObjectProperties",&CXMLOWL2Handler::jumpFunctionParseInverseObjectPropertiesNode);
			mParseFunctionJumpHash.insert("ObjectPropertyRange",&CXMLOWL2Handler::jumpFunctionParseObjectPropertyRangeNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectPropertyRange",&CXMLOWL2Handler::jumpFunctionParseObjectPropertyRangeNode);
			mParseFunctionJumpHash.insert("ObjectPropertyDomain",&CXMLOWL2Handler::jumpFunctionParseObjectPropertyDomainNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectPropertyDomain",&CXMLOWL2Handler::jumpFunctionParseObjectPropertyDomainNode);
			mParseFunctionJumpHash.insert("TransitiveObjectProperty",&CXMLOWL2Handler::jumpFunctionParseTransetiveObjectPropertyNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":TransitiveObjectProperty",&CXMLOWL2Handler::jumpFunctionParseTransetiveObjectPropertyNode);
			mParseFunctionJumpHash.insert("FunctionalObjectProperty",&CXMLOWL2Handler::jumpFunctionParseFunctionalObjectPropertyNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":FunctionalObjectProperty",&CXMLOWL2Handler::jumpFunctionParseFunctionalObjectPropertyNode);
			mParseFunctionJumpHash.insert("SymmetricObjectProperty",&CXMLOWL2Handler::jumpFunctionParseSymmetricObjectPropertyNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":SymmetricObjectProperty",&CXMLOWL2Handler::jumpFunctionParseSymmetricObjectPropertyNode);
			mParseFunctionJumpHash.insert("AsymmetricObjectProperty",&CXMLOWL2Handler::jumpFunctionParseAsymmetricObjectPropertyNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":AsymmetricObjectProperty",&CXMLOWL2Handler::jumpFunctionParseAsymmetricObjectPropertyNode);
			mParseFunctionJumpHash.insert("ReflexiveObjectProperty",&CXMLOWL2Handler::jumpFunctionParseReflexiveObjectPropertyNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ReflexiveObjectProperty",&CXMLOWL2Handler::jumpFunctionParseReflexiveObjectPropertyNode);
			mParseFunctionJumpHash.insert("IrreflexiveObjectProperty",&CXMLOWL2Handler::jumpFunctionParseIrreflexiveObjectPropertyNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":IrreflexiveObjectProperty",&CXMLOWL2Handler::jumpFunctionParseIrreflexiveObjectPropertyNode);
			mParseFunctionJumpHash.insert("InverseFunctionalObjectProperty",&CXMLOWL2Handler::jumpFunctionParseInverseFunctionalObjectPropertyNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":InverseFunctionalObjectProperty",&CXMLOWL2Handler::jumpFunctionParseInverseFunctionalObjectPropertyNode);

			mParseFunctionJumpHash.insert("ObjectIntersectionOf",&CXMLOWL2Handler::jumpFunctionParseObjectIntersectionOfNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectIntersectionOf",&CXMLOWL2Handler::jumpFunctionParseObjectIntersectionOfNode);
			mParseFunctionJumpHash.insert("ObjectUnionOf",&CXMLOWL2Handler::jumpFunctionParseObjectUnionOfNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectUnionOf",&CXMLOWL2Handler::jumpFunctionParseObjectUnionOfNode);
			mParseFunctionJumpHash.insert("ObjectMaxCardinality",&CXMLOWL2Handler::jumpFunctionParseObjectMaxCardinalityNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectMaxCardinality",&CXMLOWL2Handler::jumpFunctionParseObjectMaxCardinalityNode);
			mParseFunctionJumpHash.insert("ObjectMinCardinality",&CXMLOWL2Handler::jumpFunctionParseObjectMinCardinalityNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectMinCardinality",&CXMLOWL2Handler::jumpFunctionParseObjectMinCardinalityNode);
			mParseFunctionJumpHash.insert("ObjectExactCardinality",&CXMLOWL2Handler::jumpFunctionParseObjectExactlyCardinalityNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectExactCardinality",&CXMLOWL2Handler::jumpFunctionParseObjectExactlyCardinalityNode);
			mParseFunctionJumpHash.insert("ObjectSomeValuesFrom",&CXMLOWL2Handler::jumpFunctionParseObjectSomeValuesFromNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectSomeValuesFrom",&CXMLOWL2Handler::jumpFunctionParseObjectSomeValuesFromNode);
			mParseFunctionJumpHash.insert("ObjectAllValuesFrom",&CXMLOWL2Handler::jumpFunctionParseObjectAllValuesFromNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectAllValuesFrom",&CXMLOWL2Handler::jumpFunctionParseObjectAllValuesFromNode);
			mParseFunctionJumpHash.insert("ObjectComplementOf",&CXMLOWL2Handler::jumpFunctionParseObjectComplementOfNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectComplementOf",&CXMLOWL2Handler::jumpFunctionParseObjectComplementOfNode);
			mParseFunctionJumpHash.insert("ObjectOneOf",&CXMLOWL2Handler::jumpFunctionParseObjectOneOfNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectOneOf",&CXMLOWL2Handler::jumpFunctionParseObjectOneOfNode);
			mParseFunctionJumpHash.insert("ObjectHasValue",&CXMLOWL2Handler::jumpFunctionParseObjectHasValueNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectHasValue",&CXMLOWL2Handler::jumpFunctionParseObjectHasValueNode);
			mParseFunctionJumpHash.insert("ObjectHasSelf",&CXMLOWL2Handler::jumpFunctionParseObjectHasSelfNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectHasSelf",&CXMLOWL2Handler::jumpFunctionParseObjectHasSelfNode);

			mParseFunctionJumpHash.insert("NamedIndividual",&CXMLOWL2Handler::jumpFunctionParseNamedIndividualNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":NamedIndividual",&CXMLOWL2Handler::jumpFunctionParseNamedIndividualNode);
			mParseFunctionJumpHash.insert("AnonymousIndividual",&CXMLOWL2Handler::jumpFunctionParseAnonymousIndividualNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":AnonymousIndividual",&CXMLOWL2Handler::jumpFunctionParseAnonymousIndividualNode);
			mParseFunctionJumpHash.insert("ClassAssertion",&CXMLOWL2Handler::jumpFunctionParseClassAssertionNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ClassAssertion",&CXMLOWL2Handler::jumpFunctionParseClassAssertionNode);
			mParseFunctionJumpHash.insert("ObjectPropertyAssertion",&CXMLOWL2Handler::jumpFunctionParseObjectPropertyAssertionNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectPropertyAssertion",&CXMLOWL2Handler::jumpFunctionParseObjectPropertyAssertionNode);
			mParseFunctionJumpHash.insert("NegativeObjectPropertyAssertion",&CXMLOWL2Handler::jumpFunctionParseNegativeObjectPropertyAssertionNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":NegativeObjectPropertyAssertion",&CXMLOWL2Handler::jumpFunctionParseNegativeObjectPropertyAssertionNode);
			mParseFunctionJumpHash.insert("SameIndividual",&CXMLOWL2Handler::jumpFunctionParseSameIndividualNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":SameIndividual",&CXMLOWL2Handler::jumpFunctionParseSameIndividualNode);
			mParseFunctionJumpHash.insert("DifferentIndividuals",&CXMLOWL2Handler::jumpFunctionParseDifferentIndividualsNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":DifferentIndividuals",&CXMLOWL2Handler::jumpFunctionParseDifferentIndividualsNode);

			mParseFunctionJumpHash.insert("#text",&CXMLOWL2Handler::jumpFunctionParseIgnoredNode);
			mParseFunctionJumpHash.insert("#comment",&CXMLOWL2Handler::jumpFunctionParseIgnoredNode);
			mParseFunctionJumpHash.insert("ExpectedResult",&CXMLOWL2Handler::jumpFunctionParseIgnoredNode);

			mParseFunctionJumpHash.insert("Ontology",&CXMLOWL2Handler::jumpFunctionTellOntologyAxiomNode);
			mParseFunctionJumpHash.insert("Tell",&CXMLOWL2Handler::jumpFunctionTellOntologyAxiomNode);
			mParseFunctionJumpHash.insert("Whereby",&CXMLOWL2Handler::jumpFunctionTellOntologyAxiomNode);
			mParseFunctionJumpHash.insert("Retract",&CXMLOWL2Handler::jumpFunctionRetractOntologyAxiomNode);


			mParseFunctionJumpHash.insert("DLSafeRule",&CXMLOWL2Handler::jumpFunctionParseRuleNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":DLSafeRule",&CXMLOWL2Handler::jumpFunctionParseRuleNode);

			mParseFunctionJumpHash.insert("Head",&CXMLOWL2Handler::jumpFunctionParseRuleAtomHeadNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":Head",&CXMLOWL2Handler::jumpFunctionParseRuleAtomHeadNode);
			mParseFunctionJumpHash.insert("Body",&CXMLOWL2Handler::jumpFunctionParseRuleAtomBodyNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":Body",&CXMLOWL2Handler::jumpFunctionParseRuleAtomBodyNode);
			mParseFunctionJumpHash.insert("ObjectPropertyAtom",&CXMLOWL2Handler::jumpFunctionParseRulePropertyAtomNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ObjectPropertyAtom",&CXMLOWL2Handler::jumpFunctionParseRulePropertyAtomNode);
			mParseFunctionJumpHash.insert("ClassAtom",&CXMLOWL2Handler::jumpFunctionParseRuleClassAtomNode);
			mParseFunctionJumpHash.insert(mOWLPrefixString+":ClassAtom",&CXMLOWL2Handler::jumpFunctionParseRuleClassAtomNode);

			return true;
		}




		CRulePropertyAtomTermExpression* CXMLOWL2Handler::parseRuleObjectPropertyAtomNode(CParseStackObject* parseStackObj) {
			CRulePropertyAtomTermExpression* rulePropertyAtomExp = nullptr;
			if (parseStackObj->hasExpressions()) {
				CParsingExpressionSplitter mParsingExpSpl(parseStackObj->getExpressions());
				if (mParsingExpSpl.proofExpressionComposition(0,1,0,2,0)) {
					rulePropertyAtomExp = new CRulePropertyAtomTermExpression(mParsingExpSpl.getFirstClassVariableTermExpression(),mParsingExpSpl.getSecondClassVariableTermExpression(),mParsingExpSpl.getFirstObjectPropertyTermExpression());
					mRuleExpContainer.append(rulePropertyAtomExp);
				}
			}
			return rulePropertyAtomExp;
		}

		CRuleClassAtomTermExpression* CXMLOWL2Handler::parseRuleClassAtomNode(CParseStackObject* parseStackObj) {
			CRuleClassAtomTermExpression* ruleClassAtomExp = nullptr;
			if (parseStackObj->hasExpressions()) {
				CParsingExpressionSplitter mParsingExpSpl(parseStackObj->getExpressions());
				if (mParsingExpSpl.proofExpressionComposition(2,0,0,1,1)) {
					ruleClassAtomExp = new CRuleClassAtomTermExpression(mParsingExpSpl.getFirstClassTermExpression(),mParsingExpSpl.getFirstClassVariableTermExpression());
					mRuleExpContainer.append(ruleClassAtomExp);
				}
			}
			return ruleClassAtomExp;
		}


		CRuleAtomListBodyExpression* CXMLOWL2Handler::parseRuleAtomBodyNode(CParseStackObject* parseStackObj) {
			CRuleAtomListBodyExpression* ruleAtomBodyExp = nullptr;
			if (parseStackObj->hasExpressions()) {
				QList<CBuildExpression*>* expList = &parseStackObj->mExpList;
				QList<CRuleAtomTermExpression*> atomExpList;
				for (QList<CBuildExpression*>::const_iterator it = expList->constBegin(), itEnd = expList->constEnd(); it != itEnd; ++it) {
					CRuleAtomTermExpression* ruleAtTeExp = dynamic_cast<CRuleAtomTermExpression*>(*it);
					if (ruleAtTeExp) {
						atomExpList.append(ruleAtTeExp);
					}
				}
				if (!atomExpList.isEmpty()) {
					ruleAtomBodyExp = new CRuleAtomListBodyExpression(atomExpList);
					mRuleExpContainer.append(ruleAtomBodyExp);
				}
			}
			return ruleAtomBodyExp;
		}


		CRuleAtomListHeadExpression* CXMLOWL2Handler::parseRuleAtomHeadNode(CParseStackObject* parseStackObj) {
			CRuleAtomListHeadExpression* ruleAtomHeadExp = nullptr;
			if (parseStackObj->hasExpressions()) {
				QList<CBuildExpression*>* expList = &parseStackObj->mExpList;
				QList<CRuleAtomTermExpression*> atomExpList;
				for (QList<CBuildExpression*>::const_iterator it = expList->constBegin(), itEnd = expList->constEnd(); it != itEnd; ++it) {
					CRuleAtomTermExpression* ruleAtTeExp = dynamic_cast<CRuleAtomTermExpression*>(*it);
					if (ruleAtTeExp) {
						atomExpList.append(ruleAtTeExp);
					}
				}
				if (!atomExpList.isEmpty()) {
					ruleAtomHeadExp = new CRuleAtomListHeadExpression(atomExpList);
					mRuleExpContainer.append(ruleAtomHeadExp);
				}
			}
			return ruleAtomHeadExp;
		}



		QHash<QString,CParseRuleAtom*> CXMLOWL2Handler::createVariableAtomHash(const QList<CParseRuleAtom*>& atomList) {
			QHash<QString,CParseRuleAtom*> variableAtomHash;
			foreach (CParseRuleAtom* atom, atomList) {
				if (!atom->mVariableName1.isEmpty()) {
					variableAtomHash.insertMulti(atom->mVariableName1,atom);
				}
				if (!atom->mVariableName2.isEmpty()) {
					variableAtomHash.insertMulti(atom->mVariableName2,atom);
				}
			}
			return variableAtomHash;
		}



		QMap<cint64,QString> CXMLOWL2Handler::createVariableCountMap(const QList<CParseRuleAtom*>& atomList) {
			QHash<QString,cint64> variableCountHash;
			foreach (CParseRuleAtom* atom, atomList) {
				if (!atom->mVariableName1.isEmpty()) {
					cint64 prevCount = variableCountHash.value(atom->mVariableName1,0);
					variableCountHash.insert(atom->mVariableName1,prevCount+1);
				}
				if (!atom->mVariableName2.isEmpty()) {
					cint64 prevCount = variableCountHash.value(atom->mVariableName2,0);
					variableCountHash.insert(atom->mVariableName2,prevCount+1);
				}
			}
			QMap<cint64,QString> countVariableMap;
			for (QHash<QString,cint64>::const_iterator it = variableCountHash.constBegin(), itEnd = variableCountHash.constEnd(); it != itEnd; ++it) {
				const QString& variableName = it.key();
				cint64 variableAtomCount = it.value();
				countVariableMap.insertMulti(variableAtomCount,variableName);
			}
			return countVariableMap;
		}



		QList<CParseRuleAtom*> CXMLOWL2Handler::parseRuleAtomNodes(CParseStackObject* parseStackObj, const CQtList<CBuildExpression*>& buildExpList, bool parseHeadAtoms) {
			QList<CParseRuleAtom*> atomList;
			for (CQtList<CBuildExpression*>::const_iterator it1 = buildExpList.constBegin(), it1End = buildExpList.constEnd(); it1 != it1End; ++it1) {
				CRuleAtomListExpression* buildListExp = nullptr;
				CRuleAtomListBodyExpression* buildBodyListExp = dynamic_cast<CRuleAtomListBodyExpression*>(*it1);
				CRuleAtomListHeadExpression* buildHeadListExp = dynamic_cast<CRuleAtomListHeadExpression*>(*it1);
				if (parseHeadAtoms) {
					buildListExp = buildHeadListExp;
				}
				if (!parseHeadAtoms) {
					buildListExp = buildBodyListExp;
				}
				if (buildListExp) {
					CEXPRESSIONLIST<CRuleAtomTermExpression*>* ruleAtomExpList = buildListExp->getRuleAtomTermExpressionList();
					for (CEXPRESSIONLIST<CRuleAtomTermExpression*>::const_iterator it2 = ruleAtomExpList->constBegin(), it2End = ruleAtomExpList->constEnd(); it2 != it2End; ++it2) {
						CRuleAtomTermExpression* ruleAtomExp = *it2;

						CParseRuleAtom* ruleAtom = nullptr;

						if (ruleAtomExp->getType() == CBuildExpression::BETRULEPROPERTYATOM) {
							CRulePropertyAtomTermExpression* rulePropertyAtomExp = (CRulePropertyAtomTermExpression*)ruleAtomExp;
							CEXPRESSIONLIST<CObjectIndividualVariableExpression*>* varList = rulePropertyAtomExp->getClassVariableTermExpressionList();
							ruleAtom = new CParseRuleAtom(CParseRuleAtom::PROPERTYATOM);
							ruleAtom->mVariableName1 = varList->first()->getName();
							ruleAtom->mVariableName2 = varList->last()->getName();
							ruleAtom->mAtomExpression = rulePropertyAtomExp->getObjectPropertyTermExpression();
							atomList.append(ruleAtom);
						} else if (ruleAtomExp->getType() == CBuildExpression::BETRULECLASSATOM) {
							CRuleClassAtomTermExpression* ruleClassAtomExp = (CRuleClassAtomTermExpression*)ruleAtomExp;
							ruleAtom = new CParseRuleAtom(CParseRuleAtom::CLASSATOM);
							ruleAtom->mVariableName1 = ruleClassAtomExp->getClassVariableExpression()->getName();
							ruleAtom->mAtomExpression = ruleClassAtomExp->getClassTermExpression();
							atomList.append(ruleAtom);
						}

					}
				}

			}

			return atomList;
		}




		void CXMLOWL2Handler::parseRuleNode(CParseStackObject* parseStackObj) {
			if (parseStackObj->hasExpressions()) {
				QList<CBuildExpression*> expressionList;

				QHash<QString,CParseRuleAtom*> bodyVariableAtomHash;
				QHash<QString,CParseRuleAtom*> headVariableAtomHash;

				CQtList<CBuildExpression*> buildExpList(*parseStackObj->getExpressions());

				QList<CParseRuleAtom*> bodyVariableAtomList = parseRuleAtomNodes(parseStackObj,buildExpList,false);
				QList<CParseRuleAtom*> headVariableAtomList = parseRuleAtomNodes(parseStackObj,buildExpList,true);

				bodyVariableAtomHash = createVariableAtomHash(bodyVariableAtomList);
				headVariableAtomHash = createVariableAtomHash(headVariableAtomList);

				QMap<cint64,QString> headCountVariableMap = createVariableCountMap(headVariableAtomList);


				QSet<CParseRuleAtom*> headProcessedAtomSet;

				QList<QString> reversHeadCountVariableList;
				for (QMap<cint64,QString>::const_iterator it = headCountVariableMap.constBegin(), itEnd = headCountVariableMap.constEnd(); it != itEnd; ++it) {
					const QString& varString = it.value();
					reversHeadCountVariableList.prepend(varString);
				}

				for (QList<QString>::const_iterator it = reversHeadCountVariableList.constBegin(), itEnd = reversHeadCountVariableList.constEnd(); it != itEnd; ++it) {
					const QString& varString = *it;

					QSet<CParseRuleAtom*> processedBodyAtoms;
					QList<CParseRuleAtom*> headAtomList = headVariableAtomHash.values(varString);
					QList<CBuildExpression*> headExpressionList;
					foreach (CParseRuleAtom* headAtom, headAtomList) {
						if (!headProcessedAtomSet.contains(headAtom)) {
							headProcessedAtomSet.insert(headAtom);

							if (headAtom->mAtomType == CParseRuleAtom::PROPERTYATOM) {
								bool inversed = false;
								QString otherVarString = headAtom->getOtherVariable(varString,inversed);
								CBuildExpression* otherVarExp = mOntoBuilder->getNominalIndividualVariable(otherVarString,mAxiomNumber);
								CBuildExpression* someExp = mOntoBuilder->getObjectSomeValuesFrom(getNeighbourAtomObjectPropertyExpression(headAtom,inversed),otherVarExp);
								headExpressionList.append(someExp);
							} else if (headAtom->mAtomType == CParseRuleAtom::CLASSATOM) {
								headExpressionList.append(headAtom->mAtomExpression);
							}

						}
					}

					if (!headExpressionList.isEmpty()) {
						CBuildExpression* headExp = nullptr;
						if (headExpressionList.count() >= 2) {
							headExp = mOntoBuilder->getObjectIntersectionOf(headExpressionList);
						} else {
							headExp = headExpressionList.first();
						}

						CBuildExpression* bodyExp = createBodyExpresions(varString,bodyVariableAtomHash,headVariableAtomHash);
						CBuildExpression* axiomExp = mOntoBuilder->getSubClassOf(bodyExp,headExp);
						expressionList.append(axiomExp);
					}

					++mAxiomNumber;
				}

				addTemporaryBuildExpressions(&expressionList);
			}
		}


		CBuildExpression* CXMLOWL2Handler::createBodyExpresions(const QString& varString, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QHash<QString,CParseRuleAtom*>& headVariableAtomHash) {
			QSet<QString> processedVarSet;
			QSet<CParseRuleAtom*> processedAtomSet;
			QList<CBuildExpression*> expList;
			CBuildExpression* mainExp = createBodyExpresions(varString,true,bodyVariableAtomHash,processedVarSet,processedAtomSet,headVariableAtomHash);
			expList.append(mainExp);

			for (QHash<QString,CParseRuleAtom*>::const_iterator it = bodyVariableAtomHash.constBegin(), itEnd = bodyVariableAtomHash.constEnd(); it != itEnd; ++it) {
				QString nextVarString = it.key();
				if (!processedVarSet.contains(nextVarString)) {
					CBuildExpression* topPropExp = mOntoBuilder->getTopObjectProberty();
					CBuildExpression* succExp = createBodyExpresions(nextVarString,true,bodyVariableAtomHash,processedVarSet,processedAtomSet,headVariableAtomHash);
					CBuildExpression* someExp = mOntoBuilder->getObjectSomeValuesFrom(topPropExp,succExp);
					expList.append(someExp);
				}
			}

			if (expList.count() >= 2) {
				return mOntoBuilder->getObjectIntersectionOf(expList);
			}
			return expList.first();
		}

		CBuildExpression* CXMLOWL2Handler::getNeighbourAtomObjectPropertyExpression(CParseRuleAtom* ruleAtom, bool inversed) {
			if (!inversed) {
				return ruleAtom->mAtomExpression;
			} else {
				return mOntoBuilder->getInverseObjectPropertyOf(ruleAtom->mAtomExpression);
			}
		}



		bool CXMLOWL2Handler::hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
				QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash) {
			
			QSet<QString> backTestProcessedVarSet;
			QSet<CParseRuleAtom*> backTestProcessedAtomSet;

			return hasBackExpressionLinks(varString,notRuleAtom,bodyVariableAtomHash,processedVarSet,processedAtomSet,headVariableAtomHash,backTestProcessedVarSet,backTestProcessedAtomSet);
		}



		bool CXMLOWL2Handler::hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
						QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash, QSet<QString>& backTestProcessedVarSet, QSet<CParseRuleAtom*>& backTestProcessedAtomSet) {

			if (processedVarSet.contains(varString)) {
				return false;
			}
			if (!backTestProcessedVarSet.contains(varString)) {
				backTestProcessedVarSet.insert(varString);

				QList<CParseRuleAtom*> atomList = bodyVariableAtomHash.values(varString);
				foreach (CParseRuleAtom* ruleAtom, atomList) {
					if (ruleAtom->mAtomType == CParseRuleAtom::PROPERTYATOM) {
						if (ruleAtom != notRuleAtom) {
							if (processedAtomSet.contains(ruleAtom)) {
								return true;
							}
							if (!backTestProcessedAtomSet.contains(ruleAtom)) {
								backTestProcessedAtomSet.insert(ruleAtom);
								bool inversed = false;
								QString otherVarString = ruleAtom->getOtherVariable(varString,inversed);
								if (hasBackExpressionLinks(otherVarString,ruleAtom,bodyVariableAtomHash,processedVarSet,processedAtomSet,headVariableAtomHash,backTestProcessedVarSet,backTestProcessedAtomSet)) {
									return true;
								}
							}
						}
					}
				}
			}
			return false;
		}


		CBuildExpression* CXMLOWL2Handler::createBodyExpresions(const QString& varString, bool direct, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash) {
			QList<CBuildExpression*> expList;
			expList.append(mOntoBuilder->getNominalIndividualVariable(varString,mAxiomNumber));

			if (!processedVarSet.contains(varString)) {
				processedVarSet.insert(varString);

				QList<CParseRuleAtom*> atomList = bodyVariableAtomHash.values(varString);
				QList<CParseRuleAtom*> processList;
				foreach (CParseRuleAtom* ruleAtom, atomList) {
					if (ruleAtom->mAtomType == CParseRuleAtom::PROPERTYATOM) {
						if (!processedAtomSet.contains(ruleAtom)) {

							bool inversed = false;
							QString otherVarString = ruleAtom->getOtherVariable(varString,inversed);

							if (direct || !headVariableAtomHash.contains(varString) || !hasBackExpressionLinks(otherVarString,ruleAtom,bodyVariableAtomHash,processedVarSet,processedAtomSet,headVariableAtomHash)) {

								processedAtomSet.insert(ruleAtom);
								processList.append(ruleAtom);
							}
						}
					} else if (ruleAtom->mAtomType == CParseRuleAtom::CLASSATOM) {
						processList.append(ruleAtom);
					}
				}
				foreach (CParseRuleAtom* ruleAtom, processList) {

					if (ruleAtom->mAtomType == CParseRuleAtom::PROPERTYATOM) {
						bool inversed = false;
						QString otherVarString = ruleAtom->getOtherVariable(varString,inversed);
						CBuildExpression* succExp = createBodyExpresions(otherVarString,false,bodyVariableAtomHash,processedVarSet,processedAtomSet,headVariableAtomHash);
						CBuildExpression* someExp = mOntoBuilder->getObjectSomeValuesFrom(getNeighbourAtomObjectPropertyExpression(ruleAtom,inversed),succExp);
						expList.append(someExp);
					} else if (ruleAtom->mAtomType == CParseRuleAtom::CLASSATOM) {
						expList.append(ruleAtom->mAtomExpression);
					}

				}
			} 
			if (expList.count() >= 2) {
				return mOntoBuilder->getObjectIntersectionOf(expList);
			}
			return expList.first();
		}



		void CXMLOWL2Handler::parseBuildNode(CParseStackObject* parseStackObj) {
			const QString& nodeString(parseStackObj->mQName);

			ParseFunction function = mParseFunctionJumpHash.value(nodeString);
			if (function) {
				(this->*function)(parseStackObj);
				if (!mStack.isEmpty() && !mTmpExpList.isEmpty()) {
					CParseStackObject* topParsStackObj = mStack.top();
					topParsStackObj->addBuildExpressions(&mTmpExpList);
				}
			} else {
				const QString& notSupportedNodeString(nodeString);
				CNotSupportedNodeErrorRecord::makeRecord(notSupportedNodeString,&recordRouter);
			}
			mTmpExpList.clear();
		}


		CDeclarationAxiomExpression* CXMLOWL2Handler::parseDeclarationNode(CParseStackObject* parseStackObj) {
			CDeclarationAxiomExpression* declarationExpression = nullptr;
			if (parseStackObj->hasExpressions()) {
				declarationExpression = mOntoBuilder->getDeclaration(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return declarationExpression;
		}


		CObjectPropertyExpression *CXMLOWL2Handler::parseObjectPropertyNode(CParseStackObject* parseStackObj) {
			const QString& probName = getEntityName(parseStackObj);
			CObjectPropertyExpression *objectExpression = 0;
			if (!probName.isEmpty()) {
				objectExpression = mOntoBuilder->getObjectProberty(probName);
			}
			return objectExpression;
		}


		CClassExpression *CXMLOWL2Handler::parseClassNode(CParseStackObject* parseStackObj) {
			const QString& className = getEntityName(parseStackObj);
			CClassExpression *classExpression = 0;
			if (!className.isEmpty()) {
				classExpression = mOntoBuilder->getClass(className);
			}
			return classExpression;
		}


		CObjectIndividualVariableExpression* CXMLOWL2Handler::parseIndividualVariableNode(CParseStackObject* parseStackObj) {
			const QString& indiVarName = getEntityName(parseStackObj);
			CObjectIndividualVariableExpression *indiVarExpression = 0;
			if (!indiVarName.isEmpty()) {
				indiVarExpression = mOntoBuilder->getNominalIndividualVariable(indiVarName,mAxiomNumber);
			}
			return indiVarExpression;
		}




		CEquivalentClassesExpression *CXMLOWL2Handler::parseEquivalentClassesNode(CParseStackObject* parseStackObj) {
			CEquivalentClassesExpression *ecExp = 0;
			if (parseStackObj->hasExpressions()) {
				ecExp = mOntoBuilder->getEquivalentClasses(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return ecExp;
		}


		CSubClassOfExpression *CXMLOWL2Handler::parseSubClassOfNode(CParseStackObject* parseStackObj) {
			CSubClassOfExpression *scoExp = 0;
			if (parseStackObj->hasExpressions()) {
				scoExp = mOntoBuilder->getSubClassOf(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return scoExp;
		}


		CObjectPropertyDomainExpression *CXMLOWL2Handler::parseObjectPropertyDomainNode(CParseStackObject* parseStackObj) {
			CObjectPropertyDomainExpression *obPrDoExp = 0;
			if (parseStackObj->hasExpressions()) {
				obPrDoExp = mOntoBuilder->getObjectPropertyDomainExpression(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return obPrDoExp;
		}

		CObjectPropertyRangeExpression *CXMLOWL2Handler::parseObjectPropertyRangeNode(CParseStackObject* parseStackObj) {
			CObjectPropertyRangeExpression *obPrRaExp = 0;
			if (parseStackObj->hasExpressions()) {
				obPrRaExp = mOntoBuilder->getObjectPropertyRangeExpression(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return obPrRaExp;
		}


		CSubObjectPropertyOfExpression *CXMLOWL2Handler::parseSubObjectPropertyOfNode(CParseStackObject* parseStackObj) {
			CSubObjectPropertyOfExpression *sopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				sopoExp = mOntoBuilder->getSubObjectPropertyOf(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return sopoExp;
		}

		CObjectPropertyChainExpression *CXMLOWL2Handler::parseObjectPropertyChainNode(CParseStackObject* parseStackObj) {
			CObjectPropertyChainExpression *sopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				sopoExp = mOntoBuilder->getObjectPropertyChain(*parseStackObj->getExpressions());
			}
			return sopoExp;
		}

		CEquivalentObjectPropertiesExpression *CXMLOWL2Handler::parseEquivalentObjectPropertiesNode(CParseStackObject* parseStackObj) {
			CEquivalentObjectPropertiesExpression *sopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				sopoExp = mOntoBuilder->getEquivalentObjectProperties(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return sopoExp;
		}

		CDisjointObjectPropertiesExpression *CXMLOWL2Handler::parseDisjointObjectPropertiesNode(CParseStackObject* parseStackObj) {
			CDisjointObjectPropertiesExpression *sopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				sopoExp = mOntoBuilder->getDisjointObjectProperties(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return sopoExp;
		}

		CInverseObjectPropertyOfExpression *CXMLOWL2Handler::parseInverseObjectPropertyOfNode(CParseStackObject* parseStackObj) {
			CInverseObjectPropertyOfExpression *iopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				iopoExp = mOntoBuilder->getInverseObjectPropertyOf(*parseStackObj->getExpressions());
			}
			return iopoExp;
		}

		CInverseObjectPropertiesExpression *CXMLOWL2Handler::parseInverseObjectPropertiesNode(CParseStackObject* parseStackObj) {
			CInverseObjectPropertiesExpression *iopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				iopoExp = mOntoBuilder->getInverseObjectProperties(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return iopoExp;
		}


		CDisjointClassesExpression *CXMLOWL2Handler::parseDisjontClassesNode(CParseStackObject* parseStackObj) {
			CDisjointClassesExpression *disClassExp = 0;
			if (parseStackObj->hasExpressions()) {
				disClassExp = mOntoBuilder->getDisjointClasses(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return disClassExp;
		}

		CDisjointUnionExpression *CXMLOWL2Handler::parseDisjontUnionNode(CParseStackObject* parseStackObj) {
			CDisjointUnionExpression *disClassExp = 0;
			if (parseStackObj->hasExpressions()) {
				disClassExp = mOntoBuilder->getDisjointUnion(*parseStackObj->getExpressions());
			}
			return disClassExp;
		}



		CObjectIntersectionOfExpression *CXMLOWL2Handler::parseObjectIntersectionOfNode(CParseStackObject* parseStackObj) {
			CObjectIntersectionOfExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectIntersectionOf(*parseStackObj->getExpressions());
			}
			return exp;
		}

		CObjectUnionOfExpression *CXMLOWL2Handler::parseObjectUnionOfNode(CParseStackObject* parseStackObj) {
			CObjectUnionOfExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectUnionOf(*parseStackObj->getExpressions());
			}
			return exp;
		}

		CObjectMaxCardinalityExpression *CXMLOWL2Handler::parseObjectMaxCardinalityNode(CParseStackObject* parseStackObj) {
			QString cardinalityName = getCardinality(parseStackObj);
			CObjectMaxCardinalityExpression *exp = 0;
			if (!cardinalityName.isEmpty()) {
				exp = mOntoBuilder->getObjectMaxCardinality(*parseStackObj->getExpressions(),cardinalityName.toInt());
			}
			return exp;
		}

		CObjectMinCardinalityExpression *CXMLOWL2Handler::parseObjectMinCardinalityNode(CParseStackObject* parseStackObj) {
			QString cardinalityName = getCardinality(parseStackObj);
			CObjectMinCardinalityExpression *exp = 0;
			if (!cardinalityName.isEmpty()) {
				exp = mOntoBuilder->getObjectMinCardinality(*parseStackObj->getExpressions(),cardinalityName.toInt());
			}
			return exp;
		}

		CObjectExactlyCardinalityExpression *CXMLOWL2Handler::parseObjectExactlyCardinalityNode(CParseStackObject* parseStackObj) {
			QString cardinalityName = getCardinality(parseStackObj);
			CObjectExactlyCardinalityExpression *exp = 0;
			if (!cardinalityName.isEmpty()) {
				exp = mOntoBuilder->getObjectExactlyCardinality(*parseStackObj->getExpressions(),cardinalityName.toInt());
			}
			return exp;
		}

		CObjectSomeValuesFromExpression *CXMLOWL2Handler::parseObjectSomeValuesFromNode(CParseStackObject* parseStackObj) {
			CObjectSomeValuesFromExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectSomeValuesFrom(*parseStackObj->getExpressions());
			}
			return exp;
		}

		CObjectHasValueExpression* CXMLOWL2Handler::parseObjectHasValueNode(CParseStackObject* parseStackObj) {
			CObjectHasValueExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectHasValue(*parseStackObj->getExpressions());
			}
			return exp;
		}

		CObjectHasSelfExpression* CXMLOWL2Handler::parseObjectHasSelfNode(CParseStackObject* parseStackObj) {
			CObjectHasSelfExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectHasSelf(*parseStackObj->getExpressions());
			}
			return exp;
		}


		CObjectAllValuesFromExpression *CXMLOWL2Handler::parseObjectAllValuesFromNode(CParseStackObject* parseStackObj) {
			CObjectAllValuesFromExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectAllValuesFrom(*parseStackObj->getExpressions());
			}
			return exp;
		}


		CObjectComplementOfExpression *CXMLOWL2Handler::parseObjectComplementOfNode(CParseStackObject* parseStackObj) {
			CObjectComplementOfExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectComplementOf(*parseStackObj->getExpressions());
			}
			return exp;
		}

		CObjectOneOfExpression *CXMLOWL2Handler::parseObjectOneOfNode(CParseStackObject* parseStackObj) {
			CObjectOneOfExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectOneOf(*parseStackObj->getExpressions());
			}
			return exp;
		}


		CTransetiveObjectPropertyExpression *CXMLOWL2Handler::parseTransetiveObjectPropertyNode(CParseStackObject* parseStackObj) {
			CTransetiveObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getTransetiveObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}


		CFunctionalObjectPropertyExpression *CXMLOWL2Handler::parseFunctionalObjectPropertyNode(CParseStackObject* parseStackObj) {
			CFunctionalObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getFunctionalObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}


		CSymmetricObjectPropertyExpression* CXMLOWL2Handler::parseSymmetricObjectPropertyNode(CParseStackObject* parseStackObj) {
			CSymmetricObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getSymmetricObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CAsymmetricObjectPropertyExpression* CXMLOWL2Handler::parseAsymmetricObjectPropertyNode(CParseStackObject* parseStackObj) {
			CAsymmetricObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getAsymmetricObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CReflexiveObjectPropertyExpression* CXMLOWL2Handler::parseReflexiveObjectPropertyNode(CParseStackObject* parseStackObj) {
			CReflexiveObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getReflexiveObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CIrreflexiveObjectPropertyExpression* CXMLOWL2Handler::parseIrreflexiveObjectPropertyNode(CParseStackObject* parseStackObj) {
			CIrreflexiveObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getIrreflexiveObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CInverseFunctionalObjectPropertyExpression* CXMLOWL2Handler::parseInverseFunctionalObjectPropertyNode(CParseStackObject* parseStackObj) {
			CInverseFunctionalObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getInverseFunctionalObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}





		CClassAssertionExpression *CXMLOWL2Handler::parseClassAssertionNode(CParseStackObject* parseStackObj) {
			CClassAssertionExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getClassAssertion(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}


		CSameIndividualExpression *CXMLOWL2Handler::parseSameIndividualNode(CParseStackObject* parseStackObj) {
			CSameIndividualExpression *exp = 0;
			if (mOntoBuilder && parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getSameIndividual(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CDifferentIndividualsExpression *CXMLOWL2Handler::parseDifferentIndividualsNode(CParseStackObject* parseStackObj) {
			CDifferentIndividualsExpression *exp = 0;
			if (mOntoBuilder && parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getDifferentIndividuals(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}


		CNamedIndividualExpression *CXMLOWL2Handler::parseNamedIndividualNode(CParseStackObject* parseStackObj) {
			const QString& indiNameName = getEntityName(parseStackObj);
			CNamedIndividualExpression *exp = 0;
			if (!indiNameName.isEmpty()) {
				if (mOntoBuilder) {
					exp = mOntoBuilder->getNamedIndividual(indiNameName);
				}
			}
			return exp;
		}


		CAnonymousIndividualExpression *CXMLOWL2Handler::parseAnonymousIndividualNode(CParseStackObject* parseStackObj) {
			if (mOntologyName.isEmpty()) {
				mOntologyName = mOntoBuilder->getOntologyName();
			}
			const QString& indiNameName = getNodeID(parseStackObj);
			CAnonymousIndividualExpression *exp = 0;
			if (!indiNameName.isEmpty()) {
				if (mOntoBuilder) {
					exp = mOntoBuilder->getAnonymousIndividual(mOntologyName,indiNameName);
				}
			}
			return exp;
		}

		CObjectPropertyAssertionExpression *CXMLOWL2Handler::parseObjectPropertyAssertionNode(CParseStackObject* parseStackObj) {
			CObjectPropertyAssertionExpression *exp = 0;
			if (mOntoBuilder && parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectPropertyAssertion(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CNegativeObjectPropertyAssertionExpression *CXMLOWL2Handler::parseNegativeObjectPropertyAssertionNode(CParseStackObject* parseStackObj) {
			CNegativeObjectPropertyAssertionExpression *exp = 0;
			if (mOntoBuilder && parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getNegativeObjectPropertyAssertion(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}






		void CXMLOWL2Handler::jumpFunctionParseRuleNode(CParseStackObject* parseStackObj) {
			parseRuleNode(parseStackObj);
		}

		void CXMLOWL2Handler::jumpFunctionParseRulePropertyAtomNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseRuleObjectPropertyAtomNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseRuleClassAtomNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseRuleClassAtomNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseRuleAtomBodyNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseRuleAtomBodyNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseRuleAtomHeadNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseRuleAtomHeadNode(parseStackObj));
		}






		void CXMLOWL2Handler::jumpFunctionTellOntologyAxiomNode(CParseStackObject* parseStackObj) {
			parseTellOntologyAxiomNode(parseStackObj);
		}

		void CXMLOWL2Handler::jumpFunctionRetractOntologyAxiomNode(CParseStackObject* parseStackObj) {
			parseRetractOntologyAxiomNode(parseStackObj);
		}

		void CXMLOWL2Handler::jumpFunctionParsePrefixNode(CParseStackObject* parseStackObj) {
			parsePrefixNode(parseStackObj);
		}






		void CXMLOWL2Handler::addTemporaryBuildExpression(CBuildExpression* buildExp) {
			if (buildExp) {
				mTmpExpList.append(buildExp);
			}
		}

		void CXMLOWL2Handler::addTemporaryBuildExpressions(QList<CBuildExpression*>* buildExps) {
			if (buildExps) {
				for (QList<CBuildExpression*>::const_iterator it = buildExps->constBegin(), itEnd = buildExps->constEnd(); it != itEnd; ++it) {
					CBuildExpression* buildExp = *it;
					if (buildExp) {
						mTmpExpList.append(buildExp);
					}
				}
			}
		}





		void CXMLOWL2Handler::jumpFunctionParseClassNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseClassNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectPropertyNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectPropertyNode(parseStackObj));
		}


		void CXMLOWL2Handler::jumpFunctionParseIndividualVariable(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseIndividualVariableNode(parseStackObj));
		}


		void CXMLOWL2Handler::jumpFunctionParseDeclarationNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseDeclarationNode(parseStackObj));
		}


		void CXMLOWL2Handler::jumpFunctionParseEquivalentClassesNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseEquivalentClassesNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseSubClassOfNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseSubClassOfNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseDisjontClassesNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseDisjontClassesNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseDisjontUnionNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseDisjontUnionNode(parseStackObj));
		}


		void CXMLOWL2Handler::jumpFunctionParseSubObjectPropertyOfNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseSubObjectPropertyOfNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectPropertyChainNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectPropertyChainNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseEquivalentObjectPropertiesNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseEquivalentObjectPropertiesNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseDisjointObjectPropertiesNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseDisjointObjectPropertiesNode(parseStackObj));
		}


		void CXMLOWL2Handler::jumpFunctionParseObjectIntersectionOfNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectIntersectionOfNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectUnionOfNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectUnionOfNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectMaxCardinalityNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectMaxCardinalityNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectMinCardinalityNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectMinCardinalityNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectExactlyCardinalityNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectExactlyCardinalityNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectSomeValuesFromNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectSomeValuesFromNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectAllValuesFromNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectAllValuesFromNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectComplementOfNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectComplementOfNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectOneOfNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectOneOfNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectHasValueNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectHasValueNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectHasSelfNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectHasSelfNode(parseStackObj));
		}


		void CXMLOWL2Handler::jumpFunctionParseClassAssertionNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseClassAssertionNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseNamedIndividualNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseNamedIndividualNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseAnonymousIndividualNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseAnonymousIndividualNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectPropertyAssertionNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectPropertyAssertionNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseNegativeObjectPropertyAssertionNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseNegativeObjectPropertyAssertionNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseSameIndividualNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseSameIndividualNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseDifferentIndividualsNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseDifferentIndividualsNode(parseStackObj));
		}


		void CXMLOWL2Handler::jumpFunctionParseObjectPropertyDomainNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectPropertyDomainNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseObjectPropertyRangeNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectPropertyRangeNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseInverseObjectPropertyOfNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseInverseObjectPropertyOfNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseInverseObjectPropertiesNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseInverseObjectPropertiesNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseTransetiveObjectPropertyNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseTransetiveObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseFunctionalObjectPropertyNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseFunctionalObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseSymmetricObjectPropertyNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseSymmetricObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseAsymmetricObjectPropertyNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseAsymmetricObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseReflexiveObjectPropertyNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseReflexiveObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseIrreflexiveObjectPropertyNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseIrreflexiveObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseInverseFunctionalObjectPropertyNode(CParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseInverseFunctionalObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2Handler::jumpFunctionParseIgnoredNode(CParseStackObject* parseStackObj) {
		}



	}; // end namespace Parser

}; // end namespace Konclude
