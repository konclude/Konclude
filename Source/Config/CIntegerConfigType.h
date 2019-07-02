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

#ifndef KONCLUDE_CONFIG_CINTEGERCONFIGTYPE_H
#define KONCLUDE_CONFIG_CINTEGERCONFIGTYPE_H

// Libraries includes
#include <QString>
#include <QVariant>

// Namespace includes
#include "CConfigType.h"
#include "CConvertStringConfigType.h"
#include "CConvertIntegerConfigType.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CIntegerConfigType
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CIntegerConfigType : virtual public CConvertIntegerConfigType, virtual public CConvertStringConfigType, virtual public CConfigType {
			// public methods
			public:
				//! Constructor
				CIntegerConfigType(qint64 value = 0);

				//! Destructor
				virtual ~CIntegerConfigType();

				
				virtual CIntegerConfigType *getTypeCopy();

				virtual bool supportsVariant();

				virtual CIntegerConfigType *setValue(qint64 value);
				virtual qint64 getValue();

				virtual CIntegerConfigType *readFrom(const QVariant &var);
				virtual QVariant getVariant();


				virtual bool readFromString(const QString &string);
				virtual QString getString();

				virtual bool readFromInteger(qint64 integer);
				virtual qint64 getInteger();



				virtual bool equals(CConfigType *dataType);

			// protected methods
			protected:

			// protected variables
			protected:
				qint64 intValue;
				
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CINTEGERCONFIGTYPE_H
