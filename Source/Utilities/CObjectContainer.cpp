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

#include "CObjectContainer.h"


namespace Konclude {

	namespace Utilities {



		CObjectContainer::CObjectContainer() {
			linker = 0;
			sLinker = 0;
			allocLinkerMemMan = 0;
		}


		CObjectContainer::~CObjectContainer() {
			clear();
		}

		CObjectContainer *CObjectContainer::addObject(CAllocationLinker *linkerObject) {
			sLinker = (CAllocationLinker *)linkerObject->insertNext(sLinker);
			return this;
		}


		void CObjectContainer::clear() {
			while (sLinker) {
				CAllocationObject::release(sLinker->getData());
				CAllocationLinker *tmpLinker = sLinker;
				sLinker = (CAllocationLinker *)sLinker->getNext();
				CAllocationObject::release(tmpLinker);
			}
			while (linker) {
				CAllocationObject::release(linker->getData());
				linker = (CAllocationLinker *)linker->getNext();
			}
			delete allocLinkerMemMan;
			allocLinkerMemMan = 0;			
		}


		CObjectContainer *CObjectContainer::addObject(CAllocationObject *allocationObject) {
			if (!allocLinkerMemMan) {
				allocLinkerMemMan = new CDynamicExpandingMemoryManager<CAllocationLinker>();
			}
			CAllocationLinker *linkerObject = allocLinkerMemMan->allocate();
			linker = (CAllocationLinker *)linkerObject->init(allocationObject,linker);
			return this;
		}





	}; // end namespace Utilities

}; // end namespace Konclude
