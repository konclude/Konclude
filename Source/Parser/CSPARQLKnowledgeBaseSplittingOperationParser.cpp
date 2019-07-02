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

#include "CSPARQLKnowledgeBaseSplittingOperationParser.h"


namespace Konclude {

	namespace Parser {



		CSPARQLKnowledgeBaseSplittingOperationParser::CSPARQLKnowledgeBaseSplittingOperationParser() {
			mKeywordSet.insert("BASE");
			mKeywordSet.insert("@BASE");
			mKeywordSet.insert("PREFIX");
			mKeywordSet.insert("@PREFIX");
			mKeywordSet.insert("SELECT");
			mKeywordSet.insert("CONSTRUCT");
			mKeywordSet.insert("DESCRIBE");
			mKeywordSet.insert("ASK");
			mKeywordSet.insert("INSERT");
			mKeywordSet.insert("LOAD");
			mKeywordSet.insert("CREATE");
			mKeywordSet.insert("DROP");
			mKeywordSet.insert("PREPARE");

			mPrefixHash.insert("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");

		}


		CSPARQLKnowledgeBaseSplittingOperationParser::~CSPARQLKnowledgeBaseSplittingOperationParser() {
			qDeleteAll(mDataContainer);
		}



		bool CSPARQLKnowledgeBaseSplittingOperationParser::parseQueryFile(const QString& filename) {
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
				LOG(ERROR,"::Konclude::Parser::SPARQLSimpleQueryParser",logTr("File '%1' not found or couldn't be opened.").arg(filename),this);
			}

			return successfullParsed;
		}


