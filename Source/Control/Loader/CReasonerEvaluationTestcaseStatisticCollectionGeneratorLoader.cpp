/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader::CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader() {
				loaderConfig = nullptr;
			}



			CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader::~CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader() {
			}


			CLoader *CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				loaderConfig = config;
				mInputDir = CConfigDataReader::readConfigString(config,"Konclude.Test.Generator.InputDirectory");
				if (!mInputDir.endsWith("/")) {
					mInputDir += QString("/");
				}
				mResponseStatisticsOutputDir = CConfigDataReader::readConfigString(config,"Konclude.Test.Generator.OutputDirectory");
				if (!mResponseStatisticsOutputDir.endsWith("/")) {
					mResponseStatisticsOutputDir += QString("/");
				}

				mOntologyStatisticsDir = CConfigDataReader::readConfigString(config,"Konclude.Test.Generator.OntologyStatisticsDirectory");
				if (!mOntologyStatisticsDir.endsWith("/")) {
					mOntologyStatisticsDir += QString("/");
				}

				return this;
			}

			QStringList CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader::collectOntologyIRIs(const QDomElement& element) {
				QStringList iriList;
				if (element.tagName() == "OntologyIRI") {
					QString iri = element.attribute("IRI");
					if (!iri.isEmpty()) {
						iriList.append(iri);
					}
				}
				QDomElement childElement = element.firstChildElement();
				while (!childElement.isNull()) {
					iriList += collectOntologyIRIs(childElement);
					childElement = childElement.nextSiblingElement();
				}
				return iriList;
			}


			QStringList CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader::collectOntologyIRIsStatistics(const QStringList& ontoIRIList) {
				QStringList statisticList;
				if (ontoIRIList.size() == 1) {
					QString ontologyIRI = ontoIRIList.first();
					if (ontologyIRI.startsWith("file:")) {
						ontologyIRI.replace(0,5,"");
					}
					if (ontologyIRI.startsWith("Evaluation/")) {
						ontologyIRI.replace(0,11,"");
					}
					if (ontologyIRI.startsWith("Ontologies/")) {
						ontologyIRI.replace(0,11,"");
					}
					QString ontoStatFileString = mOntologyStatisticsDir+ontologyIRI+".txt";
					QFile ontoStatFile(ontoStatFileString);
					if (ontoStatFile.open(QIODevice::ReadOnly)) {
						while (!ontoStatFile.atEnd()) {
							QByteArray statDataLine(ontoStatFile.readLine());
							statisticList.append(statDataLine.trimmed());
						}
						ontoStatFile.close();
					}
				}
				return statisticList;
			}


			CLoader* CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader::collectTestcasesStatisticsForTestcase(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString) {

				QFile testcasteFile(testFilePathString+testFileNameString);
				if (testcasteFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&testcasteFile,true);
					QDomElement documentElement(document.documentElement());
					QStringList ontologyIRIs = collectOntologyIRIs(documentElement);
					testcasteFile.close();

					QStringList statList = collectOntologyIRIsStatistics(ontologyIRIs);
					if (!statList.isEmpty()) {
						QDir outputDir(mResponseStatisticsOutputDir);
						outputDir.mkpath(subDirPathString);
						QString responseStatFileString(mResponseStatisticsOutputDir+subDirPathString+testFileNameString+".txt");
						QFile responseStatFile(responseStatFileString);
						if (responseStatFile.open(QIODevice::WriteOnly)) {
                            responseStatFile.write(statList.join("\r\n").toLocal8Bit());
							responseStatFile.close();
						}
					}
				}

				return this;
			}


			CLoader* CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader::collectTestcasesStatisticsForDirectory(const QString& dirString, const QString& subDirPathString) {
				QDir inputDir(dirString);
				foreach (const QString& subDirString, inputDir.entryList(QDir::Dirs)) {
					if (subDirString != ".." && subDirString != ".") {
						collectTestcasesStatisticsForDirectory(dirString+subDirString+"/",subDirPathString+subDirString+"/");
					}
				}
				foreach (const QString& fileString, inputDir.entryList(QDir::Files)) {

					collectTestcasesStatisticsForTestcase(fileString,dirString,subDirPathString);

				}
				LOG(INFO,"::Konclude::Generator::TestCaseGenerator",logTr("Generated test cases for all files in %1.").arg(dirString),this);
				return this;
			}


			CLoader *CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader::load() {
				collectTestcasesStatisticsForDirectory(mInputDir,"");
				return this;
			}




			CLoader *CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader::exit() {
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
