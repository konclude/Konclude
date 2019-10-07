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

#ifndef KONCLUDE_PARSER_COWL2QTXMLONTOLOGYPARSER_H
#define KONCLUDE_PARSER_COWL2QTXMLONTOLOGYPARSER_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>
#include <QMap>

// Namespace includes
#include "COntologyParser.h"
#include "COntologyBuilder.h"
#include "CParseRuleAtom.h"


// Other includes
#include "Control/Command/CCommandRecordRouter.h"
#include "Control/Command/Records/CNotSupportedNodeErrorRecord.h"
#include "Control/Command/Records/CUnspecifiedMessageErrorRecord.h"

#include "Logger/CLogIdentifier.h"

#include "Utilities/Container/CQtHash.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

using namespace std;


namespace Konclude {

	using namespace Utilities::Container;
	using namespace Control::Command;
	using namespace Control::Command::Records;

	namespace Parser {



#define KONCLUDE_PARSER_USEPARSEJUMPFUNCTIONHASH




		/*! 
		 *
		 *		\class		COWL2QtXMLOntologyParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for OWL/XML Files
		 *
		 */
		class COWL2QtXMLOntologyParser : public COntologyParser, public CLogIdentifier {
			// public methods
			public:
				//! Constructor
				COWL2QtXMLOntologyParser(COntologyBuilder* ontoBuilder);
				COWL2QtXMLOntologyParser(COntologyBuilder* ontoBuilder, CCommandRecordRouter* commandRecordRouter);

				//! Destructor
				virtual ~COWL2QtXMLOntologyParser();

				virtual bool parseOntologyFile(const QString& filename);
				virtual bool parseOntologyNode(QDomElement* ontologyNode);
				virtual bool parseTellOntologyAxiomNode(QDomElement* ontologyNode);
				virtual bool parseRetractOntologyAxiomNode(QDomElement* ontologyNode);

				virtual CBuildExpression* getParsedBuildNode(QDomElement* node);

			// protected methods
			protected:
				bool initializeParseFunctionJumpHash();
				QString getEntityName(QDomElement* node);

				cint64 parseCardinality(QDomElement* node);

				CBuildExpression* parseBuildNode(QDomElement* node);
				QList<CBuildExpression* > parseChildNodes(QDomElement* node);

				CClassExpression* parseClassNode(QDomElement* node);
				CObjectPropertyExpression* parseObjectPropertyNode(QDomElement* node);
				CObjectIndividualVariableExpression* parseIndividualVariableNode(QDomElement* node);

				CDeclarationAxiomExpression* parseDeclarationNode(QDomElement* node);

				CEquivalentClassesExpression* parseEquivalentClassesNode(QDomElement* node);
				CSubClassOfExpression* parseSubClassOfNode(QDomElement* node);
				CDisjointClassesExpression* parseDisjontClassesNode(QDomElement* node);
				CDisjointUnionExpression* parseDisjontUnionNode(QDomElement* node);

				CSubObjectPropertyOfExpression* parseSubObjectPropertyOfNode(QDomElement* node);
				CObjectPropertyChainExpression* parseObjectPropertyChainNode(QDomElement* node);
				CEquivalentObjectPropertiesExpression* parseEquivalentObjectPropertiesNode(QDomElement* node);
				CDisjointObjectPropertiesExpression* parseDisjointObjectPropertiesNode(QDomElement* node);

				CObjectIntersectionOfExpression* parseObjectIntersectionOfNode(QDomElement* node);
				CObjectUnionOfExpression* parseObjectUnionOfNode(QDomElement* node);
				CObjectMaxCardinalityExpression* parseObjectMaxCardinalityNode(QDomElement* node);
				CObjectMinCardinalityExpression* parseObjectMinCardinalityNode(QDomElement* node);
				CObjectExactlyCardinalityExpression* parseObjectExactlyCardinalityNode(QDomElement* node);
				CObjectSomeValuesFromExpression* parseObjectSomeValuesFromNode(QDomElement* node);
				CObjectAllValuesFromExpression* parseObjectAllValuesFromNode(QDomElement* node);
				CObjectComplementOfExpression* parseObjectComplementOfNode(QDomElement* node);
				CObjectOneOfExpression* parseObjectOneOfNode(QDomElement* node);
				CObjectHasValueExpression* parseObjectHasValueNode(QDomElement* node);
				CObjectHasSelfExpression* parseObjectHasSelfNode(QDomElement* node);


