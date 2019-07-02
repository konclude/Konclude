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
				mInputDir = CConfigDataReader::readConfigString(config,"Konclude.Test.Generator.InputDirectory");
				mOutputDir = CConfigDataReader::readConfigString(config,"Konclude.Test.Generator.OutputDirectory");


				mConfCreateClassifyTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateClassifyTests",false);
				mConfCreateConsistencyTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateConsistencyTests",false);
				mConfCreateSatisfiabilityTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateSatisfiabilityTests",false);
				mConfCreateTypeTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateTypeTests",false);


				mOutputDirClassify = mOutputDir+"Classification/";
				mOutputDirConsistency = mOutputDir+"Consistency/";
				mOutputDirSatisfiability = mOutputDir+"Satisfiability/";
				mOutputDirTypes = mOutputDir+"Types/";

				mSatisfiabilityTestCount = 3;
				mTypesTestCount = 3;

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
					QDir consistencyOutputDir(mOutputDirConsistency);
					QDir satisfiabilityOutputDir(mOutputDirSatisfiability);
					QDir typeOutputDir(mOutputDirTypes);

					outFileString = outFileString.replace(".owl.xml","");
					QString classifyOutFileString = outFileString+"-classify-request.xml";
					QString consistencyOutFileString = outFileString+"-consistency-request.xml";
					QString satisfiabilityOutFileString = outFileString+"-satisfiability-%1-request.xml";
					QString typeOutFileString = outFileString+"-type-%1-request.xml";
					if (mConfCreateClassifyTests) {
						classifyOutputDir.mkpath(subDirPathString);
						mClassifyGen->generateOWLlinkClassifyRequest(dirString+fileString,mOutputDirClassify+subDirPathString+classifyOutFileString);
					}
					if (mConfCreateConsistencyTests) {
						consistencyOutputDir.mkpath(subDirPathString);
						mConsistencyGen->generateOWLlinkConsistencyRequest(dirString+fileString,mOutputDirConsistency+subDirPathString+consistencyOutFileString);
					}
					if (mConfCreateSatisfiabilityTests) {
						satisfiabilityOutputDir.mkpath(subDirPathString);
						mSatisfiabilityGen->generateOWLlinkSatisfiabilityRequests(dirString+fileString,mOutputDirSatisfiability+subDirPathString+satisfiabilityOutFileString,mSatisfiabilityTestCount);
					}
					if (mConfCreateTypeTests) {
						typeOutputDir.mkpath(subDirPathString);
						mTypesGen->generateOWLlinkTypesRequests(dirString+fileString,mOutputDirTypes+subDirPathString+typeOutFileString,mTypesTestCount);
					}
				}
				LOG(INFO,"::Konclude::Generator::TestCaseGenerator",logTr("Generated test cases for all files in %1.").arg(dirString),this);
				return this;
			}


			CLoader *COWLFilesOWLlinkTestcaseGeneratorLoader::load() {
				mClassifyGen = new COWLFileOWLlinkClassifyRequestGenerator(loaderConfig);
				mConsistencyGen = new COWLFileOWLlinkConsistencyRequestGenerator(loaderConfig);
				mSatisfiabilityGen = new COWLFileOWLlinkSatisfiabilityRequestGenerator(loaderConfig);
				mTypesGen = new COWLFileOWLlinkTypesRequestGenerator(loaderConfig);

				generateTestcasesForDirectory(mInputDir,"");

				delete mClassifyGen;
				delete mConsistencyGen;
				delete mSatisfiabilityGen;
				delete mTypesGen;
				return this;
			}




			CLoader *COWLFilesOWLlinkTestcaseGeneratorLoader::exit() {
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
