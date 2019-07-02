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

#ifndef KONCLUDE_PARSER_CXMLOWL2HANDLER_H
#define KONCLUDE_PARSER_CXMLOWL2HANDLER_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>
#include <QMap>
#include <QStack>
#include <QXmlStreamAttributes>

// Namespace incldues
#include "CXMLHandler.h"
#include "CParseRuleAtom.h"
#include "COntologyBuilder.h"
#include "CParsingExpressionSplitter.h"


// Other includes
#include "Parser/Expressions/CRulePropertyAtomTermExpression.h"
#include "Parser/Expressions/CRuleClassAtomTermExpression.h"
#include "Parser/Expressions/CRuleAtomListBodyExpression.h"
#include "Parser/Expressions/CRuleAtomListHeadExpression.h"

#include "Control/Command/CCommandRecordRouter.h"
#include "Control/Command/Records/CNotSupportedNodeErrorRecord.h"

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




		class CParseStackObject {
			public:
				inline CParseStackObject(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts) :
				mNamespaceURI(namespaceURI),mLocalName(localName),mQName(qName),mAtts(atts) {
				}

				inline CParseStackObject* addBuildExpression(CBuildExpression* buildExpression) {
					mExpList.append(buildExpression);
					return this;
				}

				inline CParseStackObject* addBuildExpressions(CQtList<CBuildExpression*>* expressions) {
					if (mExpList.isEmpty()) {
						mExpList = *expressions;
					} else {
						mExpList.append(*expressions);
					}
					return this;
				}

				inline bool hasExpressions() {
					return !mExpList.isEmpty();
				}

				inline CQtList<CBuildExpression*>* getExpressions() {
					return &mExpList;
				}

				const QString mNamespaceURI;
				const QString mLocalName;
				const QString mQName;
				const QXmlAttributes mAtts;

				CQtList<CBuildExpression*> mExpList;
		};




		/*! 
		 *
		 *		\class		CXMLOWL2Handler
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for OWL/XML Files
		 *
		 */
		class CXMLOWL2Handler : public CXMLHandler, public CLogIdentifier {
			// public methods
			public:
				//! Constructor
				CXMLOWL2Handler(COntologyBuilder* ontoBuilder);
				CXMLOWL2Handler(COntologyBuilder* ontoBuilder, CCommandRecordRouter *commandRecordRouter);


				virtual bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts, CParsingHandlingContext* handlingContext);
				virtual bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName, CParsingHandlingContext* handlingContext);

				virtual bool startDocument(CParsingHandlingContext* handlingContext);
				virtual bool endDocument(CParsingHandlingContext* handlingContext);
				
				

			// protected methods
			protected:
				bool initializeParseFunctionJumpHash();
				QString getEntityName(CParseStackObject* parseStackObj);
				QString getCardinality(CParseStackObject* parseStackObj);
				QString getNodeID(CParseStackObject* parseStackObj);

				void parseBuildNode(CParseStackObject* parseStackObj);

				CClassExpression* parseClassNode(CParseStackObject* parseStackObj);
				CObjectPropertyExpression* parseObjectPropertyNode(CParseStackObject* parseStackObj);
				CObjectIndividualVariableExpression* parseIndividualVariableNode(CParseStackObject* parseStackObj);

				CDeclarationAxiomExpression* parseDeclarationNode(CParseStackObject* parseStackObj);

				CEquivalentClassesExpression* parseEquivalentClassesNode(CParseStackObject* parseStackObj);
				CSubClassOfExpression* parseSubClassOfNode(CParseStackObject* parseStackObj);
				CDisjointClassesExpression* parseDisjontClassesNode(CParseStackObject* parseStackObj);
				CDisjointUnionExpression* parseDisjontUnionNode(CParseStackObject* parseStackObj);

				CSubObjectPropertyOfExpression* parseSubObjectPropertyOfNode(CParseStackObject* parseStackObj);
				CObjectPropertyChainExpression* parseObjectPropertyChainNode(CParseStackObject* parseStackObj);
				CEquivalentObjectPropertiesExpression* parseEquivalentObjectPropertiesNode(CParseStackObject* parseStackObj);
				CDisjointObjectPropertiesExpression* parseDisjointObjectPropertiesNode(CParseStackObject* parseStackObj);

				CObjectIntersectionOfExpression* parseObjectIntersectionOfNode(CParseStackObject* parseStackObj);
				CObjectUnionOfExpression* parseObjectUnionOfNode(CParseStackObject* parseStackObj);
				CObjectMaxCardinalityExpression* parseObjectMaxCardinalityNode(CParseStackObject* parseStackObj);
				CObjectMinCardinalityExpression* parseObjectMinCardinalityNode(CParseStackObject* parseStackObj);
				CObjectExactlyCardinalityExpression* parseObjectExactlyCardinalityNode(CParseStackObject* parseStackObj);
				CObjectSomeValuesFromExpression* parseObjectSomeValuesFromNode(CParseStackObject* parseStackObj);
				CObjectAllValuesFromExpression* parseObjectAllValuesFromNode(CParseStackObject* parseStackObj);
				CObjectComplementOfExpression* parseObjectComplementOfNode(CParseStackObject* parseStackObj);
				CObjectOneOfExpression* parseObjectOneOfNode(CParseStackObject* parseStackObj);
				CObjectHasValueExpression* parseObjectHasValueNode(CParseStackObject* parseStackObj);
				CObjectHasSelfExpression* parseObjectHasSelfNode(CParseStackObject* parseStackObj);


				CClassAssertionExpression* parseClassAssertionNode(CParseStackObject* parseStackObj);
				CNamedIndividualExpression* parseNamedIndividualNode(CParseStackObject* parseStackObj);
				CAnonymousIndividualExpression* parseAnonymousIndividualNode(CParseStackObject* parseStackObj);
				CObjectPropertyAssertionExpression* parseObjectPropertyAssertionNode(CParseStackObject* parseStackObj);
				CNegativeObjectPropertyAssertionExpression* parseNegativeObjectPropertyAssertionNode(CParseStackObject* parseStackObj);
				CSameIndividualExpression* parseSameIndividualNode(CParseStackObject* parseStackObj);
				CDifferentIndividualsExpression* parseDifferentIndividualsNode(CParseStackObject* parseStackObj);


				CObjectPropertyDomainExpression* parseObjectPropertyDomainNode(CParseStackObject* parseStackObj);
				CObjectPropertyRangeExpression* parseObjectPropertyRangeNode(CParseStackObject* parseStackObj);
				CInverseObjectPropertyOfExpression* parseInverseObjectPropertyOfNode(CParseStackObject* parseStackObj);
				CInverseObjectPropertiesExpression* parseInverseObjectPropertiesNode(CParseStackObject* parseStackObj);
				CTransetiveObjectPropertyExpression* parseTransetiveObjectPropertyNode(CParseStackObject* parseStackObj);
				CFunctionalObjectPropertyExpression* parseFunctionalObjectPropertyNode(CParseStackObject* parseStackObj);
				CSymmetricObjectPropertyExpression* parseSymmetricObjectPropertyNode(CParseStackObject* parseStackObj);
				CAsymmetricObjectPropertyExpression* parseAsymmetricObjectPropertyNode(CParseStackObject* parseStackObj);
				CReflexiveObjectPropertyExpression* parseReflexiveObjectPropertyNode(CParseStackObject* parseStackObj);
				CIrreflexiveObjectPropertyExpression* parseIrreflexiveObjectPropertyNode(CParseStackObject* parseStackObj);
				CInverseFunctionalObjectPropertyExpression* parseInverseFunctionalObjectPropertyNode(CParseStackObject* parseStackObj);




				CRulePropertyAtomTermExpression* parseRuleObjectPropertyAtomNode(CParseStackObject* parseStackObj);
				CRuleClassAtomTermExpression* parseRuleClassAtomNode(CParseStackObject* parseStackObj);
				CRuleAtomListBodyExpression* parseRuleAtomBodyNode(CParseStackObject* parseStackObj);
				CRuleAtomListHeadExpression* parseRuleAtomHeadNode(CParseStackObject* parseStackObj);



				void parseRuleNode(CParseStackObject* parseStackObj);
				QList<CParseRuleAtom*> parseRuleAtomNodes(CParseStackObject* parseStackObj, const CQtList<CBuildExpression*>& buildExpList, bool parseHeadAtoms);




				QHash<QString,CParseRuleAtom*> createVariableAtomHash(const QList<CParseRuleAtom*>& atomList);
				QMap<cint64,QString> createVariableCountMap(const QList<CParseRuleAtom*>& atomList);
				CBuildExpression* createBodyExpresions(const QString& varString, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);

				bool hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
						QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash, QSet<QString>& backTestProcessedVarSet, QSet<CParseRuleAtom*>& backTestProcessedAtomSet);
			
				bool hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
						QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);

				CBuildExpression* createBodyExpresions(const QString& varString, bool direct, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);
				CBuildExpression* getNeighbourAtomObjectPropertyExpression(CParseRuleAtom* ruleAtom, bool inversed);




				void parsePrefixNode(CParseStackObject* parseStackObj);
				void parseTellOntologyAxiomNode(CParseStackObject* parseStackObj);
				void parseRetractOntologyAxiomNode(CParseStackObject* parseStackObj);


				void jumpFunctionOntologyNode(CParseStackObject* parseStackObj);

				void jumpFunctionParseRuleNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseRulePropertyAtomNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseRuleClassAtomNode(CParseStackObject* parseStackObj);
			
				void jumpFunctionParseRuleAtomBodyNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseRuleAtomHeadNode(CParseStackObject* parseStackObj);


				void jumpFunctionTellOntologyAxiomNode(CParseStackObject* parseStackObj);
				void jumpFunctionRetractOntologyAxiomNode(CParseStackObject* parseStackObj);

				void jumpFunctionParsePrefixNode(CParseStackObject* parseStackObj);

				void jumpFunctionParseClassNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectPropertyNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseIndividualVariable(CParseStackObject* parseStackObj);

				void jumpFunctionParseDeclarationNode(CParseStackObject* parseStackObj);

				void jumpFunctionParseSubClassOfNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseEquivalentClassesNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseDisjontClassesNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseDisjontUnionNode(CParseStackObject* parseStackObj);

				void jumpFunctionParseSubObjectPropertyOfNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectPropertyChainNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseEquivalentObjectPropertiesNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseDisjointObjectPropertiesNode(CParseStackObject* parseStackObj);

				void jumpFunctionParseObjectIntersectionOfNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectUnionOfNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectMaxCardinalityNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectMinCardinalityNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectExactlyCardinalityNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectSomeValuesFromNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectAllValuesFromNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectComplementOfNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectOneOfNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectHasValueNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectHasSelfNode(CParseStackObject* parseStackObj);

				void jumpFunctionParseClassAssertionNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseNamedIndividualNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseAnonymousIndividualNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectPropertyAssertionNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseNegativeObjectPropertyAssertionNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseSameIndividualNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseDifferentIndividualsNode(CParseStackObject* parseStackObj);

				void jumpFunctionParseObjectPropertyDomainNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseObjectPropertyRangeNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseInverseObjectPropertyOfNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseInverseObjectPropertiesNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseTransetiveObjectPropertyNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseFunctionalObjectPropertyNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseSymmetricObjectPropertyNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseAsymmetricObjectPropertyNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseReflexiveObjectPropertyNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseIrreflexiveObjectPropertyNode(CParseStackObject* parseStackObj);
				void jumpFunctionParseInverseFunctionalObjectPropertyNode(CParseStackObject* parseStackObj);

				void jumpFunctionParseIgnoredNode(CParseStackObject* parseStackObj);



				void addTemporaryBuildExpression(CBuildExpression* buildExp);
				void addTemporaryBuildExpressions(QList<CBuildExpression*>* buildExps);

			// protected variables
			protected:

				QStack<CParseStackObject*> mStack;

				QString mOWLPrefix;


				CCommandRecordRouter recordRouter;
				COntologyBuilder* mOntoBuilder;
				QString mOntologyName;
				cint64 mAxiomNumber;

				typedef void (CXMLOWL2Handler::*ParseFunction) (CParseStackObject*  parseStackObj);

				CQtHash<QString,ParseFunction> mParseFunctionJumpHash;

				QList<CBuildExpression*> mRuleExpContainer;


				CQtList<CBuildExpression*> mTmpExpList;

				QString mOWLString;
				QString mOWLPrefixString;

				QString mURIString;
				QString mIRIString;
				QString mAbbreviatedIRIString;

				QString mOWLPrefixURIString;
				QString mOWLPrefixIRIString;
				QString mOWLPrefixAbbreviatedIRIString;


				QString mCardinalityString;
				QString mOWLPrefixCardinalityString;

				QString mNodeIDString;
				QString mOWLPrefixNodeIDString;

				QString mNameString;
				QString mOWLPrefixNameString;

			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CXMLOWL2HANDLER_H
