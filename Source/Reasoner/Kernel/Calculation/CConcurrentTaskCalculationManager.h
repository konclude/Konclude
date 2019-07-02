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

#ifndef KONCLUDE_REASONER_KERNEL_CALCULATION_CCONCURRENTTASKCALCULATIONMANAGER_H
#define KONCLUDE_REASONER_KERNEL_CALCULATION_CCONCURRENTTASKCALCULATIONMANAGER_H

// Libraries includes


// Namespace includes
#include "CCalculationManager.h"
#include "CConcurrentTaskCalculationEnvironment.h"


// Other includes
#include "Reasoner/Generator/CSatisfiableCalculationTaskFromCalculationJobGenerator.h"

#include "Reasoner/Query/CSatisfiableCalculationJob.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Query;
		using namespace Generator;

		namespace Kernel {

			namespace Calculation {


				/*! 
				 *
				 *		\class		CConcurrentTaskCalculationManager
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConcurrentTaskCalculationManager : public CCalculationManager {
					// public methods
					public:
						//! Constructor
						CConcurrentTaskCalculationManager(CWatchDog *watchDog = 0);

						virtual CCalculationManager *calculateJobs(const QList< QPair<CCalculationJob*,CCallbackData*> >& jobCallbackList);
						virtual CCalculationManager *calculateJob(CCalculationJob* job, CCallbackData* callbackData = nullptr);
						virtual CCalculationManager *calculateTask(CSatisfiableCalculationTask* task);

						virtual CCalculationManager *initializeManager(CCalculationEnvironmentFactory *contextFactory, CConfigurationProvider *configurationProvider);

						virtual CCalculationEnviroment *getCalculationContext();

						virtual QString getCalculationErrorString(cint64 errorCode);
						
						virtual QHash<QString,cint64>* getCalculationStatistics();
						virtual QHash<QString,cint64>* getUpdatedCalculationStatistics(QHash<QString,cint64>* stat);

						virtual double getCalculationApproximatedRemainingTasksCount();

					// protected methods
					protected:

					// protected variables
					protected:
						CCalculationEnviroment *calcContext;
						CConcurrentTaskCalculationEnvironment* mTaskCalcEn;
						CGeneratorTaskHandleContextBase* mGenTaskHandleContext;
						CMemoryTemporaryAllocationManager* mTemMemMan;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Calculation

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CALCULATION_CCONCURRENTTASKCALCULATIONMANAGER_H
