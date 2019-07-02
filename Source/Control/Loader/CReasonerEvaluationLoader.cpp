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

#include "CReasonerEvaluationLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CReasonerEvaluationLoader::CReasonerEvaluationLoader() {
				mReasonerCommander = nullptr;
				mDeleteReasonerCommander = nullptr;
				mLoaderConfig = nullptr;
			}



			CReasonerEvaluationLoader::~CReasonerEvaluationLoader() {
			}


			CLoader *CReasonerEvaluationLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {

				mLoaderConfig = config;

				CConfigurationGroup* group = mLoaderConfig->getConfigurationGroup();

				CConfigData* data = mLoaderConfig->createConfig("Konclude.Execution.CommanderManager");
				CCommanderManagerConfigType* rCConfig = dynamic_cast<CCommanderManagerConfigType*>(data->getConfigType());
				if (rCConfig) {
					mReasonerCommander = dynamic_cast<CCommanderManagerThread*>(rCConfig->getCommanderManager());
				}

				if (!mReasonerCommander) {
					mDeleteReasonerCommander = mReasonerCommander = new CCommanderManagerThread();
					rCConfig->setCommanderManager(mReasonerCommander);
				}

				group->setConfigDefaultData(group->getConfigIndex("Konclude.Execution.CommanderManager"),data);

				return this;
			}



			CLoader *CReasonerEvaluationLoader::load() {
				if (mDeleteReasonerCommander) {
					mDeleteReasonerCommander->realizeCommand(new CInitializeConfigurationCommand(mLoaderConfig));
				}

				QString globalConfig = CConfigDataReader::readConfigString(mLoaderConfig,"Konclude.Evaluation.ConfigFile");
				QString reasonerConfig = CConfigDataReader::readConfigString(mLoaderConfig,"Konclude.Evaluation.Reasoner.ConfigFile");

				QString testInputDirectory = CConfigDataReader::readConfigString(mLoaderConfig,"Konclude.Evaluation.InputDirectory");
				QString testOutputDirectory = CConfigDataReader::readConfigString(mLoaderConfig,"Konclude.Evaluation.OutputDirectory");

				QString plattform = CConfigDataReader::readConfigString(mLoaderConfig,"Konclude.Evaluation.Platform");

				CReasonerEvaluationTestsuite* reasonerEvalTestsuite = new CReasonerEvaluationTestsuite();
				reasonerEvalTestsuite->initEvaluationTestsuite(globalConfig,reasonerConfig,testInputDirectory,testOutputDirectory,plattform,nullptr);

				CBlockingCallbackData blockCallData;

				CReasonerEvaluationExecutorThread* reasonerExecutorThread = new CReasonerEvaluationExecutorThread(mLoaderConfig);
				reasonerExecutorThread->executeEvaluation(reasonerEvalTestsuite,&blockCallData);
				blockCallData.waitForCallback();
				delete reasonerExecutorThread;

				return this;
			}




			CLoader *CReasonerEvaluationLoader::exit() {
				delete mDeleteReasonerCommander;
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
