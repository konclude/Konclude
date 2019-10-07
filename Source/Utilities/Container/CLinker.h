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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CLINKER_H
#define KONCLUDE_UTILITIES_CONTAINER_CLINKER_H

// Namespace includes
#include "ContainerSettings.h"

// Library includes

// Other includes

// Logger includes


namespace Konclude {

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CLinkerBase
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class S>
			class CLinkerBase {
				// public methods
				public:
					//! Constructor
					inline CLinkerBase();
					inline CLinkerBase(const T& dataElement, CLinkerBase<T,S>* nextLink = nullptr);

					//! Destructor
					inline ~CLinkerBase();

					inline S* initLinker(const T& dataElement, CLinkerBase<T,S>* nextLink = nullptr);

					inline bool hasNext() const;


					inline S* insertNext(CLinkerBase<T,S>* nextLink);
					inline S* getNext() const;

					inline S* clearNext();

					inline S* setNext(CLinkerBase<T,S>* nextLink);

					inline T& getData();
					inline S* setData(const T& dataElement);


					inline S* append(CLinkerBase<T,S>* appendingList);

					inline S* getRemovedNext();

					inline S* getLastListLink() const;

					inline cint64 getCount() const;
					inline cint64 size() const;

					inline bool contains(const T& dataElement) const;

					inline S* removeOne(const T& dataElement);
					inline S* removeAll(const T& dataElement);


				// protected methods
				protected:

				// protected variables
				protected:
					CLinkerBase<T,S>* next;
					T data;

				// private methods
				private:

				// private variables
				private:
			};


			/*! 
			*
			*		\class		CLinker
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			template<class T>
			class CXLinker : public CLinkerBase<T,CXLinker<T> > {
				// public methods
				public:
					// Constructor
					inline CXLinker() : CLinkerBase<T,CXLinker<T> >() {};
					inline CXLinker(const T& dataElement, CLinkerBase<T,CXLinker<T> >* nextLink = nullptr) : CLinkerBase<T,CXLinker<T> >(dataElement,nextLink) {};

					//! Destructor
					inline ~CXLinker() {};
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CLinker.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CLINKER_H
