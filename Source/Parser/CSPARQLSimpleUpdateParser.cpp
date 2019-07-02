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

#include "CSPARQLSimpleUpdateParser.h"


namespace Konclude {

	namespace Parser {



		CSPARQLSimpleUpdateParser::CSPARQLSimpleUpdateParser(COntologyBuilder* ontoBuilder, CConcreteOntology* ontology) : CSPARQLSimpleBuildingParser(ontoBuilder,ontology) {
			mKeywordSet.insert("INSERT");
			mKeywordSet.insert("DELETE");
		}


		CSPARQLSimpleUpdateParser::~CSPARQLSimpleUpdateParser() {
		}



		bool CSPARQLSimpleUpdateParser::parseUpdateFile(const QString& filename) {
			bool successfullParsed = true;

			// parse file
			QFile file(filename);
			if (file.open(QIODevice::ReadOnly)) {

				QStringList partStringList;
				while (!file.atEnd()) {
					QString line(file.readLine());
					cint64 linePos = 0;
					while (linePos != line.length()) {
						QString partString = getNextPart(line, linePos);
						if (!partString.isEmpty()) {
							if (isKeyword(partString) && !partStringList.isEmpty()) {
								processParts(partStringList);
								partStringList.clear();
							}
							partStringList.append(partString);
						}
					}
				}
				if (!partStringList.isEmpty()) {
					processParts(partStringList);
				}

				file.close();

			} else {
				successfullParsed = false;
				LOG(ERROR,"::Konclude::Parser::SPARQLSimpleUpdateParser",logTr("File '%1' not found or couldn't be opened.").arg(filename),this);
			}

			return successfullParsed;
		}


		bool CSPARQLSimpleUpdateParser::parseUpdateText(const QString& filetext) {
			QStringList partStringList;
			QString line(filetext);
			cint64 linePos = 0;
			while (linePos != line.length()) {
				QString partString = getNextPart(line, linePos);
				if (!partString.isEmpty()) {
					if (isKeyword(partString) && !partStringList.isEmpty()) {
						processParts(partStringList);
						partStringList.clear();
					}
					partStringList.append(partString);
				}
			}
			if (!partStringList.isEmpty()) {
				processParts(partStringList);
			}
			return true;
		}


		bool CSPARQLSimpleUpdateParser::parseUpdateTextList(const QStringList& queryTextList) {
			QStringList partStringList;
			QStringList tmpQueryTextList = queryTextList;
			cint64 linePos = 0;
			while (!tmpQueryTextList.isEmpty()) {
				QString partString = tmpQueryTextList.takeFirst();
				if (!partString.isEmpty()) {
					if (isKeyword(partString) && !partStringList.isEmpty()) {
						processParts(partStringList);
						partStringList.clear();
					}
					partStringList.append(partString);
				}
			}
			if (!partStringList.isEmpty()) {
				processParts(partStringList);
			}
			return true;
		}




		bool CSPARQLSimpleUpdateParser::processParts(QStringList& parsedParts) {
			if (parsedParts.size() > 0) {
				QString keyword = parsedParts.takeFirst().toUpper();
				if (keyword == "BASE" || keyword == "@BASE") {
					mBaseIRI = "";
					if (parsedParts.size() >= 1) {
						mBaseIRI = getIRI(parsedParts.takeFirst());
					}
				} else if (keyword == "PREFIX" || keyword == "@PREFIX") {
					if (parsedParts.size() >= 2) {
						QString prefix = parsedParts.takeFirst();
						while (prefix.endsWith(":")) {
							prefix = prefix.mid(0, prefix.length() - 1);
						}
						QString iri = getIRI(parsedParts.takeFirst());
						mPrefixHash.insert(prefix, iri);
					}
				} else if (keyword == "INSERT" || keyword == "DELETE") {

					bool insertAxioms = keyword == "INSERT";
					bool deleteAxioms = keyword == "DELETE";

					QStringList dataClauseStringList = getBracketContent(parsedParts, '{', '}', false);

					for (QStringList::iterator it = dataClauseStringList.begin(), itEnd = dataClauseStringList.end(); it != itEnd;) {
						const QString& graphPartString(*it);
						if (graphPartString.toUpper() == "GRAPH") {
							it = dataClauseStringList.erase(it);
							const QString& namedPartString(*it);
							if (namedPartString.toUpper() == "NAMED") {
								it = dataClauseStringList.erase(it);
							}
							it = dataClauseStringList.erase(it);
						} else {
							break;
						}
					}

					QHash<QString, CRDFHahsedSubjectData>* subjectTripleHash = getRDFSubjectHash(dataClauseStringList);
                    QList<CAxiomExpression*> axiomList = getTripleOWLAxioms(subjectTripleHash, false);
					delete subjectTripleHash;
					for (CAxiomExpression* axiom : axiomList) {
						if (insertAxioms) {
							mOntoBuilder->tellOntologyAxiom(axiom);
						} else {
							mOntoBuilder->retractOntologyAxiom(axiom);
						}
					}

				}
			}
			return true;
		}

	


	}; // end namespace Parser

}; // end namespace Konclude
