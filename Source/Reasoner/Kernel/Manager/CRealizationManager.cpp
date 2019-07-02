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

#include "CRealizationManager.h"
#include "CReasonerManager.h"

#include "Reasoner/Realizer/COptimizedRepresentativeKPSetOntologyRealizingThread.h"
#include "Reasoner/Realizer/CIncrementalKPSetOntologyConceptRealizingThread.h"


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

						bool onlyChangedABoxAxioms = ontology->getIncrementalRevisionData()->getAxiomChangeData()->hasOnlyChangedABoxAxioms();
						bool previousRealizedOntologyVersion = ontology->getIncrementalRevisionData()->getPreviousClassTypesRealizedOntology() != nullptr;

						mReadWriteLock.lockForWrite();
						realizer = mOntoRealizerHash.value(ontology);
						if (!realizer) {

							if (previousRealizedOntologyVersion && onlyChangedABoxAxioms) {
								realizer = new CIncrementalKPSetOntologyConceptRealizingThread(mReasonerManager);
							} else {
								realizer = new COptimizedRepresentativeKPSetOntologyRealizingThread(mReasonerManager);
							}
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
