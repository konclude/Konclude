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

#include "CXMLOWL2StreamHandler.h"


namespace Konclude {

	namespace Parser {



		CXMLOWL2StreamHandler::CXMLOWL2StreamHandler(COntologyBuilder* ontoBuilder)
				: CLogIdentifier("::Konclude::Parser::OWL2QtXMLStreamOntologyParser",this),recordRouter(0,this) {

			mOntoBuilder = ontoBuilder;
			mAxiomNumber = 0;
			mHasCurrentXMLBase = false;


			mCurrentXMLBaseValidDepth = 0;
			mCurrentXMLNamespaceValidDepth = 0;

			mDataTypeReportErrorCount = 100;
			mAnnotationReportErrorCount = 100;

			mPunctChar = QChar('.');
			mDoublePunctChar = QChar(':');

			initializeParseFunctionJumpHash();
			LOG(INFO,getLogDomain(),logTr("OWL 2 XML Qt stream parser initialized."),this);
		}

		CXMLOWL2StreamHandler::CXMLOWL2StreamHandler(COntologyBuilder* ontoBuilder, CCommandRecordRouter *commandRecordRouter)
			: CLogIdentifier("::Konclude::Parser::OWL2QtXMLStreamOntologyParser",this),recordRouter(*commandRecordRouter,this) {

			mOntoBuilder = ontoBuilder;
			mAxiomNumber = 0;
			mHasCurrentXMLBase = false;

			mCurrentXMLBaseValidDepth = 0;
			mCurrentXMLNamespaceValidDepth = 0;

			mDataTypeReportErrorCount = 100;
			mAnnotationReportErrorCount = 100;

			mPunctChar = QChar('.');
			mDoublePunctChar = QChar(':');

			initializeParseFunctionJumpHash();
			LOG(INFO,getLogDomain(),logTr("OWL 2 XML Qt stream parser initialized."),this);
		}




		bool CXMLOWL2StreamHandler::parseXMLAttributs(const QXmlStreamAttributes& atts) {
			QStringRef xmlBaseStringRef(atts.value("http://www.w3.org/XML/1998/namespace","base"));
			if (!xmlBaseStringRef.isEmpty()) {
				if (mHasCurrentXMLBase) {
					mXMLBaseStack.push(CStreamXMLBaseStackObject(mCurrentXMLBaseValidDepth,mCurrentXMLBase));
				}
				mCurrentXMLBase = xmlBaseStringRef.toString();
				mCurrentXMLBaseValidDepth = mStack.count();
				mHasCurrentXMLBase = true;
			}

			
			QStringRef xmlNamespaceStringRef(atts.value("http://www.w3.org/XML/1998/namespace","xmlns"));
			if (!xmlNamespaceStringRef.isEmpty()) {
				QHash<QString,bool> newOWLNamespaceHash;
				if (!newOWLNamespaceHash.isEmpty()) {
					CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* newParseHash = new CQtHash<CStringRefStringHasher,CStreamParseFunctionData>(*mCurrentParseFunctionJumpHash);
					
					for (QHash<QString,bool>::const_iterator it = newOWLNamespaceHash.constBegin(), itEnd = newOWLNamespaceHash.constEnd(); it != itEnd; ++it) {
						const QString& abbreviationString(it.key());
						bool abbreviationAdded(it.value());
						if (abbreviationAdded) {
							extendParsingHashForNamespace(&mBaseParseFunctionJumpHash,newParseHash,abbreviationString);
						} else {
							reduceParsingHashForNamespace(&mBaseParseFunctionJumpHash,newParseHash,abbreviationString);
						}
					}

					mXMLNamespaceStack.push(CStreamXMLNamespaceStackObject(mCurrentXMLNamespaceValidDepth,newParseHash));
					mCurrentParseFunctionJumpHash = newParseHash;

				}
				mCurrentXMLNamespaceValidDepth = mStack.count();
			}

			return true;
		}



		bool CXMLOWL2StreamHandler::startElement(const QStringRef& namespaceURI, const QStringRef& localName, const QStringRef& qName, const QXmlStreamAttributes& atts) {

			parseXMLAttributs(atts);

			const CStreamParseFunctionData& functionData = mCurrentParseFunctionJumpHash->value(qName);
			if (functionData.mParseDirectFunction) {
				StreamParseFunctionDirect function = functionData.mParseDirectFunction;
				(this->*function)(atts);
				if (!mStack.isEmpty() && !mTmpExpList.isEmpty()) {
					CStreamParseStackObject* topParsStackObj = mStack.top();
					topParsStackObj->addBuildExpressions(&mTmpExpList);
				}
				mStack.push(&mDummyFunctionData);
			} else if (functionData.mParseDelayedFunction) {
				StreamParseFunctionDelayed function = functionData.mParseDelayedFunction;
				bool requiredCardinality = functionData.mRequiresCardinalityParsing;
				if (requiredCardinality) {
					cint64 cardinality = 0;
					bool cardinalityParsed = getCardinality(&cardinality,atts);
					mStack.push(new CStreamParseStackObject(function,cardinality));
				} else {
					mStack.push(new CStreamParseStackObject(function));
				}
			} else {
				mStack.push(&mDummyFunctionData);
			}
			return true;
		}


