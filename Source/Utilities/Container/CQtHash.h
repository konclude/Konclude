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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTHASH_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTHASH_H

// Namespace includes
#include "ContainerSettings.h"

#ifdef KONCLUDE_EMPLOY_QT_HASH_CONTAINER

// Library includes
#include <QHash>

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
			 *		\class		CQtHash
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class Key, class T>
			class CQtHash : public QHash<Key,T> {
				// public methods
				public:
					inline CQtHash() : QHash<Key, T>() {}
					inline CQtHash(CContext* context) : QHash<Key, T>() {}
					inline CQtHash(const CQtHash<Key, T> &other) : QHash<Key, T>(other) {}
					inline CQtHash(const CQtHash<Key, T> &other, CContext* context) : QHash<Key, T>(other) {}
					inline CQtHash(const QHash<Key, T> &other) : QHash<Key, T>(other) {}
					inline CQtHash(const QHash<Key, T> &other, CContext* context) : QHash<Key, T>(other) {}


					inline CQtHash<Key, T>* init(CContext* context = 0) {
						QHash<Key, T>::clear();
						return this;
					}

					inline CQtHash<Key, T>* init(const CQtHash<Key, T> &other, CContext* context = 0) {
						this->operator =(other);
						return this;
					}

					inline CQtHash<Key, T>* init(const CQtHash<Key, T> *other, CContext* context = 0) {
						if (other) {
							this->operator =(*other);
						} else {
							QHash<Key, T>::clear();
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

#include "CQtHash.cpp"

#endif // KONCLUDE_EMPLOY_QT_HASH_CONTAINER

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTHASH_H
