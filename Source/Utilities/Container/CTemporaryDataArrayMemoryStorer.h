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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CTEMPORARYDATAARRAYMEMORYSTORER_H
#define KONCLUDE_UTILITIES_CONTAINER_CTEMPORARYDATAARRAYMEMORYSTORER_H

// Libraries includes


// Namespace includes
#include "ContainerSettings.h"
#include "CDataArrayMemoryManager.h"
#include "CModelDataArray.h"
#include "CModelDataLevelArray.h"


// Other includes
#include "Utilities/CDynamicExpandingMemoryManager.hpp"
#include "Utilities/CAllocationObject.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			/*! 
			 *
			 *		\class		CTemporaryDataArrayMemoryStorer
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTemporaryDataArrayMemoryStorer : public CDataArrayMemoryManager, public CAllocationObject {
				// public methods
				public:
					//! Constructor
					CTemporaryDataArrayMemoryStorer(CDataArrayMemoryManager *modelMemoryManager);

					//! Destructor
					virtual ~CTemporaryDataArrayMemoryStorer();

					virtual CModelDataArray *allocateModelDataArray();
					virtual CModelDataLevelArray *allocateModelDataLevelArray();

					virtual CTemporaryDataArrayMemoryStorer *releaseTemporaryMemory();


				// protected methods
				protected:

				// protected variables
				protected:
					CDataArrayMemoryManager *modelMemMan;

					QList<CAllocationObject *> tmpMemList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CTemporaryDataArrayMemoryStorer_H
