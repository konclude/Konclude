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

#include "CReasonerEvaluationOntologyExtractionLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CReasonerEvaluationOntologyExtractionLoader::CReasonerEvaluationOntologyExtractionLoader() {
				loaderConfig = nullptr;
			}



			CReasonerEvaluationOntologyExtractionLoader::~CReasonerEvaluationOntologyExtractionLoader() {
			}


			CLoader *CReasonerEvaluationOntologyExtractionLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				loaderConfig = config;
				mInputDir = CConfigDataReader::readConfigString(config,"Konclude.Test.Generator.InputDirectory");
				if (!mInputDir.endsWith("/")) {
					mInputDir += QString("/");
				}
				mOutputDir = CConfigDataReader::readConfigString(config,"Konclude.Test.Generator.OutputDirectory");
				if (!mOutputDir.endsWith("/")) {
					mOutputDir += QString("/");
				}

				mOntologyStatisticsDir = CConfigDataReader::readConfigString(config,"Konclude.Test.Generator.OntologyStatisticsDirectory");
				if (!mOntologyStatisticsDir.endsWith("/")) {
					mOntologyStatisticsDir += QString("/");
				}

				mFiltering = new CReasonerEvaluationFiltering(mOntologyStatisticsDir);

				mFilterFile = CConfigDataReader::readConfigString(config,"Konclude.Test.Generator.FilterFile");
				loadFilterConditions(mFilterFile);

				QDir newOntoFileDir(".");
				newOntoFileDir.mkpath(mOutputDir);

				mExtractedOntStatOutputFileString = mOutputDir+"ExtractedOntologies.csv";

				mCollectExtractedOntologyStatistics = true;
				mCopyExtractedOntologyToOutput = true;

				return this;
			}


			bool CReasonerEvaluationOntologyExtractionLoader::loadFilterConditions(const QString& fileNameString) {
				return mFiltering->loadFilterFromFile(fileNameString);
			}


			bool CReasonerEvaluationOntologyExtractionLoader::satisfiesExtractionCondition(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString, const QStringList& statisticList) {
				return mFiltering->testSatisfiesFilter(testFileNameString,testFilePathString,subDirPathString,statisticList);
			}


			CLoader* CReasonerEvaluationOntologyExtractionLoader::addExtractedOntologyStatistics(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString, const QStringList& statisticList) {
				QHash<QString,QString>* ontStatHash = new QHash<QString,QString>();
				ontStatHash->insert("Path",subDirPathString);
				ontStatHash->insert("Ontology",testFileNameString);
				foreach (const QString& statisticString, statisticList) {
					QStringList statDataList(statisticString.split("\t"));
					if (statDataList.count() == 2) {
						QString statNameString = statDataList.takeFirst().replace(":","").trimmed();
						QString statValueString = statDataList.takeFirst().trimmed();
						ontStatHash->insert(statNameString,statValueString);
						if (!mExtractedStatsSet.contains(statNameString)) {
							mExtractedStatsSet.insert(statNameString);
							mExtractedStatsList.append(statNameString);
						}
					}
				}
				mExtractedOntStatHashList.append(ontStatHash);
				return this;
			}


			CLoader* CReasonerEvaluationOntologyExtractionLoader::saveExtractedOntologyStatistics(const QString& fileNameString) {
				mExtractedStatsList.prepend("Ontology");
				mExtractedStatsList.prepend("Path");

				QString outFileString = fileNameString;
				QFile outFile(outFileString);
				if (outFile.open(QIODevice::WriteOnly)) {

					QString titleString;
					for (QList<QString>::const_iterator itVal = mExtractedStatsList.constBegin(), itValEnd = mExtractedStatsList.constEnd(); itVal != itValEnd; ++itVal) {
						QString valString(*itVal);
						if (!titleString.isEmpty()) {
							titleString.append("\t");
						}
						titleString += valString;
					}
					outFile.write(QString("%1\r\n").arg(titleString).toLocal8Bit());

					for (QList< QHash<QString,QString>* >::const_iterator it = mExtractedOntStatHashList.constBegin(), itEnd = mExtractedOntStatHashList.constEnd(); it != itEnd; ++it) {
						QHash<QString,QString>* ontStatHash(*it);

						QString dataString;
						for (QList<QString>::const_iterator itVal = mExtractedStatsList.constBegin(), itValEnd = mExtractedStatsList.constEnd(); itVal != itValEnd; ++itVal) {
							QString valString(*itVal);
							QString valDataString(ontStatHash->value(valString));
							if (!dataString.isEmpty()) {
								dataString.append("\t");
							}
							dataString += valDataString;
						}
						outFile.write(QString("%1\r\n").arg(dataString).toLocal8Bit());
					}

					outFile.close();
				}
				return this;
			}


			CLoader* CReasonerEvaluationOntologyExtractionLoader::copyExtractedOntology(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString) {
				QString ontoFileString = testFilePathString+testFileNameString;
				QFile ontoFile(ontoFileString);
				if (ontoFile.open(QIODevice::ReadOnly)) {
					QDir newOntoFileDir(mOutputDir);
					newOntoFileDir.mkpath(subDirPathString);
					QString newOntoFileString = mOutputDir+subDirPathString+testFileNameString;
					QFile newOntoFile(newOntoFileString);
					if (newOntoFile.open(QIODevice::WriteOnly)) {
						while (!ontoFile.atEnd()) {
							QByteArray ontoData(ontoFile.read(1000000));
							newOntoFile.write(ontoData);
						}
						newOntoFile.close();
						ontoFile.close();
					}
				}
				return this;
			}




			CLoader* CReasonerEvaluationOntologyExtractionLoader::extractOntologiesForTestcase(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString) {
				QStringList statisticList;
				QString ontoStatFileString = mOntologyStatisticsDir+subDirPathString+testFileNameString+".txt";
				QFile ontoStatFile(ontoStatFileString);
				if (ontoStatFile.open(QIODevice::ReadOnly)) {
					while (!ontoStatFile.atEnd()) {
						QByteArray statDataLine(ontoStatFile.readLine());
						statisticList.append(statDataLine.trimmed());
					}
					ontoStatFile.close();

					if (!statisticList.isEmpty()) {
						if (satisfiesExtractionCondition(testFileNameString,testFilePathString,subDirPathString,statisticList)) {
							// extract ontology
							if (mCollectExtractedOntologyStatistics) {
								addExtractedOntologyStatistics(testFileNameString,testFilePathString,subDirPathString,statisticList);
							}
							if (mCopyExtractedOntologyToOutput) {
								copyExtractedOntology(testFileNameString,testFilePathString,subDirPathString);
							}
						}
					}
				}
				return this;
			}




			CLoader* CReasonerEvaluationOntologyExtractionLoader::extractOntologiesForDirectory(const QString& dirString, const QString& subDirPathString) {
				QDir inputDir(dirString);
				foreach (const QString& subDirString, inputDir.entryList(QDir::Dirs)) {
					if (subDirString != ".." && subDirString != ".") {
						extractOntologiesForDirectory(dirString+subDirString+"/",subDirPathString+subDirString+"/");
					}
				}
				foreach (const QString& fileString, inputDir.entryList(QDir::Files)) {

					extractOntologiesForTestcase(fileString,dirString,subDirPathString);

				}
				LOG(INFO,"::Konclude::Generator::TestCaseGenerator",logTr("Extracted ontologies in %1.").arg(dirString),this);
				return this;
			}


			CLoader *CReasonerEvaluationOntologyExtractionLoader::load() {
				extractOntologiesForDirectory(mInputDir,"");
				saveExtractedOntologyStatistics(mExtractedOntStatOutputFileString);
				return this;
			}




			CLoader *CReasonerEvaluationOntologyExtractionLoader::exit() {
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
