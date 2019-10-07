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

#ifndef KONCLUDE_Control_COMMAND_CDEFAULTCOMMANDERINITIALIZATIONFACTORY_H
#define KONCLUDE_Control_COMMAND_CDEFAULTCOMMANDERINITIALIZATIONFACTORY_H

// Libraries includes


// Namespace includes
#include "CCommand.h"
#include "CCommanderInitializationFactory.h"
#include "CConfigManagerReader.h"

// Other includes
#include "Reasoner/Kernel/Manager/CAnalyseReasonerManager.h"
#include "Reasoner/Kernel/Manager/CExperimentalReasonerManager.h"
#include "Reasoner/Kernel/Manager/CReasonerManagerThread.h"

#include "Reasoner/Classifier/CClassificationManager.h"
#include "Reasoner/Classifier/CConfigDependedSubsumptionClassifierFactory.h"

#include "Reasoner/Revision/CSPOntologyRevisionManager.h"

#include "Reasoner/Revision/Persistence/CConfigurationBasedOntologyRevisionPersistencerFactory.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Reasoner::Kernel;
	using namespace Reasoner::Classifier;
	using namespace Reasoner::Revision;
	using namespace Reasoner::Revision::Persistence;
	using namespace Reasoner::Kernel::Manager;

	namespace Control {

		namespace Command {

			/*! 
			 *
			 *		\class		CDefaultCommanderInitializationFactory
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDefaultCommanderInitializationFactory : public CCommanderInitializationFactory {
				// public methods
				public:
					//! Constructor
					CDefaultCommanderInitializationFactory(CReasonerManager *useReasonerManager = 0, CClassificationManager *useClassificationManager = 0, COntologyRevisionManager *useOntologyRevisionManager = 0);

					//! Destructor
					virtual ~CDefaultCommanderInitializationFactory();


					virtual CReasonerManager *createReasonerManager(CConfigurationProvider *configProvider);
					virtual CClassificationManager *createClassificationManager(CConfigurationProvider *configProvider);
					virtual COntologyRevisionManager *createOntologyRevisionManager(CConfigurationProvider *configProvider);
				
					virtual CCommanderInitializationFactory *initializeReasonerManager(CReasonerManager *reasonerManager, CConfigurationProvider *configProvider);
					virtual CCommanderInitializationFactory *initializeClassificationManager(CClassificationManager *classificationManager, CConfigurationProvider *configProvider);
					virtual CCommanderInitializationFactory *initializeOntologyRevisionManager(COntologyRevisionManager *ontologyRevisionManager, CConfigurationProvider *configProvider);

				// protected methods
				protected:

				// protected variables
				protected:
					CReasonerManager *useReasonerMan;
					CClassificationManager *useClassificationMan;
					COntologyRevisionManager *useOntRevMan;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CDEFAULTCOMMANDERINITIALIZATIONFACTORY_H
