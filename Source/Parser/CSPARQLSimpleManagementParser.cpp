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

#include "CSPARQLSimpleManagementParser.h"


namespace Konclude {

	namespace Parser {



		CSPARQLSimpleManagementParser::CSPARQLSimpleManagementParser(CCommandDelegater *commandDelegater, const QString& kbName, CCommand* command) {
			mKeywordSet.insert("LOAD");
			mKeywordSet.insert("CREATE");
			mKeywordSet.insert("DROP");
			mKeywordSet.insert("PREPARE");
			mKeywordSet.insert("CONNECT");

			mCommandDelegater = commandDelegater;
			mCommand = command;
			mKBName = kbName;
		}


		CSPARQLSimpleManagementParser::~CSPARQLSimpleManagementParser() {
		}



		bool CSPARQLSimpleManagementParser::parseUpdateFile(const QString& filename) {
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
				LOG(ERROR,"::Konclude::Parser::SPARQLSimpleManagementParser",logTr("File '%1' not found or couldn't be opened.").arg(filename),this);
			}

			return successfullParsed;
		}


		bool CSPARQLSimpleManagementParser::parseUpdateText(const QString& filetext) {
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


		bool CSPARQLSimpleManagementParser::parseUpdateTextList(const QStringList& queryTextList) {
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




		bool CSPARQLSimpleManagementParser::processParts(QStringList& parsedParts) {
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
				} else if (keyword == "LOAD") {

					bool silent = false;
					QString addressIRIString;
					QString graphIRIString;
					while (!parsedParts.isEmpty() && addressIRIString.isEmpty()) {
						QString partString = parsedParts.takeFirst();

						QString upperPartString = partString.toUpper();
						if (upperPartString != "SILENT") {
							if (addressIRIString.isEmpty()) {
								addressIRIString = getIRI(partString);
							} else {
								graphIRIString = getGraphName(partString, parsedParts, false);
							}
						} else {
							silent = true;
						}
					}
					if (graphIRIString.isEmpty()) {
						graphIRIString = mKBName;
					}
					if (!addressIRIString.isEmpty()) {
						CLoadKnowledgeBaseOWLAutoOntologyCommand* loadKBOntComm = new CLoadKnowledgeBaseOWLAutoOntologyCommand(graphIRIString, QStringList(addressIRIString));
						if (mCommand) {
							mCommand->makeToSubCommand(loadKBOntComm);
						}
						mCommandDelegater->delegateCommand(loadKBOntComm);

					}
				} else if (keyword == "CREATE" || keyword == "DROP") {

					bool silent = false;
					QString graphIRIString;
					if (!parsedParts.isEmpty()) {
						QString partString = parsedParts.takeFirst();

						QString upperPartString = partString.toUpper();
						if (upperPartString == "SILENT") {
							silent = true;
						} else {
							graphIRIString = getGraphName(partString, parsedParts, false);
						}
					}
					if (graphIRIString.isEmpty()) {
						graphIRIString = mKBName;
					}
					if (keyword == "CREATE") {
						CCreateKnowledgeBaseCommand* loadKBOntComm = new CCreateKnowledgeBaseCommand(graphIRIString);
						if (mCommand) {
							mCommand->makeToSubCommand(loadKBOntComm);
						}
						mCommandDelegater->delegateCommand(loadKBOntComm);
					} else if (keyword == "DROP") {
						CReleaseKnowledgeBaseCommand* loadKBOntComm = new CReleaseKnowledgeBaseCommand(graphIRIString);
						if (mCommand) {
							mCommand->makeToSubCommand(loadKBOntComm);
						}
						mCommandDelegater->delegateCommand(loadKBOntComm);
					}



				} else if (keyword == "PREPARE") {

					bool querying = false;
					QString graphIRIString;
					if (!parsedParts.isEmpty()) {
						QString partString = parsedParts.takeFirst();

						QString upperPartString = partString.toUpper();
						if (upperPartString == "QUERYING") {
							querying = true;
						} else {
							graphIRIString = getGraphName(partString, parsedParts, false);
						}
					}
					if (graphIRIString.isEmpty()) {
						graphIRIString = mKBName;
					}

					CGetCurrentKnowledgeBaseRevisionCommand* getCurrentKBOntComm = new CGetCurrentKnowledgeBaseRevisionCommand(graphIRIString);
					if (mCommand) {
						mCommand->makeToSubCommand(getCurrentKBOntComm);
					}

					CPrepareKnowledgeBaseCommand* prepKBC = new CPrepareKnowledgeBaseCommand(getCurrentKBOntComm);
					prepKBC->addCommandPrecondition(new CCommandProcessedPrecondition(getCurrentKBOntComm));
					if (mCommand) {
						mCommand->makeToSubCommand(prepKBC);
					}
					COntologyProcessingStepVector* ontProStepVec = COntologyProcessingStepVector::getProcessingStepVectorInstance();
					prepKBC->addRequirement(new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSINITREALIZE), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));
					prepKBC->addRequirement(new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSANSWERCOMPLEXQUERY), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0));


					mCommandDelegater->delegateCommand(getCurrentKBOntComm);
					mCommandDelegater->delegateCommand(prepKBC);

				} else if (keyword == "CONNECT") {

					bool silent = false;
					QString graphIRIString;
					QString triplesDBString;
					QString dsnConfigString;

					if (!parsedParts.isEmpty()) {
						triplesDBString = getGraphName(parsedParts.takeFirst(), parsedParts, true);
					}


					while (!parsedParts.isEmpty() && (dsnConfigString.isEmpty())) {
						QString partString = parsedParts.takeFirst();
						QString upperPartString = partString.toUpper();

						if (upperPartString == "WITH") {
							if (!partString.isEmpty()) {
								dsnConfigString = parsedParts.takeFirst();
							}
						} else {
							graphIRIString = getGraphName(partString, parsedParts, false);
						}

						if (!parsedParts.isEmpty()) {
							dsnConfigString = parsedParts.takeFirst();
						}
					}


					if (dsnConfigString.startsWith("\"")) {
						dsnConfigString = dsnConfigString.mid(1);
						if (dsnConfigString.endsWith("\"")) {
							dsnConfigString = dsnConfigString.mid(0, dsnConfigString.length() - 1);
						}
					}

					if (graphIRIString.isEmpty()) {
						graphIRIString = mKBName;
					}
					if (!dsnConfigString.isEmpty() && !triplesDBString.isEmpty()) {
						CConnectKnowledgeBaseExternalTriplesDSNCommand* connectExTrDSNComm = new CConnectKnowledgeBaseExternalTriplesDSNCommand(graphIRIString, triplesDBString, dsnConfigString);
						if (mCommand) {
							mCommand->makeToSubCommand(connectExTrDSNComm);
						}
						mCommandDelegater->delegateCommand(connectExTrDSNComm);

					}

				}
			}
			return true;
		}



		QString CSPARQLSimpleManagementParser::getGraphName(const QString& currentPartString, QStringList& parsedParts, bool directGraphReference) {
			QString graphIRIString;
			QString currentPartStringUpper = currentPartString.toUpper();
			if (directGraphReference || currentPartStringUpper == "FROM" || currentPartStringUpper == "TO" || currentPartStringUpper == "INTO" || currentPartStringUpper == "GRAPH") {
				if (directGraphReference || !parsedParts.isEmpty()) {
					QString graphString = currentPartString;
					QString graphStringUpper = graphString.toUpper();
					if (!directGraphReference) {
						graphString = parsedParts.takeFirst();
						graphStringUpper = graphString.toUpper();
					}
					if (graphStringUpper == "DEFAULT") {
					}
					else {
						if (graphStringUpper == "GRAPH") {
							graphString = parsedParts.takeFirst();
						}
						if (graphStringUpper == "NAMED") {
							if (!parsedParts.isEmpty()) {
								QString namedGraphString = parsedParts.takeFirst();
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



	}; // end namespace Parser

}; // end namespace Konclude
