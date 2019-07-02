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

#ifndef KONCLUDE_PARSER_CXMLOWL2STREAMHANDLER_H
#define KONCLUDE_PARSER_CXMLOWL2STREAMHANDLER_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>
#include <QMap>
#include <QStack>
#include <QXmlStreamAttributes>

// Namespace incldues
#include "CParseRuleAtom.h"
#include "COntologyBuilder.h"
#include "CParsingExpressionSplitter.h"


// Other includes
#include "Parser/Expressions/CRuleDifferentIndividualsAtomTermExpression.h"
#include "Parser/Expressions/CRulePropertyAtomTermExpression.h"
#include "Parser/Expressions/CRuleClassAtomTermExpression.h"
#include "Parser/Expressions/CRuleAtomListBodyExpression.h"
#include "Parser/Expressions/CRuleAtomListHeadExpression.h"

#include "Control/Command/CCommandRecordRouter.h"
#include "Control/Command/Records/CNotSupportedNodeErrorRecord.h"

#include "Logger/CLogIdentifier.h"

#include "Utilities/Container/CQtHash.h"
#include "Reasoner/Generator/CStringRefStringHasher.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

using namespace std;


namespace Konclude {

	using namespace Utilities;
	using namespace Container;
	using namespace Control::Command;
	using namespace Control::Command::Records;
	using namespace Reasoner::Generator;

	namespace Parser {

		class CXMLOWL2StreamHandler;
		class CStreamParseStackObject;

		typedef void (CXMLOWL2StreamHandler::*StreamParseFunctionDelayed) (CStreamParseStackObject*  parseStackObj);
		typedef void (CXMLOWL2StreamHandler::*StreamParseFunctionDirect) (const QXmlStreamAttributes& attributes);


		class CStreamParseFunctionData {
			public:
				CStreamParseFunctionData() {
					mParseDelayedFunction = nullptr;
					mParseDirectFunction = nullptr;
					mExtraParsingFlags = 0;
				}
				CStreamParseFunctionData(StreamParseFunctionDelayed parseDelayedFunction, cint64 extraParsingFlags = 0) {
					mParseDelayedFunction = parseDelayedFunction;
					mParseDirectFunction = nullptr;
					mExtraParsingFlags = extraParsingFlags;
				}
				CStreamParseFunctionData(StreamParseFunctionDirect parseDirectFunction) {
					mParseDelayedFunction = nullptr;
					mParseDirectFunction = parseDirectFunction;
					mExtraParsingFlags = 0;
				}

				StreamParseFunctionDelayed mParseDelayedFunction;
				StreamParseFunctionDirect mParseDirectFunction;

				const static cint64 EPF_CARDINALITY					= 0x0001;
				const static cint64 EPF_DATATYPE_IRI				= 0x0002;
				const static cint64 EPF_FACET_IRI					= 0x0004;
				const static cint64 EPF_TEXT_ELEMENT				= 0x0008;
				const static cint64 EPF_LITERAL_APPENDIX			= 0x0010;

				cint64 mExtraParsingFlags;
		};


		class CStreamParseStackObject {
			public:
				inline CStreamParseStackObject() : mParseFunction(nullptr),mCardinality(0),mCardinalityParsed(false),mIRIParsed(false) {
				}
				inline CStreamParseStackObject(StreamParseFunctionDelayed parseFunction) : mParseFunction(parseFunction),mCardinality(0),mCardinalityParsed(false),mIRIParsed(false) {
				}
				inline CStreamParseStackObject(StreamParseFunctionDelayed parseFunction, cint64 cardinality) : mParseFunction(parseFunction),mCardinality(cardinality),mCardinalityParsed(true),mIRIParsed(false) {
				}
				inline CStreamParseStackObject(StreamParseFunctionDelayed parseFunction, const QString& parsedIRI) : mParseFunction(parseFunction),mCardinality(0),mCardinalityParsed(false),mIRIParsed(true),mParsedIRI(parsedIRI) {
				}

				inline CStreamParseStackObject* addBuildExpression(CBuildExpression* buildExpression) {
					mExpList.append(buildExpression);
					return this;
				}

				inline CStreamParseStackObject* addBuildExpressions(CQtList<CBuildExpression*>* expressions) {
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

				inline QString& getParsedIRI() {
					return mParsedIRI;
				}

				StreamParseFunctionDelayed mParseFunction;
				cint64 mCardinality;
				bool mCardinalityParsed;
				QString mParsedIRI;
				bool mIRIParsed;

				CQtList<CBuildExpression*> mExpList;
		};


