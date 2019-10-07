/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CKRSSOntologyParser.h"


namespace Konclude {

	namespace Parser {



		CKRSSOntologyParser::CKRSSOntologyParser(COntologyBuilder* ontoBuilder)
				: CLogIdentifier("::Konclude::Parser::KRSSOntologyParser",this),recordRouter(0,this) {

			mOntoBuilder = ontoBuilder;
			LOG(INFO,getLogDomain(),logTr("Created KRSS Parser."),this);

			mDefName = QString("KRSSTestConcept");
			mDefNumber = 1;

#ifdef KONCLUDE_PARSER_USEPARSEJUMPFUNCTIONHASH
			initializeParseFunctionJumpHash();
#endif
		}


		CKRSSOntologyParser::CKRSSOntologyParser(COntologyBuilder* ontoBuilder, const QString& defNameString, CCommandRecordRouter *commandRecordRouter)
				: CLogIdentifier("::Konclude::Parser::KRSSOntologyParser",this),recordRouter(*commandRecordRouter,this) {
				
			mOntoBuilder = ontoBuilder;
			LOG(INFO,getLogDomain(),logTr("Created KRSS Parser."),this);

			mDefName = defNameString;
			if (mDefName.isEmpty()) {
				mDefName = "KRSSTestConcept";
			}
			mDefNumber = 1;

#ifdef KONCLUDE_PARSER_USEPARSEJUMPFUNCTIONHASH
			initializeParseFunctionJumpHash();
#endif
		}


		CKRSSOntologyParser::~CKRSSOntologyParser() {
		}




		bool CKRSSOntologyParser::parseOntologyFile(const QString& filename) {
			LOG(INFO,getLogDomain(),logTr("Loading KRSS File: '%1'.").arg(filename),this);
			bool successfullParsed = true;


			// parse file
			QFile file(filename);
			if (file.open(QIODevice::ReadOnly)) {
				while (!file.atEnd()) {
					QByteArray krssLine = file.readLine();
					parseKRSSLine(krssLine);
				}

				file.close();
			} else {
				successfullParsed = false;
				LOG(ERROR,getLogDomain(),logTr("File '%1' not found or couldn't be opened.").arg(filename),this);
			}

			return successfullParsed;
		}


		bool CKRSSOntologyParser::parseOntologyText(const QString& text) {
			bool successfullParsed = false;
			QStringList list(text.split("\n"));
			cint64 listCount = list.count();
			for (cint64 i = 0; i < listCount; ++i) {
				QString string = list.at(i);
				parseKRSSLine(string);
			}
			return successfullParsed;
		}



		CBuildExpression* CKRSSOntologyParser::parseKRSSLine(const QString& line) {
			CBuildExpression* buildExpression = nullptr;
			if (line.startsWith("(")) {
				cint64 index = 0;
				buildExpression = parseKRSSExpression(line,index);
				QString nextDefNameString = QString("%1%2").arg(mDefName).arg(mDefNumber++);
				CBuildExpression* classExp = mOntoBuilder->getClass(nextDefNameString);
				parseDeclarationNode(classExp);
				CAxiomExpression* subClassExp = mOntoBuilder->getSubClassOf(classExp,buildExpression);
				mOntoBuilder->tellOntologyAxiom(subClassExp);
			}
			return buildExpression;
		}


		CBuildExpression* CKRSSOntologyParser::parseKRSSExpression(const QString& line, cint64& index) {
			CBuildExpression* buildExpression = nullptr;
			cint64 beginIndex = index;
			cint64 clamInc = 0;
			while (index < line.size() && line[(uint)index] == QChar('(')) {
				++clamInc;
				++index;
			}
			QString ident = parseIdentifierNode(line,index);

			ParseFunction function = mParseFunctionJumpHash.value(ident);
			if (function) {
				buildExpression = (this->*function)(line,index);
			} else {
				buildExpression = mOntoBuilder->getClass(ident);
				parseDeclarationNode(buildExpression);
			}

			index += clamInc;
			return buildExpression;
		}



