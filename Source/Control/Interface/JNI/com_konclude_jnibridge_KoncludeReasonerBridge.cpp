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

#ifdef KONCLUDE_COMPILE_JNI_INTERFACE



#include <QCoreApplication>
#include <jni.h>



#include "com_konclude_jnibridge_KoncludeReasonerBridge.h"
#include "CJNIHandler.h"
#include "CJNIInstanceManager.h"
#include "CJNICommandProcessor.h"
#include "CJNIOntologyRevisionData.h"
#include "CJNIAxiomExpressionVisitingLoader.h"
#include "CJNIQueryProcessor.h"





#include "CKoncludeInfo.h"

#include "Utilities/UtilitiesSettings.h"

#include "Logger/CLogger.h"
#include "Logger/CConsolePrintLogObserver.h"
#include "Logger/CQtDebugPrintLogObserver.h"

#include "Control/Loader/CDefaultLoaderFactory.h"
#include "Control/Loader/CJNICommandLoader.h"

//#include "Control/Interface/CommandLine/CCommandLinePreparationTranslatorSelector.h"




using namespace Konclude;
using namespace Konclude::Logger;
using namespace Konclude::Utilities;
using namespace Konclude::Control::Loader;
//using namespace Konclude::Interface::CommandLine;




static int qtAppargc = 1;
static char* qtAppargv[] = {"Konclude", NULL};
static QCoreApplication* qtApp = NULL;
static bool qtAppCreated = false;
//static QThread* thread = NULL;
QString qJNIConfigString;
QByteArray qJNIConfigByteArray;



JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_initKoncludeLibraryInstance(JNIEnv * jenv, jobject bridgeObj, jstring jniConfigString) {

	char* defaultJNIArgumentStrings[] =  { 
		//"-CoutLogObserverLoader ",
		"-DefaultReasonerLoader +=Konclude.Execution.CalculationManager=Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager +=Konclude.Debugging.WriteDebuggingData=TRUE ",
		//"-DefaultReasonerLoader +=Konclude.Execution.CalculationManager=Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager ",
		"-JNICommandProcessorLoader ",
		" "
	};

	int defaultJNIArgumentCount = 2;

	if (CJNIHandler::getJNIInstanceManagerBaseFieldID(jenv)) {
		if (CJNIHandler::setJNIInstanceManager(jenv,bridgeObj,nullptr)) {



			if (QCoreApplication::instance() == nullptr) {
				qtApp = new QCoreApplication(qtAppargc, qtAppargv);
				qtAppCreated = true;
				//qtApp->exec();
			}


			CLogger *logger = CLogger::getInstance();

			try {



				QString koncludeString(QString("%1 - %2").arg(CKoncludeInfo::getKoncludeName()).arg(CKoncludeInfo::getKoncludeNameExtension()));
				LOG(INFO,"::Konclude::Main","Starting Konclude via JNI ...",0);
				LOG(INFO,"::Konclude::Main",koncludeString,0);
				LOG(INFO,"::Konclude::Main",QString("%1, %2, Version %3 (%4)\r\n").arg(CKoncludeInfo::getKoncludeDescription()).arg(CKoncludeInfo::getKoncludeBitPlatformString()).arg(CKoncludeInfo::getKoncludeVersionString()).arg(CKoncludeInfo::getKoncludeCompilationDateString()),0);


				CJNIInstanceManager* jniInstanceManager = new CJNIInstanceManager();
				CJNIHandler* jniHandler = new CJNIHandler(jenv);
				jniInstanceManager->setJNIHandler(jniHandler);
				jniHandler->setJNIInstanceManager(jenv,bridgeObj,jniInstanceManager);

				char** jniArgumentStrings = defaultJNIArgumentStrings;
				int jniArgumentCount = defaultJNIArgumentCount;
				if (jniConfigString) {
					qJNIConfigString = jniHandler->getString(jenv,jniConfigString);
					if (!qJNIConfigString.isEmpty()) {
						//LOG(INFO,"::Konclude::Main",QString("Received '%1' through JNI.").arg(qJNIConfigString),0);
						qJNIConfigByteArray = qJNIConfigString.toLocal8Bit();
						jniArgumentStrings[0] = qJNIConfigByteArray.data();
						jniArgumentCount = 1;
					}
				}



				QStringList arguments = CJNICommandLoader::getEncodedArguments(jniArgumentCount,jniArgumentStrings,false,false);

				//CCommandLinePreparationTranslatorSelector transSelector(loaderFactory);
				//arguments = transSelector.translateArguments(arguments);
				// 
				LOG(INFO,"::Konclude::Main",QString("JNI loading arguments: %1.").arg(arguments.join(", ")),0);

				CJNICommandLoader* jniCommandLoader = new CJNICommandLoader(jniInstanceManager,arguments);
				jniInstanceManager->setJNICommandLoader(jniCommandLoader);
				CLoaderFactory* loaderFactory = new CDefaultLoaderFactory();
				jniInstanceManager->setJNILoaderFactory(loaderFactory);
				jniCommandLoader->init(loaderFactory);
				jniCommandLoader->load();
				jniCommandLoader->waitSynchronization();


				if (!jniInstanceManager->getJNICommandProcessor()) {
					LOG(WARN,"::Konclude::Main","No JNI command processor loaded!",0);
				} else {
					LOG(INFO,"::Konclude::Main","JNI command processer successfully initialised.",0);
				}



			} catch (...) {
				LOG(CATASTROPHIC,"::Konclude::Main",logTr("Stop because of an unknown catastrophic error."),0);
			}

		}
	}
}





JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_closeKoncludeLibraryInstance(JNIEnv * jenv, jobject bridgeObj) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {

		CJNICommandLoader* jniCommandLoader = jniInstanceManager->getJNICommandLoader();
		jniCommandLoader->exit();

		LOG(INFO,"::Konclude::Main",logTr("Stopping Konclude ..."),0);

		delete jniInstanceManager;
		CJNIHandler::setJNIInstanceManager(jenv,bridgeObj,nullptr);
	}
}




JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_initAxiomExpressionVisitingBridge(JNIEnv * jenv, jobject bridgeObj, jobject builderObj) {
	LOG(INFO,"::Konclude::Main",logTr("Creating axiom/expression building bridge."),0);
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNICommandProcessor* jniProcessor = jniInstanceManager->getJNICommandProcessor();



		COntologyRevision* ontRev = nullptr;
		CJNIOntologyRevisionData* ontRevData = jniInstanceManager->getOntologyRevisionData();
		if (!ontRevData) {
			LOG(INFO,"::Konclude::Main",logTr("Creating new anonomous ontology"),0);
			ontRev = jniProcessor->getNewlyCreatedAnonymousOntologyRevision();
		} else {
			LOG(INFO,"::Konclude::Main",logTr("Creating new ontology revision."),0);
			ontRev = jniProcessor->getNewOntologyRevision(ontRevData->getOntologyRevision());
		}
		ontRevData = new CJNIOntologyRevisionData(ontRevData);
		ontRevData->setOntologyRevision(ontRev);
		
		CJNIAxiomExpressionVisitingLoader* axiomExpVisitLoader = new CJNIAxiomExpressionVisitingLoader(jniInstanceManager,ontRevData);
		axiomExpVisitLoader->initializeBuilding();
		jniHandler->setAxiomExpressionBuildingBridgeNativeData(jenv,builderObj,axiomExpVisitLoader);
	}
}







JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_finalizeAxiomExpressionVisitingBridge(JNIEnv * jenv, jobject bridgeObj, jobject builderObj) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		LOG(INFO,"::Konclude::Main",logTr("Finalizing axiom/expression building bridge."),0);
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNICommandProcessor* jniProcessor = jniInstanceManager->getJNICommandProcessor();
		CJNIAxiomExpressionVisitingLoader* axiomExpVisitLoader = jniHandler->getAxiomExpressionBuildingBridgeNativeData(jenv,builderObj);
		if (axiomExpVisitLoader) {
			axiomExpVisitLoader->completeBuilding();
			CJNIOntologyRevisionData* ontRevData = axiomExpVisitLoader->getOntologyRevisionData();
			COntologyRevision* ontRev = jniProcessor->installNewOntologyRevision(ontRevData->getOntologyRevision());
			// mapping hash merging? (in case several axioms/expressions are installed by different builders)
			ontRevData->setOntologyRevision(ontRev);
			jniInstanceManager->setOntologyRevisionData(ontRevData);
			delete axiomExpVisitLoader;
		}
	}
}








JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_initQueryingBridge(JNIEnv * jenv, jobject bridgeObj, jobject processorObj) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		LOG(INFO,"::Konclude::Main",logTr("Creating querying bridge."),0);
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNICommandProcessor* jniProcessor = jniInstanceManager->getJNICommandProcessor();


		CJNIOntologyRevisionData* ontRevData = jniInstanceManager->getOntologyRevisionData();
		if (ontRevData) {
			if (!ontRevData->isQueryingPrepared()) {
				jniProcessor->prepareOntologyQuerying(ontRevData->getOntologyRevision());
				ontRevData->setQueryingPrepared(true);
			}

			CJNIQueryProcessor* queryProcessor = new CJNIQueryProcessor(jniInstanceManager,ontRevData);
			jniHandler->setQueryingBridgeNativeData(jenv,processorObj,queryProcessor);
		}
	}
}







JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_finalizeQueryingBridge(JNIEnv * jenv, jobject bridgeObj, jobject processorObj) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		LOG(INFO,"::Konclude::Main",logTr("Closing querying bridge"),0);
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNICommandProcessor* jniProcessor = jniInstanceManager->getJNICommandProcessor();
		CJNIQueryProcessor* queryProcessor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (queryProcessor) {
			delete queryProcessor;
		}
	}
}



#endif