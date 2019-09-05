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

#ifndef KONCLUDE_CONTEXT_CMEMORYPOOLREALLOCATIONINCREASINGCONTEXT_H
#define KONCLUDE_CONTEXT_CMEMORYPOOLREALLOCATIONINCREASINGCONTEXT_H

// Library includes


// Namespace includes
#include "ContextSettings.h"
#include "CContext.h"

// Other includes
#include "Utilities/Memory/CMemoryAllocationManager.h"
#include "Utilities/Memory/CReallocationIncreasingMemoryPoolProvider.h"
#include "Utilities/Memory/CMemoryPoolContainerAllocationManager.h"
#include "Utilities/Memory/CMemoryPoolContainer.h"

#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;

	namespace Context {


		/*! 
		 *
		 *		\class		CMemoryPoolReallocationIncreasingContext
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CMemoryPoolReallocationIncreasingContext : public CContext, public CMemoryPoolContainer {
			// public methods
			public:
				//! Constructor
				CMemoryPoolReallocationIncreasingContext(CMemoryAllocationManager* reallocationMemMan, cint64 defaultPoolSize = DEFAULTMEMORYPOOLSIZE, cint64 maxPoolSize = -1, double increasingFactor = 2.);

				//! Destructor
				virtual ~CMemoryPoolReallocationIncreasingContext();

				virtual CMemoryAllocationManager* getMemoryAllocationManager();


			// protected methods
			protected:

			// protected variables
			protected:
				CReallocationIncreasingMemoryPoolProvider mDefNewMemPoolProvider;
				CMemoryPoolContainerAllocationManager mMemMan;

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Context

}; // end namespace Konclude

#endif // KONCLUDE_CONTEXT_CMEMORYPOOLREALLOCATIONINCREASINGCONTEXT_H
