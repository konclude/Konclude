/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_UTILITIES_CONTAINER_CHASH_H
#define KONCLUDE_UTILITIES_CONTAINER_CHASH_H

// Library includes


// Namespace includes
#include "ContainerSettings.h"
#ifdef KONCLUDE_CONTAINER_HASH_USE_QT_HASH_FOR_DEFAULT
#include "CQtHash.h"
#endif

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CHash
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class K, class V>
			class CHash 
#ifdef KONCLUDE_CONTAINER_HASH_USE_QT_HASH_FOR_DEFAULT
					: public CQtHash<K,V>
#endif
			{
				// public methods
				public:

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:
			};

		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CHash.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CHASH_H