		class CStreamXMLBaseStackObject {
			public:
				inline CStreamXMLBaseStackObject() {
					mStackValidDepth = 0;
				}
				inline CStreamXMLBaseStackObject(cint64 stackValidDepth, const QString& xmlBaseString) : mStackValidDepth(stackValidDepth),mXMLBaseString(xmlBaseString) {
				}

				cint64 mStackValidDepth;
				QString mXMLBaseString;
		};



		class CStreamXMLNamespaceStackObject {
			public:
				inline CStreamXMLNamespaceStackObject() {
					mParseHash = nullptr;
					mStackValidDepth = 0;
				}
				inline CStreamXMLNamespaceStackObject(cint64 stackValidDepth, CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* parseHash) : mStackValidDepth(stackValidDepth),mParseHash(parseHash) {
				}

				cint64 mStackValidDepth;
				CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* mParseHash;
		};



		/*! 
		 *
		 *		\class		CXMLOWL2StreamHandler
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for OWL/XML Files
		 *
		 */
		class CXMLOWL2StreamHandler : public CLogIdentifier {
			// public methods
			public:
				//! Constructor
				CXMLOWL2StreamHandler(COntologyBuilder* ontoBuilder);
				CXMLOWL2StreamHandler(COntologyBuilder* ontoBuilder, CCommandRecordRouter *commandRecordRouter);


				virtual bool startElement(const QStringRef& namespaceURI, const QStringRef& localName, const QStringRef& qName, const QXmlStreamAttributes& atts);
				virtual bool endElement(const QStringRef& namespaceURI, const QStringRef& localName, const QStringRef& qName);


				virtual bool readText(const QStringRef& text);


				virtual bool startDocument();
				virtual bool endDocument();
				
				bool hasOntologyNodeFound();

			// protected methods
			protected:
				bool initializeParseFunctionJumpHash();
				CStringRefStringHasher getEntityName(const QXmlStreamAttributes& attributes);
				bool getCardinality(cint64* cardinality, const QXmlStreamAttributes& attributes);
				QStringRef getNodeID(const QXmlStreamAttributes& attributes);
				bool startsWithScheme(const QStringRef& uriString);
				bool getDatatypeIRI(QString* datatypeIRI, const QXmlStreamAttributes& attributes);
				bool getFacetIRI(QString* facetIRI, const QXmlStreamAttributes& attributes);

				bool getLiteralAppendix(QString* literalAppendix, const QXmlStreamAttributes& attributes);

				virtual bool parseXMLAttributs(const QXmlStreamAttributes& atts);


				CClassExpression* parseClassNode(const QXmlStreamAttributes& attributes);
				CObjectPropertyExpression* parseObjectPropertyNode(const QXmlStreamAttributes& attributes);
				CObjectIndividualVariableExpression* parseIndividualVariableNode(const QXmlStreamAttributes& attributes);

				CDeclarationAxiomExpression* parseDeclarationNode(CStreamParseStackObject* parseStackObj);

				CEquivalentClassesExpression* parseEquivalentClassesNode(CStreamParseStackObject* parseStackObj);
				CSubClassOfExpression* parseSubClassOfNode(CStreamParseStackObject* parseStackObj);
				CDisjointClassesExpression* parseDisjontClassesNode(CStreamParseStackObject* parseStackObj);
				CDisjointUnionExpression* parseDisjontUnionNode(CStreamParseStackObject* parseStackObj);

				CSubObjectPropertyOfExpression* parseSubObjectPropertyOfNode(CStreamParseStackObject* parseStackObj);
				CObjectPropertyChainExpression* parseObjectPropertyChainNode(CStreamParseStackObject* parseStackObj);
				CEquivalentObjectPropertiesExpression* parseEquivalentObjectPropertiesNode(CStreamParseStackObject* parseStackObj);
				CDisjointObjectPropertiesExpression* parseDisjointObjectPropertiesNode(CStreamParseStackObject* parseStackObj);

				CObjectIntersectionOfExpression* parseObjectIntersectionOfNode(CStreamParseStackObject* parseStackObj);
				CObjectUnionOfExpression* parseObjectUnionOfNode(CStreamParseStackObject* parseStackObj);
				CObjectMaxCardinalityExpression* parseObjectMaxCardinalityNode(CStreamParseStackObject* parseStackObj);
				CObjectMinCardinalityExpression* parseObjectMinCardinalityNode(CStreamParseStackObject* parseStackObj);
				CObjectExactlyCardinalityExpression* parseObjectExactlyCardinalityNode(CStreamParseStackObject* parseStackObj);
				CObjectSomeValuesFromExpression* parseObjectSomeValuesFromNode(CStreamParseStackObject* parseStackObj);
				CObjectAllValuesFromExpression* parseObjectAllValuesFromNode(CStreamParseStackObject* parseStackObj);
				CObjectComplementOfExpression* parseObjectComplementOfNode(CStreamParseStackObject* parseStackObj);
				CObjectOneOfExpression* parseObjectOneOfNode(CStreamParseStackObject* parseStackObj);
				CObjectHasValueExpression* parseObjectHasValueNode(CStreamParseStackObject* parseStackObj);
				CObjectHasSelfExpression* parseObjectHasSelfNode(CStreamParseStackObject* parseStackObj);


