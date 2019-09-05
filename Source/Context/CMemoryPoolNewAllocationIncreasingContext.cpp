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

#include "CMemoryPoolNewAllocationIncreasingContext.h"


namespace Konclude {

	namespace Context {

		CMemoryPoolNewAllocationIncreasingContext::CMemoryPoolNewAllocationIncreasingContext(cint64 defaultPoolSize, cint64 maxPoolSize, double increasingFactor) : CMemoryPoolContainer(), mDefNewMemPoolProvider(defaultPoolSize, maxPoolSize, increasingFactor), mMemMan(this, &mDefNewMemPoolProvider) {
		}

		CMemoryPoolNewAllocationIncreasingContext::~CMemoryPoolNewAllocationIncreasingContext() {
		}

		CMemoryAllocationManager* CMemoryPoolNewAllocationIncreasingContext::getMemoryAllocationManager() {
			return &mMemMan;
		}



	}; // end namespace Context

}; // end namespace Konclude

