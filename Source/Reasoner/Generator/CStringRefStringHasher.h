/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_UTILITIES_HASHER_CSTRINGREFSTRINGHASHER_H
#define KONCLUDE_UTILITIES_HASHER_CSTRINGREFSTRINGHASHER_H

// Libraries includes
#include <QString>
#include <QStringRef>

// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			/*! 
			 *
			 *		\class		CStringRefStringHasher
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CStringRefStringHasher {
				// public methods
				public:
					//! Constructor
					CStringRefStringHasher();
					CStringRefStringHasher(const QString& string);
					CStringRefStringHasher(const QStringRef& stringRef);

					bool isReferenceOnly() const;
					bool isEmpty() const;

					const QString& getString() const;
					const QStringRef& getStringRef() const;

					QStringRef toStringRefernce() const;

					bool operator==(const CStringRefStringHasher& other) const;


				// protected methods
				protected:

				// protected variables
				protected:
					bool mReferenceOnly;
					QString mString;
					QStringRef mStringRef;


				// private methods
				private:

				// private variables
				private:

			};


			inline uint qHash(const CStringRefStringHasher& stringRefStringHasher) {
				if (stringRefStringHasher.isReferenceOnly()) {
					return qHash(stringRefStringHasher.getStringRef());
				} else {
					return qHash(stringRefStringHasher.getString());
				}
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // KONCLUDE_UTILITIES_HASHER_CSTRINGREFSTRINGHASHER_H
