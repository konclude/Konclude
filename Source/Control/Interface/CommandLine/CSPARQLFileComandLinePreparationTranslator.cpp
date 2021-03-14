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

#include "CSPARQLFileComandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {

				CSPARQLFileComandLinePreparationTranslator::CSPARQLFileComandLinePreparationTranslator(CLoaderFactory *loaderFactory) : CCommandLinePreparationTranslator(loaderFactory) {
					mInterfaceTypeString = QString("CLI");
					mMinNecessaryArguments = true;
					mFirstArgumentTranslation = false;
					mSPARQLQueryFileTranslated = false;
					mReasonerConfList.append(QString("+=Konclude.Calculation.Querying.ComplexQueryingSupport=true"));
					mReasonerConfList.append(QString("+=Konclude.Calculation.Optimization.RepresentativePropagation=false"));
					mReasonerConfList.append(QString("+=Konclude.Calculation.Optimization.SignatureMirroringBlocking=false"));
				}


				CSPARQLFileComandLinePreparationTranslator::~CSPARQLFileComandLinePreparationTranslator() {
				}

				QStringList CSPARQLFileComandLinePreparationTranslator::combineTranslatedArguments() {
					QStringList translatedArgList;

					if (!mFirstArgumentTranslation) {
						mMinNecessaryArguments = false;
					}

					if (!mMinNecessaryArguments) {
						LOG(ERROR,getLogDomain(),logTr("Execution with inadequate command line arguments."),this);
						mHelpRequested = true;
					}

					if (!mSPARQLQueryFileTranslated) {
						LOG(ERROR, getLogDomain(), logTr("SPARQL query file not specified."), this);
						mHelpRequested = true;
					}

					if (!mHelpRequested) {
						if (mLogConfList.isEmpty()) {
							mLogConfList.append(QString("-ConfigurableCoutLogObserverLoader -LoggerConfigurationLoader"));
						}
						translatedArgList += mLogConfList;
						translatedArgList += mGeneralConfList;

						translatedArgList += QString("-DefaultReasonerLoader ");
						translatedArgList += mReasonerConfList;
						translatedArgList += mGeneralConfList;

						if (!mConfigConfList.isEmpty()) {
							translatedArgList += QString("-OWLlinkBatchFileLoader +=Konclude.OWLlink.CloseAfterProcessedRequest=false +=Konclude.OWLlink.BlockUntilProcessedRequest=true");
							translatedArgList += mConfigConfList;
						}

						if (mRequestFileTranslated) {
							translatedArgList += QString("-CLILoadingBatchProcessingLoader +=Konclude.CLI.CloseAfterProcessedRequest=false +=Konclude.CLI.BlockUntilProcessedRequest=true");
							translatedArgList += mRequestConfList;
						}

						if (mExtendendLogging) {
							translatedArgList += QString("+=Konclude.SPARQL.LoggingExtendedProcessingTimes=true");
						}


						translatedArgList += QString("-SPARQLBatchFileLoader +=Konclude.SPARQL.CloseAfterProcessedRequest=true +=Konclude.SPARQL.BlockUntilProcessedRequest=true +=Konclude.SPARQL.RequestFile=%1 +=Konclude.SPARQL.ResponseFile=%2").arg(mSPARQLQueryFileName).arg(mSPARQLResponseFileName);


						translatedArgList += mMiscellaneousList;
					} else {
						translatedArgList += QString("-ConfigurableCoutLogObserverLoader -CommandLineTranlatorHelpLoader");
					}
					return translatedArgList;
				}

				bool CSPARQLFileComandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {

					bool argumentTranslated = false;


					if (!mSkipTranslation) {

						QString modArgText = argumentText.trimmed();
						QString parmArgText;
						
						if (modArgText.startsWith("-")) {
							parmArgText = modArgText;
							while (parmArgText.startsWith("-")) {
								parmArgText.remove(0, 1);
								parmArgText = parmArgText.trimmed();
							}
						}

						if (parmArgText.size() == 1 && (parmArgText.indexOf("r") == 0 || parmArgText.indexOf("R") == 0 || parmArgText.indexOf("o") == 0 || parmArgText.indexOf("O") == 0)) {
							// response/output file
							argumentTranslated = true;
							mSPARQLResponseFileName = "response.xml";
							if (remainingArgList.count() > 0) {
								mSPARQLResponseFileName = remainingArgList.takeFirst();
								if (mSPARQLResponseFileName.trimmed().indexOf("-") == 0 || mSPARQLResponseFileName.trimmed().indexOf("+") == 0) {
									mSPARQLResponseFileName = "response.xml";
								}
							}
							if (!mGenerateAutoResponseFile) {
								mRequestConfList.append(QString("+Konclude.SPARQL.ResponseFile=%1").arg(mSPARQLResponseFileName));
							}
							mResponseFileTranslated = true;


						} else if (CCommandLinePreparationTranslator::translate(argumentText, remainingArgList)) {
							argumentTranslated = true;

						} else if (parmArgText.size() == 1 && (parmArgText.indexOf("s") == 0 || parmArgText.indexOf("S") == 0)) {
							// sparql query/input file
							QString configVal = "query.sparql";
							if (remainingArgList.count() > 0) {
								configVal = remainingArgList.takeFirst();
							}
							mSPARQLQueryFileName = configVal.trimmed();
							argumentTranslated = true;
							mSPARQLQueryFileTranslated = true;


						} else if (modArgText == "sparqlfile" || modArgText == "sparql") {
							argumentTranslated = true;
							mFirstArgumentTranslation = true;
						}
					}
					return argumentTranslated;
				}



				bool CSPARQLFileComandLinePreparationTranslator::canTranslate(const QString& string) {
					if (QString::compare(string,"sparqlfile",Qt::CaseInsensitive) == 0 || QString::compare(string,"sparql",Qt::CaseInsensitive) == 0) {
						return true;
					}
					return false;
				}


			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
 