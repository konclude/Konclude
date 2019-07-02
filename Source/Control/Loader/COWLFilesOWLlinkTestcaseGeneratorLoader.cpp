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

				mFilterDir = CConfigDataReader::readConfigString(config,"Konclude.Test.Generator.FilterDirectory");

				mConfCreateClassifyTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateClassifyTests",false);
				mConfCreateConsistencyTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateConsistencyTests",false);
				mConfCreateSatisfiabilityTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateSatisfiabilityTests",false);
				mConfCreateTypeTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateTypeTests",false);





				mOutputDirClassify = mOutputDir+"Classification/";
				mOutputDirConsistency = mOutputDir+"Consistency/";
				mOutputDirSatisfiability = mOutputDir+"Satisfiability/";
				mOutputDirTypes = mOutputDir+"Types/";


				mConfCreateIncAssConsistencyTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateIncrementalAssertionConsistencyTests",false);
				mIncAssConsistencyAdditionCount = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.AssertionAdditionCount",10);
				mIncAssConsistencyRemovalCount = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.AssertionRemovalCount",10);
				mIncAssConsTestCount = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.TestCount",10);
				mConfCreateIncAssConsistencyByRatio = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.RatioAdditionRemoval",false);
				mIncAssConsistencyAdditionCountRatioDenominator = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.AssertionAdditionRatioDenominator",1000);
				mIncAssConsistencyRemovalCountRatioDenominator = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.AssertionRemovalRatioDenominator",1000);

				mOutputDirIncAssConsistency = mOutputDir+QString("Incremental/NumberAssertionsAddedRemoved/Consistency/#A%1/#R%2/").arg(mIncAssConsistencyAdditionCount).arg(mIncAssConsistencyRemovalCount);
				if (mConfCreateIncAssConsistencyByRatio) {
					mOutputDirIncAssConsistency = mOutputDir+QString("Incremental/RatioAssertionsAddedRemoved/Consistency/A%1of%2/R%3of%4/").arg(mIncAssConsistencyAdditionCount).arg(mIncAssConsistencyAdditionCountRatioDenominator).arg(mIncAssConsistencyRemovalCount).arg(mIncAssConsistencyRemovalCountRatioDenominator);
				}





				mConfCreateIncAssClassificationTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateIncrementalAssertionClassificationTests",false);
				mIncAssClassificationAdditionCount = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.AssertionAdditionCount",10);
				mIncAssClassificationRemovalCount = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.AssertionRemovalCount",10);
				mIncAssClassificationTestCount = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.TestCount",10);
				mConfCreateIncAssClassificationByRatio = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.RatioAdditionRemoval",false);
				mIncAssClassificationAdditionCountRatioDenominator = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.AssertionAdditionRatioDenominator",1000);
				mIncAssClassificationRemovalCountRatioDenominator = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.AssertionRemovalRatioDenominator",1000);

				mOutputDirIncAssClassification = mOutputDir+QString("Incremental/NumberAssertionsAddedRemoved/Classification/#A%1/#R%2/").arg(mIncAssClassificationAdditionCount).arg(mIncAssClassificationRemovalCount);
				if (mConfCreateIncAssClassificationByRatio) {
					mOutputDirIncAssClassification = mOutputDir+QString("Incremental/RatioAssertionsAddedRemoved/Classification/A%1of%2/R%3of%4/").arg(mIncAssClassificationAdditionCount).arg(mIncAssClassificationAdditionCountRatioDenominator).arg(mIncAssClassificationRemovalCount).arg(mIncAssClassificationRemovalCountRatioDenominator);
				}




				mConfCreateIncAssRealizationTests = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.GenerateIncrementalAssertionRealizationTests",false);
				mIncAssRealizationAdditionCount = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.AssertionAdditionCount",10);
				mIncAssRealizationRemovalCount = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.AssertionRemovalCount",10);
				mIncAssRealizationTestCount = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.TestCount",10);
				mConfCreateIncAssRealizationByRatio = CConfigDataReader::readConfigBoolean(config,"Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.RatioAdditionRemoval",false);
				mIncAssRealizationAdditionCountRatioDenominator = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.AssertionAdditionRatioDenominator",1000);
				mIncAssRealizationRemovalCountRatioDenominator = CConfigDataReader::readConfigInteger(config,"Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.AssertionRemovalRatioDenominator",1000);

				mOutputDirIncAssRealization = mOutputDir+QString("Incremental/NumberAssertionsAddedRemoved/Realization/#A%1/#R%2/").arg(mIncAssRealizationAdditionCount).arg(mIncAssRealizationRemovalCount);
				if (mConfCreateIncAssRealizationByRatio) {
					mOutputDirIncAssRealization = mOutputDir+QString("Incremental/RatioAssertionsAddedRemoved/Realization/A%1of%2/R%3of%4/").arg(mIncAssRealizationAdditionCount).arg(mIncAssRealizationAdditionCountRatioDenominator).arg(mIncAssRealizationRemovalCount).arg(mIncAssRealizationRemovalCountRatioDenominator);
				}
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
				if (mFilterDir.isEmpty() || subDirPathString == mFilterDir || subDirPathString.startsWith(mFilterDir)) {
					foreach (const QString& fileString, inputDir.entryList(QDir::Files)) {
						QString outFileString = fileString;
						QDir classifyOutputDir(mOutputDirClassify);
						QDir consistencyOutputDir(mOutputDirConsistency);
						QDir satisfiabilityOutputDir(mOutputDirSatisfiability);
						QDir typeOutputDir(mOutputDirTypes);
						QDir consIncAssConsDir(mOutputDirIncAssConsistency);
						QDir consIncAssClassificationDir(mOutputDirIncAssClassification);
						QDir consIncAssRealizationDir(mOutputDirIncAssRealization);

						outFileString = outFileString.replace(".owl.xml","");
						QString classifyOutFileString = outFileString+"-classify-request.xml";
						QString consistencyOutFileString = outFileString+"-consistency-request.xml";
						QString satisfiabilityOutFileString = outFileString+"-satisfiability-%1-request.xml";
						QString typeOutFileString = outFileString+"-type-%1-request.xml";
						QString incAssConsFileString = outFileString+"-inc-ass-consistency-%1-request.xml";
						QString incAssClassificationFileString = outFileString+"-inc-ass-classification-%1-request.xml";
						QString incAssRealizationFileString = outFileString+"-inc-ass-realization-%1-request.xml";
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
						if (mConfCreateIncAssConsistencyTests) {
							consIncAssConsDir.mkpath(subDirPathString);
							if (mConfCreateIncAssConsistencyByRatio) {
								mIncAssConsistencyGen->generateOWLlinkIncrementalAssertionRequests(dirString+fileString,mOutputDirIncAssConsistency+subDirPathString+incAssConsFileString,(double)mIncAssConsistencyRemovalCount/(double)mIncAssConsistencyRemovalCountRatioDenominator,(double)mIncAssConsistencyAdditionCount/(double)mIncAssConsistencyAdditionCountRatioDenominator,mIncAssConsTestCount);
							} else {
								mIncAssConsistencyGen->generateOWLlinkIncrementalAssertionRequests(dirString+fileString,mOutputDirIncAssConsistency+subDirPathString+incAssConsFileString,mIncAssConsistencyRemovalCount,mIncAssConsistencyAdditionCount,mIncAssConsTestCount);
							}
						}
						if (mConfCreateIncAssClassificationTests) {
							consIncAssClassificationDir.mkpath(subDirPathString);
							if (mConfCreateIncAssClassificationByRatio) {
								mIncAssClassificationGen->generateOWLlinkIncrementalAssertionRequests(dirString+fileString,mOutputDirIncAssClassification+subDirPathString+incAssClassificationFileString,(double)mIncAssClassificationRemovalCount/(double)mIncAssClassificationRemovalCountRatioDenominator,(double)mIncAssClassificationAdditionCount/(double)mIncAssClassificationAdditionCountRatioDenominator,mIncAssClassificationTestCount);
							} else {
								mIncAssClassificationGen->generateOWLlinkIncrementalAssertionRequests(dirString+fileString,mOutputDirIncAssClassification+subDirPathString+incAssClassificationFileString,mIncAssClassificationRemovalCount,mIncAssClassificationAdditionCount,mIncAssConsTestCount);
							}
						}
						if (mConfCreateIncAssRealizationTests) {
							consIncAssRealizationDir.mkpath(subDirPathString);
							if (mConfCreateIncAssRealizationByRatio) {
								mIncAssRealizationGen->generateOWLlinkIncrementalAssertionRequests(dirString+fileString,mOutputDirIncAssRealization+subDirPathString+incAssRealizationFileString,(double)mIncAssRealizationRemovalCount/(double)mIncAssRealizationRemovalCountRatioDenominator,(double)mIncAssRealizationAdditionCount/(double)mIncAssRealizationAdditionCountRatioDenominator,mIncAssRealizationTestCount);
							} else {
								mIncAssRealizationGen->generateOWLlinkIncrementalAssertionRequests(dirString+fileString,mOutputDirIncAssRealization+subDirPathString+incAssRealizationFileString,mIncAssRealizationRemovalCount,mIncAssRealizationAdditionCount,mIncAssConsTestCount);
							}
						}
					}
					LOG(INFO,"::Konclude::Generator::TestCaseGenerator",logTr("Generated test cases for all files in %1.").arg(dirString),this);
				}
				return this;
			}


			CLoader *COWLFilesOWLlinkTestcaseGeneratorLoader::load() {
				mClassifyGen = new COWLFileOWLlinkClassifyRequestGenerator(loaderConfig);
				mConsistencyGen = new COWLFileOWLlinkConsistencyRequestGenerator(loaderConfig);
				mSatisfiabilityGen = new COWLFileOWLlinkSatisfiabilityRequestGenerator(loaderConfig);
				mTypesGen = new COWLFileOWLlinkTypesRequestGenerator(loaderConfig);
				mIncAssConsistencyGen = new COWLFileOWLlinkIncrementalAssertionConsistencyRequestGenerator(loaderConfig);
				mIncAssClassificationGen = new COWLFileOWLlinkIncrementalAssertionClassificationRequestGenerator(loaderConfig);
				mIncAssRealizationGen = new COWLFileOWLlinkIncrementalAssertionRealizationRequestGenerator(loaderConfig);

				generateTestcasesForDirectory(mInputDir,"");

				delete mClassifyGen;
				delete mConsistencyGen;
				delete mSatisfiabilityGen;
				delete mTypesGen;
				delete mIncAssConsistencyGen;
				delete mIncAssClassificationGen;
				delete mIncAssRealizationGen;
				return this;
			}




			CLoader *COWLFilesOWLlinkTestcaseGeneratorLoader::exit() {
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
