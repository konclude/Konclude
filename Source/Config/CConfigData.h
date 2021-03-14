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

#ifndef KONCLUDE_CONFIG_CCONFIGDATA_H
#define KONCLUDE_CONFIG_CCONFIGDATA_H

// Libraries includes
#include <QString>
#include <QVariant>

// Namespace includes
#include "ConfigSettings.h"
#include "CConfigDescription.h"
#include "CConfigType.h"
#include "CIntegerConfigType.h"
#include "CDoubleConfigType.h"
#include "CStringConfigType.h"
#include "CStringListConfigType.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CConfigData
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConfigData {
			// public methods
			public:
				//! Constructor
				CConfigData(CConfigDescription *configDesciption, CConfigType *takeConfigValue);
				CConfigData(CConfigDescription *configDesciption);

				//! Destructor
				virtual ~CConfigData();

				virtual bool equals(CConfigData *data);

				virtual bool supportsStringConvertion();
				virtual bool readFromString(const QString &string);
				virtual QString getString();

				virtual bool supportsStringListConvertion();
				virtual bool readFromStringList(const QStringList &string);
				virtual QStringList getStringList();

				virtual bool supportsIntegerConvertion();
				virtual bool readFromInteger(qint64 integer);
				virtual qint64 getInteger();

				virtual bool supportsBooleanConvertion();
				virtual bool readFromBoolean(bool boolean);
				virtual bool getBoolean();

				virtual bool supportsDoubleConvertion();
				virtual bool readFromDouble(double value);
				virtual double getDouble();


				virtual CConfigDescription *getConfigDescription();
				virtual CConfigType *getConfigType();

				virtual qint64 getLevelLock();
				virtual bool hasLock();
				virtual CConfigData *setLevelLock(qint64 levelLock = -1);


			// protected methods
			protected:

			// protected variables
			protected:		
				CConfigDescription *configDes;
				CConfigType *configVal;
				qint64 lockLevel;
				
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CCONFIGDATA_H
