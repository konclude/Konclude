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

#include "CRealizationManager.h"
#include "CReasonerManager.h"

#include "Reasoner/Realizer/COptimizedKPSetOntologyConceptRealizingThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {


				CRealizationManager::CRealizationManager(CReasonerManager* reasonerManager) {
					mReasonerManager = reasonerManager;
				}


				CRealizationManager::~CRealizationManager() {
				}

				CRealizer* CRealizationManager::getRealizer(CConcreteOntology *ontology, CConfigurationBase *config) {
					CRealizer* realizer = nullptr;
					mReadWriteLock.lockForRead();
					realizer = mOntoRealizerHash.value(ontology);
					mReadWriteLock.unlock();

					if (!realizer) {
						mReadWriteLock.lockForWrite();
						realizer = mOntoRealizerHash.value(ontology);
						if (!realizer) {
							realizer = new COptimizedKPSetOntologyConceptRealizingThread(mReasonerManager);
							mOntoRealizerHash.insert(ontology,realizer);
							mRealizerSet.insert(realizer);
						}
						mReadWriteLock.unlock();
					}

					return realizer;
				}


				CRealizationProgress* CRealizationManager::getRealizationProgress() {
					CRealizationProgress newRealProg;
					mReadWriteLock.lockForRead();
					double percentAvg = 0;
					cint64 percentCount = 0;
					foreach (CRealizer* realizer, mRealizerSet) {
						CRealizationProgress* realizationProgress = realizer->getRealizationProgress();
						if (realizationProgress) {
							newRealProg.setTotalClasses(newRealProg.getTotalClasses()+realizationProgress->getTotalClasses());
							newRealProg.setRealizationCount(newRealProg.getRealizationCount()+realizationProgress->getRealizationCount());
							newRealProg.setTestedClasses(newRealProg.getTestedClasses()+realizationProgress->getTestedClasses());
							newRealProg.setRemainingMilliSeconds(qMax(newRealProg.getRemainingMilliSeconds(),realizationProgress->getRemainingMilliSeconds()));
							percentAvg += realizationProgress->getProgessPercent();
							++percentCount;
						}
					}
					mReadWriteLock.unlock();
					if (percentCount != 0) {
						percentAvg /= (double)percentCount;
					}
					newRealProg.setProgessPercent(percentAvg);
					mRealizationProgress = newRealProg;
					return &mRealizationProgress;
				}


			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
