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

#ifndef KONCLUDE_CONFIG_CSTRINGLISTCONFIGTYPE_H
#define KONCLUDE_CONFIG_CSTRINGLISTCONFIGTYPE_H

// Libraries includes
#include <QString>
#include <QVariant>

// Namespace includes
#include "CConfigType.h"
#include "CConvertStringConfigType.h"
#include "CConvertStringListConfigType.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CStringListConfigType
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CStringListConfigType : virtual public CConvertStringConfigType, virtual public CConvertStringListConfigType, virtual public CConfigType {
			// public methods
			public:
				//! Constructor
				CStringListConfigType(const QString& value = QString(""), const QString& separator = QString("\t"));
				CStringListConfigType(const QStringList& value, const QString& separator = QString("\t"));

				//! Destructor
				virtual ~CStringListConfigType();

				
				virtual CStringListConfigType *getTypeCopy();

				virtual bool supportsVariant();

				virtual CStringListConfigType *readFrom(const QVariant &var);
				virtual QVariant getVariant();

				virtual CStringListConfigType *setValue(QString value);
				virtual QStringList getValue();



				virtual bool supportsStringConvertion();
				virtual bool readFromString(const QString &string);
				virtual QString getString();



				virtual bool supportsStringListConvertion();

				virtual bool readFromStringList(const QStringList &stringList);
				virtual QStringList getStringList();



				virtual bool equals(CConfigType *dataType);

			// protected methods
			protected:

			// protected variables
			protected:
				QStringList mStringListValue;
				QString mSeparator;
				
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CSTRINGLISTCONFIGTYPE_H