		bool CXMLOWL2StreamHandler::endElement(const QStringRef& namespaceURI, const QStringRef& localName, const QStringRef& qName) {
			if (!mStack.isEmpty()) {
				CStreamParseStackObject* parseStackObject = mStack.pop();
				if (parseStackObject != &mDummyFunctionData) {

					StreamParseFunctionDelayed function = parseStackObject->mParseFunction;
					if (function) {
						(this->*function)(parseStackObject);
						if (!mStack.isEmpty() && !mTmpExpList.isEmpty()) {
							CStreamParseStackObject* topParsStackObj = mStack.top();
							topParsStackObj->addBuildExpressions(&mTmpExpList);
						}
					}

					delete parseStackObject;
				}
			}
			mTmpExpList.clear();

			if (mHasCurrentXMLBase) {
				if (mStack.count() <= mCurrentXMLBaseValidDepth) {
					if (mXMLBaseStack.isEmpty()) {
						mHasCurrentXMLBase = false;
					} else {
						CStreamXMLBaseStackObject prevXMLBaseObject = mXMLBaseStack.pop();
						mCurrentXMLBase = prevXMLBaseObject.mXMLBaseString;
						mCurrentXMLBaseValidDepth = prevXMLBaseObject.mStackValidDepth;
					}
				}
			}

			if (mStack.count() <= mCurrentXMLNamespaceValidDepth) {
				if (mXMLNamespaceStack.isEmpty()) {
					mCurrentParseFunctionJumpHash = &mBaseParseFunctionJumpHash;
				} else {
					CStreamXMLNamespaceStackObject prevXMLNSObject = mXMLNamespaceStack.pop();
					mCurrentParseFunctionJumpHash = prevXMLNSObject.mParseHash;
					mCurrentXMLNamespaceValidDepth = prevXMLNSObject.mStackValidDepth;
				}
			}


			return true;
		}


		bool CXMLOWL2StreamHandler::startDocument() {
			mStack.push(&mDummyFunctionData);
			return true;
		}

		bool CXMLOWL2StreamHandler::endDocument() {
			if (mStack.count() > 0) {
				CStreamParseStackObject* parseStackObject = mStack.pop();
				parseTellOntologyAxiomNode(parseStackObject);
				delete parseStackObject;
			}
			return true;
		}






