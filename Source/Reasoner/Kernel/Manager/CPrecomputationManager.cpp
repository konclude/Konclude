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

#include "CPrecomputationManager.h"
#include "CReasonerManager.h"

#include "Reasoner/Consistiser/CTotallyPrecomputationThread.h"
#include "Reasoner/Consistiser/CIncrementalPrecomputationThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {


				CPrecomputationManager::CPrecomputationManager(CReasonerManager* reasonerManager) {
					mReasonerManager = reasonerManager;
				}


				CPrecomputationManager::~CPrecomputationManager() {
				}

				CPrecomputator* CPrecomputationManager::getPrecomputator(CConcreteOntology *ontology, CConfigurationBase *config) {
					CPrecomputator* precomputator = nullptr;
					mReadWriteLock.lockForRead();
					precomputator = mOntoPrecomputatorHash.value(ontology);
					mReadWriteLock.unlock();

					bool onlyChangedABoxAxioms = ontology->getIncrementalRevisionData()->getAxiomChangeData()->hasOnlyChangedABoxAxioms();
					bool previousConsistentOntologyVersion = ontology->getIncrementalRevisionData()->getPreviousConsistentOntology() != nullptr;

					if (!precomputator) {
						mReadWriteLock.lockForWrite();
						precomputator = mOntoPrecomputatorHash.value(ontology);
						if (!precomputator) {
							if (onlyChangedABoxAxioms && previousConsistentOntologyVersion) {
								precomputator = new CIncrementalPrecomputationThread(mReasonerManager);
							} else {
								precomputator = new CTotallyPrecomputationThread(mReasonerManager);
							}
							mOntoPrecomputatorHash.insert(ontology,precomputator);
						}
						mReadWriteLock.unlock();
					}

					return precomputator;
				}

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