				CClassAssertionExpression* parseClassAssertionNode(CStreamParseStackObject* parseStackObj);
				CNamedIndividualExpression* parseNamedIndividualNode(const QXmlStreamAttributes& attributes);
				CAnonymousIndividualExpression* parseAnonymousIndividualNode(const QXmlStreamAttributes& attributes);
				CObjectPropertyAssertionExpression* parseObjectPropertyAssertionNode(CStreamParseStackObject* parseStackObj);
				CNegativeObjectPropertyAssertionExpression* parseNegativeObjectPropertyAssertionNode(CStreamParseStackObject* parseStackObj);
				CSameIndividualExpression* parseSameIndividualNode(CStreamParseStackObject* parseStackObj);
				CDifferentIndividualsExpression* parseDifferentIndividualsNode(CStreamParseStackObject* parseStackObj);


				CObjectPropertyDomainExpression* parseObjectPropertyDomainNode(CStreamParseStackObject* parseStackObj);
				CObjectPropertyRangeExpression* parseObjectPropertyRangeNode(CStreamParseStackObject* parseStackObj);
				CInverseObjectPropertyOfExpression* parseInverseObjectPropertyOfNode(CStreamParseStackObject* parseStackObj);
				CInverseObjectPropertiesExpression* parseInverseObjectPropertiesNode(CStreamParseStackObject* parseStackObj);
				CTransetiveObjectPropertyExpression* parseTransetiveObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				CFunctionalObjectPropertyExpression* parseFunctionalObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				CSymmetricObjectPropertyExpression* parseSymmetricObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				CAsymmetricObjectPropertyExpression* parseAsymmetricObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				CReflexiveObjectPropertyExpression* parseReflexiveObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				CIrreflexiveObjectPropertyExpression* parseIrreflexiveObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				CInverseFunctionalObjectPropertyExpression* parseInverseFunctionalObjectPropertyNode(CStreamParseStackObject* parseStackObj);




				CRulePropertyAtomTermExpression* parseRuleObjectPropertyAtomNode(CStreamParseStackObject* parseStackObj);
				CRuleClassAtomTermExpression* parseRuleClassAtomNode(CStreamParseStackObject* parseStackObj);
				CRuleAtomListBodyExpression* parseRuleAtomBodyNode(CStreamParseStackObject* parseStackObj);
				CRuleAtomListHeadExpression* parseRuleAtomHeadNode(CStreamParseStackObject* parseStackObj);

				CRuleDifferentIndividualsAtomTermExpression* parseRuleDifferentIndividualsAtomNode(CStreamParseStackObject* parseStackObj);


				void parseRuleNode(CStreamParseStackObject* parseStackObj);
				QList<CParseRuleAtom*> parseRuleAtomNodes(CStreamParseStackObject* parseStackObj, const CQtList<CBuildExpression*>& buildExpList, bool parseHeadAtoms);




				QHash<QString,CParseRuleAtom*> createVariableAtomHash(const QList<CParseRuleAtom*>& atomList);
				QMap<cint64,QString> createVariableCountMap(const QList<CParseRuleAtom*>& atomList);
				CBuildExpression* createBodyExpresions(const QString& varString, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);

				bool hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
						QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash, QSet<QString>& backTestProcessedVarSet, QSet<CParseRuleAtom*>& backTestProcessedAtomSet);
			
				bool hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
						QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);

				CBuildExpression* createBodyExpresions(const QString& varString, bool direct, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);
				CBuildExpression* getNeighbourAtomObjectPropertyExpression(CParseRuleAtom* ruleAtom, bool inversed);




				void parsePrefixNode(const QXmlStreamAttributes& attributes);
				void parseTellOntologyAxiomNode(CStreamParseStackObject* parseStackObj);
				void parseRetractOntologyAxiomNode(CStreamParseStackObject* parseStackObj);


				void jumpFunctionOntologyNode(CStreamParseStackObject* parseStackObj);

				void jumpFunctionParseRuleNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseRulePropertyAtomNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseRuleClassAtomNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDifferentIndividualsAtomNode(CStreamParseStackObject* parseStackObj);
			
				void jumpFunctionParseRuleAtomBodyNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseRuleAtomHeadNode(CStreamParseStackObject* parseStackObj);


				void jumpFunctionTellOntologyAxiomNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionRetractOntologyAxiomNode(CStreamParseStackObject* parseStackObj);

				void jumpFunctionParsePrefixNode(const QXmlStreamAttributes& attributes);

				void jumpFunctionParseClassNode(const QXmlStreamAttributes& attributes);
				void jumpFunctionParseObjectPropertyNode(const QXmlStreamAttributes& attributes);
				void jumpFunctionParseIndividualVariable(const QXmlStreamAttributes& attributes);

				void jumpFunctionParseDeclarationNode(CStreamParseStackObject* parseStackObj);

				void jumpFunctionParseSubClassOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseEquivalentClassesNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDisjontClassesNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDisjontUnionNode(CStreamParseStackObject* parseStackObj);

				void jumpFunctionParseSubObjectPropertyOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectPropertyChainNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseEquivalentObjectPropertiesNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDisjointObjectPropertiesNode(CStreamParseStackObject* parseStackObj);

				void jumpFunctionParseObjectIntersectionOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectUnionOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectMaxCardinalityNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectMinCardinalityNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectExactlyCardinalityNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectSomeValuesFromNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectAllValuesFromNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectComplementOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectOneOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectHasValueNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectHasSelfNode(CStreamParseStackObject* parseStackObj);

				void jumpFunctionParseClassAssertionNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseNamedIndividualNode(const QXmlStreamAttributes& attributes);
				void jumpFunctionParseAnonymousIndividualNode(const QXmlStreamAttributes& attributes);
				void jumpFunctionParseObjectPropertyAssertionNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseNegativeObjectPropertyAssertionNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseSameIndividualNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDifferentIndividualsNode(CStreamParseStackObject* parseStackObj);

				void jumpFunctionParseObjectPropertyDomainNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseObjectPropertyRangeNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseInverseObjectPropertyOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseInverseObjectPropertiesNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseTransetiveObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseFunctionalObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseSymmetricObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseAsymmetricObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseReflexiveObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseIrreflexiveObjectPropertyNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseInverseFunctionalObjectPropertyNode(CStreamParseStackObject* parseStackObj);

				void jumpFunctionParseIgnoredNode(CStreamParseStackObject* parseStackObj);

				void jumpFunctionUnsupportedDatatypeNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionUnsupportedRDFNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionUnsupportedImportNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionUnsupportedAnnotationNode(const QXmlStreamAttributes& attributes);

				void jumpFunctionParseImportNode(CStreamParseStackObject* parseStackObj);



				CDatatypeExpression* parseDatatypeNode(const QXmlStreamAttributes& attributes);
				CDataPropertyExpression* parseDataPropertyNode(const QXmlStreamAttributes& attributes);
				CDataLiteralExpression* parseDataLiteralNode(CStreamParseStackObject* parseStackObj);
				CDataSomeValuesFromExpression* parseDataSomeValuesFromNode(CStreamParseStackObject* parseStackObj);


				CDataFacetRestrictionExpression* parseDataFacetRestrictionNode(CStreamParseStackObject* parseStackObj);

				CDataAllValuesFromExpression* parseDataAllValuesFromNode(CStreamParseStackObject* parseStackObj);
				CDataHasValueExpression* parseDataHasValueNode(CStreamParseStackObject* parseStackObj);
				CDataIntersectionOfExpression* parseDataIntersectionOfNode(CStreamParseStackObject* parseStackObj);
				CDataOneOfExpression* parseDataOneOfNode(CStreamParseStackObject* parseStackObj);
				CDataUnionOfExpression* parseDataUnionOfNode(CStreamParseStackObject* parseStackObj);
				CDataComplementOfExpression* parseDataComplementOfNode(CStreamParseStackObject* parseStackObj);
				CDatatypeRestrictionExpression* parseDatatypeRestrictionNode(CStreamParseStackObject* parseStackObj);
				CDataMaxCardinalityExpression* parseDataMaxCardinalityNode(CStreamParseStackObject* parseStackObj);
				CDataMinCardinalityExpression* parseDataMinCardinalityNode(CStreamParseStackObject* parseStackObj);
				CDataExactCardinalityExpression* parseDataExactCardinalityNode(CStreamParseStackObject* parseStackObj);
				CDataPropertyAssertionExpression* parseDataPropertyAssertionNode(CStreamParseStackObject* parseStackObj);
				CNegativeDataPropertyAssertionExpression* parseNegativeDataPropertyAssertionNode(CStreamParseStackObject* parseStackObj);
				CSubDataPropertyOfExpression* parseSubDataPropertyOfNode(CStreamParseStackObject* parseStackObj);
				CEquivalentDataPropertiesExpression* parseEquivalentDataPropertiesNode(CStreamParseStackObject* parseStackObj);
				CDisjointDataPropertiesExpression* parseDisjointDataPropertiesNode(CStreamParseStackObject* parseStackObj);
				CDataPropertyDomainExpression* parseDataPropertyDomainNode(CStreamParseStackObject* parseStackObj);
				CDataPropertyRangeExpression* parseDataPropertyRangeNode(CStreamParseStackObject* parseStackObj);
				CFunctionalDataPropertyExpression* parseFunctionalDataPropertyNode(CStreamParseStackObject* parseStackObj);


