/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CCommandLineLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CCommandLineLoader::CCommandLineLoader() : CThread("CommandLineLoader") {
			}


			QStringList CCommandLineLoader::getEncodedArguments(int argc, char *argv[], bool skipFirst, bool encodeSpaces) {
				QString argString;
				int startArg = 0;
				if (skipFirst) {
					startArg = 1;
				}
				for (int i = startArg; i < argc; ++i) {
					if (!argString.isEmpty()) {
						argString += QString(" ");
					}
					QString translatedArgString = QString::fromUtf8(argv[i]);
					//QString translatedArgString = QString::fromLocal8Bit(argv[i]).trimmed();
					if (translatedArgString.contains(" ") && encodeSpaces) {
						argString += QString("\"%1\"").arg(translatedArgString);
					} else {
						argString += QString("%1").arg(translatedArgString);
					}
				}
				QString encodedString;
				bool encodeModus = false;
				for (QString::const_iterator it = argString.constBegin(), itEnd = argString.constEnd(); it != itEnd; ++it) {
					QChar character(*it);
					if (character == QChar('"')) {
						encodeModus = !encodeModus;
					} else if (character == QChar(' ') && encodeModus) {
						encodedString += QString("?_");
					//} else if (character == QChar('=') && encodeModus) {
					//	encodedString += QString("?~");
					} else if (character == QChar('?')) {
						encodedString += QString("??");
					} else {
						encodedString += character;
					}
				}
				while (encodedString.contains("  ")) {
					encodedString.replace("  "," ");
				}
				QStringList encodedList;
				if (!encodedString.isEmpty()) {
					encodedList = encodedString.split(" ");
				}
				return encodedList;
			}

			QString CCommandLineLoader::getDecodedArgument(const QString& string) {
				QString decodedString;
				for (QString::const_iterator it = string.constBegin(), itEnd = string.constEnd(); it != itEnd; ++it) {
					QChar character(*it);
					if (character == QChar('?')) {
						++it;
						QChar nextCharacter(*it);
						if (nextCharacter == QChar('?')) {
							decodedString += QString("?");
						} else if (nextCharacter == QChar('_')) {
							decodedString += QString(" ");
						//} else if (nextCharacter == QChar('~')) {
						//	decodedString += QString("=");
						}
					} else {
						decodedString += character;
					}
				}
				return decodedString;
			}

			CCommandLineLoader::CCommandLineLoader(int argc, char *argv[], bool qtGUIApplication) : CThread("CommandLineLoader") {
				for (int i = 0; i < argc; ++i) {
					commandLineParameters.append(argv[i]);
				}
				qtGuiUsable = qtGUIApplication;
			}


			CCommandLineLoader::CCommandLineLoader(const QStringList &argList, bool qtGUIApplication) : CThread("CommandLineLoader") {
				foreach (QString arg, argList) {
					commandLineParameters.append(arg);
				}
				qtGuiUsable = qtGUIApplication;
			}


			CCommandLineLoader::~CCommandLineLoader() {
				qDeleteAll(loaderContainer);
				qDeleteAll(configContainer);
			}


			CLoader *CCommandLineLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				configuration = config;
				lFactory = loaderFactory;

				qint64 paramCount = commandLineParameters.count();

				CConfigurationGroup *reasonerConfigGroup = 0;
				if (configuration) {
					reasonerConfigGroup = configuration->getConfigurationGroup();
				} else {
					reasonerConfigGroup = CSingletonProvider<CReasonerConfigurationGroup>::getInstance()->getReferencedConfigurationGroup();
					configuration = new CConfiguration(reasonerConfigGroup);
				}

				reasonerConfigGroup->addConfigProperty(new CConfigDescription("Konclude.Execution.CommandLine.ParameterCount","",new CIntegerConfigType(paramCount)));

				QString text;
				QStringList::const_iterator it = commandLineParameters.constBegin();
				for (qint64 i = 0; i < paramCount && it != commandLineParameters.constEnd(); ++i,++it) {
					text += (*it) + " ";
					reasonerConfigGroup->addConfigProperty(new CConfigDescription(QString("Konclude.Execution.CommandLine.Parameter%1").arg(i),"",new CStringConfigType(getDecodedArgument(*it))));
				}
				reasonerConfigGroup->addConfigProperty(new CConfigDescription("Konclude.Execution.CommandLine.Text","",new CStringConfigType(getDecodedArgument(text))));
				
				configuration->extendConfigurationGroup(reasonerConfigGroup);



				qint64 nextPos = getFirstTextOccur(text,"-");
				QStringList matchesList(getMatchesFromPattern("(\\+\\=\\s*(\\w+\\.?)+\\s*\\=\\s*\\S+)",text,nextPos));
				foreach (QString configString, matchesList) {
					setConfiguration(configString,configuration,true);
				}					

				loadingText = text.mid(nextPos);


				QString nextLoadingText = loadingText;

				bool oneLoadingSuccess = false;

				while (!nextLoadingText.isEmpty()) {

					qint64 nextLoadingPos = getFirstTextOccur(nextLoadingText," -",1);
					qint64 nextConfigPos = qMin(getFirstTextOccur(nextLoadingText,"+",1),nextLoadingPos-1);

					QStringList loadStringList = getMatchesFromPattern("(-\\s*\\w+\\s+)",nextLoadingText,nextConfigPos);
					QString loadString;
					if (loadStringList.count() > 0) {
						loadString = loadStringList.first();
					}

					QStringList matchesList(getMatchesFromPattern("(\\+\\=\\s*(\\w+\\.?)+\\s*\\=\\s*\\S+)",nextLoadingText,nextLoadingPos));
					foreach (QString configString, matchesList) {
						setConfiguration(configString,configuration,true);
					}					

					if (!loadString.isEmpty()) {

						loadString = loadString.remove("-");
						loadString = loadString.trimmed();

						CLoader *loader = lFactory->createLoaderFromName(loadString);

						if (loader) {
							oneLoadingSuccess = true;
							CConfiguration *loadConfiguration = new CConfiguration(configuration);

							configContainer.append(loadConfiguration);
							loaderContainer.append(loader);

							QStringList matchesList(getMatchesFromPattern("(\\+\\s*(\\w+\\.?)+\\s*\\=\\s*\\S+)",nextLoadingText,nextLoadingPos));
							foreach (QString configString, matchesList) {
								setConfiguration(configString,loadConfiguration);
							}

							loader->init(lFactory,loadConfiguration);
						} else {
							LOG(ERROR,"::Konclude::Loader::CommandLineLoader",logTr("No Loader for '%1' available.").arg(loadString),0);
						}
					}

					nextLoadingText = nextLoadingText.mid(nextLoadingPos);
				}

				if (!oneLoadingSuccess) {
					LOG(ERROR,"::Konclude::Loader::CommandLineLoader",logTr("No loader specified for '%1'.").arg(commandLineParameters.join(", ")),0);
					CLoader *loader1 = lFactory->createLoaderFromName("ConfigurableCoutLogObserverLoader");
					CLoader *loader2 = lFactory->createLoaderFromName("CommandLineTranlatorHelpLoader");
					loaderContainer.append(loader1);
					loaderContainer.append(loader2);
				}

				return this;
			}


			QStringList CCommandLineLoader::getMatchesFromPattern(const QString &pattern, const QString &text, qint64 maxPosition) {
				QStringList list;
				QRegExp configExp(pattern);
				int pos = 0;
				while ((pos = configExp.indexIn(text,pos)) != -1 && pos <= maxPosition) {
					list << configExp.cap(1);
					pos += configExp.matchedLength();
				}
				return list;
			}


			qint64 CCommandLineLoader::getFirstTextOccur(const QString &text, const QString &textOccur, qint64 beginSearchPos) {
				qint64 pos = text.indexOf(textOccur,beginSearchPos);
				if (pos == -1) {
					pos = text.length();
				}
				return pos;
			}

			QStringList CCommandLineLoader::getMatchesFromPattern(const QString &pattern, const QString &text) {
				return getMatchesFromPattern(pattern,text,text.length());
			}


			CCommandLineLoader *CCommandLineLoader::setConfiguration(const QString &configString, CConfiguration *configuration, bool equalsConfSet) {
				QRegExp probNameRegExp;
				if (equalsConfSet) {
					probNameRegExp = QRegExp("(\\+\\=\\s*(\\w+\\.?)+\\s*\\=\\s*)");
				} else {
					probNameRegExp = QRegExp("(\\+\\s*(\\w+\\.?)+\\s*\\=\\s*)");
				}
				probNameRegExp.indexIn(configString);
				QString name = probNameRegExp.cap(1);
				name = name.remove("+");
				name = name.remove("=");
				name = name.trimmed();
				qint64 lastEqIdx = configString.lastIndexOf("=");
				QString dat = configString.mid(lastEqIdx);
				dat = dat.remove("=");
				dat = dat.trimmed();

				CConfigurationGroup *configGroup = configuration->getConfigurationGroup();

				QString decodedConfigName = getDecodedArgument(name);
				QString decodedConfigData = getDecodedArgument(dat);
				if (!decodedConfigName.isEmpty()) {					

					if (!configGroup->getConfigDescription(decodedConfigName) && decodedConfigName.startsWith("Load.")) {

						configGroup = configGroup->getReferencedConfigurationGroup();

						CConfigType *configType = new CStringConfigType();

						CConfigDescription *configDes = new CConfigDescription(decodedConfigName,"",configType);
						CConfigData *confData = new CConfigData(configDes,configType->getTypeCopy());
						confData->readFromString(decodedConfigData);
						configGroup->addConfigProperty(configDes,confData);
						configuration->extendConfigurationGroup(configGroup);
					} else {
						CConfigDescription *conDes = configGroup->getConfigDescription(decodedConfigName);
						if (conDes) {
							if (conDes->supportsStringConvertion()) {
								CConfigData *confData = configuration->createAndSetConfig(decodedConfigName);
								if (confData) {
									confData->readFromString(decodedConfigData);
								}
							} else {
								LOG(ERROR,"::Konclude::Loader::CommandLineLoader",logTr("Configuration '%1' couldn't parsed from string.").arg(decodedConfigName),0);
							}
						} else {
							LOG(ERROR,"::Konclude::Loader::CommandLineLoader",logTr("Configuration '%1' not supported.").arg(decodedConfigName),0);
						}
					}
				}
				return this;
			}


			CLoader *CCommandLineLoader::load() {
				startThread();
				return this;
			}

			CLoader *CCommandLineLoader::exit() {
				stopThread();
				return this;
			}

			void CCommandLineLoader::threadStarted() {
				containerSync.lock();
				foreach (CLoader *loader, loaderContainer) {
					loader->load();
				}
				containerSync.unlock();
			}

			
			
			void CCommandLineLoader::threadStopped() {
				containerSync.lock();
				foreach (CLoader *loader, loaderContainer) {
					loader->exit();
				}
				containerSync.unlock();
			}


		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
