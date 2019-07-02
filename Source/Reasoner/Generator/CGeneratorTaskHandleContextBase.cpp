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

#include "CGeneratorTaskHandleContextBase.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			CGeneratorTaskHandleContextBase::CGeneratorTaskHandleContextBase() {
				mMemPoolProv = new CNewAllocationMemoryPoolProvider();
				mMemMan = new CTaskHandleMemoryPoolAllocationManager(mMemPoolProv);
			}


			CGeneratorTaskHandleContextBase::~CGeneratorTaskHandleContextBase() {
				delete mMemPoolProv;
				delete mMemMan;
			}

			CTaskHandleMemoryAllocationManager* CGeneratorTaskHandleContextBase::getTaskHandleMemoryAllocationManager() {
				return mMemMan;
			}

			CMemoryAllocationManager* CGeneratorTaskHandleContextBase::getMemoryAllocationManager() {
				return mMemMan;
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
