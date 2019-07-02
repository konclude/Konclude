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

#ifndef KONCLUDE_Control_LOADER_CLOADERFACTORY_H
#define KONCLUDE_Control_LOADER_CLOADERFACTORY_H

// Libraries includes
#include <QString>
#include <QLinkedList>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"


// Other includes
#include "Config/CConfiguration.h"

#include "Control/Command/CReasonerConfigurationGroup.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Config;

	namespace Control {

		using namespace Command;

		namespace Loader {

			/*! 
			 *
			 *		\class		CLoaderFactory
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CLoaderFactory {
				// public methods
				public:
					//! Constructor
					CLoaderFactory();

					//! Destructor
					virtual ~CLoaderFactory();

					virtual CLoader *createLoaderFromName(const QString &loaderName, CConfiguration *config = 0) = 0;
					virtual bool canCreateLoaderFromName(const QString &loaderName, CConfiguration *config = 0) = 0;


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_LOADER_CLOADERFACTORY_H
