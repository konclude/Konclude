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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTLIST_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTLIST_H

// Namespace includes
#include "ContainerSettings.h"

#ifdef KONCLUDE_EMPLOY_QT_LIST_CONTAINER

// Library includes
#include <QList>

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
			 *		\class		CQtList
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CQtList : public QList<T> {
				// public methods
				public:
					inline CQtList() : QList<T>() {}
					inline CQtList(CContext* context) : QList<T>() {}
					inline CQtList(const CQtList<T> &l) : QList<T>(l) {}
					inline CQtList(const CQtList<T> &l, CContext* context) : QList<T>(l) {}
					inline CQtList(const QList<T> &l) : QList<T>(l) {}
					inline CQtList(const QList<T> &l, CContext* context) : QList<T>(l) {}

					inline CQtList(const T &t) : QList<T>() { append(t); }
					inline CQtList(const T &t, CContext* context) : QList<T>() { append(t); }


					inline CQtList<T>* init(CContext* context = nullptr) {
						QList<T>::clear();
						return this;
					}

					inline CQtList<T>* init(const CQtList<T>& other, CContext* context = nullptr) {
						this->operator =(other);
						return this;
					}

					inline CQtList<T>* init(const CQtList<T>* other, CContext* context = nullptr) {
						if (other) {
							this->operator =(*other);
						} else {
							QList<T>::clear();
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

#include "CQtList.cpp"

#endif // KONCLUDE_EMPLOY_QT_LIST_CONTAINER

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTLIST_H