				CClassAssertionExpression* parseClassAssertionNode(QDomElement* node);
				CNamedIndividualExpression* parseNamedIndividualNode(QDomElement* node);
				CAnonymousIndividualExpression* parseAnonymousIndividualNode(QDomElement* node);
				CObjectPropertyAssertionExpression* parseObjectPropertyAssertionNode(QDomElement* node);
				CNegativeObjectPropertyAssertionExpression* parseNegativeObjectPropertyAssertionNode(QDomElement* node);
				CSameIndividualExpression* parseSameIndividualNode(QDomElement* node);
				CDifferentIndividualsExpression* parseDifferentIndividualsNode(QDomElement* node);


				CObjectPropertyDomainExpression* parseObjectPropertyDomainNode(QDomElement* node);
				CObjectPropertyRangeExpression* parseObjectPropertyRangeNode(QDomElement* node);
				CInverseObjectPropertyOfExpression* parseInverseObjectPropertyOfNode(QDomElement* node);
				CInverseObjectPropertiesExpression* parseInverseObjectPropertiesNode(QDomElement* node);
				CTransetiveObjectPropertyExpression* parseTransetiveObjectPropertyNode(QDomElement* node);
				CFunctionalObjectPropertyExpression* parseFunctionalObjectPropertyNode(QDomElement* node);
				CSymmetricObjectPropertyExpression* parseSymmetricObjectPropertyNode(QDomElement* node);
				CAsymmetricObjectPropertyExpression* parseAsymmetricObjectPropertyNode(QDomElement* node);
				CReflexiveObjectPropertyExpression* parseReflexiveObjectPropertyNode(QDomElement* node);
				CIrreflexiveObjectPropertyExpression* parseIrreflexiveObjectPropertyNode(QDomElement* node);
				CInverseFunctionalObjectPropertyExpression* parseInverseFunctionalObjectPropertyNode(QDomElement* node);



				QList<CBuildExpression*> parseRuleNode(QDomElement* node);
				QHash<QString,CParseRuleAtom*> createVariableAtomHash(const QList<CParseRuleAtom*>& atomList);
				QMap<cint64,QString> createVariableCountMap(const QList<CParseRuleAtom*>& atomList);
				CBuildExpression* createBodyExpresions(const QString& varString, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);

				bool hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
						QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash, QSet<QString>& backTestProcessedVarSet, QSet<CParseRuleAtom*>& backTestProcessedAtomSet);
			
				bool hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
						QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);

				CBuildExpression* createBodyExpresions(const QString& varString, bool direct, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);
				CBuildExpression* getNeighbourAtomObjectPropertyExpression(CParseRuleAtom* ruleAtom, bool inversed);

				QList<CParseRuleAtom*> parseRuleAtomNodes(const QString& ruleNodePrefix, const QDomElement& node);


				bool parsePrefixNode(QDomElement* node);


				CBuildExpression* jumpFunctionOntologyNode(QDomElement* node);

				CBuildExpression* jumpFunctionTellOntologyAxiomNode(QDomElement* node);
				CBuildExpression* jumpFunctionRetractOntologyAxiomNode(QDomElement* node);

				CBuildExpression* jumpFunctionParsePrefixNode(QDomElement* node);

				CBuildExpression* jumpFunctionParseClassNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectPropertyNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseIndividualVariable(QDomElement* node);

				CBuildExpression* jumpFunctionParseDeclarationNode(QDomElement* node);

				CBuildExpression* jumpFunctionParseSubClassOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseEquivalentClassesNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDisjontClassesNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDisjontUnionNode(QDomElement* node);

				CBuildExpression* jumpFunctionParseSubObjectPropertyOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectPropertyChainNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseEquivalentObjectPropertiesNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDisjointObjectPropertiesNode(QDomElement* node);

				CBuildExpression* jumpFunctionParseObjectIntersectionOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectUnionOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectMaxCardinalityNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectMinCardinalityNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectExactlyCardinalityNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectSomeValuesFromNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectAllValuesFromNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectComplementOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectOneOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectHasValueNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectHasSelfNode(QDomElement* node);

				CBuildExpression* jumpFunctionParseClassAssertionNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseNamedIndividualNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseAnonymousIndividualNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectPropertyAssertionNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseNegativeObjectPropertyAssertionNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseSameIndividualNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDifferentIndividualsNode(QDomElement* node);