		bool CKRSSOntologyParser::initializeParseFunctionJumpHash() {

			mParseFunctionJumpHash.insert("AND",&CKRSSOntologyParser::parseObjectIntersectionOfNode);
			mParseFunctionJumpHash.insert("OR",&CKRSSOntologyParser::parseObjectUnionOfNode);
			mParseFunctionJumpHash.insert("SOME",&CKRSSOntologyParser::parseObjectSomeValuesFromNode);
			mParseFunctionJumpHash.insert("ALL",&CKRSSOntologyParser::parseObjectAllValuesFromNode);
			mParseFunctionJumpHash.insert("NOT",&CKRSSOntologyParser::parseObjectComplementOfNode);

			return true;
		}


		CDeclarationAxiomExpression* CKRSSOntologyParser::parseDeclarationNode(CBuildExpression* expression) {
			QList<CBuildExpression *> builds;
			builds.append(expression);
			CDeclarationAxiomExpression* declarationExpression = mOntoBuilder->getDeclaration(builds);
			mOntoBuilder->tellOntologyAxiom(declarationExpression);
			return declarationExpression;
		}


		QString CKRSSOntologyParser::parseIdentifierNode(const QString& text, cint64& index) {
			QString entitiy;
			cint64 endIdentL = text.indexOf(" ",index);
			cint64 endIdentC = text.indexOf(")",index);

			cint64 endIdent = 0;
			if (endIdentL == -1) {
				endIdent = endIdentC;
			} else if (endIdentC == -1) {
				endIdent = endIdentL;
			} else {
				endIdent = qMin(endIdentL,endIdentC);
			}
			QString ident = text.mid(index,endIdent-index);
			index = endIdent;
			if (endIdent == endIdentL) {
				++index;
			}
			return ident;
		}



		bool CKRSSOntologyParser::checkIdentifierNode(const QString& text, const QString& expectedString, cint64& index) {
			QString entitiy;
			cint64 clOff = 0;
			if (text[(uint)index] == QChar('(')) {
				++clOff;
			}
			cint64 endIdentL = text.indexOf(" ",index+clOff);
			cint64 endIdentC = text.indexOf(")",index+clOff);

			cint64 endIdent = 0;
			if (endIdentL == -1) {
				endIdent = endIdentC;
			} else if (endIdentC == -1) {
				endIdent = endIdentL;
			} else {
				endIdent = qMin(endIdentL,endIdentC);
			}
			QString ident = text.mid(index+clOff,endIdent-index-clOff);
			bool expected = ident == expectedString;
			if (expected) {
				index = endIdent;
				if (endIdent == endIdentL) {
					++index;
				}
			}
			return expected;
		}

		CObjectPropertyExpression *CKRSSOntologyParser::parseObjectPropertyNode(const QString& text, cint64& index) {
			CObjectPropertyExpression* propExpression = nullptr;
			QString ident = parseIdentifierNode(text,index);
			propExpression = mOntoBuilder->getObjectProberty(ident);
			parseDeclarationNode(propExpression);
			return propExpression;
		}


		CClassTermExpression *CKRSSOntologyParser::parseClassNode(const QString& text, cint64& index) {
			CClassTermExpression* classExpression = nullptr;
			QString ident = parseIdentifierNode(text,index);
			if (ident == "*BOTTOM*") {
				classExpression = mOntoBuilder->getBottomClass();
			} else if (ident == "*TOP*") {
				classExpression = mOntoBuilder->getTopClass();
			} else {
				classExpression = mOntoBuilder->getClass(ident);
				parseDeclarationNode(classExpression);
			}
			return classExpression;
		}


		QList<CBuildExpression *> CKRSSOntologyParser::parseClassChildNodes(const QString& text, cint64& index) {
			QList<CBuildExpression *> builds;
			while (index < text.size() && text[(uint)index] != QChar(')')) {
				if (text[(uint)index] == QChar(' ')) {
					++index;
				} else {
					CBuildExpression *expression = parseKRSSExpression(text,index);
					if (expression) {
						builds.append(expression);
					}
				}
			}
			return builds;
		}


