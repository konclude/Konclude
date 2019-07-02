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

#include "CConcreteOntologyContextBase.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CConcreteOntologyContextBase::CConcreteOntologyContextBase() {
				mPoolProvider = new CNewAllocationMemoryPoolProvider();
				mMemMan = new CMemoryPoolAllocationManager(mPoolProvider);

				mOntMemMan = nullptr;
				mObjCon = nullptr;
			}


			CConcreteOntologyContextBase::~CConcreteOntologyContextBase() {
				delete mMemMan;
				delete mPoolProvider;

				delete mOntMemMan;
				delete mObjCon;
			}


			CMemoryAllocationManager* CConcreteOntologyContextBase::getMemoryAllocationManager() {
				return mMemMan;
			}

			CMemoryPoolProvider* CConcreteOntologyContextBase::getMemoryPoolProvider() {
				return mPoolProvider;
			}

			CObjectContainer *CConcreteOntologyContextBase::getObjectContainer(bool create) {
				if (!mObjCon && create) {
					mObjCon = new CObjectContainer();
				}
				return mObjCon;
			}

			COntologyContextMemoryManager *CConcreteOntologyContextBase::getOnologyMemoryManager(COntologyContextMemoryManagerProvider *provider) {
				if (!mOntMemMan && provider) {
					mOntMemMan = provider->getOntologyMemoryManager();
				}
				return mOntMemMan;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
