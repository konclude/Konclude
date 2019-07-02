/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include <QCoreApplication>

#include "CKoncludeInfo.h"

#include "Utilities/UtilitiesSettings.h"

#include "Logger/CLogger.h"
#include "Logger/CConsolePrintLogObserver.h"
#include "Logger/CQtDebugPrintLogObserver.h"


#include "Control/Loader/CCommandLineLoader.h"
#include "Control/Loader/CDefaultLoaderFactory.h"

#include "Control/Interface/CommandLine/CCommandLinePreparationTranslatorSelector.h"


using namespace Konclude;
using namespace Konclude::Logger;
using namespace Konclude::Utilities;
using namespace Konclude::Control::Loader;
using namespace Konclude::Control::Interface::CommandLine;




int main(int argc, char *argv[])
{

	QCoreApplication a(argc, argv);

	CLogger *logger = CLogger::getInstance();

	try {

		QString koncludeString(QString("%1 - %2").arg(CKoncludeInfo::getKoncludeName()).arg(CKoncludeInfo::getKoncludeNameExtension()));

		LOG(INFO,"::Konclude::Main","Starting Konclude ... ",0);
		LOG(INFO,"::Konclude::Main",koncludeString,0);
		LOG(INFO,"::Konclude::Main",QString("%1, %2, Version %3 (%4)\r\n").arg(CKoncludeInfo::getKoncludeDescription()).arg(CKoncludeInfo::getKoncludeBitPlatformString()).arg(CKoncludeInfo::getKoncludeVersionString()).arg(CKoncludeInfo::getKoncludeCompilationDateString()),0);

		CLoaderFactory *loaderFactory = new CDefaultLoaderFactory();

		QStringList arguments = CCommandLineLoader::getEncodedArguments(argc,argv);

		CCommandLinePreparationTranslatorSelector transSelector(loaderFactory);
		arguments = transSelector.translateArguments(arguments);

		CCommandLineLoader *cmdLineLoader = new CCommandLineLoader(arguments,false);
		cmdLineLoader->init(loaderFactory);

		cmdLineLoader->load();
		a.exec();
		cmdLineLoader->exit();
		delete cmdLineLoader;
		delete loaderFactory;



		LOG(INFO,"::Konclude::Main",logTr("Stopping Konclude ... ."),0);

	} catch (...) {
		LOG(CATASTROPHIC,"::Konclude::Main",logTr("Stop because of an unknown catastrophic error."),0);
	}

	logger->shutdownLogger();

	return 0;
}