		void CXMLOWL2StreamHandler::parseTellOntologyAxiomNode(CStreamParseStackObject* parseStackObj) {
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

		void CXMLOWL2StreamHandler::parseRetractOntologyAxiomNode(CStreamParseStackObject* parseStackObj) {
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


		void CXMLOWL2StreamHandler::parsePrefixNode(const QXmlStreamAttributes& attributes) {
			QStringRef abbName = attributes.value(mNameString);
			QStringRef totalIRIName = attributes.value(mIRIString);

			if (abbName.isEmpty()) {
				abbName = attributes.value(mOWLPrefixNameString);
			}
			if (totalIRIName.isEmpty()) {
				totalIRIName = attributes.value(mOWLPrefixIRIString);
			}

			if (!totalIRIName.isEmpty()) {
				mOntoBuilder->addNameAbbreviation(totalIRIName.toString(),abbName.toString());
			}
		}


		bool CXMLOWL2StreamHandler::startsWithScheme(const QStringRef& uriString) {
			for (int i = 0; i < uriString.count()-1; ++i) {
				const QChar& c = uriString.at(i);
				if (!c.isLetterOrNumber() && c != mPunctChar) {
					if (c == mDoublePunctChar) {
						return i != 0;
					}
					return false;
				}
			}
			return false;
		}


		CStringRefStringHasher CXMLOWL2StreamHandler::getEntityName(const QXmlStreamAttributes& attributes) {
			CStringRefStringHasher entityName;

			bool abbreviated = false;

			QStringRef attrNameString;
			attrNameString = attributes.value(mIRIString);
			if (attrNameString.isEmpty()) {
				attrNameString = attributes.value(mOWLPrefixIRIString);
			}
			if (attrNameString.isEmpty()) {
				attrNameString = attributes.value(mAbbreviatedIRIString);
				abbreviated = true;
			}
			if (attrNameString.isEmpty()) {
				attrNameString = attributes.value(mOWLPrefixAbbreviatedIRIString);
				abbreviated = true;
			}
			if (attrNameString.isEmpty()) {
				attrNameString = attributes.value(mURIString);
			}
			if (attrNameString.isEmpty()) {
				attrNameString = attributes.value(mOWLPrefixURIString);
			}
			if (attrNameString.isEmpty()) {
				abbreviated = false;
			}
			if (abbreviated) {
				entityName = CStringRefStringHasher(mOntoBuilder->resolveAbbreviationName(attrNameString));
			} else {
				if (mHasCurrentXMLBase) {
					if (!startsWithScheme(attrNameString)) {
						entityName = CStringRefStringHasher(mCurrentXMLBase+attrNameString.toString());
					}
				}
				if (entityName.isEmpty()) {
					entityName = CStringRefStringHasher(attrNameString);
				}
			}

			return entityName;
		}

		bool CXMLOWL2StreamHandler::getCardinality(cint64* cardinality, const QXmlStreamAttributes& attributes) {
			QStringRef cardinalityString;
			cardinalityString = attributes.value(mCardinalityString);
			if (cardinalityString.isEmpty()) {
				cardinalityString = attributes.value(mOWLPrefixCardinalityString);
			}

			if (!cardinalityString.isEmpty()) {
				bool succParsed = false;
				*cardinality = cardinalityString.toString().toInt(&succParsed);
				return succParsed;
			} else {
				bool succParsed = false;
				*cardinality = cardinalityString.toString().toInt(&succParsed);
				return succParsed;
			}
		}


		QStringRef CXMLOWL2StreamHandler::getNodeID(const QXmlStreamAttributes& attributes) {
			QStringRef nodeIDString;
			nodeIDString = attributes.value(mNodeIDString);
			if (nodeIDString.isEmpty()) {
				nodeIDString = attributes.value(mOWLPrefixNodeIDString);
			}

			return nodeIDString;
		}


		bool CXMLOWL2StreamHandler::extendParsingHashForNamespace(CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* baseParseFunctionJumpHash, CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* extendParseFunctionJumpHash, const QString& namespaceExtendString) {
			bool extendHash = false;
			for (CQtHash<CStringRefStringHasher,CStreamParseFunctionData>::const_iterator it = baseParseFunctionJumpHash->constBegin(), itEnd = baseParseFunctionJumpHash->constEnd(); it != itEnd; ++it) {
				const CStringRefStringHasher& stringRefStringHasher(it.key());
				const CStreamParseFunctionData& streamParseFunctionData(it.value());
				const QString& keyParseString = stringRefStringHasher.getString();
				QString extendedKeyParseString = namespaceExtendString+keyParseString;
				extendParseFunctionJumpHash->insert(CStringRefStringHasher(extendedKeyParseString),streamParseFunctionData);
				extendHash = true;
			}
			return extendHash;
		}


		bool CXMLOWL2StreamHandler::reduceParsingHashForNamespace(CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* baseParseFunctionJumpHash, CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* reduceParseFunctionJumpHash, const QString& namespaceReduceString) {
			bool reducedHash = false;
			for (CQtHash<CStringRefStringHasher,CStreamParseFunctionData>::const_iterator it = baseParseFunctionJumpHash->constBegin(), itEnd = baseParseFunctionJumpHash->constEnd(); it != itEnd; ++it) {
				const CStringRefStringHasher& stringRefStringHasher(it.key());
				const CStreamParseFunctionData& streamParseFunctionData(it.value());
				const QString& keyParseString = stringRefStringHasher.getString();
				QString reduceKeyParseString = namespaceReduceString+keyParseString;
				reduceParseFunctionJumpHash->insert(CStringRefStringHasher(reduceKeyParseString),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseIgnoredNode));
				reducedHash = true;
			}
			return reducedHash;
		}


		bool CXMLOWL2StreamHandler::initializeParseFunctionJumpHash() {

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




			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Prefix"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParsePrefixNode));

			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Class"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseClassNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("OWLClass"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseClassNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectProperty"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("IndividualVariable"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseIndividualVariable));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Variable"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseIndividualVariable));

			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Declaration"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseDeclarationNode));

			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("SubClassOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseSubClassOfNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("EquivalentClasses"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseEquivalentClassesNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DisjointClasses"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseDisjontClassesNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DisjointUnion"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseDisjontUnionNode));

			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("SubObjectPropertyOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseSubObjectPropertyOfNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectPropertyChain"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyChainNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("EquivalentObjectProperties"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseEquivalentObjectPropertiesNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DisjointObjectProperties"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseDisjointObjectPropertiesNode));

			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectInverseOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseInverseObjectPropertyOfNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("InverseObjectProperties"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseInverseObjectPropertiesNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectPropertyRange"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyRangeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectPropertyDomain"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyDomainNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("TransitiveObjectProperty"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseTransetiveObjectPropertyNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("FunctionalObjectProperty"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseFunctionalObjectPropertyNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("SymmetricObjectProperty"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseSymmetricObjectPropertyNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("AsymmetricObjectProperty"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseAsymmetricObjectPropertyNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ReflexiveObjectProperty"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseReflexiveObjectPropertyNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("IrreflexiveObjectProperty"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseIrreflexiveObjectPropertyNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("InverseFunctionalObjectProperty"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseInverseFunctionalObjectPropertyNode));

			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectIntersectionOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectIntersectionOfNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectUnionOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectUnionOfNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectMaxCardinality"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectMaxCardinalityNode,true));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectMinCardinality"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectMinCardinalityNode,true));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectExactCardinality"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectExactlyCardinalityNode,true));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectSomeValuesFrom"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectSomeValuesFromNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectAllValuesFrom"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectAllValuesFromNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectComplementOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectComplementOfNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectOneOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectOneOfNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectHasValue"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectHasValueNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectHasSelf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectHasSelfNode));

			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("NamedIndividual"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseNamedIndividualNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("AnonymousIndividual"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseAnonymousIndividualNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ClassAssertion"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseClassAssertionNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectPropertyAssertion"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyAssertionNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher(mOWLPrefixString+":ObjectPropertyAssertion"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyAssertionNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("NegativeObjectPropertyAssertion"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseNegativeObjectPropertyAssertionNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher(mOWLPrefixString+":NegativeObjectPropertyAssertion"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseNegativeObjectPropertyAssertionNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("SameIndividual"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseSameIndividualNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher(mOWLPrefixString+":SameIndividual"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseSameIndividualNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DifferentIndividuals"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseDifferentIndividualsNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher(mOWLPrefixString+":DifferentIndividuals"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseDifferentIndividualsNode));

			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("#text"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseIgnoredNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("#comment"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseIgnoredNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ExpectedResult"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseIgnoredNode));
			
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataRange"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Datatype"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataIntersectionOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataUnionOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataComplementOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataOneOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DatatypeRestriction"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataPropertyExpression"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataProperty"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataSomeValuesFrom"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataAllValuesFrom"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataHasValue"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataMinCardinality"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataMaxCardinality"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataExactCardinality"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("SubDataPropertyOf"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("EquivalentDataProperties"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DisjointDataProperties"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataPropertyDomain"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataPropertyRange"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("FunctionalDataProperty"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("HasKey"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DataPropertyAssertion"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("NegativeDataPropertyAssertion"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode));

			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Ontology"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionTellOntologyAxiomNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Tell"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionTellOntologyAxiomNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Whereby"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionTellOntologyAxiomNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Retract"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionRetractOntologyAxiomNode));


			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DLSafeRule"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseRuleNode));

			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Head"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseRuleAtomHeadNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("Body"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseRuleAtomBodyNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ObjectPropertyAtom"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseRulePropertyAtomNode));
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("ClassAtom"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseRuleClassAtomNode));
			
			mBaseParseFunctionJumpHash.insert(CStringRefStringHasher("DifferentIndividualsAtom"),CStreamParseFunctionData(&CXMLOWL2StreamHandler::jumpFunctionParseDifferentIndividualsAtomNode));

			mCurrentParseFunctionJumpHash = &mBaseParseFunctionJumpHash;
			CQtHash<CStringRefStringHasher,CStreamParseFunctionData>* newParseHash = new CQtHash<CStringRefStringHasher,CStreamParseFunctionData>(*mCurrentParseFunctionJumpHash);
			extendParsingHashForNamespace(&mBaseParseFunctionJumpHash,newParseHash,mOWLPrefix);
			mCurrentParseFunctionJumpHash = newParseHash;

			return true;
		}


		CRuleDifferentIndividualsAtomTermExpression* CXMLOWL2StreamHandler::parseRuleDifferentIndividualsAtomNode(CStreamParseStackObject* parseStackObj) {
			CRuleDifferentIndividualsAtomTermExpression* ruleDiffIndAtomExp = nullptr;
			if (parseStackObj->hasExpressions()) {
				CParsingExpressionSplitter mParsingExpSpl(parseStackObj->getExpressions());
				if (mParsingExpSpl.proofExpressionComposition(0,0,0,2,0)) {
					ruleDiffIndAtomExp = new CRuleDifferentIndividualsAtomTermExpression(mParsingExpSpl.getFirstClassVariableTermExpression(),mParsingExpSpl.getSecondClassVariableTermExpression());
					mRuleExpContainer.append(ruleDiffIndAtomExp);
				}
			}
			return ruleDiffIndAtomExp;
		}


		CRulePropertyAtomTermExpression* CXMLOWL2StreamHandler::parseRuleObjectPropertyAtomNode(CStreamParseStackObject* parseStackObj) {
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

		CRuleClassAtomTermExpression* CXMLOWL2StreamHandler::parseRuleClassAtomNode(CStreamParseStackObject* parseStackObj) {
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


		CRuleAtomListBodyExpression* CXMLOWL2StreamHandler::parseRuleAtomBodyNode(CStreamParseStackObject* parseStackObj) {
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


		CRuleAtomListHeadExpression* CXMLOWL2StreamHandler::parseRuleAtomHeadNode(CStreamParseStackObject* parseStackObj) {
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



		QHash<QString,CParseRuleAtom*> CXMLOWL2StreamHandler::createVariableAtomHash(const QList<CParseRuleAtom*>& atomList) {
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



		QMap<cint64,QString> CXMLOWL2StreamHandler::createVariableCountMap(const QList<CParseRuleAtom*>& atomList) {
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



		QList<CParseRuleAtom*> CXMLOWL2StreamHandler::parseRuleAtomNodes(CStreamParseStackObject* parseStackObj, const CQtList<CBuildExpression*>& buildExpList, bool parseHeadAtoms) {
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
						} else if (ruleAtomExp->getType() == CBuildExpression::BETRULEDIFFERENTINDIVIDUALSATOM) {
							CRuleDifferentIndividualsAtomTermExpression* diffIndAtomExp = (CRuleDifferentIndividualsAtomTermExpression*)ruleAtomExp;
							ruleAtom = new CParseRuleAtom(CParseRuleAtom::DIFFERENTINDIVIDUALSATOM);
							CEXPRESSIONLIST<CObjectIndividualVariableExpression*>* varList = diffIndAtomExp->getClassVariableTermExpressionList();
							ruleAtom->mVariableName1 = varList->first()->getName();
							ruleAtom->mVariableName2 = varList->last()->getName();
							atomList.append(ruleAtom);
						}

					}
				}

			}

			return atomList;
		}




		void CXMLOWL2StreamHandler::parseRuleNode(CStreamParseStackObject* parseStackObj) {
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
								CBuildExpression* otherVarExp = mOntoBuilder->getIndividualVariable(otherVarString,mAxiomNumber);
								CBuildExpression* someExp = mOntoBuilder->getObjectSomeValuesFrom(getNeighbourAtomObjectPropertyExpression(headAtom,inversed),otherVarExp);
								headExpressionList.append(someExp);
							} else if (headAtom->mAtomType == CParseRuleAtom::CLASSATOM) {
								headExpressionList.append(headAtom->mAtomExpression);
							} else if (headAtom->mAtomType == CParseRuleAtom::DIFFERENTINDIVIDUALSATOM) {
								bool inversed = false;
								QString otherVarString = headAtom->getOtherVariable(varString,inversed);
								CBuildExpression* otherVarExp = mOntoBuilder->getIndividualVariable(otherVarString,mAxiomNumber);
								CBuildExpression* complExp = mOntoBuilder->getObjectComplementOf(otherVarExp);
								headExpressionList.append(complExp);
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


		CBuildExpression* CXMLOWL2StreamHandler::createBodyExpresions(const QString& varString, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QHash<QString,CParseRuleAtom*>& headVariableAtomHash) {
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

		CBuildExpression* CXMLOWL2StreamHandler::getNeighbourAtomObjectPropertyExpression(CParseRuleAtom* ruleAtom, bool inversed) {
			if (!inversed) {
				return ruleAtom->mAtomExpression;
			} else {
				return mOntoBuilder->getInverseObjectPropertyOf(ruleAtom->mAtomExpression);
			}
		}



		bool CXMLOWL2StreamHandler::hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
				QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash) {
			
			QSet<QString> backTestProcessedVarSet;
			QSet<CParseRuleAtom*> backTestProcessedAtomSet;

			return hasBackExpressionLinks(varString,notRuleAtom,bodyVariableAtomHash,processedVarSet,processedAtomSet,headVariableAtomHash,backTestProcessedVarSet,backTestProcessedAtomSet);
		}



		bool CXMLOWL2StreamHandler::hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, 
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


		CBuildExpression* CXMLOWL2StreamHandler::createBodyExpresions(const QString& varString, bool direct, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash) {
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
					} else if (ruleAtom->mAtomType == CParseRuleAtom::DIFFERENTINDIVIDUALSATOM) {
						if (!processedAtomSet.contains(ruleAtom)) {
							processedAtomSet.insert(ruleAtom);
							processList.append(ruleAtom);
						}
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
					} else if (ruleAtom->mAtomType == CParseRuleAtom::DIFFERENTINDIVIDUALSATOM) {
						bool inversed = false;
						QString otherVarString = ruleAtom->getOtherVariable(varString,inversed);
						CBuildExpression* otherVarExp = mOntoBuilder->getIndividualVariable(otherVarString,mAxiomNumber);
						CBuildExpression* complExp = mOntoBuilder->getObjectComplementOf(otherVarExp);
						expList.append(complExp);
					}

				}
			} 
			if (expList.count() >= 2) {
				return mOntoBuilder->getObjectIntersectionOf(expList);
			}
			return expList.first();
		}




		CDeclarationAxiomExpression* CXMLOWL2StreamHandler::parseDeclarationNode(CStreamParseStackObject* parseStackObj) {
			CDeclarationAxiomExpression* declarationExpression = nullptr;
			if (parseStackObj->hasExpressions()) {
				declarationExpression = mOntoBuilder->getDeclaration(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return declarationExpression;
		}


		CObjectPropertyExpression *CXMLOWL2StreamHandler::parseObjectPropertyNode(const QXmlStreamAttributes& attributes) {
			const CStringRefStringHasher& probName = getEntityName(attributes);
			CObjectPropertyExpression *objectExpression = 0;
			if (!probName.isEmpty()) {
				objectExpression = mOntoBuilder->getObjectProberty(probName.toStringRefernce());
			}
			return objectExpression;
		}


		CClassExpression *CXMLOWL2StreamHandler::parseClassNode(const QXmlStreamAttributes& attributes) {
			const CStringRefStringHasher& className = getEntityName(attributes);
			CClassExpression *classExpression = 0;
			if (!className.isEmpty()) {
				classExpression = mOntoBuilder->getClass(className.toStringRefernce());
			}
			return classExpression;
		}


		CObjectIndividualVariableExpression* CXMLOWL2StreamHandler::parseIndividualVariableNode(const QXmlStreamAttributes& attributes) {
			const CStringRefStringHasher& indiVarName = getEntityName(attributes);
			CObjectIndividualVariableExpression *indiVarExpression = 0;
			if (!indiVarName.isEmpty()) {
				indiVarExpression = mOntoBuilder->getIndividualVariable(indiVarName.toStringRefernce(),mAxiomNumber);
			}
			return indiVarExpression;
		}




		CEquivalentClassesExpression *CXMLOWL2StreamHandler::parseEquivalentClassesNode(CStreamParseStackObject* parseStackObj) {
			CEquivalentClassesExpression *ecExp = 0;
			if (parseStackObj->hasExpressions()) {
				ecExp = mOntoBuilder->getEquivalentClasses(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return ecExp;
		}


		CSubClassOfExpression *CXMLOWL2StreamHandler::parseSubClassOfNode(CStreamParseStackObject* parseStackObj) {
			CSubClassOfExpression *scoExp = 0;
			if (parseStackObj->hasExpressions()) {
				scoExp = mOntoBuilder->getSubClassOf(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return scoExp;
		}


		CObjectPropertyDomainExpression *CXMLOWL2StreamHandler::parseObjectPropertyDomainNode(CStreamParseStackObject* parseStackObj) {
			CObjectPropertyDomainExpression *obPrDoExp = 0;
			if (parseStackObj->hasExpressions()) {
				obPrDoExp = mOntoBuilder->getObjectPropertyDomainExpression(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return obPrDoExp;
		}

		CObjectPropertyRangeExpression *CXMLOWL2StreamHandler::parseObjectPropertyRangeNode(CStreamParseStackObject* parseStackObj) {
			CObjectPropertyRangeExpression *obPrRaExp = 0;
			if (parseStackObj->hasExpressions()) {
				obPrRaExp = mOntoBuilder->getObjectPropertyRangeExpression(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return obPrRaExp;
		}


		CSubObjectPropertyOfExpression *CXMLOWL2StreamHandler::parseSubObjectPropertyOfNode(CStreamParseStackObject* parseStackObj) {
			CSubObjectPropertyOfExpression *sopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				sopoExp = mOntoBuilder->getSubObjectPropertyOf(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return sopoExp;
		}

		CObjectPropertyChainExpression *CXMLOWL2StreamHandler::parseObjectPropertyChainNode(CStreamParseStackObject* parseStackObj) {
			CObjectPropertyChainExpression *sopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				sopoExp = mOntoBuilder->getObjectPropertyChain(*parseStackObj->getExpressions());
			}
			return sopoExp;
		}

		CEquivalentObjectPropertiesExpression *CXMLOWL2StreamHandler::parseEquivalentObjectPropertiesNode(CStreamParseStackObject* parseStackObj) {
			CEquivalentObjectPropertiesExpression *sopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				sopoExp = mOntoBuilder->getEquivalentObjectProperties(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return sopoExp;
		}

		CDisjointObjectPropertiesExpression *CXMLOWL2StreamHandler::parseDisjointObjectPropertiesNode(CStreamParseStackObject* parseStackObj) {
			CDisjointObjectPropertiesExpression *sopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				sopoExp = mOntoBuilder->getDisjointObjectProperties(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return sopoExp;
		}

		CInverseObjectPropertyOfExpression *CXMLOWL2StreamHandler::parseInverseObjectPropertyOfNode(CStreamParseStackObject* parseStackObj) {
			CInverseObjectPropertyOfExpression *iopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				iopoExp = mOntoBuilder->getInverseObjectPropertyOf(*parseStackObj->getExpressions());
			}
			return iopoExp;
		}

		CInverseObjectPropertiesExpression *CXMLOWL2StreamHandler::parseInverseObjectPropertiesNode(CStreamParseStackObject* parseStackObj) {
			CInverseObjectPropertiesExpression *iopoExp = 0;
			if (parseStackObj->hasExpressions()) {
				iopoExp = mOntoBuilder->getInverseObjectProperties(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return iopoExp;
		}


		CDisjointClassesExpression *CXMLOWL2StreamHandler::parseDisjontClassesNode(CStreamParseStackObject* parseStackObj) {
			CDisjointClassesExpression *disClassExp = 0;
			if (parseStackObj->hasExpressions()) {
				disClassExp = mOntoBuilder->getDisjointClasses(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return disClassExp;
		}

		CDisjointUnionExpression *CXMLOWL2StreamHandler::parseDisjontUnionNode(CStreamParseStackObject* parseStackObj) {
			CDisjointUnionExpression *disClassExp = 0;
			if (parseStackObj->hasExpressions()) {
				disClassExp = mOntoBuilder->getDisjointUnion(*parseStackObj->getExpressions());
			}
			return disClassExp;
		}



		CObjectIntersectionOfExpression *CXMLOWL2StreamHandler::parseObjectIntersectionOfNode(CStreamParseStackObject* parseStackObj) {
			CObjectIntersectionOfExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectIntersectionOf(*parseStackObj->getExpressions());
			}
			return exp;
		}

		CObjectUnionOfExpression *CXMLOWL2StreamHandler::parseObjectUnionOfNode(CStreamParseStackObject* parseStackObj) {
			CObjectUnionOfExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectUnionOf(*parseStackObj->getExpressions());
			}
			return exp;
		}

		CObjectMaxCardinalityExpression *CXMLOWL2StreamHandler::parseObjectMaxCardinalityNode(CStreamParseStackObject* parseStackObj) {
			cint64 cardinality = parseStackObj->mCardinality;
			CObjectMaxCardinalityExpression *exp = 0;
			if (parseStackObj->mCardinalityParsed) {
				exp = mOntoBuilder->getObjectMaxCardinality(*parseStackObj->getExpressions(),cardinality);
			}
			return exp;
		}

		CObjectMinCardinalityExpression *CXMLOWL2StreamHandler::parseObjectMinCardinalityNode(CStreamParseStackObject* parseStackObj) {
			cint64 cardinality = parseStackObj->mCardinality;
			CObjectMinCardinalityExpression *exp = 0;
			if (parseStackObj->mCardinalityParsed) {
				exp = mOntoBuilder->getObjectMinCardinality(*parseStackObj->getExpressions(),cardinality);
			}
			return exp;
		}

		CObjectExactlyCardinalityExpression *CXMLOWL2StreamHandler::parseObjectExactlyCardinalityNode(CStreamParseStackObject* parseStackObj) {
			cint64 cardinality = parseStackObj->mCardinality;
			CObjectExactlyCardinalityExpression *exp = 0;
			if (parseStackObj->mCardinalityParsed) {
				exp = mOntoBuilder->getObjectExactlyCardinality(*parseStackObj->getExpressions(),cardinality);
			}
			return exp;
		}

		CObjectSomeValuesFromExpression *CXMLOWL2StreamHandler::parseObjectSomeValuesFromNode(CStreamParseStackObject* parseStackObj) {
			CObjectSomeValuesFromExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectSomeValuesFrom(*parseStackObj->getExpressions());
			}
			return exp;
		}

		CObjectHasValueExpression* CXMLOWL2StreamHandler::parseObjectHasValueNode(CStreamParseStackObject* parseStackObj) {
			CObjectHasValueExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectHasValue(*parseStackObj->getExpressions());
			}
			return exp;
		}

		CObjectHasSelfExpression* CXMLOWL2StreamHandler::parseObjectHasSelfNode(CStreamParseStackObject* parseStackObj) {
			CObjectHasSelfExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectHasSelf(*parseStackObj->getExpressions());
			}
			return exp;
		}


		CObjectAllValuesFromExpression *CXMLOWL2StreamHandler::parseObjectAllValuesFromNode(CStreamParseStackObject* parseStackObj) {
			CObjectAllValuesFromExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectAllValuesFrom(*parseStackObj->getExpressions());
			}
			return exp;
		}


		CObjectComplementOfExpression *CXMLOWL2StreamHandler::parseObjectComplementOfNode(CStreamParseStackObject* parseStackObj) {
			CObjectComplementOfExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectComplementOf(*parseStackObj->getExpressions());
			}
			return exp;
		}

		CObjectOneOfExpression *CXMLOWL2StreamHandler::parseObjectOneOfNode(CStreamParseStackObject* parseStackObj) {
			CObjectOneOfExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectOneOf(*parseStackObj->getExpressions());
			}
			return exp;
		}


		CTransetiveObjectPropertyExpression *CXMLOWL2StreamHandler::parseTransetiveObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			CTransetiveObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getTransetiveObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}


		CFunctionalObjectPropertyExpression *CXMLOWL2StreamHandler::parseFunctionalObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			CFunctionalObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getFunctionalObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}


		CSymmetricObjectPropertyExpression* CXMLOWL2StreamHandler::parseSymmetricObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			CSymmetricObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getSymmetricObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CAsymmetricObjectPropertyExpression* CXMLOWL2StreamHandler::parseAsymmetricObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			CAsymmetricObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getAsymmetricObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CReflexiveObjectPropertyExpression* CXMLOWL2StreamHandler::parseReflexiveObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			CReflexiveObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getReflexiveObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CIrreflexiveObjectPropertyExpression* CXMLOWL2StreamHandler::parseIrreflexiveObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			CIrreflexiveObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getIrreflexiveObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CInverseFunctionalObjectPropertyExpression* CXMLOWL2StreamHandler::parseInverseFunctionalObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			CInverseFunctionalObjectPropertyExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getInverseFunctionalObjectProperty(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}





		CClassAssertionExpression *CXMLOWL2StreamHandler::parseClassAssertionNode(CStreamParseStackObject* parseStackObj) {
			CClassAssertionExpression *exp = 0;
			if (parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getClassAssertion(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}


		CSameIndividualExpression *CXMLOWL2StreamHandler::parseSameIndividualNode(CStreamParseStackObject* parseStackObj) {
			CSameIndividualExpression *exp = 0;
			if (mOntoBuilder && parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getSameIndividual(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CDifferentIndividualsExpression *CXMLOWL2StreamHandler::parseDifferentIndividualsNode(CStreamParseStackObject* parseStackObj) {
			CDifferentIndividualsExpression *exp = 0;
			if (mOntoBuilder && parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getDifferentIndividuals(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}


		CNamedIndividualExpression *CXMLOWL2StreamHandler::parseNamedIndividualNode(const QXmlStreamAttributes& attributes) {
			const CStringRefStringHasher& indiNameName = getEntityName(attributes);
			CNamedIndividualExpression *exp = 0;
			if (!indiNameName.isEmpty()) {
				if (mOntoBuilder) {
					exp = mOntoBuilder->getNamedIndividual(indiNameName.toStringRefernce());
				}
			}
			return exp;
		}


		CAnonymousIndividualExpression *CXMLOWL2StreamHandler::parseAnonymousIndividualNode(const QXmlStreamAttributes& attributes) {
			if (mOntologyName.isEmpty()) {
				mOntologyName = mOntoBuilder->getOntologyName();
			}
			QStringRef indiNameName = getNodeID(attributes);
			CAnonymousIndividualExpression *exp = 0;
			if (!indiNameName.isEmpty()) {
				if (mOntoBuilder) {
					exp = mOntoBuilder->getAnonymousIndividual(mOntologyName.leftRef(-1),indiNameName);
				}
			}
			return exp;
		}

		CObjectPropertyAssertionExpression *CXMLOWL2StreamHandler::parseObjectPropertyAssertionNode(CStreamParseStackObject* parseStackObj) {
			CObjectPropertyAssertionExpression *exp = 0;
			if (mOntoBuilder && parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getObjectPropertyAssertion(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}

		CNegativeObjectPropertyAssertionExpression *CXMLOWL2StreamHandler::parseNegativeObjectPropertyAssertionNode(CStreamParseStackObject* parseStackObj) {
			CNegativeObjectPropertyAssertionExpression *exp = 0;
			if (mOntoBuilder && parseStackObj->hasExpressions()) {
				exp = mOntoBuilder->getNegativeObjectPropertyAssertion(*parseStackObj->getExpressions());
				++mAxiomNumber;
			}
			return exp;
		}






		void CXMLOWL2StreamHandler::jumpFunctionParseRuleNode(CStreamParseStackObject* parseStackObj) {
			parseRuleNode(parseStackObj);
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseRulePropertyAtomNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseRuleObjectPropertyAtomNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseRuleClassAtomNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseRuleClassAtomNode(parseStackObj));
		}


		void CXMLOWL2StreamHandler::jumpFunctionParseDifferentIndividualsAtomNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseRuleDifferentIndividualsAtomNode(parseStackObj));
		}




		void CXMLOWL2StreamHandler::jumpFunctionParseRuleAtomBodyNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseRuleAtomBodyNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseRuleAtomHeadNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseRuleAtomHeadNode(parseStackObj));
		}






		void CXMLOWL2StreamHandler::jumpFunctionTellOntologyAxiomNode(CStreamParseStackObject* parseStackObj) {
			parseTellOntologyAxiomNode(parseStackObj);
		}

		void CXMLOWL2StreamHandler::jumpFunctionRetractOntologyAxiomNode(CStreamParseStackObject* parseStackObj) {
			parseRetractOntologyAxiomNode(parseStackObj);
		}

		void CXMLOWL2StreamHandler::jumpFunctionParsePrefixNode(const QXmlStreamAttributes& attributes) {
			parsePrefixNode(attributes);
		}






		void CXMLOWL2StreamHandler::addTemporaryBuildExpression(CBuildExpression* buildExp) {
			if (buildExp) {
				mTmpExpList.append(buildExp);
			}
		}

		void CXMLOWL2StreamHandler::addTemporaryBuildExpressions(QList<CBuildExpression*>* buildExps) {
			if (buildExps) {
				for (QList<CBuildExpression*>::const_iterator it = buildExps->constBegin(), itEnd = buildExps->constEnd(); it != itEnd; ++it) {
					CBuildExpression* buildExp = *it;
					if (buildExp) {
						mTmpExpList.append(buildExp);
					}
				}
			}
		}





		void CXMLOWL2StreamHandler::jumpFunctionParseClassNode(const QXmlStreamAttributes& attributes) {
			addTemporaryBuildExpression(parseClassNode(attributes));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyNode(const QXmlStreamAttributes& attributes) {
			addTemporaryBuildExpression(parseObjectPropertyNode(attributes));
		}


		void CXMLOWL2StreamHandler::jumpFunctionParseIndividualVariable(const QXmlStreamAttributes& attributes) {
			addTemporaryBuildExpression(parseIndividualVariableNode(attributes));
		}


		void CXMLOWL2StreamHandler::jumpFunctionParseDeclarationNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseDeclarationNode(parseStackObj));
		}


		void CXMLOWL2StreamHandler::jumpFunctionParseEquivalentClassesNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseEquivalentClassesNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseSubClassOfNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseSubClassOfNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseDisjontClassesNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseDisjontClassesNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseDisjontUnionNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseDisjontUnionNode(parseStackObj));
		}


		void CXMLOWL2StreamHandler::jumpFunctionParseSubObjectPropertyOfNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseSubObjectPropertyOfNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyChainNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectPropertyChainNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseEquivalentObjectPropertiesNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseEquivalentObjectPropertiesNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseDisjointObjectPropertiesNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseDisjointObjectPropertiesNode(parseStackObj));
		}


		void CXMLOWL2StreamHandler::jumpFunctionParseObjectIntersectionOfNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectIntersectionOfNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectUnionOfNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectUnionOfNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectMaxCardinalityNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectMaxCardinalityNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectMinCardinalityNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectMinCardinalityNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectExactlyCardinalityNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectExactlyCardinalityNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectSomeValuesFromNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectSomeValuesFromNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectAllValuesFromNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectAllValuesFromNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectComplementOfNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectComplementOfNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectOneOfNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectOneOfNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectHasValueNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectHasValueNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectHasSelfNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectHasSelfNode(parseStackObj));
		}


		void CXMLOWL2StreamHandler::jumpFunctionParseClassAssertionNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseClassAssertionNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseNamedIndividualNode(const QXmlStreamAttributes& attributes) {
			addTemporaryBuildExpression(parseNamedIndividualNode(attributes));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseAnonymousIndividualNode(const QXmlStreamAttributes& attributes) {
			addTemporaryBuildExpression(parseAnonymousIndividualNode(attributes));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyAssertionNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectPropertyAssertionNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseNegativeObjectPropertyAssertionNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseNegativeObjectPropertyAssertionNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseSameIndividualNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseSameIndividualNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseDifferentIndividualsNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseDifferentIndividualsNode(parseStackObj));
		}


		void CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyDomainNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectPropertyDomainNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseObjectPropertyRangeNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseObjectPropertyRangeNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseInverseObjectPropertyOfNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseInverseObjectPropertyOfNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseInverseObjectPropertiesNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseInverseObjectPropertiesNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseTransetiveObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseTransetiveObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseFunctionalObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseFunctionalObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseSymmetricObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseSymmetricObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseAsymmetricObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseAsymmetricObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseReflexiveObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseReflexiveObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseIrreflexiveObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseIrreflexiveObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseInverseFunctionalObjectPropertyNode(CStreamParseStackObject* parseStackObj) {
			addTemporaryBuildExpression(parseInverseFunctionalObjectPropertyNode(parseStackObj));
		}

		void CXMLOWL2StreamHandler::jumpFunctionParseIgnoredNode(CStreamParseStackObject* parseStackObj) {
		}

		void CXMLOWL2StreamHandler::jumpFunctionUnsupportedDatatypeNode(CStreamParseStackObject* parseStackObj) {
			if (mDataTypeReportErrorCount > 0) {
				--mDataTypeReportErrorCount;
				LOG(ERROR,getLogDomain(),logTr("Skipped parsing of not supported datatype expression."),this);
			} else if (mDataTypeReportErrorCount == 0) {
				--mDataTypeReportErrorCount;
				LOG(ERROR,getLogDomain(),logTr("Remaining not supported datatype expressions are ignored."),this);
			}
		}


	}; // end namespace Parser

}; // end namespace Konclude
