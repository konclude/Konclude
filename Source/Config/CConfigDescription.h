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

#ifndef KONCLUDE_CONFIG_CCONFIGDESCRIPTION_H
#define KONCLUDE_CONFIG_CCONFIGDESCRIPTION_H

// Libraries includes
#include <QString>
#include <QVariant>

// Namespace includes
#include "CConfigType.h"
#include "CConvertStringConfigType.h"
#include "CConvertIntegerConfigType.h"
#include "CConvertBooleanConfigType.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CConfigDescription
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConfigDescription {
			// public methods
			public:
				//! Constructor
				CConfigDescription(const QString &name, const QString &description, CConfigType *takeConfigType);

				//! Destructor
				virtual ~CConfigDescription();

				virtual CConfigType *getConfigType();
				virtual CConfigDescription *setConfigType(CConfigType *takeConfigType);


				virtual CConfigDescription *setConfigName(const QString &name);
				virtual CConfigDescription *setConfigDescription(const QString &description);
				virtual CConfigDescription *setConfigID(qint64 id);

				virtual QString getConfigName();
				virtual QString getConfigDescription();
				virtual qint64 getConfigID();


				virtual bool supportsStringConvertion();
				virtual bool supportsIntegerConvertion();
				virtual bool supportsBooleanConvertion();



			// protected methods
			protected:

			// protected variables
			protected:	
				QString configProperty;
				QString propertyDescription;
				qint64 configPropertyID;
				CConfigType *type;

				bool stringConvertable;
				bool integerConvertable;
				bool booleanConvertable;
		
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CCONFIGDESCRIPTION_H
