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

#include "COWL2RDFTurtleAssertionsSimpleParser.h"


namespace Konclude {

	namespace Parser {



		COWL2RDFTurtleAssertionsSimpleParser::COWL2RDFTurtleAssertionsSimpleParser(COntologyBuilder* ontoBuilder, CConcreteOntology* ontology) {
			mOntoBuilder = ontoBuilder;
			mOntology = ontology;

			mKeywordSet.insert("BASE");
			mKeywordSet.insert("@BASE");
			mKeywordSet.insert("PREFIX");
			mKeywordSet.insert("@PREFIX");

		}


		COWL2RDFTurtleAssertionsSimpleParser::~COWL2RDFTurtleAssertionsSimpleParser() {
		}


		bool COWL2RDFTurtleAssertionsSimpleParser::isKeyword(const QString& part) {
			return mKeywordSet.contains(part.toUpper());
		}




		bool COWL2RDFTurtleAssertionsSimpleParser::parseOntology(const QString& fileString) {
			bool successfullParsed = true;

			// parse file
			QFile file(fileString);
			if (file.open(QIODevice::ReadOnly)) {
				successfullParsed = parseOntology(&file);
			} else {
				successfullParsed = false;
				LOG(ERROR, "::Konclude::Parser::OWL2RDFTurtleAssertionsSimpleParser", logTr("File '%1' not found or could not be opened.").arg(fileString), this);
			}

			return successfullParsed;
		}

		bool COWL2RDFTurtleAssertionsSimpleParser::parseOntology(QIODevice* ioDevice) {

			QStringList partStringList;
			while (!ioDevice->atEnd()) {
				QString line(ioDevice->readLine());
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

			ioDevice->close();
			return !mParsingError;
		}

		QString COWL2RDFTurtleAssertionsSimpleParser::getErrorString() {
			return mLastParsingError;
		}

		bool COWL2RDFTurtleAssertionsSimpleParser::hasError() {
			return mParsingError;
		}







		bool COWL2RDFTurtleAssertionsSimpleParser::parseUpdateText(const QString& filetext) {
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


		bool COWL2RDFTurtleAssertionsSimpleParser::parseUpdateTextList(const QStringList& queryTextList) {
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




		bool COWL2RDFTurtleAssertionsSimpleParser::processParts(QStringList& parsedParts) {
			while (parsedParts.size() > 0) {
				QString keyword = parsedParts.first().toUpper();
				if (keyword == "BASE" || keyword == "@BASE") {
					parsedParts.removeFirst();
					mBaseIRI = "";
					if (parsedParts.size() >= 1) {
						mBaseIRI = getIRI(parsedParts.takeFirst());
					}
					if (parsedParts.size() >= 1) {
						if (parsedParts.first() == ".") {
							parsedParts.takeFirst();
						}
					}
				} else if (keyword == "PREFIX" || keyword == "@PREFIX") {
					parsedParts.removeFirst();
					if (parsedParts.size() >= 2) {
						QString prefix = parsedParts.takeFirst();
						while (prefix.endsWith(":")) {
							prefix = prefix.mid(0, prefix.length() - 1);
						}
						QString iri = getIRI(parsedParts.takeFirst());
						mPrefixHash.insert(prefix, iri);
					}
					if (parsedParts.size() >= 1) {
						if (parsedParts.first() == ".") {
							parsedParts.takeFirst();
						}
					}
				} else {

					QList<CRDFStringTriple>* subjectTripleHash = getRDFStringTriples(parsedParts);
					if (!mParsingError) {
						QList<CAxiomExpression*> axiomList = getTripleOWLAxioms(subjectTripleHash);
						delete subjectTripleHash;
						for (CAxiomExpression* axiom : axiomList) {
							mOntoBuilder->tellOntologyAxiom(axiom);
						}
					}
					parsedParts.clear();

				}
			}
			return true;
		}






		QString COWL2RDFTurtleAssertionsSimpleParser::getVariableName(const QString& variableName) {
			return variableName.mid(1);
		}


		QList<CAxiomExpression*> COWL2RDFTurtleAssertionsSimpleParser::getTripleOWLAxioms(QList<CRDFStringTriple>* tripleStringList) {
			QList<CAxiomExpression*> axiomList;
			for (auto triple : *tripleStringList) {

				QString subjectString = triple.string[0];
				CIndividualTermExpression* indiExpression = nullptr;
				if (subjectString.startsWith("_:")) {
					indiExpression = mOntoBuilder->getAnonymousIndividual(mOntoBuilder->getOntologyAnonymousIdentifier(mOntoBuilder->getOntologyName()), subjectString.mid(2));
				} else {
					indiExpression = mOntoBuilder->getNamedIndividual(subjectString);
				}

				QString predicateString = triple.string[1];
				QString objectString = triple.string[2];

				if (predicateString == "a" || predicateString == "http://www.w3.org/1999/02/22-rdf-syntax-ns#type") {
					CClassExpression* classExp = mOntoBuilder->getClass(objectString);
					CAxiomExpression* assExp = mOntoBuilder->getClassAssertion(indiExpression, classExp);
					axiomList.append(assExp);
				} else {
					if (!isDataProperty(predicateString, objectString)) {
						CIndividualTermExpression* otherIndiExpression = nullptr;
						if (objectString.startsWith("_:")) {
							otherIndiExpression = mOntoBuilder->getAnonymousIndividual(mOntoBuilder->getOntologyAnonymousIdentifier(mOntoBuilder->getOntologyName()), objectString.mid(2));
						} else {
							otherIndiExpression = mOntoBuilder->getNamedIndividual(objectString);
						}
						CObjectPropertyExpression* propExp = mOntoBuilder->getObjectProberty(predicateString);
						CObjectPropertyAssertionExpression* propAss = mOntoBuilder->getObjectPropertyAssertion(indiExpression, propExp, otherIndiExpression);
						axiomList.append(propAss);
					} else {
						CDataLiteralTermExpression* dataRangeExpr = nullptr;
						QPair<QString, QString> literalDatatypeStringPair = extractDataLiteral(objectString);
						CDataLexicalValueExpression* literalValueExp = mOntoBuilder->getDataLexicalValue(literalDatatypeStringPair.first);
						CDatatypeExpression* datatypeExp = mOntoBuilder->getDatatype(literalDatatypeStringPair.second);
						dataRangeExpr = mOntoBuilder->getDataLiteral(literalValueExp,datatypeExp);
						CDataPropertyExpression* propExp = mOntoBuilder->getDataProberty(predicateString);
						CDataPropertyAssertionExpression* propAss = mOntoBuilder->getDataPropertyAssertion(indiExpression, propExp, dataRangeExpr);
						axiomList.append(propAss);
					}
				}
			}
			return axiomList;
		}


		bool COWL2RDFTurtleAssertionsSimpleParser::isDataProperty(const QString& propertyName, const QString& objectString) {
			if (objectString.startsWith("\"")) {
				return true;
			}	
			return mOntology->getBuildData()->getDataPropertyEntityBuildHash()->contains(propertyName);
		}




	}; // end namespace Parser

}; // end namespace Konclude
