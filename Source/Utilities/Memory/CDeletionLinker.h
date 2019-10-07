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

#ifndef KONCLUDE_UTILITIES_MEMORY_CDELETIONLINKER_H
#define KONCLUDE_UTILITIES_MEMORY_CDELETIONLINKER_H

// Library includes


// Namespace includes


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		using namespace Container;

		namespace Memory {


			/*! 
			 *
			 *		\class		CDeletionLinker
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDeletionLinker : public CLinkerBase<void*,CDeletionLinker> {
				// public methods
				public:
					//! Constructor
					CDeletionLinker(void* object);

					CDeletionLinker* initDeletionLinker(void* object);

					virtual CDeletionLinker* deleteObject() = 0;

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:
			};



			template<class T>
			class CDeletionLinkerBase : public CDeletionLinker {

			public:
				CDeletionLinkerBase(T* object) : CDeletionLinker(object) {
				}

				CDeletionLinker* deleteObject() {
					T* object = (T*)getData();
					delete object;
				}

			};



		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CDELETIONLINKER_H
