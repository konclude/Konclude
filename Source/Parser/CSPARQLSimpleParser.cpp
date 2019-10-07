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

#include "CSPARQLSimpleParser.h"


namespace Konclude {

	namespace Parser {



		CSPARQLSimpleParser::CSPARQLSimpleParser() {

			mKeywordSet.insert("BASE");
			mKeywordSet.insert("@BASE");
			mKeywordSet.insert("PREFIX");
			mKeywordSet.insert("@PREFIX");
			mKeywordSet.insert("SELECT");
			mKeywordSet.insert("CONSTRUCT");
			mKeywordSet.insert("DESCRIBE");
			mKeywordSet.insert("ASK");

		}


		CSPARQLSimpleParser::~CSPARQLSimpleParser() {
		}



		bool CSPARQLSimpleParser::isKeyword(const QString& part) {
			return mKeywordSet.contains(part.toUpper());
		}



		bool CSPARQLSimpleParser::getNextRDFStringTriple(QStringList& partList, CRDFStringTriple& lastNextTriple, QList<CRDFStringTriple>& nextTripleList, QList<QStringList>& filterList, int startingSopPos) {
			int sopPos = startingSopPos;
			while (!partList.isEmpty()) {
				if (partList.first() == "." && !nextTripleList.isEmpty()) {
					lastNextTriple = nextTripleList.takeFirst();
					return true;
				}
				QString part = partList.takeFirst();
				bool isTriplePart = false;
				if (part == ".") {
					sopPos = 0;
				} else if (part == ";") {
					sopPos = 1;
				} else if (part == ",") {
					sopPos = 2;
				} else if (part.startsWith("?") || part.startsWith("$")) {
					isTriplePart = true;
				} else if (part.toUpper().startsWith("FILTER")) {
					QStringList filterParts = getBracketContent(partList, '(', ')', false);
					filterList.append(filterParts);
					sopPos = 0;
				} else if (part == "{") {
					getBracketContent(partList, '{', '}', true);
					sopPos = 0;
				} else if (part == "[") {
					part = getNextAnonymousNode();
					QStringList anonymousNodeTripleParts = getBracketContent(partList, '[', ']', true);
					QList<CRDFStringTriple> tmpNextTripleList;
					CRDFStringTriple lastTriple;
					lastTriple.string[0] = part;
					while (getNextRDFStringTriple(anonymousNodeTripleParts, lastTriple, tmpNextTripleList, filterList, 1)) {
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
					lastNextTriple.string[sopPos] = part;
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

		QHash<QString, CRDFHahsedSubjectData>* CSPARQLSimpleParser::getRDFSubjectHash(QStringList& partList) {
			return CRDFSimpleTurtleParser::getRDFSubjectHash(partList);
		}


		QHash<QString, CRDFHahsedSubjectData>* CSPARQLSimpleParser::getRDFSubjectHash(QStringList& partList, QList<QStringList>& filterList) {
			QHash<QString, CRDFHahsedSubjectData>* subjectHash = new QHash<QString, CRDFHahsedSubjectData>();
			QList<CRDFStringTriple> nextTripleList;

			CRDFStringTriple lastTriple;
			while (getNextRDFStringTriple(partList, lastTriple, nextTripleList, filterList)) {
				subjectHash->insertMulti(lastTriple.string[0], CRDFHahsedSubjectData(lastTriple));
			}

			return subjectHash;
		}



		QList<CRDFStringTriple>* CSPARQLSimpleParser::getRDFStringTriples(QStringList& partList, QList<QStringList>& filterList) {
			QList<CRDFStringTriple>* tripleList = new QList<CRDFStringTriple>();
			QList<CRDFStringTriple> nextTripleList;

			CRDFStringTriple lastTriple;
			while (getNextRDFStringTriple(partList, lastTriple, nextTripleList, filterList)) {
				tripleList->append(lastTriple);
			}

			return tripleList;
		}




		
	}; // end namespace Parser

}; // end namespace Konclude
