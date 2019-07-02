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

#ifndef KONCLUDE_UTILITIES_CMEMORYMANAGER_H
#define KONCLUDE_UTILITIES_CMEMORYMANAGER_H

// Libraries includes
#include <QLinkedList>


// Namespace includes
#include "CAllocationObject.h"
#include "UtilitiesSettings.h"
#include "CMemoryReleaser.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Utilities {


		/*! 
		 *
		 *		\class		CMemoryManager
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CMemoryManager : public CMemoryReleaser, public CAllocationObject {
			// public methods
			public:
				//! Constructor
				CMemoryManager();

				//! Destructor
				virtual ~CMemoryManager();


				virtual T *allocate() = 0;
				virtual void release(T *&mem) = 0;
				virtual void release(CAllocationObject *object) = 0;

			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:

		};


		template<class T>
		CMemoryManager<T>::CMemoryManager() {
		}

		template<class T>
		CMemoryManager<T>::~CMemoryManager() {
		}



	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CMEMORYMANAGER_H
