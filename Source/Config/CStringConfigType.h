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

#ifndef KONCLUDE_CONFIG_CSTRINGCONFIGTYPE_H
#define KONCLUDE_CONFIG_CSTRINGCONFIGTYPE_H

// Libraries includes
#include <QString>
#include <QVariant>

// Namespace includes
#include "CConfigType.h"
#include "CConvertStringConfigType.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CStringConfigType
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CStringConfigType : virtual public CConvertStringConfigType, virtual public CConfigType {
			// public methods
			public:
				//! Constructor
				CStringConfigType(const QString &value = QString(""));

				//! Destructor
				virtual ~CStringConfigType();

				
				virtual CStringConfigType *getTypeCopy();

				virtual bool supportsVariant();

				virtual CStringConfigType *readFrom(const QVariant &var);
				virtual QVariant getVariant();

				virtual CStringConfigType *setValue(QString value);
				virtual QString getValue();



				virtual bool readFromString(const QString &string);
				virtual QString getString();

				virtual bool equals(CConfigType *dataType);

			// protected methods
			protected:

			// protected variables
			protected:
				QString stringValue;
				
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CSTRINGCONFIGTYPE_H
