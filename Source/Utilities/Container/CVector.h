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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CVECTOR_H
#define KONCLUDE_UTILITIES_CONTAINER_CVECTOR_H

// Library includes


// Namespace includes
#include "ContainerSettings.h"
#ifdef KONCLUDE_CONTAINER_VECTOR_USE_QT_VECTOR_FOR_DEFAULT
#include "CQtVector.h"
#endif

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CVector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CVector 
#ifdef KONCLUDE_CONTAINER_VECTOR_USE_QT_VECTOR_FOR_DEFAULT
					: public CQtVector<T>
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

#include "CVector.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CVECTOR_H
