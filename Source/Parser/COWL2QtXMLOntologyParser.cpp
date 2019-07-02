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

#include "COWL2QtXMLOntologyParser.h"


namespace Konclude {

	namespace Parser {



		COWL2QtXMLOntologyParser::COWL2QtXMLOntologyParser(COntologyBuilder* ontoBuilder)
				: CLogIdentifier("::Konclude::Parser::OWL2QtXMLOntologyParser",this),recordRouter(0,this) {

			mOntoBuilder = ontoBuilder;
			mAxiomNumber = 0;

			mReportErrorCount = 100;

			LOG(INFO,getLogDomain(),logTr("Created OWL 2.0 Qt XML Parser."),this);

#ifdef KONCLUDE_PARSER_USEPARSEJUMPFUNCTIONHASH
			initializeParseFunctionJumpHash();
#endif
		}


		COWL2QtXMLOntologyParser::COWL2QtXMLOntologyParser(COntologyBuilder* ontoBuilder, CCommandRecordRouter *commandRecordRouter)
				: CLogIdentifier("::Konclude::Parser::OWL2QtXMLOntologyParser",this),recordRouter(*commandRecordRouter,this) {
				
			mOntoBuilder = ontoBuilder;
			mAxiomNumber = 0;

			mReportErrorCount = 100;

			LOG(INFO,getLogDomain(),logTr("Created OWL 2.0 Qt XML Parser."),this);

#ifdef KONCLUDE_PARSER_USEPARSEJUMPFUNCTIONHASH
			initializeParseFunctionJumpHash();
#endif
		}


		COWL2QtXMLOntologyParser::~COWL2QtXMLOntologyParser() {
		}




		bool COWL2QtXMLOntologyParser::parseOntologyFile(const QString& filename) {
			LOG(INFO,getLogDomain(),logTr("Loading XML File: '%1'.").arg(filename),this);
			bool successfullParsed = true;


			// parse file
			QFile file(filename);
			if (file.open(QIODevice::ReadOnly)) {
				QDomDocument document;
				QString errorMessage;
				int errorLine = 0, errorColumn = 0;
				if (document.setContent(file.readAll(),true,&errorMessage,&errorLine,&errorColumn)) {

					QDomElement documentElement(document.documentElement());

					QString docElemName = documentElement.tagName();
					if (docElemName == "Ontology" || docElemName == "owl:Ontology") {
						LOG(INFO,getLogDomain(),logTr("Found OWL-Ontology Node."),this);
						parseTellOntologyAxiomNode(&documentElement);
					} else if (docElemName == "Tell" || docElemName == "owl:Tell") {
						LOG(INFO,getLogDomain(),logTr("Found Ontology-Tell Node."),this);
						parseTellOntologyAxiomNode(&documentElement);
					} else {
						successfullParsed = false;
						LOG(ERROR,getLogDomain(),logTr("Not supported node '%1'.").arg(docElemName),this);
					}
					LOG(INFO,getLogDomain(),logTr("Parsing of '%1' completed.").arg(filename),this);

				} else {
					LOG(ERROR,getLogDomain(),logTr("XML error: %1, file: %2, line: %3, column %4.").arg(errorMessage).arg(filename).arg(errorLine).arg(errorColumn),this);
				}
				file.close();
			} else {
				successfullParsed = false;
				LOG(ERROR,getLogDomain(),logTr("File '%1' not found or couldn't be opened.").arg(filename),this);
			}

			return successfullParsed;
		}



		bool COWL2QtXMLOntologyParser::parseTellOntologyAxiomNode(QDomElement *ontologyNode) {
			QDomElement node = ontologyNode->firstChildElement();
			while (!node.isNull()) {
				// build for each node an expression
				CBuildExpression* expression = parseBuildNode(&node);
				if (expression) {
					CAxiomExpression* axiomExpression = dynamic_cast<CAxiomExpression*>(expression);
					if (axiomExpression) {
						mOntoBuilder->tellOntologyAxiom(axiomExpression);
					}
				} else {

					QList<CBuildExpression*> expressionList(parseRuleNode(&node));
					foreach (CBuildExpression* expression, expressionList) {
						CAxiomExpression* axiomExpression = dynamic_cast<CAxiomExpression*>(expression);
						if (axiomExpression) {
							mOntoBuilder->tellOntologyAxiom(axiomExpression);
						}
					}
				}

				node = node.nextSiblingElement();
			}
			return true;
		}


		bool COWL2QtXMLOntologyParser::parseOntologyNode(QDomElement* ontologyNode) {
			mOntologyName = ontologyNode->attribute("ontologyIRI");
			parseTellOntologyAxiomNode(ontologyNode);
			return true;
		}



		bool COWL2QtXMLOntologyParser::parseRetractOntologyAxiomNode(QDomElement *ontologyNode) {
			QDomElement node = ontologyNode->firstChildElement();
			while (!node.isNull()) {
				// build for each node an expression
				CBuildExpression* expression = parseBuildNode(&node);
				if (expression) {
					CAxiomExpression* axiomExpression = dynamic_cast<CAxiomExpression*>(expression);
					if (axiomExpression) {
						mOntoBuilder->retractOntologyAxiom(axiomExpression);
					}
				} else {

					QList<CBuildExpression*> expressionList(parseRuleNode(&node));
					foreach (CBuildExpression* expression, expressionList) {
						CAxiomExpression* axiomExpression = dynamic_cast<CAxiomExpression*>(expression);
						if (axiomExpression) {
							mOntoBuilder->retractOntologyAxiom(axiomExpression);
						}
					}
				}

				node = node.nextSiblingElement();
			}
			return true;
		}


		CBuildExpression *COWL2QtXMLOntologyParser::getParsedBuildNode(QDomElement *node) {
			return parseBuildNode(node);
		}

		bool COWL2QtXMLOntologyParser::parsePrefixNode(QDomElement* node) {
			QString abbName = node->attribute(QString("name"));
			QString totalIRIName = node->attribute(QString("IRI"));
			if (!totalIRIName.isEmpty()) {
				mOntoBuilder->addNameAbbreviation(totalIRIName,abbName);
				return true;
			}
			return false;
		}

		QString COWL2QtXMLOntologyParser::getEntityName(QDomElement* node) {
			QString entityName;

			bool abbreviated = false;

			QString attrName = node->attribute(QString("IRI"));
			if (attrName.isEmpty()) {
				attrName = node->attribute(QString("abbreviatedIRI"));
				abbreviated = true;
			}
			if (attrName.isEmpty()) {
				attrName = node->attribute(QString("owl:IRI"));
			}
			if (attrName.isEmpty()) {
				attrName = node->attribute(QString("owl:abbreviatedIRI"));
				abbreviated = true;
			}
			if (attrName.isEmpty()) {
				attrName = node->attribute(QString("URI"));
			} 
			if (attrName.isEmpty()) {
				attrName = node->attribute(QString("owl:URI"));
			}
			if (abbreviated) {
				entityName = mOntoBuilder->resolveAbbreviationName(attrName);
			} else {
				entityName = attrName;
			}

			return entityName;
		}


