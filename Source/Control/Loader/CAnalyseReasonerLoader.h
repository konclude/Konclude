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

#ifndef KONCLUDE_Control_LOADER_CANALYSEREASONERLOADER_H
#define KONCLUDE_Control_LOADER_CANALYSEREASONERLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>

// Namespace includes
#include "CLoader.h"
#include "CDefaultReasonerLoader.h"

// Other includes
#include "Utilities/CSingletonProvider.hpp"

#include "Control/Command/CCommanderManagerThread.h"
#include "Control/Command/CCommanderManagerConfigType.h"

#include "Control/Command/Instructions/CInitializeConfigurationCommand.h"
#include "Control/Command/Instructions/CInitializeReasonerCommand.h"


#include "Reasoner/Kernel/Manager/CAnalyseReasonerManager.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CConsolePrintLogObserver.h"


namespace Konclude {

	using namespace Logger;
	using namespace Utilities;

	using namespace Reasoner::Kernel;
	using namespace Reasoner::Kernel::Algorithm;
	using namespace Reasoner::Kernel::Manager;

	namespace Control {

		using namespace Command;
		using namespace Instructions;

		namespace Loader {

			/*! 
			 *
			 *		\class		CAnalyseReasonerLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnalyseReasonerLoader : public CDefaultReasonerLoader {
				// public methods
				public:
					//! Constructor
					CAnalyseReasonerLoader();

					//! Destructor
					virtual ~CAnalyseReasonerLoader();


					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);


				// protected methods
				protected:

				// protected variables
				protected:

					CAnalyseReasonerManager *reasoner;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_LOADER_CANALYSEREASONERLOADER_H