				void jumpFunctionParseDatatypeNode(const QXmlStreamAttributes& attributes);
				void jumpFunctionParseDataPropertyNode(const QXmlStreamAttributes& attributes);
				void jumpFunctionParseDataLiteralNode(CStreamParseStackObject* parseStackObj);

				void jumpFunctionParseDataSomeValuesFromNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataAllValuesFromNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataHasValueNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataIntersectionOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataOneOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataUnionOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataComplementOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDatatypeRestrictionNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataMaxCardinalityNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataMinCardinalityNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataExactCardinalityNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataPropertyAssertionNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseNegativeDataPropertyAssertionNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseSubDataPropertyOfNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseEquivalentDataPropertiesNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDisjointDataPropertiesNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataPropertyDomainNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataPropertyRangeNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseFunctionalDataPropertyNode(CStreamParseStackObject* parseStackObj);
				void jumpFunctionParseDataFacetRestrictionNode(CStreamParseStackObject* parseStackObj);





				void addTemporaryBuildExpression(CBuildExpression* buildExp);
				void addTemporaryBuildExpressions(QList<CBuildExpression*>* buildExps);


			// protected variables
			protected:

				QStack<CStreamParseStackObject*> mStack;
				QStack<CStreamXMLBaseStackObject> mXMLBaseStack;
				QString mCurrentXMLBase;
				bool mHasCurrentXMLBase;
				cint64 mCurrentXMLBaseValidDepth;


				QStack<CStreamXMLNamespaceStackObject> mXMLNamespaceStack;
				CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* mCurrentParseFunctionJumpHash;
				cint64 mCurrentXMLNamespaceValidDepth;


				QString mOWLPrefix;


				CCommandRecordRouter recordRouter;
				COntologyBuilder* mOntoBuilder;
				QString mOntologyName;
				cint64 mAxiomNumber;




				CStreamParseStackObject mDummyFunctionData;

				CQtHash<CStringRefStringHasher,CStreamParseFunctionData> mBaseParseFunctionJumpHash;

				QList<CBuildExpression*> mRuleExpContainer;

				QString mLastReadText;
				QString mLastTextAppendix;
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

				QString mDatatypeIRIString;
				QString mOWLPrefixDatatypeIRIString;

				QString mFacetIRIString;
				QString mOWLPrefixFacetIRIString;

				QString mNodeIDString;
				QString mOWLPrefixNodeIDString;

				QString mNameString;
				QString mOWLPrefixNameString;


				QChar mPunctChar;
				QChar mDoublePunctChar;


				cint64 mDataTypeReportErrorCount;
				cint64 mAnnotationReportErrorCount;

				bool mUnsupportedImportError;
				bool mUnsupportedAnnotationError;
				bool mOntologyNodeFound;


				cint64 mParseIgnoreDepth;
				bool mIgnoreParsingSubElements;
				bool mParseTextElement;


				QString baseAttributeNamespace = "http://www.w3.org/XML/1998/namespace";
				QString baseAttributeName = "base";

				QString xmlnsAttributeNamespace = "http://www.w3.org/XML/1998/namespace";
				QString xmlnsAttributeName = "xmlns";

				QString mDefaultDatatype = "http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral";

			// private methods
			private:
				bool extendParsingHashForNamespace(CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* baseParseFunctionJumpHash, CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* extendParseFunctionJumpHash, const QString& namespaceExtendString);
				bool reduceParsingHashForNamespace(CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* baseParseFunctionJumpHash, CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* reduceParseFunctionJumpHash, const QString& namespaceReduceString);


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CXMLOWL2STREAMHANDLER_H
