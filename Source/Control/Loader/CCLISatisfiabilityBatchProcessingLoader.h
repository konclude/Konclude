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

#ifndef KONCLUDE_CONTROL_LOADER_CCLISATISFIABILITYBATCHPROCESSINGLOADER_H
#define KONCLUDE_CONTROL_LOADER_CCLISATISFIABILITYBATCHPROCESSINGLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>
#include <QTime>
#include <QDir>

// Namespace includes
#include "LoaderSettings.h"
#include "CCLIBatchProcessingLoader.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;

	namespace Control {

		namespace Loader {

			/*! 
			 *
			 *		\class		CCLISatisfiabilityBatchProcessingLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCLISatisfiabilityBatchProcessingLoader : public CCLIBatchProcessingLoader {
				// public methods
				public:
					//! Constructor
					CCLISatisfiabilityBatchProcessingLoader();

					//! Destructor
					virtual ~CCLISatisfiabilityBatchProcessingLoader();


				// protected methods
				protected:

					void createSatisfiabilityTesingCommands();
					virtual void createTestingCommands();

					virtual void writeCommandOutput(const QString& outputFileName, CCommand* processedCommand);


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

#endif // KONCLUDE_CONTROL_LOADER_CCLISATISFIABILITYBATCHPROCESSINGLOADER_H
