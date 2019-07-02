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

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_CPRECOMPUTATIONMANAGER_H
#define KONCLUDE_REASONER_KERNEL_MANAGER_CPRECOMPUTATIONMANAGER_H

// Libraries includes
#include <QReadWriteLock>
#include <QHash>

// Namespace includes
#include "ManagerSettings.h"

// Other includes
#include "Reasoner/Consistiser/CPrecomputator.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Config/CConfigurationProvider.h"

#include "Control/Command/CConfigManagerReader.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;
	using namespace Control::Command;

	namespace Reasoner {

		using namespace Consistiser;
		using namespace Ontology;

		namespace Kernel {

			namespace Manager {


				/*! 
				 *
				 *		\class		CPrecomputationManager
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CPrecomputationManager {
					// public methods
					public:
						//! Constructor
						CPrecomputationManager(CReasonerManager* reasonerManager);

						//! Destructor
						virtual ~CPrecomputationManager();

						virtual CPrecomputator* getPrecomputator(CConcreteOntology *ontology, CConfigurationBase *config);


					// protected methods
					protected:

					// protected variables
					protected:
						CReasonerManager* mReasonerManager;
						QReadWriteLock mReadWriteLock;
						QHash<CConcreteOntology*,CPrecomputator*> mOntoPrecomputatorHash;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_MANAGER_CPRECOMPUTATIONMANAGER_H