				CBuildExpression* jumpFunctionParseObjectPropertyDomainNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseObjectPropertyRangeNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseInverseObjectPropertyOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseInverseObjectPropertiesNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseTransetiveObjectPropertyNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseFunctionalObjectPropertyNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseSymmetricObjectPropertyNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseAsymmetricObjectPropertyNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseReflexiveObjectPropertyNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseIrreflexiveObjectPropertyNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseInverseFunctionalObjectPropertyNode(QDomElement* node);

				CBuildExpression* jumpFunctionParseIgnoredNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseImportNode(QDomElement* node);


				CBuildExpression* jumpFunctionParseDatatypeNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataPropertyNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataSomeValuesFromNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataLiteralNode(QDomElement* node);




				CBuildExpression* jumpFunctionParseDataAllValuesFromNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataHasValueNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataIntersectionOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataOneOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataUnionOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataComplementOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDatatypeRestrictionNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataMaxCardinalityNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataMinCardinalityNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataExactCardinalityNode(QDomElement* node);

				CBuildExpression* jumpFunctionParseDataPropertyAssertionNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseNegativeDataPropertyAssertionNode(QDomElement* node);

				CBuildExpression* jumpFunctionParseSubDataPropertyOfNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseEquivalentDataPropertiesNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDisjointDataPropertiesNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataPropertyDomainNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseDataPropertyRangeNode(QDomElement* node);
				CBuildExpression* jumpFunctionParseFunctionalDataPropertyNode(QDomElement* node);

				CBuildExpression* jumpFunctionParseDataFacetRestrictionNode(QDomElement* node);





				CDatatypeExpression* parseDatatypeNode(QDomElement* node);
				CDataPropertyExpression* parseDataPropertyNode(QDomElement* node);
				CDataSomeValuesFromExpression* parseDataSomeValuesFromNode(QDomElement* node);
				CDataLiteralExpression* parseDataLiteralNode(QDomElement* node);

				CDataFacetRestrictionExpression* parseDataFacetRestrictionNode(QDomElement* node);


				CDataAllValuesFromExpression* parseDataAllValuesFromNode(QDomElement* node);
				CDataHasValueExpression* parseDataHasValueNode(QDomElement* node);
				CDataIntersectionOfExpression* parseDataIntersectionOfNode(QDomElement* node);
				CDataOneOfExpression* parseDataOneOfNode(QDomElement* node);
				CDataUnionOfExpression* parseDataUnionOfNode(QDomElement* node);
				CDataComplementOfExpression* parseDataComplementOfNode(QDomElement* node);
				CDatatypeRestrictionExpression* parseDatatypeRestrictionNode(QDomElement* node);
				CDataMaxCardinalityExpression* parseDataMaxCardinalityNode(QDomElement* node);
				CDataMinCardinalityExpression* parseDataMinCardinalityNode(QDomElement* node);
				CDataExactCardinalityExpression* parseDataExactCardinalityNode(QDomElement* node);

				CDataPropertyAssertionExpression* parseDataPropertyAssertionNode(QDomElement* node);
				CNegativeDataPropertyAssertionExpression* parseNegativeDataPropertyAssertionNode(QDomElement* node);

				CSubDataPropertyOfExpression* parseSubDataPropertyOfNode(QDomElement* node);
				CEquivalentDataPropertiesExpression* parseEquivalentDataPropertiesNode(QDomElement* node);
				CDisjointDataPropertiesExpression* parseDisjointDataPropertiesNode(QDomElement* node);
				CDataPropertyDomainExpression* parseDataPropertyDomainNode(QDomElement* node);
				CDataPropertyRangeExpression* parseDataPropertyRangeNode(QDomElement* node);
				CFunctionalDataPropertyExpression* parseFunctionalDataPropertyNode(QDomElement* node);

			// protected variables
			protected:



				cint64 mReportErrorCount;


				CCommandRecordRouter recordRouter;
				COntologyBuilder* mOntoBuilder;
				QString mOntologyIdentifier;
				cint64 mAxiomNumber;

				typedef CBuildExpression* (COWL2QtXMLOntologyParser::*ParseFunction) (QDomElement* node);

				CQtHash<QString,ParseFunction> mParseFunctionJumpHash;


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_COWL2QTXMLONTOLOGYPARSER_H
