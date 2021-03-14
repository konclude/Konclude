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

#include "COntologyMultiAutoParsingLoader.h"


namespace Konclude {

	namespace Parser {


		COntologyMultiAutoParsingLoader::COntologyMultiAutoParsingLoader(CConcreteOntology *ont, COntologyConfigurationExtension *ontConfig) : CLogIdentifier("::Konclude::Parser::MultiAutoParsingLoader", this) {
			mOntConfig = ontConfig;
			mOnt = ont;
		}


		COntologyMultiAutoParsingLoader::~COntologyMultiAutoParsingLoader() {
		}




		bool COntologyMultiAutoParsingLoader::parseOntologyFile(const QString& filename) {
			QFile file(filename);
			if (file.open(QIODevice::ReadOnly)) {
				bool parsed = parseOntologyFile(&file);
				file.close();
				return parsed;
			} else {
				return false;
			}
		}



		bool COntologyMultiAutoParsingLoader::parseOntologyFile(QIODevice* device) {
			if (mOntConfig && mOnt) {
				
				QList<QString> importOntologiesList;
				QStringList parserErrorList;
				CCommandRecordRouter commandRecordRouter(nullptr, this);
				bool parsed = parseOntology(device, QString(), QString(), mOnt, mOntConfig, importOntologiesList, parserErrorList, commandRecordRouter);
				return parsed;

			} else {
				return false;
			}
		}




		QStringList COntologyMultiAutoParsingLoader::getParserOrderFromFileName(const QString& fileName) {
			QStringList parserList;
			bool owl2FunctionalParserAdded = false;
			bool owl2XMLParserAdded = false;
			bool turtleParserAdded = false;
			bool owl2RDFXMLParserAdded = false;
			QString upperFileString = fileName.toUpper();
			if (!owl2RDFXMLParserAdded && (upperFileString.endsWith(".OWX") || upperFileString.endsWith(".OWL"))) {
				owl2RDFXMLParserAdded = true;
				owl2XMLParserAdded = true;
				parserList.append(QString("OWL2XML"));
				parserList.append(QString("OWLRDFXML"));
			}if (!owl2RDFXMLParserAdded && (upperFileString.endsWith(".RDF.XML") || upperFileString.endsWith(".OWL.RDF.XML"))) {
				owl2RDFXMLParserAdded = true;
				parserList.append(QString("OWLRDFXML"));
			}
			if (!turtleParserAdded && (upperFileString.endsWith(".NT"))) {
				turtleParserAdded = true;
				parserList.append(QString("RDFNTRIPLES"));
			}
			if (!turtleParserAdded && (upperFileString.endsWith(".TURTLE") || upperFileString.endsWith(".TTL") || upperFileString.endsWith(".NT"))) {
				turtleParserAdded = true;
				parserList.append(QString("RDFTURTLE"));
			}
			if (!owl2XMLParserAdded && (upperFileString.endsWith(".OWL.XML") || upperFileString.endsWith(".XML.OWL"))) {
				owl2XMLParserAdded = true;
				parserList.append(QString("OWL2XML"));
			}
			if (!owl2FunctionalParserAdded && (upperFileString.endsWith(".OWL.FSS") || upperFileString.endsWith(".FSS.OWL"))) {
				owl2FunctionalParserAdded = true;
				parserList.append(QString("OWL2Functional"));
			}
			if (!owl2XMLParserAdded && (upperFileString.endsWith(".XML"))) {
				owl2XMLParserAdded = true;
				parserList.append(QString("OWL2XML"));
			}
			if (!owl2FunctionalParserAdded && (upperFileString.endsWith(".FSS"))) {
				owl2FunctionalParserAdded = true;
				parserList.append(QString("OWL2Functional"));
			}
			if (!owl2XMLParserAdded && (upperFileString.contains(".XML"))) {
				owl2XMLParserAdded = true;
				parserList.append(QString("OWL2XML"));
			}
			if (!owl2FunctionalParserAdded && (upperFileString.contains(".FSS"))) {
				owl2FunctionalParserAdded = true;
				parserList.append(QString("OWL2Functional"));
			}
			if (!owl2XMLParserAdded && (upperFileString.contains("XML"))) {
				owl2XMLParserAdded = true;
				parserList.append(QString("OWL2XML"));
			}
			if (!owl2FunctionalParserAdded && (upperFileString.contains("FSS"))) {
				owl2FunctionalParserAdded = true;
				parserList.append(QString("OWL2Functional"));
			}
			if (!owl2FunctionalParserAdded) {
				owl2FunctionalParserAdded = true;
				parserList.append(QString("OWL2Functional"));
			}
			if (!owl2XMLParserAdded) {
				owl2XMLParserAdded = true;
				parserList.append(QString("OWL2XML"));
			}
			if (!owl2RDFXMLParserAdded) {
				owl2RDFXMLParserAdded = true;
				parserList.append(QString("OWLRDFXML"));
			}
			if (!turtleParserAdded) {
				turtleParserAdded = true;
				parserList.append(QString("RDFTURTLE"));
			}
			return parserList;
		}


#ifdef KONCLUDE_REDLAND_INTEGRATION

