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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CNEGLINKER_H
#define KONCLUDE_UTILITIES_CONTAINER_CNEGLINKER_H

// Namespace includes
#include "ContainerSettings.h"
#include "CLinker.h"
#include "CNegator.h"

// Library includes

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CNegLinkerBase
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class S>
			class CNegLinkerBase : public CLinkerBase<T,S>, public CNegator {
				// public methods
				public:
					// Constructor
					inline CNegLinkerBase();
					inline CNegLinkerBase(const T& dataElement, bool negation = false, CNegLinkerBase<T,S>* nextLink = nullptr);

					//! Destructor
					inline ~CNegLinkerBase();

					inline S* initNegLinker(const T& dataElement, bool negation = false, CNegLinkerBase<T,S>* nextLink = nullptr);

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:
			};


			/*! 
			*
			*		\class		CNegLinker
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			template<class T>
			class CXNegLinker : public CNegLinkerBase<T,CXNegLinker<T> > {
				// public methods
				public:
					// Constructor
					inline CXNegLinker() : CNegLinkerBase<T,CXNegLinker<T> >() {}
					inline CXNegLinker(const T& dataElement, bool negation = false, CXNegLinker<T>* nextLink = nullptr) : CNegLinkerBase<T,CXNegLinker<T> >(dataElement,negation,nextLink) {}

					//! Destructor
					inline ~CXNegLinker() {}
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CNegLinker.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CNEGLINKER_H
