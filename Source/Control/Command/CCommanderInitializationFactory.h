/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_Control_COMMAND_CCOMMANDERINITIALIZATIONFACTORY_H
#define KONCLUDE_Control_COMMAND_CCOMMANDERINITIALIZATIONFACTORY_H

// Libraries includes


// Namespace includes
#include "CCommand.h"

// Other includes
#include "Config/CConfiguration.h"

#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Reasoner/Classifier/CClassificationManager.h"

#include "Reasoner/Revision/COntologyRevisionManager.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Config;
	using namespace Reasoner::Kernel::Manager;
	using namespace Reasoner::Classifier;
	using namespace Reasoner::Revision;

	namespace Control {

		namespace Command {

			/*! 
			 *
			 *		\class		CCommanderInitializationFactory
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCommanderInitializationFactory {
				// public methods
				public:
					//! Constructor
					CCommanderInitializationFactory();

					//! Destructor
					virtual ~CCommanderInitializationFactory();


					virtual CReasonerManager *createReasonerManager(CConfigurationProvider *configProvider) = 0;
					virtual CClassificationManager *createClassificationManager(CConfigurationProvider *configProvider) = 0;
					virtual COntologyRevisionManager *createOntologyRevisionManager(CConfigurationProvider *configProvider) = 0;
				
					virtual CCommanderInitializationFactory *initializeReasonerManager(CReasonerManager *reasonerManager, CConfigurationProvider *configProvider) = 0;
					virtual CCommanderInitializationFactory *initializeClassificationManager(CClassificationManager *classificationManager, CConfigurationProvider *configProvider) = 0;
					virtual CCommanderInitializationFactory *initializeOntologyRevisionManager(COntologyRevisionManager *ontologyRevisionManager, CConfigurationProvider *configProvider) = 0;

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CCOMMANDERINITIALIZATIONFACTORY_H
