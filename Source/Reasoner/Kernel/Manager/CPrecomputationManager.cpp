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

#include "CPrecomputationManager.h"
#include "CReasonerManager.h"

#include "Reasoner/Consistiser/CTotallyPrecomputationThread.h"


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

					if (!precomputator) {
						mReadWriteLock.lockForWrite();
						precomputator = mOntoPrecomputatorHash.value(ontology);
						if (!precomputator) {
							precomputator = new CTotallyPrecomputationThread(mReasonerManager);
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