		bool CSPARQLKnowledgeBaseSplittingOperationParser::parseQueryText(const QString& filetext) {
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



		bool CSPARQLKnowledgeBaseSplittingOperationParser::isKeyword(const QString& part) {
			return mKeywordSet.contains(part.toUpper());
		}



		bool CSPARQLKnowledgeBaseSplittingOperationParser::processParts(QStringList& parsedParts) {
			if (parsedParts.size() > 0) {
				QString keyword = parsedParts.takeFirst().toUpper();
				if (keyword == "BASE" || keyword == "@BASE") {
					mBaseIRI = "";
					mCommonPartStringList.append(keyword);
					if (parsedParts.size() >= 1) {
						QString prefixIriString = parsedParts.takeFirst();
						mBaseIRI = getIRI(prefixIriString);
						mCommonPartStringList.append(prefixIriString);
					}
				} else if (keyword == "PREFIX" || keyword == "@PREFIX") {
					mCommonPartStringList.append(keyword);
					if (parsedParts.size() >= 2) {
						QString prefix = parsedParts.takeFirst();
						QString resolvedPrefix = prefix;
						while (resolvedPrefix.endsWith(":")) {
							resolvedPrefix = resolvedPrefix.mid(0, resolvedPrefix.length() - 1);
						}
						QString prefixIriString = parsedParts.takeFirst();
						QString iri = getIRI(prefixIriString);
						mPrefixHash.insert(resolvedPrefix,iri);
						mCommonPartStringList.append(prefix);
						mCommonPartStringList.append(prefixIriString);
					}
				} else if (keyword == "SELECT" || keyword == "ASK") {
					QStringList queryStringList;
					queryStringList += mCommonPartStringList;
					queryStringList.append(keyword);
					QString fromIRIString;
					bool fromClauseFound = false;
					bool whereClauseStarted = false;
					while (!parsedParts.isEmpty() && !fromClauseFound && !whereClauseStarted) {
						QString partString = parsedParts.takeFirst();
						queryStringList.append(partString);


						partString = partString.toUpper();

						fromIRIString = getGraphName(partString, parsedParts, queryStringList, false);


						if (partString == "{") {
							whereClauseStarted = true;
						} 
					}


					QStringList whereClauseStringList = getBracketContent(parsedParts, '{', '}', whereClauseStarted, true);
					queryStringList += whereClauseStringList;
					queryStringList += parsedParts;
					if (!mKnowledgeBaseQueryStringListHash.contains(fromIRIString)) {
						mKnowledgeBaseList.append(fromIRIString);
					}
					addKnowledgeBaseOperation(new CSPARQLKnowledgeBaseSplittingOperationData(CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_QUERY, fromIRIString, queryStringList));

				} else if (keyword == "INSERT" || keyword == "DELETE") {
					QStringList updateStringList;
					updateStringList += mCommonPartStringList;
					updateStringList.append(keyword);
					QString graphIRIString;
					bool fromClauseFound = false;
					bool whereClauseStarted = false;
					bool whereClauseFinished = false;
					cint64 openBrackets = 0;
					while (!parsedParts.isEmpty() && !fromClauseFound && !whereClauseFinished) {
						QString partString = parsedParts.takeFirst();
						updateStringList.append(partString);


						partString = partString.toUpper();

						graphIRIString = getGraphName(partString, parsedParts, updateStringList, false);
						if (!graphIRIString.isEmpty()) {
							fromClauseFound = true;
						}

						if (partString == "{") {
							whereClauseStarted = true;
							++openBrackets;
						} else if (partString == "}") {
							openBrackets--;
							if (openBrackets <= 0) {
								whereClauseFinished = true;
							}
						}

					}


					if (!whereClauseFinished) {
						QStringList whereClauseStringList = getBracketContent(parsedParts, '{', '}', whereClauseStarted, true);
						updateStringList += whereClauseStringList;
					}
					if (!mKnowledgeBaseQueryStringListHash.contains(graphIRIString)) {
						mKnowledgeBaseList.append(graphIRIString);
					}
					addKnowledgeBaseOperation(new CSPARQLKnowledgeBaseSplittingOperationData(CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_UPDATE_MODIFY, graphIRIString, updateStringList));

				} else if (keyword == "LOAD") {
					QStringList manageStringList;
					manageStringList += mCommonPartStringList;
					manageStringList.append(keyword);
					QString graphIRIString;
					QString addressIRIString;
					while (!parsedParts.isEmpty() && graphIRIString.isEmpty()) {
						QString partString = parsedParts.takeFirst();
						manageStringList.append(partString);

						QString upperPartString = partString.toUpper();
						if (upperPartString != "SILENT") {
							if (addressIRIString.isEmpty()) {
								addressIRIString = getIRI(partString);
							} else {
								graphIRIString = getGraphName(partString, parsedParts, manageStringList, false);
							}
						}
					}


					if (!mKnowledgeBaseQueryStringListHash.contains(graphIRIString)) {
						mKnowledgeBaseList.append(graphIRIString);
					}
					addKnowledgeBaseOperation(new CSPARQLKnowledgeBaseSplittingOperationData(CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_UPDATE_MANAGE, graphIRIString, manageStringList));

				} else if (keyword == "CREATE" || keyword == "DROP") {
					QStringList manageStringList;
					manageStringList += mCommonPartStringList;
					manageStringList.append(keyword);
					QString graphIRIString;
					while (!parsedParts.isEmpty() && graphIRIString.isEmpty()) {
						QString partString = parsedParts.takeFirst();
						manageStringList.append(partString);

						QString upperPartString = partString.toUpper();
						if (upperPartString != "SILENT") {
							graphIRIString = getGraphName(partString, parsedParts, manageStringList, false);
						}
					}

					if (!mKnowledgeBaseQueryStringListHash.contains(graphIRIString)) {
						mKnowledgeBaseList.append(graphIRIString);
					}
					addKnowledgeBaseOperation(new CSPARQLKnowledgeBaseSplittingOperationData(CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_UPDATE_MANAGE, graphIRIString, manageStringList));

				} else if (keyword == "PREPARE") {
					QStringList manageStringList;
					manageStringList += mCommonPartStringList;
					manageStringList.append(keyword);
					QString graphIRIString;
					while (!parsedParts.isEmpty() && graphIRIString.isEmpty()) {
						QString partString = parsedParts.takeFirst();
						manageStringList.append(partString);

						QString upperPartString = partString.toUpper();
						if (upperPartString != "QUERYING") {
							graphIRIString = getGraphName(partString, parsedParts, manageStringList, false);
						}
					}

					if (!mKnowledgeBaseQueryStringListHash.contains(graphIRIString)) {
						mKnowledgeBaseList.append(graphIRIString);
					}
					addKnowledgeBaseOperation(new CSPARQLKnowledgeBaseSplittingOperationData(CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_UPDATE_MANAGE, graphIRIString, manageStringList));

				}
			}
			return true;
		}


		QString CSPARQLKnowledgeBaseSplittingOperationParser::getGraphName(const QString& currentPartString, QStringList& parsedParts, QStringList& updateStringList, bool directGraphReference) {
			QString graphIRIString;
			QString currentPartStringUpper = currentPartString.toUpper();
			if (directGraphReference || currentPartStringUpper == "FROM" || currentPartStringUpper == "TO" || currentPartStringUpper == "INTO" || currentPartStringUpper == "GRAPH") {
				if (directGraphReference || !parsedParts.isEmpty()) {
					QString graphString = currentPartString; 
					QString graphStringUpper = graphString.toUpper();
					if (!directGraphReference) {
						graphString = parsedParts.takeFirst();
						graphStringUpper = graphString.toUpper();
						updateStringList.append(graphString);
					}
					if (graphStringUpper == "DEFAULT") {
					} else {
						if (graphStringUpper == "GRAPH") {
							graphString = parsedParts.takeFirst();
							updateStringList.append(graphString);
						}
						if (graphStringUpper == "NAMED") {
							if (!parsedParts.isEmpty()) {
								QString namedGraphString = parsedParts.takeFirst();
								updateStringList.append(namedGraphString);
								graphIRIString = getIRI(namedGraphString);
							}
						} else {
							graphIRIString = getIRI(graphString);
						}
					}
				}
			}
			return graphIRIString;
		}


		CSPARQLKnowledgeBaseSplittingOperationParser* CSPARQLKnowledgeBaseSplittingOperationParser::addKnowledgeBaseOperation(CSPARQLKnowledgeBaseSplittingOperationData* kbOperationData) {
			mKnowledgeBaseQueryStringListHash.insertMulti(kbOperationData->getKnowledgeBaseString(), kbOperationData);
			mDataContainer.append(kbOperationData);
			return this;
		}


		QStringList CSPARQLKnowledgeBaseSplittingOperationParser::getKnowlegdeBaseList() {
			return mKnowledgeBaseList;
		}

		QList<CSPARQLKnowledgeBaseSplittingOperationData*> CSPARQLKnowledgeBaseSplittingOperationParser::getKnowlegdeBaseSPARQLOperationList(const QString& knowledgeBase) {
			return mKnowledgeBaseQueryStringListHash.values(knowledgeBase);
		}

		QList<CSPARQLKnowledgeBaseSplittingOperationData*> CSPARQLKnowledgeBaseSplittingOperationParser::getSPARQLOperationList() {
			return mDataContainer;
		}


	}; // end namespace Parser

}; // end namespace Konclude
