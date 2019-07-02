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

#ifndef KONCLUDE_TEST_COWLFILEOWLLINKCLASSIFYREQUESTGENERATOR_H
#define KONCLUDE_TEST_COWLFILEOWLLINKCLASSIFYREQUESTGENERATOR_H

// Libraries includes
#include <QProcess>
#include <QDir>
#include <QDomDocument>
#include <iostream>

// Namespace includes


// Other includes
#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


using namespace std;


namespace Konclude {

	using namespace Config;
	using namespace Concurrent;

	namespace Test {


		/*! 
		 *
		 *		\class		COWLFileOWLlinkClassifyRequestGenerator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLFileOWLlinkClassifyRequestGenerator : public CLogIdentifier {

			// public methods
			public:
				//! Constructor
				COWLFileOWLlinkClassifyRequestGenerator(CConfiguration *config);

				//! Destructor
				virtual ~COWLFileOWLlinkClassifyRequestGenerator();

				bool generateOWLlinkClassifyRequest(const QString& requestFileName, const QString& responseFileName);

			// protected methods
			protected:
				QDomDocument createRequestDocument();

			// protected variables
			protected:
				CConfiguration* mConfig;

				QString mReasonerTestKBName;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_COWLFILEOWLLINKCLASSIFYREQUESTGENERATOR_H