		QList<CBuildExpression *> CKRSSOntologyParser::parseSpecialClassChildNodes(const QString& text, const QString& identicalChild, cint64& index) {
			QList<CBuildExpression *> builds;
			cint64 identDepth = 0;
			while (index < text.size() && (identDepth > 0 || text[(uint)index] != QChar(')'))) {
				if (text[(uint)index] == QChar(' ')) {
					++index;
				} else if (text[(uint)index] == QChar(')')) {
					--identDepth;
					++index;
				} else {
					if (checkIdentifierNode(text,identicalChild,index)) {
						++identDepth;
					} else {
						CBuildExpression *expression = parseKRSSExpression(text,index);
						if (expression) {
							builds.append(expression);
						}
					}
				}
			}
			return builds;
		}


		QList<CBuildExpression* > CKRSSOntologyParser::parsePropertyClassChildNodes(const QString& text, cint64& index) {
			QList<CBuildExpression *> builds;
			CBuildExpression* properyExp = parseObjectPropertyNode(text,index);
			CBuildExpression* classExp = parseKRSSExpression(text,index);
			if (properyExp) {
				builds.append(properyExp);
			}
			if (classExp) {
				builds.append(classExp);
			}
			return builds;
		}


		CBuildExpression *CKRSSOntologyParser::parseObjectIntersectionOfNode(const QString& text, cint64& index) {
			QList<CBuildExpression *> builds = parseSpecialClassChildNodes(text,"AND",index);
			CObjectIntersectionOfExpression *exp = mOntoBuilder->getObjectIntersectionOf(builds);
			return exp;
		}

		CBuildExpression *CKRSSOntologyParser::parseObjectUnionOfNode(const QString& text, cint64& index) {
			QList<CBuildExpression *> builds = parseSpecialClassChildNodes(text,"OR",index);
			CObjectUnionOfExpression *exp = mOntoBuilder->getObjectUnionOf(builds);
			return exp;
		}


		CBuildExpression *CKRSSOntologyParser::parseObjectSomeValuesFromNode(const QString& text, cint64& index) {
			QList<CBuildExpression *> builds = parsePropertyClassChildNodes(text,index);
			CObjectSomeValuesFromExpression *exp = mOntoBuilder->getObjectSomeValuesFrom(builds);
			return exp;
		}


		CBuildExpression *CKRSSOntologyParser::parseObjectAllValuesFromNode(const QString& text, cint64& index) {
			QList<CBuildExpression *> builds = parsePropertyClassChildNodes(text,index);
			CObjectAllValuesFromExpression *exp = mOntoBuilder->getObjectAllValuesFrom(builds);
			return exp;
		}


		CBuildExpression *CKRSSOntologyParser::parseObjectComplementOfNode(const QString& text, cint64& index) {
			QList<CBuildExpression *> builds = parseClassChildNodes(text,index);
			CObjectComplementOfExpression *exp = mOntoBuilder->getObjectComplementOf(builds);
			return exp;
		}






		CBuildExpression* CKRSSOntologyParser::jumpFunctionParseObjectIntersectionOfNode(const QString& text, cint64& index) {
			return parseObjectIntersectionOfNode(text,index);
		}

		CBuildExpression* CKRSSOntologyParser::jumpFunctionParseObjectUnionOfNode(const QString& text, cint64& index) {
			return parseObjectUnionOfNode(text,index);
		}


		CBuildExpression* CKRSSOntologyParser::jumpFunctionParseObjectSomeValuesFromNode(const QString& text, cint64& index) {
			return parseObjectSomeValuesFromNode(text,index);
		}

		CBuildExpression* CKRSSOntologyParser::jumpFunctionParseObjectAllValuesFromNode(const QString& text, cint64& index) {
			return parseObjectAllValuesFromNode(text,index);
		}

		CBuildExpression* CKRSSOntologyParser::jumpFunctionParseObjectComplementOfNode(const QString& text, cint64& index) {
			return parseObjectComplementOfNode(text,index);
		}




	}; // end namespace Parser

}; // end namespace Konclude
