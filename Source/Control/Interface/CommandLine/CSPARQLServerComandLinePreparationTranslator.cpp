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

#include "CSPARQLServerComandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {


				CSPARQLServerComandLinePreparationTranslator::CSPARQLServerComandLinePreparationTranslator(CLoaderFactory *loaderFactory) : CCommandLinePreparationTranslator(loaderFactory) {
					mInterfaceTypeString = QString("CLI");
					mMinNecessaryArguments = true;
					mFirstArgumentTranslation = false;
				}


				CSPARQLServerComandLinePreparationTranslator::~CSPARQLServerComandLinePreparationTranslator() {
				}


				bool CSPARQLServerComandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {

					bool argumentTranslated = false;


					QString modArgText = argumentText.trimmed();
					if (!mSkipTranslation) {
						if (CCommandLinePreparationTranslator::translate(argumentText,remainingArgList)) {
							argumentTranslated = true;
						} else {
							if (modArgText.indexOf("-") == 0) {
								while (modArgText.startsWith("-")) {
									modArgText.remove(0,1);
									modArgText = modArgText.trimmed();
								}

								if (modArgText.indexOf("p") == 0 || modArgText.indexOf("P") == 0) {
									argumentTranslated = true;
									QString portVal = "";
									if (remainingArgList.count() > 0) {
										portVal = remainingArgList.takeFirst();
									}
									mParameterizingConfList.append(QString("+=Konclude.SPARQL.Server.Port=%2").arg(portVal.trimmed()));

								} 

							} else if (modArgText == "sparqlserver" || modArgText == "sserver") {
								argumentTranslated = true;
								mFirstArgumentTranslation = true;
							}
						}
					}
					return argumentTranslated;
				}

				bool CSPARQLServerComandLinePreparationTranslator::canTranslate(const QString& string) {
					if (QString::compare(string,"sparqlserver",Qt::CaseInsensitive) == 0 || QString::compare(string,"sserver",Qt::CaseInsensitive) == 0) {
						return true;
					}
					return false;
				}


				QStringList CSPARQLServerComandLinePreparationTranslator::combineTranslatedArguments() {
					QStringList translatedArgList;

					if (!mFirstArgumentTranslation) {
						mMinNecessaryArguments = false;
					}

					if (!mMinNecessaryArguments) {
						LOG(ERROR,getLogDomain(),logTr("Execution with inadequate command line arguments."),this);
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

						if (!mRequestConfList.isEmpty()) {
							translatedArgList += QString("-CLILoadingBatchProcessingLoader +=Konclude.CLI.CloseAfterProcessedRequest=false +=Konclude.CLI.BlockUntilProcessedRequest=true");
							translatedArgList += mRequestConfList;
						}

						translatedArgList += QString("-SPARQLHttpServerLoader");
						translatedArgList += mParameterizingConfList;

						translatedArgList += mMiscellaneousList;
					} else {
						translatedArgList += QString("-ConfigurableCoutLogObserverLoader -CommandLineTranlatorHelpLoader");
					}
					return translatedArgList;				
				}





			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
