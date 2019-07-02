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

#include "COWLFilesOWLlinkTestcaseGeneratorLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			COWLFilesOWLlinkTestcaseGeneratorLoader::COWLFilesOWLlinkTestcaseGeneratorLoader() {
				loaderConfig = nullptr;
			}



			COWLFilesOWLlinkTestcaseGeneratorLoader::~COWLFilesOWLlinkTestcaseGeneratorLoader() {
			}


			CLoader *COWLFilesOWLlinkTestcaseGeneratorLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				loaderConfig = config;
				CConfigData *data = config->getLastConfigChange("Konclude.Test.Generator.InputDirectory");
				if (data) {
					mInputDir = data->getString();
				}
				if (!mInputDir.endsWith("/")) {
					mInputDir += QString("/");
				}

				data = config->getLastConfigChange("Konclude.Test.Generator.OutputDirectory");
				if (data) {
					mOutputDir = data->getString();
				}
				if (!mOutputDir.endsWith("/")) {
					mOutputDir += QString("/");
				}
				mOutputDirClassify = mOutputDir+"Classification/";
				mOutputDirConsistency = mOutputDir+"Consistency/";
				mOutputDirSatisfiability = mOutputDir+"Satisfiability/";

				mSatisfiabilityTestCount = 3;

				return this;
			}



			CLoader* COWLFilesOWLlinkTestcaseGeneratorLoader::generateTestcasesForDirectory(const QString& dirString, const QString& subDirPathString) {
				QDir inputDir(dirString);
				foreach (const QString& subDirString, inputDir.entryList(QDir::Dirs)) {
					if (subDirString != ".." && subDirString != ".") {
						generateTestcasesForDirectory(dirString+subDirString+"/",subDirPathString+subDirString+"/");
					}
				}
				foreach (const QString& fileString, inputDir.entryList(QDir::Files)) {
					QString outFileString = fileString;
					QDir classifyOutputDir(mOutputDirClassify);
					classifyOutputDir.mkpath(subDirPathString);
					QDir consistencyOutputDir(mOutputDirConsistency);
					consistencyOutputDir.mkpath(subDirPathString);
					QDir satisfiabilityOutputDir(mOutputDirSatisfiability);
					satisfiabilityOutputDir.mkpath(subDirPathString);

					outFileString = outFileString.replace(".owl.xml","");
					QString classifyOutFileString = outFileString+"-classify-request.xml";
					QString consistencyOutFileString = outFileString+"-consistency-request.xml";
					QString satisfiabilityOutFileString = outFileString+"-satisfiability-%1-request.xml";
					mClassifyGen->generateOWLlinkClassifyRequest(dirString+fileString,mOutputDirClassify+subDirPathString+classifyOutFileString);
					mConsistencyGen->generateOWLlinkConsistencyRequest(dirString+fileString,mOutputDirConsistency+subDirPathString+consistencyOutFileString);
					mSatisfiabilityGen->generateOWLlinkConsistencyRequests(dirString+fileString,mOutputDirSatisfiability+subDirPathString+satisfiabilityOutFileString,mSatisfiabilityTestCount);
				}
				LOG(INFO,"::Konclude::Generator::TestCaseGenerator",logTr("Generated test cases for all files in %1.").arg(dirString),this);
				return this;
			}


			CLoader *COWLFilesOWLlinkTestcaseGeneratorLoader::load() {
				mClassifyGen = new COWLFileOWLlinkClassifyRequestGenerator(loaderConfig);
				mConsistencyGen = new COWLFileOWLlinkConsistencyRequestGenerator(loaderConfig);
				mSatisfiabilityGen = new COWLFileOWLlinkSatisfiabilityRequestGenerator(loaderConfig);

				generateTestcasesForDirectory(mInputDir,"");

				delete mClassifyGen;
				delete mConsistencyGen;
				delete mSatisfiabilityGen;
				return this;
			}




			CLoader *COWLFilesOWLlinkTestcaseGeneratorLoader::exit() {
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