		bool COWL2QtXMLOntologyParser::initializeParseFunctionJumpHash() {

			mParseFunctionJumpHash.insert("Prefix",&COWL2QtXMLOntologyParser::jumpFunctionParsePrefixNode);
			mParseFunctionJumpHash.insert("owl:Prefix",&COWL2QtXMLOntologyParser::jumpFunctionParsePrefixNode);

			mParseFunctionJumpHash.insert("Class",&COWL2QtXMLOntologyParser::jumpFunctionParseClassNode);
			mParseFunctionJumpHash.insert("owl:Class",&COWL2QtXMLOntologyParser::jumpFunctionParseClassNode);
			mParseFunctionJumpHash.insert("OWLClass",&COWL2QtXMLOntologyParser::jumpFunctionParseClassNode);
			mParseFunctionJumpHash.insert("owl:OWLClass",&COWL2QtXMLOntologyParser::jumpFunctionParseClassNode);
			mParseFunctionJumpHash.insert("ObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyNode);
			mParseFunctionJumpHash.insert("owl:ObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyNode);
			mParseFunctionJumpHash.insert("IndividualVariable",&COWL2QtXMLOntologyParser::jumpFunctionParseIndividualVariable);
			mParseFunctionJumpHash.insert("owl:IndividualVariable",&COWL2QtXMLOntologyParser::jumpFunctionParseIndividualVariable);

			mParseFunctionJumpHash.insert("Declaration",&COWL2QtXMLOntologyParser::jumpFunctionParseDeclarationNode);
			mParseFunctionJumpHash.insert("owl:Declaration",&COWL2QtXMLOntologyParser::jumpFunctionParseDeclarationNode);

			mParseFunctionJumpHash.insert("SubClassOf",&COWL2QtXMLOntologyParser::jumpFunctionParseSubClassOfNode);
			mParseFunctionJumpHash.insert("owl:SubClassOf",&COWL2QtXMLOntologyParser::jumpFunctionParseSubClassOfNode);
			mParseFunctionJumpHash.insert("EquivalentClasses",&COWL2QtXMLOntologyParser::jumpFunctionParseEquivalentClassesNode);
			mParseFunctionJumpHash.insert("owl:EquivalentClasses",&COWL2QtXMLOntologyParser::jumpFunctionParseEquivalentClassesNode);
			mParseFunctionJumpHash.insert("DisjointClasses",&COWL2QtXMLOntologyParser::jumpFunctionParseDisjontClassesNode);
			mParseFunctionJumpHash.insert("owl:DisjointClasses",&COWL2QtXMLOntologyParser::jumpFunctionParseDisjontClassesNode);
			mParseFunctionJumpHash.insert("DisjointUnion",&COWL2QtXMLOntologyParser::jumpFunctionParseDisjontUnionNode);
			mParseFunctionJumpHash.insert("owl:DisjointUnion",&COWL2QtXMLOntologyParser::jumpFunctionParseDisjontUnionNode);

			mParseFunctionJumpHash.insert("SubObjectPropertyOf",&COWL2QtXMLOntologyParser::jumpFunctionParseSubObjectPropertyOfNode);
			mParseFunctionJumpHash.insert("owl:SubObjectPropertyOf",&COWL2QtXMLOntologyParser::jumpFunctionParseSubObjectPropertyOfNode);
			mParseFunctionJumpHash.insert("ObjectPropertyChain",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyChainNode);
			mParseFunctionJumpHash.insert("owl:ObjectPropertyChain",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyChainNode);
			mParseFunctionJumpHash.insert("EquivalentObjectProperties",&COWL2QtXMLOntologyParser::jumpFunctionParseEquivalentObjectPropertiesNode);
			mParseFunctionJumpHash.insert("owl:EquivalentObjectProperties",&COWL2QtXMLOntologyParser::jumpFunctionParseEquivalentObjectPropertiesNode);
			mParseFunctionJumpHash.insert("DisjointObjectProperties",&COWL2QtXMLOntologyParser::jumpFunctionParseDisjointObjectPropertiesNode);
			mParseFunctionJumpHash.insert("owl:DisjointObjectProperties",&COWL2QtXMLOntologyParser::jumpFunctionParseDisjointObjectPropertiesNode);

			mParseFunctionJumpHash.insert("ObjectInverseOf",&COWL2QtXMLOntologyParser::jumpFunctionParseInverseObjectPropertyOfNode);
			mParseFunctionJumpHash.insert("owl:ObjectInverseOf",&COWL2QtXMLOntologyParser::jumpFunctionParseInverseObjectPropertyOfNode);
			mParseFunctionJumpHash.insert("InverseObjectProperties",&COWL2QtXMLOntologyParser::jumpFunctionParseInverseObjectPropertiesNode);
			mParseFunctionJumpHash.insert("owl:InverseObjectProperties",&COWL2QtXMLOntologyParser::jumpFunctionParseInverseObjectPropertiesNode);
			mParseFunctionJumpHash.insert("ObjectPropertyRange",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyRangeNode);
			mParseFunctionJumpHash.insert("owl:ObjectPropertyRange",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyRangeNode);
			mParseFunctionJumpHash.insert("ObjectPropertyDomain",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyDomainNode);
			mParseFunctionJumpHash.insert("owl:ObjectPropertyDomain",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyDomainNode);
			mParseFunctionJumpHash.insert("TransitiveObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseTransetiveObjectPropertyNode);
			mParseFunctionJumpHash.insert("owl:TransitiveObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseTransetiveObjectPropertyNode);
			mParseFunctionJumpHash.insert("FunctionalObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseFunctionalObjectPropertyNode);
			mParseFunctionJumpHash.insert("owl:FunctionalObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseFunctionalObjectPropertyNode);
			mParseFunctionJumpHash.insert("SymmetricObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseSymmetricObjectPropertyNode);
			mParseFunctionJumpHash.insert("owl:SymmetricObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseSymmetricObjectPropertyNode);
			mParseFunctionJumpHash.insert("AsymmetricObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseAsymmetricObjectPropertyNode);
			mParseFunctionJumpHash.insert("owl:AsymmetricObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseAsymmetricObjectPropertyNode);
			mParseFunctionJumpHash.insert("ReflexiveObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseReflexiveObjectPropertyNode);
			mParseFunctionJumpHash.insert("owl:ReflexiveObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseReflexiveObjectPropertyNode);
			mParseFunctionJumpHash.insert("IrreflexiveObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseIrreflexiveObjectPropertyNode);
			mParseFunctionJumpHash.insert("owl:IrreflexiveObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseIrreflexiveObjectPropertyNode);
			mParseFunctionJumpHash.insert("InverseFunctionalObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseInverseFunctionalObjectPropertyNode);
			mParseFunctionJumpHash.insert("owl:InverseFunctionalObjectProperty",&COWL2QtXMLOntologyParser::jumpFunctionParseInverseFunctionalObjectPropertyNode);

			mParseFunctionJumpHash.insert("ObjectIntersectionOf",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectIntersectionOfNode);
			mParseFunctionJumpHash.insert("owl:ObjectIntersectionOf",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectIntersectionOfNode);
			mParseFunctionJumpHash.insert("ObjectUnionOf",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectUnionOfNode);
			mParseFunctionJumpHash.insert("owl:ObjectUnionOf",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectUnionOfNode);
			mParseFunctionJumpHash.insert("ObjectMaxCardinality",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectMaxCardinalityNode);
			mParseFunctionJumpHash.insert("owl:ObjectMaxCardinality",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectMaxCardinalityNode);
			mParseFunctionJumpHash.insert("ObjectMinCardinality",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectMinCardinalityNode);
			mParseFunctionJumpHash.insert("owl:ObjectMinCardinality",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectMinCardinalityNode);
			mParseFunctionJumpHash.insert("ObjectExactCardinality",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectExactlyCardinalityNode);
			mParseFunctionJumpHash.insert("owl:ObjectExactCardinality",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectExactlyCardinalityNode);
			mParseFunctionJumpHash.insert("ObjectSomeValuesFrom",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectSomeValuesFromNode);
			mParseFunctionJumpHash.insert("owl:ObjectSomeValuesFrom",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectSomeValuesFromNode);
			mParseFunctionJumpHash.insert("ObjectAllValuesFrom",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectAllValuesFromNode);
			mParseFunctionJumpHash.insert("owl:ObjectAllValuesFrom",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectAllValuesFromNode);
			mParseFunctionJumpHash.insert("ObjectComplementOf",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectComplementOfNode);
			mParseFunctionJumpHash.insert("owl:ObjectComplementOf",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectComplementOfNode);
			mParseFunctionJumpHash.insert("ObjectOneOf",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectOneOfNode);
			mParseFunctionJumpHash.insert("owl:ObjectOneOf",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectOneOfNode);
			mParseFunctionJumpHash.insert("ObjectHasValue",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectHasValueNode);
			mParseFunctionJumpHash.insert("owl:ObjectHasValue",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectHasValueNode);
			mParseFunctionJumpHash.insert("ObjectHasSelf",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectHasSelfNode);
			mParseFunctionJumpHash.insert("owl:ObjectHasSelf",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectHasSelfNode);

			mParseFunctionJumpHash.insert("NamedIndividual",&COWL2QtXMLOntologyParser::jumpFunctionParseNamedIndividualNode);
			mParseFunctionJumpHash.insert("owl:NamedIndividual",&COWL2QtXMLOntologyParser::jumpFunctionParseNamedIndividualNode);
			mParseFunctionJumpHash.insert("AnonymousIndividual",&COWL2QtXMLOntologyParser::jumpFunctionParseAnonymousIndividualNode);
			mParseFunctionJumpHash.insert("owl:AnonymousIndividual",&COWL2QtXMLOntologyParser::jumpFunctionParseAnonymousIndividualNode);
			mParseFunctionJumpHash.insert("ClassAssertion",&COWL2QtXMLOntologyParser::jumpFunctionParseClassAssertionNode);
			mParseFunctionJumpHash.insert("owl:ClassAssertion",&COWL2QtXMLOntologyParser::jumpFunctionParseClassAssertionNode);
			mParseFunctionJumpHash.insert("ObjectPropertyAssertion",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyAssertionNode);
			mParseFunctionJumpHash.insert("owl:ObjectPropertyAssertion",&COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyAssertionNode);
			mParseFunctionJumpHash.insert("NegativeObjectPropertyAssertion",&COWL2QtXMLOntologyParser::jumpFunctionParseNegativeObjectPropertyAssertionNode);
			mParseFunctionJumpHash.insert("owl:NegativeObjectPropertyAssertion",&COWL2QtXMLOntologyParser::jumpFunctionParseNegativeObjectPropertyAssertionNode);
			mParseFunctionJumpHash.insert("SameIndividual",&COWL2QtXMLOntologyParser::jumpFunctionParseSameIndividualNode);
			mParseFunctionJumpHash.insert("owl:SameIndividual",&COWL2QtXMLOntologyParser::jumpFunctionParseSameIndividualNode);
			mParseFunctionJumpHash.insert("DifferentIndividuals",&COWL2QtXMLOntologyParser::jumpFunctionParseDifferentIndividualsNode);
			mParseFunctionJumpHash.insert("owl:DifferentIndividuals",&COWL2QtXMLOntologyParser::jumpFunctionParseDifferentIndividualsNode);

			mParseFunctionJumpHash.insert("#text",&COWL2QtXMLOntologyParser::jumpFunctionParseIgnoredNode);
			mParseFunctionJumpHash.insert("#comment",&COWL2QtXMLOntologyParser::jumpFunctionParseIgnoredNode);
			mParseFunctionJumpHash.insert("ExpectedResult",&COWL2QtXMLOntologyParser::jumpFunctionParseIgnoredNode);

			mParseFunctionJumpHash.insert("Ontology",&COWL2QtXMLOntologyParser::jumpFunctionTellOntologyAxiomNode);
			mParseFunctionJumpHash.insert("Tell",&COWL2QtXMLOntologyParser::jumpFunctionTellOntologyAxiomNode);
			mParseFunctionJumpHash.insert("Whereby",&COWL2QtXMLOntologyParser::jumpFunctionTellOntologyAxiomNode);
			mParseFunctionJumpHash.insert("Where",&COWL2QtXMLOntologyParser::jumpFunctionTellOntologyAxiomNode);
			mParseFunctionJumpHash.insert("Retract",&COWL2QtXMLOntologyParser::jumpFunctionRetractOntologyAxiomNode);


			mParseFunctionJumpHash.insert("DLSafeRule",&COWL2QtXMLOntologyParser::jumpFunctionParseIgnoredNode);
			mParseFunctionJumpHash.insert("owl:DLSafeRule",&COWL2QtXMLOntologyParser::jumpFunctionParseIgnoredNode);

			return true;
		}


		QList<CParseRuleAtom*> COWL2QtXMLOntologyParser::parseRuleAtomNodes(const QString& ruleNodePrefix, const QDomElement& node) {
			QList<CParseRuleAtom*> atomList;
			QDomElement ruleAtomNode = node.firstChildElement();
			while (!ruleAtomNode.isNull()) {
				QString nodeString(ruleAtomNode.tagName());

				CParseRuleAtom* ruleAtom = nullptr;

				if (nodeString == ruleNodePrefix+"ObjectPropertyAtom") {
					ruleAtom = new CParseRuleAtom(CParseRuleAtom::PROPERTYATOM);
				} else if (nodeString == ruleNodePrefix+"ClassAtom") {
					ruleAtom = new CParseRuleAtom(CParseRuleAtom::CLASSATOM);
				}

				if (ruleAtom) {
					QDomElement elemNode = ruleAtomNode.firstChildElement();
					while (!elemNode.isNull()) {
						QString elemNodeString(elemNode.tagName());

						if (elemNodeString == ruleNodePrefix+"Variable" || elemNodeString == ruleNodePrefix+"IndividualVariable" || elemNodeString == ruleNodePrefix+"ObjectVariable") {
							QString variableName = getEntityName(&elemNode);
							ruleAtom->addVariableString(variableName);
						} else {
							CBuildExpression* exp = parseBuildNode(&elemNode);
							ruleAtom->mAtomExpression = exp;
						}

						elemNode = elemNode.nextSiblingElement();
					}
					atomList.append(ruleAtom);
				}

				ruleAtomNode = ruleAtomNode.nextSiblingElement();
			}

			return atomList;
		}


		QHash<QString,CParseRuleAtom*> COWL2QtXMLOntologyParser::createVariableAtomHash(const QList<CParseRuleAtom*>& atomList) {
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



		QMap<cint64,QString> COWL2QtXMLOntologyParser::createVariableCountMap(const QList<CParseRuleAtom*>& atomList) {
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


		QList<CBuildExpression*> COWL2QtXMLOntologyParser::parseRuleNode(QDomElement* node) {
			QString nodeString(node->tagName());
			QList<CBuildExpression*> expressionList;

			QHash<QString,CParseRuleAtom*> bodyVariableAtomHash;
			QHash<QString,CParseRuleAtom*> headVariableAtomHash;

			QList<CParseRuleAtom*> bodyVariableAtomList;
			QList<CParseRuleAtom*> headVariableAtomList;

			if (nodeString == "DLSafeRule" || nodeString == "owl:DLSafeRule") {
				QString ruleNodePrefix = "";
				if (nodeString == "owl:DLSafeRule") {
					ruleNodePrefix = "owl:";
				}
				QDomElement bodyElement = node->firstChildElement(ruleNodePrefix+"Body");
				bodyVariableAtomList = parseRuleAtomNodes(ruleNodePrefix,bodyElement);
				QDomElement headElement = node->firstChildElement(ruleNodePrefix+"Head");
				headVariableAtomList = parseRuleAtomNodes(ruleNodePrefix,headElement);

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
								CBuildExpression* otherVarExp = mOntoBuilder->getIndividualVariable(otherVarString,mAxiomNumber);
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

			}

			return expressionList;
		}


		CBuildExpression* COWL2QtXMLOntologyParser::createBodyExpresions(const QString& varString, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QHash<QString,CParseRuleAtom*>& headVariableAtomHash) {
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

		CBuildExpression* COWL2QtXMLOntologyParser::getNeighbourAtomObjectPropertyExpression(CParseRuleAtom* ruleAtom, bool inversed) {
			if (!inversed) {
				return ruleAtom->mAtomExpression;
			} else {
				return mOntoBuilder->getInverseObjectPropertyOf(ruleAtom->mAtomExpression);
			}
		}



		bool COWL2QtXMLOntologyParser::hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
				QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash) {
			
			QSet<QString> backTestProcessedVarSet;
			QSet<CParseRuleAtom*> backTestProcessedAtomSet;

			return hasBackExpressionLinks(varString,notRuleAtom,bodyVariableAtomHash,processedVarSet,processedAtomSet,headVariableAtomHash,backTestProcessedVarSet,backTestProcessedAtomSet);
		}



		bool COWL2QtXMLOntologyParser::hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
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


		CBuildExpression* COWL2QtXMLOntologyParser::createBodyExpresions(const QString& varString, bool direct, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash) {
			QList<CBuildExpression*> expList;
			expList.append(mOntoBuilder->getIndividualVariable(varString,mAxiomNumber));

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



		CBuildExpression *COWL2QtXMLOntologyParser::parseBuildNode(QDomElement *node) {
			CBuildExpression *expression = 0;

			QString nodeString(node->tagName());

#ifdef KONCLUDE_PARSER_USEPARSEJUMPFUNCTIONHASH
			ParseFunction function = mParseFunctionJumpHash.value(nodeString);
			if (function) {
				expression = (this->*function)(node);
			} else {
				QString notSupportedNodeString(nodeString);
				CNotSupportedNodeErrorRecord::makeRecord(notSupportedNodeString,&recordRouter);
			}
#else


			if (nodeString == "Class" || nodeString == "owl:Class" || nodeString == "OWLClass" || nodeString == "owl:OWLClass") {
				expression = parseClassNode(node);
			} else if (nodeString == "ObjectProperty" || nodeString == "owl:ObjectProperty") {
				expression = parseObjectPropertyNode(node);
			} else if (nodeString == "IndividualVariable" || nodeString == "owl:IndividualVariable") {
				expression = parseIndividualVariableNode(node);
			} else if (nodeString == "Declaration" || nodeString == "owl:Declaration") {
				expression = parseDeclarationNode(node);

			} else if (nodeString == "SubClassOf" || nodeString == "owl:SubClassOf") {
				expression = parseSubClassOfNode(node);
			} else if (nodeString == "EquivalentClasses" || nodeString == "owl:EquivalentClasses") {
				expression = parseEquivalentClassesNode(node);
			} else if (nodeString == "DisjointClasses" || nodeString == "owl:DisjointClasses") {
				expression = parseDisjontClassesNode(node);
			} else if (nodeString == "DisjointUnion" || nodeString == "owl:DisjointUnion") {
				expression = parseDisjontUnionNode(node);

			} else if (nodeString == "SubObjectPropertyOf" || nodeString == "owl:SubObjectPropertyOf") {
				expression = parseSubObjectPropertyOfNode(node);
			} else if (nodeString == "ObjectPropertyChain" || nodeString == "owl:ObjectPropertyChain") {
				expression = parseObjectPropertyChainNode(node);
			} else if (nodeString == "EquivalentObjectProperties" || nodeString == "owl:EquivalentObjectProperties") {
				expression = parseEquivalentObjectPropertiesNode(node);
			} else if (nodeString == "DisjointObjectProperties" || nodeString == "owl:DisjointObjectProperties") {
				expression = parseDisjointObjectPropertiesNode(node);

			} else if (nodeString == "ObjectInverseOf" || nodeString == "owl:ObjectInverseOf") {
				expression = parseInverseObjectPropertyOfNode(node);
			} else if (nodeString == "InverseObjectProperties" || nodeString == "owl:InverseObjectProperties") {
				expression = parseInverseObjectPropertiesNode(node);
			} else if (nodeString == "ObjectPropertyRange" || nodeString == "owl:ObjectPropertyRange") {
				expression = parseObjectPropertyRangeNode(node);
			} else if (nodeString == "ObjectPropertyDomain" || nodeString == "owl:ObjectPropertyDomain") {
				expression = parseObjectPropertyDomainNode(node);
			} else if (nodeString == "TransitiveObjectProperty" || nodeString == "owl:TransitiveObjectProperty") {
				expression = parseTransetiveObjectPropertyNode(node);
			} else if (nodeString == "FunctionalObjectProperty" || nodeString == "owl:FunctionalObjectProperty") {
				expression = parseFunctionalObjectPropertyNode(node);
			} else if (nodeString == "SymmetricObjectProperty" || nodeString == "owl:SymmetricObjectProperty") {
				expression = parseSymmetricObjectPropertyNode(node);
			} else if (nodeString == "AsymmetricObjectProperty" || nodeString == "owl:AsymmetricObjectProperty") {
				expression = parseAsymmetricObjectPropertyNode(node);
			} else if (nodeString == "ReflexiveObjectProperty" || nodeString == "owl:ReflexiveObjectProperty") {
				expression = parseReflexiveObjectPropertyNode(node);
			} else if (nodeString == "IrreflexiveObjectProperty" || nodeString == "owl:IrreflexiveObjectProperty") {
				expression = parseIrreflexiveObjectPropertyNode(node);
			} else if (nodeString == "InverseFunctionalObjectProperty" || nodeString == "owl:InverseFunctionalObjectProperty") {
				expression = parseInverseFunctionalObjectPropertyNode(node);

			} else if (nodeString == "ObjectIntersectionOf" || nodeString == "owl:ObjectIntersectionOf") {
				expression = parseObjectIntersectionOfNode(node);
			} else if (nodeString == "ObjectComplementOf" || nodeString == "owl:ObjectComplementOf") {
				expression = parseObjectComplementOfNode(node);
			} else if (nodeString == "ObjectUnionOf" || nodeString == "owl:ObjectUnionOf") {
				expression = parseObjectUnionOfNode(node);
			} else if (nodeString == "ObjectMaxCardinality" || nodeString == "owl:ObjectMaxCardinality") {
				expression = parseObjectMaxCardinalityNode(node);
			} else if (nodeString == "ObjectMinCardinality" || nodeString == "owl:ObjectMinCardinality") {
				expression = parseObjectMinCardinalityNode(node);
			} else if (nodeString == "ObjectExactCardinality" || nodeString == "owl:ObjectExactCardinality") {
				expression = parseObjectExactlyCardinalityNode(node);
			} else if (nodeString == "ObjectSomeValuesFrom" || nodeString == "owl:ObjectSomeValuesFrom") {
				expression = parseObjectSomeValuesFromNode(node);
			} else if (nodeString == "ObjectAllValuesFrom" || nodeString == "owl:ObjectAllValuesFrom") {
				expression = parseObjectAllValuesFromNode(node);
			} else if (nodeString == "ObjectOneOf" || nodeString == "owl:ObjectOneOf") {
				expression = parseObjectOneOfNode(node);
			} else if (nodeString == "ObjectHasValue" || nodeString == "owl:ObjectHasValue") {
				expression = parseObjectHasValueNode(node);
			} else if (nodeString == "ObjectHasSelf" || nodeString == "owl:ObjectHasSelf") {
				expression = parseObjectHasSelfNode(node);

			} else if (nodeString == "ClassAssertion" || nodeString == "owl:ClassAssertion") {
				expression = parseClassAssertionNode(node);
			} else if (nodeString == "NamedIndividual" || nodeString == "owl:NamedIndividual") {
				expression = parseNamedIndividualNode(node);
			} else if (nodeString == "ObjectPropertyAssertion" || nodeString == "owl:ObjectPropertyAssertion") {
				expression = parseObjectPropertyAssertionNode(node);
			} else if (nodeString == "NegativeObjectPropertyAssertion" || nodeString == "owl:NegativeObjectPropertyAssertion") {
				expression = parseNegativeObjectPropertyAssertionNode(node);
			} else if (nodeString == "SameIndividual" || nodeString == "owl:SameIndividual") {
				expression = parseSameIndividualNode(node);
			} else if (nodeString == "DifferentIndividuals" || nodeString == "owl:DifferentIndividuals") {
				expression = parseDifferentIndividualsNode(node);

			} else if (nodeString == "#text") {
				// ignoring text nodes
			} else if (nodeString == "#comment") {
				// ignoring comment nodes
			} else if (nodeString == "ExpectedResult") {
				// ignoring expected results nodes
			} else if (nodeString == "Retract") {
				parseRetractOntologyAxiomNode(node);
			} else if (nodeString == "Tell" || nodeString == "Whereby") {
				parseTellOntologyAxiomNode(node);
			} else {
				if (mReportErrorCount > 0) {
					--mReportErrorCount;
					QString notSupportedNodeString(nodeString);
					CNotSupportedNodeErrorRecord::makeRecord(notSupportedNodeString,&recordRouter);
				} else if (mReportErrorCount == 0) {
					--mReportErrorCount;
					CUnspecifiedMessageErrorRecord::makeRecord(QString("Remaining not supported expressions are ignored"),&recordRouter);
				}
				//LOG(WARNING,getLogDomain(),logTr("Not supported XML node: '%1'. Continue with next/child node.").arg(notSupportedNodeString),this);
			}
#endif
			return expression;
		}


		CDeclarationAxiomExpression* COWL2QtXMLOntologyParser::parseDeclarationNode(QDomElement* node) {
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CDeclarationAxiomExpression* declarationExpression = nullptr;
			declarationExpression = mOntoBuilder->getDeclaration(builds);
			++mAxiomNumber;
			return declarationExpression;
		}


		CObjectPropertyExpression *COWL2QtXMLOntologyParser::parseObjectPropertyNode(QDomElement *node) {
			// parse <Class URI="xxxx"/>
			QString probName = getEntityName(node);
			CObjectPropertyExpression *objectExpression = 0;
			if (!probName.isEmpty()) {
				objectExpression = mOntoBuilder->getObjectProberty(probName);
			}
			return objectExpression;
		}


		CClassExpression *COWL2QtXMLOntologyParser::parseClassNode(QDomElement *node) {
			// parse <ObjectProperty URI="xxxx"/>
			QString className = getEntityName(node);
			CClassExpression *classExpression = 0;
			if (!className.isEmpty()) {
				classExpression = mOntoBuilder->getClass(className);
			}
			return classExpression;
		}


		CObjectIndividualVariableExpression* COWL2QtXMLOntologyParser::parseIndividualVariableNode(QDomElement* node) {
			// parse <ObjectIndividualVariable URI="xxxx"/>
			QString indiVarName = getEntityName(node);
			CObjectIndividualVariableExpression *indiVarExpression = 0;
			if (!indiVarName.isEmpty()) {
				indiVarExpression = mOntoBuilder->getIndividualVariable(indiVarName,mAxiomNumber);
			}
			return indiVarExpression;
		}



		QList<CBuildExpression *> COWL2QtXMLOntologyParser::parseChildNodes(QDomElement *node) {
			QList<CBuildExpression *> builds;
			QDomElement childNode = node->firstChildElement();
			while (!childNode.isNull()) {
				CBuildExpression *expression = parseBuildNode(&childNode);
				if (expression) {
					builds.append(expression);
				}
				childNode = childNode.nextSiblingElement();
			}
			return builds;
		}


		CEquivalentClassesExpression *COWL2QtXMLOntologyParser::parseEquivalentClassesNode(QDomElement *node) {
			// parse <EquivalentClasses> <...> <...> </EquivalentClasses>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CEquivalentClassesExpression *ecExp = 0;

			ecExp = mOntoBuilder->getEquivalentClasses(builds);
			++mAxiomNumber;
			return ecExp;
		}


		CSubClassOfExpression *COWL2QtXMLOntologyParser::parseSubClassOfNode(QDomElement *node) {
			// parse <SubClassOf> <...> <...> </SubClassOf>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CSubClassOfExpression *scoExp = 0;

			scoExp = mOntoBuilder->getSubClassOf(builds);
			++mAxiomNumber;
			return scoExp;
		}


		CObjectPropertyDomainExpression *COWL2QtXMLOntologyParser::parseObjectPropertyDomainNode(QDomElement *node) {
			// parse <ObjectPropertyDomain> <...> <...> </ObjectPropertyDomain>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectPropertyDomainExpression *obPrDoExp = 0;

			obPrDoExp = mOntoBuilder->getObjectPropertyDomainExpression(builds);
			++mAxiomNumber;
			return obPrDoExp;
		}

		CObjectPropertyRangeExpression *COWL2QtXMLOntologyParser::parseObjectPropertyRangeNode(QDomElement *node) {
			// parse <ObjectPropertyRange> <...> <...> </ObjectPropertyRange>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectPropertyRangeExpression *obPrRaExp = 0;

			obPrRaExp = mOntoBuilder->getObjectPropertyRangeExpression(builds);
			++mAxiomNumber;
			return obPrRaExp;
		}


		CSubObjectPropertyOfExpression *COWL2QtXMLOntologyParser::parseSubObjectPropertyOfNode(QDomElement *node) {
			// parse <SubObjectPropertyOf> <...> <...> </SubObjectPropertyOf>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CSubObjectPropertyOfExpression *sopoExp = 0;

			sopoExp = mOntoBuilder->getSubObjectPropertyOf(builds);
			++mAxiomNumber;
			return sopoExp;
		}

		CObjectPropertyChainExpression *COWL2QtXMLOntologyParser::parseObjectPropertyChainNode(QDomElement *node) {
			// parse <ObjectPropertyChain> <...> <...> </ObjectPropertyChain>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectPropertyChainExpression *sopoExp = 0;

			sopoExp = mOntoBuilder->getObjectPropertyChain(builds);
			return sopoExp;
		}

		CEquivalentObjectPropertiesExpression *COWL2QtXMLOntologyParser::parseEquivalentObjectPropertiesNode(QDomElement *node) {
			// parse <EquivalentObjectProperties> <...> <...> </EquivalentObjectProperties>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CEquivalentObjectPropertiesExpression *sopoExp = 0;

			sopoExp = mOntoBuilder->getEquivalentObjectProperties(builds);
			++mAxiomNumber;
			return sopoExp;
		}

		CDisjointObjectPropertiesExpression *COWL2QtXMLOntologyParser::parseDisjointObjectPropertiesNode(QDomElement *node) {
			// parse <DisjointObjectProperties> <...> <...> </DisjointObjectProperties>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CDisjointObjectPropertiesExpression *sopoExp = 0;

			sopoExp = mOntoBuilder->getDisjointObjectProperties(builds);
			++mAxiomNumber;
			return sopoExp;
		}

		CInverseObjectPropertyOfExpression *COWL2QtXMLOntologyParser::parseInverseObjectPropertyOfNode(QDomElement *node) {
			// parse <ObjectInverseOf> <...> <...> </ObjectInverseOf>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CInverseObjectPropertyOfExpression *iopoExp = 0;

			iopoExp = mOntoBuilder->getInverseObjectPropertyOf(builds);
			return iopoExp;
		}

		CInverseObjectPropertiesExpression *COWL2QtXMLOntologyParser::parseInverseObjectPropertiesNode(QDomElement *node) {
			// parse <InverseObjectProperties> <...> <...> </InverseObjectProperties>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CInverseObjectPropertiesExpression *iopoExp = 0;

			iopoExp = mOntoBuilder->getInverseObjectProperties(builds);
			++mAxiomNumber;
			return iopoExp;
		}


		CDisjointClassesExpression *COWL2QtXMLOntologyParser::parseDisjontClassesNode(QDomElement *node) {
			// parse <CDisjointClasses> <...> <...> </CDisjointClasses>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CDisjointClassesExpression *disClassExp = 0;

			disClassExp = mOntoBuilder->getDisjointClasses(builds);
			++mAxiomNumber;
			return disClassExp;
		}

		CDisjointUnionExpression *COWL2QtXMLOntologyParser::parseDisjontUnionNode(QDomElement *node) {
			// parse <DisjointUnion> <...> <...> </DisjointUnion>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CDisjointUnionExpression *disClassExp = 0;

			disClassExp = mOntoBuilder->getDisjointUnion(builds);
			return disClassExp;
		}



		CObjectIntersectionOfExpression *COWL2QtXMLOntologyParser::parseObjectIntersectionOfNode(QDomElement *node) {
			// parse <ObjectIntersectionOf> <...> <...> <...> ... </ObjectIntersectionOf>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectIntersectionOfExpression *exp = 0;

			exp = mOntoBuilder->getObjectIntersectionOf(builds);
			return exp;
		}

		CObjectUnionOfExpression *COWL2QtXMLOntologyParser::parseObjectUnionOfNode(QDomElement *node) {
			// parse <ObjectUnionOf> <...> <...> <...> ... </ObjectUnionOf>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectUnionOfExpression *exp = 0;

			exp = mOntoBuilder->getObjectUnionOf(builds);
			return exp;
		}

		CObjectMaxCardinalityExpression *COWL2QtXMLOntologyParser::parseObjectMaxCardinalityNode(QDomElement *node) {
			// parse <ObjectMaxCardinality cardinality="xxx"> <...> <...> <...> ... </ObjectMaxCardinality>

			QString cardinalityName = node->attribute(QString("cardinality"));
			if (cardinalityName.isEmpty()) {
				cardinalityName = node->attribute(QString("owl:cardinality"));
			}
			CObjectMaxCardinalityExpression *exp = 0;
			if (!cardinalityName.isEmpty()) {
				QList<CBuildExpression *> builds = parseChildNodes(node);

				exp = mOntoBuilder->getObjectMaxCardinality(builds,cardinalityName.toInt());
			}
			return exp;
		}

		CObjectMinCardinalityExpression *COWL2QtXMLOntologyParser::parseObjectMinCardinalityNode(QDomElement *node) {
			// parse <ObjectMinCardinality cardinality="xxx"> <...> <...> <...> ... </ObjectMinCardinality>

			QString cardinalityName = node->attribute(QString("cardinality"));
			if (cardinalityName.isEmpty()) {
				cardinalityName = node->attribute(QString("owl:cardinality"));
			}
			CObjectMinCardinalityExpression *exp = 0;
			if (!cardinalityName.isEmpty()) {
				QList<CBuildExpression *> builds = parseChildNodes(node);

				exp = mOntoBuilder->getObjectMinCardinality(builds,cardinalityName.toInt());
			}
			return exp;
		}

		CObjectExactlyCardinalityExpression *COWL2QtXMLOntologyParser::parseObjectExactlyCardinalityNode(QDomElement *node) {
			// parse <ObjectExactlyCardinality cardinality="xxx"> <...> <...> <...> ... </ObjectExactlyCardinality>

			QString cardinalityName = node->attribute(QString("cardinality"));
			if (cardinalityName.isEmpty()) {
				cardinalityName = node->attribute(QString("owl:cardinality"));
			}
			CObjectExactlyCardinalityExpression *exp = 0;
			if (!cardinalityName.isEmpty()) {
				QList<CBuildExpression *> builds = parseChildNodes(node);

				exp = mOntoBuilder->getObjectExactlyCardinality(builds,cardinalityName.toInt());
			}
			return exp;
		}

		CObjectSomeValuesFromExpression *COWL2QtXMLOntologyParser::parseObjectSomeValuesFromNode(QDomElement *node) {
			// parse <ObjectSomeValueFrom> <...> <...> <...> ... </ObjectSomeValueFrom>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectSomeValuesFromExpression *exp = 0;

			exp = mOntoBuilder->getObjectSomeValuesFrom(builds);
			return exp;
		}

		CObjectHasValueExpression* COWL2QtXMLOntologyParser::parseObjectHasValueNode(QDomElement* node) {
			// parse <ObjectHasValue> <...> <...> <...> ... </ObjectHasValue>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectHasValueExpression *exp = 0;

			exp = mOntoBuilder->getObjectHasValue(builds);
			return exp;
		}

		CObjectHasSelfExpression* COWL2QtXMLOntologyParser::parseObjectHasSelfNode(QDomElement* node) {
			// parse <ObjectHasSelf> <...> <...> <...> ... </ObjectHasSelf>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectHasSelfExpression *exp = 0;

			exp = mOntoBuilder->getObjectHasSelf(builds);
			return exp;
		}


		CObjectAllValuesFromExpression *COWL2QtXMLOntologyParser::parseObjectAllValuesFromNode(QDomElement *node) {
			// parse <ObjectAllValueFrom> <...> <...> <...> ... </ObjectAllValueFrom>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectAllValuesFromExpression *exp = 0;

			exp = mOntoBuilder->getObjectAllValuesFrom(builds);
			return exp;
		}


		CObjectComplementOfExpression *COWL2QtXMLOntologyParser::parseObjectComplementOfNode(QDomElement *node) {
			// parse <ObjectComplementOf> <...> </ObjectComplementOf>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectComplementOfExpression *exp = 0;

			exp = mOntoBuilder->getObjectComplementOf(builds);
			return exp;
		}

		CObjectOneOfExpression *COWL2QtXMLOntologyParser::parseObjectOneOfNode(QDomElement *node) {
			// parse <ObjectOneOf> <...> </ObjectOneOf>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectOneOfExpression *exp = 0;

			exp = mOntoBuilder->getObjectOneOf(builds);
			return exp;
		}


		CTransetiveObjectPropertyExpression *COWL2QtXMLOntologyParser::parseTransetiveObjectPropertyNode(QDomElement *node) {
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CTransetiveObjectPropertyExpression *exp = 0;

			exp = mOntoBuilder->getTransetiveObjectProperty(builds);
			++mAxiomNumber;
			return exp;
		}


		CFunctionalObjectPropertyExpression *COWL2QtXMLOntologyParser::parseFunctionalObjectPropertyNode(QDomElement *node) {
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CFunctionalObjectPropertyExpression *exp = 0;

			exp = mOntoBuilder->getFunctionalObjectProperty(builds);
			++mAxiomNumber;
			return exp;
		}


		CSymmetricObjectPropertyExpression* COWL2QtXMLOntologyParser::parseSymmetricObjectPropertyNode(QDomElement* node) {
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CSymmetricObjectPropertyExpression *exp = 0;

			exp = mOntoBuilder->getSymmetricObjectProperty(builds);
			++mAxiomNumber;
			return exp;
		}

		CAsymmetricObjectPropertyExpression* COWL2QtXMLOntologyParser::parseAsymmetricObjectPropertyNode(QDomElement* node) {
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CAsymmetricObjectPropertyExpression *exp = 0;

			exp = mOntoBuilder->getAsymmetricObjectProperty(builds);
			++mAxiomNumber;
			return exp;
		}

		CReflexiveObjectPropertyExpression* COWL2QtXMLOntologyParser::parseReflexiveObjectPropertyNode(QDomElement* node) {
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CReflexiveObjectPropertyExpression *exp = 0;

			exp = mOntoBuilder->getReflexiveObjectProperty(builds);
			++mAxiomNumber;
			return exp;
		}

		CIrreflexiveObjectPropertyExpression* COWL2QtXMLOntologyParser::parseIrreflexiveObjectPropertyNode(QDomElement* node) {
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CIrreflexiveObjectPropertyExpression *exp = 0;

			exp = mOntoBuilder->getIrreflexiveObjectProperty(builds);
			++mAxiomNumber;
			return exp;
		}

		CInverseFunctionalObjectPropertyExpression* COWL2QtXMLOntologyParser::parseInverseFunctionalObjectPropertyNode(QDomElement* node) {
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CInverseFunctionalObjectPropertyExpression *exp = 0;

			exp = mOntoBuilder->getInverseFunctionalObjectProperty(builds);
			++mAxiomNumber;
			return exp;
		}





		CClassAssertionExpression *COWL2QtXMLOntologyParser::parseClassAssertionNode(QDomElement *node) {
			// parse <ClassAssertion> <...> </ClassAssertion>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CClassAssertionExpression *exp = 0;

			if (mOntoBuilder) {
				exp = mOntoBuilder->getClassAssertion(builds);
			}
			++mAxiomNumber;
			return exp;
		}


		CSameIndividualExpression *COWL2QtXMLOntologyParser::parseSameIndividualNode(QDomElement *node) {
			// parse <SameIndividual> <...> </SameIndividual>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CSameIndividualExpression *exp = 0;
			if (mOntoBuilder) {
				exp = mOntoBuilder->getSameIndividual(builds);
			}
			++mAxiomNumber;
			return exp;
		}

		CDifferentIndividualsExpression *COWL2QtXMLOntologyParser::parseDifferentIndividualsNode(QDomElement *node) {
			// parse <DifferentIndividuals> <...> </DifferentIndividuals>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CDifferentIndividualsExpression *exp = 0;
			if (mOntoBuilder) {
				exp = mOntoBuilder->getDifferentIndividuals(builds);
			}
			++mAxiomNumber;
			return exp;
		}


		CNamedIndividualExpression *COWL2QtXMLOntologyParser::parseNamedIndividualNode(QDomElement *node) {
			// parse <NamedIndividual> <...> </NamedIndividual>
			QString indiNameName = getEntityName(node);
			CNamedIndividualExpression *exp = 0;
			if (!indiNameName.isEmpty()) {
				if (mOntoBuilder) {
					exp = mOntoBuilder->getNamedIndividual(indiNameName);
				}
			}
			return exp;
		}


		CAnonymousIndividualExpression *COWL2QtXMLOntologyParser::parseAnonymousIndividualNode(QDomElement *node) {
			// parse <NamedIndividual> <...> </NamedIndividual>
			if (mOntologyName.isEmpty()) {
				mOntologyName = mOntoBuilder->getOntologyName();
			}
			QString indiNameName = node->attribute(QString("nodeID"));
			CAnonymousIndividualExpression *exp = 0;
			if (!indiNameName.isEmpty()) {
				if (mOntoBuilder) {
					exp = mOntoBuilder->getAnonymousIndividual(mOntologyName,indiNameName);
				}
			}
			return exp;
		}

		CObjectPropertyAssertionExpression *COWL2QtXMLOntologyParser::parseObjectPropertyAssertionNode(QDomElement *node) {
			// parse <ObjectPropertyAssertion> <...> </ObjectPropertyAssertion>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CObjectPropertyAssertionExpression *exp = 0;

			if (mOntoBuilder) {
				exp = mOntoBuilder->getObjectPropertyAssertion(builds);
			}
			++mAxiomNumber;
			return exp;
		}

		CNegativeObjectPropertyAssertionExpression *COWL2QtXMLOntologyParser::parseNegativeObjectPropertyAssertionNode(QDomElement *node) {
			// parse <NegativeObjectPropertyAssertion> <...> </NegativeObjectPropertyAssertion>
			QList<CBuildExpression *> builds = parseChildNodes(node);
			CNegativeObjectPropertyAssertionExpression *exp = 0;

			if (mOntoBuilder) {
				exp = mOntoBuilder->getNegativeObjectPropertyAssertion(builds);
			}
			++mAxiomNumber;
			return exp;
		}


		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionOntologyNode(QDomElement* node) {
			parseOntologyNode(node);
			return nullptr;
		}


		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionTellOntologyAxiomNode(QDomElement* node) {
			parseTellOntologyAxiomNode(node);
			return nullptr;
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionRetractOntologyAxiomNode(QDomElement* node) {
			parseRetractOntologyAxiomNode(node);
			return nullptr;
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParsePrefixNode(QDomElement* node) {
			parsePrefixNode(node);
			return nullptr;
		}


		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseClassNode(QDomElement* node) {
			return parseClassNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyNode(QDomElement* node) {
			return parseObjectPropertyNode(node);
		}


		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseIndividualVariable(QDomElement* node) {
			return parseIndividualVariableNode(node);
		}


		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseDeclarationNode(QDomElement* node) {
			return parseDeclarationNode(node);
		}


		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseEquivalentClassesNode(QDomElement* node) {
			return parseEquivalentClassesNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseSubClassOfNode(QDomElement* node) {
			return parseSubClassOfNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseDisjontClassesNode(QDomElement* node) {
			return parseDisjontClassesNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseDisjontUnionNode(QDomElement* node) {
			return parseDisjontUnionNode(node);
		}


		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseSubObjectPropertyOfNode(QDomElement* node) {
			return parseSubObjectPropertyOfNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyChainNode(QDomElement* node) {
			return parseObjectPropertyChainNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseEquivalentObjectPropertiesNode(QDomElement* node) {
			return parseEquivalentObjectPropertiesNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseDisjointObjectPropertiesNode(QDomElement* node) {
			return parseDisjointObjectPropertiesNode(node);
		}


		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectIntersectionOfNode(QDomElement* node) {
			return parseObjectIntersectionOfNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectUnionOfNode(QDomElement* node) {
			return parseObjectUnionOfNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectMaxCardinalityNode(QDomElement* node) {
			return parseObjectMaxCardinalityNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectMinCardinalityNode(QDomElement* node) {
			return parseObjectMinCardinalityNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectExactlyCardinalityNode(QDomElement* node) {
			return parseObjectExactlyCardinalityNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectSomeValuesFromNode(QDomElement* node) {
			return parseObjectSomeValuesFromNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectAllValuesFromNode(QDomElement* node) {
			return parseObjectAllValuesFromNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectComplementOfNode(QDomElement* node) {
			return parseObjectComplementOfNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectOneOfNode(QDomElement* node) {
			return parseObjectOneOfNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectHasValueNode(QDomElement* node) {
			return parseObjectHasValueNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectHasSelfNode(QDomElement* node) {
			return parseObjectHasSelfNode(node);
		}


		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseClassAssertionNode(QDomElement* node) {
			return parseClassAssertionNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseNamedIndividualNode(QDomElement* node) {
			return parseNamedIndividualNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseAnonymousIndividualNode(QDomElement* node) {
			return parseAnonymousIndividualNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyAssertionNode(QDomElement* node) {
			return parseObjectPropertyAssertionNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseNegativeObjectPropertyAssertionNode(QDomElement* node) {
			return parseNegativeObjectPropertyAssertionNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseSameIndividualNode(QDomElement* node) {
			return parseSameIndividualNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseDifferentIndividualsNode(QDomElement* node) {
			return parseDifferentIndividualsNode(node);
		}


		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyDomainNode(QDomElement* node) {
			return parseObjectPropertyDomainNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseObjectPropertyRangeNode(QDomElement* node) {
			return parseObjectPropertyRangeNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseInverseObjectPropertyOfNode(QDomElement* node) {
			return parseInverseObjectPropertyOfNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseInverseObjectPropertiesNode(QDomElement* node) {
			return parseInverseObjectPropertiesNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseTransetiveObjectPropertyNode(QDomElement* node) {
			return parseTransetiveObjectPropertyNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseFunctionalObjectPropertyNode(QDomElement* node) {
			return parseFunctionalObjectPropertyNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseSymmetricObjectPropertyNode(QDomElement* node) {
			return parseSymmetricObjectPropertyNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseAsymmetricObjectPropertyNode(QDomElement* node) {
			return parseAsymmetricObjectPropertyNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseReflexiveObjectPropertyNode(QDomElement* node) {
			return parseReflexiveObjectPropertyNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseIrreflexiveObjectPropertyNode(QDomElement* node) {
			return parseIrreflexiveObjectPropertyNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseInverseFunctionalObjectPropertyNode(QDomElement* node) {
			return parseInverseFunctionalObjectPropertyNode(node);
		}

		CBuildExpression* COWL2QtXMLOntologyParser::jumpFunctionParseIgnoredNode(QDomElement* node) {
			return nullptr;
		}



	}; // end namespace Parser

}; // end namespace Konclude