		bool COntologyMultiAutoParsingLoader::parseOntologyWithRaptor(QIODevice* device, CConcreteOntologyUpdateCollectorBuilder *builder, const QString& format, const QString& formatName, const QString& resolvedIRI, QString& parsingTryLogString, QStringList& parserErrorList, CConfiguration* configuration, CCommandRecordRouter& commandRecordRouter) {
			bool parsingSucceeded = false;
			CRDFRedlandRaptorParser *owl2Parser = nullptr;
			bool mConfParallelRaptorParsing = CConfigDataReader::readConfigBoolean(configuration, "Konclude.Parser.RedlandRaptor.ConcurrentParsing");
			if (mConfParallelRaptorParsing) {
				owl2Parser = new CRDFRedlandRaptorSimpleConcurrentParser(builder, CTRIPLES_DATA_UPDATE_TYPE::TRIPLES_DATA_ADDITION, format, configuration);
			} else {
				owl2Parser = new CRDFRedlandRaptorParser(builder, CTRIPLES_DATA_UPDATE_TYPE::TRIPLES_DATA_ADDITION, format, configuration);
			}
			parsingTryLogString = QString("Trying %1 parsing of '%2' with Redland Raptor.").arg(formatName).arg(resolvedIRI);
			LOG(INFO, getLogDomain(), parsingTryLogString, this);
			if (device->open(QIODevice::ReadOnly)) {
				device->reset();
				if (owl2Parser->parseTriples(device, resolvedIRI)) {
					parsingSucceeded = true;
					LOG(INFO, getLogDomain(), logTr("Finished %1 parsing of '%2' with Redland Raptor.").arg(formatName).arg(resolvedIRI), this);
				} else {
					parserErrorList.append(QString("%1 parsing of '%2' with Redland Raptor failed due to error '%3'.").arg(formatName).arg(resolvedIRI).arg(owl2Parser->getErrorString()));
				}
				device->close();
			} else {
				CUnspecifiedMessageErrorRecord::makeRecord(QString("Data from '%1' cannot be read.").arg(resolvedIRI), &commandRecordRouter);
			}
			delete owl2Parser;
			return parsingSucceeded;
		}

#endif // !KONCLUDE_REDLAND_INTEGRATION





		bool COntologyMultiAutoParsingLoader::parseOntology(QIODevice* device, const QString& ontoIRI, const QString& resolvedIRI, CConcreteOntology *ont, COntologyConfigurationExtension *ontConfig, QList<QString>& importOntologiesList, QStringList& parserErrorList, CCommandRecordRouter& commandRecordRouter) {
			CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ont);
			builder->initializeBuilding();

			bool parsingSucceeded = parseOntology(device, ontoIRI, resolvedIRI, ont, builder, ontConfig, importOntologiesList, parserErrorList, commandRecordRouter);

			if (parsingSucceeded) {
				builder->completeBuilding();
			}

			delete builder;
			return parsingSucceeded;
		}


