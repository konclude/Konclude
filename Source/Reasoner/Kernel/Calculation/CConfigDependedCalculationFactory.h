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

#ifndef KONCLUDE_REASONER_KERNEL_CALCULATION_CCONFIGDEPENDEDCALCULATIONFACTORY_H
#define KONCLUDE_REASONER_KERNEL_CALCULATION_CCONFIGDEPENDEDCALCULATIONFACTORY_H

// Libraries includes


// Namespace includes
#include "CCalculationFactory.h"
#include "CConcurrentTaskCalculationManager.h"
#include "CCalculationEnvironmentFactory.h"
#include "CConfigDependedCalculationEnvironmentFactory.h"
#include "CTaskHandleAlgorithmBuilder.h"

// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTaskJobCallbackExecuter.h"

#include "Config/CConfigurationProvider.h"
#include "Config/CConfigDataReader.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		namespace Kernel {

			namespace Calculation {


				/*! 
				 *
				 *		\class		CConfigDependedCalculationFactory
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConfigDependedCalculationFactory : public CCalculationFactory {
					// public methods
					public:
						//! Constructor
						CConfigDependedCalculationFactory(CTaskHandleAlgorithmBuilder* taskHandleAlgBuilder);

						//! Destructor
						virtual ~CConfigDependedCalculationFactory();

						virtual CCalculationManager* createCalculationManager(CConfigurationProvider* configurationProvider);

						virtual CCalculationManager* initializeManager(CCalculationManager* calculationManager, CConfigurationProvider* configurationProvider);

					// protected methods
					protected:

					// protected variables
					protected:
						CTaskHandleAlgorithmBuilder* mTaskHandleAlgBuilder;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Calculation

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CALCULATION_CCONFIGDEPENDEDCALCULATIONFACTORY_H
