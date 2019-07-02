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

#include "CExperimentalReasonerManager.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {


				CExperimentalReasonerManager::CExperimentalReasonerManager(CWatchDog *threadWatchDog) : CAnalyseReasonerManager(threadWatchDog) {
				}


				CExperimentalReasonerManager::~CExperimentalReasonerManager() {
				}


				void CExperimentalReasonerManager::threadStarted() {
					CIntervalThread::threadStarted();
					// generate worker framework
					LOG(WARNING,"::Konclude::Reasoner::Kernel::ExperimentalReasonerManager",logTr("Initializing EXPERIMENTAL Reasoner."),this);

					unsatCache = new COccurrenceUnsatisfiableCache(mWorkControllerCount+2);
					mSatExpCache = new CSignatureSatisfiableExpanderCache();
					mReuseCompGraphCache = new CReuseCompletionGraphCache();
					mSatNodeExpCache = new CSaturationNodeAssociatedExpansionCache(configProvider->getCurrentConfiguration());
					mCompConsCache = new CComputedConsequencesCache(configProvider->getCurrentConfiguration());

					CConfigDependedCalculationFactory* calcFactory = new CConfigDependedCalculationFactory(this);
					CCalculationManager* calculationManager = calcFactory->createCalculationManager(configProvider);
					calcFactory->initializeManager(calculationManager,configProvider);
					mCalculationManager = calculationManager;

					cint64 workExecuterPerControllerCount = 1;

					LOG(INFO,"::Konclude::Reasoner::Kernel::ExperimentalReasonerManager",logTr("Reasoner initialized with %1 processing unit(s).").arg(mWorkControllerCount),this);


					startTimerWithInterval(PROGRESSQUERYTIMER,10000);
				}

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