		bool COntologyMultiAutoParsingLoader::parseOntology(QIODevice* device, const QString& ontoIRI, const QString& resolvedIRI, CConcreteOntology *ont, CConcreteOntologyUpdateCollectorBuilder* builder, COntologyConfigurationExtension *ontConfig, QList<QString>& importOntologiesList, QStringList& parserErrorList, CCommandRecordRouter& commandRecordRouter) {

			QString iriFileString = resolvedIRI;
			QStringList parserList = getParserOrderFromFileName(iriFileString);


			bool parsingSucceeded = false;
			bool triplesParsed = false;
			for (QStringList::const_iterator parserIt = parserList.constBegin(), parserItEnd = parserList.constEnd(); !parsingSucceeded && parserIt != parserItEnd; ++parserIt) {

				QString parserString(*parserIt);
				QString parsingTryLogString;
				builder->resetAxiomUpdates();

				if (parserString == "OWLRDFXML") {
#ifdef KONCLUDE_REDLAND_INTEGRATION
					parsingSucceeded = parseOntologyWithRaptor(device, builder, "rdfxml", "RDF/XML", resolvedIRI, parsingTryLogString, parserErrorList, ontConfig, commandRecordRouter);
					if (parsingSucceeded) {
						triplesParsed = true;
					}
#endif // !KONCLUDE_REDLAND_INTEGRATION

				} else if (parserString == "OWL2Functional") {
					COWL2FunctionalJAVACCOntologyStreamParser *owl2Parser = new COWL2FunctionalJAVACCOntologyStreamParser(builder);
					parsingTryLogString = QString("Trying stream-based OWL2/Functional ontology parsing for '%1'.").arg(iriFileString);
					LOG(INFO, getLogDomain(), parsingTryLogString, this);
					if (device->open(QIODevice::ReadOnly)) {
						device->reset();
						if (owl2Parser->parseOntology(device)) {
							parsingSucceeded = true;
							LOG(INFO, getLogDomain(), logTr("Finished stream-based OWL2/Functional ontology parsing for '%1'.").arg(iriFileString), this);
						} else {
							if (owl2Parser->hasError()) {
								parserErrorList.append(QString("Stream-based OWL2/Functional ontology parsing error: %1").arg(owl2Parser->getErrorString()));
							}
							parserErrorList.append(QString("Stream-based OWL2/Functional ontology parsing for '%1' failed.").arg(iriFileString));
						}
						device->close();
					} else {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("Data for '%1' cannot be read.").arg(resolvedIRI), &commandRecordRouter);
					}

					delete owl2Parser;

				} else if (parserString == "OWL2XML") {

					bool enforceUTF8ConvertingParsing = CConfigDataReader::readConfigBoolean(ontConfig, "Konclude.Parser.UTF8CompatibilityEnforcedXMLStreamParsing");
					COWL2QtXMLOntologyStreamParser *owl2Parser = nullptr;
					if (enforceUTF8ConvertingParsing) {
						owl2Parser = new COWL2QtXMLOntologyStableStreamParser(builder);
					} else {
						owl2Parser = new COWL2QtXMLOntologyStreamParser(builder);
					}
					parsingTryLogString = QString("Trying stream-based OWL2/XML ontology parsing for '%1'.").arg(iriFileString);
					LOG(INFO, getLogDomain(), parsingTryLogString, this);
					if (device->open(QIODevice::ReadOnly)) {
						device->reset();
						if (owl2Parser->parseOntology(device)) {
							parsingSucceeded = true;
							LOG(INFO, getLogDomain(), logTr("Finished stream-based OWL2/XML ontology parsing for '%1'.").arg(iriFileString), this);
						} else {
							if (owl2Parser->hasError()) {
								parserErrorList.append(QString("Stream-based OWL2/XML ontology parsing error: %1").arg(owl2Parser->getErrorString()));
							}
							parserErrorList.append(QString("Stream-based OWL2/XML ontology parsing for '%1' failed.").arg(iriFileString));
						}
						device->close();
					} else {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("Data for '%1' cannot be read.").arg(resolvedIRI), &commandRecordRouter);
					}

					delete owl2Parser;

				} else if (parserString == "RDFTURTLE") {

#ifdef KONCLUDE_REDLAND_INTEGRATION
					parsingSucceeded = parseOntologyWithRaptor(device, builder, "turtle", "RDF Turtle", resolvedIRI, parsingTryLogString, parserErrorList, ontConfig, commandRecordRouter);
					if (parsingSucceeded) {
						triplesParsed = true;
					}
#endif // !KONCLUDE_REDLAND_INTEGRATION

				} else if (parserString == "RDFNTRIPLES") {

#ifdef KONCLUDE_REDLAND_INTEGRATION
					parsingSucceeded = parseOntologyWithRaptor(device, builder, "ntriples", "RDF NTriples", resolvedIRI, parsingTryLogString, parserErrorList, ontConfig, commandRecordRouter);
					if (parsingSucceeded) {
						triplesParsed = true;
					}
#endif // !KONCLUDE_REDLAND_INTEGRATION

				}
			}

			if (triplesParsed) {
#ifdef KONCLUDE_REDLAND_INTEGRATION
				CConcreteOntologyRedlandTriplesDataExpressionMapper* triplesMapper = new CConcreteOntologyRedlandTriplesDataExpressionMapper(builder);
				triplesMapper->mapTriples(ont, ont->getOntologyTriplesData());
#endif // !KONCLUDE_REDLAND_INTEGRATION
			}

			importOntologiesList.append(builder->takeAddedImportOntologyList());

			return parsingSucceeded;
		}





	}; // end namespace Parser

}; // end namespace Konclude
