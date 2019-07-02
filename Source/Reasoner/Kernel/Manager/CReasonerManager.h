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

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_CREASONERMANAGER_H
#define KONCLUDE_REASONER_KERNEL_MANAGER_CREASONERMANAGER_H

// Libraries includes


// Namespace includes
#include "ManagerSettings.h"


// Other includes
#include "Reasoner/Kernel/Calculation/CCalculationManager.h"

#include "Reasoner/Query/CQuery.h"
#include "Reasoner/Query/CSatisfiableCalculationJob.h"
#include "Reasoner/Query/CJobSatisfiableResult.h"

#include "Reasoner/Kernel/Cache/CUnsatisfiableCache.h"
#include "Reasoner/Kernel/Cache/CSatisfiableCache.h"
#include "Reasoner/Kernel/Cache/CCompletionGraphCache.h"

#include "Config/CConfigurationProvider.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Query;

		namespace Kernel {

			using namespace Cache;
			using namespace Task;
			using namespace Calculation;

			namespace Manager {


				/*! 
				 *
				 *		\class		CReasonerManager
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReasonerManager {
					// public methods
					public:
						//! Constructor
						CReasonerManager();

						//! Destructor
						virtual ~CReasonerManager();

						virtual CReasonerManager* reasoningSatisfiableCalcualtionJob(CSatisfiableCalculationJob* satCalcJob, CCallbackData* callback) = 0;
						virtual CJobSatisfiableResult* reasoningSatisfiableCalcualtionJob(CSatisfiableCalculationJob* satCalcJob) = 0;

						virtual CReasonerManager* reasoningQuery(CQuery *query, CCallbackData *callback) = 0;
						virtual CReasonerManager* reasoningQuery(CQuery *query) = 0;
						
						virtual CReasonerManager* prepareOntology(CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& reqList, CCallbackData *callback) = 0;
						virtual CReasonerManager* prepareOntology(CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& reqList) = 0;

						virtual CUnsatisfiableCache *getUnsatisfiableCache() = 0;
						virtual CCompletionGraphCache *getCompletionGraphCache() = 0;

						virtual CCalculationManager *getCalculationManager() = 0;
						virtual CPrecomputationManager* getPrecomputationManager() = 0;

						virtual CReasonerManager *initializeManager(CConfigurationProvider *configurationProvider) = 0;


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_MANAGER_CREASONERMANAGER_H
