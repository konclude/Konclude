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

#ifndef KONCLUDE_CONFIG_CCONFIGURATIONGROUP_H
#define KONCLUDE_CONFIG_CCONFIGURATIONGROUP_H

// Libraries includes
#include <QVector>
#include <QReadWriteLock>


// Namespace includes
#include "CConfigDescription.h"
#include "CConfigData.h"
#include "CConfigType.h"

// Other includes
#include "Utilities/CDynRefVector.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Config {

		/*! 
		 *
		 *		\class		CConfigurationGroup
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConfigurationGroup {
			// public methods
			public:
				//! Constructor
				CConfigurationGroup();

				//! Destructor
				virtual ~CConfigurationGroup();

				virtual qint64 addConfigProperty(CConfigDescription *takeConfigDescription, CConfigData *takeDefaultConfigValue);
				virtual qint64 addConfigProperty(CConfigDescription *takeConfigDescription, CConfigType *takeDefaultConfigType);
				virtual qint64 addConfigProperty(CConfigDescription *takeConfigDescription);

				virtual CConfigDescription *getConfigDescription(qint64 index);
				virtual CConfigDescription *getConfigDescription(const QString &name);

				virtual CConfigData *getConfigDefaultData(qint64 index);
				virtual CConfigData *getConfigDefaultData(const QString &name);


				virtual CConfigurationGroup *setConfigDefaultData(qint64 index, CConfigData *takeDefaultConfigValue);


				virtual qint64 getConfigPropertyCount();

				virtual qint64 getConfigIndex(const QString &name);

				virtual CConfigurationGroup* getReferencedConfigurationGroup();

				virtual QList<CConfigDescription*> getConfigDescriptonList();

			// protected methods
			protected:				

			// protected variables
			protected:
				CDynRefVector<CConfigDescription> configDesVector;
				CDynRefVector<CConfigData> configDataVector;

				QLinkedList<CConfigDescription *> configDesContainer;
				QLinkedList<CConfigData *> configDataContainer;

				QHash<QString,qint64> propertyNameIndexHash;

				QReadWriteLock readWriteSync;
			
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CCONFIGURATIONGROUP_H
