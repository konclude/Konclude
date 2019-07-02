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

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_CPREPROCESSINGMANAGER_H
#define KONCLUDE_REASONER_KERNEL_MANAGER_CPREPROCESSINGMANAGER_H

// Libraries includes
#include <QReadWriteLock>
#include <QHash>

// Namespace includes
#include "ManagerSettings.h"

// Other includes
#include "Reasoner/Preprocess/CPreprocessor.h"
#include "Reasoner/Preprocess/CRequirementConfigPreprocessingThread.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Config/CConfigurationProvider.h"

#include "Control/Command/CConfigManagerReader.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;
	using namespace Control::Command;

	namespace Reasoner {

		using namespace Preprocess;
		using namespace Ontology;

		namespace Kernel {

			namespace Manager {


				/*! 
				 *
				 *		\class		CPreprocessingManager
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CPreprocessingManager {
					// public methods
					public:
						//! Constructor
						CPreprocessingManager(CReasonerManager* reasonerManager);

						//! Destructor
						virtual ~CPreprocessingManager();

						virtual CPreprocessor* getPreprocessor(CConcreteOntology *ontology, CConfigurationBase *config);


					// protected methods
					protected:

					// protected variables
					protected:
						CReasonerManager* mReasonerManager;
						QReadWriteLock mReadWriteLock;
						QHash<CConcreteOntology*,CPreprocessor*> mOntoPreprocessHash;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_MANAGER_CPREPROCESSINGMANAGER_H
