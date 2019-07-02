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

#ifndef KONCLUDE_CONTEXT_CCONTEXTBASE_H
#define KONCLUDE_CONTEXT_CCONTEXTBASE_H

// Library includes


// Namespace includes
#include "ContextSettings.h"
#include "CContext.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Context {


		/*! 
		 *
		 *		\class		CContextBase
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CContextBase : public CContext {
			// public methods
			public:
				//! Constructor
				CContextBase(CMemoryAllocationManager* memAllocManager);

				//! Destructor
				virtual ~CContextBase();

				virtual CMemoryAllocationManager* getMemoryAllocationManager();

			// protected methods
			protected:

			// protected variables
			protected:
				CMemoryAllocationManager* mMemAllocManager;

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Context

}; // end namespace Konclude

#endif // KONCLUDE_CONTEXT_CCONTEXTBASE_H
