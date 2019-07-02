/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CSET_H
#define KONCLUDE_UTILITIES_CONTAINER_CSET_H

// Library includes


// Namespace includes
#include "ContainerSettings.h"
#ifdef KONCLUDE_CONTAINER_SET_USE_QT_SET_FOR_DEFAULT
#include "CQtSet.h"
#endif

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CSet
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CSet 
#ifdef KONCLUDE_CONTAINER_SET_USE_QT_SET_FOR_DEFAULT
					: public CQtSet<T>
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

#include "CSet.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CSET_H
