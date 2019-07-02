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

#include "CCommandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {

				CCommandLinePreparationTranslator::CCommandLinePreparationTranslator(CLoaderFactory *loaderFactory) : CLogIdentifier("::Konclude::Loader::CommandLineTranslator",this) {
					mSkipTranslation = false;
					mGenerateAutoResponseFile = false;
					mLoaderFac = loaderFactory;
					mHelpRequested = false;
					mExtendendLogging = false;
					mInterfaceTypeString = QString("CLI");
					mRequestFileTranslated = false;
					mResponseFileTranslated = false;
					mConfigFileTranslated = false;
					mIRITranslated = false;
				}


				CCommandLinePreparationTranslator::~CCommandLinePreparationTranslator() {
				}


				QStringList CCommandLinePreparationTranslator::translateArguments(int argc, char *argv[]) {
					QStringList argList;
					for (int i = 0; i < argc; ++i) {
						QString argumentText = QString(argv[i]);
						argList.append(argumentText);
					}
					return translateArguments(argList);
				}



				QStringList CCommandLinePreparationTranslator::combineTranslatedArguments() {
					QStringList translatedArgList;
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
							translatedArgList += QString("-%1BatchFileLoader +=Konclude.%1.CloseAfterProcessedRequest=false +=Konclude.%1.BlockUntilProcessedRequest=true").arg(mInterfaceTypeString);
							translatedArgList += mConfigConfList;
						}

						translatedArgList += QString("-%1 +=Konclude.%2.CloseAfterProcessedRequest=true").arg(mLoaderTypeString).arg(mInterfaceTypeString);
						if (mExtendendLogging) {
							translatedArgList += QString("+=Konclude.OWLlink.LoggingExtendedProcessingTimes=true");
						}
						translatedArgList += mRequestConfList;

						translatedArgList += mMiscellaneousList;
					} else {
						translatedArgList += QString("-ConfigurableCoutLogObserverLoader -CommandLineTranlatorHelpLoader");
					}
					return translatedArgList;
				}



				QStringList CCommandLinePreparationTranslator::translateArguments(const QStringList& argList) {
					QStringList processList(argList);
					if (processList.count() <= 0) {
						mHelpRequested = true;
					}
					while (processList.count() > 0) {
						QString arg = processList.takeFirst();
						translate(arg,processList);
					}


					if (mGenerateAutoResponseFile) {
						QString responseFile = mRequestFileName;
						if (responseFile.indexOf("request.xml") >= 0) {
							responseFile.replace("request.xml","response.xml");
						} else if (responseFile.indexOf(".xml") >= 0) {
							responseFile.replace(".xml","-response.xml");
						} else {
							responseFile.append("-response.xml");
						}
						mRequestConfList.append(QString("+Load.%1.ResponseFile=%2").arg(mInterfaceTypeString).arg(responseFile));
					}

					QStringList translatedArgList;
					if (!mSkipTranslation) {
						translatedArgList = combineTranslatedArguments();
					} else {
						translatedArgList = argList;
					}
					return translatedArgList;
				}


				bool CCommandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {

					bool argumentTranslated = false;


					QString modArgText = argumentText.trimmed();
					if (!mSkipTranslation) {

						if (modArgText.indexOf("-") == 0) {

							while (modArgText.startsWith("-")) {
								modArgText.remove(0,1);
								modArgText = modArgText.trimmed();
							}

							if (mLoaderFac->canCreateLoaderFromName(modArgText)) {
								argumentTranslated = true;
							} else if (modArgText.indexOf("t") == 0 || modArgText.indexOf("T") == 0) {
								mSkipTranslation = true;
								argumentTranslated = true;

							} else if (modArgText.indexOf("v") == 0 || modArgText.indexOf("V") == 0) {
								mExtendendLogging = true;
								argumentTranslated = true;

							} else if (modArgText.indexOf("u") == 0 || modArgText.indexOf("U") == 0) {
								mExtendendLogging = true;
								argumentTranslated = true;
								mLogConfList += QString("-CoutLogObserverLoader -LoggerConfigurationLoader");

							} else if (modArgText.indexOf("h") == 0 || modArgText.indexOf("H") == 0) {
								mHelpRequested = true;
								argumentTranslated = true;


							} else if (modArgText.indexOf("e") == 0 || modArgText.indexOf("E") == 0) {
								mReasonerConfList.append(QString("+Konclude.Execution.DefaultReasonerManager=ASExperimentalReasonerManager"));
								argumentTranslated = true;
							} else if (modArgText.indexOf("a") == 0 || modArgText.indexOf("A") == 0) {
								mReasonerConfList.append(QString("+Konclude.Execution.DefaultReasonerManager=ASAnalyseReasonerManager"));
								argumentTranslated = true;


							} else if (modArgText.indexOf("w") == 0 || modArgText.indexOf("W") == 0) {
								// worker count
								QString configVal = "AUTO";
								if (remainingArgList.count() > 0) {
									configVal = remainingArgList.first();
									if (configVal.trimmed().indexOf("-") == 0 || configVal.trimmed().indexOf("+") == 0) {
										 configVal = "AUTO";
									} else {
										remainingArgList.removeFirst();
									}
								}
								mReasonerConfList.append(QString("+Konclude.Calculation.ProcessorCount=%1").arg(configVal));
								argumentTranslated = true;

							} else if (modArgText.indexOf("q") == 0 || modArgText.indexOf("Q") == 0 || modArgText.indexOf("i") == 0 || modArgText.indexOf("I") == 0) {
								// query/input file
								QString configVal = "request.xml";
								if (remainingArgList.count() > 0) {
									configVal = remainingArgList.takeFirst();
								}
								mRequestFileName = configVal.trimmed();
								mRequestConfList.append(QString("+Konclude.%1.RequestFile=%2").arg(mInterfaceTypeString).arg(configVal.trimmed()));
								argumentTranslated = true;
								mRequestFileTranslated = true;

							} else if (modArgText.indexOf("c") == 0 || modArgText.indexOf("C") == 0) {
								// owllink query file
								QString configVal = "config.xml";
								if (remainingArgList.count() > 0) {
									configVal = remainingArgList.takeFirst();
								}
								mConfigConfList.append(QString("+Konclude.OWLlink.RequestFile=%1").arg(configVal.trimmed()));
								argumentTranslated = true;
								mConfigFileTranslated = true;

							} else if (modArgText.indexOf("r") == 0 || modArgText.indexOf("R") == 0 || modArgText.indexOf("o") == 0 || modArgText.indexOf("O") == 0) {
								// response/output file
								argumentTranslated = true;
								QString configVal = "response.xml";
								if (remainingArgList.count() > 0) {
									configVal = remainingArgList.first();
									if (configVal.trimmed().indexOf("-") == 0 || configVal.trimmed().indexOf("+") == 0) {
										mGenerateAutoResponseFile = true;
									} else {
										mGenerateAutoResponseFile = false;
									}
								} else {
									mGenerateAutoResponseFile = true;
								}
								if (!mGenerateAutoResponseFile) {
									mRequestConfList.append(QString("+Konclude.%1.ResponseFile=%2").arg(mInterfaceTypeString).arg(configVal.trimmed()));
								}
								mResponseFileTranslated = true;

							} else if (modArgText.indexOf("x") == 0 || modArgText.indexOf("X") == 0) {
								// port
								QString iriVal = "";
								if (remainingArgList.count() > 0) {
									iriVal = remainingArgList.takeFirst();
								}
								mRequestConfList.append(QString("+Konclude.%1.IRIName=%2").arg(mInterfaceTypeString).arg(iriVal.trimmed()));
								argumentTranslated = true;
								mIRITranslated = true;

							}

						} else if (modArgText.indexOf("+") == 0) {

							mGeneralConfList.append(modArgText);
							while (!remainingArgList.isEmpty() && !remainingArgList.first().startsWith("+") && !remainingArgList.first().startsWith("-")) {
								mGeneralConfList.append(remainingArgList.takeFirst());
							}
							argumentTranslated = true;

						}
					}

					return argumentTranslated;
				}



			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
