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

#ifndef KONCLUDE_REASONER_KERNEL_CALCULATION_CCALCULATIONMANAGER_H
#define KONCLUDE_REASONER_KERNEL_CALCULATION_CCALCULATIONMANAGER_H

// Libraries includes


// Namespace includes
#include "CCalculationEnviroment.h"
#include "CCalculationEnvironmentFactory.h"

// Other includes
#include "Reasoner/Query/CCalculationJob.h"

#include "Config/CConfigurationProvider.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Query;

		namespace Kernel {

			namespace Calculation {


				/*! 
				 *
				 *		\class		CCalculationManager
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCalculationManager {
					// public methods
					public:
						//! Constructor
						CCalculationManager();

						//! Destructor
						virtual ~CCalculationManager();

						virtual CCalculationManager *calculateJob(CCalculationJob* job, CCallbackData* callbackData = nullptr) = 0;
						virtual CCalculationManager *calculateJobs(const QList< QPair<CCalculationJob*,CCallbackData*> >& jobCallbackList);

						virtual CCalculationManager *initializeManager(CCalculationEnvironmentFactory *contextFactory, CConfigurationProvider *configurationProvider) = 0;

						virtual CCalculationEnviroment *getCalculationContext() = 0;

						virtual QString getCalculationErrorString(cint64 errorCode) = 0;
						virtual QHash<QString,cint64>* getCalculationStatistics() = 0;
						virtual QHash<QString,cint64>* getUpdatedCalculationStatistics(QHash<QString,cint64>* stat) = 0;

					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Calculation

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CALCULATION_CCALCULATIONMANAGER_H
