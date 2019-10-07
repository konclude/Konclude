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

#include "CSPARQLSimpleQueryParser.h"


namespace Konclude {

	namespace Parser {



		CSPARQLSimpleQueryParser::CSPARQLSimpleQueryParser(CSPARQLQueryBuilder *queryBuilder, COntologyBuilder* ontoBuilder, CConcreteOntology* ontology) : CSPARQLSimpleBuildingParser(ontoBuilder, ontology) {
			mQueryBuilder = queryBuilder;
		}


		CSPARQLSimpleQueryParser::~CSPARQLSimpleQueryParser() {
		}



		bool CSPARQLSimpleQueryParser::parseQueryFile(const QString& filename) {
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


		bool CSPARQLSimpleQueryParser::parseQueryText(const QString& filetext) {
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


		bool CSPARQLSimpleQueryParser::parseQueryTextList(const QStringList& queryTextList) {
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




		bool CSPARQLSimpleQueryParser::processParts(QStringList& parsedParts) {
			if (parsedParts.size() > 0) {
				QString keyword = parsedParts.takeFirst().toUpper();
				if (keyword == "BASE") {
					mBaseIRI = "";
					if (parsedParts.size() >= 1) {
						mBaseIRI = getIRI(parsedParts.takeFirst());
					}
				} else if (keyword == "PREFIX") {
					if (parsedParts.size() >= 2) {
						QString prefix = parsedParts.takeFirst();
						while (prefix.endsWith(":")) {
							prefix = prefix.mid(0, prefix.length() - 1);
						}
						QString iri = getIRI(parsedParts.takeFirst());
						mPrefixHash.insert(prefix, iri);
					}
				} else if (keyword == "SELECT") {
					QStringList disVarStringList;
					bool distinctModifier = false;
					bool selectVarsFinished = false;
					bool selectAllVariables = false;
					bool whereClausesStarting = false;
					while (!parsedParts.isEmpty() && !selectVarsFinished) {
						QString varString = parsedParts.takeFirst();

						QString modifierString = varString.toUpper();
						if (modifierString == "DISTINCT") {
							distinctModifier = true;
						}

						int nextVarStartPos = qMax(varString.indexOf(",?"), varString.indexOf(",$"));
						if (nextVarStartPos != -1) {
							if (nextVarStartPos == 0) {
								QString otherPartString = varString.mid(1);
								parsedParts.prepend(otherPartString);
							} else {
								varString = varString.mid(0, nextVarStartPos);
								QString otherPartString = varString.mid(nextVarStartPos + 1);
								parsedParts.prepend(otherPartString);
							}
						}										

						if (varString.startsWith("*")) {
							selectAllVariables = true;
						}

						if (varString.startsWith("?") || varString.startsWith("$")) {
							while (varString.endsWith(",")) {
								varString = varString.mid(0, varString.length() - 1);
							}
							if (!varString.isEmpty()) {
								varString = varString.mid(1);
								disVarStringList.append(varString);
							}

						} else if (varString.toUpper() == "WHERE" || varString.toUpper() == "FROM") {
							selectVarsFinished = true;
							
						} else if (varString == "{") {
							selectVarsFinished = true;
							whereClausesStarting = true;
						}
					}






					CEXPRESSIONLIST<COrderingTermExpression*> orderingList;
					QList<CFilteringTermExpression*> filteringList;

					QStringList whereClauseStringList = getBracketContent(parsedParts, '{', '}', whereClausesStarting);
					QList<QStringList> filterList;
					QHash<QString, CRDFHahsedSubjectData>* subjectTripleHash = getRDFSubjectHash(whereClauseStringList, filterList);
                    QList<CAxiomExpression*> axiomList = getTripleOWLAxioms(subjectTripleHash, true);
					delete subjectTripleHash;
					QList<CExpressionVariable*> varExpList;
					QList<CExpressionVariable*> disVarList;
					for (QString varString : disVarStringList) {
						CExpressionVariable* expVar = mNameVarHash.value(varString);
						if (expVar) {
							disVarList.append(expVar);
						} else {
							LOG(WARN, "::Konclude::Parser::SPARQLSimpleQueryParser", logTr("Variable '%1' in SELECT does not occur in WHERE clause, variable is ignored.").arg(varString), this);
						}
					}
					for (CExpressionVariable* varExp : mVarExpSet) {
						varExpList.append(varExp);
						if (selectAllVariables) {
							disVarList.append(varExp);
						}
					}

					filteringList = getFilters(filterList);



					cint64 limit = -1;
					cint64 offset = 0;
					bool orderingVarsFinished = false;
					while (!parsedParts.isEmpty() && !orderingVarsFinished) {
						QString keyString = parsedParts.first();
						QString keyUpperString = keyString.toUpper();
						if (keyUpperString == "LIMIT") {
							parsedParts.removeFirst();
							if (!parsedParts.isEmpty()) {
								limit = parsedParts.takeFirst().toLongLong();
							}
						} else if (keyUpperString == "OFFSET") {
							parsedParts.removeFirst();
							if (!parsedParts.isEmpty()) {
								offset = parsedParts.takeFirst().toLongLong();
							}
						} else if (keyUpperString == "ORDER" || keyUpperString == "BY") {
							parsedParts.removeFirst();
						} else {
							parsedParts.removeFirst();
							QString varString;

							bool variableBracketed = false;
							bool ascOrdered = true;
							if (keyUpperString == "ASC") {
								ascOrdered = true;
								variableBracketed = true;
							} else if (keyUpperString == "DESC") {
								ascOrdered = false;
								variableBracketed = true;
							} else {
								varString = keyString;
							}

							if (variableBracketed) {
								QStringList variableClauseStringList = getBracketContent(parsedParts, '(', ')');
								if (!variableClauseStringList.isEmpty()) {
									varString = variableClauseStringList.first();
								}
							}

							if (varString.startsWith("?") || varString.startsWith("$")) {
								varString = varString.mid(1);
								CExpressionVariable* expVar = mNameVarHash.value(varString);
								if (expVar) {
									COrderingTermExpression* orderingVarExp = mQueryBuilder->getOrderingVariableExpression(expVar, ascOrdered);
									orderingList.append(orderingVarExp);
								}
							}

						}
					}

					CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternSelectQuery(axiomList, disVarList, orderingList, filteringList, distinctModifier, limit, offset);
					mSPARQLBGPQueryList.append(sparqlQuery);


				} else if (keyword == "ASK") {

					QStringList askClauseStringList = getBracketContent(parsedParts, '{', '}', false);
					QList<QStringList> filterList;
					QHash<QString, CRDFHahsedSubjectData>* subjectTripleHash = getRDFSubjectHash(askClauseStringList, filterList);
					QList<CAxiomExpression*> axiomList = getTripleOWLAxioms(subjectTripleHash, true);
					QList<CFilteringTermExpression*> filteringList;
					filteringList = getFilters(filterList);

					CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternAskQuery(axiomList, filteringList);
					mSPARQLBGPQueryList.append(sparqlQuery);
				}
			}
			return true;
		}

	


		QList<CFilteringTermExpression*> CSPARQLSimpleQueryParser::getFilters(QList<QStringList>& filterList) {
			QList<CFilteringTermExpression*> filterExpList;
			while (!filterList.isEmpty()) {
				QStringList filterParts = filterList.takeFirst();
				CBuildExpression* filterExp = getFilterORExpression(filterParts);
				if (filterExp && dynamic_cast<CFilteringTermExpression*>(filterExp)) {
					filterExpList.append((CFilteringTermExpression*)filterExp);
				}
			}
			return filterExpList;
		}


		CBuildExpression* CSPARQLSimpleQueryParser::getExpression(QStringList& filterParts) {
			QString filterString = filterParts.takeFirst();
			if (filterString == "(") {
				QStringList innerFilterParts = getBracketContent(filterParts, '(', ')', true);
				return getFilterORExpression(innerFilterParts);
			} else if (filterString.startsWith("?") || filterString.startsWith("$")) {
				QString varString = filterString.mid(1);
				CExpressionVariable* var = mNameVarHash.value(varString, nullptr);
				if (var) {
					if (dynamic_cast<CDataLiteralVariableExpression*>(var)) {
						return (CDataLiteralVariableExpression*)var;
					}
					else if (dynamic_cast<CIndividualVariableExpression*>(var)) {
						return (CIndividualVariableExpression*)var;
					}
				}
			} else if (filterString.startsWith("\"") || filterString.startsWith("'")) {
				QPair<QString,QString> datatypeLiteralStringPair = extractDataLiteral(filterString);
				QString datatypeString = datatypeLiteralStringPair.second;
				QString literalString = datatypeLiteralStringPair.first;
				CDatatypeExpression* datatypeExp = mOntoBuilder->getDatatype(datatypeString);
				CDataLexicalValueExpression* lexiValExp = mOntoBuilder->getDataLexicalValue(literalString);
				CDataLiteralExpression* dataLiteralExp = mOntoBuilder->getDataLiteral(lexiValExp, datatypeExp);
				return dataLiteralExp;
			}
			return nullptr;
		}


		CBuildExpression* CSPARQLSimpleQueryParser::getFilterORExpression(QStringList& filterParts) {
			QList<CBuildExpression*> expressions;
			while (!filterParts.isEmpty()) {
				QString filterString = filterParts.first();
				if (filterString == "(" || filterString.startsWith("?") || filterString.startsWith("$") || filterString.startsWith("\"") || filterString.startsWith("'")) {
					CBuildExpression* exp = getExpression(filterParts);
					if (exp) {
						expressions.append(exp);
					}
				} else if (filterString == "<" || filterString == ">" || filterString == "<=" || filterString == ">=" || filterString == "==" || filterString == "!=") {
					filterParts.removeFirst();
					CBuildExpression* secExp = getExpression(filterParts);
					CBuildExpression* firstExp = nullptr;
					if (expressions.count() > 0) {
						firstExp = expressions.takeLast();
					}
					if (firstExp && secExp) {
						if (dynamic_cast<CDataLiteralVariableExpression*>(firstExp) && dynamic_cast<CDataLiteralExpression*>(secExp)) {
							expressions.append(getDataLiteralComparisonFilter((CDataLiteralVariableExpression*)firstExp, (CDataLiteralExpression*)secExp, filterString, false));
						} else if (dynamic_cast<CDataLiteralVariableExpression*>(secExp) && dynamic_cast<CDataLiteralExpression*>(firstExp)) {
							expressions.append(getDataLiteralComparisonFilter((CDataLiteralVariableExpression*)secExp, (CDataLiteralExpression*)firstExp, filterString, true));
						}
					}
				}
			}
			if (expressions.count() > 0) {
				return expressions.first();
			}
			return nullptr;
		}


		CFilteringComparisonDataLiteralExpression* CSPARQLSimpleQueryParser::getDataLiteralComparisonFilter(CDataLiteralVariableExpression* dataLitVar, CDataLiteralExpression* dataLiteral, const QString& compareOperatorString, bool reverse) {
			CFilteringComparisonDataLiteralExpression::COMPARING_TYPE type = CFilteringComparisonDataLiteralExpression::EQUAL;
			if (compareOperatorString == "==") {
				type = CFilteringComparisonDataLiteralExpression::EQUAL;
			} else if (compareOperatorString == "!=") {
				type = CFilteringComparisonDataLiteralExpression::UNEQUAL;
			} else if (compareOperatorString == "<=") {
				if (reverse) {
					type = CFilteringComparisonDataLiteralExpression::GREATER_EQUAL;
				} else {
					type = CFilteringComparisonDataLiteralExpression::LESS_EQUAL;
				}
			} else if (compareOperatorString == ">=") {
				if (reverse) {
					type = CFilteringComparisonDataLiteralExpression::LESS_EQUAL;
				} else {
					type = CFilteringComparisonDataLiteralExpression::GREATER_EQUAL;
				}
			} else if (compareOperatorString == "<") {
				if (reverse) {
					type = CFilteringComparisonDataLiteralExpression::GREATER;
				} else {
					type = CFilteringComparisonDataLiteralExpression::LESS;
				}
			} else if (compareOperatorString == ">") {
				if (reverse) {
					type = CFilteringComparisonDataLiteralExpression::LESS;
				} else {
					type = CFilteringComparisonDataLiteralExpression::GREATER;
				}
			}
			CFilteringComparisonDataLiteralExpression* compDataLiteralFilterExp = new CFilteringComparisonDataLiteralExpression(dataLiteral, dataLitVar, type);
			return compDataLiteralFilterExp;
		}


	}; // end namespace Parser

}; // end namespace Konclude
