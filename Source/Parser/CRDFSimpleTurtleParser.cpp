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

#include "CRDFSimpleTurtleParser.h"


namespace Konclude {

	namespace Parser {



		CRDFSimpleTurtleParser::CRDFSimpleTurtleParser() {

			mPrefixHash.insert("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
			nextAnonymousNodeId = 0;

		}


		CRDFSimpleTurtleParser::~CRDFSimpleTurtleParser() {
		}


		QString CRDFSimpleTurtleParser::getBlankNode(const QString& blankNodeString) {
			return blankNodeString.mid(2);
		}


		QString CRDFSimpleTurtleParser::getNextAnonymousNode() {
			return QString("_:konclude_internal_anonymous_node_%1").arg(nextAnonymousNodeId++);
		}



		QString CRDFSimpleTurtleParser::getIRI(const QString& iriString) {
			if (iriString.startsWith("<") && iriString.endsWith(">")) {
				return iriString.mid(1, iriString.length() - 2);
			} else {
				int colonPos = iriString.indexOf(':');
				if (colonPos != -1) {
					QString prefix = iriString.mid(0, colonPos);
					QString prefixResolvedString = mPrefixHash.value(prefix);
					if (!prefixResolvedString.isEmpty()) {
						QString remainingIRI = iriString.mid(colonPos+1);
						if (!remainingIRI.isEmpty()) {
							return prefixResolvedString + remainingIRI;
						}
					}
				}

				return mBaseIRI + iriString;
			}
		}


		QStringList CRDFSimpleTurtleParser::getBracketContent(QStringList& partList, QChar openingBracket, QChar closingBracket, bool bracketOpened, bool exculdeOthers) {
			int bracketOpen = 0;
			if (bracketOpened) {
				bracketOpen++;
			}
			QStringList splittedPartList;
			while (!partList.isEmpty()) {
				QString partString = partList.takeFirst();


				bool isOpeningBracket = false;
				bool isBracket = false;
				bool isClosingBracket = false;


				if (partString == openingBracket) {
					bracketOpened = true;
					++bracketOpen;
					isOpeningBracket = true;
				} else if (partString == closingBracket) {
					--bracketOpen;
					isClosingBracket = true;
				}

				if (isOpeningBracket || isClosingBracket) {
					isBracket = true;
				}

				if (exculdeOthers || !isBracket && bracketOpened) {
					splittedPartList.append(partString);
				}


				if (bracketOpen == 0 && bracketOpened) {
					return splittedPartList;
				}
			}
			if (exculdeOthers) {
				return splittedPartList;
			} else {
				return QStringList();
			}
		}


		bool CRDFSimpleTurtleParser::getNextRDFStringTriple(QStringList& partList, CRDFStringTriple& lastNextTriple, QList<CRDFStringTriple>& nextTripleList, int startingSopPos) {
			int sopPos = startingSopPos;
			bool maybeLastParsingTerminator = true;
			while (!partList.isEmpty()) {
				if (partList.first() == "." && !nextTripleList.isEmpty()) {
					lastNextTriple = nextTripleList.takeFirst();
					return true;
				}
				QString part = partList.takeFirst();
				bool isTriplePart = false;
				if (part == ".") {
					if (sopPos != 3) {
						if (maybeLastParsingTerminator) {
							maybeLastParsingTerminator = false;
						} else {
							mParsingError = true;
							mLastParsingError = QString("RDF triple terminator '.' is not at correct position");
						}
					}
					sopPos = 0;
				} else if (part == ";") {
					if (sopPos != 3) {
						if (maybeLastParsingTerminator) {
							maybeLastParsingTerminator = false;
						} else {
							mParsingError = true;
							mLastParsingError = QString("RDF object concatenator ';' is not at correct position");
						}
					}
					sopPos = 1;
				} else if (part == ",") {
					if (sopPos != 3) {
						if (maybeLastParsingTerminator) {
							maybeLastParsingTerminator = false;
						} else {
							mParsingError = true;
							mLastParsingError = QString("RDF predicate object concatenator ',' is not at correct position");
						}
					}
					sopPos = 2;
				} else if (part.startsWith("?") || part.startsWith("$")) {
					isTriplePart = true;
				} else if (part == "{") {
					getBracketContent(partList, '{', '}', true);
					sopPos = 0;
				} else if (part == "[") {
					part = getNextAnonymousNode();
					QStringList anonymousNodeTripleParts = getBracketContent(partList, '[', ']', true);
					QList<CRDFStringTriple> tmpNextTripleList;
					CRDFStringTriple lastTriple;
					lastTriple.string[0] = part;
					while (getNextRDFStringTriple(anonymousNodeTripleParts, lastTriple, tmpNextTripleList, 1)) {
						nextTripleList.append(lastTriple);
					}
					isTriplePart = true;
				} else if (part == "_:") {
					part = getBlankNode(part);
					isTriplePart = true;
				} else {
					part = getIRI(part);
					isTriplePart = true;
				}
				if (isTriplePart) {
					if (sopPos <= 2) {
						lastNextTriple.string[sopPos] = part;
					} else {
						mParsingError = true;
						mLastParsingError = QString("RDF triple not terminated");
					}
					++sopPos;
					if (sopPos >= 3) {
						return true;
					}
				}
			}
			if (!nextTripleList.isEmpty()) {
				lastNextTriple = nextTripleList.takeFirst();
				return true;
			}			
			return false;

		}



		QPair<QString, QString> CRDFSimpleTurtleParser::extractDataLiteral(const QString& string) {
			QString dataString = string;
			QString dataLexicalValueString;
			QString datatypeString = mParsingUtilities.DATATYPE_XML_STRING;
			if (dataString.contains("^^")) {
				QStringList splittedParts = dataString.split("^^");
				dataString = splittedParts.first();
				datatypeString = getIRI(splittedParts.last());
			} else {
				int lastLanguageTagSeparator = dataString.lastIndexOf("@");
				if (lastLanguageTagSeparator >= 0 && lastLanguageTagSeparator < dataString.length() - 2) {
					if (mParsingUtilities.matchesLanguageTag(dataString.mid(lastLanguageTagSeparator + 1))) {
						datatypeString = mParsingUtilities.DATATYPE_RDF_PLAINLITERAL;
					}
				}
			}
			if (dataString.startsWith("\"") && dataString.endsWith("\"") || dataString.startsWith("'") && dataString.endsWith("'")) {
				dataString.remove(dataString.length()-1, 1);
				dataString.remove(0, 1);
			}
			return QPair<QString, QString>(dataString, datatypeString);
		}



		QHash<QString, CRDFHahsedSubjectData>* CRDFSimpleTurtleParser::getRDFSubjectHash(QStringList& partList) {
			QHash<QString, CRDFHahsedSubjectData>* subjectHash = new QHash<QString, CRDFHahsedSubjectData>();
			QList<CRDFStringTriple> nextTripleList;

			CRDFStringTriple lastTriple;
			while (getNextRDFStringTriple(partList,lastTriple, nextTripleList)) {
				subjectHash->insertMulti(lastTriple.string[0],CRDFHahsedSubjectData(lastTriple));
			}

			return subjectHash;
		}



		QList<CRDFStringTriple>* CRDFSimpleTurtleParser::getRDFStringTriples(QStringList& partList) {
			QList<CRDFStringTriple>* tripleList = new QList<CRDFStringTriple>();
			QList<CRDFStringTriple> nextTripleList;

			CRDFStringTriple lastTriple;
			while (getNextRDFStringTriple(partList, lastTriple, nextTripleList)) {
				tripleList->append(lastTriple);
			}

			return tripleList;
		}




		QString CRDFSimpleTurtleParser::getNextPart(const QString& text, cint64& pos) {
			cint64 startPos = pos;
			cint64 size = text.size();
			bool hasNonSeparator = false;
			bool comment = false;
			bool string = false;
			while (pos < size) {
				QChar textChar = text.at(pos);
				bool isSeperator = false;
				bool isBracket = false;
				if (comment && (textChar == '\r' || textChar == '\n')) {
					comment = false;
					startPos = pos;
				}
				if (textChar == '#' && !hasNonSeparator) {
					comment = true;
				}
				if (!comment) {
					if (textChar == "\"") {
						string = !string;
					}
					if (!string) {
						if (textChar == ' ' || textChar == '\t' || textChar == '\r' || textChar == '\n') {
							isSeperator = true;
						}
						if (textChar == '{' || textChar == '}' || textChar == '(' || textChar == ')' || textChar == '[' || textChar == ']') {
							isBracket = true;
							isSeperator = true;
						}
					}
					if (isSeperator) {
						if (hasNonSeparator) {
							QString partString = text.mid(startPos, pos - startPos);
							return partString.trimmed();
						}
						if (isBracket) {
							++pos;
							return textChar;
						}
					} else {
						if (!hasNonSeparator) {
							hasNonSeparator = true;
							startPos = pos;
						}
					}
				}
				++pos;
			}
			if (!comment) {
				return text.mid(startPos).trimmed();
			} else {
				return QString();
			}
		}




	}; // end namespace Parser

}; // end namespace Konclude
