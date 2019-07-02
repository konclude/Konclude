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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTSET_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTSET_H

// Namespace includes
#include "ContainerSettings.h"

#ifdef KONCLUDE_EMPLOY_QT_SET_CONTAINER

// Library includes
#include <QSet>

// Other includes
#include "Context/CContext.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Context;

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CQtSet
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CQtSet : public QSet<T> {
				// public methods
				public:
					inline CQtSet() : QSet<T>() {}
					inline CQtSet(CContext* context) : QSet<T>() {}
					inline CQtSet(const CQtSet<T>& other) : QSet<T>(other) {}
					inline CQtSet(const CQtSet<T>& other, CContext* context) : QSet<T>(other) {}
					inline CQtSet(const QSet<T>& other) : QSet<T>(other) {}
					inline CQtSet(const QSet<T>& other, CContext* context) : QSet<T>(other) {}


					inline CQtSet<T>* init(CContext* context = nullptr) {
						QSet<T>::clear();
						return this;
					}

					inline CQtSet<T>* init(const CQtSet<T>& other, CContext* context = nullptr) {
						this->operator =(other);
						return this;
					}

					inline CQtSet<T>* init(const CQtSet<T>* other, CContext* context = nullptr) {
						if (other) {
							this->operator =(*other);
						} else {
							QSet<T>::clear();
						}
						return this;
					}


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

#include "CQtSet.cpp"

#endif // KONCLUDE_EMPLOY_QT_SET_CONTAINER

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTSET_H
