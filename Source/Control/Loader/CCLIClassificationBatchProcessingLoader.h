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

#ifndef KONCLUDE_CONTROL_LOADER_CCLICLASSIFICATIONBATCHPROCESSINGLOADER_H
#define KONCLUDE_CONTROL_LOADER_CCLICLASSIFICATIONBATCHPROCESSINGLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>
#include <QTime>
#include <QDir>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CLoaderFactory.h"
#include "CCLIBatchProcessingLoader.h"

// Other includes
#include "Control/Interface/OWLlink/COWLlinkProcessor.h"
#include "Control/Interface/OWLlink/COWLLinkRecordInterpreter.h"

#include "Control/Command/CConfigManagerReader.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;

	namespace Control {

		using namespace Interface::OWLlink;
		using namespace Command;

		namespace Loader {

			/*! 
			 *
			 *		\class		CCLIClassificationBatchProcessingLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCLIClassificationBatchProcessingLoader : public CCLIBatchProcessingLoader {
				// public methods
				public:
					//! Constructor
					CCLIClassificationBatchProcessingLoader();

					//! Destructor
					virtual ~CCLIClassificationBatchProcessingLoader();


				// protected methods
				protected:

					void createClassificationTestingCommands();
					virtual void createTestingCommands();

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

#endif // KONCLUDE_CONTROL_LOADER_CCLICLASSIFICATIONBATCHPROCESSINGLOADER_H
