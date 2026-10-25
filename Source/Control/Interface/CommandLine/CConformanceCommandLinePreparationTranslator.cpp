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

#include "CConformanceCommandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {

				CConformanceCommandLinePreparationTranslator::CConformanceCommandLinePreparationTranslator(CLoaderFactory *loaderFactory)
						: CCommandLinePreparationTranslator(loaderFactory) {
					mInterfaceTypeString = QString("Conformance");
					mFirstArgumentTranslation = false;
				}


				CConformanceCommandLinePreparationTranslator::~CConformanceCommandLinePreparationTranslator() {
				}


				bool CConformanceCommandLinePreparationTranslator::canTranslate(const QString& string) {
					return QString::compare(string, "conformance", Qt::CaseInsensitive) == 0
						|| QString::compare(string, "conf", Qt::CaseInsensitive) == 0;
				}


				bool CConformanceCommandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {
					bool argumentTranslated = false;
					QString modArgText = argumentText.trimmed();
					if (mSkipTranslation) {
						return false;
					}

					if (modArgText.compare("conformance", Qt::CaseInsensitive) == 0 || modArgText.compare("conf", Qt::CaseInsensitive) == 0) {
						mFirstArgumentTranslation = true;
						return true;
					}

					if (modArgText.startsWith("-")) {
						QString opt = modArgText;
						while (opt.startsWith("-")) {
							opt.remove(0, 1);
						}
						opt = opt.trimmed();

						auto takeValue = [&remainingArgList](const QString& def) -> QString {
							if (!remainingArgList.isEmpty() && !remainingArgList.first().startsWith("-") && !remainingArgList.first().startsWith("+")) {
								return remainingArgList.takeFirst().trimmed();
							}
							return def;
						};

						if (opt.compare("i", Qt::CaseInsensitive) == 0 || opt.compare("input", Qt::CaseInsensitive) == 0) {
							mConformanceConfList.append(QString("+Konclude.Conformance.InputDirectory=%1").arg(takeValue("Tests/cases")));
							argumentTranslated = true;
						} else if (opt.compare("o", Qt::CaseInsensitive) == 0 || opt.compare("output", Qt::CaseInsensitive) == 0) {
							mConformanceConfList.append(QString("+Konclude.Conformance.OutputFile=%1").arg(takeValue("")));
							argumentTranslated = true;
						} else if (opt.compare("j", Qt::CaseInsensitive) == 0 || opt.compare("jobs", Qt::CaseInsensitive) == 0) {
							mConformanceConfList.append(QString("+Konclude.Conformance.Jobs=%1").arg(takeValue("1")));
							argumentTranslated = true;
						} else if (opt.compare("repeat", Qt::CaseInsensitive) == 0) {
							mConformanceConfList.append(QString("+Konclude.Conformance.Repeat=%1").arg(takeValue("1")));
							argumentTranslated = true;
						} else if (opt.compare("timeout", Qt::CaseInsensitive) == 0) {
							mConformanceConfList.append(QString("+Konclude.Conformance.TimeoutSeconds=%1").arg(takeValue("60")));
							argumentTranslated = true;
						} else if (opt.compare("progress", Qt::CaseInsensitive) == 0) {
							// Progress / estimated-remaining-time report interval in seconds (0 disables it).
							mConformanceConfList.append(QString("+Konclude.Conformance.ProgressInterval=%1").arg(takeValue("3")));
							argumentTranslated = true;
						} else if (opt.compare("c", Qt::CaseInsensitive) == 0 || opt.compare("config", Qt::CaseInsensitive) == 0) {
							// Additional configuration file that is passed to every executed test case
							// (e.g. AllowBackendNeighbourExpansionBlocking=false). Test-specific
							// kt:usesTestSystemConfig assertions take precedence over this default.
							mConformanceConfList.append(QString("+Konclude.Conformance.ConfigFile=%1").arg(takeValue("")));
							argumentTranslated = true;
						} else if (opt.compare("binary", Qt::CaseInsensitive) == 0) {
							mConformanceConfList.append(QString("+Konclude.Conformance.BinaryFile=%1").arg(takeValue("")));
							argumentTranslated = true;
						} else if (opt.compare("h", Qt::CaseInsensitive) == 0 || opt.compare("help", Qt::CaseInsensitive) == 0 || opt == "?") {
							mHelpRequested = true;
							argumentTranslated = true;
						}
					}
					return argumentTranslated;
				}


				QStringList CConformanceCommandLinePreparationTranslator::combineTranslatedArguments() {
					QStringList translatedArgList;
					if (mHelpRequested || !mFirstArgumentTranslation) {
						translatedArgList += QString("-ConfigurableCoutLogObserverLoader");
						translatedArgList += QString("-CommandLineTranlatorHelpLoader");
						return translatedArgList;
					}
					translatedArgList += QString("-ConfigurableCoutLogObserverLoader -LoggerConfigurationLoader");
					translatedArgList += QString("-ConformanceLoader");
					translatedArgList += mConformanceConfList;
					return translatedArgList;
				}


			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
