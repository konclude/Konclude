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

#include "CConfigurableCoutLogObserverLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CConfigurableCoutLogObserverLoader::CConfigurableCoutLogObserverLoader() {
				consoleSingletonLogObserver = 0;
			}



			CConfigurableCoutLogObserverLoader::~CConfigurableCoutLogObserverLoader() {
			}


			CLoader *CConfigurableCoutLogObserverLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				return this;
			}



			CLoader *CConfigurableCoutLogObserverLoader::load() {
				QStringList infoDomainList;
				infoDomainList<<QString("::Konclude::Main");
				infoDomainList<<QString("::Konclude::Reasoner::Kernel::ReasonerManager");
				infoDomainList<<QString("::Konclude::Reasoner::Kernel::ExperimentalReasonerManager");
				infoDomainList<<QString("::Konclude::Test");
				infoDomainList<<QString("::Konclude::Test::ReasonerTestfileTester");
				infoDomainList<<QString("::Konclude::Test::ReasonerTestsuiteTester");
				infoDomainList<<QString("::Konclude::Control::Interface::OWLlink::OWLlinkProcessor");
				infoDomainList<<QString("::Konclude::Reasoner::Classifier::Factory");
				infoDomainList<<QString("::Konclude::Test::ReasonerTestfileErrorEvaluator");
				infoDomainList<<QString("::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor");
				infoDomainList<<QString("::Konclude::Network::HTTP::HttpListner");
				infoDomainList<<QString("::Konclude::Loader::OWLlinkTranslationHelpLoader");
				infoDomainList<<QString("::Konclude::Loader::CommandLineTranslator");
				infoDomainList<<QString("::Konclude::CLIBatchProcessor");

				consoleSingletonLogObserver = new CConsolePrintConfigurableLogObserver(infoDomainList,60.,true,false,true,true);
				CLogger::getInstance()->addLogObserver(consoleSingletonLogObserver);				

				return this;
			}

			CLoader *CConfigurableCoutLogObserverLoader::exit() {
				CLogger::getInstance()->removeObserverFromAllDomains(consoleSingletonLogObserver);		
				delete consoleSingletonLogObserver;
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
